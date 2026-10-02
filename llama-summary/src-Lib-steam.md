# src/Lib/steam

[Back to the index](../LLAMA_SUMMARY.md) - 39 files, 22,381 lines. Notes by a local model, to orient by: not verified facts.

The `src/Lib/steam` folder contains essential interfaces and implementations for interacting with Steam services within the Allegiance game. It facilitates communication between the game and the Steam client, enabling features such as user authentication, matchmaking, remote storage, and more.

- steam_api_interop.cs: Provides interop methods for calling Steam API functions from managed code using P/Invoke.
- steamclientpublic.h: Contains declarations for various enumerations, macros, and classes used in the Steam client API.
- steam_api_flat.h: Declares various Steam API functions for interacting with the Steam client and related services.
- isteammatchmaking.h: Defines interfaces for managing game lobbies and server lists through Steam's matchmaking system.
- isteamremotestorage.h: Defines interfaces for accessing user-generated content (UGC) in Steam's remote storage system.

Largest files: steam_api_interop.cs (11168 lines), steamclientpublic.h (1239 lines), steam_api_flat.h (830 lines), isteammatchmaking.h (751 lines), isteamremotestorage.h (681 lines).

## `src/Lib/steam/isteamapplist.h`

PURPOSE: This file defines an interface for accessing installed applications in the Steam client, allowing debugging tools to list and interact with these apps.

KEY PARTS:
- ISteamAppList: Interface class providing methods to get information about installed apps.
  - GetNumInstalledApps(): Returns the number of installed apps.
  - GetInstalledApps(): Retrieves a list of installed app IDs.
  - GetAppName(): Gets the name of an app by its ID.
  - GetAppInstallDir(): Gets the installation directory of an app by its ID.
  - GetAppBuildId(): Retrieves the build ID of an app.

CONNECTS TO: isteamclient.h, steamtypes.h

NOTES: None.

## `src/Lib/steam/isteamapps.h`

PURPOSE: This file defines the interface for accessing application data and related functionalities in Steam, such as checking subscription status, managing DLC, and requesting proof of purchase keys.

KEY PARTS:
- `ISteamApps`: Interface class providing methods to interact with app data.
  - `BIsSubscribed()`: Checks if the user is subscribed to the current app.
  - `GetCurrentGameLanguage()`: Returns the current game language.
  - `GetDLCCount()`: Returns the number of DLC pieces for the running app.
  - `RequestAppProofOfPurchaseKey()`: Requests a legacy CD-key for the app or its DLC.
- `STEAMAPPS_INTERFACE_VERSION`: Version string for the SteamApps interface.

CONNECTS TO: This file is part of the Steam client library and interacts with other Steam-related components.

NOTES: None.

## `src/Lib/steam/isteamappticket.h`

**PURPOSE:**  
This header file defines an interface for accessing critical bits of a Steam3 application ownership ticket. It provides methods to retrieve the raw signed buffer and indices into that buffer where the app ID and Steam ID are located.

**KEY PARTS:**
- **Class: ISteamAppTicket**
  - Provides a method `GetAppOwnershipTicketData` to fetch data from an application ownership ticket.
  
- **Function: GetAppOwnershipTicketData**
  - Parameters:
    - `nAppID`: The application ID.
    - `pvBuffer`: Pointer to the buffer where the data will be stored.
    - `cbBufferLength`: Length of the buffer.
    - `piAppId`: Output parameter for the app ID.
    - `piSteamId`: Output parameter for the Steam ID.
    - `piSignature`: Output parameter for the signature.
    - `pcbSignature`: Output parameter for the size of the signature.
  - Returns: A uint32 value indicating success or failure.

- **Macro: STEAMAPPTICKET_INTERFACE_VERSION**
  - Defines the version of the interface as "STEAMAPPTICKET_INTERFACE_VERSION001".

**CONNECTS TO:**  
This file is used by the Steam DRM wrapper to verify application ownership with greater confidence.

**NOTES:**  
None.

## `src/Lib/steam/isteamclient.h`

PURPOSE: This file defines the interface for interacting with the Steam client API from within a game or application.

KEY PARTS:
- `ISteamClient`: Main interface for creating and managing communication pipes to the Steam client.
- `CreateSteamPipe()`: Creates a new communication pipe to the Steam client.
- `BReleaseSteamPipe()`: Releases a previously created communication pipe.
- `ConnectToGlobalUser()`: Connects to an existing global user.
- `CreateLocalUser()`: Creates a local user that won't be shared with anyone else.
- `GetISteamUser()`, etc.: Retrieves various Steam interfaces like ISteamUser, ISteamFriends, etc.

