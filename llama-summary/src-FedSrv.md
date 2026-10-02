# src/FedSrv

[Back to the index](../LLAMA_SUMMARY.md) - 66 files, 32,750 lines. Notes by a local model, to orient by: not verified facts.

The `src/FedSrv` folder contains essential server-side logic and components for managing Allegiance games, including game state, player interactions, and mission handling.

- FedSrv.CPP: Implements the core server logic for handling game state, player interactions, and mission management.
- fsmission.cpp: Manages mission creation, player management, voting, and game state for Allegiance multiplayer space games.
- fsship.cpp: Handles ship and player interactions, including status updates, warp jumps, loadouts, and side affiliations.
- allsrvmodule.cpp: Manages service-related functions and COM/ATL support for Allegiance's AllSrv module.
- AllSrvModuleIDL.idl: Defines interfaces and coclasses for the Allegiance server administration API.

Largest files: FedSrv.CPP (14126 lines), fsmission.cpp (6199 lines), fsship.cpp (1463 lines), allsrvmodule.cpp (1367 lines), AllSrvModuleIDL.idl (622 lines).

## `src/FedSrv/AdminEventLoggerHook.cpp`

PURPOSE:
The file `AdminEventLoggerHook.cpp` implements a class `CAdminEventLoggerHook` that handles logging events asynchronously. It processes event data and posts it to a database for storage.

KEY PARTS:
- `CAdminEventLoggerHook`: Class responsible for handling event logging.
- `EventLogged(CQLogEvent* pquery)`: Callback method invoked when an event is logged, signaling the completion of the operation.
- `LogEvent(IAGCEvent* pEvent, VARIANT_BOOL bSynchronous)`: Interface method to log events synchronously or asynchronously.

CONNECTS TO:
- `CQLogEvent` and related classes for handling query operations.
- Database interaction through `g.sql`.

NOTES:
- Contains commented-out code related to SQLCore initialization and posting queries, which are no longer used.

## `src/FedSrv/AdminEventLoggerHook.h`

PURPOSE: This file defines a COM object that hooks into the event logging system of Allegiance, allowing for custom event logging behavior.

KEY PARTS:
- CAdminEventLoggerHook: A COM class implementing IAGCEventLoggerHook to intercept and log events.
- EventLogged: Static method called when an event is logged.
- LogEvent: Method to log a single event, either synchronously or asynchronously.

CONNECTS TO: IAGCEventLoggerHook interface, likely used by other parts of the game for event logging.

NOTES: None.

## `src/FedSrv/AdminGame.cpp`

PURPOSE: This file contains the implementation of the `CAdminGame` class, which is responsible for managing administrative functions in a game mission.

KEY PARTS:
- `CAdminGame`: Main class implementing administrative game functionalities.
- `get_GameParameters()`: Retrieves game parameters such as team names and story text.
- `SendMsg(BSTR bstrMessage)`: Sends a message to all players in the game.
- `Kill()`: Commits suicide by deleting the host IGC.
- `get_Users()`: Returns the collection of users currently in the game.
- `get_GameOwnerUser()`: Retrieves the user that owns the game.
- `StartGame()`: Starts the game if it is not already started.
- `GetDescription(BSTR * pbstrDescription)`: Generates a description string for the game.
- `StartCountdown()`: Initiates a countdown before the game starts.
- `SetTeamName(int iSideID, BSTR bstrName)`: Sets the name of a team by side ID.
- `OverrideTechBit(int iSideID, int nTechBitID, BOOL bSetting)`: Overrides the tech bit setting for a specific team and technology bit.
- `SetDetailsFiles(BSTR bstrName)`: Sets the details files for the game.
- `RandomizeTeams()`: Randomizes the teams in the mission.

CONNECTS TO: `ImissionIGC`, `CFSMission`, `IAdminUsers`, `IAdminUser`.

NOTES: None.

## `src/FedSrv/AdminGame.h`

PURPOSE: This file defines the CAdminGame class, which implements the IAdminGame interface for managing administrative aspects of a game session in Allegiance.

KEY PARTS:
- Name: CAdminGame - Manages administrative functions for a game session.
- Name: Init - Initializes the game with an ImissionIGC object.
- Name: SendMsg - Sends a message to all users in the game.
- Name: Kill - Terminates the game session.
- Name: get_Users - Retrieves the collection of users currently playing the game.
- Name: StartGame - Starts the game session.
- Name: SetTeamName - Sets the name for a team in the game.
- Name: OverrideTechBit - Overrides a technology bit setting for a team.

CONNECTS TO: IAGCGameImpl, CAdminUsers

NOTES: None.

## `src/FedSrv/AdminGame.rgs`

PURPOSE: This file is a Windows registry script that defines the class information for a game component in Allegiance.

KEY PARTS:
- AllSrv.Game.1: Defines the version-specific ProgID and CLSID.
- AllSrv.Game: Defines the default ProgID and points to the version-specific ProgID.
- CLSID {9F17114A-E6CE-11D2-8B4B-00C04F681633}: The unique identifier for the game component.
- AppID: Links the CLSID to an application ID.
- TypeLib: Specifies the type library associated with the game component.

CONNECTS TO: Windows Registry

NOTES: None.

## `src/FedSrv/AdminGames.cpp`

PURPOSE: This file contains the implementation of the CAdminGames class, which manages game creation and parameters in a server environment.

KEY PARTS:
- CAdminGames::Add(): Adds a new game to the collection with specified mission parameters.
- TC_OBJECT_EXTERN_NON_CREATEABLE_IMPL(CAdminGames): Defines that CAdminGames cannot be created directly but must be used through interfaces.
- Error(): A function to handle error conditions and return appropriate HRESULT values.

CONNECTS TO: 
- IAGCGameParameters: Interface for game parameters.
- g.strLobbyServer, g.fmLobby: Global variables related to the lobby server connection.
- CFSMission: Class representing a mission in the game.
- FedSrvSite: Class representing a site in the federation server.

NOTES:
- The code includes checks for server pause status and limits on the number of players per team.
- There are conditional compilation directives for standalone versus non-standalone servers.
- Error handling is done using custom error messages and HRESULT values.

## `src/FedSrv/AdminGames.h`

PURPOSE: This file declares and implements the CAdminGames class, which manages a collection of admin games in the Allegiance server.

KEY PARTS:
- CAdminGames: Manages a collection of admin games.
- Add: Adds a new game to the collection using IAGCGameParameters.

CONNECTS TO: agc\IAGCCollectionImpl.h

NOTES: None.

## `src/FedSrv/AdminGames.rgs`

PURPOSE: This file is a Windows Registry Script (RGS) that defines the class information for the Games component in Allegiance's server system.

KEY PARTS:
- AllSrv.Games.1: Defines the version-specific class name and CLSID.
- AllSrv.Games: Defines the default class name and points to the specific version.
- CLSID {67757064-E6FC-11D2-8B4B-00C04F681633}: The unique identifier for the Games class.
- ProgID = s 'AllSrv.Games.1': Maps the programmatic identifier to the specific version of the class.
- VersionIndependentProgID = s 'AllSrv.Games': Provides a version-independent way to access the class.
- AppID: Links the class to an application ID for additional configuration or integration.
- TypeLib: Specifies the type library associated with the class.

CONNECTS TO: Windows Registry

NOTES: None.

## `src/FedSrv/AdminServer.cpp`

PURPOSE: The file `AdminServer.cpp` contains the implementation of the `CAdminServer` class, which provides administrative functionality for managing games and users on a server.

KEY PARTS:
- `CAdminServer`: Main class implementing the admin server interface.
- `get_Games()`: Returns an object representing all current games.
- `get_PlayerCount()`: Retrieves the number of players currently online.
- `get_MissionCount()`: Retrieves the number of missions (games) currently running.
- `SendMsg()`: Sends a message to all players on the server.
- `get_Users()`: Returns an object representing all current users.
- `get_FindUser()`: Finds and returns a user by name.
- `get_LookupUser()`: Finds and returns a user by unique ID.
- `put_LobbyServer()`: Sets or disconnects from the lobby server.
- `CreateDefaultGames()`: Creates default games if none are running.
- `get_PublicLobby()` and `put_PublicLobby()`: Manage whether the server is public or private.

CONNECTS TO: 
- `Global g`: Global state object.
- `CFSMission` and related classes for managing missions and ships.
- `AdminUserID` and other admin-related types.
- Windows registry for storing lobby server settings.

NOTES:
- The file includes several methods that are no longer supported or have been deprecated, such as `get_PacketsInPerSecond()`, `get_PlayersOnline()`, and `get_TimeInnerLoop()`.
- There is conditional compilation based on the build configuration (`ALLSRV_STANDALONE`), which affects the behavior of some functions.
- The file includes error handling and logging mechanisms to manage server state and connectivity.

## `src/FedSrv/AdminServer.h`

PURPOSE: This file defines the CAdminServer class, which is a COM object implementing the IAdminServer interface for managing Allegiance server operations.

KEY PARTS:
- Name: CAdminServer
  - Provides administrative functions for Allegiance servers.
- get_Games: Retrieves information about games running on the server.
- get_PlayerCount: Returns the number of players currently online.
- SendMsg: Sends a message to all connected clients.
- get_Users: Retrieves information about users connected to the server.
- get_LookupUser: Finds and retrieves a user by their unique ID.
- get_FindUser: Searches for a user by name.
- get_MachineName: Returns the machine name of the server.
- get_PacketsIn: Returns the number of packets received by the server.
- get_PlayersOnline: Returns the number of players currently online.
- get_TimeInnerLoop: Retrieves the time taken for the inner loop of the game.
- put_LobbyServer: Sets the lobby server address.
- get_LobbyServer: Retrieves the lobby server address.
- put_PublicLobby: Sets whether the lobby is public or private.
- get_PublicLobby: Retrieves the current state of the lobby (public or private).
- CreateDefaultGames: Creates default games on the server.

CONNECTS TO:
- Allegiance game engine and infrastructure for managing server operations.

NOTES:
- None.

## `src/FedSrv/AdminServer.rgs`

PURPOSE: This file is a Windows registry script that defines the class information for an administrative server in the Allegiance game.

KEY PARTS:
- AllSrv.Server.1: Defines the version 1 of the server class.
- CLSID: Specifies the globally unique identifier (GUID) for the server class.
- CurVer: Points to the current version of the server class.
- ProgID: Assigns a programmatic identifier to the server class.
- VersionIndependentProgID: Provides a version-independent identifier.
- AppID: Links to an application ID that can be used for additional configuration or security settings.
- TypeLib: References a type library associated with the server class.

CONNECTS TO: Windows registry and Allegiance game components.

NOTES: None.

## `src/FedSrv/AdminSession.cpp`

PURPOSE:
This file contains the implementation of the `CAdminSession` class, which provides administrative methods for managing sessions in Allegiance's server component.

KEY PARTS:
- CAdminSession: Manages admin sessions and exposes log-on methods.
- FinalConstruct: Initializes the event sink object.
- FinalRelease: Terminates the event sink object.
- put_SessionInfo, get_SessionInfo, get_SessionInfos: Manage session information.
- ActivateEvents, DeactivateEvents: Control event firing for specific events.
- Stop, Pause, Continue: Server control methods.
- get_WhoStartedServer: Retrieves the admin session that started the server.
- get_EventLog: Provides access to the event logger object.
- get_PerfCounters: Returns performance counters as an AGCEvent object.
- SendAdminChat: Sends admin chat messages.

CONNECTS TO:
- AdminSessionEventSink.h
- zreg.h
- AllSrv module

NOTES:
- Contains TODOs for thread safety and potential shutdown logic.

## `src/FedSrv/AdminSession.h`

PURPOSE: This file defines the `CAdminSession` class, which provides administrative session management functionality for Allegiance server using COM interfaces.

KEY PARTS:
- Class: `CAdminSession`
  - Manages admin sessions and exposes methods to interact with them.
- Methods: 
  - `put_SessionInfo`, `get_SessionInfo`, `get_Server`, etc.
  - Manage session information, events, and server control.
- Data Members:
  - `m_pEventSink`: Pointer to the event sink for handling admin session events.
  - `s_cs`: Critical section for thread-safe access to shared data.
  - `s_vecSessions`: Vector of active admin sessions.

