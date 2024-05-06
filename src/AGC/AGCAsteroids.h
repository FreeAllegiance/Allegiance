#pragma once

/////////////////////////////////////////////////////////////////////////////
// AGCAsteroids.h : Declaration of the CAGCAsteroids class.
//

#include "resource.h"
#include "IAGCCollectionImpl.h"


/////////////////////////////////////////////////////////////////////////////
// CAGCAsteroids
//
class ATL_NO_VTABLE CAGCAsteroids :
  public IAGCCollectionImpl<CAGCAsteroids, const AsteroidListIGC, IAGCAsteroids,
    IasteroidIGC, IAGCAsteroid, &LIBID_AGCLib>,
  public CComCoClass<CAGCAsteroids, &CLSID_AGCAsteroids>
{
// Declarations
public:
  DECLARE_REGISTRY_RESOURCEID(IDR_AGCAsteroids)
  DECLARE_PROTECT_FINAL_CONSTRUCT()

// Interface Map
public:
  BEGIN_COM_MAP(CAGCAsteroids)
    COM_INTERFACE_ENTRY(IAGCAsteroids)
    COM_INTERFACE_ENTRIES_IAGCCollectionImpl()
  END_COM_MAP()

// Category Map
public:
  BEGIN_CATEGORY_MAP(CAGCAsteroids)
    IMPLEMENTED_CATEGORY(CATID_AGC)
  END_CATEGORY_MAP()
};


/////////////////////////////////////////////////////////////////////////////