#ifndef __PropertyClass_h__
#define __PropertyClass_h__

/////////////////////////////////////////////////////////////////////////////
// PropertyClass.h | Declaration of the TCComPropertyClass template class.
//

#include <..\TCLib\ObjectLock.h>


/////////////////////////////////////////////////////////////////////////////
// TCComPropertyClassRoot

/////////////////////////////////////////////////////////////////////////////
// Description: Provides basic property persistence support.
//
// TCComPropertyClassRoot serves as the base class of TCComPropertyClass.
// Since that class is a template, the portions of functionality that are not
// affected by the template paramters are implemented in this class to avoid
// duplicated code. See TCComPropertyClass for more information.
//
// See Also: TCComPropertyClass, TCComPropertyClass::CLoadSaveScope,
// TCComPropertyPut, TCComPropertyGet
class ATL_NO_VTABLE TCComPropertyClassRoot
{
// Construction
public:
  #ifdef _DEBUG
    TCComPropertyClassRoot(LPCSTR pszType);
  #else // _DEBUG
    TCComPropertyClassRoot();
  #endif // _DEBUG

// Attributes
public:
  void SetDirty(BOOL bDirty);
  BOOL GetDirty() const;
  bool IsLoading();
  void SetLoading(bool bLoading);
  bool IsSaving();
  void SetSaving(bool bSaving);

// Implementation
public:
  HRESULT TCPersistStreamInit_Load(IStream* pStm, ATL_PROPMAP_ENTRY* pMap,
    void* pT, IUnknown* pUnk);
  HRESULT TCPersistStreamInit_Save(IStream* pStm, BOOL /* fClearDirty */,
    ATL_PROPMAP_ENTRY* pMap, void* pT, IUnknown* pUnk);
  HRESULT TCPersistPropertyBag_Load(IPropertyBag* pPropBag,
    LPERRORLOG pErrorLog, ATL_PROPMAP_ENTRY* pMap, void* pT, IUnknown* pUnk);
  HRESULT TCPersistPropertyBag_Save(IPropertyBag* pPropBag,
    BOOL fClearDirty, BOOL fSaveAllProperties, ATL_PROPMAP_ENTRY* pMap,
    void* pT, IUnknown* pUnk);
  HRESULT ISpecifyPropertyPages_GetPages(CAUUID* pPages,
    ATL_PROPMAP_ENTRY* pMap);

// Group=Types
protected:
  ///////////////////////////////////////////////////////////////////////////
  // Description: Nested class used to symetrically increment and decrement
  // an indicator of the object's save or load state.
  //
  // This class is used to symetrically increment and decrement an indicator
  // of an object's save or load state.
  //
  // See Also: TCComPropertyClassRoot
  class CLoadSaveScope
  {
  // Construction / Destruction
  public:
    CLoadSaveScope(TCComPropertyClassRoot* pObject, bool bLoading = true);
    ~CLoadSaveScope();

  // Group=Data Members
  protected:
    // The outer class instance on which this scoping object operates.
    TCComPropertyClassRoot* m_pObject;
    // Indicates whether this object is being used to enclose a load
    // operation, *true*, or a save operation, *false*.
    bool m_bLoading;
  };

// Group=Data Members
public:
  bool m_bRequiresSave:1; // The object's dirty flag.
protected:
#pragma pack(push, 4)
  long m_nLoadingRefs;  // The number of nested load operations in progress.
  long m_nSavingRefs;   // The number of nested save operations in progress.
#pragma pack(pop)

  ///////////////////////////////////////////////////////////////////////////
  // Description: A COM interface pointer of type *IDataAdviseHolder*.
  //
  // Provides a standard means to hold advisory connections between data
  // objects and advise sinks. (A data object is an object that can transfer
  // data and that implements *IDataObject*, whose methods specify the format
  // and transfer medium of the data.) 
  //
  // The interface m_spDataAdviseHolder implements the
  // *IDataObject::DAdvise* and *IDataObject::DUnadvise* methods to establish
  // and delete advisory connections to the object client. The object's
  // client must implement an advise sink by supporting the *IAdviseSink* „
  // interface.
  //
  // This data member is used by *ATL::IDataObjectImpl* to manage data
  // advisory connections.
  //
  // TODO: This is a carry-over from the *ATL::CComControl* code that was
  // copied when creating this class. To fully support the
  // *ATL::IDataObjectImpl* class, this class (or a derived class) would need
  // to implement an *IDataObject_GetData* method, which is called by the ATL
  // implementation of *IDataObject::GetData*.
  //
  // See Also: TCComPropertyClass::SendOnDataChange
  CComPtr<IDataAdviseHolder> m_spDataAdviseHolder;

  #ifdef _DEBUG
    // This is used under _DEBUG builds to point to the name of the
    // most-derived class, used for diagnostic purposes.
    LPCSTR m_pszType;
  #endif // _DEBUG
};


/////////////////////////////////////////////////////////////////////////////
// Group=Construction / Destruction

