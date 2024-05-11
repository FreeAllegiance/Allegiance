#pragma once

#pragma warning(disable: 4786)
#include <vector>
#include <comdef.h>

#ifdef _DOCJET_ONLY
  ///////////////////////////////////////////////////////////////////////////
  // {tag:STRTYPE, STRTYPE2CT}
  // These declarations are only used by the string attributes of the
  // TCPropertyPageSite class.
  //
  // STRTYPE - This type definition is used to declare a string return value
  // most convenient (and available) for the framework in use. When using
  // MFC, it is defined as a CString, otherwise it is an LPCOLESTR.
  //
  // STRTYPE2CT - This macro converts a STRTYPE value to an LPCTSTR (const
  // TCHAR*). This macro should be preceded (somewhere in the same scope)
  // with the USES_CONVERSION macro.
  //
  // Macro Group: STRTYPE Declarations
  //
  // Declaration:
  // #ifdef _AFX
  //   typedef CString STRTYPE;
  //   #define STRTYPE2CT(str) LPCTSTR(str)
  // #else
  //   typedef LPCOLESTR STRTYPE;
  //   #define STRTYPE2CT(str) OLE2CT(str)
  // #endif
  // 
  // See Also: TCPropertyPageSite, TCPropertyPageSite::GetTitle,
  // TCPropertyPageSite::GetDocString, TCPropertyPageSite::GetHelpFile
  #define STRTYPE_
#endif

#ifdef _AFX
  typedef CString STRTYPE;              // {partof:STRTYPE_}
  #define STRTYPE2CT(str) LPCTSTR(str)  // {partof:STRTYPE_}
#else
  typedef LPCOLESTR STRTYPE;            // {partof:STRTYPE_}
  #define STRTYPE2CT(str) OLE2CT(str)   // {partof:STRTYPE_}
#endif


/////////////////////////////////////////////////////////////////////////////
// PropertyPageSite.h | Declaration of the TCPropertyPageSite class.
//


/////////////////////////////////////////////////////////////////////////////
// TCPropertyPageSite is a framework-neutral implementation of a property
// page site. A property page site is an object, implemented with a window,
// that contains a property page. A property page, in this context, is a
// component object that implements the IPropertyPage interface.
class TCPropertyPageSite
{
// Construction / Destruction
public:
  TCPropertyPageSite();
  TCPropertyPageSite(REFCLSID clsidPage);
  TCPropertyPageSite(IPropertyPage* pPage);
  virtual ~TCPropertyPageSite();
  HRESULT Page_Create(REFCLSID clsidPage);
  HRESULT Page_Attach(IPropertyPage* pPage);
  IPropertyPage* Page_Detach();
protected:
  void CommonConstruct();
  void CommonDestruct();

// Attributes
public:
  STRTYPE GetTitle() const;
  SIZE GetSize() const;
  STRTYPE GetDocString() const;
  STRTYPE GetHelpFile() const;
  DWORD GetHelpContext() const;
  HRESULT SetObjects(ULONG cObjects, IUnknown** ppUnk);
  REFCLSID GetClassID() const;
  void SetDirty(bool bDirty);

// Operations
public:
  BOOL OnPageSiteWndMsg(UINT message, WPARAM wParam, LPARAM lParam,
    LRESULT* plResult);
  HRESULT Page_Show();
  HRESULT Page_Hide();
  bool    Page_IsDirty();
  HRESULT Page_Apply();
  HRESULT Page_Help();
  HRESULT Page_TranslateAccelerator(MSG* pMsg);
  HRESULT Page_QI(REFIID iid, void** ppvObject);

// Group=Overrides
protected:
  virtual HRESULT OnStatusChange(DWORD dwFlags);
  virtual HRESULT OnGetLocaleID(LCID* pLocaleID);
  virtual HRESULT OnGetPageContainer(IUnknown** ppUnk);
  virtual HRESULT OnTranslateAccelerator(MSG* pMsg);
  virtual BOOL    OnIsModal();
  virtual HWND    OnGetWindow();

// Group=Implementation
protected:
  HRESULT Activate();

// Group=Types
public:
  ///////////////////////////////////////////////////////////////////////////
  // Description: Nested class that implements IPropertyPageSite.
  //
  // This class is nested within TCPropertyPageSite so that the IUnknown
  // method names will not conflict with the same methods in another base
  // class of an TCPropertyPageSite-derived class.
  //
  // Rather than directly overriding the IPropertyPageSite methods of this
  // class, a derived class should override the TCPropertyPageSite virtual
  // methods, which are called by the IPropertyPageSite methods.
  //
  // See Also: TCPropertyPageSite, TCPropertyPageSite::m_xPageSite,
  // TCPropertyPageSite::OnStatusChange, TCPropertyPageSite::OnGetLocaleID,
  // TCPropertyPageSite::OnGetPageContainer,
  // TCPropertyPageSite::OnTranslateAccelerator,
  // TCPropertyPageSite::OnIsModal, TCPropertyPageSite::OnGetWindow
  class XPageSite : public IPropertyPageSite
  {
  public:
    STDMETHODIMP_(ULONG) AddRef();
    STDMETHODIMP_(ULONG) Release();
    STDMETHODIMP QueryInterface(REFIID iid, LPVOID* ppvObj);
    STDMETHODIMP OnStatusChange(DWORD dwFlags);
    STDMETHODIMP GetLocaleID(LCID* pLocaleID);
    STDMETHODIMP GetPageContainer(IUnknown** ppUnk);
    STDMETHODIMP TranslateAccelerator(MSG* pMsg);
  // Group=Data Members
  public:
    TCPropertyPageSite* m_pThis;  // Pointer to the outer class instance.
  };
  friend class XPageSite;

// Group=Types
protected:
  // An STL vector of IUnknown smart pointers used to maintain the set of
  // objects being edited (or to be edited) by the managed property page.
  typedef std::vector<IUnknownPtr> vectorUnknown;

// Group=Data Members
protected:
  // An instance of the nested XPageSite class.
  XPageSite        m_xPageSite;
  // The managed property page's CLSID;
  CLSID            m_clsid;
  // An array of objects being edited (or to be edited) by the managed
  // property page.
  vectorUnknown    m_vecObjects;
  // The managed property page interface pointer.
  IPropertyPagePtr m_pPage;
  // Info retrieved by the managed property page's *IPropertyPage::GetPageInfo* method.
  PROPPAGEINFO     m_info;
  // Indicates if the managed property page is active.
  bool             m_bActivated;
  // Indicates if the managed property page (or this site) is dirty.
  bool             m_bDirty;
};


