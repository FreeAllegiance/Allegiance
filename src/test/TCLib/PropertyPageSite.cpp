/////////////////////////////////////////////////////////////////////////////
// PropertyPageSite.cpp | Implementation of the TCPropertyPageSite class.
//

#include "pch.h"
#include "..\Inc\TCLib.h"
#include "PropertyPageSite.h"
#include "AutoHandle.h"


/////////////////////////////////////////////////////////////////////////////
// TCPropertyPageSite


/////////////////////////////////////////////////////////////////////////////
// Group=Construction / Destruction

/////////////////////////////////////////////////////////////////////////////
// Parameters:
//   clsidPage - The CLSID of a property page to be created and managed by
// this object. Calls Page_Create to create the object.
//   pPage - An IPropertyPage* of an existing property page component object
// to be managed by this object. Calls Page_Attach to attach to the page.
//
// See Also: TCPropertyPageSite::Page_Create, TCPropertyPageSite::Page_Attach
TCPropertyPageSite::TCPropertyPageSite()
{
  CommonConstruct();
}

TCPropertyPageSite::TCPropertyPageSite(REFCLSID clsidPage)
{
  CommonConstruct();
  HRESULT hr = Page_Create(clsidPage);
  ZSucceeded(hr);
}

TCPropertyPageSite::TCPropertyPageSite(IPropertyPage* pPage)
{
  CommonConstruct();
  HRESULT hr = Page_Attach(pPage);
  ZSucceeded(hr);
}

TCPropertyPageSite::~TCPropertyPageSite()
{
  CommonDestruct();
}

