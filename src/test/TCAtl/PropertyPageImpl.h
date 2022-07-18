#ifndef __PropertyPageImpl_h_
#define __PropertyPageImpl_h_


/////////////////////////////////////////////////////////////////////////////
// PropertyPageImpl.h | Declaration of the TCPropertyPageImpl class.

#pragma warning(disable: 4786)
#include <vector>
#include <map>
#include <set>
#include <comdef.h>
#include <typeinfo.h>
#include "..\TCLib\AdviseHolder.h"
#include "InsidePropPage.h"
#include "PageEntry.h"


/////////////////////////////////////////////////////////////////////////////
// UI Helper Functions

/////////////////////////////////////////////////////////////////////////////
// Description: Converts a boolean expression to a check box state constant.
//
// Converts the specified  boolean expression to a check box state constant.
//
// Parameters:
//   bExp - An expression of any type, to be evaluated as a boolean
// expression.
//
// Return Value: *BST_CHECKED* if the /bExp/ is non-zero, otherwise
// *BST_UNCHECKED*.
//
// See Also: Checked_false, VARIANT_TRUE_Checked, VARIANT_TRUE_Unchecked
template <class T>
inline UINT Checked_true(T bExp)
{
  return bExp ? BST_CHECKED : BST_UNCHECKED;
}

/////////////////////////////////////////////////////////////////////////////
// Description: Converts a boolean expression to a check box state constant.
//
// Converts the specified  boolean expression to a check box state constant.
//
// Parameters:
//   bExp - An expression of any type, to be evaluated as a boolean
// expression.
//
// Return Value: *BST_UNCHECKED* if the /bExp/ is non-zero, otherwise
// *BST_CHECKED*.
//
// See Also: Checked_true, VARIANT_TRUE_Checked, VARIANT_TRUE_Unchecked
template <class T>
inline UINT Checked_false(T bExp)
{
  return Checked_true(!bExp);
}

