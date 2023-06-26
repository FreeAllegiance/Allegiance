using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using AllegianceInterop;
using Wopr;
using Wopr.Constants;

namespace Wopr.Strategies
{
    public class ConnectToGame : StrategyBase
    {
        private const int MaxTeams = 2;
        //private const string TargetServer = "LocalTest";
        //private const string TargetServer = "AEast2";
        private const string GameName = "SuperBotMatch9000";
        private readonly string CoreName = Configuration.Core;
        private const int MaxPlayersPerGame = 200;

        private bool _isClientConnectedToServer = false;
        private bool _hasCreatedGame = false;
        private int _waitingForMissionCount = 0;
        private int _receivedMissionCount = 0;
        private int _shipID;
        private int _playerCookie;
        private bool _hasSetTeamAndCiv;
        private short _targetCivID;
        private bool[] _readyStatus = new[] { false, false, false, false, false, false };

        public ConnectToGame() : base(StrategyID.ConnectToGame, TimeSpan.FromMinutes(1))
        {
        }


        public override void AttachMessages(MessageReceiver messageReceiver, string botAuthenticationGuid, string playerName, int sideIndex, bool isGameController, bool isCommander)
        {
            messageReceiver.FMD_L_LOGON_ACK += this.MessageReceiver_FMD_L_LOGON_ACK;
            messageReceiver.FMD_L_SERVERS_LIST += MessageReceiver_FMD_L_SERVERS_LIST;
            messageReceiver.FMD_LS_LOBBYMISSIONINFO += MessageReceiver_FMD_LS_LOBBYMISSIONINFO;
            //messageReceiver.FMD_L_JOIN_MISSION += MessageReceiver_FMD_L_JOIN_MISSION;
            messageReceiver.FMD_S_LOGONACK += MessageReceiver_FMD_S_LOGONACK;
            messageReceiver.FMD_S_JOINED_MISSION += MessageReceiver_FMD_S_JOINED_MISSION;
            messageReceiver.FMD_S_JOIN_SIDE += MessageReceiver_FMD_S_JOIN_SIDE;
            messageReceiver.FMD_S_AUTODONATE += MessageReceiver_FMD_S_AUTODONATE;
            messageReceiver.FMD_CS_SET_MISSION_OWNER += MessageReceiver_FMD_CS_SET_MISSION_OWNER;
            messageReceiver.FMD_CS_QUIT_SIDE += MessageReceiver_FMD_CS_QUIT_SIDE;
            
            messageReceiver.FMD_CS_SET_TEAM_LEADER += MessageReceiver_FMD_CS_SET_TEAM_LEADER;
            messageReceiver.FMD_S_TEAM_READY += MessageReceiver_FMD_S_TEAM_READY;
            messageReceiver.FMD_CS_CHATMESSAGE += MessageReceiver_FMD_CS_CHATMESSAGE;
            messageReceiver.FMD_CS_FORCE_TEAM_READY += MessageReceiver_FMD_CS_FORCE_TEAM_READY;

            messageReceiver.FMD_S_MISSION_STAGE += MessageReceiver_FMD_S_MISSION_STAGE;

            messageReceiver.FMD_S_SHIP_STATUS += MessageReceiver_FMD_S_SHIP_STATUS;
        }

       

        //private void MessageReceiver_FMD_L_JOIN_MISSION(ClientConnection client, AllegianceInterop.FMD_L_JOIN_MISSION message)
        //{
        //    bool connected = false;
        //    while (connected == false && _cancellationTokenSource.IsCancellationRequested == false)
        //    {
        //        Log($"Connecting to szServer: {message.szServer}, message.dwPort: {message.dwPort}, PlayerName: {PlayerName}, message.dwCookie: {message.dwCookie}");
        //        // Once the client connects to the server, the FM_S_LOGONACK response will trigger the disconnect from the lobby.
        //        if (client.ConnectToServer(message.szServer, (int)message.dwPort, PlayerName, BotAuthenticationGuid, (int)message.dwCookie) == true)
        //        {
        //            Log("\tConnected!");
        //            connected = true;
        //            break;
        //        }

