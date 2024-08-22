/////////////////////////////////////////////////////////////////////////////
// AGCDBParams.h : Declaration of the CAGCDBParams class
//

#pragma once

#include <AGC.h>
#include "resource.h"


/////////////////////////////////////////////////////////////////////////////
// CAGCDBParams
class ATL_NO_VTABLE CAGCDBParams : 
  public IDispatchImpl<IAGCDBParams, &IID_IAGCDBParams, &LIBID_AGCLib>,
  public IPersistStreamInit,
  public IPersistPropertyBag,
  public AGCObjectSafetyImpl<CAGCDBParams>,
  public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CAGCDBParams, &CLSID_AGCDBParams>
{
// Declarations
public:
  DECLARE_REGISTRY_RESOURCEID(IDR_AGCDBParams)
  DECLARE_PROTECT_FINAL_CONSTRUCT()
  DECLARE_GET_CONTROLLING_UNKNOWN()

// Interface Map
public:
  BEGIN_COM_MAP(CAGCDBParams)
    COM_INTERFACE_ENTRY(IAGCDBParams)
    COM_INTERFACE_ENTRY(IDispatch)
    COM_INTERFACE_ENTRY(IPersistStreamInit)
    COM_INTERFACE_ENTRY(IPersistPropertyBag)
    COM_INTERFACE_ENTRY2(IPersistStream, IPersistStreamInit)
    COM_INTERFACE_ENTRY2(IPersist, IPersistStreamInit)
    COM_INTERFACE_ENTRY_AUTOAGGREGATE(IID_IMarshal, m_punkMBV.p,
      CLSID_TCMarshalByValue)
    COM_INTERFACE_ENTRY(IObjectSafety)
  END_COM_MAP()

// Category Map
public:
  BEGIN_CATEGORY_MAP(CAGCDBParams)
    IMPLEMENTED_CATEGORY(CATID_AGC)
    IMPLEMENTED_CATEGORY(CATID_SafeForScripting)
    IMPLEMENTED_CATEGORY(CATID_SafeForInitializing)
  END_CATEGORY_MAP()

// Construction / Destruction
public:
  CAGCDBParams() :
    m_bDirty(false)
  {
  }

// IAGCDBParams Interface Methods
public:
  STDMETHODIMP put_ConnectionString(BSTR bstr);
  STDMETHODIMP get_ConnectionString(BSTR* pbstr);
  STDMETHODIMP put_TableName(BSTR bstr);
  STDMETHODIMP get_TableName(BSTR* pbstr);

// IPersist Interface Methods
public:
  STDMETHODIMP GetClassID(CLSID* pClassID);

// IPersistStreamInit Interface Methods
public:
  STDMETHODIMP IsDirty();
  STDMETHODIMP Load(LPSTREAM pStm);
  STDMETHODIMP Save(LPSTREAM pStm, BOOL fClearDirty);
  STDMETHODIMP GetSizeMax(ULARGE_INTEGER* pCbSize);
  STDMETHODIMP InitNew( void);

// IPersistPropertyBag Interface Methods
public:
  STDMETHODIMP Load(IPropertyBag* pPropBag, IErrorLog* pErrorLog);
  STDMETHODIMP Save(IPropertyBag* pPropBag, BOOL fClearDirty,
    BOOL fSaveAllProperties);

// Types
protected:
  typedef TCObjectLock<CAGCDBParams> XLock;

// Data Members
protected:
  CComBSTR m_bstrConnectionString, m_bstrTableName;
  bool     m_bDirty : 1;
public:  
  CComPtr<IUnknown> m_punkMBV;
};


/////////////////////////////////////////////////////////////////////////////