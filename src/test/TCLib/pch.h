// pch.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_pch_H__81172B8C_CBEE_11D2_9646_00C04F68DEB0__INCLUDED_)
#define AFX_pch_H__81172B8C_CBEE_11D2_9646_00C04F68DEB0__INCLUDED_

#if _MSC_VER > 1000
  #pragma once
#endif // _MSC_VER > 1000

#define WIN32_LEAN_AND_MEAN   // Exclude rarely-used stuff from Windows headers
#define _WIN32_DCOM

#pragma warning(disable: 4786)

#include <stdexcept>
#include <algorithm>
#include <string>
#include <vector>
#include <map>


#include <crtdbg.h>
#include <windows.h>
#include <tchar.h>
#include <malloc.h>
#include <comcat.h>
#include <..\zlib\zassert.h>
#include <comdef.h>
#include <commctrl.h>
#include <atlconv.h>
#include <process.h> // for _beginthreadex and _endthreadex
#include <mmsystem.h>

// ZLib headers

#include <..\zlib\zlib.h>


#endif // !defined(AFX_pch_H__81172B8C_CBEE_11D2_9646_00C04F68DEB0__INCLUDED_)
