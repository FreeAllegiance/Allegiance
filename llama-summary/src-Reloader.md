# src/Reloader

[Back to the index](../LLAMA_SUMMARY.md) - 4 files, 466 lines. Notes by a local model, to orient by: not verified facts.

The `src/Reloader` folder contains source code for a Windows application named Reloader.exe, which is responsible for managing file operations and restarting Allegiance.exe after it exits.

- Reloader.cpp: Contains the main logic of the Reloader application, including waiting for Allegiance.exe to exit, moving files from a temporary directory, and restarting Allegiance.
- resource.h: Defines resources used by the Reloader application, such as strings and icons.
- pch.cpp: A precompiled header source file that includes commonly used headers for faster compilation.
- pch.h: A precompiled header include file that lists headers to be precompiled.

## `src/Reloader/Reloader.cpp`

PURPOSE:
This file contains the source code for a Windows application named Reloader.exe. Its primary function is to wait for Allegiance.exe to exit, move certain files from a temporary directory to their proper locations, and then restart Allegiance.exe with the original command-line parameters.

KEY PARTS:
- `GetArtPath()`: Retrieves the Art Path of client as specified in the registry.
- `DisplayErrorMsg(char *szError)`: Reports an error message to the user.
- `CAutoDownloadSink`: Handles errors during file move operations, allowing retries if necessary.
- `RenameMangledFiles()`: Renames files with mangled filenames as part of a specific build fix.
- `WinMain()`: Main function that orchestrates the process of waiting for Allegiance.exe to exit, moving files, and restarting Allegiance.exe.

CONNECTS TO:
- Allegiance.exe: The application depends on Allegiance.exe to be running and then exits it before proceeding with file operations and restart.

NOTES:
- Contains a commented-out `DebugBreak()` call that was likely used for debugging purposes.
- Includes functionality to handle mangled filenames, which is specific to a particular build fix (PC-Gamer/Beta1).
- Uses Windows API functions such as `RegOpenKeyEx`, `RegQueryValueEx`, `CreateFile`, `MoveFile`, and `ShellExecute` to perform file operations and process management.

## `src/Reloader/pch.cpp`

PURPOSE: tiny file (1 lines), shown in full rather than summarized.

    #include "pch.h"

## `src/Reloader/pch.h`

PURPOSE: tiny file (6 lines), shown in full rather than summarized.

    #define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
    #include <windows.h>
    #include <crtdbg.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <shellapi.h>

## `src/Reloader/resource.h`

PURPOSE: tiny file (13 lines), shown in full rather than summarized.

    //{{NO_DEPENDENCIES}}
    // Microsoft Visual C++ generated include file.
    // Used by Reloader.rc
    // Next default values for new objects
    //
    #ifdef APSTUDIO_INVOKED
    #ifndef APSTUDIO_READONLY_SYMBOLS
    #define _APS_NEXT_RESOURCE_VALUE        101
    #define _APS_NEXT_COMMAND_VALUE         40001
    #define _APS_NEXT_CONTROL_VALUE         1001
    #define _APS_NEXT_SYMED_VALUE           101
    #endif
    #endif
