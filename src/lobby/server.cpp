/*-------------------------------------------------------------------------
  Server.cpp
  
  Per server stuff, including site for servers session
  
  Owner: 
  
  Copyright 1986-2000 Microsoft Corporation, All Rights Reserved
 *-----------------------------------------------------------------------*/

#include "pch.h"
#include <string.h> // mmf added for strncmp used below
#include "client.h" // BT - 9/11/2010 - Included to use URL functions.
#include <regex> // BT - 1/27/2012 - using regex to parse rank from callsign when auth is turned off.
#include <inttypes.h> // BT - STEAM

const DWORD        CFLServer::c_dwID        = 19680815;
const CFLMission * CFLServer::c_AllMissions = (CFLMission*) -1;

// mmf old C style (i.e. I did not stick this in a class) function to check lobby.cfg file
// for allowed servers.  I used the existing GetPrivateProfileString functions, not the most
// efficient as it pulls the info one line at a time but it is more than quick enough
// for this check
bool IsServerAllowed(const char *ip)
{
	char    config_dir[MAX_PATH];
	char    config_file_name[MAX_PATH];

	GetCurrentDirectory(MAX_PATH,config_dir);

	sprintf(config_file_name,"%s\\Allegiance.cfg",config_dir);

	const char * c_szCfgApp = "Lobby";
    char numservStr[128], ftStr[128], szStr[128];
    
    GetPrivateProfileString(c_szCfgApp, "NumberOfServers", "", 
                                  numservStr, sizeof(numservStr), config_file_name);

    GetPrivateProfileString(c_szCfgApp, "FilterType", "", 
                                  ftStr, sizeof(ftStr), config_file_name);

	int numServers=0, ipLen=0, ftLen=0, i=0;
	bool bFilterTypeAllow=true;
	char key[128];
	
	ipLen = strlen(ip);
    ftLen =	strlen(ftStr);

	if (ftLen>0) {
	    if (!strncmp("Block",ftStr,ftLen)) bFilterTypeAllow=false;
		else bFilterTypeAllow=true;
	}

	if (strlen(numservStr)>0) {
		// find out how many servers are in the file
		numServers = atoi(numservStr);
		for (i=1; i<=numServers; i++) {
			sprintf(key,"Server%d",i);
			GetPrivateProfileString(c_szCfgApp, key, "", 
                                  szStr, sizeof(szStr), config_file_name);
			if (!strncmp(ip,szStr,ipLen)) {
			// found match in list now figure out if we block it or allow it	
			   if (bFilterTypeAllow) return true;
			   else return false;
			}
		}
		// if we got this far we did not find a match
		if (bFilterTypeAllow) return false;
		else return true;
	} else {
		// no entry so allow all servers
		return true;
	}
}

// end mmf