        //        Log("\tCouldn't connect, retrying.");

        //        Thread.Sleep(100);
        //    }
        //}

        // Handles a client rejoining a game already in progress.
        private void MessageReceiver_FMD_S_SHIP_STATUS(ClientConnection client, AllegianceInterop.FMD_S_SHIP_STATUS message)
        {
            if(client.GetCore().GetMissionStage() == MissionStage.STAGE_STARTED)
                FinishStrategy();
        }

        public override void Start()
        {
            
        }

        //public override void DetachMessages(MessageReceiver messageReceiver)
        //{
        //    messageReceiver.FMD_L_LOGON_ACK -= this.MessageReceiver_FMD_L_LOGON_ACK;
        //    messageReceiver.FMD_L_SERVERS_LIST -= MessageReceiver_FMD_L_SERVERS_LIST;
        //    messageReceiver.FMD_LS_LOBBYMISSIONINFO -= MessageReceiver_FMD_LS_LOBBYMISSIONINFO;
        //    messageReceiver.FMD_S_LOGONACK -= MessageReceiver_FMD_S_LOGONACK;
        //    messageReceiver.FMD_S_JOINED_MISSION -= MessageReceiver_FMD_S_JOINED_MISSION;
        //    messageReceiver.FMD_S_JOIN_SIDE -= MessageReceiver_FMD_S_JOIN_SIDE;
        //    messageReceiver.FMD_CS_SET_MISSION_OWNER -= MessageReceiver_FMD_CS_SET_MISSION_OWNER;
        //    messageReceiver.FMD_CS_QUIT_SIDE -= MessageReceiver_FMD_CS_QUIT_SIDE;

        //    messageReceiver.FMD_CS_SET_TEAM_LEADER -= MessageReceiver_FMD_CS_SET_TEAM_LEADER;
        //    messageReceiver.FMD_S_TEAM_READY -= MessageReceiver_FMD_S_TEAM_READY;
        //    messageReceiver.FMD_CS_CHATMESSAGE -= MessageReceiver_FMD_CS_CHATMESSAGE;
        //    messageReceiver.FMD_CS_FORCE_TEAM_READY -= MessageReceiver_FMD_CS_FORCE_TEAM_READY;

        //    messageReceiver.FMD_S_MISSION_STAGE -= MessageReceiver_FMD_S_MISSION_STAGE;
        //}

        private void MessageReceiver_FMD_S_MISSION_STAGE(ClientConnection client, AllegianceInterop.FMD_S_MISSION_STAGE message)
        {
            if ((MissionStage)message.stage == MissionStage.STAGE_STARTED)
            {
                FinishStrategy();
            }
        }

        private void MessageReceiver_FMD_CS_FORCE_TEAM_READY(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_CS_FORCE_TEAM_READY message)
        {
            Log($"FMD_CS_FORCE_TEAM_READY - side: {message.iSide}, ready: {message.fForceReady}");

            _readyStatus[message.iSide] = message.fForceReady;

            if (IsGameController == true)
                CheckForLaunch(client, message.iSide, message.fForceReady);
        }


