using AllegianceInterop;
using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using Wopr;
using Wopr.Constants;
using Wopr.Entities;

namespace Wopr.Strategies
{
    public class CommanderResearchAndExpand : StrategyBase
    {
        private const int AmountOfMoneyToKeepOnHand = 1000; // The amount of money that the commander should keep on hand to let players buy bombers and stuff.
        private const string SecondTechbaseMarker = "Second Techbase";

        private List<String> _investmentList = new List<String>();

        // Key: Ship Object ID, Value: Cluster Object ID.
        Dictionary<int, InflightConstructor> _constuctorsInFlightToClusterObjectIDByShipObjectID = new Dictionary<int, InflightConstructor>();

        List<int> _clustersWaitingForConstructors = new List<int>();

        TechpathType ? _primaryTechpath = null;
        TechpathType? _secondaryTechpath = null;

        Dictionary<TechpathType, List<String>> _primaryTechPaths;
        Dictionary<TechpathType, List<String>> _secondaryTechPaths;

        bool _readyForSecondTechbase = false;

        public CommanderResearchAndExpand() : base(Wopr.Constants.StrategyID.CommanderResearchAndExpand, TimeSpan.MaxValue)
        {
            _primaryTechPaths = new Dictionary<TechpathType, List<string>>();
            _secondaryTechPaths = new Dictionary<TechpathType, List<string>>();

            List<String> supremacyTechPath = new List<string>(new string[] {
                "Booster 1",
                "Enh Miner",
                "Enh Fighter",
                "PW Gattling Gun 2",
                "MRM Seeker 2",
                "SRM Dumbfire 2",
                "Ship Hull 1",
                "Sm Shield 2",
                "GA: Missile Damage 1",
                "Bomber",

                "Upgrade Supremacy",

                "Enh Constructor",
                "Adv Fighter",
                "PW Gattling Gun 3",
                "MRM Seeker 3",
                "SRM Dumbfire 3",
                "Sm Shield 3",
                "GA: Ripcord 1",
                "Hvy Bomber",
                "SRM Anti-Base 2",
                "Counter 2",
                "Adv Scout",
                "Gunship",

                SecondTechbaseMarker, // We can now build a secondary tech if we want to.

                "PW Auto-Cannon 2",
                "EW Disruptor 2",
                "Med Shield 2",
                "ER Nanite 2",
                "EW Galvonic Blaster",
                "EWS Probe 2",
                "Fighter/Bomber",
                "Booster 3",
                "XRM Anti-Base 1",
                "XRM Anti-Base 2",
                "Lt Booster 1",
                "Hvy Booster",
                "Cruise Booster",

                "Upgrade Garrison",

                "Mine Pack",
                "Teleport Probe 1",
                "Teleport Probe 2",
                "Hvy Scout",
                "SRM Anti-Base 3",
                "GA: Missile Damage 2",
                "GA: Ship Hull 2",
                "Booster",
                "GA: Ship Shield 2",
                "GA: Ship Sensors 2",
                "Prox Mine 2",
                "GA: Ripcord 2",
                "Counter 3",
                "PW Auto-Cannon 3",
                "Prox Mine 3",
                "EWS Probe 3",
                "Med Shield 3",
                "ER Nanite 3",
                "Omni Fighter"
            });

            _primaryTechPaths.Add(TechpathType.Supremacy, supremacyTechPath);

            List<String> tacticalTechPath = new List<string>(new string[] {
                "Enh Miner",
                "Stealth Fighter",
                "EW Utl Cannon 2",
                "EW Sniper Rifle 1",
                "LRM Hunter 2",
                "Sig Cloak 1",
                "Sig Cloak 2",
                "Booster 1",
                "GA: Missile Track 1",
                "GA: Ship Energy 1",
                "GA: Ship Agility 1",
                "GA: EW Range 1",
                "GA: Ship Speed 1",
                
                "Bomber",
                "Upgrade Tactical",
                "Adv Stealth Fighter",

                SecondTechbaseMarker, // We can now build a secondary tech if we want to.

                "EW Utl Cannon 3",
                "EW Sniper Rifle 2",
                "LRM Hunter 3",
                "Sig Cloak 3",
                "GA: Missile Track 2",
                "GA: Ship Energy 2",
                "GA: Ship Agility 2",
                "GA: EW Range 2",
                "GA: Ship Speed 2",
                

                "Stealth Bomber",

                "Upgrade Garrison",
                //"Hvy Bomber",
                
                //"Counter 2",
                "Adv Scout",
                "Gunship",
                
                "Med Shield 2",
                "ER Nanite 2",
                
                //"EWS Probe 2",
                
                "Teleport Probe 1",
                //"Teleport Probe 2",
                "Hvy Scout",
                
                //"EWS Probe 3",
                "Med Shield 3",
                "ER Nanite 3",
                "Omni Fighter"
            });

            _primaryTechPaths.Add(TechpathType.Tactical, tacticalTechPath);

            List<String> expansionTechPath = new List<string>(new string[] {
                "Enh Miner",
                "Booster 1",
                "Interceptor",
                "PW Mini-Gun 2",

                "GA: Faster He3 1",
                "GA: He3 Yield 1",
                "GA: Ship Acceleration 1",
                "GA: PW&EW Damage 1",
                "GA: PW Range 1",
                
                //"Bomber",
                "Upgrade Expansion",

                "Hvy Interceptor",

                //"Enh Constructor",

                SecondTechbaseMarker, // We can now build a secondary tech if we want to.

                "PW Mini-Gun 3",
                "GA: Station Hull 1",
                "GA: Station Shield 1",

                "GA: Faster He3 2",
                "GA: He3 Yield 2",
                "GA: Ship Acceleration 2",
                "GA: PW&EW Damage 2",
                "GA: PW Range 2",

                "Troop Transport",
                "Hvy Troop Transport",
                "SRM EMP Missile",

                "GA: Ship Signature 1",
                "GA: Ship Signature 2",

                "Prox Mine 2",
                "Pulse Probe 1",
                
                "Upgrade Garrison",
                //"Hvy Bomber",
                
                "Counter 2",
                "Adv Scout",
                "Gunship",

                "Med Shield 2",
                "ER Nanite 2",
                "Med Shield 3",

                //"EWS Probe 2",

                "Hvy Scout",

                "Prox Mine 3",
                "Pulse Probe 2",

                "GA: EWS Probe 3",
                "GA: Med Shield 3",
                "GA: ER Nanite 3",
                "Counter 3",

                "GA: Station Hull 2",
                "GA: Station Shield 2",
                "Omni Fighter"

            });

            _primaryTechPaths.Add(TechpathType.Expansion, expansionTechPath);

            List<String> expansionSecondary = new List<string>(new string[]
                {
                    "Booster 1",
                    "GA: PW&EW Damage 1",
                    "GA: PW Range 1",
                    "GA: Faster He3 1",
                    "GA: He3 Yield 1",
                    "GA: Ship Acceleration 1",
                    "GA: Ship Signature 1",

                    "Pulse Probe 1",

                    "Upgrade Expansion",

                    "GA: PW&EW Damage 2",
                    "GA: PW Range 2",
                    "GA: Faster He3 2",
                    "GA: He3 Yield 2",
                    "GA: Station Hull 1",
                    "GA: Station Shield 1",
                    "GA: Ship Acceleration 2",
                    "GA: Ship Signature 2",
                    "GA: Station Hull 2",
                    "GA: Station Shield 2",
                    
                    "Pulse Probe 2",
                    "Adv Miner",
                    "Adv Constructor"
                });

            _secondaryTechPaths.Add(TechpathType.Expansion, expansionSecondary);

            List<String> tacticalSecondary = new List<string>(new string[]
                {
                    "GA: Ship Agility 1",
                    "GA: Ship Speed 1",
                    "GA: Missile Track 1",
                    "Upgrade Tactical",
                    "GA: Ship Agility 2",
                    "GA: Ship Speed 2",
                    "GA: Missile Track 2",
                });

            _secondaryTechPaths.Add(TechpathType.Tactical, tacticalSecondary);

            List<String> supremacySecondary = new List<string>(new string[]
                {
                    "Booster 2",
                    "GA: Ship Hull 1",
                    "GA: Missile Damage 1",
                    "GA: Sensors 1",
                    "GA: Ripcord 1",
                    "Upgrade Supremacy",
                    "EWS Probe 2",
                    "GA: Ship Hull 2",
                    "GA: Missile Damage 2",
                    "GA: Sensors 2",
                    "GA: Ripcord 2",
                    "SRM Anti-Base 2",
                    "SRM Anti-Base 3",
                    "EWS Probe 3"
                });

            _secondaryTechPaths.Add(TechpathType.Supremacy, supremacySecondary);

        }

