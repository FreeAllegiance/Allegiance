# src/Inc

[Back to the index](../LLAMA_SUMMARY.md) - 19 files, 9,846 lines. Notes by a local model, to orient by: not verified facts.

The `src/Inc` folder contains header files essential for various aspects of the Allegiance game, including networking, messaging, and database operations.

- AGCIDL.idl: Defines interfaces and enumerations for game objects, abilities, events, and utilities.
- PigsIDL.idl: Manages pig players, their behaviors, teams, missions, and ships.
- dplay8.h: Contains declarations for DirectPlay 8 interfaces used for networking.
- Messages.h: Defines message types for client-server communication.
- messagecore.h: Central to managing all messaging operations within the game.

Largest files: AGCIDL.idl (2753 lines), PigsIDL.idl (1462 lines), dplay8.h (1456 lines), Messages.h (1056 lines), messagecore.h (743 lines).

## `src/Inc/AGCIDL.idl`

PURPOSE: Defines interfaces and enumerations for various game objects, abilities, events, and utilities used in Allegiance.

KEY PARTS:
- AGCObjectType: Enumerates different types of game objects.
- IAGCVector: Interface for vector operations.
- IAGCOrientation: Interface for orientation matrix properties.
- IAGCEvent: Interface for game event handling.
- IAGCGameParameters: Interface for managing game parameters.
- ITCSessionInfo: Interface exposing session connection details.

CONNECTS TO: Allegiance game engine and other related files.

NOTES:
- Contains both public and private interfaces, indicating additional functionality not exposed to the user.
- Some properties have default values specified, which might be an oversight or intentional design choice.
- Uses HRESULT return types typical of COM programming.

PARTS (in file order):

- lines 1-343
  - AGCObjectType: Enumerates different types of objects in the game.
  - AGCHullAbility: Enumerates abilities that can be assigned to hulls.
  - AGCAxis: Enumerates axes for orientation and movement.
  - AGCEquipmentType: Enumerates types of equipment that can be mounted on ships or stations.
  - AGCTreasureType: Enumerates different types of treasures in the game.
  - AGCChatTarget: Enumerates targets for chat messages.
  - AGCEventID: Enumerates different types of events that can occur in the game.
  - FileAttributes: Enumerates attributes that can be applied to files.
- lines 344-710
  - AGCObjectType: Enumerates types of game objects.
  - AGCHullAbility: Bitmask for hull abilities.
  - AGCAsteroidAbility: Bitmask for asteroid abilities.
  - AGCAxis: Enumerates axes for movement and controls.
  - AGCEquipmentType: Enumerates types of equipment.
  - AGCTreasureType: Enumerates types of treasures.
  - AGCChatTarget: Enumerates chat targets.
  - AGCGameStage: Enumerates game stages.
- lines 711-1086
  - IAGCVector: Interface for a vector object with properties and methods for operations like addition, subtraction, normalization, etc.
  - IAGCOrientation: Interface for an orientation matrix with properties for forward, backward, up, right vectors.
  - IAGCEvent: Interface for game events with properties for ID, time, description, and parameters.
  - IAGCGameParameters: Interface for game parameters with various properties like min/max players, power-ups, resources, etc.
  - The interfaces use HRESULT return types which are common in COM programming but may not be familiar to non-COM developers.
  - There are multiple private interfaces (IAGCVectorPrivate, IAGCOrientationPrivate) that suggest there is additional functionality not exposed through the public interfaces.
  - Some properties have default values specified in their declarations, which is unusual and might indicate a mistake or oversight.
- lines 1087-1449
  - IAGCCommand: Interface to an AGC Command object.
  - IAGCCommandPrivate: Private interface to an AGC Command object.
  - IAGCVersionInfo: Interface for version information.
  - IAGCDBParams: Interface for database parameters.
  - IAGCEventLogger: Interface for logging events.
  - IAGCEventLoggerHook: Interface for hooking event logging.
  - IAGCEventLoggerPrivate: Private interface for event logging.
  - IAGCRangePrivate: Private interface to an AGC Range object.