/////////////////////////////////////////////////////////////////////////////
// Group=Attributes

/////////////////////////////////////////////////////////////////////////////
// Description: Gets the managed property page's title string.
//
// Gets the managed property page's title string.
// Return Value: The managed property page's title string.
inline STRTYPE TCPropertyPageSite::GetTitle() const
{
  return m_info.pszTitle;
}

/////////////////////////////////////////////////////////////////////////////
// Description: Gets the managed property page's size in pixels.
//
// Gets the managed property page's size in pixels.
// Return Value: The managed property page's size in pixels.
inline SIZE TCPropertyPageSite::GetSize() const
{
  return m_info.size;
}

/////////////////////////////////////////////////////////////////////////////
// Description: Gets the managed property page's documentation string.
//
// Gets the managed property page's documentation string.
// Return Value: The managed property page's documentation string.
inline STRTYPE TCPropertyPageSite::GetDocString() const
{
  return m_info.pszDocString;
}

/////////////////////////////////////////////////////////////////////////////
// Description: Gets the managed property page's help file string.
//
// Gets the managed property page's help file string.
// Return Value: The managed property page's help file string.
inline STRTYPE TCPropertyPageSite::GetHelpFile() const
{
  return m_info.pszHelpFile;
}

/////////////////////////////////////////////////////////////////////////////
// Description: Gets the managed property page's help context ID.
//
// Gets the managed property page's help context ID.
// Return Value: The managed property page's help context ID.
inline DWORD TCPropertyPageSite::GetHelpContext() const
{
  return m_info.dwHelpContext;
}

/////////////////////////////////////////////////////////////////////////////
// Description: Gets the managed property page's CLSID.
//
// Gets the managed property page's CLSID. If the managed property page was
// not created by Page_Create, the CLSID can not be determined and will,
// therefore, be CLSID_NULL.
// Return Value: The managed property page's CLSID.
inline REFCLSID TCPropertyPageSite::GetClassID() const
{
  return m_clsid;
}

/////////////////////////////////////////////////////////////////////////////
// Description: Flags the managed property page as dirty.
//
// Parameters:
//   bDirty - true to set the dirty flag; false to clear it.
//
// Flags the managed property page as dirty. Since the IPropertyPage
// interface does not provide a method to  modify it's dirty flag, the page
// site maintains one for it at a higher level.
//
// This method will be called from the parent/site object, *not* from the
// managed property page. Thus, no notification to the outer page site should
// be necessary.
//
// See Also: TCPropertyPageSite::Page_IsDirty,
// TCPropertyPageSite::XPageSite::OnStatusChange
inline void TCPropertyPageSite::SetDirty(bool bDirty)
{
  m_bDirty = bDirty;
}


/////////////////////////////////////////////////////////////////////////////
// Group=Operations

/////////////////////////////////////////////////////////////////////////////
// Description: Gets the state of property page's dirty flag.
//
// Return Value: true if the page is dirty, otherwise false.
//
// Remarks: Since the site receives notification from the page through the
// OnStatusChange method, the site object's dirty flag is set if either the
// page notified us that it is dirty, or if the SetDirty method was called
// explicitly to change it.
//
// See Also: TCPropertyPageSite::SetDirty,
// TCPropertyPageSite::XPageSite::OnStatusChange
inline bool TCPropertyPageSite::Page_IsDirty()
{
  return m_bDirty;
}

/////////////////////////////////////////////////////////////////////////////
// Description: Applies changes to the objects being viewed by the managed
// property page.
//
// Return Value: E_UNEXPECTED if called prior to a property page being
// created or attached. Otherwise, the result from the property page's
// interface method.
//
// Remarks: Prior to delegating to the IPropertyPage::Apply method of the
// managed property page, the current focus window is saved. The focus is
// restored to that window following the method call so as to lessen the
// visual impact of the user interface when applying changes.
inline HRESULT TCPropertyPageSite::Page_Apply()
{
  if (NULL != m_pPage)
  {
    HRESULT hr = S_OK;
    HWND hwndFocus = ::GetFocus();
    if (m_bDirty)
    {
      hr = m_pPage->Apply();
      _TRACE1("TCPropertyPageSite::Page_Apply() Apply returned: 0x%08X\n", hr);
      if (SUCCEEDED(hr))
        m_bDirty = false;
    }
    if (::GetFocus() != hwndFocus)
      ::SetFocus(hwndFocus);
    return hr;
  }

  _TRACE0("TCPropertyPageSite::Apply(): Called with a NULL m_pPage\n");
  return E_UNEXPECTED;
}


/////////////////////////////////////////////////////////////////////////////