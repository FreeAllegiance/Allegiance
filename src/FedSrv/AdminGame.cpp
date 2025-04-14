
/*-------------------------------------------------------------------------
 * fedsrv\AdminGame.CPP
 * 
 * Implementation of CAdminGame.  
 * 
 * Owner: 
 * 
 * Copyright 1986-1999 Microsoft Corporation, All Rights Reserved
 *-----------------------------------------------------------------------*/

#include "pch.h"


/////////////////////////////////////////////////////////////////////////////
// CAdminGame

TC_OBJECT_EXTERN_NON_CREATEABLE_IMPL(CAdminGame)


/////////////////////////////////////////////////////////////////////////////
// Construction / Destruction

CAdminGame::CAdminGame() :
  m_pUsers(NULL)
{
}

void CAdminGame::Init(ImissionIGC* pIGC)
{
  // Perform default processing
  IAGCCommonImplBase::Init(pIGC);
}

/////////////////////////////////////////////////////////////////////////////
// IAGCGame Interface Methods


/*-------------------------------------------------------------------------
 * get_GameParameters()
 *-------------------------------------------------------------------------
 */
STDMETHODIMP CAdminGame::get_GameParameters(IAGCGameParameters** ppParams) // overrides IAGCGameImpl
{
  // Perform default processing
  RETURN_FAILED(IAGCGameImplBase::get_GameParameters(ppParams));
  assert(*ppParams);

  // Get the IGC pointer
  ImissionIGC * pIGCMission = GetIGC();
  assert(pIGCMission);

  // Populate the team names
  for (SideLinkIGC*   psl = pIGCMission->GetSides()->first(); (psl != NULL); psl = psl->next())
  {
    IsideIGC*   pside = psl->data();

    if (pside)
    {
      CComBSTR bstrTemp(pside->GetName()); 
      RETURN_FAILED((*ppParams)->put_TeamName(pside->GetObjectID(), bstrTemp));
    }
  }

  // Get the CFSMission pointer
  CFSMission* pFSMission = GetHostIGC();

  // Set the StoryText property
  CComBSTR bstrStoryText(pFSMission->GetStoryText());
  RETURN_FAILED((*ppParams)->put_StoryText(bstrStoryText));

  // Set the starting tech bits
  if (pFSMission->m_pttbmAltered && pFSMission->m_pttbmNewSetting)
  {
    // Loop through the techbit masks for each team
    for (int iSide = 0; iSide < c_cSidesMax; ++iSide)
    {
      // Loop through each techbit
      for (short iBit = 0; iBit < c_ttbMax; ++iBit)
      {
        if (pFSMission->m_pttbmAltered[iSide].GetBit(iBit))
        {
          bool bBit = pFSMission->m_pttbmNewSetting[iSide].GetBit(iBit);
          (*ppParams)->put_OverriddenTechBit(iSide, iBit, VARBOOL(bBit));
        }
      }
    }
  }

  // Indicate success
  return S_OK;
}


/////////////////////////////////////////////////////////////////////////////
// IAdminGame Interface Methods

/*-------------------------------------------------------------------------
 * SendMsg()
 *-------------------------------------------------------------------------
 * Purpose:
 *    Send a message to everyone in the game (aka mission)
 * 
 */
STDMETHODIMP CAdminGame::SendMsg(BSTR bstrMessage)
{
  // Do nothing if string is empty
  if (!bstrMessage || !SysStringLen(bstrMessage))
    return S_FALSE;

  assert(GetHostIGC());

  USES_CONVERSION;
  GetHostIGC()->GetSite()->SendChat(NULL,   //from system
                  CHAT_EVERYONE,            //send to everyone
                  NA,                       //no target ship 
                  NA,                       //No voice over
                  OLE2A(bstrMessage), c_cidNone, NA, NA, NULL, 
                  true); // true means object model generated

  // Indicate success
	return S_OK;
}


/*-------------------------------------------------------------------------
 * CAdminGame::Kill()
 *-------------------------------------------------------------------------
 * Purpose:
 *    Commit sucide
 * 
 */
STDMETHODIMP CAdminGame::Kill()
{

  assert(GetHostIGC());
  delete GetHostIGC();

  return S_OK;
}



/*-------------------------------------------------------------------------
 * CAdminGame::get_Users()
 *-------------------------------------------------------------------------
 * Purpose:
 *   Returns the collection of users currently in the game.   
 * 
 */
STDMETHODIMP CAdminGame::get_Users(/*[out, retval]*/ IAdminUsers** ppUsers)
{
  assert(GetHostIGC());
  //  CLEAROUT(ppGame, (IAdminGame*)NULL);

  RETURN_FAILED(CAdminSponsor<CAdminUsers>::Make(IID_IAdminUsers,
    (void**)ppUsers));

  //
  // finish construction
  //
  CAdminSponsor<CAdminUsers>::GetLimb()->SetMission(GetIGC());

  return S_OK;
}


/*-------------------------------------------------------------------------
 * CAdminGame::get_GameOwnerUser()
 *-------------------------------------------------------------------------
 * Purpose:
 *   Returns the user that owns the game.   
 * 
 */
