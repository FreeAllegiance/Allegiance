using AllegianceInterop;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using Wopr;
using Wopr.Constants;
using Wopr.Loadouts;

namespace Wopr.Strategies
{
    public class ScoutExploreMap : StrategyBase
    {
        private const int SweepHopCount = 10;    // The number of waypoint hops a scout will make when doing an orbit of a sector scanning for stuff.
                                                // Increasing this value makes the circle smoother, but it also slows down the ship!

        private int _currentSectorID = -1;
        private bool _isSweeping = false;
        private IclusterIGCWrapper _navigatingToCluster = null;

        private bool _waitingForLaunchToComplete = false;

        //private bool _sweepLeft = false;
        //private bool _sweepRight = false;
        //private double _currentAngleInRadians;
        //private double _startingAngleInRadians;
        //private double _totalRadiansTraversed;
        //private VectorWrapper _startingPoint;
        //private int _currentSweepHop = 0;

        public ScoutExploreMap() : base(Wopr.Constants.StrategyID.ScoutExploreMap, TimeSpan.MaxValue)
        {
        }

        public override void AttachMessages(MessageReceiver messageReceiver, string botAuthenticationGuid, string playerName, int sideIndex, bool isGameController, bool isCommander)
        {
            Log("AttachMessages");

            messageReceiver.FMD_S_SET_CLUSTER += MessageReceiver_FMD_S_SET_CLUSTER;
            messageReceiver.FMD_S_SINGLE_SHIP_UPDATE += MessageReceiver_FMD_S_SINGLE_SHIP_UPDATE;
            messageReceiver.FMD_S_SHIP_STATUS += MessageReceiver_FMD_S_SHIP_STATUS;
            messageReceiver.FMD_S_DOCKED += MessageReceiver_FMD_S_DOCKED;
            messageReceiver.FMD_CS_CHATMESSAGE += MessageReceiver_FMD_CS_CHATMESSAGE;
        }

        private void MessageReceiver_FMD_CS_CHATMESSAGE(ClientConnection client, AllegianceInterop.FMD_CS_CHATMESSAGE message)
        {
            var ship = ClientConnection.GetShip();
            
            if (message.Message.Equals("die") == true)
            {
                AllegianceInterop.FMD_C_SUICIDE suicide = new AllegianceInterop.FMD_C_SUICIDE();
                client.SendMessageServer(suicide);
            }
        }

        private void MessageReceiver_FMD_S_DOCKED(ClientConnection client, AllegianceInterop.FMD_S_DOCKED message)
        {
            Log("MessageReceiver_FMD_S_DOCKED");

            if (_waitingForLaunchToComplete == true)
            {
                Log("\tA launch is already in progress, skipping message.");
                return;
            }

            var ship = ClientConnection.GetShip();
            var station = ship.GetStation();

            // If we are in a station, then get into a scout and get out there.
            if (station != null)
            {
                // Mark the current sector as invalid so that we will aquire a new destination on launch.
                _currentSectorID = -1;

                Log("\tDocked at station, changing ship to scout and relaunching.");
                ChangeShip(ShipType.Scout, new ScoutProbeLoadout());
                return;
            }
        }


