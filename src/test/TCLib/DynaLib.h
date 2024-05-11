#pragma once

/////////////////////////////////////////////////////////////////////////////
// DynaLib.h | Declaration of the TCDynaLib class used to wrap the
// loading of DLL's and resolving a set of API's that may or may not exist at
// runtime.
//

#pragma warning(disable: 4786)
#include "AutoHandle.h"
#include "ObjectLock.h"
#include "AutoCriticalSection.h"


/////////////////////////////////////////////////////////////////////////////
// Encapsulates the runtime loading of DLL's and the resolving of a set of
// API exports that may or may not exist in that DLL.
//
// To use this class, first derive your own class from it.  Then, in the
// declaration of your derived class, use the TCDynaLib_Fn macros to declare
// the function prototypes of the API exports that you wish to expose from
// your derived class.
//
// The constructor of your derived class can specify the name of the DLL from
// which the API functions are exported.  This can be done either by calling
// the TCDynaLib::LoadLibrary member function, or by specifying the DLL name
// as the parameter to the second form of the base class constructor. Prior
// to calling any of the member functions that represent the API exports, you
// should call IsLibraryLoaded (or IsLibraryLoadedAndResolved) to ensure that
// the library was successfully loaded. In addition, the TCDynaLib_Fn macros
// declare a member function of the form:
//
//        bool Exists_name()
//
// where /name/ is the API export name. This member function should be called
// to check that the API export was resolved. Note that if the API export
// name was specified as being required (see TCDynaLib_REQUIRED_FUNCTION),
// then IsLibraryLoadedAndResolved will fail if *any* of the required API's
// could not be resolved.
//
// See Also: TCDynaLib_Fn macros, TCDynaLib_FnExists,
// TCDynaLib::IsLibraryLoaded, TCDynaLib::IsLibraryLoadedAndResolved,
// TCDynaLib_REQUIRED_FUNCTION
//
class TCDynaLib
{
// Construction / Destruction
public:
  TCDynaLib();
  TCDynaLib(LPCTSTR pszLibName);
  virtual ~TCDynaLib();

// Attributes
public:
  bool IsLibraryLoaded();
  bool IsLibraryLoadedAndResolved();

// Operations
public:
  void Lock();
  void Unlock();
  bool LoadLibrary(LPCTSTR pszLibName);
  FARPROC GetProcAddress(LPCSTR pszFn);
  FARPROC GetProcAddress(LPCSTR pszFn, char chDecoration, int cbArgs);

// Implementation
protected:
  static void FunctionNotFound(LPCSTR pszFn);

// Types
protected:
  // Declares an auto-scoping object used internally for thread
  // synchronization.
  typedef TCObjectLock<TCDynaLib> CLock;

// Data Members
protected:
  // Used internally for thread synchronization.
  TCAutoCriticalSection m_cs;

  // The instance handle of the currently loaded DLL, if any.
  HINSTANCE m_hinst;
};


/////////////////////////////////////////////////////////////////////////////
// Inline Operations


/////////////////////////////////////////////////////////////////////////////
// Description: Locks the object's critical section used for thread
// synchronization.
// 
// This is intended to be used by the CLock object and not to be called
// directly.
inline void TCDynaLib::Lock()
{
  m_cs.Lock();
}


/////////////////////////////////////////////////////////////////////////////
// Description: Unlocks the object's critical section used for thread
// synchronization.
// 
// This is intended to be used by the CLock object and not to be called
// directly.
inline void TCDynaLib::Unlock()
{
  m_cs.Unlock();
}


/////////////////////////////////////////////////////////////////////////////
// Macros

/////////////////////////////////////////////////////////////////////////////
// Specifies the beginning of a set of API exports that are required to be
// resolved.
//
// This is to be used within the class declaration of classes derived from
// TCDynaLib.  The API exports that are required to be in the loaded DLL will
// follow this macro, specified using the TCDynaLib_REQUIRED_FUNCTION macro,
// and ending with the TCDynaLib_END_REQUIRED_MAP macro.  As a group, these
// macros declare the member function, TCDynaLib::IsLibraryLoadedAndResolved.
//
// See Also: TCDynaLib_END_REQUIRED_MAP, TCDynaLib_REQUIRED_FUNCTION, 
// TCDynaLib::IsLibraryLoadedAndResolved
#define TCDynaLib_BEGIN_REQUIRED_MAP()                                      \
  bool IsLibraryLoadedAndResolved()                                         \
  {                                                                         \
    CLock lock(this);

/////////////////////////////////////////////////////////////////////////////
// Specifies the end of a set of API exports that are required to be
// resolved.
//
// See Also: TCDynaLib_BEGIN_REQUIRED_MAP, TCDynaLib_REQUIRED_FUNCTION, 
// TCDynaLib::IsLibraryLoadedAndResolved
#define TCDynaLib_END_REQUIRED_MAP()                                        \
    return IsLibraryLoaded();                                               \
  }

