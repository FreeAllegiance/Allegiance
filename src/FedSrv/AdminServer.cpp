
/*-------------------------------------------------------------------------
 * fedsrv\AdminServer.CPP
 * 
 * Implementation of CAdminServer
 * 
 * Owner: 
 * 
 * Copyright 1986-1999 Microsoft Corporation, All Rights Reserved
 *-----------------------------------------------------------------------*/

#include "pch.h"


/////////////////////////////////////////////////////////////////////////////
// External References

extern Global g;


/////////////////////////////////////////////////////////////////////////////
// CAdminServer

TC_OBJECT_EXTERN_NON_CREATEABLE_IMPL(CAdminServer)


/////////////////////////////////////////////////////////////////////////////
// ISupportsErrorInfo Interface Methods

STDMETHODIMP CAdminServer::InterfaceSupportsErrorInfo(REFIID riid)
{
  static const IID* arr[] = 
  {
    &IID_IAdminServer
  };
  for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
  {
    if (InlineIsEqualGUID(*arr[i],riid))
      return S_OK;
  }
  return S_FALSE;
}


/////////////////////////////////////////////////////////////////////////////
// IAdminServer Interface Methods

/*-------------------------------------------------------------------------
 * get_Games()
 *-------------------------------------------------------------------------
 * Purpose:
 *    
 * 
 */
STDMETHODIMP CAdminServer::get_Games(IAdminGames** ppAdminGames)
{
  //
  // Create a Games object
  //
  return GetAGCGlobal()->GetAGCObject(CFSMission::GetMissions(),
    IID_IAdminGames, (void**)ppAdminGames);
}


/*-------------------------------------------------------------------------
 * get_cPlayers()
 *-------------------------------------------------------------------------
 * Purpose:
 *    Determine the number of players currently playing
 * 
 */
STDMETHODIMP CAdminServer::get_PlayerCount(long* pVal)
{
  CLEAROUT(pVal, (long)g.pServerCounters->cPlayersOnline);
  return S_OK;
}


/*-------------------------------------------------------------------------
 * get_cMissions()
 *-------------------------------------------------------------------------
 * Purpose:
 *    Determine the number of missions (that is, games) currently running
 * 
 */
STDMETHODIMP CAdminServer::get_MissionCount(long* pVal)
{
  const ListFSMission* plistMission = CFSMission::GetMissions();
  CLEAROUT(pVal, (long)plistMission->n());
  return S_OK;
}


/*-------------------------------------------------------------------------
 * SendMsg()
 *-------------------------------------------------------------------------
 * Purpose:
 *    Send a message to everyone on the server
 * 
 * Note: If no one is in the sector, no messages are sent
 */
STDMETHODIMP CAdminServer::SendMsg(BSTR bstrMessage)
{
  // Do nothing if specified string is empty
  if (!BSTRLen(bstrMessage))
    return S_OK;

  //
  // Iterate thru all missions
  //
  const ListFSMission * plistMission = CFSMission::GetMissions();
  if (plistMission->n())
  {
    // Convert the specified string to ANSI once
    USES_CONVERSION;
    LPCSTR pszMessage = OLE2CA(bstrMessage);

    // Loop through and send chat to each mission
    for (LinkFSMission* plinkMission = plistMission->first(); plinkMission; plinkMission = plinkMission->next())
    {
      CFSMission * pfsMission = plinkMission->data();
      pfsMission->GetSite()->SendChat(
        NULL,                     //from system
        CHAT_EVERYONE,            //send to everyone
        NA,                       //no target ship 
        NA,                       //No voice over
        pszMessage,
        c_cidNone,
        NA,
        NA,
        NULL,
        true);                    // true means object model generated
    }
  }

  // Indicate success
  return S_OK;
}





/*-------------------------------------------------------------------------
 * CAdminServer::get_Users()
 *-------------------------------------------------------------------------
 * Purpose:
 *   Returns the collection of users currently on the server.   
 * 
 */

STDMETHODIMP CAdminServer::get_Users(/*[out, retval]*/ IAdminUsers** ppAdminUsers)
{
    static CAdminSponsor<CAdminUsers> s_UsersSponsor;

    RETURN_FAILED (s_UsersSponsor.Make(IID_IAdminUsers, (void**)ppAdminUsers));

    return S_OK;
}




