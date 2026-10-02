# src/training

[Back to the index](../LLAMA_SUMMARY.md) - 180 files, 16,282 lines. Notes by a local model, to orient by: not verified facts.

The `src/training` folder in Allegiance contains essential components for managing and executing training missions within the game. These files define conditions, actions, and goals necessary for players to learn specific game mechanics.

- AbstractTarget.cpp: Implements a base class for different target types used in game training.
- Action.cpp: Contains the implementation of the "action" interface within the training library.
- Mission2.h: Defines the header for a specific training mission interface named "Mission2".
- Training.cpp: Manages the core functionality for training missions, including initialization and execution.

Largest files: mission3.cpp (1159 lines), mission4.cpp (1045 lines), mission6.cpp (974 lines), mission10.cpp (970 lines), trainingmission.cpp (935 lines).

## `src/training/AbstractTarget.cpp`

PURPOSE:
This file contains the implementation of the `AbstractTarget` interface in the training library for Allegiance. The `AbstractTarget` class is designed to provide a base class for various target types used in game training.

KEY PARTS:
- `AbstractTarget::~AbstractTarget(void)`: Destructor for the `AbstractTarget` class.
- `ImodelIGC* AbstractTarget::operator -> (void)`: Overloaded operator to allow pointer-like access to the `ImodelIGC` interface.

CONNECTS TO:
- `pch.h`: Precompiled header file.
- `AbstractTarget.h`: Header file declaring the `AbstractTarget` interface.

NOTES:
- None.

## `src/training/AbstractTarget.h`

**PURPOSE:** This file defines an interface for abstract targets in the training library of Allegiance. It provides a base class with virtual functions that derived classes must implement.

**KEY PARTS:**
- **AbstractTarget:** Base class defining an interface for abstract targets.
  - `~AbstractTarget(void):` Virtual destructor to ensure proper cleanup.
  - `operator ImodelIGC*(void) = 0;`: Pure virtual function that derived classes must implement, returning a pointer to an `ImodelIGC` object.
  - `operator ->(void);`: Overloaded operator to allow using the arrow operator on instances of `AbstractTarget`.

**CONNECTS TO:** 
- ImodelIGC.h: The interface for model objects.

**NOTES:** None.

## `src/training/Action.cpp`

PURPOSE: This file contains the implementation of the training library's "action" interface in Allegiance.

KEY PARTS:
- Action::~Action(): Destructor for the Action class.
- Action::Stop(): Function to stop an action.

CONNECTS TO: pch.h, Action.h

NOTES: None.

## `src/training/Action.h`

PURPOSE:
The file `Action.h` defines an interface for actions within the training library of Allegiance. It provides a base class with virtual functions to execute and stop actions.

KEY PARTS:
- **Class: Action**
  - **Destructor:** Virtual destructor to ensure proper cleanup.
  - **Execute:** Pure virtual function that must be implemented by derived classes to define action execution logic.
  - **Stop:** Virtual function to stop an action, with a default implementation that does nothing.

CONNECTS TO:
- Other training library components or systems within Allegiance.

NOTES:
- None.

## `src/training/AddRespawnAction.cpp`

PURPOSE:
This file contains the implementation of the AddRespawnAction class, which is responsible for adding a respawn action to a training mission.

KEY PARTS:
- AddRespawnAction: A class that represents an action to add a respawn point in a training mission.
  - Constructor initializes the action with parameters such as CreateDroneAction pointer, downtime, spawn group, and whether to spawn on addition.
  - Destructor is empty.
  - Execute: Method that calls the AddRespawn method of the global TrainingMission instance to add the respawn action.

CONNECTS TO:
- TrainingMission.h: The class that manages training missions and includes the AddRespawn method.

NOTES:
- None.

## `src/training/AddRespawnAction.h`

PURPOSE: This file defines a class `AddRespawnAction` that extends the base `Action` class. It is used to manage the respawn of drones in a game.

KEY PARTS:
- `AddRespawnAction`: A class representing an action to add a drone with specified parameters.
  - Constructor: Initializes the action with a `CreateDroneAction`, downtime, spawn group, and whether to spawn on addition.
  - Destructor: Cleans up resources.
  - `Execute`: Executes the action to respawn the drone.

CONNECTS TO:
- `Action.h`: Inherits from the base `Action` class.
- `CreateDroneAction.h`: Uses a `CreateDroneAction` object for creating drones.

NOTES: None.

## `src/training/CommandAcknowledgedCondition.cpp`

PURPOSE: This file implements a condition in the training library that checks if a specific command has been acknowledged.

KEY PARTS:
- CommandAcknowledgedCondition: A class representing a condition that evaluates whether a target command has been acknowledged.
- Evaluate: A method that returns true if the current command acknowledged matches the target command, otherwise false.
- SetCommandAcknowledged: A method to set the current command acknowledged state.
- m_cTargetCommand: A member variable storing the target command to be checked.
- m_cCommandAcknowledged: A static member variable storing the current command acknowledged state.

CONNECTS TO: Training library

NOTES: None.

## `src/training/CommandAcknowledgedCondition.h`

PURPOSE: This file defines a condition class in the training library that checks if a specific command has been acknowledged.

KEY PARTS:
- CommandAcknowledgedCondition: A class derived from Condition that evaluates whether a target command has been acknowledged.
- Evaluate: A virtual function that returns true if the target command has been acknowledged.
- SetCommandAcknowledged: A static function to set the current acknowledged command.

CONNECTS TO: Condition.h

NOTES: None.

## `src/training/Condition.cpp`

**PURPOSE:**  
This file contains the implementation of the condition interface for the training library in Allegiance. It defines the lifecycle methods for conditions used in training scenarios.

**KEY PARTS:**  
- `Condition::~Condition()`: Destructor for the Condition class.
- `bool Condition::Start(void)`: Starts the condition by evaluating it immediately.
- `void Condition::Stop(void)`: Stops the condition, though its implementation is currently empty.

**CONNECTS TO:**  
`Condition.h`

**NOTES:**  
None.

## `src/training/Condition.h`

PURPOSE:
This file defines the interface for conditions in the training library of Allegiance. Conditions are used to evaluate certain states or events during gameplay.

KEY PARTS:
- Class Condition: Base class for all condition types.
  - ~Condition(): Virtual destructor.
  - Start(): Initializes the condition.
  - Stop(): Cleans up the condition.
  - Evaluate(): Pure virtual function that must be implemented by derived classes to check if the condition is met.

CONNECTS TO:
None

NOTES:
- None

## `src/training/CreateDroneAction.cpp`

PURPOSE:
The file `CreateDroneAction.cpp` implements the training library interface for creating drone ships in Allegiance. It handles the creation and configuration of drone ships with various attributes such as hull type, pilot type, tech level, position, orientation, and command targets.

KEY PARTS:
- `CreateDroneAction`: Class representing an action to create a drone ship.
  - Constructor initializes the drone's properties.
  - Destructor cleans up resources if necessary.
  - `Execute`: Method that creates the drone ship with specified attributes and upgrades based on tech level.
  - Various setter methods (`SetCreatedLocation`, `SetCreatedOrientation`, etc.) to configure the drone's properties before execution.

CONNECTS TO:
- `TrainingMission`: The mission class where drones are created.
- `trekClient`: Core client interface for accessing game resources like models, parts, and clusters.

NOTES:
- Contains debug print statements for logging purposes.
- Includes logic to upgrade weapons, boosters, and missiles based on the tech level.
- Handles setting up command targets and behaviors for the drone.

## `src/training/CreateDroneAction.h`

PURPOSE:
The file defines the `CreateDroneAction` class, which is part of the training library for creating drone actions in Allegiance. This class inherits from the `Action` base class and provides methods to configure and execute the creation of a drone.

KEY PARTS:
- Class: `CreateDroneAction`
  - Inherits from: `Action`
  - Methods: Configure various properties of the drone (e.g., name, location, orientation, tech level) and execute the action.
  
- Data Members:
  - `m_name`: Name of the action
  - `m_shipID`, `m_hullID`, `m_sideID`, `m_pilotType`: Drone attributes
  - `m_sectorID`, `m_position`, `m_orientation`: Location and orientation details
  - `m_stationTypeID`, `m_expendableTypeID`: Additional drone properties
  - `m_SpawnAtShipID`, `m_SpawnAtShipOffset`: Details for spawning the drone at a specific ship
  - `m_techLevel`, `m_commandTargetID`, `m_commandTargetType`, `m_commandID`: Command and behavior settings
  - `b_addedConsumer`: Flag indicating if a consumer has been added

CONNECTS TO:
- Depends on `Action.h` for the base class definition.

NOTES:
- None.

## `src/training/CreateObjectAction.cpp`

**PURPOSE:** This file contains the implementation of the `CreateObjectAction` class within the training library, which is responsible for creating objects in a game mission.

**KEY PARTS:**
- **Class: CreateObjectAction**
  - **Constructor:** Initializes the object type, data pointer, and data size.
  - **Destructor:** Sets the result to 0.
  - **Method: Execute**
    - Calls `CreateObject` on the mission interface with the last update time, object type, data, and data size.

**CONNECTS TO:** 
- `trekClient.GetCore()` which provides access to the mission interface.

**NOTES:** None.

## `src/training/CreateObjectAction.h`

PURPOSE:
The file defines the `CreateObjectAction` class, which is part of a training library in Allegiance. This class represents an action for creating objects in the game.

KEY PARTS:
- `CreateObjectAction`: A class that inherits from `Action`. It is responsible for creating objects based on specified type and data.
  - Constructor: Initializes the object type, data pointer, and data size.
  - Destructor: Cleans up any resources used by the action.
  - `Execute`: Method to perform the creation of the object.
- `ObjectType`: An enumeration or type representing different types of objects that can be created.
- `m_pData` and `m_iDataSize`: Member variables to store additional data needed for object creation.
- `m_result`: A reference to an interface (`IbaseIGC`) that might hold the result of the object creation.

CONNECTS TO:
- Depends on `Action.h`, which defines the base class for actions in the training library.

NOTES:
- None.

## `src/training/CreateProbeAction.cpp`

PURPOSE:
This file contains the implementation of the `CreateProbeAction` class, which is part of a training library in Allegiance. The class is responsible for creating probe objects in the game.

KEY PARTS:
- `CreateProbeAction`: A constructor that initializes the action with probe ID, type ID, position, side ID, and sector ID.
- `~CreateProbeAction`: Destructor that does nothing.
- `Execute`: Method to execute the creation of a probe object by setting up its data and calling the base class's `Execute` method.

CONNECTS TO:
- `trekClient.GetCore()`: Retrieves the mission core interface for accessing game state.
- `IprobeTypeIGC`: Interface for probe types.
- `ISideIGC`: Interface for sides (player factions).
- `IClusterIGC`: Interface for clusters (sectors).

NOTES:
- None.

## `src/training/CreateProbeAction.h`

PURPOSE:
This header file defines the `CreateProbeAction` class within the training library of Allegiance. It inherits from `CreateObjectAction` and is responsible for creating probe objects in the game.

KEY PARTS:
- `CreateProbeAction`: A class that extends `CreateObjectAction` to handle the creation of probe objects.
  - Constructor: Initializes a new instance with parameters such as probe ID, type ID, position, side ID, sector ID, and probe data.
  - Destructor: Cleans up resources when an instance is destroyed.
  - `Execute`: A virtual function that executes the action of creating a probe object.

CONNECTS TO:
- `CreateObjectAction.h`: This file includes the base class for creating objects in the game.

NOTES:
- None.

## `src/training/CreateWaypointAction.cpp`

PURPOSE: This file contains the implementation of the `CreateWaypointAction` class, which is part of a training library for creating waypoints in a game.

KEY PARTS:
- `CreateWaypointAction`: Constructor and destructor for the action that creates a waypoint.
- `Execute`: Method to execute the creation of the waypoint by setting its properties and calling the base class method.

CONNECTS TO: `trekClient.GetCore()`, which is used to get the mission interface.

NOTES: None.

## `src/training/CreateWaypointAction.h`

PURPOSE:
This header file defines the `CreateWaypointAction` class within the training library of Allegiance. It inherits from `CreateObjectAction` and is responsible for creating a waypoint object in the game.

KEY PARTS:
- `CreateWaypointAction`: A class that extends `CreateObjectAction` to create a specific type of object, a waypoint.
  - Constructor: Initializes the waypoint with a buoy ID, position, sector ID, type, and visibility.
  - Destructor: Cleans up any resources associated with the waypoint.
  - `Execute`: Method to execute the creation of the waypoint in the game.

CONNECTS TO:
- `CreateObjectAction.h`: The base class for creating objects in the training library.

NOTES:
- None.

## `src/training/CurrentTarget.cpp`

PURPOSE:
The file `CurrentTarget.cpp` implements the training library's "CurrentTarget" interface, which manages and retrieves the current target of a ship in Allegiance.

KEY PARTS:
- `class CurrentTarget`: Manages the current target of a ship.
  - Constructor: Initializes the target and caching options.
  - Destructor: Cleans up allocated resources.
  - `operator ImodelIGC*()`: Retrieves the current target, optionally caching it to handle changes in targets without elimination.

CONNECTS TO:
- `TypeIDTarget.h`: Used for caching target information.
- `IshipIGC.h` and `ImodelIGC.h`: Interfaces for ship and model objects, respectively.

NOTES:
- The class handles caching of the target to manage cases where the target might change before elimination.
- Assertions are used to ensure that the ship interface is correctly cast.

## `src/training/CurrentTarget.h`

PURPOSE:
This header file defines the `CurrentTarget` class within the Allegiance game's training library. The `CurrentTarget` class is an implementation of the `AbstractTarget` interface and manages a target for training purposes.

KEY PARTS:
- **Class: CurrentTarget**
  - Inherits from `AbstractTarget`.
  - Manages a target (`m_pTarget`) and optionally locks on to it upon first fetch.
  - Provides an operator to cast to `ImodelIGC*`.

- **Constructor: CurrentTarget**
  - Initializes the target pointer and lock-on flag.

- **Destructor: ~CurrentTarget**
  - Cleans up any resources.

- **Operator: operator ImodelIGC***
  - Allows casting of `CurrentTarget` instances to `ImodelIGC*`.

CONNECTS TO:
- Depends on `AbstractTarget.h` for its base class implementation.

NOTES:
- None.

## `src/training/DoDamageAction.cpp`

PURPOSE: This file implements a training library action that deals damage to a ship based on whether the hull is damaged.

KEY PARTS:
- DoDamageAction: A class representing an action that can deal damage to a ship.
- Execute: A method within DoDamageAction that performs the damage calculation and application.
- m_bHull: A boolean member variable indicating whether the action targets the hull or shield of the ship.
- IshipIGC, IshieldIGC: Interfaces for accessing ship and shield properties.
- trekClient.GetShip(): Retrieves the current ship object.
- pShield->GetFraction(): Gets the fraction of shield strength remaining.
- pShield->SetFraction(0.2f): Sets the shield fraction to 20%.
- pShip->ReceiveDamage(): Applies damage to the ship.

CONNECTS TO: trekClient, IshipIGC, IshieldIGC

NOTES: None

## `src/training/DoDamageAction.h`

PURPOSE:
The file defines a header for the "DoDamageAction" class within the training library of Allegiance. This class is derived from the base "Action" class and is used to represent actions that deal damage.

KEY PARTS:
- DoDamageAction: A class representing an action that deals damage, inheriting from Action.
  - Constructor: Initializes a new instance of the DoDamageAction class with a boolean indicating whether the damage affects the hull.
  - Destructor: Cleans up resources associated with the DoDamageAction object.
  - Execute: Executes the damage action.
- m_bHull: A protected member variable that indicates whether the damage affects the hull.

CONNECTS TO:
Action.h

NOTES:
None.

## `src/training/DoDamageAdjustableAction.cpp`

PURPOSE:
This file contains the implementation of a training library action that adjusts damage to a ship.

