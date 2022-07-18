#ifndef __IAGCStationImpl_h__
#define __IAGCStationImpl_h__

/////////////////////////////////////////////////////////////////////////////
// IAGCStationImpl.h : Declaration of the IAGCStationImpl class template.
//

#include "IAGCScannerImpl.h"


/////////////////////////////////////////////////////////////////////////////
// Interface Map Macro
//
// Classes derived from IAGCStationImpl should include this macro in their
// interface maps.
//
#define COM_INTERFACE_ENTRIES_IAGCStationImpl()                             \
    COM_INTERFACE_ENTRY(IAGCStation)                                        \
    COM_INTERFACE_ENTRY(IDispatch)                                          \
    COM_INTERFACE_ENTRIES_IAGCScannerImpl()


/////////////////////////////////////////////////////////////////////////////
// IAGCStationImpl
//
template <class T, class IGC, class ITF, const GUID* plibid>
class ATL_NO_VTABLE IAGCStationImpl : 
  public IAGCScannerImpl<T, IGC, ITF, plibid, IstationIGC, IAGCStation>
{
// Types
public:
  typedef IAGCStationImpl<T, IGC, ITF, plibid> IAGCStationImplBase;

// IAGCStation Interface Methods
public:
  STDMETHODIMP put_ShieldFraction(float Val)
  {
    ZAssert(GetIGC());
    GetIGC()->SetShieldFraction(Val);
    return S_OK;
  }

  STDMETHODIMP get_ShieldFraction(float* pVal)
  {
    ZAssert(GetIGC());
    CLEAROUT(pVal, GetIGC()->GetShieldFraction());
    return S_OK;
  }
};


/////////////////////////////////////////////////////////////////////////////

#endif //__IAGCStationImpl_h__
