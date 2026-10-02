# src/mdlc

[Back to the index](../LLAMA_SUMMARY.md) - 3 files, 431 lines. Notes by a local model, to orient by: not verified facts.

The `src/mdlc` folder contains essential components for handling and compiling 3D model files in Allegiance. It includes:

- `mdlc.cpp`: Implements the MDL Compiler application responsible for processing and compiling 3D model files.
- `pch.h`: A precompiled header file that defines common includes and disables certain compiler warnings, enhancing build performance.
- `pch.cpp`: A tiny file containing a single line of code, likely used in conjunction with `pch.h` to facilitate the precompilation process.

## `src/mdlc/mdlc.cpp`

PURPOSE: The file `mdlc.cpp` contains the implementation of the MDL Compiler application, which processes and compiles 3D model files.

KEY PARTS:
- `ModelerSiteImpl`: Implements the `ModelerSite` interface to handle errors by printing them and exiting.
- `MDLCApp`: Main class for the MDL Compiler application, handling various commands like compile, optimize, convert, etc.
- `DoCompile`, `DoOptimize`, `DoConvert`, `DoCompressAnim`, `DoInfo`, `DoGenHeader`: Functions that perform specific operations based on command-line arguments.

CONNECTS TO: 
- `main.h`
- `regkey.h`
- `VideoSettingsDX9.h`
- Various Allegiance engine components like `Modeler`, `EffectApp`, `CD3DDevice9`, etc.

NOTES:
- Contains commented-out code and assertions that indicate potential issues or unfinished features.
- Uses DirectX 9 for rendering, which is specific to the game's engine architecture.

## `src/mdlc/pch.cpp`

PURPOSE: tiny file (1 lines), shown in full rather than summarized.

    #include "pch.h"

## `src/mdlc/pch.h`

PURPOSE: This file is a precompiled header for the Allegiance game engine, defining common includes and disabling certain compiler warnings.

KEY PARTS:
- `__MODULE__`: Defines the module name used by assertions.
- `#pragma warning(disable ...)`: Disables specific compiler warnings to suppress noise in the build output.
- Includes various system headers such as Windows API, zlib, and custom engine modules like `engine.h`, `effect.h`, etc.

CONNECTS TO: Other engine components and libraries, including DirectX for graphics rendering.

NOTES: None.