KEY PARTS:
- DoDamageAdjustableAction: Class representing an action that deals adjustable damage to a ship.
- Execute: Function that applies the damage and sets various control states on the target ship.

CONNECTS TO:
DoDamageAdjustableAction depends on the trekClient, Time, Vector, IshipIGC, and ControlData classes.

NOTES:
None.

## `src/training/DoDamageAdjustableAction.h`

PURPOSE: This file defines the header for a training library action class named `DoDamageAdjustableAction`, which is used to adjust damage dealt by a ship.

KEY PARTS:
- `DoDamageAdjustableAction`: A class that inherits from `Action` and is responsible for adjusting damage dealt by a specified ship.
  - Constructor: Initializes with a `ShipID` and a `float` representing the amount of damage adjustment.
  - Destructor: Cleans up any resources used by the action.
  - `Execute`: Virtual function to perform the action, which in this case adjusts damage.

CONNECTS TO:
- `Action.h`: The base class for actions in the training library.

NOTES:
- None.

## `src/training/GetAutopilotCondition.cpp`

PURPOSE: This file contains the implementation of a class that evaluates whether a ship has autopilot enabled.

KEY PARTS:
- GetAutopilotCondition: Class that implements an interface to check if a ship's autopilot is active.
  - Constructor initializes with a reference to the ship.
  - Destructor sets the ship reference to null.
  - Evaluate method returns the state of the ship's autopilot.

CONNECTS TO: src/training/GetAutopilotCondition.h (header file for interface)

NOTES: None.

## `src/training/GetAutopilotCondition.h`

PURPOSE: This header file defines the interface and implementation for a condition used in training that checks the autopilot status of a ship.

KEY PARTS:
- Class GetAutopilotCondition: Inherits from Condition and evaluates whether the autopilot is active on a given ship.
- Constructor: Initializes with a reference to an IshipIGC object representing the ship.
- Destructor: Cleans up resources.
- Evaluate: Returns true if the autopilot is active, otherwise false.

CONNECTS TO: Condition.h (base class)

NOTES: None.

## `src/training/GetChatCondition.cpp`

**PURPOSE:**  
This file implements a training library interface called `GetChatCondition` which is used to check if a specific chat message has been received by the expected recipient during a training mission.

**KEY PARTS:**
- **Class: GetChatCondition**
  - Manages the condition for receiving a chat message from an expected recipient.
- **Function: Start()**
  - Initializes the chat condition and sets it in the global `g_pMission`.
- **Function: Evaluate()**
  - Evaluates whether the expected chat message has been received and resets the state.
- **Function: RecordChat()**
  - Records a chat message and checks if it matches the expected recipient.

**CONNECTS TO:**  
`TrainingMission.h`

**NOTES:**  
None.

## `src/training/GetChatCondition.h`

PURPOSE:
This header file defines the `GetChatCondition` class within the Allegiance training library. It is used to check if a specific chat message has been received by a player.

KEY PARTS:
- GetChatCondition: A class that inherits from `Condition`. It checks if a chat message was received by an expected recipient.
  - Constructor: Initializes with the expected recipient of the chat.
  - Destructor: Cleans up any resources.
  - Start: Begins the condition evaluation process.
  - Evaluate: Checks if the expected chat has been received.
  - RecordChat: Records that a chat message was received from a specific recipient.

CONNECTS TO:
- Condition.h: The base class for conditions in the training library.

NOTES:
- None.

## `src/training/GetCommandCondition.cpp`

PURPOSE:
This file implements the `GetCommandCondition` class within the training library of Allegiance. It is responsible for evaluating whether a ship has a specific command assigned to it.

KEY PARTS:
- Class: `GetCommandCondition`
  - Constructor: Initializes with either an `IshipIGC*` or an `ObjectID`, setting up a target ship and the command to check.
  - Destructor: Cleans up the allocated memory for the target ship.
  - Method: `Evaluate`
    - Checks if the target ship has the specified command (`c_cmdAccepted` or `c_cmdCurrent`) assigned.

CONNECTS TO:
- Depends on classes from the Allegiance game engine, including `IshipIGC`, `ImodelIGC`, and `TypeIDTarget`.

NOTES:
- None.

## `src/training/GetCommandCondition.h`

PURPOSE:
This header file defines the `GetCommandCondition` class within the Allegiance training library. It is used to evaluate whether a ship has been given a specific command.

KEY PARTS:
- GetCommandCondition: A class that inherits from Condition, designed to check if a ship has received a particular command.
  - Constructor: Initializes the condition with either an IshipIGC pointer or an ObjectID for the ship and a CommandID for the command.
  - Destructor: Cleans up resources when the object is destroyed.
  - Evaluate: Checks if the specified command has been issued to the ship.

CONNECTS TO:
- Condition.h
- AbstractTarget.h

NOTES:
None.

## `src/training/GetControlActiveCondition.cpp`

PURPOSE: This file implements a condition that checks if a ship's control input is active based on a specified axis and expected value.

KEY PARTS:
- GetControlActiveCondition class: Manages the condition for checking control input.
- Evaluate method: Determines if the control input meets the expected criteria.
- g_inputControls global variable: Stores player input controls like yaw, pitch, roll, and throttle.

CONNECTS TO: src/training/GetControlActiveCondition.h

NOTES: None.

## `src/training/GetControlActiveCondition.h`

PURPOSE:
This header file defines a class `GetControlActiveCondition` within the Allegiance game's training library. The class is used to check if a ship's control input along a specified axis matches an expected value.

KEY PARTS:
- `GetControlActiveCondition`: A class derived from `Condition` that evaluates whether a ship's control input on a given axis meets a specified threshold.
  - Constructor: Initializes the condition with a ship reference, an axis, an expected value, and an optional flag to use input controls.
  - Destructor: Cleans up resources associated with the condition.
  - `Evaluate`: Checks if the current control input of the ship along the specified axis matches the expected value.

CONNECTS TO:
- `Condition.h`: The base class for conditions used in the training library.

NOTES:
- None.

## `src/training/GetKeyCondition.cpp`

**PURPOSE:**  
This file implements a training library condition that checks if a specific key is pressed during a mission.

**KEY PARTS:**  
- `GetKeyCondition`: A class representing the condition to check for a key press.
  - `GetKeyCondition(TrekKey key)`: Constructor initializes with a specific key and sets the initial state as not pressed.
  - `~GetKeyCondition()`: Destructor cleans up any resources if necessary.
  - `Start(void)`: Registers the condition with the mission and resets the pressed state to false.
  - `Evaluate(void)`: Returns whether the key was pressed since the last evaluation and resets the state.
  - `KeyPressed(TrekKey key)`: Updates the internal state to true if the specified key is pressed.

**CONNECTS TO:**  
- `TrainingMission.h`: The mission class that manages conditions like this one.

**NOTES:**  
- None.

## `src/training/GetKeyCondition.h`

PURPOSE:
The file defines a header for the "GetKeyCondition" class within the training library of Allegiance. This class is used to check if a specific key has been pressed.

KEY PARTS:
- GetKeyCondition: A class derived from Condition that checks if a specified key has been pressed.
  - Constructor: Initializes with a TrekKey parameter.
  - Destructor: Cleans up resources.
  - Start: Starts the condition evaluation process.
  - Evaluate: Checks if the specified key is currently pressed.
  - KeyPressed: Updates the internal state when a key press event occurs.

CONNECTS TO:
Condition.h

NOTES:
None.

## `src/training/GetPickedCondition.cpp`

PURPOSE:
This file implements a condition in the training library that checks if an object is currently picked by the player.

KEY PARTS:
- GetPickedCondition: Class representing the condition to check if an object is picked.
- Evaluate: Function that evaluates whether the current picked object matches the target object.
- TypeIDTarget: Class used to store and retrieve the target object type and ID.
- GetWindow, GetConsoleImage, GetConsoleData, GetPickedObject: Functions/methods accessed to retrieve the currently picked object.

CONNECTS TO:
- Training library
- Allegiance game engine

NOTES:
None.

## `src/training/GetPickedCondition.h`

PURPOSE: This header file defines the `GetPickedCondition` class within the training library, which is used to evaluate whether a specific object has been picked.

KEY PARTS:
- `GetPickedCondition`: A class that inherits from `Condition`. It checks if an object of a specified type and ID has been picked.
- `Evaluate`: A virtual function that returns `true` if the target object has been picked, otherwise `false`.
- `m_pTarget`: A protected member variable that holds a pointer to an `AbstractTarget`, representing the target object.

CONNECTS TO: `Condition.h`, `AbstractTarget.h`

NOTES: None

## `src/training/GetProbeCountCondition.cpp`

PURPOSE:
This file contains the implementation of a condition class in the Allegiance game's training library that checks if the number of probes matches a specified count.

KEY PARTS:
- GetProbeCountCondition: Class representing a condition that evaluates whether the current probe count equals a given value.
- Evaluate: Function that returns true if the current probe count matches the stored value, otherwise false.

CONNECTS TO:
GetProbeCountCondition depends on trekClient.GetCore()->GetProbes() to retrieve the current probe count.

NOTES:
None.

## `src/training/GetProbeCountCondition.h`

PURPOSE:
This header file defines a condition class for the training library in Allegiance. It checks if the number of probes matches a specified count.

KEY PARTS:
- GetProbeCountCondition: A class that inherits from Condition and evaluates whether the current probe count matches the specified count.
- Evaluate: A virtual function that returns true if the current probe count equals m_iProbeCount, otherwise false.
- m_iProbeCount: An integer member variable representing the required probe count.

CONNECTS TO:
Condition.h

NOTES:
None.

## `src/training/GetRadarLODCondition.cpp`

PURPOSE:
This file implements a condition used in training scenarios to check if the current radar level of detail (LOD) matches an expected value.

KEY PARTS:
- GetRadarLODCondition: Class representing the condition that checks if the radar LOD is as expected.
  - Constructor initializes with the expected radar LOD.
  - Destructor cleans up resources.
  - Evaluate method returns true if the current radar mode matches the expected LOD, otherwise false.

CONNECTS TO:
- RadarImage::RadarLOD: Enumerates different levels of detail for the radar display.
- GetWindow(): Function or class that provides access to the game window and its properties.

NOTES:
- None.

## `src/training/GetRadarLODCondition.h`

PURPOSE: This header file defines a condition class for the training library that checks if the radar image's level of detail (LOD) matches an expected value.

KEY PARTS:
- GetRadarLODCondition: A class derived from Condition that evaluates whether the current radar LOD matches the expected LOD.
- Evaluate: A virtual function that returns true if the actual radar LOD equals the expected LOD, otherwise false.
- m_expectedRadarLOD: A protected member variable storing the expected radar LOD.

CONNECTS TO: Condition.h

NOTES: None.

## `src/training/GetSectorCondition.cpp`

**PURPOSE:**  
This file implements the `GetSectorCondition` interface for a training library in Allegiance. It evaluates whether an object is located within a specific sector.

**KEY PARTS:**
- **Class: GetSectorCondition**
  - Manages the condition of an object being in a specified sector.
- **Constructor Overloads**
  - Initializes the `GetSectorCondition` object with different types of targets and sectors.
- **Destructor**
  - Cleans up the dynamically allocated `TypeIDTarget`.
- **Evaluate Method**
  - Checks if the model's cluster ID matches the specified sector ID.

**CONNECTS TO:**  
`ImodelIGC`, `TypeIDTarget`, `AbstractTarget`

**NOTES:**  
None.

## `src/training/GetSectorCondition.h`

PURPOSE:
This header file defines the `GetSectorCondition` class within the Allegiance game's training library. The class is used to evaluate conditions related to a sector in space.

KEY PARTS:
- `GetSectorCondition`: A class that inherits from `Condition`. It evaluates whether certain objects or models are present in a specified sector.
  - Constructors: Initialize the condition with different types of targets (model, object type and ID, abstract target) and a sector ID.
  - Destructor: Cleans up resources when an instance is destroyed.
  - `Evaluate`: A virtual function that checks if the condition is met based on the current state of the game.

CONNECTS TO:
- `Condition.h`: The base class for conditions.
- `AbstractTarget.h`: Defines abstract targets which can be used as conditions.

NOTES:
- None.

## `src/training/GetShipHasAmmoCondition.cpp`

PURPOSE:
This file implements a condition in the Allegiance game's training library that checks if a ship has a certain amount of ammo.

KEY PARTS:
- GetShipHasAmmoCondition: A class representing the condition that evaluates whether a ship has at least a specified amount of ammo.
  - Constructor: Initializes with a reference to a ship and a count of required ammo.
  - Destructor: Clears the ship reference.
  - Evaluate: Returns true if the ship's ammo is less than the required count, otherwise false.

CONNECTS TO:
- TRef<IshipIGC>: The interface for the ship object that provides the ammo count.

NOTES:
- None.

## `src/training/GetShipHasAmmoCondition.h`

PURPOSE: This header file defines a condition class for the training library that checks if a ship has a certain amount of ammo.

KEY PARTS:
- GetShipHasAmmoCondition: A class derived from Condition that evaluates whether a given ship has a specified number of ammo.
  - Constructor: Initializes with a reference to an IshipIGC object representing the ship and an optional ammo count (default is -1).
  - Destructor: Cleans up resources.
  - Evaluate: Returns true if the ship's ammo count meets or exceeds the specified amount, otherwise false.

CONNECTS TO: Condition.h

NOTES: None.

## `src/training/GetShipHasCargoCondition.cpp`

PURPOSE:
The file `GetShipHasCargoCondition.cpp` implements a condition in the Allegiance game's training library that checks if a ship has cargo of a specific type.

KEY PARTS:
- `GetShipHasCargoCondition`: A class representing the condition that evaluates whether a ship has cargo of a specified equipment type.
  - Constructor: Initializes with a reference to a ship and an equipment type.
  - Destructor: Cleans up by setting the ship reference to null.
  - `Evaluate`: Checks if any part of the ship matches the specified equipment type.

CONNECTS TO:
- `IshipIGC` and `PartListIGC`: Interfaces for accessing ship parts and their types.

NOTES:
- None.

## `src/training/GetShipHasCargoCondition.h`

PURPOSE: This file defines a condition class used in the training library to check if a ship has cargo in a specific part.

KEY PARTS:
- GetShipHasCargoCondition: A class that inherits from Condition and checks if a ship has cargo in a specified equipment part.
  - Constructor: Initializes with a reference to an IshipIGC object representing the ship and an EquipmentType indicating the part.
  - Destructor: Cleans up resources.
  - Evaluate: Returns true if the ship has cargo in the specified part, otherwise false.

CONNECTS TO:
- Condition.h: The base class for conditions used in training scenarios.

NOTES:
- None.

## `src/training/GetShipHasMountedCondition.cpp`

PURPOSE:
The file `GetShipHasMountedCondition.cpp` implements a condition in the training library that checks if a specific ship part is mounted.

KEY PARTS:
- `GetShipHasMountedCondition`: Class representing the condition to check if a ship part is mounted.
  - Constructor: Initializes with a reference to a ship and a part ID.
  - Destructor: Cleans up by setting the ship reference to null.
  - `Evaluate`: Method that checks if the specified part is mounted on the ship.

CONNECTS TO:
- `IshipIGC` and `PartListIGC`: Interfaces for accessing ship parts and their properties.

NOTES:
- None.

## `src/training/GetShipHasMountedCondition.h`

PURPOSE:
This file defines a condition class used in the training library to check if a specific part of a ship is mounted.

KEY PARTS:
- GetShipHasMountedCondition: A class that inherits from Condition and evaluates whether a specified part of a ship is mounted.
  - Constructor: Initializes with a reference to an IshipIGC object representing the ship and a PartID indicating the part to check.
  - Destructor: Cleans up resources.
  - Evaluate: Returns true if the specified part is mounted, otherwise false.

CONNECTS TO:
Condition.h: The base class for conditions used in the training library.

NOTES:
None.

## `src/training/GetShipIsDamagedCondition.cpp`