CONNECTS TO: 
- COM interfaces (`IAdminSession`, `IDispatch`, etc.)
- Other Allegiance server components

NOTES:
- None

## `src/FedSrv/AdminSession.rgs`

PURPOSE: This file is an RGS (Registration Script) used to register COM components for the Allegiance game's administrative session functionality.

KEY PARTS:
- AdminSession Class: Represents the core class for managing administrative sessions.
- CLSID: Unique identifier for the AdminSession class, {AAA8270F-DDA4-11d2-8B46-00C04F681633}.
- ProgID: Programmatic Identifier used to reference the AdminSession class in code.
- VersionIndependentProgID: Used for backward compatibility with older versions of the software.

CONNECTS TO: Allegiance game server components, likely other COM objects related to session management and administrative tasks.

NOTES: None.

## `src/FedSrv/AdminSessionClass.cpp`

PURPOSE: This file contains the implementation of the `CAdminSessionClass` class, which is responsible for creating admin sessions in the Allegiance game server.

KEY PARTS:
- `IsEqualBSTR`: Compares two BSTRs (Basic String Types) for equality.
- `CreateInstance`: Implements the IClassFactory interface method to create instances of admin session objects. It includes production-specific checks and version validation logic.
- `CreateSession`: Creates a new admin session by validating the host, creating a decoy cookie, and ensuring compatibility with the server's version.

CONNECTS TO: 
- `AdminSessionSecure.h` for secure stream creation
- `IAdminSessionHost`, `IAdminSession`, `IAGCVersionInfo`, `IPersistStream` interfaces

NOTES:
- Contains conditional compilation to allow or disallow normal object creation based on the build configuration.
- Includes version validation logic to ensure compatibility between the server and admin session objects.

## `src/FedSrv/AdminSessionClass.h`

PURPOSE: This file defines a COM class factory and interface for managing admin sessions in the Allegiance game server.

KEY PARTS:
- CAdminSessionClass: Implements IAdminSessionClass to create admin sessions.
- IsEqualBSTR: Static method to compare two BSTRs.
- CreateInstance: Implementation of IClassFactory's CreateInstance method.
- CreateSession: Method to create an admin session hosted by a given host.

CONNECTS TO: Depends on the Allegiance game server infrastructure, specifically interfaces related to admin sessions and hosting.

NOTES: None.

## `src/FedSrv/AdminSessionEventSink.cpp`

PURPOSE: The file `AdminSessionEventSink.cpp` implements the event sink for administrative sessions in the Allegiance game. It handles events triggered by the session and forwards them to the owning session object.

KEY PARTS:
- `CAdminSessionEventSink`: Class that implements the `IAGCEventSink` interface to handle events.
- `Init(CAdminSession* pSession)`: Initializes the event sink with a reference to the owning session.
- `Term()`: Cleans up and revokes all registered events when the session is terminated.
- `OnEventTriggered(IAGCEvent* pEvent)`: Handles an event triggered by the game, forwarding it to the owning session.

CONNECTS TO: `AdminSession.h`, `AdminSession.cpp`, `IAGCEventSink`, `AGCGlobal`.

NOTES: None.

## `src/FedSrv/AdminSessionEventSink.h`

PURPOSE:
The file defines a COM object `CAdminSessionEventSink` that implements the `IAGCEventSink` interface to handle events related to an administrative session.

KEY PARTS:
- CAdminSessionEventSink: A COM class implementing IAGCEventSink to manage event handling for admin sessions.
- Init: Initializes the sink with a pointer to an `CAdminSession`.
- Term: Terminates the sink and cleans up resources.
- OnEventTriggered: Handles events triggered by the admin session.
- GetGITCookie: Returns a GIT cookie, likely used for tracking or synchronization.

CONNECTS TO:
Depends on `CAdminSession` and implements `IAGCEventSink`.

NOTES:
None.

## `src/FedSrv/AdminSessionSecure.h`

PURPOSE:
This file defines a template class `CAdminSessionSecure` that provides secure encryption and decryption functionality for streams using a GUID-based XOR algorithm.

KEY PARTS:
- Name: CAdminSessionSecure
  - What it does: Template class providing methods to create encrypted and decrypted streams using a GUID-based XOR algorithm.
  
- Function: CreateEncryptedStream
  - What it does: Encrypts the input stream by XORing its contents with a specified GUID, then writes the encrypted data to an output stream.

- Function: CreateDecryptedStream
  - What it does: Decrypts the input stream using the same GUID-based XOR algorithm as `CreateEncryptedStream`.

CONNECTS TO:
- Depends on Windows COM interfaces (IStream).

NOTES:
- The encryption and decryption algorithms are identical, which might be a design oversight or intentional for simplicity.
- No TODOs or other notes were identified.

## `src/FedSrv/AdminSessionSecureHost.h`

PURPOSE: This file defines a class `AdminSessionSecureHost` that implements the `IAdminSessionHost` interface for secure administration sessions in Allegiance.

KEY PARTS:
- AdminSessionSecureHost: Implements the IAdminSessionHost interface to handle admin session requests securely.
- QueryInterface: Handles COM interface queries, returning S_OK if the requested interface is supported.
- AddRef and Release: Stub implementations of reference counting methods (always return 1).
- GetIdentity: Processes a request for identity information, creating a secure stream of version data.

CONNECTS TO: Allegiance game engine components, specifically related to admin sessions and COM interfaces.

NOTES: None.

## `src/FedSrv/AdminShip.cpp`

PURPOSE: This file contains the implementation of the CAdminShip class, which represents an administrative ship in the Allegiance game. It includes methods for retrieving information about the pilot of the ship.

KEY PARTS:
- CAdminShip: Represents an administrative ship.
- get_User: Retrieves a pointer to the User object piloting the ship.

CONNECTS TO: src/FedSrv/CFSShip.cpp, src/FedSrv/CFSPlayer.cpp

NOTES: None.

## `src/FedSrv/AdminShip.h`

PURPOSE: This file defines the CAdminShip class, which represents an administrative ship in the Allegiance game server. It implements the IAdminShip interface and inherits from IAGCShipImpl.

KEY PARTS:
- CAdminShip: Represents an administrative ship with methods to interact with users.
- Host2Igc and Igc2Host: Conversion functions between CFSShip and IshipIGC interfaces.
- DECLARE_REGISTRY_RESOURCEID, DECLARE_PROTECT_FINAL_CONSTRUCT: Macros for COM registration and construction protection.
- BEGIN_COM_MAP, END_COM_MAP: Maps the CAdminShip class to its COM interfaces.
- BEGIN_CATEGORY_MAP, END_CATEGORY_MAP: Defines the category of the component.

CONNECTS TO: AGC\IAGCShipImpl.h (for IAGCShipImpl base class), FedSrv\AdminUsers.h (for CAdminUsers).

NOTES: None.

## `src/FedSrv/AdminShip.rgs`

PURPOSE: This file is a Windows registry script that defines the class information for a ship object in the Allegiance game. It registers the CLSID and ProgID for the ship class.

KEY PARTS:
- AllSrv.Ship.1: Defines the version-specific class name.
- CLSID: Specifies the unique identifier for the ship class.
- CurVer: Points to the current version of the ship class.
- ForceRemove: Ensures that the class can be removed if necessary.
- ProgID: Provides a programmatic identifier for the ship class.
- VersionIndependentProgID: Allows compatibility with older versions.
- AppID and TypeLib: References associated application and type library identifiers.

CONNECTS TO: Windows registry system, Allegiance game engine.

NOTES: None.

## `src/FedSrv/AdminUser.cpp`

**PURPOSE:**  
This file contains the implementation of the `CAdminUser` class, which represents an administrative user in the Allegiance game. It provides methods to interact with and manage player data, such as sending messages, retrieving player statistics, and logging off users.

**KEY PARTS:**
- **Class:** `CAdminUser`
  - Represents an administrative user.
- **Function:** `InterfaceSupportsErrorInfo`
  - Checks if the interface supports error information.
- **Function:** `Init`
  - Initializes the `CAdminUser` object with a player.
- **Function:** `get_Name`
  - Retrieves the name of the player.
- **Function:** `get_Ship`
  - Retrieves the ship that the player is piloting.
- **Function:** `SendMsg`
  - Sends a message to the player.
- **Function:** `get_isReady` and `put_isReady`
  - Gets and sets whether the user is ready.
- **Function:** `Boot`
  - Logs off the user by removing them from the mission.
- **Function:** `get_UserID`, `get_UniqueID`
  - Retrieves unique identifiers for the user.
- **Function:** `get_PlayerStats`
  - Retrieves player statistics.

**CONNECTS TO:**
- `CFSPlayer`: Manages player data.
- `IAdminShip`: Represents a ship that can be piloted by a player.
- `AGCGlobal`: Provides global game objects and services.
- `CComBSTR`, `CComVariant`: Used for string and variant manipulation.

**NOTES:**
- The `Init` function has a TODO comment to remove it.
- There is no explicit security or network handling noted.

## `src/FedSrv/AdminUser.h`

PURPOSE: This file defines the CAdminUser class, which represents an administrative user in the Allegiance game server. It provides methods for managing and interacting with players.

KEY PARTS:
- Name: CAdminUser
  - Manages administrative operations for users.
- Function: get_Name
  - Retrieves the name of the admin user.
- Function: SendMsg
  - Sends a message to the associated player.
- Function: get_isReady
  - Gets whether the admin user is ready.
- Function: put_isReady
  - Sets whether the admin user is ready.
- Function: Boot
  - Boots the associated player.
- Data Member: m_pPlayer
  - Pointer to the CFSPlayer object representing the player.

CONNECTS TO:
- CFSPlayer: The class that represents a player in the game.

NOTES:
- None.

## `src/FedSrv/AdminUser.rgs`

PURPOSE: This file is a Windows registry script that defines the class information for the User Class in Allegiance's administrative server.

KEY PARTS:
- Name: AllSrv.User.1 - Represents the version 1 of the User Class.
- CLSID: {E71EA5B8-EBA4-11D2-8B4B-00C04F681633} - Unique identifier for the User Class.
- ProgID: AllSrv.User.1 - Programmatic Identifier for version 1 of the User Class.
- VersionIndependentProgID: AllSrv.User - Base programmatic identifier for the User Class.
- AppID: {E4E8767E-DFDB-11d2-8B46-00C04F681633} - Application ID associated with the User Class.
- TypeLib: {BB5FC41C-DDA4-11d2-8B46-00C04F681633} - Type Library for the User Class.

CONNECTS TO: Windows Registry

NOTES: None.

## `src/FedSrv/AdminUsers.cpp`

PURPOSE: This file contains the implementation of the `CAdminUsers` class, which manages and provides access to administrative users in a game server.

KEY PARTS:
- `CAdminUsers`: Manages a collection of admin users.
- `InterfaceSupportsErrorInfo`: Checks if the interface supports error information.
- `get_Count`: Returns the number of players online or in a specific mission.
- `get__NewEnum`: Provides iteration support for enumerating admin users.
- `get_Item`: Intentionally not supported to avoid O(n) time complexity.

CONNECTS TO: `CFSPlayer`, `CFSShip`, `IAdminUser`, and other game-related classes and interfaces.

NOTES:
- The `get_Item` function is intentionally disabled due to performance concerns.

## `src/FedSrv/AdminUsers.h`

PURPOSE: This file defines a COM object `CAdminUsers` that implements the `IAdminUsers` interface for managing administrative users in the Allegiance game server.

KEY PARTS:
- CAdminUsers: Main class implementing IAdminUsers and related interfaces.
- get_Count, get__NewEnum, get_Item: Methods to retrieve user count, enumerate users, and access individual users.
- SetMission: Method to set the mission context for user management.

CONNECTS TO: Depends on `ImissionIGC` for mission-related data.

NOTES: None.

## `src/FedSrv/AdminUsers.rgs`

PURPOSE: This file defines a registry script for the Users class in Allegiance's administrative server.

KEY PARTS:
- AllSrv.Users.1: Represents the version 1 of the Users class.
- CLSID: Unique identifier for the Users class.
- CurVer: Points to the current version of the Users class.
- AppID and TypeLib: References to application and type library identifiers.

