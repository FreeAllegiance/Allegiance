# src/WinTrek

[Back to the index](../LLAMA_SUMMARY.md) - 89 files, 67,940 lines. Notes by a local model, to orient by: not verified facts.

The WinTrek folder contains core functionality and user interface components of the Allegiance game client application. It handles various aspects such as game screens, user interactions, network operations, and rendering.

- WinTrek.cpp: Manages game window functionality, camera control, and various game state events.
- trekigc.cpp: Manages game interactions within a cluster site, including sound effects, explosions, and model management.
- console.cpp: Manages console interactions and chat functionality within the Allegiance game.
- CmdView.cpp: Handles rendering various graphical elements such as a grid, ships, and text strings in a 3D space.

Largest files: WinTrek.cpp (11360 lines), trekigc.cpp (5589 lines), teamscreen.cpp (3705 lines), console.cpp (3511 lines), SquadsScreen.cpp (3391 lines).

## `src/WinTrek/CallsignTagInfo.cpp`

**PURPOSE:** The file `CallsignTagInfo.cpp` manages information related to player callsign tags and Steam group IDs, including fixing up callsign tags, rendering them with tokens, loading and saving settings from the registry, and updating officer status.

**KEY PARTS:**
- **Class: CallsignTagInfo**
  - Manages player's callsign tag, Steam group ID, index, and officer status.
  - Provides methods to fixup callsign tags, render callsigns with tokens, load and save settings from the registry, and update officer status.

- **Function: FixupCallsignTag(ZString callsignTag)**
  - Cleans up and standardizes player's callsign tag by removing special characters and handling specific cases like "MsAlleg" and "AllegFAO".

- **Function: Render(ZString callsign)**
  - Renders the player's callsign with an optional token if they are an officer.

- **Function: LoadFromRegistry()**
  - Loads Steam group ID and officer token from the registry and updates the object's state accordingly.

- **Function: SaveToRegistry()**
  - Saves the current state of the object, including Steam group ID and officer token, to the registry.

- **Function: UpdateStringValues(ZString selectedToken)**
  - Updates the callsign tag and officer status based on the player's role in the Steam group.

- **Function: GetAvailableTokens()**
  - Returns a string of available tokens for the player based on their role in the Steam group.

- **Function: SetSteamGroupID(uint64 steamGroupID, ZString callsignTag)**
  - Sets the Steam group ID and clears other related data, then saves to the registry.

- **Function: SetToken(ZString token)**
  - Sets the player's token and saves it to the registry.

**CONNECTS TO:** Configuration system for loading and saving settings, Steam API for interacting with Steam groups and users.

**NOTES:** None.

## `src/WinTrek/CallsignTagInfo.h`

PURPOSE: The file defines a class `CallsignTagInfo` used to manage and store information related to callsign tags in the Allegiance game. It handles operations such as loading and saving data from/to the registry, updating string values, and rendering callsigns.

KEY PARTS:
- `CallsignTagInfo`: Manages callsign tag information.
- `FixupCallsignTag`: Fixes up a callsign tag.
- `LoadFromRegistry`: Loads callsign tag information from the Windows registry.
- `SaveToRegistry`: Saves callsign tag information to the Windows registry.
- `UpdateStringValues`: Updates string values based on a selected token.
- `m_callsignTag`: Stores the callsign tag.
- `m_callsignToken`: Stores the callsign token.
- `m_steamGroupID`: Stores the Steam group ID associated with the callsign tag.
- `m_index`: Stores an index value.
- `m_isOfficer`: Indicates if the user is an officer.

CONNECTS TO: Windows registry for data persistence.

NOTES: None.

## `src/WinTrek/CharInfoScreen.cpp`

**PURPOSE:** The `CharInfoScreen.cpp` file in Allegiance is responsible for managing the character information screen, which displays statistics and medals related to a player's performance.

**KEY PARTS:**
- **CCharacterStats**: Holds various statistics of a character such as score, kills, deaths.
- **IZoneMedal**: Interface defining methods for zone medals.
- **CZoneMedalImpl**: Concrete implementation of the IZoneMedal interface.
- **CreateZoneMedal**: Function to create an instance of CZoneMedalImpl.
- **CharInfoScreen**: Manages the character information screen, handling user interactions and displaying data.
- **ForwardCharInfoMessage**: Forwards character info messages to the global `g_pCharInfoScreen` instance for processing.
- **OnCivChange**: Updates the screen when the selected civilization changes.
- **RefreshMedalsList**: Refreshes the medals listbox by sorting medals based on their ranking and updating the display.
- **MedalsItemPainter**: Custom item painter for the medals listbox, responsible for drawing medal icons and selection bars.

**CONNECTS TO:** Other systems or files it depends on or serves include game state management, player data handling, and UI components like listboxes.

**NOTES:**
- There are TODOs to make exports for certain functions and handle unknown CivIDs more gracefully.
- Conditional compilation logic for server connection handling is present with `NO_CLUB_SERVER_CONNECTION`.
- The use of `assert` to check the number of civ IDs matches names might be redundant if initialization is properly handled.

## `src/WinTrek/CreateGameDialog.h`

PURPOSE: This file defines a dialog popup for creating a game in the Allegiance game. It handles user interactions and data management for selecting a server and core, entering a game name, and initiating the game creation process.

KEY PARTS:
- CreateGameDialogPopup: Main class that manages the create game dialog.
- ServerItemPainter and CoreItemPainter: Classes responsible for painting items in the server and core lists.
- OnButtonOK and OnButtonCancel: Functions to handle the OK and Cancel button events.
- OnServersList: Function to update the list of servers with new data.

CONNECTS TO: trekClient, TrekResources, IPopupContainer, and other UI components.

NOTES: None.

## `src/WinTrek/PostTrainingSlideshow.cpp`

PURPOSE: This file contains the implementation of a slideshow screen that appears after training in Allegiance.

KEY PARTS:
- PostTrainingSlideshow: A class derived from Slideshow that handles the post-training slideshow.
- CreatePostTrainingSlideshow: A function to create an instance of PostTrainingSlideshow.

CONNECTS TO: slideshow.h, training.h

NOTES: None.

## `src/WinTrek/Slideshow.cpp`

PURPOSE: The Slideshow.cpp file implements a slideshow feature in the Allegiance game, which displays images and sounds sequentially.

KEY PARTS:
- `Slideshow` class: Manages the slideshow functionality.
- `CleanUpTimers`: Removes any active timers to prevent events after the slideshow ends.
- `StopSound`: Stops any currently playing sound and cleans up associated event sinks.
- `NextSlide`: Advances to the next slide in the sequence, updating the image and starting a new sound if available.
- `Dismiss`: Placeholder function for dismissing the slideshow (currently empty).
- `OnEvent`: Handles events, including timer events and keyboard inputs.
- `Picked`: Called when an object is picked, advancing to the next slide.
- `GetImage`: Returns the current image being displayed.
- `OnFrame`: Processes frame updates, checking if a slide change is needed.

CONNECTS TO: 
- Modeler: For loading slideshow data from MDL files.
- INameSpace and related interfaces: For managing namespaces and accessing slideshow data.
- ITimerEventSource and ISoundInstance: For handling timers and sound playback.
- IKeyboardInput: For processing keyboard inputs to advance slides.

NOTES:
- The `Dismiss` function is currently a placeholder and does nothing.
- There's a TODO comment indicating that the timer event source is not passing the event source correctly.

## `src/WinTrek/Slideshow.h`

PURPOSE: The Slideshow.h file defines a class for managing a slideshow screen in the Allegiance game. It handles image display, navigation, and sound effects.

KEY PARTS:
- Slideshow: Main class that manages the slideshow functionality.
- PickImage: Nested class within Slideshow that handles mouse interactions to pick images.
- NextSlide: Function to move to the next slide in the slideshow.
- GetImage: Returns the current image being displayed.
- OnFrame: Handles frame updates for the slideshow screen.
- Dismiss: Closes or dismisses the slideshow screen.
- OnEvent: Processes events related to the slideshow.
- Picked: Called when an image is picked (e.g., clicked).
- OnKey: Handles keyboard input for navigation.

CONNECTS TO: Depends on classes like IObjectList, Image, WrapImage, ISoundInstance, and IKeyboardInput.

NOTES: None.

## `src/WinTrek/SquadsScreen.cpp`

PURPOSE: Manages and displays squad information for players in Allegiance.

KEY PARTS:
- SquadsScreen: Main class handling the squad screen interface.
- FilterType: Enumerates filter types (ALL, MY, DUDEX) for squad display.
- XSquads: Class representing a list of squads.
- XZonePlayers: Class representing a list of zone players.
- TEvent<ZString>::Sink and TEvent<ItemID>::Sink: Event sinks for handling ZString and ItemID events.
- HistoryDialogPopup: Popup dialog for displaying ownership history.
- OnButtonZoneClub, OnButtonPlayerProfile, etc.: Handles various button click events on the squad screen.
- FillCombo and FillIntegerArray: Functions to populate combo boxes and integer arrays with data from the modeler.
- SquadsItemPainter and PlayersItemPainter: Classes for painting items in listboxes.

CONNECTS TO: Other UI elements and game systems for handling player interactions, squad management, and event processing.

NOTES:
- Contains TODOs related to ranking discontinuity and forced squad display.
- Includes commented-out sections with previous logic for handling rankings and filtering.

PARTS (in file order):

- lines 1-395
  - SquadsScreen: Main class for the Squads Screen.
  - FilterType: Enumerates different filter types for squad display (ALL, MY, DUDEX).
  - XSquads: Class representing a list of squads.
  - XZonePlayers: Class representing a list of zone players.
  - TEvent<ZString>::Sink: Sink for handling ZString events.
  - TEvent<ItemID>::Sink: Sink for handling ItemID events.
  - HistoryDialogPopup: Popup dialog for displaying ownership history.
  - OnLeavingScreen(): Destructor method for SquadsScreen class.
- lines 396-784
  - SquadsScreen: Manages the squad screen interface.
  - OnButtonZoneClub: Handles click event for Zone Club button.
  - OnButtonPlayerProfile: Handles click event for Player Profile button.
  - OnButtonLeaderboard: Handles click event for Leaderboard button.
  - OnButtonPrev, OnButtonNext, OnButtonTop, OnButtonEditSquad, OnButtonReject, OnButtonAccept, OnButtonCommission, OnButtonDemote, OnButtonTransferControl, OnButtonRequestJoin, OnButtonCancelRequest, OnButtonQuitSquad, OnButtonCreateSquad: Handle various button click events.
  - OnButtonLog, OnButtonApply, OnButtonJumpURL, OnButtonFindSquad: Handles other specific button click events.
  - FillCombo, FillIntegerArray: Functions to populate combo boxes and integer arrays with data from the modeler.
  - SquadsItemPainter, PlayersItemPainter: Classes for painting items in listboxes.
- lines 785-1133
  - FindSquadByCreationID: Returns a squad given its creation ID.
  - FindSquadByRanking: Returns a squad given its ranking.
  - FindPlayerByID: Returns a player given his ID.
  - CreateZoneSquad: Creates and adds a new squad to the list.
  - CreateZonePlayer: Creates and adds a new player to the list.
  - SEND_FOR_ACTIVE_MEMBER: Sends a message for an active member.
  - SEND_FOR_SELECTED_MEMBER: Sends a message for a selected member.
  - ResetHighLowRanges: Resets the highest/lowest ranking ranges.
- lines 1134-1501
  - ForwardSquadMessage: Processes server-generated messages for squad screen.
  - UpdateHighLowRanges: Updates highest and lowest rankings for a squad.
  - FindSquadByRanking: Finds a squad by ranking in a specific column.
  - AddSquadToScreen: Adds a squad to the screen if it's within the current range.
  - SelectSquad: Selects a squad on the screen.
  - RefreshScreen: Refreshes the squad screen display.
  - TODOs: "Ranking discontinuity detected in column %d, Ranking of %d" "Warning, Forcing squad to be shown (after search): %s" "Warning, pre-column change squad is not on screen when it should, resending Request for Page: %s"
- lines 1502-1864
  - SQUAD_CREATE_ACK: A message type indicating the acknowledgment of a squad creation request.
  - FindSquadByCreationID: Finds a squad by its creation ID.
  - DeleteSquad: Deletes a squad from the system.
  - ResetHighLowRanges: Resets the high and low ranking ranges for squads.
  - RequestPage: Requests a page of squad data based on the current ranking.
  - MessageBox: Displays a message box with the given string.
  - RefreshScreen: Refreshes various UI elements on the screen.
  - DisableAllOwnerButtons: Disables all buttons that are specific to squad owners.
- lines 1865-2269
  - GetWindow(): Returns a pointer to the window object.
  - LeaderBoardScreenForPlayer(playerName): Displays the leaderboard screen for the specified player.
  - CharInfoScreenForPlayer(playerID): Displays the character information screen for the specified player.
  - OnLeavingScreen(): Handles actions when leaving the current screen.
  - s_pSelectedPlayer: Pointer to the currently selected player.
  - m_nTab: Current tab index.
  - FILTER_MY, FILTER_DUDEX, FILTER_ALL: Constants representing different tabs.
  - m_listSquads: List of squads displayed on the screen.
- lines 2270-2625
  - OnButtonBarTabs: Handles tab selection and updates the squad list accordingly.
  - OnButtonBarPlayers: Manages player sorting by setting priorities.
  - AddSquadToScreen: Adds a squad to the screen's display list.
  - DeleteSquad: Removes a squad from the system and updates the display.
  - IsSquadAssociatedWithPlayer: Checks if a squad is associated with a given player.
  - RefreshSquadsListBox: Redraws the squad list box on the screen.
  - TODO: The commented-out code related to ranking and filtering should be reviewed and possibly integrated back into the main functionality.
  - Unusual Code: The commented-out sections suggest that there was previous logic for handling rankings and filtering, which has been temporarily removed.
- lines 2626-3007
  - SquadsScreen: Manages the squad screen and its components.
  - RefreshPlayersListBox: Updates the list box for players based on current filters and sorting.
  - CompareStrings: Compares two strings up to a specified number of characters.
  - FillCombo: Populates a combo box with items from an INameSpace.
  - FillIntegerArray: Fills an integer array with values from an INameSpace.
  - CivID2String: Converts a CivID to its corresponding string representation.
  - SquadRankingCompare, SquadNameCompare, ScoreCompare, WinsCompare, LossesCompare, HoursCompare: Comparison functions for sorting squads based on various attributes.
  - PlayerNameCompare, PlayerStatusCompare, PlayerLastDateCompare, PlayerRankCompare: Comparison functions for sorting players based on various attributes.
- lines 3008-3390
  - SquadsScreen: Manages the squad screen interface and handles player and squad interactions.
  - RefreshSquadDetailsArea: Updates the details area of the squad screen with information about the selected squad.
  - SelectSquad: Sets the currently selected squad.
  - SelectPlayer: Sets the currently selected player.
  - OnSquadSelected: Handles events when a squad is selected.
  - OnPlayerSelected: Handles events when a player is selected.
  - PlayersItemPainter: Custom item painter for the players listbox, responsible for rendering player information.
  - GetPane: Returns the main pane of the screen.

## `src/WinTrek/SteamClans.cpp`

PURPOSE: This file contains the implementation of the SteamClans class, which handles interactions with Steam clans to retrieve and manage available callsign tags for players.

KEY PARTS:
- SteamClans(): Constructor that initializes the SteamClans object by fetching clan information from Steam.
- OnClanOfficerListResponse(ClanOfficerListResponse_t *pCallback, bool bIOFailure): Callback function triggered when Steam API returns a response to the clan officer list request.
- GetAvailableCallsignTags(): Returns a pointer to the list of available callsign tags.

CONNECTS TO: src/WinTrek/SteamClans.h (header file), Steam API for clan information retrieval.

NOTES: None.

## `src/WinTrek/SteamClans.h`

PURPOSE: tiny file (15 lines), shown in full rather than summarized.

    #pragma once
    #ifdef STEAM_APP_ID
    // BT - STEAM
    class SteamClans
    {
    private:
    	TVector<CallsignTagInfo, DefaultEquals, DefaultCompare>	m_availableCallsignTags;
    	bool m_officerDataReceived = false;
    	void OnClanOfficerListResponse(ClanOfficerListResponse_t *pCallback, bool bIOFailure);
    	CCallResult< SteamClans, ClanOfficerListResponse_t > m_SteamCallResultClanOfficerListResponse;
    public:
    	SteamClans();
    	TVector<CallsignTagInfo, DefaultEquals, DefaultCompare> * GetAvailableCallsignTags();
    };
    #endif

## `src/WinTrek/TrainingSlideshow.cpp`

PURPOSE: This file contains the implementation of a training slideshow screen for the Allegiance game. It handles the display and dismissal of the training slideshow, as well as initiating the training simulation.

KEY PARTS:
- TrainingSlideshow: A class that inherits from Slideshow and EventTargetContainer<TrainingSlideshow>. It manages the training slideshow screen.
- Dismiss: A method that dismisses the slideshow, sets a wait cursor, shows a message box indicating that the training simulation has started, and pauses to allow the message box to draw itself.
- OnSwitchOut: An event handler that starts the training simulation when the slideshow is dismissed.

CONNECTS TO:
- slideshow.h: For Slideshow base class.
- training.h: For related training functionalities.

NOTES:
- None.

## `src/WinTrek/TrekDlgs.h`

PURPOSE: Manages various dialog windows and controls for the Allegiance game interface.

KEY PARTS:
- TrekControlWindow: Base class for control windows with enable/disable functionality.
- TrekScrollBar: Custom scroll bar implementation using PaneWindow.
- TrekImgButton: Button that displays an image and can be checked or unchecked.
- MainDlg: Manages the main game dialog.
- MissionsDlg: Handles mission-related dialogs and updates.
- SideStrategyDlg: Manages side strategy settings and player interactions.
- GameOverDlg: Displays the game over screen.
- LoadoutDlg: Manages player loadouts and inventory.
- CreateMissionDlg: Allows players to create new missions.

CONNECTS TO: PaneWindow, other dialog components.

NOTES: None.

## `src/WinTrek/Treki.h`

PURPOSE: This file defines the main window class for Allegiance, which handles the game's user interface and interactions.

KEY PARTS:
- TrekWindow: Main class representing the game window, managing various UI elements and game states.
- ScreenID: Enumerates different screens within the game (e.g., combat, splash screen).
- GetConfiguration(): Function to retrieve configuration settings for the game.
- Create(): Static method to create an instance of TrekWindow.

CONNECTS TO:
- cmdview.h
- load.h
- console.h

NOTES: None.

## `src/WinTrek/WinTrek.cpp`

PURPOSE: Manages game window functionality, camera control, and various game state events.

KEY PARTS:
- CameraControl: Manages camera properties like position, orientation, field of view, and jiggle effects.
- TrekWindowImpl: Main class for implementing window functionality, including handling keyboard input, managing screens, and playing video clips.
- GameStateCloseSink: Handles game state close events by updating win condition grids.
- LoadAnimatedImage: Loads or retrieves animated images from a cache.
- PlayMovieClip: Plays video clips in a separate thread under specific conditions.
- IsInputEnabled: Determines if a given virtual key should be considered enabled based on the current screen state and composition status.
- Camera: Manages camera properties and transformations.
- SetCameraMode: Changes the camera mode based on input, updating settings.
- GetViewMode: Returns the current view mode.
- UpdateCamera: Updates the camera position and orientation based on ship movement.
- ComputeExternalChase: Calculates the camera position for a chase view.
- AddMuzzleFlare: Adds a muzzle flare at a given direction and duration.
- DoTrekUpdate: Updates the game world based on elapsed time and frame ID.
- TakeScreenShot: Captures and saves a screenshot of the screen as a 24-bit bitmap.
- CreateMenu: Creates a new menu with items and handles popup menus.
- ToggleFilterUnknownChats: Toggles filtering of unknown chats and updates preferences.

CONNECTS TO: Allegiance game engine systems, video playback libraries, input handling subsystems.

NOTES: None.

PARTS (in file order):

