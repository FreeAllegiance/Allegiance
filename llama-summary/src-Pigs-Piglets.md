# src/Pigs/Piglets

[Back to the index](../LLAMA_SUMMARY.md) - 35 files, 7,705 lines. Notes by a local model, to orient by: not verified facts.

This folder contains various behavior scripts and configuration files for Allegiance's Piglets AI system. These scripts define how different piglet entities behave in the game, including their state transitions, actions, and interactions with the environment.

- scout2.pig: Manages the behavior of a scout piglet, handling mission joining, team management, launching, flying, damage responses, sector changes, enemy detection, and resource scanning.
- marksn1.pig: Defines the AI logic for a piglet in a specific match configuration, including mission joining, state transitions, and behaviors like scanning sectors, finding targets, and engaging enemies.
- framerate.pig: Tests frame rate performance by defining behaviors and actions for a piglet during various states.
- bulldozer.pig: Controls the behavior of a Bulldozer piglet, managing its actions based on its state and environment.
- mark3.pig: Defines the AI logic for a piglet during various states such as logging on to the mission server, joining missions, selecting hulls, launching into combat, and engaging enemies.

Largest files: scout2.pig (749 lines), marksn1.pig (610 lines), framerate.pig (533 lines), mark3.pig (402 lines), bulldozer.pig (334 lines).

## `src/Pigs/Piglets/Creator.pig`

PURPOSE: Handles the creation and management of a piglet's state transitions in Allegiance, specifically focusing on logging onto the mission server and creating/joining missions.

KEY PARTS:
- OnStateNonExistant(eStatePrevious): Manages the initial state where the pig is being created by attempting to logon to the mission server.
- OnStateMissionList(eStatePrevious): Handles the transition to a state where the pig joins or creates a mission, setting up mission parameters and creating a mission.

CONNECTS TO: Allegiance's mission server and game creation systems.

NOTES: None.

## `src/Pigs/Piglets/DPSeq.pig`

PURPOSE: This file contains a JScript behavior script for the Allegiance game that handles chat sequences between ships.

KEY PARTS:
- TestChats: Initializes chat sequence with a target ship and number of iterations.
- SendChats: Sends a chat message to either the target ship or its team, alternating each time.
- FormatSeq: Formats the current chat sequence number as a 10-digit string.

CONNECTS TO: common.js, AutoStartGame.js, GetIntoTeam.js

NOTES: None.

## `src/Pigs/Piglets/Default.pig`

PURPOSE: This file contains a JScript behavior script for the Allegiance game's piglet AI. It handles various state transitions and actions such as logging on to the mission server, joining missions, selecting ship hulls, and controlling ship movement.

KEY PARTS:
- OnStateNonExistant: Handles initial logon to the mission server.
- OnStateMissionList: Joins or creates a mission.
- OnStateTeamList: Joins a random team.
- SelectBestHull: Chooses the best ship hull based on type.
- OnStateDocked: Buys and selects the best hull when docked.
- OnStateFlying: Controls ship throttle and thrust.
- OnInitialThrottle: Sets initial throttle and starts thrusting.
- OnEndThrusting: Stops thrusting after a random duration.

CONNECTS TO: AutoStartGame.js, Pig.Logon(), Pig.JoinMission(), Pig.CreateMission(), Pig.Ship.BuyHull(), Host.Trace(), Timer.Kill(), Ship.Throttle, Ship.Thrust(), Ship.AllStop()

NOTES: None.

## `src/Pigs/Piglets/DistanceLatency.pig`

PURPOSE:
This file contains a script for managing the behavior of a piglet in the Allegiance game. It handles various state transitions and actions such as logging on to the mission server, joining missions, selecting hull types, and launching the ship.

KEY PARTS:
- OnStateNonExistant: Handles initial logon to the mission server.
- OnStateMissionList: Joins or creates a mission.
- OnStateTeamList: Joins a random team.
- SelectBestHull: Chooses the best hull type for the piglet.
- OnStateDocked: Buys and selects the best hull when docked, then schedules the launch.
- OnStateFlying: Sets the ship's throttle to 100% and turns on the thruster.

