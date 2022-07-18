#ifndef __IAGCAsteroidImpl_h__
#define __IAGCAsteroidImpl_h__

/////////////////////////////////////////////////////////////////////////////
// IAGCAsteroidImpl.h : Declaration of the IAGCAsteroidImpl class template.
//

#include "IAGCDamageImpl.h"


/////////////////////////////////////////////////////////////////////////////
// Interface Map Macro
//
// Classes derived from IAGCAsteroidImpl should include this macro in their
// interface maps.
//
#define COM_INTERFACE_ENTRIES_IAGCAsteroidImpl()                            \
    COM_INTERFACE_ENTRY(IAGCAsteroid)                                       \
    COM_INTERFACE_ENTRY(IDispatch)                                          \
    COM_INTERFACE_ENTRIES_IAGCDamageImpl()


/////////////////////////////////////////////////////////////////////////////
// IAGCAsteroidImpl
//
template <class T, class IGC, class ITF, const GUID* plibid, class AGCIGC = IGC, class AGCITF = ITF>
class ATL_NO_VTABLE IAGCAsteroidImpl : 
  public IAGCDamageImpl<T, IGC, ITF, plibid, AGCIGC, AGCITF>
{
// Types
public:
  typedef IAGCAsteroidImpl<T, IGC, ITF, plibid, AGCIGC, AGCITF>
    IAGCAsteroidImplBase;

// IAGCAsteroid Interface Methods
public:
  STDMETHODIMP get_Ore(float* pfOre)
  {
    ZAssert(GetIGC());
    CLEAROUT(pfOre, GetIGC()->GetOre());
    return S_OK;
  }

  STDMETHODIMP get_Capabilities(AGCAsteroidAbilityBitMask* paabmCapabilities)
  {
    ZAssert(GetIGC());
    CLEAROUT(paabmCapabilities,
      (AGCAsteroidAbilityBitMask)GetIGC()->GetCapabilities());
    return S_OK;
  }

  STDMETHODIMP get_HasCapability(AGCAsteroidAbilityBitMask aabm,
    VARIANT_BOOL* pbHasCapability)
  {
    ZAssert(GetIGC());
    CLEAROUT(pbHasCapability,
      VARBOOL(GetIGC()->HasCapability((AsteroidAbilityBitMask)aabm)));
    return S_OK;
  }
};


/////////////////////////////////////////////////////////////////////////////

#endif //__IAGCAsteroidImpl_h__