        private void MessageReceiver_FMD_S_SHIP_STATUS(ClientConnection client, AllegianceInterop.FMD_S_SHIP_STATUS message)
        {
            
            var ship = ClientConnection.GetShip();
            var station = ship.GetStation();

            // Ignore messages that are not for us.
            if (message.shipID != ship.GetObjectID())
                return;

            // Ignore messages until we actually get into a sector
            if (ship.GetCluster() == null)
                return;

            Log($"MessageReceiver_FMD_S_SHIP_STATUS: my ship id = {ship.GetObjectID()},  message.shipID = {message.shipID}, message.status.GetSectorID(): {message.status.GetSectorID()}");

           
            string shipName = ship.GetName();

            //UpdateUnexploredWarpsList();

            // If we are in a station, then get into a scout and get out there.
            if (station != null)
            {
                if (_waitingForLaunchToComplete == false)
                {
                    Log("\tI'm sitting in base? Time to launch a scout!");
                    ChangeShip(ShipType.Scout, new ScoutProbeLoadout());
                    return;
                }
            }
            else
            {
                _waitingForLaunchToComplete = false;
            }

            if (IsCurrentShipAScout() == false)
            {
                Log("\tI'm not in a scout... returning to base.");
                ReturnToBase();
                return;
            }

            // If we don't have any command and the ship is stopped, reset and get moving!
            if ((CommandID)ship.GetCommandID((sbyte)CommandType.c_cmdCurrent) <= CommandID.c_cidNone && ship.GetVelocity().Length() == 0)
            {
                Log($"\tWe're stopped with no command. Resetting current variables, and selecting a new destination. _currentSectorID: {_currentSectorID}, _isSweeping: {_isSweeping}, _navigatingToCluster: {_navigatingToCluster}, _waitingForLaunchToComplete: {_waitingForLaunchToComplete}");

                _currentSectorID = -1;
                _isSweeping = false;
                _navigatingToCluster = null;
                _waitingForLaunchToComplete = false;
                
                //return;
            }

            // If we are already moving, and this status message is for our current sector, or we are already sweeping, then ignore it.
            if (ship.GetVelocity().Length() > 0 && (message.status.GetSectorID() == _currentSectorID || _isSweeping == true))
            {
                Log($"\tSkipping message: ship.GetVelocity(): {ship.GetVelocity().Length()},  ship.GetCluster() = {ship.GetCluster()?.GetName()}, message.status.GetSectorID() = {message.status.GetSectorID()}, _currentSectorID = {_currentSectorID}, _isSweeping = {_isSweeping}");
                return;
            }

            // These clusters still need to be explored.
            var currentUnexploredClusters = GameInfo.GetUnexploredClusters(client.GetCore());

            // Are we on the way to another cluster to explore?
            if (_navigatingToCluster != null)
            {
                // If we are not at the destination yet...
                if (_navigatingToCluster.GetObjectID() != ship.GetCluster().GetObjectID())
                {
                    // And our destination cluster still hasn't been explored...
                    if (currentUnexploredClusters.ContainsKey(_navigatingToCluster.GetObjectID()) == true)
                    {
                        Log($"\tEntered cluster: {ship.GetCluster().GetName()}, continuing on route to {_navigatingToCluster.GetName()}");
                        return;
                    }
                    else // The detination was explored by someone else, let's find a new destination.
                    {
                        Log($"\tArrived at cluster: {ship.GetCluster().GetName()}, but destination cluster {_navigatingToCluster.GetName()} has already been swept. Cancelling navigation, checking for next step.");
                        _navigatingToCluster = null;
                    }
                }
                else // We've arrived, let's see what we need to do next.
                {
                    Log($"\tArrived at cluster: {ship.GetCluster().GetName()}, checking for next step.");
                    _navigatingToCluster = null;
                }
            }


            _currentSectorID = message.status.GetSectorID();

            var otherFriendlyScoutShips = ship.GetCluster().GetShips().Where(p => p.GetObjectID() != ship.GetObjectID() && p.GetBaseHullType().GetName().Contains("Scout") == true && p.GetSide().GetObjectID() == ship.GetSide().GetObjectID());
            //var otherSweepingScoutShips = otherFriendlyScoutShips.Where(p => p.GetCommandTarget((sbyte)CommandType.c_cmdCurrent)?.GetObjectType() == (short)ObjectType.OT_buoy);
            var sweepingScoutCount = GameInfo.GetSweepingScoutCount(ship.GetCluster());

            Log($"\totherFriendlyScoutShips: {otherFriendlyScoutShips.Count()}, otherSweepingScoutShips: {sweepingScoutCount}, is unexplored: {currentUnexploredClusters.ContainsKey(ship.GetCluster().GetObjectID())} ");

            // Ship has entered a cluster, find the next target and start scouting.

            // If cluster is unexplored, and less than 2 ships sweeping, start sweeping
            if (currentUnexploredClusters.ContainsKey(ship.GetCluster().GetObjectID()) == true && sweepingScoutCount < 2)
                SweepCluster();

            // If there are two ships sweeping, fly to a point 1/3 of the angle we entered in at, and wait for alephs to appear.
            else if (sweepingScoutCount >= 2)
                FlyToStationPointAndWaitForAelphs();

            // If there are no ships sweeping, go to the next system. 
            else
                SelectNextAelphTarget();




            //var mission = ship.GetMission();

            //var warps = mission.GetWarps();

            //// Get all the clusters that are available in the mission.
            //var missionClusters = mission.GetClusters();

            //var currentShipCluster = ship.GetCluster();
            //var currentMissionCluster = missionClusters.FirstOrDefault(p => p.GetObjectID() == currentShipCluster.GetObjectID());

            //var shipVisibleWarps = currentShipCluster.GetWarps();
            //var allWarpsInCluster = currentMissionCluster.GetWarps();







        }

        //private void UpdateUnexploredWarpsList()
        //{
        //    Log("\tUpdateUnexploredWarpsList()");

        //    //var ship = _client.GetShip();
        //    var unexploredWarps = ClientConnection.GetCore().GetWarps().Where(p => p.GetDestination().GetCluster().GetAsteroids().Count == 0 && GameInfo.UnexploredClustersByObjectID.ContainsKey(p.GetDestination().GetCluster().GetObjectID()) == false);

        //    foreach (var unexploredWarp in unexploredWarps)
        //    {
        //        Log($"\t\tFound unexplored cluster: {unexploredWarp.GetDestination().GetCluster().GetName()}, adding cluster's id to the GameInfo.UnexploredClustersByObjectID list.");

        //        lock (GameInfo.UnexploredClustersByObjectID)
        //        {
        //            if(GameInfo.UnexploredClustersByObjectID.ContainsKey(unexploredWarp.GetDestination().GetCluster().GetObjectID()) == false)
        //                GameInfo.UnexploredClustersByObjectID.Add(unexploredWarp.GetDestination().GetCluster().GetObjectID(), unexploredWarp.GetDestination().GetCluster().GetName());
        //        }
        //    }

        //    StringBuilder s = new StringBuilder();
        //    foreach (var unexploredClusterName in GameInfo.UnexploredClustersByObjectID.Values)
        //        s.Append(unexploredClusterName + " ");

        //    Log("\t\tCurrent unexplored clusters: " + s.ToString());
        //}

