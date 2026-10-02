# src/clintlib

[Back to the index](../LLAMA_SUMMARY.md) - 9 files, 13,165 lines. Notes by a local model, to orient by: not verified facts.

The `src/clintlib` folder in Allegiance contains core client-side functionality essential for managing game operations and interactions. It includes modules for handling downloads, processing server messages, filtering bad words, and managing player and mission data.

- AutoDownload.cpp: Manages automatic file downloads, verifying local files, and handling download errors.
- AutoDownload.h: Defines interfaces and utility functions for the auto-download system.
- appmsg.cpp: Processes various messages and updates from the server to manage game state and player interactions.
- badwords.cpp: Implements filtering of bad words in text strings within the game.
- clintlib.cpp: Contains core client operations, including event handling and mission management.
- clintlib.h: Manages client-side operations and interactions, such as player and mission data.

Largest files: clintlib.cpp (4396 lines), appmsg.cpp (3594 lines), clintlib.h (2478 lines), AutoDownload.cpp (1545 lines), AutoDownload.h (744 lines).

## `src/clintlib/AutoDownload.cpp`

PURPOSE: Handles auto-download functionality for Allegiance, including managing file downloads, verifying local files, and handling errors.

KEY PARTS:
- CAutoDownloadImpl: Implements IAutoDownload and IHTTPSessionSink interfaces to manage the download process.
- CFileInfo: Stores file information such as CRC, size, and timestamp.
- SetFTPSite: Sets the FTP site URL and initial directory for downloading game files.
- BeginUpdate: Starts the update process by initiating the download of FileList.txt.
- HandleAutoDownload: Orchestrates the auto-download process, verifying local files and downloading necessary game files.
- OnFileCompleted: Handles completion of a file download.
- CLocalFilesVerifier: Manages local file verification and downloading.

CONNECTS TO:
- IAutoDownload
- IHTTPSessionSink
- Reloader.exe

NOTES:
- "Need to restart allegiance"
- "This command-line needs to be in sync with the command-line reader in Reloader.exe"

PARTS (in file order):

- lines 1-372
  - CAutoDownloadImpl: Implements IAutoDownload and IHTTPSessionSink interfaces for handling auto-download functionality.
  - CFileInfo: Struct to store file information like CRC, size, and timestamp.
  - SetFTPSite: Sets the FTP site URL and initial directory.
  - SetOfficialFileListAttributes: Sets the official CRC and file list size for verification.
  - SetArtPath: Sets the art path ensuring it's in a friendly format.
  - BeginUpdate: Starts the update process by setting up the environment and initiating the download of FileList.txt.
  - HandleAutoDownload: Handles the auto-download process, verifying local files and downloading game files.
  - Abort: Aborts the download process and handles errors.
- lines 377-759
  - DoError: Handles error messages by logging them and notifying the user.
  - AddFileToDownloadList: Adds files to the download list with details like name, size, CRC, and timestamp.
  - OnAnalysisProgress: Notifies about progress in file analysis.
  - HasErrorOccurred: Checks if an error has occurred during the download process.
  - SetNeedToMoveFiles: Sets whether files need to be moved after download.
  - ShouldFilterFile: Determines if a file should be filtered out from downloading.
  - ShouldRetry: Checks if a file should be retried for download based on its content.
  - OnError: Implementation of the error handling method from IFTPSessionUpdateSink.
- lines 761-1144
  - CAutoDownloadImpl: Manages the auto-download process.
  - OnFileCompleted: Handles completion of a file download.
  - OnProgress: Updates progress during file downloads.
  - OnTransferFinished: Notifies when the transfer is finished.
  - ConnectToHTTPSite: Establishes connection to the HTTP server.
  - BeginMainDownload: Starts the main download phase.
  - OnFinishLocalFilesAnalysis: Analyzes local files and decides on next steps.
  - MoveFilesThenRestartOrRelogOn: Moves downloaded files and restarts/relogs if necessary.
