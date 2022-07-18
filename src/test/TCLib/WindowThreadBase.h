#ifndef __WindowThreadBase_h__
#define __WindowThreadBase_h__

#if _MSC_VER > 1000
  #pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable: 4786)
#include <map>
#include <utility>
#include <malloc.h>

#include <..\MMACRtl\AutoHandle.h>


/////////////////////////////////////////////////////////////////////////////
// Description: Non-template base class for TCWindowThreadImpl.
//
// See Also: TCWindowThreadImpl, TCWindowThread
//
class TCWindowThreadBase
{
// Group=Types
public:
  struct XArgs
  {
    HANDLE    hevt;
    void*     pvParam;
    HACCEL    hAccel;
    bool      bXlateDlgMsgs;
    HWND      hwnd;
    HWND      hwndParent;
    LPCTSTR   pszClassName, pszWindowName;
    int       x, y, cx, cy;
    DWORD     dwStyle, dwExStyle;
    UINT      nIdOrMenu;
    HINSTANCE hInstance;
    DWORD     dwLastError;
  };
protected:
  typedef TCObjectLock<TCWindowThreadBase> CLock;
  struct XThreadArgs
  {
    void*  pvThis;
    HANDLE hevt;
  };
  struct XItem
  {
    HACCEL hAccel;
    bool   bXlateDlgMsgs;
  };
  typedef std::map<HWND, XItem> XItems;

// Group=Construction / Destruction
public:
  TCWindowThreadBase(DWORD dwCoInit);
  virtual ~TCWindowThreadBase();

// Disallow copy constructor
private:
  TCWindowThreadBase(const TCWindowThreadBase&);

// Group=Attributes
public:
  TCThread* GetThread();
  virtual TCThread* GetOrCreateThread() = 0;
  static int GetTitleWidth(LPCTSTR psz);

// Group=Operations
public:
  HWND CreateWindowOnThread(LPCTSTR pszWindowName, int* pnError = NULL);
  HWND CreateWindowOnThread(LPCTSTR pszClassName, LPCTSTR pszWindowName,
    DWORD dwStyle, int x, int y, int nWidth, int nHeight, HWND hwndParent,
    UINT nIdOrMenu, HINSTANCE hInstance, void* pvParam,
    int* pnError = NULL, bool bSizeIsClient = false,
    bool bXlateDlgMsgs = false, HACCEL hAccel = NULL);
  HWND CreateWindowExOnThread(DWORD dwExStyle, LPCTSTR pszClassName,
    LPCTSTR pszWindowName, DWORD dwStyle, int x, int y,
    int nWidth, int nHeight, HWND hwndParent, UINT nIdOrMenu,
    HINSTANCE hInstance, void* pvParam,
    int* pnError = NULL, bool bSizeIsClient = false,
    bool bXlateDlgMsgs = false, HACCEL hAccel = NULL);
  bool DestroyWindowOnThread(HWND hwnd);
  DWORD ShutdownWindowThread(DWORD dwMilliseconds = INFINITE);

// Group=Overrides
public:
  HWND DoCreateWindowOnThread(XArgs& args);
  bool DoDestroyWindowOnThread(HWND hwnd);
  bool DoInitThread();
  void DoExitThread();

// Group=Implementation
public:
  void Lock();
  void Unlock();
protected:
  long GetRefsDlgXlate();
  long GetRefsAccel();
  virtual void InternalCreateWindow(XArgs& args) = 0;
  virtual void InternalDestroyWindow(XArgs& args) = 0;
  void AddWindow(XArgs& args);
  void RemoveWindow(HWND hwnd);
  static ATOM& GetWndProcAtom();
  static void SubclassWindow(HWND hwnd);
  static void UnsubclassWindow(HWND hwnd);
  static LRESULT CALLBACK SubclassWindowProc(HWND, UINT, WPARAM, LPARAM);
  void MessageLoop();
  bool ProcessThreadMessage(MSG& msg);
  bool TranslateAccelerators(MSG& msg);
  bool TranslateDialogMessages(MSG& msg);

// Group=Enumerations
public:
  enum EErrorCodes
  {
    e_Ok,
    e_EventCreationFailed,
    e_ThreadCreationFailed,
    e_WindowCreationFailed,
    e_WindowDestructionFailed,
  };
protected:
  enum
  {
    wm_CreateWindowOnThread = WM_APP,
    wm_DestroyWindowOnThread,
    wm_WindowDestroyed,
  };

// Group=Data Members
protected:
  DWORD                 m_dwCoInit;
  long                  m_nRefsDlgXlate, m_nRefsAccel;
  TCThread*             m_pth;
  TCAutoCriticalSection m_cs;
  XItems                m_windows;
};


