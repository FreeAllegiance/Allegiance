# src/zlib

[Back to the index](../LLAMA_SUMMARY.md) - 65 files, 16,523 lines. Notes by a local model, to orient by: not verified facts.

The Allegiance source code folder `src/zlib` contains essential utilities and components for various aspects of the game's functionality, including networking, logging, memory management, and event handling.

- AutoCriticalSection.h: Manages Win32 critical section objects for thread synchronization.
- FTPSession_win.cpp: Handles asynchronous FTP download sessions from an FTP server.
- Logger.cpp: Implements logging functionality with timestamped log files and output to both files and debug output.
- StackTracer.cpp: Captures and logs the call stack when exceptions occur using Structured Exception Handling (SEH).
- TlsValue.h: Provides thread-local storage for various data types with synchronization mechanisms.
- VersionInfo_msvc.cpp: Retrieves and parses version information from Windows executable files.
- Win32app.cpp: Contains main application logic, initialization, exception handling, logging, and memory management.
- matrix.cpp: Implements a 4x4 transformation matrix class for geometric transformations in 3D space.
- orientation.cpp: Represents a 3D orientation using a rotation matrix with methods for setting and manipulating orientations.
- token.cpp: Parses text and command-line inputs into tokens such as strings, numbers, symbols, and special characters.
- zstring.cpp: Implements a custom string class with operations like token extraction, encryption, and path manipulation.

Largest files: window.cpp (1230 lines), zstring.cpp (1030 lines), matrix.cpp (910 lines), FTPSession_win.cpp (892 lines), VersionInfo_msvc.cpp (688 lines).

## `src/zlib/AutoCriticalSection.h`

PURPOSE: This file defines a class `ZAutoCriticalSection` that manages Win32 critical section objects for thread synchronization.

KEY PARTS:
- ZAutoCriticalSection: Manages the lifecycle of a Win32 CRITICAL_SECTION object.
  - Constructor initializes the critical section.
  - Destructor cleans up the critical section.
  - Lock acquires ownership of the critical section.
  - Unlock releases ownership of the critical section.

CONNECTS TO: Windows.h for Win32 API functions.

NOTES: None.

## `src/zlib/CMakeLists.txt`

PURPOSE: This file is a CMake configuration script for building the zlib library in Allegiance. It sets up the source files and platform-specific sources, and specifies the C++ standard version.

KEY PARTS:
- SET_MODULE("zlib"): Defines the module name.
- INCLUDE_DIRECTORIES(../Inc): Includes the header directory.
- Zlib_SRC: Lists all source files for the zlib library.
- IF (MSVC) ... ELSE(): Conditionally includes platform-specific source files based on the compiler.
- ADD_LIBRARY(Zlib ${Zlib_SRC} ${PLATFORM_SRC}): Adds the library target with specified sources.
- SET_PROPERTY(TARGET Zlib PROPERTY CXX_STANDARD 14): Sets the C++ standard version to 14.

CONNECTS TO: Other Allegiance modules and platform-specific implementations.

NOTES: None.

## `src/zlib/FTPSession.h`

PURPOSE: This file defines interfaces and a factory for creating FTP session objects that allow easy downloading of files from an FTP server.

KEY PARTS:
- IFTPSessionUpdateSink: Interface for receiving updates about the download progress.
- IInternetSessionBase: Base interface with common methods for managing internet sessions.
- IFTPSession: Interface specific to FTP sessions, providing methods to connect to a site and initiate downloads.
- CreateFTPSession: Factory function to create an instance of IFTPSession.

CONNECTS TO: None

NOTES: None

## `src/zlib/FTPSession_win.cpp`

**PURPOSE:** The file `src/zlib/FTPSession_win.cpp` is part of the Allegiance game's network module and handles FTP download sessions asynchronously. It manages the entire process from connecting to an FTP server, initiating downloads, handling retries, and updating progress.

**KEY PARTS:**
- **Class: CInternetSessionImpl**: Implements `IInternetSession` interface for managing internet sessions.
- **Function: DownloadThread**: Runs in a separate thread to handle file downloads asynchronously.
- **Function: StartNextFile**: Opens the next file from the list for downloading and handles retries if opening fails.
- **Function: FinishCurrentFile**: Closes the current file, flushes data, and optionally writes it to disk.
- **Function: DownloadFileBlock**: Reads a block of data from the FTP server, updates progress, and handles completion.
- **Function: OpenDownloadFile**: Creates a local file for writing downloaded data.
- **Function: FlushDownloadBuffer**: Writes buffer to local file and notifies user intervention if a sink is set.
- **Function: CloseDownloadFile**: Closes the local file handle.

**CONNECTS TO:** `src/zlib/FTPSession.h`, `src/zlib/HTTPSession.h`

**NOTES:** Contains TODOs for error handling, retry logic, platform-specific code, and debugging statements. Uses `TerminateThread` to terminate threads, which is generally discouraged due to potential resource leaks.

## `src/zlib/Logger.cpp`

PURPOSE: This file contains implementations for logging functionality in the Allegiance game, including creating timestamped log files and writing logs to both files and the debug output.

KEY PARTS:
- `CreateTimestampedFileLogger`: Creates a logger that writes to a file with a timestamped name.
- `CreateDirectoryRecursivelyForFilepath`: Recursively creates directories for a given filepath if they do not exist.
- `FileLogger`: Class responsible for logging messages to a file, handling file creation and writing.
- `OutputLogger`: Class that logs messages to the debug output.

CONNECTS TO: `Logger.h`, uses Windows API functions like `CreateFile` and `WriteFile`.

NOTES: None.

## `src/zlib/Logger.h`

PURPOSE: This file defines a logging interface and several implementations for different logging strategies in the Allegiance game.

KEY PARTS:
- ILogger: Interface for logging text messages.
- FileLogger: Logs messages to a file, with an option to append to existing files.
- NullLogger: A logger that does nothing, useful for disabling logging.
- OutputLogger: Logs messages to standard output (console).
- SettableLogger: Allows setting and changing the underlying logger dynamically.
- MultiLogger: Combines multiple loggers, forwarding messages to all of them.

CONNECTS TO: Other parts of the game that require logging functionality.

NOTES: None.

## `src/zlib/StackTracer.cpp`

PURPOSE: This file implements a stack tracer to capture and log the call stack when an exception occurs in the Allegiance game. It uses Structured Exception Handling (SEH) and Windows debugging APIs to gather information about the exception and the call stack at the time of the exception.

KEY PARTS:
- `StackTracer`: A class that manages capturing and storing exception details and the call stack.
- `ExceptionFilter`: An SEH filter function that captures exceptions and passes them to the `HandleException` method.
- `HandleException`: Processes the exception, initializes symbol handling, and traces the call stack using Windows debugging APIs.
- `TraceCallStack`: Walks through the stack frames to gather function names, file names, and line numbers.