CONNECTS TO: Registry system, Allegiance administrative server.

NOTES: None.

## `src/FedSrv/AdminUtil.h`

PURPOSE: This file defines templates and base classes for managing object lifetimes through sponsorship. It allows a sponsor to manage multiple limbs, ensuring that if the sponsor is destroyed, all its limbs are also destroyed.

KEY PARTS:
- `CBaseSponsor`: Abstract base class defining the interface for detaching a limb.
- `CAdminSponsor`: Template class representing a sponsor that can create and manage a single limb. It ensures proper cleanup when the sponsor is destroyed.
- `CAdminLimb`: Template class representing a limb that can be sponsored by another object. It handles detachment from its sponsor.

CONNECTS TO: None, operates internally within the system.

NOTES:
- The file includes debugging code (`SHOW_SPONSOR_INFO`) to track the number of sponsors and limbs, which is conditionally compiled based on `DEBUG` and `markcu`.
- The `CDebugClass` is a temporary class used for debugging purposes, tracking sponsors and limbs.
- The `_ForceDestruction` method in `CAdminLimb` uses `CoDisconnectObject` to ensure proper cleanup of COM objects.

## `src/FedSrv/AllSrv32.rgs`

PURPOSE: This file is a Windows registry script for the Allegiance game server application, defining its AppID and event log settings.

KEY PARTS:
- Name: AllSrv
- AppID: Unique identifier for the AllSrv application.
- EventLog: Configuration for logging events related to the AllSrv application.
- EventMessageFile: Specifies the location of message files for event logs.
- TypesSupported: Defines the types of events supported by the log.

CONNECTS TO: Windows registry and Allegiance game server application (AllSrv.EXE).

NOTES: None.

## `src/FedSrv/AllSrvModuleIDL.idl`

PURPOSE: This file defines interfaces and coclasses for the Allegiance server administration API, allowing remote management of game sessions, users, and servers.

KEY PARTS:
- `IAdminSession`: Interface for managing a single admin session.
- `IAdminServer`: Interface for managing the server itself, including games and users.
- `IAdminUser`: Interface for managing individual users.
- `AdminInterfaces`: Coclass to reference all admin interfaces in the type library.
- `AdminSession`, `AdminShip`, `AdminGame`, etc.: Individual coclasses implementing specific admin interfaces.

CONNECTS TO: AGCIDL.idl, which contains definitions used by this file.

NOTES: None.

## `src/FedSrv/BitArray.cpp`

PURPOSE:
The file `BitArray.cpp` contains the implementation of a class `CBitArray` that manages a 2D bit array. It provides functionality to set and check individual bits, clear rows and columns, and dump the state of the bit array for debugging purposes.

KEY PARTS:
- **Class: CBitArray**
  - Manages a 2D bit array.
  - Provides methods to set and check individual bits.
  - Includes functions to clear entire rows and columns.
  - Offers a method to dump the bit array's state for debugging.

- **Function: CBitArray(int nBits)**
  - Constructor that initializes a square bit array with `nBits` bits per dimension.

- **Function: CBitArray(int nBitsMin, int nBitsMax)**
  - Initializes a rectangular bit array with dimensions from `nBitsMin` to `nBitsMax`.

- **Function: ~CBitArray()**
  - Destructor that frees the allocated memory for the bit array.

- **Function: FIsSet(int x, int y)**
  - Checks if the bit at position `(x, y)` is set.

- **Function: Set(int x, int y, bool b)**
  - Sets or clears the bit at position `(x, y)` to the value `b`.

- **Function: ClearRow(int x)**
  - Clears all bits in the row at position `x`.

- **Function: ClearColumn(int y)**
  - Clears all bits in the column at position `y`.

- **Function: Dump(int min, int max)**
  - Dumps the state of the bit array within the specified range for debugging purposes.

CONNECTS TO:
- Depends on `pch.h` and `debugf()` function (likely defined elsewhere).

NOTES:
- None.

## `src/FedSrv/BitArray.h`

PURPOSE: This file defines a class `CBitArray` for managing and manipulating bit arrays, which can be used for various purposes such as representing game states, flags, or other binary data efficiently.

KEY PARTS:
- CBitArray(int nBits): Constructor to create a bit array with a specified number of bits.
- CBitArray(int nBitsMin, int nBitsMax): Constructor to create a bit array with a range of bits.
- ~CBitArray(): Destructor to clean up the allocated memory.
- bool FIsSet(int x, int y): Checks if a specific bit is set at position (x, y).
- bool Set(int x, int y, bool b): Sets or clears a specific bit at position (x, y) based on the boolean value `b`.
- void ClearRow(int x): Clears all bits in a specified row.
- void ClearColumn(int y): Clears all bits in a specified column.
- void Dump(int min, int max): Dumps the state of the bit array within a specified range.

CONNECTS TO: None

NOTES: none

## `src/FedSrv/CSteamAchievements.cpp`

PURPOSE: This file contains the implementation of a class `CSteamAchievements` that manages Steam achievements for players in the Allegiance game. It handles requests to get and set user stats, check achievements, and update leaderboard information.

KEY PARTS:
- CSteamAchievements: Manages Steam achievements for a player.
  - InitiateStatsRequest(): Initiates a request to fetch user stats from Steam.
  - GetStat(), SetStat(): Retrieve and update user stats.
  - GetAchievement(), SetAchievement(): Check and set achievement status.
  - SaveStats(): Stores the updated stats back to Steam.
  - OnUserStatsReceived(), OnUserStatsStored(): Callback functions for handling Steam API responses.

CONNECTS TO: Allegiance game server, Steam API.

NOTES:
- The file includes commented-out code that was likely used for debugging or testing but is not currently active.
- There are TODO comments indicating areas that need further development or review.

## `src/FedSrv/CSteamAchievements.h`

PURPOSE: This file defines the CSteamAchievements class responsible for managing achievements and statistics in the Allegiance game using Steam's API.

KEY PARTS:
- EAchievements: Enumerates all available achievement IDs.
- EStats: Enumerates all available statistic IDs.
- CSteamAchievements: Class handling achievement and statistic operations, including requesting stats from Steam servers, setting achievements, and saving statistics.

CONNECTS TO: Steam API for handling achievements and statistics.

NOTES: None.

## `src/FedSrv/CSteamUserGroupStatus.cpp`

PURPOSE: This file contains the implementation of a class that checks if a user is a member of specific Steam groups.

KEY PARTS:
- CSteamUserGroupStatus: Manages the status of a user's membership in a Steam group.
  - Constructor initializes with user and group IDs.
  - GetUserGroupStatus: Requests and waits for the user's group status, then returns whether the user is a member or officer.
  - OnUserGroupStatus: Handles callbacks to update the user's group status once received.
- IsMemberOfModeratorsGroup/IsMemberOfDevelopersGroup: Convenience functions to check if a user is a member of specific Steam groups (moderators and developers).

CONNECTS TO:
- CSteamGameServer, CSteamID, GSClientGroupStatus_t: Used for Steam API interactions.

NOTES:
- Contains TODOs to move group IDs into config files.
- Uses a spin lock in GetUserGroupStatus, which could be improved with threading.

## `src/FedSrv/CSteamUserGroupStatus.h`

PURPOSE: tiny file (17 lines), shown in full rather than summarized.

    #pragma once
    class CSteamUserGroupStatus
    {
    private:
    	CSteamID m_userID;
    	CSteamID m_groupID;
    	bool m_isMember = false;
    	bool m_isOfficer = false;
    	bool m_requestComplete = false;
    	void GetUserGroupStatus(bool &isMember, bool &isOfficer);
    	STEAM_GAMESERVER_CALLBACK(CSteamUserGroupStatus, OnUserGroupStatus, GSClientGroupStatus_t);
    public:
    	CSteamUserGroupStatus(CSteamID userID, CSteamID groupID);
    	static bool IsMemberOfModeratorsGroup(const char *steamID);
    	static bool IsMemberOfDevelopersGroup(const char *strSteamID);
    	static bool IsMemberOfDevelopersGroup(CSteamID steamID);
    };

## `src/FedSrv/CSteamValidation.cpp`

PURPOSE: The file `CSteamValidation.cpp` handles the Steam authentication process for player logins in Allegiance. It validates user tickets and processes responses from Steam to determine if a user is authorized to join the game.

KEY PARTS:
- `CSteamValidation`: Class that manages Steam authentication.
  - Constructor initializes member variables with provided parameters.
  - `BeginSteamAuthentication()`: Starts the Steam authentication process, handles different outcomes, and posts messages based on results.
  - `OnValidateAuthTicketResponse(ValidateAuthTicketResponse_t *pResponse)`: Processes responses from Steam regarding user authentication tickets.
- `GetRankForSteamID(CSteamID steamID)`: Retrieves a player's rank using their Steam ID.

CONNECTS TO: 
- `CSQLQuery` and `CQLogonStatsData` for database queries and logon statistics.
- `SteamGameServer()` for Steam API interactions.
- `g.idReceiveThread` for posting messages to the receive thread.

NOTES:
- Contains logic to handle various authentication outcomes, including successful validation, timeouts, and errors.
- Includes commented-out code related to player rank retrieval and lobby communication.

## `src/FedSrv/CSteamValidation.h`

PURPOSE: tiny file (20 lines), shown in full rather than summarized.

    #pragma once
    class CSteamValidation
    {
    private:
    	DWORD				m_dwThreadID;
    	CSQLQuery *			m_sqlQuery;
    	CQLogonStatsData *	m_logonStatsData;
    	int					m_characterID;
    	bool				m_bAuthenticationComplete = false;
    	ZString				m_responseText;
    	bool				m_isValid;
    public:
    	CSteamValidation(DWORD dwThreadID, int characterID, CSQLQuery * pQuery, CQLogonStatsData * pqd);
    	void BeginSteamAuthentication();
    private:
    	RankID GetRankForSteamID(CSteamID steamID);
    	// Tells us a client has been authenticated and approved to play by Steam (passes auth, license check, VAC status, etc...)
    	STEAM_GAMESERVER_CALLBACK(CSteamValidation, OnValidateAuthTicketResponse, ValidateAuthTicketResponse_t);
    	void OnValidateAuthTicketResponse(CSteamID m_SteamID, EAuthSessionResponse eAuthSessionResponse, CSteamID ownerSteamID);
    };

## `src/FedSrv/DPMono.rgs`

PURPOSE: This file is a Windows Registry Script that defines the CLSID (Class Identifier) entries for two components used in Microsoft Allegiance: 'Microsoft Allegiance Transport' and 'Microsoft Allegiance Lobby'. These entries specify the DLL file (`msrgtran.dll`) that implements these components and indicate that they support both single-threaded and multi-threaded apartments.

KEY PARTS:
- MSRGTRAN: Defines the CLSID for the 'Microsoft Allegiance Transport' component.
- MSGVLOBBY: Defines the CLSID for the 'Microsoft Allegiance Lobby' component.
- InprocServer32: Specifies the DLL file (`msrgtran.dll`) that implements these components.
- ThreadingModel: Indicates that both single-threaded and multi-threaded apartments are supported.

CONNECTS TO:
- msrgtran.dll: The DLL file that contains the implementation of the transport and lobby components.

NOTES:
- None.

## `src/FedSrv/FedSrv.CPP`

PURPOSE: Allegiance's server-side logic for managing game state, player interactions, and mission handling.

KEY PARTS:
- FedSrv_Terminate: Cleans up game state before exiting.
- SetSessionDetails: Generates session details for messaging.
- JoinMission: Handles logic for joining a mission, including checks and requirements.
- CFSPlayer: Manages player data and interactions.
- CFSDrone: Represents a drone ship with functions to create, leave, and board ships.
- DoCheatCode: Parses chat lines and performs actions based on cheat codes.
- MoveShips: Controls the movement of ships and manages scan range events.
- FedSrvSiteBase: Base class for server site management, handling connections and session events.
- CSteamValidation: Manages Steam validation for player logon processes.
- pfsMission: Handles mission-related operations such as starting countdowns, sending messages, and managing player interactions.
- FM_CS_SET_TEAM_INFO: Handles setting team information.
- FM_C_CHANGE_ALLIANCE: Manages changing alliances between teams.
- FM_CS_DELPOSITIONREQ: Deletes player positions.
- FM_CS_MISSIONPARAMS: Adjusts mission parameters.
- FM_C_START_GAME: Starts the game if conditions are met.
- ProcessNetworkMessages: Processes network messages, including periodic roll calls and connection checks.
- ThingSiteImpl: Manages visibility and interaction of game objects, including showing and hiding objects for specific sides.
- FedSrvApp::AsyncFileOut: Asynchronously writes debug output to a file.
- FedSrvApp::DebugOutput: Handles the actual logging and debugging output.

