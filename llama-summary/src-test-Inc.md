# src/test/Inc

[Back to the index](../LLAMA_SUMMARY.md) - 11 files, 3,447 lines. Notes by a local model, to orient by: not verified facts.

This folder contains header and implementation files related to testing and development utilities in Allegiance. It includes custom dialog classes, memory management overrides, IDL definitions for COM interfaces, ATL extensions, and utility functions.

- Allegiance.h: Tiny header file with basic definitions.
- AppWizStepDlg.h: Defines a custom wizard step dialog class for handling initialization and message processing.
- TCLib.h: Contains various utility functions and macros for debugging, error reporting, and type manipulation.
- ScrRun.idl: IDL definitions for file system operations and scripting in a COM-based environment.
- TCAtl.cpp: Utility functions and implementations for handling interfaces and determining object sizes.

Largest files: TCLib.h (1143 lines), ScrRun.idl (555 lines), TCIDL.idl (508 lines), AppWizStepDlg.h (400 lines), TCAtl.cpp (347 lines).

## `src/test/Inc/Allegiance.h`

PURPOSE: tiny file (20 lines), shown in full rather than summarized.

    #ifndef __Allegiance_h__
    #define __Allegiance_h__
    /////////////////////////////////////////////////////////////////////////////
    #include <utility.h>
    #include <..\engine\engine.h>
    #include <..\effect\effect.h>
    #include <..\igc\igc.h>
    #include <..\Zone\Zone.h>
    //#include <..\zone\zauth.h>
    #include <..\igc\mapmakerigc.h>
    #include <messagesAll.h>
    #include <messages.h>
    #include <messagesLC.h>
    //#include <networkgame.h>
    #include <..\clintlib\clintlib.h>
    #include <..\clintlib\AutoDownload.h>
    //#include <..\drones\actionlib.h>
    #include <..\igc\shipigc.h>
    /////////////////////////////////////////////////////////////////////////////
    #endif // !__Allegiance_h__

## `src/test/Inc/AppWizStepDlg.h`

PURPOSE: This header file defines a custom dialog class `TCAppWizStepDlg` that extends the functionality of a standard wizard step dialog in a Windows application. It handles initialization, message processing, and subclassing to customize the appearance and behavior of the dialog.

KEY PARTS:
- TCAppWizStepDlg: Custom dialog class extending CAppWizStepDlg.
- EnableFinish: Enables or disables the Finish button on the parent dialog.
- OnHelpClicked: Handles the Help button click event.
- OnInitDialog: Initializes the dialog, changes the etched line, loads images, and subclasses the parent window.
- OnWndMsg: Processes custom messages for enabling/disabling the Finish button.
- FrameFindProc: Enumerates child windows to find and replace black frames with etched lines.
- PictureFindProc: Loads images into picture controls.
- ParentWndProc: Subclass procedure for handling commands and show/hide events on the parent window.

CONNECTS TO: CAppWizStepDlg, WindowFinder.h

NOTES: None.

## `src/test/Inc/DevStudio.h`

PURPOSE: This file is a header file that includes various definitions and interfaces for Microsoft Visual Studio development tools.

KEY PARTS:
- #import statements: Importing type libraries from DLLs and PKG files related to Visual Studio's IDE components.
- Named GUIDs: Ensuring that the imported types have unique identifiers (GUIDs) for proper COM interaction.

CONNECTS TO: Microsoft Visual Studio IDE components, specifically its development tools and environment.

NOTES: None.

## `src/test/Inc/FixDelete.h`

PURPOSE: This file provides a custom implementation of the `delete` operator to ensure compatibility with the Microsoft Visual C++ runtime's memory management in debug builds when using the Standard Template Library (STL).

KEY PARTS:
- Name: `operator delete`
  - What it does: Overrides the global `delete` operator to correctly pass the block type flag to `_free_dbg` for debug builds, preventing assert failures related to memory allocation types.

CONNECTS TO: 
- `<new>` and `<crt/src/dbgdel.cpp>`

NOTES:
- None.

## `src/test/Inc/ScrRun.idl`

PURPOSE: This file contains IDL (Interface Definition Language) definitions for various interfaces related to file system operations and scripting in a COM-based environment.

