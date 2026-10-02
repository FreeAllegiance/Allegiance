# src/Wopr/Wopr/Entities

[Back to the index](../LLAMA_SUMMARY.md) - 10 files, 322 lines. Notes by a local model, to orient by: not verified facts.

This folder contains classes and structures related to game entities and information management in Allegiance.

- GameInfo.cs: Manages state information between bots within a team, including exploration sweeps and unexplored areas.
- ServerCoreInfo.cs: Represents information about server cores, including their name, remote address, and current games.
- PersistPlayerScoreObject.cs: Defines a structure to persist player scores in the game.
- CoreInfo.cs: Holds details about a core within the game, such as its name and whether it is an official core.
- MessageElement.cs: Contains utility methods for reading message elements from binary streams.

Largest files: GameInfo.cs (101 lines), ServerCoreInfo.cs (36 lines), PersistPlayerScoreObject.cs (30 lines), CoreInfo.cs (29 lines), MessageElement.cs (27 lines).

## `src/Wopr/Wopr/Entities/CoreInfo.cs`

PURPOSE: The `CoreInfo` class in the Allegiance game's source code is designed to represent information about a core within the game. It includes details such as the core's name, whether it is an official core, a bit value, and static core information.

KEY PARTS:
- `Name`: A string representing the name of the core, limited to 25 characters.
- `bOfficial`: A boolean indicating whether the core is an official core.
- `dwBit`: An integer representing a bit value associated with the core.
- `mStatic`: An instance of `StaticCoreInfo`, which likely contains additional static information about the core.

CONNECTS TO: This class depends on `MessageElement` and `StaticCoreInfo`.

NOTES: None.

## `src/Wopr/Wopr/Entities/GameInfo.cs`

PURPOSE:
The `GameInfo` class in Allegiance's Wopr game is used to share state information between bots within a team. It manages clusters and warps, tracks exploration sweeps, and identifies unexplored areas.

KEY PARTS:
- **Clusters**: A list of all clusters and warps in the game.
- **SweepingScoutCountByClusterObjectID**: Tracks the number of scouts currently sweeping each cluster.
- **GetSweepingScoutCount**: Retrieves the current sweep count for a given cluster.
- **IncrementSweepingScoutCount**: Increments the sweep count for a given cluster.
- **DecrementSweepingScoutCount**: Decrements the sweep count for a given cluster.
- **GetUnexploredClusters**: Identifies clusters that have not been fully explored or contain critical resources.

CONNECTS TO:
- `AllegianceInterop`: Interoperability library for Allegiance game objects.
- `IclusterIGCWrapper` and `ImissionIGCWrapper`: Interfaces for accessing game cluster and mission data.

NOTES:
- The commented-out `UnexploredClustersByObjectID` suggests an alternative approach to tracking unexplored clusters.
- The class is marked as `[Serializable]`, indicating it can be serialized for network communication or storage.

## `src/Wopr/Wopr/Entities/ISupportReadingFromStreams.cs`

PURPOSE: tiny file (14 lines), shown in full rather than summarized.

    using System;
    using System.Collections.Generic;
    using System.IO;
    using System.Linq;
    using System.Text;
    using System.Threading.Tasks;
    namespace Wopr.Entities
    {
        public interface ISupportReadingFromStreams
        {
            void Read(BinaryReader br);
            int GetByteSizeOfEntity();
        }
    }

## `src/Wopr/Wopr/Entities/InflightConstructor.cs`

PURPOSE: tiny file (16 lines), shown in full rather than summarized.

    using AllegianceInterop;
    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Text;
    using System.Threading.Tasks;
    using Wopr.Constants;
    namespace Wopr.Entities
    {
        public class InflightConstructor
        {
            public IclusterIGCWrapper Cluster;
            public IshipIGCWrapper Ship;
            public StationType StationType;
        }
    }

## `src/Wopr/Wopr/Entities/MessageElement.cs`

PURPOSE: The file `MessageElement.cs` contains utility methods for reading message elements from a binary stream in the Allegiance game.

KEY PARTS:
- `ReadString(BinaryReader br, int maxLength)`: Reads a string from a binary reader up to a specified maximum length. It handles ASCII encoding and trims any null characters at the end of the string.

CONNECTS TO: This file is used by other parts of the game that need to parse message data from binary streams.

NOTES:
- The commented-out `ReadInt32` method was likely intended for reading a 32-bit integer but was not implemented.

## `src/Wopr/Wopr/Entities/PersistPlayerScoreObject.cs`

PURPOSE: This file defines a structure to persist player scores in the Allegiance game, including total score, combat rating, rank, and civilization ID.

KEY PARTS:
- PersistPlayerScoreObject: A struct that implements ISupportReadingFromStreams interface.
  - m_totalScore: Stores the total score of the player.
  - m_fCombatRating: Stores the combat rating of the player.
  - m_rank: Stores the rank of the player.
  - m_civID: Stores the civilization ID associated with the player.
  - GetByteSizeOfEntity(): Returns the size in bytes of the struct.
  - Read(BinaryReader br): Reads data from a BinaryReader stream into the struct.

CONNECTS TO: ISupportReadingFromStreams interface, likely used for reading game state data from streams.

NOTES: None.

## `src/Wopr/Wopr/Entities/ServerCoreInfo.cs`

PURPOSE: This file defines a class `ServerCoreInfo` that represents information about a server core in the Allegiance game. It includes details such as the server's name, remote address, location, current and maximum games, and core mask.

KEY PARTS:
- Name: szName - Stores the name of the server.
- Remote Address: szRemoteAddress - Stores the IP address or hostname of the server.
- Location: szLocation - Stores the location of the server.
- Current Games: iCurGames - Indicates the number of games currently running on the server.
- Maximum Games: iMaxGames - Specifies the maximum number of games that can run on the server.
- Core Mask: dwCoreMask - A 32-bit mask representing the cores available to the server.

CONNECTS TO: This class is used by other parts of the game to manage and communicate with server cores, likely through network messages or configuration files.

NOTES: None.

## `src/Wopr/Wopr/Entities/ServerInfo.cs`

PURPOSE: This file defines a class `ServerInfo` used to store and read information about game servers.

KEY PARTS:
- `ServerCoreInfo mStatic`: Holds core information about the server.
- `int ping`: Stores the latency (ping) of the server.
- `bool bOfficial`: Indicates whether the server is an official server or not.

CONNECTS TO: Depends on `ServerCoreInfo`.

NOTES: None.

## `src/Wopr/Wopr/Entities/StaticCoreInfo.cs`

PURPOSE: tiny file (19 lines), shown in full rather than summarized.

    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Text;
    using System.Threading.Tasks;
    using System.IO;
    namespace Wopr.Entities
    {
        public class StaticCoreInfo : MessageElement
        {
            public string cbIGCFile; // c_cbFileName = 13
            internal static StaticCoreInfo Read(BinaryReader br)
            {
                StaticCoreInfo returnValue = new StaticCoreInfo();
                returnValue.cbIGCFile = ReadString(br, 13);
                return returnValue;
            }
        };
    }

## `src/Wopr/Wopr/Entities/TeamDirectorPlayerInfo.cs`

PURPOSE: tiny file (15 lines), shown in full rather than summarized.

    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Text;
    using System.Threading.Tasks;
    namespace Wopr.Entities
    {
        public class TeamDirectorPlayerInfo
        {
            public string PlayerName;
            public short SideIndex;
            public bool IsCommander;
            public bool IsGameController;
        }
    }
