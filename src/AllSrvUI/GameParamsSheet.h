#pragma once

/////////////////////////////////////////////////////////////////////////////
// GameParamsSheet.h : header file
//

#include "GamePageTeam.h"
#include "GamePageWin.h"
#include "GamePageSpecifics.h"


/////////////////////////////////////////////////////////////////////////////
// CGameParamsSheet dialog

class CGameParamsSheet : public CPropertySheet
{
// Declarations
protected:
  DECLARE_MESSAGE_MAP()

// Construction
public:
  CGameParamsSheet(CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

// Implementation
protected:
  void InitButtons();

// Overrides
public:
  // ClassWizard generate virtual function overrides
  //{{AFX_VIRTUAL(CGameParamsSheet)
  //}}AFX_VIRTUAL

// Message Handlers
protected:
  // Generated message map functions
  //{{AFX_MSG(CGameParamsSheet)
  virtual BOOL OnInitDialog();
  //}}AFX_MSG

// Data Members
protected:
  CGamePageWin       m_PageWin;
  CGamePageTeam      m_PageTeam;
  CGamePageSpecifics m_PageSpecifics;
};

/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
/////////////////////////////////////////////////////////////////////////////