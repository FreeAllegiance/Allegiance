# src/Wopr/AllegianceInterop

[Back to the index](../LLAMA_SUMMARY.md) - 24 files, 19,342 lines. Notes by a local model, to orient by: not verified facts.

This folder contains interop code for Allegiance, facilitating communication between managed and native C++ components.

- AllMessages.h: Defines message types used in client-server communication.
- igcWrapper.cpp: Implements wrappers for game entities like ships and stations.
- igcWrapper.h: Declares classes and functions to interact with game objects.
- NativeClientThingSite.h: Manages visibility and side-specific information for game objects.
- ClientConnection.cpp: Handles connections to the game lobby and server.

Largest files: AllMessages.h (10213 lines), igcWrapper.cpp (5027 lines), igcWrapper.h (2267 lines), NativeClientThingSite.h (349 lines), ClientConnection.cpp (291 lines).

## `src/Wopr/AllegianceInterop/AllMessages.h`

PURPOSE: The file `AllMessages.h` contains definitions for various message types used in communication between clients and servers in the Allegiance game. It includes both client-to-server (C) and server-to-client (S) messages, as well as structures and classes to manage these messages.

KEY PARTS:
- FMD_C_LOGONREQ: Represents a client logon request.
- FMD_S_LOGONACK: Represents a server acknowledgment of a client logon.
- ManagedObject: Base class for all message objects handling byte array conversion.
- FMD_S_STATIC_MAP_INFO: Contains static map information.
- FMD_CS_PROJECTILE_INSTANCE: Represents a projectile with creation details and position.
- FMD_S_MISSIONDEF: Detailed mission definition.
- FMD_CS_SIDE_INACTIVE: Indicates side activity status.
- FMD_CS_FIRE_MISSILE: Message for firing a missile.
- ServerSingleShipUpdate: Structure for single ship update details.
- FMD_S_SHIP_STATUS: Ship status message.
- FMD_C_START_GAME: Class for starting a game.
- FMD_S_ENTER_LIFEPOD: Class for entering a life pod.
- SimpleClient: Manages client connections and operations.
- FedMessaging: Handles messaging between clients, servers, and components.

CONNECTS TO: None

NOTES: None

PARTS (in file order):

- lines 2-391
  - FMD_C_LOGONREQ: Represents a client logon request message.
  - FMD_S_LOGONACK: Represents a server logon acknowledgment message.
  - FMD_CS_LOGOFF: Represents a client logoff message.
  - FMD_C_BUY_LOADOUT: Represents a client buy loadout request message.
  - FMD_S_BUY_LOADOUT_ACK: Represents a server buy loadout acknowledgment message.
  - ManagedObject: Base class for all message objects, handling byte array conversion.
  - BEGIN_PFM_CREATE_ALLOC and END_PFM_CREATE: Macros used to create and allocate message instances.
  - FM_VAR_PARM: Macro to handle parameter passing in message creation.
- lines 392-769
  - FMD_S_OBJECT_SPOTTED: Represents an object spotted message.
  - FMD_S_SET_CLUSTER: Represents a set cluster message.
  - FMD_S_DEV_COMPLETED: Represents a development completed message.
  - FMD_S_SET_BRIEFING_TEXT: Represents a set briefing text message.
  - FMD_S_PLAYERINFO: Represents a player information message.
  - ObjectType, ObjectID, SectorID, ServerSingleShipUpdate, Cookie, DevelopmentID, PersistPlayerScoreObject, String, Money, SideID, ShipID, RankID, DroneTypeID, PilotType, AbilityBitMask, ObjectID: Data types used in the messages.
  - TODOs: Handle rgPersistPlayerScores objects. Convert CharacterName to a managed string correctly. Implement ToManagedByteArray method.
- lines 770-1158
  - FMD_S_STATIC_MAP_INFO: Represents static map information.
  - FMD_CS_PROJECTILE_INSTANCE: Represents a projectile instance with creation time, data, and position.
  - FMD_S_RANK_INFO: Represents rank information with ranks and count.
  - FMD_S_SET_START_TIME: Represents setting the start time.
  - FMD_CS_SET_TEAM_INFO: Represents setting team information including squad ID, side ID, and side name.
  - FMD_S_PLAYER_RESCUED: Represents a player being rescued by another player.
  - The commented-out property for WingID indicates potential future functionality that was not implemented.
  - There are no explicit TODO comments in the provided code snippet.
- lines 1159-1545
  - FMD_S_BALLOT: Represents a ballot message sent by the server.
  - FMD_S_CANCEL_BALLOT: Represents a cancel ballot message sent by the server.
  - FMD_C_VOTE: Represents a vote message sent by the client.
  - FMD_S_SHIP_DELETE: Represents a ship delete message sent by the server.
  - FMD_C_RANDOMIZE_TEAMS: Represents a randomize teams command sent by the client.
  - ChatDataWrapper: Wraps a ChatData instance and provides properties for accessing its fields.
  - FMD_CS_CHATMESSAGE: Represents a chat message sent by the client or server.
  - TODOs: None identified in this snippet.
- lines 1546-1940
  - FMD_C_SHIP_UPDATE: Represents a ship update message.
  - FMD_S_EXPORT: Represents an export message for objects.
  - FMD_S_POSTER: Represents a poster message with geographic data.
  - FMD_S_TREASURE: Represents a treasure message.
  - FMD_S_DESTROY_TREASURE: Represents a destroy treasure message.
  - FMD_CS_NEW_SHIP_INSTANCE: Represents a new ship instance creation message.
  - FMD_CS_DROP_PART: Represents a drop part message for ships.
  - TODOs: "FM_VAR_PARM(StringToCharArray(exportData), CB_ZTS)" - Consider replacing with more efficient method. "strcpy(pfm->textureName, StringToCharArray(textureName));" - Ensure buffer overflow is handled safely.
- lines 1942-2315
  - FMD_CS_SWAP_PART: Represents a message for swapping equipment on a ship.
  - FMD_CS_PING: Represents a ping message between client and server.
  - FMD_S_URLROOT: Represents a URL root message.
  - FMD_S_MISSION_STAGE: Represents the current stage of a mission.
  - FMD_S_MISSIONDEF: Represents a detailed definition of a mission, including various parameters and descriptions.
  - BEGIN_PFM_CREATE_ALLOC: Macro for creating and allocating a new message instance.
  - END_PFM_CREATE: Macro for finalizing the creation of a message instance.
  - StringToCharArray: Converts a managed string to a character array.