CONNECTS TO: Other Steam API interfaces and the Steam client itself.

NOTES: None.

## `src/Lib/steam/isteamcontroller.h`

PURPOSE: This file defines the interface for interacting with Steam Controllers in the Allegiance game.

KEY PARTS:
- `ISteamController`: Main interface class providing methods to interact with Steam Controllers.
- `ControllerHandle_t`, `ControllerActionSetHandle_t`, etc.: Types used to identify controllers and actions.
- `GetConnectedControllers()`: Enumerates currently connected controllers.
- `ActivateActionSet()`: Activates a specific action set on a controller.
- `GetDigitalActionData()`, `GetAnalogActionData()`: Retrieves the current state of digital and analog actions.
- `TriggerHapticPulse()`, `TriggerVibration()`: Controls haptic feedback and vibration.

CONNECTS TO: `isteamclient.h` (dependency for Steam API interactions).

NOTES: None.

## `src/Lib/steam/isteamfriends.h`

PURPOSE: This file defines the interface and callbacks for accessing information about users, including friends, clans, and game sessions.

KEY PARTS:
- `ISteamFriends`: Interface class providing methods to interact with user data.
- `PersonaStateChange_t`: Callback structure for changes in a friend's status.
- `SetPersonaNameResponse_t`: Callback structure for the result of changing the user's persona name.
- `FriendRichPresenceUpdate_t`: Callback structure for updates to a friend's rich presence.

CONNECTS TO: `isteamclient.h` and other Steam client-related files.

NOTES: None.

## `src/Lib/steam/isteamgamecoordinator.h`

PURPOSE: Defines the interface and callbacks for interacting with the Game Coordinator in a Steam application.

KEY PARTS:
- ISteamGameCoordinator: Interface class for sending and receiving messages from the Game Coordinator.
  - SendMessage: Sends a message to the Game Coordinator.
  - IsMessageAvailable: Checks if there is a message waiting in the queue.
  - RetrieveMessage: Retrieves the first message from the queue.
- EGCResults: Enumerates possible return values for API functions.
- GCMessageAvailable_t: Callback notification when a new message is available.
- GCMessageFailed_t: Callback notification when a message fails to reach the Game Coordinator.

CONNECTS TO: steamclientpublic.h, steamtypes.h

NOTES: None.

## `src/Lib/steam/isteamgameserver.h`

PURPOSE: This file defines the interface for a game server to interact with Steam's servers for authentication, logging in, and managing server data.

KEY PARTS:
- ISteamGameServer: Main class providing methods for initializing, logging on, setting server properties, managing players, and interacting with Steam.
- LogOn: Initiates login process for the game server.
- SetProduct, SetGameDescription, etc.: Methods to set various server properties like product name, map name, etc.
- SendUserConnectAndAuthenticate: Validates a user's connection ticket.
- EnableHeartbeats: Controls whether the server sends heartbeats to Steam's master servers.

CONNECTS TO: isteamclient.h (for ISteamClient interface)

NOTES: None

## `src/Lib/steam/isteamgameserverstats.h`

PURPOSE: Defines an interface for game servers to interact with Steam's statistics and achievements system.

KEY PARTS:
- ISteamGameServerStats: Interface class providing methods to request and update user stats and achievements.
- RequestUserStats: Downloads stats for a specified user.
- GetUserStat: Retrieves the value of a specific stat for a user.
- SetUserStat: Updates or sets the value of a specific stat for a user.
- StoreUserStats: Stores the current user stats on the server.

CONNECTS TO: isteamclient.h

NOTES: None.

## `src/Lib/steam/isteamhtmlsurface.h`

PURPOSE: This file defines the interface for displaying HTML pages in a texture using Steam's API.

KEY PARTS:
- `ISteamHTMLSurface`: Main interface class for creating and managing HTML browsers.
- `CreateBrowser`: Creates a new browser instance for displaying HTML pages.
- `LoadURL`: Navigates to a specified URL.
- `SetSize`: Sets the size of the HTML browser surface.
- `ExecuteJavascript`: Executes JavaScript code within the current page.

CONNECTS TO: Steam API, other rendering systems.

NOTES: None.

## `src/Lib/steam/isteamhttp.h`

PURPOSE: This file defines the interface for HTTP client functionality in Steam, allowing developers to send HTTP requests and handle responses asynchronously.

