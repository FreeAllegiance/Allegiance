# src/test/TCAtl

[Back to the index](../LLAMA_SUMMARY.md) - 33 files, 17,100 lines. Notes by a local model, to orient by: not verified facts.

This folder contains source code related to COM (Component Object Model) functionality and Windows-based UI components in the Allegiance game. It includes classes for managing COM modules, handling combo boxes, dual event calls, error formatting, property pages, persistence, and user accounts.

- ComModule.h: Manages COM module functionality including command-line parsing, error reporting, security initialization, and shutdown handling.
- ComboBoxWindow.h: Defines classes and functions for handling combo box controls in Windows applications.
- PropertyPageBase.cpp: Implements a base class for property pages in Allegiance, providing common functionality and interfaces for managing property page components.
- PropertyClass.h: Provides a framework for property persistence and management using COM interfaces.
- PropertyPageImpl.h: Contains a template class for implementing property pages in a Microsoft application.

Largest files: ComboBoxWindow.h (2085 lines), PropertyPageBase.cpp (1568 lines), ComModule.h (1344 lines), PropertyPageImpl.h (1315 lines), PropertyClass.h (1108 lines).

## `src/test/TCAtl/ComModule.h`

PURPOSE: The file `ComModule.h` contains definitions and implementations for managing COM module functionality, including command-line parsing, error reporting, security initialization, and shutdown handling.

KEY PARTS:
- TCComModule: Template class for managing COM module functionality.
- XCmdLineOption: Structure defining command-line options and their handlers.
- ParseCommandLine: Parses a command line string or arguments.
- OnRegServer, OnUnregServer, OnNoExit, OnExit, OnHelp: Handlers for specific command-line options.
- ReportError, ReportSuccess: Functions to report errors and successes with descriptions.
- TCComModule<T>: Main class template for COM module functionality with methods like OnRegServer, OnUnregServer, etc.
- OpenAppIDRegKey: Opens the registry key associated with an application ID.
- InitializeSecurity, RegisterSecurity: Initializes and registers security settings.
- GetAccessSecurity, GetLaunchSecurity: Retrieves security descriptors for access and launch permissions.
- CreateShutdownEvent, ConsoleCtrlHandler: Handles shutdown events and console control signals.

CONNECTS TO: Other COM-related files and system libraries.

NOTES: None.

PARTS (in file order):

- lines 1-354
  - TCComModule: Template class for managing COM module functionality.
  - XCmdLineOption: Structure defining command-line options and their handlers.
  - ParseCommandLine(LPCTSTR pszCmdLine): Parses a command line string.
  - ParseCommandLine(int argc, TCHAR argv[]): Parses command-line arguments.
  - OnRegServer, OnUnregServer, OnNoExit, OnExit, OnHelp: Handlers for specific command-line options.
  - ReportError(HRESULT hr, LPCTSTR pszDesc): Reports an error with description.
  - ReportSuccess(LPCTSTR pszDesc): Reports a successful operation.
  - Echo functions: Output formatted text to the console or message window.
- lines 355-708
  - TCComModule<T>: Main class template for COM module functionality.
  - OnRegServer: Registers server components and security options.
  - OnUnregServer: Unregisters server components.
  - OnNoExit: Marks the module as permanent.
  - OnExit: Signals the shutdown event to exit the server.
  - OnHelp: Displays help syntax.
  - Syntax: Calls on derived class to display its syntax.
  - ReportError: Reports errors with system messages.
- lines 709-1061
  - OpenAppIDRegKey: Opens the registry key associated with an application ID.
  - InitializeSecurity: Initializes security settings for the module.
  - RegisterSecurity: Registers security permissions and authentication levels.
  - GetAccessSecurity: Retrieves a security descriptor for access permissions.
  - GetLaunchSecurity: Retrieves a security descriptor for launch permissions.
  - GetAuthenticationLevel: Retrieves the authentication level for the module.
  - OutputText: Outputs text to debug monitor and standard output.
  - parse_cmdline: Parses the command line into arguments.
- lines 1062-1344
  - CreateShutdownEvent: Creates a shutdown event with appropriate security settings.
  - ConsoleCtrlHandler: Handles console control events by signaling the server to exit.
  - MonitorThunk: Thunk function to delegate to the non-static MonitorProc method.
  - MonitorProc: Monitors for idle time and shuts down if no activity is detected.
  - BEGIN_CMDLINE_OPTION_MAP: Macro to define a command line option map.
  - CMDLINE_OPTION_ENTRY: Macro to add an entry to the command line option map.
  - CMDLINE_OPTION_ENTRY_EX: Extended macro for adding entries with additional parameters.
  - CMDLINE_OPTION_Automation: Macro for automation-related command line options.

## `src/test/TCAtl/ComboBoxWindow.h`

PURPOSE: The file `ComboBoxWindow.h` contains definitions for classes and functions related to handling combo box controls in a Windows application.

KEY PARTS:
- TCComboBoxWindow: Derivation of CWindowImpl that encapsulates a Windows combo box control.
- TCTypedComboBox: Template class derived from TCComboBoxWindow for accessing list item data as specified type I.
- TCSelfDrawComboBox: Template class derived from TCTypedComboBox, subclasses parent window to handle owner-draw messages.
- GetCount: Retrieves the number of items in the list box of a combo box.
- GetCurSel: Retrieves the index of the currently selected item in the list box of a combo box.
- SetCurSel: Selects an item in the list box of a combo box by its zero-based index.
- AddString: Adds a string to the list box of a combo box.
- DrawItem: Virtual function for drawing items in owner-draw mode.

CONNECTS TO: CWindowImpl, other Windows API functions

NOTES: None

PARTS (in file order):

- lines 1-343
  - TCComboBoxWindow: Derivation of CWindowImpl that encapsulates a Windows combo box control.
  - TCTypedComboBox: Template class derived from TCComboBoxWindow for accessing list item data as specified type I.
  - TCSelfDrawComboBox: Template class derived from TCTypedComboBox, subclasses parent window to handle owner-draw messages.
  - GetCount: Retrieves the number of items in the list box of a combo box.
  - GetCurSel: Retrieves the index of the currently selected item in the list box of a combo box.
  - SetCurSel: Selects an item in the list box of a combo box by its zero-based index.
  - AddString: Adds a string to the list box of a combo box.
  - DrawItem: Virtual function for drawing items in owner-draw mode.