CONNECTS TO:
- Mission server
- Game state management

NOTES:
None.

## `src/Pigs/Piglets/FaceOutpost.pig`

PURPOSE: This file contains a JScript behavior script for the FaceOutpost piglet in Allegiance. It defines how the piglet should face an outpost when it stops thrusting.

KEY PARTS:
- OnEndThrusting(): Function that is called when the piglet stops thrusting.
  - Calls BaseBehavior.OnEndThrusting() to perform default processing.
  - Faces the outpost using Ship.Face("Outpost", "AllStop(); Beep(5000, 50);").
  - Catches and logs any exceptions that occur during the face operation.

CONNECTS TO: Ship class for facing operations.

NOTES: None.

## `src/Pigs/Piglets/JoinGame.pig`

PURPOSE: Handles the initial state transitions and actions for a piglet joining a game in Allegiance.

KEY PARTS:
- OnStateNonExistant(eStatePrevious): Logs on to the mission server when the pig is created.
- OnStateMissionList(eStatePrevious): Joins or creates a mission if possible, otherwise shuts down the pig.

CONNECTS TO: Mission server and game state management systems.

NOTES: None.

## `src/Pigs/Piglets/LogonOnly.pig`

PURPOSE: This file contains a JScript behavior script for a piglet in the Allegiance game that handles the logon process to the mission server when the piglet is created.

KEY PARTS:
- OnStateNonExistant(eStatePrevious): Handles state transition logic, specifically logging on to the mission server when the piglet is being created.
- UpdatesPerSecond: Sets the update frequency to 20 updates per second.
- Logon(): Attempts to logon to the mission server.
- Shutdown(): Shuts down the pig if the logon fails.

CONNECTS TO: Allegiance game engine, mission server

NOTES: None

## `src/Pigs/Piglets/Teleporter.pig`

PURPOSE: This file contains a JScript behavior script for a piglet in the Allegiance game. It handles various state transitions and behaviors of the piglet, such as logging on to the mission server, joining missions, selecting hulls, launching, and managing ship operations.

KEY PARTS:
- OnStateNonExistant(eStatePrevious): Handles the initial logon to the mission server.
- OnStateMissionList(eStatePrevious): Joins or creates a mission based on availability.
- OnStateTeamList(eStatePrevious): Joins a random team if available.
- SelectBestHull(objHullTypes): Selects the best hull from a collection of hull types.
- OnStateDocked(ePrev): Launches the pig as soon as it becomes docked.
- OnStateFlying(eStatePrevious): Manages ship throttle and thrust operations.
- OnInitialThrottle(): Sets initial throttle to 100% and then decreases it.
- OnEndThrusting(): Stops the ship from thrusting and checks for ripcord probes.
- CheckForRipCordProbes(): Deploys ripcord probes if available.

CONNECTS TO: AutoStartGame.js, Pig.Logon(), Pig.JoinMission(), Pig.CreateMission(), Pig.Ship.BuyHull(), Host.Trace(), Timer.Kill(), Ship.Throttle(), Ship.Thrust(), Game.Probes

NOTES: None.

## `src/Pigs/Piglets/aaa.js`

PURPOSE: This JavaScript file contains configuration settings and event handlers for a bot named "Perpetual Bot DM" in the Allegiance game. It manages the bot's behavior during different states of play.

KEY PARTS:
- GameName, ServerName, ServerAddr, KillGoal, DebugSpam: Configuration variables.
- OnReceiveChat: Handles chat messages, including a command to check game state.
- OnStateFlying: Manages the bot's actions when flying.
- CheckBoost, CheckFireMissile: Functions to control ship boost and firing based on distance.
- FindTarget: Determines the nearest enemy target.
- UpdateTargetTimer: Updates the bot's target and performs actions like attacking or running away.
- CheckRunAway: Handles conditions for the bot to retreat from combat.
- OnShipDamaged, OnShipKilled: Event handlers for ship damage and death.