/////////////////////////////////////////////////////////////////////////////
// Group=Construction / Destruction

inline TCWindowThreadBase::TCWindowThreadBase(DWORD dwCoInit) :
  m_dwCoInit(dwCoInit),
  m_nRefsDlgXlate(0),
  m_nRefsAccel(0),
  m_pth(NULL)
{
  // Create or AddRef a global atom
  GetWndProcAtom() = ::GlobalAddAtom(_T("TCWindowThreadBase::PrevWndProc"));
}

inline TCWindowThreadBase::~TCWindowThreadBase()
{
  // Delete or Release the global atom
  GlobalDeleteAtom(GetWndProcAtom());
}


/////////////////////////////////////////////////////////////////////////////
// Group=Attributes

inline TCThread* TCWindowThreadBase::GetThread()
{
  CLock lock(this);
  return m_pth;
}

inline int TCWindowThreadBase::GetTitleWidth(LPCTSTR psz)
{
  SIZE size = {0, 0};
  if (psz)
    GetTextExtentPoint32(TCWindowDC(NULL), psz, _tcslen(psz), &size);
  return size.cx;
}


/////////////////////////////////////////////////////////////////////////////
// Group=Operations

inline HWND TCWindowThreadBase::CreateWindowOnThread(LPCTSTR pszWindowName,
  int* pnError)
{
  // Get the screen extents
  int cxScreen = ::GetSystemMetrics(SM_CXSCREEN);
  int cyScreen = ::GetSystemMetrics(SM_CYSCREEN);

  // Get the client width needed for the title
  //   and compute the height, proportional to the screen
  int cx = GetTitleWidth(pszWindowName);
  int cy = cx * cyScreen / cxScreen;

  // Define the window style
  const DWORD dwStyle = WS_OVERLAPPED | WS_THICKFRAME | WS_CAPTION;

  // Determine which instance handle to use
  #if defined(_ATL)
    HINSTANCE hinst = _Module.GetModuleInstance();
  #elif defined(_AFX)
    HINSTANCE hinst = AfxGetInstanceHandle();
  #else
    HINSTANCE hinst = GetModuleHandle(NULL);
  #endif

  // Create a dialog window
  HWND hwnd = CreateWindowOnThread(WC_DIALOG, pszWindowName, dwStyle,
    0, 0, cx, cy, NULL, 0, hinst, NULL, pnError, true, true, NULL);

  // Center the window
  if (hwnd)
  {
    // Get the window rectangle
    RECT rc;
    ::GetWindowRect(hwnd, &rc);
    cx = rc.right - rc.left;
    cy = rc.bottom - rc.top;

    // Compute the centered origin
    const int x = (cxScreen - cx) / 2;
    const int y = (cyScreen - cy) / 2;

    // Position the window
    const UINT uFlags = SWP_NOSIZE | SWP_NOZORDER;
    ::SetWindowPos(hwnd, NULL, x, y, 0, 0, uFlags);
  }

  // Return the created window
  return hwnd;
}

inline HWND TCWindowThreadBase::CreateWindowOnThread(LPCTSTR pszClassName,
  LPCTSTR pszWindowName, DWORD dwStyle, int x, int y,
  int nWidth, int nHeight, HWND hwndParent, UINT nIdOrMenu,
  HINSTANCE hInstance, void* pvParam, int* pnError, bool bSizeIsClient,
  bool bXlateDlgMsgs, HACCEL hAccel)
{
  return CreateWindowExOnThread(0, pszClassName, pszWindowName, dwStyle,
    x, y, nWidth, nHeight, hwndParent, nIdOrMenu, hInstance, pvParam,
    pnError, bSizeIsClient, bXlateDlgMsgs, hAccel);
}

