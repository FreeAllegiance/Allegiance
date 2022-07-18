#ifndef __IAGCProbeImpl_h__
#define __IAGCProbeImpl_h__

/////////////////////////////////////////////////////////////////////////////
// IAGCProbeImpl.h : Declaration of the IAGCProbeImpl class template.
//

#include "IAGCScannerImpl.h"


/////////////////////////////////////////////////////////////////////////////
// Interface Map Macro
//
// Classes derived from IAGCProbeImpl should include this macro in their
// interface maps.
//
#define COM_INTERFACE_ENTRIES_IAGCProbeImpl()                               \
    COM_INTERFACE_ENTRY(IAGCProbe)                                          \
    COM_INTERFACE_ENTRY(IDispatch)                                          \
    COM_INTERFACE_ENTRIES_IAGCScannerImpl()


/////////////////////////////////////////////////////////////////////////////
// IAGCProbeImpl
//
template <class T, class IGC, class ITF, const GUID* plibid>
class ATL_NO_VTABLE IAGCProbeImpl :
  public IAGCScannerImpl<T, IGC, ITF, plibid, IprobeIGC, IAGCProbe>
{
// Types
public:
  typedef IAGCProbeImpl<T, IGC, ITF, plibid> IAGCProbeImplBase;

// IAGCProbe Interface Methods
public:
//  STDMETHODIMP get_ProjectileType(IAGCProjectileType** ppType)
//  {
//    // Not currently supported
//    return E_NOTIMPL;
//  }

  STDMETHODIMP get_EmissionPoint(IAGCVector** ppEmissionPoint)
  {
    ZAssert(GetIGC());

    // Create an instance of the AGCVector object
    return GetAGCGlobal()->MakeAGCVector(&GetIGC()->GetEmissionPt(),
      ppEmissionPoint);
  }

  STDMETHODIMP get_Lifespan(float* pfLifespan)
  {
    ZAssert(GetIGC());
    CLEAROUT(pfLifespan, GetIGC()->GetLifespan());
    return S_OK;
  }

  STDMETHODIMP get_DtBurst(float* pfDtBurst)
  {
    ZAssert(GetIGC());
    CLEAROUT(pfDtBurst, GetIGC()->GetDtBurst());
    return S_OK;
  }

  STDMETHODIMP get_Accuracy(float* pfAccuracy)
  {
    ZAssert(GetIGC());
    CLEAROUT(pfAccuracy, GetIGC()->GetAccuracy());
    return S_OK;
  }

  STDMETHODIMP get_IsRipcord(VARIANT_BOOL* pbIsRipcord)
  {
    ZAssert(GetIGC());
    CLEAROUT(pbIsRipcord, VARBOOL(GetIGC()->GetCanRipcord()));
    return S_OK;
  }
};


/////////////////////////////////////////////////////////////////////////////

#endif //__IAGCProbeImpl_h__
