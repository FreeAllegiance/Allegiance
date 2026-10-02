# src/AGC

[Back to the index](../LLAMA_SUMMARY.md) - 155 files, 21,796 lines. Notes by a local model, to orient by: not verified facts.

The AGC folder in Allegiance's source code contains essential components for managing game objects and operations. It includes classes for handling game parameters, events, models, hull types, probes, asteroids, and more.

- AGCGameParameters.cpp: Manages various game parameters including mission-specific settings.
- AGCGlobal.cpp: Handles global state and event handling in the game.
- AGCHullType.cpp: Represents properties and behaviors of different ship hull types.
- AGCEventData.cpp: Manages event data, including serialization and deserialization.
- AGCEventLogger.cpp: Responsible for logging events within the game using both NT Event Log and database logging.

Largest files: AGCGameParameters.cpp (2896 lines), TCPropBagOnRegKey.cpp (1433 lines), AGCEventLogger.cpp (1390 lines), agcevent.cpp (1053 lines), AGCEvents.xml (900 lines).

## `src/AGC/AGC.cpp`

PURPOSE: This file contains the implementation of a DLL for the Allegiance Active Game Core, providing functionality related to game objects and management.

KEY PARTS:
- CAGCModule: Manages the module's lifecycle.
- DllMain: Entry point for the DLL, handling initialization and termination.
- DllCanUnloadNow: Determines if the DLL can be unloaded by COM.
- DllGetClassObject: Returns a class factory to create objects of requested types.
- DllRegisterServer: Registers the DLL with the system registry.
- DllUnregisterServer: Unregisters the DLL from the system registry.

CONNECTS TO: Other Allegiance game components and libraries, specifically those related to game objects and management.

NOTES: None.

## `src/AGC/AGC.def`

PURPOSE: tiny file (9 lines), shown in full rather than summarized.

    ; AGC.def : Declares the module parameters.
    LIBRARY      "AGC.DLL"
    EXPORTS
        DllCanUnloadNow     PRIVATE
        DllGetClassObject   PRIVATE
        DllRegisterServer   PRIVATE
        DllUnregisterServer PRIVATE
        DllRegisterAGC
        DllUnregisterAGC

## `src/AGC/AGC.idl`

PURPOSE: This file defines the interface definitions and coclasses for the Allegiance game's core components using Microsoft's IDL (Interface Definition Language). It is used to generate type libraries and marshalling code for COM-based communication.

KEY PARTS:
- AGCLib: Defines the main library containing all interfaces and coclasses.
- IAGCEventSink, IAGCGlobal, etc.: Various interfaces representing different game components like events, global settings, vectors, orientations, and more.
- AGCInterfaces: A hidden coclass that references other interfaces to ensure they are included in the type library.
- AGCGlobal, AGCVector, AGCOrientation, etc.: Individual coclasses each implementing a specific interface.

CONNECTS TO: Other IDL files like "AGCIDL.idl" and standard COM libraries such as "stdole32.tlb" and "stdole2.tlb".

NOTES: None.

## `src/AGC/AGCAleph.cpp`

PURPOSE: This file contains the implementation of the CAGCAleph class, which is part of the Allegiance game's AGC module. It handles error information for the IAGCAleph interface.

KEY PARTS:
- CAGCAleph: The main class that implements the IAGCAleph interface.
- InterfaceSupportsErrorInfo: A method to check if the object supports a given error interface.

CONNECTS TO: None explicitly mentioned, but likely depends on other game systems for context and functionality.

NOTES: None.

## `src/AGC/AGCAleph.h`

PURPOSE: This file defines the interface and implementation for a COM object named CAGCAleph, which is part of the Allegiance game's AGCLib library.

KEY PARTS:
- Name: CAGCAleph
  - Provides COM interfaces for game-related functionality.
- DECLARE_REGISTRY_RESOURCEID(IDR_AGCAleph)
  - Registers the COM object with a resource ID.
- DECLARE_PROTECT_FINAL_CONSTRUCT()
  - Ensures that the constructor is protected and final.
- BEGIN_COM_MAP(CAGCAleph)
  - Maps COM interfaces to their implementations.
- BEGIN_CATEGORY_MAP(CAGCAleph)
  - Specifies categories for the COM object.

CONNECTS TO: IwarpIGC, IAGCAleph (other interfaces in the AGCLib library).

NOTES: None.

## `src/AGC/AGCAleph.rgs`

PURPOSE: This file is a Windows Registry Script (.rgs) that defines the class information for the AGCAleph object in Allegiance. It registers the CLSID and ProgID for the AGCAleph class.

KEY PARTS:
- Name: AGC.AGCAleph.1, AGC.AGCAleph
  - What it does: These are the versioned and non-versioned ProgIDs for the AGCAleph class.
- CLSID: {46AF05A8-0325-11D3-8B57-00C04F681633}
  - What it does: This is the unique identifier (CLSID) for the AGCAleph class.
- TypeLib: {AB33E136-0A3E-11D3-8B58-00C04F681633}
  - What it does: This links to the type library that contains the interface definitions for the AGCAleph class.

CONNECTS TO:
- Allegiance game engine, specifically related to object registration and COM components.

NOTES:
- None.

## `src/AGC/AGCAlephs.cpp`

PURPOSE: tiny file (8 lines), shown in full rather than summarized.

    /////////////////////////////////////////////////////////////////////////////
    // AdminAGCAlephs.cpp : Implementation of CAGCAlephs
    //
    #include "pch.h"
    #include "AGCAlephs.h"
    /////////////////////////////////////////////////////////////////////////////
    // CAGCAlephs
    TC_OBJECT_EXTERN_NON_CREATEABLE_IMPL(CAGCAlephs)

## `src/AGC/AGCAlephs.h`

PURPOSE: This file defines a COM class `CAGCAlephs` that implements the `IAGCAlephs` interface and extends functionality through the `IAGCCollectionImpl` template.

KEY PARTS:
- CAGCAlephs: A COM class implementing `IAGCAlephs` and extending `IAGCCollectionImpl`.
- DECLARE_REGISTRY_RESOURCEID: Registers the class with a resource ID.
- DECLARE_PROTECT_FINAL_CONSTRUCT: Protects against premature destruction of the object.
- BEGIN_COM_MAP: Maps COM interfaces to their implementations.
- BEGIN_CATEGORY_MAP: Categorizes the component for COM registration.

CONNECTS TO: AGCLib library and related interfaces.

NOTES: None.

## `src/AGC/AGCAlephs.rgs`

PURPOSE: This file is a Windows Registry Script (RGS) that defines the class information for the AGCAlephs COM object used in Allegiance.

KEY PARTS:
- Name: AGC.AGCAlephs.1 and AGC.AGCAlephs
  - What it does: These are ProgIDs for the AGCAlephs COM object, which is likely a core component of the game's functionality.
  
- CLSID: {991B6C06-026B-11D3-8B55-00C04F681633}
  - What it does: This is the unique identifier for the AGCAlephs COM object, used to register and instantiate the object in Windows.

- TypeLib: {AB33E136-0A3E-11D3-8B58-00C04F681633}
  - What it does: This is the identifier for the type library associated with the AGCAlephs COM object, which contains information about the object's methods and properties.

CONNECTS TO:
- Allegiance game engine

NOTES:
- none

## `src/AGC/AGCAsteroid.cpp`

PURPOSE: This file contains the implementation of the CAGCAsteroid class, which represents asteroids in the Allegiance game.

KEY PARTS:
- Name: CAGCAsteroid - Manages asteroid objects within the game.
- InterfaceSupportsErrorInfo - Checks if the interface supports error information.

CONNECTS TO: AGCAsteroid.h

NOTES: None.

## `src/AGC/AGCAsteroid.h`

PURPOSE: This file declares and implements the CAGCAsteroid class, which represents an asteroid in the Allegiance game. It provides functionality for managing asteroid objects through COM interfaces.

KEY PARTS:
- Name: CAGCAsteroid
  - What it does: Manages asteroid objects within the game.
- Name: DECLARE_REGISTRY_RESOURCEID
  - What it does: Registers the class with a resource ID.
- Name: DECLARE_PROTECT_FINAL_CONSTRUCT
  - What it does: Ensures that the constructor is protected and final.
- Name: BEGIN_COM_MAP
  - What it does: Maps COM interfaces to their implementations.
- Name: ISupportErrorInfo
  - What it does: Provides error information support for COM objects.

CONNECTS TO:
- IAGCAsteroidImpl.h: Contains the implementation details of the asteroid interface.

NOTES:
- None.

## `src/AGC/AGCAsteroid.rgs`

PURPOSE: This file is a Windows Registry Script (.rgs) that defines the class information for the AGCAsteroid object in the Allegiance game. It specifies the CLSID and ProgID for the asteroid class, as well as versioning information.

KEY PARTS:
- Name: AGC.AGCAsteroid
  - Description: The main ProgID for the AGCAsteroid class.
- CLSID: {12430BFC-38B5-11d3-A51D-00C04F68DEB0}
  - Description: The unique identifier for the AGCAsteroid class.
- VersionIndependentProgID: AGC.AGCAsteroid
  - Description: Provides a version-independent ProgID for compatibility.

CONNECTS TO:
- TypeLib: {AB33E136-0A3E-11D3-8B58-00C04F681633}
  - Description: The type library that contains the interface definitions for the AGCAsteroid class.

NOTES: None.

## `src/AGC/AGCAsteroids.cpp`

PURPOSE: tiny file (8 lines), shown in full rather than summarized.

    /////////////////////////////////////////////////////////////////////////////
    // AGCAsteroids.cpp : Implementation of the CAGCAsteroids class.
    //
    #include "pch.h"
    #include "AGCAsteroids.h"
    /////////////////////////////////////////////////////////////////////////////
    // CAGCAsteroids
    TC_OBJECT_EXTERN_NON_CREATEABLE_IMPL(CAGCAsteroids)

## `src/AGC/AGCAsteroids.h`

PURPOSE: This file defines the interface and implementation for a collection of asteroids in the Allegiance game.

KEY PARTS:
- CAGCAsteroids: Class implementing the IAGCAsteroids interface to manage a collection of asteroids.
- DECLARE_REGISTRY_RESOURCEID, DECLARE_PROTECT_FINAL_CONSTRUCT: Macros for registry management and final construction protection.
- BEGIN_COM_MAP, END_COM_MAP: Maps COM interfaces to class methods.
- BEGIN_CATEGORY_MAP, END_CATEGORY_MAP: Maps categories to the class.

CONNECTS TO: IAGCCollectionImpl.h (for collection implementation details).

NOTES: None.

## `src/AGC/AGCAsteroids.rgs`

PURPOSE: This file is a Windows Registry Script that defines the class information for the AGCAsteroids component in Allegiance.

KEY PARTS:
- Name: AGC.AGCAsteroids.1
  - Description: Represents the version 1 of the AGCAsteroids class.
- CLSID: {12430BFD-38B5-11d3-A51D-00C04F68DEB0}
  - Unique identifier for the AGCAsteroids component.
- CurVer: AGC.AGCAsteroids.1
  - Specifies the current version of the class.
- ProgID: AGC.AGCAsteroids.1
  - Programmatic Identifier used to reference the class in code.
- VersionIndependentProgID: AGC.AGCAsteroids
  - Alternative identifier that is independent of version numbers.

CONNECTS TO:
- TypeLib: {AB33E136-0A3E-11D3-8B58-00C04F681633}
  - References the type library for the AGCAsteroids component.

NOTES:
- None.

## `src/AGC/AGCCP.h`

PURPOSE: This file contains template classes that implement connection points for various game events in the Allegiance game engine. These classes facilitate communication between different components of the game by allowing objects to subscribe to and receive notifications about specific events.

KEY PARTS:
- CProxy_IAGCGameEvents: Manages connections for general game events.
- CProxy_IAGCShipEvents: Manages connections for ship-specific events.
- CProxy_IAGCTeamEvents: Manages connections for team-related events, including a method to fire an event with text data.
- CProxy_IAGCSectorEvents: Manages connections for sector-specific events, including a method to fire an event when a new station is created.

CONNECTS TO: Other components of the Allegiance game engine that need to subscribe to or trigger these events.

NOTES:
- The `Fire_Event_Blank` and `Fire_OnNewStation` methods use COM interfaces to invoke methods on subscribed objects.
- There are TODO comments indicating potential enhancements or further development needed.

## `src/AGC/AGCChat.h`

PURPOSE: This file contains declarations and utility functions related to chat functionality in the Allegiance game.

KEY PARTS:
- FindCommandName(const char* pszCommand, CommandID* pID, REFCLSID clsid, REFIID iid): Searches for a command string and returns its corresponding ID.
- FindCommandName(const OLECHAR* pszCommand, CommandID* pID, REFCLSID clsid, REFIID iid): Overloaded function to handle Unicode strings.

CONNECTS TO: igc.h, agc.h

NOTES: None.

## `src/AGC/AGCCommand.cpp`

PURPOSE: This file contains the implementation of the CAGCCommand class, which represents a command in the Allegiance game. It implements various interfaces for serialization, persistence, and property management.

KEY PARTS:
- CAGCCommand: Implements IAGCCommand, IPersist, IPersistStreamInit, IPersistPropertyBag interfaces to handle command data.
- get_Target, get_Verb, get_Text: Retrieve properties of the command.
- Init: Initializes the command with target and verb.
- GetClassID: Returns the class ID of the object.
- IsDirty, Load, Save, GetSizeMax, InitNew: Methods for persistence and dirty flag management.
- Load, Save (IPersistPropertyBag): Methods for loading and saving properties using a property bag.

CONNECTS TO: Allegiance game engine components that require command handling and persistence.

NOTES: None.

## `src/AGC/AGCCommand.h`

**PURPOSE:** The file `AGCCommand.h` declares and defines the class `CAGCCommand`, which implements various interfaces related to command objects in the Allegiance game. It provides functionality for managing command targets, verbs, and text, as well as persistence mechanisms for saving and loading commands.

**KEY PARTS:**
- **Class:** `CAGCCommand`
  - Implements interfaces like `IAGCCommand`, `IDispatch`, `IPersistStreamInit`, `IPersistPropertyBag`, and others.
  - Manages command properties such as target, verb, and text.
  - Provides methods for initializing commands, persisting their state, and handling property bags.

- **Methods:**
  - `get_Target`, `get_Verb`, `get_Text`: Retrieve the target, verb, and text of the command.
  - `Init`: Initializes the command with a target and verb.
  - Persistence methods like `IsDirty`, `Load`, `Save`, `GetSizeMax`, and `InitNew`.

- **Data Members:**
  - `m_bstrTarget`, `m_bstrVerb`: BSTRs to store the target and verb of the command.
  - `m_bDirty`: A boolean flag indicating if the command has been modified.

**CONNECTS TO:** 
- Other interfaces and classes within the Allegiance game, particularly those related to game objects and command handling.

**NOTES:**
- None.

## `src/AGC/AGCCommand.rgs`

PURPOSE: This file is an RGS (Registration Script) used to register a COM class for the Active Game Core Command in Allegiance.

KEY PARTS:
- AGC.Command.1: ProgID for the command class version 1.
- CLSID {4272AD82-FF3F-11d2-A50F-00C04F68DEB0}: Unique identifier for the COM class.
- TypeLib {AB33E136-0A3E-11D3-8B58-00C04F681633}: Reference to the type library containing interface definitions.
- InprocServer32: Path to the DLL that implements the COM class.

CONNECTS TO: Allegiance game engine, specifically the Active Game Core.

NOTES: None.

## `src/AGC/AGCDBParams.cpp`

PURPOSE: This file contains the implementation of the `CAGCDBParams` class, which manages database connection parameters and provides persistence methods for saving and loading these parameters.

KEY PARTS:
- `CAGCDBParams`: Manages database connection string and table name.
- `put_ConnectionString`, `get_ConnectionString`: Set and get the database connection string.
- `put_TableName`, `get_TableName`: Set and get the database table name.
- `IsDirty`: Checks if the parameters have been modified since last save.
- `Load`, `Save`: Persist the parameters to a stream or property bag.
- `GetSizeMax`: Returns the maximum size of the persisted data.

CONNECTS TO: Allegiance game engine, likely used for managing database connections in various parts of the game.

NOTES: None.

## `src/AGC/AGCDBParams.h`

PURPOSE: This file defines a COM object class `CAGCDBParams` that implements interfaces for managing database connection parameters and persistence.

KEY PARTS:
- Name: CAGCDBParams
  - Manages database connection string and table name.
- Interface Methods:
  - put_ConnectionString, get_ConnectionString: Set and get the database connection string.
  - put_TableName, get_TableName: Set and get the database table name.
- Persistence Interfaces:
  - IPersistStreamInit, IPersistPropertyBag: Provides methods for saving and loading object state.

