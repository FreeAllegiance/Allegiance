# src/Pigs/Scripts

[Back to the index](../LLAMA_SUMMARY.md) - 37 files, 3,117 lines. Notes by a local model, to orient by: not verified facts.

This folder contains JavaScript and VBScript scripts used in Allegiance for managing various aspects of the game server and client interactions. The scripts handle tasks such as account management, pig creation, deployment, logging, and more.

- DeployPigs.js: Orchestrates the deployment of ActivePigs on multiple servers.
- ForEach.js: Executes a command line on multiple remote servers listed in a semicolon-separated string.
- AllSrvVerInfo.js: Retrieves and displays version information for specified files using Allegiance's administrative session object.
- RemListAccts.js: Lists available and in-use accounts from the Pig Account Dispenser.
- ListAccts.js: Provides functionality to list accounts from a specified server.

Largest files: DeployPigs.js (579 lines), ForEach.js (186 lines), AllSrvVerInfo.js (132 lines), RemListAccts.js (129 lines), ListAccts.js (123 lines).

## `src/Pigs/Scripts/AccountServer.js`

PURPOSE: This JavaScript file is an entry point for a script that manages the AccountServer property of the Pig Server. It allows users to either display the current AccountServer value or change it to a new value.

KEY PARTS:
- main(args, cArgs): The primary function that parses command line arguments and interacts with the Pig Server's AccountServer property.
- Syntax(): Displays the usage syntax for the script.
- objSession: An ActiveXObject used to connect to the Pig Server and access its properties.

CONNECTS TO: Pigs (ActiveXObject)

NOTES: None.

## `src/Pigs/Scripts/AllSrvVerInfo.js`

PURPOSE: This script retrieves and displays version information for a specified file using the Allegiance game's administrative session object.

KEY PARTS:
- `TCUtil`: ActiveX object used to create other objects.
- `AllSrv.AdminSession`: Object representing an administrative session in Allegiance.
- `objVerInfo`: Object containing version information of a file.
- `WScript.Echo`: Function to output text to the console.

CONNECTS TO: Allegiance game's administrative system and file versioning data.

NOTES: None.

## `src/Pigs/Scripts/ArtPath.js`

PURPOSE: This JavaScript file is a script used to manage the ArtPath property of the Pig Server. It allows users to either display the current ArtPath value or set it to a new value.

KEY PARTS:
- main(args, cArgs): The entry point function that parses command line arguments and calls other functions based on the input.
- Syntax(): Displays the usage syntax for the script.
- objSession: An ActiveXObject used to interact with the Pig Server.

CONNECTS TO: Pigs (ActiveXObject)

NOTES: None

## `src/Pigs/Scripts/CreatePig.js`

PURPOSE: This JavaScript file is an entry point script for creating a pig in the Allegiance game using the Pigs API. It handles command-line arguments to specify the behavior of the pig and connects to the Pig Server to create the pig.

KEY PARTS:
- main(args, cArgs): Main function that parses command line arguments, validates usage, and creates a pig.
- Syntax(): Function that displays the correct usage syntax for the script.
- objSession: Global variable representing the connection to the Pig Server.

CONNECTS TO: Pigs.Session (the Pig Server API).

NOTES: None.

## `src/Pigs/Scripts/CreatePigs.js`

PURPOSE:
The file `CreatePigs.js` is a script used to create pigs in the Allegiance game. It connects to the Pig Server and allows specifying the behavior type and count of pigs to be created.

KEY PARTS:
- main(args, cArgs): The entry point function that parses command line arguments, validates usage, and creates pigs.
- Syntax(): Displays the usage syntax for the script.
- objSession: An ActiveXObject representing the connection to the Pig Server.

CONNECTS TO:
Pig Server (via `ActiveXObject("Pigs.Session")`)

NOTES:
- The commented-out code suggests that there was an attempt to use a `SessionInfo` object, which is not currently in use.
- There are no TODOs or hacks noted in the provided code.

