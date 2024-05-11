// pch.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#pragma once

#define WIN32_LEAN_AND_MEAN   // Exclude rarely-used stuff from Windows headers
#define _WIN32_DCOM
#define __MODULE__ "TCLib"

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