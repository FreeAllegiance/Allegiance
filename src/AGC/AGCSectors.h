#pragma once

/////////////////////////////////////////////////////////////////////////////
// AGCSectors.h : Declaration of the CAGCSectors class.
//

#include "resource.h"
#include "IAGCCollectionImpl.h"


/////////////////////////////////////////////////////////////////////////////
// CAGCSectors
//
class ATL_NO_VTABLE CAGCSectors :
  public IAGCCollectionImpl<CAGCSectors, const ClusterListIGC, IAGCSectors,
    IclusterIGC, IAGCSector, &LIBID_AGCLib>,
  public CComCoClass<CAGCSectors, &CLSID_AGCSectors>
{
// Declarations
public:
  DECLARE_REGISTRY_RESOURCEID(IDR_AGCSectors)
  DECLARE_PROTECT_FINAL_CONSTRUCT()

// Interface Map
public:
  BEGIN_COM_MAP(CAGCSectors)
    COM_INTERFACE_ENTRY(IAGCSectors)
    COM_INTERFACE_ENTRIES_IAGCCollectionImpl()
  END_COM_MAP()

// Category Map
public:
  BEGIN_CATEGORY_MAP(CAGCSectors)
    IMPLEMENTED_CATEGORY(CATID_AGC)
  END_CATEGORY_MAP()
};


/////////////////////////////////////////////////////////////////////////////