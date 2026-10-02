# src/AllSrvUI

[Back to the index](../LLAMA_SUMMARY.md) - 36 files, 7,238 lines. Notes by a local model, to orient by: not verified facts.

The `src/AllSrvUI` folder contains source code for the Allegiance game server's user interface, responsible for managing administrative tasks and player interactions.

- AdminPageThreadWnd.cpp: Handles administrative messages and events in the server UI.
- AllSrvUISheet.cpp: Manages a property sheet dialog for configuring the game server.
- PagePlayers.cpp: Manages the player-related UI page, handling game state changes and chat interactions.
- PageChat.cpp: Manages chat functionality within the game client's user interface.
- AutoSizer.cpp: Dynamically resizes and repositions controls based on specified rules.

Largest files: PagePlayers.cpp (833 lines), PageChat.cpp (776 lines), AllSrvUISheet.cpp (696 lines), AutoSizer.cpp (610 lines), PageGameCreate.cpp (556 lines).

## `src/AllSrvUI/AdminPageThreadWnd.cpp`

**PURPOSE:**  
The `AdminPageThreadWnd.cpp` file implements a window class (`CAdminPageThreadWnd`) that handles administrative messages and events in the Allegiance game server UI. It processes messages related to admin alerts and runs specified actions based on those messages.

**KEY PARTS:**
- **Class:** `CAdminPageThreadWnd`
  - Manages an AGCEvent object for handling admin-related events.
  - Handles messages using a message map.
- **Functions:**
  - `PostAdminPageMsg`: Posts a message indicating an admin page event.
  - `PostAdminPageRun`: Posts a message to run a specified action.
  - `TranslateEvent`: Translates an event from a stream back into the AGCEvent object.
  - `GetDate`, `GetSender`, `GetMessage`: Retrieve specific details from the AGCEvent object.
- **Data:**
  - `m_spEvent` and `m_spEventPersist`: Interfaces for managing the AGCEvent object.

**CONNECTS TO:**  
Depends on Allegiance game server UI components, specifically related to handling admin messages and events.

**NOTES:**  
None.

## `src/AllSrvUI/AdminPageThreadWnd.h`

PURPOSE:
The file defines a class `CAdminPageThreadWnd` that extends `CWnd` to create a window for managing admin pages in the Allegiance game server. It handles posting messages and events related to admin operations.

KEY PARTS:
- CAdminPageThreadWnd: Extends `CWnd` to create a window for admin page management.
- PostAdminPageMsg, PostAdminPageRun: Functions to post specific admin-related messages or run commands.
- TranslateEvent: Translates event data into a readable format.
- GetDate, GetSender, GetMessage: Retrieve formatted date, sender information, and message content.
- VariantTimeToLocalTime: Converts a variant time to local system time.

CONNECTS TO:
Depends on Allegiance-specific interfaces like `IAGCEvent` and `IPersistStream`.

NOTES:
None.

## `src/AllSrvUI/AllSrvUI.cpp`

PURPOSE: This file contains the implementation of the main application class for Allegiance Server UI, which initializes the application, handles events, and manages the user interface.

KEY PARTS:
- CAllSrvUIApp: Main application class responsible for initialization, event handling, and managing the property sheet window.
- InitInstance: Initializes COM, creates a property sheet window, and checks if another instance is already running.
- ExitInstance: Uninitializes COM before exiting the application.
- GetArtPath: Retrieves the path to artwork resources from the registry.
- GetAllSrvUIEvent: Opens or creates an event to ensure only one instance of the application runs.
- CreateAllSrvUIEvent: Creates an event used for synchronization.

CONNECTS TO:
- CAllSrvUISheet: The property sheet window class that serves as the main user interface.

NOTES:
- EULA-related code is commented out and not included in this file.

## `src/AllSrvUI/AllSrvUI.h`

PURPOSE: This file is the main header file for the Allegiance Server User Interface application.

