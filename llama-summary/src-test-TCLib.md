# src/test/TCLib

[Back to the index](../LLAMA_SUMMARY.md) - 47 files, 14,033 lines. Notes by a local model, to orient by: not verified facts.

This folder contains utility classes and functions for managing various aspects of the Allegiance game engine, including COM operations, threading, memory management, and string manipulation.

- ActiveObject.h: Provides functionality similar to OLE API `RegisterActiveObject` without security context limitations.
- AdviseHolder.h: Manages connecting to and disconnecting from connectable component objects using IConnectionPointContainer.
- AsyncDebugOutput.h: Asynchronously writes debug output with timestamps to a log file.
- AutoHandle.h: Automatically manages the lifecycle of Windows handles, ensuring they are closed when out of scope.
- DynaLib.h: Framework for loading dynamic link libraries and resolving their exported functions.
- PropertyPageSite.cpp: Manages an IPropertyPage interface, handling creation, attachment, and manipulation of property pages.
- TCLib.cpp: Contains utility functions and classes for managing component categories, formatting dates, streams, and object operations.
- TCThread.h: Defines a class for managing threads in Allegiance, similar to CWinThread but without MFC dependencies.
- UtilityThread.h: Base class for performing background tasks in a separate thread, allowing queued work items to be processed asynchronously.

Largest files: DynaLib.h (1602 lines), WotLib.h (1063 lines), TCLib.cpp (1049 lines), PropertyPageSite.cpp (966 lines), AutoHandle.h (867 lines).

## `src/test/TCLib/ActiveObject.h`

PURPOSE: This file contains a function `MyRegisterActiveObject` that provides functionality similar to the OLE API `RegisterActiveObject`, but without its security context limitations.

KEY PARTS:
- Name: MyRegisterActiveObject
  - What it does: Registers an object in the Running Object Table (ROT) and handles the `ROTFLAGS_ALLOWANYCLIENT` flag, unlike `RegisterActiveObject`.

CONNECTS TO:
- Depends on OLE APIs such as `StringFromCLSID`, `CreateItemMoniker`, `GetRunningObjectTable`, and `IRunningObjectTable::Register`.
- Serves other parts of the application that need to register objects in the ROT.

NOTES: 
- Contains debug code that displays a message box if `IROT::Register` fails with `CO_E_WRONG_SERVER_IDENTITY`. This is related to security context issues when debugging.

## `src/test/TCLib/AdviseHolder.h`

PURPOSE: The file defines a class `TCAdviseHolder` that encapsulates the process of connecting to and disconnecting from a connectable component object using the IConnectionPointContainer interface.

KEY PARTS:
- `TCAdviseHolder`: Manages the connection to a connectable object.
- `XData`: Nested class for reference-counted data used by `TCAdviseHolder`.
- `FindConnectionPoint`: Finds the specified outgoing interface in an object.
- `Advise`: Sets up an advisory connection to an outgoing interface of a connectable object.
- `Unadvise`: Terminates the advisory connection.

CONNECTS TO: `comdef.h`, `RefCountedData.h`

NOTES:
- The class uses reference counting to manage connections, making it suitable for use within STL containers.
- The destructor automatically calls `Unadvise` to disconnect from the object when an instance is destroyed.

## `src/test/TCLib/AsyncDebugOutput.h`

PURPOSE: The file `AsyncDebugOutput.h` defines a class for asynchronously writing debug output to a log file. It handles formatting messages with timestamps and manages asynchronous I/O operations.

KEY PARTS:
- `TCAsyncDebugOutput`: Main class that encapsulates the functionality of opening a file, formatting messages, and performing asynchronous writes.
- `Write`, `WriteV`, `WriteNoTime`, `WriteNoTimeV`, `WriteLen`, `WriteLenV`, `WriteLenNoTime`, `WriteLenNoTimeV`: Member functions to write formatted messages with or without timestamps, and with a specified maximum length.
- `FileIOCompletionRoutine`: Callback function that handles the completion of asynchronous I/O operations.

CONNECTS TO: Uses Windows API functions like `CreateFile`, `WriteFileEx`, `HeapAlloc`, and `HeapFree`.

NOTES: None.

## `src/test/TCLib/AutoCriticalSection.h`

PURPOSE: This file defines a class `TCAutoCriticalSection` that manages the initialization and cleanup of Win32 critical section objects. It ensures thread-safe access to shared resources by automatically locking and unlocking the critical section.

KEY PARTS:
- TCAutoCriticalSection: Class for managing critical sections.
  - Constructor: Initializes the critical section.
  - Destructor: Deletes the critical section.
  - Lock: Acquires ownership of the critical section.
  - Unlock: Releases ownership of the critical section.

CONNECTS TO: None

NOTES: none

## `src/test/TCLib/AutoHandle.h`

**PURPOSE:** The file `AutoHandle.h` provides a template class `TCAutoHandle` and several typedefs for managing various types of Windows handles automatically by ensuring they are closed when the object goes out of scope. This helps prevent resource leaks and ensures proper cleanup.

**KEY PARTS:**
- **Name:** TCAutoHandle
  - Description: A template class that encapsulates a handle type (T) and ensures proper cleanup using a specified close function.
  
- **Name:** TCEventSourceHandle
  - Description: A typedef for TCAutoHandle specialized to handle Win32 event source handles, closing them with the DeregisterEventSource API.

- **Name:** TCGlobalFree
  - Description: A struct implementing an operator() that calls GlobalFree on a HGLOBAL handle and returns true if successful.
  
- **Name:** TCGlobal
  - Description: A typedef for TCAutoHandle specialized to handle Win32 global memory handles, closing them with the GlobalFree API.

- **Name:** TCGlobalUnlock
  - Description: A struct implementing an operator() that calls GlobalUnlock on a HGLOBAL handle and returns true if successful.
  
- **Name:** TCGlobalPtr
  - Description: A typedef for TCAutoHandle specialized to handle pointers returned by GlobalLock, closing them with the GlobalUnlock API.

