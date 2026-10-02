# src/Wopr/Wopr/Loadouts

[Back to the index](../LLAMA_SUMMARY.md) - 2 files, 91 lines. Notes by a local model, to orient by: not verified facts.

The `Loadouts` folder in Allegiance contains classes related to managing different configurations of equipment and items that can be equipped on ships.

- LoadoutBase.cs: Defines a base class for loadouts, providing properties and methods to manage various types of items that can be equipped on a ship.
- ScoutProbeLoadout.cs: Specifies a particular loadout configuration designed for scout probes in the game.

## `src/Wopr/Wopr/Loadouts/LoadoutBase.cs`

PURPOSE: This file defines a base class for loadouts in the Allegiance game. It includes properties to store different types of items that can be equipped on a ship and a method to convert an `ItemType` enum value to its corresponding string representation.

KEY PARTS:
- `LoadoutBase`: An abstract class representing a ship's loadout.
  - Properties for various item types (`Weapon1`, `Weapon2`, etc.).
  - `GetItemTypeString(ItemType itemType)`: Converts an `ItemType` enum to a readable string.

CONNECTS TO: 
- `Wopr.Constants.ItemType`: The enumeration that defines the different types of items.

NOTES:
- None

## `src/Wopr/Wopr/Loadouts/ScoutProbeLoadout.cs`

PURPOSE: Defines a specific loadout configuration for a scout probe in the Allegiance game.

KEY PARTS:
- ScoutProbeLoadout: Inherits from LoadoutBase and sets up the initial configuration for a scout probe with weapons, missiles, dispensers, and cargo items.

CONNECTS TO: Wopr.Loadouts.LoadoutBase

NOTES: None.
