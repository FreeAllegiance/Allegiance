# src/Pigs/PigSrv

[Back to the index](../LLAMA_SUMMARY.md) - 76 files, 14,336 lines. Notes by a local model, to orient by: not verified facts.

This folder contains source code related to the Allegiance game's PigSrv component, which manages game objects and behaviors in the client-server architecture of the game.

- Pig.cpp: Implements the CPig class, central to managing game objects and behaviors.
- PigEngine.cpp: Manages the overall state and behavior of the game server, handling script loading and session management.
- PigBehaviorScriptType.cpp: Manages behavior scripts for pigs, including loading, unloading, and processing XML-based scripts.
- PigShip.cpp: Handles the behavior and actions of ships controlled by players, including initialization and command execution.
- PigSession.cpp: Manages sessions in the game server, handling initialization, event management, and session-related operations.

Largest files: Pig.cpp (3197 lines), PigShip.cpp (1212 lines), PigEngine.cpp (1148 lines), PigBehaviorScriptType.cpp (816 lines), PigBehaviorScript.cpp (596 lines).

## `src/Pigs/PigSrv/BeeperJS.pig`

PURPOSE: This file contains a JScript behavior script for a pig in the Allegiance game. The script defines the actions of the pig when it enters the PigState_NonExistant state.

KEY PARTS:
- OnStateNonExistant(eStatePrevious): Handles the transition into the PigState_NonExistant state.
- Beep(frequency, duration): Plays a beep sound with the specified frequency and duration.
- CreateTimer(interval, command): Creates a timer that executes the given command after the specified interval.

CONNECTS TO: None

NOTES: none

## `src/Pigs/PigSrv/BeeperVB.pig`

PURPOSE: This file contains a VBScript behavior script for a pig in the Allegiance game. The script defines the actions of the pig during its lifetime.

KEY PARTS:
- Name: OnStateNonExistant - Handles the initial state transition and sets up timers.
- Name: OnKillPig - Kills the ticker object, beeps, and shuts down the pig.
- Name: nLifeTime - Global variable defining the pig's lifetime in seconds.
- Name: objTicker - Global Ticker object used for periodic actions.

CONNECTS TO: Allegiance game engine (ActivePigs objects).

NOTES: None.

## `src/Pigs/PigSrv/Defend.pig`

PURPOSE: This file contains a JScript behavior script for the Allegiance game's Pig AI, defining its actions based on different states such as logging in, joining missions, buying hulls, and launching into space.

KEY PARTS:
- OnStateNonExistant(eStatePrevious): Handles the initial state transition by logging onto the mission server.
- OnStateMissionList(eStatePrevious): Joins or creates a mission for the pig.
- OnStateTeamList(eStatePrevious): Joins a random team if not already joining one.
- OnStateDocked(ePrev): Buys a hull and launches into space when docked.
- OnStateFlying(eStatePrevious): Calls the Defend function with "Outpost" as an argument.

CONNECTS TO: Pig, Host, ActivePigs, HullTypes, Ship

NOTES: None.

## `src/Pigs/PigSrv/IActiveScriptSiteImpl.h`

PURPOSE: This file defines a template class `IActiveScriptSiteImpl` that implements the `IActiveScriptSite` interface for managing script execution in Allegiance.

KEY PARTS:
- Name: `IActiveScriptSiteImpl`
  - Implements methods of the `IActiveScriptSite` interface to manage script execution.
- Name: `SetScriptFileName`
  - Sets and retrieves the script file name and short name.
- Name: `GetItemInfo`
  - Returns an error indicating that item information is not found.
- Name: `OnScriptError`
  - Handles script errors by triggering an event with detailed error information.

CONNECTS TO:
- `AGC.h`, `activscp.h`, `TCLib.h`, and other Allegiance headers for necessary types and functions.

NOTES:
- Contains debug code that is commented out.

## `src/Pigs/PigSrv/MissParms.js`

PURPOSE: tiny file (13 lines), shown in full rather than summarized.

    /////////////////////////////////////////////////////////////////////////////
    // Create the Mission Parameters object
    var objMissParms = new ActiveXObject("Pigs.MissionParams");
    objMissParms.TeamCount      = 2;
    objMissParms.MaxTeamPlayers = 16;
    objMissParms.MinTeamPlayers = 1;
    objMissParms.MapType        = 0;
    objMissParms.Validate();
    // Display the properties
    WScript.Echo("TeamCount      = " + objMissParms.TeamCount      + "\n"
               + "MaxTeamPlayers = " + objMissParms.MaxTeamPlayers + "\n"
               + "MinTeamPlayers = " + objMissParms.MinTeamPlayers + "\n"
               + "MapType        = " + objMissParms.MapType);

## `src/Pigs/PigSrv/Pig.cpp`

PURPOSE: The file `Pig.cpp` contains the implementation of the `CPig` class, which is central to managing game objects and behaviors in Allegiance's Pig game object.

KEY PARTS:
- CPig: Main class for handling game logic and behavior.
- ReceiveSendAndUpdate: Handles receiving, sending, and updating within a timer loop.
- SendChat: Sends chat messages to specified targets or recipients.
- Stack_get_Count, Stack_get__NewEnum, Stack_Push, Stack_Pop, Stack_get_Top: Manage behaviors in a stack.
- CPig::ThreadCreatePig: Creates a new pig object and initializes it.
- CPig::ThreadProc: Main thread procedure for managing pig behavior and state transitions.
- CPig::ProcessAppMessage: Handles application messages for the Pig class.
- CPig::Logon, Logoff: Manage player login and logout.
- CPig::CreateMission, JoinMission: Handle mission creation and joining.
- CPig::SetSkills: Sets the skills for the ship.

CONNECTS TO:
- Allegiance game engine systems for handling game objects, behaviors, and network communication.

NOTES:
- Unusual code: The use of `reinterpret_cast` for message parameter parsing.

PARTS (in file order):

- lines 1-367
  - CPig: Main class for the Pig game object.
  - XThreadParams: Parameters used between the creator thread and the pig thread.
  - Create: Static method to create a new instance of CPig.
  - FinalConstruct: Called when the object is being constructed.
  - FinalRelease: Called when the object is being destroyed.
  - SetCurrentState: Sets the current state of the Pig game object.
  - GetOink: Returns a random oink sound ID.
  - ReceiveSendAndUpdate: Handles receiving, sending, and updating operations.
- lines 368-736
  - CPig: Main class for handling game logic and behavior.
  - ReceiveSendAndUpdate: Handles receiving, sending, and updating within a timer loop.
  - SendChat: Sends chat messages to specified targets or recipients.
  - Stack_get_Count: Returns the count of behaviors in the stack.
  - Stack_get__NewEnum: Creates an enumerator for iterating through behaviors.
  - Stack_Push: Adds a new behavior to the stack.
  - Stack_Pop: Removes the top behavior from the stack.
  - Stack_get_Top: Retrieves the top behavior from the stack.
- lines 738-1085
  - CPig::ThreadCreatePig: Creates a new pig object and initializes it.
  - CPig::ThreadProc: Main thread procedure for managing pig behavior and state transitions.
  - CPig::HandleThreadMessage: Handles messages sent to the pig thread.
  - IPigAccountDispenser: Interface for dispensing pig accounts.
  - IPigAccount: Interface representing a pig account.
  - CPigBehaviorScript: Class for managing pig behaviors.
  - PigState: Enumerated type representing different states of a pig.
  - wm_TerminateBehavior: Message to terminate a behavior.
- lines 1086-1430
  - CPig::ProcessAppMessage: Handles application messages for the Pig class.
  - CPig::OnChat_AreYouAPig: Processes a chat command to check if the sender is a pig.
  - CPig::OnChat_Evaluate: Evaluates an expression sent via chat and replies with the result.
  - CPig::FindMissionName: Checks if a given mission name exists in the mission map.
  - CPig::CacheMaxShipAngleThresholds: Caches the maximum ship angle threshold from the thresholds array.
  - CPig::CacheMaxShipDistanceThresholds: Caches the maximum ship distance threshold from the thresholds array.
  - TODOs: "imago 10/14" - Indicates a potential todo or comment left by an author named imago on 10/14.
  - Unusual code: The use of reinterpret_cast for message parameter parsing, which can lead to undefined behavior if not handled carefully.
