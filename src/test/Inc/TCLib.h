#ifndef __TCLib_h__
#define __TCLib_h__

/////////////////////////////////////////////////////////////////////////////
// {group:Class Libraries}
// TCLib.h | Declarations and definitions of extensions to C++ Runtime
// Library.  These declarations and definitions are to be independent from
// both the Active Template Library (ATL) and the Microsoft Foundation Class
// (MFC) frameworks, but are also to be compatible with them.
//
// The files in this library are intended to be used in much the same way as
// the files in the ATL framework.  The TCLib.h file should be included
// from a project's pch.h file (or the functional equivalent), while the
// TCLib.cpp file should be included by ONLY ONE module in the project.
// Usually this will be the pch.cpp file (or the functional equivalent).
//

// Disable this warning since STL may generate some very long symbolic names
#pragma warning(disable: 4786)
#include <typeinfo>
#include <comdef.h>
#include <crtdbg.h>
#include <zassert.h>


/////////////////////////////////////////////////////////////////////////////
// Macros

#ifndef sizeofArray
  ///////////////////////////////////////////////////////////////////////////
  // Counts the number of elements in a fixed-length array.
  // Parameters:  x - The name of the array of which to compute the size.
  #define sizeofArray(x) (sizeof(x) / sizeof(x[0]))
#endif // !sizeofArray


///////////////////////////////////////////////////////////////////////////
// High order bit extraction.
// Parameters:
//   x - value to be shifted.
//   nBitCount - number of high order bits to be shifted into the low order
//   bits.
//
// Return Value: A value of type, /T/, shifted to the right so that the
// specified number of high order bits are now in the low order bits.
//
// See Also: HiBit, LoBits, LoBit
template <class T>
inline T HiBits(T x, unsigned nBitCount)
  {return x >> (sizeof(T) * 8 - nBitCount);}

///////////////////////////////////////////////////////////////////////////
// High order bit extraction.
// Parameters:
//   x - value to be shifted.
//
// Return Value: A value of type, /T/, shifted to the right so that the
// most significant bit is now the least significant bit.
//
// See Also: HiBits, LoBit, LoBits
template <class T>
inline T HiBit(T x)
  {return x >> (sizeof(T) * 8 - 1);}

///////////////////////////////////////////////////////////////////////////
// Low order bit extraction.
// Parameters:
//   x - value to be masked.
//   nBitCount - number of low order bits to be masked.
//
// Return Value: A value of type /T/, masked so that only the specified
// number of low order bits are preserved from the original value, /x/. The
// remaining high order bits are all set to zero.
// See Also: LoBit, HiBits, HiBit
template <class T>
inline T LoBits(T x, unsigned nBitCount)
  {return x & ((1 < nBitCount) - 1);}

///////////////////////////////////////////////////////////////////////////
// Low order bit extraction.
// Parameters:
//   x - value to be masked.
//
// Return Value: A value of type /T/, masked so that only the least
// significant bit is preserved from the original value, /x/. The
// remaining high order bits are all set to zero.
// See Also: LoBits, HiBit, HiBits
template <class T>
inline T LoBit(T x)
  {return x & 0x01;}


///////////////////////////////////////////////////////////////////////////
// Translates a boolean expression to a valid VARIANT_BOOL.
//
// Parameters:  expression - boolean expression to be translated.
//
// Return Value: *VARIANT_TRUE* (short(-1)) if the expression is non-zero,
// otherwise *VARIANT_FALSE* (short(0)).
#define VARBOOL(expression) \
  VARIANT_BOOL((expression) ? VARIANT_TRUE : VARIANT_FALSE)


///////////////////////////////////////////////////////////////////////////
// Sets an [out] parameter to an initial value.  The assignment is
// performed within a *__try* block of an exception handler. The
// corresponding *__except* block of the exception handler simply returns
// E_POINTER.
//
// Return Value: *S_OK* if the assignment succeeded, otherwise *E_POINTER*.
//
// Parameters:
//   dest - Address of variable to initialize.
//   init - Value to be assigned to /dest/.
//
// See Also: CLEAROUT, CLEAROUT_ALLOW_NULL
template <class T>
inline HRESULT ClearOut(T* dest, const T& init)
{
  __try
  {
    *dest = init;
    return S_OK;
  }
  __except(1)
  {
    return E_POINTER;
  }
}


///////////////////////////////////////////////////////////////////////////
// Macro wrapper for the ClearOut function that will return if the ClearOut
// function fails.
//
// Parameters:
//   dest - Address of variable to initialize.
//   init - Value to be assigned to /dest/.
//
// See Also: ClearOut, CLEAROUT_ALLOW_NULL
#define CLEAROUT(dest, init) \
{ \
  HRESULT _hr = ClearOut(dest, init); \
  if (FAILED(_hr)) \
    return _hr; \
}


///////////////////////////////////////////////////////////////////////////
// Macro wrapper for the ClearOut function that will return if the ClearOut
// function fails.  If /dest/ is equal to NULL, the macro does not attempt
// to initialize it.
//
// Parameters:
//   dest - Address of variable to initialize.
//   init - Value to be assigned to /dest/.
//
// See Also: ClearOut, CLEAROUT
#define CLEAROUT_ALLOW_NULL(dest, init) \
  if (NULL != dest) \
    CLEAROUT(dest, init)


