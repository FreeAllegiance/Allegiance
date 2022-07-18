#ifndef __AppWizStepDlg_h__
#define __AppWizStepDlg_h__

/////////////////////////////////////////////////////////////////////////////
// AppWizStepDlg.h : Declaration of the TCAppWizStepDlg class.
//

#include <WindowFinder.h>


/////////////////////////////////////////////////////////////////////////////
// TCAppWizStepDlg dialog
class TCAppWizStepDlg : public CAppWizStepDlg
{
// Construction
public:
  TCAppWizStepDlg(UINT nIDTemplate, HINSTANCE hinstResource);

// Operations
public:
  void EnableFinish(bool bEnable = true);

// Overrides
protected:
  virtual bool OnHelpClicked();

// Message Handlers
protected:
  virtual BOOL OnInitDialog();
  virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam,
    LRESULT* pResult);

// Implementation
protected:
  static HWND& GetEtchedLine();
  static HWND& GetFinishButton();
  static HWND& GetHelpButton();
  static HWND& GetSubclassedParent();
  static WNDPROC& GetSubclassedParentProc();
  static TCAppWizStepDlg*& GetCurrentStep();
  static bool IsStaticControl(HWND hwnd);
  static BOOL CALLBACK FrameFindProc(HWND hwnd, LPARAM lp);
  static BOOL CALLBACK PictureFindProc(HWND hwnd, LPARAM lp);
  static LRESULT CALLBACK ParentWndProc(HWND hwnd, UINT uMsg, WPARAM wParam,
    LPARAM lParam);

// Enumerations
protected:
  enum {wm_EnableFinish = WM_APP + 0x0200};

// Data Members
protected:
  HINSTANCE m_hinstResource;
  bool      m_bEnableFinish;
};


/////////////////////////////////////////////////////////////////////////////
// Group=Construction

/////////////////////////////////////////////////////////////////////////////
// {group:Construction}
//
inline TCAppWizStepDlg::TCAppWizStepDlg(UINT nIDTemplate,
  HINSTANCE hinstResource) :
  CAppWizStepDlg(nIDTemplate),
  m_hinstResource(hinstResource),
  m_bEnableFinish(true)
{
}


/////////////////////////////////////////////////////////////////////////////
// Group=Operations

/////////////////////////////////////////////////////////////////////////////
// Description: Enables or disables the Finish button on the parent dialog.
//
// Enables or disables the Finish button on the parent dialog.
inline void TCAppWizStepDlg::EnableFinish(bool bEnable)
{
  PostMessage(wm_EnableFinish, m_bEnableFinish = bEnable);
}


/////////////////////////////////////////////////////////////////////////////
// Group=Overrides

/////////////////////////////////////////////////////////////////////////////
// Description: Called when the Help button of the parent window is clicked.
//
inline bool TCAppWizStepDlg::OnHelpClicked()
{
  // The default does nothing
  return false;
}


/////////////////////////////////////////////////////////////////////////////
// Group=Message Handlers