CONNECTS TO: Allegiance's client-side code, network infrastructure, Steam API.

NOTES: None.

PARTS (in file order):

- lines 1-342
  - FedSrv_Terminate: Cleanup function called before exiting.
  - SetSessionDetails: Generates a string of name/value pairs for session details in FedMessaging.
  - UnloadDbCache: Clears out database cache that doesn't change between missions.
  - LoadTechBits, LoadRankInfo, LoadStaticCoreInfo, LoadStaticMapInfo: Functions to load various game data from files or databases.
  - VerifyUserMap: Checks if a user map is valid based on the number of teams and file name.
  - AddNamedValue: Helper function to add named values to a string for session details.
  - g: Global variable containing game state information.
  - TODO: "Imago 6/10 #35 - Dropped player stats are reset" (comment in code)
- lines 343-687
  - FetchGlobalAttributeSet: Fetches attributes from a database for specified global attribute.
  - IsAnotherPlayerUsingThisName: Checks if another player is using the same name in the mission.
  - JoinMission: Handles the logic for joining a mission, including checks for password, player limits, and rank requirements.
  - g.hKillReceiveEvent, g.hPlayerEvent: Handles event handling related to game events.
  - g.fm.Shutdown(): Shuts down the message manager.
  - DisconnectFromLobby(): Disconnects from the lobby server.
  - ShutDownSQL(): Shuts down SQL operations if not in standalone mode.
  - timeEndPeriod(1): Ends the time period for high-resolution timers.
- lines 688-1043
  - CFSPlayer: Manages player data and interactions.
  - GotLogonDetails: Callback function for logon details query.
  - pfmLogonAck: Packet format for logon acknowledgment.
  - CQCharSquads: Query class for character squads information.
  - GotCharSquads: Callback function for character squads query results.
  - SetCharStats: Function to upload character persisted stats.
  - TODOs: "KG: eventually remove this" in GotInvitationList and GotCharSquads functions. Debugging statements like debugf that should be removed or commented out in production code.
- lines 1044-1388
  - SetCharStats: Updates character statistics based on mission scores and rank calculations.
  - RecordSquadGame: Uploads results of a squad game to the database.
  - GetInvitations: Sends a request to get invitations for a specific mission.
  - LoadDbCache: Loads static data from the database into memory for caching purposes.
  - TODOs: Stress test commented out at the end of SetCharStats function. Debug statements in RecordSquadGame and LoadDbCache functions.
  - Unusual code: Use of _alloca for dynamic memory allocation on the stack. Multiple while loops with similar logic to assign loser squad IDs in RecordSquadGame.
- lines 1389-1747
  - DataWeaponTypeIGC: Represents weapon data.
  - IweaponIGC: Interface for weapons.
  - DataMissileTypeIGC: Represents missile data.
  - ImissileIGC: Interface for missiles.
  - DataChaffTypeIGC: Represents chaff data.
  - DataShieldTypeIGC: Represents shield data.
  - DataCloakTypeIGC: Represents cloak data.
  - DataAfterburnerTypeIGC: Represents afterburner data.
- lines 1748-2091
  - CFSCluster: Manages cluster data.
  - GetGroupSectorDocked: Retrieves the group sector docked for a cluster.
  - GetGroupSectorFlying: Retrieves the group sector flying for a cluster.
  - ExportObj: Exports an IGC object to the client.
  - SendPlayerInfo: Sends player information to another player or drone.
  - rgperplrScore: Array of PersistPlayerScoreObject used in SendPlayerInfo.
  - TODO: The comment "is this specified anywhere else? -- KGJV : 8 to 20, why 8 anyway ?" suggests a potential issue with the value of cCivsMax.
- lines 2092-2444
  - CFSDrone: Represents a drone ship.
  - CreateStockDrone: Creates a new stock drone based on the given drone type and side.
  - LeaveShip: Handles the process of leaving a ship for a player.
  - BoardShip: Handles the process of boarding a ship by a player.
  - pfmPlayerInfo: Structure used to store information about a player's ship, including orders, base object ID, side, ship ID, rank ID, readiness status, and money.
  - CheatSides: Function that appears to be related to debugging or cheating, but is commented out.
- lines 2445-2779
  - CheatCreateDrones: Handles cheat code to create drones.
  - EnumPlayersCallback: Callback function for enumerating players and groups in DPlay.
  - CheatDPGroups: Cheat code to enumerate all our dpgroups for debugging purposes.
  - CheatJPD: The Joel P Dehlin cheat to create a friendly and enemy ship in front of you.
  - LogTrapHack: Logs trap hacks with event parameters.
  - TODOs: None identified in the provided snippet.
  - Unusual code: Uses assert statements, retailf for logging, and conditional compilation directives.
- lines 2780-3103
  - DoCheatCode: Parses chat lines and performs actions based on cheat codes.
  - LogTrapHack: Logs and handles hack events.
  - HandleCOMChatEvents: Generates COM events for chat messages.
  - g.fm.DeleteConnection: Deletes a connection in the game framework.
  - EventID_HackBoot, EventID_HackBootNoPlayer: Constants representing specific event IDs.
  - CFSMission, CFSShip, CFSSide: Classes representing mission, ship, and side objects.
  - ZString: A string class used for handling text.
  - Sleep: A function to pause execution for a specified duration.
- lines 3106-3447
  - CFSShip: Represents a ship in the game.
  - IshipIGC: Interface for ship-related information and operations.
  - AGCEventID: Enumerates different event IDs used in the game.
  - ChatData: Struct containing chat message data.
  - ForwardChatMessageToWing: Sends a chat message to all ships in a specified wing.
  - ForwardChatMessage: Handles forwarding of chat messages based on target type.
  - CommandToDrone: Processes commands for drones, including setting default orders and handling special cases like mining and building.
  - WingID: Identifier for a wing or group of ships.
- lines 3448-3810
  - CmpShipDeviation: Sort function for qsort to sort ships based on deviation.
  - AddUpdate: Adds ship updates to a message buffer.
  - SendShipUpdates: Sends ship updates to all clients in the sector.
  - pszDest: Destination of the chat message.
  - pcd->chatTarget: Chat target type.
  - ForwardChatMessageToWing: Forwards chat messages to specific wings.
  - CSteamAchievements::AwardIGCAchievements: Awards achievements based on IGC score triggers.
  - TODO: NYI (Not Yet Implemented) for CHAT_ADMIN case.
- lines 3811-4162
  - CFSPlayer: Represents a player in the game.
  - CFSShip: Represents a ship in the game.
  - IshipIGC, ImodelIGC, IstationIGC, IdamageIGC: Interfaces for different types of game objects.
  - ServerHeavyShipUpdate, ServerLightShipUpdate, ServerActiveTurretUpdate: Structures used to update specific aspects of ships and turrets.
  - qsort: Standard C library function for sorting arrays.
  - random: Function for generating random numbers.
  - g.timeNow: Global variable representing the current game time.
  - pcluster: Variable representing the cluster of objects in the game.
- lines 4163-4528
  - MoveShips: Keeps ships in constant motion, detects scan range entrances and exits, and sends appropriate notifications.
  - CreateTreasure: Creates treasure on the server and exports its existence to all clients in the mission.
  - ExportStaticIGCObjs: Not being used but contains code to send static IGC objects to clients.
  - OnMessageBox: Handles message box events by printing them and triggering an event.
  - OnSysMessage: Increments system messages counters when a system message is received.
  - OnSQLErrorRecord: Triggers an event for SQL error records, indicating they may not be fatal.
  - OnOLEDBErrorRecord: Triggers an event for OLEDB error records, indicating they are always fatal.
  - TODO: Remove unused code related to sending FTP site information.
- lines 4529-4882
  - FedSrvSiteBase: Base class for server site handling.
  - OnNewConnection: Handles new connections, logs IP address.
  - OnDestroyConnection: Cleans up after a connection is destroyed.
  - OnSessionLost: Triggers an event when the session is lost.
  - OnBadCRC: Handles messages with bad CRC by dropping the player.
  - OnMessageSent: Callback to update performance counters for sent messages.
  - DisplaceProbe: Adjusts positions to avoid overlap with models or clusters.
  - GetPlayerHandleFromSteamID: Converts Steam ID to a custom player handle.
- lines 4883-5226
  - CASTPFM: Macro for casting a pointer to a specific message type.
  - INACTIVE_TURRET_UPDATE: Message type indicating an inactive turret update.
  - IshipIGC: Interface for ship information.
  - GetCluster, GetParentShip, GetTurretID: Methods of IshipIGC interface.
  - SetInactiveTurretUpdate: Method to set the inactive turret update state.
  - FM_CS_PING: Message type for ping requests.
  - CS, PING: Specific message types within the FM_CS_PING case.
  - g.timeNow: Global variable representing current time.
- lines 5227-5586
  - pfsMission->GetSite()->SendChatf: Sends a chat message to a specific player.
  - pfsPlayer->GetIGCShip()->GetObjectID(): Retrieves the object ID of the player's ship.
  - pfsMission->StartCountdown(1): Starts a countdown for the mission.
  - ForwardChatMessage: A function that forwards chat messages between players and missions.
  - SetWingID: Allows a player to set their wing ID within a mission.
  - TallyVote: Tallys votes from players in a mission.
  - DecrementChatBudget: Decrements the chat budget for a player.
  - GetAutoDonate: Retrieves the auto-donation target of a player's ship.
- lines 5587-5948
  - CFSShip: Represents a ship in the game.
  - CQLogonStats: Manages logon statistics and queries.
  - CQLogonStatsData: Holds data related to logon statistics.
  - FM_VAR_REF: Macro for accessing variables in a message frame.
  - g.pzas: Pointer to the Zone Authentication Service, responsible for handling authentication.
  - g.strAuthServer: The expected authentication server address.
  - g.m_szToken: Token used for authentication.
  - GetStateID: Function that retrieves the state ID based on a character name.
- lines 5949-6318
  - CSteamValidation: Manages Steam validation for player logon.
  - LogonThread: Thread function to handle Steam validation and player rank retrieval.
  - CQLogonStats: Query class for retrieving logon statistics.
  - GotLogonDetails: Callback function for when logon details are retrieved.
  - FM_C_TREASURE_ACK: Message handler for treasure acknowledgment.
  - FM_C_BUY_LOADOUT: Message handler for buying a ship loadout.
  - FM_CS_DROP_PART: Message handler for dropping a part from a ship.
  - pqd: Player query data structure containing login information.
- lines 6319-6689
  - g.fm.ForwardMessage: Sends a message to all players in the same sector.
  - ObjectType: Enumerates different types of objects in the game.
  - CreateTreasure: Creates a treasure object at a specified location.
  - pfsPlayer->GetIGCShip(): Retrieves the player's ship.
  - pfsMission->GetStage(): Gets the current stage of the mission.
  - LeaveShip: Handles leaving a parent ship.
  - BoardShip: Handles boarding another ship.
  - SetCluster: Sets the player's cluster.
- lines 6690-7065
  - pfsMission->RequestPosition: Handles a position request from a player.
  - FM_C_POSITIONACK: Processes a position acknowledgment message.
  - FM_CS_QUIT_MISSION: Handles a mission quit request from a player.
  - FM_CS_QUIT_SIDE: Handles a side quit request from a player.
  - FM_CS_AUTO_ACCEPT: Manages auto-accept settings for sides in a mission.
  - FM_CS_LOCK_LOBBY: Locks or unlocks the lobby of a mission.
  - FM_CS_LOCK_SIDES: Locks or unlocks the sides of a mission.
  - FM_C_RANDOMIZE_TEAMS: Randomizes or flushes teams based on mission parameters.