KEY PARTS:
- `ISteamHTTP`: Main interface class for HTTP operations.
- `HTTPRequestHandle` and `HTTPCookieContainerHandle`: Types used to manage HTTP request handles and cookie containers respectively.
- `CreateHTTPRequest`, `SendHTTPRequest`, `SendHTTPRequestAndStreamResponse`: Functions to initiate and send HTTP requests.
- `GetHTTPResponseBodyData`, `GetHTTPStreamingResponseBodyData`: Functions to retrieve response data.
- `SetHTTPRequestContextValue`, `SetHTTPRequestHeaderValue`: Functions to set various parameters for HTTP requests.

CONNECTS TO: `isteamclient.h` - This file depends on the Steam client interface for asynchronous API calls and callbacks.

NOTES: None.

## `src/Lib/steam/isteaminventory.h`

PURPOSE: This file defines the interface and callbacks for interacting with Steam's inventory system, allowing games to query, modify, and manage player inventories.

KEY PARTS:
- `ISteamInventory`: Main interface class for accessing Steam Inventory functionality.
- `SteamItemInstanceID_t`: Unique identifier for each item instance.
- `SteamItemDef_t`: Identifier for item types.
- `SteamItemDetails_t`: Structure containing details about an inventory item.
- `SteamInventoryResult_t`: Handle for asynchronous inventory operations.
- `GetResultStatus()`: Checks the status of an asynchronous result handle.
- `GetAllItems()`: Captures the entire state of a user's Steam inventory.
- `SerializeResult()`: Serializes an inventory result set for secure transmission.
- `DeserializeResult()`: Deserializes and verifies a serialized inventory result.

CONNECTS TO: `isteamclient.h` (Steam client interface).

NOTES: None.

## `src/Lib/steam/isteammasterserverupdater.h`

PURPOSE: tiny file (1 lines), shown in full rather than summarized.

    #error "This file isn't used any more"

## `src/Lib/steam/isteammatchmaking.h`

**PURPOSE:** The file `src/Lib/steam/isteammatchmaking.h` defines the interface for managing game lobbies and server lists through Steam's matchmaking system.

**KEY PARTS:**
- **Class: ISteamMatchmaking**
  - Manages game lobbies and favorites.
  
- **Function: RequestLobbyList()**
  - Asynchronously requests a list of relevant lobbies based on current filters.
  
- **Function: CreateLobby(ELobbyType eLobbyType, int cMaxMembers)**
  - Creates a new lobby with the specified type and maximum members.
  
- **Function: JoinLobby(CSteamID steamIDLobby)**
  - Joins an existing lobby identified by its CSteamID.
  
- **Function: SetLobbyData(CSteamID steamIDLobby, const char *pchKey, const char *pchValue)**
  - Sets metadata for a lobby using a key-value pair.
  
- **Function: GetLobbyData(CSteamID steamIDLobby, const char *pchKey)**
  - Retrieves the value associated with a specific key in the lobby's metadata.
  
- **Function: SendLobbyChatMsg(CSteamID steamIDLobby, const void *pvMsgBody, int cubMsgBody)**
  - Sends a chat message to all users in the specified lobby.
  
- **Class: ISteamMatchmakingServerListResponse**
  - Callback interface for handling responses after querying server lists.
  
- **Function: RequestInternetServerList(AppId_t iApp, ARRAY_COUNT(nFilters) MatchMakingKeyValuePair_t **ppchFilters, uint32 nFilters, ISteamMatchmakingServerListResponse *pRequestServersResponse)**
  - Asynchronously requests a list of internet servers that match specified filters.
  
- **Function: GetServerDetails(HServerListRequest hRequest, int iServer)**
  - Retrieves details for a server at the given index in the server list.
  
- **Function: PingServer(uint32 unIP, uint16 usPort, ISteamMatchmakingPingResponse *pRequestServersResponse)**
  - Asynchronously pings a server to get its response time and other details.

**CONNECTS TO:** Steam matchmaking system and related callbacks.

**NOTES:** 
- Contains commented-out code related to `RequestFriendsLobbiesResponse_t`.
- Ends with an unusual use of `#pragma pack( pop )` to restore default packing alignment.

## `src/Lib/steam/isteammusic.h`

PURPOSE:
This file defines the interface for controlling music playback within the Steam client. It includes functions to check if music is enabled, playing, and to control playback status such as play, pause, previous, and next.

KEY PARTS:
- ISteamMusic: Interface class with methods to control music playback.
  - BIsEnabled(): Checks if music is enabled.
  - BIsPlaying(): Checks if music is currently playing.
  - GetPlaybackStatus(): Returns the current playback status.
  - Play(): Starts or resumes music playback.
  - Pause(): Pauses music playback.
  - PlayPrevious(): Plays the previous track in the playlist.
  - PlayNext(): Plays the next track in the playlist.
  - SetVolume(float flVolume): Sets the volume level (0.0 to 1.0).
  - GetVolume(): Returns the current volume level.