- lines 1145-1545
  - CLocalFilesVerifier: Manages local file verification and downloading.
  - ContinueLocalFilesVerification: Continues the analysis of local files for updates.
  - LaunchReloaderAndExit: Restarts Allegiance by launching Reloader.exe and exits the current process.
  - SetLocalFileTime: Sets the file time for a given file.
  - IsTimeClose: Checks if two system times are close within a specified tolerance.
  - ConsiderDownloadingFile: Determines whether to download a file based on its attributes.
  - OpenAndGetFileInfo: Opens a file and retrieves its information.
  - TODOs: "Need to restart allegiance" "This command-line needs to be in sync with the command-line reader in Reloader.exe"

## `src/clintlib/AutoDownload.h`

**PURPOSE:** The file `AutoDownload.h` defines interfaces and utility functions for handling automatic downloading of files in the Allegiance game. It includes classes for setting up download parameters, managing the download process, and moving downloaded files to their final destination.

**KEY PARTS:**
- **IAutoDownload**: Interface for auto-download functionality with methods to set FTP details, begin updates, handle downloads, and abort.
- **IAutoUpdateSink**: Interface for receiving events about the autodownload system.
- **CAutoDownloadUtil**: Utility class with static methods for file operations like moving files and getting executable names.
- **CreateAutoDownload()**: Function to create an instance of IAutoDownload.

**CONNECTS TO:** Other parts of the Allegiance game that require automatic downloading of resources, such as assets, updates, or patches.

**NOTES:**
- Multiple redundant sections for handling different executable files suggest potential refactoring.
- Several commented-out sections need attention.
- The repetitive structure of file name checks and path appending indicates a possible centralized list of files.

## `src/clintlib/appmsg.cpp`

PURPOSE: Handles various messages and updates from the server to manage game state, player interactions, and ship statuses.

KEY PARTS:
- BaseClient::HandleMsg: Processes incoming messages from the server.
- BallotInfo: Represents a ballot with text, ID, and expiration time.
- ServerLightShipUpdate: Updates for light ships.
- Orientation: Represents an orientation in space.
- FM_CS_SWAP_PART: Handles swapping parts between ships.
- PlayerInfo: Manages player-specific information.
- SideID: Represents a side or team ID.

CONNECTS TO: Allegiance game engine systems, including ship management, player interactions, and mission handling.

NOTES: None.

PARTS (in file order):

- lines 1-359
  - BaseClient::HandleMsg: Handles incoming messages from the server.
  - FEDMESSAGE: Structure representing a message sent by the server.
  - Time: Class for handling time-related operations.
  - HRESULT: Return type indicating success or failure of an operation.
  - S_OK: Success code.
  - S_FALSE: Failure code indicating that the message was not handled.
  - E_FAIL: Error code indicating a general failure.
  - FM_S_LOGONACK: Message ID for logon acknowledgment.
- lines 360-710
  - BallotInfo: Represents a ballot with text, ID, and expiration time.
  - ClientTimeFromServerTime: Converts server time to client time.
  - SetViewCluster: Sets the current view cluster for the player.
  - RemovePlayerFromSide and RemovePlayerFromMission: Handles player removal from sides and missions.
  - PlaySoundEffect: Plays a sound effect based on station type.
  - PostText: Posts text messages to the game interface.
  - FM_S_CANCEL_BALLOT: Handles cancellation of a ballot.
  - FM_CS_PING: Handles ping requests to measure network latency.
- lines 712-1075
  - ServerLightShipUpdate: Represents updates for light ships.
  - ServerHeavyShipUpdate: Represents updates for heavy ships.
  - ServerActiveTurretUpdate: Represents updates for active turrets.
  - FM_S_HEAVY_SHIPS_UPDATE: Message type for heavy ship updates.
  - FM_S_PROMOTE: Message type for promotion events.
  - FM_S_WARP_BOMB: Message type for warp bomb events.
  - FM_S_RIPCORD_ACTIVATE: Message type for ripcord activation events.
  - FM_S_RIPCORD_DENIED: Message type for ripcord denial events.