## `src/Pigs/Scripts/DeployPigs.js`

PURPOSE: This JavaScript file is a script for deploying ActivePigs on multiple servers. It handles command-line arguments, validates input, and orchestrates the deployment process.

KEY PARTS:
- `main`: Entry point that parses command line arguments and calls other functions.
- `DoMessage` and `DoLog`: Functions to handle output messages and logs.
- `Init`: Initializes global variables and checks command line arguments for validity.
- `SetupPigs`: Orchestrates the deployment process on specified servers.
- `Run`: Executes a command on a remote server.
- `GetCommandList`: Parses command-line arguments into a dictionary.
- `StringToCollection`: Converts a string of server names into a collection, handling ranges and lists.
- `ErrorText` and `SyntaxText`: Provide error messages and usage syntax.

CONNECTS TO: TCDeployment ActiveX object for remote deployment tasks.

NOTES: None.

## `src/Pigs/Scripts/EventLog.js`

PURPOSE:
The file `EventLog.js` is a script used to manage and display the Event Log parameters for the Allegiance game server. It allows administrators to either retrieve the current settings or update them with new database connection details.

KEY PARTS:
- `main(args, cArgs)`: The main function that handles command line arguments and performs the necessary operations.
- `Syntax()`: Displays the usage syntax of the script.
- `objSession`: An ActiveXObject used to connect to the Pig Server.
- `objOld` and `objNew`: ActiveXObjects representing the old and new EventLog parameters, respectively.

CONNECTS TO:
- The Allegiance game server through an ActiveXObject named "Pigs".

NOTES:
- None.

## `src/Pigs/Scripts/EventLogDB.js`

PURPOSE: This JavaScript file is used to manage and display the database parameters for event logging in Allegiance, a Microsoft multiplayer space game. It allows setting new parameters or retrieving the current ones.

KEY PARTS:
- main(args, cArgs): The entry point function that handles command-line arguments and performs the necessary operations.
- Syntax(): Displays the usage syntax of the script.
- objSession: An ActiveXObject representing an administrative session for Allegiance.
- objEventLog: An object representing the event log settings in Allegiance.
- objOld: Holds the current database parameters for event logging.
- objNew: Used to create new database parameters when setting new values.

CONNECTS TO: AllSrv.AdminSession, AGC.DBParams

NOTES: None.

## `src/Pigs/Scripts/EventViewerX.xsl`

PURPOSE: This file is an XSLT stylesheet used to transform XML event data into an HTML table format for display in the Event Viewer of Allegiance.

KEY PARTS:
- Name: `EventViewerX.xsl`
- What it does: Transforms XML event data into an HTML table with columns for Date/Time, ID, Subject, and Description.
- SortHeader class: Styles the header cells to allow sorting by clicking on them.
- OnSortByDateTime, OnSortByEvent, OnSortBySubjectName, OnSortByDescription functions: JavaScript functions that handle sorting based on the clicked column.

CONNECTS TO: The Event Viewer in Allegiance.

NOTES: None.

## `src/Pigs/Scripts/ForEach.js`

PURPOSE: The file `ForEach.js` is a script designed to run a specified command line on multiple remote servers listed in a semicolon-separated string.

KEY PARTS:
- `main(args, cArgs)`: Entry point function that parses command-line arguments and processes them.
- `StringToCollection(strIn)`: Parses a string into a dictionary of server names, handling range specifications like "10-20".
- `ErrorText(e)`: Formats error objects into readable messages.
- `Syntax()`: Displays usage syntax for the script.

CONNECTS TO: Depends on ActiveX objects such as `TCUtil` and `TCDeployment`.

NOTES: None.

## `src/Pigs/Scripts/ListAccts.js`

PURPOSE: This script lists available and in-use accounts from the Pig Account Dispenser.

KEY PARTS:
- main(args, cArgs): Parses command-line arguments and calls other functions to list accounts.
- FormatLine(strName, bAvailable): Formats a line of text showing an account name and its availability status.
- FindString(objStrings, strString): Checks if a string exists in a collection of strings.
- Syntax(): Displays the usage syntax for the script.