- lines 1451-1787
  - IAGCEventIDRanges: Interface for managing a collection of event ID ranges.
  - IntersectsWithValue: Determines if a specific value intersects with any range in the set.
  - IntersectsWithRangeValues: Determines if a specified range intersects with any range in the set.
  - IntersectsWithRange: Determines if a specified range object intersects with any range in the set.
  - ITCCollection: Base interface for most collection interfaces, providing methods like Count and _NewEnum.
  - ITCStrings: Interface to manage a collection of strings, including adding, removing, and manipulating strings.
  - ITCPropBagOnRegKey: Interface to create a property bag on a registry key, allowing operations like creating, opening, and saving objects.
  - ITCSessionInfo: Interface exposing information about the connection of a session object, including user name, computer name, application name, creation time, duration, and cookie.
- lines 1788-2168
  - ITCSessionInfos: Interface for accessing session information.
  - ITCCollectionPersistHelper: Interface for collection persistence helper functionality.
  - ITCUtility: Utility interface providing various utility functions.
  - IAdminSessionHelper: Helper interface for administrative session management.
  - IAGCPrivate: Internally-used interface on an AGC object.
  - IAGCCommon: Base interface for common AGC properties and methods.
  - IAGCCollection: Base interface for AGC collection interfaces.
  - IAGCBase: Interface for basic AGC object properties.
- lines 2169-2560
  - IAGCHullType: Interface for hull types with properties like capabilities and max ammo.
  - IAGCTreasure: Interface for treasures with properties like type and amount.
  - IAGCTreasures: Collection interface for multiple treasures.
  - IAGCDamage: Interface for damage objects with a fraction property.
  - IAGCScanner: Interface extending IAGCDamage, adding scanner-related properties.
  - IAGCProbe: Interface extending IAGCScanner, adding probe-specific properties like emission point and lifespan.
  - IAGCProbes: Collection interface for multiple probes.
  - IAGCGame: Main game interface with properties and methods to manage game state.
- lines 2561-2752
  - IAGCTeam Interface: Defines methods and properties for managing a team in the game.
  - IAGCTeams Interface: Provides access to a collection of teams.
  - IAGCAsteroid Interface: Represents an asteroid with properties like ore amount and capabilities.
  - IAGCAsteroids Interface: Manages a collection of asteroids.
  - Smart Pointer Declarations: Defines smart pointer types for COM interfaces, aiding in memory management.
  - TODOs or Unusual Code: The commented-out sections suggest potential future features or unfinished code. The use of cpp_quote to include C++ code within an IDL file is unusual and specific to Microsoft's tools.

## `src/Inc/ClubMessages.h`

PURPOSE:
ClubMessages.h defines messages and structures for communication between the AllClub system and the Allegiance game. It includes various message types for logging on/off, requesting character information, managing squads, querying leaderboards, and retrieving rank information.

KEY PARTS:
- LOGON_CLUB: Client logs into the Club server.
- LOGOFF_CLUB: Client logs off from the Club server.
- CHARACTER_INFO_BY_CIV: Server responds with civ-specific character info.
- SQUAD_CREATE: Client requests to create a new squad.
- SQUAD_DETAILS_REQ: Client requests details about a specific squad.
- LEADER_BOARD_QUERY: Client queries the leaderboard for a specific character or rank.
- CLUB_RANK_INFO: Server sends rank information for all civilizations.

CONNECTS TO:
- Squad.h and Member.h: For squad and member data structures.
- messagecore.h: For core messaging framework definitions.

NOTES:
- None.

## `src/Inc/IBN.H`

PURPOSE: tiny file (5 lines), shown in full rather than summarized.

    // INCBUILDNUM can be used when rebuilding w/o a full ssync.
    // eg: from 1.00.nnnn.0 to 1.00.nnnn.1, etc.
    //
    // INCBUILDNUM must be hand-set (ie: it is not done by slm automatically)
    #define INCBUILDNUM 0  // incremental build numbers for patches on build machine

## `src/Inc/MessageVersion.h`

PURPOSE: This header file defines a constant representing the version of message structures used in the Allegiance game. It is incremented with each change to ensure client and server versions are synchronized.

KEY PARTS:
- MSGVER: A constant integer representing the current version number of message structures.

CONNECTS TO: parts.h

NOTES: None

## `src/Inc/Messages.h`

PURPOSE: The file `Messages.h` contains definitions for various message types used in the Allegiance game's client-server communication system.

KEY PARTS:
- PassengerData: Stores information about passengers on a ship.
- LOGONREQ and LOGONACK: Handle player logon requests and responses.
- BUY_LOADOUT and BUY_LOADOUT_ACK: Manage loadout purchases by clients.
- OBJECT_SPOTTED and SET_CLUSTER: Notify clients of spotted objects and update cluster information.

