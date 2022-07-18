#ifndef __IAGCBuyableImpl_h__
#define __IAGCBuyableImpl_h__

/////////////////////////////////////////////////////////////////////////////
// IAGCBuyableImpl.h : Declaration of the IAGCBuyableImpl class template.
//

#include "IAGCBaseImpl.h"


/////////////////////////////////////////////////////////////////////////////
// Interface Map Macro
//
// Classes derived from IAGCBuyableImpl should include this macro in their
// interface maps.
//
#define COM_INTERFACE_ENTRIES_IAGCBuyableImpl()                             \
    COM_INTERFACE_ENTRY(IAGCBuyable)                                        \
    COM_INTERFACE_ENTRY(IDispatch)                                          \
    COM_INTERFACE_ENTRIES_IAGCBaseImpl()


/////////////////////////////////////////////////////////////////////////////
// IAGCBuyableImpl
//
template <class T, class IGC, class ITF, const GUID* plibid, class AGCIGC = IGC, class AGCITF = ITF>
class ATL_NO_VTABLE IAGCBuyableImpl :
  public IAGCBaseImpl<T, IGC, ITF, plibid, AGCIGC, AGCITF>
{
// Types
public:
  typedef IAGCBuyableImpl<T, IGC, ITF, plibid, AGCIGC, AGCITF> IAGCBuyableImplBase;

// IAGCBuyable Interface Methods
public:
  STDMETHODIMP get_Name(BSTR* pbstrName)
  {
    ZAssert(GetIGC());
    CLEAROUT(pbstrName, (BSTR)NULL);
    *pbstrName = CComBSTR(GetIGC()->GetName()).Detach();
    return S_OK;
  }

  STDMETHODIMP get_Description(BSTR* pbstrDescription)
  {
    ZAssert(GetIGC());
    CLEAROUT(pbstrDescription, (BSTR)NULL);
    *pbstrDescription = CComBSTR(GetIGC()->GetDescription()).Detach();
    return S_OK;
  }

  STDMETHODIMP get_ModelName(BSTR* pbstrModelName)
  {
    ZAssert(GetIGC());
    CLEAROUT(pbstrModelName, (BSTR)NULL);
    *pbstrModelName = CComBSTR(GetIGC()->GetModelName()).Detach();
    return S_OK;
  }

  STDMETHODIMP get_Price(AGCMoney* pmoneyPrice)
  {
    ZAssert(GetIGC());
    CLEAROUT(pmoneyPrice, (AGCMoney)GetIGC()->GetPrice());
    return S_OK;
  }

  STDMETHODIMP get_TimeToBuild(long* pnTimeToBuild)
  {
    ZAssert(GetIGC());
    CLEAROUT(pnTimeToBuild, (long)GetIGC()->GetTimeToBuild());
    return S_OK;
  }
};


/////////////////////////////////////////////////////////////////////////////

#endif //__IAGCBuyableImpl_h__