- lines 2316-2705
  - FMD_C_POSITIONREQ: Represents a client request for position information.
  - FMD_S_POSITIONREQ: Represents a server response to a client's position request.
  - FMD_C_POSITIONACK: Represents a client acknowledgment of a position change or deletion request.
  - FMD_CS_DELPOSITIONREQ: Represents a client-server request to delete a position request.
  - FMD_CS_LOCK_LOBBY: Represents a client-server request to lock the lobby.
  - FMD_CS_LOCK_SIDES: Represents a client-server request to lock the sides.
  - FMD_CS_PLAYER_READY: Represents a client-server notification of player readiness.
  - TODOs or unusual code: The commented-out properties for rgStationID, rgShipIDLeaders, rgcPlayers, rgfAutoAccept, rgfReady, rgfForceReady, rgfActive, and rgfAllies suggest these were originally intended but are not currently used.
- lines 2706-3102
  - FMD_S_TEAM_READY: Represents a message indicating that a team is ready.
  - FMD_CS_FORCE_TEAM_READY: Represents a client-server message to force a team to be ready.
  - FMD_C_DOCKED: Represents a message for when a ship docks at a station.
  - FMD_CS_CHANGE_TEAM_CIV: Represents a client-server message to change the civilization of a side.
  - FMD_CS_AUTO_ACCEPT: Represents a client-server message to set auto-accept status for a side.
  - FMD_S_STATIONS_UPDATE: Represents a server message with updated station states.
  - FMD_S_STATION_CAPTURE: Represents a server message indicating that a station has been captured.
  - FMD_S_STATION_DESTROYED: Represents a server message indicating that a station has been destroyed.
- lines 3103-3501
  - FMD_S_MONEY_CHANGE: Represents a message about money change between ships.
  - FMD_S_DOCKED: Represents a message indicating that a ship has docked at a station.
  - FMD_S_GAME_OVER: Represents a game over message with details about the winner and other sides.
  - FMD_S_GAME_OVER_PLAYERS: Represents a message containing player information after a game ends.
  - FMD_C_BUCKET_DONATE: Represents a client command to donate money to a bucket.
  - FMD_S_BUCKET_STATUS: Represents a server message with the status of a bucket, including total time and money donated.
  - FMD_C_PLAYER_DONATE: Represents a client command to donate money from a player's ship.
  - FMD_CS_SIDE_INACTIVE: Represents a client-side message indicating that a side is inactive.
- lines 3502-3876
  - FMD_CS_SIDE_INACTIVE: Represents a message indicating side activity status.
  - FMD_CS_FIRE_MISSILE: Represents a message for firing a missile.
  - FMD_S_SIDE_TECH_CHANGE: Represents a message for changing side technology developments.
  - FMD_S_SIDE_ATTRIBUTE_CHANGE: Represents a message for changing side attributes.
  - FMD_S_EJECT: Represents a message for ejecting a ship.
  - FMD_S_SINGLE_SHIP_UPDATE: Represents a message for updating a single ship's status.
  - TODOs: None identified in this snippet.
  - Unusual code: The use of ManagedObject and related classes, which suggest integration with managed code environments. The presence of BEGIN_PFM_CREATE_ALLOC and END_PFM_CREATE macros, indicating a specific message format creation process.
- lines 3877-4273
  - FMD_S_SINGLE_SHIP_UPDATE: A class representing a single ship update message.
  - ObjectType: An enumeration type for object types.
  - ObjectID: A type representing an object identifier.
  - ServerSingleShipUpdate: A structure containing details about a single ship update.
  - bool bIsRipcording: A boolean indicating if ripcording is active.
  - FMD_C_SUICIDE: A class representing a suicide message.
  - EquipmentType: An enumeration type for equipment types.
  - FMD_S_MISSILE_DESTROYED: A class representing a missile destroyed message.
- lines 4275-4655
  - FMD_S_SHIP_STATUS: Represents a ship status message.
  - FMD_S_PROBES_UPDATE: Represents an update on probe states.
  - FMD_CS_ORDER_CHANGE: Represents a change in order for a command.
  - FMD_S_LEAVE_SHIP: Represents a ship leaving another ship.
  - FMD_S_JOINED_MISSION: Represents a ship joining a mission.
  - BEGIN_PFM_CREATE_ALLOC: Macro to start creating and allocating a message.
  - END_PFM_CREATE: Macro to end the creation of a message.
  - FM_VAR_REF: Macro to get a reference to a variable in a message.
- lines 4656-5052
  - Cookie: A class that has a method get() which returns a member variable m_Instance->cookie.
  - FMD_S_LOADOUT_CHANGE: A serializable class representing a loadout change message with properties for loadout, passengers, and ship ID.
  - FMD_C_ACTIVE_TURRET_UPDATE: A serializable class representing an active turret update message with properties for time update and turret update data.
  - FMD_C_INACTIVE_TURRET_UPDATE: A serializable class representing an inactive turret update message.
  - FMD_S_TELEPORT_ACK: A serializable class representing a teleport acknowledgment message with properties for station ID and new hull status.
  - FMD_C_BOARD_SHIP: A serializable class representing a board ship command with a property for the parent ship ID.
  - FMD_C_VIEW_CLUSTER: A serializable class representing a view cluster command with properties for cluster ID, target object type, and target object ID.
  - FMD_S_KILL_SHIP: A serializable class representing a kill ship message with properties for ship ID, credit details, killer side ID, and credit flags.
- lines 5053-5447
  - FMD_CS_SET_WINGID: Sends a command to set a ship's wing ID.
  - FMD_S_ICQ_CHAT_ACK: Represents an ICQ chat acknowledgment message.
  - FMD_CS_CHATBUOY: Sends a chat buoy message with a message and data.
  - FMD_S_CREATE_CHAFF: Sends a message to create chaff at specified coordinates.
  - FMD_S_MISSILE_SPOOFED: Indicates that a missile has been spoofed.
  - FMD_S_END_SPOOFING: Signals the end of spoofing.
  - FMD_C_AUTODONATE: Command to donate money to another ship.
  - MapType: Enumerates different map types for missions.
- lines 5448-5846
  - FMD_S_PAYDAY: Represents a message for paying money.
  - FMD_S_SET_MONEY: Represents a message for setting money on a ship.
  - FMD_S_AUTODONATE: Represents a message for autodonating money between ships.
  - FMD_C_MUTE: Represents a client-side message to mute a player.
  - FMD_CS_SET_TEAM_LEADER: Represents a server-to-client message to set a team leader.
  - FMD_CS_SET_TEAM_INVESTOR: Represents a server-to-client message to set a team investor.
  - FMD_CS_SET_MISSION_OWNER: Represents a server-to-client message to set the mission owner.
  - FMD_CS_QUIT_MISSION: Represents a client-to-server message to quit a mission.