        private void FlyToStationPointAndWaitForAelphs()
        {
            Log("\tFlyToStationPointAndWaitForAelphs");

            var ship = ClientConnection.GetShip();
            var centerPoint = new VectorWrapper(0, 0, 0);
            var shipPosition = ship.GetPosition();

            var startingPoint = shipPosition;
            var startingAngleInRadians = shipPosition.AngleInRadians(centerPoint);

            // 1/3 of 360 degrees in radians.
            double oneThirdAngle = Math.PI * 2 / 3;

            // Move to a new point 1/3 or 2/3 of the total angle and the same distance as the entry aleph.
            double newAngle = startingAngleInRadians + (oneThirdAngle * _random.Next(1, 3));

            VectorWrapper nextPoint = new VectorWrapper((startingPoint - centerPoint).Length() * (float)Math.Cos(newAngle), (startingPoint - centerPoint).Length() * (float)Math.Sin(newAngle), 0);

            var buoy = ClientConnection.CreateBuoy((sbyte)BuoyType.c_buoyWaypoint, nextPoint.X(), nextPoint.Y(), nextPoint.Z(), ship.GetCluster().GetObjectID(), true);
            var command = ship.GetDefaultOrder(buoy);
            ship.SetCommand((sbyte)CommandType.c_cmdCurrent, buoy, command);
            ship.SetAutopilot(true);

            //List<int> consideredWarpObjectIDs = new List<int>();

            // Wait for Aelphs to appear. 
            Task.Run(() =>
            {
                try
                {
                    Interlocked.Increment(ref _runningTasks);

                    var currentUnexploredClusters = GameInfo.GetUnexploredClusters(ClientConnection.GetCore());

                    bool foundNewWarp = false;
                    bool updatedUnexploredClusterListAfterMove = false;
                    for (int i = 0; i < 120 * 100 && _cancellationTokenSource.IsCancellationRequested == false && foundNewWarp == false; i++)
                    {
                        var otherScoutShips = ship.GetCluster().GetShips().Where(p => p.GetObjectID() != ship.GetObjectID() && p.GetBaseHullType().GetName().Contains("Scout") == true);
                        //var otherSweepingScoutShips = otherScoutShips.Where(p => p.GetCommandTarget((sbyte)CommandType.c_cmdCurrent)?.GetObjectType() == (short)ObjectType.OT_buoy);
                        int sweepingScoutCount = GameInfo.GetSweepingScoutCount(ship.GetCluster());

                        if (i % 500 == 0)
                        {
                            Log($"\t\tWaiting for Alephs to be found: {i}.");

                            // Once we get within 400 of the waypoint, update the unexplored clusters list in case we happen to have found a new unexplored cluster. 
                            if (updatedUnexploredClusterListAfterMove == false && (nextPoint - ship.GetPosition()).LengthSquared() <  Math.Pow(400, 2))
                            {
                                Log("\t\tUpdating unexplored cluster list after ship move.");
                                currentUnexploredClusters = GameInfo.GetUnexploredClusters(ClientConnection.GetCore());
                                updatedUnexploredClusterListAfterMove = true;
                            }
                        }

                        var newWarps = ship.GetCluster().GetWarps().Where(p => currentUnexploredClusters.ContainsKey((p?.GetDestination()?.GetCluster()?.GetObjectID()).GetValueOrDefault(-1)) == true /*&& consideredWarpObjectIDs.Contains(p.GetObjectID()) == false*/);
                        if (newWarps.Count() > 0)
                        {
                            foreach (var newWarp in newWarps)
                            {
                                // If there are other non-sweeping ships in the sector, then only 33% of those ships should go to the newly visible aelph.
                                if (otherScoutShips.Count() - sweepingScoutCount > 0)
                                {
                                    if (_random.Next(0, 100) < 33)
                                    {
                                        Log($"\t\tWarp found, 33% chance success, heading to {newWarp.GetName()}");

                                        ship.SetCommand((sbyte)CommandType.c_cmdCurrent, newWarp, (sbyte)CommandID.c_cidGoto);
                                        ship.SetAutopilot(true);
                                        foundNewWarp = true;
                                        break;
                                    }
                                }
                                else // We are the only non-sweeping scout in the cluster, go to the new warp.
                                {
                                    Log($"\t\tWarp found, heading to {newWarp.GetName()}");

                                    ship.SetCommand((sbyte)CommandType.c_cmdCurrent, newWarp, (sbyte)CommandID.c_cidGoto);
                                    ship.SetAutopilot(true);
                                    foundNewWarp = true;
                                    break;
                                }
                            }
                        }

                        // If the cluster gets marked as explored and no new unexplored alephs were found, then just move on. 
                        if (currentUnexploredClusters.ContainsKey(ship.GetCluster().GetObjectID()) == false)
                        {
                            Log("\t\tCluster marked as explored, moving on.");
                            break;
                        }

                        Thread.Sleep(100);
                    }

                    if (foundNewWarp == false)
                    {
                        Log("\t\tNo aelph was found, heading to next available warp.");
                        SelectNextAelphTarget();
                    }
                }
                finally
                {
                    Interlocked.Decrement(ref _runningTasks);
                }
            });
        }

