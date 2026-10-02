# src/_Utility

[Back to the index](../LLAMA_SUMMARY.md) - 19 files, 9,260 lines. Notes by a local model, to orient by: not verified facts.

The `src/_Utility` folder in Allegiance contains essential utility functions and classes used across various parts of the game. It provides core functionalities such as file operations, collision detection, network messaging, and spatial partitioning.

- CRC.cpp: Implements CRC32 checksum calculations for memory and files.
- CollisionEntry.cpp: Provides sorting algorithms for `CollisionEntry` objects based on their collision times.
- ConfigurationStore.cpp: Manages configuration settings using JSON and Windows Registry backends.
- Endpoint.cpp: Offers sorting algorithms for `Endpoint` objects, including quicksort and bubble sort.
- FileLoader.cpp: Handles loading and managing game resources like artwork files.
- HitTest.cpp: Contains functions and classes for collision detection and hit testing in the game.
- KDnode.cpp: Implements a KD-tree node for efficient spatial partitioning.
- KDroot.cpp: Manages a collection of `HitTest` objects using a KD-tree structure.
- Messages.cpp: Manages network messaging and player connections using DirectPlay.
- Utility.cpp: Provides utility functions and classes for various game-related operations.
- Utility.h: Contains utility classes and functions for collision detection, frame data handling, and other operations.

Largest files: Messages.cpp (2172 lines), HitTest.cpp (1948 lines), Utility.h (1513 lines), listwrappers.h (704 lines), Utility.cpp (646 lines).

## `src/_Utility/CRC.cpp`

PURPOSE:
This file contains implementations for calculating CRC32 checksums of memory and files. It provides functions to compute the CRC32 checksum for a block of data in memory and for reading a file into memory to compute its CRC32.

KEY PARTS:
- `CRC32_TABLE`: A lookup table used by the CRC32 algorithm.
- `update_crc`: Computes the CRC32 checksum for a given block of data. It has different implementations for 64-bit and 32-bit systems.
- `MemoryCRC`: Generates a 32-bit checksum for a block of memory.
- `FileCRC`: Reads a file into memory and computes its CRC32 checksum.

CONNECTS TO:
- Depends on Windows API functions like `GetFileSize`, `VirtualAlloc`, `ReadFile`, and `CloseHandle`.

NOTES:
- There is a hack in the 64-bit implementation to cast the result of `update_crc` to an integer, which might be unnecessary.
- Error handling for file operations includes storing error messages if provided.

## `src/_Utility/CRC.h`

PURPOSE: tiny file (20 lines), shown in full rather than summarized.

    #pragma once
    #include <windows.h>
    /***************************************************************************
    Function:		MemoryCRC()
    Parameters:		data: pointer to data
    				size: size of the data (must be non-zero)
    Purpose:		generate a 32-bit checksum
    Returns:		checksum of the data (never returns zero)
    ****************************************************************************/
    int MemoryCRC(const void *_data, unsigned size);
    /***************************************************************************
    Function:		FileCRC()
    Parameters:		szFileName:   file to get CRC from
                    szErrorMsg:   this can be NULL if you don't care about a msg,
                                  or else it must be able to store 100+MAX_PATH chars
    Returns:        Checksum of the file, returns 0 on error
    Example:		CRC = FileCRC("autoexec.bat", 0, 0);
    ****************************************************************************/
    int FileCRC(const char *szFilename, char * szErrorMsg);
    int FileCRC(HANDLE hFile,           char * szErrorMsg);

## `src/_Utility/CollisionEntry.cpp`

PURPOSE: This file contains implementation of sorting algorithms for `CollisionEntry` objects in the Allegiance game. The primary purpose is to efficiently sort arrays of `CollisionEntry` instances based on their collision times.

KEY PARTS:
- `swap(DataType* a, DataType* b)`: Swaps two `CollisionEntry` objects.
- `comp(DataType const * a, DataType const * b)`: Compares two `CollisionEntry` objects based on their collision times.
- `longSort(DataType* lo, DataType* hi)`: Implements a hybrid sorting algorithm that uses quicksort for large arrays and bubblesort for small arrays to sort `CollisionEntry` objects.
- `shortSort(DataType* lo, DataType* hi)`: Implements a bubble sort to sort `CollisionEntry` objects when the array size is small.

