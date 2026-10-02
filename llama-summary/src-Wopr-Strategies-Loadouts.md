# src/Wopr/Strategies/Loadouts

[Back to the index](../LLAMA_SUMMARY.md) - 2 files, 91 lines. Notes by a local model, to orient by: not verified facts.

This folder contains classes related to loadouts in the Allegiance game, which specify various item types that can be equipped by different units.

- LoadoutBase.cs: Defines a base class for loadouts, specifying common properties and methods for all loadouts.
- ScoutProbeLoadout.cs: Defines a specific loadout configuration for scout probes, detailing the items and settings unique to this unit type.

## `src/Wopr/Strategies/Loadouts/LoadoutBase.cs`

PURPOSE: Defines a base class for loadouts in the game, which specifies various item types that can be equipped.

KEY PARTS:
- LoadoutBase: Abstract class representing a loadout with properties for different weapon and cargo slots.
- GetItemTypeString: Method to convert an ItemType enum value to its corresponding string representation.

CONNECTS TO: Wopr.Constants.ItemType

NOTES: None.

## `src/Wopr/Strategies/Loadouts/ScoutProbeLoadout.cs`

PURPOSE: Defines a specific loadout configuration for a scout probe in the Allegiance game.

KEY PARTS:
- ScoutProbeLoadout: Class representing a loadout for a scout probe.
  - Weapon1: Gatling weapon.
  - Missiles: MRM missiles.
  - Dispenser: EWS probe dispenser.
  - Cargo1 to Cargo5: EWS probes and nanites.

CONNECTS TO: Strategies.Loadouts.LoadoutBase

NOTES: None.
