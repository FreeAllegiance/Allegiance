# src/lobby

[Back to the index](../LLAMA_SUMMARY.md) - 32 files, 5,837 lines. Notes by a local model, to orient by: not verified facts.

The `src/lobby` folder contains essential components of Allegiance's lobby system, responsible for managing player logins, mission creation, and server interactions.

- AutoUpdate.cpp: Manages the AutoUpdate system, checking file integrity via FTP.
- client.cpp: Handles client-side logic for player logins and interactions within the lobby.
- lobbyapp.cpp: Manages the lobby application, including initialization and message processing.
- lobby.cpp: Registers and manages the lobby service as a Windows service.
- server.cpp: Implements server-related functionality for managing server connections and events.

Largest files: lobbyapp.cpp (1180 lines), client.cpp (608 lines), lobby.cpp (576 lines), server.cpp (553 lines), zgameinfo.h (327 lines).

## `src/lobby/AutoUpdate.cpp`

PURPOSE: Manages the AutoUpdate system in Allegiance, responsible for loading settings and checking file integrity via FTP.

KEY PARTS:
- CAutoUpdateImpl: Implements IAutoUpdate interface to handle auto-update functionality.
- LoadSettings: Reads configuration from registry and validates FTP server details.
- LoadCRC: Opens a file, calculates its CRC, and logs errors if unsuccessful.
- GetFTPServer, GetFTPInitialDir, GetFTPAccount, GetFTPPassword: Accessors for FTP credentials.
- GetFileListCRC, GetFileListSize: Provides file list integrity information.

CONNECTS TO: Registry for settings, FTP server for updates, and files for CRC calculation.

NOTES: None.

## `src/lobby/AutoUpdate.h`

PURPOSE: tiny file (20 lines), shown in full rather than summarized.

    /*-------------------------------------------------------------------------
      AutoUpdate.h
      Management of the AutoUpdate system
      Owner:
      Copyright 1986-2000 Microsoft Corporation, All Rights Reserved
     *-----------------------------------------------------------------------*/
    class IAutoUpdate
    {
    public:
        virtual void     LoadSettings(HKEY hk)           = 0;
        virtual void     LoadCRC(char * szFileName)      = 0;
        virtual char *   GetFTPServer()      = 0;
        virtual char *   GetFTPInitialDir()  = 0;
        virtual char *   GetFTPAccount()     = 0;
        virtual char *   GetFTPPassword()    = 0;
        virtual int      GetFileListCRC()    = 0;
        virtual unsigned GetFileListSize()   = 0;
    };
    void CreateAutoUpdate(HKEY hk, char * szFileName);
    extern IAutoUpdate * g_pAutoUpdate; // this is NULL is g_pAutoUpdate is not enabled

## `src/lobby/CSteamValidation.cpp`

PURPOSE: This file contains the implementation of Steam validation for user authentication in the Allegiance game lobby.

KEY PARTS:
- CSteamValidation: Class responsible for initiating and managing Steam authentication.
- BeginSteamAuthentication: Function to start Steam authentication process.
- OnValidateAuthTicketResponse: Callback function to handle Steam authentication response.

CONNECTS TO: 
- CSQLQuery: For database query operations.
- CQLobbyLogonData: To store logon data and validation results.
- SteamGameServer API: For Steam-specific authentication functions.

NOTES: None.

## `src/lobby/CSteamValidation.h`

PURPOSE: tiny file (17 lines), shown in full rather than summarized.

    #pragma once
    class CSteamValidation
    {
    private:
    	DWORD			m_dwThreadID;
    	CSQLQuery *		m_sqlQuery;
    	CQLobbyLogonData *	m_lobbyLogonData;
    	int				m_characterID;
    	bool			m_bAuthenticationComplete = false;
    public:
    	CSteamValidation(DWORD dwThreadID, int characterID, CSQLQuery * pQuery, CQLobbyLogonData * pqd);
    	void BeginSteamAuthentication();
    private:
    	// Tells us a client has been authenticated and approved to play by Steam (passes auth, license check, VAC status, etc...)
    	STEAM_GAMESERVER_CALLBACK(CSteamValidation, OnValidateAuthTicketResponse, ValidateAuthTicketResponse_t);
    	void OnValidateAuthTicketResponse(CSteamID m_SteamID, EAuthSessionResponse eAuthSessionResponse, CSteamID ownerSteamID);
    };