inline HWND TCWindowThreadBase::CreateWindowExOnThread(DWORD dwExStyle,
  LPCTSTR pszClassName, LPCTSTR pszWindowName, DWORD dwStyle, int x, int y,
  int nWidth, int nHeight, HWND hwndParent, UINT nIdOrMenu,
  HINSTANCE hInstance, void* pvParam, int* pnError, bool bSizeIsClient,
  bool bXlateDlgMsgs, HACCEL hAccel)
{
  HWND hwnd = NULL;
  int nError = e_Ok;

  // Ensure that the thread is created
  if (!GetOrCreateThread())
    nError = e_ThreadCreationFailed;
  else
  {
    // Compute the size of the window, if client size was specified
    if (bSizeIsClient)
    {
      RECT rc = {0, 0, nWidth, nHeight};
      AdjustWindowRectEx(&rc, dwStyle, !hwndParent && nIdOrMenu, dwExStyle);
      nWidth = rc.right - rc.left;
      nHeight = rc.bottom - rc.top;
    }

    // Create a structure for the arguments
    XArgs args =
    {
      NULL, pvParam, hAccel, bXlateDlgMsgs, NULL, hwndParent,
      pszClassName, pszWindowName, x, y, nWidth, nHeight, dwStyle, dwExStyle,
      nIdOrMenu, hInstance, 0
    };

    // Only post the thread message when called from another thread
    if (GetCurrentThreadId() != GetThread()->m_nThreadID)
    {
      // Create an event to be signaled when the window creation is finished
      TCHandle hevt = CreateEvent(NULL, FALSE, FALSE, NULL);
      if (!hevt.GetHandle())
        nError = e_EventCreationFailed;
      else
      {
        // Post a message to the thread, telling it to create the window
        args.hevt = hevt;
        if (!GetThread()->PostThreadMessage(wm_CreateWindowOnThread,
          WPARAM(&args), 0))
            nError = e_WindowCreationFailed;
        else
          WaitForSingleObject(hevt, INFINITE);
      }
    }
    else
    {
      // Call the creation method directly when called from same thread
      InternalCreateWindow(args);
    }

    // Get the created window handle and set the last error
    if (!nError && !(hwnd = args.hwnd))
    {
      ::SetLastError(args.dwLastError);
      nError = e_WindowCreationFailed;
    }
  }

  if (pnError)
    *pnError = nError;
  return hwnd;
}

inline bool TCWindowThreadBase::DestroyWindowOnThread(HWND hwnd)
{
  // Fail if thread is not created
  if (!GetThread())
    return false;

  // Create a structure for the arguments
  XArgs args = {NULL, NULL, false, NULL, hwnd};

  // Only post the thread message when called from another thread
  if (GetCurrentThreadId() != GetThread()->m_nThreadID)
  {
    // Create an event to be signaled when the window destruction is finished
    TCHandle hevt = CreateEvent(NULL, FALSE, FALSE, NULL);
    if (!hevt.GetHandle())
      return false;

    // Post a message to the thread, telling it to destroy the window
    args.hevt = hevt;
    if (!GetThread()->PostThreadMessage(wm_DestroyWindowOnThread,
      WPARAM(&args), 0))
        return false;

    // Wait for the window destruction event
    WaitForSingleObject(hevt, INFINITE);
  }
  else
  {
    // Call the destroy method directly when called from same thread
    InternalDestroyWindow(args);
  }

  // Get the created window handle and set the last error
  if (!args.hwnd)
  {
    ::SetLastError(args.dwLastError);
    return false;
  }

  // Indicate success
  return true;
}

inline DWORD TCWindowThreadBase::ShutdownWindowThread(DWORD dwMilliseconds)
{
  // Get some information from the thread
  HANDLE hth;
  DWORD idth;
  HWND* phwnds;
  HWND* phwndNext;
  {
    CLock lock(this);

    // Do nothing if the thread does not exist
    if (!GetThread())
      return WAIT_OBJECT_0;

    // Get the thread handle and ID
    hth = GetThread()->m_hThread;
    idth = GetThread()->m_nThreadID;

    // Create an array of the window handles
    phwndNext = phwnds = (HWND*)_alloca((m_windows.size()+1) * sizeof(HWND));
    for (XItems::iterator it = m_windows.begin(); it != m_windows.end(); ++it)
      *phwndNext++ = it->first;
    *phwndNext = NULL;
  }

  // Destroy each window
  for (phwndNext = phwnds; *phwndNext; ++phwndNext)
    DestroyWindowOnThread(*phwndNext);

  // Post a quit message to the thread
  ::PostThreadMessage(idth, WM_QUIT, 0, 0);

  // Wait the specified amount of time for the thread to exit
  return WaitForSingleObject(hth, dwMilliseconds);
}


