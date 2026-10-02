# src/test/TCObj

[Back to the index](../LLAMA_SUMMARY.md) - 27 files, 3,125 lines. Notes by a local model, to orient by: not verified facts.

This folder contains source code related to COM (Component Object Model) components and utilities in Allegiance, a 2000 Microsoft multiplayer space game. It includes various classes for managing objects, properties, strings, and utility functions across process boundaries.

- ObjRef.js: Manages object references.
- SrcInc.cpp: Header file with common includes.
- SrcInc.h: Header file with common declarations.
- TCMarshalByValue.cpp: Implements a COM component that marshals data by value using the IMarshal interface.
- TCMarshalByValue.h: Defines the `CTCMarshalByValue` class for marshaling and unmarshaling objects across process boundaries.
- TCMarshalByValue.rgs: Registers the Marshal By Value Helper Object in the Allegiance game.
- TCNullStream.cpp: Implements a COM component that provides functionality through interfaces like `IUnknown`.
- TCNullStream.h: Defines the `CTCNullStream` class for a Null IStream Object.
- TCNullStream.rgs: Registers the "Null IStream Object" COM object.
- TCObj.cpp: Manages common objects in the Allegiance game.
- TCObj.def: Module definition file for the TCObj.dll.
- TCObj.idl: Defines type library and interfaces for the TCObj.dll.
- TCObjPS.def: Module definition file for the PS (Proxy Stub) DLL.
- TCPropBagOnRegKey.cpp: Implements a component object that manages properties stored in Windows registry keys.
- TCPropBagOnRegKey.h: Defines the `ITCPropBagOnRegKey` interface and provides functionality for managing properties on registry keys.
- TCPropBagOnRegKey.rgs: Registers a COM object that implements a property bag on a registry key.
- TCStrings.cpp: Manages a collection of strings and provides methods to manipulate them.
- TCStrings.h: Defines the `CTCStrings` class for managing string collections.
- TCStrings.rgs: Registers the "Strings collection" COM class.
- TCUtility.cpp: Implements utility methods for creating objects, managing object references, sleeping, and retrieving objects by moniker.
- TCUtility.h: Defines the `ITCUtility` interface and provides utility methods.
- TCUtility.rgs: Registers a utility object used in Allegiance.
- dlldatax.h: Header file for `dlldatax.c`.
- pch.cpp: Precompiled header file for the TCObj project.
- pch.h: Includes standard system headers, project-specific headers, and third-party libraries.
- resource.h: Resource definitions for the Allegiance game.

Largest files: TCPropBagOnRegKey.cpp (1433 lines), TCStrings.cpp (376 lines), TCPropBagOnRegKey.h (170 lines), TCObj.cpp (162 lines), TCMarshalByValue.cpp (157 lines).

## `src/test/TCObj/ObjRef.js`

PURPOSE: tiny file (19 lines), shown in full rather than summarized.

    // Create the utility object
    var objUtil = new ActiveXObject("TCUtil");
    // Create an object, using the JScript operator new
    var objStrings = new ActiveXObject("TCObj.Strings");
    // Add some strings to the collection
    objStrings.Add("One");
    objStrings.Add("Two");
    objStrings.Add("Three");
    // Create an ObjRef string for the object
    var strObjRef = objUtil.ObjectReference(objStrings);
    // Display the string
    WScript.Echo(strObjRef);
    // Get the object from it's reference string
    var objStringsFromObjRef = WScript.GetObject(strObjRef);
    // Display the strings in the collection
    var str = "";
    for (i = 0; i < objStringsFromObjRef.Count; ++i)
      str += "objStringsFromObjRef(i) = " + objStringsFromObjRef(i) + "\n";
    WScript.Echo(str);

## `src/test/TCObj/SrcInc.cpp`

PURPOSE: tiny file (9 lines), shown in full rather than summarized.

    /////////////////////////////////////////////////////////////////////////////
    // This file should contain the CPP files for all source code-based
    // libraries used in the project. Any corresponding header files for the
    // libraries should be placed in SrcInc.h.
    //
    #include "pch.h"
    #include <TCAtl.cpp>
    #include <..\TCAtl\PropertyClass.cpp>
    #include <..\TCAtl\TCNullStreamImpl.cpp>

## `src/test/TCObj/SrcInc.h`