CONNECTS TO:
- isteamclient.h: This header file is included, indicating that ISteamMusic depends on Steam client functionality.

NOTES:
- None

## `src/Lib/steam/isteammusicremote.h`

PURPOSE: This file defines the interface for a Steam Music Remote, allowing applications to interact with and control music playback remotely.

KEY PARTS:
- ISteamMusicRemote: Interface class for managing remote music controls.
- RegisterSteamMusicRemote: Registers the application as a Steam Music Remote.
- DeregisterSteamMusicRemote: Unregisters the application as a Steam Music Remote.
- BIsCurrentMusicRemote: Checks if the application is currently the active music remote.
- SetDisplayName: Sets the display name for the music remote.
- UpdatePlaybackStatus: Updates the playback status (e.g., playing, paused).
- UpdateVolume: Updates the volume level.

CONNECTS TO: isteamclient.h and isteammusic.h

NOTES: None.

## `src/Lib/steam/isteamnetworking.h`

**PURPOSE:**  
This file defines the interface for managing network connections between game clients and servers using Steam's networking services. It includes functions for sending and receiving data over peer-to-peer connections, as well as establishing and closing sessions.

**KEY PARTS:**
- **ISteamNetworking:** The main interface class providing methods to send and receive packets, manage sessions, and handle socket operations.
- **SendP2PPacket:** Sends a packet of data to another user via the P2P network.
- **IsP2PPacketAvailable:** Checks if there is any data available for reading from a peer-to-peer session.
- **ReadP2PPacket:** Reads a packet sent by another user.
- **AcceptP2PSessionWithUser:** Accepts a request to establish a peer-to-peer session with another user.
- **CloseP2PSessionWithUser:** Closes an established peer-to-peer session with another user.
- **GetP2PSessionState:** Retrieves the current state of a peer-to-peer session.
- **AllowP2PPacketRelay:** Allows or disallows using relay servers for P2P connections.

**CONNECTS TO:**
- `steamtypes.h`
- `steamclientpublic.h`

**NOTES:**
- The file includes enums and structures to define error codes, send types, and socket states.
- Callbacks are defined for handling session requests, connection failures, and socket status changes.

## `src/Lib/steam/isteamparentalsettings.h`

PURPOSE: Defines an interface for accessing and managing Steam parental settings, including enabling/disabling parental controls, blocking applications and features.

KEY PARTS:
- ISteamParentalSettings: Interface class with methods to check the status of parental controls and block lists.
- EParentalFeature: Enumerates different features that can be controlled by parental settings.
- STEAMPARENTALSETTINGS_INTERFACE_VERSION: Version string for the interface.
- SteamParentalSettingsChanged_t: Callback structure indicating when parental settings have changed.

CONNECTS TO: Steam client or server systems responsible for user authentication and content management.

NOTES: None.

## `src/Lib/steam/isteamps3overlayrenderer.h`

PURPOSE: This file defines interfaces and enums for integrating the Steam overlay rendering functionality in a game running on PS3.

KEY PARTS:
- `EOverlayGradientDirection`: Enumerates gradient directions for rendering.
- `STEAM_COLOR_RED`, `STEAM_COLOR_GREEN`, `STEAM_COLOR_BLUE`, `STEAM_COLOR_ALPHA`: Macros to extract color components from ARGB packed DWORD colors.
- `ISteamPS3OverlayRenderHost`: Interface the game must implement to provide Steam with rendering capabilities.
  - `DrawTexturedRect`: Draws a textured rectangle with optional gradient effects.
  - `LoadOrUpdateTexture`: Loads or updates a texture for rendering.
  - `DeleteTexture`: Deletes a previously loaded texture.
  - `DeleteAllTextures`: Deletes all previously uploaded textures.
- `ISteamPS3OverlayRender`: Interface Steam exposes to the game for controlling overlay rendering and input handling.
  - `BHostInitialize`: Initializes the Steam overlay with the game's render host interface.
  - `Render`: Signals the Steam overlay to render in the current frame.
  - `BHandleCellPadData`: Handles controller input data, indicating if the overlay is active.
  - `BResetInputState`: Resets input state when no controllers are connected or XMB intercepts input.

CONNECTS TO: Allegiance game engine and Steam SDK for PS3.

NOTES: None.

## `src/Lib/steam/isteamremotestorage.h`

PURPOSE: This file defines the interface and callbacks for accessing user-generated content (UGC) in Steam's remote storage system.

