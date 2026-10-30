using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using AllegianceInterop;
using Newtonsoft.Json;
using Wopr;
using Wopr.Constants;
using Wopr.Entities;

namespace Wopr.Strategies
{
    public abstract class StrategyBase : MarshalByRefObject
    {
        public delegate void OnStrategryCompleteHandler(StrategyBase strategy);
        public event OnStrategryCompleteHandler OnStrategyComplete;

        public delegate void OnGameCompleteHandler(StrategyBase strategy);
        public event OnGameCompleteHandler OnGameComplete;


        private static Dictionary<StrategyID, Type> _strategyTypesByStrategyID = null;

        protected static Random _random = new Random();

        protected int _runningTasks = 0;

        
        
        protected MessageReceiver _messageReceiver;

        public AllegianceInterop.ClientConnection ClientConnection { get; set; }


        public StrategyID StrategyID { get; }
        public TimeSpan Timeout { get; }

        public DateTime StartTime { get; }

        public String PlayerName { get; set; }
        public bool IsGameController { get; set; }
        public bool IsCommander { get; set; }
        public string BotAuthenticationGuid { get; set; }
        public short SideIndex { get; set; }


        //public bool IsStrategyComplete { get; set; } = false;

        protected GameInfo GameInfo { get; set; } 

        protected CancellationTokenSource _cancellationTokenSource { get; set; }

        public StrategyBase(StrategyID strategyID, TimeSpan timeout)
        {
            StrategyID = strategyID;
            Timeout = timeout;

            StartTime = DateTime.Now;

            _cancellationTokenSource = new CancellationTokenSource();
        }

        ~StrategyBase()
        {
            //if (_client != null)
            //{
            //    _client.OnAppMessage -= _messageReceiver.OnAppMessage;
            //    _client = null;
            //    _messageReceiver = null;
            //}

            Console.WriteLine($"Strategy destroyed for {PlayerName}, {StrategyID.ToString()}");
        }

        public void Attach(AllegianceInterop.ClientConnection client, GameInfo gameInfo, string botAuthenticationGuid, string playerName, short sideIndex, bool isGameController, bool isCommander)
        {
            ClientConnection = client;
            PlayerName = playerName;
            SideIndex = sideIndex;
            IsGameController = isGameController;
            IsCommander = isCommander;
            BotAuthenticationGuid = botAuthenticationGuid;
            GameInfo = gameInfo;

            _messageReceiver = new MessageReceiver(playerName);
            client.OnAppMessage += _messageReceiver.OnAppMessage;

            _messageReceiver.FMD_S_JOIN_SIDE += _messageReceiver_FMD_S_JOIN_SIDE;
            _messageReceiver.FMD_S_SHIP_STATUS += _messageReceiver_FMD_S_SHIP_STATUS;
            //_messageReceiver.FMD_S_EXPORT += _messageReceiver_FMD_S_EXPORT;
            _messageReceiver.FMD_S_CLUSTERINFO += _messageReceiver_FMD_S_CLUSTERINFO;
            _messageReceiver.FMD_CS_PING += _messageReceiver_FMD_CS_PING; // We'll get one of these every 5 seconds, using this to drive "less frequent" updates.
            _messageReceiver.FMD_S_DOCKED += _messageReceiver_FMD_S_DOCKED;
            _messageReceiver.FMD_S_MISSION_STAGE += _messageReceiver_FMD_S_MISSION_STAGE;
            _messageReceiver.FMD_S_BALLOT += _messageReceiver_FMD_S_BALLOT;
            _messageReceiver.FMD_S_GAME_OVER += _messageReceiver_FMD_S_GAME_OVER;

            AttachMessages(_messageReceiver, botAuthenticationGuid, playerName, sideIndex, isGameController, isCommander);

            Log($"Starting strategy: {StrategyID.ToString()}");

            Start();
        }

        private void _messageReceiver_FMD_S_GAME_OVER(ClientConnection client, AllegianceInterop.FMD_S_GAME_OVER message)
        {
            // Don't receive any more messages from the client for this strategy. 
            ClientConnection.OnAppMessage -= _messageReceiver.OnAppMessage;
            
            OnGameComplete?.Invoke(this);
        }

        private void _messageReceiver_FMD_S_BALLOT(ClientConnection client, AllegianceInterop.FMD_S_BALLOT message)
        {
            if ((ObjectType) message.otInitiator == ObjectType.OT_ship)
            {
                var shipName = client.GetCore().GetShip(message.oidInitiator)?.GetName();

                if (shipName != null && (shipName.EndsWith("@Dev", StringComparison.InvariantCultureIgnoreCase) == true
                                        || shipName.EndsWith("@Alleg", StringComparison.InvariantCultureIgnoreCase) == true
                                        || shipName.EndsWith("@Bot", StringComparison.InvariantCultureIgnoreCase) == true
                                        )
                      )
                {
                    Log($"Received ballot: {message.BallotText} from {shipName}, voting yes. (Remove this after debugging)");

                    AllegianceInterop.FMD_C_VOTE vote = new AllegianceInterop.FMD_C_VOTE(message.ballotID, true);
                    ClientConnection.SendMessageServer(vote);
                }
            }
        }