///////////////////////////////////////////////////////////////////////////
// Initializes a VARIANT* [out] parameter.  The initialization is performed
// within a *__try* block of an exception handler. The corresponding
// *__except* block of the exception handler simply returns E_POINTER.
//
// Return Value: *S_OK* if the initialization succeeded, otherwise
// *E_POINTER*.
//
// Parameters:
//   pvar - Address of variant to initialize.
//
// See Also: INIT_VARIANT_OUT, CLEAROUT
inline HRESULT InitVariantOut(VARIANT* pvar)
{
  __try
  {
    ::VariantInit(pvar);
    return S_OK;
  }
  __except(1)
  {
    return E_POINTER;
  }
}


///////////////////////////////////////////////////////////////////////////
// Macro wrapper for the InitVariantOut function that will return if the
// InitVariantOut function fails.
//
// Parameters:
//   pvar - Address of variant to initialize.
//
// See Also: InitVariantOut, CLEAROUT
#define INIT_VARIANT_OUT(pvar)        \
do                                    \
{                                     \
  HRESULT _hr = InitVariantOut(pvar); \
  if (FAILED(_hr))                    \
    return _hr;                       \
} while (0)


#ifdef _DEBUG

  /////////////////////////////////////////////////////////////////////////
  // Outputs a debug string under _DEBUG builds indicating that the module
  // is initializing or terminating.
  // Parameters:
  //   hinst - Module instance that is initializing or terminating.
  //   bInit - *true* if module is initializing, otherwise *false*,
  // specifying that the module is terminating.
  inline void TRACE_MODULE_INIT(HINSTANCE hinst, bool bInit)
  {
    TCHAR szModule[_MAX_PATH + 32];
    DWORD cch = GetModuleFileName(hinst, szModule, sizeofArray(szModule));
    LPTSTR pszBackSlash = szModule + cch;
    while (*pszBackSlash != TEXT('\\') && pszBackSlash > szModule)
      --pszBackSlash;
    *pszBackSlash = TEXT('\0');

    TCHAR szInit[] = TEXT(" Initialized\n");
    TCHAR szTerm[sizeofArray(szInit)] = TEXT(" Terminated\n");
    CopyMemory(szModule + cch, bInit ? szInit : szTerm, sizeof(szInit));
    USES_CONVERSION;
    ZDebugOutputImpl(T2CA(pszBackSlash + 1));
  }

#else // _DEBUG

  #define TRACE_MODULE_INIT(hinst, bInit)

#endif // _DEBUG


/////////////////////////////////////////////////////////////////////////////
// An exception-safe and type-safe way to clear a block of memory.
//
template <class T>
inline HRESULT TCZeroMemory(T* p, unsigned cb = sizeof(T))
{
  __try
  {
    ::ZeroMemory(p, cb);
    return S_OK;
  }
  __except(1)
  {
    return E_POINTER;
  }
}


/////////////////////////////////////////////////////////////////////////////
// An exception-safe and type-safe way to QueryInterface an IUnknown pointer.
//
template <class TIID>
inline HRESULT TCSafeQI(IUnknown* punk, TIID** ppOut)
{
  __try
  {
    return punk->QueryInterface(__uuidof(**ppOut), (void**)ppOut);
  }
  __except(1)
  {
    return E_POINTER;
  }
}


/////////////////////////////////////////////////////////////////////////////
// 64-bit Integer Types

typedef __int64 TCINT64;
typedef unsigned __int64 TCUINT64;


/////////////////////////////////////////////////////////////////////////////
// FILETIME Conversions

inline TCUINT64 FileTimeToUINT64(const FILETIME& ft)
{
  #ifdef _M_IX86
    return *((const TCUINT64*)(&ft));
  #else
    return ((TCUINT64)ft.dwHighDateTime << 32) | ft.dwLowDateTime;
  #endif
}

inline void UINT64ToFileTime(TCUINT64 n64, FILETIME& ft)
{
  #ifdef _M_IX86
    ft = *((FILETIME*)(&n64));
  #else
    ft.dwLowDateTime = (DWORD)(n64);
    ft.dwHighDateTime = (DWORD)(n64 >> 32);
  #endif
}

inline TCUINT64 GetSystemTimeAsUINT64()
{
  FILETIME ft;
  GetSystemTimeAsFileTime(&ft);
  return FileTimeToUINT64(ft);
}

inline DWORD FileTimeToMs(TCUINT64 n64)
{
  return (DWORD)(n64 / TCUINT64(10000));
}

inline DWORD FileTimeToMs(const FILETIME& ft)
{
  return FileTimeToMs(FileTimeToUINT64(ft));
}


