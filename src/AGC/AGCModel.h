#pragma once

/////////////////////////////////////////////////////////////////////////////
// AGCModel.h : Declaration of the CAGCModel class.
//

#include "resource.h"
#include "IAGCModelImpl.h"


/////////////////////////////////////////////////////////////////////////////
// CAGCModel
//
class ATL_NO_VTABLE CAGCModel : 
  public IAGCModelImpl<CAGCModel, ImodelIGC, IAGCModel, &LIBID_AGCLib>,
  public ISupportErrorInfo,
  public CComCoClass<CAGCModel, &CLSID_AGCModel>
{
// Declarations
public:
  DECLARE_REGISTRY_RESOURCEID(IDR_AGCModel)
  DECLARE_PROTECT_FINAL_CONSTRUCT()

// Interface Map
public:
  BEGIN_COM_MAP(CAGCModel)
    COM_INTERFACE_ENTRY(ISupportErrorInfo)
    COM_INTERFACE_ENTRIES_IAGCModelImpl()
  END_COM_MAP()

// Category Map
public:
  BEGIN_CATEGORY_MAP(CAGCModel)
    IMPLEMENTED_CATEGORY(CATID_AGC)
  END_CATEGORY_MAP()

// ISupportsErrorInfo Interface Methods
public:
  STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);
};


/////////////////////////////////////////////////////////////////////////////