        public override void AttachMessages(MessageReceiver messageReceiver, string botAuthenticationGuid, string playerName, int sideIndex, bool isGameController, bool isCommander)
        {
            messageReceiver.FMD_S_PAYDAY += MessageReceiver_FMD_S_PAYDAY;
            messageReceiver.FMD_S_SHIP_STATUS += MessageReceiver_FMD_S_SHIP_STATUS;
            messageReceiver.FMD_S_BUCKET_STATUS += MessageReceiver_FMD_S_BUCKET_STATUS;
            messageReceiver.FMD_S_STATIONS_UPDATE += MessageReceiver_FMD_S_STATIONS_UPDATE;
            messageReceiver.FMD_CS_CHATMESSAGE += MessageReceiver_FMD_CS_CHATMESSAGE;
            messageReceiver.FMD_S_MONEY_CHANGE += MessageReceiver_FMD_S_MONEY_CHANGE;
            //messageReceiver.FMD_S_PINGDATA += MessageReceiver_FMD_S_PINGDATA; // Use this to drive infrequent updates (updates every 5 seconds).
            messageReceiver.FMD_CS_PING += MessageReceiver_FMD_CS_PING; // Use this to drive infrequent updates (updates every 5 seconds).
            messageReceiver.FMD_CS_REQUEST_MONEY += MessageReceiver_FMD_CS_REQUEST_MONEY;
        }

        private void MessageReceiver_FMD_CS_REQUEST_MONEY(ClientConnection client, AllegianceInterop.FMD_CS_REQUEST_MONEY message)
        {
            // If we got the cash, hand out the goodies!
            if (client.GetMoney() > message.amount)
            {
                var requestingShip = client.GetSide().GetShip(message.shipidRequest);

                if (requestingShip != null && requestingShip?.GetPlayerInfo()?.GetMoney() < message.amount)
                {
                    Log($"Got a money request from {requestingShip.GetName()} for {message.amount}");

                    client.DonateMoney(client.GetSide().GetShip(message.shipidRequest).GetPlayerInfo(), message.amount);
                }
            }
        }

        private void MessageReceiver_FMD_CS_PING(ClientConnection client, AllegianceInterop.FMD_CS_PING message)
        {
            string nextTechName = GetNextTechbaseName(); 


            // Check all in-flight cons to make sure their destinations are still legit.
            foreach (short shipID in _constuctorsInFlightToClusterObjectIDByShipObjectID.Keys.ToArray())
            {
                var constructorInFlight = _constuctorsInFlightToClusterObjectIDByShipObjectID[shipID];

                var ship = ClientConnection.GetSide().GetShip(shipID);
                if (ship == null) // The constructor has built on the target rock.
                {
                    Log($"shipID: {shipID} is no longer valid, removing from _constuctorsInFlightToClusterObjectIDByShipObjectID list.");

                    _constuctorsInFlightToClusterObjectIDByShipObjectID.Remove(shipID);
                    continue;
                }

                //if (ship.GetCommandID((sbyte)CommandType.c_cmdCurrent) <= (sbyte)CommandID.c_cidNone) // The constructor's target rock is no longer available, or it has no current plan. 
                //{
                //    Log($"shipID: {shipID} doesn't have a current command. Finding a new destination for {ship.GetName()}");

                //    _constuctorsInFlightToClusterObjectIDByShipObjectID.Remove(shipID);

                //    this.PlaceStation(ship, GetHomeCluster().GetObjectID());
                //}
            }

            // Find any idle cons and see if we can send them on their way.
            // Looks like we don't always have an updated command from cons, so this won't work. 
            //foreach (var ship in client.GetSide().GetShips().Where(p => (PilotType)p.GetPilotType() == PilotType.c_ptBuilder))
            //{
            //    if (ship.GetCommandID((sbyte)CommandType.c_cmdCurrent) <= (sbyte)CommandID.c_cidNone) // The constructor's target rock is no longer available, or it has no current plan. 
            //    {
            //        Log($"{ship.GetName()} doesn't have a current command. Finding a new destination for it.");

            //        _constuctorsInFlightToClusterObjectIDByShipObjectID.Remove(ship.GetObjectID());

            //        this.PlaceStation(ship, GetHomeCluster().GetObjectID());
            //    }
            //}

            if(client.GetSide()?.GetBuckets()?.Count > 0)
                QueueTechForAvailableMoney();

            if (client.GetSide()?.GetShips()?.Count > 0)
                UndockMinersAndStartMining();
        }

        

        private void MessageReceiver_FMD_S_MONEY_CHANGE(ClientConnection client, AllegianceInterop.FMD_S_MONEY_CHANGE message)
        {
            //QueueTechForAvailableMoney();
        }

        private void MessageReceiver_FMD_CS_CHATMESSAGE(ClientConnection client, AllegianceInterop.FMD_CS_CHATMESSAGE message)
        {
            var ship = client.GetCore().GetShip(message.cd.sidSender);
            if (ship != null && (PilotType)ship.GetPilotType() == PilotType.c_ptBuilder && ship.GetSide().GetObjectID() == client.GetSide().GetObjectID())
            {
                if (ship.GetStationType().GetConstructorNeedRockSound() == message.cd.voiceOver && ship.GetCluster() != null)
                {
                    // Clear previous destination.
                    if (_constuctorsInFlightToClusterObjectIDByShipObjectID.ContainsKey(ship.GetObjectID()) == true)
                    {
                        Log($"Constructor {ship.GetName()} needs a new target rock.");
                        _constuctorsInFlightToClusterObjectIDByShipObjectID.Remove(ship.GetObjectID());
                    }

                    if (ship.GetCluster() != null)
                    {
                        PlaceStation(ship, ship.GetCluster().GetObjectID());
                    }
                    else
                    {
                        //  Waiting on FMD_S_SHIP_STATUS to update this ship's current position. (This happens when a constructor first launches.).
                        Log($"Constructor {ship.GetName()} is waiting for rock, but it doesn't have a cluster yet. Will use current Home Cluster {GetHomeCluster().GetName()} for starting point.");
                        PlaceStation(ship, GetHomeCluster().GetObjectID());
                    }
                }
                else
                {
                   
                }
               
            }
            
        }

        private void MessageReceiver_FMD_S_STATIONS_UPDATE(ClientConnection client, AllegianceInterop.FMD_S_STATIONS_UPDATE message)
        {
            foreach (var conInFlight in _constuctorsInFlightToClusterObjectIDByShipObjectID.Keys.ToArray())
            {
                if (client.GetSide().GetShip((short)conInFlight) == null)
                {
                    Log($"Constructor has planted, removing ship id: {conInFlight} from in flight list.");
                    _constuctorsInFlightToClusterObjectIDByShipObjectID.Remove(conInFlight);
                }
            }
        }

        private void MessageReceiver_FMD_S_BUCKET_STATUS(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_S_BUCKET_STATUS message)
        {
            //if (message.sideID != SideIndex)
            //    return;

            //var side = client.GetSide();
            //var completedBucket = side.GetBucket(message.iBucket);
            //var completedBucketName = completedBucket.GetName();

            //QueueTechForAvailableMoney();

            //if (completedBucketName.StartsWith(".") == true && completedBucketName.Contains("Miner") == true)
            //{
            //    QueueMinerIfRequired();
                
            //}
        }

        //private void QueueMinerIfRequired()
        //{
        //    if (ClientConnection.GetSide().GetShips().Where(p => p.GetName().Contains("Miner")).Count() < ClientConnection.GetCore().GetMissionParams().GetMaxDrones())
        //    {
        //        QueueTech(".Miner");
        //    }
        //}

        private void MessageReceiver_FMD_S_SHIP_STATUS(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_S_SHIP_STATUS message)
        {
            //            Log($@"FMD_S_SHIP_STATUS:
            //\tshipID:{message.shipID}
            //\thullID:{message.status.GetHullID()}
            //\thullName:{client.GetCore().GetHullType(message.status.GetHullID()).GetName()}
            //\tshipName:{client.GetCore().GetShip(message.shipID).GetName()}
            //\tship.Autopilot: {client.GetCore().GetShip(message.shipID).GetAutopilot()}");

            // Ignore ships that are already on thier way to a destination.
            if (_constuctorsInFlightToClusterObjectIDByShipObjectID.ContainsKey(message.shipID) == true)
                return;


            var ship = client.GetCore().GetShip(message.shipID);


            if ((PilotType) ship.GetPilotType() == PilotType.c_ptBuilder && message.status.GetSectorID() >= 0 && ship.GetSide().GetObjectID() == client.GetSide().GetObjectID())
                PlaceStation(ship, message.status.GetSectorID());
            
        }


