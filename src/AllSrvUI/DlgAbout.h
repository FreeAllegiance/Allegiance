#pragma once

/////////////////////////////////////////////////////////////////////////////
// DlgAbout.h : header file
//


/////////////////////////////////////////////////////////////////////////////
// CDlgAbout dialog

class CDlgAbout : public CDialog
{
// Declarations
public:
  DECLARE_MESSAGE_MAP()

// Construction
public:
  CDlgAbout(CWnd* pParent);

// Overrides
public:
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CDlgAbout)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

// Message Handlers
protected:
  // Generated message map functions
  //{{AFX_MSG(CDlgAbout)
  virtual BOOL OnInitDialog();
  //}}AFX_MSG

// Data Members
protected:
  //{{AFX_DATA(CDlgAbout)
  enum { IDD = IDD_ABOUTBOX };
  CString m_strDescription;
  CString m_strVersion;
  CString m_strProductID;
  CString m_strCopyright;
  CString m_strLegalWarning;
  //}}AFX_DATA
};


/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
/////////////////////////////////////////////////////////////////////////////