CONNECTS TO: `_Utility/CollisionEntry.h`, which defines the `CollisionEntry` class.

NOTES: None.

## `src/_Utility/CollisionQueue.cpp`

PURPOSE:
CollisionQueue.cpp manages a queue of collision events in the Allegiance game. It handles sorting and purging of collisions, as well as adding new collision entries.

KEY PARTS:
- CollisionQueue: Manages a queue of collision events.
- sort: Sorts collision entries based on their time of collision.
- flush: Removes and processes collision entries up to a specified index.
- purge: Clears all collision entries from the queue.
- addCollision: Adds a new collision entry to the queue.

CONNECTS TO:
Utility classes for handling collisions and hit tests.

NOTES:
- The constructor with parameters allows reusing an existing array of CollisionEntry objects, which is useful for performance optimization.
- The file includes debugf statements that would output messages to a debugger or log if enabled.

## `src/_Utility/ConfigurationStore.cpp`

PURPOSE: This file implements a configuration store using JSON and Windows Registry as backends. It provides methods to read and write various data types (bool, int, float, string) from these storage mechanisms.

KEY PARTS:
- JsonConfigurationStore: Manages configuration settings stored in a JSON file.
- RegistryConfigurationStore: Manages configuration settings stored in the Windows Registry.
- FallbackConfigurationStore: Combines both JSON and Registry stores to provide a fallback mechanism for reading and writing configurations.

CONNECTS TO: Uses nlohmann::json for parsing and manipulating JSON data, and Windows API functions for accessing the registry (RegOpenKeyEx, RegQueryValueEx, etc.).

NOTES: None.

## `src/_Utility/ConfigurationStore.h`

PURPOSE: This file defines an interface and implementations for storing configuration settings in different ways, primarily using the Windows Registry and JSON files.

KEY PARTS:
- IConfigurationStore: Interface defining methods to read and write various data types (bool, float, int, string) with default values.
- RegistryConfigurationStore: Implementation of IConfigurationStore that stores and retrieves configuration settings from the Windows Registry.
- CreateJsonConfigurationStore: Function to create a IConfigurationStore implementation that reads and writes JSON files.
- FallbackConfigurationStore: Implementation of IConfigurationStore that attempts to read from a JSON store first, falling back to a registry store if the value is not found in the JSON store.

CONNECTS TO:
- Windows.h for Windows API functions related to the Registry.
- JSON file handling (not explicitly shown but implied by CreateJsonConfigurationStore).

NOTES: None.

## `src/_Utility/Endpoint.cpp`

**PURPOSE:**  
This file contains implementation of sorting algorithms for `Endpoint` objects in the Allegiance game. It provides both a quicksort (`longSort`) and a bubble sort (`shortSort`) to handle different sizes of arrays efficiently.

**KEY PARTS:**
- **Function: `swap(DataType* a, DataType* b)`**  
  Swaps two `Endpoint` pointers.
- **Function: `comp(DataType const * a, DataType const * b)`**  
  Compares two `Endpoint` objects based on their values and highF fields.
- **Function: `longSort(DataType* lo, DataType* hi)`**  
  Implements the quicksort algorithm to sort an array of `Endpoint` pointers.
- **Function: `shortSort(DataType* lo, DataType* hi)`**  
  Implements a bubble sort for smaller arrays of `Endpoint` pointers.

**CONNECTS TO:**  
Depends on the `Endpoint` class definition and related utility functions.

**NOTES:**  
- The quicksort algorithm uses a stack to manage recursion, allowing it to handle large arrays without hitting the call stack limit.
- The bubble sort is used for smaller arrays where its simplicity makes it faster than quicksort.

## `src/_Utility/FileLoader.cpp`

PURPOSE: The file `FileLoader.cpp` is responsible for loading and managing game resources such as artwork files. It provides functionality to check if a file exists, retrieve the full path of a file, and open a file for reading.

KEY PARTS:
- `ExitWithError`: Handles errors by minimizing the window, showing an error message box, and exiting the application.
- `FileLoader`: A class that implements the `IFileLoader` interface to load files from specified paths. It checks if a file exists, retrieves its path, or opens it for reading.
- `SteamSecureFileLoader`: An extension of `FileLoader` that adds security features by verifying file hashes using Steam's secure content verification system.