CONNECTS TO: Uses Windows debugging APIs (`dbghelp.lib`) for symbol resolution and stack walking.

NOTES:
- The class uses a singleton pattern with `s_StackTracer`.
- It includes a map of exception codes to their descriptions.
- The call stack is limited to a depth of 6 frames.
- There are commented-out sections for additional exception codes and other features.

## `src/zlib/StackTracer.h`

**PURPOSE:** This file provides a mechanism to capture and output the call stack when an exception occurs in a Windows application using Structured Exception Handling (SEH).

**KEY PARTS:**
- **Class: StackTracer**
  - Manages capturing and storing the call stack information during exceptions.
- **Function: ExceptionFilter**
  - The SEH filter function that captures the exception and calls `HandleException`.
- **Function: HandleException**
  - Handles the actual work of tracing the call stack using Windows APIs like `StackWalk64`.
- **Function: TraceCallStack**
  - Recursively traces the call stack from the current context.
- **Function: GetExceptionMsg**
  - Returns a formatted string containing the exception message and call stack.
- **Function: GetExceptionCode**
  - Retrieves the exception code.
- **Function: GetExceptionCallStack**
  - Provides access to the captured call stack data.

**CONNECTS TO:** This file is used in conjunction with SEH handling in other parts of the application to capture exceptions and provide debugging information.

**NOTES:** None.

## `src/zlib/TlsValue.h`

PURPOSE: The file defines a template class `TlsValue` that provides thread-local storage functionality for various data types. It ensures safe access to TLS slots using synchronization mechanisms.

KEY PARTS:
- TlsValue<T>: A template class that manages thread-local storage for any type `T`.
  - Constructor/Destructor: Initializes and cleans up the TLS slot.
  - GetValue(): Retrieves the value stored in the TLS slot.
  - SetValue(const T&): Sets the value in the TLS slot.
  - Operators: Overloaded operators to provide arithmetic, logical, and bitwise operations on the stored value.
- GetSlot(): Allocates a TLS slot if not already allocated and ensures thread-safe allocation using a critical section.

CONNECTS TO: AutoCriticalSection.h (for synchronization).

NOTES: None.

## `src/zlib/VersionInfo.h`

PURPOSE: The file `VersionInfo.h` declares the class `ZVersionInfo`, which is used to retrieve and manipulate version information from executable files.

KEY PARTS:
- Class: `ZVersionInfo`
  - Provides methods to load, unload, and access various version attributes of an executable.
- Methods:
  - Constructors for different ways to initialize a `ZVersionInfo` object.
  - Functions to get file and product versions, flags, target operating systems, file types, and string values.
- Private Member: `d`
  - A unique pointer to the private implementation details.

CONNECTS TO: `zassert.h`, `zstring.h`, Windows headers (`windows.h`, `winver.h`, `tchar.h`).

NOTES: None.

## `src/zlib/VersionInfo_dummy.cpp`

PURPOSE: This file provides a dummy implementation of the `ZVersionInfo` class, which is used to handle version information for modules in the Allegiance game. It includes methods to load and retrieve various version-related strings and flags.

KEY PARTS:
- ZVersionInfoPrivate: A private class containing a dummy member variable.
- ZVersionInfo: The main class with constructors, destructor, and methods to load and retrieve version information.
  - Load: Methods to load version information from different sources (module handle, file pointer).
  - GetStringValue: Retrieves string values for various keys.
  - GetStringValues for specific version-related strings like FileVersion, ProductVersion, etc.
  - IsDebug, IsInfoInferred, etc.: Methods to check various flags about the module's build and target.

CONNECTS TO: None. This is a standalone utility class not dependent on other systems or files.

NOTES: All methods return dummy values (e.g., true for Load, empty strings for GetStringValue), indicating this is a placeholder implementation.

## `src/zlib/VersionInfo_msvc.cpp`

PURPOSE: The file `VersionInfo_msvc.cpp` contains the implementation of a class `ZVersionInfo` that provides functionality to retrieve and parse version information from Windows executable files.

KEY PARTS:
- Class `ZVersionInfo`: Manages loading, parsing, and accessing version information.
- Function `Load(LPCTSTR szModule)`: Loads version info from a specified module.
- Function `GetStringValue(LPCTSTR pszKey, bool* pbExists)`: Retrieves string values from the version info.
- Function `GetFileVersionString()`: Returns the file version as a formatted string.

CONNECTS TO: Windows API functions like `GetFileVersionInfoSize`, `GetFileVersionInfo`, and `VerQueryValue`.

NOTES: None.

## `src/zlib/Win32app.cpp`

PURPOSE: This file contains the main application logic and initialization for a Windows-based Allegiance game client. It handles application startup, exception handling, logging, and memory manipulation.

KEY PARTS:
- Win32App: Main application class that initializes, terminates, and manages the game window.
- GenerateDump: Function to create crash dumps for debugging purposes.
- ZAssertImpl: Assertion function to handle failed assertions.
- DebugOutput: Outputs debug messages to a logger.
- OnException: Handles unhandled exceptions by generating a dump.
- ExceptionHandler: Windows exception handler that calls GenerateDump.

CONNECTS TO:
- Window.h: For window initialization and message loop.
- regkey.h, SlmVer.h: For registry and version information handling.
- dbghelp.h, crtdbg.h: For debugging utilities.
- zstring.h, VersionInfo.h, zmath.h: Custom utility headers.
- steam_api.h: For Steam integration.

NOTES:
- Contains code for generating crash dumps on exceptions.
- Uses custom logging mechanisms (`SettableLogger`, `FileLogger`, `OutputLogger`).
- Includes conditional compilation for Steam API integration.
- Contains commented-out code related to memory protection and exception handling.

## `src/zlib/Win32app.h`

PURPOSE: This file defines the Win32App class and related utilities for managing a Windows application in Allegiance.

KEY PARTS:
- Win32App: Main class for handling Win32-specific application logic.
- ExceptionData: Holds exception records for error handling.
- GetExecutablePath: Retrieves the path of the executable.
- GlobalConfigureLoggers: Configures logging to output and file.
- Exit: Exits the application with a given value.
- Initialize: Initializes the application with command line parameters.
- Terminate: Terminates the application.
- OnException: Handles exceptions by returning an error code.
- DebugOutput: Outputs debug information.
- OnAssert: Handles assertions, returning whether to break on failure.
- IsBuildDX9: Checks if the build is for DirectX 9.
- EnforceFilter: Enforces a memory filter.
- WriteMemory: Writes memory from source to target.
- GenerateDump: Generates a dump file for debugging.
- ExceptionHandler: Global exception handler for unhandled exceptions.

CONNECTS TO: windows.h, tref.h, Allegiance's IObject interface.

NOTES: None.

## `src/zlib/alloc.h`

PURPOSE:
This file provides memory allocation and debugging utilities for the Allegiance game engine. It includes functions to allocate and free memory with debug information and macros to replace standard new/delete operators.