- **Name:** TCRegCloseKey
  - Description: A struct implementing an operator() that calls RegCloseKey on a HKEY handle and returns true if successful.
  
- **Name:** TCRegKeyHandle
  - Description: A typedef for TCAutoHandle specialized to handle Win32 registry keys, closing them with the RegCloseKey API.

- **Name:** TCCloseServiceHandle
  - Description: A struct implementing an operator() that calls CloseServiceHandle on a SC_HANDLE and returns true if successful.
  
- **Name:** TCServiceHandle
  - Description: A typedef for TCAutoHandle specialized to handle Win32 service handles, closing them with the CloseServiceHandle API.

- **Name:** TCDeleteObject
  - Description: A struct implementing an operator() that calls DeleteObject on a HGDIOBJ and returns true if successful.
  
- **Name:** TCGDIObjHandle
  - Description: A typedef for TCAutoHandle specialized to handle GDI objects, closing them with the DeleteObject API.

**CONNECTS TO:** Other Windows API functions such as `CloseHandle`, `FindClose`, `HeapDestroy`, `FreeLibrary`, and `UnmapViewOfFile`.

**NOTES:** The use of function objects (functor classes) for handle closing operations is efficient due to inline implementation.

## `src/test/TCLib/AutoMutex.h`

PURPOSE: The file `AutoMutex.h` declares and defines the `TCAutoMutex` class, which provides methods for obtaining and releasing ownership of a Win32 mutex object. It automatically initializes the mutex in its constructor and cleans it up in its destructor.

KEY PARTS:
- TCAutoMutex: Class that manages a Win32 mutex object.
  - Lock(): Acquires ownership of the mutex.
  - Lock(DWORD dwMilliseconds): Acquires ownership with a timeout.
  - Unlock(): Releases ownership of the mutex.
  - m_hmtx: Protected data member representing the Win32 mutex handle.

CONNECTS TO: `AutoHandle.h` (dependency for managing handles).

NOTES: None.

## `src/test/TCLib/BinString.h`

PURPOSE: The file defines a template class `TCBinString` that provides methods for encoding binary data into a string format and decoding such strings back to binary data. It uses an algorithm defined in the nested struct `TCBinStringAlgorithm_0_0`.

KEY PARTS:
- `TCBinStringAlgorithm`: A template struct providing algorithms for encoding and decoding binary data.
  - `GetEncodedStringLength`: Calculates the length of the encoded string.
  - `EncodeToString`: Encodes binary data into a string.
  - `IsAlgorithmSignature`, `GetDecodedDataSize`, `DecodeFromString`: Methods for decoding strings back to binary data.
- `TCBinString_Literal`: A macro for declaring string literals in templates.
- `TCBinStringAlgorithm_0_0`: Inherits from `TCBinStringAlgorithm` and implements specific encoding and decoding logic.
  - `IsByteDisplayable`, `GetCharCode`: Helper functions for encoding.
  - Constants: Signature, escape sequences.
  - Encoding methods: `GetEncodedStringLength`, `EncodeToString`.
  - Decoding methods: `IsAlgorithmSignature`, `GetDecodedDataSize`, `DecodeFromString`.

CONNECTS TO: None explicitly mentioned.

NOTES: The file uses a macro to handle string literals for different character types (ANSI and Unicode). There is no indication of any TODOs, hacks, disabled code, or security/network handling concerns.

## `src/test/TCLib/CoInit.h`

PURPOSE: The file defines a class `TCCoInit` that simplifies the initialization and cleanup of COM (Component Object Model) in Windows applications.

KEY PARTS:
- Name: TCCoInit
  - What it does: Manages the initialization and uninitialization of COM using `CoInitializeEx` or `CoInitialize`, ensuring proper resource management.
  
- Name: CoInitializeEx
  - What it does: Initializes the COM library on the current thread with specified apartment model.

- Name: CoInitialize
  - What it does: Initializes the COM library on the current thread in a single-threaded apartment (STA).

- Name: Uninit
  - What it does: Safely uninitializes COM if initialization was successful, preventing double uninitialization.

- Name: Succeeded
  - What it does: Checks if COM initialization was successful.

- Name: Failed
  - What it does: Checks if COM initialization failed.

- Name: HRESULT
  - What it does: Represents the result of a COM operation, used for error handling.

CONNECTS TO:
- TCLib.h: This file is included at the top, indicating that `TCCoInit` depends on other components defined in this library.

NOTES:
- None.

## `src/test/TCLib/CommonControls.h`

PURPOSE: The file `CommonControls.h` declares the `TCCommonControls` class, which provides a run-time interface to the Win32 Common Control Library. It dynamically loads the necessary library and checks for the availability of newer features.

KEY PARTS:
- TCCommonControls: Class that manages loading and initializing the Common Controls Library.
- InitEx: Method to initialize the Common Controls Library, using `InitCommonControlsEx` if available, otherwise falling back to `InitCommonControls`.
- TCDynaLib_Fn0_void(InitCommonControls): Macro for declaring a function with no parameters and returning void.
- TCDynaLib_Fn1(BOOL, InitCommonControlsEx, LPINITCOMMONCONTROLSEX): Macro for declaring a function that takes an `LPINITCOMMONCONTROLSEX` parameter and returns a boolean.

CONNECTS TO: `DynaLib.h`, `<commctrl.h>`

NOTES: None.

## `src/test/TCLib/CompositeValue.h`

PURPOSE: The file defines a template class `TCCompositeValue` that evaluates a series of values to determine a composite value representing the series. This is useful for property pages in applications where multiple objects' properties need to be displayed and managed.

KEY PARTS:
- Class: `TCCompositeValue<T>`
  - Evaluates a series of values to determine a composite value.
  - Supports initialization with default and unequal result values.
  - Provides methods to evaluate values, check equality, get the current value, and reset the state.
- Methods:
  - `Evaluate(const T& tNextValue)`: Evaluates the next value in the series.
  - `IsEqual() const`: Checks if all evaluated values are equal.
  - `GetValue() const`: Gets the current composite value.
  - `Reset(const T& tInitValue = T())`: Resets the evaluation state.

