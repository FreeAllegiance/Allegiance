#pragma once

/////////////////////////////////////////////////////////////////////////////
// AGCStation.h : Declaration of the CAGCStation class.
//

#include "resource.h"
#include "IAGCStationImpl.h"


/////////////////////////////////////////////////////////////////////////////
// CAGCStation
//
class ATL_NO_VTABLE CAGCStation : 
  public IAGCStationImpl<CAGCStation, IstationIGC, IAGCStation, &LIBID_AGCLib>,
  public ISupportErrorInfo,
  public CComCoClass<CAGCStation, &CLSID_AGCStation>
{
// Declarations
public:
  DECLARE_REGISTRY_RESOURCEID(IDR_AGCStation)
  DECLARE_PROTECT_FINAL_CONSTRUCT()

// Interface Map
public:
  BEGIN_COM_MAP(CAGCStation)
    COM_INTERFACE_ENTRIES_IAGCStationImpl()
    COM_INTERFACE_ENTRY(ISupportErrorInfo)
  END_COM_MAP()

// Category Map
public:
  BEGIN_CATEGORY_MAP(CAGCStation)
    IMPLEMENTED_CATEGORY(CATID_AGC)
  END_CATEGORY_MAP()

// ISupportsErrorInfo Interface Methods
public:
  STDMETHODIMP InterfaceSupportsErrorInfo(REFIID riid);
};


/////////////////////////////////////////////////////////////////////////////