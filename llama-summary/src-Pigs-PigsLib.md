# src/Pigs/PigsLib

[Back to the index](../LLAMA_SUMMARY.md) - 18 files, 1,671 lines. Notes by a local model, to orient by: not verified facts.

This folder contains source code for Allegiance's PigsLib library, which provides functionality related to game objects and behaviors.

- PigMissionParams.cpp: Implements mission parameters management.
- PigMissionParams.h: Defines the COM object class for managing mission parameters.
- PigObject.cpp: Implements basic functionality for a pig object in the game.
- PigObject.h: Declares and defines the COM object for a pig object.
- PigsLib.cpp: Contains the implementation of the DLL for PigsLib, providing game-specific functionalities.
- PigsLib.idl: Defines interface definitions and coclasses for various game entities.

Largest files: PigsLib.h (401 lines), PigMissionParams.cpp (313 lines), PigsLib.idl (265 lines), PigsLib.cpp (165 lines), PigMissionParams.h (131 lines).

## `src/Pigs/PigsLib/PigMissionParams.cpp`

PURPOSE: This file contains the implementation of the CPigMissionParams class, which manages mission parameters for a game mission in Allegiance.

KEY PARTS:
- CPigMissionParams: Manages and validates mission parameters such as team count, player limits, map type, and various game goals.
- Validate: Checks if the mission parameters are within acceptable ranges.
- put_TeamCount, get_TeamCount: Set and get the number of teams in the mission.
- put_MaxTeamPlayers, get_MaxTeamPlayers: Set and get the maximum number of players per team.
- put_MinTeamPlayers, get_MinTeamPlayers: Set and get the minimum number of players per team.
- put_MapType, get_MapType: Set and get the type of map for the mission.
- put_TeamKills, get_TeamKills: Set and get the goal for team kills.
- put_GameName, get_GameName: Set and get the name of the game.
- put_CoreName, get_CoreName: Set and get the core name associated with the game.

CONNECTS TO: Allegiance game engine components that utilize mission parameters.

NOTES: None.

## `src/Pigs/PigsLib/PigMissionParams.h`

PURPOSE: This file defines a COM object class `CPigMissionParams` that represents mission parameters for a game mission in Allegiance. It includes properties and methods to manage various aspects of the mission such as team counts, map types, game names, and more.

KEY PARTS:
- CPigMissionParams: Main class implementing IPigMissionParams, IDispatch, ISupportErrorInfo, and other interfaces.
- DECLARE_REGISTRY_RESOURCEID: Registers the class with a resource ID.
- BEGIN_COM_MAP: Maps COM interfaces to methods in the class.
- BEGIN_PROP_MAP: Defines properties that can be accessed via COM.
- CPigMissionParams::FinalConstruct: Constructor for the object.
- IPigMissionParams Interface Methods: Various methods to get and set mission parameters.

CONNECTS TO:
- Allegiance game engine components, specifically related to mission management.

NOTES:
- None.

## `src/Pigs/PigsLib/PigMissionParams.rgs`

PURPOSE: This file is a Windows Registry Script (RGS) that defines the class information for the Pig Mission Parameters Class in the Allegiance game. It specifies the CLSID, ProgID, and other properties needed to register this class with the Windows Registry.

KEY PARTS:
- Name: Pigs.MissionParams.1
  - What it does: Represents a specific version of the Pig Mission Parameters Class.
  
- Name: Pigs.MissionParams
  - What it does: The default ProgID for the Pig Mission Parameters Class, pointing to the latest version.

- Name: CLSID {15219787-D562-11d2-9646-00C04F68DEB0}
  - What it does: Unique identifier for the Pig Mission Parameters Class.
  
- Name: TypeLib
  - What it does: Specifies the type library associated with this class.

- Name: ProgID
  - What it does: Identifies the programmatic identifier for the class.

- Name: VersionIndependentProgID
  - What it does: Provides a version-independent identifier for the class.

- Name: InprocServer32
  - What it does: Specifies the in-process server executable that implements the class.
  
- Name: ThreadingModel
  - What it does: Defines the threading model used by the class (Both).