PURPOSE: This file implements a condition in the training library that checks if a ship is damaged based on its health fraction.

KEY PARTS:
- GetShipIsDamagedCondition: Class representing the condition to check if a ship is damaged.
- Evaluate: Function that evaluates whether the ship meets the specified damage threshold.
- m_pTarget: Pointer to an AbstractTarget representing the ship being checked.
- m_fFraction: Fraction of health at which the condition is considered met.

CONNECTS TO:
- TypeIDTarget: Used to identify and track the target ship.
- IdamageIGC: Interface for accessing the ship's damage information.

NOTES: None.

## `src/training/GetShipIsDamagedCondition.h`

PURPOSE:
This header file defines a condition class for the training library that checks if a ship is damaged by comparing its current health to a specified fraction of its maximum health.

KEY PARTS:
- GetShipIsDamagedCondition: A class derived from Condition that evaluates whether a ship is damaged based on its health.
  - Constructor overloads allow specifying the ship using different methods (damage interface, object type and ID, or abstract target).
  - Destructor cleans up any resources.
  - Evaluate method returns true if the ship's current health is less than the specified fraction of its maximum health.

CONNECTS TO:
- Condition.h: Base class for conditions.
- AbstractTarget.h: Interface for abstract targets that can be used to specify the ship.

NOTES:
- None.

## `src/training/GetShipIsDestroyedCondition.h`

PURPOSE:
This header file defines a macro to create a condition that checks if a ship is destroyed by negating the condition of whether the ship is damaged.

KEY PARTS:
- GetShipIsDestroyedCondition: Macro to create a NotCondition wrapping a GetShipIsDamagedCondition with 0.0f damage threshold.
- GetShipIsDestroyedCondition2: Similar macro but takes shipType and shipID as parameters.
- NotCondition: Class that negates the result of another condition.
- GetShipIsDamagedCondition: Class that checks if a ship is damaged.

CONNECTS TO:
GetShipIsDamagedCondition.h, NotCondition.h

NOTES:
None.

## `src/training/GetShipIsStoppedCondition.cpp`

PURPOSE:
This file implements a condition in the training library that checks if a ship is stopped.

KEY PARTS:
- GetShipIsStoppedCondition: Class representing the condition to check if a ship is stopped.
  - Constructor with IshipIGC reference and ShipID parameter.
  - Destructor to release resources.
  - Evaluate method to determine if the ship's velocity is below a certain threshold (indicating it is stopped).

CONNECTS TO:
- trekClient.GetSide()->GetShip(m_shipID): Retrieves the ship object using its ID.

NOTES:
- None.

## `src/training/GetShipIsStoppedCondition.h`

PURPOSE:
This header file defines a condition class for the training library that checks if a ship is stopped.

KEY PARTS:
- GetShipIsStoppedCondition: A class derived from Condition that evaluates whether a specified ship is stopped.
  - Constructor takes either an IshipIGC reference or a ShipID.
  - Destructor cleans up resources.
  - Evaluate method returns true if the ship is stopped, false otherwise.

CONNECTS TO:
Condition.h

NOTES:
None.

## `src/training/GetStateBitsCondition.cpp`

PURPOSE:
The file `GetStateBitsCondition.cpp` implements a condition in the training library that checks if specific bits are set in a ship's state.

KEY PARTS:
- `GetStateBitsCondition`: A class representing a condition that evaluates whether certain bits are set in a ship's state.
  - Constructor: Initializes with a reference to a ship and a bit mask.
  - Destructor: Clears the ship reference.
  - `Evaluate`: Checks if any of the specified bits are set in the ship's current state.

CONNECTS TO:
- Depends on `IshipIGC` interface for accessing ship state information.

NOTES:
- None.

## `src/training/GetStateBitsCondition.h`

PURPOSE: This file defines a condition class used in the training library of Allegiance, specifically for evaluating state bits related to a ship.

KEY PARTS:
- GetStateBitsCondition: A class that inherits from Condition and evaluates whether a ship's state bits match specified criteria.
  - Constructor: Initializes with a reference to an IshipIGC object (representing a ship) and an integer representing the state bits to check.
  - Destructor: Cleans up resources.
  - Evaluate: Checks if the ship's current state bits match the specified bits.

CONNECTS TO:
- Condition.h: The base class for conditions in the training library.

NOTES:
- None.

## `src/training/GetTargetCondition.cpp`

PURPOSE:
This file implements the training library's "gettargetcondition" interface, which evaluates whether a ship is currently targeting a specified target.

KEY PARTS:
- GetTargetCondition: Class that encapsulates the logic for evaluating if a ship is targeting a specific object.
  - Constructors to initialize the ship and target objects.
  - Destructor to clean up allocated memory.
  - Evaluate: Function that checks if the current command target of the ship matches the expected target.

CONNECTS TO:
- TypeIDTarget: Used to store and compare object types and IDs.
- IshipIGC, ImodelIGC: Interfaces representing ships and models in the game.

NOTES:
- None.

## `src/training/GetTargetCondition.h`

PURPOSE:
This header file defines the `GetTargetCondition` class within the training library. It is used to evaluate whether a ship has a specific target based on various conditions.

KEY PARTS:
- `GetTargetCondition`: A class that inherits from `Condition`. It evaluates if a ship has a target meeting certain criteria.
  - Constructor overloads allow specifying the ship, expected target by model and ID, or an abstract target directly.
  - Destructor cleans up resources.
  - `Evaluate`: Virtual function to check if the condition is met (i.e., if the ship has the specified target).

CONNECTS TO:
- `Condition.h`: Base class for conditions.
- `AbstractTarget.h`: Abstract base class representing targets.

NOTES:
- None.

## `src/training/GetViewSectorCondition.cpp`

PURPOSE:
This file contains the implementation of a condition class in the Allegiance training library that checks if the player's current view sector matches a specified sector ID.

KEY PARTS:
- GetViewSectorCondition: Class representing the condition to check the player's view sector.
- Evaluate: Function that returns true if the player's current view sector matches the specified sector ID, otherwise false.

CONNECTS TO:
- trekClient.GetViewCluster(): Retrieves the cluster containing the player's current view sector.

NOTES:
None.

## `src/training/GetViewSectorCondition.h`

**PURPOSE:**  
This file defines a condition class for the training library in Allegiance that checks if a specific sector is within view.

**KEY PARTS:**  
- **Class: GetViewSectorCondition**
  - Inherits from `Condition`
  - Constructor takes a `SectorID` parameter to specify which sector to check
  - Destructor
  - Virtual function `Evaluate()` to determine if the specified sector is in view
  - Protected member variable `m_sector` to store the sector ID

**CONNECTS TO:**  
- Depends on `Condition.h`

**NOTES:**  
- None

## `src/training/Goal.cpp`

PURPOSE:
This file contains the implementation of the `Goal` class within the training library, which is responsible for managing and evaluating game goals.

KEY PARTS:
- Goal: Represents a game goal with success and skip conditions.
- Start: Initializes and starts the goal's actions and conditions.
- Stop: Stops all associated actions and conditions.
- Evaluate: Checks if the goal has been completed based on its success condition.
- AddStartAction: Adds an action to be executed when the goal starts.
- AddConstraintCondition: Adds a condition that must be true for the goal to progress.
- SetSkipGoalCondition: Sets a condition under which the goal should be skipped.

CONNECTS TO:
GoalList.h

NOTES:
None.

## `src/training/Goal.h`

PURPOSE:
This header file defines the `Goal` class within the training library of Allegiance. The `Goal` class represents a specific objective that can be achieved through a series of actions and conditions.

KEY PARTS:
- Goal: Represents a goal with methods to start, stop, evaluate its completion, and add start actions and constraint conditions.
- StartActionList: Manages a list of actions to execute when the goal starts.
- ConstraintConditionList: Manages a list of conditions that must be met for the goal to proceed.
- SkipGoalCondition: Allows setting a condition under which the goal should be skipped.

CONNECTS TO:
- ConditionList.h
- ActionList.h

NOTES:
None.

## `src/training/KillsReachedCondition.cpp`

PURPOSE: This file contains the implementation of a condition that checks if a certain number of kills have been reached in the training mode of Allegiance.

KEY PARTS:
- `KillsReachedCondition`: A class representing a condition that evaluates whether the number of kills has reached a specified threshold.
  - Constructor (`KillsReachedCondition(int kills)`) initializes the number of required kills.
  - Destructor (`~KillsReachedCondition(void)`) cleans up any resources if necessary.
  - `Evaluate()`: Checks if the current kill count is greater than or equal to the required number of kills and logs a message if it has been reached.

CONNECTS TO: 
- `Training.h` and `Training.cpp` for access to the global kill count.

NOTES:
- None.

## `src/training/KillsReachedCondition.h`

PURPOSE: This file defines a condition class that checks if a certain number of kills have been reached.

KEY PARTS:
- KillsReachedCondition: A class derived from Condition that evaluates whether the specified number of kills has been achieved.
  - Constructor: Initializes the number of kills required.
  - Destructor: Cleans up any resources used by the object.
  - Evaluate: Returns true if the number of kills has been reached, otherwise false.

CONNECTS TO: Condition.h

NOTES: None.

## `src/training/Mission2.h`

PURPOSE:
The file defines the header for a training mission interface named "Mission2" within the Allegiance game. It extends the base class `TrainingMission` and includes methods to create specific goals and manage the mission's universe.

KEY PARTS:
- Mission2: Inherits from TrainingMission and implements various virtual functions to define the mission.
- CreateUniverse: Initializes the game world for this mission.
- CreateMission: Sets up the initial conditions of the mission.
- CreateGoal01 through CreateGoal09: Define specific goals that players must achieve during the mission.

CONNECTS TO:
TrainingMission.h

NOTES:
None

## `src/training/Mission3.h`

PURPOSE: This file defines the header for a training mission in Allegiance, specifically Mission3.

KEY PARTS:
- Class: Mission3 - Inherits from TrainingMission and implements various virtual functions to define the behavior of the mission.
- Function: GetMissionID - Returns the ID of the mission.
- Function: GetStartSectorID - Returns the starting sector for the mission.
- Function: RecordKeyPress - Handles key press events during the mission.
- Function: ShipLanded - Checks if a ship has landed in the mission.

CONNECTS TO: TrainingMission.h

NOTES: None.

## `src/training/Mission4.h`

PURPOSE: This file defines the header for the Mission4 class in the training library, which is part of a larger game system.

KEY PARTS:
- Class: Mission4 - Inherits from TrainingMission and represents a specific training mission.
- Functions: 
  - ~Mission4() - Destructor for Mission4.
  - GetMissionID() - Returns the ID of this mission.
  - GetStartSectorID() - Returns the sector ID where the mission starts.
  - ShipLanded() - Checks if a ship has landed.
  - CreateUniverse() - Creates the universe for this mission.
  - CreateMission() - Creates the mission conditions.
- Data: 
  - m_enemyScoutID, m_enemyMiner1ID, etc. - IDs of enemy ships involved in the mission.

CONNECTS TO: TrainingMission.h, Goal.h

NOTES: None

## `src/training/Mission5.h`

PURPOSE:
This header file defines the interface and implementation for Mission5 in the training library of Allegiance. It extends the TrainingMission class to create a specific mission scenario.

KEY PARTS:
- Class: Mission5 - Inherits from TrainingMission and implements methods to set up the mission, handle ship and camera configurations, record key presses, and create various goals.
- Functions: CreateUniverse, CreateMission, CreateGoal01 through CreateGoal05 - These functions are responsible for setting up the universe, defining the mission conditions, and creating specific goals for the mission.

CONNECTS TO:
- TrainingMission.h - Inherits from this base class to utilize its functionality.
- Goal.h - Uses Goal objects to define mission objectives.

NOTES:
- None

## `src/training/ModifyCreateDroneActionAction.cpp`

PURPOSE: This file contains the implementation of a class responsible for modifying properties of a drone creation action in Allegiance.

KEY PARTS:
- `ModifyCreateDroneActionAction`: A class that modifies various attributes of a `CreateDroneAction` object.
  - `ModifyCreatedBehaviour`: Sets the created behaviour bitmask.
  - `ModifyCreatedTechLevel`: Sets the created tech level.
  - `ModifyCreatedHullType`: Sets the created hull type ID.
  - `ModifyCreatedName`: Sets the created name and marks that the name should be changed.
  - `ModifyCreatedLocation`: Sets the created location sector and position.
  - `ModifyCreatedCommand`: Sets the created command target, type, and ID.

CONNECTS TO: `CreateDroneAction` class for applying modifications.

NOTES: None.

## `src/training/ModifyCreateDroneActionAction.h`

PURPOSE: This file defines a class `ModifyCreateDroneActionAction` that extends the base `Action` class and is used to modify properties of a `CreateDroneAction`.

KEY PARTS:
- ModifyCreateDroneActionAction: Inherits from Action, modifies properties of a CreateDroneAction.
- Execute: Executes the modification of the drone action's properties.
- ModifyCreatedBehaviour: Changes the behaviour bitmask of the created drone.
- ModifyCreatedTechLevel: Sets the technology level of the created drone.
- ModifyCreatedHullType: Changes the hull type of the created drone.
- ModifyCreatedName: Renames the created drone.
- ModifyCreatedLocation: Moves the created drone to a new sector and position.
- ModifyCreatedCommand: Assigns a command to the created drone.

CONNECTS TO: Action.h, CreateDroneAction.h

NOTES: None.

## `src/training/PlayerShipTarget.cpp`

PURPOSE: This file contains the implementation of the `PlayerShipTarget` class, which is part of the training library in Allegiance. It provides functionality to get the ship model for a player.

KEY PARTS:
- PlayerShipTarget: Class representing the target ship for a player.
- ~PlayerShipTarget: Destructor for the PlayerShipTarget class.
- operator ImodelIGC*: Overloaded operator that returns the ship model for the player.

CONNECTS TO: trekClient.GetShip(): This function is used to retrieve the ship model for the player.

NOTES: None.

## `src/training/PlayerShipTarget.h`

PURPOSE:
This file defines the PlayerShipTarget class within the Training library, which is an interface for representing player-controlled ships as targets in a game.

KEY PARTS:
- PlayerShipTarget: A class derived from AbstractTarget that represents player-controlled ships as targets.
- ~PlayerShipTarget(): Destructor for the PlayerShipTarget class.
- operator ImodelIGC*(): Overloaded cast operator to convert PlayerShipTarget to ImodelIGC*.

CONNECTS TO:
AbstractTarget.h

NOTES:
None.

## `src/training/Predicate.cpp`

PURPOSE:
This file contains the implementation of the `Predicate` class in the training library, which is responsible for evaluating conditions and determining if they match an expected value.

KEY PARTS:
- `Predicate::Predicate`: Constructor that initializes a `Predicate` object with a condition and an expected boolean value.
- `Predicate::~Predicate`: Destructor that deletes the associated condition.
- `Predicate::Start`: Starts the condition and checks if it matches the expected value.
- `Predicate::Stop`: Stops the condition.
- `Predicate::Evaluate`: Evaluates the condition and checks if it matches the expected value.

CONNECTS TO:
- `Condition.h`: The header file for the condition interface that `Predicate` depends on.

NOTES:
- None.

## `src/training/Predicate.h`

PURPOSE: The file defines a class `Predicate` within the `Training` namespace that extends the `Condition` class. It is used to evaluate whether a given condition meets an expected value.

KEY PARTS:
- Name: Predicate - Extends Condition and evaluates if a condition matches an expected boolean value.
- Constructor: Initializes with a pointer to a Condition and the expected boolean value.
- Destructor: Cleans up resources.
- Start: Starts the evaluation process.
- Stop: Stops the evaluation process.
- Evaluate: Checks if the current state of the condition matches the expected value.

CONNECTS TO: Depends on `Condition.h`.

NOTES: None.

## `src/training/ProxyAction.cpp`

PURPOSE: This file implements the `ProxyAction` class in the training library, which is used to hold an action object and execute it when needed.

