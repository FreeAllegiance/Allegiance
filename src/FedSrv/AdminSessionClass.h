#pragma once


/////////////////////////////////////////////////////////////////////////////
//
class ATL_NO_VTABLE CAdminSessionClass : 
  public CComClassFactory,
  public IAdminSessionClass
{
// Interface Map
public:
  BEGIN_COM_MAP(CAdminSessionClass)
    COM_INTERFACE_ENTRY(IAdminSessionClass)
		COM_INTERFACE_ENTRY_CHAIN(CComClassFactory)
  END_COM_MAP()

// Implementation
protected:
  static bool IsEqualBSTR(BSTR bstr1, BSTR bstr2);

// IClassFactory Interface Methods
public:
  STDMETHODIMP CreateInstance(IUnknown* pUnkOuter, REFIID riid, void** ppvObject);

// IAdminSessionClass Interface Methods
public:
  STDMETHODIMP CreateSession(IAdminSessionHost* pHost, IAdminSession** ppSession);
};


/////////////////////////////////////////////////////////////////////////////