- lines 5847-6242
  - FMD_S_JOIN_SIDE: Represents a message sent by the server when a player joins a side.
  - FMD_CS_QUIT_SIDE: Represents a client-side message to quit a side with a reason and ship ID.
  - FMD_S_ENTER_GAME: Represents a message from the server indicating that a game has been entered.
  - FMD_CS_RELOAD: Represents a client-side message to reload items for a specific ship.
  - FMD_S_GAIN_FLAG: Represents a message sent by the server when a player gains a flag.
  - FMD_C_START_GAME: Represents a client-side message to start a game.
  - TODO: Handle all arrays in this message
  - Unusual code: The commented-out section for FMD_S_THREAT and FMD_S_GAME_STATE, which were likely intended but not fully implemented.
- lines 6243-6648
  - FMD_C_START_GAME: Class for starting a game.
  - FMD_S_ACQUIRE_TREASURE: Class for acquiring treasure.
  - FMD_C_TREASURE_ACK: Class for acknowledging treasure acquisition.
  - FMD_S_ADD_PART: Class for adding parts to a ship.
  - FMD_S_ENTER_LIFEPOD: Class for entering a life pod.
  - FMD_S_LIGHT_SHIPS_UPDATE: Class for updating light ships.
  - FMD_S_HEAVY_SHIPS_UPDATE: Class for updating heavy ships.
  - FMD_S_VIEW_CLUSTER: Class for viewing a cluster.
- lines 6649-7053
  - FMD_S_BOARD_NACK: Represents a board not acknowledged message.
  - FMD_S_ASTEROIDS_UPDATE: Contains updated information about asteroids.
  - FMD_S_ASTEROID_DRAINED: Indicates an asteroid has been drained.
  - FMD_S_BUILDINGEFFECT_DESTROYED: Signals the destruction of a building effect.
  - FMD_CS_REQUEST_MONEY: A client request for money.
  - FMD_S_SHIP_RESET: Server reset message for a ship.
  - FMD_C_RIPCORD_REQUEST: Client request to activate ripcord.
  - FMD_S_RIPCORD_ACTIVATE: Server activation of ripcord.
- lines 7054-7455
  - SimpleClient: Manages client connections.
  - FedMessaging: Handles messaging between clients and servers.
  - BEGIN_PFM_CREATE_ALLOC: Initializes message creation with allocation.
  - END_PFM_CREATE: Ends message creation process.
  - ShipID: Represents a unique identifier for a ship.
  - Time: Represents time-related data.
  - WarpID: Represents a unique identifier for a warp point.
  - ExpendableTypeID: Represents a type of expendable item.
- lines 7456-7858
  - FMD_C_REQPINGDATA: Represents a request for ping data.
  - FMD_C_CHANGE_ALLIANCE: Represents a change in alliance between sides.
  - FMD_S_CHANGE_ALLIANCES: Represents changes to alliances sent by the server.
  - FMD_S_ASTEROID_MINED: Represents an asteroid being mined.
  - FMD_CS_HIGHLIGHT_CLUSTER: Represents highlighting a cluster on the map.
  - FMD_C_LOGON_LOBBY_OLD: Represents logging into the lobby with old protocol.
  - FMD_C_LOGOFF_LOBBY: Represents logging off from the lobby.
  - FMD_L_AUTO_UPDATE_INFO: Represents auto-update information sent by the server.
- lines 7859-8283
  - FMD_C_CREATE_MISSION_REQ: Represents a request to create a mission.
  - FMD_L_CREATE_MISSION_ACK: Represents an acknowledgment of creating a mission.
  - FMD_L_CREATE_MISSION_NACK: Represents a negative acknowledgment of creating a mission.
  - FMD_C_JOIN_GAME_REQ: Represents a request to join a game.
  - FMD_L_JOIN_GAME_NACK: Represents a negative acknowledgment of joining a game.
  - FMD_L_JOIN_MISSION: Represents a notification that a player has joined a mission.
  - FMD_L_LOGON_ACK: Represents an acknowledgment of a successful logon.
  - FMD_L_LOGON_NACK: Represents a negative acknowledgment of a logon attempt.
- lines 8284-8668
  - SimpleClient: A class used for client operations.
  - FedMessaging: Manages messaging between different components of the game.
  - FMD_L_FOUND_PLAYER: Represents a message indicating that a player has been found.
  - FMD_C_LOGON_LOBBY: Represents a logon request to the lobby.
  - ServerCoreInfo_Managed and StaticCoreInfo_Managed: Structs representing server and static core information in a managed format.
  - FMD_L_SERVERS_LIST: Represents a list of servers, including both static and server core details.
  - FMD_LS_LOBBYMISSIONINFO: Represents mission information for the lobby.
  - The use of FM_VAR_REF and FM_VAR_PARM macros for handling message variables.
- lines 8669-9027
  - SimpleClient: Likely a client class for handling network communications.
  - FedMessaging: A messaging system used for inter-client communication.
  - BEGIN_PFM_CREATE_ALLOC: Macro or function to start creating an allocation for message passing.
  - END_PFM_CREATE: Macro or function to end the creation of an allocation.
  - szGameName: Represents the name of the game.
  - rgSquadIDs: Represents the squad IDs associated with the game.
  - dwPort: Represents the port number for network communication.
  - dwCookie: A unique identifier for the mission or session.
- lines 9028-9430
  - FMD_LS_MISSION_GONE: Represents a mission gone message with properties for dwCookie.
  - FMD_LS_SQUAD_MEMBERSHIPS: Manages squad memberships with properties for squadMemberships and cSquadMemberships.
  - FMD_S_LOGON_LOBBY: Handles logon lobby messages with various string and integer properties.
  - FMD_S_LOGOFF_LOBBY: Represents a logoff lobby message without any properties.
  - FMD_L_CREATE_MISSION_REQ: Manages mission creation requests with properties for GameName, IGCStaticFile, and dwCookie.
  - TODO: Sort this one out if needed. - Indicates a comment suggesting further work on the commented-out section of code related to FMD_S_NEW_MISSION.
- lines 9431-9857
  - FMD_L_NEW_MISSION_ACK: Represents a message acknowledging a new mission.
  - FMD_S_HEARTBEAT: Represents a heartbeat message sent by the server to keep connections alive.
  - FMD_L_QUIT: Represents a quit message from a client.
  - FMD_L_TOKEN: Represents a token message sent by a client.
  - FMD_S_PLAYER_JOINED: Represents a player joining a mission.
  - FMD_S_PLAYER_QUIT: Represents a player quitting a mission.
  - FMD_L_REMOVE_PLAYER: Represents a request to remove a player from a mission.
  - FMD_S_PAUSE: Represents a pause command sent by the server.
