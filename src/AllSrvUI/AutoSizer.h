#pragma once

/////////////////////////////////////////////////////////////////////////////
// AutoSizer.h | Declaration of the CAutoSizer class.

#include <vector>


/////////////////////////////////////////////////////////////////////////////
// Enumerations

enum AutoSizer_Follower
{
  AutoSizer_Follow_Left,
  AutoSizer_Follow_Right,
  AutoSizer_Follow_LeftRight,
  AutoSizer_Follow_HCenter,
  AutoSizer_Follow_Top,
  AutoSizer_Follow_Bottom,
  AutoSizer_Follow_TopBottom,
  AutoSizer_Follow_VCenter,
};

enum AutoSizer_Leader
{
  AutoSizer_Lead_Left,
  AutoSizer_Lead_Right,
  AutoSizer_Lead_HCenter,
  AutoSizer_Lead_Top,
  AutoSizer_Lead_Bottom,
  AutoSizer_Lead_VCenter,
};

enum AutoSizer_Refresh
{
  AutoSizer_Refresh_NoRefresh,
  AutoSizer_Refresh_Refresh,
  AutoSizer_Refresh_BkRefresh,
};


/////////////////////////////////////////////////////////////////////////////
// AutoSizerRule

struct AutoSizerRule
{
// Data Members
public:
  #pragma pack(push, 4)
    UINT               m_idFollower;
    UINT               m_idLeader;
    AutoSizer_Follower m_eFollower :12;
    AutoSizer_Leader   m_eLeader   :12;
    AutoSizer_Refresh  m_eRefresh  : 8;
  #pragma pack(pop)

// Attributes
public:
  bool IsEnd() const
  {
    return !m_idFollower;
  }
};


/////////////////////////////////////////////////////////////////////////////
// Rule Map Macros

#define BEGIN_AUTO_SIZER_MAP(name)                                          \
  static AutoSizerRule name [] =                                            \
  {

#define AUTO_SIZER_ENTRY(idFollow, edgeFollow, idLead, edgeLead, refresh)   \
    {                                                                       \
      (UINT)idFollow,                                                       \
      (UINT)idLead,                                                         \
      AutoSizer_Follow_##edgeFollow,                                        \
      AutoSizer_Lead_##edgeLead,                                            \
      AutoSizer_Refresh_##refresh                                           \
    },

#define END_AUTO_SIZER_MAP()                                                \
    AUTO_SIZER_ENTRY(0, Left, 0, Left, NoRefresh)                           \
  };


/////////////////////////////////////////////////////////////////////////////
// CAutoSizer

class CAutoSizer
{
// Construction / Destruction
public:
  CAutoSizer();

// Attributes
public:
  bool SetWindowAndRules(HWND hwnd, const AutoSizerRule* pRules);
  bool SetMinSize(int cx, int cy, bool bDlgUnits = true);
  bool SetMinSize(SIZE size, bool bDlgUnits = true);
  SIZE GetMinSize() const;
  HWND GetWindow() const;
  const RECT* GetClientRect() const;
  const RECT* GetOriginalClientRect() const;

// Operations
public:
  bool ProcessMessage(MSG* pMsg);
  bool ProcessMessage(UINT uMsg, WPARAM wp, LPARAM lp, LRESULT* plr);
  bool AddRule(HWND hwndFollower, AutoSizer_Follower edgeFollower,
    HWND hwndLeader, AutoSizer_Leader edgeLeader, AutoSizer_Refresh refresh);
  bool RemoveRules(HWND hwndFollower);

// Implementation
protected:
  void ComputeMinRects();
  void UpdatePositions();

// Message Handlers
protected:
  void OnDestroy();
  void OnGetMinMaxInfo(MINMAXINFO* pMMI);
  void OnSize(UINT nType, int cx, int cy);
  void OnStyleChanged();

// Types
protected:
  struct XRule
  {
    XRule();
    XRule(const AutoSizerRule* pRule);
    XRule(const XRule& that);
    const XRule& operator=(const AutoSizerRule* pRule);
    const XRule& operator=(const XRule& that);
    bool operator==(const AutoSizerRule* pRule);
    HWND GetFollower() const;
    HWND GetLeader() const;
    void ResolveIDs(CAutoSizer* pSizer);
    bool SaveInitialOffsets(CAutoSizer* pSizer);
    bool UpdateRectangle(CAutoSizer* pSizer, RECT& rcNew) const;
  private:
    AutoSizerRule m_Rule;
    int           m_Offsets[2];
  };
  typedef std::vector<XRule> XRules;
  typedef XRules::iterator   XRuleIt;

// Data Members
protected:
  XRules m_Rules;
  HWND   m_hwnd;
  RECT   m_rcMinClient;
  RECT   m_rcMinWindow;
  RECT   m_rcClient;
  RECT   m_rcClientOriginal;
  bool   m_bRecomputeMinWindow : 1;
};


