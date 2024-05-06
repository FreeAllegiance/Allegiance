#pragma once

/////////////////////////////////////////////////////////////////////////////
// TCUtility.h | Declaration of the CTCUtility class, which implements the
// CLSID_TCUtility component object.

#include "resource.h"       // main symbols
#include <AGC.h>
#include <objsafe.h>


/////////////////////////////////////////////////////////////////////////////
// CTCUtility

class ATL_NO_VTABLE CTCUtility :
  public IDispatchImpl<ITCUtility, &IID_ITCUtility, &LIBID_AGCLib>,
  public CComCoClass<CTCUtility, &CLSID_TCUtility>,
  public CComObjectRoot
{
// Declarations
public:
  DECLARE_REGISTRY_RESOURCEID(IDR_TCUtility)
  DECLARE_GET_CONTROLLING_UNKNOWN()

// Category Map
public:
  BEGIN_CATEGORY_MAP(CTCUtility)
    IMPLEMENTED_CATEGORY(CATID_AGC)
    IMPLEMENTED_CATEGORY(CATID_SafeForScripting)
    IMPLEMENTED_CATEGORY(CATID_SafeForInitializing)
  END_CATEGORY_MAP()

// Interface Map
public:
  BEGIN_COM_MAP(CTCUtility)
    COM_INTERFACE_ENTRY(ITCUtility)
    COM_INTERFACE_ENTRY2(IDispatch, ITCUtility)
  END_COM_MAP()

// Construction / Destruction
public:
  CTCUtility() {}

// ITCUtility Interface Methods
public:
  STDMETHODIMP CreateObject(BSTR bstrProgID, BSTR bstrComputer,
    IUnknown** ppUnk);
  STDMETHODIMP ObjectReference(IUnknown* pUnk, BSTR* pbstrObjectReference);
  STDMETHODIMP Sleep(long nDurationMS);
  STDMETHODIMP GetObject(BSTR bstrMoniker, VARIANT_BOOL bAllowUI,
    long nTimeoutMS, IUnknown** ppUnk);
};


/////////////////////////////////////////////////////////////////////////////