## `src/lobby/LobbyAppSite.h`

PURPOSE: tiny file (13 lines), shown in full rather than summarized.

    #pragma once
    #ifndef __LobbyAppSite_h__
    #define __LobbyAppSite_h__
    /////////////////////////////////////////////////////////////////////////////
    // LobbyAppSite.h
    //
    class ATL_NO_VTABLE ILobbyAppSite
    {
    public:
      virtual int LogEvent(WORD wType, int id, ...) = 0;
    };
    /////////////////////////////////////////////////////////////////////////////
    #endif // !__LobbyAppSite_h__

## `src/lobby/broadcast.cpp`

PURPOSE: This file contains implementation of UDP socket operations for broadcasting and listening in the Allegiance game.

KEY PARTS:
- UdpSocket: Manages a UDP socket for sending and receiving data.
- Broadcast: Specialized class for broadcasting messages using UDP.
- Listen: Class for listening to incoming UDP messages.

CONNECTS TO: 
- pch.h: Precompiled header file
- broadcast.h: Header file declaring the classes

NOTES:
- The commented-out sections of code suggest that some features were intended but not implemented, such as setting socket options and handling TCP/IP ports.
- DebugPrint macro is used for debugging output, which can be enabled by defining the DebugPrint macro.

## `src/lobby/broadcast.h`

PURPOSE: This file defines a class `UdpSocket` for handling UDP socket operations to send and receive broadcasts across routers.

KEY PARTS:
- UdpSocket: Manages opening a UDP port, binding it to an address and port, enabling broadcasting, sending data to a specific address and port, and receiving data.
- Bind: Binds the socket to a specified port and optionally an IP address.
- Close: Closes the socket.
- EnableBroadcast: Enables broadcast mode on the socket.
- SendTo: Sends data to a specified address and port.
- Recv: Receives data from the socket.

CONNECTS TO: winsock.h (Windows Sockets API).

NOTES: None.

## `src/lobby/client.cpp`

**PURPOSE:**  
This file contains the implementation of client-side logic for managing player logins and interactions within a multiplayer game lobby. It handles various messages from clients, processes authentication, manages mission creation and joining, and maintains session state.

**KEY PARTS:**
- `LobbyClientSite::OnAppMessage`: Handles application-specific messages received from clients.
- `QueueMissions`: Sends mission information to connected clients.
- `GotLogonInfo`: Callback function for logon queries, processes authentication results.
- `LogonThread`: Thread function for handling Steam-based authentication.
- `encodeURL`: URL encodes strings for safe transmission.

**CONNECTS TO:**  
Depends on various game systems including authentication, mission management, and network communication.

**NOTES:**
- Contains code related to Steam-based authentication.
- Includes commented-out sections of old authentication logic.
- Has TODOs for handling auto-update and other features.

## `src/lobby/client.h`

PURPOSE: This file defines the `CFLClient` class and a utility function for URL encoding. The `CFLClient` class manages client connections in the lobby system.

KEY PARTS:
- CFLClient: Manages client connections by maintaining a link between the connection object (`CFMConnection`) and its corresponding client instance.
- encodeURL: A utility function to encode URLs using a given token.

CONNECTS TO: 
- `CFMConnection`: The class that represents a connection in the lobby system.

NOTES:
- None.

## `src/lobby/counters.h`

PURPOSE: This file defines structures and types used for tracking various counters in the game lobby system.

KEY PARTS:
- LOBBY_COUNTERS: A structure containing various counters related to player activity, mission counts, and queue lengths.
- PER_SERVER_COUNTERS: A structure detailing counters specific to each server, including player count, mission count, and load percentage.

CONNECTS TO: lobby.cpp

NOTES: None.

## `src/lobby/lobby.cpp`

PURPOSE:
The file `Lobby.cpp` is part of the Allegiance game's lobby service implementation. It handles the registration and management of the lobby service, including installation, uninstallation, and running as a Windows service.

KEY PARTS:
- `CServiceModule`: Manages the lifecycle of the service, including initialization, registration, and handling control requests.
- `RegisterServer`, `UnregisterServer`: Functions to register or unregister the service with the system.
- `InstallService`, `UninstallService`: Implement the actual installation and uninstallation of the service in the Windows Service Control Manager (SCM).
- `Start`, `Run`: Entry points for starting the service and running its main logic.