KEY PARTS:
- `IDictionary`: Provides methods for managing key-value pairs.
- `IFileSystem`: Offers functions for file and folder manipulation, including creating, deleting, copying, and moving files and folders.
- `IDrive`: Represents a drive and provides information about its properties such as type, free space, and volume name.
- `IFolder`: Represents a folder and includes methods for accessing subfolders and files within it.
- `IFile`: Represents a file and provides methods for reading, writing, and deleting the file.
- `ITextStream`: Provides methods for reading from and writing to text streams.
- `IScriptEncoder`: Offers functionality to encode script files.

CONNECTS TO: SCRRUN.DLL, which is a COM library providing scripting and automation capabilities.

NOTES: None.

## `src/test/Inc/TCAtl.cpp`

**PURPOSE:** This file contains utility functions and implementations for handling interfaces, streaming safe arrays, and determining the size of objects that support persistence.

**KEY PARTS:**
- `TCGetIIDName(REFIID riid)`: Converts an interface identifier (IID) to a human-friendly name.
- `TCReadSafeArrayFromStream(IStream* pstm, CComVariant& var)`: Reads a SAFEARRAY from an IStream into a CComVariant.
- `TCWriteSafeArrayToStream(IStream* pstm, CComVariant& var)`: Writes a SAFEARRAY in a CComVariant to an IStream.
- `TCGetPersistStreamSize(IUnknown* punk, ULARGE_INTEGER* pCbSize)`: Computes the size of stream necessary to persist an object that supports IPersistStream or IPersistStreamInit.

**CONNECTS TO:** 
- `<map>`, `<string>`: Standard C++ libraries for data structures.
- `TCAutoCriticalSection` and `TCObjectLock<TCAutoCriticalSection>`: Custom synchronization classes from the TCLib library.
- `CRegKey`: ATL class for registry operations.
- `SafeArray*` functions: Windows API functions for manipulating SAFEARRAYs.

**NOTES:** 
- The function `QueryStringValue` is mentioned but not implemented, which might be a placeholder or an oversight.

## `src/test/Inc/TCAtl.h`

PURPOSE: This file contains declarations and definitions of extensions to Microsoft's Active Template Library (ATL) framework. These extensions are designed to be independent from the Microsoft Foundation Class (MFC) framework but compatible with it.

KEY PARTS:
- TCGetIIDName(REFIID riid): Retrieves the name of an interface ID.
- TCReadSafeArrayFromStream(IStream* pstm, CComVariant& var): Reads a SafeArray from a stream into a variant.
- TCWriteSafeArrayToStream(IStream* pstm, CComVariant& var): Writes a SafeArray to a stream from a variant.
- TCGetPersistStreamSize(IUnknown* punk, ULARGE_INTEGER* pCbSize): Determines the size of a persistable object's stream.

CONNECTS TO: comdef.h, atlctl.h, afxole.h, TCLib.h

NOTES: None.

## `src/test/Inc/TCIDL.idl`

PURPOSE: This file contains interface definitions and enumerations for a COM-based system in Allegiance, likely used for managing collections, properties, sessions, and utility functions.

KEY PARTS:
- ITCCollection: Base interface for most collection interfaces.
- ITCStrings: Interface to manage or query a collection of strings.
- ITCPropBagOnRegKey: Interface to create a property bag on a registry key.
- ITCSessionInfo: Exposes information about the connection of a session object.
- ITCSessionInfos: Enumerates a collection of Session Information Objects.
- ITCCollectionPersistHelper: Helper interface for collection persistence.
- ITCUtility: Provides utility functions like creating objects and sleeping.

CONNECTS TO: ScrRun.idl, likely other COM interfaces used in the Allegiance game.

NOTES: None.

## `src/test/Inc/TCLib.h`

PURPOSE: The file `TCLib.h` contains utility functions and macros for various purposes such as debugging, error reporting, type manipulation, and COM error handling.

KEY PARTS:
- sizeofArray(x): Counts the number of elements in a fixed-length array.
- HiBits<T>(x, nBitCount): Extracts high-order bits and shifts them to the low order.
- VARBOOL(expression): Translates a boolean expression to a valid VARIANT_BOOL.
- TCReportComErr: Reports COM errors.
- TCERRLOG: Macro for logging error messages.
- PRIVATE_ASSERTE, PRIVATE_VERIFY, PRIVATE_VERIFYE: Macros for assertions and verification that behave differently in debug and release builds.

