# src/AutoUpdate

[Back to the index](../LLAMA_SUMMARY.md) - 6 files, 796 lines. Notes by a local model, to orient by: not verified facts.

The AutoUpdate folder in Allegiance's source code is responsible for managing automatic updates of the game files to ensure players have the latest version.

- AutoUpdate.cpp: Implements the logic for downloading, verifying, and applying updates.
- AutoUpdate.h: Defines the dialog class `CAutoUpdate` used for displaying update progress and handling user interactions.
- pch.cpp: Precompiled header file for faster compilation of other source files in the project.
- pch.h: Header file included by precompiled header files to provide common declarations.
- resource.h: Contains definitions for resources such as strings and dialog identifiers used in the auto-update process.

Largest files: AutoUpdate.cpp (614 lines), AutoUpdate.h (108 lines), pch.cpp (22 lines), resource.h (22 lines), pch.h (19 lines).

## `src/AutoUpdate/AutoUpdate.cpp`

**PURPOSE:**  
This file contains the implementation of the AutoUpdate system for Allegiance, a 2000 Microsoft multiplayer space game. It handles the auto-update process, including downloading files, verifying them, and executing post-update actions.

**KEY PARTS:**
- **CAutoDownloadSink:** Implements the `IAutoUpdateSink` interface to handle various events during the update process such as file retrieval progress, errors, and system termination.
- **MsgBox:** A utility function to display messages either through a message box or by writing to a log file based on whether the application is running as a service.
- **lBigGameIndex:** Determines the game with the most players in an Allegiance server.
- **WinMain:** The main entry point of the AutoUpdate system, handling command-line arguments and initializing the update process.

**CONNECTS TO:**
- `AutoUpdate.h`: Header file containing class declarations for `CAutoDownloadSink` and other related classes.
- `AdminSessionSecureHost.h`: Interface for managing admin sessions and servers.
- `AllSrvModuleIDL.h`: IDL file defining interfaces used by the AutoUpdate system.

**NOTES:**
- The code includes commented-out sections that were likely intended to provide user options during update errors but were disabled.
- There are TODO comments indicating areas where further development or improvements might be needed.

## `src/AutoUpdate/AutoUpdate.h`

PURPOSE: This file defines a dialog class `CAutoUpdate` for managing the auto-update process in Allegiance.

KEY PARTS:
- CAutoUpdate: Main class that handles the auto-update dialog.
  - OnInitDialog: Initializes the dialog and sets up UI elements.
  - OnDestroyWindow: Handles window destruction by posting a quit message.
  - OnOK/OnCancel: Handles user actions to abort the update process.
  - m_ProgressFileList, m_ProgressAnalyze, m_ProgressDownload, m_staticFileName: UI controls for displaying progress and file information.
  - m_szPostUpdateEXE: Buffer to store the path of an executable to run after update.

CONNECTS TO:
- IAutoDownload: Interface used for downloading updates.

NOTES:
- Contains commented-out code that could be re-enabled or removed.

## `src/AutoUpdate/AutoUpdate.rgs`

PURPOSE: tiny file (11 lines), shown in full rather than summarized.

    HKCR
    {
    	NoRemove AppID
    	{
    		{BB44CBB1-9D4D-47C4-9C45-00AA56B13A97} = s 'AutoUpdate'
    		'AutoUpdate.EXE'
    		{
    			val AppID = s {BB44CBB1-9D4D-47C4-9C45-00AA56B13A97}
    		}
    	}
    }

## `src/AutoUpdate/pch.cpp`

PURPOSE: tiny file (17 lines), shown in full rather than summarized.

    #include "pch.h"
    CComModule _Module;
    // #include <atlimpl.cpp>
    /////////////////////////////////////////////////////////////////////////////
    /*
    void _debugf(const char* format, ...)
    {
        const size_t size = 256;
        char         bfr[size];
        va_list vl;
        va_start(vl, format);
        _vsnprintf(bfr, size, format, vl);
        va_end(vl);
        debugf(bfr);
    }
    */
    //Imago removed 6/10

## `src/AutoUpdate/pch.h`

PURPOSE: tiny file (12 lines), shown in full rather than summarized.

    #pragma once
    #define _WIN32_WINNT 0x0A00
    #define __MODULE__ "AutoUpdate"
    #include <atlbase.h>
    extern CComModule _Module;
    #include <atlcom.h>
    #include <atlwin.h>
    #include <commctrl.h>
    #include "zlib.h"
    #include "Utility.h"
    #include "zreg.h"
    #include "AutoDownload.h"

## `src/AutoUpdate/resource.h`

PURPOSE: This file contains resource definitions used by the AutoUpdate module of Allegiance. It includes string and dialog identifiers for user interface elements.

KEY PARTS:
- IDS_PROJNAME: Project name identifier.
- IDR_AutoUpdate: Resource ID for the AutoUpdate module.
- IDD_AUTOUPDATE: Dialog ID for the AutoUpdate window.
- IDC_PROGRESS_FILELIST, IDC_PROGRESS_ANALYZE, IDC_PROGRESS_DOWNLOAD: Control IDs for progress bars in the AutoUpdate dialog.
- IDC_STATIC_FILENAME: Control ID for a static text element displaying file names.

CONNECTS TO: AutoUpdate.rc (the resource script that uses these definitions).

NOTES: None.