/////////////////////////////////////////////////////////////////////////////
// Group=Overrides

inline HWND TCWindowThreadBase::DoCreateWindowOnThread(
  TCWindowThreadBase::XArgs& args)
{
  // Attempt to create the window
  HWND hwnd = ::CreateWindowEx(args.dwExStyle, args.pszClassName,
    args.pszWindowName, args.dwStyle, args.x, args.y, args.cx, args.cy,
    args.hwndParent, (HMENU)args.nIdOrMenu, args.hInstance, args.pvParam);
  return hwnd;
}

inline bool TCWindowThreadBase::DoDestroyWindowOnThread(HWND hwnd)
{
  ::SendMessage(hwnd, WM_CLOSE, 0, 0);
  return ::IsWindow(hwnd) ? 0 != ::DestroyWindow(hwnd) : true;
}

inline bool TCWindowThreadBase::DoInitThread()
{
  // Enter this thread into a COM apartment, if specified
  if (DWORD(-1) != m_dwCoInit)
    if (FAILED(CoInitializeEx(NULL, m_dwCoInit)))
      return false;

  // Lock the module so that it can't unload while thread is active
  #ifdef _ATL
    _Module.Lock();
  #endif // _ATL

  // Indicate success
  return true;
}

inline void TCWindowThreadBase::DoExitThread()
{
  // Unlock the module
  #ifdef _ATL
    _Module.Unlock();
  #endif // _ATL

  // Remove this thread from the COM apartment, if it entered one
  if (DWORD(-1) != m_dwCoInit)
    CoUninitialize();
}


/////////////////////////////////////////////////////////////////////////////
// Group=Implementation

inline void TCWindowThreadBase::Lock()
{
  m_cs.Lock();
}

inline void TCWindowThreadBase::Unlock()
{
  m_cs.Unlock();
}

inline long TCWindowThreadBase::GetRefsDlgXlate()
{
  CLock lock(this);
  return m_nRefsDlgXlate;
}

inline long TCWindowThreadBase::GetRefsAccel()
{
  CLock lock(this);
  return m_nRefsAccel;
}

inline void TCWindowThreadBase::AddWindow(TCWindowThreadBase::XArgs& args)
{
  // Do nothing with a NULL handle
  if (!args.hwnd)
    return;

  // Add the window to the collection
  CLock lock(this);
  XItem item = {args.hAccel, args.bXlateDlgMsgs};
  m_windows.insert(XItems::value_type(args.hwnd, item));

  // Update translation reference counts
  if (args.hAccel)
    ++m_nRefsAccel;
  if (args.bXlateDlgMsgs)
    ++m_nRefsDlgXlate;
}

inline void TCWindowThreadBase::RemoveWindow(HWND hwnd)
{
  ZAssert(hwnd);

  // Find the item for the specified handle
  CLock lock(this);
  XItems::iterator it = m_windows.find(hwnd);
  ZAssert(it != m_windows.end());

  // Update translation reference counts
  if (it->second.hAccel)
    --m_nRefsAccel;
  if (it->second.bXlateDlgMsgs)
    --m_nRefsDlgXlate;

  // Remove the window from the collection
  m_windows.erase(it);
}

inline ATOM& TCWindowThreadBase::GetWndProcAtom()
{
  static ATOM atom = NULL;
  return atom;
}

inline void TCWindowThreadBase::SubclassWindow(HWND hwnd)
{
  // Do nothing with a NULL handle
  if (!hwnd)
    return;

  // Set the new WindowProc and save the old one as a window property
  ::SetProp(hwnd, MAKEINTATOM(GetWndProcAtom()),
    (HANDLE)::SetWindowLong(hwnd, GWL_WNDPROC, (LONG)SubclassWindowProc));
}

inline void TCWindowThreadBase::UnsubclassWindow(HWND hwnd)
{
  // Do nothing with a NULL handle
  if (!hwnd)
    return;

  // Get the previous WindowProc and restore it
  WNDPROC pfnPrev = (WNDPROC)::GetProp(hwnd, MAKEINTATOM(GetWndProcAtom()));
  ZAssert(pfnPrev);
  WNDPROC pfn = (WNDPROC)::SetWindowLong(hwnd, GWL_WNDPROC, (LONG)pfnPrev);
  ZAssert(SubclassWindowProc == pfn);

  // Remove the window property
  ::RemoveProp(hwnd, MAKEINTATOM(GetWndProcAtom()));
}