KEY PARTS:
- ProxyAction: A class that holds a pointer to an `Action` object and provides a method to execute it.
- Execute: A member function of `ProxyAction` that calls the `Execute` method on the contained `Action` object.

CONNECTS TO: The training library, specifically the `Action` interface.

NOTES: None.

## `src/training/ProxyAction.h`

PURPOSE:
The file defines a header for the "proxyaction" interface in the training library of Allegiance. It provides a class that acts as a proxy for another action, allowing additional functionality to be added without modifying the original action.

KEY PARTS:
- ProxyAction: A class derived from Action that serves as a proxy for another action.
  - Constructor: Initializes with a pointer to an Action object.
  - Destructor: Cleans up resources.
  - Execute: Calls the Execute method of the proxied Action object.
  - m_pAction: Pointer to the proxied Action object.

CONNECTS TO:
- Action.h: The base class for actions in Allegiance.

NOTES:
- None.

## `src/training/ProxyCondition.cpp`

PURPOSE: The file `ProxyCondition.cpp` implements the training library's "ProxyCondition" interface, which provides a wrapper around an existing condition to allow for additional functionality or modifications.

KEY PARTS:
- ProxyCondition: A class that acts as a proxy for another Condition object. It holds a pointer to the actual Condition and delegates its Evaluate method calls to it.
- ~ProxyCondition: The destructor for the ProxyCondition class, which currently does nothing.
- Evaluate: A member function of ProxyCondition that evaluates the condition by calling the Evaluate method on the encapsulated Condition object.

CONNECTS TO:
- Condition.h: This file likely contains the definition of the Condition interface or class that ProxyCondition wraps.

NOTES:
- None.

## `src/training/ProxyCondition.h`

PURPOSE:
The file defines a class `ProxyCondition` within the `Training` namespace that acts as a proxy for another condition object. It inherits from the base class `Condition` and provides functionality to evaluate conditions indirectly through its proxied object.

KEY PARTS:
- Class: ProxyCondition
  - Inherits from Condition
  - Constructor takes a pointer to another Condition object
  - Destructor cleans up resources
  - Evaluate method delegates evaluation to the proxied condition

CONNECTS TO:
Condition.h (base class for conditions)

NOTES:
None

## `src/training/ReleaseConsumerAction.cpp`

PURPOSE: This file contains the implementation of the `ReleaseConsumerAction` class within the training library, which is responsible for releasing a consumer associated with an buoy.

KEY PARTS:
- ReleaseConsumerAction: Constructor that takes a reference to an IbuoyIGC object and stores it.
- ~ReleaseConsumerAction: Destructor that sets the m_buoy member to null.
- Execute: Calls the `ReleaseConsumer` method on the stored IbuoyIGC object.

CONNECTS TO: IbuoyIGC (training library)

NOTES: None.

## `src/training/ReleaseConsumerAction.h`

PURPOSE: This file defines the ReleaseConsumerAction class, which is part of the training library and represents an action to release a consumer from a buoy.

KEY PARTS:
- Class: ReleaseConsumerAction - Inherits from Action and implements functionality to release a consumer from a buoy.
- Function: Execute - The method that performs the action of releasing the consumer.
- Member Variable: m_buoy - A reference to the IbuoyIGC object representing the buoy from which the consumer will be released.

CONNECTS TO: Action.h

NOTES: None.

## `src/training/RepairRearmAction.cpp`

PURPOSE: The file `RepairRearmAction.cpp` implements the functionality for repairing and rearming a ship in the Allegiance game training library.

KEY PARTS:
- `RepairRearmAction`: Class representing an action to repair and rearm a ship.
- `Execute`: Method that performs the repair and rearming of the ship by healing it, reloading its energy, fuel, and ammunition, and filling cargo slots with missiles and ammo.

CONNECTS TO: 
- `TrainingMission`: The mission class that manages parts being added to ships.

NOTES:
- None.

## `src/training/RepairRearmAction.h`

PURPOSE: This header file defines the interface for a training library action that handles repairing and rearming entities.

KEY PARTS:
- RepairRearmAction: A class derived from Action that represents an action to repair and rearm entities.
  - ~RepairRearmAction(): Virtual destructor for proper cleanup of derived classes.
  - Execute(): Virtual function to perform the repair and rearm operation.

CONNECTS TO: Action.h, which defines the base class for actions in the training library.

NOTES: None.

## `src/training/SetAutopilotAction.cpp`

PURPOSE: This file implements the training library interface for setting autopilot actions in a ship.

KEY PARTS:
- SetAutopilotAction: Class representing an action to set the autopilot state of a ship.
- Execute: Method that executes the action by calling SetAutopilot on the target ship.

CONNECTS TO: IshipIGC, ImodelIGC, TypeIDTarget

NOTES: None.

## `src/training/SetAutopilotAction.h`

PURPOSE: This header file defines the interface and implementation for a training library action that sets the autopilot status of a ship.

KEY PARTS:
- SetAutopilotAction: A class derived from Action that represents setting the autopilot status of a ship.
  - Constructor: Initializes the action with a ship pointer or ship ID and an optional new autopilot value.
  - Destructor: Cleans up resources used by the action.
  - Execute: Executes the action, which sets the autopilot status of the specified ship to the new value.

CONNECTS TO:
- Action.h: The base class for all actions in the training library.
- AbstractTarget.h: Defines the interface for abstract targets that can be acted upon.

NOTES:
- None.

## `src/training/SetCargoNanAction.cpp`

PURPOSE:
This file implements a training library action that modifies the cargo of a ship by setting specific parts and weapons.

KEY PARTS:
- SetCargoNanAction: Class representing the action to set cargo on a ship.
- Execute: Method that performs the actual modification of the ship's cargo.
- nanShipID: Member variable storing the ID of the ship to modify.
- PartData: Struct used to define part data for creation and addition to the ship.

CONNECTS TO:
TrainingMission.h, IshipIGC.h, IpartIGC.h

NOTES:
None.

## `src/training/SetCargoNanAction.h`

**PURPOSE:**  
This header file defines an interface for a training library action that sets cargo nanites on a ship.

**KEY PARTS:**  
- **SetCargoNanAction:** A class derived from `Action` that represents setting cargo nanites on a specified ship.
  - **Constructor:** Initializes the action with a `ShipID`.
  - **Destructor:** Cleans up any resources used by the action.
  - **Execute:** Executes the action, which involves setting cargo nanites on the designated ship.
- **nanShipID:** A protected member variable that stores the ID of the ship to which cargo nanites will be set.

**CONNECTS TO:**  
Action.h

**NOTES:**  
None.

## `src/training/SetControlConstraintsAction.cpp`

PURPOSE:
This file implements a training library action that sets control constraints for player input in the Allegiance game.

KEY PARTS:
- SetControlConstraintsAction: Class to manage and execute control constraints.
- Execute: Method to apply the set control values and masks.
- EnableInputAction: Method to enable specific input actions.
- DisableInputAction: Method to disable specific input actions.
- ScaleInputControl: Method to scale a specific input control axis.

CONNECTS TO:
Dependencies on global variables `g_allowableControlScalars` and `g_iAllowableActionMask`.

NOTES:
None.

## `src/training/SetControlConstraintsAction.h`

PURPOSE:
This header file defines the interface and implementation for a training library action that sets control constraints.

KEY PARTS:
- SetControlConstraintsAction: A class derived from Action that manages input controls by enabling, disabling specific actions and scaling input controls.
- Execute: Virtual function to execute the action.
- EnableInputAction: Enables a specific input action using a bitmask.
- DisableInputAction: Disables a specific input action using a bitmask.
- ScaleInputControl: Scales an input control for a given axis.

CONNECTS TO:
Depends on Action.h, which is included at the beginning of the file.

NOTES:
None.

## `src/training/SetControlsAction.cpp`

PURPOSE:
The file `SetControlsAction.cpp` implements the training library's "SetControlsAction" interface, which is responsible for setting specific control values and actions in a game.

KEY PARTS:
- `SetControlsAction`: Class that encapsulates the logic to set controls.
- `Execute`: Method that applies the stored control settings to global variables.
- `SetInputAction`: Method to update the input action with a bitmask.
- `SetInputControl`: Method to set a specific axis control value.

CONNECTS TO:
- Depends on `ControlData` and `g_setInputControls`, which are defined in other files.

NOTES:
- None.

## `src/training/SetControlsAction.h`

**PURPOSE:**  
This header file defines the `SetControlsAction` class within the Allegiance training library. The class is derived from `Action` and is used to set controls for a player's input actions.

**KEY PARTS:**
- **Class: SetControlsAction**
  - Inherits from `Action`
  - Contains methods to execute control settings, set specific input actions, and adjust input controls.
  
- **Function: Execute()**
  - Virtual function that executes the action of setting controls.
  
- **Function: SetInputAction(int inputAction, int iMask = 0)**
  - Sets a specific input action with an optional mask.

- **Function: SetInputControl(Axis axis, float fScalar)**
  - Adjusts a control value for a given axis by a scalar factor.

- **Data Member: m_inputControls**
  - Stores the current state of input controls.
  
- **Data Member: m_iInputAction**
  - Stores the current input action setting.

**CONNECTS TO:**  
`Action.h`

**NOTES:**  
None.

## `src/training/SetDroneNanAction.cpp`

**PURPOSE:**  
This file implements a training library interface for setting drone nanite actions in the Allegiance game. It defines a class `SetDroneNanAction` that handles the execution of specific actions on a drone ship.

**KEY PARTS:**
- **Class: SetDroneNanAction**
  - Initializes with a ship ID.
  - Executes an action to terminate any existing weapon part and add a Nanite 2 part to the front of the drone ship.
  
- **Function: Execute**
  - Retrieves the drone ship using `trekClient.GetSide()->GetShip(nanShipID)`.
  - Terminates any existing weapon part on the drone.
  - Creates and adds a Nanite 2 part to the front of the drone.

**CONNECTS TO:**  
`TrainingMission.h`, `IshipIGC.h`, `IpartIGC.h`

**NOTES:**  
- None.

## `src/training/SetDroneNanAction.h`

PURPOSE: This header file defines the interface and implementation for a specific action in the training library of Allegiance, which is used to set up drone nanotechnology actions for ships.

KEY PARTS:
- SetDroneNanAction: A class that inherits from Action, representing an action to set up drone nanotechnology on a ship.
  - Constructor: Initializes the SetDroneNanAction object with a ShipID.
  - Destructor: Cleans up resources when the object is destroyed.
  - Execute: Virtual function that executes the action of setting up drone nanotechnology for the specified ship.
  - nanShipID: Protected member variable to store the ID of the ship on which the drone nanotechnology will be set up.

CONNECTS TO:
- Action.h: This file includes the base class Action, from which SetDroneNanAction inherits.

NOTES:
- None.

## `src/training/SetHUDOverlayAction.cpp`

PURPOSE: This file implements an action class for setting a HUD overlay in the training library of Allegiance.

KEY PARTS:
- SetHUDOverlayAction: Class representing an action to set a specific HUD overlay.
- Execute: Method that executes the action by calling SetTrainingOverlay with the stored overlay value.
- m_overlay: Member variable storing the HUD overlay to be set.

CONNECTS TO: TrainingOverlay, SetTrainingOverlay function

NOTES: None

## `src/training/SetHUDOverlayAction.h`

PURPOSE: This header file defines an interface for setting HUD overlays in the training library of Allegiance.

KEY PARTS:
- SetHUDOverlayAction: A class that inherits from Action and is used to set a specific HUD overlay.
- Execute: A virtual function that executes the action of setting the HUD overlay.
- m_overlay: A protected member variable that stores the HUD overlay to be set.

CONNECTS TO: Action.h

NOTES: None.

## `src/training/SetRadarLODAction.cpp`

PURPOSE: This file implements an action within a training library that sets the level of detail (LOD) for the radar in a game window.

KEY PARTS:
- SetRadarLODAction: A class representing an action to set the radar's level of detail.
- Execute: A method that applies the LOD setting to the game window if it differs from the current setting.
- GetWindow: Retrieves the game window instance.
- RadarImage::RadarLOD: An enumeration defining different levels of detail for the radar.

CONNECTS TO: Training library, TrekWindow (game window).

NOTES: None.

## `src/training/SetRadarLODAction.h`

PURPOSE:
This header file defines an interface for a training library action that sets the level of detail (LOD) for radar images.

KEY PARTS:
- SetRadarLODAction: A class derived from Action that sets the radar LOD.
  - Constructor: Initializes with a RadarImage::RadarLOD value.
  - Destructor: Virtual destructor to ensure proper cleanup.
  - Execute: Method to perform the action of setting the radar LOD.
  - m_radarLOD: Protected member variable storing the radar LOD.

CONNECTS TO:
- Action.h: The base class for all actions in the training library.

NOTES:
- None.

## `src/training/SetSignatureAction.cpp`

PURPOSE: The file `SetSignatureAction.cpp` implements the training library's interface for setting a signature value on an object in the game.

KEY PARTS:
- `SetSignatureAction`: Class that encapsulates the action of setting a signature value on a target object.
  - Constructors to initialize the class with different types of targets (object pointer, object type and ID, or abstract target).
  - Destructor to clean up the dynamically allocated target object.
  - `Execute`: Method that applies the signature value to the target object if it is valid.

CONNECTS TO: Depends on classes from the training library, including `TypeIDTarget` for managing target objects.

NOTES: None.

## `src/training/SetSignatureAction.h`

PURPOSE:
This header file defines the `SetSignatureAction` class within the training library of Allegiance. The purpose is to allow setting a signature value for an object, which can be used in various game mechanics such as detection and tracking.

KEY PARTS:
- `SetSignatureAction`: A class derived from `Action` that sets a signature value for a target.
  - Constructor overloads: Initializes the action with different types of targets (ImodelIGC*, ObjectType, ObjectID, AbstractTarget*).
  - Destructor: Cleans up any resources used by the action.
  - `Execute`: Method to perform the action, setting the signature on the target.

CONNECTS TO:
- Depends on `Action.h` and `AbstractTarget.h`.

NOTES:
- None.

## `src/training/ShipSeesObjectCondition.cpp`

PURPOSE:
This file implements a condition in the training library that checks if a ship can see another object within its scanner range.

KEY PARTS:
- ShipSeesObjectCondition: Class representing the condition where a ship sees an object.
- Evaluate: Function to determine if the ship is within scanner range of the target object.
- TypeIDTarget: Helper class to store and compare object type and ID.

CONNECTS TO:
- IshipIGC, ImodelIGC: Interfaces for ship and model objects in the game.

NOTES:
- None.

## `src/training/ShipSeesObjectCondition.h`

**PURPOSE:**  
This file defines a condition in the training library that checks if a ship can see another object.

**KEY PARTS:**  
- **Class: ShipSeesObjectCondition**
  - Checks if a ship (represented by `m_pShip`) can see another object (`m_pTarget`).
  - Supports three constructors to initialize with different types of targets.
  - Destructor cleans up resources.
  - `Evaluate()` method returns `true` if the ship sees the target, otherwise `false`.

- **Dependencies:**
  - Inherits from `Condition`.
  - Uses `AbstractTarget` for representing both the ship and the target.

**CONNECTS TO:**  
- Other condition classes in the training library.
- `Condition.h` and `AbstractTarget.h` header files.

**NOTES:**  
- None.

## `src/training/ShowEyeballAction.cpp`

PURPOSE:
The file `ShowEyeballAction.cpp` implements a training library interface called `ShowEyeballAction`, which is responsible for controlling whether an eyeball indicator is shown on the player's ship in the game Allegiance.

KEY PARTS:
- `class ShowEyeballAction`: The main class that encapsulates the functionality to show or hide the eyeball indicator.
  - Constructor: Initializes the `m_showEyeball` member variable with a boolean indicating whether to show the eyeball.
  - Destructor: Cleans up any resources if necessary (currently empty).
  - `Execute()`: Executes the action by modifying the player's ship status to either show or hide the eyeball indicator.