KEY PARTS:
- CAllSrvUIApp: Main application class derived from CWinApp.
- GetArtPath: Static function to retrieve the path to art assets.
- InitInstance, ExitInstance, OnIdle: Overrides of virtual functions for application initialization, cleanup, and idle processing.
- GetAllSrvUIEvent, CreateAllSrvUIEvent: Functions related to synchronization events.
- m_shEventSync, m_hrCoInit: Data members for event handle and COM initialization result.

CONNECTS TO: Allegiance game engine and user interface components.

NOTES: None.

## `src/AllSrvUI/AllSrvUISheet.cpp`

PURPOSE: This file contains the implementation of a property sheet dialog for managing a game server in Allegiance. It handles user interactions, event management, and UI updates.

KEY PARTS:
- `CAllSrvUISheet`: Main class representing the property sheet dialog.
- `XEvents`: Event handler class implementing `IAdminSessionEvents` to handle server events.
- `InitSysMenu`, `InitIcon`: Initialize system menu and icon for the dialog.
- `AddChildrenToAutoSizer`: Set up auto-sizing rules for child controls.
- `UpdateStatus`: Update status bar text based on game stage.
- `HandleError`: Handle errors and display error messages.

CONNECTS TO: Allegiance game server components, UI elements like property pages and status bar.

NOTES: None.

## `src/AllSrvUI/AllSrvUISheet.h`

PURPOSE: The file defines a dialog class `CAllSrvUISheet` that inherits from `CPropertySheet`, used for managing various configuration pages in the Allegiance server UI.

KEY PARTS:
- CAllSrvUISheet: Main dialog class inheriting from `CPropertySheet`.
- XEvents: Nested class implementing `IAdminSessionEvents` to handle session events.
- OnInitDialog: Initializes the dialog.
- OnSysCommand, OnSize, OnMove, OnTimer, OnDestroy, OnClose: Message handlers for various window events.

CONNECTS TO:
- PageDummy, PageConnect, PageGameCreate, PageChat, PagePlayers: Various UI pages managed by `CAllSrvUISheet`.
- IAdminSession, IAdminServer, IAdminGame: Interfaces for server and game management.

NOTES:
- None.

## `src/AllSrvUI/AutoSizer.cpp`

PURPOSE: The file `AutoSizer.cpp` implements the `CAutoSizer` class, which manages dynamic resizing and repositioning of controls within a window based on specified rules.

KEY PARTS:
- `CAutoSizer`: Manages the auto-sizing logic for a window.
- `SetWindowAndRules`: Initializes the auto-sizing with a set of rules defining how controls should resize and reposition.
- `SetMinSize`: Sets the minimum size of the window, converting dialog units to pixels if specified.
- `ProcessMessage`: Handles messages like `WM_DESTROY`, `WM_GETMINMAXINFO`, `WM_SIZE`, and `WM_STYLECHANGED`.
- `AddRule`: Adds a rule defining how one control should follow another.
- `RemoveRules`: Removes all rules associated with a specific follower window.
- `UpdatePositions`: Updates the positions of controls based on current window size and rules.
- `OnDestroy`, `OnGetMinMaxInfo`, `OnSize`, `OnStyleChanged`: Message handlers for various window events.

CONNECTS TO: This file depends on other parts of the Allegiance game, particularly related to window management and control positioning.

NOTES: None.

## `src/AllSrvUI/AutoSizer.h`

**PURPOSE:** The file `AutoSizer.h` defines the `CAutoSizer` class, which is responsible for automatically sizing and positioning child windows within a parent window based on predefined rules.

**KEY PARTS:**
- **Class: CAutoSizer**
  - Manages the sizing and positioning of child windows.
  - Handles messages to update positions when the parent or child windows change size or style.
  
- **Struct: AutoSizerRule**
  - Defines a rule for how one window (follower) should be sized and positioned relative to another window (leader).
  
