#ifndef __SliderWindow_h__
#define __SliderWindow_h__

/////////////////////////////////////////////////////////////////////////////
// SliderWindow.h | Declaration of the TCSliderWindow class.

#include <TCLib.h>


/////////////////////////////////////////////////////////////////////////////
// TCSliderWindow

class TCSliderWindow : public CWindowImpl<TCSliderWindow>
{
// Declarations
public:
  DECLARE_WND_SUPERCLASS(NULL, TRACKBAR_CLASS)

// Message Map
public:
  BEGIN_MSG_MAP(TCSliderWindow)
  END_MSG_MAP()

// Construction
public:
  TCSliderWindow();

// Attributes / Operations
public:
  BOOL SubclassWindow(HWND hWnd);
  int GetPos() const;
  void GetRange(int& nMin, int& nMax) const;
  int GetRangeMin() const;
  int GetRangeMax() const;
  int GetTic(int nTic) const;
  bool SetTic(int nTic);
  void SetPos(int nPos);
  void SetRange(int nMin, int nMax, bool bRedraw = false);
  void SetRangeMin(int nMin, bool bRedraw = false);
  void SetRangeMax(int nMax, bool bRedraw = false);
  void ClearTics(bool bRedraw = false);
  void SetSelection(int nStart, int nEnd, bool bRedraw = true);
  void SetSel(int nStart, int nEnd, bool bRedraw = true);
  void SetSelStart(int nStart, bool bRedraw = false);
  void SetSelEnd(int nEnd, bool bRedraw = false);
  DWORD* GetTicArray() const;
  int GetTicPos(int nTic) const;
  UINT GetNumTics() const;
  void GetSelection(int& nStart, int& nEnd) const;
  void GetSel(int& nStart, int& nEnd) const;
  int GetSelStart() const;
  int GetSelEnd() const;
  void ClearSel(bool bRedraw = false);
  void SetTicFreq(int nFreq);
  int SetPageSize(int nSize);
  int GetPageSize() const;
  int SetLineSize(int nSize);
  int GetLineSize() const;
  void GetThumbRect(LPRECT lprc) const;
  void GetChannelRect(LPRECT lprc) const;
  void SetThumbLength(int nLength);
  int GetThumbLength() const;
  void SetToolTips(HWND hwndTT);
  HWND GetToolTips() const;
  int SetTipSide(int fLocation);
  HWND SetBuddy(bool bAboveOrLeft, HWND hwndBuddy);
  HWND GetBuddy(bool bAboveOrLeft) const;

// Operators
public:
  const TCSliderWindow& operator=(HWND hwnd);
};


/////////////////////////////////////////////////////////////////////////////
// Construction

inline TCSliderWindow::TCSliderWindow()
{
  // Initialize the slider common control
  INITCOMMONCONTROLSEX icce = {sizeof(icce), ICC_BAR_CLASSES};
  _VERIFYE(TCCommonControls().InitEx(&icce));
}


/////////////////////////////////////////////////////////////////////////////
// Attributes / Operations

inline BOOL TCSliderWindow::SubclassWindow(HWND hWnd)
{
  // Perform default processing
  if (!CWindowImpl<TCSliderWindow>::SubclassWindow(hWnd))
    return FALSE;

  // Fix the slider thumb size
  FixSliderThumbSize(hWnd);

  // Indicate success
  return TRUE;
}

inline int TCSliderWindow::GetPos() const
{
  ZAssert(::IsWindow(m_hWnd));
  return (int) ::SendMessage(m_hWnd, TBM_GETPOS, 0, 0l);
}

inline void TCSliderWindow::GetRange(int& nMin, int& nMax) const
{
  nMin = GetRangeMin();
  nMax = GetRangeMax();
}

inline int TCSliderWindow::GetRangeMin() const
{
  ZAssert(::IsWindow(m_hWnd));
  return (int) ::SendMessage(m_hWnd, TBM_GETRANGEMIN, 0, 0l);
}

inline int TCSliderWindow::GetRangeMax() const
{
  ZAssert(::IsWindow(m_hWnd));
  return (int) ::SendMessage(m_hWnd, TBM_GETRANGEMAX, 0, 0l);
}

