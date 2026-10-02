# src/Pigs/Piglets/include

[Back to the index](../LLAMA_SUMMARY.md) - 11 files, 1,719 lines. Notes by a local model, to orient by: not verified facts.

This folder contains JavaScript and VBScript files related to game automation and management in Allegiance. It includes scripts for starting games automatically, handling chat commands, managing player teams, and implementing AI behavior.

- AutoStartCustomGame.js: Automates the start of a custom game by checking team player counts.
- AutoStartGame.js: Automatically starts a game when the minimum number of players per team join.
- IDL.js: Bridges COM Enums into JavaScript for use in Allegiance development.
- imago.js: Manages AI behavior, mission joining, and ship launching for Piglets in Allegiance.

Largest files: imago.js (342 lines), IDL.js (336 lines), common.js (318 lines), dijkstra.js (206 lines), asteroids.js (196 lines).

## `src/Pigs/Piglets/include/AutoStartCustomGame.js`

PURPOSE: This JavaScript file automates the start of a custom game in Allegiance by checking if all teams have enough players and then initiating the game.

KEY PARTS:
- AutoStartGame(objParams): Initializes the auto-start process with parameters.
- AutoStartGame_Tick(): Checks if each team has the minimum number of players required to start the game.
- AutoStartGameDelay(): Delays the actual game start by a few seconds to ensure all teams are ready.
- StartCustomGame(MissionParams): Starts the custom game with the specified mission parameters.

CONNECTS TO: Game, Teams, Properties, Timer, GameController

NOTES: None.

## `src/Pigs/Piglets/include/AutoStartGame.js`

PURPOSE: This file contains JavaScript code to automatically start a game in Allegiance when the minimum number of players per team have joined.

KEY PARTS:
- AutoStartGame(cMinTeamPlayers): Initializes a timer to check for the presence of the minimum number of players in each team.
- AutoStartGame_Tick(cMinTeamPlayers): Called periodically to check if the minimum number of players is met on all teams. If so, it starts the game and kills the timer.

CONNECTS TO: Game.Teams, Timer, PigState

NOTES: None

## `src/Pigs/Piglets/include/GetIntoTeam.js`

PURPOSE: Handles state transitions and manages interactions with the mission server for piglets in Allegiance.

KEY PARTS:
- OnStateNonExistant(eStatePrevious): Logs on to the mission server if the previous state was PigState_Terminated.
- OnStateMissionList(eStatePrevious): Joins or creates a mission based on available missions.
- OnStateTeamList(eStatePrevious): Joins a random team.

CONNECTS TO: Mission server, game states (PigState_Terminated, PigState_JoiningTeam).

NOTES: None.

## `src/Pigs/Piglets/include/IDL.js`

PURPOSE: This file provides JavaScript-compatible enumerations and type pass-through functions to bridge COM Enums defined in IDL into a format usable for JavaScript/TypeScript development within the Allegiance game.

KEY PARTS:
- AGCObjectType: Enumerates different types of objects in the game.
- AGCHullAbility: Enumerates abilities associated with ship hulls.
- AGCAsteroidAbility: Enumerates abilities specific to asteroids.
- AGCAxis: Enumerates axes for movement and control.
- AGCEquipmentType: Enumerates types of equipment that can be equipped on ships.
- AGCTreasureType: Enumerates types of treasures found in the game.
- AGCChatTarget: Enumerates different chat targets within the game.
- AGCGameStage: Enumerates stages of a game session.

CONNECTS TO: Allegiance's IDL files and JavaScript/TypeScript development environment.

NOTES: None.

## `src/Pigs/Piglets/include/asteroids.js`

PURPOSE:
The file `asteroids.js` contains JavaScript code for tracking and managing asteroids in the game Allegiance. It categorizes asteroids based on their type (Uranium, Silicon, Carbon, Helium3) and updates lists of these asteroids.

KEY PARTS:
- `InitAsteroidLists`: Initializes four dictionaries to store different types of asteroids.
- `CheckOreAsteroid`: Checks if an asteroid is a special ore type and adds it to the appropriate list, reporting status messages.
- `SectorAsteroidCount`: Returns a report of the number and locations of asteroids of a specific type in all sectors.
- `UpdateAsteroidLists`: Updates all four asteroid lists with current game data.
- `SectorAsteroidListUpdate`: Updates a specific asteroid list based on the game state.
- `ScanAsteroidsForOres`: Iterates through asteroids in a specified sector and calls `CheckOreAsteroid` for each one.

CONNECTS TO:
Depends on the game's global objects such as `Game`, `Ship`, and `Sector`.

NOTES:
- Uses `ActiveXObject("Scripting.Dictionary")` which is specific to Internet Explorer.
- The code assumes certain properties (e.g., `Name`, `Sector.Name`) are available on asteroid objects.

## `src/Pigs/Piglets/include/autostartgame.vbs`

PURPOSE: This file contains VBScript code to automatically start a game in Allegiance once the minimum number of players per team have joined.