CONNECTS TO: Pigs.AccountDispenser (ActiveX object).

NOTES: None.

## `src/Pigs/Scripts/LobbyMode.js`

PURPOSE: This file is a script written in JavaScript for the Allegiance game that allows users to display or change the current lobby mode of the game. It provides a command-line interface to interact with the game's settings.

KEY PARTS:
- main(args, cArgs): The entry point function that parses command line arguments and calls other functions based on the input.
- LobbyModeName(eMode): Converts an enum value representing a lobby mode into its corresponding string name.
- LobbyModeFromName(strMode): Converts a string representing a lobby mode into its corresponding enum value.
- Syntax(): Displays the usage syntax of the script.

CONNECTS TO: Pigs (ActiveXObject), Allegiance game settings

NOTES: None

## `src/Pigs/Scripts/LobbyServer.js`

PURPOSE: This file contains a script for managing the MissionServer property of the Pigs LobbyServer in Allegiance. It allows users to either display the current server name or change it to a new one.

KEY PARTS:
- main(args, cArgs): The entry point function that parses command line arguments and calls the appropriate action.
- Syntax(): Displays the usage syntax for the script.
- objSession: An ActiveXObject used to interact with the Pigs LobbyServer.
- MissionServer property: The property of objSession that holds the server name.

CONNECTS TO: Pigs ActiveX object

NOTES: None

## `src/Pigs/Scripts/LogAllEvents.js`

PURPOSE: This script enables or disables logging of all events generated by the Allegiance server to a database log.

KEY PARTS:
- main(args, cArgs): Parses command line arguments and manages enabling/disabling event logging.
- Syntax(): Displays usage instructions for the script.
- objSession: Connects to the AllSrv object model.
- objEventLog: Accesses the EventLog property of the AllSrv session.
- objRanges: Manages ranges of event IDs to log.

CONNECTS TO: Allegiance server via ActiveX objects.

NOTES: None.

## `src/Pigs/Scripts/MaxPigs.js`

PURPOSE: This file contains a script to manage and display the maximum number of pigs in Allegiance, a space game. It allows users to either retrieve the current setting or update it via command-line arguments.

KEY PARTS:
- main(args, cArgs): The entry point function that parses command line arguments and interacts with the Pig Server.
- Syntax(): Displays the usage syntax for the script.
- objSession: An ActiveXObject used to connect to the Pig Server and interact with its properties.

CONNECTS TO: Pigs (ActiveXObject)

NOTES: None.

## `src/Pigs/Scripts/RemAccountServer.js`

PURPOSE: This JavaScript file is a script used to manage the AccountServer property of a Pig server. It allows setting or displaying the current AccountServer value.

KEY PARTS:
- main(args, cArgs): The entry point function that parses command line arguments and performs the necessary actions.
- Syntax(): Displays the usage syntax for the script.
- objUtil: An ActiveXObject used to create a session with the Pig server.
- strServer: The name of the computer hosting the Pig server.
- objSession: A session object created using the Pig server's name.
- AccountServer: The property that holds the current AccountServer value.

CONNECTS TO: TCUtil, Pigs

NOTES: None.

## `src/Pigs/Scripts/RemCreatePig.js`

PURPOSE: This JavaScript file is a script used to create a pig in the Allegiance game by connecting to a Pig Server and specifying the behavior type.

KEY PARTS:
- main(args, cArgs): The entry point function that parses command line arguments, validates usage, creates utility and session objects, connects to the Pig Server, and creates a pig with specified behavior.
- Syntax(): Displays the correct usage syntax for the script.

CONNECTS TO: Pigs.SessionInfo, Pigs.Session, TCUtil

NOTES: None.

## `src/Pigs/Scripts/RemCreatePigs.js`

