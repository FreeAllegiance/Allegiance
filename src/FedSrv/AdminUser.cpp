
/*-------------------------------------------------------------------------
 * fedsrv\AdminUser.CPP
 * 
 * Implementation of CAdminUser
 * 
 * Owner: 
 * 
 * Copyright 1986-1999 Microsoft Corporation, All Rights Reserved
 *-----------------------------------------------------------------------*/



#include "pch.h"


/////////////////////////////////////////////////////////////////////////////
// CAdminUser

TC_OBJECT_EXTERN_NON_CREATEABLE_IMPL(CAdminUser)


/////////////////////////////////////////////////////////////////////////////
// ISupportErrorInfo Interface Methods

STDMETHODIMP CAdminUser::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IAdminUser
	};
	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}



/*-------------------------------------------------------------------------
 * Init()
 *-------------------------------------------------------------------------
 * Purpose:
 *   Finish construction  
 * 
 */

//  FIX: remove me
  void CAdminUser::Init(CFSPlayer *pPlayer) 
{
  m_pPlayer=pPlayer;
  m_pIshipIGC=pPlayer->GetIGCShip();
} 


/*-------------------------------------------------------------------------
 * get_Name()
 *-------------------------------------------------------------------------
 * Purpose:
 *   Return the name (of the user)  
 * 
 */
STDMETHODIMP CAdminUser::get_Name(BSTR* pbstr)
{
   CComBSTR bstrTemp(m_pPlayer->GetName()); 

  *pbstr = bstrTemp.Copy();
  
  return S_OK;
}


/*-------------------------------------------------------------------------
 * get_Ship()
 *-------------------------------------------------------------------------
 * Purpose:
 *    Return the ship that the player is piloting
 * 
 */
STDMETHODIMP CAdminUser::get_Ship(/*[out, retval]*/ IAdminShip** ppShip)
{
  //  CLEAROUT(ppAGCTeam, (IAGCTeam*)NULL);

  ZAssert(m_pPlayer);

  IshipIGC * pIshipIGC = m_pPlayer->GetIGCShip();

  if (pIshipIGC == NULL) // just in case
  {
    *ppShip = NULL;
    return S_OK;
  }
  
  return GetAGCGlobal()->GetAGCObject(pIshipIGC, IID_IAdminShip,
    (void**)ppShip);
}


/*-------------------------------------------------------------------------
 * SendMsg()
 *-------------------------------------------------------------------------
 * Purpose:
 *    Send a message to a User (aka player)
 * 
 */
STDMETHODIMP CAdminUser::SendMsg(BSTR bstrMessage)
{
  ZAssert(m_pPlayer);

  USES_CONVERSION;
 
  m_pPlayer->GetMission()->GetSite()->SendChat(NULL,                                // no FEDMESSAGE
                                               CHAT_INDIVIDUAL,                     // send to User
                                               m_pPlayer->GetIGCShip()->GetObjectID(),
                                               NA,                                  //No voiceover
                                               OLE2A(bstrMessage),
                                               c_cidNone,
                                               NA,
                                               NA,
                                               NULL,
                                               true);                    // true means object model generated

	return S_OK;
}


/*-------------------------------------------------------------------------
 * get_isReady()
 *-------------------------------------------------------------------------
 * Purpose:
 *    Get the value of the CAdminUser property.
 * 
 */
STDMETHODIMP CAdminUser :: get_isReady(VARIANT_BOOL* pVal)
{
  ZAssert(m_pPlayer);
  *pVal = VARBOOL(m_pPlayer->GetReady());
  return S_OK;
}

/*-------------------------------------------------------------------------
 * put_isReady()
 *-------------------------------------------------------------------------
 * Purpose:
 *    Put the value of the CAdminUser property.
 * 
 */
STDMETHODIMP CAdminUser :: put_isReady(VARIANT_BOOL Val)
{
  ZAssert(m_pPlayer);
  m_pPlayer->SetReady(!!Val);
  return S_OK;
}


/*-------------------------------------------------------------------------
 * Boot()
 *-------------------------------------------------------------------------
 * Purpose:
 *   Log off user.  (atually sends to lobby)
 *
 * Remarks:
 *   This will eventually cause this CAdminUser to be deleted
 * 
 */
STDMETHODIMP CAdminUser::Boot()
{
  if (m_pPlayer)
  {
    CFSMission * pfsMission = m_pPlayer->GetMission();
    pfsMission->RemovePlayerFromMission(m_pPlayer, QSR_AdminBooted);
    m_pPlayer = NULL;
  }

  return S_OK;
}


/*-------------------------------------------------------------------------
 * get_UserID()
 *-------------------------------------------------------------------------
 * Purpose:
 *   Returns to ship id.
 */
STDMETHODIMP CAdminUser::get_UserID(AdminUserID *userid)
{                                              
  *userid = DetermineID(m_pPlayer); 

  return S_OK;
}


/*-------------------------------------------------------------------------
 * get_UniqueID()
 *-------------------------------------------------------------------------
 * Purpose:
 *   Returns an AGC unique id.
 */