        private void MessageReceiver_FMD_CS_CHATMESSAGE(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_CS_CHATMESSAGE message)
        {
            var ship = client.GetShip();

            if (ship.GetCommandTarget((sbyte)CommandType.c_cmdCurrent) != null)
            {
                Log($"\tcommandCurrent: {ship.GetCommandID((sbyte)CommandType.c_cmdCurrent)}, commandTarget: {ship.GetCommandTarget((sbyte)CommandType.c_cmdCurrent).GetName()}");
                Log($"\tcommandPlan: {ship.GetCommandID((sbyte)CommandType.c_cmdPlan)}, commandTarget: {ship.GetCommandTarget((sbyte)CommandType.c_cmdPlan).GetName()}");
                Log($"\tship.GetAutopilot() = {ship.GetAutopilot()}");
            }

            if (message.Message == "go")
            {
                var side = client.GetSide();
                var mission = side.GetMission();
                var hullTypes = mission.GetHullTypes();

                var station = ship.GetStation();

                if (station == null)
                {
                    // Works!
                    //{
                    //    var buoy = client.CreateBuoy((sbyte)BuoyType.c_buoyWaypoint, _random.Next(-1000, 1000), _random.Next(-1000, 1000), _random.Next(-1000, 1000), ship.GetCluster().GetObjectID(), true);
                    //    var command = ship.GetDefaultOrder(buoy);
                    //    ship.SetCommand((sbyte)CommandType.c_cmdCurrent, buoy, command);
                    //    ship.SetAutopilot(true);
                    //}


                    // Works!
                    {
                        var warps = ship.GetCluster().GetWarps();
                        var warp = warps[_random.Next(0, warps.Count())];
                        //var warp = ship.GetCluster().GetWarps().First();
                        CommandID command = (CommandID)ship.GetDefaultOrder(warp);

                        Log($"moving to point: {warp.GetName()}, command: {command.ToString()}");

                        // This should be it.
                        ship.SetCommand((sbyte)CommandType.c_cmdCurrent, warp, (sbyte)CommandID.c_cidGoto);
                        ship.SetAutopilot(true);
                    }

                    return;
                }

                List<AllegianceInterop.IhullTypeIGCWrapper> buyableHullTypes = new List<AllegianceInterop.IhullTypeIGCWrapper>();

                foreach (var hullType in hullTypes)
                {
                    if (hullType.GetGroupID() >= 0 && station.CanBuy(hullType) == true && station.IsObsolete(hullType) == false)
                    {
                        Log("buyable hullType: " + hullType.GetName());
                        buyableHullTypes.Add(hullType);
                    }
                }





                // Get a scout.
                var scoutHull = buyableHullTypes.FirstOrDefault(p => p.GetName().Contains("Scout"));
                if (scoutHull == null)
                    return;

                Log($"Found Scout: {scoutHull.GetName()}");

                var partTypes = mission.GetPartTypes();
                List<AllegianceInterop.IpartTypeIGCWrapper> buyablePartTypes = new List<AllegianceInterop.IpartTypeIGCWrapper>();
                foreach (var partType in partTypes)
                {
                    if (partType.GetGroupID() >= 0 && station.CanBuy(partType) == true && station.IsObsolete(partType) == false)
                    {
                        short equipmentTypeID = partType.GetEquipmentType();

                        bool isIncluded = false;

                        switch ((EquipmentType)equipmentTypeID)
                        {
                            case EquipmentType.NA:
                                isIncluded = true;
                                break;

                            case EquipmentType.ET_Weapon:
                            case EquipmentType.ET_Turret:
                                for (sbyte i = 0; i < scoutHull.GetMaxFixedWeapons(); i++)
                                {
                                    isIncluded |= scoutHull.GetPartMask(equipmentTypeID, i) != 0 && scoutHull.CanMount(partType, i) == true;
                                }
                                break;

                            default:
                                isIncluded |= scoutHull.GetPartMask(equipmentTypeID, 0) != 0 && scoutHull.CanMount(partType, 0) == true;

                                break;
                        }

                        if (isIncluded == true)
                        {
                            buyablePartTypes.Add(partType);
                            Log($"\tFound part: {partType.GetName()}, capacity for part: {scoutHull.GetCapacity(partType.GetEquipmentType())}");

                        }
                    }
                }

                var mines = buyablePartTypes.FirstOrDefault(p => p.GetName().Contains("Prox Mine") == true);

                //scoutHull.

                // Creating the empty ship adds the ship object to the core so that it will receive updates when the core.Update() is called.
                //var ship = client.CreateEmptyShip();
                ship.SetBaseHullType(scoutHull);
                client.BuyDefaultLoadout(ship, station, scoutHull, client.GetMoney());

                // Clear the cargo.
                for (sbyte i = -5; i < 0; i++)
                {
                    var currentPart = ship.GetMountedPart((short)EquipmentType.NA, i);
                    if (currentPart != null)
                        currentPart.Terminate();

                }

                if (mines != null)
                {
                    for (sbyte i = -5; i < 0; i++)
                    {
                        int amount = client.BuyPartOnBudget(ship, mines, i, client.GetMoney());
                        Log("Bought: " + amount + " mines.");
                    }

                    var dispenser = ship.GetMountedPart((short)EquipmentType.ET_Dispenser, 0);
                    dispenser.Terminate();

                    client.BuyPartOnBudget(ship, mines, 0, client.GetMoney());
                }

                client.BuyLoadout(ship, true);

                // Clean up the newShip.
                //newShip.Terminate();

                //ship = client.Get

                Task.Run(() =>
                {
                    // Wait for the ship to enter the cluster.
                    while (ship.GetCluster() == null)
                        Thread.Sleep(100);

                    client.FireDispenser(ship);

                    //var buoy = client.CreateBuoy((sbyte)BuoyType.c_buoyWaypoint, 3000, 0, 0, ship.GetCluster().GetObjectID(), true);

                    //ship.SetAutopilot(true);
                    //ship.SetCommand((sbyte)CommandType.c_cmdAccepted, buoy, (sbyte)CommandID.c_cidGoto);
                    //ship.SetAutopilot(true);

                    //// Drop mines!
                    //var launcher = AllegianceInterop.IdispenserIGCWrapper.GetDispenserForPart(ship.GetMountedPart((short)EquipmentType.ET_Dispenser, 0));

                    ////var launcher = (AllegianceInterop.IdispenserIGCWrapper) ship.GetMountedPart((short) EquipmentType.ET_Dispenser, 0);
                    //client.FireExpendable(ship, launcher, (uint)DateTime.Now.Ticks);
                });





            }

            if (message.Message == "miner")
            {
                var side = client.GetSide();
                var buckets = side.GetBuckets();
                var money = client.GetMoney();

                foreach (var bucket in buckets)
                {
                    if (bucket.GetName() == ".Miner")
                    {
                        if (money > bucket.GetPrice() && bucket.GetCompleteF() == false)
                        {
                            Log("Adding money to bucket. Bucket wants: " + bucket.GetPrice() + ", we have: " + money);
                            client.AddMoneyToBucket(bucket, bucket.GetPrice());
                        }
                        else
                        {
                            Log("Already building: " + bucket.GetPercentComplete());
                        }
                    }
                }
            }
        }