PURPOSE: This JavaScript file is a script used to create multiple pigs in a game server. It takes command-line arguments for the server name, count of pigs, and an optional behavior type.

KEY PARTS:
- main(args, cArgs): The entry point function that parses command-line arguments, validates usage, connects to the Pig Server, and creates the specified number of pigs.
- Syntax(): Displays the correct usage syntax for the script.
- objSession: A global variable representing the session information object used to connect to the Pig Server.

CONNECTS TO: Pigs.SessionInfo, TCUtil, Pigs.Session

NOTES: None.

## `src/Pigs/Scripts/RemListAccts.js`

PURPOSE: The file `RemListAccts.js` is a script designed to list accounts from a specified server in Allegiance, either all accounts or only those that are available or in use.

KEY PARTS:
- `main(args, cArgs)`: Main function that parses command line arguments and calls other functions to process the account listing.
- `FormatLine(strName, bAvailable)`: Formats a line of output showing an account name and its availability status.
- `FindString(objStrings, strString)`: Searches for a specific string within a collection of strings.
- `Syntax()`: Displays the usage syntax for the script.

CONNECTS TO: The script depends on the `TCUtil` ActiveX object to interact with the Pig Account Dispenser.

NOTES: None.

## `src/Pigs/Scripts/RemListPigs.js`

PURPOSE: The file `RemListPigs.js` is a script designed to list the pigs currently running on a specified computer in Allegiance.

KEY PARTS:
- main(args): Entry point of the script that parses command line arguments and lists the pigs.
- Syntax(): Displays the usage syntax for the script.
- objSession: Represents the session information object connected to the Pig Server.
- objUtil: ActiveX object used for utility functions, including creating objects.

CONNECTS TO:
- Allegiance game environment through the `Pigs` ActiveX object.

NOTES:
- The commented-out code suggests that additional functionality related to session information and application name was intended but not implemented.

## `src/Pigs/Scripts/RemLobbyServer.js`

PURPOSE: The file `RemLobbyServer.js` is a script used to manage the MissionServer property of a Pig server. It allows users to either display the current MissionServer or change it to a new value.

KEY PARTS:
- `main(args, cArgs)`: Main function that parses command line arguments and performs the necessary actions.
- `Syntax()`: Function that displays the usage syntax for the script.
- `objUtil`: ActiveXObject used for utility functions.
- `strServer`: The name of the Pig server to connect to.
- `objSession`: Object representing the session with the Pig server.
- `MissionServer`: Property of `objSession` that holds the MissionServer value.

CONNECTS TO: `TCUtil`, `Pigs`

NOTES: None

## `src/Pigs/Scripts/RemZoneAuthServer.js`

PURPOSE: This script is used to display or change the current ZoneAuthServer property of a Pigs server.

KEY PARTS:
- main(args, cArgs): Entry point and main function that parses command line arguments and interacts with the Pigs server.
- Syntax(): Displays the usage syntax for the script.
- objUtil: Utility object created using ActiveXObject to interact with the Pigs server.
- strServer: The name of the computer where the Pigs server is located.
- objSession: Object representing the session with the Pigs server.
- ZoneAuthServer property: Gets or sets the current ZoneAuthServer value.

CONNECTS TO: Pigs server, ActiveXObject for COM interaction.

NOTES: None.

## `src/Pigs/Scripts/RemZoneAuthTimeout.js`

PURPOSE: This JavaScript file is a script used to display or change the ZoneAuthTimeout property of the Pigs server. It allows administrators to manage the authentication timeout settings for players in the game.

KEY PARTS:
- main(args, cArgs): The entry point function that parses command line arguments and performs the main operations.
- Syntax(): Displays the usage syntax of the script.
- objUtil: An ActiveXObject used to interact with the Pig Server.
- strServer: The name of the computer hosting the Pigs server.
- objSession: A session object created to manage interactions with the Pigs server.
- ZoneAuthTimeout: The property that controls the authentication timeout for players.

CONNECTS TO: Pigs server, TCUtil ActiveXObject

