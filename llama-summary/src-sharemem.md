# src/sharemem

[Back to the index](../LLAMA_SUMMARY.md) - 3 files, 506 lines. Notes by a local model, to orient by: not verified facts.

This folder contains source code related to shared memory and synchronization mechanisms used in performance monitoring within the Allegiance game.

- pch.cpp: Precompiled header file for faster compilation.
- pch.h: Declaration of symbols included in the precompiled header.
- sharemem.cpp: Implements a class `CPerfShare` that manages shared memory and synchronization for performance counters, enabling efficient data sharing between different parts of the game.

## `src/sharemem/pch.cpp`

PURPOSE: tiny file (1 lines), shown in full rather than summarized.

    #include "pch.h"

## `src/sharemem/pch.h`

PURPOSE: tiny file (4 lines), shown in full rather than summarized.

    #include <windows.h>
    #include <stdio.h>
    #include <tchar.h>
    #include <sharemem.h>

## `src/sharemem/sharemem.cpp`

PURPOSE: The file `sharemem.cpp` contains the implementation of a class `CPerfShare` that manages shared memory and synchronization for performance counters in the Allegiance game.

KEY PARTS:
- `CPerfShare`: Manages shared memory, mutexes, and instances for performance counters.
- `Lock()`: Acquires a lock on the shared memory to ensure thread safety.
- `Unlock()`: Releases the lock on the shared memory.
- `Initialize()`: Initializes the shared memory and mutex.
- `Terminate()`: Cleans up resources by freeing allocated memory and closing handles.
- `AllocateCounters()`: Allocates memory for performance counters in shared memory.
- `FreeCounters()`: Frees memory used by performance counters.

CONNECTS TO: Depends on Windows API functions like `CreateFileMapping`, `MapViewOfFile`, `WaitForSingleObject`, etc., and uses registry access to retrieve counter values.

NOTES: None.