        private void MessageReceiver_FMD_S_TEAM_READY(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_S_TEAM_READY message)
        {
            // Only the game controller can launch the game.
            if (IsGameController == false)
            {
                // If we are not the game controller, then re-send the ready message in case the other side joined after we readied up the first time.
                AllegianceInterop.FMD_CS_FORCE_TEAM_READY forceReady = new AllegianceInterop.FMD_CS_FORCE_TEAM_READY(SideIndex, true);
                client.SendMessageServer(forceReady);
                return;
            }

            Log($"FMD_S_TEAM_READY - Received ready status: {message.fReady} for side: {message.iSide}");

            CheckForLaunch(client, message.iSide, message.fReady);
        }

        // When a bot comm is made team leader, then it should set it's team name and faction.
        private void MessageReceiver_FMD_CS_SET_TEAM_LEADER(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_CS_SET_TEAM_LEADER message)
        {
            if (message.shipID <= 0)
                return;

            AllegianceInterop.FMD_S_PLAYERINFO playerInfo;
            if (message.shipID == _shipID)
            {
                Log($"This player is now the team leader of side index: {message.sideID}, _hasSetTeamAndCiv: {_hasSetTeamAndCiv}");

                if (_hasSetTeamAndCiv == false)
                {
                    int noSquad = -1;

                    string[] teamNames = { "Yellow Pigs", "Blue Pigs" };
                    AllegianceInterop.FMD_CS_SET_TEAM_INFO teamInfo = new AllegianceInterop.FMD_CS_SET_TEAM_INFO(noSquad, SideIndex, teamNames[SideIndex]);
                    client.SendMessageServer(teamInfo);


                    var enumValues = Enum.GetValues(typeof(Civilization));

                    _targetCivID = (short)enumValues.GetValue(_random.Next(0, enumValues.Length));

                    AllegianceInterop.FMD_CS_CHANGE_TEAM_CIV teamCiv = new AllegianceInterop.FMD_CS_CHANGE_TEAM_CIV(SideIndex, _targetCivID, false);
                    client.SendMessageServer(teamCiv);

                    AllegianceInterop.FMD_CS_FORCE_TEAM_READY forceReady = new AllegianceInterop.FMD_CS_FORCE_TEAM_READY(SideIndex, true);
                    client.SendMessageServer(forceReady);

                    _hasSetTeamAndCiv = true;

                    //_isCurrentCommander = true;
                }
            }
            //else if (message.sideID == SideIndex) // Someone else was made the leader of our side. This is an outrage!
            //{
            //    _isCurrentCommander = false;
            //}
        }

