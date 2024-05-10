
/*-------------------------------------------------------------------------
 * fedsrv\AdminUser.H
 * 
 * Declaration of CAdminUser
 * 
 * Owner: 
 * 
 * Copyright 1986-1999 Microsoft Corporation, All Rights Reserved
 *-----------------------------------------------------------------------*/

#pragma once


class CFSPlayer;

/////////////////////////////////////////////////////////////////////////////
// CAdminUser
class ATL_NO_VTABLE CAdminUser : 
	public IDispatchImpl<IAdminUser, &IID_IAdminUser, &LIBID_ALLEGIANCESERVERLib>,
	public ISupportErrorInfo,
  public CAdminLimb<CAdminUser, IAdminUser>,
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CAdminUser, &CLSID_AdminUser>
{
// Declarations
public:
  DECLARE_REGISTRY_RESOURCEID(IDR_USER)
  DECLARE_PROTECT_FINAL_CONSTRUCT()

// Interface Map
public:
  BEGIN_COM_MAP(CAdminUser)
	  COM_INTERFACE_ENTRY(IAdminUser)
	  COM_INTERFACE_ENTRY(IDispatch)
	  COM_INTERFACE_ENTRY(ISupportErrorInfo)
  END_COM_MAP()

// Category Map
public:
  BEGIN_CATEGORY_MAP(CAdminUser)
    IMPLEMENTED_CATEGORY(CATID_AllegianceAdmin)
  END_CATEGORY_MAP()

// Construction / Destruction
public:
  CAdminUser() :
    m_fWantChat(false)
	{
	}
	virtual ~CAdminUser()
  {
  }

// ISupportsErrorInfo Interface Methods
public:
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// IAdminUser Interface Methods
public:
  STDMETHODIMP get_Name(BSTR* pbstr);
  STDMETHODIMP SendMsg(BSTR bstrMessage);
  STDMETHODIMP get_isReady(VARIANT_BOOL* pVal);
  STDMETHODIMP put_isReady(VARIANT_BOOL Val);
  STDMETHODIMP get_Ship(IAdminShip** ppShip);
  STDMETHODIMP Boot();
  STDMETHODIMP get_UserID(AdminUserID *userid);
  STDMETHODIMP get_UniqueID(AGCUniqueID *uniqueID);
  STDMETHODIMP get_PlayerStats(IAGCEvent** ppStats);

// CAdminUser
public:
  
  void            Init(CFSPlayer *pPlayer); // finish construction
  const           BOOL& bWantChat()           { return m_fWantChat; }
  static AdminUserID DetermineID(CFSPlayer * pPlayer);

// Data Members
protected:
  CFSPlayer *     m_pPlayer;
  IshipIGC  *     m_pIshipIGC;  // here for speed and reduced code size
  BOOL            m_fWantChat;  // should we fire off chat events?
};


/////////////////////////////////////////////////////////////////////////////