CONNECTS TO: None

NOTES:
- The class can be used with various types of values, including strings.
- TODOs include adding a second template parameter for binary operations and coercing results to symbolic check box states.

## `src/test/TCLib/DynaLib.cpp`

PURPOSE: The file `DynaLib.cpp` implements the `TCDynaLib` class, which provides functionality to load dynamic link libraries (DLLs) and resolve API exports at runtime.

KEY PARTS:
- **Class: TCDynaLib**
  - Manages loading and unloading of DLLs.
  - Provides methods to check if a library is loaded and resolved.
  - Includes functions to get the address of an exported function from a DLL.

- **Function: LoadLibrary(LPCTSTR pszLibName)**
  - Loads a specified DLL into the instance. Returns true if successful, false otherwise.

- **Function: GetProcAddress(LPCSTR pszFn, char chDecoration, int cbArgs)**
  - Resolves the address of an exported function from the loaded DLL using a specific calling convention and argument count.

- **Function: FunctionNotFound(LPCSTR pszFn)**
  - Handles the case when a required function cannot be resolved. Outputs a debug message and asserts.

CONNECTS TO:
- Other parts of the Allegiance game that require dynamic loading of DLLs and resolving API exports.

NOTES:
- None

## `src/test/TCLib/DynaLib.h`

PURPOSE: The file `DynaLib.h` provides a framework for loading dynamic link libraries (DLLs) and resolving their exported functions in the Allegiance game engine.

KEY PARTS:
- TCDynaLib: Manages the loading and resolution of DLLs.
- IsLibraryLoaded: Checks if a library has been successfully loaded.
- IsLibraryLoadedAndResolved: Ensures that all required APIs are resolved.
- LoadLibrary: Loads a DLL by its name.
- GetProcAddress: Retrieves the address of an exported function from a DLL.
- FunctionNotFound: Handles cases where a required function is not found in the DLL.
- CLock: Provides thread synchronization for library operations.
- TCDynaLib_BEGIN_REQUIRED_MAP and TCDynaLib_END_REQUIRED_MAP: Macros to define which API exports are required.

CONNECTS TO: Allegiance game engine components that require dynamic library functionality.

NOTES: None.

PARTS (in file order):

- lines 1-339
  - TCDynaLib: Class for loading DLLs and resolving API exports.
  - IsLibraryLoaded: Checks if a library has been loaded.
  - IsLibraryLoadedAndResolved: Checks if a library has been loaded and all required APIs are resolved.
  - LoadLibrary: Loads a library by name.
  - GetProcAddress: Retrieves the address of an exported function.
  - FunctionNotFound: Handles cases where a function is not found in the DLL.
  - CLock: Internal class for thread synchronization.
  - TCDynaLib_BEGIN_REQUIRED_MAP, TCDynaLib_END_REQUIRED_MAP: Macros to define required API exports.
- lines 340-656
  - TCDynaLib_Fn0(retType, name): Macro for declaring a member function with 0 parameters.
  - TCDynaLib_Fn1(retType, name, P1): Macro for declaring a member function with 1 parameter.
  - TCDynaLib_Fn2(retType, name, P1, P2): Macro for declaring a member function with 2 parameters.
  - TCDynaLib_Fn3(retType, name, P1, P2, P3): Macro for declaring a member function with 3 parameters.
  - TCDynaLib_Fn4(retType, name, P1, P2, P3, P4): Macro for declaring a member function with 4 parameters.
  - TCDynaLib_Fn5(retType, name, P1, P2, P3, P4, P5): Macro for declaring a member function with 5 parameters.
  - TCDynaLib_Fn6(retType, name, P1, P2, P3, P4, P5, P6): Macro for declaring a member function with 6 parameters.
  - TCDynaLib_Fn7(retType, name, P1, P2, P3, P4, P5, P6, P7): Macro for declaring a member function with 7 parameters.
- lines 657-978
  - TCDynaLib: Main class for dynamic library management.
  - FunctionNotFound: Function to handle cases where a required function is not found in the library.
  - TCPtr: Smart pointer class used for managing pointers to dynamically loaded functions.
  - Exists_##name(): Macro to check if a function exists in the loaded library.
  - TCDynaLib_FnExists(name): Macro to declare existence of a function.
  - TCDynaLib_PreFn0(retType, pre, name): Macro for declaring and implementing a function with no parameters.
  - TCDynaLib_PreFn1(retType, pre, name, P1): Macro for declaring and implementing a function with one parameter.
  - TCDynaLib_PreFn2(retType, pre, name, P1, P2): Macro for declaring and implementing a function with two parameters.
- lines 980-1301
  - TCDynaLib_PreFn12: Defines a member function that takes 12 parameters and returns a value.
  - TCDynaLib_PreFn0_void: Defines a member function with no parameters and no return value.
  - TCDynaLib_PreFn1_void: Defines a member function that takes one parameter and no return value.
  - TCDynaLib_PreFn2_void: Defines a member function that takes two parameters and no return value.
  - TCDynaLib_PreFn3_void: Defines a member function that takes three parameters and no return value.
  - TCDynaLib_PreFn4_void: Defines a member function that takes four parameters and no return value.
  - TCDynaLib_PreFn5_void: Defines a member function that takes five parameters and no return value.
  - TCDynaLib_PreFn6_void: Defines a member function that takes six parameters and no return value.
- lines 1302-1601
  - TCDynaLib_DecPreFn0_void: Defines a function that takes no parameters and returns void.
  - TCDynaLib_DecPreFn1_void: Defines a function that takes one parameter and returns void.
  - TCDynaLib_DecPreFn2_void: Defines a function that takes two parameters and returns void.
  - TCDynaLib_DecPreFn3_void: Defines a function that takes three parameters and returns void.
  - TCDynaLib_DecPreFn4_void: Defines a function that takes four parameters and returns void.
  - TCDynaLib_DecPreFn5_void: Defines a function that takes five parameters and returns void.
  - TCDynaLib_DecPreFn6_void: Defines a function that takes six parameters and returns void.
  - TCDynaLib_DecPreFn7_void: Defines a function that takes seven parameters and returns void.