- **Macros: BEGIN_AUTO_SIZER_MAP, AUTO_SIZER_ENTRY, END_AUTO_SIZER_MAP**
  - Used to define and initialize an array of `AutoSizerRule` structures that specify the sizing rules.

**CONNECTS TO:** 
- Other windows within the same parent window for which sizing rules are defined.

**NOTES:** None.

## `src/AllSrvUI/ComboValueTranslator.h`

PURPOSE: This file defines an interface and a template implementation for translating between combo box indices and values in the Allegiance game.

KEY PARTS:
- IComboValueTranslator: Interface with methods to get default index, translate value to index, and translate index to value.
- IComboValueTranslatorImpl<T>: Template class implementing the IComboValueTranslator interface using a provided array of values.
- BEGIN_ComboTranslator and END_ComboTranslator: Macros for defining concrete translators that provide an array of values and a default index.

CONNECTS TO: Other files or systems that use combo boxes to select values, such as game settings or UI components.

NOTES: None.

## `src/AllSrvUI/DlgAbout.cpp`

PURPOSE: This file implements the dialog box used to display information about the Allegiance server software, including its description, version, copyright, product ID, and legal warning.

KEY PARTS:
- CDlgAbout: The main class for the About dialog.
- OnInitDialog: Initializes the dialog by setting up data bindings and loading information from resources and the registry.
- DoDataExchange: Handles data exchange between the dialog controls and member variables.
- ZVersionInfo: A utility class to retrieve version information from a resource.
- CRegKey: Used to access the Windows registry for reading the product ID.

CONNECTS TO:
- VersionInfo resource: For retrieving file description, version, and copyright.
- Registry (HKEY_LOCAL_MACHINE\HKLM_AllSrvUI): To read the product ID.
- String resources: For loading the legal warning message.

NOTES: None.

## `src/AllSrvUI/DlgAbout.h`

PURPOSE: This file defines the header for a dialog box used to display information about the Allegiance game.

KEY PARTS:
- CDlgAbout: A class representing the About dialog box.
  - Handles initialization and data exchange.
  - Displays version, product ID, copyright, and legal warning information.

CONNECTS TO: src/AllSrvUI/DlgAbout.cpp (implementation file).

NOTES: None.

## `src/AllSrvUI/DlgChatPreferences.cpp`

PURPOSE: The file `DlgChatPreferences.cpp` implements a dialog for managing chat preferences in the Allegiance game. It allows users to enable or disable features like chat lists and limits, as well as manage an administrative shortcut.

KEY PARTS:
- `CDlgChatPreferences`: Main class representing the chat preferences dialog.
- `DoDataExchange`: Handles data exchange between the dialog controls and member variables.
- `LoadFromRegistry`/`SaveToRegistry`: Functions to load and save settings from/to the Windows registry.
- `ShortcutExists`: Checks if an administrative shortcut exists.
- `UpdateUI`: Enables/disables UI elements based on current settings.
- `OnInitDialog`, `OnOK`: Initialize dialog and handle OK button click.

CONNECTS TO: 
- `CPageChat`: The parent page or container for the chat preferences dialog.

NOTES:
- Contains commented-out code related to shortcut properties (`#if 0`).
- Uses COM interfaces like `IShellLinkPtr` and `IAGCEventPtr`.
- Saves settings in the Windows registry under `HKEY_LOCAL_MACHINE\HKLM_AllSrvUI`.

## `src/AllSrvUI/DlgChatPreferences.h`

PURPOSE: This file defines the header for a dialog class `CDlgChatPreferences` used in Allegiance to manage chat preferences.

