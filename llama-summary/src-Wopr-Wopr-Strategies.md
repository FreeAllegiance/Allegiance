# src/Wopr/Wopr/Strategies

[Back to the index](../LLAMA_SUMMARY.md) - 4 files, 3,855 lines. Notes by a local model, to orient by: not verified facts.

The `src/Wopr/Wopr/Strategies` folder in Allegiance contains essential strategy implementations for managing game operations such as research and expansion, scouting, and connection management.

- CommanderResearchAndExpand.cs: Manages investments, technology queues, and station placement to facilitate the bot's growth and exploration.
- ScoutExploreMap.cs: Handles scouting and exploration strategies, including movement through clusters, finding warps, and returning to base.
- ConnectToGame.cs: Manages the game client connection and initialization, handling events related to player interactions and server communication.
- StrategyBase.cs: Provides a foundational class for implementing various strategies, offering methods for communication with the game client and managing strategy lifecycle.

## `src/Wopr/Wopr/Strategies/CommanderResearchAndExpand.cs`

PURPOSE:
The CommanderResearchAndExpand.cs file implements a strategy for researching and expanding in Allegiance's Wopr game. It manages investments, technology queues, and station placement.

KEY PARTS:
- CommanderResearchAndExpand: Main class implementing research and expansion strategy.
- QueueTechForAvailableMoney: Queues technologies based on available funds.
- PlaceStation: Places stations at clusters.
- InvestInTech: Attempts to invest in technologies.
- ProcessInvestmentQueue: Manages the investment queue, handling money and tech availability.

CONNECTS TO:
- Allegiance game engine systems for managing resources, technology, and station placement.

NOTES:
None.

PARTS (in file order):

- lines 1-377
  - CommanderResearchAndExpand: Main class that implements a strategy for researching and expanding.
  - AmountOfMoneyToKeepOnHand: Constant defining how much money to keep on hand.
  - SecondTechbaseMarker: Marker string for second techbase.
  - _investmentList: List of investments.
  - _constuctorsInFlightToClusterObjectIDByShipObjectID: Dictionary mapping ship IDs to cluster object IDs for in-flight constructors.
  - _clustersWaitingForConstructors: List of clusters waiting for constructors.
  - _primaryTechpath and _secondaryTechpath: Nullable variables for primary and secondary tech paths.
  - _primaryTechPaths and _secondaryTechPaths: Dictionaries mapping tech path types to lists of technology names.
- lines 378-782
  - QueueTechForAvailableMoney: Queues technology for available money.
  - UndockMinersAndStartMining: Undocks miners and starts mining.
  - MessageReceiver_FMD_S_MONEY_CHANGE: Handles money change messages.
  - MessageReceiver_FMD_CS_CHATMESSAGE: Handles chat message messages.
  - MessageReceiver_FMD_S_STATIONS_UPDATE: Handles stations update messages.
  - MessageReceiver_FMD_S_BUCKET_STATUS: Handles bucket status messages.
  - MessageReceiver_FMD_S_SHIP_STATUS: Handles ship status messages.
  - PlaceStation: Places a station at a specified cluster.
- lines 783-1164
  - Log: Writes log messages to a file or console.
  - DijkstraPathFinder: Class used for finding the shortest path between clusters using Dijkstra's algorithm.
  - GetCore(): Returns the core game object containing various game data.
  - GetClusters(): Retrieves all clusters in the galaxy.
  - GetStations(): Returns a list of stations within a cluster.
  - GetSide(): Retrieves the side (player) information.
  - GetMissionParams(): Provides mission parameters such as maximum drones.
  - QueueTech: Queues a technology for investment and construction.
- lines 1165-1624
  - QueueTech: Queues a technology for investment.
  - GetNextTechbaseName: Determines the next tech base to build based on available resources and current stations.
  - IsTechAvailableForInvestment: Checks if a technology is available for investment.
  - InvestInTech: Attempts to invest in a technology, handling insufficient funds and already in progress states.
  - ProcessInvestmentQueue: Processes the queue of technologies to invest in, managing money and tech availability.
  - QueueTechForTechpath: Queues technologies based on a predefined tech path.
  - GetTeleportCount: Counts the number of teleport stations.
  - GetOutpostCount: Counts the number of outpost stations.

## `src/Wopr/Wopr/Strategies/ConnectToGame.cs`

**PURPOSE:** The `ConnectToGame.cs` file in Allegiance is responsible for managing the connection and initialization process of a game client. It handles various events related to game state, player interactions, and server communication.