## `src/test/TCLib/ExcepInfo.cpp`

PURPOSE: This file contains implementations related to error handling in the Allegiance game engine, specifically focusing on managing and creating error information structures.

KEY PARTS:
- TCEXCEPINFO::Release: Frees memory allocated for a `EXCEPINFO` structure.
- TCEXCEPINFO::CreateErrorInfo: Creates an `IErrorInfo` object from a `EXCEPINFO` structure.
- TCEXCEPINFO::SetErrorInfo: Sets the current thread's error information to that provided by a `EXCEPINFO` structure.

CONNECTS TO: Uses COM interfaces (`ICreateErrorInfo`, `IErrorInfo`) for error handling.

NOTES: None.

## `src/test/TCLib/ExcepInfo.h`

PURPOSE: This file defines a class `TCEXCEPINFO` that encapsulates the functionality of the Windows COM error information structure `tagEXCEPINFO`, providing methods to manage and manipulate error information.

KEY PARTS:
- TCEXCEPINFO: A class derived from `tagEXCEPINFO` with additional methods for managing error information.
  - Constructor: Initializes a new instance of the `TCEXCEPINFO` class.
  - Destructor: Releases any resources held by the `TCEXCEPINFO` instance.
  - Release: Releases the error information associated with this object.
  - CreateErrorInfo: Creates an error information object from the current state of this object.
  - SetErrorInfo: Sets the error information for the calling thread.

CONNECTS TO: COM (Component Object Model) libraries, specifically related to error handling in COM applications.

NOTES: None.

## `src/test/TCLib/InvokeArgs.cpp`

PURPOSE: The file `InvokeArgs.cpp` implements the `TCInvokeArgs` class, which is used to manage and invoke methods on COM objects through the `IDispatch` interface.

KEY PARTS:
- `TCInvokeArgs`: Manages dispatch parameters for calling methods on COM objects.
- `~TCInvokeArgs`: Destructor that cleans up allocated resources.
- `Invoke`: Calls the `Invoke` method on an `IDispatch` object with previously set dispatch parameters.
- `SetDispParamsV`: Sets the dispatch ID and initializes an array of `VARIANTARG` structures for use in subsequent `Invoke` calls.

CONNECTS TO: `TCLib.h`, `InvokeArgs.h`

NOTES: None.

## `src/test/TCLib/InvokeArgs.h`

PURPOSE: The file `InvokeArgs.h` declares the class `TCInvokeArgs`, which is used to manage and invoke method calls through COM's `IDispatch::Invoke` interface.

KEY PARTS:
- **Class: TCInvokeArgs**
  - Manages arguments for COM method invocations.
  - Provides methods to set dispatch parameters and invoke methods on an IDispatch object.
  
- **Function: SetDispParams**
  - Sets the dispatch ID and parameters for subsequent Invoke calls using a variable argument list.

- **Function: SetDispParamsV**
  - Similar to `SetDispParams`, but takes a va_list directly instead of a variable argument list.

- **Function: Invoke**
  - Invokes a method on an IDispatch object with the current dispatch parameters.

- **Function: Empty**
  - Resets the instance by clearing the dispatch parameters and releasing any allocated memory.

CONNECTS TO:
- `IDispatch::Invoke` (COM interface)

NOTES:
- None

## `src/test/TCLib/Lockable.h`

PURPOSE: This file defines a class `TCLockable` that provides thread-safe locking mechanisms using critical sections.

KEY PARTS:
- `TCLockable`: A class designed to manage lock operations.
  - `Lock()`: Method to acquire the lock.
  - `Unlock()`: Method to release the lock.
- `XLock`: A type alias for `TCObjectLock<TCLockable>`, which is used internally for locking operations.
- `m_cs`: A member variable of type `TCAutoCriticalSection` that handles the critical section.

CONNECTS TO: Depends on `AutoCriticalSection.h` and `ObjectLock.h`.

NOTES: None.

## `src/test/TCLib/LookupTable.h`

PURPOSE: This file defines a template structure and macros for creating and using static lookup tables in C++ classes.

KEY PARTS:
- TCLookupTablePair: Template struct holding key-value pairs.
- TCLookupTable_Find: Function to find a value by key in a lookup table.
- TCLookupTable_DECLARE: Macro to declare a static lookup table as a class member.
- TCLookupTable_BEGIN, TCLookupTable_ENTRY, TCLookupTable_END: Macros for initializing the lookup table.

CONNECTS TO: None

NOTES: None

## `src/test/TCLib/NetApi.h`

PURPOSE:
This header file defines utility classes and macros for managing Windows Network API resources in a safe manner.

KEY PARTS:
- TCLsaHandle: A custom smart pointer class that manages LSA_HANDLE objects using the LsaClose function.
- TCNetApiPtr: A custom smart pointer class that manages NetApiBuffer objects using the NetApiBufferFree function.

CONNECTS TO:
lmerr.h, lmcons.h, lmapibuf.h, lmaccess.h, lmserver.h, ntsecapi.h, AutoHandle.h

NOTES:
None.

## `src/test/TCLib/ObjectLock.h`

PURPOSE:
The file `ObjectLock.h` defines a template class `TCObjectLock` that provides a convenient mechanism for locking and unlocking protected resources. It ensures that the lock is automatically released when the object goes out of scope.

KEY PARTS:
- **Class: TCObjectLock**
  - Manages the locking and unlocking of an object.
  - Automatically locks the object in its constructor and unlocks it in its destructor.
  - Supports optional tracing of lock operations based on a registry setting.
  
- **Function: TCObjectLock_EnableTraceLocks**
  - Determines if tracing for a specific class is enabled by reading a registry value.

CONNECTS TO:
- `AutoHandle.h`: For handling auto-releasing resources.
- `TCLib.h`: General library header.
- `AtlConv.h`: For converting between ANSI and Unicode strings.
- Registry: To read settings that control lock tracing.

NOTES:
- The trace locking functionality is currently disabled due to the `_ENABLE_TRACE_LOCKS` directive not being defined.