CONNECTS TO:
- `trekClient.GetPlayerInfo()`: Retrieves the player's information from the game client.
- `ShipStatus`: A class representing the current status of the player's ship, which includes whether the eyeball is detected.

NOTES:
- None.

## `src/training/ShowEyeballAction.h`

PURPOSE:
This header file defines an interface for a training library action that controls whether to show or hide an eyeball in the game.

KEY PARTS:
- ShowEyeballAction: A class derived from Action that manages showing or hiding an eyeball.
  - Constructor: Initializes the action with an optional parameter to set the initial state of the eyeball (show or hide).
  - Destructor: Cleans up any resources used by the action.
  - Execute: Virtual function that performs the action of showing or hiding the eyeball based on the current state.
  - m_showEyeball: A protected boolean member variable indicating whether the eyeball should be shown.

CONNECTS TO:
Action.h: This file includes the base class Action, which is a dependency for ShowEyeballAction.

NOTES:
- None.

## `src/training/ShowPaneAction.cpp`

PURPOSE:
The file `ShowPaneAction.cpp` implements the training library's "ShowPaneAction" interface, which is responsible for showing a specific pane in the game window by toggling overlay flags.

KEY PARTS:
- **Class: ShowPaneAction**
  - Constructor initializes with an `OverlayMask`.
  - Destructor does nothing.
  - Method `Execute`: Turns off certain overlay flags and turns on the specified `m_overlayMask`.

CONNECTS TO:
- Depends on `TrekWindow` class for window operations.

NOTES:
- None.

## `src/training/ShowPaneAction.h`

PURPOSE:
This header file defines the `ShowPaneAction` class within the Allegiance game's training library. The class is responsible for showing a pane with an optional overlay mask.

KEY PARTS:
- `ShowPaneAction`: A class that inherits from `Action`. It has a constructor to initialize the `m_overlayMask` and a destructor.
- `Execute`: A virtual function that executes the action of showing the pane with the specified overlay mask.
- `m_overlayMask`: A protected member variable that holds the overlay mask for the pane.

CONNECTS TO:
- Depends on the `Action` class defined in "Action.h".

NOTES:
- None.

## `src/training/SuspendedPlaySoundAction.cpp`

**PURPOSE:**  
This file implements a training library action that plays a sound and waits for it to finish before continuing. It is part of the training mission system.

**KEY PARTS:**  
- `SuspendedPlaySoundAction`: A class representing an action to play a sound and wait for its completion.
- `SuspendedSoundFinishedCondition`: A condition used to check if the sound has finished playing.
- `Execute`: The method that plays the sound and adds a wait condition to the mission.

**CONNECTS TO:**  
- `TrainingMission`: The main class managing training missions.
- `PlaySoundAction`: The base class for actions that play sounds.
- `SuspendedSoundFinishedCondition`: A condition used to manage the waiting state.

**NOTES:**  
- None.

## `src/training/SuspendedPlaySoundAction.h`

PURPOSE:
This file defines a class `SuspendedPlaySoundAction` in the Allegiance game's training library. It extends the functionality of `PlaySoundAction` to allow sound playback that can be suspended and resumed.

KEY PARTS:
- SuspendedPlaySoundAction: A class derived from PlaySoundAction, designed for playing sounds that can be suspended.
- Constructor: Initializes a new instance with a SoundID and an optional flag to delete the action when done.
- Destructor: Cleans up resources used by the action.
- Execute: Method to execute the sound playback.
- m_bDeleteWhenDone: A protected member variable indicating whether the action should be deleted after execution.

CONNECTS TO:
PlaySoundAction.h

NOTES:
None.

## `src/training/SuspendedSoundFinishedCondition.cpp`

PURPOSE: Implements a condition in the training library that checks if a suspended sound has finished playing.

KEY PARTS:
- SuspendedSoundFinishedCondition: A class representing a condition that waits for a suspended sound to finish playing.
- Constructor: Initializes the condition with a pointer to a PlaySoundAction and a flag indicating whether to delete the action when done.
- Destructor: Deletes the PlaySoundAction if the deletion flag is set.

CONNECTS TO: SoundFinishedCondition (base class), PlaySoundAction

NOTES: None

## `src/training/SuspendedSoundFinishedCondition.h`

PURPOSE:
This header file defines a class `SuspendedSoundFinishedCondition` within the Allegiance training library. It extends the functionality of `SoundFinishedCondition` to handle sound conditions that are suspended and need to be resumed upon completion.

KEY PARTS:
- Class: SuspendedSoundFinishedCondition
  - Inherits from SoundFinishedCondition
  - Constructor: Initializes with a pointer to a PlaySoundAction object and an optional flag to delete itself when done.
  - Destructor: Cleans up resources.
  - Protected Member: m_bDeleteWhenDone - Indicates whether the object should be deleted when its condition is met.

CONNECTS TO:
- SoundFinishedCondition.h (base class)

NOTES:
- None.

## `src/training/SwapWeaponAction.cpp`

**PURPOSE:** This file contains the implementation of the `SwapWeaponAction` class, which is part of the training library in Allegiance. The class handles the swapping of weapons on a ship.

**KEY PARTS:**
- **Class: SwapWeaponAction**: Manages the swapping of weapon equipment on a ship.
  - **Constructor (`SwapWeaponAction`)**: Initializes the `equipmentType` and `mount` for the weapon to be swapped.
  - **Destructor (`~SwapWeaponAction`)**: Cleans up any resources used by the class.
  - **Method (`Execute`)**: Executes the swapping of the specified weapon equipment on the ship.

**CONNECTS TO:** 
- `GetWindow()->SwapPart(m_equipmentType, m_mount)`: Calls a method to perform the actual swapping of the weapon part.

**NOTES:** None.

## `src/training/SwapWeaponAction.h`

PURPOSE:
The file defines a class `SwapWeaponAction` within the `Training` namespace, which is part of a training library for a game. This class represents an action that swaps a weapon on a specified mount.

KEY PARTS:
- `SwapWeaponAction`: A class derived from `Action` that handles swapping weapons.
  - Constructor: Initializes with `EquipmentType` and `Mount`.
  - Destructor: Cleans up resources.
  - `Execute`: Method to perform the weapon swap action.
- `m_equipmentType`: Stores the type of equipment (weapon) to be swapped.
- `m_mount`: Specifies which mount the weapon should be swapped on.

CONNECTS TO:
Depends on the `Action` class defined in `Action.h`.

NOTES:
None.

## `src/training/ToggleWeaponAction.cpp`

**PURPOSE:**  
This file contains the implementation of a training library action that toggles the selected weapon for a mount in a game.

**KEY PARTS:**  
- `ToggleWeaponAction`: A class representing an action to toggle the selected weapon for a specific mount.
- `Execute`: The method that performs the actual functionality of toggling the weapon.

**CONNECTS TO:**  
- `GetWindow()->ToggleWeapon(m_mount)`: This function call indicates that the action interacts with the window system to toggle the weapon.

**NOTES:**  
- None.

## `src/training/ToggleWeaponAction.h`

PURPOSE: This file defines a class `ToggleWeaponAction` within the Allegiance game's training library, which is responsible for toggling weapon actions on a mount.

KEY PARTS:
- Class: `ToggleWeaponAction`
  - Inherits from: `Action`
  - Constructor: `ToggleWeaponAction(Mount mount)`
    - Initializes the action with a specific mount.
  - Destructor: `~ToggleWeaponAction()`
    - Cleans up resources if necessary.
  - Method: `Execute()`
    - Executes the weapon toggle action on the associated mount.
  - Protected Member: `Mount m_mount`
    - Stores the mount object to which the weapon action is applied.

CONNECTS TO:
- Depends on `Action.h` for base class definition.

NOTES:
- None.

## `src/training/Training.cpp`

PURPOSE: The file `Training.cpp` implements the core functionality for training missions in the Allegiance game. It manages mission initialization, execution, and termination, as well as handles user input and control constraints.

KEY PARTS:
- `StartMission(int iMissionIndex)`: Initializes and starts a specific training mission based on the given index.
- `HandleMission(void)`: Processes the current mission frame if one is active.
- `EndMission(void)`: Terminates the currently running mission.
- `ApproveControls(ControlData& inputControls)`: Applies control constraints to the player's input.
- `ApproveActions(int iInputMask)`: Filters and modifies the player's actions based on allowed controls.

CONNECTS TO: Other training mission files (`Mission2.h`, `Mission3.h`, etc.) and game systems for handling user input, ship behavior, and HUD overlays.

NOTES: None.

## `src/training/Training.h`

PURPOSE: This header file defines the external interfaces for the training library in Allegiance, providing functions to start, handle, and end training missions, as well as event trapping and control constraint functions.

KEY PARTS:
- `StartMission(int iMissionIndex)`: Starts a specific training mission.
- `HandleMission(void)`: Handles the current training mission.
- `EndMission(void)`: Ends the current training mission.
- `SetSkipPostSlideshow(void)`: Sets whether to skip the post-slideshow screen after a mission.
- `IsTraining(void)`: Checks if a training mission is currently active.
- `IsInstalled(void)`: Checks if the training library is installed.
- `CommandViewEnabled(void)`: Checks if command view mode is enabled.
- `GetTrainingMissionID(void)`: Returns the ID of the current training mission.
- `GetStartSectorID(void)`: Returns the sector ID where the training mission starts.
- `GetKillCount(void)`: Returns the number of kills made during the current training mission.
- `SetupShipAndCamera(void)`: Sets up the ship and camera for a new training mission.
- `RecordChat(ChatTarget recipient)`: Records chat messages sent to a specific target.
- `RecordKeyPress(TrekKey key)`: Records a key press during a training mission.
- `ShipLanded(void)`: Handles the event when a ship lands.
- `ShipDied(ImodelIGC* pLauncher)`: Handles the event when a ship dies.
- `RestoreShip(void)`: Restores the player's ship after it is destroyed.
- `ShipKilled(IshipIGC* pShip, ImodelIGC* pLauncher)`: Handles the event when a ship is killed by another ship.
- `HandlePickDefaultOrder(IshipIGC* pShip)`: Handles picking the default order for a ship during training.
- `KillStationEvent(IstationIGC* pStation, ImodelIGC* pLauncher)`: Handles events related to killing a station.
- `ApproveControls(ControlData& inputControls)`: Approves and modifies control inputs.
- `ApproveActions(int iInputMask)`: Approves and modifies action inputs.

CONNECTS TO: Other training-related files and systems within the Allegiance game engine.

NOTES: None.

## `src/training/TurnShipAboutCondition.cpp`

PURPOSE: This file implements a condition that checks if a ship has turned around by comparing its current forward direction with its starting forward direction.

KEY PARTS:
- TurnShipAboutCondition: Class representing the condition to check if a ship has turned about.
  - Constructor: Initializes the ship reference.
  - Destructor: Sets the ship reference to null.
  - Start: Records the initial forward direction of the ship.
  - Evaluate: Determines if the ship has turned by comparing its current forward direction with the initial one.

CONNECTS TO: ImodelIGC (interface for accessing ship properties).

NOTES: None.

## `src/training/TurnShipAboutCondition.h`

PURPOSE:
This file defines a condition class in the Allegiance game's training library that checks if a ship has turned around.

KEY PARTS:
- TurnShipAboutCondition: A class derived from Condition that evaluates whether a specified ship has completed a 180-degree turn.
- m_ship: A reference to the ship being monitored for the turn.
- m_startingForward: Stores the initial forward direction of the ship when the condition starts.

CONNECTS TO:
Condition.h - The base class for conditions in the training library.

NOTES:
None.

## `src/training/TurnToAction.cpp`

PURPOSE:
The file `TurnToAction.cpp` implements the training library's "TurnToAction" interface, which is responsible for making a ship turn towards a specified target.

KEY PARTS:
- `TurnToAction::TurnToAction`: Constructor to initialize the object and target.
- `TurnToAction::~TurnToAction`: Destructor to clean up allocated resources.
- `TurnToAction::Execute`: Method that calculates the direction to the target, updates the ship's controls accordingly, and applies a scaling factor to the control inputs.

CONNECTS TO:
- Depends on classes like `ImodelIGC`, `IshipIGC`, `TypeIDTarget`, and `ControlData`.

NOTES:
- None.

## `src/training/TurnToAction.h`

**PURPOSE:** This file defines the `TurnToAction` class within the training library, which is used to make an object turn towards a specified target.

**KEY PARTS:**
- **Class: TurnToAction**
  - Inherits from `Action`.
  - Constructor overloads for initializing with different types of targets (object pointer, target type and ID, or abstract target).
  - Destructor.
  - `Execute` method to perform the turn action.
- **Protected Members:**
  - `m_pObject`: Pointer to the object performing the turn.
  - `m_pTarget`: Pointer to the target towards which the object is turning.

**CONNECTS TO:** 
- Depends on classes from "Action.h" and "AbstractTarget.h".

**NOTES:** None.

## `src/training/TypeIDTarget.cpp`

PURPOSE: This file contains the implementation of the `TypeIDTarget` class, which is part of a training library in Allegiance. The class represents a target identified by its type and ID.

KEY PARTS:
- `TypeIDTarget`: A constructor that initializes the target type and ID.
- `~TypeIDTarget`: The destructor for the `TypeIDTarget` class.
- `operator ImodelIGC*`: An overloaded operator to convert the `TypeIDTarget` object to an `ImodelIGC` pointer, which retrieves the model associated with the target type and ID.

CONNECTS TO: `trekClient.GetCore()->GetModel(m_targetType, m_targetID)`

NOTES: None.

## `src/training/TypeIDTarget.h`

PURPOSE: This file defines the `TypeIDTarget` class, which is part of a training library in Allegiance. It represents a target based on an object type and ID.

KEY PARTS:
- `TypeIDTarget`: A class that inherits from `AbstractTarget`. It encapsulates a target identified by its type and ID.
  - Constructor: Initializes the target with a specific type and ID.
  - Destructor: Cleans up resources associated with the target.
  - Operator Overload: Allows casting of `TypeIDTarget` to `ImodelIGC*`.

CONNECTS TO:
- `AbstractTarget`: The base class for all targets in the training library.

NOTES:
- None.

## `src/training/actionlist.cpp`

PURPOSE:
The file `actionlist.cpp` implements the training library's "actionlist" interface, which manages a list of actions that can be executed and stopped.

KEY PARTS:
- `ActionList::ActionList()`: Constructor for the ActionList class.
- `ActionList::~ActionList()`: Destructor for the ActionList class, responsible for deleting all actions in the list.
- `ActionList::Execute()`: Executes all actions in the list.
- `ActionList::Stop()`: Stops all actions in the list.
- `ActionList::AddAction(Action* pAction)`: Adds an action to the list.
- `m_actionList`: A private member variable that stores a list of Action pointers.

CONNECTS TO:
Depends on the `Action` class and likely other parts of the training library.

NOTES:
None.

## `src/training/actionlist.h`

PURPOSE:
The file defines the ActionList class, which is part of a training library in Allegiance. It extends the Action base class and manages a list of actions that can be executed sequentially.

KEY PARTS:
- ActionList: A class representing a list of actions to be executed.
  - Constructor: Initializes an empty action list.
  - Destructor: Cleans up the action list.
  - Execute: Executes all actions in the list.
  - Stop: Stops all actions in the list.
  - AddAction: Adds an action to the list.

CONNECTS TO:
- Action.h: The base class for actions.

NOTES:
None.

## `src/training/andcondition.cpp`

PURPOSE:
The file `andcondition.cpp` implements the training library's "AndCondition" interface, which combines two conditions into a single condition that evaluates to true only if both sub-conditions are true.

KEY PARTS:
- `AndCondition::AndCondition(Condition* pConditionA, Condition* pConditionB)`: Constructor initializes with two sub-conditions.
- `AndCondition::~AndCondition(void)`: Destructor deletes the sub-conditions.
- `bool AndCondition::Start(void)`: Starts both sub-conditions and returns true if both start successfully.
- `void AndCondition::Stop(void)`: Stops both sub-conditions.
- `bool AndCondition::Evaluate(void)`: Evaluates both sub-conditions and returns true only if both are true.