CONNECTS TO: `Messages.cpp`, which contains implementations for these message types.

NOTES: None.

## `src/Inc/PigsIDL.idl`

PURPOSE: Defines interfaces and enumerations for managing pig players, their behaviors, teams, missions, and ships within the Allegiance game.

KEY PARTS:
- IPigBehaviorType: Interface to a pig behavior type object.
- IPig: Interface representing a pig player object.
- IPigMissionParams: Interface for mission parameters.
- IPigShip: Interface for controlling a ship in Allegiance.
- PigMapType: Enumerated type for mission map types.

CONNECTS TO: None

NOTES: Contains both C++ and C# code, with some TODO comments.

PARTS (in file order):

- lines 1-340
  - dispid_PigsIDL: Enumeration of dispatch identifiers for PigsIDL.
  - PigState: Enumerates states a pig player can be in.
  - IPigBehaviorType: Interface to a pig behavior type object.
  - IPigBehaviorTypes: Interface to a collection of pig behavior script objects.
  - IPigBehaviorScriptType: Interface to a pig behavior script object, extending IPigBehaviorType.
  - IPigBehaviorHost: Interface to a pig behavior host object.
  - IPigBehavior: Interface to a pig behavior object.
- lines 341-745
  - IPigBehavior: Interface for behavior objects associated with a pig player.
  - IPigBehaviors: Interface to a collection of pig behavior objects.
  - IPigBehaviorStack: Interface to a stack of pig behavior objects.
  - IPig: Interface representing a pig player object.
  - IPigs: Interface to a collection of pig player objects.
  - IPigTeam: Interface representing a team object.
  - IPigTeams: Interface to a collection of team objects.
  - PigMapType: Enumerated type for mission map types.
- lines 746-1124
  - IPigMissionParams: Interface for mission parameters.
  - IPigSession: Main interface for session management.
  - IPigEvent: Interface for event handling.
  - IPigTimer: Interface for timer events.
  - PigThrust: Enumerates thrust actions.
  - TODO: Does this go into teams? - Comment in IPigMission interface.
- lines 1126-1460
  - IPigShip: Interface for controlling a ship in Allegiance.
  - IPigShipEvent: Interface for handling events related to a ship's actions.
  - IPigVector: Interface representing a 3D vector.
  - IPigAccount: Interface for managing user accounts.
  - IPigAccountDispenser: Interface for dispensing and managing user accounts.
  - IPigHullTypes: Interface for accessing a collection of hull types.
  - TC_COM_SMARTPTR_TYPEDEF: Macro to define smart pointers for COM interfaces.

## `src/Inc/ScrRun.idl`

**PURPOSE:** This file is an IDL (Interface Definition Language) file for the Scripting.FileSystemObject library in Microsoft's SCRRUN.DLL. It defines interfaces and types for accessing and manipulating files, folders, drives, and text streams.

**KEY PARTS:**
- **IDictionary**: Interface for a dictionary object that stores key-value pairs.
- **IFileSystem**: Interface for file system operations such as creating, deleting, and copying files and folders.
- **IDrive**: Interface for representing a drive in the file system.
- **IFolder**: Interface for representing a folder in the file system.
- **IFile**: Interface for representing a file in the file system.
- **ITextStream**: Interface for reading from and writing to text streams.
- **IScriptEncoder**: Interface for encoding scripts.

**CONNECTS TO:** SCRRUN.DLL

**NOTES:** None.

## `src/Inc/SlmVer.h`

PURPOSE: tiny file (12 lines), shown in full rather than summarized.

    // mmf modified version, these set FileVersion and ProductVersion used by VerRes.rc
    #define rmj		1
    #define rmm		9		// Imago (R#)
    #define rel     1040158.00       // mmf added to designate release number for a given build
    #define rup		311412  // mmf make sure this has 6 digits otherwise version number conversion for esc display may fail,
    						// rup is specified in octal, so for instance 11/9/2010 (US format)
    						// Year: 2010 - use last two digits: 10, convert to octal = 12.
    						// Month: 11 - convert to octal = 13
    						// Day: 9 - convert to octal = 11
    						// Actual rup value = 121311.
    #define szVerName	""
    #define szVerUser	"CURTC3"

## `src/Inc/Utility.h`

PURPOSE: tiny file (1 lines), shown in full rather than summarized.

    #include "../_Utility/Utility.h"

## `src/Inc/XMLPtrs.h`