- lines 1076-1424
  - Orientation: Represents an orientation in space.
  - SetOrientation: Sets the orientation of a ship.
  - ShipListIGC: A list of ships.
  - GetShips: Retrieves the list of ships.
  - ShipLinkIGC: A link to a ship in a list.
  - IshipIGC: Interface for a ship.
  - SetCluster: Assigns a ship to a cluster.
  - Time: Represents a time value.
- lines 1425-1771
  - FM_CS_SWAP_PART: Handles swapping parts between ships.
  - FM_CS_RELOAD: Manages reloading of weapons and other equipment.
  - FM_CS_FIRE_MISSILE: Processes firing of missiles by players.
  - FM_S_FIRE_EXPENDABLE: Handles firing of expendable items like mines.
  - FM_S_CREATE_CHAFF: Creates chaff objects for anti-missile defense.
  - FM_S_MISSILE_SPOOFED: Updates missile targets to chaff when spoofed.
  - FM_S_END_SPOOFING: Resets the last created chaff object.
  - FM_S_ASTEROID_DESTROYED: Handles asteroid destruction and explosions.
- lines 1772-2141
  - FM_S_BUILDINGEFFECT_DESTROYED: Handles the destruction of a building effect.
  - FM_S_MISSILE_DESTROYED: Handles the destruction of a missile.
  - FM_S_MINE_DESTROYED: Handles the destruction of a mine.
  - FM_CS_ORDER_CHANGE: Processes changes in orders for ships.
  - FM_S_TELEPORT_ACK: Handles teleportation acknowledgments.
  - FM_S_BOARD_NACK: Handles negative responses to boarding requests.
  - FM_S_LEAVE_SHIP: Manages when a ship leaves another ship.
  - FM_S_ENTER_LIFEPOD: Handles entering a life pod.
- lines 2142-2504
  - PlayerInfo: Represents player information within a mission.
  - MissionInfo: Manages mission details and state.
  - SideIGC: Represents a side or team within a mission.
  - IshipIGC: Interface for ship objects.
  - FM_S_JOINED_MISSION: Message indicating a player has joined a mission.
  - FM_S_PLAYERINFO: Message containing information about a player in the mission.
  - FM_CS_PLAYER_READY: Client-to-server message indicating a player is ready.
  - FM_S_MISSION_STAGE: Server-to-client message indicating the current stage of the mission.
- lines 2505-2852
  - PlayerInfo: Manages player-specific information.
  - SideID: Represents a side or team ID.
  - FM_CS_CHANGE_TEAM_CIV, FM_CS_SET_TEAM_INFO, etc.: Message types for changing team civilization, setting team info, and other related actions.
  - CASTPFM: Macro to cast message pointers to specific types.
  - FindPlayer: Function to find a player by ship ID.
  - RemovePlayerFromMission: Removes a player from the mission with a given reason.
  - m_pClientEventSource: Event source for client events.
  - OnDelRequest, OnTeamCivChange, etc.: Methods on m_pClientEventSource to handle various team-related events.
- lines 2853-3215
  - pCluster->GetClusterSite()->AddExplosion(pship, ...): Adds an explosion effect to a ship based on its type.
  - SoundID destroyedSound; switch (pship->GetPilotType()): Determines and plays the appropriate sound for a ship being destroyed.
  - EjectPlayer(m_pCoreIGC->GetModel(...)): Sets up the camera view when a player dies.
  - ChangeGameState(): Changes the game state based on the current event.
  - AddPlayerToSide(pplayer, pfmJoinSide->sideID): Adds a player to a specific side in the mission.
  - RemovePlayerFromSide(pplayer, pfmQuitSide->reason, ...): Removes a player from their side and handles the reason for quitting.
  - ForwardSquadMessage(pfm): Forwards squad-related messages to the squads screen.
  - ForwardCharInfoMessage(pfm): Forwards character information messages to the character info screen.
