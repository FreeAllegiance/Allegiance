/*-------------------------------------------------------------------------
  fslobby.cpp
  
  site for FedSrvLobbySite
  
  Owner: 
  
  Copyright 1986-2000 Microsoft Corporation, All Rights Reserved
 *-----------------------------------------------------------------------*/
#include "pch.h"
#include "FedSrv.H"

HRESULT FedSrvLobbySite::OnAppMessage(FedMessaging * pthis, CFMConnection & cnxnFrom, FEDMESSAGE * pfm)
{
  HRESULT hr = S_OK;
  ZAssert(&g.fmLobby == pthis);

  static CTempTimer timerOnAppMessage("in FedSrvLobbySite::OnAppMessage", .02f);
  timerOnAppMessage.Start();

  switch(pfm->fmid)
  {
	  // KGJV #114 - reactivate create mission
    //#if !defined(ALLSRV_STANDALONE)
      case FM_L_CREATE_MISSION_REQ:
      {
#if !defined(SRV_CHILD)
        CASTPFM(pfmCreateMissionReq, L, CREATE_MISSION_REQ, pfm);
        MissionParams mp;
		lstrcpy(mp.strGameName,    ZString(FM_VAR_REF(pfmCreateMissionReq, GameName)));// + "'s game");
		lstrcpy(mp.szIGCStaticFile,ZString(FM_VAR_REF(pfmCreateMissionReq, IGCStaticFile)));
        mp.bScoresCount = true; // BT - 9/17 - Scores always count. 
		mp.iMaxImbalance = 0x7ffe;// added
        ZAssert(!mp.Invalid());
#endif

		//Imago - give birth right here, feed it and off it goes...

#if defined(SRV_PARENT)
		//start missions as thier own process
		STARTUPINFO si;
		PROCESS_INFORMATION pi;
		ZeroMemory( &si, sizeof(si) );	
		si.cb = sizeof(si);				
		ZeroMemory( &pi, sizeof(pi) );	
		char szCmd[255]; char szName[32]; char szFile[32];
		ZString strName = mp.strGameName;
		ZString strFile = mp.szIGCStaticFile;
		si.lpTitle = mp.strGameName;
		Strcpy(szName,(PCC)strName);
		Strcpy(szFile,(PCC)strFile);
		sprintf(szCmd,"AllSrv.exe \"%s\" %s %x",szName,szFile,pfmCreateMissionReq->dwCookie);
		
		// Create a NULL dacl to give "everyone" access
		SECURITY_DESCRIPTOR sd;
		SECURITY_ATTRIBUTES sa = {sizeof(sa), &sd, false};
		InitializeSecurityDescriptor(&sd, SECURITY_DESCRIPTOR_REVISION);
		SetSecurityDescriptorDacl(&sd, true, NULL, FALSE);

		if( !CreateProcess(
			NULL,                   // No module name (use command line). 
			szCmd,					
			NULL,                   
			NULL,                   
			FALSE,                  // Set handle inheritance to FALSE. 
			CREATE_NEW_PROCESS_GROUP|CREATE_NEW_CONSOLE, // we're destined to do amazing things
			NULL,                   // Use parent's environment block. 
			NULL,                   // Use parent's starting directory. 
			&si,                    // Pointer to STARTUPINFO structure.
			&pi )                   // Pointer to PROCESS_INFORMATION structure.
		) 
		{
			debugf( "CreateProcess failed (%d).\n", GetLastError() );
		}

		// check to make sure the child is ready before restart
 		char strFilename[10] = "\0";
		sprintf(strFilename,"%d.pid",pi.dwProcessId);
        HANDLE hFile = (HANDLE)CreateFile(strFilename, 0, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_NO_BUFFERING, NULL);
		DWORD dwError = GetLastError();
		int i = 0;
		while(hFile == INVALID_HANDLE_VALUE || dwError == ERROR_FILE_NOT_FOUND) {	
			if (i >= 14) //30s
				break;

			Sleep(2500);
			hFile = (HANDLE)CreateFile(strFilename, 0, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_NO_BUFFERING, NULL);
			dwError = GetLastError();
			i++;
		}
		_Module.AddPID(pi.dwProcessId);
		CloseHandle(hFile);
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
		g.bRestarting = true;
		PostQuitMessage(0);

#endif 
	
#if !defined(SRV_CHILD)
#if !defined(SRV_PARENT)
		// pkk 2011-07-24 - Create games only with IGCs from cores.txt
		bool ValidCore = false;
		for ( int i = 0; i<g.cStaticCoreInfo; i++ )
		{
			if ( !lstrcmpi(mp.szIGCStaticFile, g.vStaticCoreInfo[i].cbIGCFile ) )
			{
				ValidCore = true;
				break; // quit loop
			}
		}
		if ( ValidCore == true )
		{
			//Imago - start the mission in this thread as usual 
			FedSrvSite * psiteFedSrv = new FedSrvSite();
			CFSMission * pfsMissionNew = new CFSMission(mp, "", psiteFedSrv, psiteFedSrv, NULL, NULL);
			pfsMissionNew->SetCookie(pfmCreateMissionReq->dwCookie);
		}
		else
		{
			debugf("Lobby sent invalid core information %s, ignoring message\n", mp.szIGCStaticFile);
		}
		// pkk end
#endif
#endif
      } //Imago 6/22/08
      break;
    //#endif -- reactivate create mission

    case FM_L_NEW_MISSION_ACK:
    {
      CASTPFM(pfmNewMissionAck, L, NEW_MISSION_ACK, pfm);
      CFSMission * pfsm = CFSMission::GetMissionFromIGCMissionID(pfmNewMissionAck->dwIGCMissionID);
      if (pfsm)
      {
        pfsm->SetCookie(pfmNewMissionAck->dwCookie);
        // If we already have players (e.g. reconnect), tell the lobby who's in the game
        const ShipListIGC * plistShip = pfsm->GetIGCMission()->GetShips();
        for (ShipLinkIGC * plinkShip = plistShip->first(); plinkShip; plinkShip = plinkShip->next())
        {
          IshipIGC * pShip = plinkShip->data();
          CFSShip * pfsShip = GETFSSHIP(pShip);
          if (pfsShip->IsPlayer() && !pShip->IsGhost())
          {
            CFSPlayer* pfsPlayer = pfsShip->GetPlayer();
            BEGIN_PFM_CREATE(g.fmLobby, pfmPlayerJoined, S, PLAYER_JOINED)
              FM_VAR_PARM(pfsPlayer->GetName(), CB_ZTS)
            END_PFM_CREATE
            pfmPlayerJoined->dwMissionCookie = pfsm->GetCookie();
          }
        }
        HRESULT hr = pthis->SendMessages(pthis->GetServerConnection(), FM_GUARANTEED, FM_FLUSH);
      }
    }
    break;

    case FM_L_TOKEN:
    {
      CASTPFM(pfmToken, L, TOKEN, pfm);
      Strcpy(g.m_szToken, FM_VAR_REF(pfmToken, Token));
    }
    break;
    
    case FM_L_REMOVE_PLAYER:
    {
      CASTPFM(pfmRemovePlayer, L, REMOVE_PLAYER, pfm);
      CFSMission * pfsMission = CFSMission::GetMission(pfmRemovePlayer->dwMissionCookie);
      const char* szCharacterName = FM_VAR_REF(pfmRemovePlayer, szCharacterName);
      const char* szMessageParam = FM_VAR_REF(pfmRemovePlayer, szMessageParam);
      
	  // BT - STEAM - enable a nice message to be displayed to the banned user.
	  QuitSideReason quitSideReason;

	  switch (pfmRemovePlayer->reason)
	  {
	  case RPR_duplicateCDKey:
		  quitSideReason = QSR_DuplicateCDKey;
		  break;

	  case RPR_bannedBySteam:
		  quitSideReason = QSR_BannedBySteam;
		  break;

	  default:
		  quitSideReason = QSR_DuplicateRemoteLogon;
		  break;
	  }

      // try to find the player in question
      if (!pfsMission)
      {        
        debugf("Asked to boot character %s from mission %x by lobby, "
          "but the mission was not found.\n", 
          szCharacterName, pfmRemovePlayer->dwMissionCookie);
      }
      else if (!pfsMission->RemovePlayerByName(szCharacterName, quitSideReason, szMessageParam))
      {
        debugf("Asked to boot character %s from mission %x by lobby, "
          "but the character was not found.\n", 
          szCharacterName, pfmRemovePlayer->dwMissionCookie);
      }
    }
    break;

    case FM_L_LOGON_SERVER_NACK:
    {
      char * szReason;

      CASTPFM(pfmLogonNack, L, LOGON_SERVER_NACK, pfm);
      
      szReason = FM_VAR_REF(pfmLogonNack, Reason);

      OnMessageBox(pthis, szReason ? szReason : "Error while try to log onto server.", "Allegiance Server Error", MB_SERVICE_NOTIFICATION);
      // TODO: consider firing out an event message
      PostQuitMessage(-1);
    }

	// BT - 12/21/2010 ACSS - The lobby server will relay the rank details back to the game server.
	case FM_LS_PLAYER_RANK:
	{
		CQLogonStats * pquery = new CQLogonStats(GotLogonDetails);
		CQLogonStatsData * pqd = pquery->GetData();

		CASTPFM(pfmPlayerRank, LS, PLAYER_RANK, pfm);

		lstrcpy(pqd->szCDKey, FM_VAR_REF(pfmPlayerRank, szCDKey));
		lstrcpy(pqd->szCharacterName, FM_VAR_REF(pfmPlayerRank, szCharacterName));
		lstrcpy(pqd->szPassword, FM_VAR_REF(pfmPlayerRank, szPassword));
		lstrcpy(pqd->szReason, FM_VAR_REF(pfmPlayerRank, szReason));

		pqd->characterID = pfmPlayerRank->characterID;
		pqd->fCanCheat = pfmPlayerRank->fCanCheat;
		pqd->fRetry = pfmPlayerRank->fRetry;
		pqd->dwCookie = pfmPlayerRank->dwCookie;
		pqd->fValid = pfmPlayerRank->fValid;
		pqd->dwConnectionID = pfmPlayerRank->dwConnectionID;

		pqd->rank = pfmPlayerRank->rank;
		pqd->sigma = pfmPlayerRank->sigma;
		pqd->mu = pfmPlayerRank->mu;
		pqd->commandRank = pfmPlayerRank->commandRank;
		pqd->commandSigma = pfmPlayerRank->commandSigma;
		pqd->commandMu = pfmPlayerRank->commandMu;

		PostThreadMessage(g.idReceiveThread, wm_sql_querydone, (WPARAM) NULL, (LPARAM) pquery);
		break;
	}

	// BT - STEAM
	case FM_L_UPDATE_DRM_HASHES:
	{
		CASTPFM(pfmUpdateDrmHashes, L, UPDATE_DRM_HASHES, pfm);

		char drmDownloadUrl[MAX_PATH];
		Strcpy(drmDownloadUrl, FM_VAR_REF(pfmUpdateDrmHashes, DrmDownloadUrl));

		debugf("received drm update request with url: %s\n", drmDownloadUrl);

		IHTTPSession * pHTTPSession = CreateHTTPSession(NULL);

		const char * szFileList[] = { drmDownloadUrl, "drm_hashes.txt",  NULL };
		pHTTPSession->InitiateDownload(szFileList, "."); // . means current path

		if (pHTTPSession)
		{
			debugf("Downloading drm hash file...");

			// we block as we download the cfg file; should be okay since we are not connected to the zone.
			while (pHTTPSession->ContinueDownload())
			{
				Sleep(100);
			}

			debugf("Done\n");

			//
			// At this point we are done downloading cfg file
			//
			bool bErrorOccurred = pHTTPSession->GetLastErrorMessage() ? true : false;
			char szErrorMsg[512];
			if (bErrorOccurred)
				_snprintf(szErrorMsg, sizeof(szErrorMsg), "\nError Connecting to Zone.  You will not be able to host a public game. \r\n%s", pHTTPSession->GetLastErrorMessage());

			delete pHTTPSession;
		}

		break;
	}




    break;
  }

  timerOnAppMessage.Stop("...for message type %s\n", g_rgszMsgNames[pfm->fmid]);
  return hr;
}


int FedSrvLobbySite::OnMessageBox(FedMessaging * pthis, const char * strText, const char * strCaption, UINT nType)
{
  return g.siteFedSrv.OnMessageBox(pthis, strText, strCaption, nType); // don't need a separate handler
}

HRESULT FedSrvLobbySite::OnSessionLost(FedMessaging * pthis)
{
  _AGCModule.TriggerEvent(NULL, AllsrvEventID_LostLobby, "", -1, -1, -1, 0);
  // KGJV: close the connexion
  if (pthis)
	pthis->Shutdown();
  return S_OK;
}

#ifndef NO_MSG_CRC
void FedSrvLobbySite::OnBadCRC(FedMessaging * pthis, CFMConnection & cnxn, BYTE * pMsg, DWORD cbMsg)
{
  ZAssert(0); // we should never get a bad crc from one of our own servers.
  // todo: something better for people running their own servers, but sufficient for our servers, 
  // since they communicate on a back channel
}
#endif

