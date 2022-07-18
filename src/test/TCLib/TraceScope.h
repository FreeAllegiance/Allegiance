#if _MSC_VER > 1000
  #pragma once
#endif // _MSC_VER > 1000

#ifndef __TraceScope_h__
#define __TraceScope_h__


/////////////////////////////////////////////////////////////////////////////
//
class TCTraceScope
{
// Group=Construction / Destruction
public:
  TCTraceScope(LPCSTR pszClass, LPCSTR pszMethod, LPCSTR pszFmt = NULL, ...);

#ifdef _DEBUG
  ~TCTraceScope();

  // Group=Implementation
  protected:
    static DWORD TlsIndex();

  // Group=Data Members
  protected:
    LPCSTR m_pszClass;
    LPCSTR m_pszMethod;
#endif
};


#ifdef _DEBUG

  ///////////////////////////////////////////////////////////////////////////
  // Group=Construction / Destruction

  inline TCTraceScope::TCTraceScope(LPCSTR pszClass, LPCSTR pszMethod,
    LPCSTR pszFmt, ...) : m_pszClass(pszClass), m_pszMethod(pszMethod)
  {
    ZAssert(pszMethod);

    // Get the indent level and increment it
    int nIndent = (int)(::TlsGetValue(TlsIndex()));
    ::TlsSetValue(TlsIndex(), LPVOID(nIndent + 1));

    // Create the formatting string
    char szFmt[_MAX_PATH] = "";
    for (int i = 0; i < nIndent; ++i)
      strcat(szFmt, "  ");
    if (m_pszClass)
    {
      strcat(szFmt, m_pszClass);
      strcat(szFmt, "::");
    }
    strcat(szFmt, m_pszMethod);
    strcat(szFmt, "()");
    if (pszFmt && strlen(pszFmt))
    {
      strcat(szFmt, ": ");
      strcat(szFmt, pszFmt);
    }
    strcat(szFmt, "\n");
    for (i = 0; i < nIndent; ++i)
      strcat(szFmt, "  ");
    strcat(szFmt, "{\n");

    // Format the message string
    va_list vaArgs;
    va_start(vaArgs, pszFmt);
    char szMsg[_MAX_PATH];
    _vsnprintf(szMsg, sizeofArray(szMsg), szFmt, vaArgs);
    va_end(vaArgs);

    // Output the message string
    OutputDebugStringA(szMsg);
  }

  inline TCTraceScope::~TCTraceScope()
  {
    // Get the indent level and decrement it
    int nIndent = (int)(::TlsGetValue(TlsIndex()));
    ::TlsSetValue(TlsIndex(), LPVOID(--nIndent));

    // Create the indent string
    int cchIndent = nIndent * 2;
    LPSTR pszIndent = (LPSTR)_alloca(cchIndent + 1);
    memset(pszIndent, ' ', cchIndent);
    pszIndent[cchIndent] = '\0';

    // Create the message string
    char szMsg[_MAX_PATH] = "";
    if (m_pszClass)
      _snprintf(szMsg, sizeofArray(szMsg), "%hs} // %hs::%hs\n", pszIndent,
        m_pszClass, m_pszMethod);
    else
      _snprintf(szMsg, sizeofArray(szMsg), "%hs} // %hs\n", pszIndent,
        m_pszMethod);

    // Output the message string
    OutputDebugStringA(szMsg);
  }

  ///////////////////////////////////////////////////////////////////////////
  // Group=Implementation

  inline DWORD TCTraceScope::TlsIndex()
  {
    static DWORD dwTlsIndex = ::TlsAlloc();
    return dwTlsIndex;
  }

#else // _DEBUG

  inline TCTraceScope::TCTraceScope(LPCSTR, LPCSTR, LPCSTR, ...)
  {
  }

#endif // _DEBUG


/////////////////////////////////////////////////////////////////////////////
// Macros

#ifdef _DEBUG

  #define _TSCOPE(cls, fn, msg) \
    TCTraceScope mctscope(#cls, #fn, msg);

  #define _TSCOPE0(cls, fn) \
    TCTraceScope mctscope(#cls, #fn);

  #define _TSCOPE1(cls, fn, msg, p1) \
    TCTraceScope mctscope(#cls, #fn, msg, p1);

  #define _TSCOPE2(cls, fn, msg, p1, p2) \
    TCTraceScope mctscope(#cls, #fn, msg, p1, p2);

  #define _TSCOPE3(cls, fn, msg, p1, p2, p3) \
    TCTraceScope mctscope(#cls, #fn, msg, p1, p2, p3);

  #define _TSCOPE4(cls, fn, msg, p1, p2, p3, p4) \
    TCTraceScope mctscope(#cls, #fn, msg, p1, p2, p3, p4);

  #define _TSCOPE5(cls, fn, msg, p1, p2, p3, p4, p5) \
    TCTraceScope mctscope(#cls, #fn, msg, p1, p2, p3, p4, p5);

#else // _DEBUG

  #define _TSCOPE(cls, fn, msg)
  #define _TSCOPE0(cls, fn)
  #define _TSCOPE1(cls, fn, msg, p1)
  #define _TSCOPE2(cls, fn, msg, p1, p2)
  #define _TSCOPE3(cls, fn, msg, p1, p2, p3)
  #define _TSCOPE4(cls, fn, msg, p1, p2, p3, p4)
  #define _TSCOPE5(cls, fn, msg, p1, p2, p3, p4, p5)

#endif // _DEBUG



/////////////////////////////////////////////////////////////////////////////

#endif // !__TraceScope_h__
