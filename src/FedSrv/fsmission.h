/*-------------------------------------------------------------------------
 * FSMission.h
 * 
 * Class header for managing missions
 * 
 * Owner: 
 * 
 * Copyright 1986-1998 Microsoft Corporation, All Rights Reserved
 *-----------------------------------------------------------------------*/

#ifndef _FSMISSION_H_
#define _FSMISSION_H_
#include "pch.h"

inline unsigned char   SideMask(SideID sid)
{
    assert (sid >= 0);
    return (unsigned char)(0x01 << sid);
}
inline unsigned char   SideMask(IsideIGC* pside)
{
    assert (pside);
    return SideMask(pside->GetObjectID());
}

class CFSMission;
class CFSPlayer;
class CFSSide;
class CAdminGame;
class CAdditionalAGCParamData;

typedef Slist_utl<CFSMission*> ListFSMission;
typedef Slink_utl<CFSMission*> LinkFSMission;

struct JoinRequest
{
  CFSPlayer * pfsPlayer;
  IsideIGC *  pSide;
};

typedef Slist_utl<JoinRequest*> ListJoinReq;
typedef Slink_utl<JoinRequest*> LinkJoinReq;

class IMissionSite : IObject
{
public:
  virtual void Create(CFSMission * pfsMission) {}
  virtual void Destroy(CFSMission * pfsMission) {}
};

class OldPlayerInfo
{
    public:
        OldPlayerInfo(void)
        :
            pso(true),
            sideID(NA),
            pclusterLifepod(NULL)
        {
        }

        ~OldPlayerInfo(void)
        {
        }

        PlayerScoreObject           pso;
        int                         characterID;
        SideID                      sideID;         //Simply using this as a cookie
        char                        name[c_cbName];
        IclusterIGC*                pclusterLifepod;
        Vector                      positionLifepod;
        unsigned char               bannedSideMask;
		// mdvalley: Track last flown for
		SideID						lastSide;
};
typedef Slist_utl<OldPlayerInfo>    OldPlayerList;
typedef Slink_utl<OldPlayerInfo>    OldPlayerLink;

// mmf/KGJV 09/07 allow only one ballot of each type at a time
typedef int BallotType;
#define BALLOT_RESIGN      1
#define BALLOT_OFFERDRAW   2
#define BALLOT_ACCEPTDRAW  3
#define BALLOT_MUTINY      4

class Ballot
{
public:
    
  // do any steps needed to update the current ballot.  Returns true iff 
  // the ballot is no longer needed.
  virtual bool Update(const Time& now);
  
  // cancels the pending vote
  virtual void Cancel();

  // records the given vote from a player
  virtual void CastVote(CFSPlayer* pfsPlayer, bool bVote);

  // gets the ID of this ballot
  BallotID GetBallotID();

  // return the ballot type  
  BallotType GetType();      // mmf/KGJV 09/07 allow only one ballot of each type at a time

  // destructor
  virtual ~Ballot() {};

protected:

  // initializes the ballot for a given vote proposed by a player to their team
  void Init(CFSPlayer* pfsInitiator, const ZString& pzProposal, const ZString& strBallotText);

  // initializes the ballot for a given vote proposed by a team to all other teams
  void Init(CFSSide* pfsideInitiator, const ZString& pzProposal, const ZString& strBallotText);

  // gets a string describing the current tally of votes
  ZString GetTallyString();

  // performs the appropriate result when the vote passes
  virtual void OnPassed();

  // performs the appropriate result when the vote passes
  virtual void OnFailed();

  // tests whether the vote passes
  bool HasPassed();

  // tests to see if we have received all of the votes
  bool AllVotesAreIn();

  // the group in question
  CFSMission*   m_pmission;
  ChatTarget    m_chattarget;
  ObjectID      m_groupID;
  CFMRecipient* m_pgroup;

  // the text of the proposal
  ZString m_strProposal;

  // the tallies of votes by side
  int m_cInFavor[c_cSidesMax];
  int m_cOpposed[c_cSidesMax];
  int m_cAbstaining[c_cSidesMax];

  // the ships that can still vote
  TVector<ShipID, DefaultEquals> m_vShips;

  // the ID of this ballot
  BallotID m_ballotID;

  // the ID of the next ballot to be created.
  static BallotID s_ballotIDNext;

  // the expiration time of the ballot
  Time m_timeExpiration;

  bool m_bCanceled;

  // KGJV #110
  bool m_bHideToLeader;

  // mmf/KGJV 09/07 allow only one ballot of each type at a time
  BallotType m_type;

};

typedef TList<Ballot*> BallotList;