PURPOSE: tiny file (10 lines), shown in full rather than summarized.

    #pragma once
    /////////////////////////////////////////////////////////////////////////////
    // This file should contain the include files for all source code-based
    // libraries used in the project. Any corresponding CPP files for the
    // libraries should be placed in SrcInc.cpp.
    //
    #include <TCAtl.h>
    #include <..\TCAtl\PropertyClass.h>
    #include <..\TCAtl\TCNullStreamImpl.h>
    #include <..\TCAtl\ObjectMap.h>

## `src/test/TCObj/TCMarshalByValue.cpp`

PURPOSE: This file implements a COM component object that marshals data by value using the IMarshal interface. It delegates marshaling operations to an outer object's IPersistStream implementation.

KEY PARTS:
- CTCMarshalByValue: Class implementing the CLSID_TCMarshalByValue component object.
- GetUnmarshalClass: Retrieves the class ID of the controlling unknown.
- GetMarshalSizeMax: Calculates the maximum size required for marshalling.
- MarshalInterface: Writes data to a stream, including an endian indicator.
- UnmarshalInterface: Reads data from a stream and delegates loading to the outer object's IPersistStream.
- ReleaseMarshalData: No-op as marshaling by value does not acquire resources.
- DisconnectObject: No-op as marshaling by value has no concept of connectedness.

CONNECTS TO:
- src/test/TCObj/TCMarshalByValue.h (header file)
- src/test/TCObj/TCPersistStream.cpp (for size calculations)

NOTES: None.

## `src/test/TCObj/TCMarshalByValue.h`

PURPOSE: This file defines a COM component object class `CTCMarshalByValue` that implements the `IMarshal` interface for marshaling and unmarshaling objects across process boundaries.

KEY PARTS:
- CTCMarshalByValue: Implements the IMarshal interface to handle object marshaling.
- GetUnmarshalClass: Retrieves the CLSID of the object's implementation.
- GetMarshalSizeMax: Calculates the maximum size required for marshalling an object.
- MarshalInterface: Marshals an object into a stream.
- UnmarshalInterface: Unmarshals an object from a stream.
- ReleaseMarshalData: Releases any data associated with a marshalled object.
- DisconnectObject: Disconnects the object from its marshaling context.

CONNECTS TO: TCObj.h, TCLib\ObjectLock.h

NOTES: None.

## `src/test/TCObj/TCMarshalByValue.rgs`

PURPOSE: This file is an RGS (Registry Script) used to register a COM object for the Marshal By Value Helper Object in the Allegiance game.

KEY PARTS:
- Name: TCObj.MarshalByValue.1
  - Description: Represents the versioned class identifier for the Marshal By Value Helper Object.
- CLSID: {8962E244-CD81-11d2-9646-00C04F68DEB0}
  - Unique identifier for the COM object.
- ProgID: TCObj.MarshalByValue.1
  - Programmatic identifier used to create instances of the object.
- VersionIndependentProgID: TCObj.MarshalByValue
  - Identifier used for version-independent access to the object.
- InprocServer32: %MODULE%
  - Path to the DLL containing the implementation of the COM object.
- ThreadingModel: Both
  - Specifies that the object can be called from both single-threaded and multi-threaded apartments.

CONNECTS TO:
- Allegiance game's COM infrastructure

NOTES:
- None.

## `src/test/TCObj/TCNullStream.cpp`

PURPOSE: tiny file (16 lines), shown in full rather than summarized.

    /////////////////////////////////////////////////////////////////////////////
    // TCNullStream.cpp | Implementation of the CTCNullStream class, which
    // implements the CLSID_TCNullStream component object.<nl><nl>
    // This object provides an implementation of the IStream interface that does
    // nothing more that count the number of bytes being written to it.  Since
    // ATL (currently 3.0) implements IPersistStreamInitImpl::GetSizeMax by
    // returning E_NOTIMPL, this object can be used in an override of that
    // method. This is accomplished by creating an instance of this null stream
    // and 'persisting' an object to it, thereby counting the number of bytes
    // needed to persist the object without allocating any memory.
    //
    #include "pch.h"
    #include "TCNullStream.h"
    /////////////////////////////////////////////////////////////////////////////
    // CTCNullStream
    TC_OBJECT_EXTERN_IMPL(CTCNullStream)

## `src/test/TCObj/TCNullStream.h`

PURPOSE: This file defines a COM component class `CTCNullStream` that implements the `CLSID_TCNullStream` and provides functionality through interfaces like `IUnknown`.

