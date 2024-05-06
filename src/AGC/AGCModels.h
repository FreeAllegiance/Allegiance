#pragma once

/////////////////////////////////////////////////////////////////////////////
// AGCModels.h : Declaration of the CAGCModels class.
//

#include "resource.h"
#include "IAGCCollectionImpl.h"


/////////////////////////////////////////////////////////////////////////////
// CAGCModels
//
class ATL_NO_VTABLE CAGCModels :
  public IAGCCollectionImpl<CAGCModels, const ModelListIGC, IAGCModels,
    ImodelIGC, IAGCModel, &LIBID_AGCLib>,
  public CComCoClass<CAGCModels, &CLSID_AGCModels>
{
// Declarations
public:
  DECLARE_REGISTRY_RESOURCEID(IDR_AGCModels)
  DECLARE_PROTECT_FINAL_CONSTRUCT()

// Interface Map
public:
  BEGIN_COM_MAP(CAGCModels)
    COM_INTERFACE_ENTRY(IAGCModels)
    COM_INTERFACE_ENTRIES_IAGCCollectionImpl()
  END_COM_MAP()

// Category Map
public:
  BEGIN_CATEGORY_MAP(CAGCModels)
    IMPLEMENTED_CATEGORY(CATID_AGC)
  END_CATEGORY_MAP()
};


/////////////////////////////////////////////////////////////////////////////