KEY PARTS:
- CDlgChatPreferences: The main dialog class for managing chat settings.
- IsAdminPageMonitored(): Checks if any admin-related chat monitoring options are enabled.
- IsChatMonitored(): Determines if the chat list is being monitored.
- GetPage(): Returns a pointer to the associated `CPageChat` object.
- DoDataExchange(): Handles data exchange between dialog controls and member variables.
- OnInitDialog(): Initializes the dialog when it is first created.
- OnOK(): Processes the OK button click, saving settings to the registry.
- OnEnableChatList(), OnUnlimitedChatHistory(), OnLimitChatHistory(), OnAdminRun(), OnAdminRunProperties(), OnAdminRunTest(): Message handlers for various UI controls.

CONNECTS TO: CPageChat (the page that this dialog is associated with).

NOTES: None.

## `src/AllSrvUI/DlgGameSelect.cpp`

PURPOSE: This file implements a dialog for selecting games in the Allegiance game server UI. It handles displaying and interacting with a list of available games.

KEY PARTS:
- CDlgGameSelect: Main class for the game selection dialog.
- PopulateGamesList: Fetches and populates the list of games from the server.
- OnInitDialog: Initializes the dialog, sets up the game list, and selects the current game.
- OnDeleteItemGameList: Handles deletion of items in the game list.
- OnItemChangedGameList: Updates the selected game when an item is changed.
- OnDblClickGameList: Selects the OK button on double-click.

CONNECTS TO: AllSrvUISheet, IAdminGames, IAdminGame

NOTES: None.

## `src/AllSrvUI/DlgGameSelect.h`

PURPOSE: This file defines the header for a dialog class `CDlgGameSelect` used in Allegiance to allow users to select and manage games.

KEY PARTS:
- CDlgGameSelect: A dialog class that allows users to select and interact with games.
- DECLARE_MESSAGE_MAP(): Macro used by MFC to map message handlers to member functions.
- OnInitDialog(): Initializes the dialog when it is first displayed.
- OnDeleteItemGameList(), OnItemChangedGameList(), OnDblClickGameList(): Message handlers for list control events.
- m_listGames: A `CListCtrl` object that displays the list of games.
- GetSelectedGame(): Returns a pointer to the selected game.

CONNECTS TO: CAllSrvUISheet, IAdminGame

NOTES: None.

## `src/AllSrvUI/GamePageSpecifics.cpp`

PURPOSE: This file contains the implementation of a property page class named `CGamePageSpecifics` for Allegiance's user interface.

KEY PARTS:
- Class: `CGamePageSpecifics`
  - Inherits from: `CPropertyPage`
  - Purpose: Represents a specific game-related property page in the Allegiance UI.
  
- Function: `OnOK()`
  - Purpose: Handles the OK button click event for the property page.

- Function: `OnSetActive()`
  - Purpose: Handles the activation of the property page.

- Function: `DoDataExchange(CDataExchange* pDX)`
  - Purpose: Manages data exchange between the dialog controls and member variables.

- Function: `OnInitDialog()`
  - Purpose: Initializes the dialog when it is first displayed.

CONNECTS TO:
- Allegiance's user interface system, specifically related to property pages.

NOTES:
- None.

## `src/AllSrvUI/GamePageSpecifics.h`

PURPOSE: This header file defines a dialog class for game-specific settings in the Allegiance game server UI.

KEY PARTS:
- CGamePageSpecifics: A dialog class derived from CPropertyPage that handles the user interface for game-specific parameters.
  - IDD = IDD_GAME_SPECIFICS: The resource ID for the dialog template.
  - OnOK(): Virtual function to handle the OK button click event.
  - OnSetActive(): Virtual function to handle when the page becomes active.
  - DoDataExchange(CDataExchange* pDX): Handles data exchange between the dialog controls and member variables.
  - OnInitDialog(): Initializes the dialog.

CONNECTS TO: CGameParamsSheet, which is likely a parent or container for this dialog.

NOTES: None.

## `src/AllSrvUI/GamePageTeam.cpp`

PURPOSE: This file contains the implementation of a property page for managing team settings in Allegiance's game interface.