KEY PARTS:
- ISteamRemoteStorage: Interface class providing methods to interact with UGC.
- RemoteStorageFileWriteAsyncComplete_t, etc.: Callback structs for asynchronous operations results.

CONNECTS TO: Steam client library, used by other parts of the game to access and manage user-generated content.

NOTES: None.

## `src/Lib/steam/isteamscreenshots.h`

PURPOSE: This file defines the interface for managing user screenshots in Steam, including adding screenshots from disk and applying metadata.

KEY PARTS:
- ISteamScreenshots: Interface class for screenshot management.
  - WriteScreenshot: Writes a screenshot to the library using raw image data.
  - AddScreenshotToLibrary: Adds a screenshot from disk to the library.
  - TriggerScreenshot: Requests Steam to take a screenshot.
  - HookScreenshots: Toggles whether the game or Steam handles screenshots.
  - SetLocation: Sets metadata about a screenshot's location.
  - TagUser: Tags a user as visible in the screenshot.
  - TagPublishedFile: Tags a published file as visible in the screenshot.
  - IsScreenshotsHooked: Checks if the app has hooked screenshots.
  - AddVRScreenshotToLibrary: Adds a VR screenshot to the library.

CONNECTS TO: isteamclient.h, which provides the base interface for Steam API interactions.

NOTES: None.

## `src/Lib/steam/isteamugc.h`

**PURPOSE:** This file defines the interface and callbacks for interacting with Steam's User Generated Content (UGC) system, allowing developers to query, create, update, and manage user-generated content within their applications.

**KEY PARTS:**
- **Class: ISteamUGC** - Provides methods for querying UGC, creating new items, updating existing items, managing subscriptions, and tracking playtime.
- **Structs:** Various structs define the data structures used in callbacks such as `SteamUGCQueryCompleted_t`, `CreateItemResult_t`, and `DownloadItemResult_t`.
- **Enums:** Defines constants and types for different UGC types, user lists, query options, item states, and more.

**CONNECTS TO:** This file is part of the Steamworks SDK and depends on other components like `isteamclient.h` for basic Steam functionality.

**NOTES:** None.

## `src/Lib/steam/isteamunifiedmessages.h`

PURPOSE: This header file defines the interface for sending and receiving unified messages using the Steam Client. It is used to interact with services in a binary serialized format.

KEY PARTS:
- `ISteamUnifiedMessages`: Interface class providing methods to send service methods, get response information, retrieve response data, release messages, and send notifications.
- `SendMethod`: Sends a service method and returns a handle.
- `GetMethodResponseInfo`: Retrieves the size of the response and the result.
- `GetMethodResponseData`: Gets the response data in binary serialized form.
- `ReleaseMethod`: Releases the message and its allocated memory.
- `SendNotification`: Sends a service notification.

CONNECTS TO: Steam Client

NOTES: None

## `src/Lib/steam/isteamuser.h`

PURPOSE: This file defines the interface for accessing and manipulating a Steam account associated with one client instance. It includes functions for user authentication, game server connections, voice recording, trading card badges, and microtransactions.

KEY PARTS:
- ISteamUser: Main class providing methods to interact with Steam user accounts.
- CallbackMsg_t: Structure used for passing callback data.
- SteamServersConnected_t: Callback indicating a connection to the Steam servers has been established.
- ValidateAuthTicketResponse_t: Callback for authentication ticket validation results.
- EncryptedAppTicketResponse_t: Callback for encrypted app ticket requests.

CONNECTS TO: isteamclient.h, which provides the base interface for Steam client interactions.

NOTES: None.

## `src/Lib/steam/isteamuserstats.h`

PURPOSE: The file defines the interface and callbacks for accessing user statistics, achievements, and leaderboards in a game through Steam.

KEY PARTS:
- `ISteamUserStats`: Main interface class for interacting with user stats, achievements, and leaderboards.
- `CALL_BACK` and `CALL_RESULT`: Macros used to define callback structures and their associated result types.
- Enums: Various enums defining constants like leaderboard sort methods, display types, and data request types.

CONNECTS TO: `isteamclient.h`, `isteamremotestorage.h`

NOTES: None.

## `src/Lib/steam/isteamutils.h`

PURPOSE: This file defines the interface to utility functions in Steam, providing various functionalities such as time-related queries, image retrieval, API call management, and overlay interactions.

KEY PARTS:
- `ISteamUtils`: Interface class for user-independent utility functions.
  - Functions like getting seconds since app active, server real-time, IP country, and more.