// KGJV #110
class MutinyBallot : public Ballot
{
  IsideIGC* m_pside;
  ShipID m_idInitiatorShip;
public:
  MutinyBallot(CFSPlayer* pfsInitiator);
  virtual void OnPassed();
};

// a ballot used when a player suggests resigning
class ResignBallot : public Ballot
{
  IsideIGC* m_pside;
public:
  ResignBallot(CFSPlayer* pfsInitiator);
  virtual void OnPassed();
};

// a ballot used when a player suggests offering a draw
class OfferDrawBallot : public Ballot
{
  CFSSide* m_pfside;
public:
  OfferDrawBallot(CFSPlayer* pfsInitiator);
  virtual void OnPassed();
};

// a ballot used when one team offers a draw
class AcceptDrawBallot : public Ballot
{
public:
  AcceptDrawBallot(CFSSide* pfsideInitiator);
  virtual void OnPassed();
};

class CFSMission
{
public:
  CFSMission(
      const MissionParams& misparms,
      char * szDesc,
      IMissionSite * psiteMission,
      IIgcSite * psiteIGC,
      CAdditionalAGCParamData * paagcParamData,
      const char* pszStoryText
      );
  virtual ~CFSMission();
  static const ListFSMission * GetMissions()
  {
    return &s_list;
  }
  static CFSMission *   GetMission(DWORD dwCookie);
  static CFSMission *   GetMissionFromIGCMissionID(DWORD dwIGCMissionID);
  MissionID             GetMissionID()
  {
    return m_pMission->GetMissionID();
  }
  DWORD                 GetCookie()    
  {
    return m_misdef.dwCookie;
  }
  void                  SetCookie(DWORD dwCookie)
  {
    m_misdef.dwCookie = dwCookie;
    SetLobbyIsDirty();
  }
  void SetLobbyIsDirty();
  bool GetLobbyIsDirty()
  {
    return m_fLobbyDirty;
  }
  void UpdateLobby(Time now);
  ImissionIGC *         GetIGCMission()   
  {
    return m_pMission;
  }
  IIgcSite*             GetSite()         
  {
    return m_psiteIGC;
  }
  const FMD_S_MISSIONDEF * GetMissionDef() 
  {
    return &m_misdef;
  }
  void                  QueueLobbyMissionInfo();
  STAGE                 GetStage()        
  {
    return m_misdef.stage;
  }
  void                  SetStage(STAGE stage);

  void                  SetSideName(SideID sid, const char* szName);
  void                  SetSideSquad(SideID sid, SquadID squadID);
  CFSPlayer *           GetLeader(SideID sid);
  void                  SetLeader(CFSPlayer * pfsPlayer);
  void                  SetLeaderID(SideID sideID, ShipID shipID);
  CFSPlayer *           GetOwner();
  void					SetOwner(short iSide); // BT - WOPR
  void                  AddPlayerToMission(CFSPlayer * pfsPlayer);
  void                  RemovePlayerFromMission(CFSPlayer * pfsPlayer, QuitSideReason reason, const char* szMessageParam = NULL);
  void                  AddPlayerToSide(CFSPlayer * pfsPlayer, IsideIGC * pside);
  void                  RemovePlayerFromSide(CFSPlayer * pfsPlayer, QuitSideReason reason, const char* szMessageParam = NULL);
  bool                  RemovePlayerByName(const char* szCharacterName, QuitSideReason reason, const char* szMessageParam = NULL);
  bool                  RemovePlayerByCDKey(const char* szCDKey, QuitSideReason reason, const char* szMessageParam = NULL);
  int                   GetCountOfPlayers(IsideIGC * pside, bool bCountGhosts);
  int                   GetSideRankSum(IsideIGC * pside, bool bCountGhosts);	// TE: Added for balancing
  int                   GetRankThreshold();	// TE: Added for balancing
  bool                  HasPlayers(IsideIGC * pside, bool bCountGhosts);
  IstationIGC *         GetBase(IsideIGC * pside);
  int                   GetCountSides()   
  {
    assert(m_pMission->GetSides()->n() == m_misdef.misparms.nTeams);
    return m_misdef.misparms.nTeams;
  }
  void                  RecordGameResults();
  void                  RecordTeamResults(IsideIGC* pside, nlohmann::json* jGame = nullptr);
  void                  RecordPlayerResults(IshipIGC* pship, CFSPlayer *player, SideID sid, nlohmann::json* jGame = nullptr); // BT - STEAM - Enable the CSteamAchievements object that is attached to the player to be sent to the stats recording functions.
  void                  QueueGameoverMessage();

