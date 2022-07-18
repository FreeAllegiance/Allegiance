#ifndef __IAGCDamageImpl_h__
#define __IAGCDamageImpl_h__

/////////////////////////////////////////////////////////////////////////////
// IAGCDamageImpl.h : Declaration of the IAGCDamageImpl class template.
//

#include "IAGCModelImpl.h"


/////////////////////////////////////////////////////////////////////////////
// Interface Map Macro
//
// Classes derived from IAGCDamageImpl should include this macro in their
// interface maps.
//
#define COM_INTERFACE_ENTRIES_IAGCDamageImpl()                              \
    COM_INTERFACE_ENTRY(IAGCDamage)                                         \
    COM_INTERFACE_ENTRY(IDispatch)                                          \
    COM_INTERFACE_ENTRIES_IAGCModelImpl()


/////////////////////////////////////////////////////////////////////////////
// IAGCDamageImpl
//
template <class T, class IGC, class ITF, const GUID* plibid, class AGCIGC = IGC, class AGCITF = ITF>
class ATL_NO_VTABLE IAGCDamageImpl :
  public IAGCModelImpl<T, IGC, ITF, plibid, AGCIGC, AGCITF>
{
// Types
public:
  typedef IAGCDamageImpl<T, IGC, ITF, plibid, AGCIGC, AGCITF> IAGCDamageImplBase;

// IAGCDamage Interface Methods
public:
  STDMETHODIMP get_Fraction(float* pfFraction)
  {
    ZAssert(GetIGC());
    CLEAROUT(pfFraction, GetIGC()->GetFraction());
    return S_OK;
  }
};


/////////////////////////////////////////////////////////////////////////////

#endif //__IAGCDamageImpl_h__