CONNECTS TO: imago.js (likely contains game-specific functions), IDL.d.ts (interface definitions).

NOTES:
- DebugSpam is enabled, which may produce a lot of chat output.
- There are TODO comments for improving target selection logic.
- Some functions like CheckFireMissile have disabled code paths.

## `src/Pigs/Piglets/aaa.pig`

PURPOSE: tiny file (16 lines), shown in full rather than summarized.

    <pigbehavior language="JScript">
    //imagotrigger@gmail.com 10/14 imago.pig originally from manuel.pig and mark.pig Microsoft
    // These files provide an intellisense bridge to COM. Use PigJSDocGenerator to update the intellisense if you modify an IDL file. -->
    <script src="include\IDL.js"/>
    // Helper functions to make your pig go.
    <script src="include\common.js"/>
    <script src="include\imago.js"/>
    <script src="include\AutoStartCustomGame.js"/>
    // The main logic of the pig lives in this file.
    <script src="aaa.js"/>
    <script>
    <![CDATA[
    // Refer to aaa.js for the core logic.
    ]]>
    </script>
    </pigbehavior>

## `src/Pigs/Piglets/alephjumper.pig`

PURPOSE:
The file `alephjumper.pig` defines a behavior script for a pig in the Allegiance game. The script causes the pig to find an aleph, fly through it, and return to its original sector repeatedly.

KEY PARTS:
- `OnActivate`: Initializes the behavior when the pig is activated.
- `SelectBestHull`: Selects the best hull type from available options.
- `OnReceiveChat`: Handles chat messages, specifically looking for a command to set a test aleph name.
- `OnStateDocked`: Launches the pig once it becomes docked and selects the best hull.
- `DoLaunch`: Starts the launch process.
- `PostLaunch`: Initiates the flight towards an aleph after launch.
- `OnSectorChange`: Handles sector changes, updating the target aleph name accordingly.
- `OnStateFlying`: Sets up initial throttle settings and schedules further actions.
- `OnInitialThrottle`: Adjusts timer intervals for orbit correction.
- `FindTargetAleph`: Finds the first aleph in the current sector to fly through.
- `CorrectOrbit`: Faces the ship towards the target aleph.

CONNECTS TO:
- `common.js`: Contains utility functions used by the script.

NOTES:
- None.

## `src/Pigs/Piglets/b1.pig`

PURPOSE: This file contains a custom bot behavior script for Allegiance, specifically designed to control a ship in a multiplayer game. The bot is programmed to engage enemies, find targets, and manage resources.

KEY PARTS:
- `OnReceiveChat`: Handles chat messages from other ships.
- `OnStateFlying`: Manages the bot's state when flying, including finding and attacking targets.
- `FindTargetTimer`: Finds the nearest enemy ship to attack.
- `UpdateTargetTimer`: Updates the target based on distance and engagement status.
- `RearmTimer`: Handles rearming and repairing the ship.
- `OnShipDamaged`: Adjusts behavior when taking damage.
- `OnShipKilled`: Adjusts behavior when killing an enemy.

CONNECTS TO: 
- common.js
- imago.js

NOTES:
- The script includes commented-out settings for different game configurations.
- There are TODO comments for various features like rescue, missiles, and countermeasures.
- DebugSpam is enabled to send chat messages with debug information.

## `src/Pigs/Piglets/bugman.pig`

PURPOSE: This file contains a JScript behavior script for a piglet named Bugman in the Allegiance game. It defines the behavior of the piglet during various states such as logging on to the mission server, joining missions, selecting teams, docking, launching, and flying.

KEY PARTS:
- OnStateNonExistant: Handles the initial state where the pig is being created and logs on to the mission server.
- OnStateMissionList: Joins or creates a mission for the pig.
- OnStateTeamList: Joins a random team if available.
- OnStateDocked: Buys a hull upon docking and launches into space.
- OnStateFlying: Defends an outpost.

CONNECTS TO: AutoStartGame.js, ActivePigs objects (This, Pig, Host), HullTypes, Ship.BuyHull, Pig.JoinMission, Pig.CreateMission, Pig.Launch, Pig.Ship.BuyHull, Pig.JoinTeam, and Pig.Defend.