- lines 1431-1791
  - CPig::CacheMaxShipsUpdateLatencyThresholds: Caches the maximum ships update latency thresholds.
  - CPig::SendBytes: Sends bytes to a specified dplay wrapper.
  - CPig::InterfaceSupportsErrorInfo: Checks if an interface supports error information.
  - CPig::OnSessionLost: Handles session loss events.
  - CPig::ModifyShipData: Modifies ship data for the pig pilot type.
  - CPig::OnQuitMission: Handles mission quit events.
  - CPig::GetArtPath: Retrieves the art path.
  - CPig::OnBeginAutoUpdate: Begins auto-update process.
- lines 1792-2151
  - CPig::MessageBox: Handles message boxes for the game.
  - CPig::ChangeStation: Manages changes in ship stations.
  - CPig::ChangeCluster: Handles changes in ship clusters.
  - CPig::TerminateModelEvent: Notifies behaviors when a model is terminated.
  - CPig::DamageShipEvent: Notifies behaviors about ship damage.
  - CPig::KillShipEvent: Notifies behaviors about ship kills.
  - CPig::HitWarpEvent: Notifies behaviors when hitting a warp point.
  - CPig::GameOver: Sets the game state to waiting for mission on game over.
- lines 2153-2517
  - CPig::put_UpdatesPerSecond: Sets the number of updates per second.
  - CPig::get_UpdatesPerSecond: Retrieves the current number of updates per second.
  - CPig::get_HullTypes: Returns a collection of hull types.
  - CPig::get_Money: Gets the player's money balance.
  - CPig::Logon: Logs the player into the game server.
  - CPig::Logoff: Logs the player out of the game server.
  - CPig::CreateMission: Creates a new mission.
  - CPig::JoinMission: Joins an existing mission.
- lines 2518-2871
  - CPig::JoinTeam(BSTR bstrCivName, BSTR bstrTeamOrPlayer): Joins a team or creates one if necessary.
  - CPig::QuitGame(): Quits the current game.
  - CPig::Launch(): Launches the player's ship from docked state.
  - CPig::IsMissionOwner(BOOL bOwner): Checks if the player is the mission owner.
  - CPig::IsTeamLeader(BOOL bLeader): Checks if the player is the team leader.
  - vecAvailableMissions: A vector of available missions with positions.
  - pMissionInfo: Pointer to the selected mission.
  - m_evtJoiningMission: Event for joining a mission.
- lines 2872-3196
  - CPig::SetSkills: Sets the skills for the ship.
  - CPig::Shutdown: Initiates a shutdown during the next message loop iteration.
  - CPig::get_Game: Retrieves the game interface.
  - CPig::get_Me: Retrieves the player's ship interface.
  - CPig::get_ChatTarget: Gets the current chat target.
  - CPig::get_Host: Provides access to the behavior host.
  - CPig::StartCustomGame: Starts a custom game with specified parameters.
  - CPig::StartGame: Initiates a game based on current mission settings.

## `src/Pigs/PigSrv/Pig.h`

PURPOSE: This file defines the CPig class, which is a central component in Allegiance's client-server architecture. It handles various aspects of player interaction and game state management.

KEY PARTS:
- CPig: Main class representing the player entity.
- XChatVerb: Struct for chat command verb keywords.
- ThreadThunk: Static function used for thread creation.
- GetEngine: Static method to access the engine instance.
- HasAnyPerformanceThresholds: Checks if any performance thresholds are set.

CONNECTS TO: Allegiance, CPigBehaviorScript, CPigShip, and other game-related components.

NOTES: None.

## `src/Pigs/PigSrv/Pig.rgs`

PURPOSE: This file is a Windows registry script that defines the class information for a COM object associated with the Pig Player Class in Allegiance.

KEY PARTS:
- Name: Pigs.Pig.1 - The ProgID for the Pig Player Class.
- CLSID: {15219788-D562-11d2-9646-00C04F68DEB0} - The unique identifier for the COM object.
- AppID: {F132B4E3-C6EF-11D2-85C9-00C04F68DEB0} - Links to an application ID that may contain additional configuration information.
- TypeLib: {F132B4E2-C6EF-11D2-85C9-00C04F68DEB0} - Links to a type library that contains the interface definitions for the COM object.

CONNECTS TO: Allegiance game engine, possibly through an application ID or type library.

NOTES: None.

## `src/Pigs/PigSrv/PigBehavior.h`

PURPOSE: tiny file (19 lines), shown in full rather than summarized.

    /////////////////////////////////////////////////////////////////////////////
    // PigBehavior.h : Declaration of the CPigBehavior class
    #pragma once
    #ifndef __PigBehavior_h__
    #define __PigBehavior_h__
    /////////////////////////////////////////////////////////////////////////////
    // CPigBehavior
    //
    // Defines the base functionality of pig behavior classes.
    //
    // Currently, scripted behaviors are the only types supported by the PigSrv
    // object model. So, CPigBehavior has only one class derived from it - the
    // CPigBehaviorScript class.
    //
    class CPigBehavior
    {
    };
    /////////////////////////////////////////////////////////////////////////////
    #endif //__PigBehavior_h__

## `src/Pigs/PigSrv/PigBehaviorHost.cpp`

PURPOSE: This file contains the implementation of the CPigBehaviorHost class, which is responsible for managing and interacting with pig entities in the Allegiance game.

KEY PARTS:
- Name: CPigBehaviorHost
  - Manages behavior and interactions for pig entities.
- Function: TraceOutput
  - Outputs trace messages to the debugger and console.
- Function: Beep
  - Generates a beep sound using the system's audio output.
- Function: CreatePig
  - Creates new pig entities based on specified type and command line arguments.
- Function: Trace
  - Traces text messages, optionally prepending the name of the associated pig entity.
- Function: get_StateName
  - Retrieves the name of a given pig state from the game engine.
- Function: Random
  - Generates a random short integer.

CONNECTS TO:
- Allegiance game engine for creating and managing pig entities and states.

NOTES:
- None.

## `src/Pigs/PigSrv/PigBehaviorHost.h`

PURPOSE:
The file `PigBehaviorHost.h` defines the interface and implementation for a COM object that hosts behavior for pigs in the Allegiance game. It provides methods to interact with pig objects, such as creating new pigs, beeping, tracing output, and getting state names.

KEY PARTS:
- CPigBehaviorHost: Main class implementing IPigBehaviorHost interface.
- Beep: Method to make a beep sound.
- CreatePig: Method to create a new pig object.
- Trace: Method for tracing output.
- get_StateName: Method to get the name of a pig state.
- Random: Method to generate a random number.

CONNECTS TO:
Depends on `CPigEngine`, `IPigBehaviorHost`, and other Allegiance game components.

NOTES:
None.

## `src/Pigs/PigSrv/PigBehaviorHost.rgs`

PURPOSE: This file is an RGS (Registry Script) used to register the Pig Behavior Host class in Windows. It defines the CLSID, ProgID, and other metadata for the class.

KEY PARTS:
- Name: Pigs.BehaviorHost.1
  - Description: Represents the versioned ProgID for the Pig Behavior Host.
- Name: Pigs.BehaviorHost
  - Description: Represents the non-versioned ProgID for the Pig Behavior Host, redirecting to the versioned one.
- Name: CLSID {8962E24C-CD81-11d2-9646-00C04F68DEB0}
  - Description: The unique identifier for the Pig Behavior Host class.
- Name: AppID {F132B4E3-C6EF-11D2-85C9-00C04F68DEB0}
  - Description: Links the CLSID to an application ID, possibly for managing the host's lifecycle or permissions.

CONNECTS TO:
- Allegiance game engine components that utilize the Pig Behavior Host class.

NOTES:
- None.

## `src/Pigs/PigSrv/PigBehaviorSchema.xml`

PURPOSE: tiny file (17 lines), shown in full rather than summarized.

    <Schema name="PigBehavior"
      xmlns="urn:schemas-microsoft-com:xml-data"
      xmlns:dt="urn:schemas-microsoft-com:datatypes">
      <ElementType name="script" content="textOnly" model="open"/>
      <ElementType name="InvokeCommand" content="textOnly"/>
      <ElementType name="registration" content="eltOnly">
        <element type="InvokeCommand" minOccurs="0" maxOccurs="*"/>
      </ElementType>
      <AttributeType name="language" required="yes" dt:type="string"/>
      <AttributeType name="base" required="no" dt:type="string"/>
      <ElementType name="pigbehavior" content="eltOnly">
        <attribute type="language"/>
        <attribute type="base"/>
        <element type="registration" minOccurs="0" maxOccurs="1"/>
        <element type="script" minOccurs="1" maxOccurs="*"/>
      </ElementType>
    </Schema>

## `src/Pigs/PigSrv/PigBehaviorScript.cpp`