KEY PARTS:
- ZAlloc: Allocates memory and records the file and line number where it was called.
- ZFree: Frees allocated memory.
- operator new: Overloaded new operator that calls ZAlloc for debugging purposes.
- operator delete: Overloaded delete operator that calls ZFree.
- InitAllocs: Initializes memory leak detection if in debug mode.
- TZeroFill: A template class that initializes its instances to zero.

CONNECTS TO:
crtdbg.h (for Windows-specific debugging functions)

NOTES:
- Debug allocation is enabled only in debug builds.
- Memory leaks are detected and reported when the program exits if _DUMP_LEAKS_ is defined.
- The ZLIB_SKIP_NEW_REDEF macro is used to avoid conflicts with redefined 'new' operator in VS.Net 2003.

## `src/zlib/base.h`

PURPOSE: This file contains basic utility classes and functions used throughout the Allegiance game, including file I/O operations, synchronization primitives, and string manipulation.

KEY PARTS:
- ZFile: Manages file operations such as reading, writing, seeking, and getting file length.
- CriticalSection: Provides a simple interface for managing critical sections using Windows API.
- SingleEnterCriticalSection: Ensures that a critical section is entered only once per instance.
- CriticalSectionLock: Automatically locks a critical section upon creation and unlocks it when destroyed.

CONNECTS TO: tref.h, zstring.h

NOTES: None.

## `src/zlib/base_win.cpp`

PURPOSE: This file contains implementation details for a custom file handling class `ZFile` in Allegiance, which provides functionalities to open, read, write, and manage files on Windows.

KEY PARTS:
- ZFilePrivate: A private class that holds the actual file handle, path string, and other related data.
- ZFile: The main class providing methods for file operations such as opening, reading, writing, and getting file size.
- GetSha1Hash: Computes the SHA-1 hash of a file using Windows Cryptography API.
- GetMostRecentFileModificationTime: Retrieves the most recent modification time of files in a specified directory.

CONNECTS TO: Uses Windows APIs like `CreateFile`, `ReadFile`, `WriteFile`, and others for file operations. Depends on `PathString` from `base.h`.

NOTES:
- Contains code to handle Unicode paths, which allows for longer path lengths.
- Implements SHA-1 hashing using Windows Cryptography API.
- Includes functionality to find the most recent modified file in a directory.
- Uses `ZAssert` and `ZDebugOutput` for assertions and debugging output.

## `src/zlib/cast.h`

PURPOSE: This file contains macros and templates for casting pointers between different types in the Allegiance game engine. It provides both static and dynamic casting mechanisms.

KEY PARTS:
- CastTo: A template function that performs casting between pointer types. In debug mode, it uses `dynamic_cast` to ensure type safety.
- DefineCastMember: A macro used to define a static cast method for a specific type.

CONNECTS TO: tref.h

NOTES: The dynamic casting code is commented out and not used in the current implementation.

## `src/zlib/color.cpp`

PURPOSE: This file contains implementations for color manipulation in the Allegiance game engine. It includes functions to set and get colors in HSBA format, convert colors to a string representation, and define common color constants.

KEY PARTS:
- HueComponent: Calculates the hue component based on the given hue value.
- SetHSBA: Sets the RGB values of a Color object using hue, saturation, brightness, and alpha.
- GetHSB: Converts an RGB color to its hue, saturation, and brightness components.
- s_colorWhite, s_colorRed, etc.: Predefined constants for common colors.

CONNECTS TO: zmath.h (for mathematical functions), color.h (for the Color class definition).

NOTES: None.

## `src/zlib/color.h`

PURPOSE: This file defines a `Color` class for representing colors in Allegiance, including RGB and HSBA color spaces, as well as utility functions to convert between different color representations.

KEY PARTS:
- `Color`: A class representing colors with RGBA components. Provides methods to set and get the color values, adjust alpha, and convert to COLORREF.
- `HSBColor`: A subclass of `Color` that represents colors in the HSBA (Hue, Saturation, Brightness, Alpha) color space.
- `MakeCOLORREF()`: Converts a `Color` object to a Windows COLORREF value.
- `AdjustAlpha()`: Adjusts the alpha component of the color by a given scale factor.

CONNECTS TO: `zstring.h`, `windows.h`

NOTES:
- The file includes macros for RGB and COLORREF manipulation, which are typically defined in `windows.h`. There is a TODO to remove this line after all `#include <windows.h>` statements are removed.
- A color code system using escape sequences (`0x81` and `0x82`) is defined for use with text rendering.

## `src/zlib/doit.bat`

PURPOSE: This batch file is used to compile and link various source files into a single library for the Allegiance game project.

KEY PARTS:
- Name: ZLib.lib
  - What it does: A static library containing compiled object files from multiple source files related to game functionality.
  
- Name: zlib.obj, ztime.obj, token.obj, zstring.obj, win32app.obj, tlist.obj, matrix.obj, matrix2.obj, orientation.obj, quaternion.obj, color.obj, event.obj, input.obj, vector.obj, VersionInfo.obj, FTPSession.obj
  - What it does: Individual object files that represent compiled source code from various modules within the Allegiance game.

CONNECTS TO:
- Other systems or files it depends on or serves: The Allegiance game project's build system and other related libraries.

NOTES: None.

## `src/zlib/event.cpp`

PURPOSE: This file contains implementations and interfaces related to event handling in the Allegiance game. It defines classes for managing event sources and sinks, including mechanisms for triggering events and managing subscriptions.

KEY PARTS:
- `NeverEvent`: A dummy event source that does nothing.
- `EventSourceImpl`: Manages a list of generic event sinks and triggers events to them.
- `IntegerEventSourceImpl`: Extends `EventSourceImpl` to handle integer-specific events with additional lists for integer sinks.
- `EventSinkDelegate`: Wraps an existing event sink to provide additional functionality or encapsulation.
- `IntegerEventSinkDelegate`: Similar to `EventSinkDelegate`, but for integer-specific event sinks.

CONNECTS TO: Other files that implement specific event sources and sinks, such as game state changes, player actions, etc.

NOTES: None.

## `src/zlib/event.h`

PURPOSE: This file defines a framework for event handling in the Allegiance game. It includes interfaces and implementations for managing event sources, sinks, and targets.

KEY PARTS:
- IEventSource: Interface for objects that can emit events.
- IEventSink: Interface for objects that can receive events.
- TEvent: Template class for creating typed events with sinks and sources.
- EventTargetContainer: Class to manage multiple event targets for a derived class.

CONNECTS TO: tlist.h, tref.h, zstring.h

NOTES: None.

## `src/zlib/fixdelete.h`

PURPOSE: This file provides a custom implementation of the `delete` operator to ensure compatibility with the Microsoft Visual C++ runtime's memory management under debug builds when using the Standard Template Library (STL).

KEY PARTS:
- Name: `operator delete`
  - What it does: Overrides the global `delete` operator to correctly handle memory deallocation in debug mode, ensuring that blocks allocated with specific flags are properly freed.

