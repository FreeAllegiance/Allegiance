# src/Pigs/PigClient/PigClient

[Back to the index](../LLAMA_SUMMARY.md) - 5 files, 571 lines. Notes by a local model, to orient by: not verified facts.

This folder contains client-side code for managing pigs in Allegiance, a multiplayer space game. It includes classes for handling commands, managing game sessions, and encapsulating ship information.

- LaunchCommand.cs: Encapsulates actions that can be conditionally executed.
- PigInfo.cs: Holds details about a player's ship in the game.

## `src/Pigs/PigClient/PigClient/App.xaml.cs`

PURPOSE: tiny file (16 lines), shown in full rather than summarized.

    using System;
    using System.Collections.Generic;
    using System.Configuration;
    using System.Data;
    using System.Linq;
    using System.Threading.Tasks;
    using System.Windows;
    namespace PigClient
    {
        /// <summary>
        /// Interaction logic for App.xaml
        /// </summary>
        public partial class App : Application
        {
        }
    }

## `src/Pigs/PigClient/PigClient/LaunchCommand.cs`

PURPOSE: This file defines a command wrapper class that implements the `ICommand` interface from the .NET framework. It allows for encapsulating an action and a condition under which that action can be executed.

KEY PARTS:
- CommandWrapper: A class implementing the `ICommand` interface, which provides methods to execute an action and check if it can be executed.
  - `_execute`: An action to be performed when the command is executed.
  - `_canExecute`: A function returning a boolean indicating whether the command can be executed.

CONNECTS TO:
- PigsLib: The file likely depends on some library or namespace named `PigsLib`.

NOTES:
- None.

## `src/Pigs/PigClient/PigClient/MainWindow.xaml.cs`

PURPOSE: This file contains the implementation of the main window for a client application that interacts with a game session to manage pigs. It handles connecting to the game server, creating and managing pig entities, and logging events.

KEY PARTS:
- MainWindow: The main window class that implements INotifyPropertyChanged for data binding.
- Connect_Click: Establishes a connection to the game session.
- CreatePig_Click: Creates a new pig entity in the game session.
- Session_OnEvent: Handles events received from the game session.
- Pi_OnLogOff: Removes a pig from the list when it logs off.

CONNECTS TO:
- PigsLib.PigSession: The game session management library.
- AGCLib.IAGCEvent: Game event interface.

NOTES:
- Contains logic for maintaining a balance of friendly and hostile pigs based on ratios.
- Includes functionality to restore pigs if enabled.
- Logs various events and states to the UI.

## `src/Pigs/PigClient/PigClient/PigInfo.cs`

PURPOSE: This file defines a class `PigInfo` that encapsulates information about a player's ship in the Allegiance game. It provides properties to access various attributes of the ship and commands to interact with it.

KEY PARTS:
- PigInfo: Manages and exposes information about a player's ship.
- Try<T>: A generic method for safely executing expressions and returning default values if an exception occurs.
- Launch: An ICommand that initiates the launch of the ship.
- Logoff: An ICommand that logs off the player and cleans up resources.
- RemovePig: Removes the player from the game and invokes the logoff event.
- Refresh: Triggers property change notifications for specific attributes.

CONNECTS TO: PigsLib.IPig, CommandWrapper

NOTES: None.

## `src/Pigs/PigClient/PigClient/Setup.txt`

PURPOSE: tiny file (17 lines), shown in full rather than summarized.

    regsvr32 .\AGC\AGC.dll /u
    regsvr32 .\AGC\AGC.dll
    regsvr32 .\Test\TCObj\TCObj.dll /u
    regsvr32 .\Test\TCObj\TCObj.dll
    regsvr32 .\Pigs\PigsLib\PigsLib.dll /u
    regsvr32 .\Pigs\PigsLib\PigsLib.dll
    PigAccts.exe -RegServer [USER] [PASS]
    PigSrv.exe -RegServer [USER] [PASS]
    Create Scripts directory at /PigSrv
    Copy *.pig into Scripts
    // AppID = Pigs
    // {F132B4E3-C6EF-11D2-85C9-00C04F68DEB0}
    // HKCR/AppID/{F132B4E3-C6EF-11D2-85C9-00C04F68DEB0}/ArtPath
    // HKCR/AppID/{F132B4E3-C6EF-11D2-85C9-00C04F68DEB0}/MissionServer property
    // HKCR/AppID/{F132B4E3-C6EF-11D2-85C9-00C04F68DEB0}/Account
    // HKCR/AppID/{F132B4E3-C6EF-11D2-85C9-00C04F68DEB0}/ZoneAuthServer
    // HKCR/AppID/{F132B4E3-C6EF-11D2-85C9-00C04F68DEB0}/MaxPigs
