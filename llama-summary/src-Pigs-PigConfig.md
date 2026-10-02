# src/Pigs/PigConfig

[Back to the index](../LLAMA_SUMMARY.md) - 15 files, 2,151 lines. Notes by a local model, to orient by: not verified facts.

This folder contains source code related to configuration settings in Allegiance's Pig client. It includes various dialog boxes and property sheets for managing different aspects of the game's configuration.

- AboutDlg.cpp: Implements a dialog box displaying information about the Allegiance game.
- PageEvents.cpp: Manages the event configuration page, handling user interactions with a tree and list control to display and configure events.
- PigConfigSheet.cpp: Implements a configuration dialog sheet for Allegiance's Pig client, allowing users to configure general settings, events, and servers.
- PageServers.cpp: Configures server settings such as lobby and account servers.
- PageGeneral.cpp: Manages general settings related to pigs and their environment.

Largest files: PageEvents.cpp (926 lines), PigConfigSheet.cpp (240 lines), PageServers.cpp (155 lines), PageGeneral.cpp (144 lines), PageEvents.h (134 lines).

## `src/Pigs/PigConfig/AboutDlg.cpp`

PURPOSE: This file contains the implementation of the `CAboutDlg` class, which is a dialog box used to display information about the Allegiance game.

KEY PARTS:
- CAboutDlg: Class representing the About dialog box.
- DoDataExchange: Function for exchanging data between controls and variables.

CONNECTS TO: src/Pigs/PigConfig/AboutDlg.h (header file).

NOTES: None.

## `src/Pigs/PigConfig/AboutDlg.h`

PURPOSE: This file declares the class `CAboutDlg`, which is a dialog box used for displaying information about the application.

KEY PARTS:
- Name: CAboutDlg
  - Purpose: Manages the About dialog box.
- IDD = IDD_ABOUTBOX
  - Defines the resource ID for the dialog box.
- DoDataExchange(CDataExchange* pDX)
  - Handles data exchange between the dialog box and its controls.

CONNECTS TO: src/Pigs/PigConfig/resource.h

NOTES: None

## `src/Pigs/PigConfig/PageEvents.cpp`

**PURPOSE:** The `PageEvents.cpp` file in Allegiance is responsible for managing the event configuration page within the game. It handles user interactions with a tree and list control to display, select, and configure events.

**KEY PARTS:**
- **Class: CPageEvents** - Manages the event configuration page.
- **Function: OnOK()** - Saves changes made to the selected node's attributes.
- **Function: DoDataExchange(CDataExchange* pDX)** - Handles data exchange between dialog controls and member variables.
- **Function: GetEventLogger()** - Retrieves the event logger object from the session.
- **Function: InitEventTree()** - Initializes the tree control for displaying events.
- **Function: InitEventList()** - Initializes the list control for displaying events in a flat list format.
- **Function: PopulateTreeAndList()** - Populates the controls with data from an XML document.
- **Function: AddXMLNodeToTree(IXMLDOMNode* pNode, HTREEITEM hParent)** - Recursively adds nodes to the tree and list controls based on the XML structure.
- **Function: GetElementSeverity(IXMLDOMElement* pElem, BSTR* pbstr)** - Retrieves the severity level of an event element.
- **Function: ToggleCheckBox(UINT id)** - Toggles the state of a checkbox with a beep sound if disabled.

**CONNECTS TO:** 
- `EventLogger` class for handling event logging configurations.
- XML document for storing and retrieving event data.

**NOTES:** 
- The use of `MessageBeep(static_cast<DWORD>(-1))` for a beep sound when toggling disabled checkboxes.

## `src/Pigs/PigConfig/PageEvents.h`

PURPOSE: This file defines a dialog class `CPageEvents` for managing event configurations in the Allegiance game's configuration sheet.

KEY PARTS:
- CPageEvents: A dialog class derived from `CPropertyPage` that handles the display and management of event-related settings.
- DECLARE_MESSAGE_MAP(): Macro to declare message map functions.
- OnInitDialog(): Initializes the dialog when it is created.
- OnOK(): Handles the OK button click, likely saving changes.
- DoDataExchange(): Manages data exchange between the dialog controls and member variables.
- GetSheet(): Returns a pointer to the parent `CPigConfigSheet`.
- PopulateTreeAndList(): Populates the event tree and list with data from XML.
- AddXMLNodeToTree(): Adds an XML node to the event tree.
- ImageFromSeverity(): Determines the image index based on severity level.