STDMETHODIMP CAdminGame::get_GameOwnerUser(/*[out, retval]*/ IAdminUser** ppUser)
{
  // Initialize the [out] parameter
  *ppUser = NULL;
  // CLEAROUT(ppUser, (IAdminUser*)NULL);

  ImissionIGC * pMission = GetIGC();

  if (!pMission || !(pMission->GetShips()) || pMission->GetShips()->n() == 0)
    return S_OK;

  CFSMission * pCFSMission = GetHostIGC();

  if (!pCFSMission)
    return S_OK;

  CFSPlayer* pfsPlayer = pCFSMission->GetOwner();

  if (!pfsPlayer)
    return S_OK;

  RETURN_FAILED(pfsPlayer->CAdminSponsor<CAdminUser>::Make(IID_IAdminUser, (void**)ppUser));

  // finish construction
  pfsPlayer->CAdminSponsor<CAdminUser>::GetLimb()->Init(pfsPlayer);

  return S_OK;
}



/*-------------------------------------------------------------------------
 * StartGame()
 *-------------------------------------------------------------------------
 */
STDMETHODIMP CAdminGame::StartGame()
{
    assert(GetIGC());
    assert(GetHostIGC());
    if (GetIGC()->GetMissionStage() == STAGE_NOTSTARTED || GetIGC()->GetMissionStage() == STAGE_STARTING)
    {
        GetHostIGC()->StartGame();
    }
    return S_OK;
}

/*-------------------------------------------------------------------------
 * GetDescription()
 *-------------------------------------------------------------------------
 *
 * Sample format:
 *
 *  a-markcu1\2\ADMIN\991008\15:30:22
 *  curtc2\0\USER\990908\01:00:59
 * 
 */
STDMETHODIMP CAdminGame::get_Description(BSTR * pbstrDescription)
{
    assert(GetIGC());
    assert(GetHostIGC());

    char szDescrip[128];

    // cache machine name because this never changes
    static char szMachine[32] = { 0 };
    if (szMachine[0] == 0)
    {
        if (gethostname(szMachine, 32) != 0)
        {
            return Error("Cannont determine machine name.");
        }
    }                    

    SYSTEMTIME * st = GetHostIGC()->GetStartTime();

    // seperator is '\\' because it should be a character not allowed in a machine name

    sprintf(szDescrip, "%s\\%d\\%s\\%02d%02d%02d\\%02d:%02d:%02d\\%0.64s",
        szMachine,
        GetIGC()->GetMissionID(),
        GetIGC()->GetMissionParams()->bObjectModelCreated ? "ADMIN" : "USER",
        st->wYear % 100,
        st->wMonth,
        st->wDay,
        st->wHour,
        st->wMinute,
        st->wSecond,
        GetIGC()->GetMissionParams()->strGameName);
    
    CComBSTR bstr(szDescrip);

    // Detach the string to the [out] parameter
    CLEAROUT(pbstrDescription, (BSTR)bstr);
    bstr.Detach();

    return S_OK;
}



/*-------------------------------------------------------------------------
 * StartGame()
 *-------------------------------------------------------------------------
 */
STDMETHODIMP CAdminGame::StartCountdown()
{
    assert(GetHostIGC());
    GetHostIGC()->StartCountdown(c_fMissionBriefingCountdown);
    return S_OK;
}



/*-------------------------------------------------------------------------
 * SetTeamName()
 *-------------------------------------------------------------------------
 */
STDMETHODIMP CAdminGame::SetTeamName(int iSideID, BSTR bstrName)
{
    assert(GetIGC());
    assert(GetHostIGC());
    if(!GetIGC()->GetSide(iSideID))
      return S_OK;
    USES_CONVERSION;
    GetHostIGC()->SetSideName(iSideID, OLE2A(bstrName));
    return S_OK;
}


/*-------------------------------------------------------------------------
 * OverrideTechBit()
 *-------------------------------------------------------------------------
 */
STDMETHODIMP CAdminGame::OverrideTechBit(int iSideID, int nTechBitID, BOOL bSetting)
{
    assert(GetIGC());
    if (GetIGC()->GetMissionStage() == STAGE_NOTSTARTED && iSideID < c_cSidesMax && iSideID >= 0)
    {
        CFSMission * pfsmission = GetHostIGC();

        pfsmission->MakeOverrideTechBits();

        (pfsmission->m_pttbmAltered)[iSideID].SetBit(nTechBitID);

        if (bSetting)
          (pfsmission->m_pttbmNewSetting)[iSideID].SetBit(nTechBitID);
        else
          (pfsmission->m_pttbmNewSetting)[iSideID].ClearBit(nTechBitID);

        debugf("\nBits: %d %d\n", int((pfsmission->m_pttbmAltered)[iSideID].GetBit(nTechBitID)), int((pfsmission->m_pttbmNewSetting)[iSideID].GetBit(nTechBitID)));
    }
    return S_OK;
}

/*-------------------------------------------------------------------------
 * SetDetailsFiles()
 *-------------------------------------------------------------------------
 */
STDMETHODIMP CAdminGame::SetDetailsFiles(BSTR bstrName)
{
    assert(GetIGC());
    assert(GetHostIGC());
    USES_CONVERSION;
    GetHostIGC()->SetDetailsFiles(OLE2A(bstrName));
    return S_OK;
}


/*-------------------------------------------------------------------------
 * RandomizeTeams()
 *-------------------------------------------------------------------------
 */
STDMETHODIMP CAdminGame::RandomizeTeams()
{
  if (GetHostIGC()->GetStage() != STAGE_NOTSTARTED)
    return Error(L"This method can only be called when the game is not started.", IID_IAdminGame, 0);

  GetHostIGC()->RandomizeSides();
  return S_OK;
}