        private void PlaceStation(IshipIGCWrapper ship, short clusterID)
        {
            if (_constuctorsInFlightToClusterObjectIDByShipObjectID.ContainsKey(ship.GetObjectID()) == true)
                return;

            StationType stationType;

            if (ship.GetName().Contains("Outpost") == true)
                stationType = StationType.Outpost;
            else if (ship.GetName().Contains("Teleport") == true)
                stationType = StationType.Teleport;
            else if (ship.GetName().Contains("Refinery") == true)
                stationType = StationType.Refinery;
            else if (ship.GetName().Contains("Supremacy") == true)
                stationType = StationType.Supremacy;
            else if (ship.GetName().Contains("Tactical") == true)
                stationType = StationType.Tactical;
            else if (ship.GetName().Contains("Expansion") == true)
                stationType = StationType.Expansion;
            else
                throw new NotSupportedException($"No station type found for ship name: {ship.GetName()}");



            var targetCluster = GetStationBuildCluster(stationType);
            var shipCluster = ClientConnection.GetCore().GetCluster(clusterID);

            Log($"\tPlaceStation: {stationType.ToString()}, constructor: {ship.GetName()}, currentCluster: {shipCluster.GetName()}");
            
            if (targetCluster == null)
            {
                Log("\t\tNo valid target cluster could be found, waiting for a cluster finish building to plot this station's destination.");
                return;
            }

            _constuctorsInFlightToClusterObjectIDByShipObjectID.Add(ship.GetObjectID(), new InflightConstructor()
            {
                Cluster = targetCluster,
                Ship = ship, 
                StationType = stationType
            });


            if (targetCluster != null)
            {
                Task.Run(() =>
                {
                    try
                    {
                        Interlocked.Increment(ref _runningTasks);

                        IclusterIGCWrapper intermediateCluster = null;

                        while (_cancellationTokenSource.IsCancellationRequested == false && targetCluster.GetAsteroids().Count == 0)
                        {
                            if (intermediateCluster == null || (shipCluster.GetObjectID() == intermediateCluster.GetObjectID()))
                            //if (ship.GetCommandID((sbyte) CommandType.c_cmdCurrent) <= (sbyte) CommandID.c_cidNone)
                            {
                                Log($"Looking for intermediate cluster, target cluster: {targetCluster.GetName()} has not been discovered yet. Current ship command is: {((CommandID)ship.GetCommandID((sbyte)CommandType.c_cmdCurrent)).ToString()}");
                                
                                var pathFinder = new DijkstraPathFinder(GameInfo.Clusters, targetCluster.GetObjectID(), GetHomeCluster().GetObjectID());

                                // Find a sector on the route that we have already explored, we will send the constructor there while we wait for scouts to find the way.
                                for (intermediateCluster = targetCluster; intermediateCluster != null && _cancellationTokenSource.IsCancellationRequested == false && intermediateCluster.GetAsteroids().Count == 0; intermediateCluster = pathFinder.NextCluster(intermediateCluster, GetHomeCluster())) ;

                                if (_cancellationTokenSource.IsCancellationRequested == true)
                                    break;

                                if (intermediateCluster != null && shipCluster != null && shipCluster.GetObjectID() != intermediateCluster.GetObjectID())
                                {
                                    Log($"Found intermediate cluster: {intermediateCluster.GetName()}, sending con there.");

                                    // Send the con to the middle of the cluster, and wait for the way to be found.
                                    var buoy = ClientConnection.CreateBuoy((sbyte)BuoyType.c_buoyWaypoint, 0, 0, 0, intermediateCluster.GetObjectID(), true);
                                    var command = ship.GetDefaultOrder(buoy);

                                    ClientConnection.SendChat(ClientConnection.GetShip(), ChatTarget.CHAT_INDIVIDUAL, ship.GetObjectID(), 0, "", (sbyte)command, buoy.GetObjectType(), buoy.GetObjectID(), buoy, true);
                                }
                            }

                            Thread.Sleep(250);
                        }

                        if (_cancellationTokenSource.IsCancellationRequested == false && targetCluster.GetAsteroids().Count > 0)
                        {
                            var targetRock = GetStationBuildRock(stationType, targetCluster);
                            Log($"Found target rock for con: {ship.GetName()}, sending con to: {targetCluster.GetName()}, {targetRock.GetName()}");

                            ClientConnection.SendChat(ClientConnection.GetShip(), ChatTarget.CHAT_INDIVIDUAL, ship.GetObjectID(), 0, "", (sbyte)CommandID.c_cidBuild, targetRock.GetObjectType(), targetRock.GetObjectID(), targetRock, true);
                        }
                    }
                    catch (Exception ex)
                    {
                        Log($"Exception in PlaceStation task: {ex.Message}");
                    }
                    finally
                    {
                        Interlocked.Decrement(ref _runningTasks);
                    }
                });
            }


           


            //var foundClusters = ClientConnection.GetCore().GetClusters();
            //var stations = ClientConnection.GetSide().GetStations();
            

            //var homeStation = stations.Where(p => p.GetCluster().GetHomeSector() == true).FirstOrDefault();
            //var homeCluster = homeStation.GetCluster();

            //var asteroidNames = homeCluster.GetAsteroids().ToDictionary(r => r.GetObjectID(), p => p.GetName());

            //Log("\t\tHome asteroids");
            //foreach (var name in asteroidNames.Values)
            //{
            //    Log($"\t\t\t{name}");
            //}

            //var homeRock = homeCluster.GetAsteroids().Where(p => p.GetName().StartsWith("He") == false && p.GetName().StartsWith("a") == false && String.IsNullOrWhiteSpace(p.GetName()) == false).FirstOrDefault();

            //Log($"\t\thomeRock: {homeRock.GetName()}");

            //var nextSectorTechRocks = homeCluster.GetWarps()
            //    .Select(p => p.GetDestination().GetCluster())
            //    .Select(r => r.GetAsteroids())
            //    .Select(q => q.Where(s => s.GetName().StartsWith("He") == false && s.GetName().StartsWith("a") == false && String.IsNullOrWhiteSpace(s.GetName()) == false).FirstOrDefault())
            //    .Where(t => t != null && String.IsNullOrWhiteSpace(t.GetName()) == false);

            //foreach (var nextSectorTechRock in nextSectorTechRocks)
            //    Log($"\t\tnextSectorTechRock: {nextSectorTechRock.GetName()}");

            //var viableNextSectors = nextSectorTechRocks.Where(p => String.IsNullOrWhiteSpace(p.GetName()) == false && String.IsNullOrWhiteSpace(homeRock.GetName()) == false  && p.GetName()[0] == homeRock.GetName()[0]);

            //foreach (var viableNextSectorItem in viableNextSectors)
            //    Log($"\t\tviableNextSector: {viableNextSectorItem.GetName()}");

            //var viableNextSector = viableNextSectors.FirstOrDefault();

            //var targetNextSector = homeCluster.GetWarps().FirstOrDefault().GetDestination().GetCluster();
            //if (viableNextSector != null)
            //    targetNextSector = viableNextSector.GetCluster();

            //Log($"targeting {targetNextSector.GetName()} for possible future tech base, will build outpost forward of this sector.");

            //// Find the next adjacent sector that does not lead back to the home sector! (We want a sector that is at least two hops away!)
            ////var targetBuildCluster = targetNextSector.GetWarps()
            ////    .Where(p => 
            ////        p.GetDestination().GetCluster().GetObjectID() != homeCluster.GetObjectID() // Don't go back to the home sector!
            ////        && p.GetDestination().GetCluster().GetWarps().Exists(r => r.GetDestination().GetCluster().GetObjectID() == homeCluster.GetObjectID()) == false)
            ////     .FirstOrDefault()
            ////     ?.GetCluster(); // Don't go to a sector that leads back to the home sector.

            //var targetBuildCluster = targetNextSector.GetWarps()
            //    .OrderByDescending(p => new DijkstraPathFinder(GameInfo.Clusters, p.GetDestination().GetCluster().GetObjectID(), homeCluster.GetObjectID()).GetDistance(p.GetDestination().GetCluster().GetObjectID(), homeCluster.GetObjectID()))
            //    .Select(p => p.GetDestination().GetCluster())
            //    .FirstOrDefault();

            //// Find the next forward sector if it's been found yet. We are only interested a sector that is two hops from home base.
            ////foreach (var nextSectorWarp in targetNextSector.GetWarps())
            ////{
            ////    var d = new DijkstraPathFinder(ClientConnection.GetCore(), nextSectorWarp.GetDestination().GetCluster(), homeCluster);
            ////}

            //bool foundTarget = false;

            //if (targetBuildCluster != null)
            //{
            //    Log($"\t\ttargeting {targetBuildCluster.GetName()} for outpost cluster.");

            //    // find closest rock to the center
            //    VectorWrapper centerPoint = new VectorWrapper(0, 0, 0);
            //    var targetRock = targetBuildCluster.GetAsteroids()
            //        .Where(p => p.GetName().StartsWith("a") == true)
            //        .OrderBy(p => (centerPoint - p.GetPosition()).LengthSquared())
            //        .FirstOrDefault();

            //    if (targetRock != null)
            //    {
            //        Log($"\t\ttargeting {targetRock.GetName()} for outpost rock.");

            //        ClientConnection.SendChat(ClientConnection.GetShip(), ChatTarget.CHAT_INDIVIDUAL, ship.GetObjectID(), 0, "", (sbyte)CommandID.c_cidBuild, targetRock.GetObjectType(), targetRock.GetObjectID(), targetRock, true);

            //        //ship.SetCommand((sbyte)CommandType.c_cmdCurrent, targetRock, (sbyte)CommandID.c_cidBuild);
            //        //ship.SetCommand((sbyte)CommandType.c_cmdAccepted, targetRock, (sbyte)CommandID.c_cidBuild);

            //        foundTarget = true;
            //        //ship.SetAutopilot(true);
            //    }
            //    else
            //    {
            //        Log($"Couldn't find a target rock in cluster: {targetBuildCluster.GetName()}");
            //    }
            //}
            //else
            //{
            //    Log($"Couldn't find a target cluster to build in.");
            //}

            //if (foundTarget == false)
            //{
            //    Log($"Couldn't find a target build rock, moving to sector: {targetNextSector.GetName()} while we wait for the rock to be found.");

            //    var buoy = ClientConnection.CreateBuoy((sbyte)BuoyType.c_buoyWaypoint, 0, 0, 0, targetNextSector.GetObjectID(), true);
            //    var command = ship.GetDefaultOrder(buoy);

            //    ClientConnection.SendChat(ClientConnection.GetShip(), ChatTarget.CHAT_INDIVIDUAL, ship.GetObjectID(), 0, "", (sbyte)command, buoy.GetObjectType(), buoy.GetObjectID(), buoy, true);


            //    //ship.SetCommand((sbyte)CommandType.c_cmdCurrent, buoy, command);
            //    //ship.SetCommand((sbyte)CommandType.c_cmdAccepted, buoy, command);

            //    //ship.SetCommand((sbyte)CommandType.c_cmdCurrent, targetNextSector, (sbyte)CommandID.c_cidGoto);
            //    //ship.SetCommand((sbyte)CommandType.c_cmdAccepted, targetNextSector, (sbyte)CommandID.c_cidGoto);
            //}
            
        }