CONNECTS TO:
Depends on the training library's condition interface (`Condition`).

NOTES:
None.

## `src/training/andcondition.h`

PURPOSE: This file defines the `AndCondition` class within the training library, which is a condition that evaluates to true only if both of its sub-conditions are true.

KEY PARTS:
- Class: AndCondition - Inherits from Condition and represents an AND logic gate for conditions.
- Function: AndCondition(Condition* pConditionA, Condition* pConditionB) - Constructor initializes the two sub-conditions.
- Function: ~AndCondition() - Destructor cleans up the sub-conditions.
- Function: Start() - Starts both sub-conditions.
- Function: Stop() - Stops both sub-conditions.
- Function: Evaluate() - Evaluates to true if both sub-conditions are true, otherwise false.

CONNECTS TO: Condition.h (base class)

NOTES: None.

## `src/training/conditionalaction.cpp`

PURPOSE:
The file `conditionalaction.cpp` implements the training library's "conditionalaction" interface, which manages actions based on conditions.

KEY PARTS:
- `ConditionalAction`: A class representing a conditional action that executes an action when a condition is met.
  - Constructor: Initializes the condition and action, along with flags for immediate execution and running once.
  - Destructor: Deletes the condition and action objects.
  - `Start()`: Starts the condition and immediately executes the action if the condition is true and immediate execution is enabled.
  - `Stop()`: Stops both the condition and action.
  - `Evaluate()`: Evaluates the condition and optionally executes the action based on the condition's result.

CONNECTS TO:
- Depends on the `Condition` and `Action` classes, which are not shown in this snippet but are referenced by pointers in `ConditionalAction`.

NOTES:
- The class has logic to handle both immediate and delayed execution of actions.
- There is a flag `m_bRunOnce` that controls whether the action should be executed only once or repeatedly.
- The code includes comments explaining the behavior of immediate and delayed execution scenarios.

## `src/training/conditionalaction.h`

**PURPOSE:** This file defines the `ConditionalAction` class within the training library, which allows for conditional execution of actions based on certain conditions.

**KEY PARTS:**
- **Class: ConditionalAction**
  - Inherits from `Condition`
  - Manages an action that is executed conditionally.
  - Supports immediate or delayed execution and can be configured to run only once.
  
- **Constructor:** Initializes the `ConditionalAction` with a condition, an action, and optional parameters for immediate execution and running once.
  
- **Destructor:** Cleans up resources associated with the `ConditionalAction`.
  
- **Methods:**
  - `Start()`: Initiates the conditional action.
  - `Stop()`: Stops the conditional action.
  - `Evaluate()`: Checks if the condition is met to execute the action.

**CONNECTS TO:** 
- Depends on `Condition.h` and `Action.h`.

**NOTES:** None.

## `src/training/conditionlist.cpp`

PURPOSE:
The file `conditionlist.cpp` implements the training library's "conditionlist" interface, which manages a list of conditions and evaluates whether all conditions in the list are true.

KEY PARTS:
- `ConditionList::~ConditionList()`: Destructor that deletes all conditions managed by the list.
- `bool ConditionList::Start()`: Evaluates if the condition list is considered "true" by default, checking each condition's `Start()` method.
- `void ConditionList::Stop()`: Calls the `Stop()` method on all conditions in the list.
- `bool ConditionList::Evaluate()`: Checks if the condition list is true, considering the size of the list and evaluating each condition's `Evaluate()` method.
- `void ConditionList::AddCondition(Condition* pCondition)`: Adds a new condition to the list.

CONNECTS TO:
Depends on the `Condition` class for individual condition evaluations.

NOTES:
None.

## `src/training/conditionlist.h`

PURPOSE:
The file `conditionlist.h` defines the header for a class in the training library that manages a list of conditions. These conditions can be evaluated together to determine if a certain state is met.

KEY PARTS:
- Class: ConditionList
  - Inherits from: Condition
  - Methods: ~ConditionList, Start, Stop, Evaluate, AddCondition
  - Data: m_conditionList (a list of Condition pointers)

CONNECTS TO:
- Depends on the `Condition` class defined in "Condition.h"

NOTES:
- None

## `src/training/elapsedtimecondition.h`

PURPOSE:
This header file defines a macro and includes necessary headers for creating an elapsed time condition in the training library. The macro `ElapsedTimeCondition` is used to create a condition that becomes true after a specified amount of time has passed.

KEY PARTS:
- ElapsedTimeCondition(fTime): A macro that creates an NTimesCondition with a PeriodicCondition containing a TrueCondition, set to trigger twice at the specified interval.
- PeriodicCondition: A class that triggers its contained condition periodically.
- NTimesCondition: A class that triggers when its contained condition is true for a specified number of times.
- TrueCondition: A class that always returns true.

CONNECTS TO:
PeriodicCondition.h, NTimesCondition.h, TrueCondition.h

NOTES:
None.

## `src/training/endmissionaction.cpp`

**PURPOSE:** This file contains the implementation of the `EndMissionAction` class within the training library, which is responsible for terminating a mission when executed.

**KEY PARTS:**
- **Class: EndMissionAction**
  - **Method: ~EndMissionAction()**
    - Destructor for the `EndMissionAction` class.
  - **Method: Execute()**
    - Terminates the current mission by calling `g_pMission->Terminate()`.

**CONNECTS TO:** 
- TrainingMission.h

**NOTES:** None.

## `src/training/endmissionaction.h`

PURPOSE:
The file defines an interface for an action that ends a mission in the training library of Allegiance.

KEY PARTS:
- EndMissionAction: A class derived from Action that represents the end mission action.
  - ~EndMissionAction(): Virtual destructor to ensure proper cleanup.
  - Execute(): Virtual function to execute the end mission action.

CONNECTS TO:
Action.h

NOTES:
None.

## `src/training/falsecondition.h`

PURPOSE: This header file defines a training library interface for a false condition, which is implemented as the negation of a true condition.

KEY PARTS:
- FalseCondition: A macro that creates an instance of NotCondition with a TrueCondition as its argument.
- NotCondition: A class representing a logical NOT operation on conditions.
- TrueCondition: A class representing a logical TRUE condition.

CONNECTS TO: NotCondition.h, TrueCondition.h

NOTES: None.

## `src/training/goallist.cpp`

PURPOSE:
The file `goallist.cpp` implements the training library's "goal list" interface, which manages a sequence of goals for training purposes.

KEY PARTS:
- GoalList: Manages a list of goals and processes them sequentially.
- Start: Initiates all goals in the list until they are complete.
- Stop: Halts the current goal being processed.
- Evaluate: Checks if the current goal is complete and moves to the next one if it is.
- AddGoal: Adds a new goal to the list.

CONNECTS TO:
Depends on `GoalList.h` for declarations and includes `pch.h`.

NOTES:
None.

## `src/training/goallist.h`

PURPOSE:
The file `goallist.h` defines the header for a training library interface called `GoalList`, which is used to manage and evaluate a list of goals in a game context.

KEY PARTS:
- **Class: GoalList**
  - Inherits from `Condition`.
  - Manages a list of `Goal` objects.
  - Provides methods to start, stop, evaluate the state, and add goals to the list.
  - Uses `std::list<Goal*>` to store and iterate through goals.

CONNECTS TO:
- Depends on `Goal.h`, which defines the `Goal` class.

NOTES:
- None.

## `src/training/messageaction.cpp`

**PURPOSE:**  
This file implements the `MessageAction` class within the training library of Allegiance. The class is responsible for managing and executing messages with associated sounds during training missions.

**KEY PARTS:**
- **Class: MessageAction**
  - Manages a list of messages and their corresponding sound IDs.
  - Provides methods to add messages and execute them, sending both text and sound to the client.
  
- **Function: Execute()**
  - Iterates through the message and sound lists and sends each message with its associated sound to the client using `trekClient.ReceiveChat`.

- **Function: AddMessage(const ZString message, const SoundID soundID)**
  - Adds a new message and its corresponding sound ID to the internal lists.
  - Resets iterators to ensure they are valid after adding new messages.

**CONNECTS TO:**  
`TrainingMission.h`, `MessageAction.h`, `trekClient`

**NOTES:**  
- The file includes comments indicating modifications by "Bahdohday" on July 6, 2007, which added the ability to send sounds with messages.
- The class uses global variables (`g_pMission`) and relies on external functions (`trekClient.ReceiveChat`) for message delivery.

## `src/training/messageaction.h`

PURPOSE:
The file `messageaction.h` defines the header for a class in the training library that handles message actions, which are likely used to display messages and optionally play sounds during gameplay.

KEY PARTS:
- **MessageAction**: A class derived from `Action` that manages a list of messages and associated sound IDs. It provides methods to execute these messages and add new ones.
- **Execute**: A virtual function that processes the message list, displaying each message and playing the corresponding sound if available.
- **AddMessage**: Adds a new message and its associated sound ID to the lists.

CONNECTS TO:
Depends on `Action.h`.

NOTES:
None.

## `src/training/mission10.cpp`

**PURPOSE:** The file `mission10.cpp` implements the tenth pseudo training mission in Allegiance called "Free Flight". This mission involves free exploration and does not have specific objectives.

**KEY PARTS:**
- **Mission10 Class:** Manages the "Free Flight" mission.
  - Constructor (`Mission10::Mission10()`) initializes the mission.
  - Destructor (`Mission10::~Mission10()`) cleans up resources when the mission ends.
  - `GetMissionID()` returns the ID of the mission.
  - `GetStartSectorID()` provides the starting sector for the mission.
  - `RestoreShip()` handles ending the mission and staying in override camera mode.
  - `ShipLanded()` shows an undock hint when a ship lands.
  - `HandlePickDefaultOrder(IshipIGC* pShip)` sets default orders for the player's ship based on nearby ships and targets.
  - `CreateUniverse()` loads the universe, sets up techs, and configures the starting loadout for the player's ship.
  - `CreateMission()` defines the mission goals and actions.

- **Goal Class:** Represents a sequence of actions that must be completed.
  - `AddStartAction` adds an action to the start of a goal.
  - `ElapsedTimeCondition` checks if a certain amount of time has elapsed.
  - `KillsReachedCondition` checks if a certain number of kills have been reached.

- **CreateDroneAction Class:** Creates a new drone (enemy ship) with specified properties.
- **ModifyCreateDroneActionAction Class:** Modifies the behavior or attributes of a `CreateDroneAction`.
  - `SetCreatedLocation`, `SetCreatedCommand`, and `SetCreatedBehaviour` configure the created ship's location, command, and behavior.

- **MessageAction Class:** Displays a message to the player or logs it.

**CONNECTS TO:**
- Other mission-related files and systems within Allegiance for managing game states, ships, and objectives.

**NOTES:**
- There are no explicit TODO comments in this part of the code.
- The commented-out section suggests there might be additional functionality related to waiting forever until the player dies, which is currently disabled.

## `src/training/mission10.h`

PURPOSE:
This header file defines the interface and implementation for Mission 10 in the training library of Allegiance. It extends the base `TrainingMission` class to provide specific functionality for this particular mission.

KEY PARTS:
- Class: Mission10 - Manages the details of Mission 10, including creating the universe, setting up the mission conditions, and handling events like ship landing.
- Function: GetMissionID() - Returns the unique identifier for Mission 10.
- Function: GetStartSectorID() - Provides the sector ID where the mission begins.
- Function: RestoreShip() - Handles restoring a player's ship at the start of the mission.
- Function: ShipLanded() - Checks if the player's ship has landed.
- Function: HandlePickDefaultOrder(IshipIGC* pShip) - Manages default orders for the player's ship during the mission.

CONNECTS TO:
- TrainingMission.h
- Goal.h

NOTES:
None.

## `src/training/mission2.cpp`

**PURPOSE:** The `mission2.cpp` file in Allegiance represents the second training mission for players. It defines the objectives and actions required to complete the mission, including playing introductory audio, controlling the ship's orientation, adjusting throttle settings, and interacting with various HUD elements.

**KEY PARTS:**
- **Class: Mission2**: Manages the overall structure of the mission.
- **Function: GetMissionID()**: Returns the unique identifier for this mission.
- **Function: GetStartSectorID()**: Specifies the starting sector where the mission begins.
- **Function: ShipLanded()**: Handles the scenario when a ship lands, terminating the mission if it occurs.
- **Function: CreateUniverse()**: Sets up the universe and positions the commander's ship.
- **Function: CreateMission()**: Constructs the main goal list for the mission.
- **Class: Goal**: Represents individual goals within the mission with associated actions.
- **Class: GoalList**: Manages a sequence of goals to be executed in order.
- **Functions: SetControlsAction, SetHUDOverlayAction, SetCommandAction**: Configure ship controls and HUD overlays.
- **Classes: ElapsedTimeCondition, GetKeyCondition, GetStateBitsCondition, PeriodicCondition, ConditionalAction**: Define conditions and actions based on game state.

**CONNECTS TO:** Other mission files and the game engine's core systems for handling gameplay mechanics and user input.

**NOTES:** The file includes TODO comments indicating areas that need further development, such as error handling and additional control instructions.

## `src/training/mission3.cpp`

PURPOSE: Implements the third training mission for Allegiance, setting up the environment and defining goals and conditions to be completed by the player.

KEY PARTS:
- Mission3: Manages the overall structure of the mission.
- GoalList: Organizes multiple goals that must be completed sequentially.
- ElapsedTimeCondition: Checks if a certain amount of time has passed.
- GetKeyCondition: Waits for a specific key press.
- SetControlsAction: Sets the controls for the player.
- ToggleWeaponAction: Toggles weapon usage on or off.
- RepairRearmAction: Repairs and rearms the player's ship.

CONNECTS TO: Allegiance game engine, mission system, and player input handling.

NOTES: None.

PARTS (in file order):

- lines 1-332
  - Mission3: Implements the first training mission.
  - GetMissionID: Returns the ID of the mission.
  - GetStartSectorID: Returns the starting sector for the mission.
  - RecordKeyPress: Handles key press events specific to the mission.
  - ShipLanded: Terminates the mission if the ship lands.
  - CreateUniverse: Sets up the game universe and initializes the mission environment.
  - CreateMission: Creates the main goal list and constraints for the mission.
  - CreateGotTargetCondition: Generates a condition that checks if any derelict is targeted.
- lines 334-688
  - Goal: Represents a single goal within a mission.
  - GoalList: A container for multiple goals that must be completed sequentially.
  - CreatePlaySoundGoal: Creates a goal that plays a sound when activated.
  - ElapsedTimeCondition: Checks if a certain amount of time has passed.
  - ConditionalAction: Executes an action only if a condition is true.
  - SetHUDOverlayAction: Changes the HUD overlay to display specific information.
  - DoDamageAction: Applies damage to the player's ship or target.
  - GetKeyCondition: Waits for a specific key press.
- lines 689-1044
  - Goal: Represents a single goal that can be achieved.
  - GoalList: A list of goals that must all be completed for the mission to succeed.
  - SetControlsAction: Sets the controls for the player.
  - ElapsedTimeCondition: Checks if a certain amount of time has passed.
  - GetShipIsDestroyedCondition: Checks if a ship is destroyed.
  - KeyCondition: Checks if a specific key has been pressed.
  - StateBitsCondition: Checks if a specific state bit is set.
  - ToggleWeaponAction: Toggles the weapon on or off.
- lines 1046-1159
  - GoalList: Manages a list of goals.
  - Goal: Represents a single goal with conditions and actions.
  - ElapsedTimeCondition: Checks if a certain amount of time has elapsed.
  - GetKeyCondition: Checks if a specific key is pressed.
  - GetTargetCondition: Checks if the player's ship targets a specific type of object within a range.
  - SetControlConstraintsAction: Enables or disables input controls.
  - ToggleWeaponAction: Toggles weapon usage.
  - RepairRearmAction: Repairs and rearms the player's ship.