HRESULT LobbyServerSite::OnAppMessage(FedMessaging * pthis, CFMConnection & cnxnFrom, FEDMESSAGE * pfm)
{
  CFLServer * pServer = CFLServer::FromConnection(cnxnFrom);
  assert(pServer);

  cnxnFrom.ResetAbsentCount();
  
  switch (pfm->fmid)
  {
    case FM_S_LOGON_LOBBY:
    {
      CASTPFM(pfmLogon, S, LOGON_LOBBY, pfm);
	  pfmLogon->cbvStaticCoreInfo;
	  char szRemote[64];
      if (pfmLogon->verLobby == LOBBYVER_LS)
      {
        if(pfmLogon->dwPort != 0)						// A port of 0 means the server couldn't find out its listening port
          pServer->SetServerPort(pfmLogon->dwPort);
        else
          pServer->SetServerPort(6073);				// Tell the client to enum the old fashioned way
		//KGJV #114
		// fill in StaticCoreInfo
		StaticCoreInfo* pcoreinfo  = (StaticCoreInfo*)FM_VAR_REF(pfmLogon, vStaticCoreInfo);
		pServer->SetStaticCoreInfo(pfmLogon->cStaticCoreInfo, pcoreinfo);
		//Imago: reorganized some stuff to get a better debug message		
		// location
		char * szLoc =  FM_VAR_REF(pfmLogon, szLocation);
		pServer->SetLocation(szLoc);
		//Imago #2 6/10
		char * szPrivilegedUsers =  FM_VAR_REF(pfmLogon, szPrivilegedUsers);
		pServer->SetPrivilegedUsers(szPrivilegedUsers);
		//Imago #62 6/10
		char * szVersion =  FM_VAR_REF(pfmLogon, szServerVersion);
		pServer->SetVersion(szVersion);
		// max games
		pServer->SetMaxGamesAllowed(pfmLogon->MaxGames);
		
		// current games (allow servers to update) - Imago 6/25/08
		pServer->SetCurrentGamesCount(pfmLogon->CurGames);

		// rebuild the master core list
		g_pLobbyApp->BuildStaticCoreInfo();
		// break; mmf took out break so we can check ip below
		// KGJV moved mmf's stuff inside lobby version check
		// mmf add check to see if they are an allowed or blocked server

		  pthis->GetIPAddress(cnxnFrom, szRemote);
		  debugf("FM_S_LOGON_LOBBY: %s:%d loc:%s #games:%i\n",&szRemote,pfmLogon->dwPort,pServer->GetLocation(),pfmLogon->CurGames);
		  
		  // BT - STEAM
		  g_pLobbyApp->CheckAndUpdateDrmHashes(true);

		  if (!strncmp("127.0.0.1",szRemote,9)) break;  // check for loopback and always allow
		  if (IsServerAllowed(szRemote)) break;
	  }

	  char * szReason;

	  // if we got this far we are not on the approved list fall through to reject below
	  szReason = "Your server IP address is not approved for connection to this Lobby.  Please contact the Lobby Admin."; //Imago fix typo 6/10
	  // end mmf
	  
      if (pfmLogon->verLobby > LOBBYVER_LS)
        szReason = "The Public Lobby server that you connected to is older than your version.  The Zone needs to update their lobby server.  Please try again later.";
      if (pfmLogon->verLobby < LOBBYVER_LS) // mmf took out the else and made this an explicit if for above szReason
        szReason = "Your server's version did not get auto-updated properly.  Please try again later.";

      BEGIN_PFM_CREATE(*pthis, pfmNewMissionAck, L, LOGON_SERVER_NACK)
          FM_VAR_PARM((char *)szReason, CB_ZTS)
      END_PFM_CREATE
      pthis->SendMessages(&cnxnFrom, FM_GUARANTEED, FM_FLUSH);
      pthis->DeleteConnection(cnxnFrom);
      break;
    }

    case FM_S_NEW_MISSION:
    {
      // a server has created a mission of their own volition. We need to map it into our "cookie space"
      CASTPFM(pfmNewMission, S, NEW_MISSION, pfm);
      CFLMission * pMission = pServer->CreateMission(NULL); // NULL = no client created this
      BEGIN_PFM_CREATE(*pthis, pfmNewMissionAck, L, NEW_MISSION_ACK)
      END_PFM_CREATE
      pfmNewMissionAck->dwIGCMissionID = pfmNewMission->dwIGCMissionID;
      pfmNewMissionAck->dwCookie = (DWORD) pMission;
      pthis->SendMessages(&cnxnFrom, FM_GUARANTEED, FM_FLUSH);
      // we won't broadcast it until the server sends us a lobby mission info, when it's good and ready
	  debugf("FM_S_NEW_MISSION id:%d cookie:%d\n",pfmNewMissionAck->dwIGCMissionID,pfmNewMissionAck->dwCookie);
    }
    break;

    case FM_LS_LOBBYMISSIONINFO:
    {
      CASTPFM(pfmLobbyMissionInfo, LS, LOBBYMISSIONINFO, pfm);
      CFLMission * pMission = CFLMission::FromCookie(pfmLobbyMissionInfo->dwCookie);
      if (pMission) // if it's already gone away--just ignore it. 
      {		  
		  //Imago 6/26/08
		  //Moved this code inside the mission check
		  //now the lobby does not crash trying to find a disconnected server's IP


			//KGJV #114 - server didnt fill szServerAddr but only reserved the bits. We fill it here.
			debugf("FM_LS_LOBBYMISSIONINFO:%d (pmission:%x cookie:%x) sent cookie:%x connected?%i\n",pfmLobbyMissionInfo->dwPort,pMission,pfmLobbyMissionInfo->dwCookie,pfmLobbyMissionInfo->dwCookie,(pthis->IsConnected()) ? 1 : 0);  
			char szAddr[64];
			pthis->GetIPAddress(cnxnFrom, szAddr); // get the real addr
			debugf("\tFM_LS_LOBBYMISSIONINFO:%s sent port %d\n",&szAddr,pfmLobbyMissionInfo->dwPort);		
			char *pfmdata = FM_VAR_REF(pfmLobbyMissionInfo, szServerAddr); // get the addr in the message	  
			Strcpy(pfmdata,szAddr); // overwrite with the real addr

			//end move code

        pMission->SetLobbyInfo(pfmLobbyMissionInfo);
        pMission->NotifyCreator();
      }
      // else TODO: do something about waiting client, if there is one
    }
    break;

    case FM_LS_MISSION_GONE:
    {
      CASTPFM(pfmMissionGone, LS, MISSION_GONE, pfm);
      CFLMission * pMission = CFLMission::FromCookie(pfmMissionGone->dwCookie);
	  debugf("deleting GONE mission: %x\n",pfmMissionGone->dwCookie);
      pServer->DeleteMission(pMission);
    }
    break;

    case FM_S_HEARTBEAT:
      // don't boot for missing roll call until we get one from them
      pServer->SetHere();
    break;
    
    case FM_S_PLAYER_JOINED:
    {
      CASTPFM(pfmPlayerJoined, S, PLAYER_JOINED, pfm);
      CFLMission * pMission = CFLMission::FromCookie(pfmPlayerJoined->dwMissionCookie);
      const char* szCharacterName = FM_VAR_REF(pfmPlayerJoined, szCharacterName);
      const char* szCDKey = FM_VAR_REF(pfmPlayerJoined, szCDKey);
	  const char* szAddress = FM_VAR_REF(pfmPlayerJoined, szAddress);

      if (NULL == szCharacterName || '\0' != szCharacterName[pfmPlayerJoined->cbszCharacterName-1])
          /* || NULL == szCDKey || '\0' != szCDKey[pfmPlayerJoined->cbszCDKey-1]  
		  Imago 6/25/08 removed above
		  */ 
      {
        // corrupt data
        g_pLobbyApp->GetSite()->LogEvent(EVENTLOG_ERROR_TYPE, LE_CorruptPlayerJoinMsg,
          cnxnFrom.GetName());
      }
      else if (NULL == pMission)
      {
        // the requested game does not exist
        g_pLobbyApp->GetSite()->LogEvent(EVENTLOG_WARNING_TYPE, LE_PlayerJoinInvalidMission,
          szCharacterName, cnxnFrom.GetName(), pfmPlayerJoined->dwMissionCookie);
      }
      else
      {
        if (g_pLobbyApp->EnforceCDKey())
        {
          char * szUnencryptedCDKey = (char*)_alloca(strlen(szCDKey) + 1);
          ZUnscramble(szUnencryptedCDKey, szCDKey, szCharacterName);

          szCDKey = szUnencryptedCDKey;
        }

        g_pLobbyApp->SetPlayerMission(szCharacterName, szCDKey, pMission, szAddress);
      }
    }
    break;

    case FM_S_PLAYER_QUIT:
    {
      CASTPFM(pfmPlayerQuit, S, PLAYER_QUIT, pfm);
      CFLMission * pMission = CFLMission::FromCookie(pfmPlayerQuit->dwMissionCookie);
      const char* szCharacterName = FM_VAR_REF(pfmPlayerQuit, szCharacterName);

      if (NULL == szCharacterName || '\0' != szCharacterName[pfmPlayerQuit->cbszCharacterName-1])
      {
        // corrupt data
        g_pLobbyApp->GetSite()->LogEvent(EVENTLOG_ERROR_TYPE, LE_CorruptPlayerQuitMsg,
          cnxnFrom.GetName());
      }
      else
        g_pLobbyApp->RemovePlayerFromMission(szCharacterName, pMission);
    }
    break;

    case FM_S_PAUSE:
    {
      CASTPFM(pfmPause, S, PAUSE, pfm);
      pServer->Pause(pfmPause->fPause);
	  // KGJV #114 cant create game on paused server
      g_pLobbyApp->BuildStaticCoreInfo();
      break;
    }    

	// BT - 12/21/2010 - CSS integration
	case FM_LS_PLAYER_RANK:
	{
		CASTPFM(pfmPlayerRankRequest, LS, PLAYER_RANK, pfm);

		const char* szCharacterName = FM_VAR_REF(pfmPlayerRankRequest, szCharacterName);
		const char* szReason = FM_VAR_REF(pfmPlayerRankRequest, szReason);
		const char* szPassword = FM_VAR_REF(pfmPlayerRankRequest, szPassword);
		const char* szCDKey = FM_VAR_REF(pfmPlayerRankRequest, szCDKey);

		char szRankName[50];
		int rankNameLen = sizeof(szRankName);
		int rank = 0;
		double sigma = 0;
		double mu = 0;
		int commandRank = 0;
		double commandSigma = 0;
		double commandMu = 0;

		// BT - 1/27/2012 - Enables lobby to return ranks when ACSS is disabled using old callsign(rank) format.
		std::tr1::regex rgx("(\\W+)?((?:\\w|@)+)(\\((\\d+)\\))?");
		std::tr1::smatch result;
		std::string charName(szCharacterName);

		if(std::tr1::regex_search(charName, result, rgx) == true)
		{
			if(result.size() > 2)
				sprintf((char *) szCharacterName, "%s%s", result[1].str().c_str(), result[2].str().c_str());
			
			if(result.size() > 4)
			{
				char rankString[50];
				sprintf(rankString, "%s", result[4].str().c_str());
				rank = atoi(rankString);
			}
		}
		

		BEGIN_PFM_CREATE(*pthis, pfmPlayerRankResponse, LS, PLAYER_RANK)
			FM_VAR_PARM(PCC(szCharacterName), CB_ZTS) 
			FM_VAR_PARM(PCC(szReason), CB_ZTS) 
			FM_VAR_PARM(PCC(szPassword), CB_ZTS) 
			FM_VAR_PARM(PCC(szCDKey), CB_ZTS) 
			FM_VAR_PARM(PCC(szRankName), CB_ZTS) 
		END_PFM_CREATE

		pfmPlayerRankResponse->characterID = pfmPlayerRankRequest->characterID;
		pfmPlayerRankResponse->fCanCheat = pfmPlayerRankRequest->fCanCheat;
		pfmPlayerRankResponse->fRetry = pfmPlayerRankRequest->fRetry;
		pfmPlayerRankResponse->dwCookie = pfmPlayerRankRequest->dwCookie;
		pfmPlayerRankResponse->fValid = pfmPlayerRankRequest->fValid;
		pfmPlayerRankResponse->dwConnectionID = pfmPlayerRankRequest->dwConnectionID;
		pfmPlayerRankResponse->rank = rank;
		pfmPlayerRankResponse->sigma = sigma;
		pfmPlayerRankResponse->mu = mu;
		pfmPlayerRankResponse->commandRank = commandRank;
		pfmPlayerRankResponse->commandSigma = commandSigma;
		pfmPlayerRankResponse->commandMu = commandMu;
		
		debugf("Client: %s from <%s> at time %u. Rank: %ld\n", g_rgszMsgNames[pfm->fmid], cnxnFrom.GetName(), Time::Now(), pfmPlayerRankResponse->rank);

		pthis->SendMessages(&cnxnFrom, FM_GUARANTEED, FM_FLUSH);

		break;
	}

	// BT - STEAM - Enable the lobby to handle chat logging.
	case FM_S_LOG_CHAT_MESSAGE:
	{
		CASTPFM(pfmLogChatMessage, S, LOG_CHAT_MESSAGE, pfm);

		std::string missionName = FM_VAR_REF_STDSTRING(pfmLogChatMessage, MissionName);
		std::string sourceName = FM_VAR_REF_STDSTRING(pfmLogChatMessage, SourceName);
		std::string targetName = FM_VAR_REF_STDSTRING(pfmLogChatMessage, TargetName);
		std::string message = FM_VAR_REF_STDSTRING(pfmLogChatMessage, Message);
		std::string sourceIP = FM_VAR_REF_STDSTRING(pfmLogChatMessage, SourceIP);

		debugf("%s %s->%s: %s\n", missionName.c_str(), sourceName.c_str(), targetName.c_str(), message.c_str());

		std::string url = g_pLobbyApp->GetChatLogUploadUrl();
		std::string host = url.substr(0, url.find_first_of("/", 8));
		std::string path = url.substr(url.find_first_of("/", 8));
		httplib::Client client(host);
		
		std::string postData = "apiKey=" + std::string(g_pLobbyApp->GetApiKey());
		
		postData += "&missionName=" + missionName;
		postData += "&sourceName=" + sourceName;
		postData += "&targetName=" + sourceName;

		postData += "&sourceIP=" + sourceIP;

		char steamID[256];
		sprintf(steamID, "%" PRIu64, pfmLogChatMessage->sourceSteamID);
		postData += "&sourceSteamID=" + std::string(steamID);

		sprintf(steamID, "%" PRIu64, pfmLogChatMessage->targetSteamID);
		postData += "&targetSteamID=" + std::string(steamID);

		postData += "&message=" + message;

		httplib::Result result = client.Post(path, postData, "application/x-www-form-urlencoded");
		int response = result->status;


		break;
	}


    default:
      ZError("unknown message\n");
  }
  return S_OK;
}