KEY PARTS:
- CGamePageTeam: A class representing a property page for configuring team-related settings.
- OnOK(): Handles the OK button click event, performing default processing.
- OnSetActive(): Handles when the property page becomes active, performing default processing.
- DoDataExchange(): Manages data exchange between the dialog controls and member variables.
- OnInitDialog(): Initializes the dialog box, performing default processing.

CONNECTS TO: Allegiance's game interface and team management system.

NOTES: None.

## `src/AllSrvUI/GamePageTeam.h`

PURPOSE: This file defines a dialog class `CGamePageTeam` for managing team dynamics settings in the Allegiance game. It includes properties such as team count, player limits, skill levels, and starting money.

KEY PARTS:
- CGamePageTeam: A dialog class derived from `CPropertyPage` that manages team dynamics settings.
- IDD_TEAM_DYNAMICS: The resource ID for the dialog template.
- OnOK(): Virtual function to handle the OK button click event.
- OnInitDialog(): Virtual function to initialize the dialog when it is created.

CONNECTS TO: CGameParamsSheet (likely another dialog or sheet class).

NOTES: 
- Combo translators are commented out and not used in this file.

## `src/AllSrvUI/GamePageWin.cpp`

PURPOSE: This file contains the implementation of a property page dialog for the Allegiance game server user interface.

KEY PARTS:
- CGamePageWin: A class representing a property page dialog in the Allegiance game server UI.
  - Handles initialization, data exchange, and message processing.
  - Provides default implementations for standard dialog events like OK button click and active state handling.

CONNECTS TO: The Allegiance game server user interface (AllSrvUI.dll).

NOTES: None.

## `src/AllSrvUI/GamePageWin.h`

PURPOSE: This file defines a dialog class `CGamePageWin` for the Allegiance game's property sheet that handles the winning conditions page.

KEY PARTS:
- CGamePageWin: A dialog class derived from `CPropertyPage`, representing the winning conditions page in the game settings.
- IDD_WINNING_CONDITIONS: The resource ID for the dialog template associated with this page.
- OnOK(): Virtual function to handle the OK button click event, likely saving or applying changes related to winning conditions.
- OnInitDialog(): Virtual function to initialize the dialog when it is first displayed.

CONNECTS TO: CGameParamsSheet (the parent property sheet).

NOTES: None.

## `src/AllSrvUI/GameParamsSheet.cpp`

PURPOSE:
The file `CGameParamsSheet.cpp` implements a dialog sheet for game parameters in the Allegiance game. It manages property pages for window settings, team configurations, and specific game details.

KEY PARTS:
- `CGameParamsSheet`: Main class representing the game parameters dialog sheet.
- `AddPage`: Adds property pages to the dialog.
- `InitButtons`: Adjusts button positions and visibility within the dialog.
- `OnInitDialog`: Initializes the dialog when it is created, setting up button states.

CONNECTS TO:
Depends on `CPropertySheet`, `CPropertyPage`, and other UI-related classes in Allegiance.

NOTES:
None.

## `src/AllSrvUI/GameParamsSheet.h`

PURPOSE: This file defines a dialog class `CGameParamsSheet` that inherits from `CPropertySheet`, used for managing game parameters in the Allegiance game.

KEY PARTS:
- CGameParamsSheet: A dialog class derived from `CPropertySheet` to manage game parameters.
- DECLARE_MESSAGE_MAP(): Macro for message map declaration.
- OnInitDialog(): Virtual function to initialize the dialog.
- InitButtons(): Protected method to initialize buttons.
- m_PageWin, m_PageTeam, m_PageSpecifics: Member variables representing different pages of the property sheet.

CONNECTS TO:
- GamePageTeam.h
- GamePageWin.h
- GamePageSpecifics.h

NOTES: None.

## `src/AllSrvUI/PageChat.cpp`