NOTES: None

## `src/Pigs/Scripts/ScriptDir.js`

PURPOSE: This file is a script written in JavaScript for the Allegiance game that allows users to display or change the current ScriptDir property of the Pig Server.

KEY PARTS:
- main(args, cArgs): Entry point function that parses command line arguments and either sets or gets the ScriptDir property.
- Syntax(): Function that displays the usage syntax of the script.
- objSession: ActiveXObject used to interact with the Pig Server.
- strOld: Stores the current value of the ScriptDir property.

CONNECTS TO: Pigs ActiveXObject (Pig Server).

NOTES: None.

## `src/Pigs/Scripts/ZoneAuthServer.js`

PURPOSE: This JavaScript file is an entry point for a script that manages the ZoneAuthServer property of the Pigs application. It allows users to either display the current value of the ZoneAuthServer or change it to a new value.

KEY PARTS:
- main(args, cArgs): The main function that parses command line arguments and performs the necessary actions.
- Syntax(): Displays the usage syntax for the script.
- objSession: An ActiveXObject used to interact with the Pigs application.

CONNECTS TO: Pigs (ActiveXObject)

NOTES: None

## `src/Pigs/Scripts/ZoneAuthTimeout.js`

PURPOSE: This JavaScript file is used to manage and display the ZoneAuthTimeout property of the Pig Server. It allows setting a new timeout value or retrieving the current timeout value.

KEY PARTS:
- main(args, cArgs): The entry point function that parses command line arguments and either sets or gets the ZoneAuthTimeout property.
- Syntax(): Displays the usage syntax for the script.
- objSession: An ActiveXObject used to interact with the Pig Server.

CONNECTS TO: Pigs (ActiveXObject)

NOTES: None.

## `src/Pigs/Scripts/create collision test game.vbs`

PURPOSE: This VBScript file is designed to create a game session in Allegiance with specific parameters for collision testing.

KEY PARTS:
- objCreateParams: Object used to set various game parameters such as lives, player limits, and map type.
- AGC.GameParameters: Class that holds the parameters for creating a new game session.
- AllSrv.AdminSession: Class representing an administrative session to manage game sessions on the server.

CONNECTS TO:
- Allegiance server (AllSrv namespace)

NOTES:
- The script is commented out except for setting up one specific game named "Collision Test".
- The script sets parameters like map type, player limits, and team count.
- There are placeholders for creating multiple games, which are currently disabled.

## `src/Pigs/Scripts/create demo game.vbs`

PURPOSE: This VBScript file is used to create a demo game in Allegiance by setting various parameters and adding the game to the server.

KEY PARTS:
- objCreateParams: An object that holds the parameters for creating a game.
- AGC.GameParameters: The class used to define game parameters.
- AllSrv.AdminSession: The class used to manage server sessions.
- Games: A collection of games managed by the server session.

CONNECTS TO: Allegiance server and game management systems.

NOTES: 
- The script is commented out for creating multiple games, only one game is created with specific parameters.
- The default values for CivIDs are set to 2, which might be a placeholder or a temporary setting.

## `src/Pigs/Scripts/create scout test game.vbs`

PURPOSE:
This VBScript file is designed to create a test game in Allegiance using the AllSrv.AdminSession object. It sets various parameters for the game such as player limits, lives, and map type.

KEY PARTS:
- objCreateParams: Object used to store game parameters.
- AGC.GameParameters: Class that holds the parameters for creating a game.
- AllSrv.AdminSession: Class that provides methods to manage server sessions and games.
- Games: Collection of games managed by the AdminSession object.

CONNECTS TO:
- Allegiance server (AllSrv namespace)

NOTES:
- The script is commented out except for setting up the parameters for a single game named "Scout Test Game".
- The script sets default values for Civilizations IDs, which are currently set to 2.
- There is commented-out code that would loop to create multiple games.

## `src/Pigs/Scripts/debugger.js`