- lines 1-382
  - CameraControl: Manages camera position, orientation, field of view, and jiggle.
  - GetCurrentTarget: Returns the current target for the ship, considering visibility and side affiliation.
  - GetSimilarTargetMask: Determines a bitmask indicating whether a target is friendly, enemy, or neutral based on the ship's side.
  - CreateMessageBox: Creates a message box with optional default and cancel buttons.
  - DDVidCreateThreadProc: Handles video playback in a separate thread.
  - SoundInit::InitializeSoundTemplates: Initializes sound templates for the game.
  - SoundInit::AddMembers: Adds members to a namespace related to sounds.
  - AnyViewMinRenderScreenSize: Minimum screen size for rendering views.
- lines 383-749
  - CameraControl::SetAnimation: Starts or stops animations for position and FOV.
  - CameraControl::GetAnimation: Returns the current animation duration.
  - CameraControl::SetJiggle: Adjusts the camera's jiggle effect based on afterburner power.
  - CameraControl::SetToModel: Sets the camera's position and orientation to match a model.
  - CameraControl::SetFOV: , SetPosition, SetOrientation: Modify the camera's properties either immediately or as part of an animation.
  - CameraControl::GetFOV: , GetPosition, GetOrientation: Retrieve the current camera properties.
  - CameraControl::GetCameraPosition: , GetCameraOrientation: Return the camera's position and orientation, falling back to internal state if no camera is set.
  - CameraListener: Implements ISoundListener to provide sound positioning based on the camera's orientation and position.
- lines 751-1123
  - TrekWindowImpl: Main class for implementing window functionality.
  - SuperKeyboardInputFilter: Filters keyboard input for special handling.
  - KeyboardInputFilter: General keyboard input filter.
  - QuickChatMenuCommandSink: Handles commands and selections from the quick chat menu.
  - PlaySonicChat: Plays a specific voice from a Sonic Chat entry.
  - GetSonicChatText: Retrieves text for a specific voice in a Sonic Chat entry.
  - OnSuperKeyFilter: Handles special key presses, particularly tab to cycle chat targets.
  - OpenMainQuickChatMenu: Opens the main quick chat menu.
- lines 1124-1477
  - TRef<SonicChatVoice>: Manages chat voice data.
  - ZString unknown("Unknown chat"): Default string for unknown chat messages.
  - m_frameID: Frame identifier for rendering updates.
  - m_timeLastFrame: Time of the last frame update.
  - m_timeLastDamage: Time of the last damage event.
  - m_fDeltaTime: Delta time between frames for timing calculations.
  - m_timeLastActivity: Timer for tracking last user activity.
  - m_bFFAutoCenter: Boolean to enable or disable auto-centering feature.
- lines 1478-1845
  - TMap<ZString, TRef<AnimatedImage> > m_mapAnimatedImages: A map to cache animated images.
  - LoadAnimatedImage(Number ptime, const ZString& str): Loads an animated image from a file or returns it from the cache if already loaded.
  - GameStateCloseSink: An event sink that handles game state close events.
  - InitializeGameStateContainer(): Initializes the game state container with various winning condition tiles.
  - TerminateGameStateContainer(): Cleans up resources used by the game state container.
  - WinConditionInfo: Abstract base class for win condition information.
  - ConquestWinConditionInfo, TerritoryWinConditionInfo, DeathMatchWinConditionInfo: Concrete classes implementing specific win conditions.
  - ShipWinConditionInfo: Abstract base class for ship-specific win condition information.
- lines 1846-2202
  - ClearLine: Clears specific columns of a grid for a given row.
  - Greater: Compares two ships or sides based on their kills and deaths (for DeathMatch) or artifacts and flags (for other conditions).
  - AnyInfo: Checks if there is any relevant information to display.
  - FillWinCondition: Populates a string grid with win condition data for sides and ships.
  - UpdateGameStateContainer: Updates the game state container by filling various win condition grids.
  - InitializeSoundTemplates: Initializes sound templates using the modeler.
  - GetUI: Returns whether the current screen is not combat.
  - SetScreen: Sets the screen and updates the UI window.
- lines 2203-2574
  - MatrixTransform2: Represents a transformation matrix for screen rendering.
  - TransformImage: Manages image transformations and rendering.
  - GroupImage: Combines multiple images into a single group.
  - PaneImage: Creates an image from a pane.
  - ScreenID: Enumerates different types of screens in the game.
  - SetUiScreen: Sets the user interface screen.
  - SetCombatScreen: Sets the combat screen.
  - screen: Handles switching between different screens, managing resources and state.
- lines 2575-2937
  - IsInputEnabled(int vk): Determines if a given virtual key (vk) should be considered enabled.
  - PlayMovieClip(bool playMovies, bool isWindowed, ZString moviePath): Plays a video clip in a separate thread if certain conditions are met.
  - TrekWindowImpl(EffectApp papp, const ZString& strCommandLine, const ZString& strArtPath, bool bMovies): Constructor for the main game window class, handling initialization and setup.
  - IsInputEnabled(int vk): Checks if a key is enabled based on current screen state and composition status.
  - PlayMovieClip(bool playMovies, bool isWindowed, ZString moviePath): Handles playing video clips in a separate thread under specific conditions.
  - TrekWindowImpl(EffectApp papp, const ZString& strCommandLine, const ZString& strArtPath, bool bMovies): Initializes the main game window with various settings and configurations.
  - TODO: BT - 9/17 - Made this a function to support chaining the opening microsoft splash with the longer classic allegiance movie.
  - TODO: BT - 10/17 - Set to always start windowed, then go full screen after game is initialized. Trying to find the source of the mystery "crash on launch" issues.
- lines 2939-3331
  - Camera: Manages camera properties and transformations.
  - Geo::GetEmpty(): Returns an empty geometry object.
  - WrapGeo: Wraps a geometry object to allow for additional transformations or behaviors.
  - MatrixTransform: Applies matrix transformations to a geometry object.
  - KeyboardInputFilter: Filters keyboard input based on certain criteria.
  - SuperKeyboardInputFilter: A specialized keyboard input filter for the game.
  - CreateJoystickImage: Creates an image representing a joystick.
  - InitializeImages(): Initializes various images used in the game.
- lines 3333-3690
  - TrekWindowImpl: Main window implementation class.
  - InitializeImages(): Initializes various image resources.
  - SetCursor(const char pszCursor): Sets the cursor to a specified image.
  - RestoreCursor(): Restores the cursor from a resource.
  - SavePreference(const ZString& szName, DWORD dwValue): Saves an integer preference value.
  - LoadPreference(const ZString& szName, DWORD dwDefault): Loads an integer preference value with a default.
  - IsWine(): Checks if the application is running under Wine.
  - Terminate(): Cleans up and terminates various resources.
- lines 3691-4032
  - contextRejectPlayer: Handles player rejection logic.
  - contextMakeLeader: Sets a player as the leader of their team.
  - contextMute: Toggles the mute status of a player.
  - contextKickPlayer: Kicks a player from the game by sending a quit message.
  - contextBanPlayer: Bans a player from the game by sending a quit message.
  - contextDockDrone: Toggles whether a player's ship stays docked.
  - contextChat: Initiates a chat with the selected player's ship.
  - Error: Displays an error message and exits fullscreen mode.
- lines 4034-4386
  - TakeScreenShot(): Captures and saves a screenshot of the screen as a 24-bit bitmap.
  - ShowMainMenu(): Creates and displays the main menu with various options like help, options, game details, and quit.
  - IsPlayerSteamModerator(): Checks if the current player is a Steam moderator by verifying their group ID.
  - ShowPlayerContextMenu(PlayerInfo  playerInfo): Displays a context menu for a player in the lobby, offering options like accept, reject, make leader, mute, kick, or ban (if moderator).
  - ShowPlayerPaneContextMenu(PlayerInfo  playerInfo): Displays a context menu for a player in the player pane, offering options like dock/launch, chat, and accept/reject based on side and ship type.
  - TODO: Move Steam-related globals to where other globals are hiding.
  - Unusual code: The screenshot functionality is currently disabled and commented out, with Steam's SteamScreenshots API being used instead.
- lines 4387-4740
  - CreateMenu: Creates a new menu.
  - AddMenuItem: Adds an item to a menu.
  - OpenPopup: Opens a popup menu at a specified position.
  - GetMousePosition: Retrieves the current mouse position.
  - IsPlayerSteamModerator: Checks if the player is a Steam moderator.
  - ShowOptionsMenu: Displays the main options menu.
  - AddAvailablePlayerTagsToMenu: Adds available player tags to a menu.
  - SetPlayerCallsign: Sets the player's callsign based on an index.
- lines 4741-5104
  - ToggleFilterUnknownChats: Toggles filtering of unknown chats and saves the preference.
  - ToggleFilterLobbyChats: Allows setting lobby chat filter options (1, 2, or 3) and updates preferences accordingly.
  - IncreaseChatLines: Increases the number of chat lines displayed and updates preferences.
  - ReduceChatLines: Decreases the number of chat lines displayed and updates preferences.
  - CycleChatSize: Cycles through different chat font sizes and updates preferences.
  - ToggleLinearControls: Toggles linear control response and saves the preference.
  - ToggleStars: Toggles visibility of stars in the UI and saves the preference.
  - TogglePosters: Toggles visibility of posters in the UI and saves the preference.
- lines 5106-5468
  - SetDeadzone(DWORD value): Adjusts the joystick deadzone based on a given value.
  - ToggleVirtualJoystick(): Toggles the virtual joystick feature and saves the preference.
  - CloseNotificationSink: A class implementing IIntegerEventSink to handle close notifications.
  - ToggleOldUi(): Toggles between old and new UI, displays a message box with a close button.
  - ToggleShowJoystickIndicator(): Toggles the display of the joystick indicator and saves the preference.
  - ToggleShowGrid(): Toggles the visibility of the grid (only applicable during loading).
  - SetRadarLOD(DWORD value): Sets the radar level of detail (only applicable during loading).
  - SetGamma(ZString& value): Sets the gamma level for rendering, saving the preference.
- lines 5470-5822
  - AdjustVoiceOverVolume: Adjusts voice over volume with a delta value.
  - AdjustFFGain: Adjusts forward firing (FF) gain with a delta value.
  - AdjustMouseSens: Adjusts mouse sensitivity with a delta value.
  - GetPostersMenuString: Returns menu string for posters setting.
  - GetDebrisMenuString: Returns menu string for debris density setting.
  - GetEnvironmentMenuString: Returns menu string for environment setting.
  - GetMaxTextureSizeMenuString: Returns menu string for maximum texture size setting.
  - GetMouseSensMenuString: Returns menu string for mouse sensitivity adjustment.
- lines 5823-6175
  - CloseMenu(): Closes the current menu.
  - DoInputConfigure(): Opens a popup for configuring input settings.
  - OnGameState(): Opens a popup with mission parameters.
  - ShowPlayerPings(): Displays connection information of players.
  - OnMenuCommand(IMenuItem pitem): Handles menu commands based on user selection.
  - GetChatHistoryHUDMenuString(): Returns the current state of chat history HUD as a string.
  - GetCenterHUDMenuString(): Returns the current state of center HUD as a string.
  - GetTargetHUDMenuString(): Returns the current state of target HUD as a string.
- lines 6177-6546
  - ToggleStars: Toggles the display of stars.
  - ToggleEnvironment: Toggles the environment rendering.
  - ToggleOldUi: Switches between old and new UI modes.
  - ToggleShowJoystickIndicator: Shows or hides joystick indicator.
  - ToggleBandwidth: Adjusts bandwidth settings (commented out).
  - ToggleMaxTextureSize: Increases texture size.
  - ToggleAA: Toggles anti-aliasing.
  - ToggleCensorChats: Censors chat messages.
- lines 6547-6908
  - SetCameraMode: Changes the camera mode based on the input parameter and updates various settings accordingly.
  - GetViewMode: Returns the current view mode.
  - SetViewMode: Sets the view mode and handles associated changes like updating overlays, resetting popup containers, and setting camera modes.
  - EnableDisplacementCommandView: Enables or disables a displacement command view feature.
  - GetOverlayFlags: Retrieves the current overlay flags for the current view mode.
  - SetOverlayFlags: Sets the overlay flags for the current view mode and updates associated UI elements.
  - TurnOnOverlayFlags: Turns on specified overlay flags.
  - TurnOffOverlayFlags: Turns off specified overlay flags.
- lines 6909-7260
  - UpdateCamera: Updates the camera position and orientation based on ship movement.
  - InternalCamera: Handles internal camera views like chase view.
  - CommandCamera: Manages command view updates.
  - ExternalCamera: Controls external camera modes such as target, fly-by, orbit, etc.
  - ComputeExternalChase: Calculates the camera position for a chase view.
  - TrackCamera: Positions the camera to track a specific target or missile.
  - m_turnRateSamples: Array of samples for turn rates.
  - m_turnRateSampleIndex: Index for current sample in turn rate array.
- lines 7261-7616
  - ComputeExternalChase: Computes external chase view for a ship.
  - AddMuzzleFlare: Adds a muzzle flare at a given direction and duration.
  - GetFPSString: Generates a string with FPS, MSPF, and other performance metrics.
  - StartTraining: Starts a training mission.
  - OrientCommandView: Sets the orientation of the command view camera.
  - GetDefaultPosition: Gets the default position for the command view.
  - PositionCommandView: Positions the command view based on target or default.
  - UpdateCommandView: Updates the command view's position and orientation.
- lines 7617-7976
  - m_ptrekInput->SetInputSite(this): Initializes input handling for the current screen.
  - UpdateMouseEnabled(): Enables or disables mouse interaction based on game state.
  - m_pscreen->OnFrame(): Calls a frame update method on the current screen object.
  - CheckCountdownSound(): Handles any countdown sounds that need to be played.
  - trekClient.HandleAutoDownload(): Manages automatic downloads, adjusting time slices based on fullscreen mode.
  - ReceiveMessages(): Processes network messages and updates game state accordingly.
  - DoTrekUpdate(): Updates the game world based on elapsed time and frame ID.
  - m_pSoundEngine->Update(): Updates sound engine to reflect current game state.
- lines 7977-8325
  - ImodelIGC: Interface for game objects.
  - IasteroidIGC: Interface for asteroid objects.
  - IafterburnerIGC: Interface for afterburner components.
  - ThingGeo: Represents graphical representation of a thing.
  - TRef<ThingGeo>: Smart pointer to ThingGeo.
  - ThingSitePrivate: Private implementation details for ThingSite.
  - SetTarget: Sets the target for a ship.
  - FindTarget: Finds a suitable target based on criteria.
- lines 8326-8681
  - WinTrek.cpp: Main file for handling input and camera controls.
  - bControlsInUse: Boolean indicating if any joystick axes are being used.
  - trekClient: Instance of a client managing game state and ship controls.
  - js.controls.jsValues: Joystick values for yaw, pitch, roll, and throttle.
  - trekClient.trekJoyStick: Current joystick state from the client.
  - g_fJoystickDeadZone: Threshold for joystick movement to be considered active.
  - Training::ApproveControls: and Training::ApproveActions: Functions for training mission hooks.
  - m_cameraControl.SetFOV: Sets the field of view based on throttle input.
- lines 8683-9036
  - Color: Represents a color with RGBA values.
  - Time: A class representing time, likely used for timing and synchronization.
  - IInputProvider: An interface for input providers.
  - KeyState: A struct representing the state of a key press.
  - TrekKey: An enum representing different keys in the game.
  - CommandID: An enum representing different command IDs.
  - OT_ship: An enum representing an object type, specifically a ship.
  - NA: A constant representing "not applicable" or "invalid".
- lines 9037-9392
  - trekClient: Manages client-side game state and interactions.
  - Mount: Enumerates weapon mount positions on a ship.
  - ET_Weapon: Enumerates weapon equipment types.
  - HelpPosition: Manages the position of help UI elements based on time.
  - PagePaneIncluderImpl: Implementation of an interface for including pages in a help pane.
  - GetVersionString: Returns the version string of the game.
  - InitializeHelp: Initializes the help system with a help pane and position.
  - OnHelp: Toggles the visibility of the help UI.
- lines 9393-9742
  - suicideF: A boolean flag that seems to indicate a suicide condition.
  - switch(tk): A switch statement based on the input key tk.
  - TK_ChatPageUp and TK_ChatPageDown: Handles page up and down for chat list pane.
  - TK_ToggleMouse: Toggles mouse overlay flags and virtual joystick enablement.
  - TK_TrackCommandView: Toggles command view tracking and posts a message accordingly.
  - TK_StartChat: Starts chat and sets focus on trek input.
  - TK_ToggleLODSlider: Toggles the level of detail slider between empty and current image.
  - TK_LockVector and TK_ToggleCloak: Handle vector locking and cloak activation based on ship conditions.
- lines 9743-10081
  - m_cm: Current camera mode.
  - m_cameraControl: Object for controlling the camera.
  - pi: Constant representing π (pi).
  - SetHeadOrientation(float angle): Function to set the head orientation of the camera.
  - GetCameraMode(): Function to get the current camera mode.
  - SetCameraMode(CameraMode cm): Function to set the camera mode.
  - GetViewMode(): Function to get the current view mode.
  - SetViewMode(ViewMode vm): Function to set the view mode.
- lines 10082-10428
  - TK_ConModeTeleport: Handles teleportation mode.
  - TK_TargetHostile: Targets the ship that has done the most damage recently.
  - TK_TargetCenter: Targets the ship closest to the reticle.
  - TrekFindTarget: Finds and sets a target based on given criteria.
  - TK_ToggleAutoPilot: Toggles the autopilot mode.
  - TK_VoteYes: Votes yes in a vote.
  - TK_VoteNo: Votes no in a vote.
  - TK_ExecuteCommand: Executes a queued command.
- lines 10429-10771
  - suicideF: A boolean flag indicating if a suicide command has been initiated.
  - m_bCommandGrid: A boolean flag to toggle the visibility of a grid in the game interface.
  - SetTarget: Function to set the target for a ship, taking parameters for the target model and command ID.
  - RejectQueuedCommand: Function to reject a queued command, optionally clearing it and sending a negative response.
  - AcceptQueuedCommand: Function to accept a queued command, executing or setting up the command based on its type.
  - SetQueuedCommand: Overloaded function to set a queued command with either a model target or money request.
  - m_pchatListPane: A reference to a chat list pane object.
  - SetChatListPane: Function to set the chat list pane object.
- lines 10772-11120
  - ChatTarget GetLobbyChatTarget(): Returns the current lobby chat target.
  - float MapJoystick(float value): Maps joystick input values to a usable range.
  - void SwitchToJoyThrottle(void): Determines whether to use joystick or keyboard throttle control.
  - bool SenseJoystick(JoystickResults js, bool bInternalCamera, bool bReadKeyboard, bool &bHandleThrottle): Reads and processes joystick and keyboard inputs.
  - HRESULT HandleMsg(FEDMESSAGE pfm, Time lastUpdate, Time now): Handles incoming messages from the game server.
  - void TerminateModelEvent(ImodelIGC model): Processes events related to model termination.
  - void OnDelPlayer(MissionInfo pMissionInfo, SideID sideID, PlayerInfo pPlayerInfo, QuitSideReason reason, const char szMessageParam): Handles player disconnection events.
  - "// NYI" in HandleMsg function for FM_S_TREASURE_SETS case.
- lines 11121-11359
  - TrekWindowImpl::CheckCountdownSound: Manages countdown sounds during missions.
  - WinTrekClient::ForwardSquadMessage: Forwards squad messages to the squad screen.
  - WinTrekClient::ForwardCharInfoMessage: Forwards character info messages to the Character Info screen.
  - WinTrekClient::ForwardLeaderBoardMessage: Forwards leaderboard messages to the Leaderboard screen.
  - VTSetText: Updates text in a virtual text edit control.
  - StartSound: Starts playing a sound using a template or ID.
  - StartLockDown: Displays a lockdown message box.
  - EndLockDown: Closes the lockdown message box.

## `src/WinTrek/WinTrek.h`

**PURPOSE:** This file is the main header file for the WinTrek client application, defining the primary class `WinTrekClient` and other related classes and constants.

**KEY PARTS:**
- **Class: WinTrekClient**
  - Manages the overall state and behavior of the WinTrek client.
  - Handles game initialization, termination, sound effects, explosions, and various game events.
  
- **Function: ConnectToServer**
  - Establishes a connection to the server using provided connection information.