inline int TCSliderWindow::GetTic(int nTic) const
{
  ZAssert(::IsWindow(m_hWnd));
  return (int) ::SendMessage(m_hWnd, TBM_GETTIC, nTic, 0L);
}

inline bool TCSliderWindow::SetTic(int nTic)
{
  ZAssert(::IsWindow(m_hWnd));
  return !!::SendMessage(m_hWnd, TBM_SETTIC, 0, nTic);
}

inline void TCSliderWindow::SetPos(int nPos)
{
  ZAssert(::IsWindow(m_hWnd));
  ::SendMessage(m_hWnd, TBM_SETPOS, TRUE, nPos);
}

inline void TCSliderWindow::SetRange(int nMin, int nMax, bool bRedraw)
{
  SetRangeMin(nMin, false);
  SetRangeMax(nMax, bRedraw);
}

inline void TCSliderWindow::SetRangeMin(int nMin, bool bRedraw)
{
  ZAssert(::IsWindow(m_hWnd));
  ::SendMessage(m_hWnd, TBM_SETRANGEMIN, bRedraw, nMin);
}

inline void TCSliderWindow::SetRangeMax(int nMax, bool bRedraw)
{
  ZAssert(::IsWindow(m_hWnd));
  ::SendMessage(m_hWnd, TBM_SETRANGEMAX, bRedraw, nMax);
}

inline void TCSliderWindow::ClearTics(bool bRedraw)
{
  ZAssert(::IsWindow(m_hWnd));
  ::SendMessage(m_hWnd, TBM_CLEARTICS, bRedraw, 0l);
}

inline void TCSliderWindow::SetSelection(int nStart, int nEnd, bool bRedraw)
{
  SetSel(nStart, nEnd);
}

inline void TCSliderWindow::SetSel(int nStart, int nEnd, bool bRedraw)
{
  ZAssert(::IsWindow(m_hWnd));
  SetSelStart(nStart, false);
  SetSelEnd(nEnd, bRedraw);
}

inline void TCSliderWindow::SetSelStart(int nStart, bool bRedraw)
{
  ZAssert(::IsWindow(m_hWnd));
  ::SendMessage(m_hWnd, TBM_SETSELSTART, (WPARAM)bRedraw, nStart);
}

inline void TCSliderWindow::SetSelEnd(int nEnd, bool bRedraw)
{
  ZAssert(::IsWindow(m_hWnd));
  ::SendMessage(m_hWnd, TBM_SETSELEND, (WPARAM)bRedraw, nEnd);
}

inline DWORD* TCSliderWindow::GetTicArray() const
{
  ZAssert(::IsWindow(m_hWnd));
  return (DWORD*) ::SendMessage(m_hWnd, TBM_GETPTICS, 0, 0l);
}

inline int TCSliderWindow::GetTicPos(int nTic) const
{
  ZAssert(::IsWindow(m_hWnd));
  return (int) ::SendMessage(m_hWnd, TBM_GETTICPOS, nTic, 0L);
}

inline UINT TCSliderWindow::GetNumTics() const
{
  ZAssert(::IsWindow(m_hWnd));
  return (UINT) ::SendMessage(m_hWnd, TBM_GETNUMTICS, 0, 0l);
}

inline void TCSliderWindow::GetSelection(int& nStart, int& nEnd) const
{
  GetSel(nStart, nEnd);
}

inline void TCSliderWindow::GetSel(int& nStart, int& nEnd) const
{
  ZAssert(::IsWindow(m_hWnd));
  nStart = GetSelStart();
  nEnd = GetSelEnd();
}

inline int TCSliderWindow::GetSelStart() const
{
  ZAssert(::IsWindow(m_hWnd));
  return (int) ::SendMessage(m_hWnd, TBM_GETSELSTART, 0, 0);
}

inline int TCSliderWindow::GetSelEnd() const
{
  ZAssert(::IsWindow(m_hWnd));
  return (int) ::SendMessage(m_hWnd, TBM_GETSELEND, 0, 0);
}