        private IclusterIGCWrapper GetHomeCluster()
        {
            return ClientConnection.GetSide().GetStations().Where(p => p.GetCluster().GetHomeSector() == true).FirstOrDefault()?.GetCluster();
        }

        private void MessageReceiver_FMD_S_PAYDAY(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_S_PAYDAY message)
        {
            //QueueTechForAvailableMoney();

            
           
        }

        public override void Start()
        {
            //ClientConnection.TestMapMaker();

            var buyableItems = ClientConnection.GetSide().GetBuckets().Where(p => ClientConnection.GetSide().CanBuy(p) == true);

            Log("Buyable Items");
            foreach (var buyableItem in buyableItems)
                Log($"\t{buyableItem.GetName()}");

            Log("Ships");
            foreach (var ship in ClientConnection.GetSide().GetShips())
                Log($"\t{ship.GetName()}");

            Log("Stations");
            foreach (var station in ClientConnection.GetSide().GetStations())
                Log($"\t{station.GetName()} - {station.GetBaseStationType().GetName()}");



            //var bestOutpostSectors = 

            //foreach (var station in ClientConnection.GetSide().GetStations())
            //{
            //    // Find clusters that are 2 away from this station
            //}
            ////SelectTargetSectors();
            //var homeCluster = ClientConnection.GetCore().GetClusters().Where(p => p.GetHomeSector() == true 

            //QueueTechForAvailableMoney();
        }

 

        private IasteroidIGCWrapper GetStationBuildRock(StationType stationType, IclusterIGCWrapper cluster)
        {
            switch (stationType)
            {
                case StationType.Expansion:
                case StationType.Supremacy:
                case StationType.Tactical:
                    return GetClusterTechRock(cluster);

                default:
                    return GetAsteroidClosestToClusterCenter(cluster);
            }
        }

        private IasteroidIGCWrapper GetAsteroidClosestToClusterCenter(IclusterIGCWrapper cluster)
        {
            VectorWrapper centerPoint = new VectorWrapper(0, 0, 0);

            return cluster.GetAsteroids()
                .Where(p => p.GetName().StartsWith("a") == true)
                .OrderBy(p => (centerPoint - p.GetPosition()).LengthSquared())
                .FirstOrDefault();
        }