- **Function: OnLogonAck**
  - Processes acknowledgment from the server after a logon attempt.

- **Function: PlaySoundEffect**
  - Plays sound effects based on the given `SoundID`.

- **Constant: c_PopupX, c_PopupY**
  - Defines the coordinates for popup dialogs within the game window.

**CONNECTS TO:** 
- Other client-side components such as `TrekWindow`, `Engine`, and `Modeler`.
- Network-related classes for handling server communication.
- Sound management classes for playing various sounds in the game.

**NOTES:**
- Contains commented-out code related to `BillboardType` which was likely used for visual effects but is no longer active.
- Includes conditional compilation directives for compatibility with different versions of Visual C++ and Windows SDKs.
- Defines global variables such as `trekClient`, `g_bActivity`, and `g_pzfFrameDump`.
- Contains macros like `debugClk()` for debugging purposes.

## `src/WinTrek/ZoneClubScreen.cpp`

**PURPOSE:** The `ZoneClubScreen.cpp` file manages the user interface and functionality for the Zone Club screen in Allegiance. It handles authentication, navigation to different screens, and downloading configuration and message of the day files.

**KEY PARTS:**
- **Class: ZoneClubScreen**
  - Manages the Zone Club screen UI.
  - Handles button clicks for various actions like returning to main menu, navigating to different services, and initiating logon.
  - Manages authentication using Passport or local configuration file.
  
- **Function: ConnectToZone**
  - Connects to either the Zone Lobby server or the Zone Club server based on user input.
  - Prompts for login information if necessary.

- **Function: OnButtonMainMenu**
  - Handles button click event for returning to the main menu screen.

- **Function: BeginConfigDownload**
  - Initiates downloading a configuration file from a specified URL, skipping if already downloaded or in quickstart mode.

- **Function: OnConfigDownloadDone**
  - Manages the process after a configuration file download completes, handling loading and auto-update.

- **Function: OnMessageOfDayDone**
  - Handles the completion of downloading the message of the day, either successfully or with errors.

- **Function: OnAutoUpdateSystemTermination**
  - Manages the termination of the auto-update system, enabling buttons and handling post-download actions.

- **Function: OnLogon**
  - Initiates the logon process by setting user credentials and connecting to either the lobby or club.

- **Data Member: m_szName, m_szPW, m_szPWOrig**
  - Stores user's name and password for authentication purposes.

**CONNECTS TO:** 
- `Screen`, `LogonSite`, `TrekClientEventSink`, `IIntegerEventSink`, `IAutoUpdateSink`, and `EventTargetContainer<ZoneClubScreen>` (inherited classes)
- Zone authentication client
- HTTP download system

**NOTES:**
- Contains TODO comments indicating areas needing further development.
- Includes commented-out sections for Steam integration and quickstart scenarios.
- Uses assert statements to ensure runtime conditions are met.

## `src/WinTrek/ZoneSquad.cpp`

PURPOSE:
The file `ZoneSquad.cpp` contains the implementation of interfaces and classes related to managing player squads in a game. It includes functionality for creating, modifying, and querying squad details, as well as managing individual players' statuses within those squads.

KEY PARTS:
- CZonePlayerImpl: Manages individual zone players with attributes like name, ID, rank, and last played date.
- CMembership: Tracks the status of a player's membership in a squad, including their position (member, leader, etc.) and last played date.
- CZoneSquadImpl: Represents a zone squad with properties like name, description, owner, and member list. Provides methods to add/remove players, set ownership, and manage player statuses.

CONNECTS TO:
Depends on other game systems for data storage, networking, and user interface updates.

NOTES:
- Contains debug code that sets random player names and ranks.
- TODO: Convert `m_listStrOwnershipLog` from a list to a simple string.
- Asserts are used in several functions to ensure proper state before performing operations.

## `src/WinTrek/ZoneSquad.h`

PURPOSE: This file defines interfaces and classes related to player squads in the Allegiance game. It includes methods for managing squad details, player membership, and interactions between players and squads.

KEY PARTS:
- IZoneSquad: Interface defining operations for a zone squad, including adding/removing players, setting ranks, and tracking wins/losses.
- IZonePlayer: Interface defining operations for a zone player, including getting and setting player details, checking player status within a squad, and managing player positions.

CONNECTS TO: Other game systems handling player data and interactions.

NOTES: None.

## `src/WinTrek/artwork.h`

PURPOSE: This file contains #defines for all the artwork files used in the Allegiance game, including control bitmaps, mission creation bitmaps, loadout bitmaps, flight bitmaps, main screen elements, missions screen elements, team room elements, win/lose screen elements, mouse cursors, explosions, and shield flares.

KEY PARTS:
- AWF_CONTROL_UP_ARROW: Bitmap for the up arrow control.
- AWF_CREATEMISSION_BACKGROUND: Bitmap for the background of the create mission screen.
- AWF_LOADOUT_BACKGROUND: Bitmap for the background of the loadout screen.
- AWF_FLIGHT_ME_ICON: Bitmap for the player's ship icon in flight.
- AWF_HUD_SPEED_INDICATOR: Bitmap for the speed indicator on the HUD.
- AWF_MAIN_BACKGROUND: Bitmap for the main game screen background.
- AWF_MISSION_DETAILS_BUTTON: Bitmap for the mission details button.

CONNECTS TO: artwork files (.bmp and .x) used throughout the game.

NOTES: None.

## `src/WinTrek/cdkeydialog.cpp`

PURPOSE: The file `cdkeydialog.cpp` implements a dialog for entering and validating a CD key in the Allegiance game.

KEY PARTS:
- `CDKeyDialogImpl`: A class that manages the CD Key dialog, including setting up UI components, handling user input, and validating the entered CD key.
- `OnButtonOK`: Handles the OK button click event to validate and save the CD key.
- `OnButtonCancel`: Handles the Cancel button click event to close the dialog without saving.
- `FormatKey`: Formats the CD key by removing spaces and converting it to uppercase.
- `CDKeyIsValid`: Validates the entered CD key (currently a simple check for non-empty strings).

CONNECTS TO: The game's client (`trekClient`) for accessing and updating the CD key.

NOTES:
- Contains a TODO comment to implement a more thorough CD key validation.

## `src/WinTrek/chatpane.cpp`

PURPOSE: The file `chatpane.cpp` contains the implementation for a chat list pane in Allegiance, which is responsible for displaying and managing chat messages.

KEY PARTS:
- ChatListItem: Represents an individual chat message item with methods to draw and update its display.
- ChatListPaneImpl: Manages the overall chat list pane, including adding and removing chat items, handling user interactions, and updating the layout.

CONNECTS TO: trekctrls.h, trekmdl.h, badwords.h, ListPaneOld, TrekResources, IKeyboardInput

NOTES: None.

## `src/WinTrek/clientdrones.cpp`

PURPOSE: tiny file (2 lines), shown in full rather than summarized.

    #include "pch.h"
    #include "drones.cpp"

## `src/WinTrek/cmdview.cpp`

PURPOSE: The file `cmdview.cpp` is part of the Allegiance game's user interface and handles rendering various graphical elements such as a grid, ships, and text strings in a 3D space.

KEY PARTS:
- CommandGeo: Manages the drawing of a grid, ships, and drop lines.
- DrawShips: Renders ships with drop lines and feet.
- DrawTop: Draws a reference string indicating the top of the sector.
- Render: Orchestrates the rendering process by calling other draw functions.
- SetCluster: Sets the cluster of models to be rendered.
- ImageStringPane: A container for text and an optional image, styled differently based on its state (up or down).
- ImageStringButtonPane: Extends ImageStringPane with a button functionality.

CONNECTS TO:
- trekClient: Provides access to game objects like ships and stations.
- TrekResources: Loads fonts and surfaces used in rendering.
- Context: Manages the drawing context for 3D graphics.

NOTES:
- Contains hardcoded colors and dimensions specific to the game's UI.
- The commented-out code related to `m_top` suggests it was intended to display an image but is currently not functional.

## `src/WinTrek/cmdview.h`

PURPOSE: This file contains definitions for various UI components and a geometry class used in the Allegiance game's command view.

KEY PARTS:
- ImageStringPane: A pane that displays text with an image, either on the left or right.
- ImageStringButtonPane: An extension of ImageStringPane that includes a button below the text/image pair.
- CommandGeo: A geometry class responsible for rendering the command view, including ships and grid lines.

CONNECTS TO: Pane, IclusterIGC, Context, Surface

NOTES: None

## `src/WinTrek/console.cpp`

PURPOSE: Manages console interactions and chat functionality within the Allegiance game.

KEY PARTS:
- ConsoleDataImpl: Manages console data and state updates.
- ChatSubject: Handles chat subjects and recipients.
- LegalRectangle: Ensures minimum distance between points.
- ComposeState: Enumerates states for composing text (chat, command).
- ModelData: Stores model-related data.
- EventSourceImpl: Implements event handling.

CONNECTS TO:
- Game logic and state management
- User interface components

NOTES:
- Contains TODO comments indicating areas for debugging or future improvements.

PARTS (in file order):

- lines 1-353
  - ConsoleDataImpl: Likely a private implementation class for managing console data.
  - ConsolePickImage: A custom image class that handles mouse interactions and hit testing.
  - ChatSubject: Manages chat subjects, including adding, deleting, and toggling ship subjects in a chat group.
  - LegalRectangle: Checks if the distance between two points is greater than a minimum size.
  - c_iRecipientOther, c_iRecipientMe, etc.: Constants defining different recipient types for messages or commands.
  - ChatTarget: Enumerates possible chat targets like individual, team, allies, etc.
  - SetRecipient: Method in ChatSubject to set the recipient of a chat message based on various conditions.
  - AddSubject, DeleteSubject, ToggleSubject: Methods in ChatSubject to manage ship subjects in a chat group.
- lines 354-733
  - ChatSubject: Manages chat recipients and subjects.
  - ConsoleDataImpl: Manages console data and state.
  - ComposeState: Enumerates states for composing text (chat, command, etc.).
  - TRef: Template class for reference counting.
  - ModifiableNumber: Class for modifiable numeric values.
  - ModifiableString: Class for modifiable string values.
  - ModelData: Class for model data.
  - EventSourceImpl: Implementation of event source.
- lines 735-1098
  - ConsoleDataImpl: Manages console data and updates UI elements.
  - Update: Updates various UI elements based on game state.
  - SetTarget: Sets the current target model.
  - SetOverlayFlags: Sets overlay flags and adjusts verbs accordingly.
  - OnSwitchViewMode: Handles switching view modes.
  - SetTipText: Sets regular tip text.
  - SetCriticalTipText: Sets critical tip text with scrolling effect.
  - SetComposeState: Sets compose state for UI interactions.
- lines 1100-1454
  - ComposeState GetComposeState(): Returns the current compose state.
  - OnMoneyChange(PlayerInfo pPlayerInfo): Handles money change events for a player.
  - OnDelPlayer(MissionInfo pMissionDef, SideID sideID, PlayerInfo pPlayerInfo, QuitSideReason reason, const char szMessageParam): Handles player deletion events during missions.
  - OnBackButton(): Handles the back button press event, changing the view mode based on whether the game is in training or not.
  - OnRecipientButton(int nColumn): Sets the recipient of a chat message based on the button pressed.
  - OnWingCombo(int index): Calls OnRecipientButton for wing recipients using an index.
  - OnVerbButton(int nColumn): Handles verb buttons, setting the compose state to command and updating the composed chat.
  - UpdateComposedChat(): Updates the displayed chat text and recipient based on the current compose state and settings.
- lines 1455-1809
  - ObjectType otTarget: Represents the type of the target object.
  - ObjectID oidTarget: Represents the ID of the target object.
  - ChatTarget ct: Enumerates different types of chat targets (e.g., group, individual).
  - IshipIGC pship: Pointer to a ship object.
  - ImodelIGC pmodel: Pointer to a model object.
  - CommandID cid: Represents a command ID for actions like attack or build.
  - AbilityBitMask aabm: Bitmask representing abilities of an entity.
  - ChatSubject m_chsCurrent: Current chat subject, managing the state of the chat composition.
- lines 1811-2159
  - UpdateComposedChat(): Updates the composed chat message based on current state.
  - IsComposingCommand(): Checks if the current composition state is for a command or mouse command.
  - GetPickedObject(): Returns the currently picked object.
  - PickModel(ImodelIGC pmodel, int button): Handles picking a model with a mouse click.
  - ResolvePick(IclusterIGC pcluster, const Point& pointPick, int button): Resolves which model was clicked based on position and button.
  - PickObject(IclusterIGC pcluster, const Point& pointPick, int button): Picks an object or creates a waypoint based on input.
  - Button(IInputProvider pprovider, const Point& point, int button, bool bCaptured, bool bInside, bool bDown): Handles mouse button events for navigation and selection.
- lines 2160-2505
  - StartComposing: Initiates composing a chat message.
  - QuitComposing: Ends the current composition of a chat message.
  - UpdateComposedChat: Updates the display of the composed chat message.
  - SetComposeState: Sets the state of the chat composition.
  - SetVerb: Sets the command verb for the chat composition.
  - SendChat: Sends the composed chat message.
  - OnChar: Handles character input during chat composition.
  - StartChat: Starts a new chat session.
- lines 2506-2840
  - MatchName: Compares two strings and returns a score indicating how well they match.
  - SetChatObject: Sets the current chat target to the specified model or cluster.
  - OnBackspace: Handles backspace key press in chat input.
  - CycleChatTarget: Cycles through possible chat recipients based on current state.
  - OnTab: Handles tab key press for autocomplete-like functionality in chat input.
  - c_badMatch: A constant indicating an invalid match score.
  - CHAT_WING, CHAT_INDIVIDUAL, etc.: Constants representing different types of chat recipients.
  - TODO: OutputDebugString calls are commented out, suggesting they were used for debugging and should be removed or uncommented as needed.
- lines 2841-3177
  - OnTab: Handles tab completion for chat recipients and commands.
  - MatchName: Compares a string to possible matches (ship names, allies, etc.) and returns a score indicating how close the match is.
  - SetComposeState: Changes the state of the chat composition process.
  - SetVerb: Sets the current verb being composed in the chat.
  - DisplayImage: A class for displaying images with animation and visibility control.
  - ConsolePickImage: Handles mouse input events for the console interface.
  - TODOs: "WLP 2005 - Only talk to enemy pilots - not cons" indicates a potential issue or improvement related to chat recipient selection. "//NYI need ome way to tell the image that it no longer needs to be updated" suggests an unimplemented feature for managing image updates.
- lines 3178-3511
  - ConsolePickImage::MouseEnter: Handles mouse enter event.
  - ConsolePickImage::MouseLeave: Handles mouse leave event.
  - ConsoleImageImpl: Main implementation class for console image.
  - SetDisplayMDL: Loads and initializes display models based on a file name.
  - CreateDisplayImage: Creates and adds a display image to the group displays.
  - AddChildren: Adds child panes from an object stack to a parent pane.
  - TODOs: VS.Net 2003 port: error C2465 here.

## `src/WinTrek/console.h`

PURPOSE: This file contains declarations for various components related to the console interface in Allegiance, including classes and functions for managing console data, image overlays, and HUD panes.

KEY PARTS:
- ConsoleData: Interface for setting target models, tips, picking objects, and checking composition status.
- ConsoleImage: Base class for console images with methods to update, set overlay flags, get viewport information, and handle input events.
- CreateTeamPane, CreateExpandedTeamPane, CreateTeleportPane: Functions to create specific HUD panes.

CONNECTS TO: Other files related to the game's UI and state management.

NOTES: None.

## `src/WinTrek/consoledata.cpp`

PURPOSE: The `consoledata.cpp` file contains classes and methods for managing and retrieving data related to models (ships and stations) and their parts within the Allegiance game.

KEY PARTS:
- ModelData: Manages properties of models, including speed, energy, ammo, fuel, hit points, and shields.
- PartWrapper: Handles properties and behaviors of individual parts on a model, such as weapon damage, range, and readiness state.

CONNECTS TO: Other systems or files it depends on or serves:
- Depends on game engine components for data retrieval and manipulation.

NOTES: None.

PARTS (in file order):

- lines 1-339
  - ModelData: Class containing methods for retrieving various properties of a model (ship or station).
  - GetModel(): Returns the model associated with the object.
  - GetModelTypeInternal() and GetModelType(): Retrieve the type of the model as an enum and float respectively.
  - GetModelTypeDesc(): Provides a description of the model's type, considering visibility and last seen data.
  - GetSpeed(): Calculates the speed of the model if it is not static.
  - GetMaxThrust(), GetMass(), GetRange(): Specific methods for retrieving properties related to movement and size.
  - GetPercentHitPoints() and GetNumHitPoints(): Methods for determining the health status of the model.
  - GetPercentShields() and GetNumShields(): Retrieves information about the shields' current state.
- lines 340-678
  - ModelData::GetMaxEnergy: Returns the maximum energy of the ship.
  - ModelData::GetNumEnergy: Returns the current energy level of the ship as a percentage.
  - ModelData::GetAmmo: Returns the current ammo level as a percentage of the max ammo.
  - ModelData::GetMaxAmmo: Returns the maximum ammo capacity of the ship.
  - ModelData::GetNumAmmo: Returns the current ammo count.
  - ModelData::GetFuel: Returns the current fuel level as a percentage of the max fuel.
  - ModelData::GetMaxFuel: Returns the maximum fuel capacity of the ship.
  - ModelData::GetNumFuel: Returns the current fuel amount.
- lines 679-1021
  - PartWrapper::GetPartName: Returns the name of the part.
  - PartWrapper::GetRange: Calculates the range of a weapon or magazine.
  - PartWrapper::GetDamage: Calculates the damage output of a weapon or magazine.
  - PartWrapper::GetRate: Returns the rate of fire for a weapon.
  - PartWrapper::GetCount: Determines the count of parts, considering cargo and pack types.
  - PartWrapper::GetAfterburnerFuelConsumption: Calculates fuel consumption for afterburners.
  - PartWrapper::GetAfterburnerFuelLeft: Computes remaining fuel for afterburners.
  - PartWrapper::GetAfterburnerTopSpeed: Returns the top speed achievable with afterburners.
- lines 1022-1200
  - PartWrapper::IsStationKiller: Returns 0 for all cases.
  - PartWrapper::IsAsteroidKiller: Returns 0 for all cases.
  - PartWrapper::IsSelected: Determines if a weapon is selected based on mount ID and client state.
  - PartWrapper::IsActive: Checks if the part is active.
  - PartWrapper::IsOutOfAmmo: Determines if a weapon or launcher is out of ammo.
  - PartWrapper::GetReadyState: Returns the readiness state of the part.
  - PartWrapper::GetMountedFraction: Returns the fraction of the part that is mounted.
  - PartWrapper::GetArmedFraction: Returns the fraction of the launcher that is armed.

## `src/WinTrek/consoledata.h`

PURPOSE: This file contains definitions for classes that manage and snapshot the state of game objects such as ships, stations, asteroids, and parts. It provides mechanisms to track changes in these objects' properties and export them for use in other systems.

KEY PARTS:
- TMemberSnapshotValue: A template class that snapshots a member function's value from an object and updates it when the object changes.
- TMemberSnapshotValueFactory: A factory class that creates instances of TMemberSnapshotValue.
- ModelWrapper: Manages a model (ship, station, etc.) and notifies listeners of changes.
- ModelData: Extends ModelWrapper to provide specific accessors for various properties of models.
- PartWrapper: Manages a part (weapon, shield, etc.) and notifies listeners of changes.

CONNECTS TO: Other game systems that need to track or display the state of game objects.

NOTES: None.

## `src/WinTrek/ddstream.cpp`

PURPOSE: The file `ddstream.cpp` contains the implementation of a class `CDShow` that handles video playback using DirectShow and DirectDraw in a Microsoft game.

KEY PARTS:
- CDShow: Manages video playback using DirectShow.
- Open: Initializes and opens a multimedia stream for video playback.
- Draw: Draws the current frame of the video to a DirectDraw surface.
- Start: Begins playing the video from the beginning.
- Stop: Stops the video playback.
- Close: Closes the multimedia stream and cleans up resources.

CONNECTS TO: `pch.h`, `DirectShow` libraries, `DirectDraw`.

