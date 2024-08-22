#pragma once

/////////////////////////////////////////////////////////////////////////////
// AGCEventIDRanges.h : Declaration of the CAGCEventIDRanges class.
//

#include <AGC.h>
#include "resource.h"
#include "IAGCRangesImpl.h"


/////////////////////////////////////////////////////////////////////////////
// CAGCEventIDRanges
class ATL_NO_VTABLE CAGCEventIDRanges : 
  public IAGCRangesImpl<CAGCEventIDRanges, AGCEventID, IAGCEventIDRanges,
    IAGCEventIDRange, &LIBID_AGCLib>,
  public AGCObjectSafetyImpl<CAGCEventIDRanges>,
	public CComObjectRootEx<CComMultiThreadModel>, 
	public CComCoClass<CAGCEventIDRanges, &CLSID_AGCEventIDRanges>
{
// Declarations
public:
  DECLARE_REGISTRY_RESOURCEID(IDR_AGCEventIDRanges)
  DECLARE_PROTECT_FINAL_CONSTRUCT()
  DECLARE_GET_CONTROLLING_UNKNOWN()

// Interface Map
public:
  BEGIN_COM_MAP(CAGCEventIDRanges)
	  COM_INTERFACE_ENTRY(IAGCEventIDRanges)
    COM_INTERFACE_ENTRIES_IAGCRangesImpl()
    COM_INTERFACE_ENTRY(IObjectSafety)
  END_COM_MAP()

// Category Map
public:
  BEGIN_CATEGORY_MAP(CAGCEventIDRanges)
    IMPLEMENTED_CATEGORY(CATID_AGC)
    IMPLEMENTED_CATEGORY(CATID_SafeForScripting)
    IMPLEMENTED_CATEGORY(CATID_SafeForInitializing)
  END_CATEGORY_MAP()

// Overrides
public:
  static HRESULT CreateRange(const AGCEventID& value1,
    const AGCEventID& value2, IAGCEventIDRange** ppRange)
  {
    return GetAGCGlobal()->MakeAGCEventIDRange(value1, value2, ppRange);
  }
};


/////////////////////////////////////////////////////////////////////////////