## `src/test/TCLib/PropertyPageSite.cpp`

**PURPOSE:** The `TCPropertyPageSite.cpp` file contains the implementation of the `TCPropertyPageSite` class, which manages an IPropertyPage interface. It provides methods for creating, attaching, detaching, and manipulating property pages within a larger application context.

**KEY PARTS:**
- **Class: TCPropertyPageSite**
  - Manages an IPropertyPage interface.
  - Provides methods for creating, attaching, detaching, and manipulating property pages.
  
- **Function: Page_Create(REFCLSID clsidPage)**
  - Creates a new instance of the specified property page and attaches it to this site object.

- **Function: Page_Attach(IPropertyPage* pPage)**
  - Attaches an existing IPropertyPage interface to this site object.

- **Function: Page_Detach()**
  - Detaches the managed property page, releasing resources and deactivating the page if it's active.
  
- **Function: SetObjects(ULONG cObjects, IUnknown** ppUnk)**
  - Sets the objects that the property page will edit.
  
- **Function: OnPageSiteWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* plResult)**
  - Handles window messages to manage the property page's visibility and activation state.

- **Function: Page_Show()**
  - Displays the managed property page by activating it if necessary and then calling the IPropertyPage::Show method.
  
- **Function: Page_Hide()**
  - Hides the managed property page by calling the IPropertyPage::Show method with the SW_HIDE parameter.
  
- **Function: Page_Help()**
  - Displays help for the managed property page using either the property page's Help method or the WinHelp API if that fails.

- **Function: Page_TranslateAccelerator(MSG* pMsg)**
  - Delegates keystroke messages to the IPropertyPage::TranslateAccelerator method of the managed property page.
  
- **TODO: Should this also set the managed property page's site to NULL?**
  - A comment indicating a potential enhancement or bug in the Page_Detach method.

**CONNECTS TO:** Other systems or files it depends on or serves, one line:
- Depends on the IPropertyPage interface and related COM components.

**NOTES:**
- The `OnGetWindow` function is currently always returning NULL and has a TODO suggesting it should be pure virtual.
- There are TODOs to ensure `OnGetWindow` is overridden in derived classes and to consider implementing a proper container interface if needed.

## `src/test/TCLib/PropertyPageSite.h`

PURPOSE: Defines a framework-neutral implementation of a property page site in Allegiance, which is an object containing a property page that implements the IPropertyPage interface.

KEY PARTS:
- TCPropertyPageSite: Main class representing a property page site.
- XPageSite: Nested class implementing IPropertyPageSite to handle COM interface methods.
- Page_Create: Creates a new property page instance.
- Page_Attach: Attaches an existing property page instance.
- Page_Detach: Detaches the current property page instance.
- GetTitle, GetDocString, etc.: Getter methods for various properties of the property page.
- SetDirty: Flags the property page as dirty or clean.
- Page_Apply: Applies changes made to the objects being viewed by the property page.

CONNECTS TO: IPropertyPage interface and related COM components.

NOTES: None.

## `src/test/TCLib/Range.h`

PURPOSE: Defines a template class `range<T>` that represents a range of values with lower and upper bounds, providing methods to check for intersections and emptiness.

KEY PARTS:
- Name: `range<T>`
  - What it does: Represents a range of values with lower and upper bounds, providing methods to check for intersections and emptiness.
- Name: `value_type`
  - What it does: Type alias for the type of elements in the range.
- Name: `value_compare`
  - What it does: Type alias for the comparison function used to compare elements.
- Name: `range()`
  - What it does: Default constructor initializing the range with default values and a default comparator.
- Name: `range(const value_type& t1, const value_type& t2, const value_compare& comp = value_compare())`
  - What it does: Constructor initializing the range with specified lower and upper bounds and an optional comparator.
- Name: `empty() const`
  - What it does: Checks if the range is empty (i.e., the lower bound is not less than the upper bound).
- Name: `intersects(const value_type& t) const`
  - What it does: Checks if a given value intersects with the range.
- Name: `intersects(const range<_Tp, _Compare>& that) const`
  - What it does: Checks if another range intersects with this range.

CONNECTS TO:
- `<algorithm>`: Used for swapping values in the constructor.

NOTES:
- none

## `src/test/TCLib/RangeSet.h`

PURPOSE: The file defines a template class `rangeset` that manages a collection of ranges, ensuring no overlapping ranges are stored and providing efficient range operations.

KEY PARTS:
- Name: `rangeset`
  - What it does: Manages a set of non-overlapping ranges using a multiset.
- Name: `rangeset_less`
  - What it does: Defines the comparison function for sorting ranges based on their upper bounds.
- Name: `insert`
  - What it does: Inserts a new range into the collection, merging with existing overlapping or adjacent ranges.
- Name: `erase`
  - What it does: Removes a specified range from the collection, splitting intersecting ranges if necessary.
- Name: `find`
  - What it does: Finds the first range that intersects with a given range.
- Name: `count`
  - What it does: Counts the number of ranges that intersect with a given range.
- Name: `lower_bound` and `upper_bound`
  - What they do: Find the lower and upper bounds for a given value or range within the collection.

CONNECTS TO:
- Depends on STL containers and algorithms, specifically `std::multiset`.

NOTES:
- None.

## `src/test/TCLib/RefCountedData.h`

PURPOSE: Defines a template class `TCRefCountedData` that provides reference counting functionality to any base class `T`.

KEY PARTS:
- Name: `TCRefCountedData`
  - What it does: Adds reference counting to the base class `T`, allowing objects of `T` to be shared and automatically cleaned up when no longer needed.

- Name: `AddRef`
  - What it does: Increments the reference count and returns the new count.

- Name: `Release`
  - What it does: Decrements the reference count, deletes the object if the count reaches zero, and returns the new count.

CONNECTS TO:
- Depends on no other specific files or systems; intended to be used with any class that can inherit from it.

NOTES:
- None.

## `src/test/TCLib/RelPath.cpp`

PURPOSE: This file contains implementation for creating relative path names from absolute paths using COM monikers.