NOTES: The class uses COM interfaces for DirectShow operations. It initializes COM in a multithreaded mode and handles surface restoration if the surface is lost during blitting.

## `src/WinTrek/ddstream.h`

PURPOSE: This file defines a class `CDShow` for managing video playback using DirectShow in Allegiance.

KEY PARTS:
- CDShow: Manages the lifecycle and operations of a multimedia stream.
- Open: Initializes the media stream with a filename and DirectDraw pointer.
- Close: Terminates the media stream.
- Draw: Renders the current frame of the video to a DirectDraw surface.
- Start: Begins playing the video.
- Stop: Halts the video playback.
- IsPlaying: Returns whether the video is currently playing.

CONNECTS TO: `ddraw.h`, `mmstream.h`, `amstream.h`, and other DirectShow interfaces for handling multimedia streams.

NOTES: None.

## `src/WinTrek/ddvideo.cpp`

PURPOSE: This file contains the implementation of a DirectDraw video player for Allegiance, allowing in-game video playback.

KEY PARTS:
- DDVideo: Main class that handles video initialization, playback, and cleanup.
- InitDirectDraw: Initializes DirectDraw surfaces and sets cooperative level based on windowed mode.
- Play: Opens and starts video playback using DShow.
- DestroyDirectDraw: Releases all DirectDraw resources.
- DestroyDDVid: Cleans up the DShow object and calls DestroyDirectDraw.
- Flip: Flips the back buffer to the primary surface for display.

CONNECTS TO:
- ddstream.h: For DShow video handling.
- EngineSettings.h: For display settings like resolution and refresh rate.

NOTES:
- Uses LoadLibrary and GetProcAddress to dynamically load DirectDraw functions, which is unusual but allows compatibility with different versions of Windows.

## `src/WinTrek/ddvideo.h`

PURPOSE: This file defines a class `DDVideo` for managing video playback and rendering in the Allegiance game using DirectX.

KEY PARTS:
- DDVideo: Manages video playback and rendering.
- Play: Initializes DirectDraw and plays a video from a specified path.
- InitDirectDraw: Initializes DirectDraw objects.
- DestroyDirectDraw: Releases DirectDraw resources.
- DestroyDDVid: Cleans up video-related resources.
- Flip: Swaps the front and back buffers.

CONNECTS TO: `ddstream.h`, `CDShow`, DirectX libraries (`dxguid.lib`, `amstrmid.lib`).

NOTES: None.

## `src/WinTrek/downloader.cpp`

PURPOSE: This file contains the implementation of a download system for Allegiance, allowing the game to initiate and manage downloads from HTTP servers.

KEY PARTS:
- DownloadHandle: Manages the state and lifecycle of a download operation.
  - Initiate: Starts the download process.
  - AddCallback: Registers callbacks to be executed upon completion or error.
  - GetResultFilePath: Retrieves the path of the downloaded file.
  - Completed: Cleans up resources and executes registered callbacks.
  - OnError: Handles errors during the download.
  - OnFileCompleted: Notifies when a file has been successfully downloaded.

CONNECTS TO:
- HTTPSession: Interface for managing HTTP sessions and downloads.

NOTES:
- None.

## `src/WinTrek/downloader.h`

PURPOSE:
The file `downloader.h` defines the interface and implementation for downloading files over HTTP in Allegiance. It includes classes to handle download sessions and manage callbacks upon completion.

KEY PARTS:
- DownloadException: Custom exception class derived from std::runtime_error.
- DownloadHandle: Manages a single download session, including initiating downloads, handling callbacks, and error management.
  - Methods: Initiate(), AddCallback(), GetResultFilePath(), Completed(), OnError(), OnFileCompleted().
- Downloader: Provides a method to initiate a file download and returns a shared pointer to the DownloadHandle.

CONNECTS TO:
IHTTPSessionSink interface (likely defined elsewhere in the codebase), IHTTPSession, and other components that handle HTTP requests and responses.

NOTES:
None.

## `src/WinTrek/gameoverscreen.cpp`

**PURPOSE:** The `gameoverscreen.cpp` file manages the game over screen in Allegiance, displaying statistics and handling user interactions such as sending chat messages and navigating back to the team lobby.

**KEY PARTS:**
- **Class: GameOverScreen**: Manages the game over screen UI elements and handles events.
  - Inherits from `Screen`, `EventTargetContainer<GameOverScreen>`, `IItemEvent::Sink`, `TEvent<ZString>::Sink`, and `TrekClientEventSink`.
  - Initializes UI components, sets up event handlers, and configures player and side statistics.

- **Function: GameOverScreen::GameOverScreen(Modeler* pmodeler)**: Constructor that initializes the game over screen.
  - Loads screen elements from a modeler namespace, sets up event handlers for buttons and chat input, and configures player and side statistics.

- **Function: GameOverScreen::~GameOverScreen()**: Destructor that cleans up event sinks and restores focus to the previous keyboard input.

- **Function: GameOverScreen::SendChat(const ZString& strChat)**: Sends a chat message based on user input.
  - Handles different types of commands (admin, individual, everyone).

- **Class: PlayerPainter**: Custom painter for player statistics in the list view.
  - Inherits from `ItemPainter`.
  - Draws player details like position, name, rank, ratings, etc.

- **Class: SidePainter**: Custom painter for side statistics in the list view.
  - Inherits from `ItemPainter`.
  - Draws side details like position, name, ratings, etc.

**CONNECTS TO:** Other UI components and event handling systems within Allegiance.

**NOTES:**
- TODOs include setting AFK status and potential errors with redundant button calls.
- The XML generation part is incomplete.

## `src/WinTrek/gamescreen.cpp`

PURPOSE: The `gamescreen.cpp` file contains the implementation for managing the game screen and handling various UI interactions within Allegiance.

KEY PARTS:
- GameScreen: Manages the main game screen, including lobby connection, mission selection, and player interaction.
- FilterDialogPopup: Handles filtering games based on criteria such as zone club status.
- FindDialogPopup: Manages a popup dialog for finding players.
- DoLobbyConnect: Logic for connecting to the lobby server.
- OnEvent: Processes events related to mission selection.
- UpdateGameList: Updates the game list with sorted items based on user preferences.
- JoinMission: Joins a selected mission using either password or direct connection.

CONNECTS TO:
- HTTP session object (m_pSession)
- mdl tables for string values and skill levels

NOTES:
- None

PARTS (in file order):

- lines 1-367
  - GameScreen: Main class for the game screen, inherits from Screen and other interfaces.
  - GameItemPainter: Class responsible for painting individual items in a list.
  - FilterDialogPopup: Popup dialog for filtering games based on various criteria.
  - DrawIcon: Helper function to draw icons on the surface.
  - m_pSession: Pointer to an HTTP session object.
  - m_strPendingFileList: String holding pending file list information.
  - m_strWaitForFile: String indicating the file being waited for.
  - m_nRetriesLeft: Number of retries left before giving up.
- lines 368-755
  - FilterDialogPopup: Manages a popup dialog for filtering games.
  - FindDialogPopup: Manages a popup dialog for finding players.
  - GameScreen: Main class representing the game screen, handling various UI elements and interactions.
  - IsZoneLobby: Checks if the current lobby is a zone club.
  - OnButtonOK: Handles the OK button click in dialogs.
  - OnButtonCancel: Handles the Cancel button click in dialogs.
  - OnButtonJoin: Joins a selected game.
  - OnButtonNewGame: Starts a new game.
- lines 756-1122
  - GameScreen: Manages the game screen UI and handles lobby connection.
  - DoLobbyConnect: Handles the logic for connecting to the lobby.
  - OnEvent: Processes events related to mission selection.
  - OnRefreshTimer: Refreshes the game list timer.
  - OnSelectMission: Updates button states based on selected mission.
  - ResetFilters: Resets all filters to default settings.
  - GameScreenFilter: Filters missions based on various criteria.
  - MissionStatusColor: Determines the color status of a mission.
- lines 1123-1483
  - GameScreen: Manages the game selection screen.
  - NumPlayersCompare: Compares missions based on the number of players.
  - SettingsCompare: Compares missions based on their settings.
  - SortingList: Sorts a list using a given comparison function and reverses if specified.
  - UpdateGameList: Updates the game list with sorted items based on user preferences.
  - GetCountInLobby: Calculates the total number of players in the lobby.
  - OnListChanged: Handles changes to the game list, updating player count.
  - JoinMission: Joins a selected mission.
- lines 1484-1896
  - OnDelRequest: Handles deletion requests for missions.
  - OnCancelPassword: Cancels a password request and disconnects the client.
  - OnPassword: Joins a mission with a given password.
  - FindSkillLevelName: Determines the skill level name based on mission rank.
  - FindStringValue: Retrieves a string value from a table in mdl.
  - FindClosestValue: Finds the closest value in multiple mdl lists.
  - OnLogonGameServer: Initiates joining a game server.
  - OnLogonGameServerFailed: Handles failed game server logon attempts.

## `src/WinTrek/gamesite.cpp`

PURPOSE: tiny file (1 lines), shown in full rather than summarized.

    #include "pch.h"

## `src/WinTrek/gamesite.h`

PURPOSE: tiny file (7 lines), shown in full rather than summarized.

    #ifndef _GAMESITE_H_
    #define _GAMESITE_H_
    int GameSite::MessageBox(const ZString& strText, const ZString& strCaption, UINT nType)
    {
        return GetWindow()->MessageBox(strText, strCaption, nType);
    }
    #endif

## `src/WinTrek/gamestartingscreen.cpp`

PURPOSE: This file defines the GameStartingScreen class, which handles the user interface and logic for the game's starting screen.

KEY PARTS:
- Name: GameStartingScreen - Manages the game's starting screen UI.
- Name: OnButtonQuit - Handles the quit button click event, transitioning to the team screen.
- Name: GetPane - Returns the main pane of the starting screen.

CONNECTS TO: Screen and EventTargetContainer<GameStartingScreen> for managing the screen and handling events.

NOTES: None.

## `src/WinTrek/gametypes.cpp`

PURPOSE: This file defines game types and manages their initialization and retrieval based on mission parameters.

KEY PARTS:
- GameTypeImpl: A concrete implementation of the GameType interface that holds specific game type properties and logic.
- VerifyGameTypeInitialization: Initializes a list of predefined game types if not already done.
- FindType: Searches for a game type that matches the given mission parameters.
- GetGameTypes: Returns the list of all initialized game types.

CONNECTS TO: gametypes.h, newgamescreen.cpp

NOTES:
- Contains hardcoded game type definitions.
- Includes an experimental game type with a special flag.

## `src/WinTrek/gametypes.h`

PURPOSE: tiny file (17 lines), shown in full rather than summarized.

    #ifndef	GAMETYPES_H
    #define	GAMETYPES_H
    /////////////////////////////////////////////////////////////////////////////
    //
    // ImageStringPane
    //
    /////////////////////////////////////////////////////////////////////////////
    class GameType : public IObject
    {
    public:
        virtual ZString GetName() = 0;
        virtual bool IsGameType(const MissionParams& misparams) = 0;
        virtual void Apply(MissionParams& misparams) = 0;
        static TRef<GameType> FindType(const MissionParams& misparams);
        static TList<TRef<GameType> >& GetGameTypes();
    };
    #endif GAMETYPES_H

## `src/WinTrek/hangarscreen.cpp`

**PURPOSE:** The file `hangarscreen.cpp` defines the implementation for the hangar screen in the Allegiance game. This screen includes buttons for launching ships, commanding stations, managing loadouts, investing in development, accessing help, teleporting, viewing team information, and exiting missions.

**KEY PARTS:**
- **Class:** HangarScreen - Manages the hangar screen interface.
  - Handles button clicks and mouse hover events.
  - Updates the current hover state.
  - Interacts with game logic for ship operations and mission management.
- **Functions:** 
  - OnButtonLaunch, OnButtonCommand, etc. - Handle specific button actions.
  - OnHoverLaunch, OnHoverCommand, etc. - Update hover state when buttons are hovered over or left.
- **Data:**
  - m_pnumberCurrentHover - Tracks the current hover state of buttons.
  - g_pnumberInTurretNumber - Tracks whether a player is in a turret.

**CONNECTS TO:** 
- trekClient - Manages game logic and interactions with the server.
- TrekWindow - Handles window management and view modes.
- Modeler - Manages game data and resources.

**NOTES:**
- None

## `src/WinTrek/hudimage.h`

PURPOSE: Defines a base class for HUD images in the Allegiance game.

KEY PARTS:
- HUDImage: Base class for HUD images, inherits from WrapImage. Manages camera and view rectangle.
- Create: Static factory method to create an instance of HUDImage.
- Update: Pure virtual function to update the HUD image.
- SetClusterName: Pure virtual function to set the cluster name.
- SetWindowSize: Pure virtual function to set the window size.

CONNECTS TO: WrapImage, Camera, RectValue

NOTES: None.

## `src/WinTrek/indicator.cpp`

**PURPOSE:** The `indicator.cpp` file manages the rendering of various indicators and training overlays on the HUD in Allegiance. It handles different types of indicators such as direction, joystick, lead, turn, and training overlays.

**KEY PARTS:**
- **Class: IndicatorImage**: Manages rendering indicators and training overlays.
- **Function: RenderDirectionIndicator**: Renders a directional indicator based on ship velocity.
- **Function: RenderJoystickIndicator**: Renders an indicator for joystick controls.
- **Function: RenderLeadIndicator**: Renders a lead indicator showing target future position.
- **Function: RenderTurnIndicator**: Renders an indicator showing the direction of another model relative to the current ship.
- **Function: RenderTrainingOverlay**: Renders training overlays with text and graphical elements.
- **Data: g_pvertex, g_pindex**: Define the geometry for indicator pointers.
- **Enum: TrainingOverlay**: Enumerates different types of training overlays.

**CONNECTS TO:** Depends on `Image`, `Context`, `Modeler`, and `Viewport` classes.

**NOTES:** Contains commented-out code suggesting previous plans for colored vertices and triangle-based rendering. Repetitive logic in the `RenderTrainingOverlay` function can be refactored into helper functions, and polymorphism could improve handling of different overlay types.

## `src/WinTrek/indicator.h`

PURPOSE: This file defines functions and enums related to indicator images and training overlays in the game Allegiance.

KEY PARTS:
- CreateIndicatorImage: Creates an indicator image using a modeler, viewport, and time.
- TrainingOverlay: Enumerates different types of training overlays available in the game.
- SetTrainingOverlay: Sets the current training overlay.
- ClearTrainingOverlay: Clears the current training overlay by setting it to NoTrainingOverlay.

CONNECTS TO: src/WinTrek/modeler.h, src/WinTrek/viewport.h, src/WinTrek/number.h

NOTES: None.

## `src/WinTrek/introscreen.cpp`

PURPOSE: Handles the introduction screen and login/logout processes for Allegiance.

KEY PARTS:
- ErrorState: Manages different error states.
- LoggedInState: Represents a logged-in state with user information.
- LoginHelper: Manages the login process, handling steps and events.
- Logout: Handles the logout process by disconnecting from the lobby.
- FindServerPopup: Manages finding and joining LAN servers.
- OnButtonJoin: Handles the join button click event.
- OnLogon: Initiates logon process by saving character name and showing a connecting dialog.
- OpenWikiSink: Redirects to training screen on OK click from a message box.

CONNECTS TO: Allegiance game engine, Steam API for user status checks, network drivers for server discovery.

NOTES: None.

PARTS (in file order):

- lines 1-370
  - ErrorState: Base class for error states.
  - NoErrorState: Represents a state with no errors.
  - YesErrorState: Represents a state with an error message.
  - LoggedOutState: Represents a logged-out state with optional error and login sink.
  - LoggingInState: Represents the logging-in process with step number and message.
  - LoggedInState: Represents a logged-in state with various user information and actions.
  - LoginHelper: Manages the login process, handling different steps and events.
  - GetLoginSink: Returns an event sink for initiating login.
- lines 371-744
  - Logout: Handles the logout process by disconnecting from the lobby and updating the state.
  - Login: Manages the login process, including checking Steam user status and downloading configuration files.
  - StartConfigRead: Asynchronously reads the configuration file for game settings.
  - OnLogonLobby: Called when the player logs into the lobby successfully.
  - OnLogonLobbyFailed: Handles failed attempts to log into the lobby.
  - OnServersList: Processes the list of servers and updates the UI accordingly.
  - GetShowNewGamePopupSink: Provides a callback for showing the new game popup.
  - IntroScreen: Main class representing the introduction screen, handling user interactions and state management.
- lines 745-1114
  - FindServerPopup: Manages the popup for finding and joining LAN servers.
  - GameNameCompare: Compares two server names for sorting.
  - Paint: Draws a single server item on a surface.
  - OnButtonJoin: Handles the join button click event.
  - OnButtonCancel: Handles the cancel button click event.
  - FindGames: Searches for LAN servers and updates the server list.
  - PollForServers: Periodically polls for new servers in the background.
  - CheckNetworkDrivers: Checks if network drivers are up to date.
- lines 1115-1460
  - IntroScreen::OnButtonTraining: Handles the training button click.
  - IntroScreen::OnButtonDiscord: Handles the Discord button click.
  - IntroScreen::OnButtonSteamLeaderboard: Handles the Steam leaderboard button click.
  - IntroScreen::OnButtonExit: Handles the exit button click.
  - IntroScreen::OnQuickstart: Determines which quickstart option to handle based on configuration.
  - LoginHelper: Manages login state.
  - NumberExposer and StringExposer: Expose numeric and string values for UI interaction.
  - UiStateModifiableValue: Represents a modifiable UI state value.
- lines 1461-1807
  - OnPickServer(GUID guidSession): Handles picking a server by setting the session GUID and opening a logon popup.
  - OnButtonDiscord(): Opens Discord in the default web browser.
  - OnButtonSteamLeaderboard(): Opens Steam leaderboard page for Allegiance.
  - OpenWikiSink: A class implementing IIntegerEventSink to handle events from a message box, redirecting to training screen on OK click.
  - OnButtonTraining(): Handles training button press, either opening training or prompting download if not installed.
  - OnLogon(const ZString& strName, const ZString& strPassword, BOOL fRememberPW): Initiates logon process by saving character name and showing a connecting dialog.
  - OnTryLogon(): Attempts to connect to the server with saved credentials after a short delay.
  - OnAddPlayer(MissionInfo pMissionInfo, SideID sideID, PlayerInfo pPlayerInfo): Handles adding player to mission, redirecting to team screen if self.
- lines 1808-1926
  - GetWindow(): Retrieves the window associated with the screen.
  - OpenPopup(m_pcreditsPopup, false): Opens a popup for credits without animation.
  - ScreenIDSplashScreen: Identifier for the splash screen.
  - m_pnumberCurrentHover: Pointer to the current hovered item.
  - hoverPlayLan, hoverPlayInt, etc.: Values representing different hover states.
  - USEAZ: Macro indicating whether AZ features are enabled.
  - OnHoverZoneClub(): Handles hovering over the Zone Club option.
  - GetPane(): Returns the pane associated with the screen.

## `src/WinTrek/inventoryimage.cpp`

PURPOSE: The file `inventoryimage.cpp` contains the implementation of the `InventoryImage` class, which manages and displays inventory items in a space game interface.

KEY PARTS:
- `InventoryImage`: Manages the creation and updating of inventory item images.
- `InventorySlotDesc`: Describes an inventory slot with details like equipment type, mount point, and key.
- `CreatePartPane`: Creates a pane for displaying a specific part or cargo slot.
- `ProcessPart`, `ProcessObservers`, `ProcessCargo`, `ProcessTurret`: Functions to process and display different types of inventory items.
- `Rebuild`: Recreates the entire inventory display based on current ship state.

CONNECTS TO: The game's modeler, client events, and ship data systems.

NOTES: None.

## `src/WinTrek/leaderboardscreen.cpp`

**PURPOSE:** The `leaderboardscreen.cpp` file manages the leaderboard screen in Allegiance, handling user interactions and updating the display based on server-provided data.