/////////////////////////////////////////////////////////////////////////////
// Specifies the name of an API export that is required to exist in the
// loaded DLL.
//
// Parameters:
//   fnName - The name of the API export that is required to be resolved.
//
// See Also: TCDynaLib_BEGIN_REQUIRED_MAP, TCDynaLib_END_REQUIRED_MAP, 
// TCDynaLib::IsLibraryLoadedAndResolved, TCDynaLib_FnExists
#define TCDynaLib_REQUIRED_FUNCTION(fnName)                                 \
  if (!Exists_##fnName())                                                   \
    return false;

/////////////////////////////////////////////////////////////////////////////
// Declares a member function that tests if an API export has been, or can
// be, resolved.
//
// This is used by the TCDynaLib_Fn macros to declare an Exists_ member
// function for each API export specified.
//
// Parameters:
//   fnName - The name of the API export to be resolved.
//
// See Also: TCDynaLib_Fn Macros, TCDynaLib_PreFnExists
#define TCDynaLib_FnExists(fnName)                                          \
  bool Exists_##fnName()                                                    \
  {                                                                         \
    if (m_pfn##fnName)                                                      \
      return true;                                                          \
    return !!(m_pfn##fnName = PFN_##fnName(GetProcAddress(#fnName)));       \
  }

/////////////////////////////////////////////////////////////////////////////
// Declares a member function that tests if an API export has been, or can
// be, resolved.
//
// This is used by the TCDynaLib_PreFn macros to declare an Exists_ member
// function for each API export specified.
//
// Parameters:
//   pre - The text, that when prefixed onto /fnName/, resolves to the name
// of the API export to be resolved.
//   fnName - The name of the member function used to call the API export.
//
// See Also: TCDynaLib_Fn Macros, TCDynaLib_FnExists
#define TCDynaLib_PreFnExists(pre, fnName)                                  \
  bool Exists_##fnName()                                                    \
  {                                                                         \
    if (m_pfn##fnName)                                                      \
      return true;                                                          \
    return !!(m_pfn##fnName = PFN_##fnName(GetProcAddress(#pre #fnName)));  \
  }


/////////////////////////////////////////////////////////////////////////////
// Declares a member function that tests if an API export has been, or can
// be, resolved.
//
// This is used by the TCDynaLib_Fn macros to declare an Exists_ member
// function for each API export specified.
//
// The possible values for the /dec/ parameter are as follows:
//   'n' - No name decoration. /cbArgs/ is ignored.
//   'c' - Decorate according to the __cdecl calling convention. /cbArgs/ is ignored.
//   's' - Decorate according to the __stdcall calling convention.
//   'f' - Decorate according to the __fastcall calling convention.
//
// Parameters:
//   dec - The calling convention used to decorate the exported name. See the
// remarks for possible values.
//   fnName - The name of the API export to be resolved.
//   cbArgs - The number of bytes of arguments passed to the export. This
// parameter is ignored when /dec/ is 'n' or 'c'.
//
// See Also: TCDynaLib_Fn Macros, TCDynaLib_PreFnExists
#define TCDynaLib_DecFnExists(dec, fnName, cbArgs)                          \
  bool Exists_##fnName()                                                    \
  {                                                                         \
    if (m_pfn##fnName)                                                      \
      return true;                                                          \
    return (m_pfn##fnName = PFN_##fnName(GetProcAddress(#fnName, dec,       \
      cbArgs)));                                                            \
  }

/////////////////////////////////////////////////////////////////////////////
// Declares a member function that tests if an API export has been, or can
// be, resolved.
//
// This is used by the TCDynaLib_PreFn macros to declare an Exists_ member
// function for each API export specified.
//
// The possible values for the /dec/ parameter are as follows:
//   'n' - No name decoration. /cbArgs/ is ignored.
//   'c' - Decorate according to the __cdecl calling convention. /cbArgs/ is ignored.
//   's' - Decorate according to the __stdcall calling convention.
//   'f' - Decorate according to the __fastcall calling convention.
//
// Parameters:
//   dec - The calling convention used to decorate the exported name. See the
//   pre - The text, that when prefixed onto /fnName/, resolves to the name
// of the API export to be resolved.
//   fnName - The name of the member function used to call the API export.
//   cbArgs - The number of bytes of arguments passed to the export. This
// parameter is ignored when /dec/ is 'n' or 'c'.
//
// See Also: TCDynaLib_Fn Macros, TCDynaLib_FnExists
#define TCDynaLib_DecPreFnExists(dec, pre, fnName, cbArgs)                  \
  bool Exists_##fnName()                                                    \
  {                                                                         \
    if (m_pfn##fnName)                                                      \
      return true;                                                          \
    return !!(m_pfn##fnName = PFN_##fnName(GetProcAddress(#pre #fnName,     \
      dec, cbArgs)));                                                       \
  }

/////////////////////////////////////////////////////////////////////////////
// {secret}
#define sizeofArg(type)                                                     \
  (((sizeof(type) + (sizeof(DWORD) - 1)) / sizeof(DWORD)) * sizeof(DWORD))

/////////////////////////////////////////////////////////////////////////////
// {partof:sizeofArg}
#define sizeof2Args(P1, P2)                                                 \
  (sizeofArg(P1) + sizeofArg(P2))

/////////////////////////////////////////////////////////////////////////////
// {partof:sizeofArg}
#define sizeof3Args(P1, P2, P3)                                             \
  (sizeofArg(P1) + sizeofArg(P2) + sizeofArg(P3))

/////////////////////////////////////////////////////////////////////////////
// {partof:sizeofArg}
#define sizeof4Args(P1, P2, P3, P4)                                         \
  (sizeofArg(P1) + sizeofArg(P2) + sizeofArg(P3) + sizeofArg(P4))

/////////////////////////////////////////////////////////////////////////////
// {partof:sizeofArg}
#define sizeof5Args(P1, P2, P3, P4, P5)                                     \
  (sizeofArg(P1) + sizeofArg(P2) + sizeofArg(P3) + sizeofArg(P4)            \
  + sizeofArg(P5))

/////////////////////////////////////////////////////////////////////////////
// {partof:sizeofArg}
#define sizeof6Args(P1, P2, P3, P4, P5, P6)                                 \
  (sizeofArg(P1) + sizeofArg(P2) + sizeofArg(P3) + sizeofArg(P4)            \
  + sizeofArg(P5) + sizeofArg(P6))

/////////////////////////////////////////////////////////////////////////////
// {partof:sizeofArg}
#define sizeof7Args(P1, P2, P3, P4, P5, P6, P7)                             \
  (sizeofArg(P1) + sizeofArg(P2) + sizeofArg(P3) + sizeofArg(P4)            \
  + sizeofArg(P5) + sizeofArg(P6) + sizeofArg(P7))

/////////////////////////////////////////////////////////////////////////////
// {partof:sizeofArg}
#define sizeof8Args(P1, P2, P3, P4, P5, P6, P7, P8)                         \
  (sizeofArg(P1) + sizeofArg(P2) + sizeofArg(P3) + sizeofArg(P4)            \
  + sizeofArg(P5) + sizeofArg(P6) + sizeofArg(P7) + sizeofArg(P8))

/////////////////////////////////////////////////////////////////////////////
// {partof:sizeofArg}
#define sizeof9Args(P1, P2, P3, P4, P5, P6, P7, P8, P9)                     \
  (sizeofArg(P1) + sizeofArg(P2) + sizeofArg(P3) + sizeofArg(P4)            \
  + sizeofArg(P5) + sizeofArg(P6) + sizeofArg(P7) + sizeofArg(P8)           \
  + sizeofArg(P9))

/////////////////////////////////////////////////////////////////////////////
// {partof:sizeofArg}
#define sizeof10Args(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10)               \
  (sizeofArg(P1) + sizeofArg(P2) + sizeofArg(P3) + sizeofArg(P4)            \
  + sizeofArg(P5) + sizeofArg(P6) + sizeofArg(P7) + sizeofArg(P8)           \
  + sizeofArg(P9) + sizeofArg(P10))

/////////////////////////////////////////////////////////////////////////////
// {partof:sizeofArg}
#define sizeof11Args(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11)          \
  (sizeofArg(P1) + sizeofArg(P2) + sizeofArg(P3) + sizeofArg(P4)            \
  + sizeofArg(P5) + sizeofArg(P6) + sizeofArg(P7) + sizeofArg(P8)           \
  + sizeofArg(P9) + sizeofArg(P10) + sizeofArg(P11))

/////////////////////////////////////////////////////////////////////////////
// {partof:sizeofArg}
#define sizeof12Args(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11, P12)     \
  (sizeofArg(P1) + sizeofArg(P2) + sizeofArg(P3) + sizeofArg(P4)            \
  + sizeofArg(P5) + sizeofArg(P6) + sizeofArg(P7) + sizeofArg(P8)           \
  + sizeofArg(P9) + sizeofArg(P10) + sizeofArg(P11) + sizeofArg(P12))

#ifdef _DOCJET_ONLY
  ///////////////////////////////////////////////////////////////////////////
  // {alias:TCDynaLib_Fn0}
  // {alias:TCDynaLib_Fn1,TCDynaLib_Fn2,TCDynaLib_Fn3,TCDynaLib_Fn4}
  // {alias:TCDynaLib_Fn5,TCDynaLib_Fn6,TCDynaLib_Fn7,TCDynaLib_Fn8}
  // {alias:TCDynaLib_Fn9,TCDynaLib_Fn10,TCDynaLib_Fn11,TCDynaLib_Fn12}
  // {alias:TCDynaLib_Fn0_void}
  // {alias:TCDynaLib_Fn1_void,TCDynaLib_Fn2_void,TCDynaLib_Fn3_void}
  // {alias:TCDynaLib_Fn4_void,TCDynaLib_Fn5_void,TCDynaLib_Fn6_void}
  // {alias:TCDynaLib_Fn7_void,TCDynaLib_Fn8_void,TCDynaLib_Fn9_void}
  // {alias:TCDynaLib_Fn10_void,TCDynaLib_Fn11_void,TCDynaLib_Fn12_void}
  //
  // Declaration:
  // TCDynaLib_Fn0(retType, name)
  // TCDynaLib_Fn1(retType, name, P1)
  // TCDynaLib_Fn2(retType, name, P1, P2)
  // TCDynaLib_Fn3(retType, name, P1, P2, P3)
  // TCDynaLib_Fn4(retType, name, P1, P2, P3, P4)
  // TCDynaLib_Fn5(retType, name, P1, P2, P3, P4, P5)
  // TCDynaLib_Fn6(retType, name, P1, P2, P3, P4, P5, P6)
  // TCDynaLib_Fn7(retType, name, P1, P2, P3, P4, P5, P6, P7)
  // TCDynaLib_Fn8(retType, name, P1, P2, P3, P4, P5, P6, P7, P8)
  // TCDynaLib_Fn9(retType, name, P1, P2, P3, P4, P5, P6, P7, P8, P9)
  // TCDynaLib_Fn10(retType, name, P1, P2, P3, P4, P5, P6, P7, P8, P9, P10)
  // TCDynaLib_Fn11(retType, name, P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11)
  // TCDynaLib_Fn12(retType, name, P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11, P12)
  //
  // TCDynaLib_Fn0_void(name)
  // TCDynaLib_Fn1_void(name, P1)
  // TCDynaLib_Fn2_void(name, P1, P2)
  // TCDynaLib_Fn3_void(name, P1, P2, P3)
  // TCDynaLib_Fn4_void(name, P1, P2, P3, P4)
  // TCDynaLib_Fn5_void(name, P1, P2, P3, P4, P5)
  // TCDynaLib_Fn6_void(name, P1, P2, P3, P4, P5, P6)
  // TCDynaLib_Fn7_void(name, P1, P2, P3, P4, P5, P6, P7)
  // TCDynaLib_Fn8_void(name, P1, P2, P3, P4, P5, P6, P7, P8)
  // TCDynaLib_Fn9_void(name, P1, P2, P3, P4, P5, P6, P7, P8, P9)
  // TCDynaLib_Fn10_void(name, P1, P2, P3, P4, P5, P6, P7, P8, P9, P10)
  // TCDynaLib_Fn11_void(name, P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11)
  // TCDynaLib_Fn12_void(name, P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11, P12)
  //
  // Macro Group: TCDynaLib_Fn Macros
  //
  // Parameters:
  //   retType - The return value type to use in the API prototype.
  //   name - The name of the API for which to declare a prototype.
  //   P1_thru_P12 - The parameter types to use in the API prototype.
  //
  // Remarks: These macros are used to declare member functions in classes
  // derived from TCDynaLib.  Each declared member function resolves the API
  // export specified by /name/ in the loaded DLL and, if successfully
  // resolved, invokes the API with the specified parameters of the member
  // function, if any.
  //
  // The *TCDyanLib_Fn0* through *TCDynaLib_Fn12* macros take /retType/ as
  // their first argument, which specifies the return type of the API export.
  // The trailing number, 0 through 12, of each macro specifies the number of
  // parameters that the API export expects.  The typenames of each
  // paramenter are to be specified in the remaining arguments of the macro.
  // One advantage to naming the macros this way is that the compiler will
  // complain if the number of parameter types specified after the return
  // type does not match the number specified in name of the macro.
  //
  // The *TCDynaLib_Fn0_void* through the *TCDynaLib_Fn12_void* macros differ
  // from the others only in that no /retType/ argument is specified,
  // indicating that the API export has no return value.
  //
  // In either case, the return type and parameter types specified should
  // match the prototype of the API export specified by /name/.
  //
  // See Also: TCDynaLib, TCDynaLib::GetProcAddress, TCDynaLib_FnExists
  #define TCDynaLib_Fn
#endif // _DOCJET_ONLY

/////////////////////////////////////////////////////////////////////////////
// {partof:TCDynaLib_Fn}
#define TCDynaLib_Fn0(retType, name)                                        \
protected:                                                                  \
  typedef retType (WINAPI *PFN_##name)();                                   \
  TCPtr<PFN_##name> m_pfn##name;                                            \
public:                                                                     \
  retType name()                                                            \
  {                                                                         \
    PFN_##name pfn = m_pfn##name;                                           \
    if (!Exists_##name())                                                   \
      FunctionNotFound(#name);                                              \
    return (*m_pfn##name)();                                                \
  }                                                                         \
  TCDynaLib_FnExists(name)

/////////////////////////////////////////////////////////////////////////////
// {partof:TCDynaLib_Fn}
#define TCDynaLib_Fn1(retType, name, P1)                                    \
protected:                                                                  \
  typedef retType (WINAPI *PFN_##name)(P1);                                 \
  TCPtr<PFN_##name> m_pfn##name;                                            \
public:                                                                     \
  retType name(P1 p1)                                                       \
  {                                                                         \
    if (!Exists_##name())                                                   \
      FunctionNotFound(#name);                                              \
    return (*m_pfn##name)(p1);                                              \
  }                                                                         \
  TCDynaLib_FnExists(name)

/////////////////////////////////////////////////////////////////////////////
// {partof:TCDynaLib_Fn}
#define TCDynaLib_Fn2(retType, name, P1, P2)                                \
protected:                                                                  \
  typedef retType (WINAPI *PFN_##name)(P1, P2);                             \
  TCPtr<PFN_##name> m_pfn##name;                                            \
public:                                                                     \
  retType name(P1 p1, P2 p2)                                                \
  {                                                                         \
    if (!Exists_##name())                                                   \
      FunctionNotFound(#name);                                              \
    return (*m_pfn##name)(p1, p2);                                          \
  }                                                                         \
  TCDynaLib_FnExists(name)

/////////////////////////////////////////////////////////////////////////////
// {partof:TCDynaLib_Fn}
#define TCDynaLib_Fn3(retType, name, P1, P2, P3)                            \
protected:                                                                  \
  typedef retType (WINAPI *PFN_##name)(P1, P2, P3);                         \
  TCPtr<PFN_##name> m_pfn##name;                                            \
public:                                                                     \
  retType name(P1 p1, P2 p2, P3 p3)                                         \
  {                                                                         \
    if (!Exists_##name())                                                   \
      FunctionNotFound(#name);                                              \
    return (*m_pfn##name)(p1, p2, p3);                                      \
  }                                                                         \
  TCDynaLib_FnExists(name)

/////////////////////////////////////////////////////////////////////////////
// {partof:TCDynaLib_Fn}
#define TCDynaLib_Fn4(retType, name, P1, P2, P3, P4)                        \
protected:                                                                  \
  typedef retType (WINAPI *PFN_##name)(P1, P2, P3, P4);                     \
  TCPtr<PFN_##name> m_pfn##name;                                            \
public:                                                                     \
  retType name(P1 p1, P2 p2, P3 p3, P4 p4)                                  \
  {                                                                         \
    if (!Exists_##name())                                                   \
      FunctionNotFound(#name);                                              \
    return (*m_pfn##name)(p1, p2, p3, p4);                                  \
  }                                                                         \
  TCDynaLib_FnExists(name)

/////////////////////////////////////////////////////////////////////////////
// {partof:TCDynaLib_Fn}
#define TCDynaLib_Fn5(retType, name, P1, P2, P3, P4, P5)                    \
protected:                                                                  \
  typedef retType (WINAPI *PFN_##name)(P1, P2, P3, P4, P5);                 \
  TCPtr<PFN_##name> m_pfn##name;                                            \
public:                                                                     \
  retType name(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5)                           \
  {                                                                         \
    if (!Exists_##name())                                                   \
      FunctionNotFound(#name);                                              \
    return (*m_pfn##name)(p1, p2, p3, p4, p5);                              \
  }                                                                         \
  TCDynaLib_FnExists(name)

/////////////////////////////////////////////////////////////////////////////
// {partof:TCDynaLib_Fn}
#define TCDynaLib_Fn6(retType, name, P1, P2, P3, P4, P5, P6)                \
protected:                                                                  \
  typedef retType (WINAPI *PFN_##name)(P1, P2, P3, P4, P5, P6);             \
  TCPtr<PFN_##name> m_pfn##name;                                            \
public:                                                                     \
  retType name(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6)                    \
  {                                                                         \
    if (!Exists_##name())                                                   \
      FunctionNotFound(#name);                                              \
    return (*m_pfn##name)(p1, p2, p3, p4, p5, p6);                          \
  }                                                                         \
  TCDynaLib_FnExists(name)

/////////////////////////////////////////////////////////////////////////////
// {partof:TCDynaLib_Fn}
#define TCDynaLib_Fn7(retType, name, P1, P2, P3, P4, P5, P6, P7)            \
protected:                                                                  \
  typedef retType (WINAPI *PFN_##name)(P1, P2, P3, P4, P5, P6, P7);         \
  TCPtr<PFN_##name> m_pfn##name;                                            \
public:                                                                     \
  retType name(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7)             \
  {                                                                         \
    if (!Exists_##name())                                                   \
      FunctionNotFound(#name);                                              \
    return (*m_pfn##name)(p1, p2, p3, p4, p5, p6, p7);                      \
  }                                                                         \
  TCDynaLib_FnExists(name)

/////////////////////////////////////////////////////////////////////////////
// {partof:TCDynaLib_Fn}
#define TCDynaLib_Fn8(retType, name, P1, P2, P3, P4, P5, P6, P7, P8)        \
protected:                                                                  \
  typedef retType (WINAPI *PFN_##name)(P1, P2, P3, P4, P5, P6, P7, P8);     \
  TCPtr<PFN_##name> m_pfn##name;                                            \
public:                                                                     \
  retType name(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8)      \
  {                                                                         \
    if (!Exists_##name())                                                   \
      FunctionNotFound(#name);                                              \
    return (*m_pfn##name)(p1, p2, p3, p4, p5, p6, p7, p8);                  \
  }                                                                         \
  TCDynaLib_FnExists(name)

/////////////////////////////////////////////////////////////////////////////
// {partof:TCDynaLib_Fn}
#define TCDynaLib_Fn9(retType, name, P1, P2, P3, P4, P5, P6, P7, P8, P9)      \
protected:                                                                    \
  typedef retType (WINAPI *PFN_##name)(P1, P2, P3, P4, P5, P6, P7, P8, P9);   \
  TCPtr<PFN_##name> m_pfn##name;                                              \
public:                                                                       \
  retType name(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8, P9 p9) \
  {                                                                           \
    if (!Exists_##name())                                                     \
      FunctionNotFound(#name);                                                \
    return (*m_pfn##name)(p1, p2, p3, p4, p5, p6, p7, p8, p9);                \
  }                                                                           \
  TCDynaLib_FnExists(name)

/////////////////////////////////////////////////////////////////////////////
// {partof:TCDynaLib_Fn}
#define TCDynaLib_Fn10(retType, name, P1, P2, P3, P4, P5, P6, P7, P8, P9, P10)         \
protected:                                                                             \
  typedef retType (WINAPI *PFN_##name)(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10);       \
  TCPtr<PFN_##name> m_pfn##name;                                                       \
public:                                                                                \
  retType name(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8, P9 p9, P10 p10) \
  {                                                                                    \
    if (!Exists_##name())                                                              \
      FunctionNotFound(#name);                                                         \
    return (*m_pfn##name)(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10);                    \
  }                                                                                    \
  TCDynaLib_FnExists(name)

/////////////////////////////////////////////////////////////////////////////
// {partof:TCDynaLib_Fn}
#define TCDynaLib_Fn11(retType, name, P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11)             \
protected:                                                                                      \
  typedef retType (WINAPI *PFN_##name)(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11);           \
  TCPtr<PFN_##name> m_pfn##name;                                                                \
public:                                                                                         \
  retType name(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8, P9 p9, P10 p10, P11 p11) \
  {                                                                                             \
    if (!Exists_##name())                                                                       \
      FunctionNotFound(#name);                                                                  \
    return (*m_pfn##name)(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11);                        \
  }                                                                                             \
  TCDynaLib_FnExists(name)

/////////////////////////////////////////////////////////////////////////////
// {partof:TCDynaLib_Fn}
#define TCDynaLib_Fn12(retType, name, P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11, P12)                 \
protected:                                                                                               \
  typedef retType (WINAPI *PFN_##name)(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11, P12);               \
  TCPtr<PFN_##name> m_pfn##name;                                                                         \
public:                                                                                                  \
  retType name(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8, P9 p9, P10 p10, P11 p11, P12 p12) \
  {                                                                                                      \
    if (!Exists_##name())                                                                                \
      FunctionNotFound(#name);                                                                           \
    return (*m_pfn##name)(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12);                            \
  }                                                                                                      \
  TCDynaLib_FnExists(name)

/////////////////////////////////////////////////////////////////////////////
// {partof:TCDynaLib_Fn}
#define TCDynaLib_Fn0_void(name)                                            \
protected:                                                                  \
  typedef void (WINAPI *PFN_##name)();                                      \
  TCPtr<PFN_##name> m_pfn##name;                                            \
public:                                                                     \
  void name()                                                               \
  {                                                                         \
    PFN_##name pfn = m_pfn##name;                                           \
    if (!Exists_##name())                                                   \
      FunctionNotFound(#name);                                              \
    (*m_pfn##name)();                                                       \
  }                                                                         \
  TCDynaLib_FnExists(name)

/////////////////////////////////////////////////////////////////////////////
// {partof:TCDynaLib_Fn}
#define TCDynaLib_Fn1_void(name, P1)                                        \
protected:                                                                  \
  typedef void (WINAPI *PFN_##name)(P1);                                    \
  TCPtr<PFN_##name> m_pfn##name;                                            \
public:                                                                     \
  void name(P1 p1)                                                          \
  {                                                                         \
    if (!Exists_##name())                                                   \
      FunctionNotFound(#name);                                              \
    (*m_pfn##name)(p1);                                                     \
  }                                                                         \
  TCDynaLib_FnExists(name)

/////////////////////////////////////////////////////////////////////////////
// {partof:TCDynaLib_Fn}
#define TCDynaLib_Fn2_void(name, P1, P2)                                    \
protected:                                                                  \
  typedef void (WINAPI *PFN_##name)(P1, P2);                                \
  TCPtr<PFN_##name> m_pfn##name;                                            \
public:                                                                     \
  void name(P1 p1, P2 p2)                                                   \
  {                                                                         \
    if (!Exists_##name())                                                   \
      FunctionNotFound(#name);                                              \
    (*m_pfn##name)(p1, p2);                                                 \
  }                                                                         \
  TCDynaLib_FnExists(name)

/////////////////////////////////////////////////////////////////////////////
// {partof:TCDynaLib_Fn}
#define TCDynaLib_Fn3_void(name, P1, P2, P3)                                \
protected:                                                                  \
  typedef void (WINAPI *PFN_##name)(P1, P2, P3);                            \
  TCPtr<PFN_##name> m_pfn##name;                                            \
public:                                                                     \
  void name(P1 p1, P2 p2, P3 p3)                                            \
  {                                                                         \
    if (!Exists_##name())                                                   \
      FunctionNotFound(#name);                                              \
    (*m_pfn##name)(p1, p2, p3);                                             \
  }                                                                         \
  TCDynaLib_FnExists(name)

/////////////////////////////////////////////////////////////////////////////
// {partof:TCDynaLib_Fn}
#define TCDynaLib_Fn4_void(name, P1, P2, P3, P4)                            \
protected:                                                                  \
  typedef void (WINAPI *PFN_##name)(P1, P2, P3, P4);                        \
  TCPtr<PFN_##name> m_pfn##name;                                            \
public:                                                                     \
  void name(P1 p1, P2 p2, P3 p3, P4 p4)                                     \
  {                                                                         \
    if (!Exists_##name())                                                   \
      FunctionNotFound(#name);                                              \
    (*m_pfn##name)(p1, p2, p3, p4);                                         \
  }                                                                         \
  TCDynaLib_FnExists(name)

/////////////////////////////////////////////////////////////////////////////
// {partof:TCDynaLib_Fn}
#define TCDynaLib_Fn5_void(name, P1, P2, P3, P4, P5)                        \
protected:                                                                  \
  typedef void (WINAPI *PFN_##name)(P1, P2, P3, P4, P5);                    \
  TCPtr<PFN_##name> m_pfn##name;                                            \
public:                                                                     \
  void name(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5)                              \
  {                                                                         \
    if (!Exists_##name())                                                   \
      FunctionNotFound(#name);                                              \
    (*m_pfn##name)(p1, p2, p3, p4, p5);                                     \
  }                                                                         \
  TCDynaLib_FnExists(name)

/////////////////////////////////////////////////////////////////////////////
// {partof:TCDynaLib_Fn}
#define TCDynaLib_Fn6_void(name, P1, P2, P3, P4, P5, P6)                    \
protected:                                                                  \
  typedef void (WINAPI *PFN_##name)(P1, P2, P3, P4, P5, P6);                \
  TCPtr<PFN_##name> m_pfn##name;                                            \
public:                                                                     \
  void name(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6)                       \
  {                                                                         \
    if (!Exists_##name())                                                   \
      FunctionNotFound(#name);                                              \
    (*m_pfn##name)(p1, p2, p3, p4, p5, p6);                                 \
  }                                                                         \
  TCDynaLib_FnExists(name)

/////////////////////////////////////////////////////////////////////////////
// {partof:TCDynaLib_Fn}
#define TCDynaLib_Fn7_void(name, P1, P2, P3, P4, P5, P6, P7)                \
protected:                                                                  \
  typedef void (WINAPI *PFN_##name)(P1, P2, P3, P4, P5, P6, P7);            \
  TCPtr<PFN_##name> m_pfn##name;                                            \
public:                                                                     \
  void name(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7)                \
  {                                                                         \
    if (!Exists_##name())                                                   \
      FunctionNotFound(#name);                                              \
    (*m_pfn##name)(p1, p2, p3, p4, p5, p6, p7);                             \
  }                                                                         \
  TCDynaLib_FnExists(name)

/////////////////////////////////////////////////////////////////////////////
// {partof:TCDynaLib_Fn}
#define TCDynaLib_Fn8_void(name, P1, P2, P3, P4, P5, P6, P7, P8)            \
protected:                                                                  \
  typedef void (WINAPI *PFN_##name)(P1, P2, P3, P4, P5, P6, P7, P8);        \
  TCPtr<PFN_##name> m_pfn##name;                                            \
public:                                                                     \
  void name(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8)         \
  {                                                                         \
    if (!Exists_##name())                                                   \
      FunctionNotFound(#name);                                              \
    (*m_pfn##name)(p1, p2, p3, p4, p5, p6, p7, p8);                         \
  }                                                                         \
  TCDynaLib_FnExists(name)

/////////////////////////////////////////////////////////////////////////////
// {partof:TCDynaLib_Fn}
#define TCDynaLib_Fn9_void(name, P1, P2, P3, P4, P5, P6, P7, P8, P9)          \
protected:                                                                    \
  typedef void (WINAPI *PFN_##name)(P1, P2, P3, P4, P5, P6, P7, P8, P9);      \
  TCPtr<PFN_##name> m_pfn##name;                                              \
public:                                                                       \
  void name(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8, P9 p9)    \
  {                                                                           \
    if (!Exists_##name())                                                     \
      FunctionNotFound(#name);                                                \
    (*m_pfn##name)(p1, p2, p3, p4, p5, p6, p7, p8, p9);                       \
  }                                                                           \
  TCDynaLib_FnExists(name)

/////////////////////////////////////////////////////////////////////////////
// {partof:TCDynaLib_Fn}
#define TCDynaLib_Fn10_void(name, P1, P2, P3, P4, P5, P6, P7, P8, P9, P10)             \
protected:                                                                             \
  typedef void (WINAPI *PFN_##name)(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10);          \
  TCPtr<PFN_##name> m_pfn##name;                                                       \
public:                                                                                \
  void name(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8, P9 p9, P10 p10)    \
  {                                                                                    \
    if (!Exists_##name())                                                              \
      FunctionNotFound(#name);                                                         \
    (*m_pfn##name)(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10);                           \
  }                                                                                    \
  TCDynaLib_FnExists(name)

/////////////////////////////////////////////////////////////////////////////
// {partof:TCDynaLib_Fn}
#define TCDynaLib_Fn11_void(name, P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11)                 \
protected:                                                                                      \
  typedef void (WINAPI *PFN_##name)(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11);              \
  TCPtr<PFN_##name> m_pfn##name;                                                                \
public:                                                                                         \
  void name(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8, P9 p9, P10 p10, P11 p11)    \
  {                                                                                             \
    if (!Exists_##name())                                                                       \
      FunctionNotFound(#name);                                                                  \
    (*m_pfn##name)(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11);                               \
  }                                                                                             \
  TCDynaLib_FnExists(name)

/////////////////////////////////////////////////////////////////////////////
// {partof:TCDynaLib_Fn}
#define TCDynaLib_Fn12_void(name, P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11, P12)                     \
protected:                                                                                               \
  typedef void (WINAPI *PFN_##name)(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11, P12);                  \
  TCPtr<PFN_##name> m_pfn##name;                                                                         \
public:                                                                                                  \
  void name(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8, P9 p9, P10 p10, P11 p11, P12 p12)    \
  {                                                                                                      \
    if (!Exists_##name())                                                                                \
      FunctionNotFound(#name);                                                                           \
    (*m_pfn##name)(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12);                                   \
  }                                                                                                      \
  TCDynaLib_FnExists(name)


/////////////////////////////////////////////////////////////////////////////
// {partof:TCDynaLib_Fn}
#define TCDynaLib_PreFn0(retType, pre, name)                                \
protected:                                                                  \
  typedef retType (WINAPI *PFN_##name)();                                   \
  TCPtr<PFN_##name> m_pfn##name;                                            \
public:                                                                     \
  retType name()                                                            \
  {                                                                         \
    PFN_##name pfn = m_pfn##name;                                           \
    if (!Exists_##name())                                                   \
      FunctionNotFound(#pre #name);                                         \
    return (*m_pfn##name)();                                                \
  }                                                                         \
  TCDynaLib_PreFnExists(pre, name)

/////////////////////////////////////////////////////////////////////////////
// {partof:TCDynaLib_Fn}
#define TCDynaLib_PreFn1(retType, pre, name, P1)                            \
protected:                                                                  \
  typedef retType (WINAPI *PFN_##name)(P1);                                 \
  TCPtr<PFN_##name> m_pfn##name;                                            \
public:                                                                     \
  retType name(P1 p1)                                                       \
  {                                                                         \
    if (!Exists_##name())                                                   \
      FunctionNotFound(#pre #name);                                         \
    return (*m_pfn##name)(p1);                                              \
  }                                                                         \
  TCDynaLib_PreFnExists(pre, name)

/////////////////////////////////////////////////////////////////////////////
// {partof:TCDynaLib_Fn}
#define TCDynaLib_PreFn2(retType, pre, name, P1, P2)                        \
protected:                                                                  \
  typedef retType (WINAPI *PFN_##name)(P1, P2);                             \
  TCPtr<PFN_##name> m_pfn##name;                                            \
public:                                                                     \
  retType name(P1 p1, P2 p2)                                                \
  {                                                                         \
    if (!Exists_##name())                                                   \
      FunctionNotFound(#pre #name);                                         \
    return (*m_pfn##name)(p1, p2);                                          \
  }                                                                         \
  TCDynaLib_PreFnExists(pre, name)

/////////////////////////////////////////////////////////////////////////////
// {partof:TCDynaLib_Fn}
#define TCDynaLib_PreFn3(retType, pre, name, P1, P2, P3)                    \
protected:                                                                  \
  typedef retType (WINAPI *PFN_##name)(P1, P2, P3);                         \
  TCPtr<PFN_##name> m_pfn##name;                                            \
public:                                                                     \
  retType name(P1 p1, P2 p2, P3 p3)                                         \
  {                                                                         \
    if (!Exists_##name())                                                   \
      FunctionNotFound(#pre #name);                                         \
    return (*m_pfn##name)(p1, p2, p3);                                      \
  }                                                                         \
  TCDynaLib_PreFnExists(pre, name)

/////////////////////////////////////////////////////////////////////////////
// {partof:TCDynaLib_Fn}
#define TCDynaLib_PreFn4(retType, pre, name, P1, P2, P3, P4)                \
protected:                                                                  \
  typedef retType (WINAPI *PFN_##name)(P1, P2, P3, P4);                     \
  TCPtr<PFN_##name> m_pfn##name;                                            \
public:                                                                     \
  retType name(P1 p1, P2 p2, P3 p3, P4 p4)                                  \
  {                                                                         \
    if (!Exists_##name())                                                   \
      FunctionNotFound(#pre #name);                                         \
    return (*m_pfn##name)(p1, p2, p3, p4);                                  \
  }                                                                         \
  TCDynaLib_PreFnExists(pre, name)

/////////////////////////////////////////////////////////////////////////////
// {partof:TCDynaLib_Fn}
#define TCDynaLib_PreFn5(retType, pre, name, P1, P2, P3, P4, P5)            \
protected:                                                                  \
  typedef retType (WINAPI *PFN_##name)(P1, P2, P3, P4, P5);                 \
  TCPtr<PFN_##name> m_pfn##name;                                            \
public:                                                                     \
  retType name(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5)                           \
  {                                                                         \
    if (!Exists_##name())                                                   \
      FunctionNotFound(#pre #name);                                         \
    return (*m_pfn##name)(p1, p2, p3, p4, p5);                              \
  }                                                                         \
  TCDynaLib_PreFnExists(pre, name)

/////////////////////////////////////////////////////////////////////////////
// {partof:TCDynaLib_Fn}
#define TCDynaLib_PreFn6(retType, pre, name, P1, P2, P3, P4, P5, P6)        \
protected:                                                                  \
  typedef retType (WINAPI *PFN_##name)(P1, P2, P3, P4, P5, P6);             \
  TCPtr<PFN_##name> m_pfn##name;                                            \
public:                                                                     \
  retType name(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6)                    \
  {                                                                         \
    if (!Exists_##name())                                                   \
      FunctionNotFound(#pre #name);                                         \
    return (*m_pfn##name)(p1, p2, p3, p4, p5, p6);                          \
  }                                                                         \
  TCDynaLib_PreFnExists(pre, name)

/////////////////////////////////////////////////////////////////////////////
// {partof:TCDynaLib_Fn}
#define TCDynaLib_PreFn7(retType, pre, name, P1, P2, P3, P4, P5, P6, P7)    \
protected:                                                                  \
  typedef retType (WINAPI *PFN_##name)(P1, P2, P3, P4, P5, P6, P7);         \
  TCPtr<PFN_##name> m_pfn##name;                                            \
public:                                                                     \
  retType name(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7)             \
  {                                                                         \
    if (!Exists_##name())                                                   \
      FunctionNotFound(#pre #name);                                         \
    return (*m_pfn##name)(p1, p2, p3, p4, p5, p6, p7);                      \
  }                                                                         \
  TCDynaLib_PreFnExists(pre, name)

/////////////////////////////////////////////////////////////////////////////
// {partof:TCDynaLib_Fn}
#define TCDynaLib_PreFn8(retType, pre, name, P1, P2, P3, P4, P5, P6, P7, P8)  \
protected:                                                                    \
  typedef retType (WINAPI *PFN_##name)(P1, P2, P3, P4, P5, P6, P7, P8);       \
  TCPtr<PFN_##name> m_pfn##name;                                              \
public:                                                                       \
  retType name(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8)        \
  {                                                                           \
    if (!Exists_##name())                                                     \
      FunctionNotFound(#pre #name);                                           \
    return (*m_pfn##name)(p1, p2, p3, p4, p5, p6, p7, p8);                    \
  }                                                                           \
  TCDynaLib_PreFnExists(pre, name)

/////////////////////////////////////////////////////////////////////////////
// {partof:TCDynaLib_Fn}
#define TCDynaLib_PreFn9(retType, pre, name, P1, P2, P3, P4, P5, P6, P7, P8, P9)  \
protected:                                                                        \
  typedef retType (WINAPI *PFN_##name)(P1, P2, P3, P4, P5, P6, P7, P8, P9);       \
  TCPtr<PFN_##name> m_pfn##name;                                                  \
public:                                                                           \
  retType name(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8, P9 p9)     \
  {                                                                               \
    if (!Exists_##name())                                                         \
      FunctionNotFound(#pre #name);                                               \
    return (*m_pfn##name)(p1, p2, p3, p4, p5, p6, p7, p8, p9);                    \
  }                                                                               \
  TCDynaLib_PreFnExists(pre, name)

/////////////////////////////////////////////////////////////////////////////
// {partof:TCDynaLib_Fn}
#define TCDynaLib_PreFn10(retType, pre, name, P1, P2, P3, P4, P5, P6, P7, P8, P9, P10)  \
protected:                                                                              \
  typedef retType (WINAPI *PFN_##name)(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10);        \
  TCPtr<PFN_##name> m_pfn##name;                                                        \
public:                                                                                 \
  retType name(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8, P9 p9, P10 p10)  \
  {                                                                                     \
    if (!Exists_##name())                                                               \
      FunctionNotFound(#pre #name);                                                     \
    return (*m_pfn##name)(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10);                     \
  }                                                                                     \
  TCDynaLib_PreFnExists(pre, name)

/////////////////////////////////////////////////////////////////////////////
// {partof:TCDynaLib_Fn}
#define TCDynaLib_PreFn11(retType, pre, name, P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11)     \
protected:                                                                                      \
  typedef retType (WINAPI *PFN_##name)(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11);           \
  TCPtr<PFN_##name> m_pfn##name;                                                                \
public:                                                                                         \
  retType name(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8, P9 p9, P10 p10, P11 p11) \
  {                                                                                             \
    if (!Exists_##name())                                                                       \
      FunctionNotFound(#pre #name);                                                             \
    return (*m_pfn##name)(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11);                        \
  }                                                                                             \
  TCDynaLib_PreFnExists(pre, name)

/////////////////////////////////////////////////////////////////////////////
// {partof:TCDynaLib_Fn}
#define TCDynaLib_PreFn12(retType, pre, name, P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11, P12)         \
protected:                                                                                               \
  typedef retType (WINAPI *PFN_##name)(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11, P12);               \
  TCPtr<PFN_##name> m_pfn##name;                                                                         \
public:                                                                                                  \
  retType name(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8, P9 p9, P10 p10, P11 p11, P12 p12) \
  {                                                                                                      \
    if (!Exists_##name())                                                                                \
      FunctionNotFound(#pre #name);                                                                      \
    return (*m_pfn##name)(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12);                            \
  }                                                                                                      \
  TCDynaLib_PreFnExists(pre, name)

/////////////////////////////////////////////////////////////////////////////
// {partof:TCDynaLib_Fn}
#define TCDynaLib_PreFn0_void(pre, name)                                    \
protected:                                                                  \
  typedef void (WINAPI *PFN_##name)();                                      \
  TCPtr<PFN_##name> m_pfn##name;                                            \
public:                                                                     \
  void name()                                                               \
  {                                                                         \
    PFN_##name pfn = m_pfn##name;                                           \
    if (!Exists_##name())                                                   \
      FunctionNotFound(#pre #name);                                         \
    (*m_pfn##name)();                                                       \
  }                                                                         \
  TCDynaLib_PreFnExists(pre, name)

/////////////////////////////////////////////////////////////////////////////
// {partof:TCDynaLib_Fn}
#define TCDynaLib_PreFn1_void(pre, name, P1)                                \
protected:                                                                  \
  typedef void (WINAPI *PFN_##name)(P1);                                    \
  TCPtr<PFN_##name> m_pfn##name;                                            \
public:                                                                     \
  void name(P1 p1)                                                          \
  {                                                                         \
    if (!Exists_##name())                                                   \
      FunctionNotFound(#pre #name);                                         \
    (*m_pfn##name)(p1);                                                     \
  }                                                                         \
  TCDynaLib_PreFnExists(pre, name)

/////////////////////////////////////////////////////////////////////////////
// {partof:TCDynaLib_Fn}
#define TCDynaLib_PreFn2_void(pre, name, P1, P2)                            \
protected:                                                                  \
  typedef void (WINAPI *PFN_##name)(P1, P2);                                \
  TCPtr<PFN_##name> m_pfn##name;                                            \
public:                                                                     \
  void name(P1 p1, P2 p2)                                                   \
  {                                                                         \
    if (!Exists_##name())                                                   \
      FunctionNotFound(#pre #name);                                         \
    (*m_pfn##name)(p1, p2);                                                 \
  }                                                                         \
  TCDynaLib_PreFnExists(pre, name)

/////////////////////////////////////////////////////////////////////////////
// {partof:TCDynaLib_Fn}
#define TCDynaLib_PreFn3_void(pre, name, P1, P2, P3)                        \
protected:                                                                  \
  typedef void (WINAPI *PFN_##name)(P1, P2, P3);                            \
  TCPtr<PFN_##name> m_pfn##name;                                            \
public:                                                                     \
  void name(P1 p1, P2 p2, P3 p3)                                            \
  {                                                                         \
    if (!Exists_##name())                                                   \
      FunctionNotFound(#pre #name);                                         \
    (*m_pfn##name)(p1, p2, p3);                                             \
  }                                                                         \
  TCDynaLib_PreFnExists(pre, name)

/////////////////////////////////////////////////////////////////////////////
// {partof:TCDynaLib_Fn}
#define TCDynaLib_PreFn4_void(pre, name, P1, P2, P3, P4)                    \
protected:                                                                  \
  typedef void (WINAPI *PFN_##name)(P1, P2, P3, P4);                        \
  TCPtr<PFN_##name> m_pfn##name;                                            \
public:                                                                     \
  void name(P1 p1, P2 p2, P3 p3, P4 p4)                                     \
  {                                                                         \
    if (!Exists_##name())                                                   \
      FunctionNotFound(#pre #name);                                         \
    (*m_pfn##name)(p1, p2, p3, p4);                                         \
  }                                                                         \
  TCDynaLib_PreFnExists(pre, name)

/////////////////////////////////////////////////////////////////////////////
// {partof:TCDynaLib_Fn}
#define TCDynaLib_PreFn5_void(pre, name, P1, P2, P3, P4, P5)                \
protected:                                                                  \
  typedef void (WINAPI *PFN_##name)(P1, P2, P3, P4, P5);                    \
  TCPtr<PFN_##name> m_pfn##name;                                            \
public:                                                                     \
  void name(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5)                              \
  {                                                                         \
    if (!Exists_##name())                                                   \
      FunctionNotFound(#pre #name);                                         \
    (*m_pfn##name)(p1, p2, p3, p4, p5);                                     \
  }                                                                         \
  TCDynaLib_PreFnExists(pre, name)

/////////////////////////////////////////////////////////////////////////////
// {partof:TCDynaLib_Fn}
#define TCDynaLib_PreFn6_void(pre, name, P1, P2, P3, P4, P5, P6)            \
protected:                                                                  \
  typedef void (WINAPI *PFN_##name)(P1, P2, P3, P4, P5, P6);                \
  TCPtr<PFN_##name> m_pfn##name;                                            \
public:                                                                     \
  void name(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6)                       \
  {                                                                         \
    if (!Exists_##name())                                                   \
      FunctionNotFound(#pre #name);                                         \
    (*m_pfn##name)(p1, p2, p3, p4, p5, p6);                                 \
  }                                                                         \
  TCDynaLib_PreFnExists(pre, name)

/////////////////////////////////////////////////////////////////////////////
// {partof:TCDynaLib_Fn}
#define TCDynaLib_PreFn7_void(pre, name, P1, P2, P3, P4, P5, P6, P7)        \
protected:                                                                  \
  typedef void (WINAPI *PFN_##name)(P1, P2, P3, P4, P5, P6, P7);            \
  TCPtr<PFN_##name> m_pfn##name;                                            \
public:                                                                     \
  void name(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7)                \
  {                                                                         \
    if (!Exists_##name())                                                   \
      FunctionNotFound(#pre #name);                                         \
    (*m_pfn##name)(p1, p2, p3, p4, p5, p6, p7);                             \
  }                                                                         \
  TCDynaLib_PreFnExists(pre, name)

/////////////////////////////////////////////////////////////////////////////
// {partof:TCDynaLib_Fn}
#define TCDynaLib_PreFn8_void(pre, name, P1, P2, P3, P4, P5, P6, P7, P8)    \
protected:                                                                  \
  typedef void (WINAPI *PFN_##name)(P1, P2, P3, P4, P5, P6, P7, P8);        \
  TCPtr<PFN_##name> m_pfn##name;                                            \
public:                                                                     \
  void name(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8)         \
  {                                                                         \
    if (!Exists_##name())                                                   \
      FunctionNotFound(#pre #name);                                         \
    (*m_pfn##name)(p1, p2, p3, p4, p5, p6, p7, p8);                         \
  }                                                                         \
  TCDynaLib_PreFnExists(pre, name)

/////////////////////////////////////////////////////////////////////////////
// {partof:TCDynaLib_Fn}
#define TCDynaLib_PreFn9_void(pre, name, P1, P2, P3, P4, P5, P6, P7, P8, P9)  \
protected:                                                                    \
  typedef void (WINAPI *PFN_##name)(P1, P2, P3, P4, P5, P6, P7, P8, P9);      \
  TCPtr<PFN_##name> m_pfn##name;                                              \
public:                                                                       \
  void name(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8, P9 p9)    \
  {                                                                           \
    if (!Exists_##name())                                                     \
      FunctionNotFound(#pre #name);                                           \
    (*m_pfn##name)(p1, p2, p3, p4, p5, p6, p7, p8, p9);                       \
  }                                                                           \
  TCDynaLib_PreFnExists(pre, name)

/////////////////////////////////////////////////////////////////////////////
// {partof:TCDynaLib_Fn}
#define TCDynaLib_PreFn10_void(pre, name, P1, P2, P3, P4, P5, P6, P7, P8, P9, P10)     \
protected:                                                                             \
  typedef void (WINAPI *PFN_##name)(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10);          \
  TCPtr<PFN_##name> m_pfn##name;                                                       \
public:                                                                                \
  void name(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8, P9 p9, P10 p10)    \
  {                                                                                    \
    if (!Exists_##name())                                                              \
      FunctionNotFound(#pre #name);                                                    \
    (*m_pfn##name)(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10);                           \
  }                                                                                    \
  TCDynaLib_PreFnExists(pre, name)

/////////////////////////////////////////////////////////////////////////////
// {partof:TCDynaLib_Fn}
#define TCDynaLib_PreFn11_void(pre, name, P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11)         \
protected:                                                                                      \
  typedef void (WINAPI *PFN_##name)(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11);              \
  TCPtr<PFN_##name> m_pfn##name;                                                                \
public:                                                                                         \
  void name(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8, P9 p9, P10 p10, P11 p11)    \
  {                                                                                             \
    if (!Exists_##name())                                                                       \
      FunctionNotFound(#pre #name);                                                             \
    (*m_pfn##name)(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11);                               \
  }                                                                                             \
  TCDynaLib_PreFnExists(pre, name)

/////////////////////////////////////////////////////////////////////////////
// {partof:TCDynaLib_Fn}
#define TCDynaLib_PreFn12_void(pre, name, P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11, P12)             \
protected:                                                                                               \
  typedef void (WINAPI *PFN_##name)(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11, P12);                  \
  TCPtr<PFN_##name> m_pfn##name;                                                                         \
public:                                                                                                  \
  void name(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8, P9 p9, P10 p10, P11 p11, P12 p12)    \
  {                                                                                                      \
    if (!Exists_##name())                                                                                \
      FunctionNotFound(#pre #name);                                                                      \
    (*m_pfn##name)(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12);                                   \
  }                                                                                                      \
  TCDynaLib_PreFnExists(pre, name)


/////////////////////////////////////////////////////////////////////////////
// {partof:TCDynaLib_Fn}
#define TCDynaLib_DecPreFn0(retType, dec, pre, name)                        \
protected:                                                                  \
  typedef retType (WINAPI *PFN_##name)();                                   \
  TCPtr<PFN_##name> m_pfn##name;                                            \
public:                                                                     \
  retType name()                                                            \
  {                                                                         \
    PFN_##name pfn = m_pfn##name;                                           \
    if (!Exists_##name())                                                   \
      FunctionNotFound(#pre #name);                                         \
    return (*m_pfn##name)();                                                \
  }                                                                         \
  TCDynaLib_DecPreFnExists(dec, pre, name, 0)

/////////////////////////////////////////////////////////////////////////////
// {partof:TCDynaLib_Fn}
#define TCDynaLib_DecPreFn1(retType, dec, pre, name, P1)                    \
protected:                                                                  \
  typedef retType (WINAPI *PFN_##name)(P1);                                 \
  TCPtr<PFN_##name> m_pfn##name;                                            \
public:                                                                     \
  retType name(P1 p1)                                                       \
  {                                                                         \
    if (!Exists_##name())                                                   \
      FunctionNotFound(#pre #name);                                         \
    return (*m_pfn##name)(p1);                                              \
  }                                                                         \
  TCDynaLib_DecPreFnExists(dec, pre, name, sizeofArg(P1))

/////////////////////////////////////////////////////////////////////////////
// {partof:TCDynaLib_Fn}
#define TCDynaLib_DecPreFn2(retType, dec, pre, name, P1, P2)                \
protected:                                                                  \
  typedef retType (WINAPI *PFN_##name)(P1, P2);                             \
  TCPtr<PFN_##name> m_pfn##name;                                            \
public:                                                                     \
  retType name(P1 p1, P2 p2)                                                \
  {                                                                         \
    if (!Exists_##name())                                                   \
      FunctionNotFound(#pre #name);                                         \
    return (*m_pfn##name)(p1, p2);                                          \
  }                                                                         \
  TCDynaLib_DecPreFnExists(dec, pre, name, sizeof2Args(P1, P2))

/////////////////////////////////////////////////////////////////////////////
// {partof:TCDynaLib_Fn}
#define TCDynaLib_DecPreFn3(retType, dec, pre, name, P1, P2, P3)            \
protected:                                                                  \
  typedef retType (WINAPI *PFN_##name)(P1, P2, P3);                         \
  TCPtr<PFN_##name> m_pfn##name;                                            \
public:                                                                     \
  retType name(P1 p1, P2 p2, P3 p3)                                         \
  {                                                                         \
    if (!Exists_##name())                                                   \
      FunctionNotFound(#pre #name);                                         \
    return (*m_pfn##name)(p1, p2, p3);                                      \
  }                                                                         \
  TCDynaLib_DecPreFnExists(dec, pre, name, sizeof3Args(P1, P2, P3))

/////////////////////////////////////////////////////////////////////////////
// {partof:TCDynaLib_Fn}
#define TCDynaLib_DecPreFn4(retType, dec, pre, name, P1, P2, P3, P4)        \
protected:                                                                  \
  typedef retType (WINAPI *PFN_##name)(P1, P2, P3, P4);                     \
  TCPtr<PFN_##name> m_pfn##name;                                            \
public:                                                                     \
  retType name(P1 p1, P2 p2, P3 p3, P4 p4)                                  \
  {                                                                         \
    if (!Exists_##name())                                                   \
      FunctionNotFound(#pre #name);                                         \
    return (*m_pfn##name)(p1, p2, p3, p4);                                  \
  }                                                                         \
  TCDynaLib_DecPreFnExists(dec, pre, name, sizeof4Args(P1, P2, P3, P4))

/////////////////////////////////////////////////////////////////////////////
// {partof:TCDynaLib_Fn}
#define TCDynaLib_DecPreFn5(retType, dec, pre, name, P1, P2, P3, P4, P5)    \
protected:                                                                  \
  typedef retType (WINAPI *PFN_##name)(P1, P2, P3, P4, P5);                 \
  TCPtr<PFN_##name> m_pfn##name;                                            \
public:                                                                     \
  retType name(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5)                           \
  {                                                                         \
    if (!Exists_##name())                                                   \
      FunctionNotFound(#pre #name);                                         \
    return (*m_pfn##name)(p1, p2, p3, p4, p5);                              \
  }                                                                         \
  TCDynaLib_DecPreFnExists(dec, pre, name, sizeof5Args(P1, P2, P3, P4, P5))

/////////////////////////////////////////////////////////////////////////////
// {partof:TCDynaLib_Fn}
#define TCDynaLib_DecPreFn6(retType, dec, pre, name, P1, P2, P3, P4, P5, P6) \
protected:                                                                   \
  typedef retType (WINAPI *PFN_##name)(P1, P2, P3, P4, P5, P6);              \
  TCPtr<PFN_##name> m_pfn##name;                                             \
public:                                                                      \
  retType name(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6)                     \
  {                                                                          \
    if (!Exists_##name())                                                    \
      FunctionNotFound(#pre #name);                                          \
    return (*m_pfn##name)(p1, p2, p3, p4, p5, p6);                           \
  }                                                                          \
  TCDynaLib_DecPreFnExists(dec, pre, name, sizeof6Args(P1, P2, P3, P4, P5,   \
    P6))

/////////////////////////////////////////////////////////////////////////////
// {partof:TCDynaLib_Fn}
#define TCDynaLib_DecPreFn7(retType, dec, pre, name, P1, P2, P3, P4, P5, P6, P7) \
protected:                                                                       \
  typedef retType (WINAPI *PFN_##name)(P1, P2, P3, P4, P5, P6, P7);              \
  TCPtr<PFN_##name> m_pfn##name;                                                 \
public:                                                                          \
  retType name(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7)                  \
  {                                                                              \
    if (!Exists_##name())                                                        \
      FunctionNotFound(#pre #name);                                              \
    return (*m_pfn##name)(p1, p2, p3, p4, p5, p6, p7);                           \
  }                                                                              \
  TCDynaLib_DecPreFnExists(dec, pre, name, sizeof7Args(P1, P2, P3, P4, P5,       \
    P6, P7))

/////////////////////////////////////////////////////////////////////////////
// {partof:TCDynaLib_Fn}
#define TCDynaLib_DecPreFn8(retType, dec, pre, name, P1, P2, P3, P4, P5, P6, P7, P8) \
protected:                                                                           \
  typedef retType (WINAPI *PFN_##name)(P1, P2, P3, P4, P5, P6, P7, P8);              \
  TCPtr<PFN_##name> m_pfn##name;                                                     \
public:                                                                              \
  retType name(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8)               \
  {                                                                                  \
    if (!Exists_##name())                                                            \
      FunctionNotFound(#pre #name);                                                  \
    return (*m_pfn##name)(p1, p2, p3, p4, p5, p6, p7, p8);                           \
  }                                                                                  \
  TCDynaLib_DecPreFnExists(dec, pre, name, sizeof8Args(P1, P2, P3, P4, P5,           \
    P6, P7, P8))

/////////////////////////////////////////////////////////////////////////////
// {partof:TCDynaLib_Fn}
#define TCDynaLib_DecPreFn9(retType, dec, pre, name, P1, P2, P3, P4, P5, P6, P7, P8, P9) \
protected:                                                                               \
  typedef retType (WINAPI *PFN_##name)(P1, P2, P3, P4, P5, P6, P7, P8, P9);              \
  TCPtr<PFN_##name> m_pfn##name;                                                         \
public:                                                                                  \
  retType name(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8, P9 p9)            \
  {                                                                                      \
    if (!Exists_##name())                                                                \
      FunctionNotFound(#pre #name);                                                      \
    return (*m_pfn##name)(p1, p2, p3, p4, p5, p6, p7, p8, p9);                           \
  }                                                                                      \
  TCDynaLib_DecPreFnExists(dec, pre, name, sizeof9Args(P1, P2, P3, P4, P5,               \
    P6, P7, P8, P9))

/////////////////////////////////////////////////////////////////////////////
// {partof:TCDynaLib_Fn}
#define TCDynaLib_DecPreFn10(retType, dec, pre, name, P1, P2, P3, P4, P5, P6, P7, P8, P9, P10) \
protected:                                                                                     \
  typedef retType (WINAPI *PFN_##name)(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10);               \
  TCPtr<PFN_##name> m_pfn##name;                                                               \
public:                                                                                        \
  retType name(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8, P9 p9, P10 p10)         \
  {                                                                                            \
    if (!Exists_##name())                                                                      \
      FunctionNotFound(#pre #name);                                                            \
    return (*m_pfn##name)(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10);                            \
  }                                                                                            \
  TCDynaLib_DecPreFnExists(dec, pre, name, sizeof10Args(P1, P2, P3, P4, P5,                    \
    P6, P7, P8, P9, P10))

/////////////////////////////////////////////////////////////////////////////
// {partof:TCDynaLib_Fn}
#define TCDynaLib_DecPreFn11(retType, dec, pre, name, P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11) \
protected:                                                                                          \
  typedef retType (WINAPI *PFN_##name)(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11);               \
  TCPtr<PFN_##name> m_pfn##name;                                                                    \
public:                                                                                             \
  retType name(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8, P9 p9, P10 p10, P11 p11)     \
  {                                                                                                 \
    if (!Exists_##name())                                                                           \
      FunctionNotFound(#pre #name);                                                                 \
    return (*m_pfn##name)(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11);                            \
  }                                                                                                 \
  TCDynaLib_DecPreFnExists(dec, pre, name, sizeof11Args(P1, P2, P3, P4, P5,                         \
    P6, P7, P8, P9, P10, P11))

/////////////////////////////////////////////////////////////////////////////
// {partof:TCDynaLib_Fn}
#define TCDynaLib_DecPreFn12(retType, dec, pre, name, P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11, P12) \
protected:                                                                                               \
  typedef retType (WINAPI *PFN_##name)(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11, P12);               \
  TCPtr<PFN_##name> m_pfn##name;                                                                         \
public:                                                                                                  \
  retType name(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8, P9 p9, P10 p10, P11 p11, P12 p12) \
  {                                                                                                      \
    if (!Exists_##name())                                                                                \
      FunctionNotFound(#pre #name);                                                                      \
    return (*m_pfn##name)(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12);                            \
  }                                                                                                      \
  TCDynaLib_DecPreFnExists(dec, pre, name, sizeof12Args(P1, P2, P3, P4, P5,                              \
    P6, P7, P8, P9, P10, P11, P12))

/////////////////////////////////////////////////////////////////////////////
// {partof:TCDynaLib_Fn}
#define TCDynaLib_DecPreFn0_void(dec, pre, name)                            \
protected:                                                                  \
  typedef void (WINAPI *PFN_##name)();                                      \
  TCPtr<PFN_##name> m_pfn##name;                                            \
public:                                                                     \
  void name()                                                               \
  {                                                                         \
    PFN_##name pfn = m_pfn##name;                                           \
    if (!Exists_##name())                                                   \
      FunctionNotFound(#pre #name);                                         \
    (*m_pfn##name)();                                                       \
  }                                                                         \
  TCDynaLib_DecPreFnExists(dec, pre, name, 0)

/////////////////////////////////////////////////////////////////////////////
// {partof:TCDynaLib_Fn}
#define TCDynaLib_DecPreFn1_void(dec, pre, name, P1)                        \
protected:                                                                  \
  typedef void (WINAPI *PFN_##name)(P1);                                    \
  TCPtr<PFN_##name> m_pfn##name;                                            \
public:                                                                     \
  void name(P1 p1)                                                          \
  {                                                                         \
    if (!Exists_##name())                                                   \
      FunctionNotFound(#pre #name);                                         \
    (*m_pfn##name)(p1);                                                     \
  }                                                                         \
  TCDynaLib_DecPreFnExists(dec, pre, name, sizeofArg(P1))

/////////////////////////////////////////////////////////////////////////////
// {partof:TCDynaLib_Fn}
#define TCDynaLib_DecPreFn2_void(dec, pre, name, P1, P2)                    \
protected:                                                                  \
  typedef void (WINAPI *PFN_##name)(P1, P2);                                \
  TCPtr<PFN_##name> m_pfn##name;                                            \
public:                                                                     \
  void name(P1 p1, P2 p2)                                                   \
  {                                                                         \
    if (!Exists_##name())                                                   \
      FunctionNotFound(#pre #name);                                         \
    (*m_pfn##name)(p1, p2);                                                 \
  }                                                                         \
  TCDynaLib_DecPreFnExists(dec, pre, name, sizeof2Args(P1, P2))

/////////////////////////////////////////////////////////////////////////////
// {partof:TCDynaLib_Fn}
#define TCDynaLib_DecPreFn3_void(dec, pre, name, P1, P2, P3)                \
protected:                                                                  \
  typedef void (WINAPI *PFN_##name)(P1, P2, P3);                            \
  TCPtr<PFN_##name> m_pfn##name;                                            \
public:                                                                     \
  void name(P1 p1, P2 p2, P3 p3)                                            \
  {                                                                         \
    if (!Exists_##name())                                                   \
      FunctionNotFound(#pre #name);                                         \
    (*m_pfn##name)(p1, p2, p3);                                             \
  }                                                                         \
  TCDynaLib_DecPreFnExists(dec, pre, name, sizeof3Args(P1, P2, P3))

/////////////////////////////////////////////////////////////////////////////
// {partof:TCDynaLib_Fn}
#define TCDynaLib_DecPreFn4_void(dec, pre, name, P1, P2, P3, P4)            \
protected:                                                                  \
  typedef void (WINAPI *PFN_##name)(P1, P2, P3, P4);                        \
  TCPtr<PFN_##name> m_pfn##name;                                            \
public:                                                                     \
  void name(P1 p1, P2 p2, P3 p3, P4 p4)                                     \
  {                                                                         \
    if (!Exists_##name())                                                   \
      FunctionNotFound(#pre #name);                                         \
    (*m_pfn##name)(p1, p2, p3, p4);                                         \
  }                                                                         \
  TCDynaLib_DecPreFnExists(dec, pre, name, sizeof4Args(P1, P2, P3, P4))

/////////////////////////////////////////////////////////////////////////////
// {partof:TCDynaLib_Fn}
#define TCDynaLib_DecPreFn5_void(dec, pre, name, P1, P2, P3, P4, P5)        \
protected:                                                                  \
  typedef void (WINAPI *PFN_##name)(P1, P2, P3, P4, P5);                    \
  TCPtr<PFN_##name> m_pfn##name;                                            \
public:                                                                     \
  void name(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5)                              \
  {                                                                         \
    if (!Exists_##name())                                                   \
      FunctionNotFound(#pre #name);                                         \
    (*m_pfn##name)(p1, p2, p3, p4, p5);                                     \
  }                                                                         \
  TCDynaLib_DecPreFnExists(dec, pre, name, sizeof5Args(P1, P2, P3, P4, P5))

/////////////////////////////////////////////////////////////////////////////
// {partof:TCDynaLib_Fn}
#define TCDynaLib_DecPreFn6_void(dec, pre, name, P1, P2, P3, P4, P5, P6)    \
protected:                                                                  \
  typedef void (WINAPI *PFN_##name)(P1, P2, P3, P4, P5, P6);                \
  TCPtr<PFN_##name> m_pfn##name;                                            \
public:                                                                     \
  void name(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6)                       \
  {                                                                         \
    if (!Exists_##name())                                                   \
      FunctionNotFound(#pre #name);                                         \
    (*m_pfn##name)(p1, p2, p3, p4, p5, p6);                                 \
  }                                                                         \
  TCDynaLib_DecPreFnExists(dec, pre, name, sizeof6Args(P1, P2, P3, P4, P5,  \
    P6))

/////////////////////////////////////////////////////////////////////////////
// {partof:TCDynaLib_Fn}
#define TCDynaLib_DecPreFn7_void(dec, pre, name, P1, P2, P3, P4, P5, P6, P7)  \
protected:                                                                    \
  typedef void (WINAPI *PFN_##name)(P1, P2, P3, P4, P5, P6, P7);              \
  TCPtr<PFN_##name> m_pfn##name;                                              \
public:                                                                       \
  void name(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7)                  \
  {                                                                           \
    if (!Exists_##name())                                                     \
      FunctionNotFound(#pre #name);                                           \
    (*m_pfn##name)(p1, p2, p3, p4, p5, p6, p7);                               \
  }                                                                           \
  TCDynaLib_DecPreFnExists(dec, pre, name, sizeof7Args(P1, P2, P3, P4, P5,    \
    P6, P7))

/////////////////////////////////////////////////////////////////////////////
// {partof:TCDynaLib_Fn}
#define TCDynaLib_DecPreFn8_void(dec, pre, name, P1, P2, P3, P4, P5, P6, P7, P8)  \
protected:                                                                        \
  typedef void (WINAPI *PFN_##name)(P1, P2, P3, P4, P5, P6, P7, P8);              \
  TCPtr<PFN_##name> m_pfn##name;                                                  \
public:                                                                           \
  void name(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8)               \
  {                                                                               \
    if (!Exists_##name())                                                         \
      FunctionNotFound(#pre #name);                                               \
    (*m_pfn##name)(p1, p2, p3, p4, p5, p6, p7, p8);                               \
  }                                                                               \
  TCDynaLib_DecPreFnExists(dec, pre, name, sizeof8Args(P1, P2, P3, P4, P5,        \
    P6, P7, P8))

/////////////////////////////////////////////////////////////////////////////
// {partof:TCDynaLib_Fn}
#define TCDynaLib_DecPreFn9_void(dec, pre, name, P1, P2, P3, P4, P5, P6, P7, P8, P9)  \
protected:                                                                            \
  typedef void (WINAPI *PFN_##name)(P1, P2, P3, P4, P5, P6, P7, P8, P9);              \
  TCPtr<PFN_##name> m_pfn##name;                                                      \
public:                                                                               \
  void name(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8, P9 p9)            \
  {                                                                                   \
    if (!Exists_##name())                                                             \
      FunctionNotFound(#pre #name);                                                   \
    (*m_pfn##name)(p1, p2, p3, p4, p5, p6, p7, p8, p9);                               \
  }                                                                                   \
  TCDynaLib_DecPreFnExists(dec, pre, name, sizeof9Args(P1, P2, P3, P4, P5,            \
    P6, P7, P8, P9))

/////////////////////////////////////////////////////////////////////////////
// {partof:TCDynaLib_Fn}
#define TCDynaLib_DecPreFn10_void(dec, pre, name, P1, P2, P3, P4, P5, P6, P7, P8, P9, P10)  \
protected:                                                                                  \
  typedef void (WINAPI *PFN_##name)(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10);               \
  TCPtr<PFN_##name> m_pfn##name;                                                            \
public:                                                                                     \
  void name(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8, P9 p9, P10 p10)         \
  {                                                                                         \
    if (!Exists_##name())                                                                   \
      FunctionNotFound(#pre #name);                                                         \
    (*m_pfn##name)(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10);                                \
  }                                                                                         \
  TCDynaLib_DecPreFnExists(dec, pre, name, sizeof10Args(P1, P2, P3, P4, P5,                 \
    P6, P7, P8, P9, P10))

/////////////////////////////////////////////////////////////////////////////
// {partof:TCDynaLib_Fn}
#define TCDynaLib_DecPreFn11_void(dec, pre, name, P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11) \
protected:                                                                                      \
  typedef void (WINAPI *PFN_##name)(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11);              \
  TCPtr<PFN_##name> m_pfn##name;                                                                \
public:                                                                                         \
  void name(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8, P9 p9, P10 p10, P11 p11)    \
  {                                                                                             \
    if (!Exists_##name())                                                                       \
      FunctionNotFound(#pre #name);                                                             \
    (*m_pfn##name)(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11);                               \
  }                                                                                             \
  TCDynaLib_DecPreFnExists(dec, pre, name, sizeof11Args(P1, P2, P3, P4, P5,                     \
    P6, P7, P8, P9, P10, P11))

/////////////////////////////////////////////////////////////////////////////
// {partof:TCDynaLib_Fn}
#define TCDynaLib_DecPreFn12_void(dec, pre, name, P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11, P12)     \
protected:                                                                                               \
  typedef void (WINAPI *PFN_##name)(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11, P12);                  \
  TCPtr<PFN_##name> m_pfn##name;                                                                         \
public:                                                                                                  \
  void name(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8, P9 p9, P10 p10, P11 p11, P12 p12)    \
  {                                                                                                      \
    if (!Exists_##name())                                                                                \
      FunctionNotFound(#pre #name);                                                                      \
    (*m_pfn##name)(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12);                                   \
  }                                                                                                      \
  TCDynaLib_DecPreFnExists(dec, pre, name, sizeof12Args(P1, P2, P3, P4, P5,                              \
    P6, P7, P8, P9, P10, P11, P12))


/////////////////////////////////////////////////////////////////////////////