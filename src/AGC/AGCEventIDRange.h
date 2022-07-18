#ifndef __AGCEventIDRange_h__
#define __AGCEventIDRange_h__

/////////////////////////////////////////////////////////////////////////////
// AGCEventIDRange.h : Declaration of the CAGCEventIDRange class.
//

#include <AGC.h>
#include "resource.h"
#include "IAGCRangeImpl.h"
#include "AGCEventDef.h"


/////////////////////////////////////////////////////////////////////////////
// CAGCEventIDRange
class ATL_NO_VTABLE CAGCEventIDRange : 
  public IAGCRangeImpl<CAGCEventIDRange, AGCEventID, VT_I4, IAGCEventIDRange, &LIBID_AGCLib>,
  public AGCObjectSafetyImpl<CAGCEventIDRange>,
	public CComObjectRootEx<CComMultiThreadModel>, 
	public CComCoClass<CAGCEventIDRange, &CLSID_AGCEventIDRange>
{
// Declarations
public:
  DECLARE_REGISTRY_RESOURCEID(IDR_AGCEventIDRange)
  DECLARE_PROTECT_FINAL_CONSTRUCT()
  DECLARE_GET_CONTROLLING_UNKNOWN()

// Interface Map
public:
  BEGIN_COM_MAP(CAGCEventIDRange)
	  COM_INTERFACE_ENTRY(IAGCEventIDRange)
    COM_INTERFACE_ENTRIES_IAGCRangeImpl()
    COM_INTERFACE_ENTRY(IObjectSafety)
  END_COM_MAP()

// Category Map
public:
  BEGIN_CATEGORY_MAP(CAGCEventIDRange)
    IMPLEMENTED_CATEGORY(CATID_AGC)
    IMPLEMENTED_CATEGORY(CATID_SafeForScripting)
    IMPLEMENTED_CATEGORY(CATID_SafeForInitializing)
  END_CATEGORY_MAP()

// Overrides
public:
  AGCEventID GetVariantValue(VARIANT* pvar)
  {
    ZAssert(VT_I4 == V_VT(pvar));
    return static_cast<AGCEventID>(V_I4(pvar));
  }
  void GetPrefixString(LPOLESTR pszPrefix)
  {
    wcscpy(pszPrefix, L"[");
  }
  void GetSuffixString(LPOLESTR pszSuffix)
  {
    wcscpy(pszSuffix, L")");
  }
  void GetValueDelimiter(LPOLESTR pszDelim)
  {
    wcscpy(pszDelim, L",");
  }
  void FormatValueString(const AGCEventID& value, LPOLESTR pszOut)
  {
    const CAGCEventDef::XEventDef* it = CAGCEventDef::find(value);
    if (it != CAGCEventDef::end())
      swprintf(pszOut, L"%ls", it->m_pszName);
    else
      swprintf(pszOut, L"%d", value);
  }
  bool ReadValueString(LPCOLESTR pszValue, AGCEventID& value)
  {
    if (CAGCEventDef::IDFromName(pszValue, value))
      return true;
    return 1 == swscanf(pszValue, L"%d", &value);
  }
};


/////////////////////////////////////////////////////////////////////////////

#endif //__AGCEventIDRange_h__