- Enums: Defines constants for gamepad text input modes, line modes, API call failures, and file signature checks.
- Callbacks: Structures for various events such as country change, low battery power, API call completion, Steam shutdown, and gamepad text input dismissal.

CONNECTS TO:
- `isteamclient.h`: This header is included to provide necessary definitions for the Steam client interface.

NOTES:
- None.

## `src/Lib/steam/isteamvideo.h`

PURPOSE: This file defines the interface for interacting with Steam Video services, allowing applications to retrieve video URLs and check broadcasting status.

KEY PARTS:
- ISteamVideo: Interface class providing methods to get video URLs and check if a user is broadcasting.
  - GetVideoURL: Retrieves a URL suitable for streaming a given video app ID's video.
  - IsBroadcasting: Checks if the user is currently uploading a live broadcast and returns the number of viewers.
  - GetOPFSettings: Retrieves OPF (Open Packaging Format) settings for 360 Video Playback.
  - GetOPFStringForApp: Retrieves the OPF string for a specific video app ID.

CONNECTS TO:
- isteamclient.h: This header file is included to provide necessary definitions and interfaces for Steam client functionalities.

NOTES:
- None.

## `src/Lib/steam/matchmakingtypes.h`

PURPOSE: This file defines types and constants used for matchmaking in the game Allegiance. It includes structures for storing server information and key-value pairs for filtering servers.

KEY PARTS:
- `MatchMakingKeyValuePair_t`: Stores a key/value pair used in matchmaking queries.
- `EMatchMakingServerResponse`: Enumerates possible responses from matchmaking server queries.
- `servernetadr_t`: Represents the network address of a game server, including IP, query port, and connection port.
- `gameserveritem_t`: Holds detailed information about a game server.

CONNECTS TO: `NETADR_H` for network address handling.

NOTES: None.

## `src/Lib/steam/steam_api.h`

PURPOSE: This file defines the Steam API header for interfacing with Steam services in applications.

KEY PARTS:
- `SteamAPI_Init`: Initializes the Steam API.
- `SteamAPI_Shutdown`: Shuts down the Steam API.
- `SteamClient`, `SteamUser`, etc.: Global accessors for Steamworks C++ APIs.
- `CCallback` and `CCallResult`: Classes for handling callbacks and call results.
- `SteamAPI_RunCallbacks`: Processes callbacks.

CONNECTS TO: steamclient.dll, other Steam-related libraries.

NOTES: None.

## `src/Lib/steam/steam_api_flat.h`

**PURPOSE:** The `steam_api_flat.h` file contains declarations for various Steam API functions that allow developers to interact with the Steam client and related services. These functions cover user management, matchmaking, remote storage, controller interaction, UGC queries, game server management, parental controls, and video management.

**KEY PARTS:**
- **Name:** SteamAPI_ISteamClient_CreateSteamPipe
  - Description: Creates a new Steam pipe for communication with the Steam client.
  
- **Name:** SteamAPI_ISteamUser_BLoggedOn
  - Description: Checks if the user is currently logged on to Steam.
  
- **Name:** SteamAPI_ISteamFriends_GetPersonaName
  - Description: Retrieves the current persona name of the local player.
  
- **Name:** SteamAPI_ISteamUtils_GetSecondsSinceAppActive
  - Description: Returns the number of seconds since the application was last active.
  
- **Name:** SteamAPI_ISteamMatchmaking_GetFavoriteGameCount
  - Description: Returns the count of favorite games stored by the user.
  
- **Name:** SteamAPI_ISteamClient_GetISteamUser
  - Description: Retrieves an interface to the ISteamUser class for a given user and pipe.
  
- **Name:** SteamAPI_ISteamMatchmaking_AddRequestLobbyListStringFilter
  - Description: Adds a string filter for lobby search.
  
- **Name:** SteamAPI_ISteamRemoteStorage_FileWriteAsync
  - Description: Asynchronously writes data to a remote storage file.
  
- **Name:** SteamAPI_ISteamUGC_RequestUGCDetails
  - Description: Requests details about a user-generated content item.

**CONNECTS TO:** Other Steam API files and libraries, such as `steam_api.h`, which contains the implementation of these functions.

**NOTES:** The use of `intptr_t` for passing instance pointers in many functions suggests a design pattern where each API call is associated with an instance to manage state or context. Functions like `SteamAPI_ISteamUGC_RequestUGCDetails` and `SteamAPI_ISteamInventory_GenerateItems` handle complex data structures and operations, indicating that the Steam API deals extensively with user-generated content and inventory management. There are several TODO comments indicating areas that need further implementation or review.

## `src/Lib/steam/steam_api_internal.h`