        private void SweepCluster()
        {
            Log("\tSweepCluster()");
            
            _isSweeping = true;

            var ship = ClientConnection.GetShip();
            var otherScoutShips = ship.GetCluster().GetShips().Where(p => p.GetObjectID() != ship.GetObjectID() && p.GetBaseHullType().GetName().Contains("Scout") == true);
            
            var centerPoint = new VectorWrapper(0, 0, 0);
            var shipPosition = ship.GetPosition();

            var startingPoint = shipPosition;
            var startingAngleInRadians = shipPosition.AngleInRadians(centerPoint);

            double nextSliceWidth = (Math.PI * 2 / SweepHopCount);
            double currentAngleInRadians = startingAngleInRadians;

            Task.Run(() =>
            {
                try
                {
                    int currentSweepingScoutCount = GameInfo.IncrementSweepingScoutCount(ship.GetCluster());

                    //bool sweepLeft = currentSweepingScoutCount > 0;

                    Interlocked.Increment(ref _runningTasks);

                    bool sweepComplete = true;

                    int hopCount = SweepHopCount;
                    bool hopCountReduced = false; // Changes to true when a second scout starts sweeping.
                    bool firstSweepingScout = currentSweepingScoutCount == 1;

                    // This is somewhat specific to hihigher, but we will assume that any cluster has an average of 3 warps and a station cluster has 4. 
                    // This lets a scout stop sweeping early if all clusters are assumed to be found.
                    int targetWarpCount = 3 + ship.GetCluster().GetStations().Count;

                    for (int i = 1; 
                            i < hopCount - 2  // Skip the first hop and the last 2 hops, aelphs won't be that close together.
                            && _cancellationTokenSource.IsCancellationRequested == false 
                            && ship.GetCluster().GetWarps().Count < targetWarpCount;  // If we find enough warps, we will consider this cluster scanned. If there are more warp clusters will be covered when the other cluster is discovered from the other side. Otherwise, it's up to the humans!
                         i++)
                    {
                        // If another scout starts sweeping, then reduce the hop count by half so that we get done quicker. The other scout will start sweeping from the other direction.
                        if (hopCountReduced == false && GameInfo.GetSweepingScoutCount(ship.GetCluster()) >= 2)
                        {
                            Log($"\t\tA second scout is also sweeping, reducing the hop count to half.");
                                hopCount = (SweepHopCount / 2) + 3; // Go a little farther so that the paths overlap. It's the classic pincer manuver!!
                        }

                        if (firstSweepingScout == true)
                            currentAngleInRadians += nextSliceWidth;
                        else
                            currentAngleInRadians -= nextSliceWidth;

                        VectorWrapper nextPoint = new VectorWrapper((startingPoint - centerPoint).Length() * (float)Math.Cos(currentAngleInRadians), (startingPoint - centerPoint).Length() * (float)Math.Sin(currentAngleInRadians), 0);

                        Log($"\t\tnextPoint: {nextPoint.GetString()}, currentAngleInRadians: {currentAngleInRadians}, sweepHop: {i}, firstSweepingScout: {firstSweepingScout}, target hopCount: {hopCount - 2}");

                        var buoy = ClientConnection.CreateBuoy((sbyte)BuoyType.c_buoyWaypoint, nextPoint.X(), nextPoint.Y(), nextPoint.Z(), ship.GetCluster().GetObjectID(), true);
                        var command = ship.GetDefaultOrder(buoy);
                        ship.SetCommand((sbyte)CommandType.c_cmdCurrent, buoy, command);
                        ship.SetAutopilot(true);

                        bool reachedWaypoint = false;
                        for (int j = 0; j < 60 * 100 && _cancellationTokenSource.IsCancellationRequested == false; j++)
                        {
                            VectorWrapper difference = nextPoint - ship.GetPosition();

                            if(j % 500 == 0)
                                Log($"\t\tCurrent distance: {Math.Abs(difference.Length())}, iterations: {j}, {nextPoint.GetString()}, ship velocity: {ship.GetVelocity().Length()}");

                            if ((nextPoint - ship.GetPosition()).LengthSquared() < Math.Pow(600, 2))
                            {
                                Log($"\t\tReached waypoint: {nextPoint.GetString()}, ship velocity: {ship.GetVelocity().Length()}"); 
                                reachedWaypoint = true;
                                break;
                            }

                            Thread.Sleep(100);
                        }

                        // We didn't reach the waypoint in one minute. Something went wrong, let's just move on.
                        if (reachedWaypoint == false)
                        {
                            sweepComplete = false;
                            Log("\t\tNo waypoint was found in 120 seconds.");
                            break;
                        }
                    }

                    if (sweepComplete == true)
                    {
                        currentSweepingScoutCount = GameInfo.GetSweepingScoutCount(ship.GetCluster());

                        // If there is more than one sweeping scout, then the second scout will perform the removal of the scanned cluster.
                        if (firstSweepingScout == false || currentSweepingScoutCount == 1)
                        {
                            var currentUnexploredClusters = GameInfo.GetUnexploredClusters(ClientConnection.GetCore());

                            if (currentUnexploredClusters.ContainsKey(ship.GetCluster().GetObjectID()) == true)
                            {
                                Log("Initial sweep complete, but we didn't find everything. TODO: take a closer look?");
                                DoTargetedSweep(ship);
                            }
                            else
                            {
                                Log($"\t\tCluster exploration complete for {ship.GetCluster().GetName()}, this cluster is no longer in the unexplored list.");
                                //GameInfo.UnexploredClustersByObjectID.Remove(ship.GetCluster().GetObjectID());
                                //UpdateUnexploredWarpsList();
                            }
                        }
                        else
                        {
                            Log($"\t\tCluster exploration complete for {ship.GetCluster().GetName()} for first sweeping scout.");
                        }
                    }

                    SelectNextAelphTarget();
                }
                finally
                {
                    Interlocked.Decrement(ref _runningTasks);
                    _isSweeping = false;
                    GameInfo.DecrementSweepingScoutCount(ship.GetCluster());
                }
            });

            //_startingPoint = shipPosition;
            //_startingAngleInRadians = shipPosition.AngleInRadians(centerPoint);




            //if (_startingAngleInRadians < 0)
            //    _startingAngleInRadians = _startingAngleInRadians + 2 * Math.PI;

            //_currentAngleInRadians = _startingAngleInRadians;

            //// If there are no scouts, sweep left, otherwise if there is already a scount, then sweep right, otherwise, locate a new aleph and go there!
            //if (otherScoutShips.Count() == 0)
            //{
            //    _sweepLeft = true;
            //    NavigateToNextSweepPoint();
            //}
            //else if (otherScoutShips.Count() == 1)
            //{
            //    _sweepRight = true;
            //    NavigateToNextSweepPoint();
            //}
            //else
            //{
            //    _sweepRight = false;
            //    _sweepLeft = false;
            //    SelectNextAelphTarget();
            //}
        }

