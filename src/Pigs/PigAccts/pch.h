// pch.h : include file for standard system include files,
//      or project specific include files that are used frequently,
//      but are changed infrequently

#if !defined(AFX_pch_H__1F7E7E81_C6F0_11D2_85C9_00C04F68DEB0__INCLUDED_)
#define AFX_pch_H__1F7E7E81_C6F0_11D2_85C9_00C04F68DEB0__INCLUDED_

#if _MSC_VER > 1000
  #pragma once
#endif // _MSC_VER > 1000

#ifndef _WIN32_DCOM
  #define _WIN32_DCOM
#endif

#pragma warning(disable: 4786)

// This next one is an annoying BoundsChecker warning
#pragma warning(disable: 4950)


#include <atlbase.h>
#include <..\TCAtl\ComModule.h>
class CExeModule : public TCComModule<CExeModule>
{
// Command-line Option Map
public:
  BEGIN_CMDLINE_OPTION_MAP()
    CMDLINE_OPTION_Automation()
    CMDLINE_OPTION_RegServer9x(CExeModule)
    CMDLINE_OPTION_RegServerNT(CExeModule)
    CMDLINE_OPTION_UnregServer(CExeModule)
    CMDLINE_OPTION_NoExit(CExeModule)
    CMDLINE_OPTION_Exit(CExeModule)
    CMDLINE_OPTION_Help(CExeModule)
  END_CMDLINE_OPTION_MAP()

// Overrides
public:
  void    DisplaySyntax();
  bool    UseMessageBox() {return false;}
  LPCTSTR GetRegInfo(UINT& idResource, bool& bRegTypeLib);
};
extern CExeModule _Module;
#include <atlcom.h>

#include <vector>
#include <comdef.h>

#pragma warning(disable:4192)


// ZLib Headers
#include <zlib.h>

#endif // !defined(AFX_pch_H__1F7E7E81_C6F0_11D2_85C9_00C04F68DEB0__INCLUDED)