HRESULT LobbyServerSite::OnSysMessage(FedMessaging * pthis) 
{
  return S_OK;
}


void    LobbyServerSite::OnMessageNAK(FedMessaging * pthis, DWORD dwTime, CFMRecipient * prcp) 
{}


HRESULT LobbyServerSite::OnNewConnection(FedMessaging * pthis, CFMConnection & cnxn) 
{
  char szRemote[64];
  pthis->GetIPAddress(cnxn, szRemote);
  CFLServer * pServer = new CFLServer(&cnxn);
  g_pLobbyApp->GetSite()->LogEvent(EVENTLOG_INFORMATION_TYPE, LE_ServerConnected, cnxn.GetName(), szRemote);

  // tell them what token to use
  BEGIN_PFM_CREATE(*pthis, pfmToken, L, TOKEN)
    FM_VAR_PARM(g_pLobbyApp->GetToken(), CB_ZTS)
  END_PFM_CREATE
  pthis->SendMessages(&cnxn, FM_GUARANTEED, FM_FLUSH);
  return S_OK;
}


HRESULT LobbyServerSite::OnDestroyConnection(FedMessaging * pthis, CFMConnection & cnxn) 
{
  // connection is already gone, so we can't get address
  g_pLobbyApp->GetSite()->LogEvent(EVENTLOG_INFORMATION_TYPE, LE_ServerDisconnected, cnxn.GetName());
  delete CFLServer::FromConnection(cnxn);
  // server gone, rebuild the master core list
  g_pLobbyApp->BuildStaticCoreInfo();
  return S_OK;
}