KEY PARTS:
- TCMakeRelativePath: Function to create a relative path name.
- _tfullpath: Converts a pathname to an absolute pathname.
- TCReplaceText: Replaces occurrences of one string with another in a given string.
- CreateFileMoniker: Creates a file moniker for the specified path.
- RelativePathTo: Computes the relative path from one moniker to another.
- GetDisplayName: Retrieves the display name of the moniker.

CONNECTS TO: TCLib.h, StrManip.h

NOTES: None.

## `src/test/TCLib/RelPath.h`

PURPOSE: tiny file (11 lines), shown in full rather than summarized.

    #ifndef __RelPath_h__
    #define __RelPath_h__
    /////////////////////////////////////////////////////////////////////////////
    // RelPath.h : Declarations of relative pathname functions.
    //
    /////////////////////////////////////////////////////////////////////////////
    // Relative Path Creation
    bool TCMakeRelativePath(LPCTSTR pszPath, LPCTSTR pszFrom, LPTSTR pszDest,
      int cchMaxDest, bool bBeginWithDot);
    /////////////////////////////////////////////////////////////////////////////
    #endif // !__RelPath_h__

## `src/test/TCLib/StrManip.cpp`

PURPOSE: This file contains implementations of string manipulation routines for replacing occurrences of a substring within another string. It provides functions to perform case-sensitive and case-insensitive replacements and includes MFC wrapper functions.

KEY PARTS:
- TCReplaceTextImpl: Core implementation function that performs the actual replacement logic.
- TCReplaceText: Wrapper function for case-sensitive replacements.
- TCReplaceTextNoCase: Wrapper function for case-insensitive replacements.
- CString overloads: MFC-specific wrappers to handle CString objects directly.

CONNECTS TO: None

NOTES: None

## `src/test/TCLib/StrManip.h`

PURPOSE: This header file contains utility functions for string manipulation in the Allegiance game.

KEY PARTS:
- TC_tcscpyn: Copies characters from a source string to a destination buffer, ensuring it is null-terminated.
- TCReplaceText: Replaces occurrences of a substring with another substring in a given string.
- TCReplaceTextNoCase: Case-insensitive version of TCReplaceText.

CONNECTS TO: Used by various parts of the game that require string manipulation.

NOTES: None.

## `src/test/TCLib/TCCAUUID.h`

PURPOSE: This file declares and implements a structure `TCCAUUID` that encapsulates a COM CAUUID, which is a counted array of UUIDs.

KEY PARTS:
- TCCAUUID: A structure derived from `tagCAUUID` to manage a counted array of UUIDs.
  - Constructor initializes the `cElems` and `pElems` members.
  - Destructor frees memory allocated for `pElems` using `CoTaskMemFree`.

CONNECTS TO: COM (Component Object Model) libraries.

NOTES: None.

## `src/test/TCLib/TCLib.cpp`

**PURPOSE:** The file `TCLib.cpp` contains utility functions and classes for managing component categories, formatting dates and times, handling streams, and performing various object operations.

**KEY PARTS:**
- **Class:** `StdComponentCategoriesMgr`
  - Manages the registration and unregistration of component categories.
- **Function:** `RegisterComponentCategory(REFGUID catid, LPCOLESTR pszDesc)`
  - Registers a component category with a given GUID and description.
- **Function:** `UnregisterComponentCategory(REFGUID catid)`
  - Unregisters a component category using its GUID.
- **Function:** `GetVariantDateTime()`
  - Retrieves the current date and time in UTC as a VariantTime.
- **Function:** `FormatDateTime(SYSTEMTIME* psystime, LPTSTR psz, int nLength)`
  - Formats a SYSTEMTIME structure into a localized date and time string.
- **Function:** `FormatVariantDateTimeAsLocal(DATE date, LPTSTR psz, int nLength)`
  - Converts a VariantTime to a localized SYSTEMTIME and formats it as a date and time string.
- **Function:** `SaveObjectToNewStream`
  - Saves an object to a new memory stream using its IPersistStream interface.
- **Function:** `LoadObjectFromStream`
  - Loads an object from a stream using its IPersistStream interface.
- **Function:** `CopyObjectThruStream`
  - Copies the state of one object to another by serializing and deserializing through streams.

**CONNECTS TO:**
- Other components that require date and time formatting, component category management, or object serialization/deserialization.

**NOTES:**
- The file includes a TODO for handling cases where the category manager might be missing.
- Uses a custom macro `_VERIFYE` for asserting conditions and logging errors.

## `src/test/TCLib/TCThread.cpp`

PURPOSE: This file contains the implementation of the TCThread class, which is used to create and manage threads in the Allegiance game.

KEY PARTS:
- TCThread: Class representing a thread.
  - Constructor: Initializes the thread parameters.
  - Destructor: Cleans up resources.
  - BeginThread: Static method to start a new thread with optional message queue support.
  - BeginMsgThread: Similar to BeginThread but sets up a message queue for the thread.
  - CreateThread: Internal method to create and configure the thread.
  - Delete: Deletes the thread if it is set to auto-delete.
  - EndThread: Ends the thread execution and optionally deletes the object.
  - TCThreadEntry: Entry point for non-message queue threads.
  - TCMsgThreadEntry: Entry point for message queue threads.

CONNECTS TO: TCLib.h, Windows API functions like CreateEvent, _beginthreadex, SetEvent, WaitForSingleObject, etc.

NOTES: None.

## `src/test/TCLib/TCThread.h`

PURPOSE: This file defines a class `TCThread` for managing threads in Allegiance, providing functionality similar to `CWinThread` but without MFC dependencies.

KEY PARTS:
- TCThread: Class representing a thread with methods to start, manage, and clean up the thread.
- BeginThread: Static method to create and start a new thread.
- GetThreadPriority, SetThreadPriority: Methods to get and set the priority of the thread.
- SuspendThread, ResumeThread: Methods to suspend and resume the thread execution.
- PostThreadMessage: Method to post messages to the thread's message queue.
- SignalExitAndWait: Method to signal the thread to exit and wait for it to finish.