- lines 9858-10213
  - FMD_LS_PLAYER_RANK: Represents a player rank message with properties for rank.
  - FMD_L_UPDATE_DRM_HASHES: Manages DRM hashes updates with a property for the download URL.
  - FMD_S_LOG_CHAT_MESSAGE: Logs chat messages with properties for mission name, source and target names, message, IPs, and Steam IDs.
  - WarpInfo: Holds information about a warp between clusters with properties for from and to clusters.
  - ClusterInfo: Represents cluster information including its ID, home sector status, and warps.
  - FMD_S_CLUSTERINFO: Manages cluster information with a list of ClusterInfo objects.
  - TODOs: The code has several instances where arrays are accessed without checking bounds (e.g., sizeof(m_Instance->clusterIDs) / sizeof(SectorID)). There is no error handling for potential issues when accessing or modifying the m_clusterInfos list.
  - Unusual Code: The use of gcnew String(FM_VAR_REF(m_Instance, ...)) to convert native C++ strings to managed strings in properties.

## `src/Wopr/AllegianceInterop/AllegianceInterop.cpp`

PURPOSE: tiny file (3 lines), shown in full rather than summarized.

    #include "stdafx.h"
    #include "AllegianceInterop.h"
    #include "ManagedObject.h"

## `src/Wopr/AllegianceInterop/AllegianceInterop.h`

PURPOSE: This file is part of the Allegiance game's interop layer, facilitating communication between managed and native code.

KEY PARTS:
- `g_rgszMsgNames`: An array of message names used for internal communication.
- `System::Reflection` namespace: Used for reflection operations, which might be related to dynamic loading or type inspection.

CONNECTS TO: `messagecore.h`, `messageslc.h`, and `messagesall.h`.

NOTES: None.

## `src/Wopr/AllegianceInterop/AssemblyInfo.cpp`

PURPOSE: tiny file (8 lines), shown in full rather than summarized.

    #include "stdafx.h"
    using namespace System;
    using namespace System::Runtime::CompilerServices;
    using namespace System::Security::Permissions;
    [assembly:AssemblyTitleAttribute(L"AllegianceInterop")];
    [assembly:AssemblyConfigurationAttribute(L"")];
    [assembly:AssemblyProductAttribute(L"AllegianceInterop")];
    [assembly:AssemblyTrademarkAttribute(L"")];

## `src/Wopr/AllegianceInterop/ClientConnection.cpp`

PURPOSE: The file `ClientConnection.cpp` is part of the Allegiance game's client-side interop layer, responsible for managing connections to the game lobby and server, as well as sending and receiving messages.

KEY PARTS:
- `ClientConnection`: Manages connections to the game lobby and server.
- `ConnectToLobby`: Establishes a connection to the game lobby.
- `GetMissionList`: Retrieves a list of available missions.
- `JoinMission`: Joins a specific mission.
- `ConnectToServer`: Connects to a game server.
- `SendAndReceiveUpdate`: Sends and receives updates from the connected server or lobby.
- `SendMessageLobby` and `SendMessageServer`: Send messages to the lobby or server, respectively.
- `OnAppMessageEvent`, `OnLogonLobbyAckEvent`, `OnLogonAckEvent`: Event handlers for various client events.

CONNECTS TO: NativeClient (unmanaged C++), FedMessaging (game messaging system).

NOTES:
- Uses interop techniques to bridge managed and unmanaged code.
- Contains commented-out code and TODOs, indicating areas that may need further investigation or cleanup.

## `src/Wopr/AllegianceInterop/ClientConnection.h`

PURPOSE: This file defines a managed class `ClientConnection` that provides an interface for managing client connections to the Allegiance game server and lobby systems. It includes methods for connecting, disconnecting, sending messages, and interacting with various in-game objects.

KEY PARTS:
- `ClientConnection`: Manages client connections and interactions with the game.
- `OnAppMessageEventDelegate`, `OnLogonLobbyAckEventDelegate`, `OnLogonAckEventDelegate`: Delegates for handling application messages, logon acknowledgments from the lobby and server.
- `ConnectToLobby`, `ConnectToServer`: Methods to establish connections to the lobby and game servers.
- `DisconnectLobby`, `DisconnectServer`: Methods to disconnect from the lobby and game servers.
- `SendMessageLobby`, `SendMessageServer`: Methods to send messages to the lobby and game servers.