CONNECTS TO: AGC.h, resource.h, AGCLib.lib

NOTES: None.

## `src/AGC/AGCDBParams.rgs`

PURPOSE: This file defines a registry entry for a COM class used in the Active Game Core Database Parameters Class of Allegiance.

KEY PARTS:
- Name: AGC.DBParams.1 - The versioned ProgID for the database parameters class.
- CLSID: {85A50536-6442-41bc-B011-2BFD93CFB5A3} - Unique identifier for the COM class.
- TypeLib: {AB33E136-0A3E-11D3-8B58-00C04F681633} - Reference to the type library containing the interface definitions.
- InprocServer32: %MODULE% - Path to the DLL that implements the COM class.

CONNECTS TO: Allegiance game engine, specifically for managing database parameters.

NOTES: None.

## `src/AGC/AGCDebugHook.h`

PURPOSE: This file defines a COM object that implements the IAGCDebugHook interface for debugging purposes in the Allegiance game.

KEY PARTS:
- CAGCDebugHook: A COM class implementing the IAGCDebugHook interface.
  - DebugOutput: Outputs debug messages to the application.
  - OnAssert: Handles assertion failures, passing details to the application.
  - OnAssertBreak: Initiates a break in execution on an assertion failure.

CONNECTS TO: g_app (the main application object)

NOTES: None.

## `src/AGC/AGCEventData.cpp`

PURPOSE: The file `AGCEventData.cpp` contains the implementation of the `CAGCEventData` class, which is responsible for managing event data in Allegiance. It handles serialization and deserialization of event data, including context strings, subject names, and variable properties.

KEY PARTS:
- `GetVarData`: Retrieves event data from a buffer and populates BSTRs and a property map.
- `ComputeVariableDataSize`: Calculates the size required to store event data in a buffer.
- `CopyVariableData`: Copies event data into a buffer.
- `CreateVARIANTFromData`, `CreateBSTRFromData`, `CreateBSTRFromData_LPSTR`, `CreateBSTRFromData_LPWSTR`, `CreateObjectFromData`: Functions for creating VARIANTs and BSTRs from data buffers.

CONNECTS TO: `AGCEventData.h` (header file), `TCAtl\SimpleStream.h`, `TCAtl\TCNullStreamImpl.h`.

NOTES:
- Contains macros for handling different variant types.
- Includes TODO comments for supporting VT_VARIANT.

## `src/AGC/AGCEventData.h`

PURPOSE: This file defines a class `CAGCEventData` used for managing event data in the Allegiance game. It encapsulates fixed and variable data related to events.

KEY PARTS:
- CAGCEventData: Manages event data, including event ID, time, subject ID, argument count, and variable data.
- XProperties: A type alias for `CAGCEvent::XProperties`.
- ComputeVariableDataSize: Calculates the size needed for variable data based on context, subject, and arguments.
- CopyVariableData: Copies variable data into the allocated memory.
- CreateVARIANTFromData, CreateBSTRFromData, CreateObjectFromData: Static methods to create different types of data from byte arrays.

CONNECTS TO: AGC.h, AGCEvent.h

NOTES: None.

## `src/AGC/AGCEventDef.cpp`

PURPOSE: The file `AGCEventDef.cpp` implements the event definition system for Allegiance, managing event IDs, names, descriptions, and parameters.

KEY PARTS:
- CAGCEventDef: Main class that manages event definitions.
- Initialize: Initializes the event name map.
- Terminate: Cleans up resources used by the event name map.
- GetEventName: Retrieves the name of an event given its ID.
- GetEventDescription: Retrieves the description of an event given its ID or event object.
- GetString: Loads a string from resources.
- ExpandFmtString: Expands format strings with event parameters.
- ExpandParams: Extracts parameters from format strings.

CONNECTS TO: AGCEventsRCH.h, AGCEventsCPP.h (XML/XSL-generated files), TCLib\ObjectLock.h

NOTES: None.

## `src/AGC/AGCEventDef.h`

PURPOSE: Defines and manages event definitions in Allegiance, including parsing, retrieval, and formatting of event data.

KEY PARTS:
- CAGCEventDef: Main class that handles the definition and management of events.
  - XEventDef: Structure defining an individual event with various properties like ID, name, description, etc.
  - XParseData: Struct used during event parsing to store intermediate data.
  - ParseState_*: Functions implementing different states for the event parsing process.

CONNECTS TO: IAGCEvent interface and related classes for handling events in Allegiance.

NOTES: None.

## `src/AGC/AGCEventIDRange.cpp`

PURPOSE: tiny file (8 lines), shown in full rather than summarized.

    /////////////////////////////////////////////////////////////////////////////
    // AGCEventIDRange.cpp : Implementation of the CAGCEventIDRange class.
    //
    #include "pch.h"
    #include "AGCEventIDRange.h"
    /////////////////////////////////////////////////////////////////////////////
    // CEventIDRange
    TC_OBJECT_EXTERN_IMPL(CAGCEventIDRange)

## `src/AGC/AGCEventIDRange.h`

PURPOSE:
This file defines a COM object class `CAGCEventIDRange` that implements the `IAGCEventIDRange` interface for managing ranges of event IDs in the Allegiance game. It provides functionality to convert between event IDs and their corresponding names, as well as formatting and parsing these values.

KEY PARTS:
- CAGCEventIDRange: Main class implementing `IAGCEventIDRange`, `IObjectSafety`, and other interfaces.
- GetVariantValue: Converts a VARIANT to an AGCEventID.
- GetPrefixString, GetSuffixString, GetValueDelimiter: Define string formatting for event IDs.
- FormatValueString: Formats an AGCEventID as a string.
- ReadValueString: Parses a string into an AGCEventID.

CONNECTS TO:
AGCEventDef.h - Provides definitions and utilities for event IDs.

NOTES:
None.

## `src/AGC/AGCEventIDRange.rgs`

PURPOSE: This file is an RGS (Registry Script) file used to register a COM class for the AGC Event ID Range functionality in Allegiance.

KEY PARTS:
- Name: AGC.EventIDRange.1 and AGC.EventIDRange classes
  - What it does: These classes represent the event ID range functionality within the game, likely handling events related to space combat or strategy.
  
- CLSID: {1537A87B-910E-4258-9C77-79554AA3C839}
  - What it does: This is the unique identifier for the COM class, used to reference and instantiate the event ID range functionality.

- ProgID: AGC.EventIDRange.1
  - What it does: This provides a programmatic identifier that can be used in code to refer to the COM object.

- VersionIndependentProgID: AGC.EventIDRange
  - What it does: Allows for version-independent access to the COM object, enabling compatibility with different versions of the software.

- InprocServer32: %MODULE%
  - What it does: Specifies the DLL or executable file that contains the implementation of the COM class.

- ThreadingModel: Both
  - What it does: Indicates that the COM object can be used in both single-threaded and multi-threaded environments.

- TypeLib: {AB33E136-0A3E-11D3-8B58-00C04F681633}
  - What it does: References the type library that contains the interface definitions for the COM class.

CONNECTS TO:
- Allegiance game engine, specifically related to space combat and strategy functionality.

NOTES:
- None.

## `src/AGC/AGCEventIDRanges.cpp`

PURPOSE: tiny file (8 lines), shown in full rather than summarized.

    /////////////////////////////////////////////////////////////////////////////
    // AGCEventIDRanges.cpp : Implementation of the CAGCEventIDRanges class.
    //
    #include "pch.h"
    #include "AGCEventIDRanges.h"
    /////////////////////////////////////////////////////////////////////////////
    // CEventIDRanges
    TC_OBJECT_EXTERN_IMPL(CAGCEventIDRanges)

## `src/AGC/AGCEventIDRanges.h`

PURPOSE: Defines a COM object class `CAGCEventIDRanges` that implements the `IAGCEventIDRanges` interface for managing ranges of event IDs in the Allegiance game.

KEY PARTS:
- CAGCEventIDRanges: Main class implementing IAGCEventIDRanges, IAGCRangesImpl, and IObjectSafety interfaces.
- CreateRange: Static method to create a range of AGCEventIDs using AGCGlobal's MakeAGCEventIDRange function.

CONNECTS TO: AGCGlobal for creating event ID ranges.

NOTES: None.

## `src/AGC/AGCEventIDRanges.rgs`

PURPOSE: This file defines a registry entry for a COM class that manages event ID ranges in the Allegiance game.

KEY PARTS:
- AGC.EventIDRanges.1: Class identifier for the specific version of the event ID range set.
- CLSID: Universally unique identifier for the class, {C63EFD2A-8763-44f4-A1CF-BEFB992482DA}.
- ProgID: Programmatic identifier for the class, 'AGC.EventIDRanges.1'.
- VersionIndependentProgID: Identifier that remains constant across versions, 'AGC.EventIDRanges'.
- InprocServer32: Specifies the DLL file containing the implementation of the class.
- ThreadingModel: Defines the threading model as 'Both', allowing both single-threaded and multi-threaded access.

CONNECTS TO: Allegiance game engine, specifically for managing event IDs.

NOTES: None.

## `src/AGC/AGCEventLogger.cpp`

PURPOSE: The file `AGCEventLogger.cpp` is responsible for managing and logging events within the Allegiance game, handling both NT Event Log and database logging.

KEY PARTS:
- CAGCEventLogger: Main class for event logging.
- LogEvent: Method to log an event.
- DisplayOLEDBErrorRecords: Function to display OLE DB error records.
- OnMessage and ReleaseWorkItem: Message handling and work item release methods.
- PRIVATE_ASSERTE: Macro for assertions in debug builds.
- XLock: Class for locking critical sections.
- IsWinNT: Function to check if the operating system is Windows NT.
- ReportEventW: Windows API function to report events to the NT Event Log.
- CloseDatabase and ChangeDatabase: Manage database connections.
- CloseNTEventLog and ChangeNTEventLog: Manage NT event log handles.
- RootKeyFromString, ReadStringValueFromRegistry, OpenWriteableRegKey: Functions for registry operations.
- OnEventTriggered and OnEventTriggeredSynchronous: Handle asynchronous and synchronous event triggering.
- get_EventList: Retrieves XML list of events.
- put_NTEventLog and get_NTEventLog: Manage NT Event Log computer name.
- put_DBEventLog and get_DBEventLog: Manage database event log parameters.
- Initialize and Terminate: Methods for initializing and cleaning up the event logger.

CONNECTS TO:
- Windows API functions like ReportEventW
- Registry operations through various helper methods

NOTES:
- "mdvalley: SetStringValue" indicates a potential code smell or placeholder.

PARTS (in file order):

- lines 1-337
  - CAGCEventLogger: Main class for logging events.
  - LogEvent: Method to log an event.
  - DisplayOLEDBErrorRecords: Function to display OLE DB error records.
  - OnMessage and ReleaseWorkItem: Message handling and work item release methods.
  - PRIVATE_ASSERTE: Macro for assertions in debug builds.
  - XLock: Class for locking critical sections.
  - IsWinNT: Function to check if the operating system is Windows NT.
  - ReportEventW: Windows API function to report events to the NT Event Log.
- lines 338-690
  - CAGCEventLogger: Manages logging events to various destinations like databases and event logs.
  - CloseDatabase: Closes connections to the database.
  - ChangeDatabase: Changes the database connection based on provided parameters.
  - CloseNTEventLog: Closes the NT event log handle.
  - ChangeNTEventLog: Opens the NT event log on a specified computer.
  - RootKeyFromString: Converts registry key strings to HKEY values.
  - ReadStringValueFromRegistry: Reads a string value from the Windows registry.
  - OpenWriteableRegKey: Opens or creates a writable registry key.
- lines 691-1057
  - CAGCEventLogger: Main class handling event logging.
  - OnEventTriggered: Handles asynchronous event triggering.
  - OnEventTriggeredSynchronous: Handles synchronous event triggering.
  - get_EventList: Retrieves XML list of events.
  - put_NTEventLog: Sets the NT Event Log computer name.
  - get_NTEventLog: Gets the NT Event Log computer name.
  - put_DBEventLog: Sets database event log parameters.
  - get_DBEventLog: Gets database event log parameters.
- lines 1058-1389
  - CAGCEventLogger: Manages event logging for both NT and DB.
  - Initialize: Initializes the event logger with registry settings.
  - Terminate: Cleans up resources when shutting down.
  - put_LoggingToNTEnabled/get_LoggingToNTEnabled: Enables/gets NT logging.
  - put_LoggingToDBEnabled/get_LoggingToDBEnabled: Enables/gets DB logging.
  - put_HookForNTLogging/get_HookForNTLogging: Sets/gets hooks for NT logging.
  - put_HookForDBLogging/get_HookForDBLogging: Sets/gets hooks for DB logging.
  - TODOs: "mdvalley: SetStringValue" indicates a potential code smell or placeholder.

## `src/AGC/AGCEventLogger.h`

PURPOSE: The file defines the CAGCEventLogger class, which is responsible for logging events in Allegiance. It handles both NT Event Log and database logging.

KEY PARTS:
- Name: CAGCEventLogger - Manages event logging to NT Event Log and databases.
- Name: LogEvent - Logs an event based on the provided IAGCEvent object.
- Name: CloseDatabase - Closes the current database connection.
- Name: ChangeDatabase - Changes the database connection using the provided IDBInitialize object.
- Name: CloseNTEventLog - Closes the NT Event Log handle.
- Name: ChangeNTEventLog - Changes the NT Event Log settings for a specified computer.
- Name: XEvent - Struct representing an event with various attributes like event ID, computer name, and subject details.
- Name: m_ds, m_session, m_table - Database connection objects.

CONNECTS TO: Allegiance game engine components, database systems, and NT Event Log services.

NOTES: None.

## `src/AGC/AGCEventLogger.rgs`

PURPOSE: This file is a Windows registry script that defines the class information for the Active Game Core Event Logger Class. It specifies the CLSID, ProgID, and other properties necessary for COM registration.

KEY PARTS:
- Name: AGC.EventLogger.1
  - Description: Represents the versioned ProgID for the event logger.
- Name: AGC.EventLogger
  - Description: Represents the non-versioned ProgID for the event logger.
- CLSID: {9EBFAA09-2793-4a50-8F57-1D7B26A307F3}
  - Description: Unique identifier for the event logger class.
- TypeLib: {AB33E136-0A3E-11D3-8B58-00C04F681633}
  - Description: Reference to the type library that contains information about the event logger's interface.
- InprocServer32: %MODULE%
  - Description: Path to the DLL or executable file containing the implementation of the event logger class.

CONNECTS TO:
- Allegiance game engine, specifically the Active Game Core component.

NOTES:
- None.

## `src/AGC/AGCEventThread.h`

PURPOSE:
The file defines a class `CAGCEventThread` that manages event processing in the Allegiance game. It handles queuing events and notifying registered listeners asynchronously.

KEY PARTS:
- CAGCEventThread: Manages event processing, including queueing events and notifying listeners.
- QueueEvent: Asynchronously queues an event for processing.
- QueueEventSynchronous: Queues an event and waits for it to be processed synchronously.
- OnGetUnknown: Returns the global unknown interface.
- OnGetWorkItemRelProc: Provides a release procedure for work items.
- OnMessage: Handles messages, specifically the `e_TriggerEvent` message to process events.
- ArgumentReleaseProc: Releases resources associated with event processing.

CONNECTS TO:
AGCGlobal, AGCEventData, TCWorkerThread

NOTES:
None.

## `src/AGC/AGCEvents.xml`

**PURPOSE:** The `AGCEvents.xml` file defines and categorizes events for the Allegiance game, including their IDs, names, descriptions, formats, severities, and logging options. It also includes a main class `AGCEvents` that holds all event definitions.

**KEY PARTS:**
- **AGCEvents:** Main class holding all event definitions.
- **EventGroup:** Represents a group of related events with properties like name, display name, description, lower bound, and upper bound.
- **Event:** Defines an individual event with properties such as ID, name, display name, description, format, severity, and logging options (LogAsNTEvent, LogAsDBEvent).
- **TODOs:** None explicitly mentioned in this part of the XML file.
- **Unusual Code:** Placeholders like `%.ComputerName%`, `%.SubjectName%`, etc., within event formats suggest that these will be replaced at runtime with actual values.

**CONNECTS TO:** Other systems or files it depends on or serves, one line. None explicitly mentioned in this snippet.

**NOTES:** The use of placeholders like `%.SubjectName%`, `%.SubjectID%`, `%Target%`, etc., in the format strings for dynamic content insertion.

## `src/AGC/AGCEventsCPP.xsl`

PURPOSE:
This XSLT stylesheet is used to generate C++ source code for event definitions in the Allegiance game. It processes XML input describing events and groups of events, transforming it into a formatted C++ array of event definitions.

