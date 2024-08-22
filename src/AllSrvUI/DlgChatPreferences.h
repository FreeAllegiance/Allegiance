#pragma once

/////////////////////////////////////////////////////////////////////////////
// DlgChatPreferences.h : header file
//


/////////////////////////////////////////////////////////////////////////////
// Forward Declaration

class CPageChat;


/////////////////////////////////////////////////////////////////////////////
// CDlgChatPreferences dialog

class CDlgChatPreferences : public CDialog
{
// Declarations
public:
  DECLARE_MESSAGE_MAP()

// Construction
public:
  CDlgChatPreferences(CPageChat* pPage);

// Attributes:
protected:
  bool IsAdminPageMonitored() const
  {
    return m_bAdminMessageBox || m_bAdminRun || m_bAdminChatList;
  }
  bool IsChatMonitored() const
  {
    return !!m_bEnableChatList;
  }
  CPageChat* GetPage()
  {
    return m_pPage;
  }

// Overrides
public:
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CDlgChatPreferences)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

// Implementation
protected:
  void LoadFromRegistry();
  void SaveToRegistry();
  bool ShortcutExists();
  void UpdateUI();

// Message Handlers
protected:
  // Generated message map functions
  //{{AFX_MSG(CDlgChatPreferences)
  virtual BOOL OnInitDialog();
  virtual void OnOK();
	afx_msg void OnEnableChatList();
	afx_msg void OnUnlimitedChatHistory();
	afx_msg void OnLimitChatHistory();
	afx_msg void OnAdminRun();
	afx_msg void OnAdminRunProperties();
	afx_msg void OnAdminRunTest();
	//}}AFX_MSG

// Data Members
protected:
  //{{AFX_DATA(CDlgChatPreferences)
	enum { IDD = IDD_CHAT_PREFERENCES };
  CButton m_btnAdminRunProperties;
	CButton	m_btnUnlimitedChat;
  CButton m_btnLimitChat;
	CEdit	m_editChatListLimit;
  CStatic m_staticChatListLimit;
	BOOL	m_bAdminChatList;
  BOOL  m_bAdminMessageBox;
  BOOL  m_bAdminRun;
	BOOL	m_bEnableChatList;
  int   m_eChatListLimit;
  DWORD m_dwChatListLimit;
	//}}AFX_DATA
protected:
  CString m_strShortcutPath;
  CPageChat* m_pPage;

// Friends
protected:
  friend class CPageChat;
};


/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
/////////////////////////////////////////////////////////////////////////////