        private void MessageReceiver_FMD_CS_QUIT_SIDE(ClientConnection client, AllegianceInterop.FMD_CS_QUIT_SIDE message)
        {
            if (String.IsNullOrWhiteSpace(message.szMessageParam) == false && message.shipID == _shipID)
            {
                Log($"Player was removed from side: {message.szMessageParam}, moving back to sideIndex: {SideIndex}.");

                AllegianceInterop.FMD_C_POSITIONREQ joinSide = new AllegianceInterop.FMD_C_POSITIONREQ(SideIndex);
                client.SendMessageServer(joinSide);
            }
        }

        private void MessageReceiver_FMD_CS_SET_MISSION_OWNER(ClientConnection client, AllegianceInterop.FMD_CS_SET_MISSION_OWNER message)
        {
            if (message.shipID == _shipID)
            {
                Log("This player is now the game controller (mission owner), resetting mission parameters.");
                ResetGameParameters(client);
            }
        }

        private void MessageReceiver_FMD_S_JOIN_SIDE(ClientConnection client, AllegianceInterop.FMD_S_JOIN_SIDE message)
        {
            AllegianceInterop.FMD_S_PLAYERINFO playerInfo;
            if (_shipID == message.shipID)
            {
                Log($"Player joined side {message.sideID}");

                //playerInfo.iSide = message.sideID;

                if (message.sideID != SideIndex)
                {
                    AllegianceInterop.FMD_C_POSITIONREQ positionRequest = new AllegianceInterop.FMD_C_POSITIONREQ(SideIndex);
                    client.SendMessageServer(positionRequest);
                }
                else
                {
                    if (IsCommander == true)
                    {
                        AllegianceInterop.FMD_CS_SET_TEAM_LEADER setTeamLeader = new AllegianceInterop.FMD_CS_SET_TEAM_LEADER(message.sideID, message.shipID);
                        client.SendMessageServer(setTeamLeader);
                    }

                    if (IsGameController == true)
                    {
                        AllegianceInterop.FMD_CS_SET_MISSION_OWNER setMissionOwner = new AllegianceInterop.FMD_CS_SET_MISSION_OWNER(message.sideID, message.shipID);
                        client.SendMessageServer(setMissionOwner);
                    }
                }
            }

            //if (_playerInfoByShipID.ContainsKey(message.shipID) == true)
            //{
            //    // If we receive an update for our player, and our player is the game controller and was not already on the team they are joining, then reset the game parameters. 
            //    //if (_isGameController == true && _playerInfoByShipID[message.shipID].CharacterName == _playerName && _playerInfoByShipID[message.shipID].iSide != message.sideID && message.sideID >= 0)
            //    //    ResetGameParameters(client);

            //    _playerInfoByShipID[message.shipID].iSide = message.sideID;
            //}
        }

        private void MessageReceiver_FMD_S_AUTODONATE(ClientConnection client, AllegianceInterop.FMD_S_AUTODONATE message)
        {
            
            // If we are supposed to be the commander and we get an "auto-donate message", force ourselves into the commander seat!
            // This also steals all the money from all the other players. 
            if (IsCommander == true && message.sidDonateTo != client.GetShip().GetObjectID())
            {
                AllegianceInterop.FMD_CS_SET_TEAM_LEADER setTeamLeader = new AllegianceInterop.FMD_CS_SET_TEAM_LEADER(client.GetSide().GetObjectID(), client.GetShip().GetObjectID());
                client.SendMessageServer(setTeamLeader);
            }

            if (IsGameController == true && message.sidDonateTo != client.GetShip().GetObjectID())
            {
                AllegianceInterop.FMD_CS_SET_MISSION_OWNER setMissionOwner = new AllegianceInterop.FMD_CS_SET_MISSION_OWNER(client.GetSide().GetObjectID(), client.GetShip().GetObjectID());
                client.SendMessageServer(setMissionOwner);
            }
        }

