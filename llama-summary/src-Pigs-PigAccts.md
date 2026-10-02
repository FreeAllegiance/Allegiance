# src/Pigs/PigAccts

[Back to the index](../LLAMA_SUMMARY.md) - 16 files, 893 lines. Notes by a local model, to orient by: not verified facts.

This folder contains source code related to managing player accounts in Allegiance, a 2000 Microsoft multiplayer space game. It includes classes and functions for creating, accessing, and dispensing player accounts.

- PigAccount.cpp: Implements the CPigAccount class, which manages individual player account details.
- PigAccountDispenser.cpp: Manages a collection of player accounts, loading data from INI files and dispensing available accounts.
- PigAccts.cpp: Contains the main entry point and initialization logic for a Windows application related to managing pig accounts.
- pch.h: Precompiled header file providing common includes and definitions used across multiple source files in this module.

Largest files: PigAccountDispenser.cpp (270 lines), PigAccts.cpp (122 lines), PigAccount.cpp (120 lines), PigAccountDispenser.h (90 lines), PigAccount.h (76 lines).

## `src/Pigs/PigAccts/PigAccount.cpp`

PURPOSE: This file contains the implementation of the CPigAccount class, which represents a player account in the Allegiance game. It manages account details and interacts with the dispenser to handle account lifecycle events.

KEY PARTS:
- CPigAccount: Manages player account data including name, password, and CD key.
- FinalConstruct: Initializes the object after construction.
- FinalRelease: Releases resources when the object is destroyed.
- Init: Sets up the account with initial parameters and references to the dispenser.
- InterfaceSupportsErrorInfo: Checks if the interface supports error information.
- get_Name, get_Password, get_CdKey: Retrieves the respective account details.

CONNECTS TO: PigAccountDispenser (depends on for lifecycle management).

NOTES: None.

## `src/Pigs/PigAccts/PigAccount.h`

PURPOSE: This file defines the CPigAccount class, which represents a player account in the Allegiance game. It provides methods to get and set account details such as name, password, and CD key.

KEY PARTS:
- Name: CPigAccount - Manages player accounts with properties for name, password, and CD key.
- Function: FinalConstruct() - Initializes the account object.
- Function: Init(CPigAccountDispenser* pDispenser, _bstr_t bstrName, _bstr_t bstrPassword, _bstr_t bstrCdKey) - Sets up the account with dispenser and credentials.
- Interface: IPigAccount - Exposes methods to get account details.
- Data Member: m_bstrName, m_bstrPassword, m_bstrCdKey - Stores account information.

CONNECTS TO: CPigAccountDispenser (depends on)

NOTES: None.

## `src/Pigs/PigAccts/PigAccount.rgs`

PURPOSE: This file is a Windows registry script that defines the class information for a COM object used in Allegiance to manage player accounts.

KEY PARTS:
- Pigs.Account.1: A version-specific ProgID for the Pig Account Class.
- CLSID: The globally unique identifier (GUID) for the Pig Account Class.
- CurVer: Points to the current version of the Pig Account Class.
- AppID: Links the COM object to an application ID.
- TypeLib: References a type library that provides additional information about the COM object.

CONNECTS TO: Allegiance game engine, likely through the AppID and TypeLib references.

NOTES: None.

## `src/Pigs/PigAccts/PigAccountDispenser.cpp`

PURPOSE: The file `PigAccountDispenser.cpp` implements the `CPigAccountDispenser` class, which manages a collection of player accounts for the Allegiance game. It provides methods to load account data from an INI file, dispense available accounts, and retrieve lists of account names.

KEY PARTS:
- CPigAccountDispenser: Manages the pool of player accounts.
- LoadAccounts: Reads account data from an INI file and populates the `m_Accounts` vector.
- get_NextAvailable: Returns the next available account to a caller.
- get_Names, get_NamesAvailable, get_NamesInUse: Retrieve lists of all account names or those currently in use.

CONNECTS TO:
- Allegiance game engine components for managing player accounts and authentication.

NOTES:
- None.

## `src/Pigs/PigAccts/PigAccountDispenser.h`

PURPOSE: This file defines a COM object that manages and dispenses game accounts.

KEY PARTS:
- CPigAccountDispenser: Main class implementing the IPigAccountDispenser interface for managing game accounts.
- XAccount: Struct representing an individual account with name, password, CD key, and availability status.
- LoadAccounts: Method to load account data from a source (not shown in this snippet).
- get_NextAvailable: Returns the next available account.
- get_Names, get_NamesAvailable, get_NamesInUse: Methods to retrieve lists of account names based on their state.

CONNECTS TO: Depends on Allegiance game engine components and libraries for COM support and string handling.

NOTES: None.

## `src/Pigs/PigAccts/PigAccountDispenser.rgs`

PURPOSE: This file is a Windows registry script used to register the Pig Account Dispenser COM component for use in Allegiance.

KEY PARTS:
- Name: Pigs.AccountDispenser.1
  - Description: Represents the versioned ProgID for the Pig Account Dispenser class.
- CLSID: {8D6763CA-E7CB-11d2-A508-00C04F68DEB0}
  - Description: The globally unique identifier for the COM component.
- CurVer: Pigs.AccountDispenser.1
  - Description: Specifies the current version of the ProgID.
- LocalServer32: %MODULE%
  - Description: Points to the executable file that implements the COM component.

CONNECTS TO:
- Allegiance game engine, specifically for managing player accounts and dispensing resources.

NOTES:
- None.

