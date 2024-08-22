#pragma once

/////////////////////////////////////////////////////////////////////////////
// AGCAleph.h : Declaration of the CAGCAleph
//

#include "resource.h"
#include "IAGCAlephImpl.h"


/////////////////////////////////////////////////////////////////////////////
// CAGCAleph
//
class ATL_NO_VTABLE CAGCAleph : 
  public IAGCAlephImpl<CAGCAleph, IwarpIGC, IAGCAleph, &LIBID_AGCLib>,
  public ISupportErrorInfo,
  public CComCoClass<CAGCAleph, &CLSID_AGCAleph>
{
// Declarations
public:
  DECLARE_REGISTRY_RESOURCEID(IDR_AGCAleph)
  DECLARE_PROTECT_FINAL_CONSTRUCT()

// Interface Map
public:
  BEGIN_COM_MAP(CAGCAleph)
    COM_INTERFACE_ENTRY(ISupportErrorInfo)
    COM_INTERFACE_ENTRIES_IAGCAlephImpl()
  END_COM_MAP()

// Category Map
public:
  BEGIN_CATEGORY_MAP(CAGCAleph)
    IMPLEMENTED_CATEGORY(CATID_AGC)
  END_CATEGORY_MAP()

// ISupportsErrorInfo Interface Methods
public:
  STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);
};


/////////////////////////////////////////////////////////////////////////////