PURPOSE: This file contains the implementation of the CPigBehaviorScript class, which manages behavior scripts for pigs in Allegiance. It handles initialization, execution, and cleanup of script engines, as well as event handling.

KEY PARTS:
- CPigBehaviorScript: Manages behavior scripts for pigs.
- Init: Initializes the script with a type, pig, command line, and derived behavior.
- Term: Cleans up resources when the script is terminated.
- Eval: Evaluates a script expression.
- OnExpiration: Handles expiration events.
- OnInterruption: Handles interruption events.
- CreateScriptingEngine: Creates and initializes the scripting engine.
- ShutdownScriptingEngine: Shuts down the scripting engine.
- SetCurrentTimer: Sets the current timer for event handling.

CONNECTS TO: Allegiance game engine, CPigBehaviorType, CPig, CPigTimer, IActiveScriptSite, IPigBehavior, and other related classes and interfaces.

NOTES: None.

## `src/Pigs/PigSrv/PigBehaviorScript.h`

PURPOSE: This header file declares the CPigBehaviorScript class, which represents a behavior script for a pig in the Allegiance game.

KEY PARTS:
- CPigBehaviorScript: Manages behavior scripts for pigs.
- Init: Initializes the behavior script with necessary parameters.
- Term: Terminates the behavior script.
- GetLastScriptError: Retrieves the last script error.
- GetScriptDispatch: Returns the script dispatch interface.
- GetDerived: Gets the derived behavior script.
- GetMostDerived: Finds the most derived behavior script.
- CreateScriptingEngine: Creates a scripting engine for the script.
- ShutdownScriptingEngine: Shuts down the scripting engine.
- ResolveScriptMethod: Resolves a script method by name and parameters.
- SetCurrentTimer: Sets the current timer for the script.

CONNECTS TO: PigBehavior.h, PigEventOwnerImpl.h, IActiveScriptSiteImpl.h

NOTES: None.

## `src/Pigs/PigSrv/PigBehaviorScript.rgs`

PURPOSE: This file is a Windows registry script that defines the class identifier (CLSID) and version information for a COM object used in Allegiance's PigSrv module. It specifies details about the behavior script class, including its ProgID and AppID.

KEY PARTS:
- Name: Pigs.BehaviorScript
  - What it does: Represents the behavior script class for Allegiance.
- CLSID: {29E0A265-CC16-11d2-9646-00C04F68DEB0}
  - What it does: Unique identifier for the COM object.
- ProgID: Pigs.BehaviorScript.1
  - What it does: Programmatic identifier for the behavior script class.
- AppID: {F132B4E3-C6EF-11D2-85C9-00C04F68DEB0}
  - What it does: Identifier for the application associated with the COM object.

CONNECTS TO: Allegiance's PigSrv module and its behavior script functionality.

NOTES: None.

## `src/Pigs/PigSrv/PigBehaviorScriptMethods.cpp`

PURPOSE: This file contains implementation methods for resolving script methods in the Allegiance game's behavior system.

KEY PARTS:
- CPigBehaviorScriptMethods: Class that handles the resolution of script methods.
- ResolveMethod: Function to resolve a method by name and parameter count, returning its DISPID.
- ResolveMethods: Function to resolve all methods listed in the Methods table for a given IDispatch object.

CONNECTS TO: Allegiance game's behavior system and script engine.

NOTES: None.

## `src/Pigs/PigSrv/PigBehaviorScriptMethods.h`

PURPOSE: This file declares and implements methods for behavior scripts in Allegiance, specifically managing method IDs and resolving script methods.

KEY PARTS:
- CPigBehaviorScriptMethods: Class that manages method IDs and resolves script methods.
- EMethod: Enumerates all possible script methods.
- XMethodInfo: Struct holding method name and parameter count.
- GetMethodID: Retrieves the DISPID for a given method.
- ResolveMethod: Resolves a method by name and parameters, returning its DISPID.
- ResolveMethods: Resolves all methods for a given dispatch object.

CONNECTS TO: TCLib, LookupTable.h

NOTES: None.

## `src/Pigs/PigSrv/PigBehaviorScriptType.cpp`

**PURPOSE:** The file `PigBehaviorScriptType.cpp` manages behavior scripts for pigs in Allegiance. It handles loading, unloading, and processing of XML-based behavior scripts.

**KEY PARTS:**
- **Class: CPigBehaviorScriptType**
  - Manages behavior scripts for pigs.
  
- **Function: Reload(const WIN32_FIND_DATA* pffd, tstring strFileName)**
  - Reloads the script from a file.
  
- **Function: Load(const WIN32_FIND_DATA* pffd, tstring strFileName)**
  - Loads and parses the behavior script from a file.
  
- **Function: Unload()**
  - Unloads the current script by shutting down the scripting engine.
  
- **Function: AddBehavior(DWORD dwGITCookie)**
  - Adds a behavior to the collection based on a GIT cookie.
  
- **Function: RemoveBehavior(DWORD dwGITCookie)**
  - Removes a behavior from the collection based on a GIT cookie.
  
- **Function: ParseScriptFile(BSTR bstrFileName, ITCStrings** ppScripts)**
  - Parses the XML script file to extract and process behaviors.
  
- **Function: ProcessRootAttributes(IXMLElement* pRoot, bool bValidate)**
  - Processes attributes of the root element in the XML script.
  
- **Function: AddInvokeCommand(IXMLElement* pCmd)**
  - Adds an invoke command from an XML element to the collection.
  
- **Function: AddInvokeCommand(BSTR bstr)**
  - Validates and adds an invoke command string to the collection.
  
- **Function: ProcessScriptElement(IXMLElement* pScr, ITCStrings* pScripts, bool bValidate)**
  - Processes a script element in the XML file, handling includes and text.
  
- **Function: CreateScriptingEngine(BSTR bstrProgID)**
  - Creates an instance of the specified scripting engine.
  
- **Function: ShutdownScriptingEngine()**
  - Shuts down the scripting engine by releasing resources.

**CONNECTS TO:** Other behavior script-related files and systems within Allegiance, particularly those handling game entities and scripting engines.

**NOTES:** The file includes TODOs for adding support for named objects in the script and retrieving the description of the behavior from the XML file.

## `src/Pigs/PigSrv/PigBehaviorScriptType.h`

PURPOSE: This file defines the CPigBehaviorScriptType class, which represents a behavior script type in Allegiance. It handles scripting and behavior management for game objects.

KEY PARTS:
- Name: CPigBehaviorScriptType
  - Manages behavior scripts for game objects.
- Methods: Load, Reload, Unload, ParseScriptFile, ProcessRootAttributes, AddInvokeCommand, CreateScriptingEngine
  - Handles loading, reloading, unloading, and parsing script files.
- Data Members: m_spInvokeCommands, m_strFileName, m_strName, m_clsidEngine, m_spAs, m_spAsp, m_spAse, m_Behaviors
  - Stores information about the script and behaviors.

CONNECTS TO:
- PigEngine: Provides engine-related functionality.
- IActiveScriptSiteImpl: Implements Active Scripting site methods.

NOTES: None.

## `src/Pigs/PigSrv/PigBehaviorScriptType.rgs`

PURPOSE: This file is a Windows registry script that defines the class information for a COM object used in Allegiance's PigSrv component. It specifies the ProgID, CLSID, and other metadata required to register the behavior script type.

KEY PARTS:
- Name: Pigs.BehaviorScriptType
  - Defines the ProgID for the behavior script type.
- CLSID: {75A3CFA1-D306-11d2-9646-00C04F68DEB0}
  - Unique identifier for the COM class.
- AppID: {F132B4E3-C6EF-11D2-85C9-00C04F68DEB0}
  - Identifier for the application associated with this component.

CONNECTS TO:
- Allegiance's PigSrv component, which uses this COM object to manage behavior scripts.

NOTES: None.

## `src/Pigs/PigSrv/PigBehaviorStack.cpp`

PURPOSE: Implements a behavior stack for pigs in Allegiance, managing a collection of behaviors that can be pushed onto and popped from the stack.

KEY PARTS:
- CPigBehaviorStack: Manages a stack of behaviors for a pig.
- Init: Initializes the behavior stack with a reference to the pig.
- InterfaceSupportsErrorInfo: Checks if the interface supports error information.
- get_Count: Returns the number of behaviors in the stack.
- get__NewEnum: Retrieves an enumerator for the collection of behaviors.
- Push: Adds a new behavior to the top of the stack.
- Pop: Removes and returns the behavior at the top of the stack.
- get_Top: Retrieves the behavior at a specified position from the top of the stack.
- get_Pig: Returns the pig associated with this behavior stack.

