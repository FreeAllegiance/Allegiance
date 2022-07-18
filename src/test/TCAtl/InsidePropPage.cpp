/////////////////////////////////////////////////////////////////////////////
// InsidePropPage.cpp | Implementation of the TCInsidePropPage class.

#include "InsidePropPage.h"


/////////////////////////////////////////////////////////////////////////////
// TCInsidePropPage


/////////////////////////////////////////////////////////////////////////////
// Group=Construction

/////////////////////////////////////////////////////////////////////////////
// {group:Construction}
// Constructs an instance of this class.
TCInsidePropPage::TCInsidePropPage()
{
}

/////////////////////////////////////////////////////////////////////////////
// Description: Creates an embedded property page as a child window.
//
// Call this method to create an embedded property page as a child window of
// the specified parent window. The /idPosCtrl/ parameter is used to specify
// an existing child window that is used to position and, optionally, size
// the embedded property page. This allows, for example, the layout of a
// dialog/property page to be designed in an editor such as Developer Studio,
// with the embedded property page represented by a static frame or rectangle
// control. The ID of the static control can then be specified in the call to
// Create, which will use the control to position the embedded property page.
//
// Parameters:
//   hwndParent - Specifies the parent window of the embedded property page.
//   pPageSite - Specifies the *IPropertyPageSite* interface pointer to be
// set on the embedded property page.
//   idPosCtrl - Specifies an existing child window of /hwndParent/ that is
// used to position, and optionally, size the embedded property page.
//   clsid - Specifies the component object CLSID of the property page to be
// created and embedded within /hwndParent/.
//   bVisible - Specifies if the embedded property page should initially be
// made visible. A value of *true* indicates to make the page immediately
// visible, while *false* indicates that the page will initially be hidden.
//   bSizeToCtrl - When *true*, the embedded property page will be made the
// same size as the control specified in /idPosCtrl/. Otherwise,
// *false* specifies that the page will be the size indicated by its
// *IPropertyPage::GetPageInfo* method.
//   bDelCtrl - When *true*, the child window identified by /idPosCtrl/ will
// be destroyed after it is used to position the embedded property page.
// Otherwise, *false* indicates that the positioning window will remain in
// existence, but will be hidden. When only one property page is to be
// created at the position of the control, this will usually be *true*, since
// the positioning window would no longer be needed. When a /group/ of
// property pages is to be create in the same position, however, this
// parameter should be *false* so that the window can be used repeatedly for
// positioning each page of the group.
//
// Return Value: *true* if the child property page was created, otherwise
// *false*.
//
// See Also: TCInsidePropertyPage::m_pPageSite,
// TCPropertyPageSite::Page_Create, CWindowImpl::Create
bool TCInsidePropPage::Create(HWND hwndParent, IPropertyPageSite* pPageSite,
  UINT idPosCtrl, REFCLSID clsid, bool bVisible, bool bSizeToCtrl,
  bool bDelCtrl)
{
  // Attempt to find the specified control in the specified parent
  HWND hwndPos = ::GetDlgItem(hwndParent, idPosCtrl);

  // Fail if the parent window does not have the specified control
  if (!hwndPos)
  {
    ZAssert(false);
    return false;
  }

  // Create the property page
  HRESULT hr = Page_Create(clsid);
  if (FAILED(hr))
  {
    ZAssert(false);
    return false;
  }

  // Get the rectangle of the positioning window
  RECT rc;
  ::GetWindowRect(hwndPos, &rc);
  ::MapWindowPoints(NULL, hwndParent, LPPOINT(&rc), 2);
  if (!bSizeToCtrl)
  {
    rc.right = rc.left + GetSize().cx;
    rc.bottom = rc.top + GetSize().cy;
  }

  // Determine the styles of the window
  DWORD dwStyle = WS_CHILD | WS_GROUP | DS_CONTROL;
  if (bVisible)
    dwStyle |= WS_VISIBLE;
  DWORD dwExStyle = WS_EX_CONTROLPARENT;

  // Create the window
  HWND hwnd = CWindowImpl<TCInsidePropPage>::Create(hwndParent, rc, NULL,
    dwStyle, dwExStyle, idPosCtrl);
  if (!hwnd)
    return false;

  // Position the page window in front of the positioning window
  ::SetWindowPos(hwnd, hwndPos, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);

  // Show the property page
  _VERIFYE(SUCCEEDED(hr = Page_Show()));
  if (FAILED(hr))
    return false;

  // Delete the positioning window, if specified, otherwise hide it
  if (bDelCtrl)
    ::DestroyWindow(hwndPos);
  else
    ::ShowWindow(hwndPos, SW_HIDE);

  // Save the page site interface pointer
  m_pPageSite = pPageSite;

  // Indicate success
  return true;
}


/////////////////////////////////////////////////////////////////////////////
// Group=Overrides