CONNECTS TO: None

NOTES: The file is part of the Allegiance game's source code and includes both C++ and C# components.

PARTS (in file order):

- lines 1-345
  - sizeofArray(x): Counts the number of elements in a fixed-length array.
  - HiBits<T>(x, nBitCount): Extracts high-order bits and shifts them to the low order.
  - HiBit<T>(x): Extracts the most significant bit from a value.
  - LoBits<T>(x, nBitCount): Masks a value to keep only the specified number of low-order bits.
  - LoBit<T>(x): Masks a value to keep only the least significant bit.
  - VARBOOL(expression): Translates a boolean expression to a valid VARIANT_BOOL.
  - ClearOut<T>(dest, init): Initializes an out parameter within an exception handler.
  - CLEAROUT(dest, init): Macro wrapper for ClearOut that returns if initialization fails.
- lines 347-673
  - _SVERIFY: Evaluates an expression and generates a debug report when the result of passing the expression to the SUCCEEDED macro is FALSE (debug version only).
  - _SVERIFYE: Similar to _SVERIFY but with additional functionality.
  - DEBUG_ONLY: Provides a shorthand way to evaluate an expression only under _DEBUG builds.
  - TCTrace: Sends a formatted string to a debug monitor.
  - _TRACE0 to _TRACE5: Macros for sending formatted strings to the debug monitor, depending on the number of parameters.
  - _TRACE_BEGIN_SIZE and _TRACE_BEGIN: Begin tracing a message with a specified maximum size.
  - _TRACE_PART0 to _TRACE_PART5: Append parts of a formatted string to the trace buffer.
  - _TRACE_END: End tracing and output the message.
- lines 674-1038
  - TCReportComErr: Reports COM errors.
  - TCReportHR: Reports HRESULT errors.
  - TCTypeName_EatPrefixesV: Removes specified prefixes from a type name.
  - TCTypeName_EatPrefixes: Wrapper for TCTypeName_EatPrefixesV.
  - TCTypeName: Macro to get a modified type name without specific prefixes.
  - RegisterComponentCategory: Registers a component category.
  - UnregisterComponentCategory: Unregisters a component category.
  - GetVariantDateTime: Retrieves the current date and time as a variant date.
- lines 1040-1143
  - TCERRLOG: Macro for logging error messages.
  - TCERRLOG0 to TCERRLOG5: Macros with varying numbers of parameters for logging formatted strings.
  - TCERRLOG_BEGIN_SIZE and TCERRLOG_BEGIN: Macros for initializing a buffer to hold the log message.
  - TCERRLOG_PART0 to TCERRLOG_PART5: Macros for appending formatted parts to the log message buffer.
  - TCERRLOG_END: Macro for outputting the log message, either using ZDebugOutputImpl or OutputDebugString based on build configuration.
  - PRIVATE_ASSERTE, PRIVATE_VERIFY, PRIVATE_VERIFYE: Macros for assertions and verification that behave differently in debug and release builds.

## `src/test/Inc/UnkDoc.idl`

PURPOSE: This file defines interfaces used in COM (Component Object Model) for managing object references and querying interfaces.

KEY PARTS:
- IUnknown: Provides methods to query interfaces, add references, and release objects.
- IDispatch: Exposes objects to automation tools and other applications through type information and method invocation.

CONNECTS TO: Other COM-related files and systems that use these interfaces for object management and automation.

NOTES: None.

## `src/test/Inc/WindowFinder.h`

PURPOSE: The file defines a class `TCWindowFinder` used to find a window by its title and optionally process ID.

KEY PARTS:
- Class `TCWindowFinder`: Manages the search for a window.
  - Constructor overloads to initialize with either an integer title or a string title.
  - Methods to get the enum proc, LPARAM, and HWND.
  - Operator overloads to facilitate easy conversion to WNDENUMPROC, LPARAM, and HWND.

CONNECTS TO: `GetWindowThreadProcessId`, `GetWindowText` from Windows API.

NOTES: None.
