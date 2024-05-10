
/*-------------------------------------------------------------------------
 * fedsrv\AdminServer.CPP
 * 
 * Declaration of CAdminServer
 * 
 * Owner: 
 * 
 * Copyright 1986-1999 Microsoft Corporation, All Rights Reserved
 *-----------------------------------------------------------------------*/

#pragma once

/////////////////////////////////////////////////////////////////////////////
// CAdminServer
//
class ATL_NO_VTABLE CAdminServer : 
  public IDispatchImpl<IAdminServer, &IID_IAdminServer, &LIBID_ALLEGIANCESERVERLib>,
  public ISupportErrorInfo,
  public CComObjectRootEx<CComSingleThreadModel>,
  public CComCoClass<CAdminServer, &CLSID_AdminServer>
{
// Declarations
public:
  DECLARE_REGISTRY_RESOURCEID(IDR_ADMINSERVER)
  DECLARE_PROTECT_FINAL_CONSTRUCT()

// Interface Map
public:
  BEGIN_COM_MAP(CAdminServer)
    COM_INTERFACE_ENTRY(IAdminServer)
    COM_INTERFACE_ENTRY(IDispatch)
    COM_INTERFACE_ENTRY(ISupportErrorInfo)
  END_COM_MAP()

// Category Map
public:
  BEGIN_CATEGORY_MAP(CAdminServer)
    IMPLEMENTED_CATEGORY(CATID_AllegianceAdmin)
  END_CATEGORY_MAP()

// ISupportsErrorInfo Interface Methods
public:
  STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// IAdminServer Interface Methods
public:
  STDMETHODIMP get_Games(IAdminGames** ppAdminGames);
  STDMETHODIMP get_PlayerCount(long *pVal);
  STDMETHODIMP get_MissionCount(long *pVal);
  STDMETHODIMP SendMsg(BSTR  bstrMessage);
  STDMETHODIMP get_Users(IAdminUsers** ppAdminUsers);
  STDMETHODIMP get_LookupUser(AGCUniqueID id, IAdminUser** ppUser);
  STDMETHODIMP get_FindUser(BSTR bstrName, IAdminUser** ppUser);
  STDMETHODIMP get_MachineName(BSTR * pbstrMachine);
  STDMETHODIMP get_PacketsIn(long * pVal);
  STDMETHODIMP get_PlayersOnline(long * pVal);
  STDMETHODIMP get_TimeInnerLoop(long * pVal);
  STDMETHODIMP put_LobbyServer(BSTR bstrLobbyServer);
  STDMETHODIMP get_LobbyServer(BSTR* pbstrLobbyServer);
  STDMETHODIMP put_PublicLobby(VARIANT_BOOL bPublic);
  STDMETHODIMP get_PublicLobby(VARIANT_BOOL* pbPublic);
  STDMETHODIMP CreateDefaultGames();
};


/////////////////////////////////////////////////////////////////////////////