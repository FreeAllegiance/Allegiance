#pragma once

/////////////////////////////////////////////////////////////////////////////
// AGCAsteroid.h : Declaration of the CAGCAsteroid class.
//

#include "resource.h"
#include "IAGCAsteroidImpl.h"


/////////////////////////////////////////////////////////////////////////////
// CAGCAsteroid
//
class ATL_NO_VTABLE CAGCAsteroid : 
  public IAGCAsteroidImpl<CAGCAsteroid, IasteroidIGC, IAGCAsteroid, &LIBID_AGCLib>,
  public ISupportErrorInfo,
  public CComCoClass<CAGCAsteroid, &CLSID_AGCAsteroid>
{
// Declarations
public:
  DECLARE_REGISTRY_RESOURCEID(IDR_AGCAsteroid)
  DECLARE_PROTECT_FINAL_CONSTRUCT()

// Interface Map
public:
  BEGIN_COM_MAP(CAGCAsteroid)
    COM_INTERFACE_ENTRIES_IAGCAsteroidImpl()
    COM_INTERFACE_ENTRY(ISupportErrorInfo)
  END_COM_MAP()

// Category Map
public:
  BEGIN_CATEGORY_MAP(CAGCAsteroid)
    IMPLEMENTED_CATEGORY(CATID_AGC)
  END_CATEGORY_MAP()

// ISupportsErrorInfo Interface Methods
public:
  STDMETHODIMP InterfaceSupportsErrorInfo(REFIID riid);
};


/////////////////////////////////////////////////////////////////////////////