- lines 3216-3594
  - FM_S_OBJECT_SPOTTED: Handles messages about spotted objects.
  - FM_S_DEV_COMPLETED: Handles messages about completed research developments.
  - FM_S_SET_BRIEFING_TEXT: Handles messages setting briefing text.
  - FM_LS_SQUAD_MEMBERSHIPS: Handles squad membership updates.
  - FM_S_STATIC_MAP_INFO: Updates static map information.
  - FM_S_RANK_INFO: Updates rank information.
  - FM_L_AUTO_UPDATE_INFO: Initiates auto-update process.
  - FM_L_CREATE_MISSION_ACK: Acknowledges mission creation request.

## `src/clintlib/badwords.cpp`

PURPOSE: This file contains the implementation for filtering bad words in text strings within the Allegiance game. It includes functions to load a list of bad words, build a finite state machine to detect them, and censor the detected words by replacing them with asterisks.

KEY PARTS:
- Machine: Manages the finite state machine used to detect bad words.
- AddWordToFilterAutomata: Adds a word to the filter automata.
- BuildFilterAutomata: Builds the filter automata from a list of bad words.
- FilterBadWords: Censors bad words in a given string by replacing them with asterisks.
- CensorBadWords: Returns a new string with bad words censored if enabled.

CONNECTS TO: none

NOTES:
- The code uses a simple and inefficient algorithm that is likely to be replaced due to performance concerns.
- There are memory leaks, particularly in the `LoadBadWords` function where `pBuffer` is allocated but not properly freed.
- The bad word list is hardcoded within the file.

## `src/clintlib/badwords.h`

PURPOSE: tiny file (11 lines), shown in full rather than summarized.

    #ifndef _BADWORDS_H_
    #define _BADWORDS_H_
    //------------------------------------------------------------------------------
    // functions to handle bad words
    //------------------------------------------------------------------------------
    void    LoadBadWords (void);
    ZString CensorBadWords (const ZString& string);
    void    ToggleCensorDisplay (void);
    bool    CensorDisplay (void);
    //------------------------------------------------------------------------------
    #endif  //  _BADWORDS_H_

## `src/clintlib/clintlib.cpp`

PURPOSE: The file `clintlib.cpp` contains core functionality for managing client operations in Allegiance, including event handling, mission management, and player interactions.

KEY PARTS:
- CriticalSectionManager: Manages critical sections for thread synchronization.
- ClientEventSource: Implements IClientEventSource to manage event sinks and dispatch events.
- MissionInfo: Manages mission details, updates, and player information.
- BaseClient: Main class handling client operations like connecting to servers, managing players, and sending messages.

CONNECTS TO:
- Allegiance game engine systems for networking, graphics, and user interface.

