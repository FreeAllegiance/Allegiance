# src/Igc

[Back to the index](../LLAMA_SUMMARY.md) - 82 files, 43,522 lines. Notes by a local model, to orient by: not verified facts.

The Igc folder in Allegiance contains core game object and mission management system components. It handles various aspects of game entities such as ships, parts, asteroids, and missions.

- igc.h: Defines interfaces and classes for game objects and data structures.
- missionigc.cpp: Manages mission-related data and operations.
- shipIGC.cpp: Implements the CshipIGC class, managing the behavior and state of ships.

Largest files: igc.h (6188 lines), missionigc.cpp (5706 lines), shipIGC.cpp (4768 lines), common.cpp (3109 lines), shipIGC.h (2673 lines).

## `src/Igc/CMakeLists.txt`

PURPOSE: This file is a CMake configuration script for building the Igc module in Allegiance. It sets up include directories, defines macros, and specifies source files to compile into a library named Igc.

KEY PARTS:
- ADD_DEFINITIONS(-D__MODULE__="igc"): Defines a macro for the current module.
- INCLUDE_DIRECTORIES(../Inc ../zlib ../engine  ../soundengine ../guids): Specifies directories containing header files.
- SET(Igc_SRC ...): Lists all source files for the Igc library.
- ADD_LIBRARY(Igc ${Igc_SRC}): Creates a library target named Igc from the specified source files.
- SET_PROPERTY(TARGET Igc PROPERTY CXX_STANDARD 14): Sets the C++ standard to C++14 for the Igc library.

CONNECTS TO: Other modules and libraries in Allegiance, such as engine, soundengine, and zlib.

NOTES: None.

## `src/Igc/afterburnerIGC.cpp`

PURPOSE: This file contains the implementation of the `CafterburnerIGC` class, which manages afterburner functionality for parts in a game.

KEY PARTS:
- `CafterburnerIGC`: Manages the state and behavior of an afterburner part.
- `Initialize`: Initializes the afterburner with mission data.
- `Terminate`: Cleans up resources when the afterburner is no longer needed.
- `Update`: Updates the afterburner's state based on time and ship conditions.
- `SetShip`: Sets the ship to which the afterburner is attached.
- `SetMountID`: Changes the mount point of the afterburner.
- `IncrementalUpdate`: Handles incremental updates, including activation, deactivation, and fuel consumption.

CONNECTS TO: `ImissionIGC`, `IshipIGC`, `IIgcSite`, `DataPartIGC`, `DataAfterburnerTypeIGC`.

NOTES: None.

## `src/Igc/afterburnerIGC.h`

PURPOSE: This file defines the CafterburnerIGC class, which represents an afterburner in the Allegiance game. It implements interfaces for initialization, update, and control of the afterburner's behavior.

KEY PARTS:
- CafterburnerIGC: Main class representing an afterburner.
- Initialize: Initializes the afterburner with mission data.
- Terminate: Cleans up resources when the afterburner is no longer needed.
- Update: Updates the state of the afterburner based on time and other factors.
- Activate/Deactivate: Controls whether the afterburner is active or not.
- GetMaxThrustWithGA: Calculates the maximum thrust considering global attributes.
- SetPower: Sets the power level of the afterburner, activating it if necessary.

CONNECTS TO: ImissionIGC (mission data), IpartTypeIGC (part type data), and IshipIGC (ship).

NOTES: None.

## `src/Igc/asteroidigc.cpp`

PURPOSE: This file contains the implementation of the CasteroidIGC class, which represents asteroids in the game. It handles initialization and data export for asteroids.

KEY PARTS:
- CasteroidIGC::Initialize: Initializes an asteroid with given data.
- CasteroidIGC::Export: Exports the current state of an asteroid to a data structure.
- AsteroidTypeRow: Defines properties of different types of asteroids.
- FindAsteroidRow: Finds the row for a specific asteroid type based on its capabilities.

CONNECTS TO: TmodelIGC, ImissionIGC, IclusterIGC, IsideIGC

NOTES:
- Contains assertions and debug checks to ensure data integrity.
- Hardcoded values for the number of special asteroids and their distribution.

## `src/Igc/asteroidigc.h`

PURPOSE:
The file `asteroidIGC.h` defines the class `CasteroidIGC`, which represents an asteroid in the Allegiance game. This class inherits from `TmodelIGC<IasteroidIGC>` and implements various interfaces such as `IbaseIGC`, `ImodelIGC`, `IdamageIGC`, and `IasteroidIGC` to handle asteroid behavior, damage, ore mining, and other interactions.

KEY PARTS:
- **Class: CasteroidIGC** - Manages the state and behavior of an asteroid in the game.
  - **Constructor**: Initializes the fraction representing the asteroid's health or completeness.
  - **Methods**:
    - `Initialize`: Sets up the asteroid with mission data.
    - `Terminate`: Cleans up resources when the asteroid is destroyed.
    - `Export`: Exports the asteroid's state to a data buffer.
    - `Update`: Updates the asteroid's state, including ore regeneration and damage handling.
    - `GetObjectType` and `GetObjectID`: Return the type and ID of the object.
    - `SetCluster`: Manages the cluster membership of the asteroid.
    - `ReceiveDamage`: Handles damage received by the asteroid.
    - `GetFraction`, `SetFraction`, `GetHitPoints`: Manage the asteroid's health fraction.
    - `GetOre`, `SetOre`, `MineOre`: Handle ore management and mining.
    - `GetCapabilities`, `HasCapability`: Check asteroid capabilities.
    - `GetBuildingEffect`, `SetBuildingEffect`: Manage any building effects on the asteroid.
    - Additional methods for managing ore seen by sides, builder visibility, and update inhibition.

CONNECTS TO:
- Other game systems such as mission management, sound effects, and side-specific data handling.

NOTES:
- The file includes commented-out code related to ore regeneration and client updates that were not fully implemented.
- There are TODO comments indicating areas for future development or improvements.

## `src/Igc/bucketigc.cpp`

**PURPOSE:** The file `bucketigc.cpp` contains the implementation of the `CbucketIGC` class, which manages a bucket in the game's mission system. Buckets represent various objects that can be built or developed within a side's resources.

**KEY PARTS:**
- **Class: CbucketIGC** - Manages a bucket object.
  - **Function: Initialize** - Initializes the bucket with mission data, sets up references to the mission and buyable object, calculates price and build time adjustments based on global attributes, and checks for predecessor buckets.
  - **Function: Terminate** - Cleans up resources by releasing references and notifying the mission of the bucket's termination.
  - **Function: Export** - Exports the bucket data (currently not implemented).

**CONNECTS TO:** 
- `ImissionIGC` - The mission object that contains the bucket.
- `IbuyableIGC` - The buyable object associated with the bucket.
- `IdevelopmentIGC` - For development buckets, it interacts with development-specific attributes and effects.

**NOTES:** None.

## `src/Igc/bucketigc.h`

PURPOSE: The file defines the CbucketIGC class, which represents a bucket in the Allegiance game. Buckets are used to track investment in various types of objects like development, drones, stations, hulls, and parts.

KEY PARTS:
- CbucketIGC: Inherits from IbucketIGC and implements methods for initializing, updating, and managing buckets.
- Initialize: Sets up the bucket with mission data.
- Update: Handles the progress of the bucket's investment over time.
- ForceComplete: Completes the bucket's investment based on its type (development, drone, station, hull, or part).
- GetPercentComplete: Calculates the percentage completion of the bucket's investment.

CONNECTS TO: ImissionIGC, DataBucketIGC, Time, and various other interfaces like IbuyableIGC and IsideIGC.

NOTES: None.

## `src/Igc/buildingeffectigc.h`

**PURPOSE:** This file contains the implementation of the `CbuildingEffectIGC` class, which manages building effects in the Allegiance game. It handles the initialization, update, and termination of building effects, including transforming asteroids into stations.

**KEY PARTS:**
- **Class: CbuildingEffectIGC**
  - Manages the lifecycle and behavior of building effects.
  - Handles initialization with mission data.
  - Updates the effect over time, changing its position, radius, and opacity.
  - Terminates the effect when complete or if a collision occurs.

- **Function: Initialize**
  - Initializes the building effect with mission data, setting up initial positions, times, and references to other game objects.

- **Function: Update**
  - Updates the visual and logical state of the building effect over time.

- **Function: Terminate**
  - Cleans up resources and handles the termination of the building effect.

- **Function: Export**
  - Exports the current state of the building effect for network transmission or saving.

- **Function: AsteroidUpdate**
  - Handles the transformation of an asteroid into a station when the building process is complete.

**CONNECTS TO:** 
- `ImissionIGC`, `IclusterIGC`, `IasteroidIGC`, `IstationTypeIGC`, and other game object interfaces for interaction and data exchange.

**NOTES:**
- None.

## `src/Igc/buoyigc.cpp`

PURPOSE:
This file contains the implementation of the CbuoyIGC class, which represents a buoy in the Allegiance game. The buoys are non-tracking missiles with a fixed run time and can be visible or invisible.

KEY PARTS:
- `CbuoyIGC`: Class representing a buoy.
  - Constructor: Initializes member variables.
  - Destructor: Cleans up resources.
  - `Initialize`: Sets up the buoy based on mission data, including position, visibility, type, and cluster association.
  - `Terminate`: Removes the buoy from the mission and cleans up resources.
  - `Export`: Exports the current state of the buoy to a data structure.

CONNECTS TO:
- `ImissionIGC`: Interface for mission management.
- `DataBuoyIGC`: Data structure containing initial parameters for buoys.

NOTES:
- None.

## `src/Igc/buoyigc.h`

PURPOSE:
The file `buoyIGC.h` defines the class `CbuoyIGC`, which represents a buoy in the Allegiance game. This class inherits from `TmodelIGC<IbuoyIGC>` and implements interfaces for initialization, termination, export, visibility control, and specific buoy-related functionality.

KEY PARTS:
- **Class: CbuoyIGC**
  - Inherits from `TmodelIGC<IbuoyIGC>`
  - Implements methods for initializing, terminating, exporting data, setting cluster, managing visibility, adding/removing consumers, and getting buoy type.
  
- **Methods:**
  - `Initialize`: Initializes the buoy with mission data.
  - `Terminate`: Cleans up resources when the buoy is no longer needed.
  - `Export`: Exports the buoy's state to a buffer.
  - `SetCluster`: Adds or removes the buoy from a cluster, managing model references.
  - `AddConsumer` and `ReleaseConsumer`: Manage consumer count for visibility control.
  - `GetBuoyType`: Returns the type of the buoy.

- **Data Members:**
  - `m_nConsumers`: Tracks the number of consumers interested in the buoy's visibility.
  - `m_buoyID`: Unique identifier for the buoy.
  - `m_type`: Type of the buoy (e.g., navigation, warning).
  - `m_visible`: Flag indicating whether the buoy is visible.

CONNECTS TO:
- `igc.h` and `modelIGC.h`, which define base interfaces and classes used by `CbuoyIGC`.

NOTES:
- None.

## `src/Igc/chaffigc.cpp`

PURPOSE: The file `chaffigc.cpp` contains the implementation of the `CchaffIGC` class, which represents chaff objects in a game mission. Chaff is used to confuse enemy missile guidance systems.

KEY PARTS:
- `CchaffIGC`: Class representing chaff objects.
- `Initialize`: Initializes the chaff object with data from the mission.
- `Terminate`: Terminates the chaff object when it expires or is no longer needed.
- `Update`: Updates the position and radius of the chaff object over time.

CONNECTS TO: `ImissionIGC`, `DataChaffIGC`, `DataChaffTypeIGC`, and other game systems for managing objects in a mission.

NOTES: None.

## `src/Igc/chaffigc.h`

**PURPOSE:** This file defines the header for the `CchaffIGC` class, which is a model representing chaff in the Allegiance game. Chaff is used to confuse enemy sensors.

**KEY PARTS:**
- **Class: CchaffIGC**
  - Inherits from `TmodelIGC<IchaffIGC>`
  - Manages initialization, termination, and updates for chaff objects.
  - Provides methods to set and get the cluster, visibility, and render state of the chaff.
  - Contains private data members for chaff type data and expiration time.

**CONNECTS TO:** 
- `igc.h` and `modelIGC.h`: These headers likely contain base classes or interfaces that `CchaffIGC` inherits from or interacts with.

**NOTES:**
- None.

## `src/Igc/chafftypeigc.cpp`

**PURPOSE:**  
This file contains the implementation of the `CchaffTypeIGC` class, which is responsible for managing chaff types in a mission within the Allegiance game. Chaff types are used to simulate electronic countermeasures that can confuse enemy radar.

**KEY PARTS:**
- **Class: CchaffTypeIGC**  
  Manages chaff types in a mission.
- **Function: Initialize**  
  Initializes the chaff type with data from a mission and adds it to the mission's expendable types.
- **Function: Export**  
  Exports the current state of the chaff type into a data structure.

**CONNECTS TO:**  
`ImissionIGC.h`, `DataChaffTypeIGC`

**NOTES:**  
None.

## `src/Igc/chafftypeigc.h`

PURPOSE: This file defines the CchaffTypeIGC class, which represents a chaff type in the Allegiance game. It implements interfaces for managing and exporting chaff type data.

KEY PARTS:
- CchaffTypeIGC: Class representing a chaff type.
  - Initializes with mission data.
  - Terminates by deleting itself from the mission's expendable types.
  - Updates and exports chaff type data.
  - Provides methods to get various properties of the chaff type, such as launcher definition, equipment type, mass, lifespan, etc.

CONNECTS TO: igc.h (interface definitions), ImissionIGC (mission management).

NOTES: None.

## `src/Igc/civilizationigc.cpp`

PURPOSE: This file contains the implementation of the CcivilizationIGC class, which manages civilization data in a mission.

KEY PARTS:
- Initialize: Initializes the civilization with mission data and sets up initial station and lifepod types.
- Terminate: Releases resources and cleans up when the civilization is terminated.
- Export: Exports the current state of the civilization to a data structure.

CONNECTS TO: ImissionIGC, StationType, HullType

NOTES: None

## `src/Igc/civilizationigc.h`

PURPOSE:
This file defines a class `CcivilizationIGC` that implements the `IcivilizationIGC` interface. It represents a civilization in the game and provides methods to initialize, terminate, export data, and retrieve various properties of the civilization.

KEY PARTS:
- Name: CcivilizationIGC
  - Implements the IcivilizationIGC interface.
  - Manages initialization, termination, and data export for a civilization.
- Initialize: Initializes the civilization with mission data.
- Terminate: Cleans up resources when the civilization is no longer needed.
- Export: Exports the current state of the civilization to a data buffer.
- GetObjectType: Returns the object type as OT_civilization.
- GetObjectID: Retrieves the unique identifier for the civilization.
- GetName: Returns the name of the civilization.
- GetIconName: Returns the icon name associated with the civilization.
- GetLifepod: Provides access to the lifepod (transport) for the civilization.
- GetBaseTechs: Returns a bitmask of base technologies available to the civilization.
- GetNoDevTechs: Returns a bitmask of technologies that cannot be developed by the civilization.
- GetBaseAttributes: Retrieves the base attributes of the civilization.
- GetInitialStationType: Provides access to the initial station type for the civilization.
- GetHUDName: Returns the HUD (heads-up display) name for the civilization.
- GetBonusMoney and GetIncomeMoney: Return financial bonuses and income rates specific to the civilization.

CONNECTS TO:
- ImissionIGC: The mission context in which the civilization operates.
- IhullTypeIGC: Represents the lifepod type associated with the civilization.
- IstationTypeIGC: Represents the initial station type for the civilization.

NOTES:
- None.

## `src/Igc/cloakigc.cpp`

PURPOSE: The file `cloakigc.cpp` contains the implementation of the `CcloakIGC` class, which manages the cloaking functionality for parts in a game mission.

KEY PARTS:
- **Class: CcloakIGC** - Manages the cloaking behavior of a part.
  - **Constructor**: Initializes member variables.
  - **Initialize**: Sets up the cloaking part with mission data.
  - **Terminate**: Cleans up resources when the part is no longer needed.
  - **Update**: Updates the cloaking state based on game time and ship conditions.
  - **SetShip**: Assigns a new ship to the cloaking part.
  - **SetMountID**: Mounts or unmounts the cloaking part on a ship.

