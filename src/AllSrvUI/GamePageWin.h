#pragma once

/////////////////////////////////////////////////////////////////////////////
// GamePageWin.h : header file
//


/////////////////////////////////////////////////////////////////////////////
// Forward Declaration

class CGameParamsSheet;


/////////////////////////////////////////////////////////////////////////////
// CGamePageWin dialog

class CGamePageWin : public CPropertyPage
{
// Declarations
public:
  DECLARE_DYNAMIC(CGamePageWin)
  DECLARE_MESSAGE_MAP()

// Construction
public:
  CGamePageWin();

// Dialog Data
  //{{AFX_DATA(CGamePageWin)
  enum { IDD = IDD_WINNING_CONDITIONS };
  //}}AFX_DATA

// Attributes
public:

// Overrides
public:
  // ClassWizard generate virtual function overrides
  //{{AFX_VIRTUAL(CGamePageWin)
  public:
  virtual void OnOK();
  virtual BOOL OnSetActive();
  protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

// Implementation
protected:

// Message Handlers
protected:
  // Generated message map functions
  //{{AFX_MSG(CGamePageWin)
  virtual BOOL OnInitDialog();
  //}}AFX_MSG

// Data Members
protected:
};


/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
/////////////////////////////////////////////////////////////////////////////