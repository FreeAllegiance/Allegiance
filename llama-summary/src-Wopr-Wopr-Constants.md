# src/Wopr/Wopr/Constants

[Back to the index](../LLAMA_SUMMARY.md) - 20 files, 771 lines. Notes by a local model, to orient by: not verified facts.

This folder contains various constants and enumerations used throughout the Allegiance game engine to define different types of data and actions. It provides a centralized location for managing game-specific values and identifiers.

- BitReader.cs: Reads bit-based data from a binary stream, ensuring alignment with C++ struct alignment.
- MessageType.cs: Defines an enumeration of message types used in communication between clients and servers.
- ObjectType.cs: Defines an enumeration of object types used to categorize different entities within the game world.
- TargetType.cs: Defines an enumeration of target types for identifying different entities and conditions that can be targeted.
- ItemType.cs: Defines an enumeration of item types used in the game.

Largest files: MessageType.cs (190 lines), BitReader.cs (142 lines), ObjectType.cs (70 lines), TargetType.cs (54 lines), ItemType.cs (32 lines).

## `src/Wopr/Wopr/Constants/BitReader.cs`

PURPOSE: The `BitReader` class is designed to read bit-based data from a binary stream, providing methods to read various primitive types like integers, booleans, shorts, floats, and bytes. It ensures alignment with C++ struct alignment by padding the data as needed.

KEY PARTS:
- `BitReader`: Main class that handles reading bit-based data.
- `CreateUsingInt32Containers`: Static method to create a `BitReader` instance using 32-bit integer containers.
- `CreateUsingByteContainers`: Static method to create a `BitReader` instance using byte containers.
- `ReadUInt32`: Reads an unsigned 32-bit integer from the stream.
- `ReadBool`: Reads a boolean value from the stream.
- `ReadShort`: Reads a short (16-bit) integer from the stream.
- `_int32DataBuckets` and `_byteDataBuckets`: Lists to store the data read from the binary stream.
- `_currentDataBucket`: Tracks the current bucket being read.

CONNECTS TO: `BinaryReader` for reading from the binary stream.

NOTES: None.

## `src/Wopr/Wopr/Constants/BuoyType.cs`

PURPOSE: tiny file (13 lines), shown in full rather than summarized.

    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Text;
    using System.Threading.Tasks;
    namespace Wopr.Constants
    {
        public enum BuoyType
        {
            c_buoyWaypoint = 0,
            c_buoyCluster = 1
        }
    }

## `src/Wopr/Wopr/Constants/Civilization.cs`

PURPOSE: tiny file (17 lines), shown in full rather than summarized.

    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Text;
    using System.Threading.Tasks;
    namespace Wopr.Constants
    {
        // Matched up to CivID in the cores.
        // TODO: Add the rest should we get AIs done for these base ones.
        public enum  Civilization : short
        {
            Belters = 101,
            //Bios = 35,
            Gigacorp = 18,
            IronCoalition = 25
        }
    }

## `src/Wopr/Wopr/Constants/CommandID.cs`

PURPOSE: This file defines an enumeration of command IDs used in the Allegiance game for various actions that can be issued to units.

KEY PARTS:
- CommandID: An enumeration type containing predefined constants representing different commands that units can execute.

CONNECTS TO: Used by unit AI and player input systems to determine what action a unit should perform.

NOTES: none

## `src/Wopr/Wopr/Constants/CommandType.cs`

PURPOSE: tiny file (16 lines), shown in full rather than summarized.

    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Text;
    using System.Threading.Tasks;
    namespace Wopr.Constants
    {
        public enum CommandType
        {
            c_cmdQueued = 0,
            c_cmdAccepted = 1,
            c_cmdCurrent = 2,
            c_cmdPlan = 3,
            c_cmdMax = 4
        }
    }

## `src/Wopr/Wopr/Constants/EquipmentType.cs`

PURPOSE: This file defines an enumeration of equipment types used in the Allegiance game.