KEY PARTS:
- CTCNullStream: Main class implementing the COM object.
- DECLARE_REGISTRY_RESOURCEID: Registers the component with the system.
- BEGIN_CATEGORY_MAP and IMPLEMENTED_CATEGORY: Specifies the category of the component.
- BEGIN_COM_MAP and COM_INTERFACE_ENTRY: Maps the component's interfaces.

CONNECTS TO: Depends on `resource.h`, `SrcInc.h`, `TCObj.h`, and `TCLib\ObjectLock.h`.

NOTES: None.

## `src/test/TCObj/TCNullStream.rgs`

PURPOSE: This file is a Windows Registry Script (RGS) that defines the registration information for a COM object named "Null IStream Object". It specifies details such as the CLSID, ProgID, and InprocServer32 path.

KEY PARTS:
- Name: TCNullStream
- Purpose: Represents a null implementation of an IStream interface.
- CLSID: {8962E248-CD81-11d2-9646-00C04F68DEB0}
- ProgID: TCObj.NullStream.1
- VersionIndependentProgID: TCObj.NullStream
- InprocServer32: Path to the module containing the implementation

CONNECTS TO: Windows Registry and COM system.

NOTES: None

## `src/test/TCObj/TCObj.cpp`

PURPOSE: This file contains the implementation of a DLL for managing common objects in the Allegiance game.

KEY PARTS:
- `DllMain`: Entry point for the DLL, handles initialization and termination.
- `DllCanUnloadNow`: Determines if the DLL can be unloaded by OLE.
- `DllGetClassObject`: Returns a class factory to create an object of the requested type.
- `DllRegisterServer`: Registers the DLL with the system registry.
- `DllUnregisterServer`: Unregisters the DLL from the system registry.

CONNECTS TO: Allegiance game, ATL (Active Template Library), and other related components.

NOTES: None.

## `src/test/TCObj/TCObj.def`

PURPOSE: tiny file (8 lines), shown in full rather than summarized.

    ; TCObj.def : Declares the module parameters for the DLL.
    ; Imago 6/10 removed ordinal and description warnings
    LIBRARY      "TCObj"
    EXPORTS
      DllCanUnloadNow     PRIVATE
      DllGetClassObject   PRIVATE
      DllRegisterServer   PRIVATE
      DllUnregisterServer PRIVATE

## `src/test/TCObj/TCObj.idl`

PURPOSE: This file defines the type library and interfaces for the TCObj.dll, which is part of the Allegiance game. It includes various COM objects and their interfaces.

KEY PARTS:
- TCObj: A hidden, non-creatable COM coclass.
- TCMarshalByValue: A COM coclass implementing IUnknown.
- TCStrings: A COM coclass implementing ITCStrings.
- TCNullStream: A COM coclass implementing IUnknown.
- TCPropBagOnRegKey: A COM coclass implementing ITCPropBagOnRegKey.
- TCUtility: A COM coclass implementing ITCUtility.

CONNECTS TO: Depends on "TCIDL.idl" and "stdole32.tlb".

NOTES: None.

## `src/test/TCObj/TCObjPS.def`

PURPOSE: tiny file (8 lines), shown in full rather than summarized.

    LIBRARY      "TCObjPS"
    DESCRIPTION  'Proxy/Stub DLL'
    EXPORTS
      DllGetClassObject       @1  PRIVATE
      DllCanUnloadNow         @2  PRIVATE
      GetProxyDllInfo         @3  PRIVATE
      DllRegisterServer       @4  PRIVATE
      DllUnregisterServer     @5  PRIVATE

## `src/test/TCObj/TCPropBagOnRegKey.cpp`

PURPOSE: The file `TCObj/TCPropBagOnRegKey.cpp` implements a component object that manages properties stored in Windows registry keys, providing functionality to create and manage local and remote objects.

KEY PARTS:
- CTCPropBagOnRegKey: Manages properties using Windows registry keys.
- _GetSubkey: Retrieves the full subkey path for an object and its parent.
- CLSIDFromProgID: Converts a programmatic identifier to a class identifier.
- _ReadVariantFromValue: Reads a value from the registry into a variant.
- RootKeyFromString: Converts a string representing a registry root key to its corresponding HKEY value.
- CreateObject, CreateLocalObject, CreateRemoteObject: Methods to create objects using the internal property bag.

CONNECTS TO: Windows Registry API (e.g., `HKEY`, `RegOpenKeyEx`, `RegSetValueEx`).

NOTES: None.

PARTS (in file order):