KEY PARTS:
- AutoStartGame(cMinTeamPlayers): Initializes a timer to check if the required number of players are present.
- AutoStartGame_Tick(cMinTeamPlayers): Checks each team's ship count and starts the game if the minimum is met.

CONNECTS TO: Game object model, Timer functions

NOTES: None

## `src/Pigs/Piglets/include/chatcommand.js`

PURPOSE: This JavaScript file handles chat commands and manages status messages in the Allegiance game.

KEY PARTS:
- `OnReceiveChat`: Searches for and processes chat commands using a predefined list (`ChatCommandList`).
- `AddShipToStatusList`: Adds a ship to the status message recipient list.
- `RemoveShipFromStatusList`: Removes a ship from the status message recipient list.
- `StatusMessage`: Sends a chat message to wing 9 and all recipients that have requested status messages.

CONNECTS TO: Depends on the game's scripting system, particularly the `ChatCommandList` and `Ship` objects.

NOTES: None.

## `src/Pigs/Piglets/include/common.js`

PURPOSE: This JavaScript file contains utility functions and constants used by the Allegiance game's piglets, which are AI-controlled ships.

KEY PARTS:
- `c_habmLifepod`, `c_habmCaptureThreat`, etc.: Constants representing different states or conditions.
- `DisplayStateTransition(eStatePrevious)`: Logs a message when the state of an object changes.
- `CanCalculatePosition(targetPosition)`: Checks if a position can be calculated.
- `ShouldBeTargeted(potentialTarget)`: Determines if a ship should be targeted based on its type.
- `NeedPickup()`: Sends a chat message to nearby friends if the piglet needs rescue.
- `KillTimers()`, `KillAllTimers()`, `KillTimer(timerName)`: Functions to stop timers.
- `Delay(action)`: Delays an action by creating a new timer.
- `dump(arr, level)`: Recursively dumps the contents of an object or array for debugging.
- `plural(nCount, strNonPlural, strPlural)`: Returns the appropriate plural form based on the count.
- `SelectBestHull(hullCollection, strFirstChoice, strSecondChoice)`: Selects the best hull from a collection based on name patterns.
- `Range2Ship(agcShip)`: Calculates the distance between the current ship and another ship.
- `FindNearestEnemy(shipCollection)`, `FindNearestFriend(shipCollection)`: Finds the nearest enemy or friend in a collection.
- `IsTargetValid(shipCollection, objTarget)`: Checks if a target is valid for the piglet to attack.
- `DoRipCord()`, `EndRipCord()`: Functions related to the ripcord action.
- `QUICKCHATBYCODE`, `TAUNTS`, `YELL`: Quick chat messages and taunts.

CONNECTS TO: Allegiance game engine, ship objects, and other AI-related scripts.

NOTES: None.

## `src/Pigs/Piglets/include/dijkstra.js`

PURPOSE: This file implements Dijkstra's algorithm to find the shortest path between sectors in a game map.

KEY PARTS:
- InitPathGenerator(): Initializes data structures for the pathfinding algorithm.
- MinimumItem(): Finds the sector with the minimum estimated distance from the source.
- DumpDictionary(): Debug function to print dictionary contents.
- SectorWeight(): Calculates a weight for each sector based on enemy presence.
- EnemiesPresent(): Checks if an enemy station is present in a sector.
- Relax(): Updates the estimate and predecessor of a sector.
- NextAlephInShortestPath(): Finds the next aleph (sector) along the shortest path from one sector to another.
- FindShortestPath(): Main function that runs Dijkstra's algorithm to calculate the shortest path.
- ReportPath(): Debug function to print the calculated path.
- NextAleph(): Returns the next sector in the precomputed path.

CONNECTS TO: Game.Sectors, Ship.Team

NOTES:
- Uses VBArray for dictionary keys due to limitations of ActiveXObject.
- SectorWeight() was intended to influence path selection but didn't work as expected.
- Contains debug functions and trace statements for development purposes.

## `src/Pigs/Piglets/include/eliza.js`

PURPOSE: tiny file (1 lines), shown in full rather than summarized.

    // We don't have a chat bot :(

## `src/Pigs/Piglets/include/imago.js`

PURPOSE: This file contains JavaScript code for a Piglet in the Allegiance game. It handles various states and events related to joining missions, creating games, managing teams, and launching ships.

KEY PARTS:
- OnStateNonExistant(eStatePrevious): Handles initial state transitions.
- OnStateMissionList(eStatePrevious): Manages mission creation or joining.
- OnStateTeamList(eStatePrevious): Joins a team if not already in one.
- OnStateWaitingForMission(eStatePrevious): Waits for the game to start.
- OnStateDocked(eStatePrevious): Handles docking and launching ships.
- buyShipSetSkillsLaunch(safe): Buys and launches a ship with random skills.
- OnMissionStarted(): Called when a mission starts.

CONNECTS TO: Allegiance game engine, JavaScript API provided by the game.

NOTES:
- Contains commented-out code for auto-starting games and setting up timers.
- Includes functions to handle various states but some are not implemented yet (e.g., OnStateCreatingMission).
- Uses global variables to track state and game progress.