        private IclusterIGCWrapper GetStationBuildCluster(StationType stationType)
        {
            var stations = ClientConnection.GetSide().GetStations();
            var homeStation = stations.Where(p => p.GetCluster().GetHomeSector() == true).FirstOrDefault();
            var homeCluster = homeStation.GetCluster();
            var homeRock = homeCluster.GetAsteroids().Where(p => p.GetName().StartsWith("He") == false && p.GetName().StartsWith("a") == false && String.IsNullOrWhiteSpace(p.GetName()) == false).FirstOrDefault();
            var enemyHomeCluster = ClientConnection.GetCore().GetClusters().Where(p => p.GetHomeSector() == true && p.GetObjectID() != homeCluster.GetObjectID()).FirstOrDefault();

            // Always build a home tele first if we don't have one in our home cluster.
            if (stationType == StationType.Teleport && homeCluster.GetStations().Exists(p => p.GetName().Contains("Teleport") == true) == false
                && _constuctorsInFlightToClusterObjectIDByShipObjectID.Values.Count(r => r.Cluster.GetObjectID() == homeCluster.GetObjectID()) == 0)
            {
                Log($"There is no home teleporter yet, let's put this tele con at home: {homeCluster.GetName()}");
                return homeCluster;
            }
            
            // Try to find empty clusters where we have secured all sides.
            if (stationType == StationType.Refinery)
            {
                // For hihigher, this is the middle cluster.
                Log("Checking for an empty cluster that is bordered by our own stations.");
                var goodRefCluster = ClientConnection.GetCore().GetClusters()
                    .Where(p => p.GetStations().Count == 0  // Make sure there are no stations in it already.
                        && _constuctorsInFlightToClusterObjectIDByShipObjectID.Values.Count(r => r.Cluster.GetObjectID() == p.GetObjectID()) == 0 // Are any other cons heading to this cluster?
                        && GameInfo.GetUnexploredClusters(p.GetMission()).ContainsKey(p.GetObjectID()) == false // Is this cluster fully explored?
                        && p.GetWarps().All(r => 
                            r.GetDestination().GetCluster().GetStations().Count > 0  // Neighboring clusters have at least one station
                            && r.GetDestination().GetCluster().GetStations().All(s => s.GetSide().GetObjectID() == ClientConnection.GetSide().GetObjectID()))) // And all the stations belong to our side.
                    .FirstOrDefault();

                if (goodRefCluster != null)
                {
                    Log($"Found a good cluster for a refinery: {goodRefCluster.GetName()}");
                    return goodRefCluster;
                }
                else
                {
                    Log($"No good refinery cluster found. Looking for a good spot on available lines.");
                }
            }

            //var availableClusters = GameInfo.Clusters.Where(p => p.GetHomeSector() == true || ClientConnection.

            var routeFinder = new DijkstraPathFinder(GameInfo.Clusters, homeCluster.GetObjectID(), enemyHomeCluster.GetObjectID());
            var singleHopClusterDistances = homeCluster.GetWarps()
                .ToDictionary(
                p => p.GetDestination().GetCluster(),
                r => new DijkstraPathFinder(GameInfo.Clusters, r.GetDestination().GetCluster().GetObjectID(), enemyHomeCluster.GetObjectID())
                                .GetDistance(r.GetDestination().GetCluster(), enemyHomeCluster));

            List<ClusterInfo> clustersNextToHome = GameInfo.Clusters.Where(p => p.GetWarps().Exists(r => r.GetDestinationCluster().GetObjectID() == homeCluster.GetObjectID()) == true).ToList();

            // Walk each path until a station is found.
            while (clustersNextToHome.Count > 0)
            {
                var currentClusterObjectID = clustersNextToHome[_random.Next(0, clustersNextToHome.Count)];
                clustersNextToHome.Remove(currentClusterObjectID);

                var pathFinder = new DijkstraPathFinder(GameInfo.Clusters, currentClusterObjectID.GetObjectID(), enemyHomeCluster.GetObjectID());

                var currentCluster = ClientConnection.GetCore().GetCluster(currentClusterObjectID.GetObjectID());

                Log($"Considering line for {currentCluster.GetName()}");

                for (var nextClusterObjectID = (short)pathFinder.NextClusterObjectID(currentCluster.GetObjectID(), enemyHomeCluster.GetObjectID());
                        currentCluster.GetObjectID() != enemyHomeCluster.GetObjectID(); 
                        currentCluster = ClientConnection.GetCore().GetCluster(nextClusterObjectID), nextClusterObjectID = (short) pathFinder.NextClusterObjectID(nextClusterObjectID, enemyHomeCluster.GetObjectID()).GetValueOrDefault(-1))
                {
                    var nextCluster = ClientConnection.GetCore().GetCluster(nextClusterObjectID);

                    Log($"Considering cluster: {currentCluster.GetName()}, nextCluster: {nextCluster.GetName()}");

                    // Don't select this cluster if another constructor is already on the way to it.
                    if(_constuctorsInFlightToClusterObjectIDByShipObjectID.Values.Count(r => r.Cluster.GetObjectID() == currentCluster.GetObjectID()) > 0)
                    //if (_constuctorsInFlightToClusterObjectIDByShipObjectID.ContainsValue(currentCluster.GetObjectID()) == true)
                    {
                        Log($"There is already a constructor heading to {currentCluster.GetName()}, skipping to next sector.");
                        continue;
                    }

                    // If the station is a friendly station, then we can put a ref or tech base next to it. 
                    if (ClientConnection.GetCore().GetCluster(nextClusterObjectID).GetStations().Exists(p => p.GetSide().GetObjectID() == ClientConnection.GetSide().GetObjectID()) == true)
                    {
                        Log($"Friendly station found in nextCluster: {nextCluster.GetName()}");

                        // We don't want to put an output behind another friendly station. See if we can get closer!
                        if (stationType == StationType.Outpost)
                        {
                            Log("There is an outpost already in this line, so we don't want to put another one behind it.");
                            continue;
                        }

                        // If this matches to our home tech rock, then let's drop it here. 
                        if (stationType == StationType.Teleport)
                        {
                            // If the sector already has a teleport, then see if we can get closer!
                            if (ClusterContainsStation(StationType.Teleport, currentCluster) == true)
                            {
                                Log("There is already a teleport here, seeing if we can get closer.");
                                continue;
                            }

                            // If the sector's tech rock matches the home rock, then put a tele in it.
                            if (currentCluster.GetStations().Count == 0 && GetClusterTechRock(currentCluster).GetName()[0] == homeRock.GetName()[0])
                            {
                                Log("There is a good tech rock here, let's a put a tele here too!");
                                return currentCluster;
                            }
                        }

                        // We have a covering station in front of this one, so this is a good sector use for mining or tech bases, but we want refs to be as close to home as possible.
                        if (stationType == StationType.Refinery
                            && ClusterContainsStation(StationType.Refinery, currentCluster) == false)
                        {
                            var homeSectorPath = new DijkstraPathFinder(ClientConnection.GetCore(), currentCluster, homeCluster);
                            IclusterIGCWrapper refCluster = currentCluster;
                            for (nextCluster = currentCluster; nextCluster != homeCluster && nextCluster != null; nextCluster = homeSectorPath.NextCluster(nextCluster, homeCluster))
                            {
                                if (nextCluster.GetStations().Count(p => p.GetSide().GetObjectID() == ClientConnection.GetSide().GetObjectID()) > 0)
                                {
                                    Log($"Found a refinery cluster that is on a line protected by another station, with a friendly station backing it: {refCluster.GetName()}");
                                    return refCluster;
                                }

                                refCluster = nextCluster;
                            }

                            //Log($"This is good spot for a refinery! Recommending: {currentCluster.GetName()}");
                            //return currentCluster;
                        }

                        if (stationType == StationType.Garrison
                            && ClusterContainsStation(StationType.Garrison, currentCluster) == false)
                        {
                            Log($"This is good spot for a garrison! Recommending: {currentCluster.GetName()}");
                            return currentCluster;
                        }

                        if (stationType == StationType.ShipYard
                            && ClusterContainsStation(StationType.ShipYard, currentCluster) == false)
                        {
                            Log($"This is good spot for a ship yard! Recommending: {currentCluster.GetName()}");
                            return currentCluster;
                        }

                        var clusterRock = GetClusterTechRock(currentCluster);

                        if (clusterRock != null && stationType == StationType.Supremacy && clusterRock.GetName().StartsWith("C") == true)
                        {
                            Log($"This is good spot for a supremacy! Recommending: {currentCluster.GetName()}");
                            return currentCluster;
                        }

                        if (clusterRock != null && stationType == StationType.Expansion && clusterRock.GetName().StartsWith("U") == true)
                        {
                            Log($"This is good spot for an expansion! Recommending: {currentCluster.GetName()}");
                            return currentCluster;
                        }

                        if (clusterRock != null && stationType == StationType.Tactical && clusterRock.GetName().StartsWith("S") == true)
                        {
                            Log($"This is good spot for a tactical! Recommending: {currentCluster.GetName()}");
                            return currentCluster;
                        }

                    }
                    
                    // If the station is an enemy station, then we want to put a tele or outpost next to it.
                    if (ClientConnection.GetCore().GetCluster(nextClusterObjectID).GetStations().Exists(p => p.GetSide().GetObjectID() != ClientConnection.GetSide().GetObjectID()) == true
                        || (nextCluster.GetAsteroids().Count == 0 && ClientConnection.GetCore().GetCluster(nextClusterObjectID).GetHomeSector() == true)) // If we haven't actually eyed the enemy home, then we know their garrison is in there. 
                    {
                        Log($"Next sector: {nextCluster.GetName()} has an enemy station in it.");

                        // Always put outposts next to enemy stations. That's our favorite!
                        if (stationType == StationType.Outpost && ClusterContainsStation(StationType.Outpost, currentCluster) == false)
                        {
                            Log($"Found a good cluster for this outpost: {currentCluster.GetName()}, next to an enemy sector.");
                            return currentCluster;
                        }

                        // If the target sector doesn't already have an outpost in it, then put a tele one sector back if possible to allow an outpost to come forward.
                        if (stationType == StationType.Teleport && ClusterContainsStation(StationType.Outpost, currentCluster) == false)
                        {
                            var previousCluster = ClientConnection.GetCore().GetCluster((short) pathFinder.NextClusterObjectID(currentCluster.GetObjectID(), homeCluster.GetObjectID()));

                            if (previousCluster.GetStations().Count == 0)
                            {
                                Log($"There is an open cluster in {previousCluster.GetName()}, let's send the tele to there!");
                                return previousCluster;
                            }
                        }

                        // If the station type is a tech base, and the current cluster has a tele + tech rock in it, then let's get aggro!
                        if (currentCluster.GetStations().Exists(p => p.GetSide().GetObjectID() == ClientConnection.GetSide().GetObjectID() && p.GetName().Contains("Teleport") == true) == true)
                        {

                            if ((stationType == StationType.Supremacy || stationType == StationType.Expansion || stationType == StationType.Tactical) 
                                && DoesClusterConatainTechrockForStationType(stationType, currentCluster) == true)
                            {
                                Log($"There is an enemy station in the next sector, but we have a tele and a tech rock, so placing tech base forward in {currentCluster.GetName()}!");
                                return currentCluster;
                            }
                        }
                    }
                }
            }

            if (stationType == StationType.Refinery)
            {
                Log($"No good cluster found for this refinery, waiting for a good option to appear.");
                return null;
            }

            if (stationType == StationType.Supremacy || stationType == StationType.Expansion || stationType == StationType.Tactical)
            {
                var nearestAvailableClusterWithTechRock = GetNearestAvailableClusterWithTechrock(stationType, homeCluster);
                if (nearestAvailableClusterWithTechRock != null)
                {
                    Log($"No good target cluster found for tech base, but found a nearby available cluster: {nearestAvailableClusterWithTechRock.GetName()}.");
                    return nearestAvailableClusterWithTechRock;
                }

                if (DoesClusterConatainTechrockForStationType(stationType, homeCluster) == true)
                {
                    Log($"No good target found, placing this tech base at home: {homeCluster.GetName()}.");
                    return homeCluster;
                }

                Log($"No good cluster is available for this tech base, waiting for one to open up.");
                return null;
            }



            Log($"No targeted clusters were found, placing con in a random open cluster.");

            // No targeted clusters were found, let's go with any random empty cluster that we know about.
            var emptyClusters = ClientConnection.GetCore().GetClusters().Where(p => p.GetAsteroids().Count > 0 
                    && p.GetStations().Count == 0 
                    && _constuctorsInFlightToClusterObjectIDByShipObjectID.Values.Count(r => r.Cluster.GetObjectID() == p.GetObjectID()) == 0)
                .ToArray();

            if (emptyClusters.Count() == 0)
                return null;

            var targetCluster = emptyClusters[_random.Next(0, emptyClusters.Count())];

            Log($"Selecting random cluster: {targetCluster.GetName()} as target.");

            return targetCluster;



            //int minHops = singleHopClusterDistances.Select(p => p.Value).OrderBy(p => p).First();
            //var bestClusterRoutesForOutposts = singleHopClusterDistances.Where(p => p.Value == minHops).ToDictionary(p => p.Key, r => r.Value);

            //foreach (var cluster in bestClusterRoutesForOutposts.Keys)
            //{
            //    Log($"Start: {cluster.GetName()}");

            //    routeFinder = new DijkstraPathFinder(GameInfo.Clusters, cluster.GetObjectID(), enemyHomeCluster.GetObjectID());

            //    for (var nextCluster = routeFinder.NextClusterObjectID(cluster.GetObjectID(), enemyHomeCluster.GetObjectID()); nextCluster != null; nextCluster = routeFinder.NextClusterObjectID(nextCluster.GetValueOrDefault(0), enemyHomeCluster.GetObjectID()))
            //    {
            //        Log($"\tNext: {ClientConnection.GetCore().GetCluster((short)nextCluster.GetValueOrDefault(0)).GetName()}");
            //    }
            //}



        }