/*-------------------------------------------------------------------------
 * get_FindUser()
 *-------------------------------------------------------------------------
 * Purpose:
 *   Converts a user name to an CAdminUser Object.
 *
 * Input:
 *   bstr: name of the user.
 *
 * Output:
 *   a pointer to an User that cooresponds to the name supplied; or NULL if not found
 */
STDMETHODIMP CAdminServer::get_FindUser(BSTR bstrName, IAdminUser** ppUser)
{
  *ppUser = NULL; // assume not found for now

  USES_CONVERSION;
                  
  char * szName = OLE2A(bstrName);

  const ListFSMission * plistMission = CFSMission::GetMissions();
  for (LinkFSMission * plinkMission = plistMission->first(); plinkMission; plinkMission = plinkMission->next())
  {
      CFSMission*    pfsMission = (CFSMission*)(plinkMission->data());

      if (pfsMission)
      {
          ImissionIGC * pMission = pfsMission->GetIGCMission();

          if (pMission)
          {
              for (ShipLinkIGC*   psl = pMission->GetShips()->first(); (psl != NULL); psl = psl->next())
              {
                  CFSShip*    pfsShip = (CFSShip*)(psl->data()->GetPrivateData());
                  if(pfsShip->IsPlayer())
                  {
                      CFSPlayer * pfsPlayer = pfsShip->GetPlayer();

                      if (_stricmp(szName, pfsShip->GetName()) == 0)
                      {
                          RETURN_FAILED (pfsPlayer->CAdminSponsor<CAdminUser>::Make(IID_IAdminUser, (void**)ppUser));

                          pfsPlayer->CAdminSponsor<CAdminUser>::GetLimb()->Init(pfsPlayer);

                          return S_OK;
                      }
                  }
              }
          }
      }
  }

  return S_OK;  // not found, which is ok
}



/*-------------------------------------------------------------------------
 * get_LookupUser()
 *-------------------------------------------------------------------------
 * Purpose:
 *   Converts a unique id to an CAdminUser Object.
 *
 * Input:
 *   id: id of the user.
 *
 * Output:
 *   a pointer to an User that cooresponds to the id supplied; or NULL if not found
 */
STDMETHODIMP CAdminServer::get_LookupUser(AGCUniqueID userid, IAdminUser** ppUser)
{
  *ppUser = NULL; // assume not found for now
  //  CLEAROUT(ppUser, (IAdminUser*)NULL);

  if ((userid >> 16) != AGC_AdminUser)
    return S_OK;

  AdminUserID id = userid & 0x0000FFFF;

  const ListFSMission * plistMission = CFSMission::GetMissions();
  for (LinkFSMission * plinkMission = plistMission->first(); plinkMission; plinkMission = plinkMission->next())
  {
      CFSMission*    pfsMission = (CFSMission*)(plinkMission->data());

      if (pfsMission)
      {
          ImissionIGC * pMission = pfsMission->GetIGCMission();

          if (pMission)
          {
              for (ShipLinkIGC*   psl = pMission->GetShips()->first(); (psl != NULL); psl = psl->next())
              {
                  CFSShip*    pfsShip = (CFSShip*)(psl->data()->GetPrivateData());
                  if(pfsShip->IsPlayer())
                  {
                      CFSPlayer * pfsPlayer = pfsShip->GetPlayer();

                      if (CAdminUser::DetermineID(pfsPlayer) == id)
                      {
                          RETURN_FAILED (pfsPlayer->CAdminSponsor<CAdminUser>::Make(IID_IAdminUser, (void**)ppUser));

                          pfsPlayer->CAdminSponsor<CAdminUser>::GetLimb()->Init(pfsPlayer);
                      }
                  }
              }
          }
      }
  }

  return S_OK;
}


/*-------------------------------------------------------------------------
 * get_PacketsInPerSecond()
 *-------------------------------------------------------------------------
 */
STDMETHODIMP CAdminServer::get_PacketsIn(long * pVal)
{
    *pVal = 0;
    return Error("This property is no longer supported. Use AdminSession::PerfCounters now.");
}

/*-------------------------------------------------------------------------
 * get_PlayersOnline()
 *-------------------------------------------------------------------------
 */