/////////////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
  ///////////////////////////////////////////////////////////////////////////
  // Macro Group: _VERIFY, _VERIFYE Macros
  //
  // These macros evaluate an expression and generate a debug report when the
  // result is FALSE (debug version only). Under release builds, the
  // expression is still evaluated, but no debug report is generated.
  //
  // Declaration:
  // #ifdef _DEBUG
  //   #define _VERIFY(f)               _ASSERT(f)
  //   #define _VERIFYE(f)              ZAssert(f)
  // #else
  //   #define _VERIFY(f)               ((void)(f))
  //   #define _VERIFYE(f)              ((void)(f))
  // #endif
  //
  // Parameters:
  //   f - A boolean expression (including pointers) that evaluates to
  // nonzero or 0.
  //
  // See Also: _SVERIFY, _SVERIFYE
  #define _VERIFY(f)             ZAssert(f)
  #define _VERIFYE(f)            ZAssert(f)            // {partof:_VERIFY}

  ///////////////////////////////////////////////////////////////////////////
  // Macro Group: _SVERIFY, _SVERIFYE Macros
  //
  // These macros evaluate an expression and generate a debug report when the
  // result of passing the expression to the SUCCEEDED macro is FALSE (debug
  // version only). Under release builds the expression is still evaluated,
  // but no debug report is generated, and the evaluated expression is not
  // passed to the SUCCEEDED macro.
  //
  // Declaration:
  // #ifdef _DEBUG
  //   #define _SVERIFY(hr)               ZSucceeded(hr)
  //   #define _SVERIFYE(hr)              ZSucceeded(hr)
  // #else
  //   #define _SVERIFY(hr)               ((void)(hr))
  //   #define _SVERIFYE(hr)              ((void)(hr))
  // #endihr
  //
  // Parameters:
  //   hr - An HRESULT expression that indicates either success (0 or
  // positive), or failure (negative).
  //
  // See Also: _VERIFY, _VERIFYE
  #define _SVERIFY(hr)         ZSucceeded(hr)
  #define _SVERIFYE(hr)        ZSucceeded(hr) // {partof:_SVERIFY}

  ///////////////////////////////////////////////////////////////////////////
  // This macro provides a shorthand way to evaluate an expression only under
  // _DEBUG builds. Under release builds the expression is *not* evaluated.
  //
  // Declaration:
  // #ifdef _DEBUG
  //   #define DEBUG_ONLY(f)          (f)
  // #else
  //   #define DEBUG_ONLY(f)          ((void)0)
  // #endif
  //
  // Parameters:
  //   f - An expression that will be evaluated only under _DEBUG builds.
  #define DEBUG_ONLY(f)          (f)

  void _cdecl TCTrace(LPCTSTR lpszFormat, ...);

  #ifdef _DOCJET_ONLY
  ///////////////////////////////////////////////////////////////////////////
  // Macro Group: The _TRACE Macros
  //
  // These macros provide similar functionality to the printf function by
  // sending a formatted string to a debug monitor. Like printf for console
  // programs, these macros are a convenient way to track the value of
  // variables as your program executes. In the Debug environment, the macro
  // output goes to the debug monitor, if any. Under release builds, they do
  // nothing.
  //
  // For sending output to the debug monitor under both _DEBUG *and* release
  // builds, see the TCERRLOG macros.
  //
  // Parameters:
  //   sz - A format string literal as used in the run-time function
  // *printf*.
  //   p1_thru_p5 - Parameters which are referenced by the '%' placeholders
  // in the /sz/ format string.
  //
  // Declaration:
  // #ifdef _DEBUG
  //   #define _TRACE0(sz)                  TCTrace(TEXT(sz))
  //   #define _TRACE1(sz, p1)              TCTrace(TEXT(sz), p1)
  //   #define _TRACE2(sz, p1,p2)           TCTrace(TEXT(sz), p1,p2)
  //   #define _TRACE3(sz, p1,p2,p3)        TCTrace(TEXT(sz), p1,p2,p3)
  //   #define _TRACE4(sz, p1,p2,p3,p4)     TCTrace(TEXT(sz), p1,p2,p3,p4)
  //   #define _TRACE5(sz, p1,p2,p3,p4,p5)  TCTrace(TEXT(sz), p1,p2,p3,p4,p5)
  // #else
  //   #define _TRACE0(sz)
  //   #define _TRACE1(sz, p1)
  //   #define _TRACE2(sz, p1,p2)
  //   #define _TRACE3(sz, p1,p2,p3)
  //   #define _TRACE4(sz, p1,p2,p3,p4)
  //   #define _TRACE5(sz, p1,p2,p3,p4,p5)
  // #endif
  //
  // See Also: TCTrace, TCERRLOG macros
  #define _TRACE
  #endif // _DOCJET_ONLY

  // {partof:_TRACE}
  #define _TRACE0(sz)                  TCTrace(TEXT(sz))
  // {partof:_TRACE}
  #define _TRACE1(sz, p1)              TCTrace(TEXT(sz), p1)
  // {partof:_TRACE}
  #define _TRACE2(sz, p1,p2)           TCTrace(TEXT(sz), p1,p2)
  // {partof:_TRACE}
  #define _TRACE3(sz, p1,p2,p3)        TCTrace(TEXT(sz), p1,p2,p3)
  // {partof:_TRACE}
  #define _TRACE4(sz, p1,p2,p3,p4)     TCTrace(TEXT(sz), p1,p2,p3,p4)
  // {partof:_TRACE}
  #define _TRACE5(sz, p1,p2,p3,p4,p5)  TCTrace(TEXT(sz), p1,p2,p3,p4,p5)

  #define _TRACE_BEGIN_SIZE(cchMax)                                         \
    {                                                                       \
      TCHAR* _trace_szMsg = (LPTSTR)_alloca(cchMax * sizeof(TCHAR));        \
      int    _trace_nIndex = 0;

  #define _TRACE_BEGIN                                                      \
    _TRACE_BEGIN_SIZE(_MAX_PATH)

  #define _TRACE_PART0(sz)                                                  \
      _trace_nIndex +=                                                      \
        _stprintf(_trace_szMsg + _trace_nIndex, _T(sz))

  #define _TRACE_PART1(sz, p1)                                              \
      _trace_nIndex +=                                                      \
        _stprintf(_trace_szMsg + _trace_nIndex, _T(sz), p1)

  #define _TRACE_PART2(sz, p1,p2)                                           \
      _trace_nIndex +=                                                      \
        _stprintf(_trace_szMsg + _trace_nIndex, _T(sz), p1,p2)

  #define _TRACE_PART3(sz, p1,p2,p3)                                        \
      _trace_nIndex +=                                                      \
        _stprintf(_trace_szMsg + _trace_nIndex, _T(sz), p1,p2,p3)

  #define _TRACE_PART4(sz, p1,p2,p3,p4)                                     \
      _trace_nIndex +=                                                      \
        _stprintf(_trace_szMsg + _trace_nIndex, _T(sz), p1,p2,p3,p4)

  #define _TRACE_PART5(sz, p1,p2,p3,p4,p5)                                  \
      _trace_nIndex +=                                                      \
        _stprintf(_trace_szMsg + _trace_nIndex, _T(sz), p1,p2,p3,p4,p5)

  #define _TRACE_END                                                        \
      USES_CONVERSION;                                                      \
      ZDebugOutputImpl(T2CA(_trace_szMsg));                                 \
    }

  #ifndef UNUSED
    /////////////////////////////////////////////////////////////////////////
    // This macro references the specified parameter so that the compiler
    // will not generate a warning to indicate that the parameter is not
    // referenced.
    //
    // The macro only references the specified parameter under release
    // builds. To reference an unused parameter under both _DEBUG
    // *and* release builds, see UNUSED_ALWAYS.
    //
    // Declaration:
    // #ifndef UNUSED
    //   #ifdef _DEBUG
    //     #define UNUSED(x)
    //   #else
    //     #define UNUSED(x) x
    //   #endif
    // #endif
    //
    // Parameters:
    //   x - The parameter of the current function that is otherwise not
    // referenced in the function.
    //
    // See Also: UNUSED_ALWAYS
    #define UNUSED(x)
  #endif // !UNUSED

