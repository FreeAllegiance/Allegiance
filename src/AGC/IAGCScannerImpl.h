#ifndef __IAGCScannerImpl_h__
#define __IAGCScannerImpl_h__

/////////////////////////////////////////////////////////////////////////////
// IAGCScannerImpl.h : Declaration of the IAGCScannerImpl class template.
//

#include "IAGCDamageImpl.h"


/////////////////////////////////////////////////////////////////////////////
// Interface Map Macro
//
// Classes derived from IAGCScannerImpl should include this macro in their
// interface maps.
//
#define COM_INTERFACE_ENTRIES_IAGCScannerImpl()                             \
    COM_INTERFACE_ENTRY(IAGCScanner)                                        \
    COM_INTERFACE_ENTRY(IDispatch)                                          \
    COM_INTERFACE_ENTRIES_IAGCDamageImpl()


/////////////////////////////////////////////////////////////////////////////
// IAGCScannerImpl
//
template <class T, class IGC, class ITF, const GUID* plibid, class AGCIGC = IGC, class AGCITF = ITF>
class ATL_NO_VTABLE IAGCScannerImpl : 
  public IAGCDamageImpl<T, IGC, ITF, plibid, AGCIGC, AGCITF>
{
// Types
public:
  typedef IAGCScannerImpl<T, IGC, ITF, plibid, AGCIGC, AGCITF>
    IAGCScannerImplBase;

// IAGCScanner Interface Methods
public:
  STDMETHODIMP get_InScannerRange(IAGCModel* pModel, VARIANT_BOOL* pbInRange)
  {
    ZAssert(GetIGC());

    IAGCPrivatePtr spPrivate(pModel);
    if (NULL == spPrivate)
      return E_INVALIDARG;
      
    CLEAROUT(pbInRange,
      VARBOOL(GetIGC()->InScannerRange((ImodelIGC*)spPrivate->GetIGCVoid())));
    return S_OK;
  }
  STDMETHODIMP get_CanSee(IAGCModel* pModel, VARIANT_BOOL* pbCanSee)
  {
    ZAssert(GetIGC());

    IAGCPrivatePtr spPrivate(pModel);
    if (NULL == spPrivate)
      return E_INVALIDARG;
      
    CLEAROUT(pbCanSee,
      VARBOOL(GetIGC()->CanSee((ImodelIGC*)spPrivate->GetIGCVoid())));
    return S_OK;
  }
};


/////////////////////////////////////////////////////////////////////////////

#endif //__IAGCScannerImpl_h__