STDMETHODIMP CAdminUser::get_UniqueID(AGCUniqueID *uniqueID)
{                                              
  *uniqueID = static_cast<AGCUniqueID>(static_cast<USHORT>(DetermineID(m_pPlayer)) | static_cast<DWORD>(AGC_AdminUser << 16));

  return S_OK;
}


/*-------------------------------------------------------------------------
 * get_UniqueID()
 *-------------------------------------------------------------------------
 * Purpose:
 *   Returns an AGCEvent object used to store the current statistics of the
 *   player.
 */
STDMETHODIMP CAdminUser::get_PlayerStats(IAGCEvent** ppStats)
{
  // Create an AGCEvent object to store the named player statistics
  IAGCEventCreatePtr spEvent;
  RETURN_FAILED(spEvent.CreateInstance(CLSID_AGCEvent));
  RETURN_FAILED(spEvent->Init());

  // Get the PlayerScoreObject
  PlayerScoreObject* ppso = m_pPlayer->GetPlayerScoreObject();

  // Populate the event with the (short) named player statistics
  CComVariant varShort(static_cast<short>(0));
  V_I2(&varShort) = ppso->GetPilotBaseKills();
  RETURN_FAILED(spEvent->AddProperty(CComBSTR(L"PilotBaseKills"), &varShort));
  V_I2(&varShort) = ppso->GetPilotBaseCaptures();
  RETURN_FAILED(spEvent->AddProperty(CComBSTR(L"PilotBaseCaptures"), &varShort));
  V_I2(&varShort) = ppso->GetTechsRecovered();
  RETURN_FAILED(spEvent->AddProperty(CComBSTR(L"TechsRecovered"), &varShort));
  V_I2(&varShort) = ppso->GetFlags();
  RETURN_FAILED(spEvent->AddProperty(CComBSTR(L"Flags"), &varShort));
  V_I2(&varShort) = ppso->GetArtifacts();
  RETURN_FAILED(spEvent->AddProperty(CComBSTR(L"Artifacts"), &varShort));
  V_I2(&varShort) = ppso->GetRescues();
  RETURN_FAILED(spEvent->AddProperty(CComBSTR(L"Rescues"), &varShort));
  V_I2(&varShort) = ppso->GetKills();
  RETURN_FAILED(spEvent->AddProperty(CComBSTR(L"Kills"), &varShort));
  V_I2(&varShort) = ppso->GetAssists();
  RETURN_FAILED(spEvent->AddProperty(CComBSTR(L"Assists"), &varShort));
  V_I2(&varShort) = ppso->GetDeaths();
  RETURN_FAILED(spEvent->AddProperty(CComBSTR(L"Deaths"), &varShort));
  V_I2(&varShort) = ppso->GetEjections();
  RETURN_FAILED(spEvent->AddProperty(CComBSTR(L"Ejections"), &varShort));

  // Populate the event with the (float) named player statistics
  CComVariant varFloat(static_cast<float>(0.f));
  V_R4(&varFloat) = ppso->GetCombatRating();
  RETURN_FAILED(spEvent->AddProperty(CComBSTR(L"CombatRating"), &varFloat));
  V_R4(&varFloat) = ppso->GetWarpsSpotted();
  RETURN_FAILED(spEvent->AddProperty(CComBSTR(L"WarpsSpotted"), &varFloat));
  V_R4(&varFloat) = ppso->GetAsteroidsSpotted();
  RETURN_FAILED(spEvent->AddProperty(CComBSTR(L"AsteroidsSpotted"), &varFloat));
  V_R4(&varFloat) = ppso->GetMinerKills();
  RETURN_FAILED(spEvent->AddProperty(CComBSTR(L"MinerKills"), &varFloat));
  V_R4(&varFloat) = ppso->GetBuilderKills();
  RETURN_FAILED(spEvent->AddProperty(CComBSTR(L"BuilderKills"), &varFloat));
  V_R4(&varFloat) = ppso->GetLayerKills();
  RETURN_FAILED(spEvent->AddProperty(CComBSTR(L"LayerKills"), &varFloat));
  V_R4(&varFloat) = ppso->GetCarrierKills();
  RETURN_FAILED(spEvent->AddProperty(CComBSTR(L"CarrierKills"), &varFloat));
  V_R4(&varFloat) = ppso->GetPlayerKills();
  RETURN_FAILED(spEvent->AddProperty(CComBSTR(L"PlayerKills"), &varFloat));
  V_R4(&varFloat) = ppso->GetBaseKills();
  RETURN_FAILED(spEvent->AddProperty(CComBSTR(L"BaseKills"), &varFloat));
  V_R4(&varFloat) = ppso->GetBaseCaptures();
  RETURN_FAILED(spEvent->AddProperty(CComBSTR(L"BaseCaptures"), &varFloat));

  // Return the new object
  return spEvent->QueryInterface(IID_IAGCEvent, (void**)ppStats);
}


/*-------------------------------------------------------------------------
 * DetermineID()
 *-------------------------------------------------------------------------
 * Purpose:
 *   Return a 16 bit unique ID for a User
 */
/*static*/AdminUserID  CAdminUser::DetermineID(CFSPlayer * pPlayer)
{
    int nTemp = (int) pPlayer ^ (int) (pPlayer->GetConnection()->GetID());

    return (AdminUserID) nTemp; // return least sig 16 bits
}