CONNECTS TO: `ImissionIGC`, `IshipIGC`, `IIgcSite`

NOTES: None.

## `src/Igc/cloakigc.h`

PURPOSE: This file defines the CcloakIGC class, which implements the IcloakIGC interface for managing a cloaking device in the Allegiance game.

KEY PARTS:
- Name: CcloakIGC
  - What it does: Manages the behavior and state of a cloaking device on a ship.
- Name: Initialize
  - What it does: Initializes the cloaking device with mission data.
- Name: Terminate
  - What it does: Cleans up resources when the cloaking device is no longer needed.
- Name: Update
  - What it does: Updates the state of the cloaking device based on game time and other factors.
- Name: Activate
  - What it does: Activates the cloaking effect on the ship.
- Name: Deactivate
  - What it does: Deactivates the cloaking effect on the ship.
- Name: GetEnergyConsumption
  - What it does: Returns the energy consumption of the cloaking device.
- Name: GetMaxCloaking
  - What it does: Returns the maximum amount of cloaking that can be achieved.

CONNECTS TO:
- ImissionIGC: Manages mission-related data and operations.
- IpartTypeIGC: Provides type-specific information about the part (cloak).
- IshipIGC: Represents the ship on which the cloaking device is mounted.

NOTES:
- None.

## `src/Igc/clusterIGC.cpp`

**PURPOSE:** The `clusterIGC.cpp` file in Allegiance is responsible for managing clusters of models within the game universe. It handles initialization, updates, and cleanup of these clusters, as well as their interactions with other game elements.

**KEY PARTS:**
- **Class: CclusterIGC**: Manages a cluster of models and handles their interactions.
- **Function: Initialize**: Initializes the cluster with mission parameters and adds it to the mission.
- **Function: Terminate**: Cleans up resources used by the cluster, including models and collision structures.
- **Function: Update**: Updates the state of the cluster, handling ship movements, collisions, and other game events.
- **Function: Export**: Exports the current state of the cluster into a data structure.
- **Function: AddModel**: Adds a new model to the cluster, updating collision detection.
- **Function: DeleteModel(ImodelIGC* modelOld)**: Deletes a model from the cluster, updating collision sets and attributes.
- **Function: GetModel(const char* name) const**: Retrieves a model by its name.
- **Function: RecalculateCollisions(float tOffset, ImodelIGC* pModel1, ImodelIGC* pModel2)**: Recalculates collisions between models in the cluster.
- **Function: CreateBuildingEffect(Time now, IasteroidIGC* pasteroid, IstationIGC* pstation, IshipIGC* pshipBuilder, float radiusAsteroid, float radiusStation, const Vector& positionStart, const Vector& positionStop)**: Creates a building effect object within the cluster.
- **Data: m_models**: List of models in the cluster.
- **Data: m_modelsPickable, m_modelsCastRay**: Lists of pickable and ray-castable models respectively.
- **Data: m_kdrStatic, m_kdrMoving**: Data structures for handling static and moving hit tests.

**CONNECTS TO:** Other systems or files it depends on or serves:
- `ImissionIGC`: Mission object that the cluster is part of.
- `ImodelIGC`, `IasteroidIGC`, `IstationIGC`, `IshipIGC`: Various game model and entity interfaces used for interactions.

**NOTES:** The code includes assertions and retail asserts to ensure data integrity. There are commented-out sections from previous versions or debugging purposes, and the function `bSimpleEye` is referenced but not defined in this snippet.

## `src/Igc/clusterIGC.h`

**PURPOSE:** The file `clusterIGC.h` defines the class `CclusterIGC`, which represents a cluster in the game Allegiance. It manages various objects within the cluster such as ships, stations, asteroids, and treasures.

**KEY PARTS:**
- **Class: CclusterIGC** - Manages all objects within a cluster.
- **Functions:** Methods for adding, deleting, and retrieving objects like ships, stations, and treasures.
- **Data Members:** Lists of models, probes, warps, stations, treasures, asteroids, missiles, mines, and ships. Also includes collision detection data and explosion information.

**CONNECTS TO:** `igc.h`, which likely contains the base interface for game objects.

**NOTES:** None

## `src/Igc/common.cpp`

PURPOSE: common.cpp contains utility functions and classes used across various parts of Allegiance for calculations, object management, and game logic.

KEY PARTS:
- solveForImpact: Calculates time of impact between objects.
- FindTarget: Searches for targets based on criteria.
- WarpListIGC: Manages connections between clusters.
- IshipIGC: Represents ships in the game world.
- DamageTrack::ApplyDamage: Applies damage to models over time.
- CreateAsteroid: Generates asteroids at random positions within clusters.

CONNECTS TO: Other game modules, particularly those related to collision detection, ship movement, and mission scoring.

NOTES:
- Contains bitwise operations for managing object capabilities.
- Includes commented-out code from previous iterations of position handling logic.

PARTS (in file order):

- lines 1-353
  - solveForImpact: Calculates the time of impact between a projectile and a target.
  - solveForLead: Determines the lead angle needed for a shooter to hit a moving target.
  - turnToFace: Computes the control inputs required for a ship to turn towards a given target.
  - FindableModel: Checks if a model is within range, visible, and has the required abilities.
  - _matherr: Custom math error handler that skips certain exceptions.
  - c_pszWingName: Array of wing names used in the game.
  - c_cdAllCommands: Array of command data for various actions.
  - c_ttTypebits: Bitmask array indicating the type of each object.
- lines 355-699
  - IsFriendlyCluster: Determines if a cluster is friendly based on station presence and enemy activity.
  - ClusterPosition: Struct to hold a cluster and its position.
  - UniqueCP: Checks if a cluster position is unique in a list.
  - NewCP: Adds a new cluster position to the list.
  - FindTarget: Finds a target model within a cluster or adjacent clusters based on various criteria.
- lines 701-1050
  - FindTarget: Searches for a target model based on various criteria.
  - WarpListIGC: Represents a list of warps (connections between clusters).
  - IwarpIGC: Represents a warp (connection) between two clusters.
  - IclusterIGC: Represents a cluster in the game world.
  - IsideIGC: Represents a side or faction in the game.
  - IshipIGC: Represents a ship controlled by a player or AI.
  - ImodelIGC: Represents any model or object in the game.
  - FindPath: Finds a path from one cluster to another using A search algorithm.
- lines 1051-1398
  - GetModelType: Determines the type of model based on its object type.
  - GetModelName: Retrieves the name of a model, handling special cases for asteroids and buoys.
  - GetAmmoState: Calculates the current state of a ship's ammunition.
  - Waypoint::DoApproach: Handles the approach logic for a waypoint, determining the best position to move towards.
  - Waypoint::GetGotoPosition: Determines the next position for a ship to move towards based on its objective and target.
  - c_gpmKillThrottle: Mask indicating that the throttle should be killed.
  - c_gpmFinished: Mask indicating that the operation is finished.
  - c_gpmPivot: Mask indicating that the ship should pivot.
- lines 1399-1742
  - DoApproach: Handles the approach logic for different types of objects.
  - getDirection: Determines the button control mask to thrust in a given direction based on the ship's orientation and desired movement.
  - Ignore: Determines whether a ship should ignore another model (ship, mine, etc.) based on their sides, capabilities, and states.
  - Dodge: Handles dodging logic for ships, avoiding collisions with other models within a cluster.
  - The commented-out code in common.cpp suggests there were previous approaches to handling positions that have been removed or replaced.
  - The use of bitwise operations (&) and bitmasks (c_habmLifepod, c_habmRescue, etc.) for checking capabilities is common in Allegiance, indicating a structured approach to defining and managing object behaviors.
  - The function solveForImpact is used multiple times to calculate the time of impact between objects, which is crucial for collision detection and avoidance.
- lines 1745-2104
  - GotoPlan::Execute: Main function for executing a goto plan.
  - GotoPlan::SetControls: Sets controls based on current state and target.
  - assert (t >= 0.0f): Ensures time is non-negative.
  - if (t < tCollide): Checks if collision will occur before the current time.
  - pmodelCollide = pmodel;: Assigns colliding model to variable.
  - ca = dp.GetOrthogonalVector();: Finds orthogonal direction for dodging.
  - m_pship->SetStateBits(c_maneuverButtons, stateM);: Sets ship's state bits.
  - m_pship->SetControls(controls);: Applies controls to the ship.
- lines 2105-2452
  - LineOfSightExist: Determines if there is a line of sight between two models in a cluster.
  - CreateDrone: Creates a new drone ship with specified attributes.
  - pcontrols->jsValues[c_axisRoll]: Sets the roll axis value based on distance and direction.
  - pcontrols->jsValues[c_axisThrottle]: Adjusts throttle based on proximity to obstacles and target.
  - LineOfSightExist uses trigonometry to calculate angles and distances for visibility checks.
  - CreateDrone initializes a new ship data structure with unique name if provided.
  - TODO: Debugging statements commented out, possibly for future use.
- lines 2454-2808
  - IshipIGC CreateObject(...): Creates a new ship object.
  - ClusterWarning GetClusterWarning(AssetMask am, bool bInvulnerableStations): Determines the warning level for assets in a cluster.
  - const char GetClusterWarningText(ClusterWarning cw): Returns the text description of a cluster warning.
  - DamageTrack::DamageTrack(DamageTrackSet pdts): Constructor for DamageTrack class.
  - void ApplyDamage(Time timeNow, ImodelIGC pmodel, float damage): Applies damage to a model in the track.
  - PlayerScoreObject::CalculateScore(ImissionIGC pmission): Calculates the player's score based on mission data.
  - float GetDistance(IshipIGC pship, IclusterIGC pcluster, IclusterIGC pclusterStop, int maxDistance): Calculates the distance between clusters using warps.
  - static void GetAsteroidName(const char pszPrefix, AsteroidID id, char bfr): Generates a name for an asteroid based on its ID.
- lines 2810-3109
  - Vector: Represents a 3D vector with x, y, and z components.
  - ModelLinkIGC: A link in a linked list of models.
  - ImodelIGC: Interface for model objects.
  - IasteroidIGC: Interface for asteroid objects.
  - DataAsteroidIGC: Structure containing data for creating an asteroid.
  - CreateAsteroid: Function to create an asteroid at a random position within a cluster.
  - RandomPosition: Function to generate a random position within a cluster, avoiding other models.
  - IstationIGC: Interface for station objects.

## `src/Igc/developmentigc.cpp`

PURPOSE:
The file `developmentigc.cpp` contains the implementation of the `CdevelopmentIGC` class, which is responsible for managing development-related data in a mission within the Allegiance game. It handles initialization, termination, and exporting of development data.

KEY PARTS:
- `Initialize`: Initializes the development object with mission data.
- `Terminate`: Removes the development object from the mission.
- `Export`: Exports the current state of the development data to a buffer.

CONNECTS TO:
- `ImissionIGC`: The mission interface that the development object interacts with.

NOTES:
- None.

## `src/Igc/developmentigc.h`

PURPOSE:
The file `developmentigc.h` defines the header for the `CdevelopmentIGC` class, which implements the `IdevelopmentIGC` interface. This class is responsible for managing development-related items in the game Allegiance.

KEY PARTS:
- **Class: CdevelopmentIGC**
  - Inherits from `IdevelopmentIGC`.
  - Manages initialization, termination, and data export for development objects.
  
- **Functions:**
  - `Initialize`: Initializes the object with mission data.
  - `Terminate`: Cleans up resources when the object is no longer needed.
  - `Export`: Exports the object's data to a buffer.
  - `GetObjectType`, `GetObjectID`: Returns the type and ID of the object.
  - `GetData`: Provides access to the object's data structure.
  - `GetModelName`, `GetIconName`, etc.: Retrieve various properties of the development item (model, icon, price, etc.).
  - `IsObsolete`: Determines if the development item is obsolete based on technology requirements.

- **Data Members:**
  - `m_pMission`: Pointer to the mission object.
  - `m_data`: Instance of `DataDevelopmentIGC`, containing specific data for the development item.
  - `m_techOnly`: Flag indicating whether the item can only be developed through technology.

CONNECTS TO:
- `igc.h`: The base interface header file that defines `IdevelopmentIGC`.
- Other game systems and files related to mission management, object types, and technology trees.

NOTES:
- None.

## `src/Igc/dispenserigc.cpp`

**PURPOSE:**  
The file `dispenserIGC.cpp` implements the `CdispenserIGC` class, which manages a dispenser part in the Allegiance game. This class handles initialization, termination, updating, and managing the state of a dispenser part within a ship.

**KEY PARTS:**
- **Class: CdispenserIGC**  
  Manages a dispenser part on a ship.
- **Function: Initialize**  
  Initializes the dispenser with mission data.
- **Function: Terminate**  
  Cleans up resources when the dispenser is no longer needed.
- **Function: Update**  
  Updates the state of the dispenser, including firing expendables.
- **Function: SetShip**  
  Sets the ship that the dispenser part belongs to.
- **Function: SetMountID**  
  Manages the mounting and unmounting of the dispenser part on a ship.

**CONNECTS TO:**  
`dispenserigc.h`, `ImissionIGC`, `IlauncherTypeIGC`, `IexpendableTypeIGC`, `IIgcSite`, `IshipIGC`.

**NOTES:**  
- The code includes comments and assertions for debugging and validation.
- There is a commented-out line that was intended to prevent crashes but was disabled (`m_expendableType->AddRef();`).
- The file uses ATL (Active Template Library) for object management.

## `src/Igc/dispenserigc.h`

PURPOSE: This file defines the CdispenserIGC class, which implements the IdispenserIGC interface for managing dispensers in a game mission.

KEY PARTS:
- Name: CdispenserIGC
  - What it does: Manages the behavior and properties of dispensers in a game mission, including initialization, updating, activation, and deactivation.
  
- Name: Initialize
  - What it does: Initializes the dispenser with mission data.
  
- Name: Terminate
  - What it does: Cleans up resources when the dispenser is terminated.
  
- Name: Update
  - What it does: Updates the state of the dispenser based on the current game time.
  
- Name: Activate
  - What it does: Activates the dispenser, changing its signature and setting a loading time.
  
- Name: Deactivate
  - What it does: Deactivates the dispenser by changing its signature.
  
- Name: SetAmount
  - What it does: Sets the amount of expendable items in the dispenser, ensuring it does not exceed the maximum allowed.
  
- Name: GetExpendableType
  - What it does: Returns the type of expendable item managed by the dispenser.

CONNECTS TO:
- ImissionIGC: The mission context in which the dispenser operates.
- IlauncherTypeIGC and IexpendableTypeIGC: Types of items that can be dispensed.
- IshipIGC: The ship to which the dispenser is attached.

NOTES: None.

## `src/Igc/dronetypeigc.cpp`

PURPOSE:
The `droneTypeIGC.cpp` file implements the `CdroneTypeIGC` class, which manages drone types in a mission for the Allegiance game. It handles initialization, termination, and exporting of drone type data.

KEY PARTS:
- `Initialize`: Initializes the drone type with mission context and data.
- `Terminate`: Cleans up resources when the drone type is no longer needed.
- `Export`: Exports the current state of the drone type to a data structure.

CONNECTS TO:
- `ImissionIGC`: The mission interface that provides context for the drone type.

NOTES:
- Contains assertions and debug code that may need removal or adjustment in non-debug builds.

## `src/Igc/dronetypeigc.h`

PURPOSE: This file defines a class `CdroneTypeIGC` that implements the interface `IdroneTypeIGC`, representing a drone type in the game Allegiance. It handles initialization, termination, and data export for drone types.

KEY PARTS:
- Class: `CdroneTypeIGC`
  - Inherits from: `IdroneTypeIGC`
  - Implements methods to initialize, terminate, and export drone type data.
  - Provides getters for various properties of the drone type such as model name, icon, price, build time, and combat skills.

