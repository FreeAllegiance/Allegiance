
/*-------------------------------------------------------------------------
 * fedsrv\AdminUsers.H
 * 
 * Declaration of CAdminUsers
 * 
 * Owner: 
 * 
 * Copyright 1986-1999 Microsoft Corporation, All Rights Reserved
 *-----------------------------------------------------------------------*/

#pragma once


class CFSShip;

typedef CAdminSponsor<CAdminUser> CAdminUserSponsor;


/////////////////////////////////////////////////////////////////////////////
// CAdminUsers
class ATL_NO_VTABLE CAdminUsers : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CAdminUsers, &CLSID_AdminUsers>,
	public ISupportErrorInfo,
	public IDispatchImpl<IAdminUsers, &IID_IAdminUsers, &LIBID_ALLEGIANCESERVERLib>,
  public CAdminLimb<CAdminUsers, IAdminUsers>
{
public:
	CAdminUsers();
  virtual ~CAdminUsers();

DECLARE_REGISTRY_RESOURCEID(IDR_USERS)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CAdminUsers)
	COM_INTERFACE_ENTRY(IAdminUsers)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()

BEGIN_CATEGORY_MAP(CAdminUsers)
  IMPLEMENTED_CATEGORY(CATID_AllegianceAdmin)
END_CATEGORY_MAP()


// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// IAdminUsers
public:
  STDMETHODIMP get_Count(long* pnCount);
  STDMETHODIMP get__NewEnum(IUnknown** ppunkEnum);
  STDMETHODIMP get_Item(VARIANT index, IAdminUser** ppUser);

// CAdminUsers
public:

  // finish construction...I wish COM allowed parameters to constructors
  void SetMission(const ImissionIGC *  pIGCmission) { m_pIGCmission = pIGCmission;} 

private:

  const ImissionIGC *           m_pIGCmission; // if NULL, then use all missions
};