        private void _messageReceiver_FMD_S_MISSION_STAGE(ClientConnection client, AllegianceInterop.FMD_S_MISSION_STAGE message)
        {
            if ((MissionStage)message.stage == MissionStage.STAGE_STARTED)
                UpdateGalaxyView();
        }

        private void _messageReceiver_FMD_S_DOCKED(ClientConnection client, AllegianceInterop.FMD_S_DOCKED message)
        {
            // When ever the server sends us a "DOCKED" message, we lose all of the info about ships we have seen, so we need to 
            // re-update our galaxy view so that we know what's where.
            UpdateGalaxyView();
        }

        private void UpdateGalaxyView()
        {
            // Only the commander needs to keep an updated galaxy, and only when it's docked.
            if (IsCommander == true && ClientConnection.GetShip()?.GetStation() != null)
            {
                var targetCluster = ClientConnection.GetCore().GetClusters().OrderBy(p => p.GetObjectID());

                foreach (var cluster in ClientConnection.GetCore().GetClusters().OrderBy(p => p.GetObjectID()))
                {
                    AllegianceInterop.FMD_C_VIEW_CLUSTER viewCluster = new AllegianceInterop.FMD_C_VIEW_CLUSTER(cluster.GetObjectID(), -1, -1);
                    ClientConnection.SendMessageServer(viewCluster);
                    //Thread.Sleep(250);
                }
            }
        }

        private void _messageReceiver_FMD_CS_PING(ClientConnection client, AllegianceInterop.FMD_CS_PING message)
        {
            // refresh the galaxy view every 5 seconds.
            UpdateGalaxyView();
        }

        private void _messageReceiver_FMD_S_CLUSTERINFO(ClientConnection client, AllegianceInterop.FMD_S_CLUSTERINFO message)
        {
            GameInfo.Clusters = message.Clusters;

            //StreamWriter sw = new StreamWriter($@"c:\1\Logs\{PlayerName}_gameinfo.json");

            //foreach (var cluster in GameInfo.Clusters)
            //{
            //    sw.Write(cluster.ClusterID + "; "  + cluster.IsHomeSector + ";");
            //    bool firstWarp = true;
            //    foreach (var warp in cluster.Warps)
            //    {
            //        if (firstWarp == true)
            //            firstWarp = false;
            //        else
            //            sw.Write(", ");

            //        sw.Write(" " + warp.FromCluster.ClusterID + "-" + warp.ToCluster.ClusterID);
            //    }

            //    sw.WriteLine("");
            //}

            //sw.Close();
        }

        //private void _messageReceiver_FMD_S_EXPORT(ClientConnection client, AllegianceInterop.FMD_S_EXPORT message)
        //{
        //    var exportObjectType = (ObjectType)message.objecttype;

        //    Log($"Export ObjectType {exportObjectType.ToString()} received.");
        //}


        private void _messageReceiver_FMD_S_SHIP_STATUS(ClientConnection client, AllegianceInterop.FMD_S_SHIP_STATUS message)
        {
            // Check to ensure that we don't have any money hanging around in the bank. 
            if (client.SideLeaderShipID(SideIndex) != client.GetShip()?.GetObjectID() && client.GetMoney() > 1000)
                SetAutoDonateAndDonateAllMoneyToTeamLeader(client.GetSide()?.GetShip(client.SideLeaderShipID(SideIndex)));

            // Update ship status for hull, station and cluster IDs. 
            var ship = client.GetCore().GetShip(message.shipID);

            var shipName = ship.GetName();

            // Update ship, clintlib doesn't do this for us, it expects us to use the sector view to 
            // get information about the game states.
            if (message.shipID != client.GetShip().GetObjectID())
            {
                //    if (message.status.GetHullID() > -1)
                //        ship.SetBaseHullType(client.GetCore().GetHullType(message.status.GetHullID()));
               

                //if (message.status.GetStationID() >= 0)
                //    ship.SetStation(client.GetCore().GetStation(message.status.GetStationID()));
                //else
                //    ship.SetStation(null);

                // For some reason, the sectorID from this message isn't set into the ship's cluster ID. 
                // I think the real allegiance client must track drone states outside of clintlib? It appears that this message only sets Last Seen Sector, but not the current cluster.
                // As a work-around, always set each ship's current cluster to the message's sector ID.
                if (message.status.GetStationID() < 0 && ship.GetHullType() != null)
                    ship.SetCluster(client.GetCore().GetCluster(message.status.GetSectorID()));
                else
                    ship.SetCluster(null);
            }

            // Log the ship's current telemetry
            StringBuilder sb = new StringBuilder();
            sb.AppendLine($"position: X: {client.GetShip()?.GetPosition()?.X()}, Y: {client.GetShip()?.GetPosition()?.Y()}, Z: {client.GetShip()?.GetPosition()?.Z()}");
            sb.AppendLine($"speed: {client.GetShip()?.GetVelocity()?.Length()}");
            sb.AppendLine($"current command: {((CommandID ?)client.GetShip()?.GetCommandID((sbyte)CommandType.c_cmdCurrent))?.ToString()}, command target: {client.GetShip()?.GetCommandTarget((sbyte)CommandType.c_cmdCurrent)?.GetName()}");

            File.WriteAllText($@"c:\1\Logs\{client.GetShip().GetName()}_telemetry.txt", sb.ToString());
        }

