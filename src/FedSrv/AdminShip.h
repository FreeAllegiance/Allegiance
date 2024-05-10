#pragma once

/*-------------------------------------------------------------------------
 * fedsrv\AdminShip.H
 * 
 * Declaration of the CAdminShip.  
 * 
 * Owner: 
 * 
 * Copyright 1986-2000 Microsoft Corporation, All Rights Reserved
 *-----------------------------------------------------------------------*/

#include "..\AGC\IAGCShipImpl.h"


/////////////////////////////////////////////////////////////////////////////
// Forward Declarations

class CAdminUsers;


/////////////////////////////////////////////////////////////////////////////
// Conversion Function Templates

template <>
inline IshipIGC* Host2Igc(CFSShip* p)
{
  return p->GetIGCShip();
}

template <>
inline CFSShip* Igc2Host(IshipIGC* p)
{
  return reinterpret_cast<CFSShip*>(p->GetPrivateData());
}


/////////////////////////////////////////////////////////////////////////////
// CAdminShip
class ATL_NO_VTABLE CAdminShip :
  public IAGCShipImpl<CAdminShip, CFSShip, IAdminShip, &LIBID_ALLEGIANCESERVERLib>,
	public CComCoClass<CAdminShip, &CLSID_AdminShip>
{
// Declarations
public:
  DECLARE_REGISTRY_RESOURCEID(IDR_ADMINSHIP)
  DECLARE_PROTECT_FINAL_CONSTRUCT()

// Interface Map
public:
  BEGIN_COM_MAP(CAdminShip)
	  COM_INTERFACE_ENTRY(IAdminShip)
    COM_INTERFACE_ENTRIES_IAGCShipImpl()
  END_COM_MAP()

// Category Map
public:
  BEGIN_CATEGORY_MAP(CAdminShip)
    IMPLEMENTED_CATEGORY(CATID_AllegianceAdmin)
  END_CATEGORY_MAP()

// Construction
public:
  CAdminShip();

// IAdminShip Interface Methods
public:
  STDMETHODIMP get_User(IAdminUser** ppUser);
};


/////////////////////////////////////////////////////////////////////////////