## `src/Pigs/PigAccts/PigAccts.cpp`

PURPOSE: This file contains the main entry point and initialization logic for a Windows application related to managing pig accounts.

KEY PARTS:
- `WinMain`: The program's entry point function.
- `_tmain`: Handles command-line arguments and initializes the application.
- `CExeModule`: Manages the application module, including registration of class objects and handling messages.
- `TCWinApp`: Represents the main application object.
- `CPigAccountDispenser` and `CPigAccount`: External classes used for account management.

CONNECTS TO: Other system components and libraries such as ATL (Active Template Library) and COM (Component Object Model).

NOTES: None.

## `src/Pigs/PigAccts/PigAccts.h`

PURPOSE: tiny file (8 lines), shown in full rather than summarized.

    #ifndef __PigAccts_h__
    #define __PigAccts_h__
    #pragma once
    /////////////////////////////////////////////////////////////////////////////
    #include <PigsIDL.h>
    #include <PigsLib.h>
    /////////////////////////////////////////////////////////////////////////////
    #endif // !__PigAccts_h__

## `src/Pigs/PigAccts/PigAccts.rgs`

PURPOSE: tiny file (11 lines), shown in full rather than summarized.

    HKCR
    {
    	NoRemove AppID
    	{
    		{9BA731AE-E7CB-11d2-A508-00C04F68DEB0} = s 'Pig Account Server'
    		'PigAccts.EXE'
    		{
    			val AppID = s {9BA731AE-E7CB-11d2-A508-00C04F68DEB0}
    		}
    	}
    }

## `src/Pigs/PigAccts/PigAcctsPS.def`

PURPOSE: tiny file (8 lines), shown in full rather than summarized.

    LIBRARY      "PigAcctsPS"
    DESCRIPTION  'Proxy/Stub DLL'
    EXPORTS
    	DllGetClassObject       @1	PRIVATE
    	DllCanUnloadNow         @2	PRIVATE
    	GetProxyDllInfo         @3	PRIVATE
    	DllRegisterServer		@4	PRIVATE
    	DllUnregisterServer		@5	PRIVATE

## `src/Pigs/PigAccts/SrcInc.cpp`

PURPOSE: tiny file (9 lines), shown in full rather than summarized.

    /////////////////////////////////////////////////////////////////////////////
    // This file should contain the CPP files for all source code-based
    // libraries used in the project. Any corresponding header files for the
    // libraries should be placed in SrcInc.h.
    //
    #include "pch.h"
    #include <TCAtl.cpp>
    #include <..\TCAtl\TCNullStreamImpl.cpp>
    #include <..\TCAtl\UserAcct.cpp>

## `src/Pigs/PigAccts/SrcInc.h`

PURPOSE: tiny file (9 lines), shown in full rather than summarized.

    #pragma once
    /////////////////////////////////////////////////////////////////////////////
    // This file should contain the include files for all source code-based
    // libraries used in the project. Any corresponding CPP files for the
    // libraries should be placed in SrcInc.cpp.
    //
    #include <TCAtl.h>
    #include <..\TCAtl\ObjectMap.h>
    #include <..\TCAtl\UserAcct.h>

## `src/Pigs/PigAccts/pch.cpp`

PURPOSE: tiny file (9 lines), shown in full rather than summarized.

    // pch.cpp : source file that includes just the standard includes
    //  pch.pch will be the pre-compiled header
    //  pch.obj will contain the pre-compiled type information
    #include "pch.h"
    #ifdef _ATL_STATIC_REGISTRY
      #include <statreg.h>
      // #include <statreg.cpp>
    #endif
    // #include <atlimpl.cpp>

## `src/Pigs/PigAccts/pch.h`

PURPOSE: This file is a precompiled header for the PigAccts module in Allegiance, providing common includes and definitions used across multiple source files.

KEY PARTS:
- CExeModule: A class derived from TCComModule that handles command-line options and registration.
- DisplaySyntax: A method to display usage syntax for the application.
- GetRegInfo: Retrieves registry information for the module.
- _Module: An instance of CExeModule, used by the COM framework.

CONNECTS TO:
- Other Allegiance modules, particularly those related to COM and automation.

NOTES:
- Disables several compiler warnings to suppress noise from BoundsChecker and other tools.

## `src/Pigs/PigAccts/resource.h`

PURPOSE: This file contains resource definitions used by the PigAccts module of Allegiance. It includes string and identifier constants for various resources and messages.

KEY PARTS:
- IDS_PROJNAME: Project name identifier.
- IDS_SYNTAX_9X: Syntax error message for Windows 9x systems.
- IDS_SYNTAX_NT: Syntax error message for Windows NT systems.
- IDR_PigAccts: Resource ID for the PigAccts module.
- IDR_PigAccountDispenser: Resource ID for the PigAccountDispenser resource.
- IDR_PigAccount: Resource ID for the PigAccount resource.
- IDS_E_NONE_AVAILABLE: Error identifier indicating no resources are available.

CONNECTS TO: This file is used by the PigAccts.rc resource script to define and reference various resources in the Allegiance game.

NOTES: None.

## `src/Pigs/PigAccts/test.vbs`

PURPOSE: tiny file (11 lines), shown in full rather than summarized.

    Set accts = CreateObject("Pigs.AccountDispenser")
    ' Jump into the debugger
    Stop
    Sub List(names)
      c = 0
      For Each name In names
        WScript.Echo name
        c = c + 1
      Next
      WScript.Echo c & " names listed"
    End Sub