PURPOSE: This file defines smart pointer types for Microsoft XML (MSXML) interfaces in C++.

KEY PARTS:
- TC_COM_SMARTPTR_TYPEDEF(Interface): Macro to define smart pointers for COM interfaces.
- IXMLDOMImplementation, IXMLDOMNode, etc.: Smart pointers for various MSXML interface types.

CONNECTS TO: msxml2.h

NOTES: None.

## `src/Inc/allegdb.h`

PURPOSE: This file contains the definitions and implementations for a database layer in Allegiance, using OLE DB. It manages SQL queries, including their execution, preparation, and notification.

KEY PARTS:
- CSQLCore: Manages the overall SQL operations, maintaining two queues for different types of queries.
- CSQLQuery: Base class for all SQL queries, defining methods that derived classes must implement.
- CSQLThread: Handles the execution of queries in separate threads based on whether they require notification.
- CSQLQueueThread: Manages adding queries to the appropriate queue.
- CQuery: Template class for creating specific types of queries with associated data structures.

CONNECTS TO:
- Allegiance game engine, using OLE DB for database interactions.

NOTES:
- Contains macros for easily defining query classes.
- Includes mutexes and critical sections for thread safety.
- Some TODOs and commented-out code related to mutex usage and compatibility.

## `src/Inc/assert.h`

PURPOSE: tiny file (2 lines), shown in full rather than summarized.

    // Empty include file so that no-one defines assert!
    #include <zassert.h>

## `src/Inc/dpaddr.h`

PURPOSE: This file defines interfaces and constants for DirectPlay8Address, which is used to represent network addresses in Microsoft's DirectPlay networking API.

KEY PARTS:
- `IDirectPlay8Address`: Interface for creating and manipulating network addresses.
- `IDirectPlay8AddressIP`: Subinterface for IP-based addresses.
- CLSIDs and IIDs for DirectPlay8Address objects.
- Constants for address components like hostname, port, and provider.
- Functions to build addresses from URLs or SOCKADDR structures.

CONNECTS TO: DirectPlay8 API (dplay8.h)

NOTES: None.

## `src/Inc/dplay8.h`

PURPOSE: The file `dplay8.h` contains declarations for DirectPlay 8 interfaces and structures used for client-server communication and peer-to-peer networking in applications.

KEY PARTS:
- CLSID_DirectPlay8Client, CLSID_DirectPlay8Server, CLSID_DirectPlay8Peer: Class IDs for different DirectPlay 8 components.
- PFNDPNMESSAGEHANDLER: Callback function type for handling messages.
- DPN_APPLICATION_DESC: Describes an application and its session settings.
- IDirectPlay8Client, IDirectPlay8Server, IDirectPlay8Peer: COM interfaces for client, server, and peer functionalities.
- DPNMSG_ADD_PLAYER_TO_GROUP, DPNMSG_ASYNC_OP_COMPLETE: Message types indicating player addition to a group and completion of asynchronous operations.

CONNECTS TO: DirectPlay 8 service provider implementations.

NOTES: None.

PARTS (in file order):

- lines 1-392
  - CLSID_DirectPlay8Client: Class ID for DirectPlay8 client.
  - CLSID_DirectPlay8Server: Class ID for DirectPlay8 server.
  - CLSID_DirectPlay8Peer: Class ID for DirectPlay8 peer.
  - IID_IDirectPlay8Client: Interface ID for DirectPlay8 client.
  - IID_IDirectPlay8Server: Interface ID for DirectPlay8 server.
  - IID_IDirectPlay8Peer: Interface ID for DirectPlay8 peer.
  - PFNDPNMESSAGEHANDLER: Callback function type for handling messages.
  - DPNID: Type for player IDs.
- lines 393-730
  - DPN_APPLICATION_DESC: Describes an application and its session settings.
  - BUFFERDESC: Defines a buffer for data transfer.
  - DPN_CAPS: Stores capabilities of a DirectPlay8 service provider.
  - DPN_CONNECTION_INFO: Contains statistics about the connection.
  - DPN_GROUP_INFO: Holds information about a group in a session.
  - DPN_PLAYER_INFO: Contains details about a player in a session.
  - DPNMSG_ADD_PLAYER_TO_GROUP: Message indicating a player has been added to a group.
  - DPNMSG_ASYNC_OP_COMPLETE: Indicates completion of an asynchronous operation.
