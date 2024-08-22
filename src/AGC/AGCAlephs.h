#pragma once

/////////////////////////////////////////////////////////////////////////////
// AGCAlephs.h : Declaration of the CAGCAlephs class.
//

#include "resource.h"
#include "IAGCCollectionImpl.h"


/////////////////////////////////////////////////////////////////////////////
// CAGCAlephs
//
class ATL_NO_VTABLE CAGCAlephs :
  public IAGCCollectionImpl<CAGCAlephs, const WarpListIGC, IAGCAlephs,
    IwarpIGC, IAGCAleph, &LIBID_AGCLib>,
  public CComCoClass<CAGCAlephs, &CLSID_AGCAlephs>
{
// Declarations
public:
  DECLARE_REGISTRY_RESOURCEID(IDR_AGCAlephs)
  DECLARE_PROTECT_FINAL_CONSTRUCT()

// Interface Map
public:
  BEGIN_COM_MAP(CAGCAlephs)
    COM_INTERFACE_ENTRY(IAGCAlephs)
    COM_INTERFACE_ENTRIES_IAGCCollectionImpl()
  END_COM_MAP()

// Category Map
public:
  BEGIN_CATEGORY_MAP(CAGCAlephs)
    IMPLEMENTED_CATEGORY(CATID_AGC)
  END_CATEGORY_MAP()
};


/////////////////////////////////////////////////////////////////////////////