KEY PARTS:
- `AGCEventsCPP.xsl`: The main XSLT stylesheet file.
- `GetPrefix()`, `GetLevel()`, `GetIndent()`, `GetIndentedCommentBar()`: JavaScript functions used to format the output with proper indentation and comments.
- `EVENTDEF_ENTRY_EX`, `EVENTDEF_ENTRY`, `EVENTDEF_BEGIN_GROUP`, `EVENTDEF_END_GROUP`: Macros defined in C++ style for formatting event definitions.
- `s_EventDefs[]`: The array of event definitions generated from the input XML.

CONNECTS TO:
AGCEventDef.cpp: This file includes the generated C++ code.

NOTES:
- The output is always generated and should not be edited manually.
- It uses macros to define event entries, which are then expanded into a structured C++ array.

## `src/AGC/AGCEventsIDL.xsl`

PURPOSE: This XML stylesheet is used to transform an XML file containing event definitions into a C++ header file that defines constants for these events. The resulting file includes comments and UUIDs for each event group and individual event.

KEY PARTS:
- Name: AGCEventsIDL.xsl
- Transforms XML to C++ header file
- Defines AGCEventID enum with event IDs
- Handles nested EventGroup structures
- Generates comments and UUIDs

CONNECTS TO: AGCIDL.idl (included from)

NOTES: None

## `src/AGC/AGCEventsMC.xsl`

PURPOSE: This XSLT stylesheet is used to generate a message catalog file for the Allegiance game. It processes XML input containing event definitions and converts them into a format suitable for use with the Windows Message Compiler (MC.exe).

KEY PARTS:
- `GetPrefix(node)`: Retrieves the prefix from an XML node or its parent.
- `GetMCFormat(node)`: Converts a format string into a message catalog format, replacing symbols and special characters.
- `GetFormatSymbols(strFormat)`: Extracts and converts symbol names from a format string into regular expressions.

CONNECTS TO: This file is used in conjunction with the Allegiance game's XML event definitions and the Windows Message Compiler (MC.exe).

NOTES: None.

## `src/AGC/AGCEventsRC2.xsl`

PURPOSE:
This XSLT stylesheet is used to generate a resource file for the Allegiance game that contains string IDs and descriptions for various events. The generated file is included in the game's resource compilation process.

KEY PARTS:
- Name: AGCEventsRC2.xsl
- Purpose: Transforms XML input into a resource script for event strings.
- Functions: 
  - GetPrefix(node): Retrieves the prefix from an XML node or its parent.
- Templates:
  - /: Applies templates to all nodes in the input document.
  - AGCEvents: Generates a string table with event descriptions and formats.
  - EventGroup: Processes each event group, generating unique IDs for events and their formats.

CONNECTS TO:
AGC.rc2, resource.h

NOTES:
- The file is marked as "ALWAYS GENERATED" and should not be edited manually.
- It includes the current time of generation and input files used.

## `src/AGC/AGCEventsRCH.xsl`

PURPOSE: This XML stylesheet is used to generate C++ resource IDs for events in the Allegiance game. It processes an input XML file containing event definitions and outputs a header file with predefined constants for these events.

KEY PARTS:
- `AGCEventsRCH.xsl`: The main XSLT stylesheet.
- `GetPrefix(node)`: Retrieves the prefix from an XML node or its parent.
- `SaveDescriptionBase(node)`: Saves the base resource ID for descriptions and returns it as a string.
- `SaveFormatBase(node)`: Saves the base resource ID for formats and returns it as a string.
- `GetDescriptionID(node, strAttr)`: Calculates the resource ID for a description based on an attribute value.
- `GetFormatID(node)`: Calculates the resource ID for a format based on an attribute value.

CONNECTS TO: AGCEventsXML.js (JavaScript file used within the XSLT script).

NOTES: None.

## `src/AGC/AGCGame.cpp`

PURPOSE: This file contains the implementation of the CAGCGame class, which is responsible for managing the game state and operations in Allegiance.

KEY PARTS:
- CAGCGame: Manages the overall game logic and state.
- InterfaceSupportsErrorInfo: Checks if the interface supports error information.

CONNECTS TO: src/AGC/AGCGame.h

NOTES: None.

## `src/AGC/AGCGame.h`

PURPOSE:
This file defines the interface and implementation for the main game object in Allegiance, providing the core functionality of the game.

KEY PARTS:
- CAGCGame: Main class implementing the IAGCGameImpl interface, which is a COM server for the AGC game.
- DECLARE_REGISTRY_RESOURCEID: Registers the component with the system using a resource ID.
- DECLARE_PROTECT_FINAL_CONSTRUCT: Ensures that the constructor can only be called by derived classes or itself.
- BEGIN_COM_MAP and END_COM_MAP: Maps COM interfaces to their implementation methods.
- BEGIN_CATEGORY_MAP and END_CATEGORY_MAP: Specifies categories for the component, in this case, CATID_AGC.
- InterfaceSupportsErrorInfo: Implements error information support.

CONNECTS TO:
IAGCGameImpl.h, ImissionIGC.h, LIBID_AGCLib

NOTES:
None.

## `src/AGC/AGCGame.rgs`

PURPOSE: This file is a Windows Registry Script that defines the class information for the AGCGame application, including its CLSID and ProgID.

KEY PARTS:
- Name: AGC.AGCGame.1
  - Description: Represents the versioned programmatic identifier for the AGCGame application.
- Name: AGC.AGCGame
  - Description: Represents the non-versioned programmatic identifier for the AGCGame application.
- CLSID: {4F860D2E-0A42-11D3-8B58-00C04F681633}
  - Description: Unique identifier for the AGCGame class.
- ProgID: AGC.AGCGame.1
  - Description: Programmatic Identifier that maps to a specific version of the application.
- VersionIndependentProgID: AGC.AGCGame
  - Description: Programmatic Identifier that maps to any version of the application.

CONNECTS TO:
- TypeLib: {AB33E136-0A3E-11D3-8B58-00C04F681633}
  - Description: The type library associated with the AGCGame application, which likely contains additional interface definitions.

NOTES:
- None.

## `src/AGC/AGCGameParameters.cpp`

PURPOSE: The file `AGCGameParameters.cpp` contains the implementation of a class `CAGCGameParameters` which manages various game parameters for Allegiance, including mission-specific and general settings.

KEY PARTS:
- CAGCGameParameters: Manages game parameters.
- GetClassID: Returns the class ID for IPersist interface support.
- IsDirty: Always returns S_OK indicating the object is always dirty.
- Load: Loads game parameters from a stream.
- Save: Saves game parameters to a stream.
- put_AllowPrivateTeams, get_PowerUps, etc.: Set and get methods for various game settings like private teams, power-ups, joiners, stations, resources, and more.

CONNECTS TO: None

NOTES: The class implements the IPersist interface for persistence operations.

PARTS (in file order):

- lines 2-369
  - CAGCGameParameters: Class representing game parameters, either attached to an existing mission or unattached for creating a new one.
  - GetClassID: Method implementing IPersist interface to return the class ID of the object.
  - IsDirty: Always returns S_OK indicating the object is always dirty.
  - Load: Loads game parameters from a stream, validating version and structure size.
  - Save: Saves game parameters to a stream, including version information and current time adjustments.
  - GetSizeMax: Returns the maximum size of the persisted data.
  - InitNew: Initializes a new game parameters object.
  - InterfaceSupportsErrorInfo: Checks if the interface supports error information.
- lines 370-716
  - CAGCGameParameters: Class representing game parameters.
  - put_AllowPrivateTeams: Method to set whether private teams are allowed.
  - get_PowerUps: Method to get whether power-ups are enabled.
  - put_PowerUps: Method to set whether power-ups are enabled.
  - get_AllowJoiners: Method to get whether players can join a mission.
  - put_AllowJoiners: Method to set whether players can join a mission.
  - get_Stations: Method to get whether stations are available.
  - put_Stations: Method to set whether stations are available.
- lines 717-1060
  - CAGCGameParameters: Manages game parameters.
  - put_Resources: Sets whether resources are enabled.
  - get_ResourceAmountsVisible: Gets visibility of resource amounts.
  - put_ResourceAmountsVisible: Sets visibility of resource amounts.
  - get_RandomWormholes: Gets random wormhole generation setting.
  - put_RandomWormholes: Sets random wormhole generation setting.
  - get_NoTeams: Gets no teams mode setting.
  - put_NoTeams: Sets no teams mode setting.
- lines 1063-1407
  - CAGCGameParameters: Manages game parameters for a mission.
  - put_StartingMoney: Sets the starting money for players if not attached to an existing mission.
  - get_Lives: Retrieves the number of lives for players.
  - put_Lives: Sets the number of lives for players if not attached to an existing mission.
  - get_GoalTeamKills: Retrieves the goal team kills for a mission.
  - put_GoalTeamKills: Sets the goal team kills for a mission if not attached to an existing mission.
  - get_MapType: Retrieves the map type for a mission.
  - put_MapType: Sets the map type for a mission if not attached to an existing mission.
- lines 1408-1753
  - CAGCGameParameters: Class representing game parameters.
  - put_MaxRank: Method to set the maximum rank property.
  - get_PlayerSectorAsteroids: Method to get the number of asteroids in player's sector.
  - put_PlayerSectorAsteroids: Method to set the number of asteroids in player's sector.
  - get_NeutralSectorAsteroids: Method to get the number of asteroids in neutral sector.
  - put_NeutralSectorAsteroids: Method to set the number of asteroids in neutral sector.
  - get_PlayerSectorTreasures: Method to get the number of treasures in player's sector.
  - put_PlayerSectorTreasures: Method to set the number of treasures in player's sector.
- lines 1754-2111
  - CAGCGameParameters: Manages game parameters.
  - put_IGCStaticFile: Sets the IGC static file path.
  - get_GamePassword: Retrieves the game password.
  - put_GamePassword: Sets the game password.
  - get_InvulnerableStations: Checks if stations are invulnerable.
  - put_InvulnerableStations: Enables or disables invulnerable stations.
  - get_ShowMap: Determines if the map is shown.
  - put_ShowMap: Enables or disables showing the map.
- lines 2112-2475
  - CAGCGameParameters: Manages game parameters.
  - get_TimeStart: Retrieves the start time of the game.
  - get_TimeStartClock: Retrieves the clock-based start time of the game.
  - get_GoalArtifactsCount: Retrieves the count of goal artifacts in the game.
  - put_GoalArtifactsCount: Sets the count of goal artifacts in the game.
  - get_AutoRestart: Retrieves whether auto-restart is enabled.
  - put_AutoRestart: Enables or disables auto-restart.
  - get_DefaultCountdown: Retrieves the default countdown time.
- lines 2476-2895
  - CAGCGameParameters: Manages game parameters and settings.
  - get_IsTechBitOverridden: Checks if a technology bit is overridden for a team.
  - put_OverriddenTechBit: Sets or clears an overridden technology bit for a team.
  - put_SetOverriddenTechBitRange: Sets or clears a range of overridden technology bits for a team.
  - get_GoalFlagsCount: Retrieves the count of goal flags.
  - put_GoalFlagsCount: Sets the count of goal flags.
  - get_StoryText: Gets the story text for the game.
  - put_StoryText: Sets the story text for the game.

## `src/AGC/AGCGameParameters.h`

PURPOSE: This file defines a COM object class `CAGCGameParameters` that implements the `IAGCGameParameters` interface to manage game parameters for the Allegiance game.

KEY PARTS:
- CAGCGameParameters: Main class implementing game parameter management.
- Init: Initializes the game parameters with mission data.
- GetIGCVoid: Returns a pointer to the internal game parameter data.
- IPersistStreamInit methods: Implement persistence and serialization of game parameters.
- IAGCGameParameters methods: Provide accessors and mutators for various game parameters such as player counts, sector types, resource settings, etc.

CONNECTS TO: AGC.h, GameParamData.h, resource.h

NOTES: None.

## `src/AGC/AGCGameParameters.rgs`

PURPOSE: This file is a Windows Registry Script (.rgs) that defines the registration information for the AGCGameParameters class in Allegiance. It specifies the CLSID, ProgID, and other properties necessary for COM object registration.

KEY PARTS:
- Name: AGC.GameParameters.1 - Class name with version.
- CLSID: {F9E52A78-EBA4-11D2-8B4B-00C04F681633} - Unique identifier for the class.
- ProgID: AGC.AGCGameParameters.1 - Programmatic identifier for the class.
- VersionIndependentProgID: AGC.AGCGameParameters - Base programmatic identifier without version.
- InprocServer32: %MODULE% - Path to the DLL containing the implementation.
- ThreadingModel: Both - Specifies that the object can be used in both single-threaded and multi-threaded apartments.

CONNECTS TO: Allegiance game engine, specifically for managing game parameters through COM interface.

NOTES: None.

## `src/AGC/AGCGlobal.cpp`

**PURPOSE:** The `AGCGlobal.cpp` file in Allegiance manages global state and event handling. It includes functions for registering and retrieving AGC objects, managing event sinks, and triggering events.

**KEY PARTS:**
- **Class: CAGCGlobal**: Manages global state and event handling.
- **Function: FinalRelease()**: Ensures the object is terminated before releasing resources.
- **Function: InterpretIID()**: Determines the corresponding IID for a given IGC pointer based on its type.
- **Function: RegisterObjectCreator()**: Registers a creator function for creating AGC objects from IGC pointers.
- **Function: GetAGCObject()**: Retrieves the corresponding AGC wrapper for an IGC pointer, creating it if necessary.
- **Function: TriggerEvent()**: Asynchronously triggers an event by queuing it to the event thread.
- **Function: TriggerEventSynchronous()**: Synchronously triggers an event by directly calling the event sinks.
- **Function: RegisterEvent()**: Registers an event sink for a specific event and unique ID.
- **Function: RevokeEvent()**: Revokes an event sink from a specific event and unique ID.

**CONNECTS TO:** Other parts of the Allegiance game engine, particularly those handling objects and events.

**NOTES:** None.

## `src/AGC/AGCGlobal.h`

PURPOSE: The file defines and implements the CAGCGlobal class, which is a singleton interface for managing global events, object creation, and resource management in the Allegiance game.

KEY PARTS:
- CAGCGlobal: Manages global events, object creation, and resource mapping.
- RegisterObjectCreator: Registers a creator function for a given CLSID.
- RevokeObjectCreator: Removes a previously registered creator function.
- GetAGCObject: Maps an IGC (Game Object) to its corresponding AGC (Allegiance Game Component).
- AddAGCObject: Adds an AGC object to the mapping.
- RemoveAGCObject: Removes an AGC object from the mapping.
- RegisterEvent: Registers an event sink for a specific event and unique ID.
- RevokeEvent: Removes an event sink from a specific event and unique ID.
- TriggerEvent: Triggers an event asynchronously.
- TriggerEventSynchronous: Triggers an event synchronously.
- FreeListeners: Frees the resources associated with event listeners.

CONNECTS TO: AGC.h, resource.h, TCLib\ObjectLock.h, TCLib\RangeSet.h, IAGCGlobal interface, CComObjectRootEx, CComCoClass, and other game components.

NOTES: None.

## `src/AGC/AGCGlobal.rgs`

PURPOSE: This file is a Windows registry script for the Allegiance game's AGCGlobal class, which likely manages global settings or data for the game.

KEY PARTS:
- Name: AGC.AGCGlobal.1 - Registers the AGCGlobal class with a specific CLSID.
- CLSID: {08118086-1FE1-11d3-AF03-0000F810C3F4} - The unique identifier for the AGCGlobal class.
- ProgID: AGC.AGCGlobal.1 - Allows the class to be referenced by a programmatic ID.
- VersionIndependentProgID: AGC.AGCGlobal - Provides a version-independent reference to the class.
- InprocServer32: %MODULE% - Specifies the DLL that implements the class.
- ThreadingModel: Both - Indicates that the class can handle both single-threaded and multi-threaded environments.

CONNECTS TO: The Allegiance game's module file (specified by %MODULE%).

NOTES: None.

## `src/AGC/AGCGuid.c`

PURPOSE: tiny file (2 lines), shown in full rather than summarized.

    #include "AGCIDL_i.c"
    #include "AGC_i.c"

## `src/AGC/AGCHullType.cpp`

PURPOSE: This file contains the implementation of the CAGCHullType class, which represents the properties and behaviors of different ship hull types in the Allegiance game.

KEY PARTS:
- CAGCHullType: Implements the IAGCHullType interface to provide access to various attributes of a ship's hull type.
- InterfaceSupportsErrorInfo: Checks if the interface supports error information.
- get_Length, get_MaxSpeed, etc.: Retrieve specific properties of the hull type such as length, maximum speed, turn rates, thrust, and more.

