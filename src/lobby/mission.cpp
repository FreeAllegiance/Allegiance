/*-------------------------------------------------------------------------
  mission.cpp
  
  Per mission stuff implementation
  
  Owner: 
  
  Copyright 1986-2000 Microsoft Corporation, All Rights Reserved
 *-----------------------------------------------------------------------*/

#include "pch.h"

const DWORD CFLMission::c_dwID = 19680815; // random number to frame the data to recognize a good instance from a bad instance

CFLMission::CFLMission(CFLServer * pServer, CFLClient * pClientCreator) :
  m_plmi(NULL),
  m_dwID(c_dwID),
  m_pServer(pServer),
  m_cPlayers(0),
  m_pClientCreator(pClientCreator),
  m_fNotifiedCreator(false)
{
  assert(m_pServer);
  char szRemote[64];
  g_pLobbyApp->GetFMServers().GetIPAddress(*m_pServer->GetConnection(), szRemote);
  
  g_pLobbyApp->GetSite()->LogEvent(
        EVENTLOG_INFORMATION_TYPE, LE_MissionCreated, 
        GetCookie(), m_pServer->GetConnection()->GetName(), szRemote,
        m_pServer->GetPlayerCount(), 
        pClientCreator ? pClientCreator->GetConnection()->GetName() : "<ops>");
}

CFLMission::~CFLMission()
{
  g_pLobbyApp->GetSite()->LogEvent(
        EVENTLOG_INFORMATION_TYPE, LE_MissionGone, 
        GetCookie(),
        m_pServer->GetConnection()->GetName(), 
        m_pServer->GetPlayerCount());

  BEGIN_PFM_CREATE(g_pLobbyApp->GetFMClients(), pfmMissionGone, LS, MISSION_GONE)
  END_PFM_CREATE
  pfmMissionGone->dwCookie = GetCookie();
  g_pLobbyApp->GetFMClients().SendMessages(g_pLobbyApp->GetFMClients().Everyone(), FM_GUARANTEED, FM_FLUSH);
  
  if (m_plmi)
    HeapFree(GetProcessHeap(), 0, m_plmi);
}

void CFLMission::SetLobbyInfo(FMD_LS_LOBBYMISSIONINFO * plmi)
{
  HANDLE hHeap = GetProcessHeap();

  if (m_plmi)
  {
    HeapFree(hHeap, 0, m_plmi);
  }

  // adjust the start time from an offset of the current time to a real time
  plmi->dwStartTime += Time::Now().clock();

  CFLMission * pMission = CFLMission::FromCookie(plmi->dwCookie);
  if (pMission)
  {
    debugf("!!! Got FMD_LS_LOBBYMISSIONINFO for mission (cookie=%x) that I don't know about\n", plmi->dwCookie);
    // we could just pass the server in to this function, but this seems like a reasonable and cheap check & balance to make sure the mission maps back to the server
    CFLServer * pServer = pMission->GetServer();  
    assert(pServer);
    m_plmi = (FMD_LS_LOBBYMISSIONINFO*) HeapAlloc(hHeap, 0, plmi->cbmsg);
    CopyMemory(m_plmi, plmi, plmi->cbmsg);
	if (m_plmi->dwPort != pServer->GetServerPort() && m_plmi->dwPort != 0) {
		pServer->SetServerPort(m_plmi->dwPort); //Imago 6/23/08 override port
	}

	// never advertize paused games - imago 7/1/09 removed old checks
    // BT - 3/11/2024 only return missions with at least one player.
    if (!pServer->GetPaused() && (m_plmi->nNumPlayers > 0 || m_plmi->fMSArena)) //&& 
        //(g_pLobbyApp->EnforceCDKey() || m_plmi->nNumPlayers > 0 || m_plmi->fMSArena ||
		//(!g_pLobbyApp->IsFreeLobby() && strcmp(FM_VAR_REF(m_plmi,szIGCStaticFile),"zone_core"))  // -KGJV - advertise custom core game on FAZ
		//))
    { // don't advertize Allegiance Zone games until someone is actually in it unless it's admin created.
      FedMessaging & fmClients = g_pLobbyApp->GetFMClients();
      fmClients.ForwardMessage(fmClients.Everyone(), plmi, FM_GUARANTEED);
    }
  }
}


void CFLMission::AddPlayer()
{
  m_pServer->AddPlayer();
  ++m_cPlayers;
}


void CFLMission::RemovePlayer()
{
  m_pServer->RemovePlayer();
  --m_cPlayers;
  assert (m_cPlayers >= 0);
}


void CFLMission::NotifyCreator()
{
  if (!m_fNotifiedCreator && GetCreator())
  {
    // Tell the creator so they auto-join
    BEGIN_PFM_CREATE(g_pLobbyApp->GetFMClients(), pfmJoinMission, L, JOIN_MISSION)
    END_PFM_CREATE
    char szServer[64];
    g_pLobbyApp->GetFMServers().GetIPAddress(*GetServer()->GetConnection(), szServer);
    assert(lstrlen(szServer) < sizeof(pfmJoinMission->szServer)); // as long as szServer is fixed length
    Strcpy(pfmJoinMission->szServer, szServer);
    pfmJoinMission->dwCookie = GetCookie();
	pfmJoinMission->dwPort = GetServer()->GetServerPort();	// KGJV #114: pass the port to the client
    g_pLobbyApp->GetFMClients().SendMessages(GetCreator()->GetConnection(), FM_GUARANTEED, FM_FLUSH);
  }
  m_fNotifiedCreator = true;
}

