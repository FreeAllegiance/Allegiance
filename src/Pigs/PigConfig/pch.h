#pragma once

/////////////////////////////////////////////////////////////////////////////
// pch.h : include file for standard system include files,
//      or project specific include files that are used frequently,
//      but are changed infrequently

#ifndef _WIN32_WINNT
  #define _WIN32_WINNT 0x0A00
#endif

#pragma warning(disable:4786)
#pragma warning(disable:4192)

// CRT Includes

// STL Includes

// MFC Includes
#define VC_EXTRALEAN
#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdtctl.h>       // MFC support for Internet Explorer 4 Common Controls
#include <afxcmn.h>         // MFC support for Windows Common Controls

// Win32 Includes
#include <mmsystem.h>

// ATL and ATLWin Includes
#include <atlbase.h>

// Allegiance Includes
#include <zlib.h>
#include <TCLib.h>

// MIDL-Generated Includes (from other projects)
#include <AGC.h>
#include <PigsIDL.h>
#include <PigsLib.h>


/////////////////////////////////////////////////////////////////////////////
// 
inline CString GetErrorText(DWORD dwError)
{
  // Format the specified error code into a text string
  LPTSTR pszError = NULL;
  DWORD dwFlags = FORMAT_MESSAGE_ALLOCATE_BUFFER |
    FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS;
  ::FormatMessage(dwFlags, NULL, dwError, 0, (LPTSTR)&pszError, 0, NULL);

  // Copy the string to the specified CString
  CString strText(pszError);

  // Free the string allocated by the API
  ::LocalFree(pszError);

  // Return the string
  return strText;
}


/////////////////////////////////////////////////////////////////////////////