        private void DoTargetedSweep(IshipIGCWrapper ship)
        {
            Log("DoTargetedSweep(): Find the missing aleph using triangulation on the other alephs.");
        }

        //private void NavigateToNextSweepPoint()
        //{
        //    var ship = _client.GetShip();

        //    double nextSliceWidth = (Math.PI * 2 / 16);
        //    double nextSlice = nextSliceWidth;

        //    if (_sweepLeft == true)
        //    {
        //        nextSlice = _currentAngleInRadians + nextSliceWidth;
        //        if (nextSlice > Math.PI * 2)
        //            nextSlice -= Math.PI * 2;
        //    }
        //    else
        //    {
        //        nextSlice = _currentAngleInRadians - nextSliceWidth;
        //        if (nextSlice < 0)
        //            nextSlice += Math.PI * 2;
        //    }

        //    // Don't travel the last slice.
        //    if (_totalRadiansTraversed + nextSliceWidth >= Math.PI * 2)
        //        SelectNextAelphTarget();

        //    _totalRadiansTraversed += nextSliceWidth;

        //    var centerPoint = new VectorWrapper(0, 0, 0);
        //    VectorWrapper nextPoint = new VectorWrapper((_startingPoint - centerPoint).Length() * (float) Math.Cos(nextSlice), (_startingPoint - centerPoint).Length() * (float) Math.Sin(nextSlice), 0);

        //    var buoy = _client.CreateBuoy((sbyte)BuoyType.c_buoyWaypoint, nextPoint.X(), nextPoint.Y(), nextPoint.Z(), ship.GetCluster().GetObjectID(), true);
        //    var command = ship.GetDefaultOrder(buoy);
        //    ship.SetCommand((sbyte)CommandType.c_cmdCurrent, buoy, command);
        //    ship.SetAutopilot(true);
        //}

        private void SelectNextAelphTarget()
        {
            Log("\tSelectNextAelphTarget()");

            var ship = ClientConnection.GetShip();
            
            var visibleWarps = ship.GetCluster().GetWarps();
            var otherFriendlyScoutShips = ship.GetCluster().GetShips().Where(p => p.GetObjectID() != ship.GetObjectID() && p.GetBaseHullType().GetName().Contains("Scout") == true && p.GetSide().GetObjectID() == ship.GetSide().GetObjectID());

            bool foundTargetWarp = false;

            var currentUnexploredClusters = GameInfo.GetUnexploredClusters(ClientConnection.GetCore());

            // When launching from home, just pick a target at random so that all clusters get some love.
            if (ship.GetCluster().GetHomeSector() == false)
            {

                foreach (var visibleWarp in visibleWarps.Where(p => currentUnexploredClusters.ContainsKey(p.GetDestination().GetCluster().GetObjectID()) == true).OrderBy(p => ClientConnection.GetDistanceSquared(p, ship)))
                {
                    if (currentUnexploredClusters.ContainsKey(visibleWarp.GetDestination().GetCluster().GetObjectID()) == true)
                    {
                        Log($"\t\tUnexplored Warp found: {visibleWarp.GetName()}");

                        float myDistance = ClientConnection.GetDistanceSquared(ship, visibleWarp);
                        bool isAnotherScoutCloser = false;
                        foreach (var otherScoutShip in otherFriendlyScoutShips)
                        {
                            float otherScoutDistance = ClientConnection.GetDistanceSquared(otherScoutShip, visibleWarp);
                            if (otherScoutDistance < myDistance)
                            {
                                Log($"\t\tAnother scout: {otherScoutShip.GetName()} is already closer to: {visibleWarp.GetName()}, looking for another target.");
                                isAnotherScoutCloser = true;
                                break;
                            }
                        }

                        // The cluster is on the unexplored list, and there's no other scout that is closer, let's go for it!
                        if (isAnotherScoutCloser == false)
                        {
                            Log($"\t\tFound target warp, going to {visibleWarp.GetName()}");

                            foundTargetWarp = true;
                            ship.SetCommand((sbyte)CommandType.c_cmdCurrent, visibleWarp, (sbyte)CommandID.c_cidGoto);
                            ship.SetAutopilot(true);
                            break;
                        }
                    }
                }
            }

            if (foundTargetWarp == false)
            {
                Log($"\t\tNo target warp found, selecting a visible warp at random.");

                currentUnexploredClusters = GameInfo.GetUnexploredClusters(ClientConnection.GetCore());

                // Pick a random warp that is unexplored. 
                var unvisitedWarps = visibleWarps.Where(p => currentUnexploredClusters.ContainsKey(p.GetDestination().GetCluster().GetObjectID()) == true).ToList();
                if (unvisitedWarps.Count > 0)
                {
                    var targetWarp = unvisitedWarps[_random.Next(0, unvisitedWarps.Count)];

                    Log($"\t\tFound unvisited random warp, going to: {targetWarp.GetName()}");

                    ship.SetCommand((sbyte)CommandType.c_cmdCurrent, targetWarp, (sbyte)CommandID.c_cidGoto);
                    ship.SetAutopilot(true);
                    foundTargetWarp = true;
                }
            }

            // Couldn't find any immediately linked clusters, so find the nearest unexplored cluster and go to it.
            if (foundTargetWarp == false)
            {
                IclusterIGCWrapper nearestCluster = FindNearestUnexploredCluster();
                if (nearestCluster != null)
                {
                    Log($"\t\tFound an unexplored cluster {nearestCluster.GetName()}, navigating to it.");

                    var buoy = ClientConnection.CreateBuoy((sbyte)BuoyType.c_buoyWaypoint, 0, 0, 0, nearestCluster.GetObjectID(), true);
                    var command = ship.GetDefaultOrder(buoy);
                    ship.SetCommand((sbyte)CommandType.c_cmdCurrent, buoy, command);
                    ship.SetAutopilot(true);

                    _navigatingToCluster = nearestCluster;

                    foundTargetWarp = true;
                }
            }

            // All the immediate warps have been visited or have other scouts closer, just pick one at random.
            if (foundTargetWarp == false)
            {
                var targetWarp = visibleWarps[_random.Next(0, visibleWarps.Count)];

                Log($"\t\tFound visited random warp, going to: {targetWarp.GetName()}");

                ship.SetCommand((sbyte)CommandType.c_cmdCurrent, targetWarp, (sbyte)CommandID.c_cidGoto);
                ship.SetAutopilot(true);
                foundTargetWarp = true;
            }
        }