- Methods:
  - `Initialize`: Initializes the drone type with mission context and data.
  - `Terminate`: Cleans up resources when the drone type is no longer needed.
  - `Export`: Exports the drone type data to a buffer.
  - Getters for properties like model name, icon name, price, build time, hull type ID, pilot type, shoot skill, move skill, bravery, and laid expendable.

- Data Members:
  - `m_pMission`: Pointer to the mission context.
  - `m_data`: Structure containing drone type data.
  - `m_petLaid`: Reference to the laid expendable type.

CONNECTS TO: 
- `ImissionIGC` (mission context)
- `DataDroneTypeIGC` (data structure for drone type information)

NOTES:
- None.

## `src/Igc/hullTypeIGC.cpp`

PURPOSE: The file `hullTypeIGC.cpp` implements the `ChullTypeIGC` class, which is responsible for managing and initializing hull types in a game mission.

KEY PARTS:
- `Initialize`: Initializes the hull type with data from a mission, setting up parts, weapons, and other properties.
- `Terminate`: Cleans up resources used by the hull type when it's no longer needed.
- `Export`: Exports the hull type data to a buffer.

CONNECTS TO: `ImissionIGC`, `DataHullTypeIGC`, `IpartTypeIGC`, `IIgcSite`, and other game systems for managing mission data, parts, and resources.

NOTES:
- Contains assertions and debug logging for development purposes.
- Some assertions are commented out or modified to prevent crashes in certain core versions.

## `src/Igc/hullTypeIGC.h`

**PURPOSE:** The file `hullTypeIGC.h` defines the class `ChullTypeIGC`, which implements the interface `IhullTypeIGC`. This class represents a hull type in the game Allegiance and contains various properties and methods related to that hull type, such as its physical characteristics, capabilities, and buyable attributes.

**KEY PARTS:**
- **Class:** `ChullTypeIGC`
  - Inherits from `IhullTypeIGC` and implements several interfaces including `IbaseIGC`, `ItypeIGC`, `IbuyableIGC`, and others.
  - Contains member variables to store data about the hull type, such as mass, signature, length, speed, etc.
  - Implements methods to get various properties of the hull type, like its name, description, price, required technologies, and physical attributes.

- **Methods:**
  - `Initialize`: Initializes the hull type with mission data.
  - `Terminate`: Cleans up resources when the hull type is no longer needed.
  - `Export`: Exports the hull type data to a buffer.
  - `GetData`: Returns the raw data associated with the hull type.
  - Various getters for specific properties like mass, signature, length, max speed, etc.

- **Data Members:**
  - `m_pMission`: Pointer to the mission object.
  - `m_phtSuccessor`: Reference to the successor hull type.
  - `m_poIcon`: Reference to an icon object associated with the hull type.
  - `m_data`: Pointer to the data structure containing the hull type's properties.
  - Various vectors and orientations for weapon positions, launch positions, etc.

**CONNECTS TO:**
- Depends on other classes/interfaces like `IhullTypeIGC`, `ImissionIGC`, `IpartTypeIGC`, `IObject`, etc., as well as data structures like `DataHullTypeIGC` and `PartTypeListIGC`.

**NOTES:** None.

## `src/Igc/igc.def`

PURPOSE: tiny file (7 lines), shown in full rather than summarized.

    ; igc.def : Declares the module parameters.
    LIBRARY      "igc.DLL"
    EXPORTS
    	DllCanUnloadNow     @1 PRIVATE
    	DllGetClassObject   @2 PRIVATE
    	DllRegisterServer   @3 PRIVATE
    	DllUnregisterServer	@4 PRIVATE

## `src/Igc/igc.h`

PURPOSE: The file `igc.h` contains definitions and interfaces for various game objects, data structures, and functions used in the Allegiance game engine.

KEY PARTS:
- Geo: Base class for geometric objects.
- ThingGeo: Represents a thing with a geometry.
- IclusterIGC: Manages a cluster of objects in the game.
- DataHullTypeIGC: Represents a ship hull type with various attributes like mass, speed, and weapon mounts.
- ShipLoadout: Represents a ship's loadout with hull and part data.
- ShipStatus: Manages the state and properties of a ship.
- DamageTrackSet: Manages damage tracks for objects.
- PersistPlayerScoreObject: Manages persistent player score data.

CONNECTS TO: Other game engine files, particularly those related to mission management, player scores, and geometric calculations.

NOTES:
- Contains numerous enums and constants defining various attributes and identifiers used throughout the game.
- Includes interfaces for different types of game objects (e.g., ships, stations, asteroids).
- Features functions for managing game data persistence and score calculation.

PARTS (in file order):

- lines 1-340
  - Geo: Base class for geometric objects.
  - ThingGeo: Represents a thing with a geometry.
  - ThingGeoSite: Represents a site where things can be placed.
  - GroupGeo: Represents a group of geometric objects.
  - BuildEffectGeo: Represents a build effect with geometry.
  - PosterImage: Represents an image that can be posted.
  - DamageTrackSet: Manages damage tracks for objects.
  - DamageTrack: Represents a single damage track.
- lines 341-702
  - TrekKey: Enumerates various game keys and their corresponding values.
  - SoundID and VoiceID: Types for sound and voice identifiers.
  - enum { ... }: Defines a list of sounds using macros.
  - NA, fNA: Constants representing "not applicable" or "not available".
  - MissionID, ObjectID, etc.: Various types representing unique IDs for different game entities.
  - ModelAttributes: Enumerates attributes that can be applied to game objects.
  - EquipmentType and AbilityType: Enums defining types of equipment and abilities.
  - DamageTypeID and DefenseTypeID: Enums for damage and defense types.
- lines 704-1058
  - PartBitMask: Represents a bitmask for parts of a ship.
  - HullAbilityBitMask: Bitmask for abilities specific to hulls (ships).
  - StationAbilityBitMask: Bitmask for abilities specific to stations.
  - AsteroidAbilityBitMask: Bitmask for abilities specific to asteroids.
  - ExpendableAbilityBitMask: Bitmask for expendable abilities that can be used by ships or other entities.
  - WingmanBehaviourBitMask: Bitmask defining behaviors for wingmen in a flight formation.
  - AchievementMask: Bitmask representing different achievements a player can earn.
  - ShipControlStateIGC: Enumerates the states of ship controls and commands.
- lines 1060-1402
  - IclusterIGC: Represents a cluster of objects in the game.
  - IasteroidIGC: Manages asteroid-related data and behavior.
  - IwarpIGC: Handles warp points within the game.
  - ItreasureIGC: Manages treasure objects and their properties.
  - IsideIGC: Represents a side or team in the game.
  - IcivilizationIGC: Manages civilization-specific data and behaviors.
  - IstationTypeIGC: Defines different types of stations available in the game.
  - IdroneTypeIGC: Handles drone-related data and behavior.
- lines 1403-1755
  - Class ImapMakerIGC: Manages game map creation and validation.
  - Function IsValid(): Validates various game parameters to ensure they meet specified criteria.
  - Struct ControlData: Holds joystick values for player control.
  - Struct PartData: Represents a part with an ID, amount, and mount location.
  - Struct ExpandedPartData: Extends PartData by adding the fraction of the part that is mounted.
  - Struct Stockpile: Stores buyable items and their counts.
  - Struct HardpointData: Contains information about a ship's hardpoints including sounds and locations.
  - Struct AsteroidDef: Describes an asteroid with properties like ore content, hit points, and model name.
- lines 1756-2108
  - DataHullTypeIGC: Represents a ship hull type with various attributes like mass, speed, and weapon mounts.
  - DataPartTypeIGC: Base class for different types of parts (e.g., weapons, shields) with common properties like mass and signature.
  - DataWeaponTypeIGC: Inherits from DataPartTypeIGC and adds specific attributes for weapons like ready time and dispersion.
  - DataShieldTypeIGC: Inherits from DataPartTypeIGC and includes shield-specific properties like regeneration rate.
  - DataCloakTypeIGC: Inherits from DataPartTypeIGC and contains properties related to cloaking technology.
  - DataAfterburnerTypeIGC: Inherits from DataPartTypeIGC and specifies afterburner-related attributes.
  - DataPackTypeIGC: Represents a pack of items with type and quantity.
  - DataLauncherTypeIGC: Describes a launcher that can fire parts or projectiles.
- lines 2109-2462
  - ShipLoadout: Represents a ship's loadout with hull and part data.
  - CompactOrientation: Stores orientation data efficiently using quaternions.
  - CompactTime: Compresses time differences into 2 bytes.
  - CompactPosition: Compresses position deltas relative to a reference point.
  - CompactState: Stores state information in 2 bytes.
  - CompactVelocity: Compresses velocity data into 5 bytes.
  - CompactTurnRates: Compresses turn rates into 3 bytes.
  - CompactControlData: Stores control data from joysticks or other inputs.
- lines 2463-2804
  - CompactState: Represents compact state data.
  - ClientActiveTurretUpdate: Holds client-side active turret update information.
  - ServerActiveTurretUpdate: Holds server-side active turret update information.
  - DataShipIGC: Stores information about a ship in the game.
  - DataWarpIGC: Contains warp-related data for a ship.
  - DataTreasureIGC: Represents treasure data, including position and lifespan.
  - DataMunitionExport: Defines export parameters for munitions.
  - DataMineBase: Base class for mine data.
- lines 2805-3186
  - ImissionIGC: Manages mission data and objects.
  - Export: Exports mission data to a buffer.
  - Import: Imports mission data from a buffer.
  - BitsToTechsList: Converts technology bits to a list of technologies.
  - TechsListToBits: Converts a list of technologies to technology bits.
  - GenerateNewShipID: Generates a new unique ship ID.
  - SetMissionStage: Sets the current mission stage.
  - GetMissionStage: Gets the current mission stage.
- lines 3187-3586
  - IshipIGC: Interface for ship objects.
  - ShipUpdateStatus: Enumerates possible statuses of a ship update.
  - SetObjectID: Sets the object ID of the ship.
  - GetValue: Returns the value of the ship.
  - GetStation: Gets the station associated with the ship.
  - Reset: Resets the ship, optionally fully.
  - AddPart: Adds a part to the ship.
  - DeletePart: Deletes a part from the ship.
- lines 3587-3962
  - IbuyableIGC: Base interface for buyable game objects.
  - IdevelopmentIGC: Interface for development items with global attributes and tech-only status.
  - IdroneTypeIGC: Interface for drone types with hull type, pilot type, skills, and laid expendables.
  - IstationTypeIGC: Interface for station types with various stats and capabilities.
  - IbucketIGC: Interface for buckets that can hold buyable items.
  - IprojectileTypeIGC: Interface for projectile types with power, blast radius, speed, and model details.
  - IexpendableTypeIGC: Base interface for expendable items like missiles, mines, and chaff.
  - ImissileTypeIGC: Interface for missile types with specific attributes like acceleration and lock time.
- lines 3963-4343
  - IpartIGC: Base interface for parts of a ship or structure.
  - IafterburnerIGC: Interface for afterburners, providing fuel consumption and power control.
  - IpackIGC: Interface for packs, likely related to cargo or equipment.
  - IlauncherIGC: Interface for launchers, including emission points and firing times.
  - ImagazineIGC: Interface for magazines, extending launcher with missile type and lock status.
  - IdispenserIGC: Interface for dispensers, extending launcher with expendable type.
  - IclusterIGC: Interface for clusters, managing various game objects like models, warps, mines, etc.
  - IasteroidIGC: Interface for asteroids, providing ore management and special abilities.
- lines 4344-4698
  - ShipStatus: Manages the state and properties of a ship.
  - ClusterSite: Represents a site within a cluster, with methods for managing posters and scanners.
  - solveForImpact: Calculates the impact point between two objects.
  - solveForLead: Determines the lead position of a target relative to a shooter.
  - turnToFace: Rotates a ship towards a target.
  - FindPath: Finds a path from one object to another.
  - GetModelType: Returns the type of a model.
  - GetModelName: Returns the name of a model.
- lines 4699-5059
  - IGC.h: Header file for Allegiance game engine.
  - IIgcSite: Interface class defining callbacks for various game events.
  - DamageTrack: Class to track and manage damage over time.
  - DamageBucket: Class representing a bucket of damage over a specific slot.
  - c_dwFlashInterval: Constant for the duration of a flash effect when taking damage.
  - c_dwDamageSlotInterval: Constant for the interval between switching damage slots.
  - c_iDamageSlotCount: Constant for the number of damage slots to track.
  - TODO: "Imago #120 #121 8/10" - Indicates potential issues or improvements related to asteroid events.
- lines 5060-5418
  - DamageTrack: Manages damage tracking for an object over time.
  - DamageTrackSet: Maintains a set of DamageTrack objects and manages their lifecycle.
  - CreateMission: Function to create a mission instance.
  - AmmoState: Enumerates the states of ammunition (empty, low, full).
  - GetAmmoState: Determines the current state of ammunition for a ship.
  - AddIbaseIGC: Adds an IbaseIGC object to a list and increments its reference count.
  - DeleteIbaseIGC: Removes an IbaseIGC object from a list and decrements its reference count.
  - GetIbaseIGC: Retrieves an IbaseIGC object from a list by ID.
- lines 5421-5773
  - PersistPlayerScoreObject: Manages persistent player score data.
  - PlayerScoreObject: Tracks and manages individual player scores during a game session.
  - CalculateScore: Calculates the player's score based on mission outcomes.
  - SetScore: Sets the player's score directly.
  - Reset: Resets all score-related statistics to default values.
  - Connect/Disconnect: Manages connection status and time played.
  - StartCommand/StopCommand: Tracks when a player starts or stops commanding.
  - EndGame: Calculates final scores at game end based on mission results.
- lines 5774-6188
  - PlayerScoreObject: Manages player scores and statistics.
  - GameOverScoreObject: Stores recent game statistics for display.
  - DumpIGCFile: Saves mission data to an IGC file.
  - LoadIGCFile: Loads mission data from an IGC file.
  - DumpIGCStaticCore: Saves static core mission data to a file.
  - LoadIGCStaticCore: Loads static core mission data from a file.
  - CacheIGCStaticCore: Caches static core mission data for faster access.
  - TODOs: Comment out unused variables/functions. Remove commented-out code. Consider optimizing combat rating calculations.

## `src/Igc/launchertypeigc.cpp`

**PURPOSE:**  
This file contains the implementation of the `ClauncherTypeIGC` class, which is responsible for managing launcher types in a mission within the Allegiance game. The class handles initialization, termination, and exporting data related to launcher types.

**KEY PARTS:**
- **Class: ClauncherTypeIGC** - Manages launcher types in a mission.
  - **Function: Initialize** - Initializes the launcher type with mission data.
  - **Function: Terminate** - Cleans up resources when the launcher type is no longer needed.
  - **Function: Export** - Exports the current state of the launcher type to a data structure.

- **Data Structure: DataLauncherTypeIGC** - Holds specific data related to launcher types.

**CONNECTS TO:**  
`ImissionIGC`, `ExpendableTypeIGC`, and other mission-related classes for managing parts and equipment in the game.

**NOTES:**  
- A conditional assertion has been added to handle a specific case where the successor part ID is 238, which corresponds to an assault expansion. This is a temporary workaround until the issue is fixed by Noir.
- The class uses assertions for debugging purposes to ensure that certain conditions are met during initialization and termination.

## `src/Igc/launchertypeigc.h`

**PURPOSE:** This file contains the declaration of the `ClauncherTypeIGC` class, which implements the `IlauncherTypeIGC` interface for a launcher type in the Allegiance game. It handles initialization, data retrieval, and other operations related to launcher types.

**KEY PARTS:**
- **Class: ClauncherTypeIGC**
  - Inherits from `IlauncherTypeIGC`.
  - Manages initialization, termination, and export of launcher type data.
  - Provides methods for retrieving various attributes such as model name, icon, price, and build time.
  - Implements specific methods for launcher types, including getting expendable type and launch count.

- **Methods:**
  - `Initialize`: Initializes the launcher type with mission data.
  - `Terminate`: Cleans up resources when the launcher type is terminated.
  - `Export`: Exports the launcher type data to a buffer.
  - `GetData`: Returns the launcher definition data.
  - `GetModelName`, `GetIconName`, etc.: Retrieve various attributes of the launcher type.