CONNECTS TO: 
- `ZString`, `TRef<ZFile>`, `IFileLoader`, `steam_api.h`, and other Allegiance-specific classes and libraries.

NOTES:
- The `SteamSecureFileLoader` class is conditionally compiled with the `STEAMSECURE` macro.
- Error handling in `ExitWithError` minimizes the window, which might not be ideal for all applications but is specific to Allegiance's behavior.

## `src/_Utility/FileLoader.h`

PURPOSE: tiny file (12 lines), shown in full rather than summarized.

    #pragma once
    #include "tref.h"
    #include "base.h"
    #include "vector"
    #include "memory"
    class IFileLoader {
    public:
        virtual bool HasFile(const ZString& path) = 0;
        virtual ZString GetFilePath(const ZString& path) = 0;
        virtual TRef<ZFile> GetFile(const ZString& path) = 0;
    };
    std::shared_ptr<IFileLoader> CreateSecureFileLoader(const std::vector<ZString>& vPaths, const ZString& pathSecure);

## `src/_Utility/HitTest.cpp`

PURPOSE: The file HitTest.cpp contains functions and classes for collision detection and hit testing in Allegiance, including bounding box calculations, shape representations, and algorithms to determine collisions between objects.

KEY PARTS:
- HitTest::SetBB: Sets the bounding box for an object based on its position and velocity.
- HitTest::UpdateBB: Updates the bounding box of an object to reflect its current state.
- HitTest::Collide: Determines if two objects collide within a given time interval.
- BoundingPoint, BoundingSphere, BoundingCone: Classes representing different types of hit test shapes.
- ConvexVertex: Represents a vertex in a convex hull and its adjacency information.
- SingleHull: Represents a single convex hull with vertices and adjacency information.
- MultiHull: Represents a multi-hull object with multiple convex hulls.
- BoundingHull: Extends HitTest to handle bounding hulls for collision detection.
- CachedMultiHull: Stores cached multi-hull data.
- IntervalCollide: Recursively checks for collisions between two convex hulls over an interval.
- Johnson4: Main function for hit testing a point against a tetrahedron.

CONNECTS TO: Other files related to game objects and physics calculations.

NOTES:
- The use of #undef new before the definition of SingleHull::operator new.
- Complex logic in BoundingCone::GetExtreme to determine extreme points on a cone.

PARTS (in file order):

- lines 1-339
  - HitTest::SetBB: Sets the bounding box for an object based on its position and velocity.
  - HitTest::UpdateBB: Updates the bounding box of an object to reflect its current state.
  - HitTest::Collide: Determines if two objects collide within a given time interval.
  - BoundingPoint, BoundingSphere, BoundingCone: Classes representing different types of hit test shapes.
  - ConvexVertex: Represents a vertex in a convex hull and its adjacency information.
  - SingleHull: Represents a single convex hull with vertices and adjacency information.
  - TODOs: "Allow collisions that occur slightly beyond m_tStop to guarantee that all collisions get processed at least once" "The objects are moving apart ... when will their bounding spheres no longer overlap."
  - Unusual code: The use of #undef new before the definition of SingleHull::operator new. The complex logic in BoundingCone::GetExtreme to determine extreme points on a cone.
- lines 340-694
  - HitTest::GetCenter: Returns the center of the hit test shape.
  - MultiHull: Represents a multi-hull object with multiple convex hulls.
  - BoundingHull: Extends HitTest to handle bounding hulls for collision detection.
  - CachedMultiHull: Stores cached multi-hull data.
  - cachedMultiHulls: Global list of cached multi-hulls.
  - HitTest::Load: Loads a multi-hull from a file, caching it if not already loaded.
- lines 695-1051
  - HitTest::Create: Creates a new HitTest object based on a file or default shape.
  - MultiHull: Represents a convex hull with multiple hulls.
  - SingleHull: Represents a single convex hull.
  - ConvexVertex: Represents a vertex of a convex hull.
  - FrameLink: Represents a frame in the game world.
  - FrameDataUTL: Contains data for a frame, including its name and position.
  - HitTest::GetMinExtreme: Calculates the minimum extreme point for a given shape.
  - HitTest::GetMaxExtreme: Calculates the maximum extreme point for a given shape.