/////////////////////////////////////////////////////////////////////////////
// Inline Attributes

inline bool CAutoSizer::SetMinSize(SIZE size, bool bDlgUnits)
{
  return SetMinSize(size.cx, size.cy, bDlgUnits);
}

inline SIZE CAutoSizer::GetMinSize() const
{
  SIZE size = {m_rcMinClient.right, m_rcMinClient.bottom};
  return size;
}

inline HWND CAutoSizer::GetWindow() const
{
  return m_hwnd;
}

inline const RECT* CAutoSizer::GetClientRect() const
{
  return &m_rcClient;
}

inline const RECT* CAutoSizer::GetOriginalClientRect() const
{
  return &m_rcClientOriginal;
}


/////////////////////////////////////////////////////////////////////////////

inline CAutoSizer::XRule::XRule()
{
  ZeroMemory(&m_Rule, sizeof(m_Rule));
  ZeroMemory(m_Offsets, sizeof(m_Offsets));
}

inline CAutoSizer::XRule::XRule(const AutoSizerRule* pRule)
{
  CopyMemory(&m_Rule, pRule, sizeof(m_Rule));
}

inline CAutoSizer::XRule::XRule(const XRule& that)
{
  CopyMemory(&m_Rule, &that.m_Rule, sizeof(m_Rule));
  CopyMemory(m_Offsets, that.m_Offsets, sizeof(m_Offsets));
}

inline const CAutoSizer::XRule& CAutoSizer::XRule::operator=(const AutoSizerRule* pRule) 
{ 
  CopyMemory(&m_Rule, pRule, sizeof(m_Rule)); 
  ZeroMemory(m_Offsets, sizeof(m_Offsets)); 
  return *this; 
} 

inline const CAutoSizer::XRule& CAutoSizer::XRule::operator=(const XRule& that) 
{ 
  CopyMemory(&m_Rule, &that.m_Rule, sizeof(m_Rule)); 
  CopyMemory(m_Offsets, that.m_Offsets, sizeof(m_Offsets)); 
  return *this; 
} 


inline bool CAutoSizer::XRule::operator==(const AutoSizerRule* pRule)
{
  return 0 == memcmp(pRule, &m_Rule, sizeof(m_Rule));
}

inline HWND CAutoSizer::XRule::GetFollower() const
{
  HWND hwnd = reinterpret_cast<HWND>(m_Rule.m_idFollower);
  _ASSERTE(::IsWindow(hwnd));
  return hwnd;
}

inline HWND CAutoSizer::XRule::GetLeader() const
{
  HWND hwnd = reinterpret_cast<HWND>(m_Rule.m_idLeader);
  _ASSERTE(::IsWindow(hwnd));
  return hwnd;
}

inline void CAutoSizer::XRule::ResolveIDs(CAutoSizer* pSizer)
{
  HWND hwndParent = pSizer->GetWindow();
  _ASSERTE(::IsWindow(hwndParent));

  HWND hwndFollower = m_Rule.m_idFollower ?
    ::GetDlgItem(hwndParent, m_Rule.m_idFollower) : hwndParent;
  _ASSERTE(::IsWindow(hwndFollower));
  m_Rule.m_idFollower = reinterpret_cast<UINT>(hwndFollower);

  HWND hwndLeader = m_Rule.m_idLeader ?
    ::GetDlgItem(hwndParent, m_Rule.m_idLeader) : hwndParent;
  _ASSERTE(::IsWindow(hwndLeader));
  m_Rule.m_idLeader = reinterpret_cast<UINT>(hwndLeader);
}


/////////////////////////////////////////////////////////////////////////////
// Message Macros for ATL usage

#ifdef _ATL
  #define AUTO_SIZER_MESSAGE_HANDLER(autoSizer)                             \
    if (autoSizer.ProcessMessage(uMsg, wParam, lParam, &lResult))           \
      return TRUE;
#endif // _ATL


/////////////////////////////////////////////////////////////////////////////