**KEY PARTS:**
- **Class: LeaderBoardScreen**: Manages the leaderboard screen interface, including event handling and data updates.
- **Class: PlayerPainter**: Responsible for painting individual player entries in the leaderboard list.
- **Class: TopPlayerPainter**: Specializes in painting top players differently.
- **Function: LeaderBoardScreen::LeaderBoardScreen(Modeler* pmodeler, const ZString& strCharacter)**: Initializes the leaderboard screen with a modeler and character name.
- **Function: LeaderBoardScreen::ForwardLeaderBoardMessage(FEDMESSAGE * pLeaderBoardMessage)**: Forwards leaderboard messages to the active leaderboard screen instance.
- **Function: LeaderBoardScreen::UpdateData()**: Updates the leaderboard data based on current sort criteria and sends queries to the server.
- **Function: HandleLeaderBoardMessage(FEDMESSAGE *pLeaderBoardMessage)**: Processes leaderboard messages from the server, handling both failure and success.
- **Variable: s_pLeaderBoard**: A static pointer to the singleton instance of `LeaderBoardScreen`.

**CONNECTS TO:** Other screens and systems within Allegiance for user interaction and data management.

**NOTES:** The code includes TODO comments indicating areas needing further development. It uses custom error handling with `ZError` and macros like `BEGIN_PFM_CREATE` and `END_PFM_CREATE` for performance monitoring or logging.

## `src/WinTrek/load.cpp`

PURPOSE: The `load.cpp` file is responsible for managing ship loadouts, including buying and equipping parts, updating the UI, and handling user interactions.

KEY PARTS:
- LoadoutImpl: Main class for managing loadouts.
- Slot: Represents an equipment slot within a loadout.
- GetBuyableName: Function to get the name of a buyable item with its cost.
- InitializeMDL(): Initializes the model data for the loadout.
- InitializePanes(): Initializes UI panes and buttons.
- UpdateWeaponPartInfo: Updates weapon-related information.
- BuyHull: Handles buying a new hull type for the ship.
- OnButtonLaunch: Processes the launch button click event.
- HullSwitched: Updates the ship's hull and slots when changed.

CONNECTS TO:
- trekClient.GetShip()->GetSourceShip(): Retrieves the source ship for the current loadout.
- Modeler pmodeler: Pointer to a modeler object used for managing resources and data.
- Number ptime: Pointer to a number object representing time-related data.

NOTES: None.

PARTS (in file order):

- lines 1-359
  - LoadoutImpl: Main class for managing loadouts.
  - Slot: Represents an equipment slot within a loadout.
  - GetBuyableName: Function to get the name of a buyable item with its cost.
  - SlotValidForHull: Static function to check if a slot is valid for a given hull type.
  - SetPart: Sets the part in a slot and updates the image accordingly.
  - UpdateLayout: Updates the layout of a slot.
  - Paint: Draws the contents of a slot on a surface.
  - MouseEnter, MouseLeave, Button: Handle mouse events for slots.
- lines 360-714
  - LoadoutImpl: Class representing the implementation of a loadout.
  - Modeler pmodeler: Pointer to a modeler object used for managing resources and data.
  - Number ptime: Pointer to a number object representing time-related data.
  - m_pship: Pointer to a ship object being managed or modified.
  - trekClient.GetShip()->GetSourceShip(): Retrieves the source ship for the current loadout.
  - moneyLots: A constant representing a large amount of money.
  - InitializeMDL(): Initializes the model data for the loadout.
  - pnsLoadoutData: Name space containing various members to store and manage loadout data.
- lines 716-1084
  - TRef<INameSpace>: A reference to a namespace interface.
  - m_pmodeler: An instance of a modeler class responsible for managing namespaces and loading data.
  - InitializePanes(): Initializes UI panes and buttons.
  - GetPane(): Returns the main pane of the screen.
  - OnFrame(): Updates the frame, specifically updating buttons.
  - UpdateCivBitmap(): Updates the civilization bitmap based on the current side.
  - CreateGeoPane(): Creates a geographical pane with a camera and viewport.
  - SetGeo(Geo pgeo): Sets the geometry for the geo pane.
- lines 1085-1463
  - UpdateWeaponPartInfo: Updates weapon-related information.
  - UpdateMagazinePartInfo: Updates magazine-related information.
  - UpdateDispenserPartInfo: Updates dispenser-related information.
  - UpdateChaffLauncherPartInfo: Updates chaff launcher-related information.
  - UpdateShieldPartInfo: Updates shield-related information.
  - UpdateCloakPartInfo: Updates cloak-related information.
  - UpdateAfterburnerPartInfo: Updates afterburner-related information.
  - UpdatePartInfo: Main function to update part information based on the type of part.
- lines 1464-1818
  - BuyHull: Handles buying a new hull type for the ship.
  - OnButtonLaunch: Processes the launch button click event.
  - OnButtonBuy: Processes the buy button click event.
  - OnButtonBack: Handles the back button click event to return to the hangar view.
  - OnButtonHullList: Opens the hull list menu.
  - GetNextHullType: Determines the next available hull type based on current selection and station capabilities.
  - NextHull: Advances or retreats to the next/previous hull type.
  - CanMount: Checks if a part can be mounted in a specific slot.
- lines 1819-2267
  - LoadoutImpl: Main class for handling ship loadouts.
  - HullSwitched: Updates the ship's hull and slots when changed.
  - PartsSwitched: Updates the parts in the slots to match the current ship state.
  - UpdateHullType: Handles updating the hull type, including buttons and performance info.
  - OnMenuCommand: Handles menu commands for buying/selling parts/hulls.
  - OnKey: Handles keyboard input for custom loadouts.
  - CreateLoadout: Factory function to create a new LoadoutImpl instance.

## `src/WinTrek/load.h`

PURPOSE: tiny file (19 lines), shown in full rather than summarized.

    #ifndef _loadout_h_
    #define _loadout_h_
    //////////////////////////////////////////////////////////////////////////////
    //
    // Loadout
    //
    //////////////////////////////////////////////////////////////////////////////
    class Loadout : public Screen {
    public:
        virtual void   UpdateHullType(void)      = 0;
        virtual Pane*  GetPane()             = 0;
    };
    //////////////////////////////////////////////////////////////////////////////
    //
    // Constructor
    //
    //////////////////////////////////////////////////////////////////////////////
    TRef<Loadout> CreateLoadout(Modeler* pmodeler, Number* ptime);
    #endif

## `src/WinTrek/loadout.cpp`

PURPOSE: The file `loadout.cpp` manages the loadout and purchase interface for ships and parts in Allegiance, including handling item selection, investment logic, and updating the UI based on player actions and game state.

KEY PARTS:
- PartListItem: Represents an item in a loadout list, providing methods to get associated buyable objects, construct items, build prerequisite lists, and manage station dependencies.
- PurchasesPaneImpl: Manages the purchases pane interface, including handling button clicks for investment, updating the list based on player actions, and managing hover events.
- PurchasesPane: Manages the purchase interface for ships and parts, updating the tech tree, enabling or disabling spend buttons, and handling money changes.
- PartInfoPane: Manages a pane for displaying information about parts or items, including drawing names, descriptions, and applying creation using a factory method.

CONNECTS TO:
- Other UI components and game systems to update the loadout and purchase interface based on player actions and game state.

NOTES:
- None.

PARTS (in file order):

- lines 1-348
  - PartListItem: Represents an item in a loadout list.
  - GetBuyable(): Returns the buyable object associated with this part list item.
  - Construct(): Initializes the part list item by loading images and building prerequisite lists.
  - BuildPrereqList(): Builds a list of prerequisites for purchasing the item, considering tech requirements and station dependencies.
  - CountDronesOfType(): Counts the number of drones of a specific type owned by the player.
  - CountStationsOfType(): Counts the number of stations of a specific type owned by the player.
  - BuildSpotFoundForStationType(): Checks if there is an asteroid suitable for building a station of a given type.
  - FindBuilderForStationType(): Finds a ship that can build a station of a given type.
- lines 349-692
  - PurchasesPaneImpl: Manages the purchases pane interface.
  - m_pListPane: List pane for displaying items.
  - m_nTabSel: Selected tab index.
  - m_pPartInfoPane: Part information pane.
  - m_pStation: Current station reference.
  - m_pButtonInvest: Invest button.
  - m_pButtonTab0 to m_pButtonTab5: Tab buttons for different categories.
  - m_pButtonClose: Close button.
- lines 693-1044
  - PurchasesPaneImpl: Manages the purchase pane interface.
  - OnInvest(): Handles investment logic for selected items.
  - OnShowAll(): Updates the list to show all items.
  - OnHideCompleted(): Alias for OnShowAll(); hides completed items if unchecked.
  - OnPartial(): Handles partial investment logic.
  - OnButtonBack(): Closes the overlay associated with the pane.
  - OnBucketChange(): Updates the list when a bucket changes state.
  - OnModelTerminated(), OnStationCaptured(), OnDiscoveredStation(), OnDiscoveredAsteroid(): Update the list on model or station events.
- lines 1045-1392
  - PurchasesPane: Manages the purchase interface for ships and parts.
  - OnTechTreeChanged: Updates the tech tree when changed.
  - CanInvest: Determines if a player can invest in a bucket (ship or part).
  - UpdateSpendButtons: Enables or disables the spend button based on investment eligibility.
  - OnMoneyChange: Updates the spend buttons when money changes.
  - OnListHover: Handles hover events over the list items, displaying information about the selected item.
  - OnMouseEnterInvestButton and OnMouseLeaveInvestButton: Handle mouse enter and leave events for the invest button.
  - OnListSelect: Updates the spend buttons when a list item is selected.
- lines 1394-1514
  - PartInfoPane: Manages a pane for displaying information about parts or items.
  - DisplayNothing(): Clears and resets the display of the part info pane.
  - UpdateLayout(): Updates the layout of the part info pane based on its background image size.
  - DrawName(Surface, IbuyableIGC) const: Draws the name of a buyable item on the surface.
  - DrawDescription(Surface, IbuyableIGC) const: Draws the description and details (cost, time to build) of a buyable item on the surface.
  - Paint(Surface): Handles the painting of the part info pane by drawing its background and any associated text.
  - Create(ConnectorPane, ConnectorPane): Factory method to create an instance of PartInfoPaneImpl.
  - Apply(ObjectStack&): Applies the creation of a PartInfoPane using the factory method.

## `src/WinTrek/loadout.h`

PURPOSE: This file contains declarations for various UI components related to managing and displaying loadout information in the game Allegiance.

KEY PARTS:
- ConnectorPane: A base class for a pane that can be connected to other panes for purchases and inventory.
- PartInfoPane: Displays information about parts, including team purchases and inventory status.
- PurchasesPane: Manages the display of purchase options based on available parts.

CONNECTS TO: trekmdl.h

NOTES: None.

## `src/WinTrek/logon.cpp`

PURPOSE: The file `logon.cpp` contains the implementation for a logon popup window in Allegiance, which is used for user authentication and login.

KEY PARTS:
- LogonPopup: A class that implements an IPopup interface to create and manage the logon popup window.
- OnButtonLogon: Handles the logon button click event, validates input, and notifies the site of the logon attempt.
- OnButtonAbort: Handles the abort button click event, notifying the site of the abort action.
- OnButtonSignUp: Opens a web page for user registration.
- OnKey: Handles keyboard input events, such as Enter and Escape keys.
- CreateLogonPopup: A factory function to create an instance of LogonPopup.

CONNECTS TO:
- Other UI components like buttons and edit fields.
- The `Modeler` class for namespace management.
- The `LogonSite` interface for communication with the main application logic.
- Keyboard input handling through the `IInputProvider` interface.

NOTES: None.

## `src/WinTrek/logon.h`

PURPOSE: This file defines the interface and functionality for logon operations in Allegiance, including creating a logon popup and handling different types of logon scenarios.

KEY PARTS:
- LogonSite: An interface class that defines methods for handling logon events (OnLogon and OnAbort).
- LogonType: An enumeration defining different types of logon scenarios (AllegianceZone, FreeZone, LAN).

CONNECTS TO: This file is used by other parts of the game to create and manage logon popups.

NOTES: None.

## `src/WinTrek/mappreview.cpp`

**PURPOSE:**  
The file `mappreview.cpp` is part of the Allegiance game and contains the implementation for a map preview feature. It handles rendering and interaction with a minimap that displays mission parameters and sector details.

**KEY PARTS:**
- **MapPreviewImageImpl:** A class implementing the `MapPreviewImage` interface, responsible for managing the state and rendering of the map preview.
- **CreateMapPreviewImage:** A function to create an instance of `MapPreviewImageImpl`.
- **CalculateScreenMinAndMax:** Calculates the screen coordinates for sectors based on mission parameters.
- **RealRender:** Renders the map preview by drawing sectors, highlights, and connecting lines between warps.
- **MouseMove:** Handles mouse movement events for dragging the map view.
- **Button:** Handles mouse button press and release events to start and stop dragging.

**CONNECTS TO:**
- `Modeler` and `Image`: For loading images used in rendering.
- `TrekClient`: For accessing mission parameters and player side information.
- `ImapPVMaker`: For creating mission preview data.
- `MapPreviewPane`: For handling mouse events related to the map preview pane.

**NOTES:**
- Contains debug code that is commented out, such as printing cluster details and warp information.
- Uses custom cursor settings for dragging interactions.

## `src/WinTrek/mappreview.h`

PURPOSE:
This file defines interfaces and implementations for rendering game map previews in Allegiance. It includes classes for creating and managing map preview images and panes.

KEY PARTS:
- MapPreviewImage: Interface for creating and displaying a game map preview.
  - CalcBounds(): Calculates the bounds of the preview image.
  - ComputePreview(const MissionParams& p): Updates the preview based on game parameters.
  - Render(Context* pcontext): Renders the preview image.
  - ShowDetails(bool bDetails): Shows or hides detailed information in the preview.
  - ShowSide(bool bSide): Shows or hides side views in the preview.

- MapPreviewPane: Pane that renders a map preview and handles user interactions.
  - Constructor initializes with modeler, image, detail visibility, and side view visibility.
  - MouseEnter(IInputProvider* pprovider, const Point& point): Handles mouse enter event.
  - MouseLeave(IInputProvider* pprovider): Handles mouse leave event.
  - ComputePreview(const MissionParams& p): Delegates to the MapPreviewImage to update the preview.

CONNECTS TO:
- controls.h: For input handling and pane rendering.
- Modeler: Likely used for creating or managing game models.
- Image: Base class for image handling, including bounds calculation and rendering.

NOTES:
- None.

## `src/WinTrek/mappvmaker.cpp`

PURPOSE: Handles the creation and generation of mission data for various map types in Allegiance.

KEY PARTS:
- CwarpPV: Represents a warp point within a mission.
- ImissionPV: Manages mission data including clusters and warps.
- CMapPVCluster: Represents a cluster of sectors in the map.
- ImapPVMaker: Base class for creating different types of maps.
- IsValid: Validates mission parameters based on map type.
- Create: Generates a mission based on mission parameters.
- pvFindPath: Recursive function to find a path between clusters using warps.
- GenerateMission: Main function to generate a mission based on parameters.
- GenerateLayout: Sets up team and neutral cluster counts based on map size.
- LinkClusters: Links clusters together in a ring pattern.
- CrossLinkClusters: Creates cross-links between teams' sectors.
- GenerateRingClusters: Generates clusters in a ring layout.
- CmapPVMakerDoubleRing: Specific implementation for double ring layout.
- CmapPVMakerHiLo: Handles high/low map generation.
- CmapPVMakerStar: Handles star-shaped map generation.
- CmapPVMakerSnowFlake: Generates a snowflake-like map layout.
- CmapPVMakerEastWest: Generates layout for East West map type.
- CmapPVMakerSplitBase: Generates layout for Split Base map type.
- CmapPVMakerFromIGCFile: Generates mission from IGC file.

CONNECTS TO: None

NOTES:
- "KGJV-: changed, use now MapSize parameter" - Indicates a change to use MapSize instead of MaxPlayersOnTeam.
- Missing break statements in "case 1:" and "case 2:" in GenerateNeutralClusterScreenPosition for CmapPVMakerHiHigher.

PARTS (in file order):

- lines 1-341
  - CwarpPV: Represents a warp point within a mission.
  - ImissionPV: Manages mission data including clusters and warps.
  - CMapPVCluster: Represents a cluster of sectors in the map.
  - ImapPVMaker: Base class for creating different types of maps.
  - IsValid: Validates mission parameters based on map type.
  - Create: Generates a mission based on mission parameters.
  - pvFindPath: Recursive function to find a path between clusters using warps.
- lines 342-710
  - CMapPVMaker: Manages the creation and generation of mission data.
  - GenerateMission: Main function to generate a mission based on parameters.
  - GenerateLayout: Generates the sector layout for the mission.
  - GenerateTeamClusterScreenPosition: Calculates screen position for team clusters.
  - GenerateTeamCluster: Creates a team cluster with specified properties.
  - GenerateNeutralClusterScreenPosition: Calculates screen position for neutral clusters.
  - GenerateNeutralCluster: Creates a neutral cluster with specified properties.
  - GenerateRingClusters: Generates clusters in a ring pattern.
- lines 712-1066
  - CmapPVMaker: Main class for creating map layouts.
  - GenerateLayout: Sets up team and neutral cluster counts based on map size.
  - LinkClusters: Links clusters together in a ring pattern.
  - CrossLinkClusters: Creates cross-links between teams' sectors.
  - GenerateRingClusters: Generates clusters in a ring layout.
  - CmapPVMakerDoubleRing: Specific implementation for double ring layout.
  - GenerateTeamClusterScreenPosition: Positions team clusters on the screen.
  - GenerateNeutralClusterScreenPosition: Positions neutral clusters on the screen.
- lines 1068-1426
  - CmapPVMakerHiLo: Handles high/low map generation.
  - GenerateLayout: Sets up cluster parameters and generates clusters.
  - GenerateTeamClusterScreenPosition: Calculates screen positions for team clusters.
  - GenerateNeutralClusterScreenPosition: Calculates screen positions for neutral clusters.
  - LinkClusters: Links clusters together in a specific pattern.
  - CmapPVMakerHiHigher: Handles high/higher map generation, extending from CmapPVMakerHiLo.
  - CmapPVMakerStar: Handles star-shaped map generation.
  - TODOs: "//KGJV-: changed, use now MapSize parameter" - Indicates a change to use MapSize instead of MaxPlayersOnTeam. "case 1:" and "case 2:" in GenerateNeutralClusterScreenPosition for CmapPVMakerHiHigher - Missing break statements.
- lines 1427-1790
  - CmapPVMakerStar: Manages star-shaped map layouts.
  - LinkClusters: Links clusters together based on team and neutral sectors.
  - IsValid: Validates mission parameters for specific map types.
  - GenerateLayout: Generates layout for different map types like Brawl, PinWheel, DiamondRing, etc.
  - GenerateNeutralClusterScreenPosition: Positions neutral clusters on the screen.
  - CmapPVMakerBrawl: Manages brawl map layouts.
  - CmapPVMakerPinWheel: Manages pinwheel map layouts.
  - CmapPVMakerDiamondRing: Manages diamond ring map layouts.
- lines 1791-2147
  - CmapPVMakerSnowFlake: Generates a snowflake-like map layout.
  - GenerateTeamClusterScreenPosition: Calculates screen positions for team clusters.
  - GenerateNeutralClusterScreenPosition: Calculates screen positions for neutral clusters.
  - LinkClusters: Links clusters together based on their type and position.
  - IsValid: Validates mission parameters for specific map types.
  - CmapPVMakerLargeSplit, CmapPVMakerInsideOut, CmapPVMakerGrid, CmapPVMakerEastWest: Different map layout classes.
  - NA: Likely a macro or constant used to represent an invalid index or value.
  - TODOs: "KGJV-: changed, use now MapSize parameter" (mentioned in comments) Validation checks for mission parameters could be more detailed.
- lines 2148-2428
  - CmapPVMakerEastWest: Generates layout for East West map type.
  - CmapPVMakerSplitBase: Generates layout for Split Base map type.
  - CmapPVMakerFromIGCFile: Generates mission from IGC file.
  - GenerateMission: Main function to generate mission from IGC file.
  - IsValid: Checks if the mission parameters are valid for a specific map type.
  - SetName: Sets the name of the map type.
  - GenerateLayout: Generates the layout of clusters for a map.
  - GenerateTeamClusterScreenPosition: Determines screen position for team clusters.

## `src/WinTrek/mappvmaker.h`