- lines 344-657
  - TCComboBoxWindow::SetCurSel: Selects a string in the list box of a combo box.
  - TCComboBoxWindow::GetCurSelRaw: Retrieves the application-supplied DWORD value associated with the selected combo-box item.
  - TCComboBoxWindow::SetCurSelRaw: Sets the DWORD (32-bit) value associated with the selected item in a combo box.
  - TCComboBoxWindow::GetLocale: Retrieves the locale identifier for a combo box.
  - TCComboBoxWindow::SetLocale: Sets the locale identifier for a combo box.
  - TCComboBoxWindow::GetTopIndex: Returns the index of the first visible item in the list-box portion of the combo box.
  - TCComboBoxWindow::SetTopIndex: Tells the list-box portion of the combo box to display the item with the specified index at the top.
  - TCComboBoxWindow::InitStorage: Preallocates blocks of memory for items and strings in the list-box portion of the combo box.
- lines 658-976
  - TCComboBoxWindow: Class representing a combo box window.
  - SetEditSel: Sets the selection range of text in the edit control of a combo box.
  - LimitText: Limits the length of text that can be entered into the edit control of a combo box.
  - GetItemDataRaw: Retrieves the application-supplied DWORD value associated with a specified item in the combo box.
  - SetItemDataRaw: Sets the application-supplied DWORD value for a specified item in the combo box.
  - GetLBText: Retrieves a string from the list box of a combo box.
  - GetLBTextLen: Gets the length of a string in the list box of a combo box.
  - SetItemHeight: Sets the height of list items or the edit-control/static-text portion of a combo box.
- lines 977-1286
  - TCComboBoxWindow::ShowDropDown: Shows or hides the drop-down list box of a combo box.
  - TCComboBoxWindow::AddString: Adds a string to the list box of a combo box.
  - TCComboBoxWindow::AddStringItemRaw: Adds a string and associates a DWORD value with it in the list box of a combo box.
  - TCComboBoxWindow::DeleteString: Deletes a string from the list box of a combo box.
  - TCComboBoxWindow::InsertString: Inserts a string into the list box of a combo box at a specified position.
  - TCComboBoxWindow::InsertStringItemRaw: Inserts a string and associates a DWORD value with it into the list box of a combo box at a specified position.
  - TCComboBoxWindow::ResetContent: Removes all items from the list box and edit control of a combo box.
  - TCComboBoxWindow::Dir: Adds a list of filenames to the list box of a combo box based on specified attributes and wildcards.
- lines 1287-1611
  - TCComboBoxWindow::FindStringExact: Finds an exact string match in a combo box's list.
  - TCComboBoxWindow::FindItemDataRaw: Searches for an item by its associated DWORD value.
  - TCComboBoxWindow::SelectItemDataRaw: Selects an item by its associated DWORD value and updates the edit control.
  - TCComboBoxWindow::Clear: Deletes the current selection from the edit control.
  - TCComboBoxWindow::Copy: Copies the current selection to the clipboard in CF_TEXT format.
  - TCComboBoxWindow::Cut: Cuts the current selection to the clipboard in CF_TEXT format.
  - TCComboBoxWindow::Paste: Pastes text from the clipboard into the edit control at the cursor position.
  - TCTypedComboBox<I>::GetItemData: Retrieves a typed value associated with an item.
- lines 1612-1941
  - TCTypedComboBox<I>: A template class for a typed combo box window.
  - AddStringItem(LPCTSTR pszString, I itemData): Adds a string to the list box and associates it with a typed value.
  - InsertStringItem(int nIndex, LPCTSTR pszString, I itemData): Inserts a string into the list box at a specified position and associates it with a typed value.
  - FindItemData(int nIndexStart, I itemData) const: Finds the first list-box item that matches a given typed value.
  - SelectItemData(int nIndexStart, I itemData): Searches for an item by its typed data and selects it if found.
  - operator=(HWND hwnd): Subclasses the specified window.
  - TCSelfDrawComboBox<T, I>: A template class for a self-draw combo box.
  - SubclassWindow(HWND hWnd): Subclasses a combo box window and attaches it to the CWindowImpl object.
- lines 1942-2085
  - TCSelfDrawComboBox<T, I>: A template class for handling owner-draw ComboBoxes.
  - OnCreate: Handles the WM_CREATE message by subclassing the parent window.
  - OnDrawItem: Delegates drawing of items to the most derived class.
  - OnMeasureItem: Delegates measurement of items to the most derived class.
  - OnCompareItem: Delegates comparison of items to the most derived class.
  - OnDeleteItem: Delegates deletion of items to the most derived class.
  - TODOs or unusual code: None identified in this snippet.

## `src/test/TCAtl/DualEventCall.h`

PURPOSE: This file defines a template class `TCComDualEventCall` used for invoking methods on IDispatch-based connection point sinks in COM objects.

KEY PARTS:
- Class: `TCComDualEventCall<If_vtbl>`
  - Maintains the set of arguments passed to an IDispatch-based connection point sink.
  - Provides a function call operator that calls `Invoke` on each connected IDispatch pointer with specified arguments.
- Function: `SetDispParams`
  - Sets the dispatch ID and dispatch parameters for subsequent `Invoke` calls using a variable argument list.
- Function: `SetDispParamsV`
  - Similar to `SetDispParams`, but takes a va_list directly.

CONNECTS TO:
- "EventCall.h" (for base class `TCComEventCall<If_vtbl>`)

NOTES:
- None.

## `src/test/TCAtl/DualEventsCP.h`

**PURPOSE:** The file `TCAtl/DualEventsCP.h` contains the implementation of a template class `TCComDualEventsCP` that manages event sinks for both IDispatch and v-table interfaces in a Microsoft C++ application. It provides functionality to fire events to all connected sinks and handles the management of these sinks.

**KEY PARTS:**
- **Class:** TCComDualEventsCP
  - Manages event sinks for both IDispatch and v-table interfaces.
  - Fires events to all connected sinks.
  
- **Function:** OnGetWorkItemRelProc
  - Returns the address of the static ReleaseArgs method, which handles the deletion of event call objects.

- **Function:** OnMessage
  - Processes messages related to firing events. Handles the e_idFireEvents message by calling FireEvents with the provided TCComDualEventCall object.
  
- **Function:** ReleaseArgs
  - Deletes the object derived from TCComDualEventCall when it is no longer needed.

- **Function:** FireEvents
  - Fires an event to all connected sinks. Copies vtable and IDispatch sinks, then notifies each sink of the event.
  
- **Function:** GetEventSinks
  - Retrieves the current list of event sinks (both v-table and IDispatch) and stores them in provided vectors.

- **Function:** RemoveFailedSink
  - Removes a failed event sink from the connection point if an event call fails.
  
- **Macros:**
  - BEGIN_TCComDualEventsCP, BEGIN_TCComDualEventsCP_IID, END_TCComDualEventsCP
    - Simplify the declaration of connection point classes with both IDispatch and v-table interfaces.
    
  - TCComDualEventCP_Fn0 through TCComDualEventCP_Fn12
    - Declare event firing functions for events with varying numbers of parameters.