CONNECTS TO: CPigConfigSheet, IXMLDOMDocumentPtr, CImageList

NOTES: None.

## `src/Pigs/PigConfig/PageGeneral.cpp`

PURPOSE: This file implements a property page in Allegiance for configuring general settings related to pigs and their environment.

KEY PARTS:
- `CPageGeneral`: Class representing the property page with controls for setting art path, piglet directory, limiting pigs, maximum pigs count, version, and flavor.
- `DoDataExchange`: Handles data exchange between the dialog controls and member variables.
- `UpdateUI`: Updates the UI based on the state of the `m_bLimitPigs` flag.
- `OnInitDialog`: Initializes the property page with values from the session object.
- `OnOK`: Saves the updated values back to the session object when the OK button is clicked.

CONNECTS TO: `CPigConfigSheet`, `IPigSessionPtr`, `IAGCVersionInfoPtr`.

NOTES: None.

## `src/Pigs/PigConfig/PageGeneral.h`

PURPOSE: This file defines a dialog page for general configuration settings in the Allegiance game, allowing users to adjust various parameters such as updates per second and pig limits.

KEY PARTS:
- CPageGeneral: A class representing the general configuration dialog page.
- OnOK(): Handles the OK button click event.
- DoDataExchange(): Manages data exchange between the dialog controls and member variables.
- OnInitDialog(): Initializes the dialog when it is first displayed.
- OnUpdatesPerSecondDefault() and OnChangeUpdatesPerSecond(): Handle updates to the updates per second setting.
- OnLimitPigs(): Handles enabling or disabling pig limits.
- m_spinMaxPigs, m_editMaxPigs, m_staticMaxPigs: Controls for displaying and editing the maximum number of pigs.
- m_strArtPath, m_strPigletDir: Strings representing paths to art assets and piglet directories.
- m_bLimitPigs: Boolean indicating whether pig limits are enabled.
- m_strVersion, m_strFlavor: Strings representing the game version and flavor text.
- m_cMaxPigs: Long integer representing the maximum number of pigs.

CONNECTS TO: CPigConfigSheet (the parent configuration sheet).

NOTES: None.

## `src/Pigs/PigConfig/PageServers.cpp`

PURPOSE: This file implements a property page in Allegiance that allows users to configure server settings, including lobby and account servers.

KEY PARTS:
- CPageServers: Class representing the property page for server configuration.
- OnInitDialog: Initializes the dialog with values from the session object.
- OnOK: Saves the configured server settings back to the session object.
- UpdateUI: Updates the UI based on the current state of the controls.
- OnSameAsLobby, OnMyComputer, OnThisComputer: Placeholder functions for handling control notifications.

CONNECTS TO: Pigs.Session object for retrieving and saving server configuration.

NOTES:
- TODOs are present for implementing functionality in OnSameAsLobby, OnMyComputer, and OnThisComputer.

## `src/Pigs/PigConfig/PageServers.h`

PURPOSE: This file contains the declaration of the CPageServers class, which is a dialog page for configuring server settings in the Allegiance game.

KEY PARTS:
- Name: CPageServers - A dialog page for configuring server settings.
- OnOK() - Handles the OK button click event to save changes.
- DoDataExchange(CDataExchange* pDX) - Manages data exchange between controls and member variables.
- OnInitDialog() - Initializes the dialog when it is first displayed.
- OnSameAsLobby(), OnMyComputer(), OnThisComputer() - Handle specific server configuration options.

CONNECTS TO: CPigConfigSheet - The parent property sheet that contains this page.

NOTES: None.

## `src/Pigs/PigConfig/PigConfig.cpp`

PURPOSE: This file contains the implementation of the main application class for a configuration utility in Allegiance, specifically responsible for initializing and managing the property sheet window.