CONNECTS TO: IAGCHullType interface

NOTES: None

## `src/AGC/AGCHullType.h`

PURPOSE: This file defines the interface and implementation for a hull type in the Allegiance game, which includes properties like length, speed, turn rates, thrust, and various capabilities.

KEY PARTS:
- CAGCHullType: Main class implementing IhullTypeIGC, IAGCHullType interfaces, and ISupportErrorInfo.
- DECLARE_REGISTRY_RESOURCEID: Registers the component with a resource ID.
- BEGIN_COM_MAP: Maps COM interfaces to methods in the class.
- BEGIN_CATEGORY_MAP: Specifies categories for the component.
- InterfaceSupportsErrorInfo: Implements error support.
- get_Length, get_MaxSpeed, etc.: Methods returning various properties of the hull type.

CONNECTS TO: IAGCBuyableImpl.h (for buyable interface implementation).

NOTES: None.

## `src/AGC/AGCHullType.rgs`

PURPOSE: This file is a Windows registry script that defines the class information for the AGCHullType COM object used in Allegiance.

KEY PARTS:
- Name: AGC.AGCHullType.1
  - Description: Represents version 1 of the AGCHullType class.
- CLSID: {6E5B27C8-2ED9-11d3-A51C-00C04F68DEB0}
  - Unique identifier for the COM object.
- CurVer: AGC.AGCHullType.1
  - Specifies the current version of the class.
- ProgID: AGC.AGCHullType.1
  - Programmatic identifier used to reference the class in code.
- VersionIndependentProgID: AGC.AGCHullType
  - Identifier for referencing the class without version information.

CONNECTS TO:
- TypeLib: {AB33E136-0A3E-11D3-8B58-00C04F681633}
  - Links to the type library that contains additional information about the COM object.

NOTES:
- None.

## `src/AGC/AGCHullTypes.rgs`

PURPOSE: This file is a Windows Registry Script (RGS) that defines the class information for the AGCHullTypes COM object used in Allegiance.

KEY PARTS:
- Name: AGCHullTypes Class
- CLSID: {E428B8B2-2ED9-11d3-A51C-00C04F68DEB0}
- ProgID: AGC.AGCHullTypes.1
- VersionIndependentProgID: AGC.AGCHullTypes
- TypeLib: {AB33E136-0A3E-11D3-8B58-00C04F681633}

CONNECTS TO: Allegiance game engine, specifically for managing ship hull types.

NOTES: None.

## `src/AGC/AGCModel.cpp`

PURPOSE: This file contains the implementation of the CAGCModel class, which is part of the Allegiance game's model system.

KEY PARTS:
- CAGCModel: The main class representing the game model.
- InterfaceSupportsErrorInfo: Method to check if the interface supports error information.

CONNECTS TO: AGCModel.h

NOTES: None.

## `src/AGC/AGCModel.h`

PURPOSE: This file defines the CAGCModel class, which is a COM object implementing the IAGCModel interface for the Allegiance game.

KEY PARTS:
- Name: CAGCModel - Implements the IAGCModel interface and provides support for error information.
- Interface Map: Maps the ISupportErrorInfo interface and other interfaces to their respective methods.
- Category Map: Registers the object under the CATID_AGC category.
- DECLARE_REGISTRY_RESOURCEID: Specifies the registry resource ID for the class.

CONNECTS TO: IAGCModelImpl.h, ImodelIGC.h, LIBID_AGCLib

NOTES: None.

## `src/AGC/AGCModel.rgs`

PURPOSE: This file is a Windows Registry Script that defines the class information for an ActiveX control named AGCModel. It specifies the CLSID, ProgID, and TypeLib associated with the control.

KEY PARTS:
- Name: AGC.AGCModel.1 - Represents the version 1 of the AGCModel class.
- CLSID: {DF2F4B4D-0A42-11D3-8B58-00C04F681633} - Unique identifier for the ActiveX control.
- ProgID: AGC.AGCModel.1 - Programmatic Identifier used to reference the control in code.
- TypeLib: {AB33E136-0A3E-11D3-8B58-00C04F681633} - Type Library associated with the control.

CONNECTS TO: Windows Registry - This script modifies the registry to register the AGCModel ActiveX control.

NOTES: None.

## `src/AGC/AGCModels.cpp`

PURPOSE: tiny file (8 lines), shown in full rather than summarized.

    /////////////////////////////////////////////////////////////////////////////
    // AdminAGCModels.cpp : Implementation of CAGCModels
    //
    #include "pch.h"
    #include "AGCModels.h"
    /////////////////////////////////////////////////////////////////////////////
    // CAGCModels
    TC_OBJECT_EXTERN_NON_CREATEABLE_IMPL(CAGCModels)

## `src/AGC/AGCModels.h`

PURPOSE: This file defines the interface and implementation for a collection of models in the Allegiance game.

KEY PARTS:
- CAGCModels: A COM class implementing IAGCModels, which is a collection of model interfaces.
- DECLARE_REGISTRY_RESOURCEID: Registers the resource ID for the class.
- DECLARE_PROTECT_FINAL_CONSTRUCT: Ensures that the constructor cannot be called from derived classes.
- BEGIN_COM_MAP and END_COM_MAP: Maps the interface to the implementation.
- BEGIN_CATEGORY_MAP and END_CATEGORY_MAP: Specifies the category of the component.

CONNECTS TO: IAGCCollectionImpl, ImodelIGC, IAGCModel, LIBID_AGCLib

NOTES: None.

## `src/AGC/AGCModels.rgs`

PURPOSE: This file is a Windows registry script that defines the class information for an ActiveX control named AGCModels.

KEY PARTS:
- Name: AGC.AGCModels.1 - The versioned ProgID for the AGCModels ActiveX control.
- CLSID: {FF8126CC-1ED2-43c5-B07E-D7B2E8487EE5} - The unique identifier for the AGCModels class.
- CurVer: AGC.AGCModels.1 - Specifies the current version of the ProgID.
- TypeLib: {AB33E136-0A3E-11D3-8B58-00C04F681633} - The type library associated with the AGCModels class.

CONNECTS TO: None

NOTES: none

## `src/AGC/AGCModule.h`

PURPOSE: The file defines the CAGCModule class, which manages event triggering and registration in the Allegiance game.

KEY PARTS:
- CAGCModule: Manages AGCGlobal object for event handling.
- GetAGCGlobal: Retrieves the AGCGlobal interface pointer.
- TriggerEvent: Triggers an event with various parameters.
- MakeAGCEvent: Creates an AGCEvent object.
- DoTriggerEvent: Internal method to handle actual event triggering logic.

CONNECTS TO: AGCGlobal, IAGCGlobal, and other related interfaces for event management.

NOTES: None.

## `src/AGC/AGCOrientation.cpp`

PURPOSE: This file contains the implementation of the CAGCOrientation class, which represents an orientation in 3D space and provides methods for manipulating and querying that orientation.

KEY PARTS:
- CAGCOrientation: Main class representing an orientation with methods to get raw orientation data, create result vectors, and compare orientations.
- GetRawOrientation: Retrieves the raw orientation data from another IAGCOrientation object.
- CreateResultVector: Creates a new IAGCVector object initialized from a given Vector.
- InterfaceSupportsErrorInfo: Checks if the interface supports error information.
- IPersist methods (GetClassID, IsDirty, Load, Save, GetSizeMax): Implement persistence for the orientation data.
- IAGCOrientation methods (InitCopy, get_Forward, get_Backward, get_Up, get_Right, get_IsEqual, get_IsRoughlyEqual): Provide functionality to copy orientations and retrieve specific axes or compare orientations.
- IAGCOrientationPrivate methods (InitFromOrientation, CopyOrientationTo): Private methods for initializing from orientation data and copying orientation data.

CONNECTS TO: AGCVector.h, AGCOrientation.h

NOTES: None.

## `src/AGC/AGCOrientation.h`

PURPOSE: This file defines a COM object class `CAGCOrientation` that represents an orientation in space, providing methods to get and set the forward, backward, up, and right vectors, as well as checking equality with other orientations.

KEY PARTS:
- CAGCOrientation: Main class implementing interfaces for orientation management.
- GetRawOrientation: Static method to convert a raw orientation to a COM interface.
- CreateResultVector: Static method to create a result vector from a raw vector.
- InterfaceSupportsErrorInfo: Implements error support.
- InitCopy, get_Forward, get_Backward, get_Up, get_Right, get_IsEqual, get_IsRoughlyEqual: Methods for orientation operations.
- InitFromOrientation, CopyOrientationTo: Private methods for internal orientation management.
- GetClassID, IsDirty, Load, Save, GetSizeMax, InitNew: IPersist and IPersistStreamInit interface implementations.

CONNECTS TO: AGCLib library, TCLib::ObjectLock, IAGCOrientation, IAGCVector interfaces.

NOTES: None.

## `src/AGC/AGCOrientation.rgs`

PURPOSE: This file is a Windows registry script that defines the class information for the AGCOrientation COM object used in Allegiance.

KEY PARTS:
- Name: AGCOrientation Class
- CLSID: {C8204960-3999-11d3-A51D-00C04F68DEB0}
- ProgID: AGC.AGCOrientation.1
- VersionIndependentProgID: AGC.AGCOrientation
- InprocServer32: %MODULE%
- ThreadingModel: Both
- TypeLib: {AB33E136-0A3E-11D3-8B58-00C04F681633}

CONNECTS TO: Allegiance game engine, specifically for handling orientation-related functionalities.

NOTES: None.

## `src/AGC/AGCProbe.cpp`

PURPOSE: Implements the CAGCProbe class, which represents a probe in the Allegiance game.

KEY PARTS:
- Name: CAGCProbe - Manages the behavior and properties of a probe in the game.
- InterfaceSupportsErrorInfo - Checks if the interface supports error information.

CONNECTS TO: AGCProbe.h

NOTES: None.

## `src/AGC/AGCProbe.h`

PURPOSE: This file defines the CAGCProbe class, which implements the IAGCProbe interface for a probe object in Allegiance.

KEY PARTS:
- Name: CAGCProbe - Implements the IAGCProbe interface for managing probe objects.
- Interface Map: Maps COM interfaces to methods within the class.
- Category Map: Registers the category of the component as CATID_AGC.
- ISupportsErrorInfo: Provides error information support.

CONNECTS TO: IAGCProbeImpl, ISupportErrorInfo, CComCoClass

NOTES: None.

## `src/AGC/AGCProbe.rgs`

PURPOSE: This file is a Windows Registry Script (RGS) that defines the class information for the AGCProbe object in Allegiance. It specifies the CLSID and ProgID for the AGCProbe class.

KEY PARTS:
- Name: AGC.AGCProbe.1, AGC.AGCProbe
  - What it does: Defines the versioned and non-versioned ProgIDs for the AGCProbe class.
- CLSID: {A23C1044-ED2D-4f80-813B-FBB5DE7B8CFB}
  - What it does: Unique identifier for the AGCProbe class in the Windows Registry.
- TypeLib: {AB33E136-0A3E-11D3-8B58-00C04F681633}
  - What it does: Links to the type library that contains the interface definitions for the AGCProbe class.

CONNECTS TO:
- Allegiance game engine, specifically related to space combat and exploration features.

NOTES:
- None.

## `src/AGC/AGCProbes.cpp`

PURPOSE: tiny file (8 lines), shown in full rather than summarized.

    /////////////////////////////////////////////////////////////////////////////
    // AdminAGCProbes.cpp : Implementation of CAGCProbes
    //
    #include "pch.h"
    #include "AGCProbes.h"
    /////////////////////////////////////////////////////////////////////////////
    // CAGCProbes
    TC_OBJECT_EXTERN_NON_CREATEABLE_IMPL(CAGCProbes)

## `src/AGC/AGCProbes.h`

PURPOSE: This file defines a COM object class `CAGCProbes` that implements the `IAGCProbes` interface and extends functionality through the `IAGCCollectionImpl` template.

KEY PARTS:
- CAGCProbes: A COM object class implementing `IAGCProbes`.
- DECLARE_REGISTRY_RESOURCEID(IDR_AGCProbes): Registers the COM object with a resource ID.
- DECLARE_PROTECT_FINAL_CONSTRUCT(): Ensures that the constructor is protected and final.
- BEGIN_COM_MAP(CAGCProbes) ... END_COM_MAP(): Maps COM interfaces to implementation methods.
- BEGIN_CATEGORY_MAP(CAGCProbes) ... END_CATEGORY_MAP(): Specifies categories for the COM object.

CONNECTS TO: IAGCCollectionImpl, IAGCProbes, ProbeListIGC, LIBID_AGCLib

NOTES: None.

## `src/AGC/AGCProbes.rgs`

PURPOSE: This file is a Windows Registry Script (RGS) that defines the class information for the AGCProbes component in Allegiance. It registers the CLSID and ProgID necessary for COM-based communication.

KEY PARTS:
- Name: AGC.AGCProbes.1, AGC.AGCProbes
  - What it does: Identifies the versioned and non-versioned programmatic identifiers for the AGCProbes class.
- CLSID: {BB92CE8C-CC20-497a-B7CC-26F29E1D8877}
  - What it does: Unique identifier for the AGCProbes class in the COM registry.
- ProgID: AGC.AGCProbes.1
  - What it does: Programmatic Identifier that maps to the CLSID.
- VersionIndependentProgID: AGC.AGCProbes
  - What it does: Allows compatibility with older versions of the component.
- TypeLib: {AB33E136-0A3E-11D3-8B58-00C04F681633}
  - What it does: Links to the type library that contains information about the interfaces and types used by the AGCProbes class.

CONNECTS TO:
- Allegiance game engine, specifically for managing probes as a COM object.

NOTES:
- None.

## `src/AGC/AGCSector.cpp`

PURPOSE: This file contains the implementation of the CAGCSector class, which is part of the Allegiance game's sector management system.

KEY PARTS:
- CAGCSector: Manages and represents a sector in the game world.
- InterfaceSupportsErrorInfo: Checks if the interface supports error information.

CONNECTS TO: pch.h and AGCSector.h

NOTES: None.

## `src/AGC/AGCSector.h`

PURPOSE: This file defines the CAGCSector class, which is an implementation of the IAGCSector interface for managing sectors in the Allegiance game.

KEY PARTS:
- Name: CAGCSector
  - What it does: Manages sectors within the game, implementing the IAGCSector interface.
- Name: DECLARE_REGISTRY_RESOURCEID(IDR_AGCSector)
  - What it does: Registers the class with the Windows registry using a resource ID.
- Name: DECLARE_PROTECT_FINAL_CONSTRUCT()
  - What it does: Ensures that the constructor is protected and final, preventing inheritance from other classes.
- Name: BEGIN_COM_MAP(CAGCSector)
  - What it does: Maps COM interfaces to their respective methods in the CAGCSector class.
- Name: IMPLEMENTED_CATEGORY(CATID_AGC)
  - What it does: Registers the class with a specific category ID.

CONNECTS TO:
- IAGCSectorImpl.h: Provides the base implementation for the IAGCSector interface.
- ISupportErrorInfo: Interface for error support, allowing the class to handle errors gracefully.

NOTES:
- None.

## `src/AGC/AGCSector.rgs`

PURPOSE: This file is a Windows Registry Script (.rgs) that defines the class information for the AGCSector object in Allegiance. It registers the CLSID and ProgID for the AGCSector class.

KEY PARTS:
- Name: AGC.AGCSector.1, AGC.AGCSector
  - Describes the versioned and non-versioned ProgIDs for the AGCSector class.
- CLSID: {338DA8FE-EBA5-11D2-8B4B-00C04F681633}
  - Unique identifier for the AGCSector class.
- TypeLib: {AB33E136-0A3E-11D3-8B58-00C04F681633}
  - Links to the type library that contains additional information about the AGCSector class.

CONNECTS TO:
- Allegiance game engine, specifically related to space sector management and combat.

NOTES: None.

## `src/AGC/AGCSectors.cpp`

PURPOSE: tiny file (8 lines), shown in full rather than summarized.

    /////////////////////////////////////////////////////////////////////////////
    // AGCSectors.cpp : Implementation of the CAGCSectors class.
    //
    #include "pch.h"
    #include "AGCSectors.h"
    /////////////////////////////////////////////////////////////////////////////
    // CAGCSectors
    TC_OBJECT_EXTERN_NON_CREATEABLE_IMPL(CAGCSectors)

## `src/AGC/AGCSectors.h`

PURPOSE: This file defines a COM object class `CAGCSectors` that implements the `IAGCSectors` interface and extends functionality through the `IAGCCollectionImpl` template.