- lines 1052-1416
  - IntervalCollide: Recursively checks for collisions between two convex hulls over an interval.
  - DoGilbert: Applies Gilbert's algorithm to find collision points between two convex hulls.
  - Johnson2: Johnson's algorithm for finding the closest point on a line segment (2D).
  - Johnson3: Johnson's algorithm for finding the closest point on a triangle (3D).
  - closest_vertex, closest_edge, closest_face: Helper functions to determine the closest feature of a simplex.
  - c_maxAttempts: Maximum number of attempts to find a separating plane between two convex hulls.
  - maxDeltaT: Threshold for determining if an interval is small enough to consider as a collision.
- lines 1417-1781
  - Johnson4: Main function for hit testing a point against a tetrahedron.
  - Vector: Class representing 3D vectors with x, y, z components.
  - closest_vertex: Function to find the closest vertex to a point.
  - closest_edge: Function to find the closest edge to a point.
  - closest_face: Function to find the closest face to a point.
  - subset1, subset2, subset3, subset4: Arrays defining subsets of vertices for hit testing.
  - dp: Matrix storing dot products of vertex vectors.
  - delta: Matrix storing calculated distances and differences for hit testing.
- lines 1782-1948
  - HitTest: Class for hit testing.
  - Johnson: Function that performs a Johnson algorithm to find the minimum distance between two convex polyhedra.
  - DoGilbert: Function that implements the Gilbert algorithm to determine if two convex polyhedra intersect.
  - epsilon: A constant representing a small value used for floating-point comparison.

## `src/_Utility/KDnode.cpp`

PURPOSE: This file contains the implementation of a KD-tree node used for efficient spatial partitioning and collision detection in the Allegiance game engine.

KEY PARTS:
- `KDnode::allocHitTestsEndpoints(int n)`: Allocates memory for hit test and endpoint arrays.
- `KDnode::reset(bool highF)`: Resets the node by filtering hit tests based on a pivot value and creating child nodes if necessary.
- `findPivot(int nHitTests, Endpoint* const * ppEndpoints, float* pValue, int* pCost)`: Finds the optimal pivot axis and value for partitioning the node's hit tests.
- `KDnode::pivot(void)`: Selects the best pivot axis and value for the node and creates child nodes if necessary.
- `KDnode::test(HitTest* pHitTest1, CollisionQueue* pQueue) const`: Tests a hit test against the node and its children.

CONNECTS TO: `_Utility/KDtree.h`, `CollisionQueue.cpp`

NOTES: None.

## `src/_Utility/KDroot.cpp`

**PURPOSE:**  
This file contains the implementation of a KDroot class, which is part of a spatial partitioning data structure used for efficient hit testing in a game environment. The KDroot manages a collection of HitTest objects and organizes them into balanced binary trees to facilitate quick queries.

**KEY PARTS:**
- **KDroot:** Manages the root node of a KD tree, handling operations like adding, deleting, and updating hit tests.
- **addHitTest:** Adds a new HitTest object to the KDtree. Handles resizing if necessary and updates endpoint lists.
- **deleteHitTest:** Marks a HitTest for deletion without immediately removing it from the tree.
- **flush:** Removes all deleted objects and endpoints from the tree, releasing resources.
- **update:** Rebalances the KD tree if necessary, using either short or long sorting algorithms based on the number of changes.

**CONNECTS TO:**
- Other classes like HitTest, Endpoint, and possibly other spatial partitioning structures within the game engine.

**NOTES:**
- The code includes assertions for debugging purposes to ensure correct state transitions.
- There is a check to prevent adding a node that was marked dead but not yet pruned, which could potentially lead to accessing freed memory.

## `src/_Utility/Messages.cpp`

PURPOSE: Manages network messaging and player connections using DirectPlay for a multiplayer game.

KEY PARTS:
- FedMessaging: Manages messaging between clients and server.
- DPlayMsgHandler: Handles messages received from DirectPlay.
- GenericSend: Sends a message to a recipient, handling guaranteed and non-guaranteed messages differently.
- ConnectToDPAddress: Attempts to connect to a machine using a DirectPlay address.
- JoinSession: Initiates joining a session as a client.
- CFMConnection: Manages a connection between two players.
- FMSessionDesc: Parses the session name to extract game details.