- Name: `_CrtMemBlockHeader`
  - What it does: A structure used by the CRT to manage memory blocks, including information about the block's allocation and deallocation.

CONNECTS TO:
- `<new>`: The STL header file that includes the default implementation of `delete`.

NOTES:
- None.

## `src/zlib/genericlist.h`

PURPOSE: This file defines a generic list interface and an empty list implementation in the Allegiance game engine.

KEY PARTS:
- List: An abstract base class representing a generic list with methods to get the count of items, retrieve an item by index, find the index of an item, get the next item after a given one, and access the event source for changes.
- EmptyList: A concrete implementation of the List interface that represents an empty list.

CONNECTS TO:
- event.h: For IEventSource
- tref.h: For ItemID

NOTES: None.

## `src/zlib/input.cpp`

PURPOSE: This file contains implementations of input delegates for keyboard and mouse inputs in the Allegiance game. These delegates allow for additional functionality to be added to existing input handlers without modifying their core logic.

KEY PARTS:
- KeyboardInputDelegate: A class that implements IKeyboardInput by delegating calls to another IKeyboardInput instance.
- MouseInputDelegate: A class that implements IMouseInput by delegating calls to another IMouseInput instance.
- CreateDelegate methods in IKeyboardInput and IMouseInput: Factory methods for creating instances of KeyboardInputDelegate and MouseInputDelegate, respectively.

CONNECTS TO: input.h (header file defining the interfaces IKeyboardInput and IMouseInput).

NOTES: None.

## `src/zlib/input.h`

PURPOSE: This file defines interfaces and classes related to input handling in the Allegiance game, including keyboard and mouse inputs.

KEY PARTS:
- IKeyboardInput: Interface for keyboard input handling.
- IMouseInput: Interface for mouse input handling.
- MouseResult: Bitmask representing different mouse results like hit, capture, and release.
- KeyState: Structure holding details about a key press or release event.

CONNECTS TO: mask.h, point.h, tref.h

NOTES: None.

## `src/zlib/main.h`

PURPOSE: This file contains the main entry point for a Windows application, including initialization and cleanup code.

KEY PARTS:
- `WinMain`: The primary function that initializes COM, calls `Win32Main`, and handles exceptions.
- `MiniDumpFunction`: A function to generate a mini-dump when an exception occurs, potentially with Steam user information.
- `main`: Converts command-line arguments into a single string and calls `WinMain`.

CONNECTS TO: 
- Other Windows API functions for initialization (`OleInitialize`), cleanup (`OleUninitialize`), and exception handling.

NOTES:
- Uses `_set_se_translator` to handle structured exceptions with a custom function.
- Initializes COM in `WinMain` and uninitializes it before returning.

## `src/zlib/mask.h`

PURPOSE: This file defines template classes `TBitMask` and `TLargeBitMask` for managing bit flags in a type-safe manner. It allows setting, clearing, testing, and manipulating flag bits.

KEY PARTS:
- TBitMask<Type, WordType>: A template class that provides methods to manipulate a single word of bit flags.
- TLargeBitMask<nBits>: A template class that extends `TBitMask` to handle more than 32 bits by using an array of bytes.

CONNECTS TO: Used in various parts of the game for managing different sets of flags, such as surface properties or player states.

NOTES: None.

## `src/zlib/matrix.cpp`

**PURPOSE:** The `matrix.cpp` file in Allegiance contains the implementation of a 4x4 transformation matrix class used for various geometric transformations such as translation, rotation, scaling, and perspective.

**KEY PARTS:**
- **Name:** Matrix - Represents a 4x4 transformation matrix.
- **Name:** InitializeTransform - Initializes the identity matrix when the program starts.
- **Name:** GetIdentity - Returns the pre-initialized identity matrix.
- **Name:** Matrix::Matrix(const Matrix2& mat2) - Converts a 2x3 matrix to a 4x4 transformation matrix.
- **Name:** Matrix::GetScale() const - Calculates the scale factor from the first three diagonal elements of the matrix.
- **Name:** Matrix::GetTranslate() const - Extracts the translation vector from the last column of the matrix.
- **Name:** Determinant(float m[4][4], int size) - Computes the determinant of a 4x4 matrix using recursive expansion by minors.
- **Name:** Matrix::GetType() const - Determines and caches the type of transformation represented by the matrix.
- **Name:** Matrix::SetIdentity() - Sets the matrix to the identity matrix and resets the transformation type.
- **Name:** Matrix::Set(int row, int col, float value) - Sets a specific element in the matrix and invalidates any cached transformation type.

**CONNECTS TO:** Other geometry-related classes and functions within Allegiance.

**NOTES:** The commented-out assembly code in the `Transform` method suggests an alternative implementation using SIMD instructions for potentially faster vector transformations.

## `src/zlib/matrix.h`

PURPOSE: This file defines classes and functions related to matrix operations for transformations in a 3D space, including translation, scaling, rotation, and perspective.

KEY PARTS:
- Matrix2: Represents a 2x3 transformation matrix with methods for setting, getting, and applying transformations.
- Matrix: Represents a 4x4 transformation matrix with methods for setting, getting, and applying transformations in 3D space.
- TransformType: Enumerates different types of transformations that can be applied to matrices.

CONNECTS TO:
- point.h: For Point class used in translation and rotation operations.
- tvector.h: For TVector template class used in batch transformation methods.
- vertex.h: For Vertex class, though not directly used here, it's part of the same system.

NOTES: None.

## `src/zlib/matrix2.cpp`

PURPOSE: This file contains the implementation of a 2D transformation matrix class, providing methods for setting and manipulating 2D transformations such as scaling, translation, rotation, and inversion.

KEY PARTS:
- Matrix2: A class representing a 3x3 matrix used for 2D transformations.
- InitializeTranform2: A static class to initialize the identity matrix when the program starts.
- GetIdentity: Returns the identity matrix.
- Constructor: Initializes a new instance of the Matrix2 class with specified values.
- Accessors: Methods to get scale, translation, and type of transformation.
- SetIdentity: Sets the matrix to the identity matrix.
- Set, Scale, Translate, Rotate: Methods to set individual elements or perform specific transformations.
- Multiply, PreMultiply: Methods for multiplying matrices.
- Inverse, Transpose: Methods for calculating the inverse and transpose of a matrix.
- Transform: Methods for transforming points using the matrix.

CONNECTS TO: Depends on the Point class and TVector<Point> container from the Allegiance engine's utility library.

NOTES: None.

## `src/zlib/olddxdefns.h`

PURPOSE: This file contains legacy definitions and macros for DirectX-related types and constants used in the Allegiance game. It provides compatibility with older DirectX versions and defines structures like D3DVECTOR, COLORVALUE, and various vertex types.