        private IclusterIGCWrapper FindNearestUnexploredCluster()
        {
            IclusterIGCWrapper nearestCluster = null;
            int nearestClusterDistance = int.MaxValue;

            var currentUnexploredClusters = GameInfo.GetUnexploredClusters(ClientConnection.GetCore());

            foreach (var unexploredClusterObjectID in currentUnexploredClusters.Keys)
            {
                var fromCluster = ClientConnection.GetShip().GetCluster();
                var toCluster = ClientConnection.GetCore().GetCluster((short)unexploredClusterObjectID);

                DijkstraPathFinder pathFinder = new DijkstraPathFinder(ClientConnection.GetCore(), fromCluster, toCluster);
                
                int distance = pathFinder.GetDistance(fromCluster, toCluster);

                if (distance < nearestClusterDistance)
                    nearestCluster = toCluster;
            }

            return nearestCluster;
        }
        

        private void MessageReceiver_FMD_S_SINGLE_SHIP_UPDATE(ClientConnection client, AllegianceInterop.FMD_S_SINGLE_SHIP_UPDATE message)
        {
            
        }

        private void MessageReceiver_FMD_S_SET_CLUSTER(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_S_SET_CLUSTER message)
        {
            
        }

        public override void Start()
        {
            var ship = ClientConnection.GetShip();
            //var side = _client.GetSide();
            //var mission = side.GetMission();
            //var hullTypes = mission.GetHullTypes();

            //var station = ship.GetStation();

            // Wait for the ship to join a cluster or a station.
            //if (ship == null || (ship.GetCluster() == null && ship.GetStation() == null))
            //{
            //    Log("Start(): Waiting 30 seconds for ship to join station or cluster.");
            //    Task.Run(() =>
            //        {
            //            for (int i = 0; i < 30 * 100; i++)
            //            {
            //                ship = ClientConnection.GetShip();
            //                if (ship?.GetCluster() != null || ship?.GetStation() != null)
            //                    break;

            //                Thread.Sleep(100);
            //            }

            //            if (ship.GetStation() != null)
            //            {
            //                Log("\tStart(): We are in a station, changing ship to scout and launching.");
            //                ChangeShip(ShipType.Scout, new ScoutProbeLoadout());
            //            }
            //            else if (IsCurrentShipAScout() == false)
            //            {
            //                Log("\tStart(): We are not in a scout, returing to the nearest station.");
            //                ReturnToBase();
            //            }
            //        }
            //    );
            //}

            if (ship.GetStation() != null)
            {
                Log("\tStart(): We are in a station, changing ship to scout and launching.");
                ChangeShip(ShipType.Scout, new ScoutProbeLoadout());
            }
            else if (IsCurrentShipAScout() == false)
            {
                Log("\tStart(): We are not in a scout, returing to the nearest station.");
                ReturnToBase();
            }

        }