**CONNECTS TO:** Other parts of the application that use event-driven communication between components.

**NOTES:**
- Macros like DECLARE_CTOR_1, DECLARE_CTOR_2, etc., are not fully expanded in this snippet and would typically be defined elsewhere.
- The file includes macros to handle different numbers of parameters for event firing functions (from 4 to 12).

## `src/test/TCAtl/EditWindow.h`

PURPOSE: The file `EditWindow.h` contains the definition and implementation of the `TCEditWindow` class, which encapsulates a Windows edit control and provides various operations for managing its content.

KEY PARTS:
- TCEditWindow: A class derived from CWindowImpl that manages an edit control.
- DECLARE_WND_SUPERCLASS: Macro to set the window superclass for TCEditWindow.
- BEGIN_MSG_MAP and END_MSG_MAP: Macros defining the message map for handling messages sent to TCEditWindow.
- CanUndo, GetLineCount, GetModify, SetModify, GetRect, GetSel, GetHandle, SetHandle, GetLine, EmptyUndoBuffer, FmtLines, LimitText, LineFromChar, LineIndex, LineLength, LineScroll, ReplaceSel, SetPasswordChar, SetRect, SetRectNP, SetSel, SetTabStops, SetTabStops(int), Undo: Member functions providing various operations and attributes for TCEditWindow.
- GetLine: Retrieves a specified line of text from an edit control.
- EmptyUndoBuffer: Resets the undo flag of an edit control.
- FmtLines: Sets the inclusion of soft line-break characters in a multiple-line edit control.
- LimitText: Limits the length of text that can be entered into an edit control.
- LineFromChar: Retrieves the line number containing a specified character index.
- LineIndex: Retrieves the character index of a specified line in a multiple-line edit control.
- LineLength: Retrieves the length of a line in an edit control.
- SetRect: Sets the formatting rectangle of a multiple-line edit control.
- SetRectNP: Sets the formatting rectangle without redrawing the control window.
- SetSel: Selects a range of characters in an edit control.
- SetTabStops(int nTabStops, LPINT rgTabStops): Sets tab stops for a multiple-line edit control using an array of dialog units.
- SetTabStops(): Sets default tab stops to 32 dialog units.
- SetTabStops(const int& cxEachStop): Sets tab stops at every specified number of dialog units.
- Undo: Reverses the last edit-control operation.

CONNECTS TO: CWindowImpl

NOTES: None.

PARTS (in file order):

- lines 1-320
  - TCEditWindow: A class derived from CWindowImpl that encapsulates a Windows edit control.
  - DECLARE_WND_SUPERCLASS: Macro to set the window superclass for TCEditWindow.
  - BEGIN_MSG_MAP and END_MSG_MAP: Macros defining the message map for handling messages sent to TCEditWindow.
  - CanUndo, GetLineCount, GetModify, SetModify, GetRect, GetSel, GetHandle, SetHandle, GetLine, EmptyUndoBuffer, FmtLines, LimitText, LineFromChar, LineIndex, LineLength, LineScroll, ReplaceSel, SetPasswordChar, SetRect, SetRectNP, SetSel, SetTabStops, SetTabStops(int), Undo: Member functions providing various operations and attributes for TCEditWindow.
  - CanUndo(): Determines if the last edit operation can be undone.
  - GetLineCount(): Retrieves the number of lines in a multiple-line edit control.
  - GetModify(): Checks if the contents of an edit control have been modified.
  - SetModify(BOOL): Sets or clears the modification flag for an edit control.
- lines 321-635
  - TCEditWindow::GetLine: Retrieves a specified line of text from an edit control.
  - TCEditWindow::GetLine(int nIndex, LPTSTR pszBuffer, int nMaxLength): Retrieves a specified line of text into a buffer with a maximum length.
  - TCEditWindow::EmptyUndoBuffer: Resets the undo flag of an edit control.
  - TCEditWindow::FmtLines: Sets the inclusion of soft line-break characters in a multiple-line edit control.
  - TCEditWindow::LimitText: Limits the length of text that can be entered into an edit control.
  - TCEditWindow::LineFromChar: Retrieves the line number containing a specified character index.
  - TCEditWindow::LineIndex: Retrieves the character index of a specified line in a multiple-line edit control.
  - TCEditWindow::LineLength: Retrieves the length of a line in an edit control.
- lines 636-951
  - TCEditWindow::SetRect: Sets the formatting rectangle of a multiple-line edit control.
  - TCEditWindow::SetRectNP: Sets the formatting rectangle without redrawing the control window.
  - TCEditWindow::SetSel: Selects a range of characters in an edit control.
  - TCEditWindow::SetTabStops(int nTabStops, LPINT rgTabStops): Sets tab stops for a multiple-line edit control using an array of dialog units.
  - TCEditWindow::SetTabStops(): Sets default tab stops to 32 dialog units.
  - TCEditWindow::SetTabStops(const int& cxEachStop): Sets tab stops at every specified number of dialog units.
  - TCEditWindow::Undo: Reverses the last edit-control operation.
  - TCEditWindow::Clear: Deletes (clears) the current selection in the edit control.
- lines 952-1081
  - TCEditWindow: A class representing an edit window control.
  - GetMargins: Returns the left and right margins of the edit window.
  - SetLimitText: Sets the maximum number of characters that can be entered into the edit window.
  - GetLimitText: Retrieves the current limit on the number of characters in the edit window.
  - PosFromChar: Gets the coordinates of a specified character index within the edit window.
  - CharFromPos: Retrieves the line and character indices for the character closest to a specified position.
  - operator=: Subclasses an existing window with this TCEditWindow object.

## `src/test/TCAtl/ErrorFormatter.h`

PURPOSE: This file defines a template class `TCErrorFormatter` and its base class `TCErrorFormatterBase` for formatting error messages in the Allegiance game. The classes provide methods to format errors into strings or BSTRs using resource IDs.

KEY PARTS:
- TCErrorFormatterBase: Provides static methods to load, format, and report error messages.
  - FormattedErrorV: Formats an error message from a resource string and reports it.
  - FormattedBSTRV: Formats an error message into a BSTR from a resource string.
- TCErrorFormatter<T, const IID*>: A template class derived from `TCErrorFormatterBase` that provides methods to format errors for specific COM objects.
  - FormattedError: Formats an error message and reports it using the object's CLSID.
  - FormattedErrorIID: Formats an error message and reports it using a specified interface ID.
  - FormattedBSTR: Formats an error message into a BSTR.

CONNECTS TO:
- Allegiance game resources (string tables).
- COM interfaces for error reporting.

NOTES:
- None.

## `src/test/TCAtl/EventCall.h`