- lines 1-364
  - CTCPropBagOnRegKey: Implements a component object that interacts with registry keys to manage properties.
  - _GetSubkey: Retrieves the full subkey path for this object and its parent.
  - _GetServer: Reads the server name associated with an object in the registry.
  - _SetServer: Writes the server name associated with an object in the registry.
  - _GetObjectCLSID: Retrieves the CLSID of an object from the registry.
  - _CreateObject: Creates a new object either locally or remotely based on the server information.
  - _CreateLocalObject: Creates a local object using COM and loads its properties.
  - _CreateRemoteObject: Creates a remote object using DCOM and loads its properties.
- lines 365-714
  - CTCPropBagOnRegKey: Manages properties stored in a registry key.
  - CLSIDFromProgID: Converts a programmatic identifier to a class identifier.
  - _ReadVariantFromValue: Reads a value from the registry into a variant.
  - _LogError: Logs an error with details about the function, description, and HRESULT.
  - _GetSubkeyVarType: Retrieves the variant type of a subkey in the registry.
  - _ReadSafeArray: Reads a safe array from the registry into a VARIANT.
  - _WriteSafeArray: Writes a safe array to the registry from a VARIANT.
- lines 715-1045
  - CTCPropBagOnRegKey::_WriteSafeArray: Writes a safe array to the registry.
  - CTCPropBagOnRegKey::_ReadVariant: Reads a variant from the registry.
  - CTCPropBagOnRegKey::_WriteVariant: Writes a variant to the registry.
  - V_VT: Retrieves the type of a VARIANT.
  - SafeArrayGetUBound: Gets the upper bound of a safe array dimension.
  - SafeArrayGetLBound: Gets the lower bound of a safe array dimension.
  - SafeArrayAccessData: Locks access to the data in a safe array.
  - SafeArrayUnaccessData: Unlocks access to the data in a safe array.
- lines 1046-1432
  - CTCPropBagOnRegKey: Manages a property bag using Windows registry keys.
  - _VarTypeFromUnknown: Determines the type of an object based on its interface.
  - RootKeyFromString: Converts a string representing a registry root key to its corresponding HKEY value.
  - KeyExists: Checks if a specified subkey exists under a given registry key.
  - QueryString: Retrieves a string value from a registry key.
  - CreateKey: Creates or opens a registry key.
  - OpenKey: Opens an existing registry key.
  - CreateObject, CreateLocalObject, CreateRemoteObject: Methods to create objects using the internal property bag.

## `src/test/TCObj/TCPropBagOnRegKey.h`

PURPOSE: This file defines a COM component that implements the `ITCPropBagOnRegKey` interface and provides functionality for managing properties stored in Windows Registry keys.

KEY PARTS:
- CTCPropBagOnRegKey: Main class implementing the `ITCPropBagOnRegKey`, `IDispatch`, and `IPropertyBag` interfaces.
- Init: Initializes the object with a registry key, subkey, and parent object.
- ClosePrevious: Detaches from the current registry key if it does not own it.
- _GetSubkey, _GetServer, _SetServer, _GetObjectCLSID: Helper methods for retrieving and setting properties.
- _CreateObject, _CreateLocalObject, _CreateRemoteObject, _LoadObject, _SaveObject: Methods to create, load, save, and manage objects based on registry keys.

CONNECTS TO: Windows Registry API (CRegKey), ATL library components (IDispatchImpl, CComCoClass, etc.), and other COM interfaces.

NOTES: None.

## `src/test/TCObj/TCPropBagOnRegKey.rgs`

PURPOSE: This file defines a registry key and its associated properties for a COM object in Allegiance. It specifies the CLSID, ProgID, and other metadata for an object that implements a property bag on a registry key.

KEY PARTS:
- Name: TCObj.PropBagOnRegKey
  - Description: Property Bag Implemented on a Registry Key

- Name: {8962E245-CD81-11d2-9646-00C04F68DEB0}
  - Description: CLSID for the COM object

- Name: {8962E243-CD81-11d2-9646-00C04F68DEB0}
  - Description: TypeLib associated with the COM object

- Name: InprocServer32
  - Description: Path to the DLL implementing the COM object

CONNECTS TO:
- Allegiance game engine, specifically for managing properties on registry keys through a COM interface.

NOTES:
- None.

## `src/test/TCObj/TCStrings.cpp`

PURPOSE: This file contains the implementation of the `CTCStrings` class, which manages a collection of strings and provides methods to manipulate and access these strings.

