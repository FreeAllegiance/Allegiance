#pragma once

/////////////////////////////////////////////////////////////////////////////
// SrvConfigDlg.h : header file
//

#include "PageDummy.h"
#include "PageConnect.h"
#include "PageGameCreate.h"
#include "PageChat.h"
#include "PagePlayers.h"
#include "AutoSizer.h"


/////////////////////////////////////////////////////////////////////////////
// CAllSrvUISheet dialog

class CAllSrvUISheet : public CPropertySheet
{
// Declarations
protected:
  DECLARE_MESSAGE_MAP()

// Construction / Destruction
public:
  CAllSrvUISheet(CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
  ~CAllSrvUISheet();
  BOOL Create();

// Attributes
public:

// Implementation
protected:
  void InitSysMenu();
  void InitIcon();
  void AddChildrenToAutoSizer();
public:
  void UpdateStatus();
  HRESULT HandleError(HRESULT hr, LPCSTR pszContext, bool bExit);
  IAdminSession* GetSession()      { return m_spSession;  }
  IAdminServer* GetServer()        { return m_spServer;   }
  IAdminGame* GetGame()            { return m_spGame;     }
  bool IsServerInMultiMode() const { return m_bMultiMode; }
  bool SelectGame();
  HRESULT CreateGame(bool bLobby, IAGCGameParameters* pGameParameters);
  HRESULT DestroyGame();
  HRESULT PostConnect(DWORD dwCookie);

// Overrides
public:
  void OnEvent(IAGCEvent* pEvent);
  // ClassWizard generate virtual function overrides
  //{{AFX_VIRTUAL(CAllSrvUISheet)
  public:
  virtual void PostNcDestroy();
  protected:
  virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
  //}}AFX_VIRTUAL

// Events Sub-object
protected:
  struct XEvents : public IAdminSessionEvents
  {
  // Construction / Destruction
    XEvents(CAllSrvUISheet* pThis);
    virtual ~XEvents();
  // IUnknown Interface Methods
    STDMETHODIMP QueryInterface(REFIID riid, void** ppUnk);
    STDMETHODIMP_(ULONG) AddRef(void);
    STDMETHODIMP_(ULONG) Release(void);
  // IAdminSessionEvents
    STDMETHODIMP OnEvent(IAGCEvent* pEvent);
  // Data Members
  private:
    CAllSrvUISheet* m_pThis;
    ULONG           m_nRefs;
  };
  friend XEvents;

// Message Handlers
protected:
  // Generated message map functions
  //{{AFX_MSG(CAllSrvUISheet)
  virtual BOOL OnInitDialog();
  afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
  afx_msg void OnSize(UINT nType, int cx, int cy);
  afx_msg void OnMove(int x, int y);
  afx_msg void OnTimer(UINT_PTR nIDEvent);
  afx_msg void OnDestroy();
  afx_msg void OnClose();
  //}}AFX_MSG

// Enumerations
protected:
  enum {c_cTimersMin, c_SizeTimer, c_StageTimer, c_cTimersMax};
  enum {c_SizeTimeout = 200, c_StageTimeout = 2000,};

// Data Members
protected:
  CPageDummy          m_PageDummy;
  CPageConnect        m_PageConnect;
  CPageGameCreate     m_PageGameCreate;
  CPageChat           m_PageChat;
  CPagePlayers        m_PagePlayers;
  IAdminSessionPtr    m_spSession;
  IConnectionPointPtr m_spcp;
  IAdminServerPtr     m_spServer;
  IAdminGamePtr       m_spGame;
  DWORD               m_dwConnectionCookie;
  CAutoSizer          m_AutoSizer;
  CStatusBarCtrl      m_wndStatusBar;
  bool                m_bInitDone;
  bool                m_bMultiMode;
};

/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
/////////////////////////////////////////////////////////////////////////////