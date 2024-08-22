#pragma once

/////////////////////////////////////////////////////////////////////////////
// AGCShips.h : Declaration of the CAGCShips class.
//

#include "resource.h"
#include "IAGCCollectionImpl.h"


/////////////////////////////////////////////////////////////////////////////
// CAGCShips
//
class ATL_NO_VTABLE CAGCShips :
  public IAGCCollectionImpl<CAGCShips, const ShipListIGC, IAGCShips,
    IshipIGC, IAGCShip, &LIBID_AGCLib>,
  public CComCoClass<CAGCShips, &CLSID_AGCShips>
{
// Declarations
public:
  DECLARE_REGISTRY_RESOURCEID(IDR_AGCShips)
  DECLARE_PROTECT_FINAL_CONSTRUCT()

// Interface Map
public:
  BEGIN_COM_MAP(CAGCShips)
    COM_INTERFACE_ENTRY(IAGCShips)
    COM_INTERFACE_ENTRIES_IAGCCollectionImpl()
  END_COM_MAP()

// Category Map
public:
  BEGIN_CATEGORY_MAP(CAGCShips)
    IMPLEMENTED_CATEGORY(CATID_AGC)
  END_CATEGORY_MAP()
};


/////////////////////////////////////////////////////////////////////////////