CONNECTS TO: CPig, PigBehaviorType, PigBehaviorScriptType

NOTES: None.

## `src/Pigs/PigSrv/PigBehaviorStack.h`

PURPOSE: Defines the CPigBehaviorStack class, which manages a stack of behaviors for a pig in the Allegiance game.

KEY PARTS:
- CPigBehaviorStack: Manages a stack of behaviors for a pig.
- Push: Adds a new behavior to the stack.
- Pop: Removes the top behavior from the stack.
- Top: Retrieves the top behavior from the stack.
- Pig: Gets the associated pig object.

CONNECTS TO: CPig, CPigEngine

NOTES: None.

## `src/Pigs/PigSrv/PigBehaviorStack.rgs`

PURPOSE: This file is a Windows Registry Script (RGS) that defines the class information for the Pig Behavior Stack component in Allegiance.

KEY PARTS:
- Name: Pigs.BehaviorStack.1 and Pigs.BehaviorStack classes
- CLSID: {4E9FF98A-364D-11d3-A51D-00C04F68DEB0}
- ProgID: Pigs.BehaviorStack.1
- VersionIndependentProgID: Pigs.BehaviorStack
- AppID: {F132B4E3-C6EF-11D2-85C9-00C04F68DEB0}
- TypeLib: {F132B4E2-C6EF-11D2-85C9-00C04F68DEB0}

CONNECTS TO: Windows Registry

NOTES: None

## `src/Pigs/PigSrv/PigBehaviorType.h`

PURPOSE: tiny file (19 lines), shown in full rather than summarized.

    /////////////////////////////////////////////////////////////////////////////
    // PigBehaviorType.h : Declaration of the CPigBehaviorType class
    #pragma once
    #ifndef __PigBehaviorType_h__
    #define __PigBehaviorType_h__
    /////////////////////////////////////////////////////////////////////////////
    // CPigBehaviorType
    //
    // Defines the base functionality of pig behavior type classes.
    //
    // Currently, scripted behaviors are the only types supported by the PigSrv
    // object model. So, CPigBehaviorType has only one class derived from it -
    // the CPigBehaviorScriptType class.
    //
    class CPigBehaviorType
    {
    };
    /////////////////////////////////////////////////////////////////////////////
    #endif //__PigBehaviorType_h__

## `src/Pigs/PigSrv/PigDirectPlay.h`

PURPOSE: This file defines a wrapper class `CPigDirectPlay` that provides a thread-safe interface to the `IDirectPlayX` COM interface. It ensures that all calls to the underlying `IDirectPlay4` interface are made within a critical section to prevent race conditions.

KEY PARTS:
- CPigDirectPlay: A class that implements the `IDirectPlayX` interface and wraps calls to an underlying `IDirectPlay4` object, ensuring thread safety.
- GetCS: Returns a static critical section used for locking.
- QueryInterface, AddRef, Release: Implement standard COM interface methods.
- IDirectPlay2 through IDirectPlay4 Methods: Wrapper methods for each method in the `IDirectPlayX` interface, calling the corresponding method on the underlying `IDirectPlay4` object and ensuring thread safety.

CONNECTS TO: `TCLib\AutoCriticalSection.h`, `TCLib\ObjectLock.h`, `messagecore.h`

NOTES: None.

## `src/Pigs/PigSrv/PigEngine.cpp`

**PURPOSE:** The `PigEngine.cpp` file in Allegiance is responsible for managing the overall state and behavior of the game server. It handles script loading, unloading, and management, as well as session initialization and uninitialization.

**KEY PARTS:**
- **Class: CPigEngine** - Manages the engine's state and behavior.
- **Function: Construct()** - Initializes the engine by setting up configurations, creating event loggers, loading registry values, and initializing script directories.
- **Function: Initialize(CPigSession* pSession)** - Initializes a session with the engine.
- **Function: Uninitialize(CPigSession* pSession)** - Uninitializes a session and deletes the engine instance if no references remain.
- **Function: ScriptDirMonitor(HANDLE hevtExit)** - Monitors changes in the script directory, reloading scripts when necessary.
- **Function: EnsureScriptsAreLoaded()** - Ensures all scripts are loaded and monitored for changes.
- **Function: ProcessScriptDirChanges()** - Processes changes in the script directory by loading, reloading, or unloading scripts as needed.
- **Function: LoadScriptFile** - Loads a script file into memory and initializes it.
- **Function: ReloadScriptFile** - Reloads an existing script file when changes are detected.
- **Function: UnloadScriptFile** - Unloads a script file from memory.
- **Function: put_ScriptDir** - Sets the script directory in the registry.
- **Function: get_ScriptDir** - Retrieves the current script directory from the registry.
- **Function: CreatePig** - Creates a new pig instance based on specified behavior type and command line.
- **Function: put_MissionServer** - Sets the mission server in the registry.
- **Function: get_MissionServer** - Retrieves the current mission server from the registry.
- **Function: put_MaxPigs** - Sets the maximum number of pigs allowed in the registry.
- **Function: get_MaxPigs** - Retrieves the current maximum number of pigs from the registry.

**CONNECTS TO:** Other systems or files it depends on or serves, one line. None explicitly mentioned in this snippet.

**NOTES:** The use of `XLock` for synchronization, `CRegKey` for registry handling, and `TCThread::BeginMsgThread` for thread management are common practices in multi-threaded applications. Error handling is consistent using HRESULTs and _com_error.

## `src/Pigs/PigSrv/PigEngine.h`

PURPOSE: The file defines the CPigEngine class, which is a central engine for managing pigs in the Allegiance game. It handles script loading, behavior types, and session management.

KEY PARTS:
- CPigEngine: Manages the overall state of the pig engine, including script loading, behavior types, and sessions.
- XScriptMap and XBehaviorMap: Maps for storing behavior scripts and types.
- ScriptDirMonitor: Monitors changes in the script directory to reload scripts dynamically.
- GetEngine: Singleton method to get the instance of CPigEngine.

CONNECTS TO:
- TCLib library for threading, locking, and string handling.
- PigSrv.h for related interfaces and classes.
- CPigs, CPigBehaviorScriptType, CPigSession, etc., for specific game entities and behaviors.

NOTES: None.

## `src/Pigs/PigSrv/PigEvent.cpp`

PURPOSE: Implements the CPigEvent class, which manages events within a game object.

KEY PARTS:
- CPigEvent: Manages an event with an owner and ensures it has a unique ID.
- Init: Initializes the event by setting its owner and adding itself to the owner's collection.
- Term: Terminates the event by removing itself from the owner's collection and detaching from the owner.

CONNECTS TO: PigEventOwner.cpp

NOTES: None.

## `src/Pigs/PigSrv/PigEvent.h`

PURPOSE: This file defines the CPigEvent class and its template implementation IPigEventImpl, which are used for managing events in Allegiance.

KEY PARTS:
- CPigEvent: Base class for event objects with methods to manage event properties like name, expiration expression, and interruption expression.
- IPigEventImpl: Template class that implements the IPigEvent interface and provides default implementations for methods defined in CPigEvent.
- GetNextEventID: Static method to generate unique event IDs.

CONNECTS TO: PigSrv.h (contains related classes), resource.h (for resource symbols).

NOTES: None.

## `src/Pigs/PigSrv/PigEventOwner.cpp`

PURPOSE: This file contains the implementation of the CPigEventOwner class, which manages a collection of events and handles their lifecycle.

KEY PARTS:
- `GetNextEventNumber`: Generates a unique event number.
- `AddEvent`: Adds an event to the collection and increments its reference count.
- `RemoveEvents`: Terminates all events in the collection.
- `RemoveEvent`: Removes a specific event from the collection and decrements its reference count.
- `PulseEvents`: Processes each event, checking for expiration or interruption, and calls appropriate methods.
- `ModelTerminated`: Handles termination of a model by interrupting associated events.

CONNECTS TO: `CPigEvent`, `ImodelIGC`

NOTES: None.

## `src/Pigs/PigSrv/PigEventOwner.h`

PURPOSE: This file defines the CPigEventOwner class and its implementation template CPigEventOwnerImpl. The class manages a list of events and provides methods to add, remove, and pulse these events.

KEY PARTS:
- CPigEventOwner: Manages a list of events and provides methods to manipulate them.
- CPigEventOwnerImpl: Template class that implements the CPigEventOwner interface with locking mechanisms.
- AddEvent: Adds an event to the list.
- RemoveEvent: Removes a specific event from the list.
- RemoveEvents: Clears all events from the list.
- PulseEvents: Processes and removes expired or interrupted events.
- ModelTerminated: Handles termination of a model associated with events.
- LockT/UnlockT: Virtual methods for locking and unlocking, implemented by derived classes.