CONNECTS TO:
- Allegiance game code, specifically related to mission parameters.

NOTES:
- None.

## `src/Pigs/PigsLib/PigObject.cpp`

PURPOSE: This file contains the implementation of the CPig$$Object$$ class, which is part of the Allegiance game's PigsLib library. The class implements the IPig$$Object$$ interface and provides basic functionality for a pig object in the game.

KEY PARTS:
- CPig$$Object$$: Class representing a pig object.
- FinalConstruct(): Method called during object construction to initialize resources.
- FinalRelease(): Method called when the object is being released, allowing for cleanup.
- InterfaceSupportsErrorInfo(REFIID riid): Checks if the interface supports error information.

CONNECTS TO: PigsLib library and IPig$$Object$$ interface.

NOTES: None.

## `src/Pigs/PigsLib/PigObject.h`

PURPOSE: This file declares and defines the CPig$$Object$$ class, which is part of the Allegiance game's PigsLib library. It provides a COM object that implements the IPig$$Object$$ interface.

KEY PARTS:
- CPig$$Object$$: Main class implementing the IPig$$Object$$ interface.
- DECLARE_REGISTRY_RESOURCEID: Registers the class with the registry using a resource ID.
- DECLARE_PROTECT_FINAL_CONSTRUCT: Ensures the constructor is protected and final.
- BEGIN_COM_MAP: Maps COM interfaces to methods in the class.
- FinalConstruct: Called during object construction.
- FinalRelease: Called when the object is released.

CONNECTS TO:
- PigEngine.h, PigSrv.h: Other headers from the PigsLib library.
- IPig$$Object$$ interface: Defines the contract for this COM object.

NOTES:
- None.

## `src/Pigs/PigsLib/PigObject.rgs`

PURPOSE: This file is a Windows registry script for the Allegiance game's PigsLib library, specifically defining the class information and ProgID for the PigObject class.

KEY PARTS:
- Name: PigObject
  - What it does: Represents an object in the game that belongs to the PigsLib library.
- CLSID: {$$CLSID_Object$$}
  - What it does: Unique identifier for the PigObject class.
- ProgID: Pigs.$$Object$$.1
  - What it does: Programmatic identifier for the PigObject class version 1.
- AppID: {F132B4E3-C6EF-11D2-85C9-00C04F68DEB0}
  - What it does: Identifier for the application that uses this class.
- TypeLib: {F132B4E2-C6EF-11D2-85C9-00C04F68DEB0}
  - What it does: Type library associated with the PigObject class.
- InprocServer32: %MODULE%
  - What it does: Specifies the DLL that implements the PigObject class.

CONNECTS TO:
- PigsLib.dll (indicated by %MODULE%)

NOTES:
- None.

## `src/Pigs/PigsLib/PigsLib.cpp`

PURPOSE: This file contains the implementation of a DLL for Allegiance's PigsLib library, which provides functionality related to mission parameters and other game-specific objects.

KEY PARTS:
- `CComModule _Module`: Manages the module state for ATL-based components.
- `TCWinApp g_app`: Represents the application object.
- `ObjectMap`: Maps class IDs to their corresponding classes.
- `DllMain`: Entry point for the DLL, handling initialization and termination.
- `DllCanUnloadNow`: Determines if the DLL can be unloaded by OLE.
- `DllGetClassObject`: Returns a class factory for creating objects of requested types.
- `DllRegisterServer`: Registers the DLL with the system registry.
- `DllUnregisterServer`: Unregisters the DLL from the system registry.

CONNECTS TO: Allegiance game engine, other libraries like TCLib and PigsIDL.

NOTES: None.

## `src/Pigs/PigsLib/PigsLib.def`

PURPOSE: tiny file (7 lines), shown in full rather than summarized.

    ; PigsLib.def : Declares the module parameters for the DLL.
    LIBRARY      "PigsLib"
    EXPORTS
      DllCanUnloadNow     PRIVATE
      DllGetClassObject   PRIVATE
      DllRegisterServer   PRIVATE
      DllUnregisterServer PRIVATE