inline void TCSliderWindow::ClearSel(bool bRedraw)
{
  ZAssert(::IsWindow(m_hWnd));
  ::SendMessage(m_hWnd, TBM_CLEARSEL, bRedraw, 0l);
}

inline void TCSliderWindow::SetTicFreq(int nFreq)
{
  ZAssert(::IsWindow(m_hWnd));
  ::SendMessage(m_hWnd, TBM_SETTICFREQ, nFreq, 0L);
}

inline int TCSliderWindow::SetPageSize(int nSize)
{
  ZAssert(::IsWindow(m_hWnd));
  return (int) ::SendMessage(m_hWnd, TBM_SETPAGESIZE, 0, nSize);
}

inline int TCSliderWindow::GetPageSize() const
{
  ZAssert(::IsWindow(m_hWnd));
  return (int) ::SendMessage(m_hWnd, TBM_GETPAGESIZE, 0, 0l);
}

inline int TCSliderWindow::SetLineSize(int nSize)
{
  ZAssert(::IsWindow(m_hWnd));
  return (int) ::SendMessage(m_hWnd, TBM_SETLINESIZE, 0, nSize);
}

inline int TCSliderWindow::GetLineSize() const
{ 
  ZAssert(::IsWindow(m_hWnd));
  return (int) ::SendMessage(m_hWnd, TBM_GETLINESIZE, 0, 0l);
}

inline void TCSliderWindow::GetThumbRect(LPRECT lprc) const
{
  ZAssert(::IsWindow(m_hWnd));
  ::SendMessage(m_hWnd, TBM_GETTHUMBRECT, 0, (LPARAM)lprc);
}

inline void TCSliderWindow::GetChannelRect(LPRECT lprc) const
{
  ZAssert(::IsWindow(m_hWnd));
  ::SendMessage(m_hWnd, TBM_GETCHANNELRECT, 0, (LPARAM)lprc);
}

inline void TCSliderWindow::SetThumbLength(int nLength)
{
  ZAssert(::IsWindow(m_hWnd));
  ::SendMessage(m_hWnd, TBM_SETTHUMBLENGTH, nLength, 0);
}

inline int TCSliderWindow::GetThumbLength() const
{
  ZAssert(::IsWindow(m_hWnd));
  return (int) ::SendMessage(m_hWnd, TBM_GETTHUMBLENGTH, 0, 0);
}

inline void TCSliderWindow::SetToolTips(HWND hwndTT)
{
  ZAssert(::IsWindow(m_hWnd));
  ::SendMessage(m_hWnd, TBM_SETTOOLTIPS, (WPARAM)hwndTT, 0);
}

inline HWND TCSliderWindow::GetToolTips() const
{
  ZAssert(::IsWindow(m_hWnd));
  return (HWND) ::SendMessage(m_hWnd, TBM_GETTOOLTIPS, 0, 0);
}

inline int TCSliderWindow::SetTipSide(int fLocation)
{
  ZAssert(::IsWindow(m_hWnd));
  return (int) ::SendMessage(m_hWnd, TBM_SETTIPSIDE, fLocation, 0);
}

inline HWND TCSliderWindow::SetBuddy(bool bAboveOrLeft, HWND hwndBuddy)
{
  ZAssert(::IsWindow(m_hWnd));
  return (HWND) ::SendMessage(m_hWnd, TBM_SETBUDDY, (WPARAM)bAboveOrLeft,
    LPARAM(hwndBuddy));
}

inline HWND TCSliderWindow::GetBuddy(bool bAboveOrLeft) const
{
  ZAssert(::IsWindow(m_hWnd));
  return (HWND) ::SendMessage(m_hWnd, TBM_GETBUDDY, (WPARAM)bAboveOrLeft, 0);
}


/////////////////////////////////////////////////////////////////////////////
// Operators

inline const TCSliderWindow& TCSliderWindow::operator=(HWND hwnd)
{
  SubclassWindow(hwnd);
  return *this;
}


/////////////////////////////////////////////////////////////////////////////

#endif // !__SliderWindow_h__
