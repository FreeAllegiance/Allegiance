# src/Wopr/Wopr

[Back to the index](../LLAMA_SUMMARY.md) - 8 files, 2,747 lines. Notes by a local model, to orient by: not verified facts.

The `src/Wopr/Wopr` folder contains essential components for managing game sessions and client interactions in Allegiance. It includes classes for loading strategy plugins, handling configuration settings, finding paths, receiving messages, and directing teams of players.

- AssemblyLoader.cs: Loads and manages strategy plugins from external assemblies.
- Configuration.cs: Retrieves configuration settings from the application's configuration file.
- DijkstraPathFinder.cs: Implements Dijkstra's algorithm to find the shortest path between clusters in a space game mission.
- MessageReceiver.cs: Handles incoming messages from client connections, processing various message types and invoking corresponding delegate methods.
- Program.cs: The entry point for the Wopr application, managing game sessions by creating and controlling teams of players through the Allegiance core.
- TeamDirector.cs: Manages clients, assigning strategies, handling client lifecycle events like connecting to the lobby and changing strategies.

Largest files: MessageReceiver.cs (1648 lines), TeamDirector.cs (519 lines), Program.cs (302 lines), DijkstraPathFinder.cs (155 lines), AssemblyLoader.cs (61 lines).

## `src/Wopr/Wopr/AssemblyLoader.cs`

PURPOSE: The AssemblyLoader class is responsible for loading and managing strategy plugins from external assemblies in the Allegiance game.

KEY PARTS:
- AssemblyLoader: Loads strategies from specified assemblies and caches them by StrategyID.
- CreateInstance: Creates an instance of a strategy based on its StrategyID.
- _strategyTypesByStrategyID: Dictionary that maps StrategyIDs to their corresponding strategy types.

CONNECTS TO: Wopr.TeamDirector (indirectly through StrategyBase).

NOTES:
- The commented-out LoadPlugins function was likely intended for creating instances but was not fully implemented.

## `src/Wopr/Wopr/Configuration.cs`

PURPOSE: This file defines a class `Configuration` that retrieves configuration settings from the application's configuration file.

KEY PARTS:
- ServerName: Retrieves the server name from the configuration.
- LobbyAddress: Retrieves the lobby address from the configuration.
- ArtPath: Retrieves the path to art assets from the configuration.
- Core: Retrieves core-related settings from the configuration.

CONNECTS TO: System.Configuration.ConfigurationManager, which is used to access application settings.

NOTES: None.

## `src/Wopr/Wopr/DijkstraPathFinder.cs`

PURPOSE: The file `DijkstraPathFinder.cs` implements the Dijkstra's algorithm to find the shortest path between clusters in a space game mission.

KEY PARTS:
- `IclusterIGCWrapperComparer`: Compares and hashes `IclusterIGCWrapper` objects based on their object ID.
- `DijkstraPathFinder`: Main class that uses Dijkstra's algorithm to find the shortest path between two clusters. It maintains dictionaries for predecessors, estimates, and a queue of clusters.
- `_predecessor`, `_estimate`, `_queue`: Dictionaries used to store information about the path and distances during the algorithm execution.
- `BuildPath`: Initializes the algorithm with the given cluster information and executes Dijkstra's algorithm.
- `Relax`: Updates the distance estimate for a destination cluster if a shorter path is found.
- `MinimumCluster`: Finds the cluster with the minimum estimated distance in the queue.
- `GetDistance`: Returns the total distance of the shortest path between two clusters.
- `NextClusterObjectID` and `NextCluster`: Retrieve the next cluster object ID or the actual `IclusterIGCWrapper` object along the shortest path.

CONNECTS TO: AllegianceInterop (for game-specific classes and interfaces), ImissionIGCWrapper, IclusterIGCWrapper, ClusterInfo, WarpInfo.

NOTES: None.

## `src/Wopr/Wopr/LaunchWithoutYellowCom.bat`

PURPOSE: tiny file (2 lines), shown in full rather than summarized.

    xcopy /s /y /q  bin\FZDebug\* temp\Debug\
    temp\Debug\wopr.exe -y

## `src/Wopr/Wopr/MessageReceiver.cs`

PURPOSE: The `MessageReceiver.cs` file handles incoming messages from client connections in the Allegiance game. It processes various message types and invokes corresponding delegate methods to handle each type.

KEY PARTS:
- MessageReceiver: Class handling incoming messages.
- OnAppMessage: Method processing message data using BinaryReader.
- FMD_ prefixed methods: Delegates for specific message types like position acknowledgments, player readiness, etc.
- File.AppendAllText: Writes log messages to a file.
- TODOs: Potential future work or areas for optimization.

CONNECTS TO: Other game systems and files that process client messages.

NOTES: Contains commented-out lines indicating skipped or incomplete message type handling.

PARTS (in file order):

- lines 2-351
  - MessageReceiver: A class that handles incoming messages from a client connection.
  - OnAppMessage: A method that processes incoming message data.
  - MemoryStream and BinaryReader: Used to read binary data from the message.
  - MessageType enumeration: Contains constants for different message types.
  - FMD_ prefixed methods: These are delegates that handle specific message types. They are invoked based on the message type received.
  - File.AppendAllText: Writes log messages to a file, appending new messages each time.
  - TODOs: The commented-out lines indicate potential future work or areas for optimization. There is no explicit TODO tag, but the commented-out code suggests that some message types are currently skipped or not fully implemented.