NOTES: None.

## `src/Pigs/Piglets/bulldozer.pig`

PURPOSE: This file contains a behavior script for a piglet named Bulldozer in the Allegiance game. The script controls the piglet's actions based on its state and environment.

KEY PARTS:
- SelectBestHull: Determines the best hull to buy from available options.
- OnStateDocked: Handles the transition when the piglet is docked, selecting a hull and launching into space.
- WhoAmI: Identifies the ship and sector of the piglet.
- FindTarget: Searches for a target ship or station by name.
- Range2Ship: Calculates the squared distance to another ship.
- OnStateFlying: Sets the throttle to 100% and schedules a timer for initial throttle correction.
- OnInitialThrottle: Initializes the throttle correction interval.
- CorrectOrbit: Adjusts the ship's orientation towards the target and manages firing weapons based on range.
- DoRipCord: Initiates a ripcord operation, stopping all movement and weapons.
- EndRipCord: Ends the ripcord operation.
- ThrustOrbit: Controls thrust and firing based on the distance to the target.
- CollisionTest: Sets the target for collision detection.

CONNECTS TO: Allegiance game engine and related scripts for ship behavior.

NOTES: None.

## `src/Pigs/Piglets/chatter.pig`

PURPOSE: This file contains behavior scripts for a piglet in the Allegiance game. It handles various states and transitions related to logging into missions, joining teams, selecting hulls, launching, and chatting.

KEY PARTS:
- OnStateNonExistant: Handles initial logon to the mission server.
- OnStateMissionList: Joins or creates a mission.
- OnStateTeamList: Joins a random team.
- SelectBestHull: Selects the best hull from available options.
- OnStateDocked: Buys and selects the best hull when docked.
- OnStateFlying: Manages throttle and thrust for flying state.
- StartChatting: Starts sending chat messages.
- StopChatting: Stops sending chat messages.

CONNECTS TO: AutoStartGame.js, Pig.Logon, Pig.JoinMission, Pig.CreateMission, Pig.Ship.BuyHull, Host.Trace, Game.SendChat

NOTES: None.

## `src/Pigs/Piglets/colbug-a.pig`

PURPOSE: This file contains the behavior script for piglets in Allegiance that navigate towards and orbit around asteroids.

KEY PARTS:
- OnStateFlying: Handles the state transition when the piglet is flying. Finds the closest asteroid, updates its position vector, sets the throttle to 100%, and creates timers.
- OnInitialThrottle: Sets the ship's throttle to 0% and configures a timer for orbit correction.
- UpdatePositionVector: Updates the position vector of the current asteroid.
- FindClosestAsteroid: Finds the closest asteroid in the sector.
- CorrectOrbit: Faces the ship towards the asteroid and calls ThrustOrbit.
- ThrustOrbit: Applies forward thrust, toggles weapon firing, checks for ammo and energy, and commits suicide if necessary.

CONNECTS TO: Allegiance game engine, AGC.AGCVector ActiveX object, Ship, Asteroids collection.

NOTES: None.

## `src/Pigs/Piglets/colbug-p.pig`

PURPOSE: This file contains a JScript behavior script for piglets in Allegiance, specifically for the colbug-p.pig entity. It defines the behavior of the piglet when it is flying.

KEY PARTS:
- OnStateFlying: Sets the target vector and throttle to 100% initially, then creates a timer to reset the throttle.
- OnInitialThrottle: Sets the ship's throttle to 0%, sets the timer interval to 2 seconds, and calls CorrectOrbit.
- CorrectOrbit: Faces the ship towards the target vector and calls ThrustOrbit if successful.
- ThrustOrbit: Toggles weapon firing, checks for empty ammo or low energy, and commits suicide if necessary.