HRESULT LobbyServerSite::OnSessionLost(FedMessaging * pthis) 
{
  g_pLobbyApp->GetSite()->LogEvent(EVENTLOG_ERROR_TYPE, LE_ServersSessionLost);
  return S_OK;
}


int LobbyServerSite::OnMessageBox(FedMessaging * pthis, const char * strText, const char * strCaption, UINT nType)
{
  debugf("LobbyServerSite::OnMessageBox: "); 
  return g_pLobbyApp->OnMessageBox(strText, strCaption, nType);
}


#ifndef NO_MSG_CRC
void LobbyServerSite::OnBadCRC(FedMessaging * pthis, CFMConnection & cnxn, BYTE * pMsg, DWORD cbMsg)
{
  assert(0); // we should never get a bad crc from one of our own servers.
}
#endif


// ********** CFLServer *************

CFLServer::CFLServer(CFMConnection * pcnxn) :
  m_dwID(c_dwID),
  m_pcnxn(pcnxn),
  m_sPort(6703),		// Mdvalley: initialize to default enum, just in case
  m_cPlayers(0),
  m_maxLoad(300), // get from reg or something
  m_bHere(false),
  m_fPaused(false),
  m_cStaticCoreInfo(0),    // KGJV #114
  m_vStaticCoreInfo(NULL), // KGJV #114
  m_dwStaticCoreMask(0),   // KGJV #114
  m_iMaxGames(20),		   // KGJV #114
  m_iCurGames(0)	// Imago
{
  assert(m_pcnxn);
  m_pcnxn->SetPrivateData((DWORD) this); // set up two-way link between connection and this

  m_pCounters = g_pLobbyApp->AllocatePerServerCounters(pcnxn->GetName());  

  Strcpy(m_szLocation,"unknown"); // KGJV #114 - default location
}