**PURPOSE:** The `PageChat.cpp` file in Allegiance is responsible for managing the chat functionality within the game client's user interface. It handles events related to chat messages and admin pages, updates the UI accordingly, and manages settings such as column order and MRU chat messages.

**KEY PARTS:**
- **Class:** CPageChat - Represents a property page for chat functionality.
- **Function:** OnEvent(IAGCEvent* pEvent) - Handles events related to chat messages and admin pages.
- **Function:** LoadFromRegistry() - Loads chat preferences and settings from the Windows registry.
- **Function:** SaveChatListColumnOrderToRegistry() - Saves the current column order of the chat list to the registry.
- **Function:** SaveChatListColumnWidthsToRegistry() - Saves the current column widths of the chat list to the registry.
- **Function:** SaveSendChatMRUToRegistry() - Saves the most recently used (MRU) chat messages to the registry.
- **Function:** SendChat() - Sends a chat message based on user input, updating the MRU list and sending the message to the appropriate game or all games.
- **Data Member:** m_Prefs - An instance of DlgChatPreferences containing user preferences for chat settings.
- **Data Member:** m_listChat - A CListCtrl used to display chat messages in a list format.

**CONNECTS TO:**
- Allegiance game client's main UI system and event handling framework.
- Windows registry for storing user preferences and settings.

**NOTES:**
- There are commented-out lines that suggest potential issues or areas for improvement, indicating refactoring or previous debugging efforts.

## `src/AllSrvUI/PageChat.h`

PURPOSE: The file defines a dialog class `CPageChat` for managing chat functionality in the Allegiance game server UI.

KEY PARTS:
- Class: CPageChat - Manages the chat interface and handles user interactions.
- Function: OnEvent(IAGCEvent* pEvent) - Handles events related to chat messages.
- Function: OnInitDialog() - Initializes the dialog when it is created.
- Function: SendChat() - Sends a chat message based on user input.
- Data Member: m_listChat - A list control for displaying chat messages.

CONNECTS TO: DlgChatPreferences.h, AutoSizer.h, CAllSrvUISheet, CAdminPageThreadWnd

NOTES: None.

## `src/AllSrvUI/PageConnect.cpp`

PURPOSE: The file `PageConnect.cpp` implements a property page for connecting to a game server in the Allegiance game. It handles session creation and progress tracking.

KEY PARTS:
- `CPageConnect`: A class representing the connection property page.
- `CreateSessionThreadThunk`: A thunk function used to create a worker thread for session creation.
- `CreateSessionThreadProc`: The actual procedure that creates an admin session and handles errors.
- `OnInitDialog`: Initializes the dialog, starts the session creation thread, and sets up a progress timer.
- `OnTimer`: Updates the progress bar every half-second until it reaches 60%.
- `OnSessionSucceeded` and `OnSessionFailed`: Handle successful and failed session creation events.

CONNECTS TO: 
- `CAllSrvUISheet`: The parent sheet that handles post-connection actions.
- `AdminSessionSecureHost`, `IAdminSessionClassPtr`, `IGlobalInterfaceTablePtr`: COM interfaces for session management.

NOTES:
- Contains TODO comments to add custom drawing for the progress bar.
- Uses older COM initialization method (`CoInitializeEx` with `COINIT_MULTITHREADED`) instead of the newer `CoInitialize`.
- Includes debug output using `debugf`.

## `src/AllSrvUI/PageConnect.h`

PURPOSE:
The file defines a dialog class `CPageConnect` for connecting to a session in the Allegiance game. It handles user interface elements and event processing.

KEY PARTS:
- CPageConnect: A dialog class derived from `CPropertyPage` that manages the connection process.
- OnEvent: Handles events related to the connection process.
- CreateSessionThreadProc: The main function for creating a session in a separate thread.
- OnInitDialog: Initializes the dialog when it is first displayed.
- OnTimer: Handles timer events during the connection process.
- OnNMCustomdrawProgressConnecting: Custom draw handler for the progress control.