- **Data Members:**
  - `m_pMission`: Pointer to the mission object.
  - `m_pptSuccessor`: Reference to the successor part type.
  - `m_pexpendabletype`: Pointer to the expendable type associated with this launcher.
  - `m_data`: Structure containing data specific to the launcher type.
  - `m_launcherDef`: Pointer to the launcher definition.
  - `m_equipmenttype`: Type of equipment represented by the launcher.

**CONNECTS TO:** 
- Depends on `IlauncherTypeIGC`, `ImissionIGC`, and other related interfaces and classes within the Allegiance game engine.

**NOTES:** None.

## `src/Igc/magazineigc.cpp`

PURPOSE:
The `magazineIGC.cpp` file contains the implementation of the `CmagazineIGC` class, which manages a magazine for missiles in the Allegiance game. It handles initialization, termination, updates, and interactions with ships and missile types.

KEY PARTS:
- `CmagazineIGC`: Manages a magazine for missiles.
- `Initialize`: Initializes the magazine with mission data.
- `Terminate`: Cleans up resources when the magazine is no longer needed.
- `Update`: Updates the state of the magazine, including loading and firing missiles.
- `SetShip`: Sets the ship associated with the magazine.
- `SetMountID`: Sets the mount point for the magazine on the ship.

CONNECTS TO:
- `ImissionIGC`: The mission object that provides game rules and constants.
- `IlauncherTypeIGC` and `ImissileTypeIGC`: Classes representing missile types and launcher types.
- `IIgcSite`: Manages game sounds and notifications.
- `IshipIGC`: Represents the ship to which the magazine is attached.

NOTES:
- The class uses reference counting (`AddRef` and `Release`) to manage resources.
- It includes assertions for debugging purposes, ensuring that objects are not null before accessing them.

## `src/Igc/magazineigc.h`

PURPOSE: This file defines the CmagazineIGC class, which represents a magazine in the Allegiance game. It handles initialization, updates, and operations related to magazines.

KEY PARTS:
- CmagazineIGC: Main class representing a magazine.
- Initialize: Initializes the magazine with mission data.
- Terminate: Cleans up resources when the magazine is no longer needed.
- Update: Updates the state of the magazine over time.
- GetObjectType: Returns the object type as OT_magazine.
- SetShip: Sets the ship to which the magazine is attached and updates its mount ID.
- Activate: Activates the magazine, setting a loading time based on mission constants and missile load times.
- Deactivate: Deactivates the magazine by changing the ship's signature.
- GetMass: Calculates the total mass of the magazine based on the number of missiles and their individual masses.
- GetPrice: Calculates the price of the magazine based on the number of missiles and their launcher definition prices.
- SetAmount: Adjusts the amount of missiles in the magazine, ensuring it does not exceed the maximum allowed by the ship's part type.
- GetTimeLoaded: Returns the time when the magazine is fully loaded.
- ResetTimeLoaded: Resets the loading time to the current time plus a calculated load duration.
- GetEmissionPt: Returns the emission point of the magazine.
- GetArmedFraction: Calculates the fraction of the magazine that is armed based on the time since it was last loaded.

CONNECTS TO:
- ImissionIGC: For mission-related data and operations.
- IlauncherTypeIGC: For launcher type-specific properties.
- ImissileTypeIGC: For missile type-specific properties.
- IshipIGC: For ship-related data and operations.

NOTES: None.

## `src/Igc/mapmakerigc.cpp`

PURPOSE: The file `src/Igc/mapmakerigc.cpp` contains classes and functions for generating various types of maps in the Allegiance game, including double ring, big ring, high-level, star-shaped, pinwheel, and diamond ring layouts.

KEY PARTS:
- CMapData: Manages map data including clusters and mission parameters.
- ImapMakerIGC: Base class for creating different types of maps.
- IsValid: Validates mission parameters based on map type.
- Create: Creates a mission based on the given parameters.
- FindPath: Recursively finds a path between two clusters.
- EnoughPaths: Checks if there are enough paths between clusters.
- OkToRemove: Determines if it's safe to remove a warp link.
- GenerateMission: Generates a mission by setting up map data and creating clusters.

CONNECTS TO: None

NOTES:
- Contains multiple classes for different map layouts such as `CmapMakerDoubleRingIGC`, `CmapMakerBigRingIGC`, etc.
- Includes functions for generating cluster positions, linking clusters, and validating mission parameters.

PARTS (in file order):

- lines 1-350
  - CMapData: Manages map data including clusters and mission parameters.
  - ImapMakerIGC: Base class for creating different types of maps.
  - IsValid: Validates mission parameters based on map type.
  - Create: Creates a mission based on the given parameters.
  - FindPath: Recursively finds a path between two clusters.
  - EnoughPaths: Checks if there are enough paths between clusters.
  - OkToRemove: Determines if it's safe to remove a warp link.
  - GenerateMission: Generates a mission by setting up map data and creating clusters.
- lines 352-725
  - CmapMakerIGC::GenerateLayout: Generates the sector layout for the map.
  - GenerateSides: Initializes sides with predefined colors and names.
  - GenerateTeamClusterScreenPosition: Calculates screen position for team clusters.
  - GenerateTeamCluster: Creates a new team cluster object.
  - GenerateNeutralClusterScreenPosition: Calculates screen position for neutral clusters.
  - GenerateNeutralCluster: Creates a new neutral cluster object.
  - GenerateRingClusters: Generates clusters in a ring pattern.
  - GenerateWarp: Creates a warp point between two clusters.
- lines 726-1085
  - CmapMakerIGC: Main class for generating IGC maps.
  - CrossLinkClusters: Links clusters between teams.
  - LinkClusters: Creates links within and between teams.
  - GenerateLayout: Sets up the initial layout of clusters.
  - GenerateStarbase: Generates a starbase at a cluster.
  - PopulateCluster: Populates a cluster with resources.
  - PopulateClusters: Populates all clusters in the map.
  - RevealHomeClusters: Reveals home clusters to their respective sides.
- lines 1086-1439
  - CmapMakerDoubleRingIGC: Handles the generation and linking of clusters for a double ring map layout.
  - GenerateLayout: Sets up team and neutral cluster counts and generates the ring clusters.
  - GenerateTeamClusterScreenPosition: Calculates screen positions for team clusters in a circular pattern.
  - GenerateNeutralClusterScreenPosition: Calculates screen positions for neutral clusters in a circular pattern.
  - LinkClusters: Links sectors within teams and forms rings around the map.
  - CmapMakerBigRingIGC: Handles the generation and linking of clusters for a big ring map layout.
  - GenerateLayout: Sets up team, neutral, and other cluster counts and generates the ring clusters.
  - GenerateTeamClusterScreenPosition: Calculates screen positions for team clusters in a circular pattern.
- lines 1440-1804
  - CmapMakerHiHigherIGC: Handles high-level map generation for a specific layout.
  - LinkClusters: Links clusters together based on the current map configuration.
  - GenerateLayout: Sets up the initial layout parameters and generates clusters.
  - GenerateTeamClusterScreenPosition: Determines the screen position of team clusters.
  - GenerateNeutralClusterScreenPosition: Determines the screen position of neutral clusters.
  - CmapMakerStarIGC: Handles map generation for a star-shaped layout.
  - IsValid: Checks if the mission parameters are valid for this map type.
  - PopulateCluster: Populates a cluster with stations and asteroids.
- lines 1805-2166
  - CmapMakerPinWheelIGC: Class for generating a pinwheel-shaped map layout.
  - IsValid: Function to validate mission parameters for pinwheel maps.
  - GenerateLayout: Function to generate the layout of clusters for pinwheel maps.
  - GenerateNeutralClusterScreenPosition: Function to calculate screen positions for neutral clusters in pinwheel maps.
  - LinkClusters: Function to link clusters together in a pinwheel map layout.
  - CmapMakerDiamondRingIGC: Class for generating a diamond ring-shaped map layout.
  - IsValid: Function to validate mission parameters for diamond ring maps.
  - GenerateLayout: Function to generate the layout of clusters for diamond ring maps.
- lines 2167-2625
  - CmapMakerIGC::LinkClusters: Links clusters on a map.
  - CmapMakerLargeSplitIGC::IsValid: Validates parameters for large split maps.
  - CmapMakerInsideOutIGC::IsValid: Validates parameters for inside-out maps.
  - CmapMakerGridIGC::GenerateLayout: Generates layout for grid maps.
  - CmapMakerEastWestIGC::IsValid: Validates parameters for east-west maps.
  - CmapMakerSplitBaseIGC::IsValid: Validates parameters for split base maps.
  - TODOs: Remove commented out code related to player counts and team validation. Consider refactoring repetitive cluster linking logic into a helper function.

## `src/Igc/mapmakerigc.h`

PURPOSE: This file contains declarations and definitions for various map generation classes in the Allegiance game. It includes constants, data structures, and methods to create different types of maps based on mission parameters.

KEY PARTS:
- CMapData: Holds all information needed to build a map.
- CmapMakerIGC: Base class for the map maker with virtual functions for generating various aspects of the map.
- Derived classes (e.g., CmapMakerSingleRingIGC, CmapMakerDoubleRingIGC): Implement specific map generation layouts.

CONNECTS TO: Igc.h, which likely contains interfaces and base classes used by these map generation classes.

NOTES: None.

## `src/Igc/mineigc.cpp`

PURPOSE: This file contains the implementation of the CmineIGC class, which represents a mine in the Allegiance game. It handles initialization, termination, and updates for mines.

KEY PARTS:
- CmineIGC: Manages the lifecycle and properties of a mine.
- Initialize: Sets up the mine with data from a mission file.
- Terminate: Removes the mine from the mission when it is no longer needed.
- Update: Handles the mine's behavior over time, including expiration.
- Export: Prepares the mine's state for export to other systems.

CONNECTS TO: ImissionIGC (mission management), IshipIGC (ships), IclusterIGC (clusters), and various data structures like DataMineBase and DataMineExport.

NOTES: None.

## `src/Igc/mineigc.h`

PURPOSE: The file defines the CmineIGC class, which represents a mine in the game Allegiance. It handles initialization, collision detection, and damage calculations for mines.

KEY PARTS:
- CmineIGC: Represents a mine object with methods to initialize, update, handle collisions, and manage its strength.
- Initialize: Sets up the mine's initial state based on mission data.
- HandleCollision: Calculates damage when a ship collides with the mine.
- GetMineType: Returns the type of mine.
- GetLauncher: Returns the ship that launched the mine.
- SetCreateNow: Marks the mine to be created immediately.
- ReduceStrength: Reduces the mine's strength based on damage taken.

CONNECTS TO:
- ImineIGC, ImodelIGC, ImodelIGC, IshipIGC, ImineTypeIGC, IclusterIGC, ImissionIGC

NOTES:
- None.

## `src/Igc/minetypeigc.cpp`

PURPOSE: This file contains the implementation of the CmineTypeIGC class, which is responsible for managing mine types in a mission.

KEY PARTS:
- CmineTypeIGC: Manages the initialization and export of mine type data.
- Initialize: Sets up the mine type with mission context and data.
- Export: Serializes the mine type data into a buffer.

CONNECTS TO: ImissionIGC (the mission interface).

NOTES: None.

## `src/Igc/minetypeigc.h`

PURPOSE: This file defines the CmineTypeIGC class, which represents a mine type in the Allegiance game. It implements interfaces for managing and exporting mine data.

KEY PARTS:
- Name: CmineTypeIGC
  - What it does: Manages and exports data for a specific mine type in the game.

- Initialize: Initializes the mine type with mission data.
- Terminate: Cleans up resources when the mine type is no longer needed.
- Update: Updates the state of the mine type.
- Export: Exports the mine type's data to a buffer.
- GetObjectType, GetObjectID: Returns the object type and ID of the mine type.
- GetData: Retrieves the raw data associated with the mine type.
- GetLauncherDef: Provides launcher definition details for the mine type.
- GetEquipmentType: Specifies the equipment type (dispenser).
- GetLoadTime, GetMass, GetLifespan, GetSignature, GetHitPoints, GetDefenseType: Returns various attributes of the mine type.
- GetName, GetModelName, GetModelTexture, GetRadius: Provides information about the model and appearance of the mine type.
- GetCapabilities, HasCapability: Checks for specific capabilities of the mine type.
- GetPower, GetEndurance, GetDamageType: Returns power, endurance, and damage type details.

CONNECTS TO:
- ImineTypeIGC interface
- ImissionIGC class

NOTES:
- None

## `src/Igc/missileigc.cpp`

PURPOSE: This file contains the implementation of the `CmissileIGC` class, which represents a missile in the Allegiance game. It handles initialization, updates, collisions, and explosions for missiles.

KEY PARTS:
- `CmissileIGC`: Manages the state and behavior of a missile.
- `Initialize`: Sets up the missile with initial data.
- `Terminate`: Cleans up resources when the missile is terminated.
- `Update`: Updates the missile's position, orientation, and checks for collisions or expiration.
- `HandleCollision`: Processes collisions between the missile and other objects.
- `Explode`: Handles the explosion of the missile.

CONNECTS TO: Other game systems such as collision detection, damage handling, and cluster management.

NOTES:
- Contains experimental code related to minefield interactions.
- Includes commented-out debug and timeout handling in Newton's method functions.

## `src/Igc/missileigc.h`

PURPOSE: This file defines the CmissileIGC class, which represents a missile in the Allegiance game. It inherits from TmodelIGC and implements interfaces for basic IGC functionality, model-specific operations, damage handling, and missile-specific attributes.

KEY PARTS:
- CmissileIGC: Represents a missile with methods to initialize, update, handle collisions, receive damage, set and get target, explode, disarm, and manage its state.
- Initialize: Sets up the missile with mission parameters and initial data.
- ReceiveDamage: Handles damage received by the missile, considering factors like friendly fire rules and experience modifiers.
- SetTarget: Updates the missile's target and lock status.
- Explode: Initiates the explosion of the missile at a specified position.

CONNECTS TO:
- igc.h: Base interface for game objects.
- modelIGC.h: Interface for model-specific operations.
- ImissileTypeIGC, IshipIGC, ImodelIGC: Interfaces related to missile type, launcher ship, and target model.

NOTES:
- Contains commented-out code for setting the trail color based on side affiliation.
- Includes logic for handling friendly fire and applying experience modifiers to damage calculations.

## `src/Igc/missiletypeigc.cpp`

PURPOSE:
The file `missileTypeIGC.cpp` contains the implementation of the `CmissileTypeIGC` class, which is responsible for managing missile types in a game mission.

KEY PARTS:
- **Class: CmissileTypeIGC**
  - Manages missile types within a game mission.
  
- **Function: Initialize**
  - Initializes the missile type with data from a mission and preloads necessary resources like models and textures.
  - Adds the missile type to the mission's expendable types.

- **Function: Export**
  - Exports the missile type's data into a buffer for saving or transmission.

CONNECTS TO:
Depends on `ImissionIGC` for mission-related operations, `HitTest::Load` for loading hit test data, and likely other game systems for resource management and gameplay logic.

NOTES:
- The code includes assertions and retail checks to ensure correct data types and values.
- There are no obvious TODOs or hacks in the provided snippet.

## `src/Igc/missiletypeigc.h`

PURPOSE: This file defines the CmissileTypeIGC class, which implements the ImissileTypeIGC interface to represent missile types in the Allegiance game.

KEY PARTS:
- Name: CmissileTypeIGC
  - Implements the ImissileTypeIGC interface for managing missile types.
- Initialize: Initializes the missile type with mission data.
- Terminate: Cleans up resources when the missile type is terminated.
- Update: Updates the state of the missile type.
- Export: Exports the missile type data to a buffer.
- GetObjectType, GetObjectID: Returns the object type and ID of the missile type.
- GetData: Retrieves the data associated with the missile type.
- GetLauncherDef, GetEquipmentType, GetLoadTime, etc.: Provides various properties of the missile type.

CONNECTS TO:
- igc.h: The header file for the ImissileTypeIGC interface.

NOTES:
- None.

## `src/Igc/missionigc.cpp`

