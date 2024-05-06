#pragma once

/////////////////////////////////////////////////////////////////////////////
// AGCTeams.h : Declaration of the CAGCTeams class.
//

#include "resource.h"
#include "IAGCCollectionImpl.h"


/////////////////////////////////////////////////////////////////////////////
// CAGCTeams
//
class ATL_NO_VTABLE CAGCTeams :
  public IAGCCollectionImpl<CAGCTeams, const SideListIGC, IAGCTeams,
    IsideIGC, IAGCTeam, &LIBID_AGCLib>,
  public CComCoClass<CAGCTeams, &CLSID_AGCTeams>
{
// Declarations
public:
  DECLARE_REGISTRY_RESOURCEID(IDR_AGCTeams)
  DECLARE_PROTECT_FINAL_CONSTRUCT()

// Interface Map
public:
  BEGIN_COM_MAP(CAGCTeams)
    COM_INTERFACE_ENTRY(IAGCTeams)
    COM_INTERFACE_ENTRIES_IAGCCollectionImpl()
  END_COM_MAP()

// Category Map
public:
  BEGIN_CATEGORY_MAP(CAGCTeams)
    IMPLEMENTED_CATEGORY(CATID_AGC)
  END_CATEGORY_MAP()
};


/////////////////////////////////////////////////////////////////////////////