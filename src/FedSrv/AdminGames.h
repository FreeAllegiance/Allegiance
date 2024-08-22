
/*-------------------------------------------------------------------------
 * fedsrv\AdminGames.H
 * 
 * Declaration of CAdminGames
 * 
 * Owner: 
 * 
 * Copyright 1986-1999 Microsoft Corporation, All Rights Reserved
 *-----------------------------------------------------------------------*/

#pragma once


#include "..\agc\IAGCCollectionImpl.h"


/////////////////////////////////////////////////////////////////////////////
// Conversion Function Templates

template <>
inline ImissionIGC* Host2Igc(CFSMission* p)
{
  return p->GetIGCMission();
}

template <>
inline CFSMission* Igc2Host(ImissionIGC* p)
{
  return reinterpret_cast<CFSMission*>(p->GetPrivateData());
}


/////////////////////////////////////////////////////////////////////////////
// CAdminGames
//
class ATL_NO_VTABLE CAdminGames :
  public IAGCCollectionImpl<CAdminGames, const ListFSMission, IAdminGames,
    CFSMission, IAdminGame, &LIBID_ALLEGIANCESERVERLib,
    const ListFSMission, IAdminGames, ImissionIGC, IAGCGame>,
  public CComCoClass<CAdminGames, &CLSID_AdminGames>
{
// Declarations
public:
  DECLARE_REGISTRY_RESOURCEID(IDR_ADMINGAMES)
  DECLARE_PROTECT_FINAL_CONSTRUCT()
  DECLARE_AGC_TYPE(admin)

// Interface Map
public:
  BEGIN_COM_MAP(CAdminGames)
    COM_INTERFACE_ENTRY(IAdminGames)
    COM_INTERFACE_ENTRIES_IAGCCollectionImpl()
  END_COM_MAP()

// Category Map
public:
  BEGIN_CATEGORY_MAP(CAdminGames)
    IMPLEMENTED_CATEGORY(CATID_AllegianceAdmin)
  END_CATEGORY_MAP()

// IAdminGames Interface Methods
public:
  STDMETHODIMP Add(IAGCGameParameters* pCreationParameters);
};


/////////////////////////////////////////////////////////////////////////////