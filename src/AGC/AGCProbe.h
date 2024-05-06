#pragma once

/////////////////////////////////////////////////////////////////////////////
// AGCProbe.h : Declaration of the CAGCProbe class.
//

#include "resource.h"
#include "IAGCProbeImpl.h"


/////////////////////////////////////////////////////////////////////////////
// CAGCProbe
//
class ATL_NO_VTABLE CAGCProbe : 
  public IAGCProbeImpl<CAGCProbe, IprobeIGC, IAGCProbe, &LIBID_AGCLib>,
  public ISupportErrorInfo,
  public CComCoClass<CAGCProbe, &CLSID_AGCProbe>
{
// Declarations
public:
  DECLARE_REGISTRY_RESOURCEID(IDR_AGCProbe)
  DECLARE_PROTECT_FINAL_CONSTRUCT()

// Interface Map
public:
  BEGIN_COM_MAP(CAGCProbe)
    COM_INTERFACE_ENTRY(ISupportErrorInfo)
    COM_INTERFACE_ENTRIES_IAGCProbeImpl()
  END_COM_MAP()

// Category Map
public:
  BEGIN_CATEGORY_MAP(CAGCProbe)
    IMPLEMENTED_CATEGORY(CATID_AGC)
  END_CATEGORY_MAP()

// ISupportsErrorInfo Interface Methods
public:
  STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);
};


/////////////////////////////////////////////////////////////////////////////