CONNECTS TO: CPigEvent, ImodelIGC

NOTES: None

## `src/Pigs/PigSrv/PigHullTypes.cpp`

PURPOSE: The file `PigHullTypes.cpp` implements the `CPigHullTypes` class, which manages and provides access to various hull types available in the game Allegiance. It handles initialization, enumeration, and retrieval of these hull types based on station capabilities.

KEY PARTS:
- `CPigHullTypes`: Manages a collection of hull types.
- `Init`: Initializes the collection with hull types that can be bought by a given station.
- `get_Count`: Returns the number of hull types in the collection.
- `get__NewEnum`: Provides an enumerator for iterating over the collection.
- `get_Item`: Retrieves a specific hull type by index.

CONNECTS TO: Depends on interfaces and classes such as `IhullTypeIGC`, `IstationIGC`, `IAGCHullType`, and others from Allegiance's internal architecture.

NOTES: None.

## `src/Pigs/PigSrv/PigHullTypes.h`

PURPOSE: This file defines a COM object class `CPigHullTypes` that implements the `IPigHullTypes` interface for managing hull types in the Allegiance game.

KEY PARTS:
- CPigHullTypes: Main class implementing IPigHullTypes, ISupportErrorInfo, ITCCollection interfaces.
- DECLARE_REGISTRY_RESOURCEID: Registers the COM object with a resource ID.
- DECLARE_PROTECT_FINAL_CONSTRUCT: Ensures the constructor is protected and final.
- BEGIN_CATEGORY_MAP: Maps the category to PigObjects.
- BEGIN_COM_MAP: Maps COM interfaces to methods.
- Init: Initializes the hull types with provided lists and station information.
- InterfaceSupportsErrorInfo: Implements error support for COM interfaces.
- get_Count, get__NewEnum: Implement ITCCollection interface methods for counting and enumerating items.
- get_Item: Retrieves a specific hull type by index.

CONNECTS TO:
- Allegiance.h: Core Allegiance game headers.
- IPigHullTypes.h: Interface definitions for hull types.
- ITCCollection.h: Collection interface implementation.
- IhullTypeIGC.h: Interface for hull type data.

NOTES: None.

## `src/Pigs/PigSrv/PigHullTypes.rgs`

PURPOSE: This file is a Windows Registry Script that defines the class information for a COM object used in Allegiance to manage different types of ship hulls.

KEY PARTS:
- Name: Pigs.HullTypes.1
  - Description: Represents the specific version of the Pig Hull Type Collection Class.
  
- Name: Pigs.HullTypes
  - Description: Provides a default ProgID for accessing the Pig Hull Type Collection Class.
  
- CLSID: {E1A2D508-F8FD-11d2-A50F-00C04F68DEB0}
  - Description: Unique identifier for the COM object.

- AppID: {F132B4E3-C6EF-11D2-85C9-00C04F68DEB0}
  - Description: Links to an application-specific settings file or code.

- TypeLib: {F132B4E2-C6EF-11D2-85C9-00C04F68DEB0}
  - Description: Specifies the type library associated with the COM object.

CONNECTS TO:
- Allegiance game engine, specifically for managing ship hull types.

NOTES:
- None.

## `src/Pigs/PigSrv/PigMacro.h`

PURPOSE: This file contains macros used to simplify the implementation of scripted behavior methods in the ActivePigs project.

KEY PARTS:
- CPigBehaviorScript_MethodImpl_0 to CPigBehaviorScript_MethodImpl_6R: Macros for implementing methods with 0 to 6 parameters and returning a value.
- TCInvokeArgs: A class used for invoking script methods with arguments.
- m_Methods: An object that manages method IDs.
- m_spdScript: A pointer to the script object.
- m_spBehaviorBase: A pointer to a base behavior object.

CONNECTS TO: TCLib\InvokeArgs.h

NOTES: None.

## `src/Pigs/PigSrv/PigObject.cpp`

PURPOSE: This file contains the implementation of the CPig$$Object$$ class, which is part of the Allegiance game's object management system.

KEY PARTS:
- CPig$$Object$$: The main class representing an object in the game.
- FinalConstruct(): Initializes the object after construction.
- FinalRelease(): Cleans up resources before the object is destroyed.
- InterfaceSupportsErrorInfo(REFIID riid): Checks if the object supports error information for a given interface.

CONNECTS TO: src/Pigs/PigSrv/PigObject.h, TCLib.h, TCAtl\ObjectMap.h

NOTES: None.

## `src/Pigs/PigSrv/PigObject.h`

PURPOSE: This file declares and implements the CPig$$Object$$ class, which is a COM object representing an object in the Allegiance game engine.

KEY PARTS:
- CPig$$Object$$: The main class that implements the IPig$$Object$$ interface.
- GetEngine(): Static method to retrieve the global instance of CPigEngine.
- DECLARE_REGISTRY_RESOURCEID, DECLARE_PROTECT_FINAL_CONSTRUCT, BEGIN_CATEGORY_MAP, END_CATEGORY_MAP, BEGIN_COM_MAP, END_COM_MAP: Macros for COM registration and interface mapping.
- ISupportErrorInfo Interface Methods: Implementation of error support.
- IPig$$Object$$ Interface Methods: Placeholder for methods related to the Pig object.

CONNECTS TO: CPigEngine (depends on it)

NOTES: None.

## `src/Pigs/PigSrv/PigObject.rgs`

PURPOSE: This file is a Windows Registry Script (RGS) that defines the class information for a Pig object in the Allegiance game. It specifies the CLSID, ProgID, and other metadata needed to register the object within the Windows environment.

KEY PARTS:
- Pigs.$$Object$$.1: Version-specific registry entry for the Pig object.
- CLSID: Unique identifier for the Pig object class.
- CurVer: Points to the current version of the Pig object class.
- ProgID: Programmatic identifier used by applications to reference the Pig object.
- AppID: Identifier for the application that uses this object.
- TypeLib: Reference to the type library associated with the Pig object.

CONNECTS TO: Windows Registry and Allegiance game codebase.

NOTES: None.

## `src/Pigs/PigSrv/PigSession.cpp`

PURPOSE: Implements the CPigSession class, which manages a session in the Allegiance game. It handles initialization, event management, and various session-related operations.

KEY PARTS:
- CPigSession: Manages the session lifecycle and delegates most functionality to the engine.
- FinalConstruct: Initializes the session and creates an event sink.
- FinalRelease: Cleans up resources when the session is destroyed.
- IPigSession Interface Methods: Provides methods for setting and getting session properties, creating pigs, and managing events.

CONNECTS TO:
- PigEngine: The core game engine that CPigSession interacts with to perform most operations.
- CPigSessionEventSink: Manages event handling within the session.

NOTES:
- Contains TODOs for refactoring and lazy initialization of components.
- Uses macros like _TRACE_BEGIN, _TRACE_PART2, and _TRACE_END for debugging output.

## `src/Pigs/PigSrv/PigSession.h`

PURPOSE: This file defines the CPigSession class, which represents a session in the Allegiance game server. It provides methods for managing sessions, behaviors, and other related functionalities.

KEY PARTS:
- CPigSession: Main class representing a session.
- IPigSession: Interface defining methods for session management.
- GetEngine(): Static method to retrieve the engine instance.
- put_SessionInfo(), get_SessionInfo(): Methods for setting and getting session information.
- CreatePig(): Method to create a new pig in the session.

CONNECTS TO:
- PigEngine: The game engine that CPigSession interacts with.

NOTES:
- None.

## `src/Pigs/PigSrv/PigSession.rgs`

PURPOSE: This file is a Windows Registry Script that defines the class information and versioning for the Pig Session Class in Allegiance.

KEY PARTS:
- Name: Pig Session Class
- CLSID: {9BCD384E-C6F2-11D2-85C9-00C04F68DEB0}
- ProgID: Pigs.Session.1
- VersionIndependentProgID: Pigs.Session
- LocalServer32: %MODULE%
- AppID: {F132B4E3-C6EF-11D2-85C9-00C04F68DEB0}
- TypeLib: {F132B4E2-C6EF-11D2-85C9-00C04F68DEB0}

CONNECTS TO: Windows Registry

NOTES: None

## `src/Pigs/PigSrv/PigSessionEventSink.cpp`

**PURPOSE:** This file contains the implementation of the `CPigSessionEventSink` class, which is responsible for handling events triggered by the Allegiance game session.

**KEY PARTS:**
- **Class: CPigSessionEventSink**
  - Manages event sinks for the game session.