STDMETHODIMP CAdminServer::get_PlayersOnline(long * pVal)
{
    *pVal = 0;
    return Error("This property is no longer supported. Use AdminSession::PerfCounters now.");
}


/*-------------------------------------------------------------------------
 * get_TimeInnerLoop()
 *-------------------------------------------------------------------------
 */
STDMETHODIMP CAdminServer::get_TimeInnerLoop(long * pVal)
{
    *pVal = 0;
    return Error("This property is no longer supported. Use AdminSession::PerfCounters now.");
}


/*-------------------------------------------------------------------------
 * get_MachineName()
 *-------------------------------------------------------------------------
 */
STDMETHODIMP CAdminServer::get_MachineName(BSTR * pbstrMachine)
{
  char szMachine[64];

  if (gethostname(szMachine, sizeof(szMachine)) != 0)
  {
    return Error("Cannot determine machine name.");
  }

  CComBSTR bstr(szMachine);

  // Detach the string to the [out] parameter
  CLEAROUT(pbstrMachine, (BSTR)bstr);
  bstr.Detach();
  return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
// Sets the LobbyServer onto which this server publishes its games
//
// Fails if called while any games are in existence.
//
STDMETHODIMP CAdminServer::put_LobbyServer(BSTR bstrLobbyServer)
{
#if defined(ALLSRV_STANDALONE)
  return Error("This method is not intended for standalone servers");
#endif

  // Connect to the specified lobby, if any
  if (BSTRLen(bstrLobbyServer))
  {
    USES_CONVERSION;
    LPCSTR pszLobbyServer = OLE2CA(bstrLobbyServer);

    // Fail if any games are running and bstrLobbyServer is different
    if (0 != _stricmp(pszLobbyServer, g.strLobbyServer))
    {
      const ListFSMission * plistMission = CFSMission::GetMissions();
      if (plistMission->n())
        return Error(IDS_E_LOBBYSERVER_GAMES_EXIST, IID_IAdminServer);
    }

    HRESULT hr = ConnectToLobby(NULL);
    if (FAILED(hr))
    {
      #if !defined(ALLSRV_STANDALONE)
        _AGCModule.TriggerEvent(NULL, AllsrvEventID_ConnectError, "", -1, -1, -1, 0);
      #endif // !defined(ALLSRV_STANDALONE)
      return Error(IDS_E_CONNECT_LOBBY, IID_IAdminServer);
    }

    // Save the new LobbyServer value to the registry
    #if !defined(ALLSRV_STANDALONE)
      CRegKey key;
      LONG lr = key.Open(HKEY_LOCAL_MACHINE, HKLM_FedSrv);
      if (ERROR_SUCCESS != lr)
      {
        g.fmLobby.Shutdown();
        _AGCModule.TriggerEvent(NULL, AllsrvEventID_ConnectError, "", -1, -1, -1, 0);
        return HRESULT_FROM_WIN32(lr);
      }
      key.SetValue(pszLobbyServer, "LobbyServer");
    #endif // !defined(ALLSRV_STANDALONE)
    
  }
  else
  {
    // Disconnect from current lobby, if any
    DisconnectFromLobby();
    ZAssert(!g.fmLobby.IsConnected());
    g.strLobbyServer.SetEmpty();
  }

  // Indicate success
  return S_OK;
}


/////////////////////////////////////////////////////////////////////////////
//
//
STDMETHODIMP CAdminServer::get_LobbyServer(BSTR* pbstrLobbyServer)
{
  // Get the current "LobbyServer" value
  CComBSTR bstr(g.strLobbyServer);

  // Detach to the [out] parameter
  CLEAROUT(pbstrLobbyServer, (BSTR)bstr);
  bstr.Detach();

  // Return the last HRESULT
  return S_OK;
}


/////////////////////////////////////////////////////////////////////////////
//
//
STDMETHODIMP CAdminServer::CreateDefaultGames()
{
  #if defined(ALLSRV_STANDALONE)
    // Don't create new games if any games are already there.  This is to 
    // prevent starting two copies of the default games.
    const ListFSMission * plistMission = CFSMission::GetMissions();
    if (plistMission->n())
      return S_FALSE;

    StartDefaultGames();
    return S_OK;
  #else
    return E_NOTIMPL;
  #endif
}


/////////////////////////////////////////////////////////////////////////////
//
//
STDMETHODIMP CAdminServer::get_PublicLobby(VARIANT_BOOL *pVal)
{
  CLEAROUT(pVal, VARBOOL(g.fm.GetHostApplicationGuid() == FEDSRV_GUID));
  return S_OK;
}


/////////////////////////////////////////////////////////////////////////////
//
//
STDMETHODIMP CAdminServer::put_PublicLobby(VARIANT_BOOL bPublic)
{
  // make sure no games are present; otherwise players could have there sessions 
  // terminated
  const ListFSMission * plistMission = CFSMission::GetMissions();
  if (plistMission->n())
    return Error(IDS_E_LOBBYSERVER_GAMES_EXIST, IID_IAdminServer);

  //
  // Switch between sessions as needed; Private and Public games use different
  // session GUIDs
  //
  if (bPublic)
  {
    if (g.fm.GetHostApplicationGuid() != FEDSRV_GUID)
    {
      // if existing session running, kill it
      if (g.fm.GetHostApplicationGuid() != GUID_NULL) 
        g.fm.Shutdown();

      printf("Preparing public session for clients ... ");
      #if !defined(ALLSRV_STANDALONE)
      _AGCModule.TriggerEvent(NULL, AllsrvEventID_HostSession, "", -1, -1, -1, 0);
      #endif

      HRESULT hr = g.fm.HostSession(FEDSRV_GUID, false, 0 /*g.hPlayerEvent*/, g.fProtocol
          #if defined(MONOLITHIC_DPLAY) && defined(ALLSRV_STANDALONE)
            , false
          #endif
          );

      if (FAILED(hr))
      {
        #if !defined(ALLSRV_STANDALONE)
        _AGCModule.TriggerEvent(NULL, AllsrvEventID_CantHostSession, "", -1, -1, -1, 0);
        #endif
        printf("failed.\n");
        return E_FAIL;
      }
      printf("succeeded.\n");
    }

    bool bSupposedToConnectToLobby;

    #if defined(ALLSRV_STANDALONE)
    bSupposedToConnectToLobby = true;
    #else
    bSupposedToConnectToLobby = !g.strLobbyServer.IsEmpty();
    #endif

    if (bSupposedToConnectToLobby)
    {
      HRESULT hr = ConnectToLobby(NULL);
      if (FAILED(hr))
      {
        #if !defined(ALLSRV_STANDALONE)
          _AGCModule.TriggerEvent(NULL, AllsrvEventID_ConnectError, "", -1, -1, -1, 0);
        #endif // !defined(ALLSRV_STANDALONE)
        return Error(IDS_E_CONNECT_LOBBY, IID_IAdminServer);
      }
    }
    else
    {
      #if !defined(ALLSRV_STANDALONE)
      _AGCModule.TriggerEvent(NULL, AllsrvEventID_NoLobby, "", -1, -1, -1, 0);
      #endif
    }
  }
  else
  {
    #if !defined(ALLSRV_STANDALONE)
    return Error("Only standalone servers can have private games"); 
    #endif
    //
    // Setup the private session for standalone server
    //
    if (g.fm.GetHostApplicationGuid() != FEDSRV_STANDALONE_PRIVATE_GUID)
    {
      // if existing session running, kill it
      if (g.fm.GetHostApplicationGuid() != GUID_NULL)
        g.fm.Shutdown();

      printf("Preparing LAN session for clients ... ");
      #if !defined(ALLSRV_STANDALONE)
      _AGCModule.TriggerEvent(NULL, AllsrvEventID_HostSession, "", -1, -1, -1, 0);
      #endif

      HRESULT hr = g.fm.HostSession(FEDSRV_STANDALONE_PRIVATE_GUID, false, 0 /*g.hPlayerEvent*/, g.fProtocol
          #if defined(MONOLITHIC_DPLAY) && defined(ALLSRV_STANDALONE)
            , false
          #endif
          );
      if (FAILED(hr))
      {
        #if !defined(ALLSRV_STANDALONE)
        _AGCModule.TriggerEvent(NULL, AllsrvEventID_CantHostSession, "", -1, -1, -1, 0);
        #endif
       printf("failed.\n");
        return E_FAIL;
      }
      printf("succeeded.\n");
    }
  }
  return S_OK;
}