NOTES:
- Contains TODO comments for documentation and code cleanup.
- Uses specific macros and comments indicating features or contributors (e.g., #ALLY, // KGJV #114).

PARTS (in file order):

- lines 1-373
  - CriticalSectionManager: Manages critical sections for thread synchronization.
  - ClientEventSource: Implements IClientEventSource to manage event sinks and dispatch events to them.
  - ForEachSink: Macro to iterate over all non-null sinks and call a method on each.
  - ClientEventSinkDelegate: A delegate class implementing IClientEventSink that forwards calls to another sink.
  - TODO: Add documentation for the classes and methods.
  - Unusual code: The use of #ALLY and // KGJV #114 comments indicating specific features or contributors.
- lines 374-737
  - IClientEventSink::CreateDelegate: Creates a delegate for handling client events.
  - ChatInfo::SetChat: Sets the chat information including recipient, text, and target model.
  - ChatInfo::ClearTarget: Clears the target of the chat message if the model is not a buoy.
  - MissionInfo::MissionInfo: Constructor for MissionInfo, initializes mission definition and side info.
  - MissionInfo::~MissionInfo: Destructor for MissionInfo, cleans up mission definition and side info.
  - MissionInfo::Update(FMD_S_MISSIONDEF): Updates mission information from a FMD_S_MISSIONDEF structure.
  - MissionInfo::Update(FMD_LS_LOBBYMISSIONINFO): Updates mission information from a FMD_LS_LOBBYMISSIONINFO structure, handling lobby-specific details.
  - TODO: Add comments to explain the purpose of some functions and variables.
- lines 739-1098
  - MissionInfo::Update: Updates mission parameters based on lobby information.
  - MissionInfo::GuaranteedPositions: Calculates the number of guaranteed positions for players.
  - MissionInfo::PurgePlayers: Removes all player information and resets mission state.
  - MissionInfo::HasSquad: Checks if a specific squad exists in any side.
  - MissionInfo::SetSideLeader: Sets a player as the leader of their side.
  - MissionInfo::FindPlayer: Determines if a player is present on a given side.
  - MissionInfo::AddPlayer: Adds a player to a specified side.
  - MissionInfo::RemovePlayer: Removes a player from a specified side.
- lines 1099-1466
  - BaseClient: Main class for managing client operations.
  - ConnectToServer: Method for connecting to a game server.
  - ConnectToLobby: Method for connecting to the lobby server.
  - FlushGameState: Method for cleaning up game state when disconnecting.
  - Initialize: Method for initializing the client with a mission.
  - Reinitialize: Method for reinitializing the client after disconnection.
  - Terminate: Method for terminating client operations.
  - UpdateLobbyLoginRequestWithSteamAuthTokenInformation: Method to update lobby login request with Steam authentication information.
- lines 1467-1839
  - BaseClient: Manages client-side operations such as connecting to servers, handling login, and managing player information.
  - ConnectToClub: Handles the connection process to a club server using provided connection information.
  - FindStandaloneServersByName: Searches for standalone servers by name and populates a list with results.
  - SetCDKey: Sets the CD key used for authentication or other purposes.
  - HandleAutoDownload: Manages automatic downloading of resources.
  - GetWeapon: Retrieves the currently selected weapon from the ship.
  - SetAutoPilot: Enables or disables autopilot mode on the ship.
  - DisconnectLobby and DisconnectClub: Handles disconnection from lobby and club servers respectively.
- lines 1840-2192
  - BaseClient: Manages client-side operations such as sending updates and handling messages.
  - SetMessageType: Sets the message type for sending messages.
  - SendMessages: Sends all queued messages over the network.
  - NextWeapon/PreviousWeapon/SetSelectedWeapon: Manage weapon selection on the player's ship.
  - JoinMission/CreateMissionReq/ServerListReq: Handle mission creation and joining requests.
  - CheckServerLag: Checks for server lag and handles session loss if necessary.
  - ReceiveMessages: Receives messages from various messaging systems.
  - OnSessionLost: Handles session termination events.
- lines 2193-2535
  - BaseClient::ParseShellCommand: Parses shell commands for muting and unmuting players.
  - BaseClient::SendChat: Sends chat messages to recipients based on different targets and conditions.
  - BaseClient::ScrollChatUp: and BaseClient::ScrollChatDown: Functions to scroll through the chat list.
  - BaseClient::GetCurrentMessage: Retrieves the current selected message from the chat list.
  - BaseClient::GetCurrentBallot: Retrieves the current ballot if any is available.
  - BaseClient::Vote: Sends a vote response for the current ballot.
  - BaseClient::SkipCurrentBallot: Removes the current ballot without voting.
  - BaseClient::GetBucketStatus: Retrieves the status of a bucket at a specific station.
- lines 2536-2892
  - BaseClient::AddMoneyToBucket: Adds money to a bucket and updates client's money.
  - BaseClient::DonateMoney: Handles player donations by updating recipient and sender's money.
  - BaseClient::GetLobbyMission: Retrieves or creates a mission info based on a cookie.
  - BaseClient::FindPlayerLink: Finds a player link by ship ID.
  - BaseClient::FindAndCreatePlayerLink: Finds or creates a player link for a given ship ID.
  - BaseClient::LookupRankName: Determines the rank name based on rank and civilization.
  - BaseClient::TerminateModelEvent: Handles model termination events, updating references and notifying clients.
  - BaseClient::LoadoutChangeEvent: Notifies clients about loadout changes.
- lines 2893-3239
  - BaseClient::CreateTreasureLocal: Creates a treasure object locally.
  - BaseClient::KillAsteroidEvent: Handles asteroid destruction events.
  - BaseClient::KillProbeEvent: Handles probe destruction events.
  - BaseClient::KillMineEvent: Handles mine destruction events.
  - BaseClient::KillMissileEvent: Handles missile destruction or disarming events.
  - BaseClient::KillTreasureEvent: Handles treasure destruction events.
  - BaseClient::KillShipEvent: Handles ship destruction events, including creating treasures and ejecting players.
  - BaseClient::DamageStationEvent: Handles damage to stations.
- lines 3240-3587
  - BaseClient::Reload: Handles reloading of weapons and fuel for a ship.
  - IexpendableTypeIGC pet = pDispenser->GetExpendableType(): Retrieves the type of expendable being used from the dispenser.
  - ObjectType type = pet->GetObjectType(): Determines the type of object (chaff, mine, probe) based on the expendable type.
  - switch (type): Handles different types of expendables with specific logic for each.
  - SetMessageType(c_mtGuaranteed); BEGIN_PFM_CREATE(m_fm, pfmFE, C, FIRE_EXPENDABLE) END_PFM_CREATE: Sends a message to the server to fire an expendable if connected to the server.
  - ReloadData reloads[c_MaxReloads]: Array used to store data about reloading operations.
  - Mount   maxWeapons = pht->GetMaxWeapons(): Retrieves the maximum number of weapons that can be mounted on the ship.
  - IweaponIGC pw = (IweaponIGC)(pship->GetMountedPart(ET_Weapon, i)): Iterates through mounted weapons to disable them if they use ammo.
- lines 3588-3956
  - BaseClient::OnQuitSide(): Handles actions when a player quits a side.
  - BaseClient::OnJoinSide(): Initializes settings and data when a player joins a side.
  - BaseClient::OnQuitMission(QuitSideReason reason, const char szMessageParam): Cleans up resources when a mission is quit.
  - BaseClient::OnEnterGame(): Signals the start of a game session.
  - BaseClient::CreateDummyShip(): Creates a placeholder ship for a player.
  - BaseClient::DestroyDummyShip(): Placeholder function (does nothing).
  - BaseClient::RemovePlayerFromSide(PlayerInfo pPlayerInfo, QuitSideReason reason, const char szMessageParam): Handles the removal of a player from a side.
  - TODO: Remove commented-out code related to chat messages.
- lines 3958-4394
  - BaseClient::RemovePlayerFromMission: Handles player removal from a mission.
  - BaseClient::AddPlayerToSide: Adds a player to a specific side in the mission.
  - BaseClient::ResetStaticData: Reloads static game data.
  - DoDecrypt: Decrypts data using XOR encryption.
  - CfgInfo::GetCfgProfileString: Retrieves configuration profile string from a file.
  - CfgInfo::Load: Loads configuration settings from a file.
  - _debugf: Custom debug output function.
  - TODO: Remove commented-out code related to duplicate logon handling.

## `src/clintlib/clintlib.h`

PURPOSE: Manages client-side operations and interactions within Allegiance, including player management, mission handling, and communication.

KEY PARTS:
- CriticalSectionManager: Manages critical sections for thread synchronization.
- PlayerInfo: Holds detailed information about a player, including their ship, mission status, and connection data.
- BaseClient: The main client class with various member variables and functions related to client operations.
- SetWing: Sets the wing ID for a ship and updates the network if connected.
- RequestRipcord: Requests a ripcord to another cluster or handles local ripcord logic.
- RipcordLocal: Handles the actual ripcord process for a ship locally.
- GetViewCluster: Returns the current view cluster.
- SetViewCluster: Sets the view cluster and optionally updates the position.
- RequestViewCluster: Requests a change in view cluster.
- ResetShip: Resets the ship's state.
- BuyLoadout: Buys a loadout for a ship and optionally launches it.
- SaveCustomLoadout: Saves a custom loadout for a ship.
- ReplaceLoadout: Replaces the current loadout of a ship with a cached loadout.
- Mount: Enumerates mount points on a ship.
- TryToBuyParts: Attempts to buy parts for a ship at a station within a budget.

CONNECTS TO: Allegiance game engine, network communication systems, and player interface components.

NOTES: None.

PARTS (in file order):

- lines 1-367
  - CriticalSectionManager: Manages critical sections for thread synchronization.
  - IClientEventSource: Interface for event sources that can notify sinks about various game events.
  - IClientEventSink: Interface for event sinks that receive notifications from event sources.
  - ChatInfo: Represents a chat message with details like sender, content, and target.
  - BallotInfo: Holds information about a ballot or vote in the game.
  - PlayerInfo: Contains detailed information about a player, including their ship, mission status, and connection data.
  - Review TODOs related to player-pings feature (w0dk4).
  - Investigate potential improvements for thread synchronization using CriticalSectionManager.
- lines 368-713
  - PlayerInfo: Holds information about a player including their side ID, ship ID, character name, money, and ship status.
  - SideID: An identifier for a side or team in the game.
  - ShipID: An identifier for a ship.
  - Money: Represents an amount of money.
  - ShipStatus: Contains details about the current state of a ship, such as its sector, hull type, and parent.
  - PlayerList: A list wrapper for PlayerInfo objects.
  - SideInfo: Holds information about a side including its members and requests.
  - MissionInfo: Manages mission parameters and player information.
- lines 715-1058
  - BallotList: A list of ballots.
  - ConnectInfo: Contains information needed for connecting and logging on to the server.
  - FedMessaging: Manages messaging between clients and servers.
  - BaseClient: The main client class with various member variables and functions related to client operations.
  - Initialize: Initializes the client.
  - Reinitialize: Reinitializes the client.
  - Terminate: Terminates the client.
  - FlushGameState: Flushes the game state.
- lines 1059-1413
  - SetWing(WingID wid, IshipIGC pship = NULL): Sets the wing ID for a ship and updates the network if connected.
  - RequestRipcord(IshipIGC pship, IclusterIGC pcluster): Requests a ripcord to another cluster or handles local ripcord logic.
  - RipcordLocal(IshipIGC pship, IclusterIGC pcluster): Handles the actual ripcord process for a ship locally.
  - GetViewCluster() const: Returns the current view cluster.
  - SetViewCluster(IclusterIGC pcluster, const Vector pposition = NULL): Sets the view cluster and optionally updates the position.
  - RequestViewCluster(IclusterIGC pcluster, ImodelIGC pmodelTarget = NULL): Requests a change in view cluster.
  - ResetShip(void): Resets the ship's state.
  - BuyLoadout(IshipIGC pshipLoadout, bool bLaunch): Buys a loadout for a ship and optionally launches it.
- lines 1414-1756
  - SaveCustomLoadout: Saves a custom loadout for a ship.
  - SaveLoadout: Saves the current loadout of a ship.
  - RestoreLoadout: Restores a saved loadout to a ship, handling upgrades and budget constraints.
  - RestoreLoadoutOnCarrier: Restores a saved loadout on a carrier, ensuring parts are duplicated correctly.
  - m_sidTeleportAfterDisembark: Stores the object ID of the station where teleportation occurs after disembarking.
  - BoardShip: Handles boarding a ship.
  - CachedLoadoutLink: Represents a link in a cached loadout list.
  - CachedPartLink: Represents a link in a cached part list.
- lines 1757-2129
  - ReplaceLoadout(IshipIGC pship, IstationIGC pstation, CachedLoadoutList & loadoutList): Replaces the current loadout of a ship with a cached loadout.
  - ReplaceLoadout(IshipIGC pship, IstationIGC pstation, CachedLoadout pcl, Money budget, CachedLoadoutList & loadoutList): Overloaded version that allows specifying a specific loadout to replace the current one.
  - RestoreLoadout(IstationIGC pstation): Restores the ship's loadout from a station.
  - ReplaceLoadout(IstationIGC pstation, bool bLaunch = false): Replaces the ship's loadout at a station and optionally launches it.
  - LoadCustomLoadoutFile(void): Loads custom loadouts from a file.
  - SaveCustomLoadoutFile(void): Saves custom loadouts to a file.
  - ExportCachedLoadout(CachedLoadoutList  loadouts, int num, byte  buffer): Exports cached loadouts to a buffer.
  - ImportCachedLoadout(byte  buffer): Imports cached loadouts from a buffer.
- lines 2131-2478
  - Mount: Enumerates mount points on a ship.
  - IpartIGC GetMountedPart(EquipmentType et, Mount mountID): Retrieves a part by equipment type and mount ID.
  - TryToBuyParts(IshipIGC pship, IstationIGC pstation, IBudgetIGC pbudget, IhtIGC pht, const ZString& preferredPartTypes): Attempts to buy parts for a ship at a station within a budget.
  - BuyPartOnBudget(IshipIGC pship, IpartTypeIGC ppt, Mount mountID, IBudgetIGC pbudget): Buys a part on a budget if possible.
  - DropPart(IpartIGC ppart): Drops a part from the ship.
  - SwapPart(IpartIGC ppart, Mount mountNew): Swaps parts between mounts.
  - ForwardSquadMessage(FEDMESSAGE pSquadMessage), ForwardCharInfoMessage(FEDMESSAGE pCharInfoMessage), ForwardLeaderBoardMessage(FEDMESSAGE pLeaderBoardMessage): Forward messages to respective screens.
  - OnReload(IpartIGC ppart, bool bConsumed): Handles part reload events.

## `src/clintlib/pch.cpp`

PURPOSE: tiny file (17 lines), shown in full rather than summarized.

    /*
    **  Copyright (C) 1996, 1997 Microsoft Corporation. All Rights Reserved.
    **
    **  File:	stdafx.cpp
    **
    **  Author:
    **
    **  Description:
    **      The standard ATL stdafx file. This file was initially created by
    **  the ATL wizard.
    **
    **  History:
    */
    // stdafx.cpp : source file that includes just the standard includes
    //  stdafx.pch will be the pre-compiled header
    //  stdafx.obj will contain the pre-compiled type information
    #include "pch.h"

## `src/clintlib/pch.h`

PURPOSE: This file is a precompiled header for the client library in Allegiance, including essential Windows headers and custom Allegiance-specific headers.

KEY PARTS:
- `windows.h`, `windowsx.h`, `mmsystem.h`: Standard Windows headers.
- `steam_api.h`: Header for Steam API integration.
- `tchar.h`: TCHAR macros for Unicode/ANSI compatibility.
- `guids.h`, `zlib.h`, `utility.h`, `igc.h`, `Zone.h`, `messagesAll.h`, `messages.h`, `messagesLC.h`, `ClubMessages.h`, `AutoDownload.h`, `AllegianceSecurity.h`, `zauth.h`: Custom Allegiance headers for various functionalities.
- `clintlib.h`: Main header file for the client library.

CONNECTS TO: Other client-side modules and systems within Allegiance, including engine components and network communications.

NOTES: None.
