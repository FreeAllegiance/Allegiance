#pragma once

/////////////////////////////////////////////////////////////////////////////
// AdminPageThreadWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAdminPageThreadWnd window

class CAdminPageThreadWnd : public CWnd
{
// Declarations
public:
	DECLARE_MESSAGE_MAP()

// Construction
public:
	CAdminPageThreadWnd();
  BOOL Create();
  BOOL Create(CString strShortcutPath);

// Operations
public:
  HRESULT PostAdminPageMsg(IAGCEvent* pEvent);
  HRESULT PostAdminPageRun(IAGCEvent* pEvent);

// Implementation
protected:
  HRESULT PostAdminPage(IAGCEvent* pEvent, UINT message);
  CString GetDate();
  CString GetSender();
  CString GetMessage();
  void TranslateEvent(WPARAM wParam);
  bool VariantTimeToLocalTime(DATE date, SYSTEMTIME* psystime);

// Message Handlers
protected:
  afx_msg LRESULT OnAdminPageMsg(WPARAM wParam, LPARAM);
  afx_msg LRESULT OnAdminPageRun(WPARAM wParam, LPARAM);

// Enumerations
protected:
  enum
  {
    wm_AdminPageMsg = WM_APP,
    wm_AdminPageRun,
  };
// Data Members
protected:
  IAGCEventPtr       m_spEvent;
  IPersistStreamPtr  m_spEventPersist;
  CString            m_strShortcutPath;
};

/////////////////////////////////////////////////////////////////////////////