**PURPOSE:** This file contains the header definitions for various components involved in creating and managing map previews for missions in Allegiance. It includes classes for representing warps, clusters, mission data, and different map maker strategies.

**KEY PARTS:**
- **CwarpPV:** Represents a warp point between sectors.
- **CMapPVCluster:** Represents a cluster of sectors with associated warps.
- **ImissionPV:** Interface for managing mission data including clusters and warps.
- **CMapPVData:** Holds all the information needed to build a map preview.
- **ImapPVMaker:** Interface for creating different types of map layouts.
- **CmapPVMaker:** Base class for implementing specific map layout strategies.

**CONNECTS TO:** 
- `mapmakerigc.h` (for data structures like DataClusterIGC and DataWarpIGC)
- Other classes and functions related to mission management and game logic

**NOTES:** None.

## `src/WinTrek/netinfo.cpp`

PURPOSE: The file `netinfo.cpp` contains functions to retrieve and validate network device information from the Windows registry.

KEY PARTS:
- `_GetDeviceValue`: Reads a value from the hardware or software key of a PnP device.
- `_FindDevice`: Enumerates started PnP devices looking for a device of a particular class.
- `GetRegistryString`: Retrieves a string value from a registry key.
- `CheckDevice`: Checks if a network device is valid by comparing its driver version and type.
- `CheckNetworkDevices`: Validates all network devices.

CONNECTS TO: Windows Registry, OS Version Information

NOTES: None

## `src/WinTrek/newgamescreen.cpp`

PURPOSE: Manages the interface and logic for the new game screen in Allegiance, handling user interactions and mission parameter settings.

KEY PARTS:
- NewGameScreen: Manages the new game screen interface and handles user interactions.
- GetResourceLevel: Calculates the resource level based on asteroid counts.
- RefreshMissionParams: Updates the UI elements with the current mission parameters.
- ApplyMissionParams: Applies mission parameters to the UI elements.
- OpenMDLFile: Initializes the modeler namespace for the new game screen data.
- FillCombo: Populates a combo box with items from a specified content name in the modeler namespace.
- FindMapType: Finds the index of a map by its file name.
- CustomMapIDToItemID: Converts a custom map ID to an item ID.
- ItemIDToCustomMapID: Converts an item ID back to a custom map ID.
- OnRefreshMapPreview: Handles refreshing the map preview based on selected settings.
- OnButtonMapNext: Handles moving to the next map type in the dropdown.
- OnButtonMapPrev: Handles moving to the previous map type in the dropdown.
- ReadControls: Reads control values and populates a mission parameters structure.

CONNECTS TO: modeler namespace for game data, team screen, mission parameters.

NOTES: None.

PARTS (in file order):

- lines 1-351
  - NewGameScreen: Manages the new game screen interface and handles user interactions.
  - GetResourceLevel: Calculates the resource level based on asteroid counts.
  - GetBaseMissionParams: Retrieves the base mission parameters.
  - RefreshMissionParams: Updates the UI elements with the current mission parameters.
  - ApplyMissionParams: Applies mission parameters to the UI elements.
  - YesNo: Converts a boolean value to "yes" or "no".
  - GetNthString: Retrieves a string from a list based on an index.
  - GetLives: Returns the lives setting as a string.
- lines 352-727
  - NewGameScreen: Manages the new game screen interface and logic.
  - OpenMDLFile: Initializes the modeler namespace for the new game screen data.
  - FillCombo: Populates a combo box with items from a specified content name in the modeler namespace.
  - FillIntComboWithLimit: Populates an integer combo box with values up to a specified limit.
  - FindValue: Retrieves a value from a table based on its index.
  - FindClosestValue: Finds the closest value to a given number in a table.
  - AddCustomMapTypes: Adds custom map types to the game screen.
- lines 728-1069
  - newgamescreen.cpp: Contains functions and logic for managing the new game screen in Allegiance.
  - FindMapType(const char szMapFile): Finds the index of a map by its file name.
  - CustomMapIDToItemID(int mapID): Converts a custom map ID to an item ID.
  - ItemIDToCustomMapID(int itemID): Converts an item ID back to a custom map ID.
  - OnRefreshMapPreview(int max): Handles refreshing the map preview based on selected settings.
  - OnButtonMapNext(): Handles moving to the next map type in the dropdown.
  - OnButtonMapPrev(): Handles moving to the previous map type in the dropdown.
  - ReadControls(MissionParams& misparams): Reads control values and populates a mission parameters structure.
- lines 1071-1370
  - NewGameScreen: Manages the new game screen interface and handles user interactions.
  - OnAddPlayer: Updates button states when a player joins or leaves the mission.
  - OnPlayerStatusChange: Updates button states when a player's status changes.
  - OnFrame: Handles frame updates, specifically updating countdown text.
  - UpdateCountdownText: Displays the remaining time until the game starts.
  - OnMissionStarted: Switches back to the team screen when the mission starts.
  - OnAddMission: Refreshes mission parameters when a new mission is added.
  - CreateNewGameScreen: Creates an instance of NewGameScreen.

## `src/WinTrek/passworddialog.cpp`

PURPOSE: This file contains the implementation of a password dialog used in Allegiance for entering passwords, particularly for joining missions.

KEY PARTS:
- PasswordDialogImpl: Manages the logic and UI elements of the password dialog.
  - Handles button clicks (OK and Cancel).
  - Processes keyboard input to handle Enter key as OK.
  - Updates mission information and notifies the parent sink upon closing.
- CreatePasswordPopup: Factory function to create an instance of PasswordDialogImpl.

CONNECTS TO:
- Other UI components like Pane, ButtonPane, EditPane, IKeyboardInput, IPopupContainer, and TrekClientEventSink.
- MissionInfo for mission-related operations.
- PasswordDialogSink for communication with the parent dialog handler.

NOTES: None.

## `src/WinTrek/passworddialog.h`

PURPOSE: tiny file (15 lines), shown in full rather than summarized.

    #ifndef _passworddialog_h_
    #define _passworddialog_h_
    //////////////////////////////////////////////////////////////////////////////
    //
    // Password Dialog
    //
    //////////////////////////////////////////////////////////////////////////////
    class PasswordDialogSink
    {
    public:
        virtual void OnCancelPassword() = 0;
        virtual void OnPassword(ZString strPassword) = 0;
    };
    TRef<IPopup> CreatePasswordPopup(PasswordDialogSink* psink, const ZString& strPassword, MissionInfo* pmission = NULL);
    #endif _passworddialog_h_

## `src/WinTrek/pch.cpp`

PURPOSE: tiny file (10 lines), shown in full rather than summarized.

    #include "pch.h"
    #ifdef _ATL_STATIC_REGISTRY
    #include <statreg.h>
    //#include <statreg.cpp> Imago removed (again)
    #endif
    #ifdef  TREK_OCX
    //#include <atlimpl.cpp>
    #include <atlctl.cpp>
    //#include <atlwin.cpp>
    #endif

## `src/WinTrek/pch.h`

PURPOSE: This file is a precompiled header for the Allegiance game's WinTrek module. It includes standard system headers, Windows-specific headers, STL containers and algorithms, DirectX headers, and project-specific headers that are frequently used across the module.

KEY PARTS:
- `__MODULE__`: Defines the module name as "WinTrek".
- C Headers: Standard C libraries like `<stdlib.h>`, `<math.h>`, etc.
- Windows Headers: Includes various Windows API headers for GUI, multimedia, and common dialogs.
- STL Headers: Commonly used containers and algorithms from the Standard Template Library.
- DX Headers: DirectX headers including input handling and graphics device interfaces.
- WinTrek Headers: Project-specific headers for game engine, effects, utilities, UI components, and more.

CONNECTS TO: Other modules within Allegiance, particularly those related to game engine functionality, user interface, and DirectX rendering.

NOTES: None.

## `src/WinTrek/radarimage.cpp`

PURPOSE: Implements radar image functionality for a space game, including drawing blips and expanded blips, setting and getting radar level of detail, and rendering various types of objects on the radar.

KEY PARTS:
- RadarImageImpl: Manages the overall radar image rendering.
- DrawBlip: Draws individual blips based on object type and parameters.
- DrawExpandedBlip: Renders an expanded view around a specific object.
- SetRadarLOD: Adjusts the level of detail for the radar display.
- GetRadarLOD: Retrieves the current level of detail setting.
- UsefulTreasure: Determines if a treasure item is useful based on game state and resources.
- RenderBlips: Renders blips for all objects in the cluster on the radar.
- Context, Rect, Color: Used for rendering images and text on the radar.

CONNECTS TO:
- trekClient: Provides access to game data and functionality.

NOTES:
- Contains multiple implementations of `DrawExpandedBlip` function.
- Uses various interfaces like IclusterIGC, IshipIGC, ItreasureIGC, ImodelIGC for different object types.

PARTS (in file order):

- lines 1-356
  - RadarImageImpl: Implements the radar image functionality.
  - TextData: Represents data for text labels on the radar.
  - Evaluate: Recalculates the radar circle vertices and indices when the view size changes.
  - DrawBlip: Draws a blip on the radar based on various parameters.
  - DrawExpandedBlip: Draws an expanded blip around an object.
  - SetRadarLOD: Sets the current radar level of detail.
  - GetRadarLOD: Retrieves the current radar level of detail.
  - c_szRadarLODs: Array of strings representing different radar modes.
- lines 357-712
  - DrawExpandedBlip: Draws an expanded blip on the radar.
  - UsefulTreasure: Determines if a treasure item is useful based on the ship's current state and resources.
  - RenderBlips: Renders blips for each model in the cluster on the radar.
  - trekClient: Global client instance providing access to game data and functionality.
  - IclusterIGC, IshipIGC, ItreasureIGC, ImodelIGC: Interfaces representing different types of game objects.
  - Context: Class for rendering context, used to draw images and text.
  - Rect: Represents a rectangle area on the screen.
  - Color: Represents a color in RGBA format.
- lines 714-1033
  - UsefulTreasure: Determines if a treasure is useful.
  - c_iUsefulTech: Indicates a treasure is useful for technology.
  - m_psurfaceTechIcon: Surface icon for useful technology treasures.
  - bLabel: Boolean indicating whether to label the radar entry.
  - OT_treasure: Type representing a treasure object.
  - OT_ship: Type representing a ship object.
  - OT_station: Type representing a station object.
  - OT_missile: Type representing a missile object.
- lines 1034-1482
  - RadarImageImpl::RenderBlips: Draws blips on the radar image.
  - RadarImageImpl::RenderRoundBorder: Renders a round border around the radar.
  - RadarImageImpl::Render: Main rendering function for the radar image.
  - RadarImageImpl::s_colorNeutral: Neutral color used in the radar display.
  - RenderBlips: Function to draw blips, likely part of RadarImageImpl.
  - RenderRoundBorder: Function to render a round border around the radar, also part of RadarImageImpl.
  - Render: Main rendering function for the radar image, part of RadarImageImpl.
  - s_colorNeutral: Neutral color used in various parts of the radar display.

## `src/WinTrek/radarimage.h`

PURPOSE: This header file defines the interface and structure for a radar image component in the Allegiance game. It specifies methods and properties related to rendering and managing different levels of detail (LOD) on the radar.

KEY PARTS:
- RadarImage: Base class for radar image components, inheriting from Image.
- SetViewport: Sets the viewport for the radar image.
- GetViewport: Retrieves the current viewport.
- GetCamera: Returns the camera associated with the radar's viewport.
- GetViewRect: Gets the view rectangle of the radar's viewport.
- Evaluate: Pure virtual function to evaluate and update the radar display.
- Create: Static method to create an instance of RadarImage.
- SetRadarLOD: Sets the level of detail for the radar display.
- GetRadarLOD: Retrieves the current level of detail setting.
- Render: Pure virtual function to render the radar image.

CONNECTS TO: Image, Viewport, Camera, Context

NOTES: None

## `src/WinTrek/screen.h`

PURPOSE: This file defines the base class `Screen` and provides factory functions to create various types of screens used in the Allegiance game.

KEY PARTS:
- Screen: Base class for all screens, providing virtual methods for getting image, pane, size, and handling frame updates.
- CreateTeamScreen, CreateGameScreen, etc.: Factory functions to create specific screen instances like team, game, intro, new game, game over, leader board, zone events, hangar, training, etc.

CONNECTS TO: Modeler, UiEngine, IPopup

NOTES:
- Contains commented-out code for video screens.
- External variables control quickstart, MOTD skipping, and other game settings.

## `src/WinTrek/sectormap.cpp`

PURPOSE: Manages and displays information about sectors in the game, including rendering the sector map and handling user interactions.

KEY PARTS:
- SectorInfoPane: Manages and displays information about a sector.
- GetClusterOwners: Determines the primary and secondary owners of a cluster based on station count.
- Render: Draws the sector map and related information on the screen.
- SectorMapPane: Manages the display and interaction with the sector map.
- SelectCluster: Updates the selected cluster and notifies SectorInfoPane.
- SetVisible: Sets the visibility of the sector map pane.

CONNECTS TO: Allegiance game engine, other UI components, game logic for asset states and mission parameters.

NOTES: None.

PARTS (in file order):

- lines 1-360
  - SectorInfoPane: Manages and displays information about a sector.
  - GetClusterOwners: Determines the primary and secondary owners of a cluster based on station count.
  - GetClusterPopulation: Counts the number of active ships in a cluster for a given side.
  - AddIcon: Adds an icon to the sector info pane, handling duplicates and sorting.
  - CalcBounds: Calculates the bounds of the sector info pane image.
  - ClearCluster: Clears the selected and mouse-over clusters.
  - SelectCluster: Sets the currently selected cluster.
  - SelectMouseOverCluster: Sets the cluster under the mouse cursor.
- lines 361-707
  - SectorInfoPaneFactory::Apply: Creates a new instance of SectorInfoPane.
  - HeliumAsteroidVisibileInCluster: Checks if any He3 asteroids are visible in a cluster.
  - Render: Draws the sector map and related information on the screen.
  - SectorMapPane: Manages the display and interaction with the sector map.
  - SelectCluster: Updates the selected cluster and notifies SectorInfoPane.
  - SetVisible: Sets the visibility of the sector map pane.
- lines 708-1052
  - m_bVisible: A boolean indicating whether the sector map is visible.
  - GetWindow(): Retrieves the window associated with the sector map.
  - GetTimer(): Retrieves the timer associated with the window.
  - AddSink(): Adds an event sink to the timer.
  - RemoveSink(): Removes an event sink from the timer.
  - SelectCluster(): Selects a cluster in the sector map.
  - m_pSectorInfoPane: A pointer to the sector information pane.
  - SelectMouseOverCluster(): Selects the mouse-over cluster in the sector info pane.
- lines 1054-1393
  - AssetMask: Represents a bitmask of asset states.
  - ClusterWarning: Enumerates different warning levels for clusters.
  - GetClusterWarning: Determines the warning level based on asset mask and mission parameters.
  - DrawImage3D: Draws an image in 3D space with specified color and position.
  - WinPoint: Represents a point in 2D space.
  - IclusterIGC: Interface for cluster game objects.
  - GetClusterPoint: Converts cluster screen coordinates to map coordinates.
  - GetClusterAtPoint: Finds the cluster at a given map point.
- lines 1394-1637
  - SectorMapPane: Manages the sector map interface.
  - GetClusterCursor: Determines the cursor to display based on current game state.
  - MouseMove: Handles mouse movement events, including dragging and selecting clusters.
  - Button: Processes mouse button events, handling clicks and drags.
  - OnClusterChanged: Updates the selected cluster when it changes externally.
  - SectorMapPaneFactory::Apply: Factory method to create instances of SectorMapPane.

## `src/WinTrek/sectorvw.h`

PURPOSE: tiny file (18 lines), shown in full rather than summarized.

    #ifndef	SECTORVW_H
    #define	SECTORVW_H
    //////////////////////////////////////////////////////////////////////////////
    //
    // SectorViewImage
    //
    //////////////////////////////////////////////////////////////////////////////
    class SectorViewImage : public WrapImage {
    protected:
        SectorViewImage() :
            WrapImage(Image::GetEmpty())
        {
        }
    public:
        static TRef<SectorViewImage> Create(Engine* pengine);
        virtual void SetWindowSize(const Point& point) = 0;
    };
    #endif

## `src/WinTrek/selpane.cpp`

PURPOSE: This file contains implementations for two UI panes in a game interface, `SelectModelPane` and `SelectionPane`, which are used for selecting models and managing selections respectively.

KEY PARTS:
- SelectModelPane: A pane that allows the selection of a model. It updates its layout based on child panes.
- SelectionPane: A pane that manages a row of selectable items and notifies when a selection changes.

CONNECTS TO: `trekmdl.h` for model data, `consoledata.h` for console-related data, and other UI components like `RowPane`.

NOTES: None.

## `src/WinTrek/soundinit.cpp`

PURPOSE: This file initializes sound templates and manages namespaces for sound definitions in the Allegiance game.

KEY PARTS:
- InitializeSoundTemplates: Loads training sound definitions and general-purpose sound templates.
- AddMembers: Adds sound IDs to a namespace.
- pmodeler: Modeler object used for loading files and managing namespaces.
- vSoundMap: Vector of sound template references.
- TRef, INameSpace, IObjectList, ISoundTemplate: Allegiance framework classes.

CONNECTS TO: src/WinTrek/modeler.h (Modeler class), src/WinTrek/sounds.h (sound definitions).

NOTES: None.

## `src/WinTrek/splashscreen.cpp`

PURPOSE: The file `splashscreen.cpp` contains the implementation of a splash screen for the Allegiance game. It handles the display and behavior of the splash screen, including timing out and transitioning to the intro screen.

KEY PARTS:
- SplashScreen: A class representing the splash screen, inheriting from Screen, EventTargetContainer, and IKeyboardInput.
  - Handles events like timeouts, button clicks, and keyboard inputs.
  - Manages the display pane and transition to the intro screen.
- CreateSplashScreen: A function that creates an instance of the SplashScreen.

CONNECTS TO:
- Modeler: Used to load and unload the splash screen namespace.
- Pane: Represents the graphical elements of the splash screen.
- ButtonPane: Specifically for button elements in the splash screen.
- IKeyboardInput: For handling keyboard inputs during the splash screen.

NOTES:
- The timeout duration is conditionally set based on `g_bQuickstart`.
- There's a TODO comment suggesting a class name and pointer.

## `src/WinTrek/teampane.cpp`

PURPOSE: Manages the team pane interface and handles various events related to player selection, team management, and resource donations.

KEY PARTS:
- TeamPane: Manages the team pane UI.
- PlayerPainter: Custom painter for rendering players in the list.
- TeamPainter: Custom painter for rendering teams in the list.
- OnEvent: Handles events from the player list and team combo.
- OnSelectPlayer: Updates the UI when a player is selected.
- CanAcceptJoiners: Checks if more players can join the current team.
- OnButtonBoot: Handles the booting of a player.
- OnRightButtonDonate: Handles large donations with a right-click.
- ExpandedTeamPane: Extended version of TeamPane with additional functionality for displaying player details.

CONNECTS TO: Allegiance game engine, PlayerList, TeamCombo

NOTES:
- Contains TODOs indicating code was removed or commented out.
- Includes custom painters and event handlers specific to the team pane interface.

PARTS (in file order):

- lines 1-361
  - TeamPane: Manages the team pane interface.
  - DrawAutoDonateIcon: Draws an icon indicating whether a player can donate resources to another ship.
  - PlayerPainter: Custom painter for rendering players in the list.
  - TeamPainter: Custom painter for rendering teams in the list.
  - sortLeader, sortStatus, etc.: Enumerations defining columns for sorting players and teams.
  - TODOs: "// draw the ship" (commented out) "//psurface->DrawString(SectorName(pplayer->LastSeenSector()));" (commented out) "//TRef<Image> pimageShipIcon;" (commented out) "//if (IsAllyOrHumanOrSpotted(pplayer)){...}" (commented out) "// draw the sector" (commented out)
  - Unusual code: Conditional compilation directives like //#include "imagetransform.h" with comments indicating they are commented out. Use of ZString for string manipulation, which is a custom string class in Allegiance.