PURPOSE: The file `missionigc.cpp` contains implementation for managing mission-related data and operations within the Allegiance game, including loading and saving IGC files, converting technology and part lists to bitmasks, and handling various types of objects like projectiles, missiles, probes, and chaff.

KEY PARTS:
- CmissionIGC: Manages mission object functionality.
- DumpIGCFile: Writes mission data to an IGC file.
- LoadIGCFile: Reads mission data from an IGC file.
- TechBitFromToken: Converts technology token to bit index.
- PartsListToMask: Converts a string of part names to a bitmask.
- BitsToTechsList: Converts a tech bitmask to a list of tech names.
- BitsToPartsList: Converts a part bitmask to a list of part names.
- readCSV: Reads data from a CSV file into a vector of vectors.
- ImportStaticIGCObjs: Imports static IGC objects from CSV files.
- DataProjectileTypeIGC, DataMissileTypeIGC, etc.: Structures representing different types of game objects.
- ObjectType: Enumerates object types (e.g., projectile, missile).
- UTL::artworkPath(): Returns the path to artwork files.

CONNECTS TO: Other mission-related classes and functions within Allegiance, such as `CstaticIGC`, `pMission->GetMissionParams()`, and various data structures like `DataStationTypeIGC`.

NOTES:
- Contains TODOs for implementing support for gzipped CSV files, resurrecting the "artwork directory monitor" feature, adding support for exporting various object types, and including artwork directory names in exported files.
- Functions like `DumpIGCFile` and `LoadIGCFile` handle file I/O operations critical to mission persistence.

PARTS (in file order):

- lines 1-326
  - CmissionIGC: Class implementing mission object functionality.
  - DumpIGCFile: Function to dump IGC file data.
  - LoadIGCFile: Function to load IGC file data.
  - TechBitFromToken: Converts technology token to bit index.
  - TechsListToBits: Converts technology list to bitmask.
  - PartMaskFromToken: Converts part token to bitmask.
  - AbilitiesListToMask: Converts abilities list to bitmask.
  - Consider changing mission object to allow only a single instance.
- lines 327-659
  - CmissionIGC: Manages mission-related data and operations.
  - PartsListToMask: Converts a string of part names to a bitmask.
  - BitsToTechsList: Converts a tech bitmask to a list of tech names.
  - BitsToPartsList: Converts a part bitmask to a list of part names.
  - LoadTechBitsList: Loads tech bit flags from a CSV file.
  - LoadPartsBitsList: Loads part bit flags from a CSV file.
  - readCSV: Reads data from a CSV file into a vector of vectors.
  - ImportStaticIGCObjs: Imports static IGC objects from CSV files.
- lines 660-968
  - DataProjectileTypeIGC: Represents projectile data.
  - DataMissileTypeIGC: Represents missile data.
  - DataProbeTypeIGC: Represents probe data.
  - ObjectType: Enumerates object types (e.g., projectile, missile).
  - readCSV: Function to read CSV data into a vector of strings.
  - UTL::artworkPath(): Returns the path to artwork files.
  - pMission->GetMissionParams()->szIGCStaticFile: Retrieves the IGC static file name.
  - TechsListToBits: Converts technology lists to bitmasks.
- lines 969-1279
  - DataChaffTypeIGC: Represents a chaff expendable type.
  - DataMineTypeIGC: Represents a mine expendable type.
  - DataLauncherTypeIGC: Represents a launcher for various expendables.
  - AbilitiesListToMask: Converts a list of abilities into a bitmask.
  - PartMaskFromToken: Converts a token representing a part type into a bitmask.
  - TechsListToBits: Converts a list of technologies into a bitmask.
  - ObjectType: Enumerates the types of objects, including probes and chaff.
  - UTL::artworkPath(): Returns the path to artwork files.
- lines 1280-1590
  - ObjectType: Enumerates different types of objects.
  - DataAfterburnerTypeIGC, DataWeaponTypeIGC, DataPackTypeIGC, DataShieldTypeIGC, DataCloakTypeIGC: Structures representing different types of equipment parts.
  - UTL::artworkPath(): Function to get the path to artwork files.
  - pMission->GetMissionParams()->szIGCStaticFile: Retrieves the static file name for mission parameters.
  - readCSV(): Function to read CSV data into a vector.
  - csvVector: A vector of vectors representing CSV data.
  - Strcpy(): Function to copy strings.
  - atoi() and atof(): Functions to convert string to integer and floating-point number, respectively.
- lines 1591-1903
  - DataLauncherTypeIGC: Represents an expendable part launcher.
  - DataHullTypeIGC: Represents a ship hull type and its associated hardpoints.
  - ObjectType: Enumerates different types of objects (e.g., parts, hulls).
  - UTL::artworkPath(): Returns the path to artwork files.
  - pMission->GetMissionParams()->szIGCStaticFile: Path to static IGC data files.
  - readCSV: Function to read CSV data into a vector.
  - TechsListToBits: Converts a list of technologies to bitmasks.
  - PartsListToMask: Converts a list of parts to a bitmask.
- lines 1904-2213
  - DataDevelopmentIGC: Represents development data for a game object.
  - DataDroneTypeIGC: Represents drone type data for a game object.
  - DataStationTypeIGC: Represents station type data for a game object.
  - DataTreasureSetIGC: Represents treasure set data for a game object.
  - readCSV: Function to read CSV data from a file into a vector of strings.
  - TechsListToBits: Converts a list of technologies to bitmasks.
  - AbilitiesListToMask: Converts a list of abilities to bitmasks.
  - Strcpy: Custom string copy function.
- lines 2214-2553
  - TreasureData: Represents treasure data with properties like type and chance.
  - DataCivilizationIGC: Holds civilization-specific data such as income, bonuses, names, and techs.
  - DumpIGCFile: Writes mission data to an IGC file.
  - LoadIGCFile: Reads mission data from an IGC file.
  - DumpIGCStaticCore: Writes static core data to an IGC file with a version timestamp.
  - LoadIGCStaticCore: Reads static core data from an IGC file and caches it for reuse.
  - CachedStaticCore: Stores cached static core data along with its version and consumer count.
  - s_cores: A list of cached static cores.
- lines 2554-2897
  - CreateMission: Function that creates a new mission object.
  - CstaticIGC::CstaticIGC: Constructor for the static game configuration class, initializing various constants and damage constants.
  - pMission->GetHullTypes(), pMission->GetStationTypes(): Methods to retrieve lists of hull types and station types in the mission.
  - strcmp(h2->GetName(), l3->data()->GetName()) == 0: Compares names of objects for equality.
  - pMission->GetDamageConstant(pmt->GetDamageType(), h2->GetDefenseType()): Retrieves damage constants based on damage type and defense type.
  - debugf("\t\t\t%8.2f", ...): Outputs formatted debug information.
  - FileHashTable fileHashTable: Used for checking file hashes, likely for security purposes.
  - UTL::getFile(strName, ".csv", szFilename, true, true): Attempts to find a text-based core file.
- lines 2898-3244
  - CmissionIGC: Manages a mission in the game.
  - TerminateStatic(CstaticIGC pStatic): Safely terminates a static object.
  - Initialize(Time now, IIgcSite pIgcSite): Initializes the mission with the given site and time.
  - Terminate(void): Terminates the mission by cleaning up all resources.
  - Update(Time now): Updates the mission state based on the current time.
  - ExportList(int64_t maskTypes, const BaseListIGC plist, char ppdata): Exports a list of objects based on the given types and data pointer.
  - Export(int64_t maskTypes, char pdata) const: Exports mission data based on the given types and data pointer.
  - Fixing fedsrv crashes (mentioned in comments)
- lines 3246-3604
  - CmissionIGC: Manages mission data and objects.
  - ExportList: Exports a list of objects to a buffer.
  - Import: Imports objects from a buffer into the mission.
  - CreateObject: Creates an object based on its type.
  - GetModel: Retrieves a model for a given object type and ID.
  - GetBase: Retrieves a base object for a given object type and ID.
  - AddWarp, DeleteWarp, GetWarp, GetWarps: Manage warp objects.
  - AddStation, DeleteStation, GetStation, GetStations: Manage station objects.
- lines 3605-3935
  - CmissionIGC: Manages mission data and entities.
  - AddSide: Adds a side to the mission.
  - DeleteSide: Removes a side from the mission.
  - GetSide: Retrieves a side by ID.
  - GetSides: Returns all sides in the mission.
  - GetSeenSides: Determines which sides can see a given model.
  - AddCluster: Adds a cluster to the mission.
  - DeleteCluster: Removes a cluster from the mission.
- lines 3936-4274
  - CmissionIGC::GenerateTreasure: Generates treasures for a cluster at a random position.
  - GenerateNewTreasureID: Generates a unique ID for a new treasure.
  - GetReplayCount: Returns the number of times this mission has been replayed.
  - GetContextName: Generates and returns a context name based on the mission creation time and other parameters.
  - ConstantsDamageComment: Provides comments for damage and defense constants, which are currently not used in the code.
- lines 4275-4597
  - ConstantsDamageComment: Converts damage type IDs to human-readable strings.
  - Obj2Txt: Main function that dumps IGC data to text files.
  - DataStationTypeIGC, DataProjectileTypeIGC, etc.: Structures representing different types of game objects.
  - ObjectType: Enumerates the types of game objects (e.g., station, projectile).
  - IbaseIGC, IexpendableTypeIGC, etc.: Interfaces for accessing game object data.
  - Export method: Exports data from an IGC object to a structure.
  - BitsToTechsList and BitsToPartsList: Convert bit flags to lists of technologies or parts.
  - UTL::getFile: Utility function to get file paths with extensions.
- lines 4598-4925
  - DataExpendableTypeIGC: Represents expendable types with capabilities and attributes.
  - DataProbeTypeIGC: Represents probe types with specific attributes like scanner range.
  - DataPartTypeIGC: Represents part types with various properties including equipment type.
  - DataBuyableIGC: Base class for buyable items, containing common attributes like price and build time.
  - DataLauncherTypeIGC, DataCloakTypeIGC, DataAfterburnerTypeIGC, DataPackTypeIGC, DataShieldTypeIGC: Specific data classes for different types of parts.
  - ET_Dispenser, ET_ChaffLauncher, ET_Magazine, ET_Cloak, ET_Afterburner, ET_Pack, ET_Shield: Enumerations representing different equipment types.
  - pMission->BitsToPartsList(): Converts bitmasks to a list of parts.
  - ConstantsDamageComment(): Function to generate damage comment based on defense type.
- lines 4926-5248
  - DataBuyableIGC: Represents an item that can be bought.
  - DataWeaponTypeIGC: Contains data specific to weapon types.
  - DataHullTypeIGC: Contains data specific to hull types.
  - DataDevelopmentIGC: Represents a development or upgrade.
  - DataDroneTypeIGC: Contains data specific to drone types.
  - DataCivilizationIGC: Represents a civilization in the game.
  - BitsToPartsList: Converts bitmasks to a list of parts.
  - BitsToTechsList: Converts bitmasks to a list of technologies.
- lines 5249-5584
  - CmissionIGC::ExportStaticIGCObjs: Exports static IGC objects to text files.
  - Obj2Txt: Converts an object to a text line and writes it to a file.
  - ConstantsGlobalComment: Provides comments for global constants.
  - UTL::getFile: Retrieves a file with the specified extension.
  - TODO: Implement support for gzipped CSV files.
  - TODO: Resurrect the "artwork directory monitor" feature.
  - TODO: Add support for exporting various object types like stations, projectiles, and expendables.
  - TODO: Include artwork directory names in exported files for convenience.
- lines 5585-5706
  - Obj2Txt: Function that converts an object of a specific type (OT_partType, OT_hullType, etc.) into text format and writes it to a file.
  - UTL::getFile: Utility function to retrieve a file path based on the mission parameters and a given directory name.
  - ZString: A string class used for handling strings in Allegiance.
  - Standard C library type representing a file handle.
  - fwrite: Standard C library function for writing data to a file.
  - fclose: Standard C library function for closing a file.
  - TODOs: None explicitly mentioned in the provided code snippet.

## `src/Igc/missionigc.h`

**PURPOSE:** The file `missionigc.h` defines the classes and structures necessary for managing game missions in Allegiance. It includes data management for static mission constants and dynamic objects like stations and treasures.

**KEY PARTS:**
- **CstaticIGC:** Manages static data for all missions, including various types of objects and resource packs.
- **CmissionIGC:** Represents a specific mission, inheriting from ImissionIGC and managing mission-specific data.
- **Constants:** Holds arrays for float and damage constants used throughout the game.
- **Data Members (CstaticIGC):** Lists of object types and pointers to resource packs.
- **Data Members (CmissionIGC):** Pointers to associated IGC site, counters, and identifiers.

**CONNECTS TO:** Other mission-related classes and structures such as `IstationIGC`, `ItreasureIGC`, and `IIgcSite`.

**NOTES:**
- The `Initialize` method of CstaticIGC has an assertion that it should never be called.
- Debugging statements are included for terminating resources.
- Reading missions from plain-text Imago is not implemented yet.
- Assertions and checks ensure consistency in object creation.

## `src/Igc/modelIGC.cpp`

PURPOSE: The file `modelIGC.cpp` contains the implementation of the `CmodelIGC` class, which manages the model and its associated properties in the Allegiance game.

KEY PARTS:
- `CmodelIGC`: Manages the model's visibility, rendering, attributes, and interactions.
- `LoadCVH`: Loads a model from a CVH file and sets up hit testing and thing site.
- `Update`: Updates the model's rotation based on its current state.
- `Terminate`: Cleans up resources when the model is terminated.
- `SetVisibleF` and `SetRender`: Control the visibility and rendering of the model.
- `SetPosition`, `SetOrientation`, `SetMyAttributes`: Set various properties of the model.

CONNECTS TO: `modelIGC.h`, `HitTest`, `IIgcSite`, `IclusterIGC`.

NOTES: None.

## `src/Igc/modelIGC.h`

**PURPOSE:** The `src/Igc/modelIGC.h` file defines the class `CmodelIGC`, which manages a model's attributes and behaviors in the Allegiance game. It includes methods for initializing, updating, and rendering models, as well as handling visibility and collision detection.

**KEY PARTS:**
- **Class:** CmodelIGC
  - Inherits from IbaseIGC and ImodelIGC.
  - Manages model properties such as position, velocity, orientation, and attributes.
  
- **Functions:**
  - Initialize: Sets up the model with mission data.
  - Terminate: Cleans up resources.
  - Update: Updates the model's state.
  - LoadCVH: Loads a CVH model from a file.
  - LoadMine: Loads a mine model.
  - LoadWarp: Loads a warp model.
  - LoadEffect: Loads an effect model.
  - MakeStatic: Sets the model as static.
  - MakeUnhitable: Makes the model unhitable but movable.
  - SetVisibleF: Controls visibility of the model.
  - GetVisibleF: Returns visibility status.
  - GetCurrentEye: Checks if seen by a specific side's eye.
  - SetRender: Configures rendering options.
  - SeenBySide: Determines visibility to a given side.
  - UpdateSeenBySide: Updates visibility information for all sides.
  - SetSideVisibility: Sets visibility for a specific side.
  - GetThingSite: Retrieves the ThingSite associated with the model.
  - FreeThingSite: Frees resources used by the ThingSite.
  - GetPosition: Returns current position.
  - SetPosition: Sets new position.

**CONNECTS TO:** Other game systems and files related to model management, collision detection, and rendering.

**NOTES:** None.

## `src/Igc/packigc.cpp`

**PURPOSE:**  
The `packIGC.cpp` file contains the implementation of the `CpackIGC` class, which is responsible for managing parts within a ship in the Allegiance game. It handles initialization, termination, and management of part data and relationships with ships.

**KEY PARTS:**
- **Class: CpackIGC**
  - Manages individual parts within a ship.
  - Handles initialization, termination, setting the parent ship, and managing mount IDs.
  
- **Function: Initialize**
  - Initializes the part with mission data and sets up references to the part type and ship.

- **Function: Terminate**
  - Cleans up resources by releasing references and removing the part from its ship.

- **Function: SetShip**
  - Updates the parent ship for the part, ensuring proper reference counting and part management.