**PURPOSE:**  
This file contains a template class `TCComEventCall` used in the connection point sinks of COM-based event handling. It facilitates calling events on connected interfaces and manages error information.

**KEY PARTS:**
- **Class: TCComEventCall<If_vtbl>**
  - Manages the call to an event sink method.
  - Maintains a set of arguments passed to the v-table connection point sink.
  - Provides methods to set and get the name of the event for diagnostic purposes.

**CONNECTS TO:**  
- `TCComEventCP_Fn` macros: Used internally by these macros to manage event calls.
- `CComPtr<IErrorInfo>`: Manages error information for thread diagnostics.

**NOTES:**  
- The class is designed to be used internally by the `TCComEventCP_Fn` macros and is unlikely to be used directly.
- Contains debug tracing and error handling mechanisms.

## `src/test/TCAtl/EventsCP.h`

PURPOSE: The file contains definitions and macros for handling events in a COM-based connection point system.

KEY PARTS:
- TCComEventsCP: Base class for managing event sinks connected to a COM interface.
- BEGIN_TCComEventsCP, END_TCComEventsCP: Macros for declaring classes that inherit from TCComEventsCP.
- TCComEventCP_Fn0-TCComEventCP_Fn10: Macros for defining event functions with varying numbers of parameters.
- FireEvents: Method to fire an event to all connected sinks.
- GetEventSinks: Method to copy the derived-class's event sinks to a vector.
- RemoveFailedSink: Method to remove an event sink if an event call failed.
- OnGetUnknown: Override method returning the IUnknown of this connection point.

CONNECTS TO: COM interfaces and sinks.

NOTES:
- Receiving results or [out] parameters from connected sinks is not supported.

## `src/test/TCAtl/ExternalConnectionImpl.h`

PURPOSE: This file defines a template class `IExternalConnectionImpl` that implements the `IExternalConnection` interface for COM objects. It manages external connections and reference counting.

KEY PARTS:
- Name: `IExternalConnectionImpl`
  - What it does: Manages external connections and reference counting for COM objects.
- Name: `DoLastReleaseClosing`
  - What it does: Handles the last release of a connection, potentially disconnecting the object if configured to do so.
- Name: `DumpConnection`
  - What it does: Debugging function that logs connection information.
- Name: `QI_IExternalConnectionImpl`
  - What it does: Query interface implementation for the `IExternalConnection` interface.
- Name: `AddConnection`
  - What it does: Adds a connection to the object, incrementing the strong connection count if specified.
- Name: `ReleaseConnection`
  - What it does: Releases a connection from the object, decrementing the strong connection count and potentially disconnecting the object if this is the last release.

CONNECTS TO:
- Other COM objects that implement this interface.

NOTES:
- Contains debugging code that is only compiled in debug builds.
- Includes functionality to disable certain features via registry settings.

## `src/test/TCAtl/InsidePropPage.cpp`

PURPOSE:
This file contains the implementation of the `TCInsidePropPage` class, which is responsible for creating and managing an embedded property page as a child window within another window.

KEY PARTS:
- **Class: TCInsidePropPage**
  - Manages the creation and positioning of an embedded property page.
  
- **Function: Create**
  - Creates an embedded property page as a child window of a specified parent window, using an existing control for positioning.
  
- **Functions: OnStatusChange, OnGetLocaleID, OnGetPageContainer, OnTranslateAccelerator**
  - Delegates method calls to the `IPropertyPageSite` interface if set; otherwise, calls base class methods.

- **Function: OnIsModal**
  - Returns `FALSE`, indicating that an embedded property page is a modeless child control.
  
- **Function: OnGetWindow**
  - Returns the handle of the window managed by this class.

CONNECTS TO:
- Depends on `TCPropertyPageSite` and related interfaces for managing the property page.

NOTES:
- None.

## `src/test/TCAtl/InsidePropPage.h`

PURPOSE:
The file `InsidePropPage.h` declares the `TCInsidePropPage` class, which allows a property page to be embedded within another property page in Allegiance.

KEY PARTS:
- TCInsidePropPage: A class that enables embedding of one property page inside another.
- DECLARE_WND_SUPERCLASS: Declares the window superclass for the property page.
- BEGIN_MSG_MAP and END_MSG_MAP: Defines message map for handling messages.
- Create: Constructor method to initialize the property page with parent, site, position, and other parameters.
- OnStatusChange, OnGetLocaleID, OnGetPageContainer, OnTranslateAccelerator, OnIsModal, OnGetWindow: Override methods from base classes to handle various property page events.

CONNECTS TO:
TCPropertyPageSite, TCPropertyPageBase, TCPropertyPageImpl

NOTES:
None.

## `src/test/TCAtl/Message.txt`

PURPOSE: tiny file (2 lines), shown in full rather than summarized.

    TCAtl: This project has nothing to build.
    It only serves as a placeholder for the files it contains.

## `src/test/TCAtl/ObjectMap.h`

PURPOSE: This file defines macros to simplify the management of object maps in an ATL-based application. Object maps are used to associate CLSIDs with class factories and other registration functions.

KEY PARTS:
- `TC_OBJECT_EXTERN`: Declares external references for a class's object map entries.
- `TC_OBJECT_EXTERN_IMPL`: Defines the actual implementations for the external references declared by `TC_OBJECT_EXTERN`.
- `TC_OBJECT_ENTRY` and `TC_OBJECT_ENTRY_NON_CREATEABLE`: Macros to define entries in the object map, linking CLSIDs with class factory functions and other registration functions.
- `TC_OBJECT_ENTRY_STD` and `TC_OBJECT_ENTRY_STD_NON_CREATEABLE`: Simplified macros for classes whose names follow a standard naming convention.

CONNECTS TO: This file is used by other parts of the application that need to register or unregister COM objects, such as in DLLs or EXEs implementing COM components.

NOTES: None.

## `src/test/TCAtl/PageEntry.h`

PURPOSE: This file defines structures and macros used for managing embedded property pages within a larger property page in the Allegiance game. It includes definitions for `XInsidePageEntry` and `XInsidePage`, as well as macros for defining and accessing these entries.

KEY PARTS:
- `XInsidePageEntry`: Structures an entry in a table of embedded property pages, containing details like control ID, CLSID, visibility, size behavior, and apply behavior.
- `XInsidePage`: Derives from `TCInsidePropPage` to add a pointer to the `XInsidePageEntry`.
- Macros for defining and accessing the embedded property page table (`BEGIN_INSIDE_PAGE_MAP`, `INSIDE_PAGE_ENTRY`, etc.).

CONNECTS TO: TCPropertyPageImpl, TCInsidePropPage

NOTES:
- TODO: Suggests moving these structures and macros to a non-template class.
- Contains macros for managing property fields and their updates/apply handlers.