- **Function: Init(CPigSession* pSession)**
  - Initializes the event sink with a reference to the owning `CPigSession` object and registers itself with the Global Interface Table (GIT).
- **Function: Term()**
  - Cleans up by revoking all registered events and unregistering from the GIT.
- **Function: OnEventTriggered(IAGCEvent* pEvent)**
  - Handles event triggers by forwarding them to the owning session's `Fire_OnEvent` method.

**CONNECTS TO:** 
- `CPigSession.h`, `PigSession.cpp`
- `IAGCEventSink`
- `GetAGCGlobal()`

**NOTES:**
- Trace macros are used for debugging, which can be enabled or disabled with preprocessor directives.
- The class includes assertions to ensure proper initialization and cleanup.

## `src/Pigs/PigSrv/PigSessionEventSink.h`

PURPOSE: This file defines a COM object `CPigSessionEventSink` that implements the `IAGCEventSink` interface to handle events related to a session in Allegiance.

KEY PARTS:
- CPigSessionEventSink: A COM object implementing IAGCEventSink to manage session events.
- Init: Initializes the event sink with a pointer to a CPigSession instance.
- Term: Terminates the event sink.
- OnEventTriggered: Handles triggered events by forwarding them to the associated session.
- GetGITCookie: Returns a GIT cookie, likely for internal tracking or synchronization purposes.

CONNECTS TO: CPigSession (the session it manages)

NOTES: None.

## `src/Pigs/PigSrv/PigShip.cpp`

**PURPOSE:** The `CPigShip.cpp` file in Allegiance is responsible for managing the behavior and actions of ships controlled by players. It handles initialization, command execution, autopilot functionality, and various ship-related operations.

**KEY PARTS:**
- **Class: CPigShip**: Represents a ship controlled by a player.
- **Function: FinalConstruct()**: Initializes the object after construction.
- **Function: Init(CPig* pPig)**: Initializes the ship with a reference to the player.
- **Function: SetIGC(IshipIGC* pShipIGC)**: Sets the IGC pointer for the ship and manages AGC object mapping.
- **Function: OnNewChatMessage()**: Handles new chat messages, checking if they contain commands.
- **Function: KillAutoAction()**: Resets the auto-pilot flag and disconnects any current action event object.
- **Function: CreateCommandWrapper(Command cmd, IAGCCommand** ppCommand)**: Creates a command wrapper for a given command ID.
- **Function: ReplaceAutoAction(IPigShipEvent* pAutoAction)**: Replaces the current auto-action with a new one.
- **Function: FindCluster(BSTR bstrSector)**: Finds a cluster by sector name.
- **Function: FindModel(BSTR bstrModel)**: Finds a model by model name within clusters.
- **Function: SellAllParts()**: Sells all parts of the ship and creates a new empty ship with the same hull type.
- **Function: BuyHull(IAGCHullType* pHullType)**: Buys a new hull for the ship, selling all current parts first.
- **Function: CommitSuicide(BSTR* pbstrResponse)**: Initiates suicide of the ship if connected to the game.

**CONNECTS TO:** Other systems and files related to player control, game objects, and command handling.

**NOTES:** None.

## `src/Pigs/PigSrv/PigShip.h`

PURPOSE: The file defines the CPigShip class, which represents a ship controlled by a player in the Allegiance game. It implements interfaces for ship control and management.

KEY PARTS:
- CPigShip: Main class representing a Pig ship.
- DECLARE_REGISTRY_RESOURCEID: Registers the resource ID for the class.
- DECLARE_PROTECT_FINAL_CONSTRUCT: Ensures the constructor is protected and final.
- DECLARE_AGC_TYPE(pig): Declares the AGC type as "pig".
- BEGIN_CATEGORY_MAP: Maps the category to PigObjects.
- BEGIN_COM_MAP: Maps the COM interfaces including IPigShip, IAGCShipImpl, and ISupportErrorInfo.
- CPigShip(): Constructor for the CPigShip class.
- FinalConstruct() and FinalRelease(): Lifecycle methods.
- Init(CPig* pPig): Initializes the ship with a Pig object.
- GetPig() const: Returns the associated Pig object.
- SetIGC(IshipIGC* pShipIGC): Sets the IshipIGC interface pointer.
- OnNewChatMessage(): Handles new chat messages.
- KillAutoAction(): Stops any auto-action.

CONNECTS TO: Allegiance game engine, IPigShip interface, IAGCShipImpl, ISupportErrorInfo, and other ship-related classes.

NOTES: None.

## `src/Pigs/PigSrv/PigShip.rgs`

PURPOSE: This file is a Windows Registry Script that defines the class information for a Pig Ship in Allegiance. It registers the CLSID and ProgID for the ship type.

KEY PARTS:
- Pigs.Ship.1: The version-specific programmatic identifier for the Pig Ship.
- CLSID: Unique identifier for the Pig Ship class.
- CurVer: Points to the current version of the Pig Ship class.
- AppID: Identifier for the application associated with the Pig Ship class.
- TypeLib: Reference to the type library for the Pig Ship class.

CONNECTS TO: Allegiance game engine, specifically related to ship objects and their registration in the Windows Registry.

NOTES: None.

## `src/Pigs/PigSrv/PigShipEvent.cpp`

PURPOSE: This file contains implementation details for event handling related to ships in the Allegiance game. It includes classes and functions for managing ship events such as setting targets and determining when events are expired.

KEY PARTS:
- CPigShipEvent: Manages events associated with ships, including setting targets and handling expiration.
- CPigFaceEvent: A specific type of event where a ship faces a target or vector.
- FindTargetName: Searches for models in the current cluster based on a given name.
- Pulse: Handles the expiration of an event by killing the associated ship action if expired.
- ModelTerminated: Resets the target when the model is terminated.

CONNECTS TO: Allegiance game engine, specifically related to ship behavior and events.

NOTES: None.

## `src/Pigs/PigSrv/PigShipEvent.h`

PURPOSE: This file defines the interface and implementation for various types of events related to ships in the Allegiance game. It includes classes for managing ship actions like facing targets, executing drone commands, and other event-driven behaviors.

KEY PARTS:
- CPigShipEvent: Base class for ship-related events, implementing IPigShipEvent and IPigEvent interfaces.
- CPigFaceEvent: Handles events where a ship needs to face a target.
- CPigDroneEvent: Manages events involving drone actions like attacking or moving towards targets.
- CPigShipDummyEvent: A dummy implementation of IPigShipEvent for testing purposes.

CONNECTS TO: Allegiance game engine, other event handling systems within the game.

NOTES: None.

## `src/Pigs/PigSrv/PigShipEvent.rgs`

PURPOSE: This file is a Windows Registry Script (.rgs) that defines the class information for the "Pig Ship Event Class" in Allegiance. It specifies the CLSID, ProgID, and other properties necessary for COM (Component Object Model) registration.

KEY PARTS:
- Name: Pigs.ShipEvent.1
  - Description: Represents a specific version of the Pig Ship Event class.
  
- Name: Pigs.ShipEvent
  - Description: The default ProgID for the Pig Ship Event class, pointing to the latest version (Pigs.ShipEvent.1).
  
- CLSID: {4F16D144-EF2F-11d2-A50D-00C04F68DEB0}
  - Description: Unique identifier for the Pig Ship Event class.
  
- AppID: {F132B4E3-C6EF-11D2-85C9-00C04F68DEB0}
  - Description: Identifier for the application associated with this class.

- TypeLib: {F132B4E2-C6EF-11D2-85C9-00C04F68DEB0}
  - Description: Reference to the type library containing type information for the Pig Ship Event class.

CONNECTS TO:
- Allegiance game engine, specifically related to ship events and COM components.

NOTES:
- None.

## `src/Pigs/PigSrv/PigSrv.cpp`

PURPOSE: This file contains the main entry point and initialization logic for the Allegiance game server component named PigSrv.

KEY PARTS:
- `WinMain`: The primary function that initializes the application, sets up COM, AGC (Assault Game Client) module, and starts the message loop.
- `DoMain`: Handles command-line parsing, security initialization, class object registration, and enters a message loop to process messages.
- `_tmain`: Entry point for the program, calls `WinMain` after performing some initial checks.

CONNECTS TO:
- AGC (Assault Game Client) module
- ATL (Active Template Library)
- Steam API for multiplayer functionality

NOTES:
- Contains conditional code for Windows 9x compatibility.
- Includes logic to initialize and manage Steam API for multiplayer features.
- Uses a message loop to handle events, including Steam callbacks.

## `src/Pigs/PigSrv/PigSrv.h`