- lines 362-725
  - TeamPane: Manages the team pane UI.
  - TeamPanelPainter: Paints individual team panels.
  - IsAllyOrHumanOrSpotted: Filters items based on visibility criteria.
  - GetSideUIColor: Returns a color for a given side ID.
  - OnEvent: Handles events from the player list and team combo.
  - OnSelectPlayer: Updates the UI when a player is selected.
  - OnSelTeam: Handles selection of a team in the team combo.
- lines 728-1096
  - NumPlayersOnSide: Counts human players on a given side.
  - MaxTeamSize: Determines the maximum team size based on mission parameters and current player imbalances.
  - CanAcceptJoiners: Checks if more players can join the current team.
  - OnSelectPlayer: Handles selection of a player in the team pane, enabling/disabling buttons based on player status and team leader privileges.
  - OnPlayerRightClicked: Adds a right-click menu for selected players.
  - OnSelTeam: Updates the team selection when a new team is chosen.
  - OnAddPlayer: Refreshes the player list when a new player joins.
  - OnDelPlayer: Refreshes the player list when a player leaves.
- lines 1097-1455
  - OnButtonBoot: Handles the booting of a player.
  - OnPlayerStatusChange: Updates the team pane when a player's status changes.
  - OnButtonDonate: Handles donating money to another player.
  - OnRightButtonDonate: Handles large donations with a right-click.
  - OnButtonStopDonate: Toggles stopping a player from donating.
  - OnButtonAutoDonate: Initiates automatic donations to a selected player.
  - OnButtonTakeMeTo: Moves the view to a selected player's ship.
  - OnButtonAccept: Accepts a position request for a player in the team lobby.
- lines 1456-1844
  - TeamPane: Manages the team pane interface.
  - SideCompare, LeaderCompare, etc.: Static functions for comparing player or side information.
  - PlayerInfo, IshipIGC, IclusterIGC, IhullTypeIGC: Classes representing game entities and their properties.
  - CreateTeamPane: Factory function to create a new instance of TeamPane.
  - ExpandedTeamPane: Extended version of TeamPane with additional functionality.
  - ExpandedPlayerPainter: Custom painter for expanded team pane players.
  - TODOs: "cut by aem" indicates code was removed or commented out. Additional TODOs may be present in the omitted sections.
- lines 1845-2049
  - ExpandedTeamPane: A class that extends TeamPane and adds additional functionality for displaying player details.
  - OnButtonSize: A member function of ExpandedTeamPane that handles button size events.
  - CreateExpandedTeamPane: A function to create an instance of ExpandedTeamPane.
  - m_pbuttonCollapse: A member variable representing a collapse button in the team pane.
  - m_plistPanePlayers: A member variable representing a list pane for displaying player details.
  - m_peventTeams: A member variable representing the event source for team selection events.
  - m_psinkTeams: A member variable representing the event sink for handling team selection events.
  - TODO: "BT - 7/15 - When the team screen is shown, miners have -1 for the wingID, which causes alleg to crash."

## `src/WinTrek/teamscreen.cpp`

PURPOSE: Manages the team selection screen and handles interactions related to player teams, civilizations, and mission briefings.

KEY PARTS:
- TeamScreen: Main class for the team selection screen.
- TeamPainter: Paints individual teams on the screen.
- CivilizationArray: Stores civilization IDs.
- GetMissionMapType: Returns the type of map for the current mission.
- GetCivMissionBriefing: Constructs briefing text based on mission parameters and player information.
- OnButtonOk: Processes "OK" button click in team settings dialog, updating side's name and civilization.
- TeamScreen constructor: Initializes team screen components and settings.
- SendChat: Handles chat messages, routing them to appropriate recipients.
- UpdateTeamButtons: Updates the state of team buttons based on mission status and player roles.
- GetSideRankSum: Counts the sum of ranks for a specified side.
- OnTeamRightClicked: Creates an alliance context menu if conditions are met.
- OnButtonDiscord: Opens a Discord link based on the current side.
- OnPlayerStatusChange: Updates button states when a player's status changes.

CONNECTS TO: trekClient, PlayerPainter, TeamIconImage, MissionBriefPopup, TeamSettingsDialogPopup

NOTES: None

PARTS (in file order):

- lines 1-360
  - TeamScreen: Main class for the team selection screen.
  - TeamPainter: Class responsible for painting individual teams on the screen.
  - CivilizationArray: Array to store civilization IDs.
  - numCivilizations: Variable to keep track of the number of civilizations.
  - lastRandCivUpdate: Time when the random civilization update was last performed.
  - sortPlayers: Enum defining sorting options for players.
  - m_fColorIntensity and m_fColorBrightness: Variables controlling color intensity and brightness.
  - CivilizationArray: Array storing civilization IDs.
- lines 361-712
  - TrekResources::SmallFont(): Returns a small font resource.
  - Color::White(): Represents white color.
  - WinPoint(m_viColumns[0]+4, m_nHeight  1/2 -2): Specifies a point for drawing text.
  - trekClient.GetCore()->GetSide(pitem->GetSideID())->GetCivilization()->GetName(): Retrieves the name of the civilization associated with a side.
  - psurface->DrawString(...): Draws a string on a surface at specified coordinates and color.
  - PlayerPainter::Paint(...): Paints player information on a surface.
  - TeamIconImage::SetSide(SideID sideId): Sets the background image for a team icon based on the side ID.
  - MissionBriefPopup::GetString(const ZString& strName, TRef<INameSpace> pns): Retrieves a string value from a namespace.
- lines 713-1068
  - GetMissionMapType: Returns a string representing the type of map for the current mission.
  - GetCivMissionBriefing: Constructs a briefing text based on the mission parameters and player information.
  - GetMissionBriefingTitle: Returns the title for the mission briefing.
  - GetMissionBriefingString: Retrieves a specific string from the mission briefing data.
  - GetMissionTip: Generates a random tip to display at the end of the mission briefing.
  - MissionBriefPopup: Manages the popup window that displays the mission briefing and tips.
  - TeamSettingsDialogPopup: Handles the dialog for setting team names, civilizations, and squads during gameplay.
  - OnButtonOk: Processes the "OK" button click in the team settings dialog, updating the side's name and civilization.
- lines 1069-1426
  - TeamScreen::OnButtonCancel: Closes the popup and restores cursor.
  - IsHuman: Determines if a player is human based on mission data.
  - TeamScreen constructor: Initializes team screen components and settings.
  - AddEventTarget: Registers event handlers for various UI elements.
  - OnWingCombo: Handles wing selection changes.
  - OnPlayerClicked, OnPlayerRightClicked, OnPlayerDoubleRightClicked: Handle player interactions.
  - UpdateButtonStates: Updates button visibility based on game state.
  - m_pbuttonbarChat: Chat tab control.
- lines 1427-1771
  - TeamScreen: Manages the team selection screen interface.
  - GetCurrentSide(): Returns the current side ID.
  - SendChat(const ZString& strChat): Handles chat messages, routing them to appropriate recipients based on content and user input.
  - SetHiddenTeamButtons(bool bHidden): Hides or shows team buttons based on whether changes are allowed.
  - UpdateTeamButtons(): Updates the state of team buttons based on mission status and player roles.
  - UpdateButtonStates(): Adjusts the enabled and visible states of various UI elements based on game conditions and player permissions.
  - UpdateTitleText(): Sets the title text for the screen, displaying the mission name.
  - UpdatePromptText(): Updates the prompt text to indicate the current team status or join requests.
- lines 1773-2119
  - GetSideRankSum: Counts the sum of ranks for a specified side.
  - GetRankThreshold: Retrieves the threshold to be used for rank balancing.
  - GetCountdownTime: Calculates the remaining time until the mission starts or ends.
  - SetCountdownTimer: Formats and sets the countdown timer text.
  - UpdateCountdownText: Updates the countdown timer display based on the current game stage.
  - SideCompare, LeaderCompare, PlayerNameCompare, PlayerRankCompare: Comparison functions for sorting players and sides.
  - PlayerStatusCompare: Custom comparison class to sort players by their status (requesting/member, ready/unready).
  - PlayerWingCompare: Custom comparison class to sort players by their wing affiliation.
- lines 2121-2477
  - ItemIDCompareFunction: A function type used for comparing ItemIDs.
  - ReverseCompare: A class that reverses the comparison result of another function.
  - SortingList: A function to sort a list using a given comparison function and reverse flag.
  - GetPane: Returns the current pane associated with the team screen.
  - OnEvent: Handles UI events, including player and team selections.
  - OnDismissRandomizeMessageBox: Handles dismissal of a randomization warning message box.
  - OnTeamClicked and OnTeamDoubleClicked: Handle left and double-click events on teams.
  - OnTeamRightClicked: Handles right-click events on teams, creating an alliance context menu if conditions are met.
- lines 2479-2831
  - UpdateCivBitmap: Updates the civilization bitmap based on the current side.
  - OnTeamCivChange: Handles changes to the team's civilization.
  - OnSelTeam: Handles selection of a team in the list.
  - OnEvent(TEvent<ZString>::Source pevent, ZString str): Handles text events for chat messages.
  - OnTimer: Updates UI elements and handles randomization of civilizations.
  - OnCivChange(int index): Handles changes to civilization selection.
  - OnWingCombo(int index): Handles wing assignment changes.
  - OnButtonBack(): Handles the back button click.
- lines 2832-3188
  - OnCancelRequest: Handles canceling a request to join a team.
  - OnButtonDiscord: Opens a Discord link based on the current side.
  - OnButtonJoin: Attempts to join the current or another team.
  - OnButtonAccept: Accepts a player's request to join the team.
  - OnButtonReject: Rejects a player's request to join the team.
  - OnCivChosen: Changes the civilization for the team if the leader is present.
  - ChatTargetToButton: Maps chat targets to button indices.
  - OnButtonBarChat: Handles chat target selection via buttons.
- lines 3189-3534
  - TeamScreen::OnButtonAwayFromKeyboard: Handles the "Away From Keyboard" button click.
  - TeamScreen::OnButtonTeamSettings: Manages team settings and ensures game control isn't started during these settings.
  - TeamScreen::OnButtonMakeLeader: Sets another player as the team leader.
  - TeamScreen::OnButtonToggleTeam: Toggles the active state of a team.
  - TeamScreen::OnAddMission: Updates the mission details when a new mission is added.
  - TeamScreen::OnAddPlayer: Handles adding players to the mission, including updating chat targets and player statuses.
  - TeamScreen::OnDelPlayer: Manages player removals, displaying appropriate messages based on the reason for removal.
  - TeamScreen::OnPlayerStatusChange: Updates button states when a player's status changes.
- lines 3535-3705
  - TeamScreen::OnDelRequest: Handles deletion requests for players and displays appropriate messages.
  - TeamScreen::UpdateStatusText: Updates the status text on the team screen.
  - TeamScreen::CreateMessageBox: Creates a message box with the given text.
  - TeamScreen::s_nLastSelectedChatTab: Stores the last selected chat tab index.
  - TeamScreen::s_bFlash: Indicates whether the screen should be flashing.
  - CreateTeamScreen: Factory function to create an instance of TeamScreen.

## `src/WinTrek/teleportpane.cpp`

PURPOSE: The `teleportpane.cpp` file contains the implementation of the TeleportPane class, which is responsible for managing the teleportation interface in the Allegiance game. This includes displaying a list of available destinations and handling user interactions such as selecting a destination and initiating a teleport.

KEY PARTS:
- **TeleportPane**: Main class that manages the teleportation pane.
  - Contains methods to handle button clicks, selection events, and rebuild the destination list.
  - Uses `DestinationPainter` to customize how destinations are displayed in the list.
- **DestinationPainter**: Custom painter for rendering each item in the destination list.
  - Handles drawing the name, type, sector, and turret information of each destination.

CONNECTS TO:
- Other game systems such as mission management, player tracking, and ship handling.

NOTES:
- None.

## `src/WinTrek/trainingscreen.cpp`

PURPOSE: The file `trainingscreen.cpp` contains the implementation of the training screen in Allegiance, a Microsoft multiplayer space game. It handles user interactions for selecting and starting different training missions.

KEY PARTS:
- **Class: TrainingScreen** - Manages the training screen UI, including buttons for mission selection and handling events.
- **Function: OnButtonTrain** - Handles the logic for advancing to the next training mission or initiating a free-flight session.
- **Function: CheckButton** - Updates the state of mission buttons based on the current mission number.
- **Variable: m_iMissionNext** - Tracks the currently selected training mission.

CONNECTS TO:
- `Screen` and related classes for managing UI screens.
- `EventTargetContainer` and `TrekClientEventSink` for handling events.
- `ModifiableNumber` for storing and updating the current mission number.

NOTES: None.

## `src/WinTrek/trekapp.cpp`

PURPOSE: This file contains the implementation of the main application class for Allegiance, including initialization and termination logic.

KEY PARTS:
- `TrekAppImpl`: Main application class that inherits from `EffectApp`.
- `Initialize`: Initializes the application, checks system requirements, parses command line arguments, and sets up the game window.
- `Terminate`: Cleans up resources before exiting the application.
- `CheckDSoundVersion`: Verifies if DirectX 7 or higher is installed.
- `CheckFreeMemory`: Ensures there is enough free memory to run the game.
- `CleanUpSteamName`: Sanitizes Steam persona names for use as character names.

CONNECTS TO: 
- Other Allegiance modules and classes, such as `TrekWindow`, `EffectApp`, and `VideoSettingsDX9`.

NOTES:
- Contains conditional compilation for Steam integration.
- Includes logic to handle command line arguments for various game settings.
- Has commented-out code related to EULA handling and CD key management.

## `src/WinTrek/trekctrls.cpp`

PURPOSE: Manages various UI components and functionalities for a list pane with scroll functionality.

KEY PARTS:
- SubImagePaneImpl: Manages a pane that displays an image.
- TabPaneImpl: Implements a tabbed interface with selectable tabs and associated panes.
- ListPaneOldImpl: A class representing a list pane with scroll functionality.
- InsertItem: Inserts a list item at a specified index.
- AppendItem: Appends a list item to the end of the list and returns its index.
- RemoveItem: Removes a list item by pointer.
- UpdateItem: Updates a list item by pointer.

CONNECTS TO: trekctrls.h

NOTES:
- Uses ZAssert for debugging checks.

## `src/WinTrek/trekctrls.h`

PURPOSE: This file contains declarations for various UI components and event sinks used in the Allegiance game client.

KEY PARTS:
- SubImagePane: A pane that displays an image.
- TabPane: A pane that can contain multiple sub-tabs, each with a corresponding button.
- ListPaneOld: A list-like pane that can display items, supports sorting and filtering.
- AnimatedTranslateTransform2: A transform that animates the translation of a rectangle over time.
- TrekClientEventSink: An event sink for handling client events.

CONNECTS TO: trekClient (other game systems)

NOTES: None.

## `src/WinTrek/trekigc.cpp`

PURPOSE: Manages game elements and interactions within a cluster site, including sound effects, explosions, and model management.

KEY PARTS:
- ClusterSiteImpl: Manages game elements and interactions within a cluster site.
- AddExplosion: Adds an explosion effect at a specified position with various parameters.
- AddThingSite: Adds a ThingSite to the scene.
- DeleteThingSite: Removes a ThingSite from the scene.
- ModelPointSoundSource: Represents a sound source attached to a model.
- SoundSite: Interface for models which emit sounds.
- ShipSoundSite: Manages sound effects for a ship.
- UpdateSounds: Updates and plays sounds based on the ship's state.
- LoadEffect: Loads a build effect with the specified color.
- LoadDecal: Loads a decal with the given texture name and width.
- WinTrekClient::WinTrekClient(): Constructor for WinTrekClient class.
- Initialize: Initializes client state and resources.
- ConnectToServer: Connects to the server and handles connection errors.

CONNECTS TO:
- BaseClient
- ISoundPositionSource

NOTES:
- Contains TODO comments for future development, such as handling turret orientation in CreateSoundSources.

PARTS (in file order):

- lines 1-349
  - ClusterSiteImpl: Manages game elements and interactions within a cluster site.
  - AddExplosion: Adds an explosion effect at a specified position with various parameters.
  - AddThingSite: Adds a ThingSite to the scene.
  - DeleteThingSite: Removes a ThingSite from the scene.
  - AddPoster: Adds a poster image to the scene.
  - SetEnvironmentGeo: Sets the environment geometry for the cluster site.
  - GetEnvironmentGeo: Retrieves the current environment geometry.
  - AddScanner: Adds a scanner to the appropriate list based on side affiliation.
- lines 350-713
  - ModelPointSoundSource: Represents a sound source attached to a model.
  - SoundSite: Interface for models which emit sounds.
  - RegisteredModelPointSoundSource: A registered version of ModelPointSoundSource.
  - AmbientSoundSite: A sound site for a model with one continuous ambient sound.
  - UpdateSounds: Updates the current sounds for this object according to its state.
  - AddHitSound: Adds a hit sound (only keeps the most recent few sounds).
  - StopSounds: Stops all sounds on this object - usually used when changing clusters.
  - GetSoundSource: Gets a sounds source that can be used for this object.
- lines 714-1064
  - ShipSoundSite: Manages sound effects for a ship.
  - ForwardThrustFraction: Calculates the fraction of forward thrust based on controls.
  - SidewaysThrustFraction: Calculates the fraction of sideways thrust based on controls.
  - TurnRate: Determines the current turn rate of the ship.
  - AfterburnerPower: Retrieves the power level of the afterburner.
  - TryingToFireWeaponWithoutAmmo: Checks if the player is trying to fire a weapon without enough ammo.
  - TryingToFireAfterburnersWithoutFuel: Checks if the player is trying to use afterburners without fuel.
  - HasMissileLock: Determines if the ship has a missile lock.
- lines 1066-1414
  - ShipSoundSite: Manages sound effects for a ship.
  - UpdateSounds: Updates and plays sounds based on the ship's state.
  - CreateSoundSources: Initializes sound sources for various parts of the ship.
  - GetGunnerInteriorSoundID: Determines the interior gunner sound ID.
  - GetAfterburnerSoundID: Retrieves the afterburner sound ID.
  - UpdateEngineSoundLevels: Adjusts engine sound levels and pitches.
  - StopSounds: Stops all sounds when changing clusters.
  - TODO: Does not handle turret orientation in CreateSoundSources.
- lines 1416-1760
  - ThingSiteImpl: Manages a site for a thing in the game.
  - Initializes the ThingSiteImpl with a model and sets up sound sites based on the object type.
  - Cleans up by deleting the sound site if it exists.
  - Purge: Clears references to things, decals, and build effects.
  - Terminate: Similar to Purge but handles additional cleanup for different cases.
  - GetChildModelOffset: Retrieves the offset of a child model frame.
  - GetChildOffset: Retrieves the offset of a child frame.
  - SetAfterburnerSmokeSize, SetAfterburnerFireDuration, SetAfterburnerSmokeDuration: Sets properties related to afterburner effects.
- lines 1762-2111
  - LoadEffect: Loads a build effect with the specified color.
  - LoadDecal: Loads a decal with the given texture name and width.
  - LoadModel: Loads a 3D model with optional texture.
  - SetTrailColor: Sets the trail color for the model or decal.
  - LoadAleph: Loads an Aleph entity with the specified texture.
  - LoadMine: Loads a mine field with the given texture, strength, and radius.
  - SetMineStrength: Updates the strength of the mine field.
  - UpdateScreenPosition: Updates the screen position and related properties.
- lines 2112-2449
  - WinTrekClient::WinTrekClient(): Constructor for WinTrekClient class.
  - SideVisibility: Class that manages visibility of sides in the game.
  - UpdateSounds(DWORD dwElapsedTime): Updates sounds based on elapsed time.
  - StopSounds(): Stops all sounds associated with the object.
  - RegisterHit(float fAmount, const Vector& vectOffset, bool bAbsorbedByShield = false): Registers a hit and plays appropriate sound.
  - GetClusterSite(): Returns the cluster site of the model.
  - TRef<ISoundPositionSource> GetSoundSource(const Vector& vectOffset): Gets the sound source with an offset.
  - TRef<ISoundPositionSource> GetSoundSource(): Gets the default sound source.
