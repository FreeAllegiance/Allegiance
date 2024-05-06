/////////////////////////////////////////////////////////////////////////////
// AGCCommand.h : Declaration of the CAGCCommand class
//

#pragma once

#include <AGC.h>
#include "resource.h"


/////////////////////////////////////////////////////////////////////////////
// CAGCCommand
class ATL_NO_VTABLE CAGCCommand : 
  public IDispatchImpl<IAGCCommand, &IID_IAGCCommand, &LIBID_AGCLib>,
  public IAGCCommandPrivate,
  public IPersistStreamInit,
  public IPersistPropertyBag,
  public AGCObjectSafetyImpl<CAGCCommand>,
  public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CAGCCommand, &CLSID_AGCCommand>
{
// Declarations
public:
  DECLARE_REGISTRY_RESOURCEID(IDR_AGCCommand)
  DECLARE_PROTECT_FINAL_CONSTRUCT()
  DECLARE_GET_CONTROLLING_UNKNOWN()

// Interface Map
public:
  BEGIN_COM_MAP(CAGCCommand)
    COM_INTERFACE_ENTRY(IAGCCommand)
    COM_INTERFACE_ENTRY(IDispatch)
    COM_INTERFACE_ENTRY(IAGCCommandPrivate)
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
  BEGIN_CATEGORY_MAP(CAGCCommand)
    IMPLEMENTED_CATEGORY(CATID_AGC)
    IMPLEMENTED_CATEGORY(CATID_SafeForScripting)
    IMPLEMENTED_CATEGORY(CATID_SafeForInitializing)
  END_CATEGORY_MAP()

// Construction / Destruction
public:
  CAGCCommand() :
    m_bDirty(false)
  {
  }

// IAGCCommand Interface Methods
public:
  STDMETHODIMP get_Target(BSTR* pbstrTarget);
  STDMETHODIMP get_Verb(BSTR* pbstrVerb);
  STDMETHODIMP get_Text(BSTR* pbstrText);

// IAGCCommandPrivate Interface Methods
public:
  STDMETHODIMP_(void) Init(LPCSTR pszTarget, LPCSTR pszVerb);

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
  typedef TCObjectLock<CAGCCommand> XLock;

// Data Members
protected:
  CComBSTR m_bstrTarget, m_bstrVerb;
  bool     m_bDirty : 1;
public:  
  CComPtr<IUnknown> m_punkMBV;
};


/////////////////////////////////////////////////////////////////////////////