KEY PARTS:
- `CTCStrings`: Manages a collection of strings.
- `get_Count`: Returns the number of strings in the collection.
- `get__NewEnum`: Provides an enumerator for iterating over the collection.
- `get_Item`: Retrieves a string by index.
- `Add`: Adds a new string to the collection.
- `Remove`: Removes a string by index.
- `RemoveAll`: Clears all strings from the collection.
- `AddDelimited`: Adds multiple strings separated by a delimiter.
- `get_DelimitedItems`: Concatenates all strings in the collection into a single string, separated by a delimiter.
- `AddStrings`: Adds strings from another `ITCStrings` collection.
- `put_Collection1` and `get_Collection1`: Persistently stores and retrieves the collection as a safe array.

CONNECTS TO: `pch.h`, `TCStrings.h`

NOTES: None.

## `src/test/TCObj/TCStrings.h`

PURPOSE: This file defines a COM component object class `CTCStrings` that implements interfaces for managing a collection of strings.

KEY PARTS:
- CTCStrings: Main class implementing various interfaces like ITCStrings, IDispatch, ITCCollection, and IPersistStorage.
- CStringVector: Internal data structure to store the collection of strings.
- get_Count, get_Item, Add, Remove, etc.: Methods for managing the string collection.

CONNECTS TO: src/inc/TCObj.h (base class headers), resource.h (resource IDs).

NOTES: None.

## `src/test/TCObj/TCStrings.rgs`

PURPOSE: This file is a Windows Registry Script (.rgs) that defines the registration information for a COM class named "Strings collection" in the Allegiance game. It specifies the CLSID, ProgID, and other properties required to register this COM component.

KEY PARTS:
- Name: TCObj.Strings
  - Description: Represents a strings collection object.
- Name: TCObj.Strings.1
  - Description: Specific version of the strings collection object.
- CLSID: {8962E245-CD81-11d2-9646-00C04F68DEB0}
  - Description: Unique identifier for the COM class.
- TypeLib: {8962E243-CD81-11d2-9646-00C04F68DEB0}
  - Description: Reference to the type library that contains the interface definitions for this COM component.
- ProgID: TCObj.Strings.1
  - Description: Programmatic identifier used to create instances of the COM object from scripting languages like VBScript or JScript.
- InprocServer32: %MODULE%
  - Description: Path to the DLL file that implements the COM class.

CONNECTS TO:
- Allegiance game engine, specifically for integrating custom string handling functionality.

NOTES:
- None.

## `src/test/TCObj/TCUtility.cpp`

PURPOSE: Implements utility methods for creating objects, managing object references, sleeping, and retrieving objects using monikers.

KEY PARTS:
- CTCUtility: Class implementing the ITCUtility interface with methods to create objects, get object references, sleep, and retrieve objects.
- CreateObject: Method to create an object using a ProgID and computer name.
- ObjectReference: Method to get a reference string for an object.
- Sleep: Method to pause execution for a specified duration in milliseconds.
- GetObject: Method to retrieve an object by moniker with optional UI interaction and timeout.

CONNECTS TO: TCUtilImpl class, which contains the actual implementation of the utility methods.

NOTES: None.

## `src/test/TCObj/TCUtility.h`

PURPOSE: This file defines a COM component object class named `CTCUtility` that implements the `ITCUtility` interface. It provides methods for creating objects, getting object references, sleeping, and retrieving objects by moniker.

KEY PARTS:
- CTCUtility: Class implementing the ITCUtility interface.
- CreateObject: Method to create an object using a ProgID.
- ObjectReference: Method to get a reference to an object.
- Sleep: Method to pause execution for a specified duration.
- GetObject: Method to retrieve an object by moniker.

CONNECTS TO: src/inc/SrcInc.h, resource.h, TCObj.h, objsafe.h

NOTES: None.

## `src/test/TCObj/TCUtility.rgs`

PURPOSE: This file is a Windows registry script that defines the class information for a utility object used in Allegiance.

KEY PARTS:
- TCUtil: The top-level key for the utility object.
- CLSID: The unique identifier for the utility object's class.
- ProgID: Programmatic Identifier for the utility object.
- InprocServer32: Specifies the DLL that implements the utility object, using the current module as the server.

CONNECTS TO: Allegiance game engine, specifically the components that utilize the utility object.

NOTES: None.

## `src/test/TCObj/dlldatax.c`

PURPOSE: This file is a wrapper around the standard `dlldata.c` file to handle proxy stub DLL registration and management for the Allegiance game.