## `src/Pigs/PigsLib/PigsLib.h`

PURPOSE: This file contains the interface definitions for various classes in the Allegiance game's PigsLib library, which is used for managing game objects and behaviors.

KEY PARTS:
- `PigsInterfaces`: Manages game interfaces.
- `PigSession`: Handles game sessions.
- `PigBehaviorScript`: Represents behavior scripts for game entities.
- `PigBehaviorScriptType`: Defines types of behavior scripts.
- `PigBehaviorHost`: Hosts and manages behavior scripts.
- `PigMissionParams`: Stores mission parameters.
- `Pig`: Represents a generic game entity.
- `Pigs`: Manages multiple Pig objects.
- `PigTimer`: Handles timers for game events.
- `PigShip`: Represents a ship in the game.
- `PigShipEvent`: Represents events related to ships.
- `PigAccount`: Manages player accounts.
- `PigAccountDispenser`: Dispenses player accounts.
- `PigHullTypes`: Defines types of ship hulls.
- `PigBehaviorStack`: Manages a stack of behavior scripts.

CONNECTS TO: PigsIDL.h

NOTES: None.

## `src/Pigs/PigsLib/PigsLib.idl`

PURPOSE: This file defines the interface definitions and coclasses for the Allegiance game's Pig library, which provides functionality related to game sessions, behaviors, missions, players, and other game entities.

KEY PARTS:
- PigsLib: The main type library containing various interfaces and coclasses.
- IPigSession: Interface for managing game sessions.
- IPigBehaviorType: Interface for behavior types.
- IPigMissionParams: Interface for mission parameters.
- IPig: Interface for individual players or entities.
- IPigs: Interface for collections of players or entities.
- PigSession: Coclass implementing the IPigSession interface.
- PigBehaviorScript: Coclass implementing the IPigBehavior interface.

CONNECTS TO: PigsIDL.idl, stdole32.tlb, stdole2.tlb, ScrRun.dll, agc.tlb

NOTES: None.

## `src/Pigs/PigsLib/PigsLibPS.def`

PURPOSE: tiny file (8 lines), shown in full rather than summarized.

    LIBRARY      "PigsLibPS"
    DESCRIPTION  'Proxy/Stub DLL'
    EXPORTS
      DllGetClassObject       @1  PRIVATE
      DllCanUnloadNow         @2  PRIVATE
      GetProxyDllInfo         @3  PRIVATE
      DllRegisterServer       @4  PRIVATE
      DllUnregisterServer     @5  PRIVATE

## `src/Pigs/PigsLib/Resource.h`

PURPOSE: tiny file (19 lines), shown in full rather than summarized.

    //{{NO_DEPENDENCIES}}
    // Microsoft Developer Studio generated include file.
    // Used by PigsLib.rc
    //
    #define IDS_E_TEAMCOUNT                 4108
    #define IDS_E_MAXTEAMPLAYERS            4109
    #define IDS_E_MINTEAMPLAYERS            4110
    #define IDS_E_MINMAXREVERSED            4111
    #define IDR_PigMissionParams            2000
    // Next default values for new objects
    //
    #ifdef APSTUDIO_INVOKED
    #ifndef APSTUDIO_READONLY_SYMBOLS
    #define _APS_NEXT_RESOURCE_VALUE        2002
    #define _APS_NEXT_COMMAND_VALUE         32768
    #define _APS_NEXT_CONTROL_VALUE         3000
    #define _APS_NEXT_SYMED_VALUE           101
    #endif
    #endif

## `src/Pigs/PigsLib/SrcInc.cpp`

PURPOSE: tiny file (9 lines), shown in full rather than summarized.

    /////////////////////////////////////////////////////////////////////////////
    // This file should contain the CPP files for all source code-based
    // libraries used in the project. Any corresponding header files for the
    // libraries should be placed in SrcInc.h.
    //
    #include "pch.h"
    #include <TCAtl.cpp>
    #include <..\TCAtl\PropertyClass.cpp>
    #include <..\TCAtl\TCNullStreamImpl.cpp>