        private void SetAutoDonateAndDonateAllMoneyToTeamLeader(IshipIGCWrapper teamLeaderShip)
        {
            if (teamLeaderShip != null)
            {
                Log($"Setting auto-donate to {teamLeaderShip?.GetName()}, donating {ClientConnection.GetMoney()} to the new commander.");

                AllegianceInterop.FMD_C_AUTODONATE autoDonate = new AllegianceInterop.FMD_C_AUTODONATE(teamLeaderShip.GetObjectID(), ClientConnection.GetMoney());
                ClientConnection.SendMessageServer(autoDonate);
            }
        }

        // If a commander joins our side, then change autodonate to that player, and donate all your money!
        private void _messageReceiver_FMD_S_JOIN_SIDE(ClientConnection client, AllegianceInterop.FMD_S_JOIN_SIDE message)
        {
            if (message.shipID == client.GetShip().GetObjectID())
                return;

            if (client.GetSide()?.GetShip(message.shipID)?.GetName().EndsWith("com@BOT") == true)
            {
                SetAutoDonateAndDonateAllMoneyToTeamLeader(client.GetSide()?.GetShip(message.shipID));
            }
            else
            {
                Log($"New player joined side: {client.GetSide()?.GetShip(message.shipID)?.GetName()}");
            }
        }

        //public void Detach()
        //{
        //    DetachMessages(_messageReceiver);
        //}

        public void FinishStrategy()
        {
            // Don't receive any more messages from the client for this strategy. 
            ClientConnection.OnAppMessage -= _messageReceiver.OnAppMessage;

            OnStrategyComplete?.Invoke(this);
            //IsStrategyComplete = true;
        }

        public void Cancel()
        {
            _cancellationTokenSource.Cancel();

            for (int i = 0; i < 100 && _runningTasks > 0; i++)
                Thread.Sleep(100);

            if (_runningTasks > 0)
                Log($"Warning: {_runningTasks} running tasks didn't exit after 10 seconds after a cancel was requested.");
        }
        
        public void OnAppMessage(byte[] bytes)
        {
            _messageReceiver.OnAppMessage(this.ClientConnection, bytes);
        }

        public void Log(string message)
        {
            Console.WriteLine($"{StrategyID.ToString()} {PlayerName}: {message}");

            string filename = $"c:\\1\\Logs\\{PlayerName}_strategy.txt";

            Exception lastException = null;
            for (int i = 0; i < 30; i++)
            {
                try
                {
                    File.AppendAllText(filename, $"{DateTime.Now} {StrategyID.ToString()} {PlayerName}: {message}\n");
                    lastException = null;
                    break;
                }
                catch (Exception ex)
                {
                    lastException = ex;
                    Thread.Sleep(10);
                }
            }

            if (lastException != null)
                throw lastException;
        }

        public abstract void AttachMessages(MessageReceiver messageReceiver, string botAuthenticationGuid, string playerName, int sideIndex, bool isGameController, bool isCommander);
        //public abstract void DetachMessages(MessageReceiver messageReceiver);
        public abstract void Start();

        internal static StrategyBase GetInstance(StrategyID strategyID, ClientConnection clientConnection, GameInfo gameInfo, string botAuthenticationGuid, string playerName, short sideIndex, bool isGameController, bool isCommander)
        {
            StrategyBase returnValue = null;

            if (_strategyTypesByStrategyID == null)
            {
                var types = from type in Assembly.GetAssembly(typeof(StrategyBase)).GetTypes()
                            where typeof(StrategyBase).IsAssignableFrom(type) && type.IsAbstract == false 
                            select type;

                _strategyTypesByStrategyID = types
                   .ToDictionary(p => ((StrategyBase)Activator.CreateInstance(p)).StrategyID, r => r);
            }

            if(_strategyTypesByStrategyID.ContainsKey(strategyID) == false)
                throw new NotSupportedException(strategyID.ToString());

            returnValue = (StrategyBase)Activator.CreateInstance(_strategyTypesByStrategyID[strategyID]);

            returnValue.Attach(clientConnection, gameInfo, botAuthenticationGuid, playerName, sideIndex, isGameController, isCommander);

            return returnValue;
        }
    }
}