- **Function: SetMountID**
  - Updates the mount ID of the part within its ship.

**CONNECTS TO:**  
`ImissionIGC`, `IshipIGC`, `DataPartIGC`, `DataPackTypeIGC`

**NOTES:**  
None.

## `src/Igc/packigc.h`

PURPOSE: The file `packigc.h` contains the declaration for the `CpackIGC` class, which implements the `IpackIGC` interface. This class represents a pack item in the game Allegiance, such as fuel or ammo.

KEY PARTS:
- **Class: CpackIGC** - Implements the `IpackIGC` interface to manage pack items.
  - **Methods**: Initialize, Terminate, GetObjectType, GetMission, GetEquipmentType, GetPartType, GetShip, SetShip, GetMountID, SetMountID, fActive, Activate, Deactivate, GetMass, GetPrice, GetMountedFraction, SetMountedFraction, Arm, GetPackType, SetAmount, GetAmount.
  - **Data Members**: m_pMission, m_typeData, m_partType, m_pship, m_amount, m_mountID.

CONNECTS TO: `igc.h` (interface file), `ImissionIGC`, `IpartIGC`, `IshipIGC`, `IhullTypeIGC`.

NOTES: None.

## `src/Igc/partTypeIGC.cpp`

PURPOSE: This file contains the implementation of the `CpartTypeIGC` class, which is responsible for managing part types in a mission within the Allegiance game.

KEY PARTS:
- `Initialize`: Initializes the part type with data from a mission.
- `Terminate`: Cleans up resources when the part type is no longer needed.
- `Export`: Exports the part type's data to another location.
- `GetEquipmentTypeName`: Returns the name of an equipment type as a string.

CONNECTS TO: `ImissionIGC`, which manages missions and part types.

NOTES: None.

## `src/Igc/partTypeIGC.h`

PURPOSE: This file defines the CpartTypeIGC class, which implements the IpartTypeIGC interface for managing part types in a game mission.

KEY PARTS:
- CpartTypeIGC: Class that represents a part type and implements various interfaces like IbaseIGC, ItypeIGC, IbuyableIGC, and IpartTypeIGC.
- Initialize: Initializes the part type with data from a mission.
- Terminate: Cleans up resources when the part type is no longer needed.
- Export: Exports the part type data to a buffer.
- GetObjectType, GetObjectID: Return the object type and ID of the part type.
- GetData: Returns the raw data associated with the part type.
- GetModelName, GetIconName, GetName, GetDescription, GetPrice, GetTimeToBuild, GetGroupID: Provide various properties of the part type.
- GetMass, GetSignature, GetEquipmentType, GetPartMask, GetRequiredTechs, GetEffectTechs, GetSuccessorPartType: Specific methods for part types.
- m_pMission, m_pptSuccessor, m_data, m_dataSize: Private members used to store mission context, successor part type, data, and size.

CONNECTS TO: igc.h (header file containing interface definitions), TRef<IpartTypeIGC> (reference counting smart pointer).

NOTES: None.

## `src/Igc/private.cpp`

PURPOSE: tiny file (15 lines), shown in full rather than summarized.

    /*
    **  Copyright (C) 1996, 1997 Microsoft Corporation. All Rights Reserved.
    **
    **  File:	private.cpp
    **
    **  Author:
    **
    **  Description:
    **      Implementation file for all things that are internal to the IGC library
    **  but not part of any one class.
    **
    **  History:
    */
    #include    "pch.h"
    #include    "private.h"

## `src/Igc/private.h`

PURPOSE: tiny file (16 lines), shown in full rather than summarized.

    /*
    **  Copyright (C) 1996, 1997 Microsoft Corporation. All Rights Reserved.
    **
    **  File:	private.h
    **
    **  Author:
    **
    **  Description:
    **      Header file for all things that are internal to the IGC library
    **  but not part of any one class.
    **
    **  History:
    */
    #ifndef private_h
        #define private_h
    #endif

## `src/Igc/probeigc.cpp`

PURPOSE:
The file `probeigc.cpp` contains the implementation of the `CprobeIGC` class, which represents a probe in the Allegiance game. This class handles initialization, termination, updating, and exporting data for probes.

KEY PARTS:
- **Class: CprobeIGC** - Manages the behavior and properties of a probe.
  - **Constructor:** Initializes member variables.
  - **Destructor:** Cleans up resources.
  - **Initialize:** Sets up the probe based on mission data.
  - **Terminate:** Handles cleanup when the probe is terminated.
  - **Update:** Updates the probe's state, including movement and shooting.
  - **Export:** Prepares the probe's data for export.

CONNECTS TO:
Depends on various game systems such as `ImissionIGC`, `IshipIGC`, `IsideIGC`, and others. Also interacts with mission parameters and cluster sites.

NOTES:
- Contains checks to ensure probes are not dropped too close to warps or asteroids.
- Includes logic for teleportation probes, which can be activated after a delay.
- Manages probe ammunition and shooting capabilities based on the probe type's attributes.

## `src/Igc/probeigc.h`

**PURPOSE:**  
The `probeIGC.h` file defines the class `CprobeIGC`, which represents a probe in the Allegiance game. This class inherits from `TmodelIGC<IprobeIGC>` and implements various interfaces such as `IbaseIGC`, `ImodelIGC`, `IdamageIGC`, `IscannerIGC`, and `IprobeIGC` to handle the behavior, damage, scanning, and specific properties of probes.

**KEY PARTS:**  
- **Class: CprobeIGC**
  - Manages the lifecycle and interactions of a probe in the game.
  - Implements methods for initialization, termination, updating, exporting data, and handling damage.
  - Provides access to probe-specific properties like type, projectile type, emission point, lifespan, etc.

- **Functions:**
  - `Initialize`: Initializes the probe with mission parameters.
  - `Terminate`: Cleans up resources when the probe is terminated.
  - `Update`: Updates the probe's state based on game time and other factors.
  - `Export`: Exports the current state of the probe to a data structure.
  - `ReceiveDamage`: Handles damage received by the probe, including checks for friendly fire and special conditions.
  - `GetFraction`, `SetFraction`: Get and set the health fraction of the probe.
  - `InScannerRange`, `CanSee`: Determines if the probe can detect other objects within its scanner range.
  - `GetProbeType`, `GetProjectileType`, etc.: Accessor methods for various properties of the probe.

- **Data Members:**
  - `m_pMission`: Pointer to the mission object.
  - `m_probeType`: Type of the probe.
  - `m_projectileType`: Type of projectile associated with the probe.
  - `m_launcher`: Reference to the ship that launched the probe.
  - `m_target`: Reference to the current target of the probe.
  - `m_fraction`: Health fraction of the probe.
  - `m_time0`, `m_timeExpire`: Time-related properties for managing the probe's lifespan and expiration.

**CONNECTS TO:**  
Depends on other game systems, particularly those related to missions (`ImissionIGC`), probes (`IprobeTypeIGC`), projectiles (`IprojectileTypeIGC`), ships (`IshipIGC`), and clusters (`IclusterIGC`).

**NOTES:**  
- The file includes comments and assertions for debugging and validation.
- There are TODOs or experimental code snippets marked with comments, such as handling friendly fire exceptions and setting achievement masks upon probe destruction.

## `src/Igc/probetypeigc.cpp`

**PURPOSE:** This file contains the implementation of the `CprobeTypeIGC` class, which is responsible for initializing and exporting probe type data in a game mission.

**KEY PARTS:**
- **Class: CprobeTypeIGC**
  - Manages initialization and export of probe type data.
- **Function: Initialize**
  - Initializes the probe type with mission data, preloads resources, loads hit test models, and sets up projectile types if applicable.
- **Function: Export**
  - Exports the current state of the probe type data to a buffer.

**CONNECTS TO:** 
- `ImissionIGC`: The mission interface that provides necessary mission-related data and functions.
- `HitTest::Load`: Loads hit test models based on model names.
- `DataProbeTypeIGC`: The structure used for storing probe type data.

**NOTES:**
- None.

## `src/Igc/probetypeigc.h`

PURPOSE:
This file defines a class `CprobeTypeIGC` that implements the interface `IprobeTypeIGC`. It represents a probe type in the game Allegiance and handles initialization, termination, updates, and data retrieval for this type of object.

KEY PARTS:
- **Class: CprobeTypeIGC** - Implements the `IprobeTypeIGC` interface to manage probe types.
- **Functions:** 
  - `Initialize`: Initializes the probe type with mission data.
  - `Terminate`: Cleans up resources and removes the probe type from the mission.
  - `Update`: Updates the state of the probe type.
  - `Export`: Exports data about the probe type.
  - `GetObjectType`, `GetObjectID`, etc.: Provides metadata about the probe type.
- **Data Members:**
  - `m_pMission`: Pointer to the mission object.
  - `m_data`: Contains specific data for the probe type.
  - `m_emissionPt`: Emission point of the probe.
  - `m_projectileType`: Pointer to the projectile type associated with the probe.

CONNECTS TO:
- **IprobeTypeIGC**: Interface that defines methods for probe types.
- **ImissionIGC**: Mission object containing game state and objects.
- **DataProbeTypeIGC**: Struct holding data specific to the probe type.
- **IprojectileTypeIGC**: Interface for projectile types.

NOTES:
- Debugging code is included with `_DEBUG_CprobeTypeIGC_AddRef_Release` defined, which logs reference counts.

## `src/Igc/projectileIGC.cpp`

**PURPOSE:** This file contains the implementation of the `CprojectileIGC` class, which represents a projectile in the Allegiance game. It handles initialization, updates, collisions, and termination of projectiles.

**KEY PARTS:**
- **Class: CprojectileIGC**
  - Manages the lifecycle of a projectile, including initialization, updating, collision handling, and termination.
  
- **Function: Initialize**
  - Initializes the projectile with data from a mission file, setting its properties such as type, velocity, orientation, lifespan, and model.

- **Function: Update**
  - Updates the projectile's state based on time, checking for collisions and managing its expiration.

- **Function: HandleCollision**
  - Handles collisions between the projectile and other objects, applying damage or creating explosions as necessary.

- **Function: Export**
  - Exports the current state of the projectile to a data structure for saving or transmission.

**CONNECTS TO:** 
- `ImissionIGC`: The mission object that manages game entities.
- `IprojectileIGC`: Interface defining the behavior of projectiles.
- `DataProjectileIGC`, `DataProjectileTypeIGC`: Data structures used for initializing and managing projectile properties.

**NOTES:**
- None.

## `src/Igc/projectileIGC.h`

**PURPOSE:** This file defines the header for the `CprojectileIGC` class, which represents a projectile in the Allegiance game. It inherits from `TmodelIGC<IprojectileIGC>` and implements interfaces like `IbaseIGC`, `ImodelIGC`, and `IprojectileIGC`.

**KEY PARTS:**
- **Class:** `CprojectileIGC`
  - Manages a projectile's behavior, including initialization, termination, updating, and collision handling.
- **Functions:**
  - `Initialize`: Initializes the projectile with mission data.
  - `Terminate`: Cleans up resources when the projectile is destroyed.
  - `Update`: Updates the projectile's state based on time.
  - `Export`: Exports the projectile's state to a data buffer.
  - `SetCluster`: Sets the cluster (group) the projectile belongs to and handles model addition/removal.
  - `HandleCollision`: Handles collisions with other models.
  - `GetProjectileType`: Returns the type of the projectile.
  - `GetLauncher`: Returns the launcher that fired the projectile.
  - `SetLauncher`: Sets the launcher for the projectile.
  - `SetGunner`: Sets the gunner who fired the projectile.
- **Data Members:**
  - `m_projectileType`: Pointer to the projectile type.
  - `m_launcher`: Pointer to the model that launched the projectile.
  - `m_pExplosionData`: Data for explosion effects.
  - `m_timeExpire`: Time at which the projectile will expire.

**CONNECTS TO:** 
- `igc.h`, `modelIGC.h` (for base interfaces and models)
- `IprojectileTypeIGC`, `ImodelIGC`, `IshipIGC` (related interfaces)

**NOTES:** None.

## `src/Igc/projectileTypeIGC.cpp`

PURPOSE: This file contains the implementation of the CprojectileTypeIGC class, which is responsible for managing projectile types in a game mission.

KEY PARTS:
- CprojectileTypeIGC: Manages the initialization and export of projectile type data.
- Initialize: Sets up the projectile type with mission context and data.
- Export: Serializes the projectile type data into a buffer.

CONNECTS TO: ImissionIGC, DataProjectileTypeIGC

NOTES: None.

## `src/Igc/projectileTypeIGC.h`

PURPOSE: This file defines the CprojectileTypeIGC class, which represents a projectile type in the Allegiance game. It implements interfaces for initializing, terminating, and exporting projectile data.

KEY PARTS:
- CprojectileTypeIGC: Class representing a projectile type.
  - Initialize: Initializes the projectile type with mission context and data.
  - Terminate: Cleans up resources when the projectile type is no longer needed.
  - Export: Exports the projectile type's data to a buffer.
  - GetObjectType, GetObjectID: Returns object type and ID.
  - GetData: Retrieves the raw data of the projectile type.
  - GetPower, GetBlastPower, etc.: Accessors for various properties of the projectile type.

CONNECTS TO:
- ImissionIGC: The mission context that manages the projectile type.
- DataProjectileTypeIGC: Struct containing the actual data of the projectile type.

NOTES: None.

## `src/Igc/shieldIGC.cpp`

PURPOSE: The file `shieldIGC.cpp` implements the functionality for a shield part in the Allegiance game. It handles initialization, updates, damage application, and state management for the shield.

KEY PARTS:
- CshieldIGC: Class representing a shield part.
  - Initialize: Sets up the shield with mission data.
  - Terminate: Cleans up resources when the shield is no longer needed.
  - Update: Handles shield regeneration and mounting/dismounting.
  - SetShip: Assigns the shield to a ship and manages its mount state.
  - SetMountID: Updates the mount ID of the shield.
  - ApplyDamage: Calculates how much damage the shield absorbs and how much leaks through.

CONNECTS TO:
- ImissionIGC: The mission interface for accessing game constants and other mission-related data.
- IshipIGC: The ship interface for interacting with the ship that has the shield part.
- IIgcSite: The site interface for playing sounds and posting notifications.

NOTES:
- None.

## `src/Igc/shieldIGC.h`

PURPOSE:
The file `shieldIGC.h` defines the interface and implementation for a shield component in the Allegiance game. It includes methods for initializing, updating, and managing the state of a shield.

KEY PARTS:
- CshieldIGC: Class representing a shield component.
  - Methods for initialization, termination, and updates.
  - Functions to get and set properties like mass, price, and fraction.
  - Shield-specific functions to manage regeneration, max strength, defense type, and applying damage.

CONNECTS TO:
Depends on `igc.h`, `DataShieldTypeIGC`, `IpartTypeIGC`, `IshipIGC`, and `ImissionIGC`.

NOTES:
- None.

## `src/Igc/shipIGC.cpp`

PURPOSE: The file `src/Igc/shipIGC.cpp` contains the implementation of the `CshipIGC` class, which manages the behavior and state of ships in the Allegiance game.

KEY PARTS:
- CshipIGC: Main class for ship objects.
- ReInitialize: Resets or initializes a ship's state based on provided data.
- Initialize: Initializes a new ship object with mission data.
- Terminate: Cleans up and terminates the ship object.
- Update: Updates the ship's state, including energy recharge and weapon updates.
- Separate: Calculates the separation between two convex hulls to resolve collisions.
- HandleCollision: Handles collisions with other objects, including warps and stations.
- ReceiveDamage: Handles damage received by a ship, including friendly fire checks and repair calculations.
- AddPart: Adds a new part to the ship and updates its mass.
- DeletePart: Removes a part from the ship and updates its mass.
- MountPart: Mounts or dismounts a part on the ship.
- SetStateM: Sets the state of the ship, including handling cloak activation sounds.
- ExecuteTurretMove: Handles turret movement based on controls and orientation.
- PreplotShipMove: Manages ship movement and behavior, including checking for ammo and running away conditions.
- FindTargetShip: Finds a target ship based on given criteria.

CONNECTS TO:
- IshipIGC interface
- ImodelIGC class
- GlobalAttributeSet