PURPOSE: tiny file (16 lines), shown in full rather than summarized.

    // TODO: Create the Session Information object
    //var info = new ActiveXObject("TCObj.SessionInfo");
    //info.ApplicationName = WScript.ScriptFullName;
    // Connect to the Pig Server
    var app = new ActiveXObject("Pigs.Session");
    // TODO: app.SessionInfo = objInfo;
    // Get the collection of pigs
    var pigs = app.Pigs;
    // Get an iterator on the collection of pigs
    var itPigs = new Enumerator(pigs);
    // Get the first pig in the iterator
    var pig = itPigs.item();
    // Get the first pig's ship
    var ship = pig ? pig.Ship : null;
    // Jump straight into the debugger
    debugger;

## `src/Pigs/Scripts/debugger.vbs`

PURPOSE: tiny file (16 lines), shown in full rather than summarized.

    ' Connect to the Pig Server
    Set app = CreateObject("Pigs.Session")
    ' Get the collection of pigs
    Set pigs = app.Pigs
    ' Get the first pig in the collection
    Dim pig, ship
    If pigs.Count Then
        Set pig = pigs(0)
        Set ship = pig.ship
    Else
        Set pig = Nothing
        Set ship = Nothing
    End If
    Set pig = itPigs.item()
    ' Jump straight into the debugger
    Stop

## `src/Pigs/Scripts/immed.js`

PURPOSE: tiny file (2 lines), shown in full rather than summarized.

    // Jump straight into the debugger
    debugger;

## `src/Pigs/Scripts/listpigs.js`

PURPOSE: The file `listpigs.js` is a script used to list the pigs (likely referring to game entities) currently running in the Allegiance game server.

KEY PARTS:
- `main(args)`: Entry point function that parses command line arguments and validates usage.
- `Syntax()`: Displays the correct usage syntax for the script.
- `objSession`: An ActiveXObject representing the session information connected to the Pig Server.

CONNECTS TO: The Pig Server, which is an ActiveX object used to interact with game entities.

NOTES: None.

## `src/Pigs/Scripts/listpigs2.js`

PURPOSE: This script is used to list information about pigs currently running in the Allegiance game server.

KEY PARTS:
- main(args): Entry point and main function that parses command line arguments, connects to the Pig Server, retrieves pig information, and lists it.
- Syntax(): Displays the usage syntax of the script.
- objSession: ActiveXObject used to connect to the Pig Server and retrieve session information.

CONNECTS TO: Allegiance game server through the Pigs.Session ActiveX object.

NOTES: None.

## `src/Pigs/Scripts/remshutdown.js`

PURPOSE: This script is used to remotely shut down pigs (player-controlled spaceships) on a specified server in the Allegiance game.

KEY PARTS:
- main(args): Entry point that parses command line arguments and calls the appropriate shutdown function.
- ShutdownPig(strName): Shuts down a specific pig by name if it is running.
- ShutdownAll(): Shuts down all pigs currently running on the server.
- Syntax(): Displays usage instructions for the script.

CONNECTS TO: Allegiance game server (remotely controls pigs).

NOTES:
- The commented-out code suggests that there was an attempt to use a SessionInfo object, but it was not fully implemented.

## `src/Pigs/Scripts/session.js`

PURPOSE: tiny file (3 lines), shown in full rather than summarized.

    // Connect to the Pig Server
    var app = new ActiveXObject("Pigs.Session");
    debugger;

## `src/Pigs/Scripts/shutdown.js`

PURPOSE: This file contains a script to shut down pigs in Allegiance, either all running pigs or a specific pig by name.

KEY PARTS:
- main(args): Entry point that parses command line arguments and calls the appropriate shutdown function.
- ShutdownPig(strName): Shuts down a single pig by name if it is running.
- ShutdownAll(): Shuts down all currently running pigs.
- Syntax(): Displays the usage syntax of the script.

CONNECTS TO: Pigs.Session, Pigs.Pigs (ActiveX objects)

NOTES: None.