KEY PARTS:
- ET_Turret: Represents a turret weapon system.
- NA: Indicates no equipment type.
- ET_ChaffLauncher: A chaff launcher for electronic warfare.
- ET_Weapon: General category for weapons.
- ET_Magazine: Stores ammunition for weapons.
- ET_Dispenser: Dispenses items or resources.
- ET_Shield: Provides protection against damage.
- ET_Cloak: Allows the ship to become invisible.
- ET_Pack: Contains various equipment items.
- ET_Afterburner: Increases ship speed by consuming fuel.
- ET_MAX: Indicates the maximum value of the enumeration.

CONNECTS TO: src/Wopr/Wopr/Entities/Ship.cs

NOTES: None.

## `src/Wopr/Wopr/Constants/InvestResult.cs`

PURPOSE: tiny file (16 lines), shown in full rather than summarized.

    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Text;
    using System.Threading.Tasks;
    namespace Wopr.Constants
    {
        public enum InvestResult
        {
            Succeeded,
            InsufficientFunds,
            AlreadyInProgress,
            InvalidTechName,
            NotAvailableYet
        }
    }

## `src/Wopr/Wopr/Constants/ItemType.cs`

PURPOSE: This file defines an enumeration of item types used in the Allegiance game.

KEY PARTS:
- ItemType: An enumeration representing different types of items in the game, including dispensers, consumables, missiles, and weapons.

CONNECTS TO: Wopr namespace, likely used by other parts of the game to reference item types.

NOTES: None.

## `src/Wopr/Wopr/Constants/MessageType.cs`

PURPOSE: This file defines an enumeration of message types used in the Allegiance game for communication between clients and servers.

KEY PARTS:
- FM_C_LOGONREQ: Client requests to log on.
- FM_S_LOGONACK: Server acknowledges client's logon request.
- FM_CS_LOGOFF: Client logs off.
- FM_C_BUY_LOADOUT: Client buys a loadout.
- FM_S_BUY_LOADOUT_ACK: Server acknowledges the buy loadout request.
- FM_S_OBJECT_SPOTTED: Server notifies clients of an object spotted.
- FM_S_SET_CLUSTER: Server sets cluster information.
- FM_S_RANK_INFO: Server sends rank information to client.

CONNECTS TO: Other game systems for handling player logins, purchases, and notifications.

NOTES: None.

## `src/Wopr/Wopr/Constants/MissionStage.cs`

PURPOSE: tiny file (17 lines), shown in full rather than summarized.

    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Text;
    using System.Threading.Tasks;
    namespace Wopr.Constants
    {
        // From igc.h
        public enum MissionStage
        {
            STAGE_NOTSTARTED,
            STAGE_STARTING,
            STAGE_STARTED,
            STAGE_OVER,
            STAGE_TERMINATE
        }
    }

## `src/Wopr/Wopr/Constants/ObjectType.cs`

PURPOSE: This file defines an enumeration of object types used in the Allegiance game engine to categorize different entities within the game world.

KEY PARTS:
- OT_invalid: Represents an invalid or unknown object type.
- OT_modelBegin and OT_modelEnd: Mark the beginning and end of model-related object types.
- OT_staticBegin and OT_staticEnd: Mark the beginning and end of static object types.
- OT_partBegin, OT_partType, and OT_partEnd: Define categories for parts that can be attached to models.
- OT_constants: Indicates a special category for constants.

CONNECTS TO: This enumeration is used throughout the game engine to identify and handle different types of objects in the game world.

NOTES: None.

## `src/Wopr/Wopr/Constants/PilotType.cs`

PURPOSE: tiny file (18 lines), shown in full rather than summarized.

    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Text;
    using System.Threading.Tasks;
    namespace Wopr.Constants
    {
        public enum PilotType
        {
            c_ptMiner = 0,
            c_ptWingman = 2,
            c_ptLayer = 5,
            c_ptBuilder = 6,
            c_ptCarrier = 9,
            c_ptPlayer = 10,
            c_ptCheatPlayer = 11
        }
    }

## `src/Wopr/Wopr/Constants/ServerType.cs`