## `src/training/mission4.cpp`

**PURPOSE:** Implements the second training mission in Allegiance, focusing on enemy engagement and strategic navigation.

**KEY PARTS:**
- **Mission4 Class:** Manages the overall structure of Mission 4.
- **GetMissionID Function:** Returns the unique ID for this mission.
- **CreateUniverse Function:** Sets up the initial state of the universe for the mission.
- **CreateMission Function:** Defines the main goal list for the mission.
- **Goal Classes:** Various classes representing different types of actions and conditions, such as setting controls, creating drones, and checking time elapsed.

**CONNECTS TO:**
- Other mission files and game systems for managing player interactions, enemy behavior, and universe state.

**NOTES:**
- Contains multiple TODO comments indicating areas for improvement or potential issues.
- Uses `SetControlConstraintsAction` to manage player input during specific parts of the mission.

## `src/training/mission5.cpp`

PURPOSE:
This file contains the implementation of Mission 5 in Allegiance's training system. It defines the objectives and actions for a specific training mission.

KEY PARTS:
- `Mission5`: Class representing Mission 5.
- `GetMissionID`, `GetStartSectorID`: Methods returning mission-specific identifiers.
- `SetupShipAndCamera`: Initializes the ship and camera settings for the mission.
- `RecordKeyPress`: Handles key press events, disabling certain keys during training.
- `CreateUniverse`: Loads the universe and sets up initial conditions for the mission.
- `CreateMission`: Defines the goals and actions for Mission 5.

CONNECTS TO:
- Training system components like `Goal`, `Condition`, and various action classes.
- Universe data and objects such as ships, stations, and warps.

NOTES:
- Contains hardcoded sound IDs (e.g., `tm_5_01Sound`) that need to be defined elsewhere.
- Includes comments indicating potential hacks or disabled functionality.

## `src/training/mission6.cpp`

**PURPOSE:** The `mission6.cpp` file manages the sixth training mission in Allegiance. It sets up the universe, defines mission goals and actions, and handles various events such as ship landings and default orders.

**KEY PARTS:**
- **Mission6 Class**: Manages the sixth training mission.
- **GetMissionID Function**: Returns the ID of the mission.
- **GetStartSectorID Function**: Returns the starting sector for the mission.
- **RestoreShip Function**: Ends the mission and prevents the engine from switching to override camera mode.
- **ShipLanded Function**: Handles actions when a ship lands, such as showing a hint.
- **HandlePickDefaultOrder Function**: Determines default orders for ships based on their position and surroundings.
- **CreateUniverse Function**: Loads the universe for the mission, including creating stations, setting up tech trees, and configuring player loadouts.
- **CreateMission Function**: Defines the goals and actions for the mission, such as creating friendly and enemy ships, playing sound effects, and displaying messages.

**CONNECTS TO:** Other mission files and game systems responsible for handling missions, player interactions, and universe generation.

**NOTES:**
- The `HandlePickDefaultOrder` function has a lot of conditional logic that might be simplified.
- Multiple instances of random waypoint positions could be optimized.
- The mission involves creating multiple enemy ships relative to the player's home station and carrier.

## `src/training/mission6.h`

PURPOSE: This file defines the header for the Mission6 class in the training library, which is part of a larger game system.

KEY PARTS:
- Mission6: Inherits from TrainingMission and implements specific methods to define and manage the parameters and behavior of Mission 6.
- GetMissionID: Returns the unique identifier for Mission 6.
- GetStartSectorID: Provides the sector ID where the mission starts.
- RestoreShip: Determines if a ship should be restored at the start of the mission.
- ShipLanded: Checks if a ship has landed in the designated area.
- HandlePickDefaultOrder: Handles the default order assignment for ships during the mission.
- CreateUniverse: Sets up the universe environment for Mission 6.
- CreateMission: Defines the specific conditions and goals for Mission 6.

CONNECTS TO: TrainingMission.h, Goal.h

NOTES: None.

## `src/training/mission8.cpp`

PURPOSE:
This file contains the implementation of Mission 8 in Allegiance, which is a nanite training mission. It defines the objectives, actions, and conditions for players to learn how to use the Nanite Repair System.

KEY PARTS:
- `Mission8`: Main class representing the nanite training mission.
- `CreateUniverse`: Initializes the game environment with specific ships, stations, and techs.
- `ShipLanded`: Handles events when the player's ship lands.
- `CreateGoal01`, `CreateGoal02`, `CreateGoal03`: Functions to create different goals within the mission, each focusing on a specific task or objective.

CONNECTS TO:
- Other mission-related files and classes in Allegiance for game logic and environment setup.

NOTES:
- None.

## `src/training/mission8.h`

PURPOSE: This file defines the header for Mission8, a training mission in Allegiance that extends the TrainingMission class.

KEY PARTS:
- Mission8: A class representing the specific details and behavior of Mission 8.
- GetMissionID: Returns the unique identifier for this mission.
- GetStartSectorID: Returns the sector ID where the mission starts.
- ShipLanded: Checks if a ship has landed in the mission.
- CreateUniverse: Initializes the universe for this mission.
- CreateMission: Creates the mission conditions.
- CreateGoal01, CreateGoal02, CreateGoal03: Methods to create specific goals within the mission.

CONNECTS TO:
- TrainingMission.h
- Goal.h

NOTES:
- None.

## `src/training/noaction.cpp`

PURPOSE:
This file contains the implementation of the "noaction" interface in the training library for Allegiance. It defines a class `NoAction` that represents an action with no actual functionality.

KEY PARTS:
- Class: NoAction - Represents an action with no actual functionality.
- Function: ~NoAction() - Destructor for the NoAction class.
- Function: Execute() - Method to execute the action, currently does nothing.

CONNECTS TO:
- None

NOTES:
- The `Execute()` method is empty and serves as a placeholder for actions that should not perform any operations.

## `src/training/noaction.h`

PURPOSE: Defines the header file for a training library interface called "noaction," which represents an action that does nothing.

KEY PARTS:
- Class NoAction: Inherits from Action and overrides the Execute method to perform no operation.
- Namespace Training: Contains the definition of the NoAction class.

CONNECTS TO: Depends on the Action.h header file, which defines the base Action class.

NOTES: None.

## `src/training/notcondition.h`

PURPOSE:
This header file defines an interface for a training library condition that negates the result of another condition.

KEY PARTS:
- NotCondition(pCondition): A macro that creates a Predicate object with the given condition and sets its value to false, effectively negating it.

CONNECTS TO:
Predicate.h - This file is included to use the Predicate class in defining the NotCondition macro.

NOTES:
None.

## `src/training/ntimescondition.cpp`

PURPOSE:
The file `ntimescondition.cpp` implements a training library condition that checks if another condition is met multiple times before returning true. It supports sticky behavior where the condition remains true after being met.

KEY PARTS:
- `NTimesCondition`: Class representing the n-times condition, which wraps another condition and evaluates it multiple times.
  - Constructor: Initializes with a wrapped condition, number of required evaluations, and sticky flag.
  - Destructor: Deletes the wrapped condition.
  - `Start()`: Starts the evaluation process by delegating to the wrapped condition.
  - `Stop()`: Stops the evaluation process by delegating to the wrapped condition.
  - `Evaluate()`: Evaluates the wrapped condition multiple times until it is met the required number of times, then returns true or maintains the sticky state.

CONNECTS TO:
- `Condition.h`: The base class for conditions used in the training library.

NOTES:
- None.

## `src/training/ntimescondition.h`

PURPOSE:
The file defines a class `NTimesCondition` within the `Training` namespace, which is part of a training library. This class extends the base `Condition` class and implements a condition that evaluates another condition multiple times before returning true.

KEY PARTS:
- Class: `NTimesCondition`
  - What it does: Extends `Condition` to evaluate another condition multiple times (`iTimes`) before returning true.
- Constructor: `NTimesCondition(Condition* pCondition, unsigned int iTimes, bool bSticky = false)`
  - Initializes the condition to be evaluated, the number of times to evaluate it, and whether the condition should remain sticky (true) or not after being met.
- Destructor: `~NTimesCondition()`
  - Cleans up resources used by the `NTimesCondition` instance.
- Method: `Start()`
  - Initializes the evaluation process.
- Method: `Stop()`
  - Stops the evaluation process.
- Method: `Evaluate()`
  - Evaluates the underlying condition multiple times and returns true if it meets the specified number of times.

CONNECTS TO:
- Depends on the `Condition` class, which is defined in "Condition.h".

NOTES:
- None.

## `src/training/objectmovingtowardscondition.cpp`

**PURPOSE:**  
This file contains the implementation of a condition class in the Allegiance training library that checks if an object is moving towards another target.

**KEY PARTS:**  
- `ObjectMovingTowardsCondition`: Class representing the condition where one object must be moving towards another.
  - Constructor: Initializes with references to the object and its target.
  - Destructor: Cleans up references.
  - `Evaluate`: Determines if the object is moving towards the target by comparing their vectors.

**CONNECTS TO:**  
- Depends on classes from the Allegiance game engine, specifically `ImodelIGC` for model interfaces.

**NOTES:**  
- The condition checks that both objects still exist to avoid crashes.
- It uses vector calculations to determine if the object's velocity is pointing towards the target.
- The angle subtended by the target's radius around the look vector is used to determine coverage.

## `src/training/objectmovingtowardscondition.h`

PURPOSE:
This header file defines a condition class for the training library in Allegiance, specifically for checking if an object is moving towards another target.

KEY PARTS:
- ObjectMovingTowardsCondition: A class that inherits from Condition and evaluates whether an object is moving towards a specified target.
- Evaluate: A virtual function that checks if the object's movement direction points towards the target.
- m_object: A reference to the object being checked.
- m_target: A reference to the target object.

CONNECTS TO:
Condition.h

NOTES:
None.

## `src/training/objectpointingatcondition.cpp`

PURPOSE:
The file `objectpointingatcondition.cpp` implements a training library condition that checks if an object is pointing at another object based on certain criteria.

KEY PARTS:
- Class: `ObjectPointingAtCondition`
  - Constructor overloads to initialize the condition with different types of targets.
  - Destructor to clean up dynamically allocated memory.
  - Method: `Evaluate` - Determines if the object is pointing at the target according to specified conditions.
  - Methods: `SetVectorMask`, `ClearVectorMask`, and `MaskedVector` - Manage and apply vector masking for more flexible angle calculations.
  - Method: `SetMinimumAngle` - Sets a minimum angle threshold for the condition.

CONNECTS TO:
- Depends on classes from the `TypeIDTarget` header, which likely handles object identification and retrieval.

NOTES:
- The code includes assertions to ensure that both objects exist before evaluating the condition.
- There is logic to handle different scenarios based on whether a specific minimum angle is set or not.
- Vector masking allows for more flexible angle calculations beyond just the center of the screen.

## `src/training/objectpointingatcondition.h`

**PURPOSE:** This header file defines the `ObjectPointingAtCondition` class within the Allegiance training library. The class is used to evaluate whether an object is pointing at a specific target based on certain conditions.

**KEY PARTS:**
- **Class: ObjectPointingAtCondition**
  - Inherits from `Condition`.
  - Constructor overloads for different ways to specify the object and target.
  - Destructor.
  - `Evaluate`: Checks if the object is pointing at the target within a specified angle and vector mask.
  - `SetVectorMask` and `ClearVectorMask`: Manage a vector mask that affects the evaluation.
  - `SetMinimumAngle`: Sets the minimum angle required for the condition to be true.

- **Data Members:**
  - `m_pObject`: Pointer to the object being evaluated.
  - `m_pTarget`: Pointer to the target object.
  - `m_vectorMask`: Vector mask that influences the evaluation.
  - `m_fMinimumAngle`: Minimum angle required for the condition to be true.

**CONNECTS TO:**
- Depends on classes from "Condition.h" and "AbstractTarget.h".

**NOTES:** None.

## `src/training/objectwithinradiuscondition.cpp`

**PURPOSE:**  
This file implements a condition in the training library that checks if an object is within a specified radius of another object.

**KEY PARTS:**  
- `ObjectWithinRadiusCondition`: Class representing the condition that checks if one object is within a certain radius of another.
- `Evaluate`: Function to evaluate whether the condition is met by checking the distance between the objects and their radii.
- `GetRadarRadius`: Function to return the radar radius for the condition, which is used to display the target on the radar.
- `Initialized`: Function to initialize the condition, ensuring that both objects exist and calculating the correct radius.

**CONNECTS TO:**  
`TypeIDTarget.h`, `ImodelIGC.h`

**NOTES:**  
None.

## `src/training/objectwithinradiuscondition.h`

PURPOSE:
The file defines a condition class used in the training library to check if an object is within a specified radius of another object.

KEY PARTS:
- Class: ObjectWithinRadiusCondition - Checks if one object is within a given radius of another object.
- Functions: 
  - Constructor overloads for different ways to specify objects and targets.
  - Destructor
  - Evaluate - Determines if the condition is met.
  - GetRadarRadius - Returns the squared radius used in the evaluation.
  - Initialized - Checks if the condition has been properly initialized.

CONNECTS TO:
Condition.h, AbstractTarget.h

NOTES:
None.

## `src/training/onetimeaction.cpp`

PURPOSE: This file implements a class `OneTimeAction` within the Allegiance game's training library. It manages the execution of an action only once.

KEY PARTS:
- OneTimeAction: Manages the execution of an action exactly once.
- Execute: Executes the encapsulated action if it hasn't been executed before.
- Stop: Stops the encapsulated action.
- m_pAction: Pointer to the action to be executed.
- m_bHasExecuted: Flag indicating whether the action has already been executed.

CONNECTS TO: Action.h (defines the Action interface)

NOTES: None.

## `src/training/onetimeaction.h`

PURPOSE: This header file defines the `OneTimeAction` class within the training library, which is a subclass of `Action`. It encapsulates another action to execute only once.

KEY PARTS:
- `OneTimeAction`: A class that inherits from `Action`, designed to execute another action only once.
  - Constructor: Initializes with another `Action` object.
  - Destructor: Cleans up resources.
  - `Execute()`: Executes the encapsulated action if it hasn't been executed yet.
  - `Stop()`: Stops the execution of the encapsulated action.
- `m_pAction`: A pointer to the encapsulated `Action` object.
- `m_bHasExecuted`: A boolean flag indicating whether the encapsulated action has already been executed.

CONNECTS TO: `Action.h`, which defines the base class for actions.

NOTES: None.

## `src/training/orcondition.cpp`

PURPOSE:
The file `orcondition.cpp` implements the training library's "OrCondition" interface, which evaluates whether at least one of two conditions is true.

KEY PARTS:
- OrCondition: Class representing an OR condition between two other conditions.
  - Constructor: Initializes with two Condition objects.
  - Destructor: Deletes both Condition objects.
  - Start: Starts both conditions and returns true if either condition starts successfully.
  - Stop: Stops both conditions.
  - Evaluate: Evaluates both conditions and returns true if at least one condition is true.

CONNECTS TO:
- Training::Condition: The base class for all conditions in the training library.

NOTES:
- None.

## `src/training/orcondition.h`

PURPOSE: This file defines the header for the `OrCondition` class in the training library, which represents a logical OR condition between two other conditions.

KEY PARTS:
- OrCondition: A class that inherits from Condition and evaluates to true if either of its two sub-conditions (m_pConditionA or m_pConditionB) is true.
- Start: Initializes the sub-conditions.
- Stop: Cleans up the sub-conditions.
- Evaluate: Returns true if either sub-condition is true.

CONNECTS TO: Depends on the Condition class defined in Condition.h.

NOTES: None.

## `src/training/pch.cpp`

PURPOSE: tiny file (1 lines), shown in full rather than summarized.

    #include "pch.h"

## `src/training/pch.h`

PURPOSE: This file is a precompiled header for the training module of Allegiance, containing common includes and macros used across various source files in that module.

