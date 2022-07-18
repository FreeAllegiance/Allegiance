/////////////////////////////////////////////////////////////////////////////
// pch.h : include file for standard system include files,
//      or project specific include files that are used frequently,
//      but are changed infrequently

#if !defined(AFX_STDAFX_H__537FEB63_0A41_11D3_8B58_00C04F681633__INCLUDED_)
#define AFX_STDAFX_H__537FEB63_0A41_11D3_8B58_00C04F681633__INCLUDED_

#if _MSC_VER > 1000
  #pragma once
#endif // _MSC_VER > 1000

#ifndef _ATL_STATIC_REGISTRY
#define _ATL_STATIC_REGISTRY
#endif

#ifndef STRICT
  #define STRICT
#endif

#ifndef _WIN32_WINNT
  #define _WIN32_WINNT 0x0A00
#endif
#define _ATL_APARTMENT_THREADED

#pragma warning(disable:4786) // identifier was truncated ... in the debug information
#pragma warning(disable:4503) // decorated name length exceeded

#include <windows.h>
#include <stdlib.h>
#include <commctrl.h>
#include <memory.h>
#include <objsafe.h>

// STL headers
#include <functional>
#include <algorithm>
#include <map>
#include <set>
#include <vector>

#include <atlbase.h>
extern CComModule _Module;
#include <atlcom.h>

// VS.Net 2003 port - COM_INTERFACE_ENTRY2 is more type strict now, revert it to VS6 version
#if _MSC_VER >= 1310
#	undef COM_INTERFACE_ENTRY2
	#define COM_INTERFACE_ENTRY2(x, x2)\
		{&_ATL_IIDOF(x),\
		(DWORD)((x*)(x2*)((_ComMapClass*)8))-8,\
		_ATL_SIMPLEMAPENTRY},
#endif 

#include <TCLib.h>
#include <TCAtl.h>
#include <..\TCAtl\ObjectMap.h>
#include <..\TCAtl\PropertyClass.h>
#include <..\TCAtl\TCNullStreamImpl.h>

#include <zlib.h>
#include <utility.h>
#include <Igc.h>

#define AGC_MODULE


/////////////////////////////////////////////////////////////////////////////
template <class T>
class ATL_NO_VTABLE AGCObjectSafetyImpl :
  public IObjectSafetyImpl<T,
    INTERFACESAFE_FOR_UNTRUSTED_CALLER | INTERFACESAFE_FOR_UNTRUSTED_DATA>
{
// Types
protected:
  typedef IObjectSafetyImpl<T, INTERFACESAFE_FOR_UNTRUSTED_CALLER |
    INTERFACESAFE_FOR_UNTRUSTED_DATA> AGCObjectSafetyImplBase;

// Construction
public:
  AGCObjectSafetyImpl()
  {
    AGCObjectSafetyImplBase::m_dwCurrentSafety =
      INTERFACESAFE_FOR_UNTRUSTED_CALLER | INTERFACESAFE_FOR_UNTRUSTED_DATA;
  }
};


/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
/////////////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_STDAFX_H__537FEB63_0A41_11D3_8B58_00C04F681633__INCLUDED)