KEY PARTS:
- `COLORVALUE`: A structure representing a color using floating-point values.
- `D3DVECTOR`: A structure representing a 3D vector.
- `D3DHVERTEX`, `D3DTLVERTEX`, `D3DLVERTEX`, `D3DVERTEX`: Structures representing different types of vertices used in rendering, including their coordinates, colors, and texture coordinates.
- Macros like `RGB_GETRED`, `RGBA_SETALPHA`, `RGB_MAKE` for manipulating color values.

CONNECTS TO: DirectX headers and Allegiance game code that uses these definitions.

NOTES: None.

## `src/zlib/orientation.cpp`

PURPOSE: The file `orientation.cpp` contains the implementation of the `Orientation` class, which represents a 3D orientation in space using a rotation matrix. It provides methods for setting and manipulating orientations, as well as performing operations like multiplication and inversion.

KEY PARTS:
- **Class: Orientation**: Represents a 3x3 rotation matrix used to describe an orientation in 3D space.
- **Constructor**: Initializes the orientation matrix, either from another `Orientation`, a 3x3 matrix, or from vectors representing the forward and up axes.
- **Operators**: Overloads operators for assignment (`=`), multiplication (`*`), and equality comparison (`==`) to facilitate operations on orientations.
- **Methods**: Provides functions like `Set`, `TurnTo`, `PostRotate`, `Yaw`, `Pitch`, and `Roll` to manipulate the orientation matrix, such as setting it from a forward vector, turning towards a target, and applying rotations around specific axes.

CONNECTS TO: The file depends on other classes and functions defined in files like `matrix.h` and `zmath.h`.

NOTES: None.

## `src/zlib/orientation.h`

PURPOSE: The file defines a class `Orientation` that represents the orientation of an object in 3D space using a 3x3 rotation matrix. It provides methods for creating, manipulating, and querying orientations.

KEY PARTS:
- Class `Orientation`: Represents a 3x3 rotation matrix.
- Constructor overloads to initialize an orientation from various parameters (identity, vectors, axis-angle).
- Static method `GetIdentity()`: Returns the identity orientation.
- Methods for transforming vectors (`TimesInverse`), multiplying orientations (`operator*`), and rotating (`Roll`, `Pitch`, `Yaw`).
- Methods for setting and getting axes (`SetForward`, `SetUp`, `SetRight`, `GetForward`, etc.).
- Utility methods for cosine calculations and renormalization.

CONNECTS TO: The file depends on the `vector.h` header for vector operations.

NOTES: None.

## `src/zlib/point.h`

PURPOSE: This file defines a template class `TPoint` for 2D points and specific instantiations of this template for Windows coordinates (`WinPoint`) and floating-point coordinates (`Point`). It provides various operations like addition, subtraction, scaling, and conversion to strings.

KEY PARTS:
- TPoint<PointType>: A template class representing a 2D point with basic arithmetic operations.
- WinPointCore: A concrete implementation of the `TPoint` template for Windows coordinates.
- PointCore: A concrete implementation of the `TPoint` template for floating-point coordinates.
- GetString functions: Convert points to string representations.

CONNECTS TO:
- zstring.h: For string manipulation functions used in the GetString methods.

NOTES:
- None.

## `src/zlib/quaternion.cpp`

PURPOSE: This file contains implementation of quaternion operations which are essential for representing and manipulating rotations in 3D space.

KEY PARTS:
- Quaternion: Class representing a quaternion with methods to get rotation, normalize, convert to orientation, and perform spherical linear interpolation.
- GetRotation: Function to calculate the axis-angle representation of a quaternion.
- operator== and operator!=: Overloaded operators for comparing two quaternions.
- Quaternion(const Orientation& o): Constructor converting an orientation matrix to a quaternion.
- operator Orientation(void) const: Conversion operator from quaternion to orientation matrix.
- Slerp: Function performing spherical linear interpolation between two quaternions.
- Normalize: Method to normalize the quaternion.

CONNECTS TO: Depends on "vector.h" for vector operations and "orientation.h" for orientation matrices.

NOTES: None.

## `src/zlib/quaternion.h`

PURPOSE:
The file defines a Quaternion class used for representing and manipulating 3D rotations in the Allegiance game engine. Quaternions provide an efficient way to handle rotations without issues like gimbal lock.

KEY PARTS:
- Quaternion: Represents a rotation using four components (a, b, c, d).
- Vector: A helper class for handling 3D vector operations.
- Orientation: Another class that may be used in conjunction with Quaternions for orientation data.
- Slerp: A friend function for spherical linear interpolation between two quaternions.
- Normalize: A member function to normalize the quaternion.

CONNECTS TO:
Depends on "vector.h" and "orientation.h".

NOTES:
None.

## `src/zlib/rect.h`

PURPOSE: This file defines template-based rectangle classes for different numeric types and coordinate systems, providing functionalities such as intersection, accumulation, offsetting, and size manipulation.

KEY PARTS:
- TRect<RectType>: A generic template class representing a rectangle with various constructors, methods, and operators.
- WinRectCore: A concrete implementation of the rectangle core using Windows RECT structure.
- WinRect: A typedef for TRect using WinRectCore.
- RectCore: Another concrete implementation of the rectangle core using floating-point coordinates.
- Rect: A typedef for TRect using RectCore.

CONNECTS TO: point.h (for PointType and related functions)

NOTES: None.

## `src/zlib/tarray.h`

PURPOSE: Defines a template class `TArray` for creating fixed-size arrays of any type.

KEY PARTS:
- Name: TArray
  - What it does: Implements a fixed-size array with bounds checking and copy functionality.
  
- Name: TArray::TArray()
  - What it does: Default constructor initializing the array elements to default values.
  
- Name: TArray::TArray(TValue pvalue[count])
  - What it does: Constructor that initializes the array with provided values.
  
- Name: TArray::TArray(const TArray& tvec)
  - What it does: Copy constructor to create a copy of another `TArray`.
  
- Name: operator=(const TArray& tvec)
  - What it does: Assignment operator to assign one `TArray` to another.
  
- Name: operator[](int index)
  - What it does: Provides indexed access to array elements with bounds checking.
  
- Name: GetCount() const
  - What it does: Returns the number of elements in the array.

CONNECTS TO: None

NOTES: none

## `src/zlib/tcompare.h`

PURPOSE: This file defines default comparison and equality operators that can be used in various parts of the Allegiance game.

KEY PARTS:
- DefaultEquals: Provides a template function to check if two values are equal using the `==` operator.
- DefaultCompare: Provides a template function to compare two values, returning true if the first is greater than the second using the `>` operator.
- DefaultNoEquals: Provides a template function that throws an error and returns false when called, indicating it should not be used for equality checks.
- DefaultNoCompare: Provides a template function that throws an error and returns false when called, indicating it should not be used for comparison.

CONNECTS TO: Used by various classes and functions in the Allegiance game to perform default comparisons and equality checks.

NOTES: None.

## `src/zlib/tlist.cpp`

