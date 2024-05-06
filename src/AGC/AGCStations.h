#pragma once

/////////////////////////////////////////////////////////////////////////////
// AGCStations.h : Declaration of the CAGCStations class.
//

#include "resource.h"
#include "IAGCCollectionImpl.h"


/////////////////////////////////////////////////////////////////////////////
// CAGCStations
//
class ATL_NO_VTABLE CAGCStations :
  public IAGCCollectionImpl<CAGCStations, const StationListIGC, IAGCStations,
    IstationIGC, IAGCStation, &LIBID_AGCLib>,
  public CComCoClass<CAGCStations, &CLSID_AGCStations>
{
// Declarations
public:
  DECLARE_REGISTRY_RESOURCEID(IDR_AGCStations)
  DECLARE_PROTECT_FINAL_CONSTRUCT()

// Interface Map
public:
  BEGIN_COM_MAP(CAGCStations)
    COM_INTERFACE_ENTRY(IAGCStations)
    COM_INTERFACE_ENTRIES_IAGCCollectionImpl()
  END_COM_MAP()

// Category Map
public:
  BEGIN_CATEGORY_MAP(CAGCStations)
    IMPLEMENTED_CATEGORY(CATID_AGC)
  END_CATEGORY_MAP()
};


/////////////////////////////////////////////////////////////////////////////