#else // _DEBUG

  #define _VERIFY(f)               ((void)(f))
  #define _VERIFYE(f)              ((void)(f))
  #define _SVERIFY(f)              _VERIFY(f)
  #define _SVERIFYE(f)             _VERIFYE(f)
  #define DEBUG_ONLY(f)            ((void)0)

  #define _TRACE0(sz)
  #define _TRACE1(sz, p1)
  #define _TRACE2(sz, p1,p2)
  #define _TRACE3(sz, p1,p2,p3)
  #define _TRACE4(sz, p1,p2,p3,p4)
  #define _TRACE5(sz, p1,p2,p3,p4,p5)

  #define _TRACE_BEGIN_SIZE(cchMax)   {
  #define _TRACE_BEGIN                {
  #define _TRACE_PART0(sz)
  #define _TRACE_PART1(sz, p1)
  #define _TRACE_PART2(sz, p1,p2)
  #define _TRACE_PART3(sz, p1,p2,p3)
  #define _TRACE_PART4(sz, p1,p2,p3,p4)
  #define _TRACE_PART5(sz, p1,p2,p3,p4,p5)
  #define _TRACE_END                  }

  #ifndef UNUSED
    #define UNUSED(x) x
  #endif // !UNUSED

#endif // _DEBUG

#ifndef UNUSED_ALWAYS
    /////////////////////////////////////////////////////////////////////////
    // This macro references the specified parameter so that the compiler
    // will not generate a warning to indicate that the parameter is not
    // referenced.
    //
    // The macros references the specified parameter under both _DEBUG and
    // release builds. To reference an unused parameter under *only* release
    // builds, see UNUSED.
    //
    // Declaration:
    // #ifndef UNUSED_ALWAYS
    //   #define UNUSED_ALWAYS(x) x
    // #endif
    //
    // Parameters:
    //   x - The parameter of the current function that is otherwise not
    // referenced in the function.
    //
    // See Also: UNUSED
  #define UNUSED_ALWAYS(x) x
#endif // !UNUSED_ALWAYS

/////////////////////////////////////////////////////////////////////////////
// Evaluates an HRESULT expression and throws the HRESULT as an exception if
// it indicates a failure.
//
// Parameters:
//   hr - The HRESULT expression to be evaluated.
//
// Return Value: If the specified /hr/ parameter indicates success, it is
// returned as specified. If the /hr/ parameter indicates failure, the
// function does not return normally, but instead throws the /hr/ as a
// C++ exception.
//
// See Also: ThrowErrorFAILED, RETURN_FAILED, RETURN_FAILED_VOID
inline HRESULT ThrowFAILED(HRESULT hr)
{
  if (FAILED(hr))
    throw hr;
  return hr;
}

/////////////////////////////////////////////////////////////////////////////
// Evaluates an HRESULT expression and throws a _com_error exception if it
// indicates a failure.
//
// Parameters:
//   hr - The HRESULT expression to be evaluated.
//
// Return Value: If the specified /hr/ parameter indicates success, it is
// returned as specified. If the /hr/ parameter indicates failure, the
// function does not return normally, but instead throws a _com_error
// C++ exception, containing the failed /hr/ and any current IErrorInfo.
//
// See Also: ThrowError, RETURN_FAILED, RETURN_FAILED_VOID
inline HRESULT ThrowErrorFAILED(HRESULT hr)
{
  if (FAILED(hr))
  {
    IErrorInfo* pei = NULL;
    ::GetErrorInfo(NULL, &pei);
    ::SetErrorInfo(NULL, pei);
    throw _com_error(hr, pei);
  }
  return hr;
}