## `src/test/TCAtl/Persist.h`

**PURPOSE:** The file `src/test/TCAtl/Persist.h` contains definitions and implementations for classes and functions related to object persistence in the Allegiance game. It provides mechanisms for saving and loading objects using streams and property bags.

**KEY PARTS:**
- **Class: TCComPropertyClassRoot**
  - Implements interfaces for persisting objects using streams and property bags.
  
- **Function: TCPersistStreamInit_Load**
  - Loads object state from a stream, handling different data types.
  
- **Function: TCPersistStreamInit_Save**
  - Saves object state to a stream, handling different data types.
  
- **Function: TCPersistPropertyBag_Load**
  - Loads object state from a property bag, handling various data types.
  
- **Function: TCPersistPropertyBag_Save**
  - Saves object state to a property bag, considering dirty flags and save options.
  
- **Data Structure: ATL_PROPMAP_ENTRY**
  - Defines entries in the property map for persisting properties.
  
- **Class: TCPersistStreamInitImpl**
  - Extends ATL's `IPersistStreamInitImpl` class, providing implementations for persistence methods.
  
- **Function: GetSizeMax**
  - Calculates the size of the stream needed to save an object.
  
- **Class: TCPersistPropertyBagImpl**
  - Implements the `IPersistPropertyBag` interface, providing methods for persistence.

**CONNECTS TO:** Other parts of the Allegiance game that require object persistence, such as game state saving and loading mechanisms.

**NOTES:** The file uses macros `_TRACE_BEGIN` and `_TRACE_PART` for tracing information. It also calls a global function `TCGetPersistStreamSize` to calculate stream sizes.

## `src/test/TCAtl/PropertyClass.cpp`

**PURPOSE:** The file `PropertyClass.cpp` contains implementations and declarations for managing object properties using interfaces like `IPersistStreamInit`, `IPersistPropertyBag`, and `ISpecifyPropertyPages`. It provides functionality to load and save properties from streams and property bags, as well as support for different data types.

**KEY PARTS:**
- **TCComPropertyClassRoot:** Manages persistence of object properties using streams and property bags.
- **SetDirty(BOOL bDirty):** Sets or clears the object's dirty flag indicating whether it needs saving.
- **GetDirty() const:** Returns the current state of the object's dirty flag.
- **TCPersistStreamInit_Load:** Implementation of `IPersistStreamInit::Load` to load an object from a stream.
- **TCPersistPropertyBag_Load:** Implementation of `IPersistPropertyBag::Load` to load an object from a property bag.
- **TCComVariantCompare:** Compares two CComVariant objects, especially handling arrays of BSTRs.
- **ISpecifyPropertyPages_GetPages:** Provides a list of CLSIDs for property pages associated with the object.

**CONNECTS TO:** Other files related to persistence and property management in Allegiance, such as `PersistStream.cpp` and `PropertyBag.cpp`.

**NOTES:** The file includes TODO comments for handling additional `VARIANT` types like `VT_BSTR`, `VT_UNKNOWN`, etc., and there is a need for improved error handling and logging.

## `src/test/TCAtl/PropertyClass.h`

**PURPOSE:** The file `PropertyClass.h` in the Allegiance game source code provides a framework for property persistence and management using COM interfaces. It includes classes and functions to handle loading, saving, and notifying changes of properties.

**KEY PARTS:**
- **Name:** TCComPropertyClassRoot - Provides basic property persistence support.
- **Name:** CLoadSaveScope - Manages the load or save state of an object by incrementing or decrementing reference counts.
- **Name:** TCComFakeNotifySink - A placeholder class implementing `IPropertyNotifySink` methods but does nothing.
- **Name:** TCComPropertyClass - Template class providing property persistence support, inheriting from `TCComPropertyClassRoot`.
- **Name:** FireOnChanged - Fires a property change notification to connected objects.
- **Name:** FireOnRequestEdit - Fires a property change request to connected objects.
- **Name:** SendOnDataChange - Notifies all advisory sinks registered with the advise holder that the object data has changed.
- **Name:** TCPersistStreamInitImpl - Extends ATL's `IPersistStreamInitImpl` class to provide implementations for loading, saving, and checking if the stream is dirty.
- **Name:** TCPersistPropertyBagImpl - Implements the `IPersistPropertyBag` interface for a class.
- **Name:** TCComPropertyCompare - Compares two values of different types.
- **Name:** TCComPropertyInit - Initializes a value to a known state.
- **Name:** TCComPropertyAssign - Assigns the value of one variable to another.
- **Name:** TCComPropertyPut - Provides a standard implementation for `[propput]` methods.
- **Name:** TCComPropertyGet - Provides a standard implementation for `[propget]` methods.

**CONNECTS TO:** Other COM interfaces and classes, such as `IPersistStreamInit`, `IPropertyBag`, and various ATL classes.

**NOTES:**
- The file includes several template specializations for different types (VARIANT, BSTR, CComVariant, etc.), which handle specific conversion and comparison logic.
- There are conditional compilation directives (`#ifdef _ATL` and `#ifdef _AFX`) that include additional functionality when certain macros are defined.

## `src/test/TCAtl/PropertyPageBase.cpp`

PURPOSE:
TCAtl/PropertyPageBase.cpp contains a base class for property pages in Allegiance, providing functionality to manage embedded property pages and handle user interface interactions.

KEY PARTS:
- TCPropertyPageBase: Base class for property pages.
- GetInsidePage: Retrieves a pointer to an embedded property page.
- SetDirty: Sets the dirty state of a property page.
- CreateInsidePages: Creates all embedded property pages specified in the property page map.
- DestroyInsidePages: Destroys all instances of embedded property pages.
- ApplyToInsidePages: Applies changes to embedded property pages.
- OnInitDialogHandler: Handles WM_INITDIALOG message.
- OnChangedHandler: Handles property change notifications.

CONNECTS TO:
TCAdviseHolder class for advisory connections.

NOTES:
- TODO comments suggest improvements, such as using TCAdviseHolder and setting advises before updating fields.

PARTS (in file order):

- lines 1-325
  - TCPropertyPageBase: Base class for property pages.
  - GetInsidePage: Retrieves a pointer to an embedded property page.
  - GetInsidePageData: Retrieves application-defined data associated with an embedded property page.
  - GetInsidePageText: Retrieves application-defined text associated with an embedded property page.
  - GetDirtyInsidePage: Checks if the dirty flag is set for an embedded property page.
  - SetDirtyInsidePage: Sets or clears the dirty flag for an embedded property page.
  - GetInsidePageOfGroup: Retrieves a pointer to an embedded property page within a group.
  - GetInsidePageOfGroupText: Retrieves application-defined text associated with an embedded property page in a group.