        private IclusterIGCWrapper GetNearestAvailableClusterWithTechrock(StationType stationType, IclusterIGCWrapper currentCluster)
        {
            // Get all the clusters in the galaxy sorted by distance to the home cluster, excluding ones that have enemy stations in them, and our home cluster.
            var availableTechClusters = ClientConnection.GetCore().GetClusters().Where(p =>
                    p.GetStations().Exists(r => r.GetSide().GetObjectID() != ClientConnection.GetSide().GetObjectID()) == false // Skip any clusters with enemy stations.
                    && p.GetObjectID() != GetHomeCluster().GetObjectID() // Skip our home cluster.
                    && DoesClusterConatainTechrockForStationType(stationType, p) == true // Only includes clusters with our target tech rock.
                    )
                .OrderBy(p => new DijkstraPathFinder(ClientConnection.GetCore(), p, GetHomeCluster()).GetDistance(p, GetHomeCluster()));

            return availableTechClusters.FirstOrDefault();
        }

        private bool DoesClusterConatainTechrockForStationType(StationType stationType, IclusterIGCWrapper currentCluster)
        {
            switch (stationType)
            {
                case StationType.Expansion:
                    return GetClusterTechRock(currentCluster)?.GetName()[0] == 'U';

                case StationType.Supremacy:
                    return GetClusterTechRock(currentCluster)?.GetName()[0] == 'C';

                case StationType.Tactical:
                    return GetClusterTechRock(currentCluster)?.GetName()[0] == 'S';

                default:
                    throw new NotSupportedException(stationType.ToString());
            }

            return false;
        }


        private bool ClusterContainsStation(StationType stationType, IclusterIGCWrapper currentCluster)
        {
            switch (stationType)
            {
                // If the tech rock is gone, then we'll consider this cluster to have this station in it
                case StationType.Expansion:
                case StationType.Supremacy:
                case StationType.Tactical:
                    return GetClusterTechRock(currentCluster) == null;

                case StationType.Garrison:
                    return currentCluster.GetStations().Exists(p => p.GetSide().GetObjectID() == ClientConnection.GetSide().GetObjectID() && (p.GetName().Contains("Garrison") == true || p.GetName().Contains("Starbase") == true));

                case StationType.ShipYard:
                    return currentCluster.GetStations().Exists(p => p.GetSide().GetObjectID() == ClientConnection.GetSide().GetObjectID() && (p.GetName().Contains("Ship") == true || p.GetName().Contains("Dry") == true));

                case StationType.Outpost:
                    return currentCluster.GetStations().Exists(p => p.GetSide().GetObjectID() == ClientConnection.GetSide().GetObjectID() && (p.GetName().Contains("Outpost") == true || p.GetName().Contains("Garrison") == true || p.GetName().Contains("Starbase") == true));

                case StationType.Refinery:
                    return currentCluster.GetStations().Exists(p => p.GetSide().GetObjectID() == ClientConnection.GetSide().GetObjectID() && (p.GetName().Contains("Refinery") == true || p.GetName().Contains("Special") == true || p.GetName().Contains("Outpost") == true || p.GetName().Contains("Garrison") == true || p.GetName().Contains("Starbase") == true));

                case StationType.Teleport:
                    return currentCluster.GetStations().Exists(p => p.GetSide().GetObjectID() == ClientConnection.GetSide().GetObjectID() && p.GetName().Contains("Teleport") == true);

                default:
                    throw new NotSupportedException(stationType.ToString());
            }

        }

        private IasteroidIGCWrapper GetClusterTechRock(IclusterIGCWrapper cluster)
        {
            if (cluster == null)
                return null;

            return cluster.GetAsteroids().Where(p => p.GetName().StartsWith("He") == false && p.GetName().StartsWith("a") == false && String.IsNullOrWhiteSpace(p.GetName()) == false).FirstOrDefault();
        }

        private void QueueTechForAvailableMoney()
        {
            if (_primaryTechpath == null)
                _primaryTechpath = GetTechpath(true);

            if (_secondaryTechpath == null)
                _secondaryTechpath = GetTechpath(false);

            // If we lost our primary tech base, and there are still tech rocks available, then build a new primary tech con, and get ready to push it.
            if (_primaryTechpath != null 
                && IsTechAvailableForInvestment(_primaryTechpath.ToString()) 
                && ClientConnection.GetSide().GetStations().Where(p => p.GetName().Contains(_primaryTechpath.ToString())).Count() == 0 
                && ClientConnection.GetSide().GetShips().Where(p => p.GetName().Contains(_primaryTechpath.ToString())).Count() == 0)
            {
                Log($"Primary tech base ({_primaryTechpath.ToString()}) is destroyed, queueing up a new one.");
                QueueTech(_primaryTechpath.ToString());
                //return;
            }

            int currentMinerCount = 0;
            if (ClientConnection.GetSide().GetShips().Count > 0)
                currentMinerCount = ClientConnection.GetSide().GetShips().Where(p => p.GetName().Contains("Miner")).Count();

            if (IsTechAvailableForInvestment(".Miner") == true && currentMinerCount < ClientConnection.GetCore().GetMissionParams().GetMaxDrones())
            {
                QueueTech(".Miner");
                //return;
            }
            
            var maxOutpostCount = ClientConnection.GetCore().GetClusters().Count / 4;

            int currentOutpostConsInFlight = ClientConnection.GetSide().GetShips().Where(p => p.GetName().Contains("Outpost")).Count();
            int currentOutpostCount = ClientConnection.GetSide().GetStations().Where(p => p.GetName().Contains("Outpost")).Count();
            int totalOutposts = currentOutpostConsInFlight + currentOutpostCount;
            
            if (IsTechAvailableForInvestment("Outpost") == true 
                && totalOutposts < maxOutpostCount
                && (currentOutpostCount < 1 || GetTechbaseCount() > 0))
            {
                QueueTech("Outpost");
                //return;
            }

            var maxTeleporterCount = maxOutpostCount - 1;

            if (IsTechAvailableForInvestment("Teleport") == true && ClientConnection.GetSide().GetShips().Where(p => p.GetName().Contains("Teleport")).Count() + ClientConnection.GetSide().GetStations().Where(p => p.GetName().Contains("Outpost")).Count() < maxTeleporterCount)
            {
                QueueTech("Teleport");
                //return;
            }

            int currentCarrierCount = 0;
            if (ClientConnection.GetSide().GetShips().Count > 0)
                currentCarrierCount = ClientConnection.GetSide().GetShips().Where(p => p.GetName().Contains("Carrier")).Count();

            if (IsTechAvailableForInvestment(".Carrier") == true && currentCarrierCount == 0 && totalOutposts == 1)
            {
                QueueTech(".Carrier");
                //return;
            }

            var maxRefineryCount = ((int) (ClientConnection.GetCore().GetClusters().Count / 6)) - maxOutpostCount;
            if (maxRefineryCount < 0)
                maxRefineryCount = 1;

            if (IsTechAvailableForInvestment("Refinery") == true && ClientConnection.GetSide().GetShips().Where(p => p.GetName().Contains("Refinery")).Count() + ClientConnection.GetSide().GetStations().Where(p => p.GetName().Contains("Refinery")).Count() < maxRefineryCount)
            {
                QueueTech("Refinery");
                //return;
            }

            bool buildPhase1Techbase = GetOutpostCount() >= 1 && GetTeleportCount() >= 1 && GetTechbaseCount() == 0;
            bool buildPhase2Techbase = GetOutpostCount() >= 2 && GetTeleportCount() >= 1 && GetTechbaseCount() == 1 && _readyForSecondTechbase == true;

            if (IsTechbaseInQueue() == false && (buildPhase1Techbase == true || buildPhase2Techbase == true))
            {
                var nextTechBaseName = GetNextTechbaseName();

                if (buildPhase1Techbase == true)
                    _primaryTechpath = (TechpathType) Enum.Parse(typeof(TechpathType), nextTechBaseName);

                if (buildPhase2Techbase == true)
                    _secondaryTechpath = (TechpathType)Enum.Parse(typeof(TechpathType), nextTechBaseName);

                if (nextTechBaseName != null)
                {
                    Log($"Techbase Time! Next tech base: {nextTechBaseName},  GetOutpostCount(): {GetOutpostCount()}, GetTeleportCount(): {GetTeleportCount()}, GetTechbaseCount(): {GetTechbaseCount()}");
                    QueueTech(nextTechBaseName);
                    //return;
                }
            }

            if (_primaryTechpath != null)
            {
                QueueTechForTechpath(_primaryTechPaths[_primaryTechpath.GetValueOrDefault()]);

                //if (QueueTechForTechpath(_primaryTechPaths[_primaryTechpath.GetValueOrDefault()]) == true)
                //    return;
            }

            if (_secondaryTechpath != null)
            {
                QueueTechForTechpath(_secondaryTechPaths[_secondaryTechpath.GetValueOrDefault()]);

                //if (QueueTechForTechpath(_secondaryTechPaths[_secondaryTechpath.GetValueOrDefault()]) == true)
                //    return;
            }

            ProcessInvestmentQueue();
        }