CONNECTS TO:
PageConnect.h depends on AutoSizer.h and includes CAllSrvUISheet, which suggests it interacts with other UI components and possibly game session management.

NOTES:
- The commented-out `GetSheet` function suggests an alternative way to access the parent sheet.
- There are custom message IDs defined for session success and failure events (`wm_SessionSucceeded`, `wm_SessionFailed`).

## `src/AllSrvUI/PageDummy.cpp`

PURPOSE: This file contains the implementation of a dummy property page class for use in Allegiance's user interface.

KEY PARTS:
- CPageDummy: A class representing a dummy property page.
- DoDataExchange: Handles data exchange between the dialog box and the member variables.

CONNECTS TO: AllSrvUI.h, AllSrvUISheet.h

NOTES: None.

## `src/AllSrvUI/PageDummy.h`

PURPOSE:
This file defines a dialog class `CPageDummy` for a property sheet in the Allegiance game's user interface. It is part of the AllSrvUI module.

KEY PARTS:
- CPageDummy: A dialog class derived from `CPropertyPage`, representing a page within a property sheet.
  - DECLARE_DYNAMIC and DECLARE_MESSAGE_MAP macros are used to support dynamic creation and message handling.
  - DoDataExchange function for data exchange between controls and member variables.
  - IDD_PAGE_DUMMY: The resource ID for the dialog template.

CONNECTS TO:
- CAllSrvUISheet: The parent property sheet class that contains this page.

NOTES:
- There is commented-out code attempting to retrieve a pointer to the parent `CAllSrvUISheet` using reinterpret_cast.
- No TODOs, hacks, disabled code, or security/network handling noted.

## `src/AllSrvUI/PageGameCreate.cpp`

PURPOSE: The file `PageGameCreate.cpp` implements a property page for creating and managing game settings in the Allegiance game client. It handles user interactions and updates the game parameters accordingly.

KEY PARTS:
- `CPageGameCreate`: Class representing the game creation property page.
- `OnPrivateGame`, `OnPublicGame`: Handlers for radio button clicks to set game visibility.
- `OnChangeGameName`, `OnChangeGameCore`: Handlers for text changes in game name and core fields.
- `OnGameSettings`: Opens a dialog to manage game settings.
- `OnGameSelect`, `OnGameCreate`, `OnGameDestroy`: Handlers for creating, selecting, and destroying games.

CONNECTS TO: `AllSrvUI.h`, `AllSrvUISheet.h`, `GameParamsSheet.h` - Other UI-related files and sheets.

NOTES: None.

## `src/AllSrvUI/PageGameCreate.h`

PURPOSE: This file defines a dialog page for creating and managing games in the Allegiance game server UI.

KEY PARTS:
- CPageGameCreate: A dialog class for creating and managing game parameters.
- OnEvent: Handles events related to game creation and management.
- UpdateUI: Updates the user interface based on current game parameters.
- OnInitDialog: Initializes the dialog when it is first created.
- OnPrivateGame, OnPublicGame: Handlers for setting game visibility.
- OnChangeGameName, OnChangeGameCore: Handlers for changing game name and core settings.
- m_spGameParameters: Pointer to game parameter interface.
- m_AutoSizer: Manages automatic sizing of UI elements.

CONNECTS TO: CAllSrvUISheet (parent sheet), IAGCEvent (events), IAGCGameParameters (game parameters).

NOTES: None.

## `src/AllSrvUI/PagePlayers.cpp`

**PURPOSE:** The `PagePlayers.cpp` file manages the player-related UI page in the Allegiance server UI. It handles events related to game state changes, populates the list of players, sends chat messages, and updates the UI based on these interactions.

**KEY PARTS:**
- **Class: CPagePlayers**
  - Manages the player list UI page.
  
- **Function: OnEvent(IAGCEvent* pEvent)**
  - Handles events related to game state changes such as player login, logout, and team changes. Updates the UI based on these events.