/////////////////////////////////////////////////////////////////////////////
// Description: Converts a check box state constant to a *VARIANT_BOOL*.
//
// Converts the specified check box state constant to a *VARIANT_BOOL*.
//
// Parameters:
//   nCheck - A check box state constant, *BST_CHECKED* or *BST_UNCHECKED*.
//
// Return Value: *VARIANT_TRUE* if /nCheck/ is equal to *BST_CHECKED*,
// otherwise, *VARIANT_FALSE*.
//
// See Also: VARIANT_TRUE_Unchecked, Checked_true, Checked_false
inline VARIANT_BOOL VARIANT_TRUE_Checked(UINT nCheck)
{
  return (nCheck == BST_CHECKED) ? VARIANT_TRUE : VARIANT_FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// Description: Converts a check box state constant to a *VARIANT_BOOL*.
//
// Converts the specified check box state constant to a *VARIANT_BOOL*.
//
// Parameters:
//   nCheck - A check box state constant, *BST_CHECKED* or *BST_UNCHECKED*.
//
// Return Value: *VARIANT_TRUE* if /nCheck/ is equal to *BST_UNCHECKED*,
// otherwise, *VARIANT_FALSE*.
//
// See Also: VARIANT_TRUE_Checked, Checked_true, Checked_false
inline VARIANT_BOOL VARIANT_TRUE_Unchecked(UINT nCheck)
{
  return (nCheck == BST_UNCHECKED) ? VARIANT_TRUE : VARIANT_FALSE;
}


/////////////////////////////////////////////////////////////////////////////
// Description: Interface map entries to be included in the interface map of
// derived classes.
//
// See Also: TCPropertyPageImpl, CHAIN_MSG_MAP_TCPropertyPageImpl
#define COM_INTERFACE_ENTRIES_TCPropertyPageImpl()                          \
  COM_INTERFACE_ENTRY_IMPL(IPropertyPage)                                   \
  COM_INTERFACE_ENTRY(IPropertyPageSite)                                    \
  COM_INTERFACE_ENTRY(ITCPropertyFieldToggle)                               \
  COM_INTERFACE_ENTRY(IPropertyNotifySink)


/////////////////////////////////////////////////////////////////////////////
// Description: Message map entries to be included in the message map of
// derived classes.
//
// See Also: TCPropertyPageImpl, COM_INTERFACE_ENTRIES_TCPropertyPageImpl
#define CHAIN_MSG_MAP_TCPropertyPageImpl()                                  \
  CHAIN_MSG_MAP(TCPropertyPageImplBase)


/////////////////////////////////////////////////////////////////////////////
// TCPropertyPageImpl provides a consistent implementation of the
// *IPropertyPage* interface. This class extends the ATL
// *IPropertyPageImpl* class and should be used in place of it.
//
// Microsoft first created the *IPropertyPage* interface for use with
// ActiveX Controls (formerly known as OLE Controls or OCX's). Since then,
// the interface has proven to be effective in other scenarios, such as
// Microsoft's DirectShow (formerly known as ActiveMovie), and are also an
// important part of the MMAC Codec Driver Model.
//
// After developing an assortment of property pages, it becomes clear that
// each one has many things in common with all of the others. To simplify
// the creation of these, the TCPropertyPageImpl template class is designed
// to take care of the many common chores of a property page implementation.
// In addition, the class provides a solid framework for the embedding of
// property pages within other pages. This helps to reduce rendundant coding
// and to increase user interface reusability.
//
// For more information on how to use this class, see the documentation for
// the Property Page Field Update Macros and the Embedded Property Page Table
// Macros. Also, the ATL documentation for the *IPropertyPageImpl* class,
// from which this class is derived, may be helpful. Of course, analysis of
// sample code, along with this reference documentation, will prove to be the
// best guide.
//
// TODO: This class should also implement the *IPropertyPage2* interface,
// which adds the *EditProperty* method to specify which field is to receive
// the focus when the property page is activated. This could easily be
// implemented from the information already contained in the entries of the
// property field map.
//
// Parameters:
//   T - Your property page class, derived from TCPropertyPageImpl.
//
// See Also: TCPropertyPageBase, Property Page Field Update Macros,
// Embedded Property Page Table Macros
#include <CodecDriver.h>

template <class T>
class ATL_NO_VTABLE TCPropertyPageImpl :
  public IPropertyPageImpl<T>,
  public IPropertyPageSite,
  public IPropertyNotifySink,
  public CDialogImpl<T>,
  public TCPropertyPageBase,
  public IDispatchImpl<ITCPropertyFieldToggle, &IID_ITCPropertyFieldToggle, &LIBID_TCCodecDriver>
{
// Group=Types
public:
  typedef TCPropertyPageImpl<T> TCPropertyPageImplBase;
  typedef TCPropertyPageImpl<T> Type;
  typedef void (Type::*XFieldUpdateProc)(ULONG, IUnknown**);
  typedef void (Type::*XFieldApplyProc)(ULONG, IUnknown**);
  typedef void (Type::*XFieldUpdateProcEx)(UINT, DISPID, REFIID, ULONG, IUnknown**);
  typedef void (Type::*XFieldApplyProcEx)(UINT, DISPID, REFIID, ULONG, IUnknown**);

  ///////////////////////////////////////////////////////////////////////////
  // Description: Declares the entry in a table of property page fields.
  struct XPageFieldEntry
  {
    UINT               idCtrl;      // {secret}
    DISPID             dispid;      // {secret}
    const IID*         piid;        // {secret}
    XFieldUpdateProc   pfnUpdate;   // {secret}
    XFieldApplyProc    pfnApply;    // {secret}
    XFieldUpdateProcEx pfnUpdateEx; // {secret}
    XFieldApplyProcEx  pfnApplyEx;  // {secret}
  };

// Group=Enumerations
public:
  // Private window message definitions
  enum EMessages
  {
    wm_OnChanged = WM_APP, // Message sent when a property changes.
  };

// Message Map
public:
  BEGIN_MSG_MAP(TCPropertyPageImpl<T>)
    MESSAGE_HANDLER(wm_OnChanged, OnChangedHandler)
    MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialogHandler)
    MESSAGE_HANDLER(WM_NCDESTROY, OnNcDestroyHandler)
    // Base class message handling
    CHAIN_MSG_MAP(IPropertyPageImpl<T>)
  END_MSG_MAP()

// Group=Construction / Destruction
public:
  TCPropertyPageImpl();
  virtual ~TCPropertyPageImpl();

// Group=Attributes
public:
  void SetDirty(BOOL bDirty);
  void SetDirtyNoApply();

// Group=Diagnostic Overrides
protected:
  #ifdef _DEBUG
    virtual LPCSTR TypeName()
    {
      return TCTypeName(T);
    }
  #endif

// Group=Overrides
protected:
  virtual HWND GetPageWindow();
  virtual IUnknown* GetPageUnknown();
  virtual bool IsObjectKnown(IUnknown* punk);
  virtual void UpdateFields(DISPID dispid = DISPID_UNKNOWN);
public:
  HWND Create(HWND hWndParent);
  void OnApplyFields();
  bool OnUpdateFields(DISPID dispid = DISPID_UNKNOWN);
  UINT GetPageFieldTable(const XPageFieldEntry** ppTable);

// Group=Message Handlers
public:
  LRESULT OnSetDirty(WORD, WORD, HWND, BOOL& bHandled);
  LRESULT OnSetDirtyNoApply(WORD, WORD, HWND, BOOL& bHandled);
  LRESULT OnInitDialogHandler(UINT, WPARAM, LPARAM, BOOL&);

// Group=IPropertyPage Interface Methods
public:
  STDMETHODIMP GetPageInfo(PROPPAGEINFO *pPageInfo);
  STDMETHODIMP SetObjects(ULONG cObjects, IUnknown** ppUnk);
  STDMETHODIMP Apply();
  STDMETHODIMP Help(LPCOLESTR pszHelpDir);

// Group=IPropertyPageSite Interface Methods
public:
  STDMETHODIMP OnStatusChange(DWORD dwFlags);
  STDMETHODIMP GetLocaleID(LCID* pLocaleID);
  STDMETHODIMP GetPageContainer(IUnknown** ppUnk);
  STDMETHODIMP TranslateAccelerator(MSG* pMsg);

// Group=IPropertyNotifySink Interface Methods
public:
  STDMETHODIMP OnChanged(DISPID dispID);
  STDMETHODIMP OnRequestEdit(DISPID dispID);

// Group=ITCPropertyFieldToggle Interface Methods
public:
  STDMETHODIMP put_EnableField(DISPID dispid, VARIANT_BOOL bEnable);
  STDMETHODIMP get_EnableField(DISPID dispid, VARIANT_BOOL* pbEnable);
  STDMETHODIMP put_ShowField(DISPID dispid, VARIANT_BOOL bShow);
  STDMETHODIMP get_ShowField(DISPID dispid, VARIANT_BOOL* pbShow);

// Group=Helpers
protected:
  void FieldToggle(DISPID dispid, bool bEnableDisable, bool bToggle);
  bool GetFieldToggle(DISPID dispid, bool bEnableDisable);

// Group=Enumerations
public:
  enum {IDD};

// Group=Data Members
public:
  WORD m_IDD;
};


/////////////////////////////////////////////////////////////////////////////
// Group=Construction / Destruction

