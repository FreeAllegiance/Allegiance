# src/Wopr/AllegianceInteropClassGenerator

[Back to the index](../LLAMA_SUMMARY.md) - 1 files, 676 lines. Notes by a local model, to orient by: not verified facts.

This folder contains tools and classes used by Allegiance to generate C# wrapper classes for interfacing with native C++ code. The primary purpose is to facilitate communication between managed (C#) and unmanaged (C++) components within the game.

- Program.cs: Processes header files to automatically generate C# wrapper classes, enabling seamless interaction between C# and C++ code.

## `src/Wopr/AllegianceInteropClassGenerator/Program.cs`

PURPOSE: The file `Program.cs` is part of the Allegiance game's interop class generator. It processes header files to generate C# wrapper classes for interfacing with native C++ code.

KEY PARTS:
- **Main**: Entry point that checks command-line arguments and calls appropriate methods based on the switch.
- **Run**: Orchestrates the generation process by calling `GenerateMessageWrappers` or `GenerateIgcWrappers`.
- **FixupNullChecks**: Adds null checks to C++ code snippets read from a header file.
- **GenerateIgcWrappers**: Parses an IGC (Interface Generator Class) header file and generates corresponding C# wrapper classes, handling class definitions and method signatures.
- **GenerateMessageWrappers**: Processes message definitions from header files to generate C# classes for managing game messages.
- **CleanClassBody**: Cleans up the body of a class definition by ensuring proper formatting.
- **GetCallParameters** and **GetSignatureParameters**: Extracts parameter information from method signatures, handling type conversions and wrapping types in C#.
- **CleanName**: Removes unnecessary comments or prefixes from variable names.

CONNECTS TO: `messages.h`, `igc.h`, and other related header files for input, and outputs generated C# files.

NOTES: The code includes regex patterns for parsing C++ headers and generating C# classes. It handles type conversions and wraps native types in C# equivalents. There are TODO comments indicating areas that need further development or review.