NOTES:
- Contains TODOs and NYI debug comments indicating unimplemented or not yet integrated features.
- Includes time-related variables like `m_dtCheckRunaway`.
- Handles various ship states, collisions, and damage.

PARTS (in file order):

- lines 1-358
  - CshipIGC: Main class for ship objects.
  - ReInitialize: Resets or initializes a ship's state based on provided data.
  - Initialize: Initializes a new ship object with mission data.
  - Terminate: Cleans up and terminates the ship object.
  - Update: Updates the ship's state, including energy recharge and weapon updates.
  - TODOs: "BT - 9/17 - Trying to hunt down the mystery damage ship event crash." "Imago 6/10 #91" "Imago 7/10 #7" "Xynth #48 8/2010"
  - Unusual code: m_dtCheckRunaway = 31.0f; and similar initialization of time-related variables. if (m_pilotType == c_ptBuilder) { ... } conditional handling for builder ships. SetRipcordModel(NULL); in multiple places, possibly to reset the ripcord state.
- lines 359-731
  - CshipIGC::Update: Updates the ship's state and handles collisions.
  - m_mountedOthers[ET_Afterburner]->Update(now): Updates the afterburner mount.
  - GetMyMission()->GetIgcSite()->KillShipEvent: Triggers a kill event for the ship if it has lifepod capability and is past its lifespan.
  - Separate: Calculates the separation between two convex hulls to resolve collisions.
  - HandleCollision: Handles collisions with other objects, including warps and stations.
  - ObjectType: Enumerates types of objects that can collide with ships.
  - OT_warp: Represents a warp point in space.
  - OT_station: Represents a space station.
- lines 732-1090
  - IsideIGC pside1 = GetSide(): Retrieves the side of the current ship.
  - IsideIGC pside2 = pModel->GetSide(): Retrieves the side of the model involved in the collision.
  - Separate(entry, false, &position1, &position2, &normal): Calculates the positions and normal vector after separation.
  - SetPosition(position1): Sets the new position of the ship.
  - SetVelocity(newVelocity1): Sets the new velocity of the ship.
  - ReceiveDamage(c_dmgidCollision, ...): Handles damage to the current ship.
  - ((IdamageIGC)pModel)->ReceiveDamage(...): Handles damage to the model involved in the collision.
  - PlaySoundEffect(collisionSound, this): Plays a collision sound effect.
- lines 1091-1452
  - CshipIGC::ReceiveDamage: Handles damage received by a ship.
  - CshipIGC::SetBaseHullType: Changes the base hull type of a ship.
  - newSpeed1 and newSpeed2: Calculate new velocities after collision.
  - cElasticity: Coefficient of elasticity for collisions.
  - vInelastic, vElastic1, vElastic2: Velocities before and after collision adjustments.
  - damage1 and damage2: Calculated damage amounts.
  - hp1 and hp2: Hit points of the ship and its model.
  - ReceiveDamage: Handles damage received by a ship, including friendly fire checks and repair calculations.
- lines 1453-1815
  - CshipIGC::GetMyMission(): Retrieves the mission associated with the ship.
  - CshipIGC::AddPart(IpartIGC part): Adds a new part to the ship and updates its mass.
  - CshipIGC::DeletePart(IpartIGC part): Removes a part from the ship and updates its mass.
  - CshipIGC::MountPart(IpartIGC part, Mount mountNew, Mount pmountOld): Mounts or dismounts a part on the ship.
  - CshipIGC::SetStateM(int newVal): Sets the state of the ship, including handling cloak activation sounds.
  - CshipIGC::ExecuteTurretMove(Time timeStart, Time timeStop, Orientation pOrientation): Handles turret movement based on controls and orientation.
  - CshipIGC::PreplotShipMove(Time timeStop): Manages ship movement and behavior, including checking for ammo and running away conditions.
  - "NYI debug" comments indicating unimplemented or not yet integrated features.
- lines 1816-2147
  - IshipIGC: Interface for ship objects.
  - FindTargetShip: Finds a target ship based on given criteria.
  - PlotShipMove: Handles movement and behavior of ships.
  - ModelHasTargetPriority: Determines if a model has priority over another ship.
  - c_ptPlayer, c_ptWingman, etc.: Constants representing different pilot types.
  - c_ttFriendly, c_ttEnemy, etc.: Constants for target type masks.
  - wantsToMineMaskIGC: Bitmask indicating the desire to mine.
  - droneRipMaskIGC: Bitmask indicating a drone is being ripped off.
- lines 2149-2488
  - IshipIGC: Represents a ship in the game.
  - ImodelIGC: Represents a model or object in the game.
  - IasteroidIGC: Represents an asteroid in the game.
  - GlobalAttributeSet: Holds global attributes for sides and other entities.
  - GetMyMission(): Returns the current mission instance.
  - GetSide(): Returns the side of the entity.
  - GetCluster(): Returns the cluster containing the entity.
  - FindTarget(): Finds a target based on given criteria.
- lines 2489-2811
  - GetStateM(): Retrieves the current state of the ship.
  - SetStateBits(mask, state): Sets or clears bits in the ship's state based on a mask and new state.
  - solveForLead(): Calculates the time until the drone can lead its target.
  - turnToFace(): Determines the turn angle to face a given direction.
  - Dodge(): Handles dodging logic for the drone.
  - FindTargetShip(): Finds the nearest enemy ship within a specified range.
  - SetCommand(): Sets the command for the drone, including attack, goto, or build actions.
  - CanSee(): Checks if the drone can see its target.
- lines 2812-3165
  - CshipIGC: Main class for ship objects, handling movement, state changes, and updates.
  - ExecuteShipMove: Updates the ship's velocity and orientation based on control inputs.
  - ProcessFractions: Sets various fractions (hull, shield, fuel, ammo, energy) of the ship.
  - LegalPosition: Checks if a given position is within legal bounds.
  - ProcessShipUpdate: Handles different types of ship updates from server or client.
  - TODO: Add comments explaining the purpose of the code blocks.
  - TODO: Review and optimize the velocity and thrust calculations for performance.
  - Debugf statements added to log unusual conditions, such as negative velocity squared or division by zero.
- lines 3167-3539
  - CshipIGC: Main class for managing ship updates and state.
  - ProcessShipUpdate: Handles processing of ship updates based on different types (ServerSingleShipUpdate, ServerActiveTurretUpdate, ClientActiveTurretUpdate).
  - CalculateShip: Calculates the ship's position, orientation, and velocity at a given time.
  - WarpShip: Similar to CalculateShip but sets the calculated values directly.
  - SetSide: Handles changing sides for the ship, including updating parent-child relationships and mission associations.
  - SetMission: Resets the ship's state when it changes missions, clearing parts and inventory.
  - Promote: Transfers ownership of a ship from one parent to another, copying over its inventory.
  - Debugging statements commented out but still present in the code.
- lines 3540-3886
  - CshipIGC::SetParentShip(IshipIGC pshipParent): Sets a new parent ship for the current ship and handles various state transitions.
  - ShipPair: A struct containing an IshipIGC pointer and an IclusterIGC pointer, used to store pairs of ships and their clusters.
  - FindRipcordModel(IclusterIGC pcluster): Finds a suitable model for ripcording based on the current ship's capabilities and mission parameters.
  - HullAbilityBitMask: An enumeration representing various abilities related to hulls.
  - c_habmCanLtRipcord, c_habmIsRipcordTarget, c_habmIsLtRipcordTarget: Bitmask values indicating specific ripcord-related abilities.
  - OT_probe, OT_ship: Object types for probes and ships.
  - GetObjectType(): Returns the object type of a model.
  - GetSide(): Returns the side associated with a ship or probe.
- lines 3887-4232
  - CshipIGC::SetAutopilot: Sets the autopilot state and updates the command if necessary.
  - CshipIGC::ResetWaypoint: Resets the waypoint based on the current command target.
  - CshipIGC::IsSafeToRipcord: Determines if it is safe to ripcord based on velocity and thrust calculations.
  - CshipIGC::bShouldUseRipcord: Checks if ripcording should be used for a given cluster.
  - debtMin: Tracks the minimum ripcord debt encountered.
  - pmodelRipcord: Pointer to the model that can be ripped corded.
  - positionGoal: Target position for the ship.
  - warps: List of warps in the current cluster.
- lines 4233-4577
  - CshipIGC::InGarage: Checks if a ship can land inside another ship's garage.
  - MyHullType::Initialize: Initializes a hull type with mission data.
  - MyHullType::Terminate: Terminates the hull type.
  - MyHullType::Update: Updates the hull type.
  - MyHullType::GetObjectType: Returns the object type as OT_hullType.
  - MyHullType::GetObjectID: Returns the object ID of the hull type.
  - MyHullType::GetData: Returns the data associated with the hull type.
  - MyHullType::GetModelName: Returns the model name of the hull type.
- lines 4578-4768
  - MyHullType::GetPartMask: Returns a bitmask of parts for a given equipment type and mount.
  - MyHullType::GetCapacity: Returns the capacity for a given equipment type.
  - MyHullType::GetMaxWeapons: Returns the maximum number of weapons that can be mounted.
  - MyHullType::GetMaxFixedWeapons: Returns the maximum number of fixed weapons that can be mounted.
  - MyHullType::GetHardpointData: Retrieves hardpoint data for a given mount ID.
  - MyHullType::GetTextureName: Returns the texture name for the hull type.
  - MyHullType::GetIconName: Returns the icon name for the hull type.
  - MyHullType::GetCapabilities: Returns the capabilities bitmask of the hull type.

## `src/Igc/shipIGC.h`

PURPOSE: The file `shipIGC.h` contains declarations for classes and methods related to managing and controlling ships in the Allegiance game. It includes functionalities for initializing, updating, moving, and processing ship-related data.

KEY PARTS:
- CshipIGC: Main class representing a ship.
- MyHullType: Implements specific data for a ship's hull type.
- Initialize: Sets up the ship with mission data.
- Update: Updates the state of the ship.
- CanSee: Determines if the ship can see another model.
- SetCommand: Sets commands for the ship.
- ExecuteShipMove: Executes a ship move.
- ExportFractions: Exports ship fractions to a CompactShipFractions object.
- HitTreasure: Handles logic for hitting treasures like parts, development items, powerups, and flags.
- GetLaunchSlot: Returns the next launch slot for a ship.
- PickDefaultOrder: Determines default orders based on the ship's type.

CONNECTS TO:
- ImodelIGC: Used for model-related operations.
- DataShipIGC: Contains data for reinitializing the ship.
- CompactShipFractions: For exporting fractions of the ship.
- MissionSite: For mission-related commands and updates.

NOTES:
- The file contains TODO comments indicating areas that need fixing, such as crash issues.

PARTS (in file order):

- lines 1-364
  - CshipIGC: Main class representing a ship in the game.
  - MyHullType: Class that implements IhullTypeIGC and provides specific data for a ship's hull type.
  - Initialize: Method to initialize the ship with mission data.
  - Terminate: Method to clean up resources when the ship is terminated.
  - Update: Method to update the state of the ship.
  - GetObjectType: Returns the object type as OT_ship.
  - GetObjectID: Returns the unique ID of the ship.
  - SetCluster: Sets the cluster (group) that the ship belongs to.
- lines 366-721
  - CanSee(ImodelIGC pModel) const: Determines if the ship can see another model.
  - SetObjectID(ObjectID id): Sets the object ID of the ship.
  - GetValue(void) const: Calculates the value of the ship based on its hull and parts.
  - ReInitialize(DataShipIGC  dataShip, Time now): Reinitializes the ship with new data.
  - Reset(bool bFull): Resets various properties of the ship.
  - GetTorqueMultiplier(void) const: Returns a multiplier for torque based on velocity.
  - GetCurrentTurnRate(Axis axis) const: Gets the current turn rate along a specific axis.
  - SetCurrentTurnRate(Axis axis, float newVal): Sets the current turn rate along a specific axis.
- lines 722-1065
  - SetCommand: Sets a command for the ship.
  - ExecuteTurretMove: Executes a turret move.
  - PreplotShipMove: Preplots a ship move.
  - PlotShipMove: Plots a ship move.
  - ExecuteShipMove: Executes a ship move.
  - ExportShipLoadout: Exports the ship's loadout.
  - EquivalentShip: Checks if the ship is equivalent to another ship.
  - ProcessShipLoadout: Processes a ship loadout.
- lines 1066-1429
  - ShipIGC: Manages ship data and operations.
  - ExportFractions: Exports ship fractions to a CompactShipFractions object.
  - ExportShipUpdate: Exports ship update information based on different contexts (light, heavy, client).
  - ProcessFractions: Processes incoming fraction updates.
  - ProcessShipUpdate: Processes various types of ship update messages.
  - HitTreasure: Handles the logic for hitting treasures like parts, development items, powerups, and flags.
  - GetLastMissileFired: Retrieves the last missile fired by the ship.
  - SetLastMissileFired: Sets the last missile fired by the ship.
- lines 1430-1778
  - IshipIGC::GetGunner(Mount turretID): const: Returns the gunner for a specific turret.
  - IshipIGC::AddChildShip(IshipIGC pship): Adds a child ship to this ship.
  - IshipIGC::DeleteChildShip(IshipIGC pship): Deletes a child ship from this ship.
  - IshipIGC::CreateAndAddPart(const PartData ppd): Creates and adds a part based on the given data.
  - IshipIGC::LegalCommand(CommandID cid, ImodelIGC pmodel = NULL): const: Checks if a command is legal for this ship.
  - IshipIGC::Complain(SoundID sid, const char pszMsg): Sends a complaint message to the team chat.
  - IshipIGC::SetAutoDonate(IshipIGC pship): Sets the auto-donate target for this ship.
  - TODO: Fix to avoid crash 8963864
- lines 1779-2109
  - GetDeaths: Returns the number of deaths.
  - SetDeaths: Sets the number of deaths.
  - AddDeath: Increments the death count and updates the side's death count.
  - GetEjections: Returns the number of ejections.
  - SetEjections: Sets the number of ejections.
  - AddEjection: Increments the ejection count and updates the side's ejection count.
  - GetExperienceMultiplier: Calculates the experience multiplier based on experience points.
  - GetRipcordModel: Gets the ripcord model associated with the ship.
- lines 2110-2446
  - IshipIGC::GetLaunchSlot: Returns the next launch slot for a ship.
  - IshipIGC::PickDefaultOrder: Determines and sets default orders for a ship based on its type (miner, builder, layer, wingman).
  - IshipIGC::GetOre: Returns the current amount of ore carried by the ship.
  - GetLocalStationToHide: Finds a friendly station to hide at if the ship is damaged.
  - PickDefaultOrder for miners: Handles specific logic for miners including mining asteroids and unloading at stations.
  - PickDefaultOrder for builders: Attempts to build structures near asteroids or other resources.
  - PickDefaultOrder for layers: Sets commands for layers to move towards targets.
  - PickDefaultOrder for wingmen: Delegates handling of default orders to the mission site, likely for training or co-op scenarios.
- lines 2447-2673
  - SetCommand: Sets the command for the ship.
  - GetMyMission: Returns the mission the ship is currently involved in.
  - SendChatf: Sends a chat message to all team members.
  - IsGhost: Checks if the ship has died more times than allowed by its mission parameters.
  - InGarage: Determines if the ship is in a garage.
  - GetEndurance: Calculates the endurance of the ship based on its hull type and lifepod capabilities.
  - IsUsingAreaOfEffectWeapon: Checks if the ship is equipped with an area-of-effect weapon.
  - SetSkills: Sets the skills for shooting, turning, and going to a target.

## `src/Igc/sideigc.cpp`

**PURPOSE:** The file `sideigc.cpp` contains the implementation of the `CsideIGC` class, which represents a side in a mission within the Allegiance game. This class manages various aspects of a side's state and interactions within the game.

**KEY PARTS:**
- **Class: CsideIGC**
  - Manages the state and behavior of a side in the game.
  
- **Function: Initialize**
  - Initializes the side with mission data, sets up initial conditions, and registers the side with the mission.
  
- **Function: Terminate**
  - Cleans up resources and removes the side from the mission when it is no longer needed.