KEY PARTS:
- CAGCSectors: A COM object class implementing `IAGCSectors` and extending collection functionality.
- DECLARE_REGISTRY_RESOURCEID(IDR_AGCSectors): Registers the COM object with a resource ID.
- DECLARE_PROTECT_FINAL_CONSTRUCT(): Ensures that the constructor is protected and final.
- BEGIN_COM_MAP(CAGCSectors) ... END_COM_MAP(): Maps COM interfaces to class methods.
- BEGIN_CATEGORY_MAP(CAGCSectors) ... END_CATEGORY_MAP(): Categorizes the COM object under `CATID_AGC`.

CONNECTS TO: IAGCCollectionImpl.h, resource.h

NOTES: None.

## `src/AGC/AGCSectors.rgs`

PURPOSE: This file is a Windows registry script that defines the class information for the AGCSectors component in Allegiance.

KEY PARTS:
- Name: AGC.AGCSectors.1
  - Description: Represents the versioned ProgID for the AGCSectors class.
- Name: AGC.AGCSectors
  - Description: Represents the non-versioned ProgID for the AGCSectors class, pointing to the latest version.
- CLSID: {4214FD6E-EBA5-11D2-8B4B-00C04F681633}
  - Description: Unique identifier for the AGCSectors class.
- TypeLib: {AB33E136-0A3E-11D3-8B58-00C04F681633}
  - Description: Links to the type library that contains information about the AGCSectors class.

CONNECTS TO:
- Allegiance game engine, specifically related to sector management and space combat.

NOTES:
- None.

## `src/AGC/AGCShip.cpp`

PURPOSE: This file contains the implementation of the CAGCShip class, which represents a ship in the Allegiance game.

KEY PARTS:
- CAGCShip: Represents a ship in the game.
- InterfaceSupportsErrorInfo: Checks if the interface supports error information.

CONNECTS TO: AGCShip.h

NOTES: None.

## `src/AGC/AGCShip.h`

PURPOSE: This file defines the CAGCShip class, which is an implementation of the IAGCShip interface for a ship in the Allegiance game.

KEY PARTS:
- Name: CAGCShip
  - What it does: Implements the IAGCShip interface to represent a ship in the Allegiance game.
- Name: DECLARE_REGISTRY_RESOURCEID
  - What it does: Registers the class with the resource ID for the ship.
- Name: DECLARE_PROTECT_FINAL_CONSTRUCT
  - What it does: Protects against premature destruction of the object during construction.
- Name: BEGIN_COM_MAP
  - What it does: Maps COM interfaces to the implementation methods.
- Name: BEGIN_CATEGORY_MAP
  - What it does: Maps categories to the class.
- Name: STDMETHOD(InterfaceSupportsErrorInfo)
  - What it does: Implements the ISupportErrorInfo interface method.

CONNECTS TO:
- IAGCShipImpl.h: The header file for the implementation of the IAGCShip interface.

NOTES:
- None.

## `src/AGC/AGCShip.rgs`

PURPOSE: This file is an RGS (Registry Script) file used to register the AGCShip class in the Windows registry. It defines the CLSID, ProgID, and TypeLib for the AGCShip class.

KEY PARTS:
- Name: AGC.AGCShip.1
  - Registers the AGCShip class with a specific CLSID.
- Name: AGC.AGCShip
  - Provides an alias for the AGCShip class.
- Name: CLSID
  - Specifies the unique identifier for the AGCShip class.
- Name: ProgID
  - Assigns a programmatic identifier to the AGCShip class.
- Name: VersionIndependentProgID
  - Defines a version-independent ProgID for the AGCShip class.
- Name: TypeLib
  - Links the AGCShip class to its associated type library.

CONNECTS TO:
- src/AGC/AGCTypes.rgs (likely contains information about the type library)

NOTES:
- None.

## `src/AGC/AGCShips.cpp`

PURPOSE: tiny file (8 lines), shown in full rather than summarized.

    /////////////////////////////////////////////////////////////////////////////
    // AGCShips.cpp : Implementation of the CAGCShips class.
    //
    #include "pch.h"
    #include "AGCShips.h"
    /////////////////////////////////////////////////////////////////////////////
    // CAGCShips
    TC_OBJECT_EXTERN_NON_CREATEABLE_IMPL(CAGCShips)

## `src/AGC/AGCShips.h`

PURPOSE: This file declares and implements a COM object for managing collections of ships in the Allegiance game.

KEY PARTS:
- CAGCShips: A COM class implementing IAGCShips interface to manage collections of ship objects.
- DECLARE_REGISTRY_RESOURCEID, DECLARE_PROTECT_FINAL_CONSTRUCT: Macros for registering the COM object and ensuring it cannot be subclassed.
- BEGIN_COM_MAP, END_COM_MAP: Defines the interfaces that this COM object implements.
- BEGIN_CATEGORY_MAP, END_CATEGORY_MAP: Specifies the category under which this COM object falls.

CONNECTS TO: IAGCCollectionImpl.h (for collection implementation), resource.h (for registry resources).

NOTES: None.

## `src/AGC/AGCShips.rgs`

PURPOSE: This file is a Windows Registry Script (RGS) that defines the class information for the AGCShips component in Allegiance. It registers the CLSID and ProgID necessary for COM-based communication.

KEY PARTS:
- Name: AGC.AGCShips.1
  - Registers the AGCShips Class with a specific CLSID.
- CLSID: {5BEA791C-1EBF-11d3-8B63-00C04F681633}
  - Unique identifier for the AGCShips class.
- ProgID: AGC.AGCShips.1
  - Programmatic Identifier used to reference the class in code.
- VersionIndependentProgID: AGC.AGCShips
  - Allows compatibility with older versions of the component.

CONNECTS TO:
- Allegiance game engine, specifically for space combat and strategy elements.

NOTES:
- None

## `src/AGC/AGCSite.h`

PURPOSE: This file defines a template class `CAGCSite` that extends another class `T`, providing event handling for various game objects such as ships, models, and missions. It ensures that any associated AGC (Allegiance Game Client) objects are properly removed when these game objects are terminated or changed.

KEY PARTS:
- `DestroySideEvent`: Handles the destruction of a side event by removing the associated AGC object.
- `TerminateModelEvent`: Handles the termination of a model event by removing the associated AGC object.
- `TerminateMissionEvent`: Handles the termination of a mission event by removing the associated AGC object.
- `ChangeCluster`: Triggers an event when a ship changes clusters and handles the removal of the old cluster's AGC object.
- `KillShipEvent`: Triggers an event when a ship is killed, providing details about the kill including the launcher, amount, and vectors involved. It also checks if the ship is a lifepod.
- `UseRipcord`: Handles the use of a ripcord by a ship and triggers an associated event.

CONNECTS TO: `igc.h`, which contains interfaces for game objects like `IsideIGC`, `ImodelIGC`, and `ImissionIGC`.

NOTES: None.

## `src/AGC/AGCStation.cpp`

PURPOSE: This file contains the implementation of the CAGCStation class, which represents a station in the Allegiance game.

KEY PARTS:
- CAGCStation: Represents a station in the game.
- InterfaceSupportsErrorInfo: Checks if the interface supports error information.

CONNECTS TO: AGCStation.h

NOTES: None.

## `src/AGC/AGCStation.h`

PURPOSE: This file defines the CAGCStation class, which is a COM object implementing the IAGCStation interface for managing stations in the Allegiance game.

KEY PARTS:
- Name: CAGCStation
  - What it does: Manages station-related operations within the Allegiance game.
  
- Name: DECLARE_REGISTRY_RESOURCEID(IDR_AGCStation)
  - What it does: Registers the COM object with the system using a resource ID.
  
- Name: DECLARE_PROTECT_FINAL_CONSTRUCT()
  - What it does: Ensures that the constructor is protected and final, preventing multiple instantiations.

- Name: BEGIN_COM_MAP(CAGCStation)
  - What it does: Maps the COM interfaces to their implementation methods.
  
- Name: BEGIN_CATEGORY_MAP(CAGCStation)
  - What it does: Specifies the category of the COM object for categorization purposes.
  
- Name: STDMETHODIMP InterfaceSupportsErrorInfo(REFIID riid)
  - What it does: Implements error support for COM interfaces.

CONNECTS TO:
- IAGCStationImpl.h: Provides the base implementation for the station interface.
- resource.h: Contains resource IDs used for registration.
- LIBID_AGCLib: Library ID for the Allegiance game's COM library.

NOTES:
- None.

## `src/AGC/AGCStation.rgs`

PURPOSE: This file is a Windows Registry Script (.rgs) that defines the class information for the AGCStation object in Allegiance. It specifies the CLSID, ProgID, and TypeLib associated with the AGCStation class.

KEY PARTS:
- Name: AGC.AGCStation.1
  - Description: Represents the versioned programmatic identifier for the AGCStation class.
- Name: AGC.AGCStation
  - Description: Represents the non-versioned programmatic identifier for the AGCStation class, which points to the latest version (AGC.AGCStation.1).
- CLSID: {88400BB2-026B-11D3-8B55-00C04F681633}
  - Description: Unique identifier for the AGCStation class.
- ProgID: AGC.AGCStation.1
  - Description: Programmatic Identifier that maps to a specific version of the AGCStation class.
- VersionIndependentProgID: AGC.AGCStation
  - Description: Programmatic Identifier that maps to the latest version of the AGCStation class.
- TypeLib: {AB33E136-0A3E-11D3-8B58-00C04F681633}
  - Description: Unique identifier for the type library associated with the AGCStation class.

CONNECTS TO:
- Allegiance game engine, specifically related to space station objects.

NOTES:
- None.

## `src/AGC/AGCStations.cpp`

PURPOSE: tiny file (8 lines), shown in full rather than summarized.

    /////////////////////////////////////////////////////////////////////////////
    // AGCStations.cpp : Implementation of the CAGCStations class.
    //
    #include "pch.h"
    #include "AGCStations.h"
    /////////////////////////////////////////////////////////////////////////////
    // CAGCStations
    TC_OBJECT_EXTERN_NON_CREATEABLE_IMPL(CAGCStations)

## `src/AGC/AGCStations.h`

PURPOSE: This file defines a COM object class for managing stations in the Allegiance game.

KEY PARTS:
- CAGCStations: Main class implementing IAGCStations interface and inheriting from IAGCCollectionImpl to manage a collection of station objects.
- DECLARE_REGISTRY_RESOURCEID, DECLARE_PROTECT_FINAL_CONSTRUCT, BEGIN_COM_MAP, END_COM_MAP: Macros for COM registration and interface mapping.
- CATID_AGC: Category identifier for the AGC component.

CONNECTS TO: Allegiance game engine, likely uses other game-specific interfaces like IstationIGC and IAGCStation.

NOTES: None.

## `src/AGC/AGCStations.rgs`

PURPOSE: This file is a Windows registry script that defines the class information for the AGCStations object in Allegiance. It registers the CLSID and ProgID for the AGCStations class.

KEY PARTS:
- Name: AGC.AGCStations.1 - Registers the main version of the AGCStations class.
- CLSID: {7A0159B4-0272-11D3-8B55-00C04F681633} - Unique identifier for the AGCStations class.
- ProgID: AGC.AGCStations.1 - Programmatic identifier used to create instances of the class.
- VersionIndependentProgID: AGC.AGCStations - Allows compatibility with older versions of the class.

CONNECTS TO: Windows registry system.

NOTES: None.

## `src/AGC/AGCTeam.cpp`

PURPOSE: Implements the CAGCTeam class, which is part of the Allegiance game's team management system.

KEY PARTS:
- CAGCTeam: Manages team-related operations.
- InterfaceSupportsErrorInfo: Checks if a given interface supports error information.

CONNECTS TO: AGCTeam.h

NOTES: None.

## `src/AGC/AGCTeam.h`

PURPOSE: This file defines the CAGCTeam class, which represents a team in the Allegiance game. It implements the IAGCTeam interface and provides support for error information.

KEY PARTS:
- CAGCTeam: Main class representing a team.
- DECLARE_REGISTRY_RESOURCEID: Registers the resource ID for the team.
- DECLARE_PROTECT_FINAL_CONSTRUCT: Ensures that the final constructor is protected.
- BEGIN_COM_MAP: Maps COM interfaces to their implementations.
- END_COM_MAP: Ends the COM interface mapping.
- BEGIN_CATEGORY_MAP: Specifies the category of the team.
- END_CATEGORY_MAP: Ends the category map.
- STDMETHOD(InterfaceSupportsErrorInfo): Implements error support for interfaces.

CONNECTS TO: IAGCTeamImpl, IsideIGC, LIBID_AGCLib

NOTES: None.

## `src/AGC/AGCTeam.rgs`

PURPOSE: This file is a Windows registry script that defines the class information for the AGCTeam object in Allegiance. It specifies the CLSID, ProgID, and TypeLib associated with the AGCTeam class.

KEY PARTS:
- Name: AGC.AGCTeam.1 - Represents the versioned programmatic identifier for the AGCTeam class.
- CLSID: {5C2B1828-EBA5-11D2-8B4B-00C04F681633} - Unique identifier for the AGCTeam class.
- ProgID: AGC.AGCTeam.1 - Programmatic identifier for the AGCTeam class.
- VersionIndependentProgID: AGC.AGCTeam - Base programmatic identifier for the AGCTeam class.
- TypeLib: {AB33E136-0A3E-11D3-8B58-00C04F681633} - Library that contains type information for the AGCTeam class.

CONNECTS TO: Allegiance game engine, specifically related to team management and player interactions within the game.

NOTES: None.

## `src/AGC/AGCTeams.cpp`

PURPOSE: tiny file (8 lines), shown in full rather than summarized.

    /////////////////////////////////////////////////////////////////////////////
    // AGCTeams.cpp : Implementation of the CAGCTeams class.
    //
    #include "pch.h"
    #include "AGCTeams.h"
    /////////////////////////////////////////////////////////////////////////////
    // CAGCTeams
    TC_OBJECT_EXTERN_NON_CREATEABLE_IMPL(CAGCTeams)

## `src/AGC/AGCTeams.h`

PURPOSE:
This file defines a COM object class for managing teams in the Allegiance game. It implements an interface for accessing and manipulating team data.

KEY PARTS:
- CAGCTeams: Main class implementing IAGCTeams interface to manage teams.
- DECLARE_REGISTRY_RESOURCEID, DECLARE_PROTECT_FINAL_CONSTRUCT: Macros for registration and final construction protection.
- BEGIN_COM_MAP, END_COM_MAP: Maps COM interfaces to implementation methods.
- BEGIN_CATEGORY_MAP, END_CATEGORY_MAP: Defines the category of the COM object.

CONNECTS TO:
AGCLib library, SideListIGC data structure.

NOTES:
None.

## `src/AGC/AGCTeams.rgs`

PURPOSE: This file is a Windows Registry Script that defines the CLSID and ProgID for the AGCTeams class in Allegiance.

KEY PARTS:
- Name: AGC.AGCTeams.1 - Represents the version 1 of the AGCTeams class.
- CLSID: {7151948E-EBA5-11D2-8B4B-00C04F681633} - Unique identifier for the AGCTeams class.
- ProgID: AGC.AGCTeams.1 - Programmatic Identifier for version 1 of the AGCTeams class.
- VersionIndependentProgID: AGC.AGCTeams - Base ProgID without version information.

CONNECTS TO: Windows Registry

NOTES: None

## `src/AGC/AGCVector.cpp`

PURPOSE: This file contains the implementation of the CAGCVector class, which represents a 3D vector in Allegiance. It provides methods for creating, manipulating, and accessing vector data.

KEY PARTS:
- CAGCVector: Main class representing a 3D vector.
- GetRawVector: Retrieves the raw vector data from an IAGCVector interface.
- CreateResultVector: Creates a new IAGCVector instance initialized with given vector data.
- InterfaceSupportsErrorInfo, GetClassID, IsDirty, Load, Save, GetSizeMax: Implement interfaces for error handling, class identification, persistence, and property bag operations.
- put_X, get_X, put_Y, get_Y, put_Z, get_Z: Properties to set and get the X, Y, and Z components of the vector.
- put_DisplayString, get_DisplayString: Methods to convert between a string representation and vector components.
- InitXYZ, InitCopy, InitRandomDirection, InitRandomPosition: Initialization methods for setting vector values.
- get_IsZero, get_IsEqual, get_Length, get_LengthSquared: Properties to check vector properties.
- Add, Subtract, Multiply, Divide, Normalize, CrossProduct, Interpolate: Methods for vector arithmetic operations.
- AddInPlace, SubtractInPlace, MultiplyInPlace, DivideInPlace, NormalizeInPlace, CrossProductInPlace, InterpolateInPlace: In-place versions of the above methods.
- InitFromVector, CopyVectorTo: Private methods for initializing and copying vector data.

CONNECTS TO: Allegiance game engine systems that require 3D vector operations.

NOTES: None.