/////////////////////////////////////////////////////////////////////////////
// Evaluates an HRESULT expression and returns it if it indicates a failure.
// This should only be used from a function that has an HRESULT (or
// compatible) return type.
//
// Parameters:
//   exp - The HRESULT expression to be evaluated.
//
// See Also: RETURN_FAILED_VOID, ThrowError, ThrowErrorFAILED
#define RETURN_FAILED(exp)  \
{                           \
  HRESULT _hr = exp;        \
  if (FAILED(_hr))          \
    return _hr;             \
}

/////////////////////////////////////////////////////////////////////////////
// Evaluates an HRESULT expression and returns if it indicates a failure.
// This should only be used from a function that has a void return type.
//
// Parameters:
//   exp - The HRESULT expression to be evaluated.
//
// See Also: RETURN_FAILED, ThrowError, ThrowErrorFAILED
#define RETURN_FAILED_VOID(exp)  \
{                                \
  if (FAILED(exp))               \
    return;                      \
}

/////////////////////////////////////////////////////////////////////////////
// Formats and reports the specified COM error.
//
// Parameters:
//   szFn - A string specifying the function where the error occurred or the
// function that is reporting the error.
//   sz2 - A string specifying what the function is doing with the error.
// Usually, this would be "Returning" or "Caught!".
//   e - A _com_error reference that indicates the error that occurred.
//
// Return Value: The HRESULT of the error that occurred.
//
// See Also: TCReportHR, _TRACE4
inline HRESULT TCReportComErr(LPCSTR szFn, LPCSTR sz2, _com_error& e)
{
  _TRACE4("%hs: %hs 0x%08X - %s\n", szFn, sz2, e.Error(), e.ErrorMessage());
  IErrorInfoPtr pei(e.ErrorInfo());
  ::SetErrorInfo(NULL, pei);
  return e.Error();
}

/////////////////////////////////////////////////////////////////////////////
// Formats and reports the specified COM error.
//
// Parameters:
//   szFn - A string specifying the function where the error occurred or the
// function that is reporting the error.
//   sz2 - A string specifying what the function is doing with the error.
// Usually, this would be "Returning" or "Caught!".
//   hr - The HRESULT of the error that occurred.
//
// Return Value: The HRESULT of the error that occurred.
//
// See Also: TCReportComErr
inline HRESULT TCReportHR(LPCSTR szFn, LPCSTR sz2, HRESULT hr)
{
  IErrorInfo* pei = NULL;
  ::GetErrorInfo(NULL, &pei);
  ::SetErrorInfo(NULL, pei);
  return TCReportComErr(szFn, sz2, _com_error(hr, pei));
}

#ifdef _CPPUNWIND

  /////////////////////////////////////////////////////////////////////////////
  // Defines a *catch* block that catches C++ exceptions of type _com_error&.
  // The *catch* block simply calls TCReportComErr with the specified function
  // name string, the string literal "Returning", and the _com_error exception
  // that was caught.
  //
  // Parameters:
  //   szFn - A string specifying the function where the error occurred or the
  // function that is reporting the error.
  //
  // See Also: TCReportComErr, TC_CATCH_HR, TC_CATCH_ALL, TC_CATCH_ALL_VOID
  #define TC_CATCH_COM_ERR(szFn) catch (_com_error& e) \
    {return TCReportComErr(szFn, "Returning", e);}

  /////////////////////////////////////////////////////////////////////////////
  // Defines a *catch* block that catches C++ exceptions of type HRESULT. The
  // *catch* block simply calls TCReportHR with the specified function name
  // string, the string literal "Returning", and the HRESULT exception that was
  // caught.
  //
  // Parameters:
  //   szFn - A string specifying the function where the error occurred or the
  // function that is reporting the error.
  //
  // See Also: TCReportHR, TC_CATCH_COM_ERR, TC_CATCH_ALL, TC_CATCH_ALL_VOID
  #define TC_CATCH_HR(szFn) catch (HRESULT _hr_) \
    {return TCReportHR(szFn, "Returning", _hr_);}

  /////////////////////////////////////////////////////////////////////////////
  // Defines a *catch* block that catches all C++ exceptions not already
  // caught. The *catch* block simply calls _TRACE1, specifying that the
  // function "Caught an unknonwn exception" and returns the specified return
  // value.
  //
  // Parameters:
  //   szFn - A string specifying the function that is reporting the error.
  //   result - The return value to return from the *catch* block.
  //
  // See Also: TC_CATCH_ALL_VOID, _TRACE1
  #define TC_CATCH_ALL(szFn, result) catch (...) \
    {_TRACE1("%hs: Caught an unknown exception\n", szFn); return result;}

  /////////////////////////////////////////////////////////////////////////////
  // Defines a *catch* block that catches all C++ exceptions not already
  // caught. The *catch* block simply calls _TRACE1, specifying that the
  // function "Caught an unknonwn exception". The *catch* block does
  // *not* return.
  //
  // TODO: This should probably be changed to end the *catch* block with a void
  // return statement. Then, another macro, TC_CATCH_ALL_NR, could be coded to
  // act as this one currently does.
  //
  // Parameters:
  //   szFn - A string specifying the function that is reporting the error.
  //
  // See Also: TC_CATCH_ALL, _TRACE1
  #define TC_CATCH_ALL_VOID(szFn) catch (...) \
    {_TRACE1("%hs: Caught an unknown exception\n", szFn);}

#endif // _CPPUNWIND