CONNECTS TO: `AutoHandle.h` for handle management, `zassert.h` for assertions.

NOTES: None.

## `src/test/TCLib/TraceScope.h`

PURPOSE: The file `TraceScope.h` provides a mechanism for tracing and debugging the execution flow of functions in a C++ application. It allows developers to automatically log entry and exit points of functions with optional custom messages.

KEY PARTS:
- TCTraceScope: A class that encapsulates the functionality of tracing function calls.
  - Constructor: Initializes the trace scope, logs the entry point of the function.
  - Destructor: Logs the exit point of the function.
  - TlsIndex: Retrieves or allocates a thread-local storage index used to track indentation levels.

CONNECTS TO:
- OutputDebugStringA: Used to output debug messages to the debugger's output window.

NOTES:
- The macros `_TSCOPE` through `_TSCOPE5` provide convenient ways to create instances of `TCTraceScope` with varying numbers of parameters.
- In release builds, the macros are defined as empty, effectively disabling tracing.

## `src/test/TCLib/UtilImpl.cpp`

PURPOSE: This file contains implementation of utility methods for managing COM objects and performing various operations such as creating object references, sleeping, and getting objects from monikers.

KEY PARTS:
- TCUtilImpl: Class implementing utility methods.
  - CreateObject: Creates an instance of a COM object using ProgID.
  - ObjectReference: Generates a string representation of the object reference.
  - Sleep: Pauses execution for a specified duration.
  - GetObject: Retrieves a COM object from a moniker.

CONNECTS TO:
- objbase.h and related Windows headers for COM operations.
- AutoHandle.h for smart pointer management.
- TCLib.h for interface definitions.

NOTES:
- The `CreateObjrefMoniker` function is not part of the public Visual C++ API but is used internally.
- The `Sleep` method handles tick count wrapping to avoid potential issues with 49-day wraparound.
- The `GetObject` method uses `CoGetObject` for binding monikers, which involves parsing and resolving moniker names.

## `src/test/TCLib/UtilImpl.h`

PURPOSE: This header file declares the implementation of utility functions for managing COM objects and performing system operations.

KEY PARTS:
- TCUtilImpl: Class containing static methods for creating COM objects, getting object references, sleeping, and retrieving objects by moniker.
  - CreateObject: Creates a COM object using a ProgID.
  - ObjectReference: Retrieves the reference string of a COM object.
  - Sleep: Pauses execution for a specified duration in milliseconds.
  - GetObject: Retrieves a COM object by its moniker.

CONNECTS TO: COM objects and system operations.

NOTES: None.

## `src/test/TCLib/UtilityThread.cpp`

PURPOSE: This file contains the implementation of a utility thread class in Allegiance, which manages a queue of work items to be processed by a shared thread.

KEY PARTS:
- TCUtilityThread: Manages a shared utility thread that processes queued work items.
- XWorkItem: Represents an element of work with arguments and a callback for cleanup.
- PostMessage/PostMessageV/PostMessageEx: Methods to post work items to the utility thread queue.
- ThreadProc: The main procedure of the utility thread, which services the message queue.
- DispatchWorkItem: Processes a queued work item by calling the derived class's OnMessage method.

CONNECTS TO: TCThread (for thread management), TCUtilityThread-derived classes (for specific work implementations).

NOTES:
- Uses COM for reference counting and threading.
- Contains TODOs for adding timed messages and ensuring 'last of type' processing.
- Includes error handling to catch exceptions during message dispatch.

## `src/test/TCLib/UtilityThread.h`

**PURPOSE:**  
The file `UtilityThread.h` defines a base class `TCUtilityThread` for performing background tasks in a separate thread. It allows derived classes to queue work items and specify how those items should be processed and cleaned up.

**KEY PARTS:**
- **Class: TCUtilityThread**
  - Manages a single utility thread.
  - Provides methods to post messages (work items) to the thread.
  - Defines pure virtual functions for derived classes to implement specific work handling and resource cleanup.
  
- **Function: PostMessage**
  - Queues a message with parameters to be processed by the utility thread.

- **Function: OnGetArgRelProc**
  - Returns a callback function pointer that will be called when a queued item is completed, allowing for resource cleanup.

- **Function: OnMessage**
  - Handles the processing of messages posted to the utility thread.

**CONNECTS TO:**  
`TCThread.h`, which provides the underlying threading implementation.

**NOTES:**  
- The class uses `WM_APP` as a base message identifier.
- There is a TODO comment suggesting that much of this functionality could be useful outside the singleton scenario.

## `src/test/TCLib/WinApp.h`

PURPOSE: This file defines a custom application class for the Allegiance game, extending the base `Win32App` class. It includes functionality for debugging and error reporting.

KEY PARTS:
- TCWinApp: A debug version of the application class that sets up detailed error reporting to both the debugger and standard output.
  - DebugOutput: Overrides the default method to prepend the module name to debug messages before reporting them.
  - OnAssert: Handles assertion failures by reporting them through `_CrtDbgReport` and optionally breaking into the debugger.
- TCWinAppDLL: A non-debug version of the application class that disables error reporting to standard output.

CONNECTS TO: `TCLib.h`, `Win32App`

NOTES:
- The debug version sets up a report hook function (`ReportHook`) to filter out certain types of messages.
- There are conditional compilation directives for `_DEBUG` mode, which affects the behavior of the application class.

## `src/test/TCLib/WindowThreadBase.h`

**PURPOSE:** The file `WindowThreadBase.h` defines a base class for managing window creation and destruction in a multi-threaded environment. It provides methods to create and destroy windows on a separate thread, handle message loops, and manage accelerator tables.

**KEY PARTS:**
- **Class: TCWindowThreadBase**
  - Manages the creation and destruction of windows.
  - Handles message processing and translation for dialogs and accelerators.
  
- **Function: CreateWindowOnThread**
  - Creates a window on a separate thread.
  
- **Function: DestroyWindowOnThread**
  - Destroys a window on a separate thread.
  
- **Function: MessageLoop**
  - Processes messages in the message queue.
  
