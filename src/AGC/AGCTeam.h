#pragma once

/////////////////////////////////////////////////////////////////////////////
// AGCTeam.cpp : Declaration of the CAGCTeam class.
//

#include "resource.h"
#include "IAGCTeamImpl.h"


/////////////////////////////////////////////////////////////////////////////
// CAGCTeam
class ATL_NO_VTABLE CAGCTeam : 
  public IAGCTeamImpl<CAGCTeam, IsideIGC, IAGCTeam, &LIBID_AGCLib>,
	public ISupportErrorInfo,
	public CComCoClass<CAGCTeam, &CLSID_AGCTeam>
{
// Declarations
public:
  DECLARE_REGISTRY_RESOURCEID(IDR_AGCTeam)
  DECLARE_PROTECT_FINAL_CONSTRUCT()

// Interface Map
public:
  BEGIN_COM_MAP(CAGCTeam)
	  COM_INTERFACE_ENTRIES_IAGCTeamImpl()
	  COM_INTERFACE_ENTRY(ISupportErrorInfo)
  END_COM_MAP()

// Category Map
public:
  BEGIN_CATEGORY_MAP(CAGCTeam)
    IMPLEMENTED_CATEGORY(CATID_AGC)
  END_CATEGORY_MAP()

// ISupportsErrorInfo Interface Methods
public:
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);
};


/////////////////////////////////////////////////////////////////////////////