/////////////////////////////////////////////////////////////////////////////
// Eliminates from the specified type's name, any of the specified prefixes.
// This is useful when using a type's name in a debug string, and a
// particular set of type prefixes are unnecessary in the debug output, such
// as "class" and "struct".
//
// Parameters:
//   ti - A constant reference to a *type_info* class, as returned from the
// C++ *typeid* operator.
//   nCount - The number of prefix strings specified the variable argument
// list.
//   argptr - A va_list containing the standardized representation of the
// variable argument list.
//
// Return Value: A pointer to the first character after one of the specified
// prefixes in the type name. If the type name has none of the prefixes, the
// return value is a pointer to the first character of the entire type name.
//
// See Also: TCTypeName
inline LPCSTR TCTypeName_EatPrefixesV(const type_info& ti, long nCount,
  va_list argptr)
{
  LPCSTR pszName = ti.name();
  for (int i = 0; i < nCount; i++)
  {
    LPCSTR pszPrefix = va_arg(argptr, LPCSTR);
    size_t nSize = strlen(pszPrefix);
    if (0 == strncmp(pszName, pszPrefix, nSize))
      pszName += nSize;
  }
  return pszName;
}

// {partof:TCTypeName_EatPrefixesV}
inline LPCSTR TCTypeName_EatPrefixes(const type_info& ti, long nCount, ...)
{
  va_list argptr;
  va_start(argptr, nCount);
  LPCSTR psz = TCTypeName_EatPrefixesV(ti, nCount, argptr);
  va_end(argptr);
  return psz;
}

/////////////////////////////////////////////////////////////////////////////
// This macro simply calls *typeid* with the specified type and removes the
// prefixes "class " and "struct ".
//
// Parameters:
//   T - The type for which a modified name will be generated.
//
// See Also: TCTypeName_EatPrefixes
#ifdef _CPPRTTI
  #define TCTypeName(T) \
    TCTypeName_EatPrefixes(typeid(T), 2, "class ", "struct ")
#else
  #define TCTypeName(T) ""
#endif


/////////////////////////////////////////////////////////////////////////////
// Component Category Registration

HRESULT RegisterComponentCategory(REFGUID catid, LPCOLESTR pszDesc);
HRESULT RegisterComponentCategory(LPCOLESTR pszCLSID, LPCOLESTR pszDesc);
HRESULT UnregisterComponentCategory(REFGUID catid);
HRESULT UnregisterComponentCategory(LPCOLESTR pszCLSID);


/////////////////////////////////////////////////////////////////////////////
// Date and Time Retrieval / Conversion

DATE GetVariantDateTime();
DATE GetVariantDate();
DATE GetVariantTime();
DATE GetLocalVariantDateTime();
DATE GetLocalVariantDate();
DATE GetLocalVariantTime();
bool TCSystemTimeToTzSpecificLocalTime(TIME_ZONE_INFORMATION* pTimeZone,
  SYSTEMTIME* pUniversalTime, SYSTEMTIME* pLocalTime);
bool VariantTimeToLocalTime(DATE date, SYSTEMTIME* psystime);
bool FileTimeToVariantTime(FILETIME* pft, DATE* pdate);
int FormatDateTime(SYSTEMTIME* psystime, LPTSTR psz, int nLength);
int FormatVariantDateTimeAsLocal(DATE date, LPTSTR psz, int nLength);
int FormatDate(SYSTEMTIME* psystime, LPTSTR psz, int nLength);
int FormatVariantDateAsLocal(DATE date, LPTSTR psz, int nLength);
int FormatTime(SYSTEMTIME* psystime, LPTSTR psz, int nLength);
int FormatVariantTimeAsLocal(DATE date, LPTSTR psz, int nLength);
int FormatTimeSpan(SYSTEMTIME* psystime, LPTSTR psz, int nLength);
int FormatVariantTimeSpan(DATE date, LPTSTR psz, int nLength);


/////////////////////////////////////////////////////////////////////////////
// MFC Date and Time Wrappers

#ifdef _AFX

  CString FormatDateTime(SYSTEMTIME* psystime);
  BOOL FormatDateTime(SYSTEMTIME* psystime, CString& str);

  CString FormatVariantDateTimeAsLocal(DATE date);
  BOOL FormatVariantDateTimeAsLocal(DATE date, CString& str);

  CString FormatDate(SYSTEMTIME* psystime);
  BOOL FormatDate(SYSTEMTIME* psystime, CString& str);

  CString FormatVariantDateAsLocal(DATE date);
  BOOL FormatVariantDateAsLocal(DATE date, CString& str);

  CString FormatTime(SYSTEMTIME* psystime);
  BOOL FormatTime(SYSTEMTIME* psystime, CString& str);

  CString FormatVariantTimeAsLocal(DATE date);
  BOOL FormatVariantTimeAsLocal(DATE date, CString& str);

  CString FormatTimeSpan(SYSTEMTIME* psystime);
  BOOL FormatTimeSpan(SYSTEMTIME* psystime, CString& str);

  CString FormatVariantTimeSpan(DATE date);
  BOOL FormatVariantTimeSpan(DATE date, CString& str);

#endif // _AFX


/////////////////////////////////////////////////////////////////////////////
// Object Persistence

HRESULT SaveObjectToNewStream(IUnknown* punk, IStream** ppStream);
HRESULT LoadObjectFromStream(IUnknown* punk, IStream* pStream);
HRESULT CopyObjectThruStream(IUnknown* punkDest, IUnknown* punkSrc);
HRESULT CreateCopyObjectThruStream(IUnknown* punk, REFIID riid, void** ppv);
HRESULT CompareObjectsThruStream(IUnknown* punk1, IUnknown* punk2);
HRESULT CompareObjectsThruThisStream(IStream* pstm1, IUnknown* punk2);
HRESULT CompareObjectCLSIDs(IUnknown* punk1, IUnknown* punk2);


