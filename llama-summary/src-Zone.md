# src/Zone

[Back to the index](../LLAMA_SUMMARY.md) - 4 files, 220 lines. Notes by a local model, to orient by: not verified facts.

The `src/Zone` folder in Allegiance contains essential components for managing game zones and their members. It includes interfaces and utilities for handling ranks, positions, statuses, and detailed statuses of zone members.

- Medal.h: Defines structures and functions related to medals, which are awarded to players based on achievements.
- Member.h: Provides a comprehensive interface for managing zone members, including rank management, position tracking, status updates, and detailed status information.
- Squad.h: Contains interfaces and constants for squad operations within zones, facilitating group actions and communications.
- Zone.h: Offers basic definitions and utilities related to game zones, setting the foundation for zone-specific functionalities.

## `src/Zone/Medal.h`

PURPOSE: tiny file (11 lines), shown in full rather than summarized.

    class IMedalBase  // inherited by IZoneMedal
    {
    public:
        virtual void            SetName(const char * szName)                  = 0;
        virtual void            SetDescription(const char * m_szDescription)  = 0;
        virtual void            SetRanking(int nRanking)                      = 0; // used for sorting
        virtual void            SetImageFileName(const char * szFileName)     = 0;
        virtual char *          GetName()           = 0;
        virtual char *          GetDescription()    = 0;
        virtual int             GetRanking()        = 0;
    };

## `src/Zone/Member.h`

**PURPOSE:**  
This file defines an interface for a member of a zone in the Allegiance game. It includes enumerations and utility functions to manage ranks, positions, statuses, and detailed statuses of members.

**KEY PARTS:**
- **IMemberBase:** An interface class that inherits from `IZonePlayer`, containing enums for rank, position, status, and detailed status.
- **Rank Enum:** Defines the ranks a member can have (Unknown, Private, Sergeant, General).
- **Position Enum:** Defines the positions a member can hold within a zone (Unknown, Member, ASL, Leader).
- **Status Enum:** Defines the statuses of a member's association with the zone (No Association, Booted, Rejected, Retired, Pending, Active).
- **DetailedStatus Enum:** Combines position and status to provide a more detailed view.
- **IsRankValid, IsPositionValid, IsStatusValid, IsDetailedStatusValid:** Static functions to validate the values of the enums.
- **Rank2String, Position2String, Status2String, DetailedStatus2String:** Static functions to convert enum values to their string representations.

**CONNECTS TO:**
- `IZonePlayer` (inherited by `IMemberBase`)

**NOTES:**
- None.

## `src/Zone/Squad.h`

PURPOSE: Defines interfaces and constants related to squads in the game.

KEY PARTS:
- ISquadBase: Interface for squad-related operations, including rank validation and conversion to strings.
  - IsRankValid: Checks if a given rank is valid.
  - Rank2String: Converts a rank enum to its corresponding string representation.
- SQUAD_SORT_COLUMN: Enumerates columns that can be used to sort squads.

CONNECTS TO: None

NOTES: None

## `src/Zone/Zone.h`

PURPOSE: tiny file (10 lines), shown in full rather than summarized.

    #ifndef ZONE_INC
    #define ZONE_INC
    const int c_cbURLDB = 255;
    const int c_cbSquadDescriptionDB = 510;
    const int c_cbLogDB = 500;
    const int c_cbZoneNameDB = 30; // size of zone names
    #include "Member.h"
    #include "Squad.h"
    #include "Medal.h"
    #endif