CONNECTS TO: DirectPlay

NOTES:
- None

PARTS (in file order):

- lines 1-362
  - FedMessaging: Manages messaging between clients and server.
  - DPlayMsgHandler: Handles messages received from DirectPlay.
  - EnumHostsCallback: Callback function for enumerating hosts.
  - SetDefaultRecipient: Sets the default recipient for messages.
  - GetDefaultRecipient: Retrieves the default recipient.
  - PFedMsgCreate: Creates a new FedMessage.
  - VerifyMessages: Ensures that output buffer contains valid FEDMESSAGEs.
  - DumpMemory: Dumps memory leaks.
- lines 363-710
  - CB: Represents a count of bytes.
  - FEDMESSAGE: A structure representing a message with fields like cbmsg and fmid.
  - FedMessaging: Manages sending and receiving messages over a network using DirectPlay.
  - GenericSend: Sends a message to a recipient, handling guaranteed and non-guaranteed messages differently.
  - SendToDefault: Sends queued messages to the default recipient.
  - SendMessages: Queues up messages for sending and sends them if possible.
  - CheckOdometer: Tracks time, number of messages, and bytes sent since the last call.
  - OnSysMessage: Handles system messages received from DirectPlay.
- lines 711-1066
  - FedMessaging::MsgHandler: Handles incoming DirectPlay messages and stores them for later processing.
  - FedMessaging::ReceiveMessages: Loops through all inbound messages in a queue and dispatches them appropriately.
  - DPNMSG_CREATE_PLAYER: Represents a message indicating the creation of a player.
  - DPNMSG_DESTROY_PLAYER: Represents a message indicating the destruction of a player.
  - DPNMSG_SEND_COMPLETE: Represents a message indicating the completion of a send operation.
  - DPNMSG_ADD_PLAYER_TO_GROUP: Represents a message indicating that a player has been added to a group.
  - DPNMSG_REMOVE_PLAYER_FROM_GROUP: Represents a message indicating that a player has been removed from a group.
  - DPNMSG_TERMINATE_SESSION: Represents a message indicating the termination of a session.
- lines 1067-1413
  - FedMessaging: Manages DirectPlay connections and messaging.
  - ConnectToDPAddress: Attempts to connect to a machine using a DirectPlay address.
  - Connect: Initiates a connection to a host machine.
  - KillSvr: Attempts to kill the dplaysvr process.
  - InitDPlayClient: Initializes a DirectPlay client.
  - InitDPlayServer: Initializes a DirectPlay server.
  - HostSession: Starts a session and becomes the host (server).
  - TODOs: How do we know when to make sure dplaysvr isn't hosed? AddComponent(DPNA_KEY_PORT): Sets the port for the DirectPlay address.
- lines 1415-1794
  - FedMessaging: Manages session creation and management.
  - JoinSession: Initiates joining a session as a client.
  - JoinSessionInstance: Joins an already enumerated session instance.
  - Shutdown: Cleans up resources when shutting down the session.
  - GetConnectionFromDpid: Retrieves a connection by its DPID.
  - GetGroupFromDpid: Retrieves a group by its DPID.
  - GetIPAddress: Fetches the IP address of a connection.
  - GetListeningPort: Determines the port the server is listening on.
- lines 1795-2172
  - CFMConnection: Manages a connection between two players.
  - CFMGroup: Represents a group of connections within a session.
  - FedMessaging: Main class for managing network messaging and player connections.
  - FMSessionDesc: Parses the session name to extract game details.
  - CreateConnection: Creates a new connection object.
  - SetSessionDetails: Updates the session details with custom information.
  - GetCountConnections: Returns the number of active connections.
  - GetLinkDetails: Retrieves link details for a specific connection.

## `src/_Utility/Utility.cpp`

PURPOSE: This file contains utility functions and classes used throughout the Allegiance game for various tasks such as file operations, string manipulation, version management, and synchronization.