- lines 7066-7428
  - FM_GUARANTEED: A message flag indicating that a message must be delivered.
  - FM_FLUSH: A message flag indicating that messages should be flushed immediately.
  - NA: A constant representing "Not Applicable" or an invalid ID.
  - pfsPlayer: An object representing the player making the request.
  - pstation: An object representing the station being interacted with.
  - pship: An object representing the ship involved in the operation.
  - pside: An object representing the side (team) of the player or ship.
  - pfsMission: An object representing the current mission.
- lines 7430-7800
  - FM_CS_SET_TEAM_INFO: Handles setting team information.
  - FM_C_CHANGE_ALLIANCE: Handles changing alliances between sides.
  - FM_CS_DELPOSITIONREQ: Handles deletion of player positions.
  - FM_CS_MISSIONPARAMS: Handles mission parameters changes.
  - FM_C_START_GAME: Starts the game if conditions are met.
  - FM_CS_SET_TEAM_LEADER: Sets a new team leader.
  - FM_CS_SET_MISSION_OWNER: Sets a new mission owner, typically by bots.
  - FM_C_RIPCORD_REQUEST: Handles ripcord requests during gameplay.
- lines 7801-8141
  - CreateUniqueMutex: Creates a mutex with a unique name to indicate when the server is running.
  - CheckForNoShows: Ensures missions are terminated if the mission creator never shows up.
  - DisconnectFromLobby: Disconnects from the lobby server.
  - ProcessNetworkMessages: Handles receiving and processing network messages, including periodic roll calls and connection checks.
  - TODO: "KGJV #114 reactivated" - Indicates a feature or fix that was previously deactivated.
  - Unusual Code: Packet Loss Update logic within ProcessNetworkMessages. Conditional compilation for different server types (e.g., SRV_CHILD, SRV_PARENT). Use of wsprintf for string formatting, which is generally discouraged in favor of safer alternatives like snprintf.
- lines 8142-8493
  - UpdateMissions: Handles updating missions and managing game stages.
  - CTimer: A timer class used for timing operations.
  - CFSMission: Represents a mission in the game.
  - STAGE_OVER, STAGE_NOTSTARTED, STAGE_STARTING, STAGE_STARTED: Constants representing different stages of a mission.
  - c_fMissionBriefingCountdown: Time before a countdown starts.
  - GetExePath: Retrieves the path to the executable file.
  - IsCFGFileValid: Validates if a configuration file is valid.
  - BeginConfigDownload: Starts downloading a configuration file from a server.
- lines 8495-8871
  - debugf: Prints debug messages.
  - RetrieveCFGFile: Downloads and processes the configuration file.
  - OnCFGDownloadDone: Handles the completion of the configuration download.
  - DownloadAutoUpdateEXE: Initiates the download of an auto-update executable.
  - AnnouncePause: Notifies the lobby whether the server is paused or not.
  - RestartPausedMissions: Restarts any paused missions when the service resumes.
  - ConnectToLobby: Establishes a connection to the lobby server.
  - TODOs: Fire event for AutoUpdate.exe download error Remove MessageBox call in RetrieveCFGFile for service mode installations Revisit re-advertizing games comment
- lines 8872-9218
  - CFSMission: Represents a mission in the game.
  - FedSrvSite: Represents a site for the server.
  - ProcessMsgPump: Processes window messages while waiting for specified time or events.
  - StartDefaultGames: Starts any games listed in the registry.
  - BEGIN_PFM_CREATE and END_PFM_CREATE: Macros used to create and manage mission data.
  - g.fmLobby: The lobby object for managing game missions.
  - g.pServerCounters: Server counters object.
  - Time::Now(): Returns the current time.
- lines 9219-9584
  - ReceiveThread: Receives messages for a mission and performs periodic processing.
  - LoadSettings: Loads static server settings from the registry.
  - Sleep(1000): Pauses execution for 1 second.
  - CoInitialize(NULL): Initializes COM library on the current thread.
  - _Module.Init(g.hInst): Initializes the ATL module.
  - CComObject<CAdminServer>::CreateInstance(&pServer): Creates an instance of the CAdminServer object.
  - GetAGCGlobal()->RegisterInterfaceInGlobal(pServer->GetUnknown(), IID_IUnknown, &g.dwServerGITCookie): Registers the interface in the GIT.
  - CreateUniqueMutex(): Creates a uniquely-named mutex.
- lines 9585-9939
  - RegQueryValueExA: Queries a value from the Windows registry.
  - StringStartsWith: Checks if one string starts with another.
  - BuildIGCFiles: Builds IGC files from the database.
  - DataClusterIGC, DataWarpIGC, DataAsteroidIGC, DataSideIGC: Structures representing different types of game objects.
  - LoadRegString: Loads a registry string value.
  - DumpIGCStaticCore: Dumps static core data to an IGC file.
  - CreateMission: Creates a new mission object.
  - SectorInfo, AlephsCount, Alephs, Asteroids: SQL queries for retrieving game data.
- lines 9940-10280
  - DataStationIGC: Represents a station with various properties like position, rotation, and type.
  - DataProbeExport: Represents a probe with properties like location, type, and launcher.
  - DataMineExport: Represents a mine with properties like location, type, and launcher.
  - DataTreasureIGC: Represents a treasure with properties like location, amount, and code.
  - RegisterAllSrv: Registers the server as an NT service or EXE.
  - UnregisterAllSrv: Unregisters the server as an NT service or EXE.
  - main: Entry point of the program that handles different modes of operation (service, executable, installation, de-installation).
  - TODO -> propogate this to the database: Indicates a task to propagate data to the database.