CONNECTS TO: NativeClient, FedMessaging, CFMConnection, FEDMESSAGE (via Allegiance's native code).

NOTES:
- The file includes commented-out sections for testing and disabled features.
- Exception handling is included in the `SendChat` method to catch and log exceptions.

## `src/Wopr/AllegianceInterop/Enums.h`

PURPOSE: This file defines enumerations used in the Allegiance game for various purposes such as connection types, mission stages, lockdown criteria, and chat targets.

KEY PARTS:
- ConnectionType: Enumerates different types of connections (Lobby, Server).
- MissionStage: Enumerates stages of a mission (NotStarted, Starting, Started, Over, Terminate).
- LockdownCriteria: Enumerates criteria for initiating a lockdown (Donating, Loadout, Teleporting).
- ChatTarget: Enumerates different chat targets within the game.

CONNECTS TO: igc.h

NOTES: None.

## `src/Wopr/AllegianceInterop/ISupportManagedByteArrays.h`

PURPOSE: tiny file (9 lines), shown in full rather than summarized.

    #pragma once
    using namespace System;
    namespace AllegianceInterop {
    	public interface class ISupportManagedByteArrays
    	{
    	public:
    		array<unsigned char> ^ ToManagedByteArray();
    	};
    }

## `src/Wopr/AllegianceInterop/ManagedObject.cpp`

PURPOSE: tiny file (4 lines), shown in full rather than summarized.

    #include "stdafx.h"
    #include "ManagedObject.h"
    namespace AllegianceInterop {
    }

## `src/Wopr/AllegianceInterop/ManagedObject.h`

PURPOSE: The file defines a generic managed class `ManagedObject` that provides functionality for managing native C++ objects within a .NET environment. It includes methods for converting between managed and unmanaged memory, as well as cleanup mechanisms.

KEY PARTS:
- `ManagedObject<T>`: A template class that manages an instance of a native C++ object.
- `CleanUpAllocatedItems`: Cleans up allocated resources when the object is destroyed.
- `GetInstance`: Returns the pointer to the managed instance.
- `ToManagedByteArray`: Converts the native object's data to a managed byte array.
- `StringToCharArray`: Converts a .NET string to a C-style character array.

CONNECTS TO: Depends on other classes and namespaces such as `NativeClient`, `SimpleClient`, and `ISupportManagedByteArrays`.

NOTES: None.

## `src/Wopr/AllegianceInterop/MessageTypeNative.h`

PURPOSE: This file defines an enumeration of message types used for inter-process communication within the Allegiance game. These messages facilitate various interactions between different components of the game.

KEY PARTS:
- FM_C_LOGONREQ: Client requests to log on.
- FM_S_LOGONACK: Server acknowledges a client's logon request.
- FM_CS_LOGOFF: Client sends a logoff message.
- FM_C_BUY_LOADOUT: Client requests to buy a loadout.
- FM_S_BUY_LOADOUT_ACK: Server acknowledges the loadout purchase.
- FM_S_OBJECT_SPOTTED: Server notifies clients of an object spotted.

CONNECTS TO: Allegiance game engine components, particularly those handling network communication and player interactions.

NOTES: None.

## `src/Wopr/AllegianceInterop/NativeClient.cpp`

**PURPOSE:**  
This file contains the implementation of the `NativeClient` class in Allegiance, which handles communication and interaction with the game's server and other clients.

**KEY PARTS:**
- **Class: NativeClient** - Manages client-server interactions, hooks into message handling, and manages logon events.
- **Function: OnAppMessage** - Processes incoming messages from the server, including mission updates and buy loadout acknowledgments.
- **Function: SendAndReceiveUpdate** - Handles sending and receiving updates to and from the server, ensuring timely communication.
- **Function: CreateThingSite** - Creates a `NativeClientThingSite` for managing individual game objects.
- **Function: CreateClusterSite** - Creates a `NativeClientClusterSite` for managing clusters of game objects.
- **Function: ChangeStation** - Handles changes in the player's station, including resetting controls and weapons.
- **Function: ChangeCluster** - Handles changes in the player's cluster, activating or deactivating clusters as needed.

**CONNECTS TO:**  
- `BaseClient` - The base class for client implementations.
- `FedMessaging` - For message handling and communication with the server.
- `NativeClientThingSite` and `NativeClientClusterSite` - Custom site classes for managing game objects and clusters.

**NOTES:**  
- Contains an exception filter to handle access violations.
- Hooks into event handlers for custom processing of messages and logon events.
- Uses critical sections to ensure thread safety when handling certain types of messages.

## `src/Wopr/AllegianceInterop/NativeClient.h`

PURPOSE: This file defines a class `NativeClient` that provides an interface for handling client events and messages in the Allegiance game. It includes methods for hooking into specific events and sending/receiving updates.

KEY PARTS:
- NativeClient: Main class that inherits from BaseClient, providing implementations for various event handlers.
- OnAppMessageEventFunction, OnLogonLobbyAckEventFunction, OnLogonAckEventFunction: Delegate types for handling different client events.
- HookOnAppMessageEvent, HookOnLogonLobbyAckEvent, HookOnLogonAckEvent: Methods to hook into specific client events.
- SendAndReceiveUpdate: Method to send and receive updates from the game server.

CONNECTS TO:
- messagecore.h, clintlib.h: Header files containing necessary declarations for handling messages and connections.

NOTES:
- Contains commented-out code that was likely used for testing or experimentation but is no longer active.

## `src/Wopr/AllegianceInterop/NativeClientClusterSite.h`

PURPOSE: This file defines a class `NativeClientClusterSite` that extends the functionality of `ClusterSite` to manage scanners for different sides in a game cluster.

KEY PARTS:
- `NativeClientClusterSite`: Inherits from `ClusterSite` and manages scanners for each side.
- `AddScanner`: Adds a new scanner to the specified side's list, ensuring it is valid.
- `DeleteScanner`: Removes an existing scanner from the specified side's list, ensuring it is valid.
- `GetScanners`: Retrieves the list of scanners for the specified side.
- `Terminate`: Calls the base class termination method.

CONNECTS TO: `ClusterSite`, `IclusterIGC`, `IsacannerIGC`, `ScannerListIGC`.

NOTES: None.

## `src/Wopr/AllegianceInterop/NativeClientThingSite.h`

PURPOSE: This file defines a class `NativeClientThingSite` that manages visibility and side-specific information for game objects in the Allegiance game. It handles updates to visibility based on scanner ranges and side relationships.

KEY PARTS:
- `NativeClientThingSite`: Inherits from `ThingSite` and manages visibility for game objects.
- `SetCluster`: Updates visibility based on the cluster, model attributes, and side relationships.
- `UpdateSideVisibility`: Recalculates visibility for a given object based on scanner ranges and last spotted information.
- `GetSideVisibility`: Returns whether a side can see the object.
- `GetCurrentEye`: Returns whether the current eye is visible to a specific side.

CONNECTS TO: `igc.h`, likely other game engine components for handling clusters, models, and sides.

NOTES:
- Contains commented-out code from previous versions of the game.
- Includes assertions and debug statements for development and testing.

## `src/Wopr/AllegianceInterop/Resource.h`

PURPOSE: tiny file (3 lines), shown in full rather than summarized.

      / / { { N O _ D E P E N D E N C I E S } } 
     / /   M i c r o s o f t   V i s u a l   C + +   g e n e r a t e d   i n c l u d e   f i l e . 
     / /   U s e d   b y   a p p . r c 

## `src/Wopr/AllegianceInterop/SimpleClient.cpp`

PURPOSE: tiny file (9 lines), shown in full rather than summarized.

    #include "stdafx.h"
    #include "SimpleClient.h"
    SimpleClient::SimpleClient()
    {
    }
    HRESULT SimpleClient::OnAppMessage(FedMessaging * pthis, CFMConnection & cnxnFrom, FEDMESSAGE * pfm)
    {
    	return E_NOTIMPL;
    }

## `src/Wopr/AllegianceInterop/SimpleClient.h`

PURPOSE: tiny file (9 lines), shown in full rather than summarized.

    #pragma once
    #include "messagecore.h"
    class SimpleClient : public IFedMessagingSite
    {
    public:
    	SimpleClient();
    	// Inherited via IFedMessagingSite
    	virtual HRESULT OnAppMessage(FedMessaging * pthis, CFMConnection & cnxnFrom, FEDMESSAGE * pfm) override;
    };

## `src/Wopr/AllegianceInterop/clintlibWrapper.cpp`

PURPOSE: tiny file (10 lines), shown in full rather than summarized.

    #include "stdafx.h"
    #include "clintlibWrapper.h"
    using namespace System;
    namespace AllegianceInterop
    {
    	MissionInfoWrapper::MissionInfoWrapper(::MissionInfo * instance)
    	{
    		m_instance = instance;
    	}
    }

## `src/Wopr/AllegianceInterop/clintlibWrapper.h`

PURPOSE: tiny file (16 lines), shown in full rather than summarized.

    #pragma once
    #include "clintlib.h"
    using namespace System;
    namespace AllegianceInterop
    {
    	public ref class MissionInfoWrapper
    	{
    	public:
    		::MissionInfo * m_instance;
    	public:
    		MissionInfoWrapper(::MissionInfo * instance);
    		String ^        Name() { return gcnew String(m_instance->Name()); }
    		String ^        Description() { return gcnew String(m_instance->Description()); }
    		int				GetCookie() { return m_instance->GetCookie(); }
    	};
    }

## `src/Wopr/AllegianceInterop/igcWrapper.cpp`

PURPOSE: The file `igcWrapper.cpp` contains a series of wrapper classes and functions to facilitate interoperability between managed and unmanaged code in Allegiance, specifically for game entities like ships, stations, asteroids, and more.

KEY PARTS:
- Wraps various game entities such as `::Geo`, `::ImapMakerIGC`, `::MissionParams`, etc.
- Provides static interfaces for managing constants and objects (`IstaticIGCWrapper`).
- Manages mission-related operations (`ImissionIGCWrapper`), including adding, deleting, and retrieving clusters, ships, stations, asteroids, warps, buoys, and treasures.
- Offers methods to update and export game data (`IbaseIGCWrapper::Update`, `IbaseIGCWrapper::Export`).
- Manages ship-specific operations (`IshipIGCWrapper`), including processing updates, setting private data, and handling collisions.
- Wraps station-related functionalities (`IstationIGCWrapper`), such as adding and deleting ships, repairing them, and retrieving their capabilities.
- Provides wrappers for various types of objects like hulls, parts, launchers, magazines, dispensers, and more.

CONNECTS TO: Allegiance game engine (unmanaged code).

NOTES: None.

PARTS (in file order):

- lines 1-305
  - GeoWrapper: Wraps a ::Geo instance.
  - ImapMakerIGCWrapper: Wraps an ::ImapMakerIGC instance.
  - MissionParamsWrapper: Wraps a ::MissionParams instance.
  - ShipLoadoutWrapper: Wraps a ::ShipLoadout instance.
  - CompactOrientationWrapper: Wraps a ::CompactOrientation instance.
  - CompactTimeWrapper: Wraps a ::CompactTime instance.
  - CompactPositionWrapper: Wraps a ::CompactPosition instance.
  - CompactStateWrapper: Wraps a ::CompactState instance.
- lines 306-618
  - IstaticIGCWrapper: Provides a static interface to interact with game constants and objects.
  - FloatConstantID, DamageTypeID, DefenseTypeID, DroneTypeID, DevelopmentID, StationTypeID, CivID, HullID, ExpendableTypeID, PartID, ProjectileTypeID, TreasureSetID: IDs for various game entities.
  - GetFloatConstant, SetFloatConstant: Methods to get and set float constants.
  - GetDamageConstant, SetDamageConstant: Methods to get and set damage constants based on damage and defense types.
  - GetConstants, SetConstants: Methods to get and set all constants as a block of data.
  - AddDroneType, DeleteDroneType, GetDroneType, GetDroneTypes: Manage and retrieve drone types.
  - AddDevelopment, DeleteDevelopment, GetDevelopment, GetDevelopments: Manage and retrieve development items.
  - AddStationType, DeleteStationType, GetStationType, GetStationTypes: Manage and retrieve station types.
- lines 619-933
  - ImissionIGCWrapper: Manages mission-related operations and data.
  - AddCluster, DeleteCluster, GetCluster, GetClusters: Manage clusters of objects in the game world.
  - AddShip, DeleteShip, GetShip, GetShips: Manage ships in the game.
  - AddStation, DeleteStation, GetStation, GetStations: Manage stations in the game.
  - AddAsteroid, DeleteAsteroid, GetAsteroid, GetAsteroids: Manage asteroids in the game.
  - AddWarp, DeleteWarp, GetWarp, GetWarps: Manage warps or teleportation points.
  - AddBuoy, DeleteBuoy, GetBuoy, GetBuoys: Manage buoys or markers.
  - AddTreasure, DeleteTreasure, GetTreasure, GetTreasures: Manage treasures in the game.
- lines 934-1241
  - IbaseIGCWrapper::Update: Updates the instance with the given time.
  - IbaseIGCWrapper::Export: Exports data to the specified location.
  - IbaseIGCWrapper::GetUniqueID: Retrieves a unique identifier for the instance.
  - IbaseIGCWrapper::GetObjectType: Returns the type of object.
  - IbaseIGCWrapper::GetObjectID: Gets the object's ID.
  - IbaseIGCWrapper::GetMission: Retrieves the mission associated with the instance.
  - ItypeIGCWrapper::ItypeIGCWrapper: Constructor for ItypeIGCWrapper.
  - ItypeIGCWrapper::GetData: Returns the data of the instance.
- lines 1242-1550
  - ImodelIGCWrapper: Provides a managed interface for ImodelIGC objects.
  - IdamageIGCWrapper: Inherits from ImodelIGCWrapper and provides additional methods for damage-related operations.
  - IscannerIGCWrapper: Inherits from IdamageIGCWrapper and adds scanner-related functionality.
  - IshipIGCWrapper: Inherits from IscannerIGCWrapper and extends it with ship-specific properties and methods.
  - GetFraction: Returns the fraction value of an object.
  - SetFraction: Sets the fraction value of an object.
  - InScannerRange: Checks if another model is within scanner range.
  - CanSee: Determines if another model can be seen by this model.
- lines 1551-1859
  - IshipIGCWrapper: A C++/CLI wrapper class for an unmanaged C++ interface (::IshipIGC), providing a managed interface to ship-related operations.
  - ExportShipUpdate(ClientShipUpdateWrapper^ pshipupdate): Exports a ship update from a managed ClientShipUpdateWrapper object to the underlying unmanaged ::IshipIGC.
  - ExportShipUpdate(ClientActiveTurretUpdateWrapper^ patu): Similar to above, but for active turret updates.
  - ProcessFractions(const CompactShipFractions& fractions): Processes ship fractions using an unmanaged interface method.
  - ProcessShipUpdate(const ServerLightShipUpdate& shipupdate), ProcessShipUpdate(const ClientShipUpdate& shipupdate), ProcessShipUpdate(const ClientActiveTurretUpdate& shipupdate): Various methods to process different types of ship updates.
  - SetPrivateData(DWORD dwPrivate), GetPrivateData(): Methods for setting and getting private data associated with the ship.
  - ReInitialize(DataShipIGC  dataShip, Time now): Reinitializes the ship with new data.
  - HitTreasure(TreasureCode treasureCode, ObjectID objectID, short amount): Handles a ship hitting a treasure.
- lines 1860-2176
  - IshipIGCWrapper: Wraps a ship's interface for interop.
  - AchievementMask: Represents achievement bits.
  - DamageTrackWrapper: Manages damage tracking for models.
  - CommandID: Identifies commands.
  - Time: Represents game time.
  - VectorWrapper: Wrapper for vector data.
  - IbaseIGCWrapper: Base class for base-related interfaces.
  - PlayerInfoWrapper: Holds player information.
- lines 2177-2483
  - IprobeIGCWrapper::GetAccuracy: Returns the accuracy of an object.
  - IprobeIGCWrapper::GetCanRipcord: Checks if an object can ripcord at a given speed.
  - IprobeIGCWrapper::GetRipcordDelay: Returns the delay before an object can ripcord.
  - IstationIGCWrapper::AddShip: Adds a ship to the station.
  - IstationIGCWrapper::DeleteShip: Deletes a ship from the station.
  - IstationIGCWrapper::GetShip: Retrieves a ship by its ID.
  - IstationIGCWrapper::GetShips: Returns a list of all ships at the station.
  - IstationIGCWrapper::RepairAndRefuel: Repairs and refuels a given ship.
- lines 2484-2789
  - IstationTypeIGCWrapper: Wraps an instance of ::IstationTypeIGC, providing managed access to station type properties and methods.
  - GetScannerRange(): Returns the scanner range of the station.
  - GetIncome(): Returns the income generated by the station.
  - GetCapabilities(): Returns the capabilities bitmask of the station.
  - HasCapability(StationAbilityBitMask habm): Checks if the station has a specific capability.
  - GetTextureName(): Returns the texture name of the station.
  - GetBuilderName(): Returns the builder name of the station.
  - GetSuccessorStationType(IsideIGCWrapper^ pside): Retrieves the successor station type for a given side.
- lines 2790-3091
  - IexpendableTypeIGCWrapper: Base class for expendable types.
  - ImissileTypeIGCWrapper: Wrapper for missile type objects.
  - ImineTypeIGCWrapper: Wrapper for mine type objects.
  - IchaffTypeIGCWrapper: Wrapper for chaff type objects.
  - IprobeTypeIGCWrapper: Wrapper for probe type objects.
  - IhullTypeIGCWrapper: Wrapper for hull type objects.
  - GetRadius: Returns the radius of an object.
  - GetCapabilities: Returns the capabilities of an object.
- lines 3092-3397
  - IhullTypeIGCWrapper::GetLandPlanes(int slotID): Returns the number of land planes for a given slot ID.
  - IpartTypeIGCWrapper::GetEquipmentType(): Retrieves the equipment type associated with this part type.
  - IlauncherTypeIGCWrapper::GetExpendableType(): Returns the expendable type associated with this launcher type.
  - IpartIGCWrapper::SetShip(IshipIGCWrapper^ newVal, Mount mount): Assigns a ship to this part and sets its mount point.
  - IweaponIGCWrapper::GetProjectileType(): Retrieves the projectile type used by this weapon.
  - IshieldIGCWrapper::GetFraction(): Returns the current fraction of shield strength.
  - IcloakIGCWrapper::GetMaxCloaking(): Returns the maximum cloaking level for this cloak.
  - IafterburnerIGCWrapper::IncrementalUpdate(Time lastUpdate, Time now, bool bUseFuel): Updates the afterburner's state based on time and fuel usage.
- lines 3398-3709
  - IlauncherIGCWrapper: A wrapper class for managing launcher objects.
  - ImagazineIGCWrapper: A wrapper class for managing magazine objects.
  - IdispenserIGCWrapper: A wrapper class for managing dispenser objects.
  - IclusterIGCWrapper: A wrapper class for managing cluster objects, which can contain various types of game entities like models, warps, mines, missiles, probes, stations, ships, asteroids, and treasures.
  - GetModel: Retrieves a model by name from the cluster.
  - GetModels: Returns all models in the cluster.
  - GetPickableModels: Returns pickable models in the cluster.
  - AddWarp: Adds a warp to the cluster.
- lines 3710-4014
  - IclusterIGCWrapper::GetAmbientLevel: Returns the ambient level.
  - IclusterIGCWrapper::GetPosterName: Returns the poster's name as a string.
  - IclusterIGCWrapper::SetPrivateData: Sets private data with a DWORD value.
  - IclusterIGCWrapper::FlushCollisionQueue: Flushes the collision queue.
  - IasteroidIGCWrapper::IasteroidIGCWrapper: Constructor for wrapping an asteroid instance.
  - IasteroidIGCWrapper::GetOre: Returns the amount of ore on the asteroid.
  - IasteroidIGCWrapper::SetOre: Sets the amount of ore on the asteroid.
  - IasteroidIGCWrapper::MineOre: Mines ore from the asteroid for a given time duration.
- lines 4015-4319
  - IsideIGCWrapper::GetShip(ShipID id): Retrieves a ship wrapper by ID.
  - IsideIGCWrapper::GetShips(): Returns a list of all ships as wrappers.
  - IsideIGCWrapper::AddBucket(IbucketIGCWrapper^ s): Adds a bucket to the instance.
  - IsideIGCWrapper::DeleteBucket(IbucketIGCWrapper^ s): Deletes a bucket from the instance.
  - IsideIGCWrapper::GetBucket(BucketID bucketID): Retrieves a bucket wrapper by ID.
  - IsideIGCWrapper::GetBuckets(): Returns a list of all buckets as wrappers.
  - IcivilizationIGCWrapper::IcivilizationIGCWrapper(::IcivilizationIGC  instance): Constructor for civilization wrapper.
  - ItreasureSetIGCWrapper::ItreasureSetIGCWrapper(::ItreasureSetIGC  instance): Constructor for treasure set wrapper.
- lines 4320-4622
  - ShipStatusWrapper: Manages ship status properties.
  - ClusterSiteWrapper: Wraps cluster site functionality.
  - SideVisibilityWrapper: Handles side visibility settings.
  - GameSiteWrapper: Represents game site operations.
  - IIgcSiteWrapper: Interface for IGC site operations.
  - DamageTrackWrapper: Manages damage tracking.
  - DamageBucketWrapper: Represents a single damage bucket.
  - DamageTrackSetWrapper: Manages a set of damage tracks.
- lines 4623-4923
  - DamageTrackWrapper::DamageTrackWrapper: Constructor for wrapping a DamageTrack instance.
  - DamageTrackSetWrapper::AddTrack: Adds a track to the set.
  - DamageTrackSetWrapper::DeleteTrack: Deletes a track from the set.
  - WaypointWrapper::WaypointWrapper: Constructor for wrapping a Waypoint instance.
  - GotoPlanWrapper::GotoPlanWrapper: Constructor for wrapping a GotoPlan instance.
  - PersistPlayerScoreObjectWrapper::Reset: Resets the player score object.
  - PersistPlayerScoreObjectWrapper::GetRank: Gets the rank of the player score object.
  - PersistPlayerScoreObjectWrapper::GetScore: Gets the score of the player score object.
- lines 4924-5027
  - GameOverScoreObjectWrapper::Set: Sets a value on the wrapped instance.
  - GameOverScoreObjectWrapper::GetRank: Retrieves the rank of the score object.
  - GameOverScoreObjectWrapper::GetRecentTimePlayed: Returns the recent time played by the player.
  - GameOverScoreObjectWrapper::GetRecentBaseKills: Gets the number of base kills in the most recent game.
  - GameOverScoreObjectWrapper::GetRecentBaseCaptures: Retrieves the number of base captures in the last game.
  - GameOverScoreObjectWrapper::GetRecentKills: Returns the number of kills made by the player recently.
  - GameOverScoreObjectWrapper::GetRecentDeaths: Gets the number of deaths experienced by the player recently.
  - GameOverScoreObjectWrapper::GetRecentEjections: Retrieves the number of ejections from the game recently.

## `src/Wopr/AllegianceInterop/igcWrapper.h`

PURPOSE: The file `igcWrapper.h` contains a series of wrapper classes and functions to facilitate interaction with various game objects and data structures in Allegiance, primarily for managing player information, mission parameters, game entities, and score tracking.

KEY PARTS:
- PlayerInfoWrapper: Manages player-related data.
- VectorWrapper: Provides methods for vector operations.
- MissionParamsWrapper: Handles mission parameters.
- ShipLoadoutWrapper: Represents a ship's loadout.
- IprobeTypeIGCWrapper: Wraps probe type information.
- IhullTypeIGCWrapper: Wraps hull type details.
- IpartTypeIGCWrapper: Manages part types for ships.
- IweaponIGCWrapper: Extends part type to include weapon-specific properties.
- ShipStatusWrapper: Tracks the state of a ship.
- SideVisibilityWrapper: Manages visibility status for game sides.

CONNECTS TO: Allegiance core game engine and other related modules.

NOTES: None.

PARTS (in file order):

- lines 2-362
  - PlayerInfoWrapper: Wraps a PlayerInfo object with methods for accessing its properties.
  - VectorWrapper: Wraps a ::Vector object with methods for vector operations and conversions.
  - GeoWrapper: Wraps a ::Geo object.
  - ImapMakerIGCWrapper: Wraps an ImapMakerIGC object with methods to get and set map maker ID and name.
- lines 363-736
  - MissionParamsWrapper: Manages mission parameters.
  - ShipLoadoutWrapper: Represents a ship's loadout.
  - CompactOrientationWrapper: Handles compact orientation data.
  - CompactTimeWrapper: Manages compact time information.
  - CompactPositionWrapper: Deals with compact position data.
  - CompactStateWrapper: Encapsulates compact state information.
  - CompactVelocityWrapper: Manages compact velocity data.
  - CompactTurnRatesWrapper: Handles compact turn rates.
- lines 737-1075
  - CstaticIGC GetStaticCore(): Retrieves a static core instance.
  - void SetStaticCore(CstaticIGC pStatic): Sets the static core instance.
  - short GetReplayCount(): Returns the replay count.
  - String ^ GetContextName(): Gets the context name as a string.
  - void UpdateAllies(const char Allies[c_cSidesMax]): Updates ally information.
  - IbaseIGCWrapper: A base class for various game objects, wrapping an underlying C++ interface.
  - ImodelIGCWrapper: Wraps a model in the game, providing methods to interact with it.
  - IdamageIGCWrapper: Extends ImodelIGCWrapper to include damage-related functionality.
- lines 1076-1444
  - BuoyType: Represents a type of buoy.
  - IchaffIGCWrapper: Wrapper class for IIchaffIGC interface.
  - IprojectileIGCWrapper: Wrapper class for IprojectileIGC interface.
  - ImissileIGCWrapper: Wrapper class for ImissileIGC interface.
  - ImineIGCWrapper: Wrapper class for ImineIGC interface.
  - IbuildingEffectIGCWrapper: Wrapper class for IbuildingEffectIGC interface.
  - IprobeIGCWrapper: Wrapper class for IprobeIGC interface.
  - IstationIGCWrapper: Wrapper class for IstationIGC interface.
- lines 1445-1803
  - IprobeTypeIGCWrapper: Wraps an IprobeTypeIGC instance, providing access to scanner range, dt burst, dispersion, accuracy, ammo, projectile type, ripcord delay, and ambient sound.
  - IhullTypeIGCWrapper: Wraps an IhullTypeIGC instance, offering properties like length, max speed, turn rate, thrust, side multiplier, back multiplier, scanner range, max energy, recharge rate, hit points, defense type, part mask, capacity, weapon mounts, texture name, mass, signature, capabilities, scale, max ammo, max fuel, ECM, ripcord speed, ripcord cost, successor hull type, interior/exterior sounds, and preferred part types.
  - IpartTypeIGCWrapper: Wraps an IpartTypeIGC instance, providing details such as equipment type, part mask, mass, signature, successor part type, inventory line MDL name, and amount on a ship.
  - IlauncherTypeIGCWrapper: Extends IpartTypeIGCWrapper, adding functionality related to launchers, including expendable type.
  - IpartIGCWrapper: Base class for parts, providing common properties like equipment type, part type, ship, mount ID, active status, mass, price, mounted fraction, amount, and methods to arm and set mount.
  - IweaponIGCWrapper: Extends IpartIGCWrapper, adding specific details for weapons such as projectile type, ammo per shot, lifespan, dt burst, gunner, firing status, and sounds.
  - IshieldIGCWrapper: Extends IpartIGCWrapper, providing shield-specific properties like regeneration, max strength, defense type, fraction, and methods to set fraction.
  - IcloakIGCWrapper: Extends IpartIGCWrapper, offering cloak-related properties such as energy consumption, max cloaking, on/off rates, engagement/disengagement sounds.
- lines 1804-2170
  - IwarpIGCWrapper: Provides a managed wrapper for the native IwarpIGC class.
  - ItreasureIGCWrapper: Manages the ItreasureIGC class, providing functionality to interact with treasure objects.
  - IsideIGCWrapper: Represents a side in the game and provides methods to manage its properties and entities.
  - IcivilizationIGCWrapper: Wraps the IcivilizationIGC class, offering access to civilization-specific data.
  - ShipStatusWrapper: Manages the state of a ship within the game.
  - ClusterSiteWrapper: Handles cluster sites and their associated environments.
  - SideVisibilityWrapper: Tracks visibility status for sides in the game.
  - IIgcSiteWrapper: Provides methods for handling various site-related events and interactions.
- lines 2171-2267
  - SetCommanderScore(float fCommanderScore): Sets the commander score.
  - GetScore(): Retrieves the current score.
  - Reset(bool bFull): Resets the score data, optionally fully.
  - SetRankRatio(float rankRatio): Sets the rank ratio.
  - Connected(): Checks if the player is connected.
  - Commanding(): Determines if the player is commanding.
  - Connect(Time t): Initiates a connection at a specific time.
  - Disconnect(Time t): Terminates a connection at a specific time.

## `src/Wopr/AllegianceInterop/stdafx.cpp`

PURPOSE: tiny file (1 lines), shown in full rather than summarized.

      # i n c l u d e   " s t d a f x . h " 

## `src/Wopr/AllegianceInterop/stdafx.h`

PURPOSE: tiny file (3 lines), shown in full rather than summarized.

      # p r a g m a   o n c e 
     # i n c l u d e   < t i m e . h > 
     # d e f i n e   _ _ M O D U L E _ _   " A l l e g i a n c e I n t e r o p " 