inline LRESULT CALLBACK TCWindowThreadBase::SubclassWindowProc(HWND hwnd,
  UINT uMsg, WPARAM wp, LPARAM lp)
{
  // Get the previous WindowProc
  WNDPROC pfnPrev = (WNDPROC)::GetProp(hwnd, MAKEINTATOM(GetWndProcAtom()));
  ZAssert(pfnPrev);

  // Check for interesting messages
  if (WM_NCDESTROY == uMsg)
  {
    UnsubclassWindow(hwnd);
    ::PostThreadMessage(GetCurrentThreadId(), wm_WindowDestroyed,
      WPARAM(hwnd), 0);
  }

  // Delegate to the previous WindowProc
  return CallWindowProc(pfnPrev, hwnd, uMsg, wp, lp);
}

inline void TCWindowThreadBase::MessageLoop()
{
  // Service the message queue
  MSG msg;
  while (GetMessage(&msg, NULL, 0, 0))
  {
    if (msg.hwnd || !ProcessThreadMessage(msg))
    {
      if (!GetRefsAccel() || !TranslateAccelerators(msg))
      {
        if (!GetRefsDlgXlate() || !TranslateDialogMessages(msg))
        {
          TranslateMessage(&msg);
          DispatchMessage(&msg);
        }
      }
    }
  }
  ZAssert(!m_windows.size());
}

inline bool TCWindowThreadBase::ProcessThreadMessage(MSG& msg)
{
  ZAssert(!msg.hwnd);
  switch (msg.message)
  {
    case wm_CreateWindowOnThread:
    {
      // Get the arguments pointer
      XArgs* pArgs = reinterpret_cast<XArgs*>(msg.wParam);
      ZAssert(pArgs);

      // Attempt to create the window as specified
      InternalCreateWindow(*pArgs);

      // Signal the window created event
      SetEvent(pArgs->hevt);

      // Indicate the we processed the message
      return true;
    }
    case wm_DestroyWindowOnThread:
    {
      // Get the arguments pointer
      XArgs* pArgs = reinterpret_cast<XArgs*>(msg.wParam);
      ZAssert(pArgs);

      // Destroy the specified window
      InternalDestroyWindow(*pArgs);

      // Signal the window destroyed event
      SetEvent(pArgs->hevt);

      // Indicate the we processed the message
      return true;
    }
    case wm_WindowDestroyed:
    {
      // Get the HWND that has been destroyed
      HWND hwnd = reinterpret_cast<HWND>(msg.wParam);
      _ASSERT(hwnd);

      // Remove the window from our collection
      RemoveWindow(hwnd);

      // Indicate the we processed the message
      return true;
    }
  }

  // Indicate that we did not process the message
  return false;
}

inline bool TCWindowThreadBase::TranslateAccelerators(MSG& msg)
{
  ZAssert(GetRefsAccel());

  // First attempt to find the message's destination window
  CLock lock(this);
  XItems::const_iterator it = m_windows.find(msg.hwnd);
  if (it != m_windows.end() && it->second.hAccel
    && ::TranslateAccelerator(it->first, it->second.hAccel, &msg))
      return true;

  // Loop thru the windows with accelerator tables
  for (it = m_windows.begin(); it != m_windows.end(); ++it)
    if (it->second.hAccel
      && ::TranslateAccelerator(it->first, it->second.hAccel, &msg))
        return true;

  // Indicate that we did not process the message
  return false;
}

inline bool TCWindowThreadBase::TranslateDialogMessages(MSG& msg)
{
  ZAssert(GetRefsDlgXlate());

  // First attempt to find the message's destination window
  CLock lock(this);
  XItems::const_iterator it = m_windows.find(msg.hwnd);
  if (it != m_windows.end() && it->second.bXlateDlgMsgs
    && ::IsDialogMessage(it->first, &msg))
      return true;

  // Loop thru the windows for which dialog message are to be translated
  for (it = m_windows.begin(); it != m_windows.end(); ++it)
    if (it->second.bXlateDlgMsgs
      && ::IsDialogMessage(it->first, &msg))
        return true;

  // Indicate that we did not process the message
  return false;
}


/////////////////////////////////////////////////////////////////////////////

#endif // !__WindowThreadBase_h__