CFLServer::~CFLServer()
{
  DeleteMission(c_AllMissions);
  m_pcnxn->SetPrivateData(0); // disconnect two-way link between connection and this

  g_pLobbyApp->FreePerServerCounters(m_pCounters);
    
  // make sure all missions on this server are dead
  MissionList::Iterator iter(m_missions);
  while (!iter.End())
  { 
    delete iter.Value();
    iter.Remove(); 
  }
  // KGJV #114
  FreeStaticCoreInfo();
}

void CFLServer::DeleteMission(const CFLMission * pMission)
{
  if (!pMission)
    return;

  if (c_AllMissions == pMission)
    g_pLobbyApp->RemoveAllPlayersFromServer(this);

  MissionList::Iterator iter(m_missions);
  while (!iter.End())
  { 
    if (c_AllMissions == pMission || iter.Value() == pMission)
    {
      if (c_AllMissions != pMission)
        g_pLobbyApp->RemoveAllPlayersFromMission(iter.Value());

      delete iter.Value();
      iter.Remove();
    }
    else
      iter.Next();
  } 
}

void CFLServer::Pause(bool fPause)
{
  if (m_fPaused == fPause)
    return;

  m_fPaused = fPause;
  MissionList::Iterator iter(m_missions);
  
  if (m_fPaused) // hide the games
  {
    g_pLobbyApp->GetSite()->LogEvent(EVENTLOG_INFORMATION_TYPE, LE_ServerPause,
        GetConnection()->GetName());

    while (!iter.End())
    { 
      BEGIN_PFM_CREATE(g_pLobbyApp->GetFMClients(), pfmMissionGone, LS, MISSION_GONE)
      END_PFM_CREATE
      pfmMissionGone->dwCookie = iter.Value()->GetCookie();
      iter.Next();
    } 
    g_pLobbyApp->GetFMClients().SendMessages(g_pLobbyApp->GetFMClients().Everyone(), FM_GUARANTEED, FM_FLUSH);
  }
  else // show the games
  {
    g_pLobbyApp->GetSite()->LogEvent(EVENTLOG_INFORMATION_TYPE, LE_ServerContinue, 
        GetConnection()->GetName());

    while (!iter.End())
    {
      FMD_LS_LOBBYMISSIONINFO * plmi = iter.Value()->GetMissionInfo();
      if (plmi)
        g_pLobbyApp->GetFMClients().QueueExistingMsg(plmi);
      iter.Next();
    }
    g_pLobbyApp->GetFMClients().SendMessages(g_pLobbyApp->GetFMClients().Everyone(), FM_GUARANTEED, FM_FLUSH);
  }
}