PURPOSE: This file contains the implementation of a templated list class (`TListImpl`) and its iterator (`IteratorImpl`). The list supports basic operations like insertion, deletion, and traversal.

KEY PARTS:
- `ListNodeImpl`: Represents a node in the list.
- `IteratorImpl`: Provides methods to iterate over the list nodes.
- `PushFrontImpl`, `PushEndImpl`: Adds new nodes at the beginning or end of the list.
- `InsertBeforeImpl`, `InsertAfterImpl`: Inserts a new node before or after an existing node.
- `PopFrontImpl`, `PopEndImpl`: Removes the first or last node from the list.
- `RemoveNode`: Removes a specific node from the list.
- `Get`: Retrieves a node at a specified index.

CONNECTS TO: None, operates independently.

NOTES: Contains unimplemented functions (`operator==`) and assertions for debugging purposes.

## `src/zlib/tlist.h`

PURPOSE: This file defines a generic list template class `TList` and its implementation `TListImpl`, along with related classes for iterators and pointer lists.

KEY PARTS:
- TListImpl: Base class implementing the core functionality of a doubly linked list.
  - ListNodeImpl: Represents a node in the list.
  - IteratorImpl: Provides iteration over the list.
- TList: Template class that extends `TListImpl` to store values of type TValue.
  - ListNode: Extends `ListNodeImpl` to include the value.
  - Iterator: Extends `IteratorImpl` with additional methods for accessing and modifying the list.
- TPointerListObject: Specialized list class designed to manage pointers to objects.

CONNECTS TO: tcompare.h, tref.h, zmath.h

NOTES:
- The file includes a warning disable directive for potential memory leaks due to exceptions during initialization.
- There are TODO comments indicating that some functions need fixing or updating.

## `src/zlib/tmap.h`

PURPOSE: The file defines a template class `TMap` that implements a sorted map data structure using a linked list. It allows for efficient insertion, deletion, and lookup operations based on keys.

KEY PARTS:
- `TMap<TKey, TValue, SinkFunctionType>`: A template class representing the map.
- `MapNode`: A nested class representing each node in the linked list, containing a key, value, and pointer to the next node.
- `Set(const TKey& key, const TValue& value)`: Inserts or updates a key-value pair in the map.
- `Find(const TKey& key, TValue& value) const`: Retrieves the value associated with a given key if it exists.
- `Remove(const TKey& key)`: Removes a key-value pair from the map based on the key.
- `Count() const`: Returns the number of key-value pairs in the map.
- `SetEmpty()`: Clears all key-value pairs from the map.

CONNECTS TO: 
- `zmath.h` and `tref.h`, which are included for mathematical operations and reference counting, respectively.

NOTES:
- None.

## `src/zlib/token.cpp`

PURPOSE: This file contains the implementation of token parsing for text and command-line inputs in Allegiance. It handles reading and interpreting tokens such as strings, numbers, symbols, and special characters.

KEY PARTS:
- TextTokenImpl: Class responsible for parsing text tokens.
  - SetError: Sets an error message during parsing.
  - SetErrorUnexpected: Handles unexpected token types.
  - Error: Checks if there is a parsing error.
  - SkipWhite: Skips whitespace and comments in the input.
  - ReadNumber: Reads numeric values from the input.
  - ReadString: Reads string literals from the input.
  - ReadSymbol: Reads symbol identifiers from the input.
  - Next: Advances to the next token in the input.

- CommandLineToken: Class for parsing command-line arguments.
  - MoreTokens: Checks if there are more tokens to process.
  - IsMinus: Parses a minus sign followed by a string.
  - SkipWhite: Skips whitespace characters.
  - IsString: Reads a string from the command line.
  - ReadExponent: Reads an exponent for numeric values.
  - ReadNumber: Reads numeric values from the command line.
  - IsNumber: Checks if the next token is a number and reads it.
  - Skip: Skips over tokens based on their type.

CONNECTS TO: Depends on classes like ZString, ZError, and possibly other parsing utilities within Allegiance.

NOTES: None.

## `src/zlib/token.h`

PURPOSE: This file contains definitions and implementations related to tokenization of text input, specifically for parsing commands and expressions in the Allegiance game.

KEY PARTS:
- TextTokenImpl: Manages the process of tokenizing text into various types such as strings, numbers, and symbols. It handles reading and interpreting escape sequences and provides methods to check and retrieve tokens.
- CommandLineToken: Specialized version of TextTokenImpl for parsing command lines, focusing on handling minus signs and skipping white spaces.

CONNECTS TO: tmap.h, zstring.h (for data structures and string manipulation)

NOTES: None.

## `src/zlib/tref.h`

PURPOSE: This file defines a reference counting mechanism for objects in the Allegiance game engine. It provides classes and templates to manage object lifetimes automatically by tracking references.

KEY PARTS:
- `IObjectSingle`: Base class for reference counted objects, implementing AddRef() and Release().
- `IObject`: Virtual base class that inherits from `IObjectSingle`.
- `TRef`: Template class that manages a pointer to an object of type `Type`, ensuring proper reference counting through constructors, assignment operators, and destructor.

CONNECTS TO: Depends on "StackTracer.h" for exception handling in Release() method.

NOTES:
- Defines `_WIN32_DCOM` and uses `DWORD_OR_UINT32T` based on the platform.
- Contains TODO comments suggesting moving some defines to a project configuration file.
- Includes debug-only code that is conditionally compiled out for non-GCC compilers.

## `src/zlib/tstack.h`

PURPOSE: This file defines a template class `TStack` for implementing a stack data structure and a derived class `TStackObject` that integrates the stack with an object interface.

KEY PARTS:
- TStack<ValueType>: A generic stack implementation using templates.
  - Push(const ValueType& value): Adds an element to the top of the stack.
  - Pop(): Removes the top element from the stack and returns it.
  - GetCount() const: Returns the number of elements in the stack.
  - operator[](int index) const: Provides access to elements by index, with bounds checking.

- TStackObject<ValueType>: A derived class that integrates `TStack` with an object interface, inheriting from `IObject`.

CONNECTS TO:
- zassert.h: For assertions and debugging checks.

NOTES:
- None.

## `src/zlib/tvector.h`

PURPOSE: This file defines a generic template class `TVector` that implements a dynamic array with various operations for managing and manipulating the elements.

KEY PARTS:
- TVector: A template class representing a dynamic array of elements.
- SetCount: Adjusts the size of the vector, allocating new memory if necessary.
- Reserve: Reserves space in the vector without changing its count.
- Get: Retrieves an element by index, ensuring it is writable.
- Insert: Inserts an element at a specified position.
- PushFront and PushEnd: Adds elements to the front or end of the vector.
- Remove: Removes an element by index or value.
- Find: Searches for an element in the vector.
- Sort: Sorts the elements in the vector using a quicksort algorithm.

CONNECTS TO: tcompare.h, zassert.h