        private void MessageReceiver_FMD_S_JOINED_MISSION(ClientConnection client, AllegianceInterop.FMD_S_JOINED_MISSION message)
        {
            Log("MessageReceiver_FMD_S_JOINED_MISSION");

            if (message.shipID == _shipID)
            {
                //// wait for target side to become active before joining.
                //Task.Run(() =>
                //{
                //    if (ClientConnection.GetCore() == null || ClientConnection.GetCore().GetSide(SideIndex) == null || ClientConnection.GetCore().GetSide(SideIndex).GetActiveF() == false)
                //    {
                //        Log($"\tWaiting 30 seconds for side {SideIndex} to become ready.");
                        
                //        for (int i = 0; i < 30 * 100; i++)
                //        {
                //            if (ClientConnection.GetCore().GetSide(SideIndex).GetActiveF() == true)
                //                break;

                //            Thread.Sleep(100);
                //        }

                //        Log($"\tDone waiting, side {SideIndex} current ActiveF status is {ClientConnection.GetCore().GetSide(SideIndex).GetActiveF()}");
                //    }

                //    Log("\tSending side join (POSIITON) request.");
                //    AllegianceInterop.FMD_C_POSITIONREQ positionReq = new AllegianceInterop.FMD_C_POSITIONREQ(SideIndex);
                //    client.SendMessageServer(positionReq);

                //});

                Log("\tSending side join (POSIITON) request.");
                AllegianceInterop.FMD_C_POSITIONREQ positionReq = new AllegianceInterop.FMD_C_POSITIONREQ(SideIndex);
                client.SendMessageServer(positionReq);


            }
        }

        private void MessageReceiver_FMD_S_LOGONACK(ClientConnection client, AllegianceInterop.FMD_S_LOGONACK message)
        {
            Log("MessageReceiver_FMD_S_LOGONACK received, disconnecting from lobby.");

            _shipID = message.shipID;
            _playerCookie = message.cookie;

            client.DisconnectLobby();
        }


        private void MessageReceiver_FMD_LS_LOBBYMISSIONINFO(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_LS_LOBBYMISSIONINFO message)
        {
            Log($"Got mission info for game: {message.szGameName}, server address: {message.szServerAddr}, server port: {message.dwPort}, mission cookie: {message.dwCookie}, _isClientConnectedToServer: {_isClientConnectedToServer}, GameName: {GameName}. ");

            //switch (_serverType)
            //{
            //    case ServerType.Lobby:
            if (_isClientConnectedToServer == false && message.szGameName == GameName)
            {
                _isClientConnectedToServer = true;

                Log("Connecting to game, waiting for FM_S_LOGONACK from server.");

                AllegianceInterop.FMD_C_JOIN_GAME_REQ joinGame = new AllegianceInterop.FMD_C_JOIN_GAME_REQ(message.dwCookie);
                client.SendMessageLobby(joinGame);

                //bool connected = false;
                //while (connected == false && _cancellationTokenSource.IsCancellationRequested == false)
                //{
                //    // Once the client connects to the server, the FM_S_LOGONACK response will trigger the disconnect from the lobby.
                //    if (client.ConnectToServer(message.szServerAddr, (int)message.dwPort, PlayerName, BotAuthenticationGuid, (int)message.dwCookie) == true)
                //    {
                //        connected = true;
                //        break;
                //    }

                //    Log("\tCouldn't connect, retrying.");

                //    Thread.Sleep(100);
                //}


                //Task.Run(() =>
                //{
                //    bool connected = false;
                //    while(connected == false && _cancellationTokenSource.IsCancellationRequested == false)
                //    {
                //        // Once the client connects to the server, the FM_S_LOGONACK response will trigger the disconnect from the lobby.
                //        if (ClientConnection.ConnectToServer(message.szServerAddr, (int)message.dwPort, PlayerName, BotAuthenticationGuid, (int)message.dwCookie) == true)
                //        {
                //            connected = true;
                //            break;
                //        }

                //        Log("\tCouldn't connect, retrying.");

                //        Thread.Sleep(100);
                //    }
                //});
            }

            if (Configuration.ServerName.Equals(message.szServerName, StringComparison.InvariantCultureIgnoreCase) == true)
            {
                _receivedMissionCount++;
                _waitingForMissionCount--;

                // No more mission details are coming, and we didn't find a bot game, so create a new one. 
                if (_waitingForMissionCount == 0 && _isClientConnectedToServer == false)
                {
                    Log($"MessageReceiver_FMD_LS_LOBBYMISSIONINFO(): Creating game. message.szServerName: {message.szServerName}, message.szServerAddr: {message.szServerAddr}, CoreName: {CoreName},  GameName: {GameName}");

                    _hasCreatedGame = true;
                    AllegianceInterop.FMD_C_CREATE_MISSION_REQ req = new AllegianceInterop.FMD_C_CREATE_MISSION_REQ(message.szServerName, message.szServerAddr, CoreName, GameName);
                    client.SendMessageLobby(req);
                }
            }
            
        }