/////////////////////////////////////////////////////////////////////////////
template <class T>
TCPropertyPageImpl<T>::TCPropertyPageImpl() :
  m_IDD(T::IDD)
{
  _TRACE1("TCPropertyPageImpl<%hs>::TCPropertyPageImpl()\n", TCTypeName(T));

  // Get the derived class pointer
  T* pThis = static_cast<T*>(this);

  // Get the table of supported interfaces
  const XPageFieldEntry* pTable = NULL;
  UINT nEntries = pThis->GetPageFieldTable(&pTable);

  // Initialize the map of supported interfaces
  for (UINT i = 0; i < nEntries; ++i)
  {
    CUnkVectorMap::value_type value(*pTable[i].piid, CUnkVector());
    m_mapInterfaces.insert(value);
  }
}

/////////////////////////////////////////////////////////////////////////////
template <class T>
TCPropertyPageImpl<T>::~TCPropertyPageImpl()
{
  _TRACE1("TCPropertyPageImpl<%hs>::~TCPropertyPageImpl()\n", TCTypeName(T));
}


/////////////////////////////////////////////////////////////////////////////
// Group=Attributes

/////////////////////////////////////////////////////////////////////////////
// Description: Sets or resets the dirty flag of the property page.
//
// Sets or resets the dirty flag of the property page. When the page is
// updating its fields from the objects (within the UpdateFields method),
// this method returns immediately.
//
// If the dirty flag is changed as a result of this method call, it will call
// the *IPropertyPageSite::OnStatusChage* method of the page's site object,
// passing both the *PROPPAGESTATU_DIRTY* and *PROPPAGESTATUS_VALIDATE* „
// flags. If the dirty flag is to be set /without/ signaling the
// *PROPPAGESTATUS_VALIDATE* flag, see the SetDirtyNoApply method.
//
// Parameters:
//   bDirty - When *TRUE*, the page's dirty flag is set. When *FALSE*, the
// page's dirty flag is reset.
//
// See Also: TCPropertyPageImpl::SetDirtyNoApply,
// TCPropertyPageImpl::m_bUpdating
template <class T>
inline void TCPropertyPageImpl<T>::SetDirty(BOOL bDirty)
{
  // Do nothing if page is updating
  if (m_bUpdating)
    return;

  // Get the derived class pointer
  T* pThis = static_cast<T*>(this);

  // Get the current dirty flag
  bool bDirtyPrev = !!pThis->m_bDirty;

  // Save the new dirty flag
  pThis->m_bDirty = bDirty;

  // Send a status change to the page site
  if (!bDirtyPrev && bDirty)
    pThis->m_pPageSite->OnStatusChange(PROPPAGESTATUS_DIRTY |
      PROPPAGESTATUS_VALIDATE);
}

/////////////////////////////////////////////////////////////////////////////
// Description: Sets the dirty flag of the property page without indicating
// to the page site that the changes should be applied.
//
// Sets the dirty flag of the property page without indicating to the page
// site that the changes should be applied. When the page is
// updating its fields from the objects (within the UpdateFields method),
// this method returns immediately.
//
// If the dirty flag is changed as a result of this method call, it will call
// the *IPropertyPageSite::OnStatusChage* method of the page's site object,
// passing only the *PROPPAGESTATU_DIRTY* flag. If the
// *PROPPAGESTATUS_VALIDATE* flag is also to be set, see the SetDirty method.
//
// See Also: TCPropertyPageImpl::SetDirty, TCPropertyPageImpl::m_bUpdating
template <class T>
inline void TCPropertyPageImpl<T>::SetDirtyNoApply()
{
  // Do nothing if page is updating
  if (m_bUpdating)
    return;

  // Get the derived class pointer
  T* pThis = static_cast<T*>(this);

  // Get the current dirty flag
  bool bDirtyPrev = !!pThis->m_bDirty;

  // Save the new dirty flag
  pThis->m_bDirty = true;

  // Send a status change to the page site
  if (!bDirtyPrev)
    pThis->m_pPageSite->OnStatusChange(PROPPAGESTATUS_DIRTY);
}


/////////////////////////////////////////////////////////////////////////////
// Group=Overrides

/////////////////////////////////////////////////////////////////////////////
// Description: Creates the property page window.
//
// Creates the property page window. This was overridden from the ATL
// *IPropertyPageImpl* class so that the m_IDD member variable would be used
// instead of the IDD enum value.
//
template <class T>
HWND TCPropertyPageImpl<T>::Create(HWND hWndParent)
{
  // Get the derived class pointer
  T* pThis = static_cast<T*>(this);

  ZAssert(pThis->m_hWnd == NULL);
  _Module.AddCreateWndData(&pThis->m_thunk.cd, (CDialogImplBase*)pThis);
  HWND hWnd = ::CreateDialogParam(_Module.GetResourceInstance(),
      MAKEINTRESOURCE(pThis->m_IDD),
      hWndParent,
      (DLGPROC)StartDialogProc,
      NULL);
  ZAssert(pThis->m_hWnd == hWnd);
  return hWnd;
}

/////////////////////////////////////////////////////////////////////////////
// Description: Returns the *HWND* of the property page.
//
// Return Value: The *HWND* of the property page.
template <class T>
HWND TCPropertyPageImpl<T>::GetPageWindow()
{
  // Get the derived class pointer
  T* pThis = static_cast<T*>(this);
  return *pThis;
}

/////////////////////////////////////////////////////////////////////////////
// Description: Returns the *IUnknown* of the property page.
//
// Used by the TCPropertyPageBase base class to get the *IUnknown* pointer of
// the most-derived class.
template <class T>
IUnknown* TCPropertyPageImpl<T>::GetPageUnknown()
{
  // Get the derived class pointer
  T* pThis = static_cast<T*>(this);
  return pThis->GetUnknown();
}