NOTES:
- The commented-out Iterator class suggests that iteration functionality was previously included but is currently disabled.
- There are TODO comments indicating potential improvements or fixes related to memory management and performance.

## `src/zlib/vector.cpp`

PURPOSE: This file contains implementation details for vector operations used in the Allegiance game engine.

KEY PARTS:
- Vector class: Represents a 3D vector with x, y, and z components.
- GetOrthogonalVector(): Returns an orthogonal vector to the current vector.
- RandomDirection(): Generates a random direction vector on the unit sphere.
- RandomPosition(float radius): Generates a random position within a sphere of a given radius.

CONNECTS TO: vector.h (header file for Vector class)

NOTES: None.

## `src/zlib/vector.h`

PURPOSE: The file defines a `Vector` class representing a three-dimensional vector with basic operations and properties.

KEY PARTS:
- Vector: Represents a 3D vector with x, y, z components.
- GetZero(): Returns the zero vector.
- LengthSquared() and Length(): Calculate the squared and actual length of the vector.
- Normalize() and SetNormalize(): Normalize the vector to unit length.
- CrossProduct(): Computes the cross product of two vectors.

CONNECTS TO: Uses `zassert.h` for assertions, `zstring.h` for string operations.

NOTES: None.

## `src/zlib/vertex.h`

PURPOSE: This file defines a class `HVector` for representing 3D vertices in space with an additional fourth component.

KEY PARTS:
- Name: HVector
  - What it does: Represents a 4-dimensional vector (x, y, z, w) commonly used in 3D graphics and game development.
  
- Name: X(), Y(), Z(), W()
  - What it does: Getter methods to retrieve the individual components of the vector.

- Name: Set(), SetX(), SetY(), SetZ(), SetW()
  - What it does: Setter methods to modify the individual components of the vector.

- Name: operator+=, operator-=, operator+, operator-, operator*, operator*
  - What it does: Overloaded operators for arithmetic operations on vectors and scalar multiplication.

- Name: operator==, operator!=
  - What it does: Overloaded operators for comparing two vectors for equality or inequality.

CONNECTS TO:
- vector.h: This file depends on the `Vector` class defined in `vector.h`.

NOTES:
- None.

## `src/zlib/window.cpp`

PURPOSE: Manages window properties and behavior, including creating, resizing, handling messages, and managing child windows.

KEY PARTS:
- Window: Manages a window's properties and behavior.
- FillSolidRect: Draws a solid rectangle on a device context.
- Create: Creates a window with specified parameters.
- SetSize: Adjusts the window size.
- AddChild: Adds a child window to this window.
- WndProc: Handles all window messages.
- MessageLoop: Main message loop function handling window messages and timers.

CONNECTS TO: Allegiance game engine, Win32 API

NOTES: None

PARTS (in file order):

- lines 1-354
  - Window: Manages a window's properties and behavior.
  - FillSolidRect: Draws a solid rectangle on a device context.
  - Construct: Initializes default values for a Window object.
  - Window::Window(): Constructor for creating a new Window object.
  - Window::Create(): Creates a window with specified parameters.
  - ~Window(): Destructor that cleans up the window.
  - CalcStyle: Recalculates and applies style settings to the window.
  - SetTopMost, SetSizeable, SetHasMinimize, SetHasMaximize, SetHasSysMenu: Methods to modify window properties and recalculate styles.
- lines 356-721
  - Window::SetSize: Adjusts the window size.
  - Window::SetClientSize: Sets the client area size of the window.
  - Window::SetMinimumClientSize: Defines the minimum client area size for the window.
  - AddChild: Adds a child window to this window.
  - RemoveChild: Removes a child window from this window.
  - RemoveAllChildren: Clears all child windows from this window.
  - AddKeyboardInputFilter: Adds a keyboard input filter to handle key events.
  - RemoveKeyboardInputFilter: Removes a keyboard input filter.
- lines 723-1083
  - Window::WndProc: Handles all window messages.
  - Window::OnClose: Posts a quit message to close the application.
  - Window::SetCursor: Updates the cursor and sends a message to the window under the cursor.
  - Window::DoIdle: Placeholder for idle processing.
  - Window::EnableIdleFunction: Enables or disables idle function calls.
  - CallIdleFunctions: Calls all registered idle functions.
  - Window::Win32WndProc: The main Win32 window procedure that maps HWNDs to Window objects.
  - Window::StaticInitialize: Registers the window class and initializes static members.
- lines 1084-1230
  - g_bContinuousIdle: Global boolean indicating whether the message loop should continuously check for idle messages.
  - Window::s_countProfile: Static integer to count profiling operations.
  - Window::s_bProfileStarted: Static boolean indicating whether profiling is currently started.
  - Window::ToggleProfiling(int count): Function to toggle or set the profiling count.
  - Window::IsProfiling(): Function to check if profiling is started.
  - Window::MessageLoop(): Main message loop function handling window messages and timers.
  - MSG msg: Structure used to store Windows messages.
  - CallTimers(): Function to call all registered timer functions.

## `src/zlib/window.h`

PURPOSE: This file defines a `Window` class that encapsulates the functionality of a window in a Windows application. It handles creation, message processing, and various properties of the window.

KEY PARTS:
- Window: The main class representing a window with methods for setting properties, handling messages, and managing child windows.
- Win32WndProc: A static callback function that serves as the window procedure for all instances of `Window`.
- FillSolidRect: A utility function to fill a rectangle on a device context with a solid color.

CONNECTS TO:
- event.h
- input.h
- rect.h
- tmap.h
- winstyles.h
- commctrl.h

NOTES:
- The class uses various Windows API functions for window management and message handling.
- There is conditional compilation for profiling, indicated by `#ifdef ICAP`.
- Some methods are declared but not implemented (e.g., drag-and-drop functionality commented out).

## `src/zlib/winstyles.h`

PURPOSE: This file defines style constants and their corresponding bitmasks for use in the Allegiance game's graphical user interface.

KEY PARTS:
- StyleBaseType: Base class for style bitmask types.
- Style: Template class for creating style bitmask objects.
- StyleChild, StylePopup, etc.: Derived classes representing specific window styles like WS_CHILD, WS_POPUP, etc.
- StyleEXBaseType: Base class for extended style bitmask types.
- StyleEXTopMost, StyleEXTransparent, etc.: Derived classes representing specific extended window styles like WS_EX_TOPMOST, WS_EX_TRANSPARENT, etc.

CONNECTS TO: mask.h

NOTES: None.

## `src/zlib/zadapt.h`

PURPOSE: This file defines a template class `ZAdapt` that allows classes like `TRefs` to be stored in STL containers without breaking due to the operator &.

KEY PARTS:
- ZAdapt<T>: A template class that adapts any type T for use in STL containers.
  - Constructors and assignment operator to handle copying of T.
  - Comparison operators (<, ==) to allow sorting and equality checks.
  - Type conversion operators (to T&, const T&) to allow direct access to the adapted object.
  - Pointer-like operators (->) to facilitate usage as pointers.