- **Function: ProcessThreadMessage**
  - Handles specific thread messages for window creation and destruction.

**CONNECTS TO:** `TCWindowDC`, `TCThread`, `TCAutoCriticalSection`, `AutoHandle`

**NOTES:** None.

## `src/test/TCLib/WindowThreadImpl.h`

PURPOSE: This file defines a template class `TCWindowThreadImpl` that implements a thread with a message loop capable of creating and managing windows owned by the thread.

KEY PARTS:
- `TCWindowThreadImpl`: A template class derived from `TCWindowThreadBase`, providing functionality to create and destroy windows within its own thread.
- `GetOrCreateThread()`: Creates or retrieves an existing thread for window management.
- `InternalCreateWindow()`: Handles the creation of a window on the thread.
- `InternalDestroyWindow()`: Handles the destruction of a window on the thread.
- `ThreadProc()`: The entry point for the thread, initializes the message loop and services messages.

CONNECTS TO: `TCWindowThreadBase`, `TCThread`, `XArgs`, `CLock`.

NOTES: None.

## `src/test/TCLib/WorkerThread.cpp`

PURPOSE: This file contains the implementation of the `TCWorkerThread` class, which manages a worker thread that processes queued work items.

KEY PARTS:
- `XWorkItem`: Manages an individual work item, including its parameters and cleanup callback.
- `PostMessage`, `PostMessageV`, `PostMessageEx`: Methods to post work items to the queue.
- `ThreadProc`: The main procedure of the worker thread that processes queued work items.
- `DispatchWorkItem` and `DestroyWorkItem`: Helper methods for processing and cleaning up work items.

CONNECTS TO: `TCThread.h` (for threading utilities), `TCLib.h` (header file for the library).

NOTES:
- Contains trace logging for debugging purposes.
- Includes TODOs for adding timed posting and 'last of type' functionality.

## `src/test/TCLib/WorkerThread.h`

PURPOSE: The file defines a base class `TCWorkerThread` for performing work in a separate worker thread. It allows derived classes to queue and process tasks asynchronously.

KEY PARTS:
- `TCWorkerThread`: Base class for worker threads.
- `XWorkItem`: Represents a work item queued for processing.
- `PostMessage`, `PostMessageV`, `PostMessageEx`: Methods to queue work items.
- `OnGetUnknown`: Pure virtual method to return an IUnknown pointer if the derived class is a COM object.
- `OnGetWorkItemRelProc`: Pure virtual method to return a callback function for releasing resources associated with queued arguments.
- `OnMessage`: Pure virtual method to process queued work items.

CONNECTS TO: Depends on other classes like `TCThread`, `AutoHandle`, and `AutoCriticalSection`.

NOTES: None.

## `src/test/TCLib/WotLib.h`

PURPOSE: The file `WotLib.h` provides a mechanism for calling functions from a DLL through a window's message thread, encapsulating the use of exported DLL functions.

KEY PARTS:
- TCWotLib: A class to manage calling DLL functions via SendMessage.
- SetWnd: Method to set the window that will receive function msg requests.
- TCWOTLIB_HANDLER: Macro for defining message handlers for the window.
- TCWotCall, TCWotCall_void, TCWotPreCall, TCWotPreCall_void: Macros to call DLL functions through SendMessage.
- TCWotLib_Fn0 to TCWotLib_Fn12: Macros to declare non-prefixed functions with return-values.
- TCWotLib_Fn0_void to TCWotLib_Fn12_void: Macros to declare non-prefixed void functions.
- TCWotLib_PreFn0 to TCWotLib_PreFn12: Macros to declare prefixed functions with return-values.
- TCWotLib_PreFn0_void to TCWotLib_PreFn12_void: Macros to declare prefixed void functions.

CONNECTS TO: `TCLib` system, likely depends on a DLL for function calls.

NOTES: None.

## `src/test/TCLib/pch.cpp`

PURPOSE: tiny file (4 lines), shown in full rather than summarized.

    // pch.cpp : source file that includes just the standard includes
    //  TCLib.pch will be the pre-compiled header
    //  pch.obj will contain the pre-compiled type information
    #include "pch.h"

## `src/test/TCLib/pch.h`

PURPOSE: This header file is a precompiled header for the Allegiance game's TCLib project, providing common includes and definitions used across multiple source files.

KEY PARTS:
- `WIN32_LEAN_AND_MEAN`: Excludes rarely-used Windows headers to reduce compile time.
- `_WIN32_DCOM`: Enables support for Distributed Component Object Model (DCOM).
- `__MODULE__ "TCLib"`: Defines the module name as "TCLib".
- `#pragma warning(disable: 4786)`: Disables warnings related to long identifiers in debug builds.
- `<stdexcept>`, `<algorithm>`, `<string>`, `<vector>`, `<map>`: Standard C++ library includes for common data structures and utilities.
- `<crtdbg.h>`: Header for debugging the C runtime library.
- `<windows.h>`, `<tchar.h>`, `<malloc.h>`, `<comcat.h>`, `<zassert.h>`, `<comdef.h>`, `<commctrl.h>`, `<atlconv.h>`, `<process.h>`, `<mmsystem.h>`: Various Windows and ATL headers for system functionality.
- `<..\zlib\zlib.h>`: Header for the ZLib compression library.

CONNECTS TO: Other source files within the TCLib project that include this precompiled header.

NOTES: None.

## `src/test/TCLib/tstring.h`

PURPOSE: This file defines custom string comparison functions for case-insensitive sorting of `tstring` and `_bstr_t` types.

KEY PARTS:
- `ci_less`: A struct that provides a case-insensitive less-than comparison function for `tstring`.
- `ci_greater`: A negated version of `ci_less`, providing a greater-than comparison.
- `ci_less_bstr_t`: A struct that provides a case-insensitive less-than comparison function for `_bstr_t`.
- `ci_greater_bstr_t`: A negated version of `ci_less_bstr_t`, providing a greater-than comparison.

CONNECTS TO: Used in sorting operations where case-insensitivity is required, such as in collections or data structures that rely on comparisons.

NOTES: None.