  IsideIGC*             CheckForVictoryByStationBuild(IsideIGC* pside);
  IsideIGC*             CheckForVictoryByStationCapture(IsideIGC* pside, IsideIGC* psideOld);
  IsideIGC*             CheckForVictoryByStationKill(IstationIGC* pstationKilled, IsideIGC* psideOld);

  IsideIGC*             CheckForVictoryByKills(IsideIGC* pside);
  IsideIGC*             CheckForVictoryByInactiveSides(bool& bAllSidesInactive);
  IsideIGC*             CheckForVictoryByFlags(IsideIGC* pside, SideID sidFlag);

  void                  GameOver(IsideIGC*      psideWin,
                                 const char*    pszReason);

  void                  SetMissionParams(const MissionParams & misparms);
  void                  StartCountdown(float fCountdownLength);
  void                  DelayCountdown(float fDelayLength);
  void                  StartGame();
  bool                  FAllReady();
  DelPositionReqReason  CheckPositionRequest(CFSPlayer * pfsPlayer, IsideIGC * pside);
  void                  RequestPosition(CFSPlayer * pfsPlayer, IsideIGC * pside, bool bRejoin);
  void                  VacateStation(IstationIGC * pstation);
  void                  SetAutoAccept(IsideIGC * pside, bool fAccept);
  bool                  GetAutoAccept(IsideIGC * pside) 
  {
    return !!m_misdef.rgfAutoAccept[pside->GetObjectID()];
  }
  void                  SetLockLobby(bool bLock);
  void                  SetLockSides(bool bLock);
  void                  SetMaxTeamImbalance(int imbalance); // mmf: added this to fmission.cpp
  bool                  GetLockSides();
  void                  FlushSides();	// TE: Added FlushSides function
  void                  RandomizeSides();

  void                  SetSideCiv(IsideIGC * pside, IcivilizationIGC * pciv);
  void					SetSideActive(SideID sideid, bool bActive); // KGJV #62
  bool					GetSideActive(SideID sideid);				// KGJV #62

  void					SetSideAllies(SideID sideid, char Allies);      // #ALLY
  char					GetSideAllies(SideID sideid);                   // #ALLY
  void                  UpdateAlliances(SideID sideID,SideID sideAlly); // #ALLY

  bool					GetAllowEmptyTeams() { return m_misdef.misparms.bAllowEmptyTeams;} // KGJV #62
  void					SetAllowEmptyTeams(bool bValue) { m_misdef.misparms.bAllowEmptyTeams = bValue;} // KGJV #62
  void                  DeactivateSide(IsideIGC * pside);
  void                  GiveSideMoney(IsideIGC * pside, Money money);
  void                  SetForceReady(SideID iSide, bool fForceReady);
  bool                  GetForceReady(SideID iSide) 
  {
    assert(iSide > NA); 
    return !!m_misdef.rgfForceReady[iSide];
  }
  bool                  GetReady(SideID iSide)      
  {
    assert(iSide > NA); 
    return !!m_misdef.rgfReady[iSide];
  }
  void                  PlayerReadyChange(CFSPlayer * pfsPlayer);
  bool                  RejectSideJoinRequests(IsideIGC * pSide);
  bool                  RemoveJoinRequest(CFSPlayer * pfsPlayer, IsideIGC * psideDest);
  SideID                PickNewSide(CFSPlayer* pfsPlayer, bool bAllowTeamLobby, unsigned char bannedSideMask);
  void                  DoTick(Time timeNow);
  void                  CreateCluster(IclusterIGC * pIclusterIGC);
  void                  DeleteCluster(IclusterIGC * pIclusterIGC);
  const std::vector<CFSCluster*> * GetFSClusters() 
  { 
    return &m_pFSClusters;
  }
  void                  CreateSide(IsideIGC * pIsideIGC);
  void                  DeleteSide(IsideIGC * pIsideIGC);
  const std::vector<CFSSide*> * GetFSSides() 
  { 
    return &m_pFSSides;
  }
  CFMGroup *            GetGroupRealSides() 
  {
    return m_pgrpSidesReal;
  }
  CFMGroup *            GetGroupLobbySide() 
  {
    CFSSide * pfsside = CFSSide::FromIGC(m_pMission->GetSide(SIDE_TEAMLOBBY));
    return pfsside ? pfsside->GetGroup() : NULL;
  }
  CFMGroup *            GetGroupMission() 
  {
    return m_pgrpMission;
  }

  DWORD                 IncrementFrame(void)
  {
      return m_nFrame++;
  }

  void                  SaveAsOldPlayer(CFSPlayer* pfsplayer, bool bBooted);