PURPOSE: This header file contains internal declarations and definitions for the Steamworks API in Allegiance. It provides functions to initialize and access various Steam interfaces.

KEY PARTS:
- `SteamAPI_GetHSteamUser`: Retrieves the handle to the current user.
- `SteamInternal_ContextInit`: Initializes the Steam context.
- `SteamInternal_CreateInterface`: Creates an interface for a given version.
- `CSteamAPIContext`: Manages the initialization and access of Steam interfaces.
- `_STEAM_CALLBACK_AUTO_HOOK`, `_STEAM_CALLBACK_HELPER`, `_STEAM_CALLBACK_SELECT`: Macros to handle Steam callbacks.
- `CCallResult`: Maps Steam async call results to class member functions.
- `CCallback`: Maps Steam callbacks to class member functions.

CONNECTS TO: Other Steamworks API files and the Allegiance game code that uses these interfaces.

NOTES: None.

## `src/Lib/steam/steam_api_interop.cs`

**PURPOSE:** The file `src/Lib/steam/steam_api_interop.cs` provides interop methods for calling Steam API functions from managed code using P/Invoke. It includes bindings for various Steam services such as remote storage, user statistics, apps, networking, screenshots, music, and HTTP requests.

**KEY PARTS:**
- **Class:** NativeEntrypoints
  - Purpose: Contains static methods that provide bindings for Steam API functions using P/Invoke to call native C++ functions from a DLL.
  
- **Function:** SteamAPI_ISteamClient_CreateSteamPipe
  - Purpose: Creates a new pipe to communicate with the Steam client.

- **Function:** SteamAPI_ISteamClient_ConnectToGlobalUser
  - Purpose: Connects to the global user on the specified pipe.

- **Function:** SteamAPI_ISteamClient_GetISteamUser
  - Purpose: Retrieves an interface for interacting with the Steam user.

- **Class:** SteamAPI_ISteamRemoteStorage
  - Name: Handles operations related to cloud-based file storage.
  
- **Class:** SteamAPI_ISteamUserStats
  - Name: Manages user achievements and statistics.
  
- **Class:** SteamAPI_ISteamNetworking
  - Name: Enables peer-to-peer networking functionality.

- **Function:** SteamAPI_ISteamHTTP_SetHTTPRequestCookieContainer
  - Purpose: Sets a cookie container for HTTP requests.

- **Class:** CSteamClient
  - Purpose: Manages Steam client connections and provides access to various Steam interfaces.
  
- **Function:** GetIntPtr()
  - Purpose: Returns the internal pointer to the Steam client instance.

**CONNECTS TO:** Other files in the `src/Lib/steam` directory, particularly those related to specific Steam services like remote storage, user statistics, networking, and HTTP requests.

**NOTES:** The file is marked as auto-generated, meaning any manual changes will be overwritten. There are TODO comments indicating areas that need further development or review. Some functions have parameters like `IntPtr pRequestServersResponse`, which suggests that these functions are part of larger structures or classes not shown here.

## `src/Lib/steam/steam_gameserver.h`

PURPOSE: This file defines the interface and functionality for a Steam game server, including initialization, callback management, and access to various Steam API components.

KEY PARTS:
- `SteamGameServer_Init`: Initializes the ISteamGameServer interface with specified parameters.
- `SteamGameServer_Shutdown`: Shuts down the Steam game server.
- `SteamGameServer_RunCallbacks`: Processes callbacks for the game server.
- `SteamGameServer_ReleaseCurrentThreadMemory`: Releases memory associated with the current thread.
- `SteamGameServer_BSecure`: Checks if the server is secure.
- `SteamGameServer_GetSteamID`: Retrieves the Steam ID of the server.
- `CSteamGameServerAPIContext`: Manages and provides access to various Steam API interfaces.

CONNECTS TO: steam_api.h, isteamgameserver.h, isteamgameserverstats.h

NOTES: None.

## `src/Lib/steam/steamclientpublic.h`

**PURPOSE:** The file `steamclientpublic.h` contains declarations for various enumerations, macros, and classes used in the Steam client API. These definitions facilitate interaction with Steam services such as authentication, user management, and game metadata.

**KEY PARTS:**
- **Name:** EResult  
  **What it does:** Enumerates result codes indicating success or failure of Steam operations.
- **Name:** EVoiceResult  
  **What it does:** Enumerates error codes for voice-related functions in the Steam API.
- **Name:** EDenyReason  
  **What it does:** Defines reasons why a client may be denied access to a game server or resource.
- **Class: CSteamID**  
  **What it does:** Represents a unique identifier for users, clans, and game servers on the Steam platform.