CONNECTS TO:
- Other parts of the Allegiance game, particularly the lobby application (`CLobbyApp`).

NOTES:
- The code includes logging functionality to report events through the Windows event log.
- There is a commented-out section related to RPC initialization that was likely used for remote procedure calls but is not currently active.

## `src/lobby/lobby.h`

PURPOSE: tiny file (1 lines), shown in full rather than summarized.

    /* MIDL: this ALWAYS GENERATED file contains the definitions for the interfaces */

## `src/lobby/lobby.rgs`

PURPOSE: This file is a Windows registry script used to register the Allegiance lobby application with the system.

KEY PARTS:
- AppID: Registers the application ID for the lobby.
- AllLobby.EXE: Specifies the executable file for the lobby application.
- EventLog: Configures event logging for the lobby application, specifying that it should log informational, warning, and error messages.

CONNECTS TO: Windows registry system.

NOTES: None.

## `src/lobby/lobbyapp.cpp`

**PURPOSE:** The `lobbyapp.cpp` file manages the lobby application for Allegiance, handling initialization, message processing, performance counters, and game information management.

**KEY PARTS:**
- **Class: CLobbyApp**: Manages the lobby application.
- **Function: ProcessMsgPump()**: Handles Windows messages.
- **Function: SetConstantGameInfo()**: Sets constant game information.
- **Function: SetVariableGameInfo()**: Updates variable game information.
- **Function: SendGameInfo()**: Sends updated game information to servers.
- **Constructor: CLobbyApp(ILobbyAppSite * plas)**: Initializes the lobby application.
- **Destructor: ~CLobbyApp()**: Cleans up resources and shuts down.
- **Function: Init()**: Initializes performance counters, hosts sessions, reads registry settings, and sets up authentication.
- **Function: UpdatePerfCounters()**: Updates performance counters with current data.
- **Function: RollCall()**: Checks for dead servers.
- **Function: CheckAndUpdateDrmHashes(bool forceUpdate)**: Updates DRM hashes.
- **TODO: Make keep-alives an option**: Suggests adding a configuration option.

**CONNECTS TO:** Other systems and files related to server management, performance counters, and game information.

**NOTES:** The file includes commented-out sections for authentication checks and calls to `CheckAndUpdateDrmHashes(false)`, indicating potential previous implementations or integration with Steam.

## `src/lobby/lobbyapp.h`

PURPOSE: The file `LobbyApp.h` defines the main class for the lobby application in Allegiance, including its lifecycle methods and various utility functions.

KEY PARTS:
- CLobbyApp: Main class that manages the lobby application's lifecycle, handles messaging, authentication, and player management.
- LobbyClientSite and LobbyServerSite: Implement `IFedMessagingSite` to handle messages from clients and servers respectively.
- Init() and Run(): Lifecycle methods for initializing and running the lobby application.
- GetFMServers() and GetFMClients(): Accessors for the FedMessaging objects handling server and client connections.
- CDKeyIsValid(): Validates a CD key for player authentication.
- SetPlayerMission() and RemovePlayerFromMission(): Manage player missions.

CONNECTS TO: Depends on `FedMessaging`, `IFedMessagingSite`, and other Allegiance components like `Win32App` and `ISQLSite2`.

NOTES: None.

## `src/lobby/lobbyevents.cpp`

**PURPOSE:**  
This file contains a list of lobby event messages used in the Allegiance game. These messages are likely used for logging and debugging purposes to track various events within the lobby system.

**KEY PARTS:**
- `g_rgszLobbyEvents[]`: An array of strings representing different lobby event messages.
- `Registry String Missing/Invalid`, `Registry Integer Missing/Invalid`: Messages indicating issues with registry entries.
- `Allegiance Lobby Service Handler not installed`, `Allegiance Lobby Service stopped`: Messages related to the service status.
- `Bad service request`, `Server %s was dropped for missing roll call`: Messages indicating errors or issues with server connections.

**CONNECTS TO:**  
`pch.h`: Precompiled header file likely containing common includes and macros used throughout the project.

**NOTES:**  
None.

## `src/lobby/lobbyevents.h`

PURPOSE: This file defines constants and event identifiers for lobby-related events in the Allegiance game.