KEY PARTS:
- CPigConfigApp: Main application class that handles initialization and cleanup.
- InitInstance: Initializes COM and creates the property sheet window.
- ExitInstance: Uninitializes COM before exiting the application.

CONNECTS TO: CPigConfigSheet (the property sheet window).

NOTES: None.

## `src/Pigs/PigConfig/PigConfig.h`

PURPOSE: This file is the main header file for the PigConfig application, which appears to be a configuration tool related to Allegiance.

KEY PARTS:
- CPigConfigApp: The main application class derived from CWinApp.
  - Contains virtual functions InitInstance and ExitInstance for application initialization and cleanup.
  - Uses ClassWizard-generated code for message map and other functionalities.

CONNECTS TO: None explicitly mentioned, but likely depends on Allegiance's core libraries and possibly other configuration files or systems.

NOTES: None.

## `src/Pigs/PigConfig/PigConfigSheet.cpp`

PURPOSE: The file `CPigConfigSheet.cpp` implements a configuration dialog sheet for Allegiance's Pig client, allowing users to configure general settings, events, and servers.

KEY PARTS:
- `CPigConfigSheet`: Main class representing the configuration dialog sheet.
- `Create()`: Initializes the dialog sheet by creating a session, adding property pages, setting window styles, and handling errors.
- `InitSysMenu()`, `InitIcon()`, `InitButtons()`: Functions to initialize system menu, icon, and button positions respectively.
- `PostNcDestroy()`: Cleans up resources when the dialog is destroyed.
- `OnInitDialog()`: Initializes the dialog by setting up the system menu, icon, and buttons.
- `OnSysCommand()`, `OnOK()`, `OnCancel()`: Handle system commands, OK, and Cancel button events respectively.

CONNECTS TO: `PigConfigSheet.h`, `AboutDlg.h`, `resource.h`

NOTES: None

## `src/Pigs/PigConfig/PigConfigSheet.h`

PURPOSE: This file defines the CPigConfigSheet class, which is a property sheet dialog for configuring Allegiance game settings.

KEY PARTS:
- CPigConfigSheet: A CPropertySheet-derived class representing the configuration dialog.
- IPigSession*: An interface pointer to the session object.
- CPageGeneral, CPageServers, CPageEvents: Page classes that represent different tabs within the configuration sheet.

CONNECTS TO: Allegiance game engine and its settings system.

NOTES: None.

## `src/Pigs/PigConfig/pch.cpp`

PURPOSE: tiny file (5 lines), shown in full rather than summarized.

    /////////////////////////////////////////////////////////////////////////////
    // pch.cpp : source file that includes just the standard includes
    //  pch.pch will be the pre-compiled header
    //  pch.obj will contain the pre-compiled type information
    #include "pch.h"

## `src/Pigs/PigConfig/pch.h`

PURPOSE:
This file is a precompiled header for the Allegiance game's PigConfig module. It includes essential headers and defines macros to streamline development by providing common functionality across multiple source files.

KEY PARTS:
- `GetErrorText(DWORD dwError)`: Function that converts an error code into a human-readable string.
- `_WIN32_WINNT 0x0A00`: Defines the minimum required version of Windows for the project.
- `#pragma warning(disable:4786)`, `#pragma warning(disable:4192)`: Disables specific compiler warnings to suppress noise.

CONNECTS TO:
Depends on various Allegiance-specific libraries and headers, including MFC, ATL, and custom game modules like AGC, PigsIDL, and PigsLib.

NOTES:
- None.

## `src/Pigs/PigConfig/resource.h`

PURPOSE: This file contains resource definitions used by the PigConfig application, which is part of Allegiance. It includes string and dialog identifiers that are referenced in the application's user interface.

KEY PARTS:
- IDI_ICON1: Icon identifier for the application.
- IDR_MAINFRAME: Resource identifier for the main frame window.
- IDM_ABOUTBOX: Command identifier for the about box menu item.
- IDS_PROJNAME, IDD_ABOUTBOX, IDS_TITLE, etc.: String and dialog identifiers used in various parts of the application.

CONNECTS TO: PigConfig.rc - This file is included by PigConfig.rc to define resources for the application's user interface.

NOTES: None.