- lines 326-647
  - TCPropertyPageBase::SetDirty: Sets the dirty state of a property page.
  - TCInsidePropPage TCPropertyPageBase::GetVisiblePageOfGroup: Retrieves the currently visible embedded property page within a group.
  - bool TCPropertyPageBase::ShowInsidePage: Makes the window of an embedded property page visible.
  - bool TCPropertyPageBase::HideInsidePage: Hides the window of an embedded property page.
  - bool TCPropertyPageBase::ShowInsidePageOfGroup: Shows a specific embedded property page within a group, hiding others.
  - bool TCPropertyPageBase::HideInsidePageOfGroup: Hides a specific embedded property page within a group.
  - bool TCPropertyPageBase::OnInitDialog: Override for derived classes to initialize user interface elements.
  - UINT TCPropertyPageBase::GetInsidePageTable: Retrieves the table of embedded property pages.
- lines 648-972
  - TCPropertyPageBase::OnSetAllAdvises: Handles setting advisory connections for property pages.
  - TCPropertyPageBase::OnClearAllAdvises: Handles clearing advisory connections for property pages.
  - TCPropertyPageBase::CreateInsidePage: Creates a window for an embedded property page.
  - TCPropertyPageBase::CreateInsidePages: Creates all embedded property pages specified in the property page map.
  - TCPropertyPageBase::DestroyInsidePages: Destroys all instances of embedded property pages.
  - TCPropertyPageBase::DestroyInsidePageMap: Destroys all embedded property pages within a specified map.
  - TCPropertyPageBase::GetInsidePageTable: Retrieves the table of child property pages.
  - TODO: Consider using TCAdviseHolder class for advisory connections.
- lines 973-1296
  - TCPropertyPageBase::_GetInsidePage: Retrieves a pointer to an embedded property page and ensures its window is created.
  - TCPropertyPageBase::_GetInsidePageOfGroup: Retrieves a pointer to an embedded property page within a group and ensures its window is created.
  - TCPropertyPageBase::SetObjects: Filters a set of objects based on the property field map, updating fields and setting advises if applicable.
  - TCPropertyPageBase::SetObjectsOfInsidePages: Forwards the original set of objects to each embedded property page.
  - TCPropertyPageBase::SaveObjectsForInsidePages: Saves the original set of objects for use by new embedded property pages.
  - TCPropertyPageBase::SetObjectsToInsidePage: Forwards the original set of objects to a newly created embedded property page.
  - TODO: Shouldn't the advises be set prior to UpdateFields?
  - __try/__except block: Handles exceptions and cleans up resources in case of failure.
- lines 1297-1567
  - TCPropertyPageBase::ApplyToInsidePages: Applies changes to embedded property pages.
  - TCPropertyPageBase::ReleaseObjectsForInsidePages: Releases objects for inside pages.
  - TCPropertyPageBase::ReleaseSupportedInterfaces: Releases interfaces supported by the property page.
  - TCPropertyPageBase::SetAllAdvises: Establishes advisory connections with specified objects.
  - TCPropertyPageBase::SetAdvises: Establishes a single advisory connection with an object.
  - TCPropertyPageBase::OnInitDialogHandler: Handles WM_INITDIALOG message.
  - TCPropertyPageBase::OnChangedHandler: Handles property change notifications.
  - TCPropertyPageBase::OnNcDestroyHandler: Handles WM_NCDESTROY message.

## `src/test/TCAtl/PropertyPageBase.h`

PURPOSE: This file defines a base class `TCPropertyPageBase` for property pages in the Allegiance game, providing common functionality and interfaces for managing property page components.

KEY PARTS:
- `TCPropertyPageBase`: Base class for property pages, implementing common functionality.
- `CPageMap`, `CPageGroupMap`, `CAdviseMap`, `CUnkVector`, `CUnkVectorMap`: Data structures to manage property pages and their associated data.
- `GetInsidePage`, `SetDirtyInsidePage`, `ShowInsidePage`: Methods for managing individual property pages.
- `OnInitDialogHandler`, `OnChangedHandler`, `OnNcDestroyHandler`: Message handlers for dialog events.

CONNECTS TO: `TCPropertyPageImpl`, `XInsidePageEntry`, `IConnectionPointContainer`, `IUnknown`.

NOTES: None.

## `src/test/TCAtl/PropertyPageImpl.h`

PURPOSE: The file `PropertyPageImpl.h` contains a template class `TCPropertyPageImpl` and related utilities for implementing property pages in a Microsoft application, likely Allegiance.

KEY PARTS:
- TCPropertyPageImpl: Template class providing a consistent implementation of the IPropertyPage interface.
- Checked_true and Checked_false: Convert boolean expressions to check box state constants.
- VARIANT_TRUE_Checked and VARIANT_TRUE_Unchecked: Convert check box states to VARIANT_BOOLs.
- COM_INTERFACE_ENTRIES_TCPropertyPageImpl and CHAIN_MSG_MAP_TCPropertyPageImpl: Macros for interface map and message map entries.
- XPageFieldEntry: Struct representing an entry in a table of property page fields.
- SetDirty, SetDirtyNoApply, Create, GetPageWindow, GetPageUnknown, IsObjectKnown, UpdateFields: Methods for managing the state and behavior of property pages.
- OnSetDirty, OnSetDirtyNoApply, OnInitDialogHandler, GetPageInfo, SetObjects, Apply, Help: Implementations of IPropertyPage methods.
- GetPageContainer, TranslateAccelerator, OnChanged, OnRequestEdit, put_EnableField, get_EnableField, put_ShowField, get_ShowField, FieldToggle: Additional implementations for handling property changes and interface interactions.

CONNECTS TO: Other parts of the Allegiance application that use property pages for configuration or editing settings.

NOTES: None.

PARTS (in file order):

- lines 1-341
  - TCPropertyPageImpl: Template class providing a consistent implementation of the IPropertyPage interface.
  - Checked_true: Converts a boolean expression to a check box state constant (BST_CHECKED).
  - Checked_false: Converts a boolean expression to a check box state constant (BST_UNCHECKED).
  - VARIANT_TRUE_Checked: Converts a check box state constant to a VARIANT_BOOL (VARIANT_TRUE if BST_CHECKED).
  - VARIANT_TRUE_Unchecked: Converts a check box state constant to a VARIANT_BOOL (VARIANT_TRUE if BST_UNCHECKED).
  - COM_INTERFACE_ENTRIES_TCPropertyPageImpl: Macro for interface map entries.
  - CHAIN_MSG_MAP_TCPropertyPageImpl: Macro for message map entries.
  - XPageFieldEntry: Struct representing an entry in a table of property page fields.
