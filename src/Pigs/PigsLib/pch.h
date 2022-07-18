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

// BT - STEAM
#include "steam_api.h"


// VS.Net 2003 port - COM_INTERFACE_ENTRY2 is more type strict now, revert it to VS6 version
#if _MSC_VER >= 1310
#	undef COM_INTERFACE_ENTRY2
	#define COM_INTERFACE_ENTRY2(x, x2)\
		{&_ATL_IIDOF(x),\
		(DWORD)((x*)(x2*)((_ComMapClass*)8))-8,\
		_ATL_SIMPLEMAPENTRY},
#endif 

#include <atlwin.h>         // ATL window classes


/////////////////////////////////////////////////////////////////////////////
// Begin: Allegiance Header Files                                          //
//                                                                         //
#include <dplay8.h>                                                         //
//#include <dplobby.h>                                                       //
#include <ddraw.h>                                                         //
//#include <d3d.h>                                                           //
#include <dsound.h>                                                        //
#include <wininet.h>                                                       //
#include <shlwapi.h>                                                       //
                                                                           //
#include <..\zlib\zlib.h>                                                  //
#include <FixDelete.h>                                                     //
//                                                                         //
// End: Allegiance Header Files                                            //
/////////////////////////////////////////////////////////////////////////////