CONNECTS TO: STL containers that require custom types for storage.

NOTES: None.

## `src/zlib/zassert.h`

PURPOSE: This file defines macros and utility functions for assertions, warnings, errors, and tracing in the Allegiance game engine. It provides conditional compilation based on whether the build is debug or retail.

KEY PARTS:
- ZAssertImpl: Core function to handle assertion failures.
- ZRetailAssert: Macro for retail builds that always evaluates to false.
- ZDebugBreak: Breaks into the debugger if assertions are enabled in a debug build.
- ZWarningImpl, ZFailedImpl, ZSucceededImpl: Functions to handle warnings and HRESULT results.
- ZAssert, ZVerify, ZWarning, ZError, ZBadCase, ZFailed, ZSucceeded: Macros for different types of checks and errors.
- debugf, ZDebugOutputImpl, ZDebugOutput: Functions/macros for logging debug information.
- ZUnimplemented: Macro to indicate unimplemented code paths.
- assert, ShouldBe, VerifyHR, BreakOnError: Aliases for various assertion macros.

CONNECTS TO: Logger.h (for logging functionality).

NOTES: None.

## `src/zlib/zlib.h`

PURPOSE: This file is a header file for Allegiance's zlib library, which includes various utility classes and functions used throughout the game.

KEY PARTS:
- `zlib.H`: Main header file for the zlib library.
- `FLOATASM` and `USEASM`: Macros related to assembly optimizations.
- Required headers: Standard C/C++ libraries, Windows headers, DirectX definitions, and custom Allegiance headers for various utilities like assertions, memory management, mathematics, and more.

CONNECTS TO: Other system files and libraries used by Allegiance, such as DirectX and Win32 API.

NOTES: None.

## `src/zlib/zmath.cpp`

PURPOSE: This file contains utility mathematical functions and constants used throughout the Allegiance game.

KEY PARTS:
- ZAlloc: Allocates memory with debug information for tracking in debug builds.
- ZFree: Frees memory allocated by ZAlloc.
- CountBits: Counts the number of set bits (1s) in a 32-bit unsigned integer.
- GetShift: Determines the position of the least significant bit that is set to 1.
- NextPowerOf2: Finds the smallest power of 2 greater than or equal to a given number.

CONNECTS TO: zmath.h, crtdbg.h, zassert.h

NOTES: None.

## `src/zlib/zmath.h`

PURPOSE: This file contains various mathematical utility functions and constants for floating-point arithmetic, bit manipulation, interpolation, and random number generation.

KEY PARTS:
- `RadiansFromDegrees`: Converts degrees to radians.
- `DegreesFromRadians`: Converts radians to degrees.
- `NextMultipleOf`: Returns the next larger multiple of a given size.
- `CountBits`, `GetShift`, `NextPowerOf2`, `PowerOf2`, `MakeMask`: Bit manipulation functions.
- `odd`: Checks if an integer is odd.
- `bound`: Bounds a value within a specified range.
- `Swap`: Swaps the values of two variables.
- `sign`: Returns the sign of a floating-point number.
- `mod`: Computes the modulus of a floating-point number with a limit.
- `random`, `randomInt`: Generates random numbers within specified ranges.
- `SmoothInterpolant`, `Interpolate`: Interpolation functions.
- `TRange`: A range class that wraps an integer value and ensures it stays within a specified count.
- `NullFunc`: An empty function object.

CONNECTS TO: This file is used by various parts of the Allegiance game engine for mathematical operations.

NOTES: None.

## `src/zlib/zreg.h`

PURPOSE: This file provides utility functions to simplify registry access in Windows applications.

KEY PARTS:
- LoadRegString<T>: Loads a string value from the registry into an output parameter.
- LoadRegStream: Loads binary data from the registry into an IStream object.
- SaveRegStream: Saves binary data from an IStream object to the registry.

CONNECTS TO: windows.h, <malloc.h>, <cstdint>

NOTES: None.

## `src/zlib/zstring.cpp`

**PURPOSE:** The `zstring.cpp` file contains the implementation of a custom string class (`ZString`) and related classes for managing strings, including operations like token extraction, encryption, and path manipulation.

**KEY PARTS:**
- **Class: ZStringData** - Manages internal data structure for storing string information.
- **Function: ZString::ZStringData::Set(int index, char ch)** - Sets a character at a specific index in the string data.
- **Function: ZToUpper(char* szDest, PCC szSource)** - Converts a source string to uppercase.
- **Class: FilenameString** - Extracts file extension and name from a filename string.
- **Class: PathString** - Manages path operations, including extraction of directory components and concatenation.

**CONNECTS TO:** Other parts of the Allegiance game that require string manipulation and handling.

**NOTES:** The code does not handle network paths (`c:\`) correctly. There is unusual code related to `ScrambleMunge`.

## `src/zlib/zstring.h`

PURPOSE: This file defines a custom string class `ZString` and related classes for handling filenames, paths, and URLs. It includes methods for manipulation, comparison, and encryption.

KEY PARTS:
- ZString: A custom string class with various constructors, operators, and methods for string operations.
- FilenameString: Inherits from ZString to handle filename-specific operations like getting the extension or name.
- PathString: Inherits from ZString to handle path-specific operations like getting the current directory, module path, or components of a path.
- URLString: Inherits from ZString to represent URLs, specifically for file paths.

CONNECTS TO: tref.h (for reference counting), IObjectSingle (interface for single-threaded objects).

NOTES: None.

## `src/zlib/ztime.cpp`

PURPOSE: This file contains implementation details for time-related functionalities in the Allegiance game, including pausing and resuming the game clock, managing debug training features, converting Windows time to Unix timestamp, and formatting current time as a string.

KEY PARTS:
- Time::Pause(): Pauses the game clock.
- Time::IsPaused(): Checks if the game clock is paused.
- Time::Continue(): Resumes the game clock after pausing.
- Time::getUnixTime(): Converts Windows system time to Unix timestamp.
- Time::getTimeString(): Formats current system time as a string.

CONNECTS TO: zassert.h, time.h

NOTES: None.

## `src/zlib/ztime.h`

**PURPOSE:** The `ztime.h` file defines the `Time` class, which is used to manage and manipulate game time in Allegiance. It provides functionality for pausing, resuming, and calculating elapsed time.

**KEY PARTS:**
- **Class: Time**: Manages game time with methods to get current time, pause, resume, and compare times.
- **Function: Now()**: Returns the current game time, accounting for pauses and shifts.
- **Function: Pause()**: Pauses the game time.
- **Function: Continue()**: Resumes the game time from a paused state.
- **Function: getUnixTime()**: Converts the current game time to a Unix timestamp (without milliseconds).
- **Function: getTimeString()**: Returns the current game time as a string.

**CONNECTS TO:** `TlsValue.h`, `windows.h`

**NOTES:** None.
