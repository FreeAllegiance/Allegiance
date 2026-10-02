# src/Pigs/Piglets/moldy

[Back to the index](../LLAMA_SUMMARY.md) - 3 files, 1,045 lines. Notes by a local model, to orient by: not verified facts.

This folder contains JScript behavior scripts for piglets in Allegiance, focusing on mission-related actions and state transitions.

- scout.pig: Defines the behavior of a scout pig, including mission joining, hull selection, and scanning for asteroids and enemy stations.
- Mark0.pig: Handles various state transitions and actions for a piglet named Mark0, such as logging on to the mission server and controlling ship launches.
- Mark1.pig: Similar to Mark0.pig, defines behavior for a piglet named Mark1 during different states like joining missions and selecting the best hull.

## `src/Pigs/Piglets/moldy/Mark0.pig`

PURPOSE: This file contains a JScript behavior script for a piglet in the Allegiance game. It handles various state transitions and actions such as logging on to the mission server, joining missions, selecting hulls, launching, and controlling ship movements.

KEY PARTS:
- OnStateNonExistant: Handles initial logon to the mission server.
- OnStateMissionList: Joins or creates a mission.
- OnStateTeamList: Joins a random team.
- SelectBestHull: Selects the best hull from available options.
- OnStateDocked: Buys and equips a ship upon docking.
- OnStateFlying: Controls ship throttle and thrust.
- OnInitialThrottle: Sets initial throttle and initiates thrusting.
- OnEndThrusting: Stops thrusting and faces an outpost.

CONNECTS TO: AutoStartGame.js, Pig.Logon, Pig.JoinMission, Pig.CreateMission, Pig.Ship.BuyHull, Pig.Launch, Host.Trace, Host.Random.

NOTES: None.

## `src/Pigs/Piglets/moldy/Mark1.pig`

PURPOSE: This file contains a JScript behavior script for a piglet named Mark1 in the Allegiance game. It defines the behavior of the piglet during various states such as logging on to the mission server, joining missions, selecting the best hull, and launching into space.

KEY PARTS:
- OnStateNonExistant: Handles state transition when the pig is being created.
- OnStateMissionList: Joins or creates a mission for the pig.
- OnStateTeamList: Joins a random team for the pig.
- SelectBestHull: Selects the best hull from a collection of hulls based on type.
- OnStateDocked: Handles state transition when the pig is docked, selecting and buying a hull, and launching into space.
- OnStateFlying: Handles state transition when the pig is flying, setting throttle and creating timers for initial thrusting and facing an outpost.
- OnInitialThrottle: Sets the ship's throttle to 0% after initial thrusting.
- OnEndThrusting: Stops the ship from thrusting and faces the outpost.

CONNECTS TO: AutoStartGame.js

NOTES: None.

## `src/Pigs/Piglets/moldy/scout.pig`

PURPOSE: This file defines the behavior of a scout pig in Allegiance, focusing on mission joining, hull selection, and various actions like scanning for asteroids and enemy stations.

KEY PARTS:
- OnStateMissionList: Joins or creates a mission.
- OnStateDocked: Buys the best hull and launches the ship.
- OnReceiveCommand: Handles chat commands from players.
- ScanForEnemies: Scans the current sector for enemy ships and stations.
- CheckOreAsteroid: Checks if an asteroid contains valuable ores.
- FindAlephTargets: Finds and targets Alephs (energy sources) in the sector.

CONNECTS TO: common.js, chatcommand.js, AutoStartGame.js

NOTES: None.