KEY PARTS:
- `__MODULE__`: Defines the module name as "Training".
- `steam_api.h`: Includes Steam API for integration with Steam services.
- `#pragma warning(disable:4786)`: Disables a specific compiler warning about BSC name truncation.
- `<windows.h>` and related headers: Windows-specific headers for GUI, multimedia, and common dialogs.
- `<zlib.h>`: ZLIB compression library header.
- Various Allegiance-specific headers like `engine.h`, `effect.h`, `utility.h`, etc., which provide functionality for game engine, effects, utilities, zones, UI, input handling, resources, messages, client library, sound engine, missions, screens, and more.

CONNECTS TO: Other training module files and potentially other modules through Allegiance's architecture.

NOTES: None.

## `src/training/periodiccondition.cpp`

PURPOSE: This file implements a training library interface for periodic conditions in the Allegiance game.

KEY PARTS:
- PeriodicCondition: A class that encapsulates another condition and evaluates it periodically.
- Start: Initializes the start time and evaluates the enclosed condition immediately.
- Stop: Stops evaluating the enclosed condition.
- Evaluate: Checks if enough time has elapsed since the last evaluation and, if so, advances the clock base and evaluates the enclosed condition.

CONNECTS TO: Training library, Time module

NOTES: None

## `src/training/periodiccondition.h`

**PURPOSE:** This file defines a class `PeriodicCondition` within the Allegiance training library. It extends the base `Condition` class to create a condition that evaluates another condition periodically.

**KEY PARTS:**
- **Class: PeriodicCondition**
  - Inherits from `Condition`.
  - Takes another `Condition` and a period (in seconds) as parameters.
  - Starts, stops, and evaluates the nested condition at specified intervals.
  
- **Constructor:** Initializes the nested condition and sets the evaluation period.

- **Destructor:** Cleans up resources if necessary.

- **Start Method:** Begins the periodic evaluation of the nested condition.

- **Stop Method:** Stops the periodic evaluation.

- **Evaluate Method:** Checks if it's time to evaluate the nested condition based on the set period and calls its `Evaluate` method if so.

**CONNECTS TO:**
- Depends on the `Condition` class defined in `Condition.h`.

**NOTES:**
- None.

## `src/training/playsoundaction.cpp`

**PURPOSE:**  
This file contains the implementation of a class `PlaySoundAction` within the Allegiance game's training library. The class is responsible for playing sound effects during gameplay.

**KEY PARTS:**
- **Class: PlaySoundAction**
  - Manages the playback of sound effects.
  - Initializes with a `SoundID`.
  - Executes by starting the sound using `trekClient.StartSound()`.
  - Stops the sound if it is currently playing.
  - Tracks whether the sound has started and stopped.

- **Function: Execute()**
  - Starts the sound associated with the `m_soundID`.

- **Function: Stop()**
  - Stops the sound if it is currently playing.

- **Function: HasStarted()**
  - Returns true if the sound has been started.

- **Function: HasStopped()**
  - Returns true if the sound has stopped playing.

**CONNECTS TO:**  
`trekClient` (likely a client object for managing game state and interactions).

**NOTES:**  
- The commented-out code suggests an alternative method to get the `ISoundPositionSource`, which might be used in future implementations.
- There is no explicit error handling or logging, which could be added for better debugging.

## `src/training/playsoundaction.h`

PURPOSE: This file defines the interface and implementation for a training library action that plays a sound.

KEY PARTS:
- PlaySoundAction: A class derived from Action that handles playing a sound identified by SoundID.
- Execute: Starts playing the sound.
- Stop: Stops playing the sound.
- HasStarted: Checks if the sound has started playing.
- HasStopped: Checks if the sound has stopped playing.
- m_soundID: The identifier for the sound to be played.
- m_soundInstance: A reference to the sound instance being managed.
- m_bHasStarted: Flag indicating whether the sound has started.

CONNECTS TO: Action.h

NOTES: None.

## `src/training/resetaction.cpp`

PURPOSE:
The file `resetaction.cpp` implements the training library's "resetaction" interface, which handles the reset process for training missions in Allegiance.

KEY PARTS:
- ResetAction: Manages the reset process, including playing sounds and executing actions.
- PlaySound: Plays a sound and sets the next stage of the reset process.
- Initialize: Initializes the reset count to zero.
- Execute: Executes the current stage of the reset process.
- PlaySoundAction: A class that plays a sound action.
- SuspendedSoundFinishedCondition: A condition that waits for a sound to finish playing before continuing.
- ConditionalAction: An action that executes based on a condition.
- ProxyAction: A proxy action that delegates execution back to the ResetAction.

CONNECTS TO:
- TrainingMission: The mission object that manages the reset process and adds wait conditions.

NOTES:
- There is disabled code within the Execute function, which was commented out.

## `src/training/resetaction.h`

PURPOSE:
This header file defines the `ResetAction` class within the training library, which is responsible for managing reset actions in a game context. It includes functionality to execute reset stages and play associated sounds.

KEY PARTS:
- ResetAction: Manages different stages of a reset action and plays sounds at specific stages.
  - ResetStage: Enumerates the stages of a reset action (START, NOTIFY, SPECIFIC_NOTIFY, COMPLETE).
  - Execute: Executes the current stage of the reset action.
  - Initialize: Initializes static members of the class.
  - PlaySound: Plays a sound associated with the reset action and advances to the next stage.

CONNECTS TO:
- Action.h
- Goal.h

NOTES:
- None.

## `src/training/resetshipaction.cpp`

PURPOSE: This file implements the `ResetShipAction` class, which is part of the training library in Allegiance. It handles resetting a ship's state based on its current position and attributes.

KEY PARTS:
- `ResetShipAction`: Constructor initializes the ship reference and state pointer.
- `~ResetShipAction`: Destructor cleans up the state pointer if it was allocated.
- `Execute`: Resets the ship's state to either the stored state or the current mission state, updating various properties like position, velocity, orientation, fuel, shield, hull, ammo, and energy.

CONNECTS TO: `IshipIGC`, `IshieldIGC` (interfaces for ship and shield components), likely other training library components.

NOTES: None.

## `src/training/resetshipaction.h`

PURPOSE: This header file defines the `ResetShipAction` class within the Allegiance game's training library. The class is responsible for resetting a ship to its initial state during training scenarios.

KEY PARTS:
- ResetShipAction: A class that inherits from `Action`. It encapsulates the logic to reset a ship's properties such as position, velocity, orientation, fuel, shield, hull, ammo, and energy.
- State: A struct within `ResetShipAction` that holds the state of the ship before it is reset.

CONNECTS TO:
- Action.h: The base class for actions in the training library.

NOTES:
- None.

## `src/training/setcommandaction.cpp`

**PURPOSE:**  
This file contains the implementation of the `SetCommandAction` class, which is part of the training library in Allegiance. The class is responsible for setting commands for ships based on given parameters and executing those commands.

**KEY PARTS:**
- **Class: SetCommandAction** - Manages setting commands for ships.
  - **Constructor Overloads:** Initializes `SetCommandAction` with different combinations of ship, target, command, and command ID.
  - **Destructor:** Cleans up dynamically allocated memory.
  - **Execute Method:** Applies the set command to the ship if it exists. Handles special cases for player commands and updates the UI accordingly.

**CONNECTS TO:**
- `TypeIDTarget.h` and `TrainingMission.h` - For managing targets and mission-related data.
- `IshipIGC`, `ImodelIGC`, and `TrekWindow` - Interfaces for ship, model, and window operations respectively.

**NOTES:**  
None.

## `src/training/setcommandaction.h`

PURPOSE:
This header file defines the `SetCommandAction` class within the Allegiance game's training library. It is used to set commands for ships in the game.

KEY PARTS:
- `SetCommandAction`: A class that inherits from `Action`. It represents an action to set a command for a ship.
  - Constructors: Initialize the action with different combinations of parameters including ship, command, and target.
  - Destructor: Cleans up resources when the action is destroyed.
  - `Execute`: Executes the action, setting the specified command on the target.

CONNECTS TO:
- Depends on `Action.h` and `AbstractTarget.h`.

NOTES:
- None.

## `src/training/setdisplaymodeaction.cpp`

PURPOSE:
This file contains the implementation of a class responsible for setting display modes in a game window during training sessions.

KEY PARTS:
- SetDisplayModeAction: Class that handles setting the display mode based on the camera mode provided.
- Execute: Method that executes the action by changing the view mode and enabling/disabling specific commands.

CONNECTS TO:
TrekWindow: The main window class where the display mode is set and the camera mode is managed.

NOTES:
None.

## `src/training/setdisplaymodeaction.h`

PURPOSE: This file defines a class `SetDisplayModeAction` within the Training library, which is used to change the display mode in a game window.

KEY PARTS:
- Class: SetDisplayModeAction - Inherits from Action and sets the display mode of the game window.
- Function: Execute - Executes the action to change the display mode.
- Member Variable: m_mode - Stores the camera mode to be set.

CONNECTS TO: Action.h (base class)

NOTES: None.

## `src/training/soundfinishedcondition.cpp`

PURPOSE:
This file implements a condition in the training library that checks whether a sound has finished playing.

KEY PARTS:
- SoundFinishedCondition: Class representing the condition to check if a sound has finished playing.
  - Constructor initializes with a pointer to a PlaySoundAction object and sets m_bHasStarted to false.
  - Destructor does nothing.
  - Evaluate: Method that returns true when the sound has stopped playing, after checking if it has started.

CONNECTS TO:
- SoundFinishedCondition depends on PlaySoundAction for determining whether the sound has started or stopped.

NOTES:
None.

## `src/training/soundfinishedcondition.h`

PURPOSE:
This header file defines the `SoundFinishedCondition` class, which is part of a training library in Allegiance. It represents a condition that checks whether a sound has finished playing.

KEY PARTS:
- SoundFinishedCondition: A class derived from Condition that evaluates whether a sound has finished playing.
  - Constructor: Takes a pointer to a PlaySoundAction object.
  - Destructor: Cleans up resources.
  - Evaluate: Checks if the sound associated with the PlaySoundAction has finished playing.
  - m_pPlaySoundAction: Pointer to the PlaySoundAction object whose sound status is being checked.
  - m_bHasStarted: Flag indicating whether the sound has started playing.

CONNECTS TO:
- Condition.h
- PlaySoundAction.h

NOTES:
None.

## `src/training/trainingmission.cpp`

**PURPOSE:** The `trainingmission.cpp` file manages the logic for training missions in Allegiance. It handles setting up conditions, actions, and game events specific to training scenarios.

**KEY PARTS:**
- **TrainingMission Class:** Manages the overall training mission logic.
  - **Constructor (`TrainingMission::TrainingMission()`):** Initializes member variables and sets up HUD style.
  - **Destructor (`TrainingMission::~TrainingMission()`):** Cleans up resources, handles mission termination, and restores HUD style.
  - **Setup Ship and Camera (`TrainingMission::SetupShipAndCamera()`):** Sets up the player ship and camera for training.
  - **Start (`TrainingMission::Start()`):** Initializes the mission by clearing loadouts, creating the universe, and starting conditions.
  - **Frame (`TrainingMission::Frame()`):** Main game loop function evaluating conditions and handling respawn mechanisms.
  - **Terminate (`TrainingMission::Terminate()`):** Pauses the game clock and sets the current condition to a dead state.
  - **Add Wait Condition (`TrainingMission::AddWaitCondition(Condition* pWaitCondition)`):** Adds wait conditions checked during each frame.
  - **Add Respawn (`TrainingMission::AddRespawn(CreateDroneAction* pCreateDroneAction, float downtime, char spawnGroup, bool spawnOnAdd)`):** Manages respawn logic for ships or drones.
  - **Remove Respawn (`TrainingMission::RemoveRespawn(ShipID shipID)`):** Removes a specific respawn entry from the mission.
  - **Record Chat (`TrainingMission::RecordChat(ChatTarget recipient)`):** Records chat messages for condition evaluation.
  - **Set Chat Condition (`TrainingMission::SetChatCondition(Condition* pChatCondition)`):** Sets or updates the chat condition used in the mission.
  - **Record Key Press (`TrainingMission::RecordKeyPress(TrekKey key)`):** Handles key press events during training.

- **Functions for Handling Events:**
  - **Ship Killed (`TrainingMission::ShipKilled(IshipIGC* pShip, ImodelIGC* pLauncher)`):** Increments kill count when an enemy wingman is killed.
  - **Handle Pick Default Order (`TrainingMission::HandlePickDefaultOrder(IshipIGC* pShip)`):** Always returns false for default order handling.
  - **Kill Station Event (`TrainingMission::KillStationEvent(IstationIGC* pStation, ImodelIGC* pLauncher)`):** Handles station destruction events and ends mission if all stations are destroyed.
  - **Get Commander Ship (`TrainingMission::GetCommanderShip(void) const`):** Returns the commander ship of the player.
  - **Add Part to Ship (`TrainingMission::AddPartToShip(IshipIGC* pShip, PartID part, Mount mount, short ammo)`):** Adds a specified part to a given ship.
  - **Create Drone (`TrainingMission::CreateDrone(const ZString& name, ShipID shipID, HullID hullID, SideID sideID, PilotType pilotType)`):** Creates and returns a new drone with specified parameters.
  - **Load Universe (`TrainingMission::LoadUniverse(const ZString& name, HullID hullID, StationID homeStationID)`):** Loads the universe for training missions.
  - **Default Loadout (`TrainingMission::DefaultLoadout(IshipIGC* pShip)`):** Sets up default loadouts for a ship.
  - **Create Player Info (`TrainingMission::CreatePlayerInfo(IshipIGC* pShip, SideID side, ShipID shipID)`):** Creates player information for the given ship.

**CONNECTS TO:** Other game systems and files related to mission management, such as `condition.cpp`, `droneaction.cpp`, and `universe.cpp`.

**NOTES:** None.

## `src/training/trainingmission.h`

PURPOSE:
The file `trainingmission.h` defines the interface for a training mission in the Allegiance game. It includes classes and functions related to managing and controlling training missions.

KEY PARTS:
- **Class: TrainingMission**
  - Manages the overall state and behavior of a training mission.
  - Provides methods for setting up, starting, and terminating the mission.
  - Handles conditions, respawns, chat interactions, and ship management.

- **Struct: RespawnData**
  - Stores data related to respawn events, including the action to create a drone, downtime, spawn group, and time of destruction.

- **Function: Start()**
  - Initializes the training mission.

- **Function: Frame()**
  - Processes one frame of the mission logic.

- **Function: AddRespawn()**
  - Adds a respawn event to the mission.

- **Function: RemoveRespawn()**
  - Removes a respawn event from the mission.

- **Function: RecordKeyPress()**
  - Records key presses during the mission.

- **Function: GetCommanderShip()**
  - Returns the commander ship of the player in the mission.

CONNECTS TO:
- `Goal.h`: For goal-related functionality.
- `CreateDroneAction.h`: For actions related to creating drones.

NOTES:
- None.

## `src/training/truecondition.cpp`

**PURPOSE:**  
This file implements a training library interface named "truecondition" in the Allegiance game. It provides a condition that always evaluates to true.

**KEY PARTS:**  
- `TrueCondition::~TrueCondition()`: Destructor for the TrueCondition class.
- `TrueCondition::Start()`: Method that starts the condition and returns true.
- `TrueCondition::Evaluate()`: Method that evaluates the condition and always returns true.

**CONNECTS TO:**  
None

**NOTES:**  
None

## `src/training/truecondition.h`

PURPOSE:
The file `truecondition.h` defines the header for a class in the training library that represents a condition which is always true.

KEY PARTS:
- Class: TrueCondition
  - Inherits from Condition
  - Destructor: ~TrueCondition()
  - Method: Start() - Initializes the condition.
  - Method: Evaluate() - Always returns true, indicating the condition is met.

CONNECTS TO:
- Depends on `Condition.h` for the base class definition.

NOTES:
- None.