- **Function: BIsOculusHMD**  
  **What it does:** Checks if the given HMD type is an Oculus HMD.
- **Macro: k_steamIDNil**  
  **What it does:** Represents an invalid Steam ID.

**CONNECTS TO:** Other parts of the Steam client API and related modules for handling user authentication, game metadata, and voice communication.

**NOTES:** The file includes comments and TODOs indicating areas that may require further development or review.

## `src/Lib/steam/steamencryptedappticket.h`

PURPOSE: This file contains utilities to decode and decrypt a ticket from the Steam API's ISteamUser::RequestEncryptedAppTicket and ISteamUser::GetEncryptedAppTicket methods.

KEY PARTS:
- CSteamEncryptedAppTicket: Class for managing encrypted app tickets.
- SteamEncryptedAppTicket_BDecryptTicket: Function to decrypt an encrypted ticket using a symmetric key.
- SteamEncryptedAppTicket_BIsTicketForApp: Function to check if the decrypted ticket is for a specific application.
- SteamEncryptedAppTicket_GetTicketIssueTime: Function to get the issue time of the decrypted ticket.
- SteamEncryptedAppTicket_GetTicketSteamID: Function to get the Steam ID associated with the decrypted ticket.
- SteamEncryptedAppTicket_GetTicketAppID: Function to get the application ID from the decrypted ticket.
- SteamEncryptedAppTicket_BUserOwnsAppInTicket: Function to check if the user owns a specific app in the ticket.
- SteamEncryptedAppTicket_BUserIsVacBanned: Function to check if the user is VAC banned based on the ticket.
- SteamEncryptedAppTicket_GetUserVariableData: Function to get variable data associated with the user from the ticket.

CONNECTS TO: steam_api.h

NOTES: None.

## `src/Lib/steam/steamhttpenums.h`

PURPOSE: This file contains enumerations related to HTTP methods and status codes used in the Steam client API.

KEY PARTS:
- EHTTPMethod: Enumerates various HTTP request methods like GET, POST, PUT, DELETE.
- EHTTPStatusCode: Enumerates HTTP response status codes from informational (100-199) to error (500-599).

CONNECTS TO: Used by the Steam client API for handling HTTP requests and responses.

NOTES: None.

## `src/Lib/steam/steamps3params.h`

PURPOSE: This header file defines parameters and structures required for initializing Steam API on PlayStation 3.

KEY PARTS:
- `SteamPS3Params_t`: Structure containing initialization parameters for Steam API on PS3.
- `STEAM_PS3_PATH_MAX`, `STEAM_PS3_SERVICE_ID_MAX`, etc.: Constants defining maximum lengths for various string fields in the structure.
- `m_unVersion`: Version of the parameters structure, set to `STEAM_PS3_CURRENT_PARAMS_VER`.
- `m_nAppId`: Application ID specific to the game.
- `m_rgchInstallationPath`, `m_rgchSystemCache`, etc.: Paths and IDs for various directories and services.

CONNECTS TO: steam_api_ps3.prx (PS3 module)

NOTES: None.

## `src/Lib/steam/steamtypes.h`

PURPOSE: This header file defines various types and constants used throughout the Steamworks API, including data types for identifiers, timestamps, and handles.

KEY PARTS:
- `uint8`, `int8`, `uint16`, `int16`, `uint32`, `int32`, `uint64`, `int64`: Basic integer types.
- `GID_t`, `JobID_t`, `TxnID_t`, `PackageId_t`, `BundleId_t`, `AppId_t`, `AssetClassId_t`, `PhysicalItemId_t`, `DepotId_t`: Unique identifiers for various Steam entities.
- `RTime32`: 32-bit representation of real-world time in seconds since January 1, 1970.
- `SteamAPICall_t`: Handle to a Steam API call.

CONNECTS TO: Other Steamworks headers and libraries that utilize these types.

NOTES: None.

## `src/Lib/steam/steamuniverse.h`

PURPOSE: This file defines enumerations and constants related to different Steam universes.

KEY PARTS:
- EUniverse: Enumerates the various Steam universes, each representing a distinct instance of the Steam platform.
  - k_EUniverseInvalid: Invalid universe identifier.
  - k_EUniversePublic: The main public Steam universe.
  - k_EUniverseBeta: Beta testing environment for new features.
  - k_EUniverseInternal: Internal use only, typically for developers and beta testers.
  - k_EUniverseDev: Development environment for testing new code.
  - k_EUniverseMax: Maximum valid universe identifier.

CONNECTS TO: None. This file is a standalone header defining constants used throughout the Steam client and related applications.

NOTES: None.
