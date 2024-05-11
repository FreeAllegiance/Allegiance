/*-------------------------------------------------------------------------
  mission.h
  
  Per mission stuff on lobby side
  
  Owner: 
  
  Copyright 1986-2000 Microsoft Corporation, All Rights Reserved
 *-----------------------------------------------------------------------*/

#pragma once

#include "messagesall.h"

class CFLServer;
class CFLClient;

class CFLMission : public IObject
{
public:
  CFLMission(CFLServer * pServer, CFLClient * pClientCreator);
  ~CFLMission();
  bool IsValidThisPtr() // this
  {
    bool fValid = this && 
                  !IsBadReadPtr(this, sizeof(*this)) &&
                  IMPLIES(m_plmi, (!IsBadReadPtr(m_plmi, sizeof(m_plmi)) && !IsBadReadPtr(m_plmi, sizeof(m_plmi->cbmsg)))) &&
                   (c_dwID == m_dwID);
    // this should only be false if a hacked client or timing issue
    return fValid;
  }

  DWORD GetCookie()
  {
    return (DWORD) this;
  }
  FMD_LS_LOBBYMISSIONINFO * GetMissionInfo()
  {
    return m_plmi;
  }

  CFLServer * GetServer()
  {
    return m_pServer;
  }

  void SetLobbyInfo(FMD_LS_LOBBYMISSIONINFO * plmi);

  int GetPlayerCount()
  {
    return m_cPlayers;
  }

  void AddPlayer();

  void RemovePlayer();

  CFLClient * GetCreator()
  {
    return m_pClientCreator;
  }

  void NotifyCreator();

  static CFLMission * FromCookie(DWORD dwCookie)
  {
    CFLMission * pMission = (CFLMission*) dwCookie;
    if (pMission->IsValidThisPtr())
      return pMission;
    else
      return NULL;
  }

    
  
private:
  static const DWORD c_dwID;
  FMD_LS_LOBBYMISSIONINFO * m_plmi;
  CFLServer *               m_pServer;
  CFLClient *               m_pClientCreator; // to notify them when the game is ready to go
  bool                      m_fNotifiedCreator;
  DWORD                     m_dwID;
  int                       m_cPlayers;
};

typedef TList<CFLMission*> MissionList;