KEY PARTS:
- LobbyEventBaseID: Base ID for lobby events.
- enum LobbyEvent: Enumerates various lobby events with unique IDs.
- g_rgszLobbyEvents[]: Array of strings corresponding to each lobby event.

CONNECTS TO: None explicitly mentioned, but likely used by other parts of the game that handle lobby operations.

NOTES: None.

## `src/lobby/lobbymodule.h`

PURPOSE: This file defines a service module for Allegiance's lobby system, responsible for managing the lobby application as a Windows service.

KEY PARTS:
- CServiceModule: Main class derived from CComModule and implementing ILobbyAppSite, handles service registration, initialization, and event handling.
- RegisterServer: Registers the server with or without type library and service account details.
- UnregisterServer: Unregisters the server.
- Init: Initializes the module with service name and description.
- Start: Starts the service.
- ServiceMain: Entry point for the service main function.
- ExeMain: Main execution loop for non-service mode.
- Handler: Handles service control messages.
- Run: Runs the lobby application.
- GetModulePath: Retrieves the path of the module.
- IsInstalled: Checks if the service is installed.
- InstallService: Installs the service with specified account and password.
- Install: Installs the service without specifying account details.
- Uninstall: Uninstalls the service.
- Unlock: Releases any locks held by the module.
- SetServiceStatus: Updates the status of the service.
- SetupAsLocalServer: Configures the server as a local server.
- ReadFromRegistry: Reads data from the registry.

CONNECTS TO: LobbyAppSite interface, Windows service management APIs, Allegiance lobby application logic.

NOTES: None.

## `src/lobby/lobqguids.cpp`

PURPOSE: tiny file (12 lines), shown in full rather than summarized.

    /*-------------------------------------------------------------------------
      LobQGuids.cpp
      Guids for LobQueries
      Owner:
      Copyright 1986-2000 Microsoft Corporation, All Rights Reserved
     *-----------------------------------------------------------------------*/
    #include "pch.h"
    #define SQLGUID(X) GUID X::s_guid = GUID_NULL;
    //Imago removed - 7/5/08
    //#ifdef USEAUTH
    SQLGUID(CQLobbyLogon)
    //#endif

## `src/lobby/lobqueries.h`

PURPOSE:
This file contains database queries specifically for the lobby system in Allegiance. It defines a query to retrieve logon information from the database.

KEY PARTS:
- CQLobbyLogon: Defines a database query to get lobby logon information.
- BEGIN_QUERY and END_QUERY: Macros used to define the structure of the query.
- CQLobbyLogonData: Class that holds data for the query, including input parameters and output columns.
- BEGIN_PARAM_MAP and END_PARAM_MAP: Macros defining how input parameters are mapped to database columns.
- BEGIN_COLUMN_MAP and END_COLUMN_MAP: Macros defining how output columns are mapped to class members.

CONNECTS TO:
allegdb.h - The Allegiance database interface header file.

NOTES:
- Contains macros for query definition, parameter mapping, and column mapping.
- Uses SAFE mechanism for connection ID.
- Includes Steam authentication fields if not disabled.

## `src/lobby/messagesall.h`

PURPOSE: This file defines messages that are used across multiple sessions in the Allegiance game lobby. It includes information about missions and squad memberships.

KEY PARTS:
- `DEFINE_FEDMSG`: Macro to define message structures.
- `LS`: Namespace for lobby-related messages.
- `LOBBYMISSIONINFO`: Message containing details about a mission, such as game name, player limits, and game modes.
- `MISSION_GONE`: Message indicating that a mission has been removed or is no longer available.
- `SquadMembership`: Class representing a squad's membership, including the squad ID, name, leader status, and assistant leader status.
- `SquadMembershipList` and `SquadMembershipLink`: Types for managing lists of squad memberships.

CONNECTS TO: `messagecore.h`, which contains core message handling definitions.

NOTES: None.

## `src/lobby/messageslc.h`

PURPOSE: This file defines message structures and constants for communication between the Allegiance game lobby and clients. It includes various messages for logon, logoff, mission creation, joining games, finding players, and server information.

KEY PARTS:
- `DEFINE_FEDMSG`: Macro to define a federated message with a unique ID.
- `LOGON_LOBBY_OLD`, `LOGOFF_LOBBY`, etc.: Specific message types with associated fields.
- `ServerCoreInfo`: Struct for storing core and server information.