        private bool QueueTechForTechpath(List<string> techPath)
        {
            foreach (var techname in techPath)
            {
                // Only trigger the secondary tech base build once.
                if (techname == SecondTechbaseMarker && _readyForSecondTechbase == true)
                    continue;
                
                if (IsTechWaitingToBeBuilt(techname) == true)
                {
                    QueueTech(techname);
                    //return true;
                }
            }

            return false;
        }

        // for debug testing, this will re-figure out the primary tech when the strategy is restarted.
        /// <summary>
        /// 
        /// </summary>
        /// <param name="primaryTechpath">If true, returns the first techbase in build order, if false, then the second techbase in build order.</param>
        /// <returns></returns>
        private TechpathType? GetTechpath(bool primaryTechpath)
        {
            // Can't have a secondary tech path until the primary techpath has been determined.
            if (_primaryTechpath == null && primaryTechpath == false)
                return null;

            var stations = ClientConnection.GetSide().GetStations().ToList();
            stations.Reverse();
            
            foreach (var station in stations)
            {
                if (station == null)
                    continue;

                if (primaryTechpath == false && _primaryTechpath != null && station.GetName().Contains(_primaryTechpath.ToString()) == true)
                    continue;

                if (station.GetName().Contains("Expansion") == true)
                    return TechpathType.Expansion;

                if (station.GetName().Contains("Supremacy") == true)
                    return TechpathType.Supremacy;

                if (station.GetName().Contains("Tactical") == true)
                    return TechpathType.Tactical;
            }

            return null;
        }

        private bool IsTechbaseInQueue()
        {
            // Do we have a tech base investment pending?
            if (_investmentList.Contains("Supremacy") || _investmentList.Contains("Expansion") || _investmentList.Contains("Tactical"))
                return true;

            // Do we have a tech base researching?
                if (IsTechInProgress("Supremacy") || IsTechInProgress("Expansion") || IsTechInProgress("Tactical"))
                return true;

            // Do we have a tech base constructor out?
            if (ClientConnection.GetSide().GetShips().Exists(p => p.GetName().Contains("Supremacy") || p.GetName().Contains("Expansion") || p.GetName().Contains("Tactical")) == true)
                return true;

            return false;
        }

        private string GetNextTechbaseName()
        {
            var homeRock = GetClusterTechRock(GetHomeCluster());
            
            var availableTechRocks = GetHomeCluster().GetWarps().Select(p => GetClusterTechRock(p.GetDestination().GetCluster())).ToList();

            if(homeRock != null)
                availableTechRocks.Add(homeRock);

            int urocks = 0;
            int crocks = 0;
            int srocks = 0;

            // We want a different type of tech for the second tech base, if possible. 
            foreach (var station in ClientConnection.GetSide().GetStations())
            {
                if (station.GetName().Contains("Expansion") == true)
                    urocks--;

                if (station.GetName().Contains("Supremacy") == true)
                    crocks--;
                
                if (station.GetName().Contains("Tactical") == true)
                    srocks--;
            }

            foreach (var rock in availableTechRocks)
            {
                // This can happen at the very start of a game.
                if (rock == null)
                    continue; 

                switch (rock.GetName()[0])
                {
                    case 'U':
                        urocks++;
                        break;

                    case 'C':
                        crocks++;
                        break;

                    case 'S':
                        srocks++;
                        break;
                }
            }

            // Debugging override...
            //if(IsTechAvailableForInvestment("Supremacy") == true)
            //    return "Supremacy";

            if (IsTechAvailableForInvestment("Expansion") == true && urocks >= crocks && urocks >= srocks)
                return "Expansion";

            if (IsTechAvailableForInvestment("Supremacy") == true &&  crocks >= srocks)
                return "Supremacy";

            if (IsTechAvailableForInvestment("Tactical") == true)
                return "Tactical";

            return null;
        }

        private int GetTeleportCount()
        {
            return ClientConnection.GetSide().GetStations().Where(p => p.GetName().Contains("Teleport")).Count();
        }

        private int GetOutpostCount()
        {
            return ClientConnection.GetSide().GetStations().Where(p => p.GetName().Contains("Outpost")).Count();
        }

        private int GetTechbaseCount()
        {
            return ClientConnection.GetSide().GetStations().Where(p => p.GetName().Contains("Tactical")).Count()
                + ClientConnection.GetSide().GetStations().Where(p => p.GetName().Contains("Expansion")).Count()
                + ClientConnection.GetSide().GetStations().Where(p => p.GetName().Contains("Supremacy")).Count()
                + ClientConnection.GetSide().GetStations().Where(p => p.GetName().Contains("Ship")).Count()
                + ClientConnection.GetSide().GetStations().Where(p => p.GetName().Contains("Dry")).Count();
        }

        private bool IsTechWaitingToBeBuilt(string techName)
        {
            var side = ClientConnection.GetSide();
            var buckets = side?.GetBuckets();

            if (side == null || buckets == null)
                return false;

            if (_investmentList.Contains(techName) == true)
                return false;

            bool isNonBucketMarker = techName == SecondTechbaseMarker;

            if (isNonBucketMarker == true && _secondaryTechpath == null)
                return true;
            
            // If it's in the tech tree and hasn't been started, then it's waiting to be built even if we can't buy it yet.
            return buckets.Exists(p => p.GetName().StartsWith(techName, StringComparison.InvariantCultureIgnoreCase) == true && p.GetCompleteF() == false);
        }

        private bool IsTechAvailableForInvestment(string techName)
        {
            var side = ClientConnection.GetSide();
            var buckets = side?.GetBuckets();

            if (side == null || buckets == null)
                return false;

            bool isNonBucketMarker = techName == SecondTechbaseMarker;

            // If it's not in the tech tree or we can buy it, or it's already been started, then we don't want to invest in it again.
            if (isNonBucketMarker == false && buckets.Exists(p => p.GetName().StartsWith(techName, StringComparison.InvariantCultureIgnoreCase) == true && side.CanBuy(p) == true && p.GetCompleteF() == false) == false)
                return false;

            if (IsTechInProgress(techName) == true)
                return false;

            if (_investmentList.Contains(techName) == true)
                return false;

            return true;
        }