/////////////////////////////////////////////////////////////////////////////
// Description: Determines if an object supports an interesting interface.
//
// Determines if the specified object supports an interface needed by the
// Property Page Field Update Macros.
//
// Parameters:
//   punk - An *IUnknown* pointer of the object to be tested for a known
// interface.
//
// Return Value: *true* if the object supports an interface specified in the
// property field map. Otherwise, *false*.
//
// See Also: Property Page Field Update Macros
template <class T>
inline bool TCPropertyPageImpl<T>::IsObjectKnown(IUnknown* punk)
{
  // Get the derived class pointer
  T* pThis = static_cast<T*>(this);

  // Get the table of supported interfaces
  const XPageFieldEntry* pTable = NULL;
  UINT nEntries = pThis->GetPageFieldTable(&pTable);

  // Loop thru the supported IID's in the table
  typedef std::set<const IID*> CIIDSet;
  CIIDSet setPIID;
  bool bKnown = false;
  for (UINT i = 0; i < nEntries; ++i)
  {
    // Get the next table entry
    const XPageFieldEntry& entry = pTable[i];

    // See if we've already tested the entry's IID
    if (setPIID.end() == setPIID.find(entry.piid))
    {
      // QueryInterface for the IID specified by piid
      IUnknown* pqi;
      if (SUCCEEDED(punk->QueryInterface(*entry.piid, (void**)&pqi)))
      {
        ZAssert(pqi);
        bKnown = true;

        // Add the interface pointer to the vector associated with the IID
        CUnkVectorMap::iterator it = m_mapInterfaces.find(*entry.piid);
        ZAssert(m_mapInterfaces.end() != it);
        CUnkVector& vec = it->second;
        vec.push_back(pqi);
      }

      // Add the entry's IID to the set
      setPIID.insert(entry.piid);
    }
  }
  return bKnown;
}

/////////////////////////////////////////////////////////////////////////////
// Description: Updates the property page fields from the set of objects
// being viewed/edited.
//
// Updates the property page fields from the set of objects being
// viewed/edited. If /dispid/ is DISPID_UNKNOWN, all properties are updated.
// Otherwise, just the specified property is updated.
//
// Parameters:
//   dispid - Specifies the property that needs to be updated. DISPID_UNKNOWN
// specifies that *all* properties are to be updated.
//
// See Also: Property Page Field Update Macros,
// TCPropertyPageImpl::OnUpdateFields
template <class T>
void TCPropertyPageImpl<T>::UpdateFields(DISPID dispid)
{
  // Get the derived class pointer
  T* pThis = static_cast<T*>(this);

  // Update dialog fields from objects
  m_bUpdating = true;
  __try
  {
    if (!pThis->OnUpdateFields(dispid))
      OnUpdateFields(dispid);
  }
  __except(1)
  {
    _TRACE1("TCPropertyPageImpl<%hs>::UpdateFields(): Error! Unhandled exception caught!\n",
      TCTypeName(T));
  }
  m_bUpdating = false;
}

/////////////////////////////////////////////////////////////////////////////
// Description: Updates the property page fields from the set of objects
// being viewed/edited.
//
// Loops thru the entries of the property field map and calls the function
// specified in the entry to update the property specified by /dispid/.
//
// Parameters:
//   dispid - Specifies the property that needs to be updated. DISPID_UNKNOWN
// specifies that *all* properties are to be updated.
//
// Return Value: Currently, this method always returns *true*.
//
// See Also: Property Page Field Update Macros,
// TCPropertyPageImpl::UpdateFields, TCPropertyPageImpl::OnApplyFields
template <class T>
bool TCPropertyPageImpl<T>::OnUpdateFields(DISPID dispid)
{
  // Get the derived class pointer
  T* pThis = static_cast<T*>(this);

  // Get the table of supported interfaces
  const XPageFieldEntry* pTable = NULL;
  UINT nEntries = pThis->GetPageFieldTable(&pTable);

  // Loop thru each field entry
  for (UINT i = 0; i < nEntries; ++i)
  {
    // Get the next field entry
    const XPageFieldEntry& entry = pTable[i];

    if (DISPID_UNKNOWN == dispid || entry.dispid == dispid || !entry.dispid)
    {
      // Only process if the entry's Update method was specified
      if (NULL != entry.pfnUpdate || NULL != entry.pfnUpdateEx)
      {
        // Get the CUnkVector for the entry's IID
        CUnkVectorMap::iterator it = m_mapInterfaces.find(*entry.piid);
        CUnkVector& vec = it->second;
        ZAssert(m_mapInterfaces.end() != it);

        // Call the specified field Update method
        __try
        {
          if (NULL != entry.pfnUpdateEx)
            (pThis->*entry.pfnUpdateEx)(entry.idCtrl, dispid, *entry.piid,
              vec.size(), vec.begin());
          else
            (pThis->*entry.pfnUpdate)(vec.size(), vec.begin());
        }
        __except(1)
        {
          LPCSTR pszClass = TCTypeName(T);
          _TRACE_BEGIN
            _TRACE_PART1("TCPropertyPageImpl<%hs>::OnUpdateFields(): ", pszClass);
            _TRACE_PART0("Error! Unhandled exception caught!\n");
          _TRACE_END
        }
      }
    }
  }

  // Indicate that we updated the fields
  return true;
}