CONNECTS TO: `messagecore.h` (depends on)

NOTES:
- The file includes both client-to-lobby (`C_`) and lobby-to-client (`L_`) messages.
- There are two logon message types (`LOGON_LOBBY_OLD` and `LOGON_LOBBY`) to support older clients that need auto-update.
- Steam authentication details are included in the logon message for compatibility with Steam users.

## `src/lobby/messagesls.h`

PURPOSE: This file defines messages and their structures for communication between the Lobby and AllSrv components of Allegiance.

KEY PARTS:
- `DEFINE_FEDMSG`: Macro used to define message types with specific IDs and variables.
- `LOGON_LOBBY`, `LOGOFF_LOBBY`, etc.: Specific message types defined for various interactions like server logon, player joins, pauses, and more.
- `FM_VAR_ITEM`: Macro used within message definitions to specify the data items each message contains.

CONNECTS TO: MessageCore.h

NOTES: None.

## `src/lobby/mission.cpp`

PURPOSE: This file contains the implementation for managing missions in the Allegiance game lobby. It handles mission creation, player management, and communication with clients.

KEY PARTS:
- CFLMission: Class representing a mission, including methods to set lobby information, add/remove players, and notify the creator.
- SetLobbyInfo: Function to update mission information from FMD_LS_LOBBYMISSIONINFO structure.
- AddPlayer/RemovePlayer: Methods to manage player count on the server.
- NotifyCreator: Method to notify the mission creator about the mission details.

CONNECTS TO:
- g_pLobbyApp: Main application object managing various components of the lobby system.
- CFLServer and CFLClient: Classes representing game servers and clients, respectively.

NOTES:
- Contains logging for mission creation and destruction.
- Includes logic to handle server port updates and mission advertisement based on player count.

## `src/lobby/mission.h`

**PURPOSE:** The file `mission.h` defines the class `CFLMission`, which represents a mission in the Allegiance game lobby. It includes methods for managing mission information and player counts.

**KEY PARTS:**
- **Class: CFLMission**
  - Manages mission data and player interactions.
  - Provides methods to add and remove players, get mission info, and notify the creator of the mission.
  
- **Function: IsValidThisPtr()**
  - Validates the pointer to ensure it is not corrupted or invalid.

- **Function: FromCookie()**
  - Converts a cookie (DWORD) back to a `CFLMission` object if valid.

**CONNECTS TO:** 
- `messagesall.h`: For message handling.
- `CFLServer`, `CFLClient`: For server and client management within the lobby.

**NOTES:** None.

## `src/lobby/pch.cpp`

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

## `src/lobby/pch.h`

PURPOSE: This file is a precompiled header for the Lobby module in Allegiance, providing common includes and definitions used across various source files within the lobby system.

KEY PARTS:
- `LobbyModule.h`: Header file for the main lobby module.
- `httplib.h`: HTTP client library for network communications.
- `TCAtl.h` and related headers: Part of the ATL (Active Template Library) for Windows development.
- `steam_gameserver.h`: Header for Steamworks API integration, used for server management.
- `LobQueries.h` and `allegdb.h`: Database query handling for lobby operations.
- `zauth.h`: Authentication-related header files.
- `broadcast.h`, `zgameinfo.h`, `LobbyApp.h`, `mission.h`, `client.h`, `server.h`, `AutoUpdate.h`, `LobbyEvents.h`, `CSteamValidation.h`: Various headers for different components of the lobby system.

CONNECTS TO: Other modules and libraries such as ATL, Steamworks API, and database handling.

NOTES:
- Several warnings are disabled to suppress compiler messages about identifier truncation.
- Some commented-out code related to authentication and specific features (like USEAUTH) indicates potential future enhancements or maintenance.

## `src/lobby/resource.h`

PURPOSE: tiny file (5 lines), shown in full rather than summarized.

    // Used by Lobby.rc
    #define IDS_SERVICENAME 100
    #define IDS_SERVICEDESC 101
    #define IDR_Lobby 100
    #define ICO_LOBBY 101

## `src/lobby/server.cpp`

PURPOSE:
The file `Server.cpp` contains the implementation of server-related functionality for the Allegiance game lobby. It handles messages from servers, manages server connections, and processes various server-specific events.

