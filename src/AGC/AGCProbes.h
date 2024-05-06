#pragma once

/////////////////////////////////////////////////////////////////////////////
// AGCProbes.h : Declaration of the CAGCProbes class.
//

#include "resource.h"
#include "IAGCCollectionImpl.h"


/////////////////////////////////////////////////////////////////////////////
// CAGCProbes
//
class ATL_NO_VTABLE CAGCProbes :
  public IAGCCollectionImpl<CAGCProbes, const ProbeListIGC, IAGCProbes,
    IprobeIGC, IAGCProbe, &LIBID_AGCLib>,
  public CComCoClass<CAGCProbes, &CLSID_AGCProbes>
{
// Declarations
public:
  DECLARE_REGISTRY_RESOURCEID(IDR_AGCProbes)
  DECLARE_PROTECT_FINAL_CONSTRUCT()

// Interface Map
public:
  BEGIN_COM_MAP(CAGCProbes)
    COM_INTERFACE_ENTRY(IAGCProbes)
    COM_INTERFACE_ENTRIES_IAGCCollectionImpl()
  END_COM_MAP()

// Category Map
public:
  BEGIN_CATEGORY_MAP(CAGCProbes)
    IMPLEMENTED_CATEGORY(CATID_AGC)
  END_CATEGORY_MAP()
};


/////////////////////////////////////////////////////////////////////////////