- lines 352-694
  - FMD_C_POSITIONACK: Handles client position acknowledgment messages.
  - FMD_CS_DELPOSITIONREQ: Handles deletion of a player's position request.
  - FMD_CS_LOCK_LOBBY: Locks the lobby for further changes.
  - FMD_CS_LOCK_SIDES: Locks the sides in the game.
  - FMD_CS_PLAYER_READY: Indicates that a player is ready to start the game.
  - FMD_S_TEAM_READY: Signals that a team is ready.
  - FMD_CS_FORCE_TEAM_READY: Forces a team to be ready.
  - FMD_C_DOCKED: Handles client docking messages.
- lines 695-1037
  - FMD_S_TELEPORT_ACK: Handles teleport acknowledgment messages.
  - FMD_C_BOARD_SHIP: Handles client board ship requests.
  - FMD_C_VIEW_CLUSTER: Handles client view cluster requests.
  - FMD_S_KILL_SHIP: Handles server kill ship notifications.
  - FMD_CS_SET_WINGID: Handles set wing ID commands from clients.
  - FMD_S_ICQ_CHAT_ACK: Handles ICQ chat acknowledgment messages.
  - FMD_CS_CHATBUOY: Handles client chat buoy messages.
  - FMD_S_CREATE_CHAFF: Handles server create chaff notifications.
- lines 1039-1382
  - MessageReceiver: Handles receiving and processing messages from clients.
  - FMD_S_WARP_BOMB: Represents a warp bomb message sent by the server to a client.
  - FMD_S_PROMOTE: Represents a promotion message sent by the server to a client.
  - FMD_C_PROMOTE: Represents a promotion request message sent by a client to the server.
  - FMD_S_CREATE_BUCKETS: Represents a create buckets message sent by the server to a client.
  - FMD_S_RELAUNCH_SHIP: Represents a relaunch ship message sent by the server to a client.
  - FMD_S_PINGDATA: Represents a ping data message sent by the server to a client.
  - FMD_C_REQPINGDATA: Represents a request ping data message sent by a client to the server.
- lines 1383-1648
  - FMD_S_DESTROY_TREASURE_Event: Event handler for when a treasure is destroyed.
  - FMD_CS_NEW_SHIP_INSTANCE_Event: Event handler for when a new ship instance is created.
  - FMD_CS_DROP_PART_Event: Event handler for when a part is dropped from a ship.
  - FMD_CS_SWAP_PART_Event: Event handler for when parts are swapped on a ship.
  - FMD_CS_PING_Event: Event handler for ping requests from clients.
  - FMD_S_URLROOT_Event: Event handler for sending the URL root to clients.
  - FMD_S_MISSION_STAGE_Event: Event handler for updating mission stages.
  - FMD_C_POSITIONREQ_Event: Event handler for position request messages from clients.

## `src/Wopr/Wopr/Program.cs`

PURPOSE: The `Program.cs` file is the entry point for the Wopr application, which manages a game session by creating and controlling teams of players through the Allegiance core.

KEY PARTS:
- `Main`: The main function that initializes the program, handles command-line arguments, sets up team directors, and runs the game loop.
- `Run`: A private method that orchestrates the game's lifecycle, including player creation, message handling, and strategy updates.
- `Log`: A utility method for writing log messages to a file.

CONNECTS TO: AllegianceInterop.dll (for communication with the Allegiance core), Wopr.Constants (for configuration constants), and other system libraries like System.IO for file operations.

NOTES:
- The code includes commented-out sections that demonstrate various debugging and testing scenarios.
- There is a check to ensure the application does not run for more than 2 hours, which could be a security measure or a limitation on bot behavior.

## `src/Wopr/Wopr/README.md`

PURPOSE: tiny file (8 lines), shown in full rather than summarized.

    If AllegianceInterop.dll fails to load, then chances are the MSVCRT runtime is not up to date. Install the latest run time from:
    	https://aka.ms/vs/17/release/vc_redist.x86.exe
    	https://aka.ms/vs/17/release/vc_redist.x64.exe
    (WOPR is usually x86, but you can install both.)
    If AllegianceInterop.dll still fails to load, use dependency walker to see what it's missing, then put those .DLLs into the same directory as WOPR.
    If WOPR hangs when starting, check that BotAuthenticationGuid is set on both the machine running WOPR and on the game server in
    SOFTWARE\WOW3264Node\Microsoft\Microsoft Games\Allegiance\1.4\Server\BotAuthenticationGuid
    If you are trying to run WOPR on a debug build, put the MSVCRTD* dlls into the same directory as WOPR.

## `src/Wopr/Wopr/TeamDirector.cs`

PURPOSE: The `TeamDirector` class in Allegiance is responsible for managing clients and assigning strategies to them. It handles the lifecycle of clients, including connecting to the lobby, sending/receiving updates, and changing strategies based on game progress.

KEY PARTS:
- `TeamDirector`: Manages client connections and strategy assignments.
- `ChangeStrategy`: Changes the strategy for a given player.
- `CreatePlayer`: Creates a new player thread to handle client connections.
- `_connectedClientsByPlayerName`: Stores connected clients by player name.
- `_currentStrategyByPlayerName`: Tracks current strategies assigned to players.

CONNECTS TO: AllegianceInterop.ClientConnection, StrategyBase, and other game-related classes.

NOTES:
- The code includes commented-out sections for loading strategies into a separate app domain, which was intended for hot-swapping strategies during gameplay but is currently unused.
- There are TODO comments suggesting setting default strategies in the `Strategy_OnStrategyComplete` method.