/////////////////////////////////////////////////////////////////////////////
// User Interface Helpers

bool FixSliderThumbSize(HWND hwndSlider);

#if defined(_AFX)
  bool FixSliderThumbSize(CWnd* pwndSlider);
#endif // _AFX


/////////////////////////////////////////////////////////////////////////////
// GUID Comparison Operators

/////////////////////////////////////////////////////////////////////////////
// Compares two GUID's to determine if /guid1/ is less than /guid2/. The
// less-than comparison is made using the memcmp runtime library function.
//
// Return Value: true if /guid1/ is less than /guid2/, otherwise false.
//
// Parameters:
//   guid1 - A constant GUID reference. Since CLSID and IID are just type
// definitions of GUID, they can also be used here.
//   guid2 - A constant GUID reference. Since CLSID and IID are just type
// definitions of GUID, they can also be used here.
inline bool operator < (const GUID& guid1, const GUID& guid2)
{
  return memcmp(&guid1, &guid2, sizeof(guid1)) < 0;
}


/////////////////////////////////////////////////////////////////////////////
// GUID Conversion

/////////////////////////////////////////////////////////////////////////////
// Generates the string representation of the specified GUID. The conversion
// is done by calling the Win32 StringFromGUID2 API.
//
// Parameters:
//   guid - A constant GUID reference. Since CLSID and IID are just type
// definitions of GUID, they can also be used here.
//
// Return Value: A _bstr_t instance that contains the string representation
// of the specified GUID.
inline _bstr_t BSTRFromGUID(REFGUID guid)
{
  OLECHAR szGUID[48];
  StringFromGUID2(guid, szGUID, sizeofArray(szGUID));
  return _bstr_t(szGUID);
}


/////////////////////////////////////////////////////////////////////////////
// Variant Helpers

/////////////////////////////////////////////////////////////////////////////
// Determines if the specified VARIANT contains an object reference.
//
// Parameters:
//   v - A pointer to the VARIANT to be tested.
//
// Return Value: true if the type of the specified VARIANT is either
// VT_UNKNOWN or VT_DISPATCH; false otherwise.
//
// See Also: V_OBJECT
#define V_ISOBJECT(v)    (VT_UNKNOWN == V_VT(v) || VT_DISPATCH == V_VT(v))

/////////////////////////////////////////////////////////////////////////////
// Returns the IUnknown* of the object reference contained by the specified
// VARIANT, if the VARIANT does contain an object reference.
//
// Parameters:
//   v - A pointer to the VARIANT to be tested.
//
// Return Value: The IUnknown* of the object reference contained by the
// specified VARIANT, if it does contain an object reference; NULL otherwise.
//
// See Also: V_ISOBJECT
#define V_OBJECT(v)                                                         \
  (V_ISOBJECT(v) ? (IUnknown*)IUnknownPtr(V_DISPATCH(v)) : (IUnknown*)NULL)  


/////////////////////////////////////////////////////////////////////////////
// BSTR Helpers


/////////////////////////////////////////////////////////////////////////////
// Safely determines the character length of a BSTR.
//
// This function first checks the specified BSTR for the NULL value and
// returns zero if it is. Otherwise, it passes the BSTR to the SysStringLen
// API.
//
// Parameters:
//   bstr - A BSTR value for which the length is to be retrieved.
//
// Return Value: The length of the BSTR, in characters.
//
inline UINT BSTRLen(BSTR bstr)
{
  return bstr ? ::SysStringLen(bstr) : 0;
}


/////////////////////////////////////////////////////////////////////////////
// Determine if we're running on Windows 9x or not.
//
// Return Value: *true* if the operation system is Windows 95 or Windows 98.
// *false* if the operating system is Windows NT or Windows 2000.
//
inline bool IsWin9x()
{
  OSVERSIONINFO osvi = {sizeof(osvi)};
  _VERIFYE(GetVersionEx(&osvi));
  return !(VER_PLATFORM_WIN32_NT & osvi.dwPlatformId);
}


/////////////////////////////////////////////////////////////////////////////
// Determine if we're running on Windows NT or not.
//
// Return Value: *true* if the operation system is Windows NT or
// Windows 2000. *false* if the operating system is Windows 95 or Windows 98.
//
inline bool IsWinNT()
{
  OSVERSIONINFO osvi = {sizeof(osvi)};
  _VERIFYE(GetVersionEx(&osvi));
  return !!(VER_PLATFORM_WIN32_NT & osvi.dwPlatformId);
}


/////////////////////////////////////////////////////////////////////////////
//
HRESULT LockAndLoadResource(HINSTANCE hinst, LPCTSTR pszType,
  LPCTSTR pszName, void** ppvData, DWORD* pcbData);


/////////////////////////////////////////////////////////////////////////////
//
bool IsInteractiveDesktop();


/////////////////////////////////////////////////////////////////////////////
// String Loading

int TCLoadStringW(HINSTANCE hInstance, UINT wID, LPWSTR wzBuf, int cchBuf);
HRESULT TCLoadBSTR(HINSTANCE hInstance, UINT wID, BSTR* pbstrOut);


/////////////////////////////////////////////////////////////////////////////
// Exception Filtering

#define TCDebugBreakExceptionFilter()                                       \
  ((EXCEPTION_BREAKPOINT == GetExceptionCode()) ?                           \
    EXCEPTION_CONTINUE_SEARCH : EXCEPTION_EXECUTE_HANDLER)