- lines 10281-10621
  - SERVICE_TABLE_ENTRY rgSTE[]: Defines service table entries for Windows service registration.
  - fShowUsage: Boolean flag to indicate if usage information should be displayed.
  - g.hInst = GetModuleHandle(NULL): Retrieves the handle of the current module.
  - _Module.TermAGC(): Terminates AGC (Assuming it's a custom module or library).
  - RegisterAllSrv(...): Registers the server as an executable or service.
  - UnregisterAllSrv(argv[0]): Unregisters the server.
  - SteamGameServer_Init(...): Initializes Steam Game Server with specified parameters.
  - SteamGameServer()->LogOnAnonymous(): Logs on to Steam Game Server anonymously.
- lines 10622-10971
  - ThingSiteImpl: Implements the ThingSite interface for managing visibility and interaction of game objects.
  - ShowObject: Static function that handles showing a game object to a specific side.
  - HideObject: Static function that handles hiding a game object from a specific side.
  - UpdateSideVisibility: Virtual function that updates the visibility of a game object for all sides in its cluster.
  - _AGCModule.TriggerEvent: Triggers an event within the Allegiance game module.
  - GetModuleHandle and GetModuleFileName: Used to get the current executable's path.
  - SetCurrentDirectory: Sets the current working directory to the executable's directory.
  - LoadTechBits: Loads technology bits for a specific cheat.
- lines 10972-11321
  - FedSrvSiteBase: Manages sites and clusters for a side.
  - CreateThingSite: Creates a new ThingSiteImpl instance.
  - CreateClusterSite: Creates a new ClusterSiteImpl instance and adds it to the mission.
  - UpgradeDrones: Upgrades any drones docked at stations for a given side.
  - DevelopmentCompleted: Handles completion of development projects, including sending messages and applying effects.
  - SideDevelopmentTechChange: Updates technology changes for a side, including upgrading station types and notifying clients.
  - SideGlobalAttributeChange: Handles global attribute changes for a side, sending notifications to clients.
  - Review: Changes for visibility of static objects (#135) may now require roid to be checked for abilities! -Imago 8/10
- lines 11322-11660
  - FedSrvSiteBase: Manages various events and interactions related to construction and upgrades.
  - HullTypeCompleted: Handles completion of hull type construction for a side.
  - PartTypeCompleted: Handles completion of part type construction for a side.
  - StationTypeCompleted: Handles the completion of station type construction, including special handling for probes and asteroids.
  - BuildStation: Creates a new station on an asteroid, handles upgrades, and manages events related to station creation.
  - DroneTypeCompleted: Handles the completion of drone type construction, including sending messages to team members.
  - UpgradeShip: Upgrades a ship based on the station it is built at.
  - TerminateModelEvent: Placeholder function for handling model termination events.
- lines 11661-12005
  - CFSShip: Represents a ship in the game.
  - IshipIGC: Interface for ship objects.
  - PlayerScoreObject: Manages player scores and achievements.
  - CreateTreasure: Function to create treasure items.
  - KillShipEvent: Handles events related to ship deaths.
  - RespawnFlag: Respawns a flag if it's outside a certain radius.
  - ShipListIGC: List of ships.
  - PartListIGC: List of parts on a ship.
- lines 12006-12345
  - FedSrvSiteBase: Manages events related to ships and stations being damaged or destroyed.
  - DamageShipEvent: Handles damage events for ships, including sending chat messages about attacks.
  - DamageStationEvent: Handles damage events for stations, including sending chat messages about attacks.
  - KillStationEvent: Handles the destruction of a station, updating mission state and sending notifications.
  - BEGIN_PFM_CREATE and END_PFM_CREATE: Macros used to create and send message frames (likely for network communication).
  - SendChatf and SendChat: Functions for sending chat messages to players or teams.
  - CFSShip, CFSPlayer, IstationIGC, ImodelIGC: Classes representing ships, players, stations, and models respectively.
  - g.fm: Likely a global object responsible for managing message frames.
- lines 12346-12690
  - FedSrvSiteBase: Manages events related to various game objects like asteroids, probes, and ships.
  - KillAsteroidEvent: Handles the destruction of an asteroid, sending a message to all sides.
  - DrainAsteroidEvent: Handles the draining of an asteroid's ore, updating the game state accordingly.
  - MineAsteroidEvent: Updates visible asteroids for players when they are mined.
  - KillProbeEvent: Handles the destruction of a probe, notifying all sides.
  - KillBuildingEffectEvent: Manages the termination and notification of building effects.
  - KillTreasureEvent: Handles the destruction of treasure, potentially respawning it.
  - KillMissileEvent: Processes the destruction of a missile, including exploding it if necessary.
- lines 12691-13047
  - FedSrvSiteBase::LandOnCarrierEvent: Handles the event when a ship lands on a carrier.
  - FedSrvSiteBase::RescueShipEvent: Handles the event when one ship rescues another.
  - IpartTypeIGC: Represents a type of part that can be mounted on a ship.
  - IweaponIGC: Represents a weapon that can be mounted on a ship.
  - IshieldIGC: Represents a shield that can be mounted on a ship.
  - ShipLoadout: Holds information about the parts and equipment of a ship's loadout.
  - BEGIN_PFM_CREATE: Macro to start creating a message for transmission.
  - FM_VAR_REF: Macro to reference a variable in a message.
- lines 13048-13405
  - FedSrvSiteBase: Manages events related to space stations and ships.
  - CaptureStationEvent: Handles capturing a station by a ship.
  - DockWithStationEvent: Manages docking of a ship with a station, including handling artifacts and flags.
  - ChangeStation: Handles changing the station of a ship, including launching from allied bases and rearming.
  - GetDroneFromShip: Returns the drone associated with a non-player ship.
  - RequestRipcord: Initiates or aborts a ripcord request for a ship.
  - ContinueRipcord: Determines if a ripcord can continue based on mission parameters and side relationships.
- lines 13406-13759
  - FedSrvSiteBase: Manages events related to ships and their interactions with the game environment.
  - UseRipcord: Handles the logic for using a ripcord on a ship or station.
  - HitWarpEvent: Processes events when a ship hits a warp point.
  - ChangeCluster: Updates the cluster (sector) of a ship.
  - LoadoutChangeEvent: Manages changes to a ship's loadout, such as adding or removing weapons.
  - HitTreasureEvent: Handles events related to ships hitting treasures, including flags and cash.
  - PaydayEvent: Distributes money to a side.
  - SendChat: Sends chat messages between players or entities in the game.
- lines 13760-14126
  - FedSrvSiteBase::LayExpendable: Handles laying expendables like mines or probes on a ship.
  - FedSrvSiteBase::RespawnFlag: Manages respawning flags for sides in the game.
  - FedSrvSiteBase::CreateSideEvent: Creates an FSSide when a side is created in IGC.
  - FedSrvSiteBase::DestroySideEvent: Destroys an FSSide when a side is destroyed in IGC.
  - FedSrvSiteBase::Destroy: Deletes the FedSrvSiteBase instance.
  - PickNewLeader: Selects a new leader for a group of ships based on auto-donate settings.
  - FedSrvApp::AsyncFileOut: Asynchronously writes debug output to a file.
  - FedSrvApp::DebugOutput: Handles the actual logging and debugging output.

## `src/FedSrv/FedSrv.H`

PURPOSE: This file contains common definitions and declarations for the server component of Allegiance, including global variables, constants, and function prototypes.

KEY PARTS:
- FedSrv_Init(): Initializes the server.
- FedSrv_Terminate(): Terminates the server.
- FedSrvSiteBase: Base class for mission sites handling various game events.
- FedSrvSite: Specific implementation of FedSrvSiteBase.
- Global: Contains global variables and settings used by the server.

CONNECTS TO:
- AGCSite.h (AGC library)
- IMissionSite, IIgcSite, IFedMessagingSite, ISQLSite, ISQLSite2 interfaces

NOTES:
- Contains constants like maximum player count and map file path length.
- Defines global variables for network, game state, and performance monitoring.
- Includes function prototypes for server initialization, termination, and various event handling.

## `src/FedSrv/FedSrvApp.h`

PURPOSE: This file defines the `FedSrvApp` class, which handles logging and debugging for the Allegiance server application. It provides functionality to open and close a log file, output debug information, and manage the debug level.

KEY PARTS:
- `FedSrvApp`: Inherits from `Win32App` and manages logging and debugging.
- `OpenLogFile`: Creates a new log file with a timestamped name in the server's directory.
- `CloseLogFile`: Closes the log file handle.
- `DebugOutput`: Outputs debug information to the log file or console based on the current debug level.
- `OnAssert`: Handles assertion failures by logging them and breaking into the debugger if necessary.
- `SetDebug`: Sets the debug level and optionally opens or closes the log file accordingly.
- `GetDebug`: Retrieves the current debug level.

CONNECTS TO: `Win32App` (base class), possibly other logging mechanisms in the client code.

NOTES:
- The log file naming logic has been commented out and replaced with a hardcoded timestamp format.
- The `SRVLOG` macro is used to conditionally compile the server-specific logging code, which is typically enabled only in debug builds.

## `src/FedSrv/allsrv.rgs`

PURPOSE: This file is a Windows registry script for the Allegiance game's server component, defining application and service metadata.

KEY PARTS:
- AppID: Specifies the application ID and name.
- EventLog: Configures event logging for the AllSrv application.

CONNECTS TO: Windows Registry

NOTES: None.

## `src/FedSrv/allsrvmodule.cpp`

PURPOSE:
allsrvmodule.cpp manages service-related functions and COM/ATL support for Allegiance's AllSrv module.

KEY PARTS:
- CServiceModule: Manages service operations like installation, removal, and running as an executable or a Windows service.
- PrintSystemErrorMessage: Prints system error messages along with their descriptions.
- IsInstalled: Checks if AllSrv was properly installed (either as a service or an EXE).
- InitAGC: Initializes AGC, sets up event logging, and hooks for database event logging.
- RunAsExecutable: Handles running the service as a standalone application for debugging purposes.
- ServiceControl: Processes control codes received by the service manager (e.g., stop, pause, continue).
- SetSvcStatus: Updates the service status to reflect its current state.
- MTAKeepAliveThread: Manages the COM Multi-threaded Apartment (MTA) for the service.

CONNECTS TO:
- Windows Service Control Manager
- Allegiance game engine

NOTES:
- Contains TODOs and unusual code using USES_CONVERSION macro and _ATL_REGMAP_ENTRY structure.

PARTS (in file order):

- lines 2-381
  - CServiceModule: Manages the service-related functions and COM/ATL support.
  - PrintSystemErrorMessage: Prints system error messages along with their descriptions.
  - IsInstalled: Checks if AllSrv was properly installed (either as a service or an EXE).
  - IsInstalledAsService: Determines if AllSrv is installed as an NT service.
  - IsInServiceControlManager: Verifies if AllSrv is an NT service by checking the Service Control Manager.
  - InitAGC: Initializes AGC, sets up event logging, and hooks for database event logging.
  - TermAGC: Terminates AGC and unhooks the event logger.
  - GetOutgoingPort / SetOutgoingPort: Manage the last used outgoing port number (only in SRV_PARENT build).
- lines 382-738
  - CServiceModule::get_EventLog: Retrieves the previously created event logger object.
  - CServiceModule::RegisterCOMObjects: Registers COM objects and starts a thread to keep the MTA alive.
  - CServiceModule::RevokeCOMObjects: Unregisters COM objects and signals the MTA keep-alive thread to exit.
  - CServiceModule::RegisterServer: Registers the module in the registry, including service registration if applicable.
  - CServiceModule::UnregisterServer: Unregisters the module from the registry and removes the service from the service control manager.
  - CServiceModule::InstallService: Installs the service into the NT service manager.
  - TODOs: "Imago #167" comment suggesting additional code for modifying service configuration.
  - Unusual Code: Use of USES_CONVERSION macro and _ATL_REGMAP_ENTRY structure for registry manipulation.
- lines 739-1105
  - CServiceModule: Manages service operations like installation, removal, and running as an executable or a Windows service.
  - RunAsExecutable: Handles running the service as a standalone application for debugging purposes.
  - RunAsWindow: Runs the service in a hidden window with a system tray icon.
  - ServiceControl: Processes control codes received by the service manager (e.g., stop, pause, continue).
  - SetSvcStatus: Updates the service status to reflect its current state.
  - ServerTerminateThread: Terminates the server and updates the service status.
  - StartServerTerminateThread: Creates a new thread to terminate the server.
  - HandlerRoutine: Handles control events like close, break, shutdown, and Ctrl+C.
- lines 1106-1366
  - CServiceModule: Manages the service lifecycle and handles control messages.
  - ServiceMain: Entry point for the Windows service, initializes the service and starts the main thread.
  - MPServiceMain: Entry point for the parent process of a multi-process service.
  - StopAllsrv: Signals allsrv to terminate.
  - MTAKeepAliveThunk: Thunk function to call MTAKeepAliveThread with the correct 'this' pointer.
  - MTAKeepAliveThread: Manages the COM Multi-threaded Apartment (MTA) for the service.
  - ServiceHandler: Handles control messages from the service control manager.
  - ThreadProc: Main thread procedure that initializes and runs the service.

## `src/FedSrv/allsrvmodule.h`

**PURPOSE:** This header file defines the `CServiceModule` class and related functions for managing a Windows service in Allegiance. It includes functionality for registering and unregistering COM objects, installing and removing the service, and handling service control events.

**KEY PARTS:**
- **Class: CServiceModule**
  - Manages the lifecycle of the service.
  - Registers and unregisters COM objects.
  - Installs and removes the service as a Windows service.
  - Handles service control events.
  
- **Function: Init**
  - Initializes the module, including setting up COM.

- **Function: RegisterServer**
  - Registers the server as a COM object and optionally registers type libraries.

- **Function: InstallService**
  - Installs the service in the Windows Service Control Manager.

- **Function: RemoveService**
  - Removes the service from the Windows Service Control Manager.

- **Function: ServiceMain**
  - The main entry point for the service, handling service start and control messages.

- **Function: SetCOMStarted**
  - Sets a flag indicating whether COM was started by the server.

- **Data Member: m_fCOMStarted**
  - Tracks whether COM has been initialized by the server.

**CONNECTS TO:** 
- Allegiance's COM infrastructure (`AGC.h`, `AutoHandle.h`)
- Windows Service Control Manager
- Other service-related files and classes

**NOTES:** None.

## `src/FedSrv/counters.h`

**PURPOSE:** This file defines a structure to hold various counters and statistics for the server, including login attempts, packet counts, player activity, and performance metrics.

**KEY PARTS:**
- `SERVER_COUNTERS`: A structure that holds all the server's counter data.
  - `cLoginAttempts`, `cLogins`, `cLoginsFailed`, `cLogoffs`, etc.: Counters for different types of user interactions.
  - `cPacketsIn`, `cPacketsOut`, `cBytesIn`, `cBytesOut`: Network traffic counters.
  - `cPlayersOnline`: Number of players currently connected.
  - `timeIGCWork`, `timeBetweenInnerLoops`, etc.: Performance timing data.

**CONNECTS TO:** Other server components that handle user sessions, network communication, and performance monitoring.

**NOTES:** None.

## `src/FedSrv/dbgutil.h`

PURPOSE: This file contains utility macros and functions for debugging in the Allegiance game engine. It provides assertions, tracing, and other debug-related features.

KEY PARTS:
- `DBG_STRING`: Macro to define a static string.
- `Panic`, `Panic0` through `Panic5`: Macros to handle critical errors with formatted messages.
- `SideAssert`, `AssertSz`: Macros for conditional assertions.
- `DbgAssert`: Function to perform actual assertion checks and log messages.
- `ODS`: Macro to output debug strings.
- `Trace_`, `Trace0` through `Trace5`: Macros for logging trace information.
- `DBG_PROC_ENTRY`: Macro to automatically log entry and exit of functions.
- `DebugInfo`: Structure to hold various debugging options and state.
- `DbgStop`, `FDbgFail`, `DbgSetFail`, `DbgGetFail`: Functions related to handling failures and stopping execution.

CONNECTS TO: Other debug-related files or systems within the Allegiance game engine.

NOTES: None.

## `src/FedSrv/fedsrvsql.h`

PURPOSE: This file contains definitions and macros related to SQL operations in the Allegiance game server. It manages SQL statements, parameters, and execution.

KEY PARTS:
- `ISQLSite`: Interface for SQL site-specific functionality.
- `AddStatement`: Adds a new SQL statement to the system.
- `AddCol`: Adds a column or parameter to an SQL statement.
- `SqlGo`: Executes an SQL statement.
- `SqlGetRow`: Retrieves the next row from an executed SQL statement.
- `InitSql`: Initializes SQL connections and settings.
- `ShutDownSQL`: Shuts down SQL connections.

CONNECTS TO: Allegiance game server core, database management system (DBMS).

NOTES:
- Contains macros for defining SQL statements and parameters in a structured way.
- Uses Windows API functions related to SQL operations.
- Includes conditional compilation directives that may be used to exclude certain parts of the code.

## `src/FedSrv/fscluster.cpp`

PURPOSE:
This file contains the implementation of the `CFSCluster` class, which represents a cluster or sector in the Allegiance game. It manages cluster groups and links to an interface for communication.

KEY PARTS:
- CFSCluster: Manages a cluster/sector, including its groups.
- ~CFSCluster: Cleans up by unlinking from the IGC Cluster.
- m_pIclusterIGC: Pointer to the IClusterIGC interface.
- m_pClusterGroups: Pointer to cluster groups.

CONNECTS TO:
fscluster.h

NOTES:
None.

## `src/FedSrv/fscluster.h`

PURPOSE: Defines the CFSCluster class, which represents a cluster or sector in the game.

KEY PARTS:
- CFSCluster: Manages information about a cluster, including its IclusterIGC interface and flying players.
- AddFlyingPlayer: Adds a player to the list of flying players.
- RemoveFlyingPlayer: Removes a player from the list of flying players.
- GetFlyingPlayers: Returns the list of flying players.
- m_pIclusterIGC: Pointer to the IclusterIGC interface for this cluster.
- m_playersFlying: List of players currently flying in this cluster.

CONNECTS TO: src/FedSrv/fsplayer.h (CFSPlayer class)

NOTES: None.

## `src/FedSrv/fslobby.cpp`

PURPOSE: The file `fslobby.cpp` contains the implementation of the lobby site for the Allegiance server, handling various messages related to mission creation, player management, and other lobby functionalities.

KEY PARTS:
- `FedSrvLobbySite::OnAppMessage`: Handles incoming application messages from clients.
- `FedSrvLobbySite::OnMessageBox`: Displays a message box on the client side.
- `FedSrvLobbySite::OnSessionLost`: Triggers an event when the session is lost.
- `FedSrvLobbySite::OnBadCRC`: Handles bad CRC errors, though it asserts and should not be reached.

CONNECTS TO: 
- `FedMessaging`, `CFMConnection`, `FEDMESSAGE`
- `CFSMission`, `CFSShip`, `CFSPlayer`
- `g.siteFedSrv`

NOTES:
- Contains code for creating missions in a child process if running on the parent server.
- Includes handling of player ranks and DRM updates.
- Asserts on bad CRC errors, indicating these should not occur with internal servers.

## `src/FedSrv/fsmission.cpp`

PURPOSE: Manages mission creation, player management, voting, and game state for Allegiance multiplayer space games.

KEY PARTS:
- CFSMission: Handles mission initialization, player addition/removal, and game lifecycle.
- PlayerScoreObject: Tracks player scores and statistics.
- Ballot: Manages voting processes like mutiny, resignation, and draw offers.
- AddPlayerToSide: Adds a player to a specific side within the mission.
- RemovePlayerFromSide: Removes a player from a side and handles leadership changes.
- SetLeader: Assigns a new leader to a side, updates squad membership, and sends notifications.
- StartGame(): Initializes the game when all ships are ready to start flying.
- DoPayday(): Handles the payday process for each side in the mission.
- GameOver: Handles the end of the game, setting the winner and triggering events.

CONNECTS TO: None

NOTES:
- Contains complex logic for distributing ships among multiple stations without breaking wings.
- Includes TODOs for various improvements and bug fixes.

PARTS (in file order):

- lines 1-355
  - CFSMission: Handles mission creation and management.
  - InitSide: Initializes side-specific data for a mission.
  - InitializeCivList: Populates a list of civilizations for a mission.
  - CFSMission::CFSMission: Constructor for creating a new mission instance.
  - CFSMission::~CFSMission: Destructor for cleaning up resources when a mission is destroyed.
  - s_list: A list to store all active missions.
  - c_cSidesMax: Maximum number of sides in a mission.
  - sideNames: Array containing names for each side.
- lines 356-720
  - CFSMission: Manages a mission, including adding and removing players.
  - AddPlayerToMission: Adds a player to the mission.
  - RemovePlayerFromMission: Removes a player from the mission.
  - AddPlayerToSide: Adds a player to a specific side within the mission.
  - OldPlayerLink: Stores old player data for persistence.
  - PlayerScoreObject: Manages player scores and statistics.
  - FM_VAR_PARM: Macro used in message creation, likely for parameter passing.
  - TODOs: Verify that players are already gone in debug builds. Set server-side default wing to NA! #91
- lines 721-1071
  - CFSPlayer: Represents a player in the game.
  - CFSMission: Manages the mission and its players.
  - GetLeader: Retrieves the leader of a side.
  - SetAutoDonate: Sets the auto-donation behavior for a player.
  - SendMissionInfo: Sends information about the mission to a player.
  - RemovePlayerFromSide: Removes a player from a side and handles team/mission leadership changes.
  - PickNewLeader: Picks a new leader based on donation behavior.
  - HasPlayers: Checks if a side has players.
- lines 1072-1423
  - CFSMission: Manages mission-related operations such as player management, voting, and side changes.
  - AddInvitation: Adds an invitation for a player to join a side in the mission.
  - IsInvited: Checks if a player is invited to any side in the mission.
  - AddBallot: Adds a ballot to the mission's list of ballots.
  - TallyVote: Tallys a vote cast by a player on a specific ballot.
  - HasBallots: Checks if there are any active ballots of a given type in the mission.
  - RemovePlayerByName: Removes a player from the mission based on their character name.
  - RemovePlayerByCDKey: Removes a player from the mission based on their CD Key.
- lines 1424-1776
  - CFSMission: Manages mission-related operations such as setting leaders, stages, and squad information.
  - GetBase: Finds a station with specific capabilities on a given side.
  - GetLeader: Retrieves the leader of a specified side.
  - SetLeaderID: Updates the leader ID for a side and handles associated commands.
  - GetOwner: Returns the owner of the mission.
  - SetOwner: Changes the owner of the mission and notifies clients.
  - SetLeader: Assigns a new leader to a side, updates squad membership, and sends notifications.
  - SetSideName: Sets the name of a side and notifies clients.
- lines 1777-2129
  - CFSMission: Manages mission parameters and state.
  - GiveSideMoney: Adds money to a side's account and processes paydays.
  - SetMissionParams: Updates mission parameters, handles team changes, and enforces rank limits.
  - DelayCountdown: Adjusts the game start time by a specified delay.
  - StartCountdown: Sets or resets the game start time and initializes mission setup.
  - TODOs: "// BT - STEAM - Scores always count in steam!" "// TE: Enforce LockSides = on if ScoresCount mmf change to MaxImbalance"
  - Unusual Code: Conditional compilation based on ALLSRV_STANDALONE for setting bClubGame. Complex logic for distributing ships among multiple stations, ensuring even distribution without breaking wings.
- lines 2130-2477
  - CFSMission: Manages the mission lifecycle and game state.
  - StartGame(): Initializes the game when all ships are ready to start flying.
  - DoPayday(): Handles the payday process for each side in the mission.
  - DoTick(Time timeNow): Processes events and updates the game state at each tick.
  - TODOs: NYI: Alliances may change in-game, create your own AGC Event! NYI: Include alliances info in AGCEvent. "TE, Modify GameStarted AGCEvent to include MissionID."
- lines 2478-2830
  - CFSMission: Manages the game mission logic.
  - GameOver: Handles the end of the game, setting the winner and triggering events.
  - RecordGameResults: Records game results to a database and sends statistics via HTTP POST.
  - MakeOverrideTechBits: Initializes technology bitmasks for sides.
  - POSTGameStats: Sends game statistics to a specified URL using HTTP POST.
  - SaveAsOldPlayer: Saves player data before they leave the mission.
  - Vacate: Kicks players out of the game, moving them to lifepods and terminating drones.
  - TODOs: "TE, Modify GameEnded AGCEvent to include MissionName and MissionID." "BT - STEAM - Making this run for all game over scenarios." "Players who disconnect before the end of the game will not get any stats recorded."
- lines 2831-3184
  - CFSMission: Manages the mission and handles gameover processing.
  - RecordTeamResults: Records team results to the database.
  - RecordPlayerResults: Records player results to the database.
  - QueueGameoverMessage: Queues messages for players indicating they are out of the game.
  - ProcessGameOver: Wraps up things when the game is actually over.
  - GetExpMult: Calculates experience multiplier based on total and side experience.
  - TODOs: "make SURE they're NULL-terminated" (in RecordTeamResults) "KGJV- make sure there is a trailing zero" (in RecordTeamResults) "BT - STEAM - Making this run for all game over scenarios." (in QueueGameoverMessage)
- lines 3185-3534
  - sideID: Array to store side IDs.
  - rank: Array to store rank sums for sides.
  - GetSideRankSum: Function to calculate the rank sum for a side.
  - PlayerScoreObject: Class representing player scores and statistics.
  - SetCharStats: Function to save player character stats.
  - RecordGameResults: Function to record game results.
  - QueueGameoverMessage: Function to queue a game over message.
  - Vacate: Function to clear clusters and destroy drones.
- lines 3535-3883
  - CFSMission: Manages the game mission.
  - CheckForVictoryByStationBuild: Checks for victory by station build.
  - CheckForVictoryByStationCapture: Checks for victory by station capture.
  - CheckForVictoryByStationKill: Checks for victory by station kill.
  - CheckForVictoryByKills: Checks for victory by kills.
  - CheckForVictoryByInactiveSides: Checks for victory when all sides are inactive.
  - CheckForVictoryByFlags: Checks for victory by flags or artifacts.
  - bRestartable: Indicates if the game is restartable.
- lines 3884-4234
  - CFSMission: Manages mission-related operations.
  - CreateDPGroups: Creates groups for a cluster and attaches them to an IGC cluster.
  - SendLobbyMissionInfo: Sends mission information to players in the lobby.
  - QueueLobbyMissionInfo: Queues up a shorter form of mission definition for players not yet choosing a game.
  - SendMissionInfo: Sends detailed mission information to players joining or playing the mission.
  - "mdvalley: Discover listening port" - Potential TODO comment.
  - "char szAddr[64]= "XXX-YYY-ZZZ-TTT";": Placeholder for server address, likely needs updating.
  - "DWORD dwPort = 0;": Variable declared but not used in the provided snippet.
- lines 4235-4590
  - CFSMission: Manages game missions.
  - MineLinkIGC: Represents a mine link in the game.
  - ProbeListIGC: Represents a list of probes.
  - BucketLinkIGC: Represents a bucket link.
  - ShipLinkIGC: Represents a ship link.
  - SideLinkIGC: Represents a side link.
  - GetMission: Retrieves a mission by cookie.
  - GetMissionFromIGCMissionID: Retrieves a mission by IGCMissionID.
- lines 4591-4935
  - CFSMission: Manages mission state and player positions.
  - CheckPositionRequest: Determines if a player can join or rejoin a side.
  - RequestPosition: Handles player requests to change sides.
  - VacateStation: Kicks players out of a station except those on the owning side.
  - AddJoinRequest: Adds a player to the join request list when they cannot be auto-accepted.
  - NotifyPlayerBoot: Notifies all players that a player has been removed from a side.
  - RejectSideJoinRequests: Rejects and removes all pending join requests for a specific side.
  - TODO: Revisit team balancing logic, especially newbstack implementation.
- lines 4936-5279
  - CFSMission: Manages mission-related operations like player requests and side management.
  - RemoveJoinRequest: Removes join requests for a specific player or all players if no player is specified.
  - SetAutoAccept: Turns auto-accept on or off for sides, handling both individual sides and all sides when passed NULL.
  - SetLockLobby: Locks or unlocks the lobby for the game.
  - SetLockSides: Locks or unlocks the sides for the game.
  - SetMaxTeamImbalance: Sets the maximum team imbalance parameter for the mission.
  - FlushSides: Removes all players (except commanders) from all sides and puts them in the team lobby.
  - GetLockSides: Retrieves the status of the LockSides game parameter.
- lines 5281-5625
  - CFSMission: Manages mission state and logic.
  - PlayerScoreObject: Represents a player's score object.
  - SetSideAllies: Sets the alliance status for a side.
  - GetSideAllies: Retrieves the alliance status for a side.
  - SetSideActive: Activates or deactivates a side.
  - GetSideActive: Checks if a side is active.
  - DeactivateSide: Marks a side as inactive and handles cleanup.
  - SetForceReady: Forces a side to be ready.
- lines 5626-5973
  - CFSMission::UpdateAlliances: Manages and updates alliances between sides in a mission.
  - Ballot::Update: Handles updating the status of a ballot based on time or votes.
  - Ballot::Cancel: Cancels an ongoing ballot.
  - Ballot::CastVote: Records a vote from a player.
  - Ballot::GetBallotID: Returns the ID of the ballot.
  - Ballot::GetType: Retrieves the type of the ballot.
  - Ballot::Init(CFSPlayer): Initializes a ballot proposed by a player.
  - Ballot::Init(CFSSide): Initializes a ballot proposed by a team.
- lines 5974-6194
  - Ballot: Manages voting processes.
  - MutinyBallot: Handles mutiny proposals.
  - ResignBallot: Handles resignation proposals.
  - OfferDrawBallot: Handles draw offers.
  - AcceptDrawBallot: Handles accepting a draw offer.
  - HasPassed: Checks if the vote has passed.
  - AllVotesAreIn: Checks if all votes are received.
  - GetTallyString: Generates a string describing the vote tally.

## `src/FedSrv/fsmission.h`

PURPOSE: This file defines the class and structures for managing missions in Allegiance, including mission creation, player management, side operations, and voting mechanisms.

KEY PARTS:
- CFSMission: Manages the lifecycle of a mission, including adding/removing players, updating game state, and handling victory conditions.
- Ballot: Base class for various types of votes (resignation, draw offer/acceptance, mutiny), managing vote counts and outcomes.
- JoinRequest: Tracks player requests to join specific sides in a mission.

CONNECTS TO: Other mission-related files like `fsplayer.h`, `fsside.h`, and `imissionsite.h`.

NOTES: None.

## `src/FedSrv/fsship.cpp`

PURPOSE: Manages ship and player interactions, including status updates, warp jumps, loadouts, and side affiliations.

KEY PARTS:
- CFSShip: Base class for all ships, handling basic ship operations.
- CFSPlayer: Manages player-specific data and interactions with ships.
- CFSDrone: Represents AI-controlled drones, extending the base ship class.
- HitWarp: Handles warp jumps between clusters.
- Launch: Sends ships into space from stations.
- ShipStatusSpotted: Updates ship status when spotted by an enemy side.
- CaptureStation: Manages logic for capturing stations by ships.
- SetCluster: Updates the cluster of a ship and handles related notifications.
- QueueLoadoutChange: Queues changes in ship loadouts for players.
- Launch: Initiates the launch process for player's ships.
- Dock: Handles docking of player's ships at stations.
- SetSide: Updates the side affiliation of ships and players.
- ShipStatusRecalculate: Recalculates the status of ships based on mission conditions.

CONNECTS TO: Allegiance game engine, DPlay API for multiplayer functionality.

NOTES:
- Contains risky code comments regarding potential issues with player destruction.

PARTS (in file order):

- lines 1-355
  - CFSShip: Base class for player and drone ships.
  - CFSPlayer: Represents a player-controlled ship.
  - CFSDrone: Represents an AI-controlled drone.
  - HitWarp: Handles warp jumps between clusters.
  - Launch: Sends the ship into space from a station.
  - AnnounceExit: Notifies other players when a ship exits the game.
  - ForceLoadoutChange: Forces a change in the player's loadout.
  - ShipStatusSpotted: Updates the ship status when spotted by an enemy side.
- lines 356-694
  - CFSShip: Manages ship status and interactions.
  - ShipStatus: Holds state information for a ship regarding its position, parent, and station.
  - CaptureStation: Handles logic when a station is captured by a ship.
  - SetCluster: Updates the cluster of a ship and handles related notifications.
  - ShipStatusRecalculate: Recalculates the status of ships based on current mission conditions.
  - ShipStatusLaunched: Sets the initial state for a launched ship.
  - ShipStatusStart: Initializes the status when a ship starts at a station or dies.
  - ShipStatusExit: Resets the status when a ship exits its current state.
- lines 696-1051
  - CFSPlayer: Manages player-specific data and interactions.
  - SetCluster: Updates the player's cluster and handles various updates for other ships and stations within the cluster.
  - QueueLoadoutChange: Queues a change in ship loadout for the player.
  - Launch: Initiates the launch process for the player's ship.
  - Dock: Handles docking of the player's ship at a station.
  - SetSide: Updates the player's side affiliation.
  - "// NYI: They might be in sector overview too" - Not Implemented
  - "Too risky, dplay may block-- g.fm.GetDPlay()->DestroyPlayer(m_dpid);": Risky code comment
- lines 1052-1460
  - CFSShip::SetSide: Updates the side of a ship and handles related score and group changes.
  - CFSPlayer::SetShipUpdate: Processes updates to a player's ship, including handling warp states.
  - CFSPlayer::SetActiveTurretUpdate: Handles updates to active turrets on a player's ship.
  - CFSPlayer::SetInactiveTurretUpdate: Resets the state of inactive turrets.
  - CFSPlayer::SetDPGroup: Manages the primary group (flying or docked) for a player in a mission.
  - CFSPlayer::IsMissionOwner: Checks if the player is the owner of the current mission.
  - CFSPlayer::SetReady: Updates the readiness status of a player and notifies the mission.
  - CFSPlayer::SetAutoDonate: Manages automatic donations between players, ensuring proper handling of autodonations.

## `src/FedSrv/fsship.h`

PURPOSE: This file defines the base class `CFSShip` and its derived classes `CFSPlayer` and `CFSDrone`, representing ships in the Allegiance game. It includes methods for managing ship status, updates, and interactions with other game elements.

KEY PARTS:
- CFSShip: Abstract base class for all ships, providing common functionality like money management, status updates, and side changes.
- CFSPlayer: Derived from CFSShip, represents player-controlled ships. Includes additional properties and methods specific to players, such as chat budget, squad membership, and achievements.
- CFSDrone: Derived from CFSShip, represents drone ships. Includes unique properties like drone type ID.

CONNECTS TO:
- IshipIGC, IsideIGC, IstationIGC, and other game interface classes for ship operations and interactions.

NOTES:
- Contains macros and constants related to ship management.
- Some functions are commented out or disabled (e.g., bandwidth patch).
- Includes friend functions for internal system integration.

## `src/FedSrv/fsside.cpp`

PURPOSE: This file contains the implementation of the `CFSSide` class, which manages a side in a mission within the Allegiance game. It handles invitations and squad management.

KEY PARTS:
- CFSSide: Manages a side in a mission.
- IsInvited: Checks if a player is invited to the side.
- AddInvitation: Adds an invitation for a side.
- GetInvitedSquadName: Retrieves the name of the invited squad.

CONNECTS TO: `IsideIGC`, `CFSMission`, and related classes for managing sides, missions, and players.

NOTES: None.

## `src/FedSrv/fsside.h`

**PURPOSE:** The file `fsside.h` defines the class `CFSSide`, which represents a side or team in a mission within the Allegiance game. It manages player invitations and provides access to related mission and group data.

**KEY PARTS:**
- **Class: CFSSide** - Manages a side in a mission, including handling player invitations.
  - **Constructor:** Initializes the side with an IsideIGC interface and a mission pointer.
  - **Destructor:** Cleans up resources.
  - **GetMission():** Returns the mission associated with this side.
  - **GetSideIGC():** Returns the IsideIGC interface for this side.
  - **GetGroup():** Returns the group associated with this side.
  - **FromIGC(IsideIGC * pside):** Static method to retrieve a CFSSide instance from an IsideIGC pointer.
  - **IsInvited(CFSPlayer * pfsPlayer):** Checks if a player is invited to this side.
  - **AddInvitation(const char * szPlayerName):** Adds a player name to the list of invitations.
  - **GetInvitedSquadName():** Returns the name of the squad being invited.

- **Data Member: m_Invitations** - A vector storing names of players invited to this side.

**CONNECTS TO:** `CFSMission`, `CFMGroup`, and `IsideIGC`.

**NOTES:** None.

## `src/FedSrv/pch.cpp`

PURPOSE: tiny file (17 lines), shown in full rather than summarized.

    /*-------------------------------------------------------------------------
     * pch.cpp
     *
     * Precompiled header for fedsrv
     *
     * Owner:
     *
     * Copyright 1986-1998 Microsoft Corporation, All Rights Reserved
     *-----------------------------------------------------------------------*/
    #include "pch.h"
    #include <atlbase.h>
    #ifdef _ATL_STATIC_REGISTRY
    #include <statreg.h>
    //#include <statreg.cpp> Imago removed (again)
    #endif
    //#include <atlimpl.cpp>
    #include <../TCAtl/UserAcct.cpp>

## `src/FedSrv/pch.h`

PURPOSE: This file includes precompiled headers for the fedsrv project, which is part of the Allegiance game server. It provides a common set of include files and macros to be used across multiple source files in the project.

KEY PARTS:
- `Global g`: A global object that likely holds shared state or configuration.
- `fedsrv.h`: The main header file for the fedsrv project.
- `AllSrvModule.h`: Header for the main module class of the server.
- `Zone.h`, `fsship.h`, `AdminGame.h`, etc.: Headers for various classes related to game zones, ships, and administrative functions.

CONNECTS TO: Other Allegiance game server components and libraries such as ATL (Active Template Library), ZLib, and Steam SDK.

NOTES:
- The file includes a lot of system headers and custom project-specific headers.
- There are conditional includes based on macros like `USEAUTH` and `ALLSRV_STANDALONE`.
- Some commented-out or disabled code is present, such as references to `dplobby.h`, `ZoneAuthDecrypt.h`, and `SWMRG.h`.

## `src/FedSrv/regini.txt`

PURPOSE: This file lists registry keys and their values that are used to configure the AllSrv service in Allegiance.

KEY PARTS:
- fWantInt3: A DWORD value indicating whether internal interrupts should be enabled.
- fWatchArtDir: A DWORD value indicating whether to watch the artwork directory for changes.
- ArtPath: The path to the server's artwork directory.
- FTPArtDir, FTPArtServer, FTPArtRoot, FTPAccount, FTPPW: Configuration values related to FTP access for artwork.
- fTimeout: A DWORD value setting a timeout period.
- SQLUser, SQLPW: Credentials for accessing the SQL database.

CONNECTS TO: setupserver.vbs

NOTES: This file is not used directly; its values are set dynamically by setupserver.vbs.

## `src/FedSrv/resource.h`

PURPOSE: This file contains resource definitions used by the FedSrv application, including icons, menus, dialog IDs, and string identifiers.

KEY PARTS:
- ICO_FEDSRV: Icon for the application.
- MNU_FEDSRV: Menu identifier for the application.
- IDD_PREFERENCES: Dialog ID for preferences settings.
- IDC_EDIT_HTTPART: Control ID for an HTTP part edit box.
- IDS_SERVICENAME: String identifier for the service name.

CONNECTS TO: FedSrv.RC (Resource Compiler file)

NOTES: None.

## `src/FedSrv/sqlhelp.cpp`

**PURPOSE:**  
This file contains helper functions for managing SQL operations in the Allegiance game server. It includes functions for setting up and tearing down ODBC connections, executing queries, and handling errors.

**KEY PARTS:**
- `SQLWhatsWrong`: Diagnoses and handles errors from ODBC calls.
- `AddStatement`: Initializes a new SQL statement and binds it for use.
- `AddCol`: Binds variables to columns or parameters in the current SQL statement.
- `ParseCommandLine`: Parses command-line arguments and retrieves configuration values.
- `InitSql`: Sets up the ODBC environment, connection, and user credentials.
- `ShutDownSQL`: Cleans up the ODBC resources.
- `SqlGo`: Executes a prepared SQL statement, handling retries for deadlocks.
- `SqlGetRow`: Fetches the next row from a result set.

**CONNECTS TO:**  
`pch.h`, `sql.h`, `sqlext.h`

**NOTES:**  
- Contains a hack to handle a specific error code (7312).
- Includes logic to retry queries in case of deadlocks.
- Uses global variables for managing SQL handles and states, which could be improved for better maintainability.

## `src/FedSrv/srvqguids.cpp`

PURPOSE: tiny file (17 lines), shown in full rather than summarized.

    /*-------------------------------------------------------------------------
      srvqguids.cpp
      Guids for srvqueries
      Owner:
      Copyright 1986-2000 Microsoft Corporation, All Rights Reserved
     *-----------------------------------------------------------------------*/
    #include "pch.h"
    #define SQLGUID(X) GUID X::s_guid = GUID_NULL;
    SQLGUID(CQLogonStats)
    SQLGUID(CQCharSquads)
    SQLGUID(CQCharStats)
    SQLGUID(CQReportSquadGame)
    SQLGUID(CQGameResults)
    SQLGUID(CQTeamResults)
    SQLGUID(CQPlayerResults)
    SQLGUID(CQGetInvitationList)
    SQLGUID(CQLogEvent)

## `src/FedSrv/srvqueries.h`

PURPOSE: This file contains definitions and declarations for various database queries used in the Allegiance game server. These queries handle operations related to player logons, character stats, game results, team results, player results, invitations, and event logging.

KEY PARTS:
- CQLogonStats: Manages player logon statistics.
- CQCharSquads: Retrieves information about player squads.
- CQCharStats: Updates or retrieves character statistics.
- CQReportSquadGame: Reports the outcome of a squad game.
- CQGameResults: Inserts game results into the database.
- CQTeamResults: Inserts team results into the database.
- CQPlayerResults: Inserts player results into the database.
- CQGetInvitationList: Retrieves invitation list data.
- CQLogEvent: Logs events in the system.

CONNECTS TO: allegdb.h (database handling library)

NOTES:
- Contains TODOs and workarounds for compiler issues.
- Some queries include dummy fields to address bugs.