- lines 342-676
  - TCPropertyPageImpl<T>: Template class for implementing property pages.
  - SetDirty(bool bDirty): Sets the dirty flag of the property page and notifies the page site if necessary.
  - SetDirtyNoApply(): Sets the dirty flag without notifying the page site.
  - Create(HWND hWndParent): Creates the property page window using the IDD member variable.
  - GetPageWindow(): Returns the HWND of the property page.
  - GetPageUnknown(): Returns the IUnknown pointer of the property page.
  - IsObjectKnown(IUnknown punk): Determines if an object supports an interesting interface.
  - UpdateFields(DISPID dispid): Updates the property page fields from the set of objects being viewed/edited.
- lines 677-1010
  - TCPropertyPageImpl<T>: Implements IPropertyPage and IPropertyPageSite interfaces for property pages.
  - OnSetDirty: Sets the dirty flag of the property page when a change occurs.
  - OnSetDirtyNoApply: Sets the dirty flag without applying changes.
  - OnInitDialogHandler: Handles WM_INITDIALOG message, delegating to base class.
  - GetPageInfo: Returns information about the property page.
  - SetObjects: Provides the property page with an array of IUnknown pointers for objects associated with this property page.
  - Apply: Applies current property page values to underlying objects specified through SetObjects.
  - Help: Invokes help in response to end-user request, always returns E_NOTIMPL.
- lines 1011-1315
  - TCPropertyPageImpl<T>: Implements IPropertyPage, IPropertyNotifySink, and ITCPropertyFieldToggle interfaces for property pages.
  - GetPageContainer: Delegates to the page site's GetPageContainer method.
  - TranslateAccelerator: Delegates to the page site's TranslateAccelerator method.
  - OnChanged: Posts a message to handle property change notifications on the main thread.
  - OnRequestEdit: Always returns S_OK, indicating that properties are allowed to change.
  - put_EnableField and get_EnableField: Enable or disable fields based on their dispatch identifier.
  - put_ShowField and get_ShowField: Show or hide fields based on their dispatch identifier.
  - FieldToggle: Toggles the enable/disable or show/hide state of a field.

## `src/test/TCAtl/RangeValueImpl.h`

PURPOSE:
The file `RangeValueImpl.h` defines a template class `ITCRangeValueImpl` that implements an interface for managing range values with properties such as minimum, maximum, default value, granularity, and tick marks. It provides methods to get and set these properties, validate the current value, and apply changes.

KEY PARTS:
- **Class: ITCRangeValueImpl**
  - Manages range values by interpreting changes and applying them.
  - Provides methods for getting and setting the minimum, maximum, default, granularity, and tick marks of a range value.
  - Validates the current value against the defined range.
  - Applies changes to the property and notifies listeners if the change is successful.

- **Macros: DECLARE_RANGE_VALUE_INTERPRET_CHANGE, BEGIN_RANGE_VALUE_MAP, RANGE_VALUE_ENTRY_EX, RANGE_VALUE_ENTRY, END_RANGE_VALUE_MAP**
  - Used to define and manage the properties of a range value in derived classes.
  - Simplify the process of defining range value entries and their corresponding methods.

- **Function: DetermineTickMarks**
  - Calculates an appropriate tick mark count based on the number of steps between the minimum and maximum values.

CONNECTS TO:
- `IDispatchImpl` from ATL (Active Template Library) for COM interface implementation.
- Other classes that derive from `ITCRangeValueImpl` to define specific range value properties and behaviors.

NOTES:
- None.

## `src/test/TCAtl/RangeValueSlider.cpp`

PURPOSE: The file `RangeValueSlider.cpp` implements the functionality of a range value slider control in Allegiance. It updates and applies settings to an object that implements the `ITCRangeValue` interface.

KEY PARTS:
- TCRangeValueSlider: Class representing the range value slider control.
  - Update: Updates the slider based on the properties of an `ITCRangeValue` object.
  - Apply: Applies the current position of the slider to the `ITCRangeValue` object.

CONNECTS TO: Depends on the `ITCRangeValue` interface and related classes for interaction with game objects.

NOTES: None.

## `src/test/TCAtl/RangeValueSlider.h`

PURPOSE: This file defines a class `TCRangeValueSlider` that extends `TCSliderWindow` to create a slider control with the ability to track and reset its value to a default position.

KEY PARTS:
- TCRangeValueSlider: Inherits from TCSliderWindow, adds functionality to check if the slider is at its default position and to set it back to that position.
- IsAtDefaultPos: Checks if the current position of the slider matches the default value.
- SetToDefaultPos: Sets the slider's position to the default value.
- Update: Updates the slider based on a collection of objects implementing ITCRangeValue.
- Apply: Applies the current slider value to a collection of objects implementing ITCRangeValue.
- operator=: Subclasses the window with a new handle.

CONNECTS TO: SliderWindow.h, CodecDriver.h

NOTES: None.

## `src/test/TCAtl/SimpleNotifyReflector.h`

PURPOSE: This file defines a template class `TCSimpleNotifyReflector` that implements the `IPropertyNotifySink` interface to handle property change notifications. It is used to reflect changes in properties of an object.

KEY PARTS:
- TCSimpleNotifyReflector_Base: Base class implementing the `IPropertyNotifySink` interface with basic functionality.
- TCSimpleNotifyReflector: Template class that inherits from `TCSimpleNotifyReflector_Base` and provides specific implementations for handling property change notifications by calling methods on a target object.

CONNECTS TO:
- IPropertyNotifySink: The interface implemented to handle property change notifications.

NOTES:
- None.

## `src/test/TCAtl/SimpleStream.h`

PURPOSE: This file defines a simple implementation of an IStream and ISequentialStream interface in C++ using ATL. It provides a fixed-size stream that can be read from and written to.

KEY PARTS:
- CTCSimpleStreamImpl: Implements the IStream and ISequentialStream interfaces for a fixed-size memory block.
- Init: Initializes the stream with a pointer to data and its size.
- Read: Reads data from the stream into a buffer.
- Write: Writes data from a buffer into the stream.
- Seek: Moves the current position within the stream.
- SetSize: Changes the size of the stream (can only shrink).
- CopyTo: Copies data from this stream to another stream.
- Commit and Revert: No-op implementations as they are not supported.
- LockRegion and UnlockRegion: Not implemented.
- Stat: Provides statistics about the stream, including its size and current time.
- Clone: Not implemented.

CONNECTS TO: None

NOTES: The implementation does not support thread synchronization or keep track of time fields other than using the current system time.

## `src/test/TCAtl/SliderWindow.h`

PURPOSE:
The SliderWindow.h file defines a class for creating and managing a slider control in a Windows application using the TrackBar common control.

