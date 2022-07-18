/////////////////////////////////////////////////////////////////////////////
// pch.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#pragma warning (disable:4786)  // disable 'identifier was truncated to '255'...' warning

#if _MSC_VER >= 1000
  #pragma once
#endif // _MSC_VER >= 1000

#define VC_EXTRALEAN        // Exclude rarely-used stuff from Windows headers
#define _WIN32_DCOM         // Enable DCOM features

// CRT headers
#include <crtdbg.h>

// STL headers
#include <algorithm>
#include <map>
#include <set>
#include <vector>

// Win32 headers
#include <windows.h>        // Win32 declarations

// ATL headers
#include <atlbase.h>        // ATL base declarations
extern CComModule _Module;  // ATL mandatory symbol
#include <atlcom.h>         // ATL component object classes
#include <atlwin.h>         // ATL window classes

// ZLib headers
#include <..\zlib\zlib.h>
#include <FixDelete.h>