/////////////////////////////////////////////////////////////////////////////
// Description: Fixes a few things in the user interface of the dialog box.
// Fixes a few things in the user interface of the dialog box.
inline BOOL TCAppWizStepDlg::OnInitDialog()
{
  // Perform default processing
  CAppWizStepDlg::OnInitDialog();

  // Get the parent window
  HWND hwndParent = ::GetParent(*this);

  // Change the black line in the parent to etched, if not already
  if (!GetEtchedLine())
    ::EnumChildWindows(hwndParent, FrameFindProc, LPARAM(m_hWnd));

  // Load the images on the dialog, if any
  ::EnumChildWindows(*this, PictureFindProc, LPARAM(m_hinstResource));

  // Subclass the parent window, if not already
  if (!GetSubclassedParent())
  {
    GetSubclassedParentProc() = (WNDPROC)::SetWindowLong(hwndParent,
      GWL_WNDPROC, (LONG)ParentWndProc);
    GetSubclassedParent() = hwndParent;   

    // Find the "Finish" button
    TCWindowFinder wfFinish("&Finish");
    ::EnumChildWindows(hwndParent, wfFinish, wfFinish);
    GetFinishButton() = wfFinish;

    // Find the "Help" button
    TCWindowFinder wfHelp("&Help");
    ::EnumChildWindows(hwndParent, wfHelp, wfHelp);
    GetHelpButton() = wfHelp;
  }

  // Return TRUE to set the focus to the first tabstop control
  return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// Description: Fixes painting so that picture control rectangles get
// clipped.
//
inline BOOL TCAppWizStepDlg::OnWndMsg(UINT message, WPARAM wParam,
  LPARAM lParam, LRESULT* pResult)
{
  // Check for interesting messages
  switch (message)
  {
    case WM_SHOWWINDOW:
    {
      if (wParam)
      {
        GetCurrentStep() = this;
        PostMessage(wm_EnableFinish, m_bEnableFinish);
      }
      else
        GetCurrentStep() = NULL;
      break;
    }
    case wm_EnableFinish:
    {
      if (GetFinishButton())
        ::EnableWindow(GetFinishButton(), wParam);
      *pResult = 0;
      return TRUE;
    }
  }

  // Perform default processing
  return CAppWizStepDlg::OnWndMsg(message, wParam, lParam, pResult);
}


/////////////////////////////////////////////////////////////////////////////
// Group=Implementation


/////////////////////////////////////////////////////////////////////////////
// Description: Gets the static HWND of the parent's etched line.
//
// Gets the static HWND of the etched line of the parent.
//
// This function exists so that the static variable can be initialized while
// keeping the entire class implementation in a header file.
inline HWND& TCAppWizStepDlg::GetEtchedLine()
{
  static HWND hwnd = NULL;
  if (hwnd && !::IsWindow(hwnd))
    hwnd = NULL;
  return hwnd;
}

/////////////////////////////////////////////////////////////////////////////
// Description: Gets the static HWND of the parent's Finish button.
//
// Gets the static HWND of the parent's Finish button.
//
// This function exists so that the static variable can be initialized while
// keeping the entire class implementation in a header file.
inline HWND& TCAppWizStepDlg::GetFinishButton()
{
  static HWND hwnd = NULL;
  if (hwnd && !::IsWindow(hwnd))
    hwnd = NULL;
  return hwnd;
}

/////////////////////////////////////////////////////////////////////////////
// Description: Gets the static HWND of the parent's Help button.
//
// Gets the static HWND of the parent's Help button.
//
// This function exists so that the static variable can be initialized while
// keeping the entire class implementation in a header file.
inline HWND& TCAppWizStepDlg::GetHelpButton()
{
  static HWND hwnd = NULL;
  if (hwnd && !::IsWindow(hwnd))
    hwnd = NULL;
  return hwnd;
}

/////////////////////////////////////////////////////////////////////////////
// Description: Gets the static HWND of the subclassed parent window.
//
// Gets the static HWND of the subclassed parent window.
//
// This function exists so that the static variable can be initialized while
// keeping the entire class implementation in a header file.
inline HWND& TCAppWizStepDlg::GetSubclassedParent()
{
  static HWND hwnd = NULL;
  if (hwnd && !::IsWindow(hwnd))
    hwnd = NULL;
  return hwnd;
}

/////////////////////////////////////////////////////////////////////////////
// Description: Gets the static WNDPROC of the subclassed parent window.
//
// Gets the static WNDPROC of the subclassed parent window.
//
// This function exists so that the static variable can be initialized while
// keeping the entire class implementation in a header file.
inline WNDPROC& TCAppWizStepDlg::GetSubclassedParentProc()
{
  static WNDPROC pfn = NULL;
  return pfn;
}

/////////////////////////////////////////////////////////////////////////////
// Description: Gets the static instance pointer of the current step.
//
// Gets the static instance pointer of the current step.
//
// This function exists so that the static variable can be initialized while
// keeping the entire class implementation in a header file.
inline TCAppWizStepDlg*& TCAppWizStepDlg::GetCurrentStep()
{
  static TCAppWizStepDlg* pDlg = NULL;
  return pDlg;
}


/////////////////////////////////////////////////////////////////////////////
// Description: Compares the class name of the specified window to "static".
//
inline bool TCAppWizStepDlg::IsStaticControl(HWND hwnd)
{
  // Get the class name of the window
  TCHAR sz[_MAX_PATH];
  ::GetClassName(hwnd, sz, sizeof(sz) / sizeof(sz[0]));

  // Is it a static?
  return 0 == _tcsicmp(sz, TEXT("static"));
}

/////////////////////////////////////////////////////////////////////////////
// Description: Changes the ugly black line on the Wizard sheet to etched.
//
// Changes the ugly black line on the Wizard sheet to etched.
inline BOOL CALLBACK TCAppWizStepDlg::FrameFindProc(HWND hwnd, LPARAM lp)
{
  // Do nothing if the child id of the window is not -1
  if (-1 != ::GetDlgCtrlID(hwnd))
    return TRUE;

  // Do nothing if the window is not static
  if (!IsStaticControl(hwnd))
    return TRUE;

  // Do nothing if the window is not a black frame
  DWORD dwStyle = ::GetWindowLong(hwnd, GWL_STYLE);
  if (SS_BLACKFRAME != (dwStyle & SS_TYPEMASK))
    return TRUE;

  // Get the frame window's parent
  HWND hwndParent = ::GetParent(hwnd);

  // Get the frame window's rectangle
  RECT rc;
  ::GetWindowRect(hwnd, &rc);
  ::MapWindowPoints(NULL, hwndParent, LPPOINT(&rc), 2);

  // Create a new frame
  dwStyle = (dwStyle & ~SS_TYPEMASK) | SS_ETCHEDHORZ;
  HWND hwndNew = ::CreateWindow(TEXT("static"), TEXT(""), dwStyle,
    rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, hwndParent,
    HMENU(-1), HINSTANCE(::GetWindowLong(HWND(lp), GWL_HINSTANCE)), NULL);
  ZAssert(hwndNew);

  // Position the new frame after the old one
  UINT uFlags = SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE;
  ::SetWindowPos(hwndNew, hwnd, 0, 0, 0, 0, uFlags);

  // Destroy the old frame
  ::DestroyWindow(hwnd);

  // Save the HWND of the etched line
  GetEtchedLine() = hwndNew;

  // Stop enumerating the child windows
  return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// Description: Loads the image of each picture control.
//
// Loads the image of each picture control.
inline BOOL CALLBACK TCAppWizStepDlg::PictureFindProc(HWND hwnd, LPARAM lp)
{
  // Do nothing if the window is not static
  if (!IsStaticControl(hwnd))
    return TRUE;

  // Do nothing if the window is not a bitmap control
  DWORD dwStyle = ::GetWindowLong(hwnd, GWL_STYLE);
  if (SS_BITMAP != (dwStyle & SS_TYPEMASK))
    return TRUE;

  // Get the text of the picture control
  TCHAR sz[_MAX_PATH];
  ::GetWindowText(hwnd, sz, sizeof sz / sizeof sz[0]);
  LPCTSTR psz = (-1 == sz[0]) ?
    MAKEINTRESOURCE(*((WORD*)(sz + 1))) : sz;

  // Load the image for the picture control
  HINSTANCE hinst = HINSTANCE(lp);
  LPARAM hbmp = (LPARAM)LoadImage(hinst, psz, IMAGE_BITMAP, 0, 0, LR_SHARED);

  // Set the image of the picture control
  ::SendMessage(hwnd, STM_SETIMAGE, IMAGE_BITMAP, hbmp);

  // Continue enumerating the child windows
  return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// Description: Window procedure used to dynamically subclass the parent
// window.
//
// Window procedure used to dynamically subclass the parent window.
inline LRESULT CALLBACK TCAppWizStepDlg::ParentWndProc(HWND hwnd, UINT uMsg,
  WPARAM wParam, LPARAM lParam)
{
  WNDPROC pfnWndProc = GetSubclassedParentProc();
  switch (uMsg)
  {
    case WM_COMMAND:
    {
      if (BN_CLICKED == HIWORD(wParam) && lParam == (LPARAM)GetHelpButton())
      {
        TCAppWizStepDlg* pdlgStep = GetCurrentStep();
        if (pdlgStep && pdlgStep->OnHelpClicked())
          return 0;
      }
      break;
    }
    case WM_SHOWWINDOW:
    {
      if (!wParam && hwnd == GetSubclassedParent())
      {
        ::SetWindowLong(hwnd, GWL_WNDPROC, (LONG)pfnWndProc);
        GetSubclassedParent() = NULL;
      }
      break;
    }
  }

  // Perform default processing
  return CallWindowProc(pfnWndProc, hwnd, uMsg, wParam, lParam);
}


/////////////////////////////////////////////////////////////////////////////

#endif // !__AppWizStepDlg_h__