KEY PARTS:
- TCSliderWindow: Class that encapsulates functionality for a slider window.
- SubclassWindow: Subclasses an existing window to create a slider control.
- GetPos, SetPos: Retrieve and set the current position of the slider thumb.
- GetRange, SetRange: Retrieve and set the range of values the slider can represent.
- GetTic, SetTic: Retrieve and set tick marks on the slider.
- SetSelection, ClearSel: Set and clear selection ranges on the slider.

CONNECTS TO:
- TCLib.h: Header file for common library functions used by TCSliderWindow.

NOTES:
None.

## `src/test/TCAtl/TCNullStreamImpl.cpp`

PURPOSE: This file contains the implementation of a null stream class named `CTCNullStreamImpl` that implements the `IStream` interface. The class is designed to count the number of bytes being written to it and can be used to determine the size required for persisting an object without allocating any memory.

KEY PARTS:
- `CTCNullStreamImpl`: Class implementing the `IStream` interface, providing methods like `Read`, `Write`, `Seek`, etc., which perform operations on a null stream.
- `Read`: Method that reads data from the stream but does not actually read anything; it simply updates the position and returns zero bytes read.
- `Write`: Method that increments the position and size of the stream when writing data, effectively counting the number of bytes written.
- `Seek`: Method to change the current position in the stream.
- `SetSize`: Method to set the new size of the stream.
- `CopyTo`: Method to copy data from this null stream to another stream.
- `Commit`, `Revert`, `LockRegion`, `UnlockRegion`, and `Stat`: Methods that return `E_NOTIMPL` as they are not implemented in this class.

CONNECTS TO: This file is part of the Allegiance game project, specifically within the test suite for ATL (Active Template Library) components. It does not depend on external systems or files outside of the Allegiance codebase.

NOTES: None.

## `src/test/TCAtl/TCNullStreamImpl.h`

PURPOSE: This file defines a class `CTCNullStreamImpl` that implements the `IStream` and `ISequentialStream` interfaces to count bytes written without actually storing them. It is used for calculating the size of objects when persisted.

KEY PARTS:
- CTCNullStreamImpl: Class implementing IStream and ISequentialStream, counting bytes written.
- Read: Method to read data (does nothing).
- Write: Method to write data, increments byte count.
- Seek: Changes the current position in the stream.
- SetSize: Sets the size of the stream.
- CopyTo: Copies data from this stream to another.
- Commit: Commits changes to the stream (does nothing).
- Revert: Reverts any changes made since the last commit (does nothing).
- LockRegion and UnlockRegion: Locks or unlocks regions of the stream (does nothing).
- Stat: Retrieves information about the stream.
- Clone: Creates a new instance of the stream.

CONNECTS TO: TCLib\ObjectLock.h for locking mechanisms.

NOTES: None.

## `src/test/TCAtl/UserAcct.cpp`

PURPOSE: This file contains implementation of the TCUserAccount class, which manages user accounts and their associated privileges in a Windows environment.

KEY PARTS:
- TCUserAccount: Manages user account operations such as initialization, SID lookup, privilege management, and setting run-as users.
- Init: Initializes the user account with the specified username.
- GetSID: Retrieves the Security Identifier (SID) for the given username.
- HasRight: Checks if a specific privilege is assigned to the user.
- SetRight: Assigns a privilege to the user.
- RemoveRight: Removes a privilege from the user.
- SetRunAsInteractiveUser: Sets the "Interactive User" as the run-as user for an application.
- SetRunAsUser: Sets a specified user as the run-as user for an application.

CONNECTS TO: Windows API functions such as NetServerGetInfo, LookupAccountNameW, LsaOpenPolicy, and others.

NOTES: None.

## `src/test/TCAtl/UserAcct.h`

PURPOSE: This file defines a class `TCUserAccount` that provides functionality for managing user accounts and their associated properties such as Security Identifiers (SIDs) and privileges.

KEY PARTS:
- TCUserAccount: Manages user account information including username, domain, and SID.
- GetSID: Retrieves the SID for a given username.
- SetRunAsInteractiveUser: Configures an application to run as an interactive user.
- SetRunAsUser: Configures an application to run under a specified user account.
- HasRight: Checks if a user has a specific privilege.
- SetRight: Grants a user a specific privilege.
- RemoveRight: Removes a privilege from a user.

CONNECTS TO: TCLib\NetApi.h, TCLib\AutoHandle.h

NOTES: None.

## `src/test/TCAtl/VersionInfoImpl.h`

PURPOSE: This file defines a template class `TCVersionInfoImpl` that implements various interfaces for managing version information of a module, such as file and product versions, flags indicating build characteristics, and string values like company name and legal copyright.

KEY PARTS:
- `TCVersionInfoImpl`: A template class implementing IDispatch, ISupportErrorInfo, IPersistStreamInit, IMarshal, and CComObjectRootEx interfaces to manage version information.
- `VerifyInit`: Ensures the object has been initialized before accessing its properties or methods.
- `get_Is##flagName` and `get_##valueName`: Macros for generating getter methods for flags and common string values.
- `Load` and `Save`: Methods for loading and saving version information to a stream.

CONNECTS TO: Other parts of the Allegiance game that need access to module version information, such as resource management or configuration files.

NOTES: None.

## `src/test/TCAtl/VersionInfoImpl.idl`

PURPOSE: This file defines an interface and its properties for retrieving version information from a file. It allows setting and getting the file name, various version components (file and product versions), flags indicating different attributes of the file, target operating systems, file types, driver types, font types, language ID, code page, existence checks for named values, and common string values like CompanyName, FileDescription, etc.

KEY PARTS:
- FileName: Gets/sets the name of the file.
- FileVersionString, ProductVersionString: Get version information as formatted strings.
- IsDebug, IsInfoInferred, etc.: Check various attributes of the file.
- LanguageID, CodePage: Set/get language and code page for string value lookups.
- Exists, Value: Check existence and get values by key.
- CompanyName, FileDescription, etc.: Get common string values.

CONNECTS TO: VersionInfoImpl.h (class template implementation)

NOTES: None.

## `src/test/TCAtl/WindowThread.h`

PURPOSE: This file defines a class hierarchy and utility functions for creating windows in a separate thread with a message loop.

KEY PARTS:
- TCWindowCreatorBase: Abstract base class for window creators.
- TCWindowImplCreator: Template class to create CWindowImpl-based windows.
- TCContainedWindowCreator: Class to create CContainedWindow-based windows.
- TCDialogImplCreator: Template class to create CDialogImpl-based dialogs.
- TCWindowThread: Class that implements a thread with a message loop for creating windows.
- CreateOnThread methods in TCWindowImpl, TCContainedWindow, and TCDialogImpl: Methods to create windows on a separate thread.

CONNECTS TO: TCLib\WindowThreadImpl.h

NOTES: None.