/////////////////////////////////////////////////////////////////////////////
// Parameters:
//   pObject - The outer object on which this scoping object operates.
//   bLoading - Indicates whether this object is being used to enclose a load
// operation, *true*, or a save operation, *false*.
//
// Remarks: Calls either the TCComPropertyClassRoot::SetLoading or the
// TCComPropertyClassRoot::SetSaving method with a parameter of *true*,
// which increments the reference count of /pObject/'s load or save
// operations, repectively.
//
// See Also: TCComPropertyClassRoot::CLoadSaveScope::destructor,
// TCComPropertyClassRoot, TCComPropertyClassRoot::SetLoading,
// TCComPropertyClassRoot::SetSaving,
inline TCComPropertyClassRoot::CLoadSaveScope::CLoadSaveScope(
  TCComPropertyClassRoot* pObject, bool bLoading) :
  m_pObject(pObject), m_bLoading(bLoading)
{
  if (m_pObject)
  {
    if (m_bLoading)
      m_pObject->SetLoading(true);
    else
      m_pObject->SetSaving(true);
  }
}

/////////////////////////////////////////////////////////////////////////////
// Calls either the TCComPropertyClassRoot::SetLoading or the
// TCComPropertyClassRoot::SetSaving method with a parameter of *false*,
// which decrements the reference count of /pObject/'s load or save
// operations, repectively.
//
// See Also: TCComPropertyClassRoot::CLoadSaveScope::constructor,
// TCComPropertyClassRoot, TCComPropertyClassRoot::SetLoading,
// TCComPropertyClassRoot::SetSaving,
inline TCComPropertyClassRoot::CLoadSaveScope::~CLoadSaveScope()
{
  if (m_pObject)
  {
    if (m_bLoading)
      m_pObject->SetLoading(false);
    else
      m_pObject->SetSaving(false);
  }
}


/////////////////////////////////////////////////////////////////////////////
// Group=


/////////////////////////////////////////////////////////////////////////////
// TCComFakeNotifySink declares a class that /looks/ like a connection point
// class for the *IPropertyNotifySink* interface, in that it declares the
// Fire_OnChanged and Fire_OnRequestEdit methods. However, these methods do
// nothing.
//
// This class is used just so that a derived class can call the
// *Fire_* methods, even when the class does not currently support the
// *IPropertyNotifySink* connection point. Primarily, this class is specified
// as the default /PNSCP/ template parameter of the TCComPropertyClass
// template class. A class derived from TCComPropertyClass should specify
// TCComPropertyNotifySinkCP for that parameter if it supports the
// *IPropertyNotifySink* connection point.
//
// See Also: TCComPropertyClass, TCComPropertyNotifySink
class TCComFakeNotifySink
{
public:
  void Fire_OnChanged(DISPID dispID)      {UNUSED(dispID);} // {secret}
  void Fire_OnRequestEdit(DISPID dispID)  {UNUSED(dispID);} // {secret}
};


/////////////////////////////////////////////////////////////////////////////
// TCComPropertyClass

/////////////////////////////////////////////////////////////////////////////
// Description: Provides basic property persistence support.
//
// The ATL framework defines a class, CComControl, that provides support for
// an object to have persistent properties through the ATL property map
// macros and the *IPersist* interfaces. However, to get this support,
// deriving from CComControl brings along a lot of overhead unnecessary for
// many simple component objects that are not full ActiveX controls. To
// alleviate this problem, the TCComPropertyClass provides the equivalent
// support for property persistence, but without the unnecessary overhead of
// CComControl.
//
// Include this class as a base class of component objects that use any of
// the ATL persistence implementations, including *IPersistStreamInitImpl*,
// *IPersistStorageImpl*, and *IPersistPropertyBagImpl*.
//
// Parameters:
//   T - Your class, derived from TCComPropertyClass.
//   PNSCP - The connection point class that is used to fire property change
// notifications. By default, this is TCComFakeNotifySink, which implements
// *Fire_OnChanged* and *Fire_OnRequestEdit* to do nothing. If a component
// object supports the *IPropertyNotifySink* connection point, this parameter
// should be TCComPropertyNotifySinkCP. The class specified by this parameter
// is used as a base class of TCComPropertyClass, and is therefore also
// inherited by /T/.
//
// See Also: TCPersistStreamInitImpl, TCComPropertyClassRoot,
// TCComPropertyPut, TCComPropertyGet, TCComFakeNotifySink,
// TCComPropertyNotifySinkCP
template <class T, class PNSCP = TCComFakeNotifySink>
class ATL_NO_VTABLE TCComPropertyClass :
  public TCComPropertyClassRoot,
  public PNSCP
{
// Construction / Destruction
public:
  TCComPropertyClass();

// Operations
public:
  HRESULT FireOnChanged(DISPID dispID);
  HRESULT FireOnRequestEdit(DISPID dispID);
  HRESULT SendOnDataChange(DWORD advf = 0);

// Overrides
public:
  virtual HRESULT ControlQueryInterface(const IID& iid, void** ppv);

// Group=Types
public:
  // Declares a type definition to allow derived classes an easier way to
  // refer to the base class.
  typedef TCComPropertyClass<T, PNSCP> TCComPropertyClassBase;
protected:
  // Declares an auto-scoping object used internally for thread
  // synchronization.
  typedef TCObjectLock<T> CLock;
};