/////////////////////////////////////////////////////////////////////////////
// Description: Creates and attaches to a property page component object.
//
// Attempts to create an instance of the property page component object
// specified by the /clsidPage/ CLSID. If successful, this method delegates
// to the Page_Attach method and saves the page's CLSID.
//
// Parameters:
//   clsidPage - The CLSID of the property page to be created and managed by
// this object.
//
// Return Value: S_OK if successful, otherwise a standard COM error code.
//
// See Also: TCPropertyPageSite::Page_Attach, TCPropertyPageSite::GetClassID
HRESULT TCPropertyPageSite::Page_Create(REFCLSID clsidPage)
{
  // Attempt to create the specified property page
  IPropertyPagePtr pPage;
  HRESULT hr = pPage.CreateInstance(clsidPage);
  if (FAILED(hr))
    return hr;

  // Delegate to the Attach method
  if (FAILED(hr = Page_Attach(pPage)))
    return hr;

  // Save the page's CLSID
  m_clsid = clsidPage;

  // Indicate success
  return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
// Description: Attaches an existing IPropertyPage interface.
//
// Manages the specified IPropertyPage interface and sets m_xPageSite as the
// page's site by calling IPropertyPage::SetPageSite. It then retrieves and
// stores the page's general informatioin by calling the
// IPropertyPage::GetPageInfo method and informs the page, by calling the 
// IPropertyPage::SetObjects method, of the array of objects which it will
// be editing. If the site window is visible, the Page_Show method is called
// to activate and display the property page.
//
// This method is used internally (by Page_Create) and does not need to be
// called directly. It is public, however, to allow a more advanced usage.
//
// When this method is called directly (not from Page_Create), the CLSID of
// the property page cannot be determined, so GetClassID will return
// CLSID_NULL.
//
// Parameters:
//   pPage - An IPropertyPage* of an existing property page component object
// to be managed by this object.
//
// Return Value: S_OK if successful, otherwise the result of the failed
// interface method.
//
// See Also: TCPropertyPageSite::constructor,
// TCPropertyPageSite::Page_Create, TCPropertyPageSite::Page_Show,
// TCPropertyPageSite::OnGetWindow, TCPropertyPageSite::GetTitle,
// TCPropertyPageSite::GetSize, TCPropertyPageSite::GetDocString,
// TCPropertyPageSite::GetHelpFile, TCPropertyPageSite::GetHelpContext,
// TCPropertyPageSite::SetObjects
HRESULT TCPropertyPageSite::Page_Attach(IPropertyPage* pPage)
{
  // Ensure that no previous page is attached
  ZAssert(NULL == m_pPage);

  // Set ourself as the page site for the page
  HRESULT hr = pPage->SetPageSite(&m_xPageSite);
  if (FAILED(hr))
    return hr;

  // Get the page information
  hr = pPage->GetPageInfo(&m_info);
  if (FAILED(hr))
    return hr;

  // Set the objects of the page
  if (m_vecObjects.size())
  {
    std::vector<IUnknown*> vecUnk;
    vecUnk.resize(m_vecObjects.size());
    std::copy(m_vecObjects.begin(), m_vecObjects.end(), vecUnk.begin());
// VS.Net 2003 port: see "Breaking Changes in the Standard C++ Library Since Visual C++ 6.0" in documentation
#if _MSC_VER >= 1310
    hr = pPage->SetObjects(vecUnk.size(), &(*(vecUnk.begin())));
#else
    hr = pPage->SetObjects(vecUnk.size(), vecUnk.begin());
#endif
    if (FAILED(hr))
      return hr;
  }

  // Save the page pointer
  m_pPage = pPage;

  // Show the page if the page site window is visible
  HWND hwnd = OnGetWindow();
  if (NULL != hwnd && ::IsWindowVisible(hwnd))
    hr = Page_Show();

  // Indicate success
  return hr;
}

/////////////////////////////////////////////////////////////////////////////
// Description: Detaches the managed property page.
//
// Detaches the managed property page by releasing the strings in m_info,
// deactivating the property page, if active, and detaching the IPropertyPage
// interface pointer.
//
// TODO: Should this also set the managed property page's site to NULL?
//
// Return Value: The IPropertyPage interface pointer that was detached from
// this site object.
//
// See Also: TCPropertyPageSite::constructor, TCPropertyPageSite::destructor,
// TCPropertyPageSite::Page_Create, TCPropertyPageSite::Page_Attach,
// TCPropertyPageSite::m_info
IPropertyPage* TCPropertyPageSite::Page_Detach()
{
  __try
  {
    // Release the PROPPAGEINFO structure
    CoTaskMemFree(m_info.pszTitle);
    CoTaskMemFree(m_info.pszDocString);
    CoTaskMemFree(m_info.pszHelpFile);
    ZeroMemory(&m_info, sizeof(m_info));
    m_info.cb = sizeof(m_info);

    // Deactivate the page, if it is activated
    if (m_bActivated && NULL != m_pPage)
      m_pPage->Deactivate();
    m_bActivated = false;

    // Clear the CLSID of the page
    m_clsid = CLSID_NULL;

    // Detach the pointer
    return m_pPage.Detach();
  }
  __except(1)
  {
    _TRACE0("TCPropertyPageSite::Detach(): Exception Caught!\n");
    return NULL;
  }
}

/////////////////////////////////////////////////////////////////////////////
// Description: Common construction processing.
//
// Performs common construction processing.
void TCPropertyPageSite::CommonConstruct()
{
  m_xPageSite.m_pThis = this;
  m_clsid = CLSID_NULL;
  ZeroMemory(&m_info, sizeof(m_info));
  m_info.cb = sizeof(m_info);
  m_bActivated = false;
  m_bDirty = false;
}

/////////////////////////////////////////////////////////////////////////////
// Description: Common destruction processing.
//
// Destructs the object by calling Page_Detach and releasing the detached
// pointer.
//
// See Also: TCPropertyPageSite::destructor, TCPropertyPageSite::Page_Detach
void TCPropertyPageSite::CommonDestruct()
{
  __try
  {
    IPropertyPage* pPage = Page_Detach();
    if (NULL != pPage)
      pPage->Release();
  }
  __except(1)
  {
    _TRACE0("TCPropertyPageSite::CommonDestruct(): Exception Caught!\n");
  }
}


/////////////////////////////////////////////////////////////////////////////
// Group=Attributes

/////////////////////////////////////////////////////////////////////////////
// Description: Provides the IUnknown pointers of the objects affected by
// the managed property page.
//
// Provides the IUnknown pointers of the objects affected by the managed
// property page. The pointers are copied to the m_vecObjects data member. If
// a property page is attached, the IPropertyPage::SetObjects method of the
// page is called.
//
// Parameters:
//   cObjects - [in] Number of *IUnknown* pointers in the array pointed to by
//   /ppUnk/.
//   ppUnk - [in] Pointer to an array of *IUnknown* interface pointers where
//   each pointer identifies a unique object affected by the managed property
//   page.
//
// Return Value: S_OK if successful, otherwise the return code from the
// property page's interface method.
HRESULT TCPropertyPageSite::SetObjects(ULONG cObjects, IUnknown** ppUnk)
{
  // Release any previous vector of objects
  m_vecObjects.clear();

  // Copy the specified array to the vector of objects
  m_vecObjects.resize(cObjects);
  std::copy(ppUnk, ppUnk + cObjects, m_vecObjects.begin());

  // Set the objects of the page, if the page exists
  if (NULL != m_pPage)
    return m_pPage->SetObjects(cObjects, ppUnk);

  // Indicate success
  return S_OK;
}


/////////////////////////////////////////////////////////////////////////////
// Group=Operations

/////////////////////////////////////////////////////////////////////////////
// Description: Message processing method to be called by the derived class.
//
// Handles the WM_CREATE, WM_SIZE, and WM_DESTROY window messages to affect
// the managed property page appropriately.
//
// A derived class must call this somewhere in it's message handling logic.
// For an MFC CWnd-derived class, the OnWndMsg virtual function is and ideal
// place for this:
//
//      BOOL TCActiveXPropertyPage::OnWndMsg(UINT message, WPARAM wParam,
//        LPARAM lParam, LRESULT* plResult)
//      {
//        // Allow the TCPropertyPageSite class a crack at the message
//        if (OnPageSiteWndMsg(message, wParam, lParam, plResult))
//          return TRUE;
//      …
//        // Perform default processing
//        return CPropertyPage::OnWndMsg(message, wParam, lParam, plResult);
//      }
//
// Return Value: TRUE if message processing should cease; otherwise FALSE.
//
// Parameters:
//   message - [in] Specifies the message being processed.
//   wParam - [in] Specifies additional message-dependent information.
//   lParam - [in] Specifies additional message-dependent information.
//   plResult - [out] The return value of the message handling.
BOOL TCPropertyPageSite::OnPageSiteWndMsg(UINT message, WPARAM wParam,
  LPARAM lParam, LRESULT* plResult)
{
  // Handle certain messages
  switch (message)
  {
    case WM_CREATE:
    {
      // Ensure that the page is activated
      if (!m_bActivated && FAILED(Activate()))
      {
        *plResult = LRESULT(-1);
        return TRUE;
      }
      break;
    }
    case WM_SIZE:
    {
      // Size the property page to match
      if (NULL != m_pPage)
      {
        RECT rect = {0, 0, LOWORD(lParam), HIWORD(lParam)};
        m_pPage->Move(&rect);
      }
      break;
    }
    case WM_DESTROY:
    {
      // Deactivate the page, if activated
      if (m_bActivated)
      {
        m_pPage->Deactivate();
        m_bActivated = false;
      }
      break;
    }
  }

  // Allow processing to continue
  return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// Description: Displays the managed property page.
//
// Displays the managed property page by first activating the page, if not
// already active, and then calling the IPropertyPage::Show method with the
// SW_SHOW parameter.
//
// Return Value: S_OK if successful, otherwise the return code from the
// property page's interface method.
//
// See Also: TCPropertyPageSite::Page_Hide, TCPropertyPageSite::Activate
HRESULT TCPropertyPageSite::Page_Show()
{
  // Activate the page, if not already active
  HRESULT hr;
  if (!m_bActivated && FAILED(hr = Activate()))
    return hr;

  // Show the property page and this window
  hr = m_pPage->Show(SW_SHOW);
  return hr;
}

/////////////////////////////////////////////////////////////////////////////
// Description: Hides the managed property page.
//
// Hides the managed property page by calling the IPropertyPage::Show method
// with the SW_HIDE parameter.
//
// Return Value: S_OK if successful, otherwise the return code from the
// property page's interface method.
//
// See Also: TCPropertyPageSite::Page_Show
HRESULT TCPropertyPageSite::Page_Hide()
{
  // Hide the property page
  return m_pPage->Show(SW_HIDE);
}

/////////////////////////////////////////////////////////////////////////////
// Description: Displays help for the managed property page.
//
// Displays help for the managed property page by first deriving the path
// name for the page's help file. The "HelpDir" and "InProcServer32" registry
// subkeys are used, in that order, to determine where in the file system the
// page's help file might be located. This path is then specified in a call
// to the IPropertyPage::Help method, which allows the page to display it's
// help in some alternate way. If that method call fails, the GetHelpFile and
// GetHelpContext methods are used in a call to the Win32 WinHelp API.
// 
// Return Value: S_OK if successful, otherwise the return code from the
// property page's failed interface method or from a failed registry API.
//
// See Also: TCPropertyPageSite::GetHelpFile,
// TCPropertyPageSite::GetHelpContext, TCRegKeyHandle
HRESULT TCPropertyPageSite::Page_Help()
{
  // Create the registry key name
  OLECHAR szCLSID[_MAX_PATH] = OLESTR("CLSID\\");
  int cchCLSIDKey = wcslen(szCLSID);

  // Convert the stored CLSID to a string representation
  StringFromGUID2(GetClassID(), szCLSID + cchCLSIDKey,
    sizeofArray(szCLSID) - cchCLSIDKey);

  // Open the registry key
  DWORD dw;
  USES_CONVERSION;
  TCRegKeyHandle hkey;
  LRESULT lr = RegCreateKeyEx(HKEY_CLASSES_ROOT, OLE2T(szCLSID), 0, REG_NONE,
    REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hkey, &dw);
  if (ERROR_SUCCESS != lr)
    return lr;

  // Open the registry subkey
  TCHAR szPath[_MAX_PATH];
  TCRegKeyHandle hkeySub;
  lr = RegCreateKeyEx(hkey, TEXT("HelpDir"), 0, REG_NONE,
    REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hkeySub, &dw);
  if (ERROR_SUCCESS == lr)
  {
    // Read the default subkey value
    DWORD dwType = REG_SZ, cbData = sizeof(szPath);
    lr = RegQueryValueEx(hkeySub, NULL, NULL, &dwType, (PBYTE)szPath,
      &cbData);
  }

  // Try another registry subkey if "HelpDir" failed
  if (ERROR_SUCCESS != lr)
  {
    lr = RegCreateKeyEx(hkey, TEXT("InProcServer32"), 0, REG_NONE,
      REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hkeySub, &dw);
    if (ERROR_SUCCESS == lr)
    {
      // Read the default subkey value
      DWORD dwType = REG_SZ, cbData = sizeof(szPath);
      if (ERROR_SUCCESS == (lr = RegQueryValueEx(hkeySub, NULL, NULL,
        &dwType, (PBYTE)szPath, &cbData)))
      {
        // Trim down to just the pathname
        TCHAR szDrive[_MAX_DRIVE], szDir[_MAX_DIR];
        _tsplitpath(szPath, szDrive, szDir, NULL, NULL);
        _tmakepath(szPath, szDrive, szDir, NULL, NULL);
      }
    }
  }

  // Return if "InProcServer32" failed
  if (ERROR_SUCCESS != lr)
    return lr;

  // Allow the property page to display it's help
  HRESULT hr = m_pPage->Help(T2COLE(szPath));
  if (SUCCEEDED(hr))
    return hr;

  // Property page failed on call to Help, so use PROPPAGEINFO
  TCHAR szDrive[_MAX_DRIVE], szDir[_MAX_DIR];
  _tsplitpath(szPath, szDrive, szDir, NULL, NULL);
  _tmakepath(szPath, szDrive, szDir, STRTYPE2CT(GetHelpFile()), NULL);
  if (!WinHelp(OnGetWindow(), szPath, HELP_CONTEXTPOPUP, GetHelpContext()))
    return HRESULT_FROM_WIN32(::GetLastError());

  // Indicate success
  return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
// Description: Processes keystroke messages.
//
// Simply delegates to the IPropertyPage::TranslateAccelerator method.
//
// Parameters:
//   pMsg - [in] Pointer to the MSG structure describing the keystroke to
// process. 
//
// Return Value: S_OK if successful, otherwise the return code from the
// property page's interface method.
HRESULT TCPropertyPageSite::Page_TranslateAccelerator(MSG* pMsg)
{
  ZAssert(NULL != m_pPage);
  return m_pPage->TranslateAccelerator(pMsg);
}


/////////////////////////////////////////////////////////////////////////////
// Description: Queries the property page for the specified interface.
//
// Delegates to the QueryInterface method of the property page. However, the
// interfaces IPropertyPage and IPropertyPage2 are explicitly never supported
// through this method. This is to help ensure that the dirty flag of the
// property page is always modified through the TCPropertyPageSite class, and
// not directly through the IPropertyPage or IPropertyPage2 interfaces. This
// could obviously be abused if the property page supported some other
// interface that directly modified its dirty flags (e.g. IPropertyPage3).
//
// Parameters:
//   riid - Identifier of the interface being requested.
//   ppvObject - Indirectly points to the interface specified in /iid/. If
// the object does not support the interface specified in iid, *ppvObject
// is set to NULL.
//
// Return Value: S_OK if the interface is supported, E_NOINTERFACE if not.
HRESULT TCPropertyPageSite::Page_QI(REFIID iid, void** ppvObject)
{
  // Check for forbidden interface
  if (IID_IPropertyPage == iid || IID_IPropertyPage2 == iid)
    return E_NOINTERFACE;

  // Otherwise, delegate to the property page
  ZAssert(NULL != m_pPage);
  return m_pPage->QueryInterface(iid, ppvObject);
}


/////////////////////////////////////////////////////////////////////////////
// Group=Overrides

/////////////////////////////////////////////////////////////////////////////
// Description: Override to perform additional processing of the
// IPropertyPageSite::OnStatusChange interface method.
//
// A derived class can override this to perform additional processing when
// the IPropertyPageSite::OnStatusChange interface method is called by the
// managed property page.
//
// Parameters:
//   dwFlags - [in] Flags to indicate what changes have occurred.
//
// Return Value: S_OK to indicate that the status change was noted.
//
// See Also: TCPropertyPageSite::XPageSite::OnStatusChange
HRESULT TCPropertyPageSite::OnStatusChange(DWORD dwFlags)
{
  UNUSED(dwFlags);
  return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
// Description: Override to perform additional processing of the
// IPropertyPageSite::GetLocaleID interface method.
//
// A derived class can override this to provide a locale ID to the managed
// property page. The default implementation returns the current thread's
// locale.
//
// Parameters:
//   pLocaleID - [out] Pointer to locale identifier.
//
// Return Value: S_OK to indicate that the locale was returned successfully.
//
// See Also: TCPropertyPageSite::XPageSite::GetLocaleID
HRESULT TCPropertyPageSite::OnGetLocaleID(LCID* pLocaleID)
{
  *pLocaleID = GetThreadLocale();
  return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
// Description: Override to perform additional processing of the
// IPropertyPageSite::GetPageContainer interface method.
//
// A derived class can override this to provide an interface pointer of the
// interface pointer to the managed property page. The default
// implementation simply returns E_NOTIMPL.
//
// Parameters:
//   ppUnk - [out] Address of IUnknown* pointer variable that receives the
// interface pointer to the container object.
//
// Return Value: E_NOTIMPL is the only return value allowed at this time. See
// TCPropertyPageSite::XPageSite::GetPageContainer for possible ideas of how
// this could be used.
//
// See Also: TCPropertyPageSite::XPageSite::GetPageContainer
HRESULT TCPropertyPageSite::OnGetPageContainer(IUnknown** ppUnk)
{
  UNUSED(ppUnk);
  return E_NOTIMPL;
}

/////////////////////////////////////////////////////////////////////////////
// Description: Override to perform additional processing of the
// IPropertyPageSite::TranslateAccelerator interface method.
//
// A derived class can override this to process a keystroke message, if it
// desires. The default implementation simply returns E_NOTIMPL.
//
// Parameters:
//   pMsg - [in] Pointer to the MSG structure describing the keystroke to
//   process. 
//
// Return Value:
//   S_OK - The page site processed the message. 
//   S_FALSE - The page site did not process the message. 
//   E_NOTIMPL - The page site does not support keyboard processing. 
//
// See Also: TCPropertyPageSite::XPageSite::TranslateAccelerator
HRESULT TCPropertyPageSite::OnTranslateAccelerator(MSG* pMsg)
{
  UNUSED(pMsg);
  return E_NOTIMPL;
}

/////////////////////////////////////////////////////////////////////////////
// Description: Override to specify if the page site's frame window is modal
// or modeless.
//
// A derived class can override this to specify that it's frame window is
// modeless. The default returns TRUE, indicating that the site's frame
// window is modal. This is used by the Activate method to supply a parameter
// to the IPropertyPage::Activate interface method.
//
// Return Value: TRUE to specify that the site's frame window is modal; FALSE
// to specify that it's modeless.
//
// See Also: TCPropertyPageSite::Activate
BOOL TCPropertyPageSite::OnIsModal()
{
  return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// Description: Override to specify the page site's window.
//
// A derived class should override this to specify it's site window. This
// override is provided so that the class is independent of any particular
// windowing framework, aside from Win32 itself.
//
// Return Value: The page site's window. This window will be used as the
// parent window of the managed property page's window.
//
// TODO: Shouldn't this method be *pure* virtual to enforce a derived class
// to override it?
//
// See Also: TCPropertyPageSite::Activate
HWND TCPropertyPageSite::OnGetWindow()
{
  return NULL;
}


/////////////////////////////////////////////////////////////////////////////
// Group=Implementation

/////////////////////////////////////////////////////////////////////////////
// Description: Activates the managed property page.
//
// Activates the managed property page, if not already active. The
// IPropertyPage::Activate method of the managed property page is used to
// create the property page window in the entire client area of the site's
// window.
//
// Note: Upon activating the property page, the window styles of the page
// site and property page windows are modified to enable Win32's built-in
// child dialog box keyboard handling. After many futile attempt, this proved
// to be much easier and more compatible than attempting to use the
// TranslateAccelerators interface methods.
//
// Return Value: S_OK if successful, otherwise the return code from the
// property page's interface method.
//
// See Also: TCPropertyPageSite::OnGetWindow, TCPropertyPageSite::OnIsModal
HRESULT TCPropertyPageSite::Activate()
{
  __try
  {
    // Do nothing if page is already activated
    if (m_bActivated)
    {
      _TRACE0("TCPropertyPageSite::Activate(): Already activated.\n");
      return S_OK;
    }

    // Get the page site window
    HWND hwnd = OnGetWindow();
    ZAssert(NULL != hwnd);

    // Compute the page's rectangle
    RECT rect;
    ::GetClientRect(hwnd, &rect);

    // Activate the property page
    HRESULT hr = m_pPage->Activate(hwnd, &rect, OnIsModal());
    if (m_bActivated = SUCCEEDED(hr))
    {
      // Modify the style and extended style of this window
      UINT nFlags = SWP_NOZORDER | SWP_NOSIZE | SWP_NOMOVE |
        SWP_FRAMECHANGED | SWP_NOACTIVATE;
      DWORD dwStyle = GetWindowLong(hwnd, GWL_STYLE);
      DWORD dwStyleEx = GetWindowLong(hwnd, GWL_EXSTYLE);
      if (WC_DIALOG == MAKEINTRESOURCE(GetClassLong(hwnd, GCW_ATOM)))
      {
        SetWindowLong(hwnd, GWL_STYLE, dwStyle | DS_CONTROL);
        SetWindowLong(hwnd, GWL_EXSTYLE, dwStyleEx | WS_EX_CONTROLPARENT);
        SetWindowPos(hwnd, NULL, 0, 0, 0, 0, nFlags);
      }

      // Modify the style and extended style of all child dialog windows
      hwnd = ::GetWindow(hwnd, GW_CHILD);
      while (NULL != hwnd)
      {
        if (WC_DIALOG == MAKEINTRESOURCE(GetClassLong(hwnd, GCW_ATOM)))
        {
          dwStyle = GetWindowLong(hwnd, GWL_STYLE);
          SetWindowLong(hwnd, GWL_STYLE, dwStyle | DS_CONTROL);
          dwStyleEx = GetWindowLong(hwnd, GWL_EXSTYLE);
          SetWindowLong(hwnd, GWL_EXSTYLE, dwStyleEx | WS_EX_CONTROLPARENT);
          SetWindowPos(hwnd, NULL, 0, 0, 0, 0, nFlags);
        }
        hwnd = ::GetWindow(hwnd, GW_HWNDNEXT);
      }
    }
    return hr;
  }
  __except(1)
  {
    _TRACE0("TCPropertyPageSite::Activate(): Unknown exception.\n");
    return E_UNEXPECTED;
  }
}


/////////////////////////////////////////////////////////////////////////////
// Group=IUnknown Interface Methods

/////////////////////////////////////////////////////////////////////////////
// Description: Returns pointers to supported interfaces.
//
// Supports the IUnknown and IPropertyPageSite interfaces.
//
// Return Value: Returns one of the following standard results:
//
//   S_OK - The specified IID is supported on this object.
//   E_NOINTERFACE - The specified IID is *not* supported on this object.
//   E_POINTER - /ppvObject/ is an invalid pointer.
//
// See Also: TCPropertyPageSite
STDMETHODIMP TCPropertyPageSite::XPageSite::QueryInterface(REFIID riid,
  void** ppvObject)
{
  __try
  {
    // Determine the requested interface
    if (IID_IUnknown == riid || IID_IPropertyPageSite == riid)
    {
      *((IUnknown**)ppvObject) = this;
      return S_OK;
    }

    // Requested interface is not implemented by this object
    *ppvObject = NULL;
    return E_NOINTERFACE;
  }
  __except(1)
  {
    return E_POINTER;
  }
}

/////////////////////////////////////////////////////////////////////////////
// Description: Increments reference count.
//
// Does nothing since the lifetime of this object is not dependent on a
// reference count.
//
// Return Value: This implementation always returns 1.
//
// See Also: TCPropertyPageSite
STDMETHODIMP_(ULONG) TCPropertyPageSite::XPageSite::AddRef()
{
  return 1;
}

/////////////////////////////////////////////////////////////////////////////
// Description: Decrements reference count.
//
// Does nothing since the lifetime of this object is not dependent on a
// reference count.
//
// Return Value: This implementation always returns 1.
//
// See Also: TCPropertyPageSite
STDMETHODIMP_(ULONG) TCPropertyPageSite::XPageSite::Release()
{
  return 1;
}


/////////////////////////////////////////////////////////////////////////////
// Group=IPropertyPageSite Interface Methods

/////////////////////////////////////////////////////////////////////////////
// Description: Indicates that the user has modified property values on the
// property page.
//
// This method is called by the managed property page to indicate a change in
// status. This implementation ignores status changes until the page becomes
// active. Otherwise, the dirty flag of the site object is set and the
// TCPropertyPageSite::OnStatusChange virtual override is called.
//
// Parameters:
//   dwFlags - [in] Flags to indicate what changes have occurred. The dwFlags
// parameter can contain either of these two values to indicate the type of
// status change:
//
//   PROPPAGESTATUS_DIRTY - Values in page have changed so the state of the
// Apply button should be updated.
//
//   PROPPAGESTATUS_VALIDATE - Now is an appropriate time to apply changes.
//
// Return Value: The result of the TCPropertyPageSite::OnStatusChange virtual
// override is returned.
//
// See Also: TCPropertyPageSite, TCPropertyPageSite::OnStatusChange,
// TCPropertyPageSite::Page_IsDirty
STDMETHODIMP TCPropertyPageSite::XPageSite::OnStatusChange(DWORD dwFlags)
{
  // Display a trace message under _DEBUG builds
  #ifdef _DEBUG
    _bstr_t bstrFlags;
    if (dwFlags & PROPPAGESTATUS_DIRTY)
      bstrFlags += L"PROPPAGESTATUS_DIRTY ";
    if (dwFlags & PROPPAGESTATUS_VALIDATE)
      bstrFlags += L"PROPPAGESTATUS_VALIDATE ";
    if (dwFlags & PROPPAGESTATUS_CLEAN)
      bstrFlags += L"PROPPAGESTATUS_CLEAN";
    _TRACE_BEGIN
    _TRACE_PART2("TCPropertyPageSite::XPageSite::OnStatusChange(0x%08X)\n\t%ls\n",
      dwFlags, LPCWSTR(bstrFlags));
    _TRACE_PART1("\tIsPageDirty() returns %s\n",
      m_pThis->Page_IsDirty() ? TEXT("true") : TEXT("false"));
    _TRACE_END
  #endif // _DEBUG

  // Ignore status change until page is activated
  if (!m_pThis->m_bActivated)
    return S_OK;

  // Set the dirty flag
  m_pThis->m_bDirty = true;

  // Delegate to virtual function
  return m_pThis->OnStatusChange(dwFlags);
}

/////////////////////////////////////////////////////////////////////////////
// Description: Returns the locale identifier so the property page can adjust
// itself to country-specific settings.
//
// Returns the locale identifier (an LCID) that the managed property page can
// use to adjust itself to the language in use and other country-specific
// settings.
// 
// This implementation gets the locale of the current thread and then calls
// the TCPropertyPageSite::OnGetLocaleID virtual override.
//
// Parameters:
//   pLocaleID - [out] Pointer to locale identifier. 
//
// Return Value: The result of the TCPropertyPageSite::OnGetLocaleID virtual
// override is returned.
//
// See Also: TCPropertyPageSite, TCPropertyPageSite::OnGetLocaleID
STDMETHODIMP TCPropertyPageSite::XPageSite::GetLocaleID(LCID* pLocaleID)
{
  __try
  {
    // Display a trace message under _DEBUG builds
    _TRACE0("TCPropertyPageSite::XPageSite::GetLocaleID()\n");

    // Initialize the [out] parameter
    *pLocaleID = GetThreadLocale();

    // Delegate to virtual function
    return m_pThis->OnGetLocaleID(pLocaleID);
  }
  __except(1)
  {
    return E_POINTER;
  }
}

/////////////////////////////////////////////////////////////////////////////
// Description: Returns an IUnknown pointer for the object representing the
// entire property frame dialog box that contains all the pages.
//
// Returns an IUnknown pointer to the object representing the entire property
// frame dialog box that contains all the pages. Calling this method could
// potentially allow one page to navigate to another.
//
// However, there are no "container" interfaces currently defined for this
// role, so this method always fails in the current standard property frame
// implementation. But since the role of TCPropertyPageSite is to create
// non-standard property frames, this method could be used as a channel for
// property pages to communicate with a custom property frame.
//
// This implementation initializes */ppUnk/ to NULL and then calls the
// TCPropertyPageSite::OnGetPageContainer virtual override.
//
// Parameters:
//   ppUnk - [out] Address of IUnknown* pointer variable that receives the
// interface pointer to the container object.
//
// Return Value: The result of the TCPropertyPageSite::OnGetPageContainer
// virtual override is returned.
//
// See Also: TCPropertyPageSite, TCPropertyPageSite::OnGetPageContainer
STDMETHODIMP TCPropertyPageSite::XPageSite::GetPageContainer(IUnknown** ppUnk)
{
  __try
  {
    // Display a trace message under _DEBUG builds
    _TRACE0("TCPropertyPageSite::XPageSite::GetPageContainer()\n");

    // Initialize the [out] parameter
    *ppUnk = NULL;

    // Delegate to virtual function
    return m_pThis->OnGetPageContainer(ppUnk);
  }
  __except(1)
  {
    return E_POINTER;
  }
}

/////////////////////////////////////////////////////////////////////////////
// Description: Passes a keystroke to the property frame for processing.
//
// Instructs the page site to process a keystroke if it desires. This
// implementation does nothing more than call the
// TCPropertyPageSite::OnTranslateAccelerator virtual override.
//
// Parameters:
//   pMsg - [in] Pointer to the MSG structure to be processed.
//
// Return Value: The result of the TCPropertyPageSite::OnTranslateAccelerator
// virtual override is returned.
//
// See Also: TCPropertyPageSite, TCPropertyPageSite::OnTranslateAccelerator
STDMETHODIMP TCPropertyPageSite::XPageSite::TranslateAccelerator(MSG* pMsg)
{
  __try
  {
    // Display a trace message under _DEBUG builds
    _TRACE0("TCPropertyPageSite::XPageSite::TranslateAccelerator()\n");

    // Delegate to virtual function
    return m_pThis->OnTranslateAccelerator(pMsg);
  }
  __except(1)
  {
    return E_POINTER;
  }
}

