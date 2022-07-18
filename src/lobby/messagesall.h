/*-------------------------------------------------------------------------
 * \src\Lobby\MessagesAll.h
 * 
 * Messages that pass over more than one session
 * 
 * Owner: 
 * 
 * Copyright 1986-2000 Microsoft Corporation, All Rights Reserved
 *-----------------------------------------------------------------------*/

#ifndef _MESSAGES_ALL_ 
#define _MESSAGES_ALL_ 

#include "messagecore.h"

/*
  *************************************************
                 MESSAGES START HERE  

         Messages range for this file is 301 - 320

  *************************************************
*/

DEFINE_FEDMSG(LS, LOBBYMISSIONINFO, 301)
  FM_VAR_ITEM(szGameName); 
  FM_VAR_ITEM(rgSquadIDs);
  FM_VAR_ITEM(szGameDetailsFiles);
  FM_VAR_ITEM(szIGCStaticFile); // KGJV added
  FM_VAR_ITEM(szServerName); // KGJV #114 added
  FM_VAR_ITEM(szServerAddr); // KGJV #114 added
  FM_VAR_ITEM(szPrivilegedUsers); // Imago 6/10 #2
  FM_VAR_ITEM(szServerVersion); // Imago 7/10 #62
  DWORD			dwPort;  //multi port/process servers Imago
  DWORD         dwCookie; // how the mission is identified on the lobby
  unsigned      dwStartTime;
  short         nMinRank;
  short         nMaxRank;
  unsigned      nNumPlayers                     : 11;
  unsigned      nNumNoatPlayers                 : 11; //Imago #169
  unsigned      nMaxPlayersPerGame              : 11;
  unsigned      nMinPlayersPerTeam              : 8;
  unsigned      nMaxPlayersPerTeam              : 8;
  unsigned      nTeams                          : 3;
  bool          fCountdownStarted               : 1;
  bool          fInProgress                     : 1;
  bool          fMSArena                        : 1;
  bool          fScoresCount                    : 1;
  bool          fInvulnerableStations           : 1;
  bool          fAllowDevelopments              : 1;
  bool          fLimitedLives                   : 1;
  bool          fConquest                       : 1;
  bool          fDeathMatch                     : 1;
  bool          fCountdown                      : 1;
  bool          fProsperity                     : 1;
  bool          fArtifacts                      : 1;
  bool          fFlags                          : 1;
  bool          fTerritorial                    : 1;
  bool          fGuaranteedSlotsAvailable       : 1;
  bool          fAnySlotsAvailable              : 1;
  bool          fSquadGame                      : 1;
  bool          fEjectPods                      : 1;
END_FEDMSG

DEFINE_FEDMSG(LS, MISSION_GONE, 302)
    DWORD dwCookie;
END_FEDMSG

#ifndef SquadID
typedef int             SquadID;
#endif

class SquadMembership
{
public:
    SquadID m_squadID;
    char  m_szSquadName[c_cbName];
    bool  m_fIsLeader           : 1;
    bool  m_fIsAssistantLeader  : 1;

    SquadMembership() {} // creator beware--uninitialized values (but this is intentional for performance in allocating arrays that will be filled in)
    SquadMembership(SquadID squadID, const char *szSquadName, bool  fIsLeader, bool  fIsAssistantLeader) :
        m_squadID(squadID),
        m_fIsLeader(fIsLeader),
        m_fIsAssistantLeader(fIsAssistantLeader)
    {
        ZAssert(!(m_fIsLeader && m_fIsAssistantLeader));
        ZAssert(strlen(szSquadName) < c_cbName);
        Strcpy(m_szSquadName, szSquadName);
    };
    
    const SquadID GetID() const         { return m_squadID; };
    const char* GetName() const         { return m_szSquadName; };
    bool  GetIsLeader() const           { return m_fIsLeader; };
    bool  GetIsAssistantLeader() const  { return m_fIsAssistantLeader; };
};

typedef Slist_utl<SquadMembership*> SquadMembershipList;
typedef Slink_utl<SquadMembership*> SquadMembershipLink;

DEFINE_FEDMSG(LS, SQUAD_MEMBERSHIPS, 303)
  FM_VAR_ITEM(squadMemberships); // an array of SquadMembership structures
  int   cSquadMemberships;
END_FEDMSG

// KGJV #114
// only core file name atm, but in a struct for futur extensibility 
struct StaticCoreInfo
{
    char    cbIGCFile[c_cbFileName];
};


#endif // _MESSAGES_LS_


