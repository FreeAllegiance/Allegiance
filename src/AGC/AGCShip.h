#pragma once

/////////////////////////////////////////////////////////////////////////////
// AGCShip.h : Declaration of the CAGCShip
//

#include "resource.h"
#include "IAGCShipImpl.h"


/////////////////////////////////////////////////////////////////////////////
// CAGCShip
//
class ATL_NO_VTABLE CAGCShip : 
  public IAGCShipImpl<CAGCShip, IshipIGC, IAGCShip, &LIBID_AGCLib>,
  public ISupportErrorInfo,
  public CComCoClass<CAGCShip, &CLSID_AGCShip>
{
// Declarations
public:
  DECLARE_REGISTRY_RESOURCEID(IDR_AGCShip)
  DECLARE_PROTECT_FINAL_CONSTRUCT()

// Interface Map
public:
  BEGIN_COM_MAP(CAGCShip)
    COM_INTERFACE_ENTRIES_IAGCShipImpl()
    COM_INTERFACE_ENTRY(ISupportErrorInfo)
  END_COM_MAP()

// Category Map
public:
  BEGIN_CATEGORY_MAP(CAGCShip)
    IMPLEMENTED_CATEGORY(CATID_AGC)
  END_CATEGORY_MAP()

// ISupportsErrorInfo Interface Methods
public:
  STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// TODO: add get_Station property
};


/////////////////////////////////////////////////////////////////////////////