KEY PARTS:
- `UTL::SetServerVersion` and `UTL::GetServerVersion`: Manage server version information.
- `UTL::SetPrivilegedUsers` and `UTL::GetPrivilegedUsers`: Handle privileged user lists.
- `UTL::PrivilegedUser`: Check if a user is privileged based on predefined rules.
- `UTL::SetUrlRoot` and `UTL::SetArtPath`: Set paths for artwork and URLs.
- `UTL::getFile`: Checks if a file exists at a given path.
- `UTL::SaveFile`, `UTL::AppendFile`: Functions to save and append data to files.
- `UTL::SearchAndReplace`: Replaces occurrences of a word in a string, case-insensitively.
- `UTL::hextoi`: Converts a hexadecimal string to an integer.
- `List_utl` and `Link_utl`: Classes for managing linked lists.
- `Lock_utl`: Class for thread-safe locking.

CONNECTS TO: Various game systems and files that require utility functions for file handling, version management, and synchronization.

NOTES: None.

## `src/_Utility/Utility.h`

PURPOSE: Contains utility classes and functions for collision detection, frame data handling, and other game-related operations.

KEY PARTS:
- UTL: Base class for utility classes.
- List_utl: A linked list implementation with basic operations like adding and removing elements.
- Link_utl: Represents an element in a linked list, providing methods to navigate through the list.
- Lock_utl: Provides thread-safe locking mechanisms for lists.
- Slist_utl: A single-threaded safe list class that manages objects of type T.
- Slink_utl: A single-threaded safe link class that holds an object of type T.
- Rotation: A class representing a rotation in 3D space with axis-angle representation.
- FrameDataUTL: A structure containing frame data including name, position, and forward direction.
- MultiHullBase: Represents a multi-hull base with an ellipse equation and original radius.
- HitTest: A class representing a hit test with various properties and methods.

CONNECTS TO: None

NOTES:
- The use of _exposed macro which seems to be a custom preprocessor directive.
- The sink() function in Slink_utl and Slist_utl, which is called after certain operations but its purpose is not immediately clear from this snippet.

PARTS (in file order):

- lines 1-350
  - UTL: Base class for utility classes.
  - List_utl: A linked list implementation with basic operations like adding and removing elements.
  - Link_utl: Represents an element in a linked list, providing methods to navigate through the list.
  - Lock_utl: Provides thread-safe locking mechanisms for lists.
  - Slist_utl: A single-threaded safe list class that manages objects of type T.
  - Slink_utl: A single-threaded safe link class that holds an object of type T.
  - c_cbFileNameDB, c_cbDescriptionDB, etc.: Constants defining maximum lengths for various string types used in the game.
  - TODOs or unusual code: The use of _exposed macro which seems to be a custom preprocessor directive. The sink() function in Slink_utl and Slist_utl, which is called after certain operations but its purpose is not immediately clear from this snippet.
- lines 351-707
  - Slink_utl<T, Sink>: A template class for a linked list node with additional functionality.
  - Mlist_utl<T>: A multi-thread safe templated list class that inherits from List_utl and Lock_utl.
  - Mlink_utl<T>: A multi-thread safe templated linked list element class that inherits from Link_utl.
  - Rotation: A class representing a rotation in 3D space with axis-angle representation.
  - FrameDataUTL: A structure containing frame data including name, position, and forward direction.
  - FrameList: A typedef for Slist_utl<FrameDataUTL>, representing a list of frame data.
  - FrameLink: A typedef for Slink_utl<FrameDataUTL>, representing a linked list element of frame data.
  - Interval: A class representing an interval with low and high endpoints.
- lines 708-1065
  - MultiHullBase: Represents a multi-hull base with an ellipse equation and original radius. GetFrameOffset: Retrieves the frame offset for a given frame name. GetFrameForward: Retrieves the forward direction of a frame. GetFrame: Searches for a frame by name. GetOriginalRadius: Returns the original radius. GetEllipseEquation: Returns the ellipse equation.
  - HitTest: A class representing a hit test with various properties and methods. Load: Loads a hit test from a file. Create: Creates a new hit test instance. AddRef/Release: Reference counting for object management. SetStaticF: Sets whether the hit test is static. SetBB: Updates bounding box based on time and velocity. Move: Moves the hit test by a given time step. AdjustTimes: Adjusts start and stop times. UpdateStaticBB: Updates static bounding box for a given radius. UpdateBB: Updates dynamic bounding box. Collide: Handles collision detection with another hit test. GetShape/GetTrueShape: Retrieves the current and true shapes of the hit test. SetShape: Sets the shape, ensuring it does not exceed the true shape. GetRadius/SetRadius: Gets and sets the radius of the hit test. GetScale: Returns the scale factor (always 1.0 in this implementation). GetDeadF/SetDeadF: Gets and sets whether the hit test is dead. GetDeletedF/SetDeletedF: Gets and sets whether the hit test has been deleted. GetData: Retrieves the associated data object. GetLastTest/SetLastTest: Gets and sets the last tested hit test. GetVelocity/SetVelocity: Gets and sets the velocity, with checks to ensure it is valid. GetID/SetID: Gets and sets the unique identifier of the hit test. GetNoHit/SetUseTrueShapeSelf/SetUseTrueShapeOther/SetNoHit: Manages no-hit conditions and shape usage. SetTimeStart/SetTimeStop: Sets the start and