        private void ScoutSector()
        {
            var ship = ClientConnection.GetShip();

            // If the ship is in a station, launch.
            if (ship.GetStation() != null)
            {
                // TODO: Transfer to desired station first.

                // Launch the ship
                //_client.BuyLoadout(ship, true);

                // Wait for the ship to enter the cluster.
                while (ship.GetCluster() == null)
                    Thread.Sleep(100);
            }

            var mission = ship.GetMission();

            // Get all the clusters that are available in the mission.
            var missionClusters = mission.GetClusters();

            var currentShipCluster = ship.GetCluster();
            var currentMissionCluster = missionClusters.FirstOrDefault(p => p.GetObjectID() == currentShipCluster.GetObjectID());

            var shipVisibleWarps = currentShipCluster.GetWarps();
            var allWarpsInCluster = currentMissionCluster.GetWarps();
        }

        private void ReturnToBase()
        {
            var ship = ClientConnection.GetShip();
            var targetBase = ClientConnection.FindTarget(ship, (int) (TargetType.c_ttStation | TargetType.c_ttFriendly | TargetType.c_ttAnyCluster), (short) StationAbilityBitMask.c_sabmFlag);

            ship.SetCommand((sbyte)CommandType.c_cmdCurrent, targetBase, (sbyte)CommandID.c_cidGoto);
            ship.SetAutopilot(true);
        }

        private bool IsCurrentShipAScout()
        {
            var ship = ClientConnection.GetShip();
            var baseHullType = ship.GetBaseHullType();

            return baseHullType.GetName().Contains("Scout");
        }