- **Function: Export**
  - Exports the current state of the side to a data structure for saving or transmission.

- **Function: DestroyBuckets**
  - Clears and resets the buckets associated with the side, updating development techs and terminating bucket objects.

- **Function: CreateBuckets**
  - Creates new buckets based on available technologies and resources, allowing the side to produce various items like stations, drones, and developments.

- **Function: IsSurroundedByTerritory**
  - Determines if a cluster is surrounded by territory claimed by the side, which affects the behavior of ships within that sector.

- **Function: HandleNewEnemyCluster**
  - Handles actions when a new enemy cluster is encountered, particularly affecting miners' commands.

- **Function: UpdateTerritory**
  - Updates the list of clusters considered part of the side's territory based on current station claims and enemy presence.

**CONNECTS TO:** 
- `ImissionIGC`, `Time`, `DataSideIGC`, `TechTreeBitMask`, `BucketLinkIGC`, `StationLinkIGC`, `ClusterLinkIGC`, etc. (various game objects and data structures)

**NOTES:**
- Contains TODOs for adding clusters claimed by allies' stations.
- Includes commented-out code related to hull and part types, which may be re-enabled in the future.
- Uses assertions and retail asserts for debugging and error checking.

## `src/Igc/sideigc.h`

**PURPOSE:** The `src/Igc/sideigc.h` file defines the class `CsideIGC`, which manages the state and behavior of a side (player) in the Allegiance game. It includes functions for initializing, updating, and terminating the side, as well as managing various attributes such as civilization, techs, and resources.

**KEY PARTS:**
- **Class:** CsideIGC
  - Inherits from IsideIGC.
  - Manages a player's state in a mission.
  
- **Function:** Initialize
  - Initializes the side with a mission, current time, and data.
  
- **Function:** Terminate
  - Cleans up resources when the side is terminated.
  
- **Function:** Update
  - Updates the state of buckets attached to the station.
  
- **Function:** Export
  - Exports the side's data to a buffer.
  
- **Function:** SetCivilization
  - Sets the civilization for the side and updates related tech bits and station types.
  
- **Function:** DestroyBuckets
  - Destroys all buckets associated with the side.
  
- **Function:** CreateBuckets
  - Creates new buckets for the side.
  
- **Function:** GetTechs
  - Returns a bitmask of all techs available to the side.

**CONNECTS TO:**
- IclusterIGC (for handling enemy clusters)
- Mission management system (for notifying changes in techs and resources)

**NOTES:**
- None.

## `src/Igc/sounds.h`

PURPOSE: The file `sounds.h` defines sound IDs and resources used in the Allegiance game for various in-game events and voice-over messages.

KEY PARTS:
- DEFSOUND: Macro to define sound IDs.
- DEFEMPTYSOUND: Macro to define unused sound IDs.
- newShip: Sound ID for a new ship being created.
- jump: Sound ID for a player jumping.
- voAwsome: Voice-over sound indicating something is awesome.
- voYoureCrazy: Defines a sound for when someone says they are crazy.
- voThatHurt: Defines a sound for when something hurts.
- droneReport4Duty: Defines a sound for a drone reporting for duty.
- salCommenceScan: Defines a sound for sensors online commencing scan.
- oink1: Defines a sound for a pig response.
- tm_slide_1_01: Defines a sound for a training mission slide.

CONNECTS TO: Other game systems and files that use these sound IDs, such as the audio engine or voice-over scripts.

NOTES:
- Multiple TODOs exist to remove unused music sounds or reactivate commented-out sounds.

## `src/Igc/stationIGC.cpp`

**PURPOSE:** The `stationIGC.cpp` file manages station objects within the Allegiance game. It handles initialization, export, damage reception, and ship management for stations.

**KEY PARTS:**
- **Class: CstationIGC**: Manages station objects.
  - **Function: Initialize**: Initializes a station with mission data.
  - **Function: Export**: Exports station data to a structured format.
  - **Function: ReceiveDamage**: Handles damage received by the station.
  - **Function: SetBaseStationType**: Changes the base station type.
  - **Function: AddShip**: Adds a ship to the station's docked list.
  - **Function: DeleteShip**: Removes a ship from the station's docked list.
  - **Function: GetShip**: Retrieves a ship by its ID.
  - **Function: Launch**: Launches a ship from the station.
  - **Function: InGarage**: Checks if a ship can land at the station.
  - **Function: RepairAndRefuel**: Repairs and refuels a docked ship.

- **Class: MyStationType**: Represents a type of station with metadata about its properties and capabilities.
  - **Functions: GetName, GetDescription, GetPrice, GetTimeToBuild, GetGroupID, GetRequiredTechs, GetEffectTechs, GetSignature, GetRadius, GetScannerRange, GetMaxArmorHitPoints, GetArmorDefenseType, GetMaxShieldHitPoints, GetShieldDefenseType, GetArmorRegeneration, GetShieldRegeneration, GetIncome, GetLocalTechs, GetCapabilities**: Retrieve various properties and capabilities of the station type.

**CONNECTS TO:** Other game systems for mission management, ship handling, and global attributes.

**NOTES:** 
- Contains a TODO comment indicating a fix for a friendly fire bug with stations.

## `src/Igc/stationIGC.h`

PURPOSE: This file defines the CstationIGC class and its associated MyStationType class for managing stations in the Allegiance game. It includes methods for initialization, updating, damage handling, scanning, and station-specific operations.

KEY PARTS:
- CstationIGC: Manages a station's state, including hit points, shield fraction, and docked ships.
- MyStationType: Represents the type of a station, providing data like armor, shields, and sounds.
- Initialize: Sets up the station with mission data.
- Update: Handles updates to the station's state over time.
- ReceiveDamage: Processes damage received by the station.
- GetFraction: Returns the current fraction of hull hit points remaining.
- CanBuy: Determines if a buyable item can be purchased at the station.
- AddShip and DeleteShip: Manage ships docked at the station.

CONNECTS TO:
- ImissionIGC, ImodelIGC, IsideIGC, IclusterIGC, IshipIGC, and other game systems for mission management, modeling, side affiliation, clustering, ship operations, and more.

NOTES:
- Contains commented-out AddRef and Release methods.
- Includes additional properties and methods related to asteroids and station capabilities (Imago modifications).

## `src/Igc/stationtypeigc.cpp`

PURPOSE: This file contains the implementation of the CstationTypeIGC class, which is responsible for managing station types in the Allegiance game. It handles initialization, termination, and exporting station type data.

KEY PARTS:
- CstationTypeIGC: Manages station types within a mission.
- Initialize: Sets up the station type with mission data, including models, textures, and construction drones.
- Terminate: Releases resources and cleans up when the station type is no longer needed.
- Export: Saves the station type data to a buffer.

CONNECTS TO:
- ImissionIGC: The mission object that contains the station type.
- DroneType: Manages drone types used for construction.
- MultiHullBase: Handles 3D model data and frame information.

NOTES:
- Contains assertions and debug code for validation and logging.
- Fixes occasional crashes related to uninitialized pointers or buffer overflows.

## `src/Igc/stationtypeigc.h`

PURPOSE: This file defines the CstationTypeIGC class, which represents a station type in the Allegiance game. It implements interfaces for various properties and behaviors of a station type.

KEY PARTS:
- CstationTypeIGC: Main class representing a station type.
- Initialize: Initializes the station type with mission data.
- Terminate: Cleans up resources when the station type is no longer needed.
- Export: Exports station type data to a buffer.
- GetObjectType, GetObjectID: Returns the object type and ID of the station type.
- GetData: Provides access to the station type's data.
- GetModelName, GetName, GetDescription, GetPrice, etc.: Retrieve various properties of the station type such as model name, price, income, etc.
- GetSuccessorStationType: Determines the successor station type based on available technologies.
- GetLaunchPosition, GetLaunchDirection, etc.: Provides launch and landing positions and directions for the station.

CONNECTS TO: igc.h (interface definitions), DataStationTypeIGC (data structure), IdroneTypeIGC (construction drone type).

NOTES: None.

## `src/Igc/treasureIGC.cpp`

PURPOSE:
The file `treasureIGC.cpp` implements the `CtreasureIGC` class, which manages treasures in a game mission. It handles initialization, updating, and exporting of treasure data.

KEY PARTS:
- CtreasureIGC: Manages the lifecycle and properties of a treasure object.
- Initialize: Sets up the treasure based on initial data and mission context.
- Update: Updates the position and state of the treasure over time.
- Export: Serializes the treasure's current state for network transmission or storage.

CONNECTS TO:
- ImissionIGC: The mission interface that provides game context and resources.
- DataTreasureIGC: Struct containing initial data for a treasure.
- IpartTypeIGC, IbuyableIGC, IsideIGC: Interfaces for different types of treasures and related entities.

NOTES:
- Contains assertions and debug checks to ensure data integrity and correct initialization.
- Handles the transition from moving to stationary states based on velocity and time.
- Uses `LoadDecal` and `Load` functions to set visual properties of the treasure.

## `src/Igc/treasureIGC.h`

PURPOSE:
The file `treasureIGC.h` defines the header for the `CtreasureIGC` class, which represents a treasure in the game Allegiance. This class inherits from `TmodelIGC<ItreasureIGC>` and implements various interfaces to handle treasure-related functionalities.

KEY PARTS:
- **Class: CtreasureIGC**
  - Inherits from `TmodelIGC<ItreasureIGC>`
  - Implements methods for initialization, termination, updating, exporting, and handling collisions.
  
- **Methods:**
  - `Initialize`: Initializes the treasure with mission data.
  - `Terminate`: Cleans up resources when the treasure is terminated.
  - `Update`: Updates the state of the treasure over time.
  - `Export`: Exports the treasure's data to a buffer.
  - `SetCluster`: Manages the cluster containing the treasure.
  - `HandleCollision`: Handles collisions with other game objects, particularly ships and static objects.
  - `GetTreasureCode`, `GetBuyable`, `GetTreasureID`, `SetTreasureID`, `GetAmount`, `SetAmount`, `ResetExpiration`, `SetCreateNow`: Accessors and mutators for treasure properties.

- **Data Members:**
  - `m_data`: Contains the treasure's data.
  - `m_buyable`: Pointer to a buyable object associated with the treasure.
  - `m_expire`: Time when the treasure expires.
  - `m_speed0`: Initial speed of the treasure.
  - `m_stationaryF`: Flag indicating if the treasure is stationary.

CONNECTS TO:
- Depends on other classes and interfaces such as `ImissionIGC`, `IclusterIGC`, `IshipIGC`, `IIgcSite`, and `IbuyableIGC`.

NOTES:
- None

## `src/Igc/treasuresetigc.cpp`

PURPOSE:
The file `treasuresetigc.cpp` contains the implementation of the `CtreasureSetIGC` class, which manages treasure sets in a game mission. It handles initialization and exporting of treasure data.

KEY PARTS:
- `Initialize`: Initializes the treasure set with mission data.
- `Export`: Exports the current state of the treasure set to a buffer.

CONNECTS TO:
- `ImissionIGC`: The mission interface that the treasure set is associated with.

NOTES:
- None.

## `src/Igc/treasuresetigc.h`

PURPOSE:
This file defines the CtreasureSetIGC class, which represents a treasure set in the Allegiance game. It implements interfaces for managing and accessing treasure data within a mission.

KEY PARTS:
- CtreasureSetIGC: Manages a collection of treasures with methods to initialize, terminate, export, and retrieve treasure data.
- Initialize: Sets up the treasure set with mission context and initial data.
- AddTreasureData: Adds a new treasure entry to the set, reallocating memory if necessary.
- GetRandomTreasureData: Selects a random treasure based on its chance of occurrence.

CONNECTS TO:
igc.h - Contains base interfaces for game objects.

NOTES:
- The class uses dynamic memory allocation and reallocation for managing treasure data arrays.
- It includes assertions to ensure valid state during operations.

## `src/Igc/warpIGC.cpp`

PURPOSE: The `warpIGC.cpp` file contains the implementation of the `CwarpIGC` class, which is responsible for managing warp points in a game mission. Warp points are locations where players can teleport to different parts of the game world.

KEY PARTS:
- `Initialize`: Initializes the warp point with data from an IGC (Game Content) file.
- `Export`: Exports the current state of the warp point to an IGC file format.
- `DataWarpIGC`: A structure used for storing warp point data.
- `LoadWarp`: Loads the visual and physical properties of the warp point.
- `m_warpDef`, `m_bFixedPosition`, `m_MassLimit`: Member variables that store specific attributes of the warp point, such as its definition, whether it is fixed in position, and mass limits.

CONNECTS TO: The game's mission management system (`ImissionIGC`), cluster system (`IclusterIGC`).

NOTES:
- Contains a hack to handle unmovable alephs (objects with fixed positions) by modifying the name.
- Adds support for mass-limited alephs, which can be identified by a leading '+' in their names and have their mass specified after a ':'.

## `src/Igc/warpIGC.h`

PURPOSE: This file defines the CwarpIGC class, which represents a warp point in the game Allegiance. It handles initialization, termination, updates, and bomb management for warps.

KEY PARTS:
- CwarpIGC: Main class representing a warp point.
- Initialize: Initializes the warp with mission data.
- Terminate: Cleans up resources when the warp is destroyed.
- Update: Handles bomb explosions and updates the state of the warp.
- Export: Exports the warp's data.
- GetObjectType: Returns the object type as OT_warp.
- GetObjectID: Returns the unique ID of the warp.
- SetCluster: Sets the cluster for the warp and manages its list.
- GetDestination: Retrieves the destination warp point.
- AddBomb: Adds a bomb to the warp with specified explosion time and missile type.
- GetBombs: Returns the list of bombs associated with the warp.
- IsFixedPosition: Checks if the warp has a fixed position.
- MassLimit: Returns the mass limit for the warp (added by Andon).

CONNECTS TO: igc.h, modelIGC.h, ImissionIGC, IwarpIGC, ImissileTypeIGC, IclusterIGC, ThingSite.

NOTES:
- The class uses ATL wizard-generated code.
- It includes functionality to handle bomb explosions and create visual effects.
- A fixed position feature was added by KG.
- Mass limit functionality was added by Andon.

## `src/Igc/weaponIGC.cpp`

**PURPOSE:**  
The `weaponIGC.cpp` file implements the `CweaponIGC` class, which manages weapon-related functionalities in the Allegiance game. This includes initialization, termination, setting the ship and mount ID, and firing weapons.

**KEY PARTS:**
- **Class:** `CweaponIGC`
  - Manages weapon operations.
  
- **Function:** `Initialize(ImissionIGC* pMission, Time now, const void* data, int dataSize)`
  - Initializes the weapon with mission data.
  
- **Function:** `Terminate(void)`
  - Cleans up resources when the weapon is no longer needed.
  
- **Function:** `SetShip(IshipIGC* newVal, Mount mount)`
  - Sets the ship and mount ID for the weapon.
  
- **Function:** `SetMountID(Mount newVal)`
  - Updates the mount ID and related properties of the weapon.
  
- **Function:** `FireWeapon(Time now)`
  - Handles the logic for firing the weapon based on current game state.

**CONNECTS TO:**
- Depends on classes like `ImissionIGC`, `IshipIGC`, `IhullTypeIGC`, `IclusterIGC`, and others from the Allegiance framework.

**NOTES:**  
- None.

## `src/Igc/weaponIGC.h`

PURPOSE: This file defines the CweaponIGC class, which implements the IweaponIGC interface for managing weapons in the Allegiance game. It handles weapon initialization, activation, firing, and state management.

KEY PARTS:
- CweaponIGC: Main class implementing IweaponIGC.
- Initialize: Initializes the weapon with mission data.
- Update: Updates the weapon's state based on time and game events.
- Activate: Activates the weapon.
- Deactivate: Deactivates the weapon.
- FireWeapon: Handles the logic for firing a weapon.
- GetObjectType, GetEquipmentType, GetPartType, GetShip: Accessor methods for various properties of the weapon.

CONNECTS TO: ImissionIGC, IshipIGC, IpartTypeIGC, IprojectileTypeIGC, and other game systems.

NOTES: None.