- lines 2450-2808
  - WinTrekClient: Main class for handling client-specific logic.
  - Initialize: Initializes client state and resources.
  - LoadExplosionImage: Loads explosion images into memory.
  - Terminate: Cleans up client resources.
  - ResetStaticData: Resets mission data.
  - GetCargoPart: Retrieves the current cargo part.
  - NextCargoPart: Advances to the next cargo part.
  - OnReload: Handles reload events for cargo parts.
- lines 2809-3152
  - WinTrekClient::OverrideCamera: Overrides the camera view for a given model.
  - WinTrekClient::TargetKilled: Handles actions when a target is killed.
  - WinTrekClient::ShipWarped: Manages actions when a ship warps to a new sector.
  - WinTrekClient::PostNotificationText: Posts notification text with optional critical status.
  - WinTrekClient::ActivateTeleportProbe: Activates a teleport probe and posts related messages.
  - WinTrekClient::DestroyTeleportProbe: Destroys a teleport probe and updates cluster assets.
  - WinTrekClient::PostPlainText: Posts plain text with optional critical status.
  - WinTrekClient::PostText: Similar to PostPlainText but uses variable arguments.
- lines 3154-3531
  - AutoDownloadProgressDialogPopup: Manages the progress dialog for auto-download operations.
  - OnBeginAutoUpdate: Initializes and displays the auto-download progress dialog.
  - ShouldFilterFile: Determines if a file should be filtered out during download based on its name.
  - OnProgress: Updates the progress of the current file being downloaded.
  - OnError: Handles errors that occur during the auto-update process.
  - OnEvent: Closes the message box when the user acknowledges an error or abort.
  - WinTrekClient::OnLogonAck: Handles acknowledgment of logon to a game server.
  - WinTrekClient::OnLogonLobbyAck: Handles acknowledgment of logon to a lobby.
- lines 3532-3892
  - WinTrekClient::OnLogonClubAck: Handles successful or failed login to Zone Club server.
  - WinTrekClient::Disconnect: Disconnects from the game server and sets m_bDisconnected to true.
  - WinTrekClient::OnSessionLost: Handles session loss, showing appropriate messages and disconnecting if necessary.
  - WinTrekClient::FlushSessionLostMessage: Flushes any stored session lost message to a popup.
  - WinTrekClient::OnEvent: Exits the application when an event is received.
  - WinTrekClient::OverrideThrottle: Overrides the throttle setting for the game.
  - DevelopmentCompleted: Applies development technologies and attributes to a side if not connected to the server.
  - StationTypeCompleted: Handles the completion of station type creation, including creating drones.
- lines 3894-4249
  - DataProbeIGC: A structure used for creating a data probe object.
  - WinTrekClient::DroneTypeCompleted: Handles the completion of a drone type creation process.
  - WinTrekClient::ContinueRipcord: Determines if the ripcord action can continue.
  - WinTrekClient::UseRipcord: Executes the ripcord action, moving a ship to a new location.
  - WinTrekClient::SetViewCluster: Updates the view cluster for the player.
  - WinTrekClient::RequestViewCluster: Sends a request to change the view cluster to the server.
  - WinTrekClient::TerminateModelEvent: Handles events when a model is terminated.
  - WinTrekClient::HandlePickDefaultOrder: Handles picking default orders for ships.
- lines 4250-4595
  - WinTrekClient: Main client class handling various game events.
  - DamageShipEvent: Handles damage to ships, including adjusting groove levels and playing effects.
  - KillStationEvent: Manages station destruction, including explosions and sound effects.
  - HitWarpEvent: Processes warp events for ships, updating orientation and velocity.
  - HitTreasureEvent: Handles treasure interactions for ships.
  - PaydayEvent: Placeholder function for payday events (not implemented).
  - GetMoneyRequest: Queues money requests from ships.
  - ReceiveChat: Manages chat messages, filtering and playing sounds based on conditions.
- lines 4596-4937
  - WinTrekClient::ReceiveChat: Handles chat messages and processes commands.
  - ChatLink: Represents a single chat message.
  - Preload: Loads models and textures for rendering.
  - CreateThingSite: Creates a site for a model.
  - CreateClusterSite: Creates a site for a cluster.
  - TODOs: NYI need to distinguish between all and friendly in CHAT_ALL_SECTOR NYI hack to piggify a pilot Xynth #14 7/2010 change color if from comm
  - Unusual code: Cheat board handling commented out Conditional compilation for DX9 build
- lines 4938-5293
  - ClusterSiteImpl: Manages cluster sites and their associated sounds.
  - WinTrekClient::PlaySoundEffect(SoundID soundID, ImodelIGC model): Plays a sound effect for a given model.
  - WinTrekClient::PlayNotificationSound(SoundID soundID, ImodelIGC model): Plays a notification sound if the model is related to the player's ship.
  - WinTrekClient::PlayFFEffect(ForceFeedbackEffectID effectID, ImodelIGC model, LONG lDirection): Plays force feedback effects for the given model and direction.
  - WinTrekClient::PlayVisualEffect(VisualEffectID effectID, ImodelIGC model, float fIntensity): Applies visual effects to a model based on the effect ID and intensity.
  - WinTrekClient::UpdateAmbientSounds(DWORD dwElapsedTime): Updates ambient sounds in the current cluster or station.
  - WinTrekClient::ResetSound(): Resets all sound effects.
  - WinTrekClient::ConnectToServer(BaseClient::ConnectInfo & ci, DWORD dwCookie, Time now, const char szPassword, bool bStandalonePrivate): Connects to the server and handles connection errors.
- lines 5294-5589
  - WinTrekClient: Main client class handling game logic and interactions.
  - OnQuitMission: Handles different types of mission quits and displays appropriate messages.
  - SetGameoverInfo: Updates gameover information with details about the endgame.
  - AddGameoverPlayers: Adds player statistics to the endgame data.
  - GetGameoverMessage: Returns the message describing the gameover reason.
  - GetEndgamePlayerInfo: Retrieves specific player's endgame information.
  - GetNumEndgamePlayers: Returns the number of players involved in the endgame.
  - GetSideEndgameInfo: Provides details about a specific side during the endgame.

## `src/WinTrek/trekinput.cpp`

PURPOSE: Handles input processing and configuration for Allegiance's game interface, including keyboard and joystick mappings.

KEY PARTS:
- TrekInputImpl: Main class implementing input handling.
- InitializeCommandsMDL(): Initializes command mappings using a modeler namespace.
- Flush(): Resets keyboard state by calling GetKeyState() on all keys.
- normalize(int val, int minval, int maxval): Normalizes an integer value to a specified range.
- CreateInputMapPopup(...): Function to create a popup for input mapping configuration.
- pnsCommands: Adds command targets and modifiers to a namespace.
- GetKeyDown(int vk): Checks if a key is currently down.
- LoadMap(const ZString& str): Loads an input map from a modeler namespace, initializing key mappings and joystick settings.
- TrekInput: Main class handling input processing.
- IsTrekKeyDown: Checks if a TrekKey is currently down.
- SetMouseTrekKey: Sets the state of a mouse-based TrekKey.
- TranslateKeyMessage: Translates Windows key messages to TrekKeys.
- HandleKeyMessage: Handles incoming key messages and updates internal states.
- PlayFFEffect: Plays force feedback effects on the joystick.
- GetAxis: Retrieves the value of a joystick axis.
- IsAxisValid: Checks if a joystick axis is valid.

CONNECTS TO: Allegiance's game engine, input handling system, and modeler namespace for command mappings.

NOTES:
- Use of GetKeyState() in Flush() method for keyboard state reset.
- Commented-out lines related to specific views or targets.

PARTS (in file order):

- lines 1-325
  - TrekInputImpl: Main class implementing the input handling for the game.
  - InitializeCommandsMDL(): Initializes command mappings using a modeler namespace.
  - Flush(): Resets keyboard state by calling GetKeyState() on all keys.
  - normalize(int val, int minval, int maxval): Normalizes an integer value to a specified range.
  - CreateInputMapPopup(...): Function to create a popup for input mapping configuration.
  - countAxis, keyMax, ModifierNone, etc.: Constants defining various input-related parameters and states.
  - TODOs: "Do things this way because SetKeyboardState() doesn't appear to work on Win'95..." "// Turkey added 8/10 #56"
  - Unusual code: The use of GetKeyState() in the Flush() method, which is not typically used for resetting keyboard state. The commented-out line for "CommandViewTurret" and similar commands related to specific views or targets.
- lines 326-679
  - pnsCommands: Adds command targets and modifiers to a namespace.
  - GetKeyDown(int vk): Checks if a key is currently down.
  - GetFPSString(): Generates a string representing current modifier states.
  - LoadMap(const ZString& str): Loads an input map from a modeler namespace, initializing key mappings and joystick settings.
  - m_pboolNone to m_pboolJustControlShiftAlt: Boolean variables representing different combinations of key modifiers.
  - keyMax, TK_Max: Constants defining the maximum number of keys and commands.
  - m_KeyMap to m_ControlShiftAltKeyMap: Arrays mapping virtual key codes to command targets based on modifier states.
  - m_wheelupTK to m_xbutton5TK: Variables for special mouse buttons mapped to command targets.
- lines 681-1047
  - TrekInput: Main class handling input processing.
  - IsTrekKeyDown: Checks if a TrekKey is currently down.
  - SetMouseTrekKey: Sets the state of a mouse-based TrekKey.
  - TranslateKeyMessage: Translates Windows key messages to TrekKeys.
  - HandleKeyMessage: Handles incoming key messages and updates internal states.
  - PlayFFEffect: Plays force feedback effects on the joystick.
  - GetAxis: Retrieves the value of a joystick axis.
  - IsAxisValid: Checks if a joystick axis is valid.
- lines 1049-1373
  - TrekKey OnXButton2(): Returns a key mapping for the second Xbox button.
  - TrekKey OnXButton3(): Returns a key mapping for the third Xbox button.
  - TrekKey OnXButton4(): Returns a key mapping for the fourth Xbox button.
  - TrekKey OnXButton5(): Returns a key mapping for the fifth Xbox button.
  - CreateInputMapPopup(Modeler pmodeler, IEngineFont pfont, Number ptime): Creates and returns an input map popup using provided parameters.
  - CreateTrekInput(HINSTANCE hInst, HWND hwnd, InputEngine pinputEngine, JoystickImage pjoystickImage): Factory function to create a new instance of TrekInputImpl.
  - g_ppccAxisDescription[]: Array containing descriptions for different axes (Yaw, Pitch, Roll, Throttle, Hat, Hat as Buttons).
  - KeyInfo g_pkeyInfo[]: Array mapping key codes to their corresponding names.
- lines 1374-1714
  - InputMapPopup: Manages a popup for input mapping.
  - KeyMapping: Represents a keyboard key and its modifiers.
  - ButtonMapping: Represents a joystick button index.
  - CommandMap: Maps commands to either keyboard keys or joystick buttons.
  - AddKeyMapping: Adds a keyboard key mapping to the command map.
  - AddButtonMapping: Adds a joystick button mapping to the command map.
  - HasKeyMapping: Checks if a specific keyboard key mapping exists in the command map.
  - RemoveKeyMapping: Removes a specific keyboard key mapping from the command map.
- lines 1715-2086
  - InputMapPopup: Manages input mapping popup functionality.
  - KeyMapping: Represents a keyboard key mapping with virtual key code and modifiers.
  - ButtonMapping: Represents a button mapping with joystick index and button index.
  - EvaluateImage: Custom image class that intercepts mouse input during editing.
  - HasButtonMapping: Checks if a button mapping exists for given joystick and button indices.
  - RemoveButtonMapping: Removes a button mapping based on joystick and button indices.
  - GetString: Generates a string representation of all key and button mappings.
  - FillModifierMap: Fills an array indicating which keys have modifiers associated with them.
- lines 2087-2480
  - InputMapPopup: Manages input mapping settings.
  - LoadMap: Loads an input map from a file.
  - SaveMap: Saves the current input map to a file.
  - OnButtonOK: Handles the OK button click event.
  - OnButtonCancel: Handles the Cancel button click event.
  - OnButtonRestore: Restores default input mappings.
  - OnDoubleClick: Handles double-click events on the list pane.
  - GetSelectionIndex: Retrieves the index of the currently selected item in the list pane.
- lines 2481-2847
  - GetCommandMapIndex: Returns the command map index based on the current selection.
  - GetAxisIndex: Calculates the axis index by subtracting a constant from the current selection index.
  - DeleteMapping: Deletes the mapping for either an axis or a command based on the current selection.
  - IsEditing: Checks if the current state is in editing mode.
  - EditSelection: Initiates editing mode, recording initial joystick values or setting up key/button input prompts.
  - StopEditing: Ends editing mode and resets related variables.
  - RemoveKeyMappings: Removes all key mappings for a given virtual key code and modifiers.
  - AddKeyMapping: Adds a key mapping if it's not already in use, prompting to overwrite if necessary.
- lines 2849-3055
  - ItemPainter: Manages painting of items on a surface.
  - GetXSize: Returns the X size of an item.
  - GetYSize: Returns the Y size of an item.
  - Paint: Draws an item on a surface with optional selection and focus indicators.
  - IPopup: Interface for popup components.
  - GetPane: Retrieves the pane associated with this popup.
  - GetImage: Generates an image representation of the popup.
  - IKeyboardInput: Handles keyboard input events.

## `src/WinTrek/trekinput.h`

PURPOSE: This file defines interfaces and functions related to input handling in the Allegiance game, specifically for managing keyboard and mouse inputs.

KEY PARTS:
- TrekInputSite: Interface for objects that need to receive trek key events.
- TrekInput: Main interface for input management, including methods for loading maps, setting focus, playing effects, getting axis values, translating key messages, creating popup menus, and handling specific mouse wheel and button clicks.
- CreateTrekInput: Function to create an instance of the TrekInput class.

CONNECTS TO: InputEngine, JoystickImage

NOTES: None.

## `src/WinTrek/trekmdl.cpp`

**PURPOSE:** The `trekmdl.cpp` file in Allegiance is responsible for managing and creating various UI panes that display different types of information and interact with the user through mouse events. It includes factories for creating these panes and handling sound effects associated with user interactions.

**KEY PARTS:**
- **SwitchValuePane:** Manages switching between different panes based on a numeric value.
- **ImageValuePane:** Displays an image and updates its size when the image changes.
- **StringValuePane:** Displays text with optional color and justification.
- **GaugeValuePane:** Displays a gauge that can flash to indicate changes.
- **SwitchPaneFactory, StringPaneFactory, GaugePaneFactory:** Factories for creating different types of panes based on input parameters.
- **ButtonPaneFactory:** Factory for creating button panes with optional sound effects and toggle functionality.
- **CreateTrekButton:** Helper function to create a button pane with specific behaviors, including mouseover and click sounds.
- **SoundIDEventSink:** Event sink that plays a sound when an event occurs.

**CONNECTS TO:**
- Other UI components and systems within the game's user interface framework.

**NOTES:**
- None.

## `src/WinTrek/trekmdl.h`

PURPOSE: This file contains declarations and definitions related to various UI components and factories for a space game interface.

KEY PARTS:
- ButtonPane: Represents a button with customizable sounds for hover and click events.
- ValuePane: Inherits from both Value and Pane, likely used for displaying values or interactive elements.
- SectorMapPaneFactory, SectorInfoPaneFactory, PurchasesPaneFactory, PartInfoPaneFactory, SelectionPaneFactory, SelectModelPaneFactory: Factories that create specific UI panes related to game sectors, purchases, parts, selections, models, and more.
- ChatListPane: A pane for displaying chat messages with methods to add/remove channels, reset channels, page up/down, set chat lines, and text size.
- ChatListPaneFactory: Factory for creating instances of ChatListPane.
- InventoryImageFactory: Factory for creating inventory image components.

CONNECTS TO: value.h (for Value class), other UI-related classes and factories in the game's interface system.

NOTES: None.

## `src/WinTrek/treksound.cpp`

PURPOSE: This file contains implementations of various factories for creating different types of sound templates in the Allegiance game engine.

KEY PARTS:
- WaveTemplateFactory: Creates a sound template from a WAV or OGG file.
- ThreeDSoundTemplateFactory: Creates a 3D sound template based on another sound template and minimum distance.
- ConeSoundTemplateFactory: Creates a cone-shaped sound template with adjustable inner and outer angles and gains.
- PitchSoundTemplateFactory: Adjusts the pitch of an existing sound template.
- SoundGainValueTemplate: A template that adjusts the gain of another sound template dynamically.
- GainSoundTemplateFactory: Creates a sound template with dynamic gain adjustment.
- PrioritySoundTemplateFactory: Sets priority for a sound template.
- LoopingSoundTemplateFactory: Creates a looping version of a sound template.
- ASRSoundTemplateFactory: Creates an audio sample repeat (ASR) sound template.
- PairedSoundTemplateFactory: Combines two sound templates into one.
- RepeatingFireSoundTemplateFactory: Creates a repeating fire sound template with adjustable rate.
- RandomSoundTemplateFactory: Chooses a random sound template from a list based on weights.
- IntermittentSoundTemplateFactory: Plays a sound template intermittently at specified intervals.
- SerializedSoundTemplateFactory: Ensures that only one instance of a sound template is played at a time.

CONNECTS TO: Modeler, ISoundTemplate, IFunction, ObjectStack

NOTES: None

## `src/WinTrek/treksound.h`

PURPOSE: tiny file (12 lines), shown in full rather than summarized.

    #ifndef _treksound_H_
    #define _treksound_H_
    //////////////////////////////////////////////////////////////////////////////
    //
    // effect sounds
    //
    //////////////////////////////////////////////////////////////////////////////
    TRef<INameSpace> CreateSoundNameSpace(
        Modeler*    pmodeler,
        INameSpace* pnsParent
    );
    #endif

## `src/WinTrek/videoscreen.cpp`

PURPOSE: The file `videoscreen.cpp` contains the implementation of a video screen class in Allegiance, which manages the display and playback of video content during the game's introduction sequence.

KEY PARTS:
- VideoScreen: Manages the video screen, including loading videos, handling user input, and transitioning between videos.
- PickImage: A custom image class that handles mouse events to trigger video transitions.
- NextVideo: Loads and displays the next video in the sequence.
- OnEvent: Handles events to trigger video transitions.
- Picked: Called when a pick event occurs, triggering the next video.
- OnKey: Handles keyboard input to advance videos.

CONNECTS TO:
- Screen.h: Inherits from Screen class for screen management.
- IEventSink.h and IKeyboardInput.h: Implements interfaces for handling events and keyboard input.
- Engine.h: Uses engine for video playback.
- WrapImage.h, GroupImage.h, Image.h: Manages image hierarchy and rendering.

NOTES:
- The code includes conditional compilation for DirectX 9 support (`BUILD_DX9`).
- There is commented-out code that was likely used for loading specific video files but has been disabled.

## `src/WinTrek/wintrekp.h`

PURPOSE: tiny file (19 lines), shown in full rather than summarized.

    #ifndef _WinTrekP_H__
    #define _WinTrekP_H__
    //////////////////////////////////////////////////////////////////////////////
    //
    // Wintrek Private Headers
    //
    //////////////////////////////////////////////////////////////////////////////
    class ThingSitePrivate : public ThingSite {
    public:
        virtual void UpdateScreenPosition(
            const Point& pointScreenPosition,
            float fScreenRadius,
            float distanceToEdge,
            unsigned char ucRadarState
        ) {
        }
        virtual TRef<ISoundPositionSource> GetSoundSource(void) = 0;
    };
    #endif

## `src/WinTrek/zoneeventsscreen.cpp`

PURPOSE:
The file `zoneeventsscreen.cpp` contains the implementation of the `ZoneEventsScreen` class, which manages a screen displaying events in a space game. It handles loading event data, updating the UI based on user interactions, and downloading additional files as needed.

KEY PARTS:
- ZoneEventsScreen: Manages the entire zone events screen, including loading events, handling button clicks, and updating the UI.
- EventListing: Represents an individual event with details like date, time, name, description, and sign-up URL.
- EventPainter: Paints each event listing on the screen.
- LoadEvents: Loads event data from a namespace.
- DownloadEventListing: Initiates downloading additional files for events.
- OnFileCompleted: Handles completion of file downloads.

CONNECTS TO:
Depends on `Screen`, `IItemEvent::Sink`, and other UI components like buttons and panes.

NOTES:
None.