PURPOSE: tiny file (8 lines), shown in full rather than summarized.

    #ifndef __PigSrv_h__
    #define __PigSrv_h__
    #pragma once
    /////////////////////////////////////////////////////////////////////////////
    #include <PigsIDL.h>
    #include <PigsLib.h>
    /////////////////////////////////////////////////////////////////////////////
    #endif // !__PigSrv_h__

## `src/Pigs/PigSrv/PigSrv.rgs`

PURPOSE: This file is a Windows registry script for the Allegiance game's Pig Server application. It defines the application ID and event log settings.

KEY PARTS:
- Name: AppID
  - Defines the application ID for the Pig Server.
- EventMessageFile
  - Specifies the path to the message file for event logging.
- TypesSupported
  - Sets the types of events supported by the event log.

CONNECTS TO:
- None

NOTES:
- none

## `src/Pigs/PigSrv/PigStaticSite.h`

PURPOSE: This file contains declarations for custom implementations of `ClusterSite`, `ThingSite`, and `IIgcSite` classes specific to the Allegiance game's Pigs module. These classes extend or override base functionality to provide side visibility features.

KEY PARTS:
- CPigsClusterSite: Extends ClusterSite to manage scanners and their interactions with sides.
- CPigsThingSite: Extends ThingSite to handle side visibility based on scanner ranges.
- CPigStaticSite: Implements IIgcSite, providing custom creation methods for `CPigsThingSite` and `CPigsClusterSite`.

CONNECTS TO: Allegiance game engine libraries (Allegiance.h), FedSrv's implementation, and trekigc's implementation.

NOTES: None.

## `src/Pigs/PigSrv/PigSvrps.def`

PURPOSE: tiny file (8 lines), shown in full rather than summarized.

    LIBRARY      "PigSrvPS"
    DESCRIPTION  'Proxy/Stub DLL'
    EXPORTS
    	DllGetClassObject       @1	PRIVATE
    	DllCanUnloadNow         @2	PRIVATE
    	GetProxyDllInfo         @3	PRIVATE
    	DllRegisterServer		@4	PRIVATE
    	DllUnregisterServer		@5	PRIVATE

## `src/Pigs/PigSrv/PigTimer.cpp`

PURPOSE: This file contains the implementation of the CPigTimer class, which is responsible for managing timers in the Allegiance game. It handles initialization, expiration checks, and pulse events.

KEY PARTS:
- CPigTimer: Manages timer intervals and repetitions.
- Init: Initializes the timer with owner, interval, expression, repetitions, and name.
- IsExpired: Checks if the timer has expired based on its interval and repetition count.
- Pulse: Handles the timer's expiration event, updating the next expiration time and decrementing the repetition count.

CONNECTS TO: IPigEventOwner (the owner of the timer).

NOTES: None.

## `src/Pigs/PigSrv/PigTimer.h`

PURPOSE: This file defines the CPigTimer class, which implements the IPigTimer interface for managing timed events in Allegiance.

KEY PARTS:
- Name: CPigTimer
  - Manages timed events with specified intervals and repetition counts.
- Name: Init
  - Initializes the timer with an owner, interval, event expression, repetitions, and name.
- Name: IsExpired
  - Checks if the timer has expired based on its current state.
- Name: Pulse
  - Handles the pulse of the timer, potentially triggering events or resetting the timer.
- Name: put_RepetitionCount
  - Sets the repetition count for the timer.
- Name: get_RepetitionCount
  - Retrieves the current repetition count of the timer.
- Name: put_Interval
  - Sets the interval time for the timer in milliseconds.
- Name: get_Interval
  - Retrieves the current interval time of the timer.

CONNECTS TO:
- IPigEventImpl, ISupportErrorInfo, CComObjectRoot, and CComCoClass for COM interface implementation and support.

NOTES: None.

## `src/Pigs/PigSrv/PigTimer.rgs`

PURPOSE: This file is a Windows registry script that registers a COM class for the Pig Timer component in Allegiance.

KEY PARTS:
- Name: Pigs.Timer.1
  - Description: Represents the versioned ProgID for the Pig Timer class.
- CLSID: {75A3CFA0-D306-11d2-9646-00C04F68DEB0}
  - Unique identifier for the COM class.
- CurVer: Pigs.Timer.1
  - Specifies the current version of the ProgID.
- AppID: {F132B4E3-C6EF-11D2-85C9-00C04F68DEB0}
  - Links to an application-specific registry entry.

CONNECTS TO:
- Allegiance game engine, specifically the Pig Timer component.

NOTES:
- None.

## `src/Pigs/PigSrv/Pigs.cpp`

PURPOSE: The file `Pigs.cpp` implements the `CPigs` class, which manages a collection of pigs in the Allegiance game. It provides methods to add, remove, and rename pigs, as well as access them by name.

KEY PARTS:
- CPigs: Manages a collection of pigs using two maps (one for cookies and one for names).
- AddPig: Adds a pig to the collection.
- RemovePig: Removes a pig from the collection.
- RenamePig: Renames an existing pig in the collection.
- get_Count: Returns the number of pigs in the collection.
- get__NewEnum: Enumerates all pigs in the collection.
- get_Item: Retrieves a pig by its name.

CONNECTS TO: `IPigs`, `ITCCollection`, and other game engine components.

NOTES: None.

## `src/Pigs/PigSrv/Pigs.h`

PURPOSE: This file contains the declaration of the CPigs class, which is a COM object implementing the IPigs interface for managing pigs in Allegiance.

KEY PARTS:
- Name: CPigs - Manages pig objects and provides methods to add, remove, rename, and find pigs.
- Exists(_bstr_t bstrName) - Checks if a pig with the specified name exists.
- AddPig(DWORD dwGITCookie, _bstr_t bstrName) - Adds a new pig with the given cookie and name.
- RemovePig(DWORD dwGITCookie) - Removes a pig by its cookie.
- RenamePig(DWORD dwGITCookie, _bstr_t bstrName) - Renames a pig by its cookie.
- Find(_bstr_t bstrName) - Finds the cookie of a pig by its name.
- GetEngine() - Returns a reference to the CPigEngine instance.

CONNECTS TO: PigEngine.h and PigSrv.h

NOTES: None.

## `src/Pigs/PigSrv/Pigs.rgs`

PURPOSE: This file is a registry script used to register the Pigs Collection Class in Windows. It defines the class ID, version information, and other metadata for the class.

KEY PARTS:
- Name: Pigs.Pigs.1
  - Description: Represents the Pigs Collection Class.
- CLSID: {DC2A26D8-DDC0-11d2-A507-00C04F68DEB0}
  - Unique identifier for the class.
- ProgID: s 'Pigs.Pigs.1'
  - Programmatic identifier used to reference the class in code.
- VersionIndependentProgID: s 'Pigs.Pigs'
  - Alternative programmatic identifier that is independent of version.
- AppID: {F132B4E3-C6EF-11D2-85C9-00C04F68DEB0}
  - Identifier for the application associated with this class.

CONNECTS TO:
- Allegiance game engine, specifically related to space combat and strategy elements.

NOTES: None.

## `src/Pigs/PigSrv/PropertyValuePairs.h`

PURPOSE: This file defines a class and function for parsing property-value pairs from a BSTR string.

KEY PARTS:
- CPropertyValuePairs: A map that stores property names as keys and their corresponding values as values.
- ParsePropertyValuePairs: A function that parses a BSTR string containing property-value pairs into the provided CPropertyValuePairs object.

CONNECTS TO: None

NOTES: The function has disabled code for parsing quoted properties, which is indicated by the `#if 0` directive.

## `src/Pigs/PigSrv/RemListPigs.js`

PURPOSE: The file `RemListPigs.js` is a script used to list the pigs (game instances) currently running on a specified computer in the Allegiance game server.

KEY PARTS:
- `main(args)`: Main function that parses command line arguments, connects to the Pig Server, and lists the running pigs.
- `Syntax()`: Function that displays the usage syntax of the script.

CONNECTS TO: 
- `TCUtil`: ActiveX object used for utility functions.
- `Pigs`: ActiveX object representing the Pig Server.

NOTES:
- The commented-out code suggests there was an intention to use session information, but it was not implemented.

## `src/Pigs/PigSrv/SrcInc.cpp`

PURPOSE: tiny file (10 lines), shown in full rather than summarized.

    /////////////////////////////////////////////////////////////////////////////
    // This file should contain the CPP files for all source code-based
    // libraries used in the project. Any corresponding header files for the
    // libraries should be placed in SrcInc.h.
    //
    #include "pch.h"
    #include <TCAtl.cpp>
    #include <..\TCAtl\PropertyClass.cpp>
    #include <..\TCAtl\TCNullStreamImpl.cpp>
    #include <..\TCAtl\UserAcct.cpp>

