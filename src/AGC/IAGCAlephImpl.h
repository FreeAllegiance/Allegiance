#ifndef __IAGCAlephImpl_h__
#define __IAGCAlephImpl_h__

/////////////////////////////////////////////////////////////////////////////
// IAGCAlephImpl.h : Declaration of the IAGCAlephImpl class template.
//

#include "IAGCModelImpl.h"


/////////////////////////////////////////////////////////////////////////////
// Interface Map Macro
//
// Classes derived from IAGCAlephImpl should include this macro in their
// interface maps.
//
#define COM_INTERFACE_ENTRIES_IAGCAlephImpl()                               \
    COM_INTERFACE_ENTRY(IAGCAleph)                                          \
    COM_INTERFACE_ENTRY(IDispatch)                                          \
    COM_INTERFACE_ENTRIES_IAGCModelImpl()


/////////////////////////////////////////////////////////////////////////////
// IAGCAlephImpl
//
template <class T, class IGC, class ITF, const GUID* plibid>
class ATL_NO_VTABLE IAGCAlephImpl :
  public IAGCModelImpl<T, IGC, ITF, plibid, IwarpIGC, IAGCAleph>
{
// Types
public:
  typedef IAGCAlephImpl<T, IGC, ITF, plibid> IAGCAlephImplBase;

// IAGCAleph Interface Methods
public:
  STDMETHODIMP get_Destination(IAGCAleph** ppDestination)
  {
    ZAssert(GetIGC());
    ZAssert(GetIGC()->GetDestination());
    return GetAGCGlobal()->GetAGCObject(GetIGC()->GetDestination(),
      IID_IAGCAleph, (void**)ppDestination);
  }
};


/////////////////////////////////////////////////////////////////////////////

#endif //__IAGCAlephImpl_h__