- lines 1066-1513
  - Utility.h: Contains utility functions and classes for collision detection, frame data handling, and other game-related operations.
  - HitTestShape: Represents a shape used in collision detection.
  - Orientation: Defines the orientation of an object in space.
  - Vector: A class representing 3D vector operations.
  - KDroot: The root node of a kd-tree used for efficient collision detection.
  - CollisionEntry: Holds information about a collision event.
  - CollisionQueue: Manages a queue of collision events.
  - UTL: Utility class with static methods for file handling, string manipulation, and other general purposes.

## `src/_Utility/allegdb.cpp`

PURPOSE: The file `allegdb.cpp` implements an OLE DB layer for Allegiance, managing SQL queries and threads to handle database operations asynchronously.

KEY PARTS:
- `SQLQueueProc`: Handles message queue processing for adding queries.
- `SQLThreadProc`: Manages the execution of SQL queries in separate threads.
- `CSQLCore`: Initializes and manages the SQL core, including creating threads and handling query queues.
- `CSQLQuery`: Represents a SQL query and handles its preparation and execution.
- `CSQLThread`: Manages individual threads that execute SQL queries.
- `AddQuery`: Adds a new query to the appropriate queue.
- `PostQuery`: Posts a message to add a query to the queue thread.

CONNECTS TO: Other parts of Allegiance, particularly related to database operations and asynchronous processing.

NOTES:
- Contains TODOs for cleaning up query lists in the destructor.
- Uses Windows API functions like `MsgWaitForMultipleObjects`, `WaitForMultipleObjects`, and `PostThreadMessage`.
- Handles error logging with `ZError` and `DumpErrorInfo`.

## `src/_Utility/listwrappers.h`

PURPOSE: This file contains a collection of utility classes and wrappers that provide additional functionality to standard list implementations in the Allegiance game engine. These utilities include event sourcing, filtering, sorting, and other enhancements.

KEY PARTS:
- ListEventSource: Wraps an EventSourceImpl to trigger events when changes occur.
- Slist_utlListWrapper: A wrapper for a singly linked list with additional utility functions like counting items and getting item indices.
- IntItemIDWrapper: Converts between ItemID and other types, handling special cases where ID=0 is invalid.
- TListListWrapper: A wrapper for a generic list with utilities to get item counts, indices, and next items.
- TMapListWrapper: Similar to TListListWrapper but for maps instead of lists.
- FilteredList: Filters items from a list based on a given filter function.
- ListDelegate: Delegates list operations to another list instance.
- ConcatinatedList: Combines two lists into one, maintaining their individual events.
- SortedList: Maintains a sorted list and updates automatically when the underlying list changes.
- SingletonList: Represents a single item that can be changed and notifies listeners of changes.

CONNECTS TO: Utility.h, event.h, genericlist.h, tref.h

NOTES: None.

## `src/_Utility/pch.cpp`

PURPOSE: tiny file (1 lines), shown in full rather than summarized.

    #include "pch.h"

## `src/_Utility/pch.h`

PURPOSE: This file is a precompiled header for the Utility module in Allegiance, providing common includes and definitions used across various source files within this module.

KEY PARTS:
- `#include "stdio.h"`: Standard I/O library.
- `#include <windows.h>`: Windows API headers.
- `#include "Utility.h"`: Local header file for the Utility module.
- `#define __MODULE__ "Utility"`: Macro defining the current module name.

CONNECTS TO:
- Other files within the Utility module, particularly those that include this precompiled header.

NOTES:
- None.