## `src/AGC/AGCVector.h`

PURPOSE: This file defines a COM object `CAGCVector` that represents a 3D vector with various operations and properties.

KEY PARTS:
- CAGCVector: Main class implementing the IAGCVector interface for managing 3D vectors.
- DECLARE_REGISTRY_RESOURCEID, DECLARE_PROTECT_FINAL_CONSTRUCT, DECLARE_GET_CONTROLLING_UNKNOWN: Macros for COM registration and construction management.
- BEGIN_COM_MAP, END_COM_MAP: Maps interface pointers to implementation methods.
- BEGIN_CATEGORY_MAP, END_CATEGORY_MAP: Defines categories for the COM object.
- m_vector: Member variable storing the actual vector data.
- put_X, get_X, put_Y, get_Y, put_Z, get_Z: Methods to set and get the X, Y, and Z components of the vector.
- InitXYZ, InitCopy, InitRandomDirection, InitRandomPosition: Initialization methods for different scenarios.
- Add, Subtract, Multiply, Divide: Vector arithmetic operations.
- Normalize: Normalizes the vector.
- CrossProduct: Computes the cross product with another vector.
- Interpolate: Interpolates between two vectors.

CONNECTS TO: AGC.h, TCLib\ObjectLock.h, resource.h, IAGCVector, IAGCVectorPrivate, ISupportErrorInfo, IPersistStreamInit, IPersistPropertyBag, AGCObjectSafetyImpl, CComObjectRootEx, CComCoClass.

NOTES: None.

## `src/AGC/AGCVector.rgs`

PURPOSE: This file is a Windows Registry Script (.rgs) that defines the class information for the AGCVector class in Allegiance. It specifies the CLSID, ProgID, and other properties required to register the class with the Windows Registry.

KEY PARTS:
- Name: AGC.AGCVector
  - Description: The ProgID for the AGCVector class.
- Name: {12249214-13B5-11d3-8B5E-00C04F681633}
  - Description: The CLSID for the AGCVector class.
- Name: ThreadingModel
  - Description: Specifies that the component supports both apartment and free-threaded apartments.

CONNECTS TO:
- Windows Registry

NOTES:
- None.

## `src/AGC/AGCVersionInfo.cpp`

PURPOSE: tiny file (7 lines), shown in full rather than summarized.

    /////////////////////////////////////////////////////////////////////////////
    // AGCVersionInfo.cpp : Implementation of the CAGCVersionInfo class.
    #include "pch.h"
    #include "AGCVersionInfo.h"
    /////////////////////////////////////////////////////////////////////////////
    // CAGCVersionInfo
    TC_OBJECT_EXTERN_IMPL(CAGCVersionInfo)

## `src/AGC/AGCVersionInfo.h`

PURPOSE: This file defines a COM class `CAGCVersionInfo` that implements the `IAGCVersionInfo` interface and provides version information for the Allegiance game.

KEY PARTS:
- CAGCVersionInfo: A COM class implementing IAGCVersionInfo, TCVersionInfoImpl, and IObjectSafety.
- DECLARE_REGISTRY_RESOURCEID(IDR_AGCVersionInfo): Registers the class with a resource ID.
- DECLARE_PROTECT_FINAL_CONSTRUCT(): Ensures the constructor is protected and final.
- DECLARE_GET_CONTROLLING_UNKNOWN(): Retrieves the controlling unknown interface pointer.
- BEGIN_CATEGORY_MAP: Maps categories for COM registration.
- BEGIN_COM_MAP: Maps COM interfaces to implementation methods.
- m_vi.Load(uintptr_t(_Module.GetModuleInstance())): Initializes version information with the module instance.

CONNECTS TO: AGC.h, resource.h, TCAtl\VersionInfoImpl.h

NOTES: None.

## `src/AGC/AGCVersionInfo.rgs`

PURPOSE: This file defines a Windows COM class registration for the AGCVersionInfo class in Allegiance.

KEY PARTS:
- Name: AGC.AGCVersionInfo
- CLSID: {9942F23D-B460-4bf0-8B2C-7B003A43F37C}
- ProgID: AGC.AGCVersionInfo
- InprocServer32: %MODULE%
- ThreadingModel: Both

CONNECTS TO: Windows COM system for class registration.

NOTES: None.

## `src/AGC/AGCWinApp.h`

PURPOSE: Declares the CAGCWinApp class, which is a subclass of TCWinAppDLL and handles application-specific functionality, including debugging hooks.

KEY PARTS:
- CAGCWinApp: Inherits from TCWinAppDLL and manages application-level operations.
- SetDebugHook: Sets an interface for handling debug output and assertions.
- DebugOutput: Outputs debug messages using the provided debug hook if available; otherwise, uses default behavior.
- OnAssert: Handles assertion failures using the debug hook if available; otherwise, uses default behavior.
- OnAssertBreak: Breaks into the debugger when an assertion fails using the debug hook if available; otherwise, uses default behavior.

CONNECTS TO: TCWinAppDLL and IAGCDebugHook (through m_spDebugHook).

NOTES: None.

## `src/AGC/AdminSessionHelper.cpp`

PURPOSE: The file contains the implementation of the CAdminSessionHelper class, which provides functionality to check if all server services are running.

KEY PARTS:
- CAdminSessionHelper: Class that implements methods for administrative session helper functionalities.
- get_IsAllSrvRunning: Method to determine if all server services are running by checking a mutex.

CONNECTS TO: pch.h (precompiled header), AdminSessionHelper.h (header file).

NOTES: None.

## `src/AGC/AdminSessionHelper.h`

PURPOSE: This file defines a COM component class named `CAdminSessionHelper` that implements the `IAdminSessionHelper` interface. The purpose is to provide administrative session management functionality.

KEY PARTS:
- CAdminSessionHelper: Implements the IAdminSessionHelper interface and provides methods for managing admin sessions.
- DECLARE_REGISTRY_RESOURCEID(IDR_AdminSessionHelper): Registers the component with a resource ID.
- DECLARE_GET_CONTROLLING_UNKNOWN(): Retrieves the controlling unknown pointer.
- BEGIN_CATEGORY_MAP(CAdminSessionHelper): Maps categories to the component, indicating it is safe for scripting and initialization.
- BEGIN_COM_MAP(CAdminSessionHelper): Maps COM interfaces to the component's methods.
- get_IsAllSrvRunning: A method that checks if all servers are running.

CONNECTS TO:
- AGCLib library (indicated by LIBID_AGCLib)

NOTES:
- None

## `src/AGC/AdminSessionHelper.rgs`

PURPOSE: This file is a Windows registry script that defines the class information for the AdminSessionHelper component in Allegiance. It registers the CLSID, ProgID, and other necessary details to allow the component to be instantiated and used by other parts of the application.

KEY PARTS:
- Name: AdminSessionHelper Class
  - Description: Manages administrative sessions within the game.
- CLSID: {859E6E3A-875D-4b0d-8775-E7C533E9EBEF}
  - Description: Unique identifier for the class.
- ProgID: TCObj.Utility.1
  - Description: Programmatic identifier used to reference the component.
- VersionIndependentProgID: AllSrv.AdminSessionHelper
  - Description: Identifier that remains constant across version changes.
- InprocServer32: %MODULE%
  - Description: Path to the DLL containing the implementation of the class.
- ThreadingModel: Both
  - Description: Specifies that the component can be used in both single-threaded and multi-threaded environments.

CONNECTS TO:
- Allegiance game engine, specifically components that require administrative session management.

NOTES:
- None.

## `src/AGC/EventBrowser.xsl`

PURPOSE: This XSLT file is used to transform XML data representing event definitions in Allegiance into an HTML page that allows users to browse and expand/collapse groups of events.

KEY PARTS:
- `ExpandContract()`: JavaScript function to toggle the display of nested event groups.
- `EventGroup` template: Handles the transformation of each `EventGroup` element, creating a collapsible section for each group.
- `Event` template: Handles the transformation of individual `Event` elements within an `EventGroup`.

CONNECTS TO: This file is used by the Allegiance game to generate the event browser interface.

NOTES: None.

## `src/AGC/IAGCAlephImpl.h`

PURPOSE: This file defines a template class `IAGCAlephImpl` that implements the `IAGCAleph` interface and extends functionality from `IAGCModelImpl`. It provides methods to interact with destination objects.

KEY PARTS:
- Class: `IAGCAlephImpl`
  - Implements the `IAGCAleph` interface.
  - Inherits from `IAGCModelImpl`.
  - Provides a method `get_Destination` to retrieve the destination object.

CONNECTS TO:
- Depends on `IAGCModelImpl`, `IAGCAleph`, and related interfaces.

NOTES: None.

## `src/AGC/IAGCAsteroidImpl.h`

PURPOSE: This file defines a template class `IAGCAsteroidImpl` that implements the `IAGCAsteroid` interface for asteroids in Allegiance. It extends functionality from `IAGCDamageImpl`.

KEY PARTS:
- IAGCAsteroidImpl: Template class implementing `IAGCAsteroid` and extending `IAGCDamageImpl`.
- get_Ore: Retrieves the ore value of an asteroid.
- get_Capabilities: Retrieves the capabilities bitmask of an asteroid.
- get_HasCapability: Checks if an asteroid has a specific capability.

CONNECTS TO:
- IAGCDamageImpl.h: Inherits from this class to extend damage-related functionality.

NOTES:
- None.

## `src/AGC/IAGCBaseImpl.h`

PURPOSE: This file defines a template class `IAGCBaseImpl` that implements the `IAGCBase` interface and provides common functionality for AGC objects representing IGC objects.

KEY PARTS:
- `IAGCBaseImpl`: A template class implementing `IAGCBase`, `IDispatch`, and `IAGCCommonImpl`.
- `get_ObjectType()`: Returns the type of the object.
- `get_ObjectID()`: Returns the ID of the object.
- `get_UniqueID()`: Combines the object's type and ID to form a unique identifier.
- `get_Game()`: Retrieves a pointer to the game in which the object lives.

CONNECTS TO: `IAGCCommonImpl`, `IAGCBase`, `IDispatch`, `AGCGlobal`.

NOTES:
- Contains assertions for debugging purposes.
- Uses macros for interface entries.

## `src/AGC/IAGCBuyableImpl.h`

PURPOSE:
This file defines a template class `IAGCBuyableImpl` that implements the `IAGCBuyable` interface for objects that can be bought in the game Allegiance. It inherits from `IAGCBaseImpl` and provides implementations for methods related to buying items, such as getting their name, description, model name, price, and time to build.

KEY PARTS:
- `IAGCBuyableImpl`: Template class implementing the `IAGCBuyable` interface.
- `get_Name`, `get_Description`, `get_ModelName`, `get_Price`, `get_TimeToBuild`: Methods that retrieve various properties of a buyable item.
- `COM_INTERFACE_ENTRIES_IAGCBuyableImpl`: Macro to define COM interface entries for `IAGCBuyableImpl`.

CONNECTS TO:
- `IAGCBaseImpl.h`: The base implementation class.
- `IAGCBuyable`: The interface being implemented.

NOTES:
- None.

## `src/AGC/IAGCCollectionImpl.h`

PURPOSE: This file defines a template class `IAGCCollectionImpl` that implements the `IAGCCollection` and `IDispatch` interfaces for collections of objects in Allegiance.

KEY PARTS:
- `IAGCCollectionImpl`: A template class implementing collection-related methods.
- `get_Count`: Returns the number of items in the collection.
- `get__NewEnum`: Provides an enumerator for iterating over the collection items.
- `get_Item`: Retrieves a specific item from the collection based on its index.

CONNECTS TO: `IAGCCommonImpl`, `CComEnum`, `VARIANT`, and other Allegiance-specific interfaces and classes.

NOTES:
- The implementation uses `CComVariant` for handling variant data types, which is typical in COM-based applications.
- There are comments indicating potential issues with memory management and performance optimizations.

## `src/AGC/IAGCCommonImpl.h`

PURPOSE: This header file defines a template class `IAGCCommonImpl` that implements common functionality for AGC objects. It provides interfaces and methods for managing object types, unique IDs, and initialization.

KEY PARTS:
- `IAGCCommonImpl`: A template class implementing the `IAGCCommon` and `IAGCPrivate` interfaces.
- `Host2Igc` and `Igc2Host`: Conversion functions between host and IGC pointers.
- `COM_INTERFACE_ENTRIES_IAGCCommonImpl`: Macro for interface map entries.
- `DECLARE_AGC_TYPE`: Macro to declare the type of an AGC object.
- `ObjectMain`: Static method to register object creators.
- `CreateAGCObject`: Static method to create instances of AGC objects.

CONNECTS TO: AGC.h, AGCGlobal, IAGCGlobalPtr

NOTES:
- Contains commented-out code for `get_UniqueID` method.
- Uses macros and templates extensively for generic implementation.

## `src/AGC/IAGCDamageImpl.h`

PURPOSE: This file defines a template class `IAGCDamageImpl` that implements the `IAGCDamage` interface and extends functionality from `IAGCModelImpl`.

KEY PARTS:
- `IAGCDamageImpl`: A template class implementing the `IAGCDamage` interface.
- `get_Fraction`: A method to retrieve a fraction value, which asserts that the underlying IGC object is valid and then calls its `GetFraction` method.

CONNECTS TO: `IAGCModelImpl`, `IAGCDamage`

NOTES: None.

## `src/AGC/IAGCGameImpl.h`

PURPOSE: The file defines a template class `IAGCGameImpl` that implements the `IAGCGame` interface for managing game data and operations in Allegiance.

KEY PARTS:
- Name: `IAGCGameImpl`
  - Purpose: Implements methods to interact with game objects such as ships, teams, sectors, and parameters.
  
- Function: `get_Name`
  - Purpose: Returns the name of the mission/game.
  
- Function: `get_Sectors`
  - Purpose: Returns a collection of sectors in the game.

- Function: `get_Teams`
  - Purpose: Returns a collection of teams in the game.

- Function: `get_LookupShip`
  - Purpose: Converts an AGC ID to a ship object.

- Function: `SendChat`
  - Purpose: Sends chat messages within the game.

- Function: `SendCommand`
  - Purpose: Sends commands to specific models (e.g., ships) in the game.

- Function: `get_ReplayCount`
  - Purpose: Returns the number of replays for the current mission.

CONNECTS TO:
- Other Allegiance components, particularly those related to game objects and networking.

NOTES:
- Contains TODO comments indicating areas that need further implementation.
- Uses ATL (Active Template Library) macros for interface management.

## `src/AGC/IAGCModelImpl.h`

PURPOSE: This file defines a template class `IAGCModelImpl` that implements the `IAGCModel` interface for managing game objects in Allegiance. It provides methods to retrieve various properties of these objects such as visibility, position, velocity, and more.

KEY PARTS:
- `IAGCModelImpl`: A template class implementing the `IAGCModel` interface.
- `get_IsVisible()`: Retrieves whether the object is visible.
- `get_IsSeenBySide()`: Checks if the object is seen by a specific team.
- `get_Position()`, `get_Velocity()`, `get_Orientation()`: Retrieve position, velocity, and orientation vectors of the object.
- `get_Radius()`: Gets the radius of the object.
- `get_Team()`: Retrieves the team associated with the object.
- `get_Mass()`: Gets the mass of the object.
- `get_Sector()`: Retrieves the sector in which the object is located.
- `get_Signature()`: Gets the signature value of the object.
- `get_Name()`: Returns the name of the object.

CONNECTS TO: `IAGCBaseImpl`, `IAGCTeam`, `IAGCSector`, `AGCGlobal`.

NOTES:
- Some methods are commented out with TODOs indicating they need implementation.

## `src/AGC/IAGCProbeImpl.h`

PURPOSE: This file defines a template class `IAGCProbeImpl` that implements the `IAGCProbe` interface for probes in Allegiance. It inherits from `IAGCScannerImpl` and provides implementations for various methods related to probe properties.

KEY PARTS:
- `IAGCProbeImpl`: Template class implementing the `IAGCProbe` interface.
- `get_EmissionPoint`, `get_Lifespan`, `get_DtBurst`, `get_Accuracy`, `get_IsRipcord`: Methods that retrieve various properties of a probe.

CONNECTS TO: `IAGCScannerImpl`, `IAGCProjectileType`, `AGCVector`, and other Allegiance game components.

NOTES: None.

## `src/AGC/IAGCRangeImpl.h`

PURPOSE: This file defines a template class `IAGCRangeImpl` that implements various interfaces for managing ranges of values, providing methods to initialize, get and set range values, check intersections, and serialize the range data.