/////////////////////////////////////////////////////////////////////////////
// Description: Applies the property page fields to the set of objects being
// edited.
//
// Applies the property page fields to the set of objects being edited.
// Loops thru the entries of the property field map and calls the function
// specified in the entry to apply the property specified by /dispid/.
//
// See Also: Property Page Field Update Macros,
// TCPropertyPageImpl::OnUpdateFields
template <class T>
inline void TCPropertyPageImpl<T>::OnApplyFields()
{
  // Get the derived class pointer
  T* pThis = static_cast<T*>(this);

  // Get the table of supported interfaces
  const XPageFieldEntry* pTable = NULL;
  UINT nEntries = pThis->GetPageFieldTable(&pTable);

  // Loop thru each field entry
  for (UINT i = 0; i < nEntries; ++i)
  {
    // Get the next field entry
    const XPageFieldEntry& entry = pTable[i];

    // Only process if the entry's Apply method was specified
    if (NULL != entry.pfnApply || NULL != entry.pfnApplyEx)
    {
      // Get the CUnkVector for the entry's IID
      CUnkVectorMap::iterator it = m_mapInterfaces.find(*entry.piid);
      CUnkVector& vec = it->second;
      ZAssert(m_mapInterfaces.end() != it);

      // Call the specified field Apply method
      __try
      {
        if (NULL != entry.pfnApplyEx)
          (pThis->*entry.pfnApplyEx)(entry.idCtrl, entry.dispid,
            *entry.piid, vec.size(), vec.begin());
        else
          (pThis->*entry.pfnApply)(vec.size(), vec.begin());
      }
      __except(1
      {
        _TRACE_BEGIN
          _TRACE_PART1("TCPropertyPageImpl<%hs>::", TCTypeName(T));
          _TRACE_PART0("OnApplyFields(): Error! Unhandled exception caught!\n");
        _TRACE_END
      }
    }
  }
}

/////////////////////////////////////////////////////////////////////////////
// Description: Gets the property page field map.
//
// Gets the address of, and the number of entries in the property pages's
// field map, as defined by the Property Page Field Update Macros.
//
// Note: This method is automatically overridden by the Property Page Field
// Macros. It is not necessary to override it manually.
//
// Return Value: The number of entries in the property page field map.
//
// See Also: Property Page Field Update Macros
template <class T>
inline UINT TCPropertyPageImpl<T>::GetPageFieldTable(
  const TCPropertyPageImpl<T>::XPageFieldEntry** ppTable)
{
  // Default does nothing
  if (ppTable)
    *ppTable = NULL;
  return 0;
}


/////////////////////////////////////////////////////////////////////////////
// Group=Message Handlers

/////////////////////////////////////////////////////////////////////////////
// Description: A general message handler that can be used by derived-classes
// to set the dirty flag of the property page.
//
// A general message handler that can be used by derived-classes to set the
// dirty flag of the property page.
//
// See Also: TCPropertyPageImpl::SetDirty
template <class T>
LRESULT TCPropertyPageImpl<T>::OnSetDirty(WORD, WORD, HWND,
  BOOL& bHandled)
{
  // Get the derived class pointer
  T* pThis = static_cast<T*>(this);

  // Set the dirty flag of the property page
  pThis->SetDirty(TRUE);

  // Act like we didn't handle this message
  bHandled = FALSE;
  return 0;
}

/////////////////////////////////////////////////////////////////////////////
// Description: A general message handler that can be used by derived-classes
// to set the dirty flag of the property page.
//
// A general message handler that can be used by derived-classes to set the
// dirty flag of the property page.
//
// See Also: TCPropertyPageImpl::SetDirtyNoApply
template <class T>
LRESULT TCPropertyPageImpl<T>::OnSetDirtyNoApply(WORD, WORD, HWND,
  BOOL& bHandled)
{
  // Get the derived class pointer
  T* pThis = static_cast<T*>(this);

  // Set the dirty flag of the property page
  pThis->SetDirtyNoApply();

  // Act like we didn't handle this message
  bHandled = FALSE;
  return 0;
}

/////////////////////////////////////////////////////////////////////////////
// Description: Overridden message handler for WM_INITDIALOG.
//
// Delegates to the TCPropertyPageBase base class.
//
// Return Value: Returns *FALSE* so as to not set the focus to the first
// tabstop control.
//
// See Also: TCPropertyPageBase::OnInitDialogHandler
template <class T>
LRESULT TCPropertyPageImpl<T>::OnInitDialogHandler(UINT, WPARAM,
  LPARAM, BOOL&)
{
  _TRACE1("TCPropertyPageImpl<%hs>::OnInitDialogHandler()\n", TCTypeName(T));

  // Delegate to the base class
  TCPropertyPageBase::OnInitDialogHandler(m_nObjects, m_ppUnk);

  // Return FALSE so as to not set the focus to the first tabstop control
  return FALSE;
}


/////////////////////////////////////////////////////////////////////////////
// Group=IPropertyPage Interface Methods

/////////////////////////////////////////////////////////////////////////////
// Description: Returns information about the property page.
//
// Fills a caller-allocated *PROPPAGEINFO* structure to provide the caller
// with information about the property page.
//
// This was overridden from the ATL *IPropertyPageImpl* class so that the
// m_IDD member variable would be used instead of the IDD enum value.
//
// Parameters:
//   pPageInfo - [out] Pointer to the caller-allocated *PROPPAGEINFO* „
// structure in which the property page stores its page information. All
// allocations stored in this structure become the responsibility of the
// caller. 
//
// Return Value: This method supports the standard return values
// *E_OUTOFMEMORY* and *E_UNEXPECTED*, as well as the following:
//
//   S_OK - The structure was successfully filled. 
//   E_POINTER - The address in /pPageInfo/ is not valid. For example, it may
// be NULL. 
template <class T>
STDMETHODIMP TCPropertyPageImpl<T>::GetPageInfo(PROPPAGEINFO *pPageInfo)
{
  _TRACE1("TCPropertyPageImpl<%hs>::GetPageInfo\n", TCTypeName(T));

  if (pPageInfo == NULL)
  {
    _TRACE0("Error : PROPPAGEINFO passed == NULL\n");
    return E_POINTER;
  }

  // Get the derived class pointer
  T* pThis = static_cast<T*>(this);

  HRSRC hRsrc = FindResource(_Module.GetResourceInstance(),
                 MAKEINTRESOURCE(pThis->m_IDD), RT_DIALOG);
  if (hRsrc == NULL)
  {
    _TRACE0("Could not find resource template\n");
    return E_UNEXPECTED;
  }

  HGLOBAL hGlob = LoadResource(_Module.GetResourceInstance(), hRsrc);
  DLGTEMPLATE* pTemp = (DLGTEMPLATE*)LockResource(hGlob);
  if (pTemp == NULL)
  {
    _TRACE0("Could not load resource template\n");
    return E_UNEXPECTED;
  }
  GetDialogSize(pTemp, &pThis->m_size);

  pPageInfo->cb = sizeof(PROPPAGEINFO);
  pPageInfo->pszTitle = LoadStringHelper(pThis->m_dwTitleID);
  pPageInfo->size = pThis->m_size;
  pPageInfo->pszHelpFile = LoadStringHelper(pThis->m_dwHelpFileID);
  pPageInfo->pszDocString = LoadStringHelper(pThis->m_dwDocStringID);
  pPageInfo->dwHelpContext = pThis->m_dwHelpContext;

  return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
// Description: Provides the property page with an array of IUnknown pointers
// for objects associated with this property page.
//
// Delegates to TCPropertyPageBase::SetObjects.
//
// Parameters:
//   cObjects - [in] Number of *IUnknown* pointers in the array pointed to
// by /ppUnk/.
//   ppUnk - [in] Pointer to an array of *IUnknown* interface pointers where
// each pointer identifies a unique object affected by the property sheet in
// which this (and possibly other) property pages are displayed.
//
// Return Value: This method supports the standard return values *E_FAIL*,
// *E_INVALIDARG*, *E_OUTOFMEMORY*, and *E_UNEXPECTED*, as well as the
// following: 
//
//   S_OK - The property page successfully saved the pointers it needed. 
//   E_NOINTERFACE - One of the objects in /ppUnk/ did not support the
// interface expected by this property page, and so this property page
// cannot communicate with it. 
//   E_POINTER - The address in /ppUnk/ is not valid. For example, it may be
// NULL.
//
// See Also: TCPropertyPageBase::SetObjects
template <class T> STDMETHODIMP
TCPropertyPageImpl<T>::SetObjects(ULONG cObjects, IUnknown** ppUnk)
{
  // Delegate to base class
  return
    TCPropertyPageBase::SetObjects(m_nObjects, m_ppUnk, cObjects, ppUnk);
}

/////////////////////////////////////////////////////////////////////////////
// Description: Applies current property page values to underlying objects
// specified through SetObjects.
//
// Applies the current values to the underlying objects associated with the
// property page as previously passed to SetObjects.
//
//
// Return Value: This method supports the standard return values
// *E_OUTOFMEMORY* and *E_UNEXPECTED*, as well as the following: 
//
//   S_OK - Changes were successfully applied and the property page is
// current with the underlying objects. 
//   S_FALSE - Changes were applied, but the property page cannot determine
// if its state is current with the objects.
//
// See Also: TCPropertyPageImpl::SetObjects
template <class T>
STDMETHODIMP TCPropertyPageImpl<T>::Apply()
{
  // Get the derived class pointer
  T* pThis = static_cast<T*>(this);

  // Forward the method to each embedded property page, including groups
  RETURN_FAILED(ApplyToInsidePages());

  __try
  {
    // Allow the derived class to apply its changes, assuming page is dirty
    // NOTE: This puts the responsibility on the page site to know if the
    // property page is dirty.
    pThis->OnApplyFields();

    // Reset the dirty flag
    pThis->SetDirty(FALSE);

    // Indicate success
    return S_OK;
  }
  __except(1)
  {
    _TRACE1("TCPropertyPageImpl<%hs>::Apply(): Error! Unhandled exception caught!\n",
      TCTypeName(T));
    return RPC_E_SERVERFAULT;
  }
}

/////////////////////////////////////////////////////////////////////////////
// Description: Invokes help in response to end-user request.
//
// Parameters:
//   pszHelpDir - [in] Pointer to the string under the *HelpDir* key in the
// property page's CLSID information in the registry. If *HelpDir* does not
// exist, this will be the path found in the *InProcServer32* entry minus the
// server file name.
//
// Return Value: This implementation always returns *E_NOTIMPL*, indicating
// that help is provided only through the information in *PROPPAGEINFO*.
// 
// See Also: TCPropertyPageImpl::GetPageInfo
template <class T>
STDMETHODIMP TCPropertyPageImpl<T>::Help(LPCOLESTR pszHelpDir)
{
  _TRACE2("TCPropertyPageImpl<%hs>::Help(\"%ls\")\n",
    TCTypeName(T), pszHelpDir);
  return E_NOTIMPL;
}


/////////////////////////////////////////////////////////////////////////////
// Group=IPropertyPageSite Interface Methods

/////////////////////////////////////////////////////////////////////////////
// Description: Indicates that the user has modified property values on the
// property page.
//
// Informs the frame that the property page managed by this site has changed
// its state, that is, one or more property values have been changed in the
// page.
//
// This is a method of the property page's *IPropertyPageSite* „
// implementation, which is provided to any property pages embedded within
// this one.
//
// This implementation sets the dirty flag of this property page and then
// simply delegates to the same method of the property page's
// *IPropertyPageSite*.
//
// Parameters:
//   dwFlags - [in] Flags to indicate what changes have occurred.
//
// Return Value: The return value of the delegated method call.
//
// See Also: Embedded Property Page Table Macros
template <class T>
STDMETHODIMP TCPropertyPageImpl<T>::OnStatusChange(DWORD dwFlags)
{
  // Check for flags that would indicate that child is dirty
  if ((PROPPAGESTATUS_DIRTY | PROPPAGESTATUS_VALIDATE) & dwFlags)
  {
    // Get the derived class pointer
    T* pThis = static_cast<T*>(this);

    // Set the dirty flag of this page
    if (PROPPAGESTATUS_VALIDATE & dwFlags)
      pThis->SetDirty(TRUE);
    else
      pThis->SetDirtyNoApply();
    return S_OK;
  }

  // Delegate to this page's site
  ZAssert(NULL != m_pPageSite);
  return m_pPageSite->OnStatusChange(dwFlags);
}

/////////////////////////////////////////////////////////////////////////////
// Description: Returns the locale identifier so the property page can adjust
// itself to country-specific settings.
//
// Returns the locale identifier (an LCID) that a property page can use to
// adjust itself to the language in use and other country-specific settings.
//
// This is a method of the property page's *IPropertyPageSite* „
// implementation, which is provided to any property pages embedded within
// this one.
//
// This implementation simply delegates to the same method of the property
// page's *IPropertyPageSite*.
//
// Parameters:
//   pLocaleID - [out] Pointer to locale identifier.
//
// Return Value: The return value of the delegated method call.
//
// See Also: Embedded Property Page Table Macros
template <class T>
STDMETHODIMP TCPropertyPageImpl<T>::GetLocaleID(LCID* pLocaleID)
{
  // Delegate to this page's site
  ZAssert(NULL != m_pPageSite);
  return m_pPageSite->GetLocaleID(pLocaleID);
}

/////////////////////////////////////////////////////////////////////////////
// Description: Returns an *IUnknown* pointer for the object representing the
// entire property frame dialog box that contains all the pages.
//
// This is a method of the property page's *IPropertyPageSite* „
// implementation, which is provided to any property pages embedded within
// this one.
//
// This implementation simply delegates to the same method of the property
// page's *IPropertyPageSite*.
//
// Parameters:
//   ppUnk - [out] Address of *IUnknown* pointer variable that receives the
// interface pointer to the container object.
//
// Return Value: The return value of the delegated method call.
//
// See Also: Embedded Property Page Table Macros
template <class T>
STDMETHODIMP TCPropertyPageImpl<T>::GetPageContainer(IUnknown** ppUnk)
{
  // Delegate to this page's site
  ZAssert(NULL != m_pPageSite);
  return m_pPageSite->GetPageContainer(ppUnk);
}

/////////////////////////////////////////////////////////////////////////////
// Description: Passes a keystroke to the property frame for processing.
//
// Instructs the page site to process a keystroke if it desires.
//
// This is a method of the property page's *IPropertyPageSite* „
// implementation, which is provided to any property pages embedded within
// this one.
//
// This implementation simply delegates to the same method of the property
// page's *IPropertyPageSite*.
//
// Parameters:
//   pMsg - [in] Pointer to the *MSG* structure to be processed.
//
// Return Value: The return value of the delegated method call.
//
// See Also: Embedded Property Page Table Macros
template <class T>
STDMETHODIMP TCPropertyPageImpl<T>::TranslateAccelerator(MSG* pMsg)
{
  // Delegate to this page's site
  ZAssert(NULL != m_pPageSite);
  return m_pPageSite->TranslateAccelerator(pMsg);
}


/////////////////////////////////////////////////////////////////////////////
// Group=IPropertyNotifySink Interface Methods

/////////////////////////////////////////////////////////////////////////////
// Description: Notifies a sink that a bindable property has changed.
//
// This implementation simply posts a message to the main window of the
// property page, to ensure that the property change notification is handled
// on the main thread, and to return more quickly to the object that fired
// the event.
//
// Parameters:
//   dispid - [in] Dispatch identifier of the property that changed, or
// *DISPID_UNKNOWN* if multiple properties have changed.
//
// Return Value: This method always returns *S_OK*.
//
// See Also: Property Page Field Update Macros,
// TCPropertyPageBase::OnChangedHandler,
// TCPropertyPageImpl<T>::OnRequestEdit, TCPropertyPageImpl::wm_OnChanged
template <class T>
STDMETHODIMP TCPropertyPageImpl<T>::OnChanged(DISPID dispid)
{
  // Get the derived class pointer
  T* pThis = static_cast<T*>(this);

  // IPropertyNotifySink should only be connected when window is active
  ZAssert((HWND)*pThis);

  // Post a message to handle the property change notification
  pThis->PostMessage(wm_OnChanged, dispid);

  // Indicate success
  return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
// Description: Notifies a sink that a requestedit property is about to
// change.
//
// Note: Currently, this implementation always returns *S_OK*, indicating
// that this page does not restrict the changing of any property specified.
//
// TODO: In the future, the specified /dispid/ should be found in a set of
// fields that are marked as dirty. This would assist in ensuring that the
// property does not get modified from more than one instance of the property
// page. This issue becomes even more complex due to the user interface
// interactions with a field. As soon as a user begins to change a field, the
// field should be marked 'dirty' so that another page instance cannot also
// edit the field. This also implies that the OnRequestEdit event is fired
// whenever a user attempt to change a field, which could be a somewhat slow
// process. Things that make you go hmmmmm.
//
// Parameters:
//   dispid - [in] Dispatch identifier of the property that is about to
// change or *DISPID_UNKNOWN* if multiple properties are about to change.
//
// Return Value: This method always returns *S_OK*, indicating that the
// specified property or properties are allowed to change.
//
// See Also: Property Page Field Update Macros,
// TCPropertyPageBase::OnChanged
template <class T>
STDMETHODIMP TCPropertyPageImpl<T>::OnRequestEdit(DISPID dispid)
{
  // TODO: Lookup the specified DISPID in the set of dirty field DISPIDs

  // Indicate success
  return S_OK;
}


/////////////////////////////////////////////////////////////////////////////
// Group=ITCPropertyFieldToggle Interface Methods

/////////////////////////////////////////////////////////////////////////////
// Description: To disable or enable fields.
//
// Parameters:
//   dispid - [in] Dispatch identifier of the field to enable or disable.
//   bEnable - [in] Enable or disable the field.
//
// Return Value: This method always returns *S_OK* if the call succeeds.
//
// See Also: 
// 
// 
template <class T>
STDMETHODIMP TCPropertyPageImpl<T>::put_EnableField(DISPID dispid, VARIANT_BOOL bEnable)
{
  // Get the derived class pointer
  T* pThis = static_cast<T*>(this);
  
  pThis->FieldToggle(dispid, true, !!bEnable);

  // Indicate success
  return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
// Description: To determine whether or not a field is disabled.
//
// Parameters:
//   dispid - [in] Dispatch identifier of the field to check.
//   pbEnable - [out, retval].
//
// Return Value: This method always returns *S_OK* if the call succeeds.
//
// See Also: 
// 
// 
template <class T>
STDMETHODIMP TCPropertyPageImpl<T>::get_EnableField(DISPID dispid, VARIANT_BOOL* pbEnable)
{
  // Get the derived class pointer
  T* pThis = static_cast<T*>(this);
  
  *pbEnable = pThis->GetFieldToggle(dispid, true);

  // Indicate success
  return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
// Description: To show or hide fields.
//
// Parameters:
//   dispid - [in] Dispatch identifier of the field to show or hide.
//   bEnable - [in] Show or hide the field.
//
// Return Value: This method always returns *S_OK* if the call succeeds.
//
// See Also: 
// 
// 
template <class T>
STDMETHODIMP TCPropertyPageImpl<T>::put_ShowField(DISPID dispid, VARIANT_BOOL bShow)
{
  // Get the derived class pointer
  T* pThis = static_cast<T*>(this);
  
  pThis->FieldToggle(dispid, false, !!bShow);

  // Indicate success
  return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
// Description: To determine whether or not a field is showing.
//
// Parameters:
//   dispid - [in] Dispatch identifier of the field to check.
//   pbEnable - [out, retval].
//
// Return Value: This method always returns *S_OK* if the call succeeds.
//
// See Also: 
// 
// 
template <class T>
STDMETHODIMP TCPropertyPageImpl<T>::get_ShowField(DISPID dispid, VARIANT_BOOL* pbShow)
{
  // Get the derived class pointer
  T* pThis = static_cast<T*>(this);
  
  *pbShow = pThis->GetFieldToggle(dispid, false);

  // Indicate success
  return S_OK;
}

/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// Group=Helpers

/////////////////////////////////////////////////////////////////////////////
// Description: To toggle fields (enable/disable or show/hide).
//
// Parameters:
//   dispid - [in] Dispatch identifier of the field to toggle.
//   bEnableDisable - [in] Is this a enable/disable or show/hide toggle.
//   bToggle - [in] Toggle flag. 
//
// Return Value: None.
//
// See Also: 
// 
// 
template <class T>
void TCPropertyPageImpl<T>::FieldToggle(DISPID dispid, bool bEnableDisable, bool bToggle)
{
  // Get the derived class pointer
  T* pThis = static_cast<T*>(this);

  // Get the table of supported interfaces
  const XPageFieldEntry* pTable = NULL;
  UINT nEntries = pThis->GetPageFieldTable(&pTable);

  // Loop thru each field entry
  for (UINT i = 0; i < nEntries; ++i)
  {
    // Get the next field entry
    const XPageFieldEntry& entry = pTable[i];

    if (entry.dispid == dispid)
    {
      // Enable/Disable the window
      HWND hwnd = GetDlgItem(entry.idCtrl);
      if (hwnd)
      {
        if (bEnableDisable)
          ::EnableWindow(hwnd, bToggle);
        else
          ::ShowWindow(hwnd, bToggle ? SW_SHOW : SW_HIDE);
      }
    }
  }
}

/////////////////////////////////////////////////////////////////////////////
// Description: Get the toggle of the field (enable/disable or show/hide).
//
// Parameters:
//   dispid - [in] Dispatch identifier of the field to toggle.
//   bEnableDisable - [in] Is this a enable/disable or show/hide toggle.
//
// Return Value: Toggle flag.
//
// See Also: 
// 
// 
template <class T>
bool TCPropertyPageImpl<T>::GetFieldToggle(DISPID dispid, bool bEnableDisable)
{
  // Get the derived class pointer
  T* pThis = static_cast<T*>(this);

  // Get the table of supported interfaces
  const XPageFieldEntry* pTable = NULL;
  UINT nEntries = pThis->GetPageFieldTable(&pTable);

  // Loop thru each field entry
  for (UINT i = 0; i < nEntries; ++i)
  {
    // Get the next field entry
    const XPageFieldEntry& entry = pTable[i];

    if (entry.dispid == dispid)
    {
      // Enable/Disable the window
      HWND hwnd = GetDlgItem(entry.idCtrl);
      if (hwnd)
      {
        if (bEnableDisable)
          return !!::IsWindowEnabled(hwnd);
        else
          return !!::IsWindowVisible(hwnd);
      }
    }
  }

  return true;
}

#endif //__PropertyPageImpl_h_