KEY PARTS:
- `REGISTER_PROXY_DLL`: Macro defined to indicate that this DLL will register proxy stubs.
- `_WIN32_WINNT 0x0400`: Sets the minimum required Windows version to WinNT 4.0 or Win95 with DCOM support.
- `USE_STUBLESS_PROXY`: Defined only when using MIDL switch `/Oicf`, indicating the use of stubless proxies.
- `DllMain`, `DllRegisterServer`, `DllUnregisterServer`, `DllGetClassObject`, `DllCanUnloadNow`: These are redefined to prefix with `Prx` and include additional logic for proxy stub management.

CONNECTS TO: `dlldata.c`, `TCObj_p.c`

NOTES: None.

## `src/test/TCObj/dlldatax.h`

PURPOSE: tiny file (19 lines), shown in full rather than summarized.

    #if !defined(AFX_DLLDATAX_H__25072D60_AB2F_11D2_97EF_00A0C932A52F__INCLUDED_)
    #define AFX_DLLDATAX_H__25072D60_AB2F_11D2_97EF_00A0C932A52F__INCLUDED_
    #if _MSC_VER >= 1000
      #pragma once
    #endif // _MSC_VER >= 1000
    #ifdef _MERGE_PROXYSTUB
    extern "C"
    {
    BOOL WINAPI PrxDllMain(HINSTANCE hInstance, DWORD dwReason,
      LPVOID lpReserved);
    STDAPI PrxDllCanUnloadNow(void);
    STDAPI PrxDllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv);
    STDAPI PrxDllRegisterServer(void);
    STDAPI PrxDllUnregisterServer(void);
    }
    #endif
    //{{AFX_INSERT_LOCATION}}
    // Microsoft Developer Studio will insert additional declarations immediately before the previous line.
    #endif // !defined(AFX_DLLDATAX_H__25072D60_AB2F_11D2_97EF_00A0C932A52F__INCLUDED_)

## `src/test/TCObj/pch.cpp`

PURPOSE: tiny file (11 lines), shown in full rather than summarized.

    /////////////////////////////////////////////////////////////////////////////
    // pch.cpp : source file that includes just the standard includes
    //  TCObj.pch will be the pre-compiled header
    //  pch.obj will contain the pre-compiled type information
    #include "pch.h"
    #ifdef _ATL_STATIC_REGISTRY
      #include <statreg.h>
      //#include <statreg.cpp>
    #endif
    //#include <atlimpl.cpp>
    //#include <atlwin.cpp>

## `src/test/TCObj/pch.h`

PURPOSE: This file is a precompiled header for the TCObj project in Allegiance. It includes standard system headers, project-specific headers, and third-party libraries to streamline compilation.

KEY PARTS:
- `#pragma warning (disable:4786)`: Disables warnings about identifier truncation.
- `VC_EXTRALEAN` and `_WIN32_DCOM`: Macros to exclude rarely-used Windows headers and enable DCOM features.
- `__MODULE__ "TCObj"`: Defines the module name for logging or debugging purposes.
- CRT headers (`<crtdbg.h>`): Core Runtime Library headers.
- STL headers (`<algorithm>`, `<map>`, `<set>`, `<vector>`): Standard Template Library containers and algorithms.
- Win32 headers (`<windows.h>`): Windows API declarations.
- ATL headers (`<atlbase.h>`, `<atlcom.h>`, `<atlwin.h>`): Active Template Library for COM and window classes.
- ZLib headers (`<zlib.h>`, `FixDelete.h`): Compression library and custom delete fix.

CONNECTS TO: Other project files that include this precompiled header, such as source files in the TCObj module.

NOTES: None.

## `src/test/TCObj/resource.h`

PURPOSE: tiny file (20 lines), shown in full rather than summarized.

    //{{NO_DEPENDENCIES}}
    // Microsoft Developer Studio generated include file.
    // Used by TCObj.rc
    //
    #define IDR_TCStrings                   100
    #define IDR_TCMarshalByValue            101
    #define IDR_TCNullStream                102
    #define IDR_TCPropBagOnRegKey           103
    #define IDR_REGISTRY1                   2000
    #define IDR_TCUtility                   2000
    // Next default values for new objects
    //
    #ifdef APSTUDIO_INVOKED
    #ifndef APSTUDIO_READONLY_SYMBOLS
    #define _APS_NEXT_RESOURCE_VALUE        2001
    #define _APS_NEXT_COMMAND_VALUE         32768
    #define _APS_NEXT_CONTROL_VALUE         3000
    #define _APS_NEXT_SYMED_VALUE           104
    #endif
    #endif