KEY PARTS:
- `IAGCRangeImpl`: A template class implementing interfaces like IDispatch, IAGCRangePrivate, ISupportErrorInfo, IPersistStreamInit, and IPersistPropertyBag.
- `GetVariantValue`, `GetPrefixString`, etc.: Methods to handle value formatting and parsing.
- `put_DisplayString` and `get_DisplayString`: Methods for setting and getting the display string representation of the range.
- `Init`, `get_Lower`, `get_Upper`, etc.: Methods to manage and retrieve range properties.
- `m_range`: Data member that stores the range values.

CONNECTS TO: Allegiance game engine, using ATL (Active Template Library) for COM interface implementation.

NOTES: None.

## `src/AGC/IAGCRangesImpl.h`

PURPOSE: This file defines a template class `IAGCRangesImpl` that implements the `IAGCRangesPrivate`, `IDispatch`, `ISupportErrorInfo`, `IPersistStreamInit`, and `IPersistPropertyBag` interfaces for managing ranges of values.

KEY PARTS:
- `IAGCRangesImpl`: A template class implementing various COM interfaces to manage a set of ranges.
- `XRangeSet`: A type alias for a range set container.
- `get_Count()`: Returns the number of ranges in the collection.
- `get__NewEnum()`: Enumerates all ranges in the collection.
- `get_Item()`: Retrieves a specific range by index.
- `put_DisplayString()`: Parses and sets ranges from a display string.
- `AddByValues()` and `RemoveByValues()`: Adds or removes ranges by value.

CONNECTS TO: Uses classes from `TCObjectLock`, `range`, `rangeset`, and other Allegiance libraries for range management and COM interface support.

NOTES:
- Contains commented-out code that may be unused.
- The `get__NewEnum()` method has a conditional compilation block related to Visual Studio version, which might need review.

## `src/AGC/IAGCScannerImpl.h`

PURPOSE: This file defines a template class `IAGCScannerImpl` that implements the `IAGCScanner` interface and extends functionality from `IAGCDamageImpl`. It provides methods to check if an object is within scanner range and if it can be seen.

KEY PARTS:
- `IAGCScannerImpl`: A template class implementing `IAGCScanner` and extending `IAGCDamageImpl`.
- `get_InScannerRange`: Checks if a model is within the scanner range.
- `get_CanSee`: Determines if a model can be seen by another model.

CONNECTS TO: `IAGCDamageImpl`, `IAGCModel`, `ImodelIGC`.

NOTES: None.

## `src/AGC/IAGCSectorImpl.h`

PURPOSE: This file defines a template class `IAGCSectorImpl` that implements the `IAGCSector` interface for managing sectors in the Allegiance game. It provides methods to interact with various objects within a sector such as stations, ships, alephs, asteroids, and more.

KEY PARTS:
- `IAGCSectorImpl`: A template class implementing the `IAGCSector` interface.
- `get_Name`: Retrieves the name of the sector.
- `get_Stations`, `get_Ships`, etc.: Retrieve collections of specific objects within the sector.
- `SendChat`: Sends a chat message to other players in the sector.
- `SendCommand`: Sends a command to a model target within the sector.

CONNECTS TO: `IAGCBaseImpl`, `AGCGlobal`, and related interfaces for managing game objects and interactions.

NOTES: None.

## `src/AGC/IAGCShipImpl.h`

PURPOSE: This file defines a template class `IAGCShipImpl` that implements the `IAGCShip` interface for managing ship properties and behaviors in Allegiance.

KEY PARTS:
- `IAGCShipImpl`: A template class implementing the `IAGCShip` interface.
- `put_Ammo`, `get_Ammo`, `put_Fuel`, `get_Fuel`, `put_Energy`, `get_Energy`, `put_WingID`, `get_WingID`: Methods to get and set read-only properties of a ship's ammunition, fuel, energy, and wing ID.
- `SendChat`: Sends chat messages from the ship.
- `SendCommand`: Sends commands to other ships or models.
- `put_AutoDonate`, `get_AutoDonate`: Methods to manage auto-donation settings for the ship.
- `put_ShieldFraction`, `get_ShieldFraction`: Methods to get and set the shield fraction of the ship.
- `get_HullType`, `get_BaseHullType`: Methods to retrieve the current and base hull types of the ship.

CONNECTS TO: `IAGCScannerImpl`, `AGCGlobal`, `ImodelIGC`, `IshieldIGC`, `IAGCHullType`.

NOTES: None.

## `src/AGC/IAGCStationImpl.h`

PURPOSE: This file defines a template class `IAGCStationImpl` that implements the `IAGCStation` interface for stations in the Allegiance game. It extends functionality from `IAGCScannerImpl`.

KEY PARTS:
- `IAGCStationImpl`: A template class implementing the `IAGCStation` interface.
- `put_ShieldFraction`: Sets the shield fraction of a station.
- `get_ShieldFraction`: Retrieves the shield fraction of a station.

CONNECTS TO: `IAGCScannerImpl`, `IAGCStation`

NOTES: None.

## `src/AGC/IAGCTeamImpl.h`

PURPOSE:
This file defines a template class `IAGCTeamImpl` that implements the `IAGCTeam` interface for managing team-related operations in Allegiance. It provides methods to get team name, stations, ships, civilization information, and send chat commands.

KEY PARTS:
- `IAGCTeamImpl`: A template class implementing the `IAGCTeam` interface.
- `get_Name`: Retrieves the team's name.
- `get_Stations`: Retrieves the team's stations.
- `get_Ships`: Retrieves the team's ships.
- `get_Civ`: Retrieves the civilization associated with the team.
- `SendChat`: Sends a chat message to the team or wing.
- `SendCommand`: Sends a command to a model target within the team.

CONNECTS TO:
Depends on `IAGCBaseImpl`, `AGCChat`, and other Allegiance game components for functionality like mission site operations, object retrieval, and sound effects.

NOTES:
None.

## `src/AGC/Pre-build.bat`

PURPOSE: tiny file (9 lines), shown in full rather than summarized.

    echo Building message resources for AGCEvents...
    set targetDir=..\%1
    cscript /e:jscript /NoLogo ..\..\src\AGC\XMLXForm.js ..\..\src\AGC\AGCEvents.xml ..\..\src\AGC\AGCEventsCPP.xsl %targetDir%AGCEventsCPP.h
    cscript /e:jscript /NoLogo ..\..\src\AGC\XMLXForm.js ..\..\src\AGC\AGCEvents.xml ..\..\src\AGC\AGCEventsRC2.xsl %targetDir%AGCEventsRC2.rc2
    cscript /e:jscript /NoLogo ..\..\src\AGC\XMLXForm.js ..\..\src\AGC\AGCEvents.xml ..\..\src\AGC\AGCEventsRCH.xsl %targetDir%AGCEventsRCH.h
    cscript /e:jscript /NoLogo ..\..\src\AGC\XMLXForm.js ..\..\src\AGC\AGCEvents.xml ..\..\src\AGC\AGCEventsMC.xsl %targetDir%AGCEventsMC.mc
    echo Compiling AGC messages
    echo mc.exe -r %targetDir% -h %targetDir% %targetDir%AGCEventsMC.mc
    mc.exe -r %targetDir% -h %targetDir% %targetDir%AGCEventsMC.mc

## `src/AGC/TCMarshalByValue.cpp`

PURPOSE:
This file contains the implementation of the `CTCMarshalByValue` class, which is a COM component object that implements the `IMarshal` interface. It provides marshaling functionality by delegating to the outer object's `IPersistStream` or `IPersistStreamInit` implementations.

KEY PARTS:
- CTCMarshalByValue: Implements the IMarshal interface for marshaling data.
- GetUnmarshalClass: Determines the class ID of the object being marshaled.
- GetMarshalSizeMax: Calculates the maximum size required to marshal the object.
- MarshalInterface: Writes the object's state to a stream.
- UnmarshalInterface: Reads the object's state from a stream and initializes the object.
- ReleaseMarshalData: Not used in this implementation.
- DisconnectObject: Not used in this implementation.

CONNECTS TO:
- IPersistStream and IPersistStreamInit interfaces of the outer object.

NOTES:
- Contains debug tracing for construction and release methods.
- Uses `CLock` for thread synchronization during unmarshaling.

## `src/AGC/TCMarshalByValue.h`

PURPOSE: This file defines a COM component class `CTCMarshalByValue` that implements the `IMarshal` interface for marshaling and unmarshaling objects across different contexts in a distributed environment.

KEY PARTS:
- CTCMarshalByValue: Implements the IMarshal interface to handle object marshalling.
- GetUnmarshalClass: Retrieves the CLSID of the class that can unmarshal the object.
- GetMarshalSizeMax: Calculates the maximum size required for the marshaled data.
- MarshalInterface: Marshals an interface pointer into a stream.
- UnmarshalInterface: Unmarshals an interface pointer from a stream.
- ReleaseMarshalData: Releases any resources held by the marshaler.
- DisconnectObject: Disconnects the object from its marshaling context.

CONNECTS TO: IMarshal, IPersistStream, and IPersistStreamInit interfaces.

NOTES: None.

## `src/AGC/TCMarshalByValue.rgs`

PURPOSE: This file is a Windows Registry Script (.rgs) that defines the registration information for a COM object used in the Allegiance game. It specifies details such as the class ID, type library, and threading model.

KEY PARTS:
- TCObj.MarshalByValue.1: Represents the versioned ProgID for the Marshal By Value Helper Object.
- CLSID {8962E244-CD81-11d2-9646-00C04F68DEB0}: Unique identifier for the COM object.
- TypeLib {AB33E136-0A3E-11D3-8B58-00C04F681633}: Reference to the type library associated with the COM object.
- ProgID: Programmatic Identifier used to create instances of the COM object.
- InprocServer32: Path to the DLL that implements the COM object, using a placeholder %MODULE% which should be replaced by the actual module name.

CONNECTS TO: Allegiance game codebase, specifically components that interact with COM objects for marshalling and remote procedure calls.

NOTES: None.

## `src/AGC/TCNullStream.cpp`

PURPOSE: tiny file (16 lines), shown in full rather than summarized.

    /////////////////////////////////////////////////////////////////////////////
    // TCNullStream.cpp | Implementation of the CTCNullStream class, which
    // implements the CLSID_TCNullStream component object.<nl><nl>
    // This object provides an implementation of the IStream interface that does
    // nothing more that count the number of bytes being written to it.  Since
    // ATL (currently 3.0) implements IPersistStreamInitImpl::GetSizeMax by
    // returning E_NOTIMPL, this object can be used in an override of that
    // method. This is accomplished by creating an instance of this null stream
    // and 'persisting' an object to it, thereby counting the number of bytes
    // needed to persist the object without allocating any memory.
    //
    #include "pch.h"
    #include "TCNullStream.h"
    /////////////////////////////////////////////////////////////////////////////
    // CTCNullStream
    TC_OBJECT_EXTERN_IMPL(CTCNullStream)

## `src/AGC/TCNullStream.h`

PURPOSE: This file defines a COM component object class named CTCNullStream that implements the CLSID_TCNullStream.

KEY PARTS:
- Name: CTCNullStream - Implements a COM component object.
- Interface Map: Defines interfaces for the component, including IUnknown and chaining to CTCNullStreamImpl.
- Category Map: Specifies the category of the component as CATID_AGC.
- DECLARE_REGISTRY_RESOURCEID: Registers the component using a resource ID.

CONNECTS TO: AGC.h - Header file containing necessary declarations for the component.

NOTES: None.

## `src/AGC/TCNullStream.rgs`

PURPOSE: This file is a Windows registry script that defines the class information for a Null IStream Object in Allegiance.

KEY PARTS:
- TCObj.NullStream.1: A versioned ProgID for the Null IStream Object.
- CLSID {8962E248-CD81-11d2-9646-00C04F68DEB0}: The unique identifier for the Null IStream Object class.
- TypeLib {AB33E136-0A3E-11D3-8B58-00C04F681633}: Links to the type library that contains information about the object's interface.
- ProgID: The programmatic identifier for the Null IStream Object.
- VersionIndependentProgID: A version-independent identifier for the Null IStream Object.
- InprocServer32: Specifies the DLL file that implements the Null IStream Object, which is Allegiance.exe in this case.
- ThreadingModel: Indicates that the object supports both single-threaded and multi-threaded apartments.

CONNECTS TO: Allegiance.exe

NOTES: None.

## `src/AGC/TCPropBagOnRegKey.cpp`

PURPOSE: The file `TCPropBagOnRegKey.cpp` implements a component object that uses a registry key to store and retrieve properties, managing various data types including safe arrays and objects.

KEY PARTS:
- CTCPropBagOnRegKey: Manages properties stored in a registry key.
- CLSIDFromProgID: Converts a programmatic identifier to a class identifier.
- _ReadVariantFromValue: Reads a value from the registry and converts it to a variant.
- _LogError: Logs an error with details about the function, description, HRESULT, property name, and error log interface.
- SafeArrayGetUBound and SafeArrayGetLBound: Get upper and lower bounds of a safe array.
- SafeArrayAccessData and SafeArrayUnaccessData: Access and release data in a safe array.
- WriteVariant: Writes a variant to a registry subkey.
- IsSubkeySafeArray and IsSubkeyObject: Check if a subkey is a safe array or an object.
- CComVariant: A COM variant class.

CONNECTS TO: Windows Registry, COM (Component Object Model), DCOM (Distributed Component Object Model).

NOTES:
- TODOs for adding support for more data types in `ReadSafeArray` and `WriteSafeArray`.
- Error handling needs improvement in various functions.

PARTS (in file order):

- lines 1-364
  - CTCPropBagOnRegKey: Implements a component object that uses a registry key to store and retrieve properties.
  - _GetSubkey: Retrieves the full subkey path for this object.
  - _GetServer: Reads the server name from the registry.
  - _SetServer: Writes the server name to the registry.
  - _GetObjectCLSID: Retrieves the CLSID of an object stored in the registry.
  - _CreateObject: Creates a new object either locally or remotely based on the server information.
  - _CreateLocalObject: Creates a local object using COM.
  - _CreateRemoteObject: Creates a remote object using DCOM.
- lines 365-714
  - CTCPropBagOnRegKey: Manages properties stored in a registry key.
  - CLSIDFromProgID: Converts a programmatic identifier to a class identifier.
  - _ReadVariantFromValue: Reads a value from the registry and converts it to a variant.
  - _LogError: Logs an error with details about the function, description, HRESULT, property name, and error log interface.
  - _GetSubkeyVarType: Retrieves the variant type of a subkey in the registry.
  - _ReadSafeArray: Reads a safe array from the registry.
  - _WriteSafeArray: Writes a safe array to the registry.
  - TODOs: Add support for more data types in ReadSafeArray and WriteSafeArray. Handle errors more gracefully in various functions.
- lines 715-1045
  - CTCPropBagOnRegKey::_WriteSafeArray: Writes a safe array to the registry.
  - CTCPropBagOnRegKey::_ReadVariant: Reads a variant from the registry.
  - CTCPropBagOnRegKey::_WriteVariant: Writes a variant to the registry.
  - SafeArrayGetUBound and SafeArrayGetLBound: Get upper and lower bounds of a safe array.
  - SafeArrayAccessData and SafeArrayUnaccessData: Access and release data in a safe array.
  - WriteVariant: Writes a variant to a registry subkey.
  - IsSubkeySafeArray and IsSubkeyObject: Check if a subkey is a safe array or an object.
  - CComVariant: A COM variant class.
- lines 1046-1432
  - CTCPropBagOnRegKey: Manages a property bag stored in the Windows registry.
  - _VarTypeFromUnknown: Determines the type of an unknown object.
  - RootKeyFromString: Converts a string representation of a registry root key to its corresponding HKEY value.
  - KeyExists: Checks if a specified subkey exists under a given registry key.
  - QueryString: Retrieves a string value from the registry.
  - CreateKey: Creates or opens a registry key.
  - OpenKey: Opens an existing registry key.
  - CreateObject, CreateLocalObject, CreateRemoteObject: Methods to create objects based on the property bag.

## `src/AGC/TCPropBagOnRegKey.h`

PURPOSE: The file defines a COM component class `CTCPropBagOnRegKey` that implements the `ITCPropBagOnRegKey` interface and provides functionality to interact with properties stored in a Windows registry key.

KEY PARTS:
- **Class**: CTCPropBagOnRegKey - Implements property bag operations using the Windows registry.
- **Methods**: 
  - `_CreateObject`, `_CreateLocalObject`, `_CreateRemoteObject`: Create objects based on registry settings.
  - `_LoadObject`, `_SaveObject`: Load and save object properties to/from the registry.
  - `Read`, `Write`: Implement IPropertyBag interface methods for reading and writing property values.
- **Attributes**: 
  - `_GetSubkey`, `_GetServer`, `_SetServer`, `_GetObjectCLSID`: Retrieve, set, and manage server and object CLSID information.