/////////////////////////////////////////////////////////////////////////////
// TCComPropertyClass Construction

#ifdef _DEBUG
  ///////////////////////////////////////////////////////////////////////////
  // {group:Construction}
  // Constructs an instance of the class.
  template <class T, class PNSCP>
  inline TCComPropertyClass<T, PNSCP>::TCComPropertyClass()
    : TCComPropertyClassRoot(TCTypeName(T))
  {
  }
#else
  template <class T, class PNSCP>
  inline TCComPropertyClass<T, PNSCP>::TCComPropertyClass()
  {
  }
#endif // _DEBUG


/////////////////////////////////////////////////////////////////////////////
// Group=Operations

/////////////////////////////////////////////////////////////////////////////
// Description: Fires a property change notification to connected objects.
//
// This method calls the *Fire_OnChanged* method of the class specified by
// the /PNSCP/ template parameter.
//
// This method is provided for compatibility with the ATL CComControl class
// from which portions of this class were copied. It is used by the
// TCComPropertyPut function template when a property has changed.
//
// Parameters:
//   dispid - [in] Dispatch identifier of the property that changed, or
// *DISPID_UNKNOWN* if multiple properties have changed. 
//
// Return Value: Currently, *S_OK* is always returned.
//
// See Also: TCComPropertyPut, TCComFakeNotifySink, TCComPropertyNotifySinkCP
template <class T, class PNSCP>
inline HRESULT TCComPropertyClass<T, PNSCP>::FireOnChanged(DISPID dispID)
{
  // Delegate to the derived-class function
  static_cast<PNSCP*>(this)->Fire_OnChanged(dispID);
  return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
// Description: Fires a property change request to connected objects.
//
// This method calls the *Fire_OnRequestEdit* method of the class specified
// by the /PNSCP/ template parameter.
//
// This method is provided for compatibility with the ATL CComControl class
// from which portions of this class were copied. It is intended to be used
// by the TCComPropertyPut function template when a property is about to be
// changed. However, that function does not currently call this method, since
// it currently always returns *S_OK*.
//
// Parameters:
//   dispid - [in] Dispatch identifier of the property that is about to
// change or *DISPID_UNKNOWN* if multiple properties are about to change. 
//
// Return Value: Currently, *S_OK* is always returned.
//
// See Also: TCComPropertyPut, TCComFakeNotifySink, TCComPropertyNotifySinkCP
template <class T, class PNSCP>
inline HRESULT TCComPropertyClass<T, PNSCP>::FireOnRequestEdit(DISPID dispID)
{
  // Delegate to the derived-class function
  static_cast<PNSCP*>(this)->Fire_OnRequestEdit(dispID);
  return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
// Description: Called by the object to notify the connected advise sinks
// that data in the object has changed.
//
// Notifies all advisory sinks registered with the advise holder that the
// object data has changed. 
//
// TODO: This is a carry-over from the *ATL::CComControl* code that was
// copied when creating this class. To fully support the
// *ATL::IDataObjectImpl* class, this class (or a derived class) would need
// to implement an *IDataObject_GetData* method, which is called by the ATL
// implementation of *IDataObject::GetData*.
//
// Parameters:
//   advf - [in] Advise flags that specify how the call to
// *IAdviseSink::OnDataChange* is made. Values are from the
// *ADVF* enumeration. 
//
// See Also: TCComPropertyClassRoot::m_spDataAdviseHolder
template <class T, class PNSCP>
HRESULT TCComPropertyClass<T, PNSCP>::SendOnDataChange(DWORD advf)
{
  HRESULT hRes = S_OK;
  if (m_spDataAdviseHolder)
  {
    CComPtr<IDataObject> pdo;
    if (SUCCEEDED(ControlQueryInterface(IID_IDataObject, (void**)&pdo)))
      hRes = m_spDataAdviseHolder->SendOnDataChange(pdo, 0, advf);
  }
  return hRes;
}


/////////////////////////////////////////////////////////////////////////////
// Group=Overrides

/////////////////////////////////////////////////////////////////////////////
// TCComPropertyClass implements this by calling the _InternalQueryInterface
// method of the derived class, /T/.
template <class T, class PNSCP>
HRESULT TCComPropertyClass<T, PNSCP>::ControlQueryInterface(const IID& iid,
  void** ppv)
{
  T* pT = static_cast<T*>(this);
  return pT->_InternalQueryInterface(iid, ppv);
}


/////////////////////////////////////////////////////////////////////////////
// Group=


/////////////////////////////////////////////////////////////////////////////
// TCPersistStreamInitImpl extends ATL's IPersistStreamInitImpl class to
// provide an implementation of the *IPersistStreamInit::GetSizeMax* „
// interface method.
//
// Parameters:
//   T - The class derived from TCPersistStreamInitImpl.
//
template <class T>
class ATL_NO_VTABLE TCPersistStreamInitImpl :
  public IPersistStreamInit
{
// Group=Types
public:
  // Declares a type definition to allow derived classes an easier way to
  // refer to the base class.
  typedef TCPersistStreamInitImpl<T> TCPersistStreamInitImplBase;

// Group=IPersist Interface Methods
public:
  STDMETHODIMP GetClassID(CLSID *pClassID);

// Group=IPersistStream Interface Methods
public:
  STDMETHODIMP IsDirty();
  STDMETHODIMP Load(IStream* pStm);
  STDMETHODIMP Save(IStream* pStm, BOOL fClearDirty);
  STDMETHODIMP GetSizeMax(ULARGE_INTEGER* pcbSize);

// Group=IPersistStreamInit Interface Methods
public:
  STDMETHODIMP InitNew();
};


/////////////////////////////////////////////////////////////////////////////
// Group=IPersist Interface Methods

template <class T>
STDMETHODIMP TCPersistStreamInitImpl<T>::GetClassID(CLSID *pClassID)
{
  _TRACE0("TCPersistStreamInitImpl<T>::GetClassID\n");
  *pClassID = T::GetObjectCLSID();
  return S_OK;
}


/////////////////////////////////////////////////////////////////////////////
// Group=IPersistStream Interface Methods

template <class T>
STDMETHODIMP TCPersistStreamInitImpl<T>::IsDirty()
{
  _TRACE0("TCPersistStreamInitImpl<T>::IsDirty\n");
  T* pT = static_cast<T*>(this);
  return (pT->m_bRequiresSave) ? S_OK : S_FALSE;
}

template <class T>
STDMETHODIMP TCPersistStreamInitImpl<T>::Load(IStream* pStm)
{
  _TRACE0("TCPersistStreamInitImpl<T>::Load\n");
  T* pT = static_cast<T*>(this);
  return pT->TCPersistStreamInit_Load(pStm, (ATL_PROPMAP_ENTRY*) T::GetPropertyMap(), pT, // kg - cast to keep VC9 happy
    pT->GetUnknown());
}

template <class T>
STDMETHODIMP TCPersistStreamInitImpl<T>::Save(IStream* pStm, BOOL fClearDirty)
{
  T* pT = static_cast<T*>(this);
  _TRACE0("TCPersistStreamInitImpl<T>::Save\n");
  return pT->TCPersistStreamInit_Save(pStm, fClearDirty, (ATL_PROPMAP_ENTRY*) T::GetPropertyMap(),// kg - cast to keep VC9 happy
    pT, pT->GetUnknown());
}


/////////////////////////////////////////////////////////////////////////////
// Group=


/////////////////////////////////////////////////////////////////////////////
// Description: Returns the size, in bytes, of the stream needed to save the
// object.
//
// This function implements the *IPersistStreamInit::GetSizeMax* interface
// method by calling the TCGetPersistStreamSize global function.
//
// Parameters:
//   pCbSize - [out] Points to a 64-bit unsigned integer value indicating the
// size in bytes of the stream needed to save this object. 
//
// Return Value: *S_OK* indicates that the size was successfully returned. 
//
// See Also: TCNullStream, CTCNullStream
template <class T>
STDMETHODIMP TCPersistStreamInitImpl<T>::GetSizeMax(ULARGE_INTEGER* pcbSize)
{
  // Get the derived class pointer
  T* pThis = static_cast<T*>(this);

  // Determine the max size by writing to a null stream
  HRESULT hr = TCGetPersistStreamSize(pThis->GetUnknown(), pcbSize);

  // Display trace information under _DEBUG builds
  _TRACE_BEGIN
    _TRACE_PART1("TCPersistStreamInitImpl<%hs>::GetSizeMax():", TCTypeName(T));
    if (SUCCEEDED(hr))
      _TRACE_PART2("Counted 0x%08X (%d) bytes\n", pcbSize->LowPart, pcbSize->LowPart);
    else
      _TRACE_PART1("Failed, hr = 0x%08X\n", hr);
  _TRACE_END

  // Return the last HRESULT
  return hr;
}


/////////////////////////////////////////////////////////////////////////////
// Group=IPersistStreamInit Interface Methods

template <class T>
STDMETHODIMP TCPersistStreamInitImpl<T>::InitNew()
{
  _TRACE0("TCPersistStreamInitImpl<T>::InitNew\n");
  T* pT = static_cast<T*>(this);
  pT->SetDirty(false);
  return S_OK;
}


/////////////////////////////////////////////////////////////////////////////
// Group=

/////////////////////////////////////////////////////////////////////////////
// TCPersistPropertyBagImpl
//
//
// Parameters:
//   T - The class derived from TCPersistPropertyBagImpl
//
template <class T>
class ATL_NO_VTABLE TCPersistPropertyBagImpl :
  public IPersistPropertyBag
{
// Group=IPersist Interface Methods
public:
  STDMETHODIMP GetClassID(CLSID *pClassID);

// Group=IPersistPropertyBag Interface Methods
public:
  STDMETHODIMP InitNew();
  STDMETHODIMP Load(IPropertyBag* pPropBag, IErrorLog* pErrorLog);
  STDMETHODIMP Save(IPropertyBag* pPropBag, BOOL fClearDirty,
    BOOL fSaveAllProperties);
};


/////////////////////////////////////////////////////////////////////////////
// Group=IPersist Interface Methods

template <class T>
STDMETHODIMP TCPersistPropertyBagImpl<T>::GetClassID(CLSID *pClassID)
{
  _TRACE0("IPersistPropertyBagImpl::GetClassID\n");
  *pClassID = T::GetObjectCLSID();
  return S_OK;
}


/////////////////////////////////////////////////////////////////////////////
// Group=IPersistPropertyBag Interface Methods

template <class T>
STDMETHODIMP TCPersistPropertyBagImpl<T>::InitNew()
{
  _TRACE0("IPersistPropertyBagImpl::InitNew\n");
  T* pT = static_cast<T*>(this);
  pT->SetDirty(false);
  return S_OK;
}

template <class T>
STDMETHODIMP TCPersistPropertyBagImpl<T>::Load(IPropertyBag* pPropBag,
  IErrorLog* pErrorLog)
{
  _TRACE0("IPersistPropertyBagImpl::Load\n");
  T* pT = static_cast<T*>(this);
  ATL_PROPMAP_ENTRY* pMap = (ATL_PROPMAP_ENTRY*) T::GetPropertyMap(); // kg - cast to keep VC9 happy
  ATLASSERT(pMap != NULL);
  return pT->TCPersistPropertyBag_Load(pPropBag, pErrorLog, pMap, pT,
    pT->GetUnknown());
}

template <class T>
STDMETHODIMP TCPersistPropertyBagImpl<T>::Save(IPropertyBag* pPropBag,
  BOOL fClearDirty, BOOL fSaveAllProperties)
{
  _TRACE0("IPersistPropertyBagImpl::Save\n");
  T* pT = static_cast<T*>(this);
  ATL_PROPMAP_ENTRY* pMap = (ATL_PROPMAP_ENTRY*) T::GetPropertyMap(); // kg - cast to keep VC9 happy
  ATLASSERT(pMap != NULL);
  return pT->TCPersistPropertyBag_Save(pPropBag, fClearDirty,
    fSaveAllProperties, pMap, pT, pT->GetUnknown());
}


/////////////////////////////////////////////////////////////////////////////
// Group=


/////////////////////////////////////////////////////////////////////////////
// Property Exchange

/////////////////////////////////////////////////////////////////////////////
// Description: Compares two values.
//
// This template function is used by the TCComPropertyPut functions to
// compare the two values, /dest/ and /src/. The default template function
// uses the global operator== to perform the comparison. Template
// specializations, however, are provided for the following type pairs:
//
// + *bool* to *VARIANT_BOOL* „
// + *DATE* to *COleDateTime* (MFC Only)
// + *CComVariant* to *VARIANT* „
// + *CComBSTR* to *BSTR* „
//
// Parameters:
//   dest - A reference to the variable on the left-hand side of the
// comparison.
//   src - A reference to the variable on the right-hand side of the
// comparison.
//
// Return Value: *true* if the arguments are equal, otherwise *false*.
//
// See Also: TCComPropertyPut, TCComVariantCompare, TCComPropertyAssign,
// TCComPropertyInit
template <class Dest, class Src>
inline bool TCComPropertyCompare(Dest& dest, Src& src)
{
  return !!(dest == src);
}

/////////////////////////////////////////////////////////////////////////////
// Description: Initializes a value.
//
// This template function is used by the TCComPropertyPut
// and TCComPropertyGet functions to initialize a value to a known state. The
// default template function simply initializes the specified value to zero,
// type cast to the type of /dest./ A template specialization, however, is
// provided for the following type:
//
// + *VARIANT* - Initialized with the Win32 *VariantInit* API.
//
// Parameters:
//   dest - A reference to the variable to be initialized.
//
// See Also: TCComPropertyPut, TCComPropertyGet, TCComPropertyAssign,
// TCComPropertyCompare
template <class Dest>
inline void TCComPropertyInit(Dest& dest)
{
  dest = (Dest)0;
}

/////////////////////////////////////////////////////////////////////////////
// Description: Assigns the specified value to a variable.
//
// This template function is used by the TCComPropertyPut and
// TCComPropertyGet functions to assign a value to a variable. The default
// template function simply performs the assignment using the global
// assignment operator=. Template specializations, however, are provided for
// the following type pairs:
//
// + *bool* from *VARIANT_BOOL* „
// + *VARIANT_BOOL* from *bool* „
// + *VARIANT* from *VARIANT* - Uses the Win32 *VariantCopy* API.
// + *SAFEARRAY* * from *SAFEARRAY* * - Uses the Win32 *SafeArrayCopy* API.
// + *BSTR* from *BSTR* - Uses the Win32 *SysReAllocString* API.
// + *BSTR* from *CComBSTR* „
// + *VARIANT* from *COleVariant* (MFC only)
// + *VARIANT* from *COleSafeArray* (MFC only)
// + *VARIANT* from *CComVariant* „
//
// Parameters:
//   dest - A reference to the variable to be assigned the value of /src/.
//   src - A reference to the variable whose value will be assigned to
// /dest/.
//
// Return Value:
// *S_OK* if the function succeeded, otherwise an appropriated error code.
//
// See Also: TCComPropertyPut, TCComPropertyGet, TCComPropertyInit,
// TCComPropertyCompare
template <class Dest, class Src>
inline HRESULT TCComPropertyAssign(Dest& dest, Src& src)
{
  dest = src;
  return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
// Description: Provides a standard implementation for [propput] methods.
//
// Use this method when implementing the [propput] methods of a component
// object interface.
//
// Properties are declared in a component object interface as methods with
// the [propput] or [propget] IDL attributes. The resulting interface method
// is named by prefixing *put_* or *get_*, respectively, to the property
// name. Consider the following interface definition:
//
//      // IMyInterface Interface
//      [
//        object, dual, pointer_default(unique),
//        uuid(E4DE3C70-21B7-11d2-97D4-00A0C932A52F),
//        helpstring("IMyInterface example interface with a property.")
//      ]
//      interface IMyInterface : IDispatch
//      {
//        [propput, helpstring("Sets/gets the Enabled property.")]
//        HRESULT Enabled([in] VARIANT_BOOL bEnabled);
//        …
//        [propget, id(DISPID_ENABLED)]
//        HRESULT Enabled([out, retval] VARIANT_BOOL* pbEnabled);
//        …
//      }; // End: interface IMyInterface : IDispatch
// 
// In this example, the *IMyInterface* interface has one property, *Enabled*,
// that can be set [propput] and queried [propget]. The declarations for this
// property in a component object class that implements *IMyInterface* would
// be similar to the following:
//
//      // IMyInterface Interface Methods
//      public:
//        STDMETHODIMP put_Enabled(VARIANT_BOOL bEnabled);
//        STDMETHODIMP get_Enabled(VARIANT_BOOL* pbEnabled);
//
// In addition, the object would usually need to store the property as a data
// member:
//
//      // Data Members
//      protected:
//        bool m_bEnabled;
//
// Now, while implementing the *put_Enabled* and *get_Enabled* interface
// methods is certainly not rocket science, several considerations come to
// mind:
//
// + To synchronize access to the /m_bEnabled/ data member from multiple
// threads, the object's *Lock* method should be called prior to accessing
// the data member and the *Unlock* method called following.
// + When the *put_Enabled* method changes the /m_bEnabled/ data member, a
// property change notification should be fired, if the object supports the
// *IPropertyNotifySink* connection point. This event should be fired
// /after/ the object's *Unlock* method is called to avoid a possible
// deadlock situation in free threaded component objects.
// + If the /bEnabled/ parameter of *put_Enabled* specifies a value
// equivalent to the value stored in the /m_bEnabled/ data member, the
// object's dirty flag should not be set, nor should the object fire a
// property change notification.
// + While the *put_Enabled* and *get_Enabled* property methods have
// parameters of type *VARIANT_BOOL* to match the IDL definition, the
// /m_bEnabled/ data member is of type *bool*, which serves as a more natural
// (and possibly compact) representation in C++.
// + The /pbEnabled/ parameter of *get_Enabled* could be an invalid pointer.
// Enclosing the code that accesses it in an exception handler would make the
// object very robust since, instead of crashing, the method could return the
// standard error value, *E_POINTER*.
//
// Since these same considerations apply for the implementation of almost
// every interface property, the TCComPropertyPut and TCComPropertyGet
// template functions were developed to encapsulate this coding pattern. All
// of the considerations listed above are taken care of with the following
// implementation of the property methods:
//
//      // IMyInterface Interface Methods
//      …
//      STDMETHODIMP CMyClass::put_Enabled(VARIANT_BOOL bEnabled)
//      {
//        return TCComPropertyPut(this, m_bEnabled, bEnabled, DISPID_Enabled);
//      }
//      …
//      STDMETHODIMP CMyClass::get_Enabled(VARIANT_BOOL* pbEnabled)
//      {
//        return TCComPropertyGet(this, pbEnabled, m_bEnabled);
//      }
//
// The template functions rely heavily on the use of the TCComPropertyInit,
// TCComPropertyAssign, and TCComPropertyCompare function templates. This
// provides the flexibility to initialize, assign, and compare values of
// varied types through function template specializations. Each of these
// helper function templates have a default implementation that use either
// the assignment operator or the equality operator, which may be overloaded
// for a particular pair of types. Also, specializations are provided for
// these function templates that perform the appropriate action based on
// specific parameter types. For example, TCComPropertyAssign has the
// following template specialization to allow the assignment of a *bool* data
// member to a *VARIANT_BOOL* [out] parameter (as in the
// *get_Enabled* example above):
//
//      template <> inline
//      HRESULT TCComPropertyAssign<VARIANT_BOOL, bool>(VARIANT_BOOL& dest,
//        bool& src)
//      {
//        dest = (src ? VARIANT_TRUE : VARIANT_FALSE);
//        return S_OK;
//      }
//
// Parameters:
//   pThis - [in] A pointer to the object that owns the data member specified
// by the /dest/ parameter. This object must provide the following
// public methods: *Lock*, *Unlock*, and *SetDirty*. When the second form of
// this function is used, this object must also provide the *FireOnChanged* „
// public method. It is recommended that this class be derived from
// TCComPropertyClass, since it provides all of these methods.
//   dest - [in, out] A reference to the data member (of the class referenced
// by /pThis/) that is used to store the property value specified by /src/.
//   src - [in] A reference to the variable whose value is to be assigned to
// the data member specified by the /dest/ parameter. Typically, the [in]
// parameter of the [propput] method is specified here.
//   pbChanged - [out] A pointer to a *bool* variable that receives the
// outcome of this function. *true* indicates that /dest/ was not equal to
// /src/ so /dest/ was changed. *false* indicates that /dest/ was already
// equal to /src/ so it did not change (and the *SetDirty* method of
// /pThis/ was *not* called). This parameter can be *NULL*, indicating that
// the outcome is not needed by the caller.
//
// See Also: TCComPropertyGet, TCComPropertyClass, TCObjectLock,
// TCComPropertyCompare, TCComPropertyInit, TCComPropertyAssign
template <class T, class Dest, class Src>
HRESULT TCComPropertyPut(T* pThis, Dest& dest, Src& src,
  bool* pbChanged = NULL)
{
  ZAssert(NULL != pThis);

  bool bLocked = false;
  HRESULT hr = S_OK;
  __try
  {
    // Initialize the indicator, if specified
    if (NULL != pbChanged)
      *pbChanged = false;

    // lock the object
    pThis->Lock();
    bLocked = true;

    // Do nothing if new value is equal to the current value
    if (!TCComPropertyCompare(dest, src))
    {
      // Save the new value
      hr = TCComPropertyAssign(dest, src);
      if (SUCCEEDED(hr))
      {
        // Set the modified flag
        pThis->SetDirty(TRUE);

        // Indicate that the stored value changed
        if (NULL != pbChanged)
          *pbChanged = true;
      }
    }
  }
  __except(1)
  {
    _TRACE0("TCComPropertyPut(): Caught an unknown exception ");
    _TRACE0("(probably an invalid pointer was specified)");
    hr = E_POINTER;
  }

  // Unlock the object if we locked it
  if (bLocked)
    pThis->Unlock();

  // Return the last result
  return hr;
}

/////////////////////////////////////////////////////////////////////////////
// Parameters:
//   dispid - Specifies the dispatch identifier to be used in a call to
// *FireOnChanged*, which notifies objects connected to the
// *IPropertyNotifySink* connection point of the property change.
template <class T, class Dest, class Src>
HRESULT TCComPropertyPut(T* pThis, Dest& dest, Src& src, DISPID dispid,
  bool* pbChanged = NULL)
{
  __try
  {
    bool bChanged;
    HRESULT hr = TCComPropertyPut(pThis, dest, src, &bChanged);
    if (bChanged)
      pThis->FireOnChanged(dispid);
    if (pbChanged)
      *pbChanged = bChanged;
    return hr;
  }
  __except(1)
  {
    _TRACE0("TCComPropertyPut(): Caught an unknown exception\n");
    return E_UNEXPECTED;
  }
}


/////////////////////////////////////////////////////////////////////////////
// Description: Provides a standard implementation for [propget] methods.
//
// Use this method when implementing the [propget] methods of a component
// object interface.
//
// See TCComPropertyPut for information on how to use this function template.
//
// Parameters:
//   pThis - [in] A pointer to the object that owns the data member specified
// by the /src/ parameter. This object must provide the following
// public methods: *Lock* and *Unlock*. It is recommended that the class be
// derived from TCComPropertyClass, since it provides those methods.
//   pDest - [out] A pointer to the variable to be assigned the value of the
// data member specified by /src/. Typically, the [out, retval] parameter of
// the [propget] method is specified here.
//   src - [in] A reference to the data member (of the class referenced by
// /pThis/) whose value is assigned to the variable specified by /pDest/.
//   bLock - [in] Specifies if the assignment operation should be done while
// the object is locked. The default, *true*, indicates that the *Lock* method
// of the class referenced by /pThis/ will be called prior to the assignment,
// and *Unlock* will be called afterward. If *false*, the object will not be
// locked for the assignment.
//
// See Also: TCComPropertyPut, TCObjectLock, TCComPropertyInit,
// TCComPropertyAssign
template <class T, class Dest, class Src>
HRESULT TCComPropertyGet(T* pThis, Dest* pDest, Src& src, bool bLock = true)
{
  ZAssert(NULL != pThis);

  bool bLocked = false;
  HRESULT hr = S_OK;
  __try
  {
    // Lock the object
    if (bLock)
    {
      pThis->Lock();
      bLocked = true;
    }

    // Initialize the [out] parameter
    TCComPropertyInit(*pDest);

    // Copy the value to the [out] parameter
    hr = TCComPropertyAssign(*pDest, src);
  }
  __except(1)
  {
    _TRACE_BEGIN
      _TRACE_PART0("TCComPropertyGet(): Caught an unknown exception ");
      _TRACE_PART0("(probably an invalid pointer was specified)");
    _TRACE_END
    hr = E_POINTER;
  }

  // Unlock the object if we locked it
  if (bLocked)
    pThis->Unlock();

  // Return the last result
  return hr;
}


/////////////////////////////////////////////////////////////////////////////
// Special cases for conversion to/from bool and VARIANT_BOOL

template <> inline
bool TCComPropertyCompare<bool, VARIANT_BOOL>(bool& dest, VARIANT_BOOL& src)
{
  return dest == !!src;
}

template <> inline
HRESULT TCComPropertyAssign<bool, VARIANT_BOOL>(bool& dest, VARIANT_BOOL& src)
{
  dest = !!src;
  return S_OK;
}

template <> inline
HRESULT TCComPropertyAssign<VARIANT_BOOL, bool>(VARIANT_BOOL& dest, bool& src)
{
  dest = (src ? VARIANT_TRUE : VARIANT_FALSE);
  return S_OK;
}


/////////////////////////////////////////////////////////////////////////////
// Special cases for conversion to/from DATE types

#ifdef _AFX
  template <> inline
  bool TCComPropertyCompare<DATE, COleDateTime>(DATE& dest, COleDateTime& src)
  {
    COleDateTime destDate(dest);
    return (destDate == src)?true:false;
  }
#endif // _AFX


/////////////////////////////////////////////////////////////////////////////
// Special cases for conversion to/from VARIANT types

template <>
inline void TCComPropertyInit<VARIANT>(VARIANT& dest)
{
  VariantInit(&dest);
}

template <> inline
HRESULT TCComPropertyAssign<VARIANT, VARIANT>(VARIANT& dest, VARIANT& src)
{
  RETURN_FAILED(VariantCopy(&dest, const_cast<VARIANT*>(&src)));
  if (NULL == V_ARRAY(&dest) && (VT_ARRAY & V_VT(&dest)))
    VariantClear(&dest);
  return S_OK;
}

#ifdef _ATL
  bool TCComVariantCompare(CComVariant& dest, VARIANT& src);

  template <>
  inline bool TCComPropertyCompare<CComVariant, VARIANT>(CComVariant& dest,
    VARIANT& src)
  {
    return TCComVariantCompare(dest, src);
  }
#endif // _ATL


/////////////////////////////////////////////////////////////////////////////
// Special cases for conversion to/from SAFEARRAY types

template <>
inline HRESULT TCComPropertyAssign<SAFEARRAY*, SAFEARRAY*>(SAFEARRAY*& dest,
  SAFEARRAY*& src)
{
  return SafeArrayCopy(const_cast<SAFEARRAY*>(src), &dest);
}

/////////////////////////////////////////////////////////////////////////////
// Special cases for conversion to/from BSTR types

template <>
inline bool TCComPropertyCompare<CComBSTR, BSTR>(CComBSTR& dest, BSTR& src)
{
  if (NULL == BSTR(dest))
    return NULL == src;
  else
    return (NULL == src) ? false : !wcscmp(dest, src);
}

template <>
inline HRESULT TCComPropertyAssign<BSTR, BSTR>(BSTR& dest, BSTR& src)
{
  return SysReAllocString(&dest, src);
}

template <>
inline HRESULT TCComPropertyAssign<BSTR, CComBSTR>(BSTR& dest, CComBSTR& src)
{
  dest = src.Copy();
  return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
// Special cases for conversion to/from MFC types

#ifdef _AFX
  template <>
  inline HRESULT TCComPropertyAssign<VARIANT, COleVariant>(VARIANT& dest,
    COleVariant& src)
  {
    return TCComPropertyAssign(dest, VARIANT(src));
  }

  template <>
  inline HRESULT TCComPropertyAssign<VARIANT, COleSafeArray>(VARIANT& dest,
    COleSafeArray& src)
  {
    return TCComPropertyAssign(dest, VARIANT(src));
  }

#endif // _AFX


/////////////////////////////////////////////////////////////////////////////
// Special cases for conversion to/from built-in types

#ifdef _ATL
  template <>
  inline HRESULT TCComPropertyAssign<VARIANT, CComVariant>(VARIANT& dest,
    CComVariant& src)
  {
    return TCComPropertyAssign(dest, VARIANT(src));
  }
#endif // _ATL


/////////////////////////////////////////////////////////////////////////////

#endif // __MMACAtl_h__