KEY PARTS:
- `IsServerAllowed`: Checks if a server IP is allowed to connect based on the `lobby.cfg` configuration.
- `LobbyServerSite::OnAppMessage`: Handles application-level messages from connected servers, including logon, new missions, player joins, quits, and pauses.
- `CFLServer`: Manages individual server instances, handling mission creation, deletion, and pause state.

CONNECTS TO:
- `client.h` for client-related functions
- `FedMessaging` for message handling
- `g_pLobbyApp` for global lobby application context

NOTES:
- Contains regex parsing to handle rank information from callsigns when authentication is disabled.
- Implements chat logging using the Steam API via HTTP POST requests.

## `src/lobby/server.h`

PURPOSE:
The file `server.h` defines the class `CFLServer`, which represents a server in the Allegiance game lobby. It handles server-specific data and operations such as managing missions, player counts, and server load.

KEY PARTS:
- CFLServer: Manages server-related data and operations.
  - CreateMission: Creates a new mission for the server.
  - DeleteMission: Deletes an existing mission.
  - GetPlayerCount: Returns the current number of players on the server.
  - AddPlayer: Increases the player count by one.
  - RemovePlayer: Decreases the player count by one.
  - GetPercentLoad: Calculates the server's load percentage based on missions and players.
  - SetServerPort: Sets the server port number.
  - GetServerPort: Retrieves the server port number.
  - SetStaticCoreInfo: Sets static core information for the server.
  - GetStaticCoreInfo: Retrieves static core information for the server.
  - GetCurrentGamesCount: Returns the current count of games on the server.
  - SetCurrentGamesCount: Sets the current count of games on the server.
  - GetMaxGamesAllowed: Retrieves the maximum number of games allowed on the server.
  - SetMaxGamesAllowed: Sets the maximum number of games allowed on the server.
  - GetLocation: Retrieves the location of the server.
  - SetLocation: Sets the location of the server.
  - GetPrivilegedUsers: Retrieves privileged users for the server.
  - SetPrivilegedUsers: Sets privileged users for the server.
  - GetVersion: Retrieves the version of the server.
  - SetVersion: Sets the version of the server.

CONNECTS TO:
- `CFMConnection`: Manages connections to clients and servers.
- `CFLMission`: Represents a mission in the game.
- `PER_SERVER_COUNTERS`: Holds counters for various server metrics.

NOTES:
- None.

## `src/lobby/zgameinfo.cpp`

PURPOSE: This file contains functions and definitions related to handling game server information messages in the Allegiance game. It includes functions for converting data between different endianness formats and managing UDP broadcast sockets.

KEY PARTS:
- `ZGameServerInfoMsgEndian`: Converts a `ZGameServerInfoMsg` structure from host byte order to network byte order.
- `ZGameInstanceInfoMsgEndian`: Converts a `ZGameInstanceInfoMsg` structure from host byte order to network byte order.
- `ZGameInstanceInfoMsgZ5Endian`, `ZGameInstanceInfoMsgZ3Endian`, `ZGameServerInfoMsgZ2Endian`: Similar to the above, but for different versions of game instance and server info messages.
- `g_Broadcast`: A global UDP socket used for broadcasting game information.

CONNECTS TO: `src/lobby/zgameinfo.h` (header file), `src/network/udpsocket.cpp` (UDP socket implementation).

NOTES:
- The code includes conditional compilation based on endianness, using assembly instructions on x86 architectures.
- There are multiple versions of the same functions for different message structures, indicating compatibility with various game versions.

## `src/lobby/zgameinfo.h`

PURPOSE: This file defines the protocol and structures for sending game information like IP address, port, and game type to client applications such as admin tools and game clients. It includes functions for initializing a UDP socket, sending game info messages, and converting data structures to network byte order.

KEY PARTS:
- `ZGameInstanceInfoMsg`: Structure containing details about a game instance.
- `ZGameServerInfoMsg`: Structure containing an array of game instances.
- `ZGameInfoInit`: Initializes a UDP socket for sending game information.
- `ZGameInfoSendTo`: Sends game information to a specified address and port.
- `ZGameInstanceInfoMsgEndian` and `ZGameServerInfoMsgEndian`: Functions to convert data structures to network byte order.

CONNECTS TO: None, operates independently of other files but is used by client applications for game discovery.

NOTES: The file includes multiple protocol versions (Z2, Z3, Z5) with varying lengths for strings. There are no TODOs or hacks noted in the comments.