        private void MessageReceiver_FMD_LS_LOBBYMISSIONINFO_Old(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_LS_LOBBYMISSIONINFO message)
        {
            Log($"Got mission info for game: {message.szGameName}, server address: {message.szServerAddr}, server port: {message.dwPort}, mission cookie: {message.dwCookie}");

            //switch (_serverType)
            //{
            //    case ServerType.Lobby:
            if (_isClientConnectedToServer == false && message.szGameName == GameName)
            {
                _isClientConnectedToServer = true;

                Log("Connecting to game, waiting for FM_S_LOGONACK from server.");

                bool connected = false;
                while (connected == false && _cancellationTokenSource.IsCancellationRequested == false)
                {
                    // Once the client connects to the server, the FM_S_LOGONACK response will trigger the disconnect from the lobby.
                    if (client.ConnectToServer(message.szServerAddr, (int)message.dwPort, PlayerName, BotAuthenticationGuid, (int)message.dwCookie) == true)
                    {
                        connected = true;
                        break;
                    }

                    Log("\tCouldn't connect, retrying.");

                    Thread.Sleep(100);
                }


                //Task.Run(() =>
                //{
                //    bool connected = false;
                //    while(connected == false && _cancellationTokenSource.IsCancellationRequested == false)
                //    {
                //        // Once the client connects to the server, the FM_S_LOGONACK response will trigger the disconnect from the lobby.
                //        if (ClientConnection.ConnectToServer(message.szServerAddr, (int)message.dwPort, PlayerName, BotAuthenticationGuid, (int)message.dwCookie) == true)
                //        {
                //            connected = true;
                //            break;
                //        }

                //        Log("\tCouldn't connect, retrying.");

                //        Thread.Sleep(100);
                //    }
                //});
            }

            _receivedMissionCount++;
            _waitingForMissionCount--;

            // No more mission details are coming, and we didn't find a bot game, so create a new one. 
            if (_waitingForMissionCount == 0 && _isClientConnectedToServer == false)
            {
                _hasCreatedGame = true;
                AllegianceInterop.FMD_C_CREATE_MISSION_REQ req = new AllegianceInterop.FMD_C_CREATE_MISSION_REQ(message.szServerName, message.szServerAddr, CoreName, GameName);
                client.SendMessageLobby(req);


            }

            //        break;

            //    case ServerType.Server:
            //        Log("Ready to join mission.");
            //        break;

            //    default:
            //        throw new NotSupportedException(_serverType.ToString());
            //}
        }