- lines 731-1058
  - IDirectPlay8Client: COM interface for DirectPlay8 client functionality.
  - IDirectPlay8Server: COM interface for DirectPlay8 server functionality.
  - IDirectPlay8Peer: COM interface for DirectPlay8 peer functionality.
  - DPNMSG_ENUM_HOSTS_RESPONSE: Structure for handling responses from EnumHosts method.
  - DPNMSG_GROUP_INFO: Structure for group information messages.
  - DPNMSG_HOST_MIGRATE: Structure for host migration messages.
  - DPNMSG_INDICATE_CONNECT: Structure for indicating a connection attempt.
  - DPNMSG_INDICATED_CONNECT_ABORTED: Structure for indicating a connection abort.
- lines 1059-1455
  - IDirectPlay8Client: Interface for client operations.
  - IDirectPlay8Server: Interface for server operations.
  - IDirectPlay8Peer: Interface for peer-to-peer operations.
  - IDirectPlay8ThreadPool: Interface for managing thread pools.
  - IDirectPlay8NATResolver: Interface for NAT traversal.
  - DPN_OK: Success code.
  - DPNERR_ABORTED: Operation aborted error.
  - TODOs or unusual code: No specific TODOs or unusual code noted in this snippet.

## `src/Inc/messagecore.h`

**PURPOSE:** The `messagecore.h` header file in Allegiance is central to managing all messaging operations within the game. It includes classes and functions for handling connections, groups of connections, message creation, sending, receiving, and session management.

**KEY PARTS:**
- **FedMessaging**: Manages all messaging operations.
- **CFMConnection**: Represents a connection to another entity (client/server).
- **CFMGroup**: Manages groups of connections.
- **FMSessionDesc**: Describes session details.
- **FMGuaranteed and FMFlush**: Enums for message delivery guarantees and flushing options.
- **PFedMsgCreate**: Macro-based function to create messages.
- **MsgHandler**: Handles incoming DirectPlay messages.
- **Connect, HostSession, JoinSession**: Functions for initiating connections, hosting sessions, and joining existing ones.
- **SendMessages and ReceiveMessages**: Functions for sending and receiving messages.

**CONNECTS TO:** DirectPlay (DPlay) for network communication.

**NOTES:**
- The file includes macros and utility functions not shown here.
- There is a commented-out section for including DirectX headers.
- Uses `CTempTimer` for performance monitoring, indicating importance on profiling.

## `src/Inc/quickchat.h`

PURPOSE: This file contains definitions for various classes related to quick chat functionality in the Allegiance game, including voice messages and menu items.

KEY PARTS:
- SonicChatVoice: Represents a voice message with associated sound and text.
- SonicChat: Manages a list of voice messages.
- QuickChatCommand: Represents a command within the quick chat system, including its ID, target type, and ability mask.
- QuickChatMenuItem: Represents an item in a quick chat menu, containing a character, string, and associated node.
- QuickChatMenu: Represents a menu in the quick chat system, containing a list of items.

CONNECTS TO: IMDLObject, IObjectList

NOTES: None

## `src/Inc/regkey.h`

PURPOSE: This file defines constants and macros related to registry keys used by the Allegiance game for storing configuration settings, service names, and other system-specific paths.

KEY PARTS:
- ALLEGIANCE_REGISTRY_KEY_ROOT: Defines the root path of the registry key based on whether the build is development or production.
- c_szSvcName: The name of the main service used by Allegiance.
- HKLM_FedSrv: Registry key for the main server service.
- HKLM_AllSrvUI: Registry key for the user interface related to the server.
- HKLM_AllLobby: Registry key for the lobby service.
- HKLM_AllClub: Registry key for the club service.
- HKLM_PigSrv: Registry key for the Pig service.

CONNECTS TO: Windows registry system, Allegiance game services.

NOTES: None.

## `src/Inc/sharemem.h`

PURPOSE: This file defines structures and a class for managing shared memory between a test tool and a performance monitoring DLL in Allegiance.

KEY PARTS:
- SHARE_INSTANCE: Structure representing an instance element in the shared memory.
- SHARE_HEADER: Structure representing the header information for the shared memory.
- CPerfShare: Class for managing the shared memory, including initialization, allocation, and deallocation of counters.

CONNECTS TO: sharemem.cpp (contains implementation details).

NOTES: None.

## `src/Inc/steam_appid.txt`

PURPOSE: tiny file (1 lines), shown in full rather than summarized.

    700480