        private void ChangeShip(ShipType shipType, LoadoutBase loadOut)
        {
            _waitingForLaunchToComplete = true;

            Log($"\tChanging ship to {shipType}");

            var side = ClientConnection.GetSide();
            var ship = ClientConnection.GetShip();
            var mission = side.GetMission();
            var hullTypes = mission.GetHullTypes();
            var station = ship.GetStation();

            if (station == null)
                return;

            string targetHullTypeName = GetHullTypeString(shipType);
            
            List<AllegianceInterop.IhullTypeIGCWrapper> buyableHullTypes = new List<AllegianceInterop.IhullTypeIGCWrapper>();

            foreach (var hullType in hullTypes)
            {
                if (hullType.GetGroupID() >= 0 && station.CanBuy(hullType) == true && station.IsObsolete(hullType) == false)
                {
                    Log("\t\tbuyable hullType: " + hullType.GetName());
                    buyableHullTypes.Add(hullType);
                }
            }
            
            // Get a scout.
            var scoutHull = buyableHullTypes.FirstOrDefault(p => p.GetName().Contains(targetHullTypeName));
            if (scoutHull == null)
                return;

            Log($"\tFound Scout: {scoutHull.GetName()}");

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
                        Log($"\t\tFound part: {partType.GetName()}, capacity for part: {scoutHull.GetCapacity(partType.GetEquipmentType())}");

                    }
                }
            }

            // Sell all the parts
            //var emptyShip = _client.CreateEmptyShip();
            //emptyShip.SetBaseHullType(scoutHull);
            //_client.BuyLoadout(emptyShip, false);

            //emptyShip.Terminate();

            // Now get the new empty ship from the client.
            //ship = _client.GetShip();

            foreach (var part in ship.GetParts())
                ship.DeletePart(part);

            // Change the ship to the scout hull. 
            ship.SetBaseHullType(scoutHull);
            ClientConnection.BuyDefaultLoadout(ship, station, scoutHull, ClientConnection.GetMoney());

            ClearShipCargo(ship);


            // Load Weapons, missiles, dispensers, etc.
            if (loadOut.Weapon1 != null)
            {
                var part = buyablePartTypes.FirstOrDefault(p => p.GetName().Contains(loadOut.GetItemTypeString(loadOut.Weapon1.Value)) == true);
                if(part != null)
                    ClientConnection.BuyPartOnBudget(ship, part, 0, ClientConnection.GetMoney());
            }

            if (loadOut.Weapon2 != null)
            {
                var part = buyablePartTypes.FirstOrDefault(p => p.GetName().Contains(loadOut.GetItemTypeString(loadOut.Weapon2.Value)) == true);
                if (part != null)
                    ClientConnection.BuyPartOnBudget(ship, part, 1, ClientConnection.GetMoney());
            }

            if (loadOut.Weapon3 != null)
            {
                var part = buyablePartTypes.FirstOrDefault(p => p.GetName().Contains(loadOut.GetItemTypeString(loadOut.Weapon3.Value)) == true);
                if (part != null)
                    ClientConnection.BuyPartOnBudget(ship, part, 2, ClientConnection.GetMoney());
            }

            if (loadOut.Weapon4 != null)
            {
                var part = buyablePartTypes.FirstOrDefault(p => p.GetName().Contains(loadOut.GetItemTypeString(loadOut.Weapon4.Value)) == true);
                if (part != null)
                    ClientConnection.BuyPartOnBudget(ship, part, 3, ClientConnection.GetMoney());
            }

            if (loadOut.Turret1 != null)
            {
                var part = buyablePartTypes.FirstOrDefault(p => p.GetName().Contains(loadOut.GetItemTypeString(loadOut.Turret1.Value)) == true);
                if (part != null)
                    ClientConnection.BuyPartOnBudget(ship, part, 0, ClientConnection.GetMoney());
            }

            if (loadOut.Turret2 != null)
            {
                var part = buyablePartTypes.FirstOrDefault(p => p.GetName().Contains(loadOut.GetItemTypeString(loadOut.Turret2.Value)) == true);
                if (part != null)
                    ClientConnection.BuyPartOnBudget(ship, part, 1, ClientConnection.GetMoney());
            }

            if (loadOut.Turret3 != null)
            {
                var part = buyablePartTypes.FirstOrDefault(p => p.GetName().Contains(loadOut.GetItemTypeString(loadOut.Turret3.Value)) == true);
                if (part != null)
                    ClientConnection.BuyPartOnBudget(ship, part, 2, ClientConnection.GetMoney());
            }

            if (loadOut.Turret4 != null)
            {
                var part = buyablePartTypes.FirstOrDefault(p => p.GetName().Contains(loadOut.GetItemTypeString(loadOut.Turret4.Value)) == true);
                if (part != null)
                    ClientConnection.BuyPartOnBudget(ship, part, 3, ClientConnection.GetMoney());
            }

            if (loadOut.Dispenser != null)
            {
                var part = buyablePartTypes.FirstOrDefault(p => p.GetName().Contains(loadOut.GetItemTypeString(loadOut.Dispenser.Value)) == true);
                if (part != null)
                    ClientConnection.BuyPartOnBudget(ship, part, 0, ClientConnection.GetMoney());
            }

            if (loadOut.Missiles != null)
            {
                var part = buyablePartTypes.FirstOrDefault(p => p.GetName().Contains(loadOut.GetItemTypeString(loadOut.Missiles.Value)) == true);
                if (part != null)
                    ClientConnection.BuyPartOnBudget(ship, part, 0, ClientConnection.GetMoney());
            }

            if (loadOut.Cargo1 != null)
            {
                var part = buyablePartTypes.FirstOrDefault(p => p.GetName().Contains(loadOut.GetItemTypeString(loadOut.Cargo1.Value)) == true);
                if (part != null)
                    ClientConnection.BuyPartOnBudget(ship, part, -1, ClientConnection.GetMoney());
            }

            if (loadOut.Cargo2 != null)
            {
                var part = buyablePartTypes.FirstOrDefault(p => p.GetName().Contains(loadOut.GetItemTypeString(loadOut.Cargo2.Value)) == true);
                if (part != null)
                    ClientConnection.BuyPartOnBudget(ship, part, -2, ClientConnection.GetMoney());
            }

            if (loadOut.Cargo3 != null)
            {
                var part = buyablePartTypes.FirstOrDefault(p => p.GetName().Contains(loadOut.GetItemTypeString(loadOut.Cargo3.Value)) == true);
                if (part != null)
                    ClientConnection.BuyPartOnBudget(ship, part, -3, ClientConnection.GetMoney());
            }

            if (loadOut.Cargo4 != null)
            {
                var part = buyablePartTypes.FirstOrDefault(p => p.GetName().Contains(loadOut.GetItemTypeString(loadOut.Cargo4.Value)) == true);
                if (part != null)
                    ClientConnection.BuyPartOnBudget(ship, part, -4, ClientConnection.GetMoney());
            }

            if (loadOut.Cargo5 != null)
            {
                var part = buyablePartTypes.FirstOrDefault(p => p.GetName().Contains(loadOut.GetItemTypeString(loadOut.Cargo5.Value)) == true);
                if (part != null)
                    ClientConnection.BuyPartOnBudget(ship, part, -5, ClientConnection.GetMoney());
            }

            //_client.EndLockDown(LockdownCriteria.lockdownLoadout);

            // Launch! 
            ClientConnection.BuyLoadout(ship, true);

            ClientConnection.EndLockDown(LockdownCriteria.lockdownLoadout);
        }

        private void ClearShipCargo(IshipIGCWrapper ship)
        {
            // Clear the cargo.
            for (sbyte i = -5; i < 0; i++)
            {
                var currentPart = ship.GetMountedPart((short)EquipmentType.NA, i);
                if (currentPart != null)
                    currentPart.Terminate();
            }

            // Clear weapons
            for (sbyte i = 0; i < 4; i++)
            {
                var currentPart = ship.GetMountedPart((short)EquipmentType.ET_Weapon, i);
                if (currentPart != null)
                    currentPart.Terminate();
            }

            // Clear turrets
            for (sbyte i = 0; i < ship.GetHullType().GetMaxWeapons() - ship.GetHullType().GetMaxFixedWeapons(); i++)
            {
                var currentPart = ship.GetMountedPart((short)EquipmentType.ET_Turret, i);
                if (currentPart != null)
                    currentPart.Terminate();
            }

            // Clear missiles
            for (sbyte i = 0; i < 1; i++)
            {
                var currentPart = ship.GetMountedPart((short)EquipmentType.ET_Magazine, i);
                if (currentPart != null)
                    currentPart.Terminate();
            }

            // Clear Dispenser
            for (sbyte i = 0; i < 1; i++)
            {
                var currentPart = ship.GetMountedPart((short)EquipmentType.ET_Dispenser, i);
                if (currentPart != null)
                    currentPart.Terminate();
            }
        }

        private string GetHullTypeString(ShipType shipType)
        {
            switch (shipType)
            {
                case ShipType.Scout:
                    return "Scout";

                case ShipType.Fighter:
                    return "Fighter";

                case ShipType.Interceptor:
                    return "Interceptor";

                case ShipType.Bomber:
                    return "Bomber";

                case ShipType.Gunship:
                    return "Gunship";

                case ShipType.StealthFighter:
                    return "Stealth Fighter";

                default:
                    throw new NotSupportedException(shipType.ToString());
            }
        }
    }
}