        private void MessageReceiver_FMD_L_SERVERS_LIST(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_L_SERVERS_LIST message)
        {
            Log("FM_L_SERVERS_LIST_OnEvent:");

            AllegianceInterop.ServerCoreInfo_Managed targetServer = null;

            foreach (var server in message.Servers)
            {
                Log("\t" + server.szName + ":" + server.szRemoteAddress);

                if (Configuration.ServerName.Equals(server.szName, StringComparison.InvariantCultureIgnoreCase) == true)
                    targetServer = server;
            }

            //AllegianceInterop.StaticCoreInfo_Managed targetCore = null;

            foreach (var core in message.Cores)
            {
                Log("\t" + "Core: " + core.cbIGCFile);

                //if (targetCore.cbIGCFile == CoreName)
                //    targetCore = core;
            }

            if (_isClientConnectedToServer == false)
            {
                if (IsGameController == true && _hasCreatedGame == false && targetServer != null && (targetServer.iCurGames == 0 || targetServer.iCurGames == _receivedMissionCount))
                {
                    _hasCreatedGame = true;

                    // Create a game on the first server found.
                    AllegianceInterop.FMD_C_CREATE_MISSION_REQ req = new AllegianceInterop.FMD_C_CREATE_MISSION_REQ(targetServer.szName, targetServer.szRemoteAddress, CoreName, GameName);
                    client.SendMessageLobby(req);
                }
                else if (targetServer != null)
                {
                    _waitingForMissionCount += targetServer.iCurGames;
                    Log($"Waiting for mission details from Lobby for {targetServer.iCurGames} missions. IsGameController: {IsGameController}, _hasCreatedGame: {_hasCreatedGame}, targetServer: {targetServer},  _waitingForMissionCount: {_waitingForMissionCount}, _receivedMissionCount: {_receivedMissionCount}");
                }
                else
                {
                    Log("No servers currently attached to the lobby, waiting for a server to join.");
                }
            }
        }

        private void MessageReceiver_FMD_L_LOGON_ACK(global::AllegianceInterop.ClientConnection client, global::AllegianceInterop.FMD_L_LOGON_ACK message)
        {
            AllegianceInterop.FMD_C_GET_SERVERS_REQ msg = new AllegianceInterop.FMD_C_GET_SERVERS_REQ();
            client.SendMessageLobby(msg);

            //bool foundMission = false;
            //var missions = client.GetMissionList();
            //foreach (var mission in missions)
            //{
            //    if (mission.Name() == GameName)
            //    {
            //        foundMission = true;
            //        client.JoinMission(mission, "");
            //        break;
            //    }
            //}

            //if (foundMission == false)
            //{

            //}
        }

        // Set the game parameters back to their proper defaults (map to hihigher, etc.)
        private void ResetGameParameters(AllegianceInterop.ClientConnection client)
        {
            AllegianceInterop.FMD_CS_MISSIONPARAMS missionParameters = new AllegianceInterop.FMD_CS_MISSIONPARAMS(
                GameName,
                AllegianceInterop.MapType.c_mmHiHigher,
                MaxPlayersPerGame,
                MaxPlayersPerGame / 2
                );

            client.SendMessageServer(missionParameters);

            Log($"ResetGameParameters: SideIndex = {SideIndex}");

            AllegianceInterop.FMD_CS_FORCE_TEAM_READY forceReady = new AllegianceInterop.FMD_CS_FORCE_TEAM_READY(SideIndex, true);
            client.SendMessageServer(forceReady);
        }

        private void CheckForLaunch(AllegianceInterop.ClientConnection client, int iSide, bool isReady)
        {
            if (iSide >= 0)
                _readyStatus[iSide] = isReady;

            int readyTeamCount = 0;

            Log("Ready Status: ");

            for (int i = 0; i < MaxTeams; i++)
            {
                Console.Write(_readyStatus[i] + " ");

                if (_readyStatus[i] == true)
                    readyTeamCount++;
            }

            Console.WriteLine("");

          
            if (readyTeamCount == MaxTeams)
            {
                Log("All sides are ready, launching the game.");
                AllegianceInterop.FMD_C_START_GAME startGame = new AllegianceInterop.FMD_C_START_GAME();
                client.SendMessageServer(startGame);
            }
        }

        //private void Log(string message)
        //{
        //    Console.WriteLine($"{_playerName}: {message}");
        //}


    }
}