/////////////////////////////////////////////////////////////////////////////
// Error Reporting Functions/Macros
// Note: These methods will output even in RELEASE builds!

void _cdecl TCErrLog(LPCTSTR lpszFormat, ...);

/////////////////////////////////////////////////////////////////////////////
// Macro Group: The TCERRLOG Macros
//
// These macros provide similar functionality to the printf function by
// sending a formatted string to a debug monitor. Like printf for console
// programs, these macros are a convenient way to track the value of
// variables as your program executes. These macros work the same under
// *both* _DEBUG and release builds.
//
// For sending output to the debug monitor under *only* _DEBUG builds, see
// the _TRACE macros.
//
// Parameters:
//   sz - A format string literal as used in the run-time function
// *printf*.
//   p1_thru_p5 - Parameters which are referenced by the '%' placeholders
// in the /sz/ format string.
//
// Declaration:
// #define TCERRLOG                       TCErrLog
// #define TCERRLOG0(sz)                  TCERRLOG(_T(sz))
// #define TCERRLOG1(sz, p1)              TCERRLOG(_T(sz), p1)
// #define TCERRLOG2(sz, p1,p2)           TCERRLOG(_T(sz), p1,p2)
// #define TCERRLOG3(sz, p1,p2,p3)        TCERRLOG(_T(sz), p1,p2,p3)
// #define TCERRLOG4(sz, p1,p2,p3,p4)     TCERRLOG(_T(sz), p1,p2,p3,p4)
// #define TCERRLOG5(sz, p1,p2,p3,p4,p5)  TCERRLOG(_T(sz), p1,p2,p3,p4,p5)
//
// See Also: TCErrLog, _TRACE macros
#define TCERRLOG                       TCErrLog
// {partof:TCERRLOG}
#define TCERRLOG0(sz)                  TCERRLOG(_T(sz))
// {partof:TCERRLOG}
#define TCERRLOG1(sz, p1)              TCERRLOG(_T(sz), p1)
// {partof:TCERRLOG}
#define TCERRLOG2(sz, p1,p2)           TCERRLOG(_T(sz), p1,p2)
// {partof:TCERRLOG}
#define TCERRLOG3(sz, p1,p2,p3)        TCERRLOG(_T(sz), p1,p2,p3)
// {partof:TCERRLOG}
#define TCERRLOG4(sz, p1,p2,p3,p4)     TCERRLOG(_T(sz), p1,p2,p3,p4)
// {partof:TCERRLOG}
#define TCERRLOG5(sz, p1,p2,p3,p4,p5)  TCERRLOG(_T(sz), p1,p2,p3,p4,p5)

#define TCERRLOG_BEGIN_SIZE(cchMax)                                         \
  {                                                                         \
    TCHAR* _trace_szMsg = (LPTSTR)_alloca(cchMax * sizeof(TCHAR));          \
    int    _trace_nIndex = 0;

#define TCERRLOG_BEGIN                                                      \
  TCERRLOG_BEGIN_SIZE(_MAX_PATH)

#define TCERRLOG_PART0(sz)                                                  \
    _trace_nIndex +=                                                        \
      _stprintf(_trace_szMsg + _trace_nIndex, _T(sz))

#define TCERRLOG_PART1(sz, p1)                                              \
    _trace_nIndex +=                                                        \
      _stprintf(_trace_szMsg + _trace_nIndex, _T(sz), p1)

#define TCERRLOG_PART2(sz, p1,p2)                                           \
    _trace_nIndex +=                                                        \
      _stprintf(_trace_szMsg + _trace_nIndex, _T(sz), p1,p2)

#define TCERRLOG_PART3(sz, p1,p2,p3)                                        \
    _trace_nIndex +=                                                        \
      _stprintf(_trace_szMsg + _trace_nIndex, _T(sz), p1,p2,p3)

#define TCERRLOG_PART4(sz, p1,p2,p3,p4)                                     \
    _trace_nIndex +=                                                        \
      _stprintf(_trace_szMsg + _trace_nIndex, _T(sz), p1,p2,p3,p4)

#define TCERRLOG_PART5(sz, p1,p2,p3,p4,p5)                                  \
    _trace_nIndex +=                                                        \
      _stprintf(_trace_szMsg + _trace_nIndex, _T(sz), p1,p2,p3,p4,p5)

#ifdef _DEBUG
  #define TCERRLOG_END                                                      \
    ZDebugOutputImpl(_trace_szMsg);                                         \
  }
#else // _DEBUG
  #define TCERRLOG_END                                                      \
    OutputDebugString(_trace_szMsg);                                        \
  }
#endif // _DEBUG


///////////////////////////////////////////////////////////////////////////
//
#ifdef _DEBUG
  #define PRIVATE_ASSERTE(expr)                                           \
    do { if (!(expr) &&                                                   \
      (1 == _CrtDbgReport(_CRT_ASSERT, __FILE__, __LINE__, NULL, #expr))) \
      _CrtDbgBreak(); } while (0)
  #define PRIVATE_VERIFY(expr)               PRIVATE_ASSERTE(expr)
  #define PRIVATE_VERIFYE(expr)              PRIVATE_ASSERTE(expr)
#else // _DEBUG
  #define PRIVATE_ASSERTE(expr)              ((void)(0))
  #define PRIVATE_VERIFY(expr)               ((void)(expr))
  #define PRIVATE_VERIFYE(expr)              ((void)(expr))
#endif // _DEBUG


/////////////////////////////////////////////////////////////////////////////

#endif // !__TCLib_h__