- **Data Members**: 
  - `m_strSubkey`, `m_pParent`, `m_key`, `m_bOwnKey`: Manage the registry key and its properties.

CONNECTS TO: Windows Registry API (`CRegKey`), COM interfaces (`IDispatch`, `IPropertyBag`, `IObjectSafety`).

NOTES: None.

## `src/AGC/TCPropBagOnRegKey.rgs`

PURPOSE: This file defines a registry key and its associated properties for a COM object in Allegiance that implements a property bag using a registry key.

KEY PARTS:
- Name: TCObj.PropBagOnRegKey.1
  - Description: Property Bag Implemented on a Registry Key, version 1.
- CLSID: {8962E245-CD81-11d2-9646-00C04F68DEB0}
  - Unique identifier for the COM object.
- TypeLib: {AB33E136-0A3E-11D3-8B58-00C04F681633}
  - Reference to the type library that defines the interface for this COM object.
- ProgID: TCObj.PropBagOnRegKey.1
  - Programmatic identifier used in code to reference this COM object.
- VersionIndependentProgID: TCObj.PropBagOnRegKey
  - Identifier used when version independence is required.
- InprocServer32: %MODULE%
  - Path to the DLL that implements the COM object, using a placeholder for the actual module name.

CONNECTS TO:
- Allegiance game engine (specifically, the component system and registry handling).

NOTES:
- None.

## `src/AGC/TCStrings.cpp`

PURPOSE: The file TCStrings.cpp implements a collection of strings with methods to add, remove, and manipulate the strings.

KEY PARTS:
- CTCStrings: Class implementing ITCCollection, ITCStrings, and ITCCollectionPersistHelper interfaces for managing a collection of strings.
- get_Count: Returns the number of strings in the collection.
- get__NewEnum: Provides an enumerator for iterating over the collection.
- get_Item: Retrieves a string by index.
- Add: Adds a new string to the collection.
- Remove: Removes a string by index.
- RemoveAll: Clears all strings from the collection.
- AddDelimited: Adds multiple strings separated by a delimiter.
- get_DelimitedItems: Returns all strings concatenated with a delimiter.
- AddStrings: Adds strings from another ITCStrings object.
- put_Collection1 and get_Collection1: Persistently store and retrieve the collection as a safe array.

CONNECTS TO: Depends on CComBSTR, CComVariant, and other COM-related classes for string manipulation and memory management.

NOTES: The code contains some conditional compilation directives related to compiler versions. There is a comment suggesting that the enumerator code may not be used or may have issues with certain compilers.

## `src/AGC/TCStrings.h`

PURPOSE: The file defines a COM component class `CTCStrings` that implements various interfaces for managing a collection of strings.

KEY PARTS:
- CTCStrings: Main class implementing interfaces like ITCStrings, ITCCollection, and IPersist.
- get_Count: Returns the number of items in the collection.
- get_Item: Retrieves an item by index.
- Add: Adds a new string to the collection.
- Remove: Removes an item from the collection.
- AddDelimited: Adds multiple strings separated by a delimiter.
- CStringVector: Internal data structure to store the collection of strings.

CONNECTS TO: AGC.h, resource.h, and other COM-related interfaces.

NOTES: None.

## `src/AGC/TCStrings.rgs`

PURPOSE: This file is a Windows Registry Script that defines the class information for a COM object named 'Strings collection'. It specifies the CLSID, ProgID, and other properties required to register this object in the Windows Registry.

KEY PARTS:
- TCObj.Strings.1: Represents the versioned interface of the 'Strings collection' COM object.
- CLSID {8962E245-CD81-11d2-9646-00C04F68DEB0}: Unique identifier for the COM object.
- ProgID TCObj.Strings.1: Programmatic Identifier for the versioned interface.
- VersionIndependentProgID TCObj.Strings: Programmatic Identifier for the version-independent interface.
- InprocServer32 %MODULE%: Specifies the DLL file that contains the implementation of the COM object.
- ThreadingModel Both: Indicates that the COM object can be used in both single-threaded and multi-threaded apartments.

CONNECTS TO: Windows Registry

NOTES: None.

## `src/AGC/TCUtility.cpp`

PURPOSE: This file contains the implementation of the CTCUtility class, which provides utility methods for creating objects, managing object references, sleeping, and retrieving objects by moniker.

KEY PARTS:
- CTCUtility: Implements the ITCUtility interface with methods to create objects, manage object references, sleep, and retrieve objects.
- CreateObject: Delegates object creation to TCUtilImpl::CreateObject.
- ObjectReference: Delegates object reference management to TCUtilImpl::ObjectReference.
- Sleep: Delegates sleeping functionality to TCUtilImpl::Sleep.
- GetObject: Delegates retrieving objects by moniker to TCUtilImpl::GetObject.

CONNECTS TO: TCUtilImpl.cpp (depends on methods in this class for actual implementation).

NOTES: None.

## `src/AGC/TCUtility.h`

PURPOSE: The file defines a COM component class `CTCUtility` that implements the `ITCUtility` interface for managing objects and performing utility functions in the Allegiance game.

KEY PARTS:
- CTCUtility: Implements the `ITCUtility` interface, providing methods to create objects, get object references, sleep, and retrieve objects by moniker.
- CreateObject: Creates an object using a ProgID and computer name.
- ObjectReference: Retrieves a reference to an object in string format.
- Sleep: Pauses execution for a specified duration in milliseconds.
- GetObject: Retrieves an object by its moniker with optional UI interaction and timeout.

CONNECTS TO: AGC.h, objsafe.h

NOTES: None.

## `src/AGC/TCUtility.rgs`

PURPOSE: This file is an RGS (Registry Script) file used to register a COM object for the Utility component in Allegiance.

KEY PARTS:
- TCUtil: The top-level key for the Utility object.
- CLSID: Contains the globally unique identifier (GUID) for the Utility object.
- ProgID: Specifies the programmatic identifier for the Utility object.
- VersionIndependentProgID: Provides a version-independent way to access the Utility object.
- InprocServer32: Points to the DLL that implements the Utility object, and specifies threading model as 'Both'.

CONNECTS TO: Allegiance game engine, specifically the component system.

NOTES: None.

## `src/AGC/VerInfo.js`

PURPOSE: This file is a script written in JavaScript for Windows Script Host (WSH) that retrieves and displays version information about a specified executable file. It uses an ActiveX object to access the version details.

KEY PARTS:
- Name: AGCVersionInfo - An ActiveX object used to retrieve version information.
- Name: WScript.Arguments - Used to get command-line arguments, specifically the file name of the executable whose version information is to be retrieved.
- Name: objVerInfo.FileName - Property set to specify the file name for which version information is requested.
- Name: objVerInfo.FileVersionString - Property that holds the string representation of the file version.
- Name: objVerInfo.ProductVersionString - Property that holds the string representation of the product version.
- Name: WScript.Echo - Function used to output the formatted version information to the console.

CONNECTS TO: AGC.AGCVersionInfo (ActiveX object).

NOTES: None.

## `src/AGC/XMLXForm.js`

PURPOSE: This file is a script written in JavaScript for Windows Script Host (WSH) that transforms an XML file using an XSLT stylesheet and saves the output to a new file. It also adds metadata about the transformation process to the root element of the XML document.

KEY PARTS:
- Name: `xmlxform`
  - What it does: Transforms an XML file using an XSLT stylesheet and outputs the result to a new file, adding metadata to the XML.
- Name: `WScript.Arguments`
  - What it does: Retrieves command-line arguments passed to the script.
- Name: `ActiveXObject("microsoft.xmldom")`
  - What it does: Creates an ActiveX object for handling XML documents.
- Name: `doc.load()`
  - What it does: Loads an XML file into the document object.
- Name: `sheet.load()`
  - What it does: Loads an XSLT stylesheet into the sheet object.
- Name: `fso.CreateTextFile()`
  - What it does: Creates a text file for writing output.
- Name: `doc.transformNode(sheet)`
  - What it does: Applies the XSLT transformation to the XML document.

CONNECTS TO:
- Other systems or files it depends on or serves: None, operates locally with no external dependencies.

NOTES:
- none

## `src/AGC/agcevent.cpp`

**PURPOSE:** The `agcevent.cpp` file in Allegiance contains the implementation of the `CAGCEvent` class, which manages event objects with properties and provides methods for serialization and deserialization.

**KEY PARTS:**
- **Class: CAGCEvent**
  - Manages event objects with properties.
  
- **Function: FinalConstruct()**
  - Initializes tracing if enabled during object construction.
  
- **Function: Init(AGCEventID idEvent, LPCSTR pszContext, LPCOLESTR pszSubject, long idSubject, long cArgTriplets, va_list argptr)**
  - Initializes the event with an ID, context, subject, and variable arguments representing properties.
  
- **Function: AddToDictionary(long cArgTriplets, ...)**
  - Adds properties to the event's property map from a list of argument triplets.
  
- **Function: WriteStringToStream(IStream* pStm, BSTR bstr)**
  - Writes a BSTR (Unicode string) to an IStream as ANSI characters.
  
- **Function: ReadStringFromStream(IStream* pStm, BSTR* pbstr, BYTE bIndicator)**
  - Reads an ANSI string from an IStream and converts it back to a BSTR.
  
- **Function: SaveToStream(IStream* pStm, BOOL fClearDirty)**
  - Saves the event's state to an IStream in a custom format.
  
- **Function: LoadFromStream(IStream* pStm)**
  - Loads the event's state from an IStream.
  
- **Function: AddStaticFields()**
  - Adds static fields to the event object during initialization and persistence operations.
  
- **Function: InterfaceSupportsErrorInfo(REFIID riid)**
  - Checks if the interface supports error information for a given interface ID.
  
- **Function: GetClassID(CLSID* pClassID)**
  - Retrieves the class ID of the event object, essential for COM object identification and registration.
  
- **Function: IsDirty()**
  - Indicates that the object's data has been modified since it was last saved.
  
- **Function: Load(LPSTREAM pStm)**
  - Loads event data from a stream, including properties, computer name, subject details, and context.
  
- **Function: Save(LPSTREAM pStm, BOOL fClearDirty)**
  - Saves the current state of the event object to a stream. It also clears the dirty flag if requested.
  
- **Function: GetSizeMax(ULARGE_INTEGER* pCbSize)**
  - Calculates and returns the maximum size of the data that can be written to the stream during serialization.
  
- **Function: InitNew()**
  - Initializes a new event object with default values, such as setting the ID to EventID_Unknown and the time to the current date and time.
  
- **Function: get_Description(BSTR* pbstrDisplayString)**
  - Retrieves a description of the event using a delegate method from CAGCEventDef class.
  
- **Function: SaveToString(BSTR* pbstr)**
  - Encodes the event object into a string format that can be stored or transmitted.
  
- **Function: LoadFromString(BSTR bstr)**
  - Decodes and loads an event object from a string representation.

**CONNECTS TO:** Other systems or files it depends on or serves, one line. None.

**NOTES:** The use of `XLock` for thread synchronization, conditional compilation for tracing (`CAGCEvent_TRACE_CONSTRUCTION`), handling BSTR conversion between ANSI and Unicode using `WideCharToMultiByte` and `MultiByteToWideChar`, and custom serialization logic for the event properties.

## `src/AGC/agcevent.h`

PURPOSE: This file defines a COM object class `CAGCEvent` that represents an event in the Allegiance game. It handles properties, serialization, and provides methods to manipulate and access event data.

KEY PARTS:
- CAGCEvent: Main class implementing interfaces for managing event data.
- DECLARE_REGISTRY_RESOURCEID: Registers the class with a resource ID.
- Init: Initializes the event with various parameters.
- AddToDictionary: Adds properties to the event's dictionary.
- SaveToStream, LoadFromStream: Methods for saving and loading event data to/from streams.
- IAGCEvent, IAGCEventCreate, IPersistStreamInit: Interfaces implemented by `CAGCEvent`.

CONNECTS TO: AGCLib library, other COM objects.

NOTES: None.

## `src/AGC/agcevent.rgs`

PURPOSE: This file is a Windows registry script that defines the class information for an ActiveX control named AGCEvent.

KEY PARTS:
- Name: AGC.AGCEvent.1
  - Description: The ProgID for the AGCEvent class, version 1.
- CLSID: {AA18A676-14A7-11d3-8B5F-00C04F681633}
  - Description: The unique identifier for the AGCEvent class.
- InprocServer32: %MODULE%
  - Description: Specifies the DLL file that implements the ActiveX control.

CONNECTS TO:
- agcevent.dll (indicated by %MODULE%)

NOTES:
- None.

## `src/AGC/dlldatax.c`

PURPOSE: This file is a wrapper around the standard `dlldata.c` file used for managing proxy stub DLL registration and class object retrieval in Allegiance.

KEY PARTS:
- `REGISTER_PROXY_DLL`: Macro defined to enable proxy stub DLL registration functions.
- `USE_STUBLESS_PROXY`: Macro indicating the use of stubless proxies.
- `DllMain`, `DllRegisterServer`, `DllUnregisterServer`, `DllGetClassObject`, `DllCanUnloadNow`: Renamed to `PrxDllMain`, `PrxDllRegisterServer`, etc., to avoid conflicts with the original `dlldata.c` functions.
- `AGCIDL_p.c`: Included for proxy stub implementation.

CONNECTS TO: `dlldata.c`, `AGCIDL_p.c`

NOTES: None.

## `src/AGC/dlldatax.h`

PURPOSE: This header file is part of a proxy stub implementation for a DLL in the Allegiance game. It defines the necessary functions and macros to support COM (Component Object Model) operations such as registration, unregistration, class object retrieval, and main DLL entry point.

KEY PARTS:
- `PrxDllMain`: The main entry point function for the DLL.
- `PrxDllCanUnloadNow`: Determines if the DLL can be unloaded from memory.
- `PrxDllGetClassObject`: Retrieves a class factory for a specified CLSID.
- `PrxDllRegisterServer`: Registers the DLL with COM.
- `PrxDllUnregisterServer`: Unregisters the DLL from COM.

CONNECTS TO: This file is used by the Allegiance game to support COM-based functionality within its DLLs.

NOTES: None.

## `src/AGC/gameparamdata.h`

PURPOSE: The file `gameparamdata.h` defines structures for additional game parameter data that extends the base mission parameters.

KEY PARTS:
- CAdditionalAGCParamData: Holds extra game parameter data specific to Allegiance, including altered and new settings for technology trees and team names.
- CGameParamData: Inherits from both MissionParams and CAdditionalAGCParamData, providing a comprehensive set of game parameters.

CONNECTS TO: MissionParams

NOTES: None

## `src/AGC/guid.cpp`

PURPOSE: tiny file (2 lines), shown in full rather than summarized.

    #include "AGCIDL_i.c"
    #include "AGC_i.c"

## `src/AGC/pch.cpp`

PURPOSE: tiny file (13 lines), shown in full rather than summarized.

    /////////////////////////////////////////////////////////////////////////////
    // pch.cpp : source file that includes just the standard includes
    //  pch.pch will be the pre-compiled header
    //  pch.obj will contain the pre-compiled type information
    #include "pch.h"
    //#include <atlimpl.cpp>
    #include <TCAtl.cpp>
    #include <..\TCAtl\PropertyClass.cpp>
    #include <..\TCAtl\TCNullStreamImpl.cpp>
    #ifdef _ATL_STATIC_REGISTRY
      #include <statreg.h>
     // #include <statreg.cpp> Imago removed (again)
    #endif

## `src/AGC/pch.h`

PURPOSE: This file is a precompiled header for the Allegiance game, providing common includes and macros used throughout the project.

KEY PARTS:
- `AGCObjectSafetyImpl`: A template class implementing IObjectSafety interface to manage object safety settings.
- `_Module`: An instance of CComModule used for COM module management.
- `TCLib.h`, `TCAtl.h`: Header files from a custom library (likely for game-specific utilities).

CONNECTS TO: Other source files and headers that include this precompiled header.

NOTES:
- Disables warnings related to identifier truncation and decorated name length.
- Defines macros for module identification and COM interface entry.

## `src/AGC/resource.h`

PURPOSE: This file contains resource definitions used by the Allegiance game, including string identifiers and resource IDs for various UI elements, game objects, and other resources.

KEY PARTS:
- IDS_PROJNAME: Project name identifier.
- IDR_AGCGlobal: Global resource ID.
- IDR_AGCHullType: Resource ID for hull types.
- IDR_AGCCommand: Command-related resource ID.
- IDR_AGCAsteroid: Asteroid-related resource ID.
- IDR_AGCOrientation: Orientation-related resource ID.
- IDR_AGCVersionInfo: Version information resource ID.
- IDR_TCStrings: String table resource ID.

CONNECTS TO: AGC.rc (Resource Compiler file).

NOTES: None.