  // Masks for dwSendBootTo above
  static const DWORD    c_sbtPlayer;
  static const DWORD    c_sbtLeader;

  SYSTEMTIME * GetStartTime() { return &m_stStartTime;} // in UTC not PST

  float GetGameDuration() {return m_flGameDuration;}

  OldPlayerLink*    GetOldPlayerLink(const char* name)
  {
    for (OldPlayerLink* popl = m_oldPlayers.first(); (popl != NULL); popl = popl->next())
    {
        if (_stricmp(popl->data().name, name) == 0)
            return popl;
    }
    return NULL;
  }

  bool                  ShouldDelete() { return m_bShouldDelete; }

  bool                  GetScoresCount()
  {
    return m_misdef.misparms.bScoresCount;
  }

  void AddInvitation(SideID sid, char * szPlayerName);

  bool                  RequiresInvitation() 
  { 
    return m_nInvitationListID != 0; 
  }

  bool                  IsInvited(CFSPlayer * pPlayer);
  bool                  IsSquadGame()
  {
    return m_misdef.misparms.bSquadGame;
  }

  const ZString&        GetDetailsFiles()
  {
    return m_strDetailsFiles;
  }
  void SetDetailsFiles(const ZString& strDetailsFiles)
  {
    m_strDetailsFiles = strDetailsFiles;
    SetLobbyIsDirty();
  }

  void                  AddBallot(Ballot * pBallot);
  void                  TallyVote(CFSPlayer* pfsPlayer, BallotID ballotID, bool bVote);
  bool					HasBallots(BallotType iType); // mmf/KGJV 09/07 allow only one ballot of each type at a time

  void                  MakeOverrideTechBits(); // alloc memory for overriding tech bits as needed

  TechTreeBitMask *     m_pttbmAltered;     // this is actually a point to an array of TechTrees. This is for overriding starting tech bits.
  TechTreeBitMask *     m_pttbmNewSetting;  // We have two masks, one for which bits have changed, and one for the new bits

  IsideIGC*             GetSideWon(void) const { return m_psideWon; }

  const char*           GetStoryText() const {return m_strStoryText;}
  
private:
  void                  InitSide(SideID sid);
  void                  MaintainSquadLeadership(SideID sid);
  void                  ProcessGameOver();
  void                  CheckForSideAllReady(IsideIGC * pside);
  void                  SetReady(SideID iSide, bool fReady);
  void                  AddJoinRequest(CFSPlayer * pfsPlayer, IsideIGC * pside);
  void                  CreateDPGroups(IclusterIGC * pcluster);
  void                  SendLobbyMissionInfo(CFSPlayer * pfsPlayer);
  void                  SendMissionInfo(CFSPlayer * pfsPlayer, IsideIGC* pside);
  void                  DoPayday(void);
  void                  DoPayday(IsideIGC*  pside);
  void                  NotifyPlayerBoot(CFSPlayer * pfsPlayer, IsideIGC * pSide);
  void                  Vacate();

  static ListFSMission  s_list;
  static int            s_iMissionID;
  FMD_S_MISSIONDEF      m_misdef;           // cached and maintained for easily delivery on demand
  ImissionIGC *         m_pMission;         // gateway to everything that is IGC in THIS mission

  Time                  m_timeNextPayday;

  int                   m_rgMoney[c_cSidesMax];
  TRef<IsideIGC>        m_psideWon;         // set when a side won--not acted upon until out of igc updates
  const char*           m_pszReason;

  IMissionSite *        m_psiteMission;
  ListJoinReq           m_listJoinReq;

  IIgcSite*             m_psiteIGC;

  std::vector<CFSSide*> m_pFSSides;  // choosing vector for O(1) access by index
  std::vector<CFSCluster*> m_pFSClusters;  // choosing vector for O(1) access by index
  CFMGroup *            m_pgrpSidesReal; // includes all sides EXCEPT the lobby side
  CFMGroup *            m_pgrpMission;   // includes everyone in this mission, including lobby side
  Time                  m_timeLastLobbyMissionInfo;
  bool                  m_fLobbyDirty;
  bool                  m_bShouldDelete;

  SYSTEMTIME            m_stStartTime; // in UTC not PST

  DWORD                 m_nFrame;

  OldPlayerList         m_oldPlayers;

  int                   m_nInvitationListID; 
  ZString               m_strDetailsFiles;

  BallotList            m_ballots;

  // Stuff for fsmon:
  float                 m_flGameDuration;

  ZString               m_strStoryText;

  bool                  m_bDraw;
};

#endif 
