# src/Wopr/Strategies

[Back to the index](../LLAMA_SUMMARY.md) - 3 files, 1,502 lines. Notes by a local model, to orient by: not verified facts.

This folder contains strategies and related logic for managing game operations in Allegiance.

- CommanderResearchAndExpand.cs: Manages a strategy focused on researching and expanding by investing in specific technologies.
- ConnectToGame.cs: Handles the process of connecting to a game server, joining a mission, and managing team readiness.
- ScoutExploreMap.cs: Implements a scouting strategy for exploring clusters, focusing on finding and managing Aelphs.

## `src/Wopr/Strategies/CommanderResearchAndExpand.cs`

PURPOSE: The file defines a strategy class named CommanderResearchAndExpand that extends StrategyBase. This strategy focuses on researching and expanding by investing in specific technologies.

KEY PARTS:
- CommanderResearchAndExpand: Extends StrategyBase and implements the research and expand strategy.
- AttachMessages: Placeholder method for attaching messages, currently empty.
- Start: Initiates the strategy by calling InvestInTech with predefined technology names.
- InvestInTech: Iterates through available buckets (technologies), checks if they start with a specified tech name, and attempts to buy them if sufficient money is available.

CONNECTS TO:
- Wopr.StrategyBase: The base class for strategies in Allegiance.
- Wopr.Constants.StrategyID.CommanderResearchAndExpand: A constant representing the strategy ID.
- Wopr.Client: Used to interact with the game client, such as retrieving side information and managing money.

NOTES:
- None.

## `src/Wopr/Strategies/ConnectToGame.cs`

PURPOSE: The `ConnectToGame` class in Allegiance is a strategy that handles the process of connecting to a game server, joining a mission, and managing team readiness.

KEY PARTS:
- `ConnectToGame`: Main class implementing the strategy.
- `AttachMessages`: Registers message handlers for various events related to logging on, server responses, and mission updates.
- `Start`: Placeholder method that could be used to initialize the strategy.
- `MessageReceiver_FMD_S_MISSION_STAGE` and `MessageReceiver_FMD_S_SHIP_STATUS`: Handle mission stage changes and ship status updates, respectively.
- `CheckForLaunch`: Determines if all teams are ready and initiates game launch.
- `ResetGameParameters`: Resets mission parameters to default values.

CONNECTS TO: 
- AllegianceInterop namespace for core game interactions.
- MessageReceiver class for handling incoming messages from the server.

NOTES:
- Contains commented-out code that could be re-enabled or modified.
- Logs various events and states using a custom logging mechanism.

## `src/Wopr/Strategies/ScoutExploreMap.cs`

PURPOSE: The file contains the implementation of a scouting strategy for exploring clusters in Allegiance, focusing on finding and managing Aelphs (likely important resources or entities).

KEY PARTS:
- ScoutExploreMap: Main class for the scouting strategy.
- AttachMessages: Handles attaching messages to the strategy.
- UpdateUnexploredWarpsList: Updates a list of unexplored clusters.
- FlyToStationPointAndWaitForAelphs: Navigates to a point and waits for Aelphs.
- SweepCluster: Sweeps through a cluster to find Aelphs.
- SelectNextAelphTarget: Determines the next target warp for exploration.
- FindNearestUnexploredCluster: Finds the nearest unexplored cluster using Dijkstra's algorithm.
- Start: Initializes the scout strategy.
- ScoutSector: Manages scouting activities within sectors.
- ReturnToBase: Returns the scout to its base.

CONNECTS TO: Other parts of the game, likely including ship management and resource handling systems.

NOTES:
- TODOs: Remove commented-out code Refactor repetitive logic
