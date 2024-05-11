#pragma once

/////////////////////////////////////////////////////////////////////////////
// TCMarshalByValue.h | Declaration of the CTCMarshalByValue, which
// implements the CLSID_TCMarshalByValue component object.

#include "resource.h"       // main symbols
#include "SrcInc.h"
#include <TCObj.h>

#include <..\TCLib\ObjectLock.h>


/////////////////////////////////////////////////////////////////////////////
// CTCMarshalByValue

class ATL_NO_VTABLE CTCMarshalByValue :
  public IMarshal,
  public CComCoClass<CTCMarshalByValue, &CLSID_TCMarshalByValue>,
  public CComObjectRoot
{
// Declarations
public:
  DECLARE_REGISTRY_RESOURCEID(IDR_TCMarshalByValue)
  DECLARE_ONLY_AGGREGATABLE(CTCMarshalByValue)

// Category Map
public:
  BEGIN_CATEGORY_MAP(CTCMarshalByValue)
    IMPLEMENTED_CATEGORY(CATID_TCObj)
  END_CATEGORY_MAP()

// Interface Map
public:
  BEGIN_COM_MAP(CTCMarshalByValue)
    COM_INTERFACE_ENTRY(IMarshal)
  END_COM_MAP()

// Construction / Destruction
public:
  CTCMarshalByValue();
  #if defined(_DEBUG) && defined(CTCMarshalByValue_DEBUG)
    HRESULT CTCMarshalByValue::FinalConstruct();
    void CTCMarshalByValue::FinalRelease();
  #endif // _DEBUG

// IMarshal Interface Methods
public:
  STDMETHODIMP GetUnmarshalClass(REFIID riid, void* pv, DWORD dwDestContext,
    void* pvDestContext, DWORD mshlflags, CLSID* pCid);
  STDMETHODIMP GetMarshalSizeMax(REFIID riid, void* pv, DWORD dwDestContext,
    void* pvDestContext, DWORD mshlflags, DWORD* pSize);
  STDMETHODIMP MarshalInterface(IStream* pStm, REFIID riid, void* pv,
    DWORD dwDestContext, void* pvDestContext, DWORD mshlflags);
  STDMETHODIMP UnmarshalInterface(IStream* pStm, REFIID riid, void** ppv);
  STDMETHODIMP ReleaseMarshalData(IStream* pStm);
  STDMETHODIMP DisconnectObject(DWORD dwReserved);

// Implementation
protected:
  HRESULT GetOuterPersistStream(IPersistStream** pps);

// Types
protected:
  typedef TCObjectLock<CTCMarshalByValue> CLock;

// Data Members
protected:
  DWORD m_dwEndian;
  const DWORD m_dwEndianOriginal, m_dwEndianInverted;
};


/////////////////////////////////////////////////////////////////////////////
// Implementation

inline HRESULT CTCMarshalByValue::GetOuterPersistStream(IPersistStream** pps)
{
  HRESULT hr = OuterQueryInterface(IID_IPersistStream, (void**)pps);
  if (SUCCEEDED(hr))
    return hr;
  return OuterQueryInterface(IID_IPersistStreamInit, (void**)pps);
}


/////////////////////////////////////////////////////////////////////////////