## `src/Pigs/PigSrv/SrcInc.h`

PURPOSE: tiny file (11 lines), shown in full rather than summarized.

    #pragma once
    /////////////////////////////////////////////////////////////////////////////
    // This file should contain the include files for all source code-based
    // libraries used in the project. Any corresponding CPP files for the
    // libraries should be placed in SrcInc.cpp.
    //
    #include <TCAtl.h>
    #include <..\TCAtl\PropertyClass.h>
    #include <..\TCAtl\TCNullStreamImpl.h>
    #include <..\TCAtl\ObjectMap.h>
    #include <..\TCAtl\UserAcct.h>

## `src/Pigs/PigSrv/Test.js`

PURPOSE: This file is a script written in JavaScript for the Allegiance game that automates the process of logging into an Allegiance server, creating a mission, and joining a team.

KEY PARTS:
- `objSession`: Connects to the Pig Server.
- `objPig`: Represents a single pig (player) with a "Default" behavior.
- `WScript.Echo`: Outputs messages to the console for debugging or logging purposes.
- `objMissionParams`: Configures mission parameters such as team count, map type, etc.

CONNECTS TO: Allegiance server through the Pig Server API.

NOTES: None.

## `src/Pigs/PigSrv/Test.vbs`

PURPOSE: This VBScript file is used to create and configure a session with the Allegiance server, logon to an Allegiance server, and create a new mission.

KEY PARTS:
- objSession: Connects to the Pig Server.
- objPig: Represents a single pig (player) in the game.
- objMissionParams: Contains parameters for creating a new mission.
- CreateObject("Pigs.PigSessionInfo"): Creates an object to hold session information.
- CreateObject("Pigs.MissionParams"): Creates an object to define mission parameters.

CONNECTS TO:
- Allegiance server

NOTES:
- The commented-out code suggests that the script was intended to use session information, but this functionality is not currently enabled.

## `src/Pigs/PigSrv/TestList.js`

PURPOSE: tiny file (17 lines), shown in full rather than summarized.

    // Create the Session Information object
    //var objInfo = new ActiveXObject("Pigs.SessionInfo");
    //objInfo.ApplicationName = WScript.ScriptFullName;
    // Connect to the Pig Server
    var objSession = new ActiveXObject("Pigs.Session");
    //objSession.SessionInfo = objInfo;
    // Create several pigs with the "Default" behavior
    WScript.Echo("Creating several pigs...");
    for (i = 0; i < 3; ++i)
      var objPig = objSession.CreatePig();
    // List the pigs that are currently running
    WScript.Echo("Listing the " + objSession.Pigs.Count + " pigs that are currently running...");
    for (var it = new Enumerator(objSession.Pigs); !it.atEnd(); it.moveNext())
    {
      var objItem = it.item();
      WScript.Echo("Name = \"" + objItem.Name + "\"");
    }

## `src/Pigs/PigSrv/beeper.js`

PURPOSE: This file is a script written in JavaScript for the Allegiance game that connects to a Pig Server and creates a new mission with specific parameters.

KEY PARTS:
- objSession: Connects to the Pig Server.
- objPig: Creates a single pig with a "BeeperVB" behavior.
- Logon: Logs on to an Allegiance server using specified credentials.
- objMissionParams: Defines mission parameters such as team count, player limits, and map type.
- CreateMission: Creates a new mission based on the provided parameters.

CONNECTS TO: Pig Server

NOTES: None

## `src/Pigs/PigSrv/bugman.pig`

PURPOSE: This file contains a JScript behavior script for the PigSrv component of Allegiance, defining the actions and state transitions for a pig in the game.

KEY PARTS:
- OnStateNonExistant: Handles the initial logon to the mission server when the pig is created.
- OnStateMissionList: Joins or creates a mission for the pig.
- OnStateTeamList: Joins a random team if available.
- OnStateDocked: Buys a hull and launches the pig once it becomes docked.
- OnStateFlying: Defends an outpost.

CONNECTS TO: PigSrv, ActivePigs objects (This, Pig, Host), and game state transitions.

NOTES: None.

## `src/Pigs/PigSrv/join.js`

PURPOSE: This file is a script written in JavaScript for the Allegiance game that connects to the Pig Server and performs actions such as logging on to an Allegiance server, joining an existing mission, and joining a team.

KEY PARTS:
- objSession: Connects to the Pig Server.
- objPig: Creates a single pig with a "Default" behavior.
- Logon: Logs on to an Allegiance server using specified credentials.
- JoinMission: Joins an existing mission.
- JoinTeam: Joins a team.

CONNECTS TO: Pig Server

NOTES: None.

## `src/Pigs/PigSrv/manual.pig`

PURPOSE: This file contains a JScript behavior script for the Allegiance game's Piglet AI system, specifically designed to handle manual control of piglets.

KEY PARTS:
- Name: Manual
  - Description: Handles state transitions and debug output for manual control of piglets.
- OnActivate(objDeactivated)
  - Description: Traces when a piglet is activated and logs the previous behavior.
- OnDeactivate(objActivated)
  - Description: Traces when a piglet is deactivated and logs the next behavior.
- DisplayStateTransition(eStatePrevious)
  - Description: Displays state transitions with debug text.
- OnMissionStarted()
  - Description: Handles the 'mission started' notification.

CONNECTS TO:
- Piglet AI system

NOTES:
- None.

## `src/Pigs/PigSrv/pch.cpp`

PURPOSE: tiny file (9 lines), shown in full rather than summarized.

    // pch.cpp : source file that includes just the standard includes
    //  pch.pch will be the pre-compiled header
    //  pch.obj will contain the pre-compiled type information
    #include "pch.h"
    #ifdef _ATL_STATIC_REGISTRY
      #include <statreg.h>
      //#include <statreg.cpp>
    #endif
    //#include <atlimpl.cpp>

## `src/Pigs/PigSrv/pch.h`

**PURPOSE:**  
This file is a precompiled header file (`pch.h`) for the Allegiance game's PigSrv project. It includes standard system headers and project-specific headers that are frequently used but changed infrequently.

**KEY PARTS:**
- `CExeModule`: A class derived from `TCComModule` that handles command-line options, security settings, and registration/unregistration of COM components.
- `_Module`: An instance of `CExeModule`.
- Various standard C++ and Windows headers such as `<atlbase.h>`, `<algorithm>`, `<map>`, and `<wininet.h>`.

**CONNECTS TO:**  
Depends on Allegiance-specific headers like `<dplay8.h>`, `<ddraw.h>`, and `<shlwapi.h>`.

**NOTES:**  
- Disables warnings related to security descriptors (`4786`) and COM interface debugging.
- Includes Steam API for integration with the game's multiplayer features.
- Contains commented-out includes for DirectX components that are not used in this project.

## `src/Pigs/PigSrv/resource.h`

PURPOSE: This file contains resource definitions used by the PigSrv application in Allegiance. It includes string and icon identifiers for various resources.

KEY PARTS:
- IDI_ICON1: Default application icon.
- IDS_PROJNAME: Project name identifier.
- IDR_PigSrv, IDR_PigSession: Resource IDs for different types of resources.
- IDS_SYNTAX_9X, IDS_SYNTAX_NT: Syntax error messages for different operating systems.
- IDR_AccessPermission_9X: Access permission resource ID for Windows 9x.
- IDR_PigBehaviorScript, IDR_PigBehaviorHost, etc.: Resource IDs for various behavior and script-related resources.

CONNECTS TO: PigSrv.rc (resource file).

NOTES: None.

## `src/Pigs/PigSrv/thrust.js`

PURPOSE: tiny file (14 lines), shown in full rather than summarized.

    /////////////////////////////////////////////////////////////////////////////
    // Create the Session Information object
    //var objInfo = new ActiveXObject("Pigs.SessionInfo");
    //objInfo.ApplicationName = WScript.ScriptFullName;
    /////////////////////////////////////////////////////////////////////////////
    // Connect to the Pig Server
    var objSession = new ActiveXObject("Pigs.Session");
    //objSession.SessionInfo = objInfo;
    /////////////////////////////////////////////////////////////////////////////
    // Get the pig
    var objPig = objSession.Pigs("jtasler-nt-000");
    WScript.Echo(objPig + ": State is \"" + objPig.PigStateName + "\"");
    // Thrust the pig forward
    objPig.Ship.Thrust(5);
