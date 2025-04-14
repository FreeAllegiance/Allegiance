
/*-------------------------------------------------------------------------
 * fedsrv\AdminGames.CPP
 * 
 * Implementation of CAdminGames
 * 
 * Owner: 
 * 
 * Copyright 1986-1999 Microsoft Corporation, All Rights Reserved
 *-----------------------------------------------------------------------*/

#include "pch.h"


/////////////////////////////////////////////////////////////////////////////
// CAdminGames

TC_OBJECT_EXTERN_NON_CREATEABLE_IMPL(CAdminGames)


/*-------------------------------------------------------------------------
 * CAdminGames::Add()
 *-------------------------------------------------------------------------
 * Purpose:
 *   Add to the collection of Games 
 * 
 * Parameters:
 *   pGameParameters: a pointer to a CGameParameters interface.
 *                        This houses the mission params.
 *
 */
STDMETHODIMP CAdminGames::Add(IAGCGameParameters* pGameParameters)
{
  if (!pGameParameters)
    return Error("Creation Parameters was Null");

  // Cannot create a game on a paused server
  if (!g.strLobbyServer.IsEmpty() && !g.fmLobby.IsConnected())
    return Error(IDS_E_GAME_SERVER_PAUSED, IID_IAdminGames);

  IAGCPrivatePtr spPrivate(pGameParameters);
  assert(NULL != spPrivate);

  CGameParamData * pMissionParams = (CGameParamData *)spPrivate->GetIGCVoid();
  assert(pMissionParams);

    // Standalone server only supports a single game
    const ListFSMission * plistMission = CFSMission::GetMissions();
    if (plistMission->n())
      return Error(IDS_E_ONE_GAME_PER_SERVER, IID_IAdminGames);

    // Standalone server only support a maximum of c_cMaxPlayersPerGame players total
    if (pMissionParams->nTotalMaxPlayersPerGame > c_cMaxPlayersPerGame)
      pMissionParams->nTotalMaxPlayersPerGame = c_cMaxPlayersPerGame;

    // Standalone games always have a min players per team of 1 and max 
    // players per team of maxPlayerPerGame / nTeams.
    // XXX Note that the training mission #7 uses the server with a
    // XXX nTotalMaxPlayers setting of 1, and we need to handle that.
    pMissionParams->nMinPlayersPerTeam = 1;
    if (pMissionParams->nTotalMaxPlayersPerGame == 1)
        pMissionParams->nMaxPlayersPerTeam = 1;
    else
        pMissionParams->nMaxPlayersPerTeam = pMissionParams->nTotalMaxPlayersPerGame / pMissionParams->nTeams;

  // make sure params are valid, if not tell user why
  const char * szInvalid = pMissionParams->Invalid();
  if (szInvalid) 
    return Error(szInvalid);

  // Creation through this method always indicates object model created
  // KGJV : hack fix for AllSrvUI created game
  // since bObjectModelCreated isnt exposed thru the AGC interface we use bAllowEmptyTeams
  // so if bAllowEmptyTeams, game will be like a player created game
  pMissionParams->bObjectModelCreated = !pMissionParams->bAllowEmptyTeams; 

  // Get the story text of the specified AGCGameParameters object
  CComBSTR bstrStoryText;
  RETURN_FAILED(pGameParameters->get_StoryText(&bstrStoryText));
  USES_CONVERSION;
  const char* pszStoryText = bstrStoryText.Length() ?
    OLE2CT(bstrStoryText) : NULL;

  //
  // Create a new mission first
  //
  FedSrvSite * psiteFedSrv = new FedSrvSite();
  CFSMission * pfsMissionNew = new CFSMission(*pMissionParams,
                                              NULL, // NULL means use description in the params
                                              psiteFedSrv, 
                                              psiteFedSrv,
                                              pMissionParams,
                                              pszStoryText);
  if (g.fmLobby.IsConnected())
  {
    BEGIN_PFM_CREATE(g.fmLobby, pfmNewMission, S, NEW_MISSION)
    END_PFM_CREATE
    pfmNewMission->dwIGCMissionID = pfsMissionNew->GetIGCMission()->GetMissionID();

    g.fmLobby.SendMessages(g.fmLobby.GetServerConnection(), FM_GUARANTEED, FM_FLUSH);
  }
  return S_OK;
}