/////////////////////////////////////////////////////////////////////////////
// Delegates to the corresponding *IPropertyPageSite* interface method of
// m_pPageSite, if not *NULL*. Otherwise, the method of the base class,
// TCPropertyPageSite, is called.
//
// Parameters:
//   dwFlags - See TCPropertyPageSite::OnStatusChange.
//
// Return Value: See TCPropertyPageSite::OnStatusChange.
//
// See Also: TCInsidePropPage::Create, TCInsidePropPage::m_pPageSite,
// TCPropertyPageSite::OnStatusChange
HRESULT TCInsidePropPage::OnStatusChange(DWORD dwFlags)
{
  // Delegate to property page site, if set
  if (NULL != m_pPageSite)
    return m_pPageSite->OnStatusChange(dwFlags);

  // Perform default processing
  return TCPropertyPageSite::OnStatusChange(dwFlags);
}

/////////////////////////////////////////////////////////////////////////////
// Delegates to the corresponding *IPropertyPageSite* interface method of
// m_pPageSite, if not *NULL*. Otherwise, the method of the base class,
// TCPropertyPageSite, is called.
//
// Parameters:
//   pLocaleID - See TCPropertyPageSite::OnGetLocaleID.
//
// Return Value: See TCPropertyPageSite::OnGetLocaleID.
//
// See Also: TCInsidePropPage::Create, TCInsidePropPage::m_pPageSite,
// TCPropertyPageSite::OnGetLocaleID
HRESULT TCInsidePropPage::OnGetLocaleID(LCID* pLocaleID)
{
  // Delegate to property page site, if set
  if (NULL != m_pPageSite)
    return m_pPageSite->GetLocaleID(pLocaleID);

  // Perform default processing
  return TCPropertyPageSite::OnGetLocaleID(pLocaleID);
}

/////////////////////////////////////////////////////////////////////////////
// Delegates to the corresponding *IPropertyPageSite* interface method of
// m_pPageSite, if not *NULL*. Otherwise, the method of the base class,
// TCPropertyPageSite, is called.
//
// Parameters:
//   ppUnk - See TCPropertyPageSite::OnGetPageContainer.
//
// Return Value: See TCPropertyPageSite::OnGetPageContainer.
//
// See Also: TCInsidePropPage::Create, TCInsidePropPage::m_pPageSite,
// TCPropertyPageSite::OnGetPageContainer
HRESULT TCInsidePropPage::OnGetPageContainer(IUnknown** ppUnk)
{
  // Delegate to property page site, if set
  if (NULL != m_pPageSite)
    return m_pPageSite->GetPageContainer(ppUnk);

  // Perform default processing
  return TCPropertyPageSite::OnGetPageContainer(ppUnk);
}

/////////////////////////////////////////////////////////////////////////////
// Delegates to the corresponding *IPropertyPageSite* interface method of
// m_pPageSite, if not *NULL*. Otherwise, the method of the base class,
// TCPropertyPageSite, is called.
//
// Parameters:
//   pMsg - See TCPropertyPageSite::OnTranslateAccelerator.
//
// Return Value: See TCPropertyPageSite::OnTranslateAccelerator.
//
// See Also: TCInsidePropPage::Create, TCInsidePropPage::m_pPageSite,
// TCPropertyPageSite::OnTranslateAccelerator
HRESULT TCInsidePropPage::OnTranslateAccelerator(MSG* pMsg)
{
  // Delegate to property page site, if set
  if (NULL != m_pPageSite)
    return m_pPageSite->TranslateAccelerator(pMsg);

  // Perform default processing
  return TCPropertyPageSite::OnTranslateAccelerator(pMsg);
}

/////////////////////////////////////////////////////////////////////////////
// Return Value: Returns *FALSE*, since an embedded property page is a
// modeless child control.
BOOL TCInsidePropPage::OnIsModal()
{
  return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// Return Value: Returns the m_hWnd member variable of the base class,
// CWindow.
HWND TCInsidePropPage::OnGetWindow()
{
  return m_hWnd;
}


/////////////////////////////////////////////////////////////////////////////
// Group=Message Handlers

/////////////////////////////////////////////////////////////////////////////
// Description: Directs window messages to the TCPropertyPageSite base class.
//
// Thie message handler allows the TCPropertyPageSite base class the first
// chance of handling all window messages.
//
// Parameters: See TCPropertyPageSite::OnPageSiteWndMsg
//
// Return Value: See TCPropertyPageSite::OnPageSiteWndMsg
//
// See Also: TCPropertyPageSite::OnPageSiteWndMsg
LRESULT TCInsidePropPage::OnMessage(UINT uMsg, WPARAM wParam, LPARAM lParam,
  BOOL& bHandled)
{
  LRESULT lResult = 0;
  bHandled = OnPageSiteWndMsg(uMsg, wParam, lParam, &lResult);
  return lResult;
}