- **Function: PopulatePlayersList()**
  - Fetches and populates the list of players currently in the game using COM interfaces like `IAdminUsers`, `IAdminUser`, and `IAdminShip`.

- **Function: SendChat()**
  - Sends a chat message to selected players or the entire game if all players are selected. Updates the MRU (Most Recently Used) list of chat messages.

- **Function: UpdateUI(bool bUpdateData = true)**
  - Enables/disables UI controls based on whether a game is in progress and if players are selected. Optionally updates data members from UI fields.

- **Function: LoadFromRegistry()**
  - Loads the state of the player list (column order, widths) and chat MRU from the Windows registry.

- **Function: SavePlayerListColumnOrderToRegistry()**
  - Saves the current column order of the player list to the Windows registry.

- **Function: SavePlayerListColumnWidthsToRegistry()**
  - Saves the current column widths of the player list to the Windows registry.

- **Function: SaveSendChatMRUToRegistry()**
  - Saves the MRU list of chat messages to the Windows registry.

- **TODOs/Unusual Code:**
  - Uses `CRegKey` and `CComBSTR` for registry operations.
  - There is a commented-out line suggesting a previous implementation using a different method to query DWORD values from the registry.
  - Includes assertions and error handling mechanisms for robust COM interface usage.

**CONNECTS TO:** Other UI components, game state management, and COM interfaces for player data retrieval.

**NOTES:** The file uses Microsoft's ATL (Active Template Library) for registry operations. There is a commented-out line that suggests an alternative method to query DWORD values from the registry.

## `src/AllSrvUI/PagePlayers.h`

PURPOSE: The file defines a dialog page for managing players in the Allegiance game server UI.

KEY PARTS:
- CPagePlayers: A property page class that handles the player management interface.
- OnEvent: Handles events related to player actions and updates the UI accordingly.
- PopulatePlayersList: Fills the player list with current player data.
- SendChat: Sends a chat message from the selected player.
- SaveToRegistry: Saves the state of the player list, including column order and widths, to the registry.

CONNECTS TO:
- CAllSrvUISheet: The parent sheet that contains this page.

NOTES:
- None.

## `src/AllSrvUI/StdAfx.cpp`

PURPOSE: tiny file (4 lines), shown in full rather than summarized.

    // stdafx.cpp : source file that includes just the standard includes
    //	AllSrvUI.pch will be the pre-compiled header
    //	stdafx.obj will contain the pre-compiled type information
    #include "stdafx.h"

## `src/AllSrvUI/StdAfx.h`

PURPOSE: This file is a precompiled header for the Allegiance server user interface module, including standard system and project-specific headers.

KEY PARTS:
- `IsWinNT()`: Determines if the operating system is Windows NT or later.
- `sizeofArray(x)`: Macro to calculate the number of elements in a fixed-length array.
- Allegiance product registry keys: Constants for managing registry settings related to the Allegiance server user interface.
- MFC and ATL includes: Provides support for Microsoft Foundation Classes and Active Template Library, essential for Windows application development.

CONNECTS TO: Other parts of the Allegiance server user interface module and system libraries like MFC, ATL, ZLib, and TCLib.

NOTES: None.

## `src/AllSrvUI/resource.h`

PURPOSE: This file contains resource definitions used by the Allegiance game's user interface, including dialog IDs, string identifiers, and bitmap IDs.

KEY PARTS:
- IDS_TITLE: Title of the application.
- IDD_ABOUTBOX: Dialog ID for the about box.
- IDC_PRIVATE_GAME: Checkbox to create a private game.
- IDC_PUBLIC_GAME: Checkbox to create a public game.
- IDD_CHAT_PREFERENCES: Dialog ID for chat preferences settings.
- IDC_SEND_CHAT_TEXT: Edit control for sending chat text.

CONNECTS TO: The user interface (UI) components of the Allegiance game.

NOTES: None.