        private bool IsTechInProgress(string techName)
        {
            var side = ClientConnection.GetSide();
            var buckets = side?.GetBuckets();

            if (side == null || buckets == null)
                return false;

            return (buckets.FirstOrDefault(p => p.GetName().StartsWith(techName, StringComparison.InvariantCultureIgnoreCase) == true)?.GetPercentComplete()).GetValueOrDefault(0) > 0;
        }

        private void ProcessInvestmentQueue()
        {
            var availableMoney = ClientConnection.GetMoney();

            foreach (var techName in _investmentList.ToArray())
            {
                var investResult = InvestInTech(techName, ref availableMoney);
                switch (investResult)
                {
                    case InvestResult.AlreadyInProgress:
                        _investmentList.Remove(techName);
                        break;

                    case InvestResult.InsufficientFunds:
                        return; // Don't process any other item in the investment queue until we can invest in the next tech in the list.

                    case InvestResult.InvalidTechName:
                        _investmentList.Remove(techName);
                        break;

                    case InvestResult.Succeeded:
                        _investmentList.Remove(techName);
                        break;

                    case InvestResult.NotAvailableYet:
                        //return; // Don't skip the item in the queue, wait for it to become available.
                        break; // If we have enough money for the not available yet tech, then keep going, maybe there is something else that is cheap that we can also invest in.

                    default:
                        throw new NotSupportedException(investResult.ToString());
                }

                // Out of money... no need to keep checking for more investments.
                if (availableMoney <= 0)
                    return;
            }
        }

        private void QueueTech(string techName)
        {
            QueueTech(techName, false);
        }

        private void QueueTech(string techName, bool topOfList)
        {
            if (_investmentList.Contains(techName) == false)
            {
                Log($"Queueing tech: {techName}, topOfList: {topOfList}");

                if (topOfList == true)
                    _investmentList.Insert(0, techName);
                else
                    _investmentList.Add(techName);
            }
            else
            {
                Log($"Tech is already in queue: {techName}");
            }
        }

        private InvestResult InvestInTech(string techName, ref int availableMoney)
        {
            var side = ClientConnection.GetSide();
            var buckets = side.GetBuckets();

            if (techName == SecondTechbaseMarker)
            {
                _readyForSecondTechbase = true;
                return InvestResult.Succeeded;
            }

            foreach (var bucket in buckets)
            {
                string bucketName = bucket.GetName();
                if (bucket.GetName().StartsWith(techName, StringComparison.InvariantCultureIgnoreCase) == true)
                {
                    
                       
                    if (side.CanBuy(bucket) == true)
                    {
                        if (bucket.GetCompleteF() == false && bucket.GetPrice() - bucket.GetMoney() > 0)
                        {
                            var money = ClientConnection.GetMoney();

                            if (money >= bucket.GetPrice() + AmountOfMoneyToKeepOnHand) // Leave a little cash to let players buy things.
                            {
                                Log($"Adding money to bucket {bucket.GetName()}. Bucket wants: {bucket.GetPrice()}, we have: {money}, reserving {AmountOfMoneyToKeepOnHand} for player purchases.");

                                // Remove the spent money from the available money so that we can keep investing in other stuff. 
                                availableMoney -= bucket.GetPrice() - bucket.GetMoney();

                                ClientConnection.AddMoneyToBucket(bucket, bucket.GetPrice());
                                return InvestResult.Succeeded;
                            }
                            else
                            {
                                Log($"Not enough money to buy {techName}, money: {money}, bucketPrice: {bucket.GetPrice()}");
                                //Log("Already building: " + bucket.GetPercentComplete());
                                return InvestResult.InsufficientFunds;
                            }
                        }
                        else
                        {
                            Log($"Bucket progress for {techName}: {bucket.GetPercentComplete()}%");
                            return InvestResult.AlreadyInProgress;
                        }
                    }
                    else
                    {
                        var buyable = bucket.GetBuyable();
                        if ((ObjectType)buyable.GetObjectType() == ObjectType.OT_stationType)
                        {
                            continue; // See if there is an advanced station in the bucket list.
                        }

                        // We can skip this tech if it's not available, but we want to set aside money so that when it becomes available we can have it.
                        availableMoney -= bucket.GetPrice() - bucket.GetMoney();

                        return InvestResult.NotAvailableYet;
                    }
                }
            }
            
            Log($"A bucket was not found for tech: {techName}, removing this tech from the queue.");
            return InvestResult.InvalidTechName;
        }

        private void UndockMinersAndStartMining()
        {
            var miners = ClientConnection.GetSide().GetShips().Where(p => p.GetName().StartsWith(".Miner"));
            var dockedMiners = miners.Where(p => (ShipState) p.GetPlayerInfo().LastSeenState() == ShipState.c_ssDocked);

            // Find any miners loafing around the base.
            foreach (var ship in dockedMiners)
            {
                var orderedClustersWithHe3 = ClientConnection.GetCore().GetClusters().Where(p => p.GetAsteroids()
                                                    .Where(r => r.GetName().StartsWith("He"))
                                                    .Sum(x => x.GetOre()) > 150)
                                                .OrderByDescending(p => p.GetAsteroids()
                                                    .Where(r => r.GetName().StartsWith("He"))
                                                    .Sum(x => x.GetOre()));
                
                var currentCluster = ClientConnection.GetCore().GetCluster(ship.GetPlayerInfo().LastSeenSector());
                var adjacentClusters = currentCluster.GetWarps().Select(p => p.GetDestination().GetCluster());

                var orderedNonEnemyClustersWithHe3 = orderedClustersWithHe3.Where(p => p.GetStations().Exists(r => r.GetSide().GetObjectID() != ClientConnection.GetSide().GetObjectID()) == false);

                var orderedNonEnemyAdjacentClustersWithHe3 = orderedNonEnemyClustersWithHe3.Where(p => adjacentClusters.Select(x => x.GetObjectID()).Contains(p.GetObjectID()) == true);

                var friendlyStations = ClientConnection.GetSide().GetStations().Where(p => p.GetSide().GetObjectID() == ClientConnection.GetSide().GetObjectID());
                var friendlyClustersWithoutEnemyStations = friendlyStations.Where(p => p.GetCluster().GetStations().Exists(r => r.GetSide().GetObjectID() != ClientConnection.GetSide().GetObjectID()) == false).Select(p => p.GetCluster());
                
                var orderedFriendlyStationClustersWithHe3 = orderedNonEnemyClustersWithHe3.Where(p => friendlyClustersWithoutEnemyStations.Select(x => x.GetObjectID()).Contains(p.GetObjectID()) == true);

                var orderedContestedStationClustersWithHe3 = orderedClustersWithHe3.Where(p => friendlyStations.Select(r => r.GetCluster()).Select(x => x.GetObjectID()).Contains(p.GetObjectID()) == true);
                
                IclusterIGCWrapper targetCluster = null;
                
                if (targetCluster == null && orderedNonEnemyAdjacentClustersWithHe3.Count() > 0)
                {
                    targetCluster = orderedNonEnemyAdjacentClustersWithHe3.First();
                    Log($"Kicking loafing miner {ship.GetName()} to non-enemy adjacent cluster {targetCluster.GetName()}");
                }

                if (targetCluster == null && orderedFriendlyStationClustersWithHe3.Count() > 0)
                {
                    targetCluster = orderedFriendlyStationClustersWithHe3.First();
                    Log($"Kicking loafing miner {ship.GetName()} to friendly station cluster {targetCluster.GetName()}");
                }

                if (targetCluster == null && orderedContestedStationClustersWithHe3.Count() > 0)
                {
                    // TODO: Request TeamDirectory for miner D.
                    targetCluster = orderedContestedStationClustersWithHe3.First();
                    Log($"Kicking loafing miner {ship.GetName()} to contested station cluster {targetCluster.GetName()}");
                }

                if (targetCluster == null && orderedClustersWithHe3.Count() > 0)
                {
                    // TODO: Request TeamDirector to bomb something!
                    // TODO: Request TeamDirectory for miner D.
                    targetCluster = orderedClustersWithHe3.First();
                    Log($"Kicking loafing miner {ship.GetName()} to enemy cluster {targetCluster.GetName()}.");
                }

                if (targetCluster != null)
                {
                    var buoy = ClientConnection.CreateBuoy((sbyte)BuoyType.c_buoyCluster, 0, 0, 0, targetCluster.GetObjectID(), true);
                    var command = ship.GetDefaultOrder(buoy);

                    ClientConnection.SendChat(ClientConnection.GetShip(), ChatTarget.CHAT_INDIVIDUAL, ship.GetObjectID(), 0, "", (sbyte)command, buoy.GetObjectType(), buoy.GetObjectID(), buoy, true);
                }
                
            }
        }
    }
}
