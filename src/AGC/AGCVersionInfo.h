#pragma once

/////////////////////////////////////////////////////////////////////////////
// AGCVersionInfo.h: Declaration of the CAGCVersionInfo class.
//

#include <AGC.h>
#include "resource.h"
#include <..\Test\TCAtl\VersionInfoImpl.h>


/////////////////////////////////////////////////////////////////////////////
// AGCVersionInfo
//
class CAGCVersionInfo :
  public TCVersionInfoImpl<CAGCVersionInfo, IAGCVersionInfo, &LIBID_AGCLib>, 
  public AGCObjectSafetyImpl<CAGCVersionInfo>,
  public CComCoClass<CAGCVersionInfo, &CLSID_AGCVersionInfo>
{
// Declarations
public:
  DECLARE_REGISTRY_RESOURCEID(IDR_AGCVersionInfo)
  DECLARE_PROTECT_FINAL_CONSTRUCT()
  DECLARE_GET_CONTROLLING_UNKNOWN()

// Category Map
public:
  BEGIN_CATEGORY_MAP(CAGCVersionInfo)
    IMPLEMENTED_CATEGORY(CATID_AGC)
    IMPLEMENTED_CATEGORY(CATID_SafeForScripting)
    IMPLEMENTED_CATEGORY(CATID_SafeForInitializing)
  END_CATEGORY_MAP()

// Interface Map
public:
  BEGIN_COM_MAP(CAGCVersionInfo)
    COM_INTERFACE_ENTRY(IAGCVersionInfo)
    COM_INTERFACE_ENTRIES_TCVersionInfoImpl()
    COM_INTERFACE_ENTRY(IObjectSafety)
  END_COM_MAP()

// Construction
public:
  CAGCVersionInfo()
  {
    // Initialize with the AGC module
    m_vi.Load(uintptr_t(_Module.GetModuleInstance()));
  }
};


/////////////////////////////////////////////////////////////////////////////