PURPOSE: tiny file (13 lines), shown in full rather than summarized.

    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Text;
    using System.Threading.Tasks;
    namespace Wopr.Constants
    {
        public enum ServerType
        {
            Lobby,
            Server
        }
    }

## `src/Wopr/Wopr/Constants/ShipState.cs`

PURPOSE: tiny file (16 lines), shown in full rather than summarized.

    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Text;
    using System.Threading.Tasks;
    namespace Wopr.Constants
    {
        public enum ShipState
        {
            c_ssDead = 1,                //A nice enumeration of all the things it could have been doing
            c_ssDocked = 2,              //when it was last seen (if only briefly)
            c_ssFlying = 3,
            c_ssObserver = 4,
            c_ssTurret = 5
        }
    }

## `src/Wopr/Wopr/Constants/ShipType.cs`

PURPOSE: tiny file (17 lines), shown in full rather than summarized.

    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Text;
    using System.Threading.Tasks;
    namespace Wopr.Constants
    {
        public enum ShipType
        {
            Scout,
            Fighter,
            Interceptor,
            StealthFighter,
            Bomber,
            Gunship
        }
    }

## `src/Wopr/Wopr/Constants/StationAbilityBitMask.cs`

PURPOSE: This file defines a bitmask enumeration `StationAbilityBitMask` used to represent various abilities that can be associated with stations in the game Allegiance.

KEY PARTS:
- `c_sabmUnload`: Ability to offload mined minerals.
- `c_sabmStart`: Start the game at this station.
- `c_sabmRestart`: Restart after dying.
- `c_sabmRipcord`: Teleport to the station.
- `c_sabmCapture`: Be captured.
- `c_sabmLand`: Land at the station.
- `c_sabmRepair`: Get repaired.
- `c_sabmRemoteLeadIndicator`: Relay lead indicator.
- `c_sabmReload`: Free fuel and ammo on launch.
- `c_sabmFlag`: Counts for victory.
- `c_sabmPedestal`: Be a pedestal for a flag.
- `c_sabmTeleportUnload`: Offload mined materials without docking.
- `c_sabmCapLand`: Land capital ships.
- `c_sabmRescue`: Rescue pods.
- `c_sabmRescueAny`: Not used (but reserved for pods).

CONNECTS TO: This enumeration is likely used in other parts of the game code to check and set station abilities.

NOTES: None.

## `src/Wopr/Wopr/Constants/StationType.cs`

PURPOSE: tiny file (19 lines), shown in full rather than summarized.

    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Text;
    using System.Threading.Tasks;
    namespace Wopr.Constants
    {
        public enum StationType
        {
            Outpost,
            Teleport,
            Refinery,
            Garrison,
            Tactical,
            Supremacy,
            Expansion,
            ShipYard
        }
    }

## `src/Wopr/Wopr/Constants/StrategyID.cs`

PURPOSE: tiny file (14 lines), shown in full rather than summarized.

    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Text;
    using System.Threading.Tasks;
    namespace Wopr.Constants
    {
        public enum StrategyID
        {
            ConnectToGame,
            CommanderResearchAndExpand,
            ScoutExploreMap
        }
    }

## `src/Wopr/Wopr/Constants/TargetType.cs`

PURPOSE: This file defines an enumeration of target types used in the Allegiance game for identifying different entities and conditions that can be targeted.

KEY PARTS:
- `TargetType`: An enumeration representing various types of targets in the game, including special commands, ships, stations, asteroids, treasures, missiles, and more.
- `c_ttShipTypes`, `c_ttAllTypes`, `c_ttAllSides`, `c_ttAll`: Compound values combining multiple target types for convenience.

CONNECTS TO: Used by various parts of the game to determine how entities should be targeted or interacted with.

NOTES: None.

## `src/Wopr/Wopr/Constants/TechpathType.cs`

PURPOSE: tiny file (14 lines), shown in full rather than summarized.

    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Text;
    using System.Threading.Tasks;
    namespace Wopr.Constants
    {
        enum TechpathType
        {
            Supremacy,
            Expansion,
            Tactical
        }
    }
