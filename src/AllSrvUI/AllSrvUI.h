// AllSrvUI.h : main header file for the ALLSRVUI application
//

#pragma once

#ifndef __AFXWIN_H__
  #error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"   // main symbols


/////////////////////////////////////////////////////////////////////////////
// AllSrvUI Existance

#define szAllSrvUIRunning TEXT("{377845DD-9600-4612-93F5-8A50338F6DE7}_Running")
#define szAllSrvUIRunningGlobal (TEXT("Global\\") szAllSrvUIRunning)


/////////////////////////////////////////////////////////////////////////////
// CAllSrvUIApp:
// See AllSrvUI.cpp for the implementation of this class
//

class CAllSrvUIApp : public CWinApp
{
// Declarations
public:
  DECLARE_MESSAGE_MAP()

// Construction
public:
  CAllSrvUIApp();

// Attributes
public:
  static void GetArtPath(char * szArtPath);

// Overrides
public:
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CAllSrvUIApp)
  public:
  virtual BOOL InitInstance();
  virtual int ExitInstance();
  virtual BOOL OnIdle(LONG lCount);
  //}}AFX_VIRTUAL

// Implementation
protected:
  HANDLE GetAllSrvUIEvent();
  void CreateAllSrvUIEvent();
  // yp your_persona march 25 2006 : Remove EULA.dll dependency patch
  //HRESULT FirstRunEula();

// Types
protected:
  typedef DWORD (*EBUPROC) (LPCTSTR lpRegKeyLocation, LPCTSTR lpEULAFileName, LPCSTR lpWarrantyFileName, BOOL fCheckForFirstRun);

// Message Handlers
protected:
  //{{AFX_MSG(CAllSrvUIApp)
  //}}AFX_MSG

// Data Members
protected:
  TCHandle m_shEventSync;
  HRESULT  m_hrCoInit;

};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.