CONNECTS TO: AGC.AGCVector, Ship (from Allegiance's game engine).

NOTES: None.

## `src/Pigs/Piglets/colbug-s.pig`

PURPOSE: This file contains a JScript behavior script for piglets in the Allegiance game. It defines the behavior of piglets when they are flying.

KEY PARTS:
- OnStateFlying(eStatePrevious): Handles state transition to flying by setting throttle and creating a timer.
- OnInitialThrottle(): Sets throttle to 0% after initial duration, sets timer interval, and calls CorrectOrbit().
- CorrectOrbit(): Faces the ship towards TargetVector and calls ThrustOrbit() if successful.
- ThrustOrbit(): Applies forward thrust, toggles weapon firing, checks for ammo and energy, and commits suicide if conditions are met.

CONNECTS TO: Allegiance game engine, AGC.AGCVector object, Ship object methods (Throttle, Face, FireWeapon), Timer object methods (CreateTimer, Interval, ExpirationExpression).

NOTES: None.

## `src/Pigs/Piglets/colltest.js`

PURPOSE: tiny file (4 lines), shown in full rather than summarized.

    function OnReceiveChat(strText, objShip) {
        var a = 2;
    }
    //# sourceMappingURL=colltest.js.map

## `src/Pigs/Piglets/defaultv.pig`

PURPOSE: This file contains a VBScript behavior script for a piglet in the Allegiance game. It defines the actions and state transitions for the piglet's behavior.

KEY PARTS:
- OnStateNonExistant: Handles the initial logon to the mission server.
- OnStateMissionList: Joins or creates a mission if none exists.
- OnStateTeamList: Joins a random team.
- SelectBestHull: Selects the best hull from available options.
- OnStateDocked: Launches the pig as soon as it becomes docked.
- OnStateFlying: Sets the ship's throttle and initiates thrusting.
- OnInitialThrottle: Adjusts the throttle and starts thrusting left.
- OnEndThrusting: Stops the ship from thrusting.

CONNECTS TO: Allegiance game engine, mission server, and other piglet behavior scripts.

NOTES: None.

## `src/Pigs/Piglets/demo.pig`

PURPOSE: This file contains a JScript behavior script for a piglet in Allegiance. It defines the behavior of the piglet when it is docked and flying.

KEY PARTS:
- SelectBestHull: Selects the best hull from a collection based on name criteria.
- OnStateDocked: Handles state transition when the piglet becomes docked, buying the best available hull and launching.
- WhoAmI: Identifies the ship associated with the piglet.
- FindTarget: Searches for a target ship or station by name.
- OnStateFlying: Sets initial throttle and creates timers for orbit correction and thrust control.
- OnReceiveCommand: Handles chat commands to enable/disable Eliza chat responses and start a demo.
- CorrectOrbit: Adjusts the piglet's orientation towards the target and corrects its orbit.
- ThrustOrbit: Controls thrust based on distance from the target, stopping if too close or committing suicide if out of ammo.

CONNECTS TO: common.js, chatcommand.js, eliza.js

NOTES: None

## `src/Pigs/Piglets/dev.pig`

PURPOSE: This file contains a JScript behavior script for a piglet in the Allegiance game. It defines the behavior of the piglet during different states such as mission list, team list, docked, and flying.

KEY PARTS:
- OnStateMissionList: Joins or creates a mission.
- OnStateTeamList: Joins a random team.
- OnStateDocked: Buys the best hull upon docking and launches into space.
- OnStateFlying: Sets throttle to 100% and starts correcting orbit.
- CorrectOrbit: Adjusts ship's velocity and thrust based on target distance.
- ThrustOrbit: Calls ProperVelocity and checks for empty ammo.

CONNECTS TO: Allegiance game engine, Pig behavior system, Game object model.

NOTES: None.

## `src/Pigs/Piglets/dm.pig`

PURPOSE: This file contains a bot script for Allegiance that plays as a Deep Space Interceptor in Deathmatch mode. It includes logic for finding and attacking enemies, managing resources, and maintaining the ship's health.

KEY PARTS:
- `OnReceiveChat`: Handles chat messages to manage rescue operations.
- `OnStateFlying`: Manages the bot's state when flying, including target selection, attack, and resource management.
- `FindTargetTimer`: Finds the nearest enemy to attack.
- `UpdateTargetTimer`: Updates the current target based on distance and engagement status.
- `RearmTimer`: Handles rearming and repairing the ship.
- `OnShipDamaged`: Adjusts target if damaged by an enemy.
- `OnShipKilled`: Increases bot skills upon killing an enemy.

CONNECTS TO: common.js, imago.js, AutoStartCustomGame.js

NOTES:
- Contains TODO comments for various features like rescue operations and missile handling.
- DebugSpam can be toggled to enable or disable debug chat messages.

## `src/Pigs/Piglets/docker.pig`

PURPOSE: This file contains a behavior script for pigs in Allegiance that controls their undocking and docking processes. It handles state transitions and sends chat commands to manage traffic control communally.

KEY PARTS:
- OnStateFlying: Handles the initial state when the pig is flying.
- StepOne: Faces a random asteroid.
- StepTwo: Sets throttle and schedules the next command.
- SendDockCommand2: Stops the ship, sends a dock command, and schedules the next action.
- OnReceiveChat: Processes chat commands to handle docking.

CONNECTS TO: ActivePigs object model for behavior management.

NOTES:
- The commented-out functions `SendDockCommand` and `Ship.AutoAcceptCommands(true)` indicate potential features that were not implemented.
- There is a TODO comment suggesting the need to uncomment `Ship.AutoAcceptCommands(true)`.

## `src/Pigs/Piglets/framerate-dummy.pig`

PURPOSE: This file contains a JScript behavior script for Allegiance's Piglets AI system. It defines the actions and behaviors of a piglet in the game.

KEY PARTS:
- `SelectBestHull`: Selects the best hull from a collection based on specific criteria.
- `OnStateDocked`: Handles state transition when the piglet is docked, launching it with the best available hull.
- `OnStateFlying`: Manages the flying state of the piglet, setting throttle and aiming.
- `OnInitialThrottle`: Initializes throttle settings and starts a timer for orbit correction.
- `OnReceiveCommand`: Processes chat commands to control AI behavior.
- `CorrectOrbit`: Adjusts the piglet's orbit based on its distance from the target.
- `ThrustOrbit`: Controls the piglet's thrust based on its distance from the target.

CONNECTS TO: 
- `include\common.js`, `include\AutoStartGame.js`, `include\GetIntoTeam.js`, `include\chatcommand.js`, `include\eliza.js`

NOTES:
- Contains commented-out code and TODOs for further development.
- Uses ActiveX objects specific to Allegiance's game engine.

## `src/Pigs/Piglets/framerate.pig`

PURPOSE: This script defines behaviors and actions for a piglet in the Allegiance game, focusing on testing frame rate performance.

KEY PARTS:
- SelectBestHull: Determines the best hull type to buy based on name.
- OnStateDocked: Handles state transition when the pig is docked, selecting and buying the best hull.
- PreLaunch: Prepares for launch by setting up timers and sending chat messages.
- StepOne through StepNine: A series of steps defining a complex sequence of actions for testing frame rate performance.
- OnReceiveCommand: Handles chat commands to enable/disable Eliza responses or count dummy pigs.

CONNECTS TO: include\common.js, include\AutoStartGame.js, include\GetIntoTeam.js, include\chatcommand.js, include\eliza.js

NOTES: None.

## `src/Pigs/Piglets/it.pig`

PURPOSE: This file contains a JScript behavior script for pigs in the Allegiance game. The script defines the actions and behaviors of piglets when they are flying.

KEY PARTS:
- OnStateFlying(eStatePrevious): Sets the ship's throttle to 100% and creates a timer to reset it.
- OnInitialThrottle(): Sets the ship's throttle to 0%, stops all movement, and adjusts a timer to start a collision test every 30 seconds.
- StartCollisionTest(): Sends a chat command to the sector to initiate a collision test with this pig.
- CollisionTest(): Sends a chat message indicating that the pig is ready for a collision.

CONNECTS TO: ActivePigs object model, Ship object, Timer object, Game object

NOTES: None.

## `src/Pigs/Piglets/mark2.pig`

PURPOSE:
This file contains a JScript behavior script for Allegiance's Piglets game AI. It defines the behavior and state transitions of a piglet in the game.

KEY PARTS:
- OnStateNonExistant: Handles initial logon to the mission server.
- OnStateMissionList: Joins or creates a mission if none exist.
- OnStateTeamList: Joins a random team.
- SelectBestHull: Chooses the best hull from available options.
- OnStateDocked: Buys and equips the best hull upon docking, then launches into space.
- OnStateFlying: Sets initial throttle and creates a timer to reset it.
- OnInitialThrottle: Resets throttle to 0% after a delay.
- CorrectOrbit: Keeps the ship in orbit around the first station or commits suicide if conditions are not met.
- ThrustOrbit: Stops the ship, thrusts left, fires weapons, and checks for ammo.

CONNECTS TO:
AutoStartGame.js

NOTES:
- None

## `src/Pigs/Piglets/mark2s.pig`

PURPOSE: This file contains a JScript behavior script for a piglet in the Allegiance game. It defines the behavior of the piglet during various states such as docking, mission joining, and flying.

KEY PARTS:
- OnActivate: Initializes the behavior when the piglet is activated.
- OnStateNonExistant: Handles the state transition when the piglet does not exist.
- OnStateMissionList: Joins or creates a mission for the piglet.
- OnStateTeamList: Joins a random team for the piglet.
- SelectBestHull: Selects the best hull from a collection of hull types.
- OnStateDocked: Handles the state transition when the piglet is docked, buying the best available hull and scheduling a launch timer.
- OnDoLaunch: Launches the piglet into space after a delay.
- OnStateFlying: Sets the ship's throttle to 100% and schedules a timer to reset it.
- OnInitialThrottle: Resets the ship's throttle to 0% and sets up a timer for orbit correction.
- CorrectOrbit: Attempts to face the "Outpost" and then calls ThrustOrbit.
- ThrustOrbit: Stops the ship, thrusts to the left, toggles weapon firing, and checks for empty ammo.

CONNECTS TO: AutoStartGame.js

NOTES: None.

## `src/Pigs/Piglets/mark3.pig`

PURPOSE: This file contains a JScript behavior script for a piglet in the Allegiance game. The script defines the behavior of the piglet during various states such as logging on to the mission server, joining missions, selecting hulls, launching into space, and engaging in combat.

KEY PARTS:
- OnStateNonExistant: Handles state transition when the pig is being created.
- OnStateMissionList: Joins or creates a mission.
- SelectBestHull: Selects the best hull from a collection of hulls.
- OnStateDocked: Launches the pig as soon as it becomes docked.
- WhoAmI: Identifies and traces information about the ship.
- FindTarget: Finds and sets the target for the piglet.
- Range2Ship: Calculates the distance squared to an AGCShip or PigShip.
- OnStateFlying: Sets the throttle and creates a timer to reset it.
- CorrectOrbit: Adjusts the orbit of the ship towards the target.
- ThrustOrbit: Controls the thrust and firing based on the distance to the target.

CONNECTS TO: Other piglet behavior scripts, mission server, game ships, and stations.

NOTES: None.

## `src/Pigs/Piglets/marksn1.pig`

PURPOSE: This file contains a JScript behavior script for a piglet in the Allegiance game. It defines the AI and actions of the piglet, including mission joining, state transitions, and various behaviors like scanning sectors, finding targets, and engaging enemies.

KEY PARTS:
- OnStateMissionList: Joins or creates a mission.
- OnStateTeamList: Joins a random team.
- OnStateDocked: Launches the pig as soon as it becomes docked.
- OnStateFlying: Handles state transition when flying.
- OnReceiveCommand: Processes chat commands from players.
- OnShipDamaged: Handles ship damage events.
- OnSectorChange: Updates sector information when changing sectors.
- UpdateSectorLists: Updates lists of visited and not visited sectors.
- ScanForEnemies: Scans for enemy ships and stations.
- FindAlephTargets: Finds targets based on the current state (scanning, retreating, mission).
- CorrectOrbit: Adjusts the ship's orbit towards a target.

CONNECTS TO: include\common.js, include\chatcommand.js, include\asteroids.js, include\dijkstra.js, include\eliza.js, include\AutoStartGame.js

NOTES: None.

## `src/Pigs/Piglets/scout2.pig`

PURPOSE: Manages the behavior and state transitions of a scout piglet in Allegiance, handling activation, mission joining, team management, launching, flying, damage responses, sector changes, enemy detection, and resource scanning.

KEY PARTS:
- OnActivate: Initializes data structures and transitions to the Flying state.
- OnStateMissionList: Joins or creates missions for the scout.
- OnStateTeamList: Ensures the piglet is in a team if not already.
- OnStateDocked: Launches the pig once it becomes docked.
- OnDoLaunch: Helper function to launch the pig after a delay.
- OnStateFlying: Sets up initial throttle and starts timers for navigation, resource scanning, and enemy detection.
- OnInitialThrottle: Configures continuous orbit correction, target finding, enemy scanning, and asteroid ore scanning.
- OnReceiveCommand: Handles chat commands from other ships, including status control, Eliza responses, autodonation, and sector reports.
- UpdateSectorLists: Rebuilds lists of visited, not visited, and claimed sectors based on current game state.
- ScanForEnemies: Counts enemies in the sector and sets retreat status if threat level is high.

CONNECTS TO: Allegiance game engine systems for ship management, mission handling, team coordination, and game state updates.

NOTES: None.

## `src/Pigs/Piglets/stem.pig`

PURPOSE: This file defines a behavior script for a pig in the Allegiance game that allows it to dock and wait for a chat command to switch to another behavior.

KEY PARTS:
- OnStateDocked: Handles the state when the pig is docked, doing nothing.
- OnActivate: Called when the pig is activated.
- OnReceiveChat: Processes chat messages to check for "become" commands and switches behaviors accordingly.
- Stack.Push: Attempts to push a specified behavior onto the stack.

CONNECTS TO: Allegiance game engine's ActivePigs object model.

NOTES: None.

## `src/Pigs/Piglets/stresslobby.pig`

PURPOSE: This file contains a JScript behavior script for the Piglets in Allegiance, specifically for managing the stress lobby state transitions and actions.

KEY PARTS:
- OnStateNonExistant(eStatePrevious): Handles the initial logon to the mission server when the pig is created.
- OnStateMissionList(eStatePrevious): Initiates joining or creating a mission after a random delay.
- DoMissionList(): Attempts to join an existing mission; shuts down if it fails.
- OnStateTeamList(eStatePrevious): Quits the game after a random delay.
- DoQuitGame(): Logs off from the mission server.

CONNECTS TO: Allegiance mission server and game systems for logon, joining missions, and logging off.

NOTES: None.

## `src/Pigs/Piglets/zzz.pig`

PURPOSE:
This file contains a custom bot behavior script for Allegiance, a 2000 Microsoft multiplayer space game. The script defines the AI logic for a bot named Imago in a specific match configuration.

KEY PARTS:
- `OnReceiveChat`: Handles chat messages to manage rescue operations.
- `OnStateFlying`: Manages the bot's state when flying and updates target selection, rearming, and other behaviors.
- `FindTargetTimer`: Finds the nearest enemy ship to attack.
- `UpdateTargetTimer`: Continuously updates the target based on distance and engagement status.
- `RearmTimer`: Handles rearming logic for the bot.
- `OnShipDamaged`: Adjusts target selection when damaged by an enemy ship.
- `OnShipKilled`: Sends chat messages upon killing an enemy or losing a ship.

CONNECTS TO:
Depends on common.js, imago.js, and AutoStartCustomGame.js for additional functionality and configurations.

NOTES:
- The script includes multiple commented-out settings blocks that can be toggled to change game parameters.
- There are TODO comments indicating areas where further development is needed.
- DebugSpam is enabled in the current configuration, which sends chat messages with debug information.
