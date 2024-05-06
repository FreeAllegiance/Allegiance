#pragma once

/////////////////////////////////////////////////////////////////////////////
// AGCSector.cpp : Declaration of the CAGCSector class.
//

#include "resource.h"
#include "IAGCSectorImpl.h"


/////////////////////////////////////////////////////////////////////////////
// CAGCSector
//
class ATL_NO_VTABLE CAGCSector : 
  public IAGCSectorImpl<CAGCSector, IclusterIGC, IAGCSector, &LIBID_AGCLib>,
  public ISupportErrorInfo,
  public CComCoClass<CAGCSector, &CLSID_AGCSector>
{
// Declarations
public:
  DECLARE_REGISTRY_RESOURCEID(IDR_AGCSector)
  DECLARE_PROTECT_FINAL_CONSTRUCT()

// Interface Map
public:
  BEGIN_COM_MAP(CAGCSector)
    COM_INTERFACE_ENTRIES_IAGCSectorImpl()
    COM_INTERFACE_ENTRY(ISupportErrorInfo)
  END_COM_MAP()        

// Category Map
public:
  BEGIN_CATEGORY_MAP(CAGCSector)
    IMPLEMENTED_CATEGORY(CATID_AGC)
  END_CATEGORY_MAP()

// ISupportsErrorInfo Interface Methods
public:
  STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);
};


/////////////////////////////////////////////////////////////////////////////