## `src/Pigs/PigsLib/SrcInc.h`

PURPOSE: tiny file (10 lines), shown in full rather than summarized.

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

## `src/Pigs/PigsLib/dlldatax.c`

PURPOSE: This file is a wrapper around the standard `dlldata.c` file to handle proxy stub DLL registration and management for Allegiance's PigsLib component.

KEY PARTS:
- `REGISTER_PROXY_DLL`: Macro defined to indicate that this DLL will register proxy stubs.
- `_WIN32_WINNT 0x0400`: Specifies the minimum Windows version required (Windows NT 4.0 or Windows 95 with DCOM).
- `USE_STUBLESS_PROXY`: Indicates that stubless proxies are used, which can improve performance by reducing overhead.
- `DllMain`, `DllRegisterServer`, `DllUnregisterServer`, `DllGetClassObject`, `DllCanUnloadNow`: These macros redefine the standard DLL entry points to use custom implementations (`PrxDllMain`, etc.).
- `TCObj_p.c`: Included file that contains proxy stub code generated by MIDL.

CONNECTS TO: `dlldata.c` and `TCObj_p.c`.

NOTES: None.

## `src/Pigs/PigsLib/dlldatax.h`

PURPOSE: tiny file (19 lines), shown in full rather than summarized.

    #if !defined(AFX_DLLDATAX_H__25072D60_AB2F_11D2_97EF_00A0C932A52F__INCLUDED_)
    #define AFX_DLLDATAX_H__25072D60_AB2F_11D2_97EF_00A0C932A52F__INCLUDED_
    #if _MSC_VER >= 1000
      #pragma once
    #endif // _MSC_VER >= 1000
    #ifdef _MERGE_PROXYSTUB
    extern "C"
    {
    BOOL WINAPI PrxDllMain(HINSTANCE hInstance, DWORD dwReason,
      LPVOID lpReserved);
    STDAPI PrxDllCanUnloadNow(void);
    STDAPI PrxDllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv);
    STDAPI PrxDllRegisterServer(void);
    STDAPI PrxDllUnregisterServer(void);
    }
    #endif
    //{{AFX_INSERT_LOCATION}}
    // Microsoft Developer Studio will insert additional declarations immediately before the previous line.
    #endif // !defined(AFX_DLLDATAX_H__25072D60_AB2F_11D2_97EF_00A0C932A52F__INCLUDED_)

## `src/Pigs/PigsLib/pch.cpp`

PURPOSE: tiny file (11 lines), shown in full rather than summarized.

    /////////////////////////////////////////////////////////////////////////////
    // pch.cpp : source file that includes just the standard includes
    //  pch.pch will be the pre-compiled header
    //  pch.obj will contain the pre-compiled type information
    #include "pch.h"
    #ifdef _ATL_STATIC_REGISTRY
      #include <statreg.h>
    //  #include <statreg.cpp>
    #endif
    //#include <atlimpl.cpp>
    //#include <atlwin.cpp>

## `src/Pigs/PigsLib/pch.h`

PURPOSE: This file is a precompiled header for the PigsLib project in Allegiance. It includes standard system headers, STL containers, Win32 API, and various Allegiance-specific libraries.

KEY PARTS:
- `#pragma warning (disable:4786)`: Disables warnings about identifier truncation.
- `VC_EXTRALEAN` and `_WIN32_DCOM`: Macros to exclude rarely-used Windows headers and enable DCOM features.
- CRT headers like `<crtdbg.h>` for debugging.
- STL containers such as `<algorithm>`, `<map>`, `<set>`, and `<vector>`.
- Win32 headers like `<windows.h>`.
- ATL headers for component object model support, including `<atlbase.h>`, `<atlcom.h>`, and `<atlwin.h>`.
- Steam API header `<steam_api.h>` for integration with Steam.
- Allegiance-specific headers like `<dplay8.h>`, `<ddraw.h>`, `<dsound.h>`, and `<wininet.h>`.

CONNECTS TO: Other system libraries (Windows, ATL, STL), Allegiance game engine components.

NOTES: None.