**KEY PARTS:**
- **Class: ConnectToGame**: Extends `StrategyBase` and implements logic for connecting to and interacting with a game.
- **Function: AttachMessages**: Registers message handlers for Allegiance events such as ship status updates, mission stages, chat messages, and team readiness.
- **Function: MessageReceiver_FMD_S_SHIP_STATUS**: Handles updates on the bot's ship status, finishing the strategy if the mission has started.
- **Function: MessageReceiver_FMD_S_MISSION_STAGE**: Finishes the strategy when the mission stage is set to `STAGE_STARTED`.
- **Function: MessageReceiver_FMD_CS_CHATMESSAGE**: Processes chat messages, including commands like "go" and "miner".
- **Function: MessageReceiver_FMD_S_TEAM_READY**: Handles updates on team readiness status, triggering launch if all sides are ready.
- **Function: MessageReceiver_FMD_CS_SET_TEAM_LEADER**: Sets the bot as team leader and configures team name and civilization.
- **Function: MessageReceiver_FMD_CS_QUIT_SIDE**: Handles side quitting events, moving back to the original side.
- **Function: MessageReceiver_FMD_CS_SET_MISSION_OWNER**: Ensures the bot is set as mission owner if it's not already.
- **Function: MessageReceiver_FMD_S_JOIN_SIDE**: Manages player joining side events, sending join requests or setting team leader.
- **Function: MessageReceiver_FMD_S_AUTODONATE**: Forces the bot to take over if auto-donate occurs and it's the commander.
- **Function: MessageReceiver_FMD_S_JOINED_MISSION**: Sends a position request when joining a mission.
- **Function: MessageReceiver_FMD_S_LOGONACK**: Disconnects from the lobby upon successful logon, storing ship ID and player cookie.
- **Function: MessageReceiver_FMD_LS_LOBBYMISSIONINFO**: Connects to the game server if it matches the desired game name, creating a new game if no suitable mission is found.
- **Function: MessageReceiver_FMD_L_SERVERS_LIST**: Handles reception of server list messages from the lobby and searches for a specific server to create a new mission.
- **Function: MessageReceiver_FMD_L_LOGON_ACK**: Responds to logon acknowledgment by requesting the server list.
- **Function: ResetGameParameters**: Resets game parameters to default values before starting a new mission.
- **Function: CheckForLaunch**: Checks if all teams are ready and initiates the game start process if so.

**CONNECTS TO:** Other systems or files it depends on include `StrategyBase`, various message handling functions, and server communication protocols.

**NOTES:** The file includes TODO comments suggesting potential enhancements for future development.

## `src/Wopr/Wopr/Strategies/ScoutExploreMap.cs`

**PURPOSE:** The `ScoutExploreMap.cs` file manages the scouting and exploration strategy for a bot in Allegiance. It handles various game events, processes ship status updates, and implements logic to move the bot through clusters, find warps, and return to base.

**KEY PARTS:**
- **Class: ScoutExploreMap:** Extends `StrategyBase`, managing the scouting and exploration strategy.
- **Function: AttachMessages:** Registers message handlers for game events like chat messages, ship status updates, docking, etc.
- **Function: MessageReceiver_FMD_CS_CHATMESSAGE:** Handles chat messages; if "die" is received, commands the bot to suicide.
- **Function: MessageReceiver_FMD_S_DOCKED:** Handles docking at a station, changing the ship to a scout and relaunching if necessary.
- **Function: MessageReceiver_FMD_S_SHIP_STATUS:** Processes ship status updates, managing bot behavior based on current state.
- **Function: FlyToStationPointAndWaitForAelphs:** Moves the bot to a point and waits for Aelphs to appear, potentially navigating to a new warp.
- **Function: SweepCluster:** Sweeps through a cluster by creating waypoints and moving between them to explore.
- **Function: DoTargetedSweep:** Placeholder function intended for targeted sweeping but not implemented.
- **Function: Start():** Initializes the scout by checking if the ship is in a station and changing it to a scout if necessary.
- **Function: ScoutSector():** Handles scouting behavior, including finding visible warps and determining targets.
- **Function: ReturnToBase():** Directs the ship back to the nearest base or station.
- **Function: IsCurrentShipAScout():** Checks if the current ship is a scout by examining its hull type name.
- **Function: ChangeShip(ShipType, LoadoutBase):** Changes the ship's type and loadout based on provided parameters.
- **Function: ClearShipCargo(IshipIGCWrapper):** Clears all cargo and equipment from the ship to ensure a clean state before changing its loadout.
- **Function: GetHullTypeString(ShipType):** Converts a `ShipType` enum value to its corresponding string representation.

**CONNECTS TO:** Other systems or files it depends on or serves, one line. None explicitly mentioned in the provided code snippets.

**NOTES:** The commented-out sections suggest additional features or optimizations that were either not completed or deemed unnecessary. The use of `Task.Run` for asynchronous operations is a common pattern to handle long-running tasks without blocking the main thread.

## `src/Wopr/Wopr/Strategies/StrategyBase.cs`

PURPOSE: This file defines a base class `StrategyBase` for implementing different strategies in the Allegiance game. It handles communication with the game client, manages events, and provides methods to start and finish strategies.

KEY PARTS:
- `StrategyBase`: Abstract class that serves as the foundation for all strategy implementations.
- `OnStrategryCompleteHandler` and `OnGameCompleteHandler`: Event delegates for handling strategy completion and game completion events.
- `_strategyTypesByStrategyID`: Dictionary mapping strategy IDs to their corresponding types.
- `_random`: Random number generator used by strategies.
- `ClientConnection`: Property for the client connection object.
- `Attach`: Method to attach the strategy to a client, setting up event handlers and initializing properties.
- `FinishStrategy`: Method to finish the strategy and detach from the client.
- `Cancel`: Method to cancel the strategy execution.
- `OnAppMessage`: Method to handle incoming application messages.
- `Log`: Method for logging messages with timestamps.

CONNECTS TO: AllegianceInterop.ClientConnection, Wopr.Entities.GameInfo

NOTES:
- The class uses reflection to dynamically create instances of strategy types based on their IDs.
- Event handlers are attached and detached within the `Attach` and `Detach` methods.
- There is a commented-out section for exporting game information, which was likely used for debugging or development purposes.
