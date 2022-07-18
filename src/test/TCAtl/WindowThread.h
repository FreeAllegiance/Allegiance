#ifndef __WindowThread_h__
#define __WindowThread_h__

#if _MSC_VER > 1000
  #pragma once
#endif // _MSC_VER > 1000

#include <..\TCLib\WindowThreadImpl.h>


/////////////////////////////////////////////////////////////////////////////
// Description: 
class TCWindowCreatorBase
{
// Overrides
public:
  virtual HWND Create(TCWindowThreadBase::XArgs& args) = 0;
};


/////////////////////////////////////////////////////////////////////////////
// Description: 
template <class T>
class TCWindowImplCreator : public TCWindowCreatorBase
{
// Construction
public:
  TCWindowImplCreator(CWindowImpl<T>* pwi) :
    m_pwi(pwi)
  {
    ZAssert(m_pwi);
  }

// Overrides
public:
  virtual HWND Create(TCWindowThreadBase::XArgs& args)
  {
    RECT rcPos = {args.x, args.y, args.x + args.cx, args.y + args.cy};
    return m_pwi->Create(args.hwndParent, rcPos, args.pszWindowName,
      args.dwStyle, args.dwExStyle, args.nIdOrMenu);
  }

// Data Members
protected:
  CWindowImpl<T>* m_pwi;
};


/////////////////////////////////////////////////////////////////////////////
// Description: 
class TCContainedWindowCreator : public TCWindowCreatorBase
{
// Construction
public:
  TCContainedWindowCreator(CContainedWindow* pcw) :
    m_pcw(pcw)
  {
    ZAssert(m_pcw);
  }

// Overrides
public:
  virtual HWND Create(TCWindowThreadBase::XArgs& args)
  {
    RECT rcPos = {args.x, args.y, args.x + args.cx, args.y + args.cy};
    return m_pcw->Create(args.hwndParent, rcPos, args.pszWindowName,
      args.dwStyle, args.dwExStyle, args.nIdOrMenu);
  }

// Data Members
protected:
  CContainedWindow* m_pcw;
};


/////////////////////////////////////////////////////////////////////////////
// Description: 
template <class T>
class TCDialogImplCreator : public TCWindowCreatorBase
{
// Construction
public:
  TCDialogImplCreator(CDialogImpl<T>* pdi) :
    m_pdi(pdi)
  {
    ZAssert(m_pdi);
  }

// Overrides
public:
  virtual HWND Create(TCWindowThreadBase::XArgs& args)
  {
    RECT rcPos = {args.x, args.y, args.x + args.cx, args.y + args.cy};
    return m_pdi->Create(args.hwndParent);
  }

// Data Members
protected:
  CDialogImpl<T>* m_pdi;
};


/////////////////////////////////////////////////////////////////////////////
// Description: Implements a thread with a message loop that can create
// windows owned by the thread.
//
// See Also: TCWndThread (MFC equivalent - doesn't exist yet)
//
class TCWindowThread : public TCWindowThreadImpl<TCWindowThread>
{
// Group=Overrides
public:
  HWND DoCreateWindowOnThread(XArgs& args);
};


/////////////////////////////////////////////////////////////////////////////
// Group=Overrides

inline HWND TCWindowThread::DoCreateWindowOnThread(
  TCWindowThread::XArgs& args)
{
  // Delegate to the base class method when pvParam is NULL
  if (!args.pvParam)
    return TCWindowThreadImplBase::DoCreateWindowOnThread(args);

  // Reinterpret the parameter as an TCWindowCreatorBase*
  TCWindowCreatorBase* pc = (TCWindowCreatorBase*)(args.pvParam);

  // Create the window as specified
  return pc->Create(args);
}


/////////////////////////////////////////////////////////////////////////////
// Description: 
template <class T>
class TCWindowImpl : public CWindowImpl<T>
{
// Group=Operations
public:
  HWND CreateOnThread(TCWindowThread& wth, HWND hwndParent, RECT& rcPos,
    LPCTSTR szWindowName = NULL, DWORD dwStyle = WS_CHILD | WS_VISIBLE,
    DWORD dwExStyle = 0, UINT nID = 0, int* pnError = NULL,
    bool bSizeIsClient = false, bool bXlateDlgMsgs = false,
    HACCEL hAccel = NULL);
};


/////////////////////////////////////////////////////////////////////////////
// Description: 
class TCContainedWindow : public CContainedWindow
{
// Group=Construction
public:
  TCContainedWindow(LPTSTR pszClassName, CMessageMap* pObject,
    DWORD dwMsgMapID = 0);

// Group=Operations
public:
  HWND CreateOnThread(TCWindowThread& wth, HWND hwndParent, RECT& rcPos,
    LPCTSTR szWindowName = NULL, DWORD dwStyle = WS_CHILD | WS_VISIBLE,
    DWORD dwExStyle = 0, UINT nID = 0, int* pnError = NULL,
    bool bSizeIsClient = false, bool bXlateDlgMsgs = false,
    HACCEL hAccel = NULL);
};


/////////////////////////////////////////////////////////////////////////////
// Description: 
template <class T>
class TCDialogImpl : public CDialogImpl<T>
{
// Group=Operations
public:
  HWND CreateOnThread(TCWindowThread& wth, HWND hwndParent,
    int* pnError = NULL);
};


/////////////////////////////////////////////////////////////////////////////
// Group=Construction

inline TCContainedWindow::TCContainedWindow(LPTSTR pszClassName,
  CMessageMap* pObject, DWORD dwMsgMapID) :
  CContainedWindow(pszClassName, pObject, dwMsgMapID)
{
}


/////////////////////////////////////////////////////////////////////////////
// Group=Operations

template <class T>
inline HWND TCWindowImpl<T>::CreateOnThread(TCWindowThread& wth,
  HWND hwndParent, RECT& rcPos, LPCTSTR szWindowName, DWORD dwStyle,
  DWORD dwExStyle, UINT nID, int* pnError, bool bSizeIsClient,
  bool bXlateDlgMsgs, HACCEL hAccel)
{
  TCWindowImplCreator<T> wic(this);
  return wth.CreateWindowExOnThread(dwExStyle, NULL, szWindowName, dwStyle,
    rcPos.left, rcPos.top, rcPos.right - rcPos.left,
    rcPos.bottom - rcPos.top, hwndParent, nID, NULL, &wic, pnError,
    bSizeIsClient, bXlateDlgMsgs, hAccel);
}

inline HWND TCContainedWindow::CreateOnThread(TCWindowThread& wth,
  HWND hwndParent, RECT& rcPos, LPCTSTR szWindowName, DWORD dwStyle,
  DWORD dwExStyle, UINT nID, int* pnError, bool bSizeIsClient,
  bool bXlateDlgMsgs, HACCEL hAccel)
{
  TCContainedWindowCreator cwc(this);
  return wth.CreateWindowExOnThread(dwExStyle, NULL, szWindowName, dwStyle,
    rcPos.left, rcPos.top, rcPos.right - rcPos.left,
    rcPos.bottom - rcPos.top, hwndParent, nID, NULL, &cwc, pnError,
    bSizeIsClient, bXlateDlgMsgs, hAccel);
}

template <class T>
inline HWND TCDialogImpl<T>::CreateOnThread(TCWindowThread& wth,
  HWND hwndParent, int* pnError)
{
  TCDialogImplCreator<T> dic(this);
  return wth.CreateWindowOnThread(NULL, NULL, 0, 0, 0, 0, 0, hwndParent, 0,
    NULL, &dic, pnError, false, true, NULL);
}


/////////////////////////////////////////////////////////////////////////////

#endif // !__WindowThread_h__
