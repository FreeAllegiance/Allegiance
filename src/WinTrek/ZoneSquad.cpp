/*-------------------------------------------------------------------------
 * wintrek\ZoneSquad.cpp
 * 
 * Implementation of IZoneSquad and IZonePlayer 
 *
 * These two classes are in the same file because are closely related.
 * 
 * Owner: 
 * 
 * Copyright 1986-2000 Microsoft Corporation, All Rights Reserved
 *-----------------------------------------------------------------------*/



#include "pch.h"


class CZonePlayerImpl;





class CZonePlayerImpl:
  public IZonePlayer
{

public:

    CZonePlayerImpl(const char *szName, int nID) :
      m_nID(nID),
      m_rank(IMemberBase::RANK_UNKNOWN)
    {
      strncpy(m_szName, szName, sizeof(m_szName)-1);
    }


    void Randomize() // fill in random data for testing
    {
#ifdef DEBUG
		// mdvalley: needs to be explicit int
    static int i = 0;

    m_rank = (Rank) (i % 3 + 1);

    switch (i % 3)
    {
      case 0:
        strcpy (m_szName, "John Doe");
        break;
      case 1:
        strcpy (m_szName, "Susan Wilson");
        break;
      case 2:
        strcpy (m_szName, "Bill Smith");
        break;
    }
  
    ++i;
#endif
    }

    char *            GetName()
    { 
      return m_szName; 
    }
    void              SetName(PCC szName)                     { strcpy(m_szName, szName); } 

    int               GetID()                                 { return m_nID; }
    void              SetID(int nID)                          { m_nID = nID; }

    char *            GetLastPlayedDate(IZoneSquad & squad);                     
    void              SetLastPlayedDate(IZoneSquad & squad, PCC szLastPlayedDate);

    Rank GetRank()
    { 
      return m_rank; 
    }
    void              SetRank(Rank rank)
    { 
      m_rank = rank;
    }
    float             GetHours(IZoneSquad & squad);   
    void              SetHours(IZoneSquad & squad, float fHours);   

    Position          GetPosition(IZoneSquad & squad);   
    void              SetPosition(IZoneSquad & squad, Position pos); // setting to leader should only be done from IZoneSquad::SetOwner()

    DetailedStatus    GetStatus(IZoneSquad & squad);


    bool IsMemberOf(IZoneSquad & squad) // is this player a member of pSquad
    {
        DetailedStatus status = GetStatus(squad);

        if (status == DSTAT_MEMBER || status == DSTAT_ASL || status == DSTAT_LEADER)
            return true;
        return false;
    }

    bool IsPendingFor(IZoneSquad & squad)
    {
        return GetStatus(squad) == DSTAT_PENDING;
    }
  
private:

  char              m_szName[40];
  int               m_nID;
  Rank              m_rank;
};






/*

  The spec called for the screen to know how many hours a player has been with a particular squad, 
  so I made CMembership.  It turns out that the database isn't going to support this, but I'm
  using CMembership for remembering position.

*/
class CMembership:
  public IObject
{
    TRef<IZonePlayer>            m_pPlayer;
    IZonePlayer::DetailedStatus  m_dstatus;
    char                         m_szLastPlayedDate[9]; 
//    float                        m_fHours;
    

public:

    CMembership() :
//      m_fHours(0),
      m_pPlayer(NULL),
      m_dstatus(IZonePlayer::DSTAT_NO_ASSOCIATION)
    {
      strcpy(m_szLastPlayedDate, "        ");
    }

//  void              SetHours(float fHours)    { asset(0);/*m_fHours = fHours; */}
//  float             GetHours()                { ZAssert(0); return 0;/*return m_fHours; */} 

  IZonePlayer::Position GetPosition()        
  { 
      return IZonePlayer::Position(m_dstatus - ((int)IZonePlayer::DSTAT_MEMBER - (int)IZonePlayer::POS_MEMBER));
  }
  void              SetPosition(IZonePlayer::Position pos) 
  { 
      m_dstatus = IZonePlayer::DetailedStatus(pos + (IZonePlayer::DSTAT_MEMBER - IZonePlayer::POS_MEMBER));
  }

  IMemberBase::DetailedStatus GetStatus()
  {
      return m_dstatus;
  }

  char *            GetLastPlayedDate()                     
  { 
    return m_szLastPlayedDate; 
  }

  void              SetLastPlayedDate(PCC szLastPlayedDate) 
  { 
    strncpy (m_szLastPlayedDate, szLastPlayedDate, sizeof(m_szLastPlayedDate));
  }

  void SetStatus(IMemberBase::DetailedStatus detailedstatus)
  {
      m_dstatus = detailedstatus;
  }
    
  TRef<IZonePlayer>     GetPlayer()                { return m_pPlayer; }
  void                  SetPlayer(TRef<IZonePlayer> pPlayer)  { m_pPlayer = pPlayer; }
};



class CZoneSquadImpl:
  public IZoneSquad 
{

public:

    char *GetName()             { return m_szName; }
    void  SetName(PCC szName)   { strcpy(m_szName, szName); }

    char *GetDescription()      { return m_szDescription; }
    void  SetDescription(PCC szDescription) { strcpy(m_szDescription, szDescription); }

    int   GetID()               { return m_nID; }
    void  SetID(int nID)        { m_nID = nID; }

    int   GetCreationID()       { return m_nCreationID; }
    void  SetCreationID(int nCreationID) { m_nCreationID = nCreationID; }

    char *GetURL()              { return m_szURL; }
    void  SetURL(PCC szURL)   { strcpy(m_szURL, szURL); }

    char *GetInceptionDate()    { return m_szInceptionDate; }
    void  SetInceptionDate(char *szInceptionDate)  { strcpy(m_szInceptionDate, szInceptionDate); }

    TRef<IZonePlayer>  GetOwner() 
    { 
      return m_pOwner; 
    }

    void  SetOwner(TRef<IZonePlayer> pOwner);

    const char * GetRank()             
    { 
      Rank rank = Rank(m_nScore / 100);

      if (rank < RANK_START || rank >= RANK_MAX)
          return Rank2String(RANK_UNKNOWN);

      return Rank2String(rank);
    }

    int   GetRanking(SQUAD_SORT_COLUMN column)               
    { 
      return m_nRanking[column]; 
    }
    void  SetRanking(SQUAD_SORT_COLUMN column, int nRanking) 
    { 
      m_nRanking[column] = nRanking; 
    }

    int   GetWins()             
    { 
      return m_nWins; 
    }
    void  SetWins(int nWins)    
    { 
      m_nWins = nWins; 
    }

    int   GetLosses()           
    { 
      return m_nLosses; 
    }
    void  SetLosses(int nLosses)
    { 
      m_nLosses = nLosses; 
    }

    CivID GetCivID()            
    { 
      return m_idCiv; 
    }
    void  SetCivID(int id)      
    { 
      m_idCiv = id; 
    }

    short GetScore()            
    { 
      return m_nScore; 
    }
    void  SetScore(short nScore)
    { 
      m_nScore = nScore; 
    }

    void  SetDetailsReceived() 
    {
      m_bDetailsReceived = true;
    }
    bool  WereDetailsReceived() 
    {
      return m_bDetailsReceived;
    }

    void AddPlayer(IZonePlayer & pPlayer);    // consider using SquadScreen::AddPlayerToSquad() over this
    void QuitPlayer(IZonePlayer & pPlayer); // consider using SquadScreen::RemovePlayerFromSquad() over this

    void RejectPlayer(IZonePlayer & pPlayer);
    void BootPlayer(IZonePlayer & pPlayer); 

    void AddJoinRequest(IZonePlayer & pPlayer);
    void CancelJoinRequest(IZonePlayer & pPlayer);

    void SetEmpty();

    void AddPlayersToList(XZonePlayers & list);

    void SetOwnershipLog(char * szLog)
    {
        m_listStrOwnershipLog.SetEmpty();

        if(!szLog)
            return;

        const char * szDelimit = ";"; 

        // set bLastOneTruncated
        int nLen = strlen(szLog);
        int c = 0;
        for (int i = 0; i < nLen; ++i)
            if (szLog[i] == ';') c++;
        bool bLastOneTruncated = c % 2 != 0;

        char * szToken;

        // establish string and get the first token
        szToken = strtok(szLog, szDelimit);

        while (szToken)
        {
            // szToken is Name
            ZString strLine(szToken);

            szToken = strtok(NULL, szDelimit);
            if (!szToken)
                break;

            strLine += "          ";

            // szToken is date
            strLine += szToken;

            szToken = strtok(NULL, szDelimit);

            m_listStrOwnershipLog.PushFront(strLine);
        }

        if(bLastOneTruncated)
            m_listStrOwnershipLog.PopFront();
    }

    void SetStatus(IZonePlayer & player, IMemberBase::DetailedStatus detailedstatus);

    void  AddPlayer(IZonePlayer & player, IMemberBase::DetailedStatus detailedstatus) 
    {
        SetStatus(player, detailedstatus);
    }

    TRef<CMembership> FindMembership(IZonePlayer & player);

    IMemberBase::DetailedStatus FindStatus(IZonePlayer & player);

    TList<ZString> * GetOwnershipLog() { return &m_listStrOwnershipLog; }

    CZoneSquadImpl() :
        m_rank(RANK_UNKNOWN),
        m_nWins(0),
        m_nLosses(0),
        m_idCiv(NA),
        m_nScore(0),
        m_bDetailsReceived(false), // false here means data for this squad is incomplete
        m_listMemberships(),
        m_bWasJustMade(false), // assume not recently made by interactive user
        m_pOwner(NULL)
    {
        for (int i = 0; i < SSC_MAX; ++i)
            m_nRanking[i] = -1;
        m_szName[0] = 0;
        m_szDescription[0] = 0;
        m_szURL[0] = 0;
        m_szInceptionDate[0] = 0;
    }

private:

    char            m_szName[32];
    char            m_szDescription[512];
    char            m_szURL[256];
    char            m_szInceptionDate[32]; 
    Rank            m_rank;
    int             m_nRanking[SSC_MAX];
    int             m_nWins;
    int             m_nLosses;
    CivID           m_idCiv;
    short           m_nScore;
    bool            m_bDetailsReceived; // have the details been gotten for the server yet?
    TRef<IZonePlayer> m_pOwner;
    XMemberships    m_listMemberships;  // members of, along with hours been with (and position data) 
    TList<ZString>  m_listStrOwnershipLog; // TODO: make this a a simple ZString, not a list
    int             m_nID;          // True ID determined by Zone database servers
    int             m_nCreationID;  // Creation ID is valid on client only
    bool            m_bWasJustMade; // mainly for interactive user
};

TRef<IZoneSquad> CreateZoneSquad()
{
    return new CZoneSquadImpl;
}


/*-------------------------------------------------------------------------
 * GetStatus()
 *-------------------------------------------------------------------------
 */
IMemberBase::DetailedStatus CZonePlayerImpl::GetStatus(IZoneSquad & squad)
{
  TRef<CMembership> pMembership = ((CZoneSquadImpl*)(&squad))->FindMembership(*this);

  if (pMembership)
      return pMembership->GetStatus();

  return DSTAT_NO_ASSOCIATION;
}

/*-------------------------------------------------------------------------
 * SetPosition()
 *-------------------------------------------------------------------------
 *  Remarks: setting pos to leader should only be done from IZoneSquad::SetOwner()
 */
void CZonePlayerImpl::SetPosition(IZoneSquad & squad, Position pos) 
{ 
  TRef<CMembership> pMembership = ((CZoneSquadImpl*)(&squad))->FindMembership(*this);

  ZAssert(pMembership); // Cannot set position if player in not a member of the squad

  if (pMembership)
  {
    pMembership->SetPosition(pos);
  }
} 


/*-------------------------------------------------------------------------
 * SetHours()
 *-------------------------------------------------------------------------
 */
void CZonePlayerImpl::SetHours(IZoneSquad & squad, float fHours)
{
    ZAssert(0); // not impl
}


/*-------------------------------------------------------------------------
 * GetPosition()
 *-------------------------------------------------------------------------
 */
IZonePlayer::Position CZonePlayerImpl::GetPosition(IZoneSquad & squad)   
{
  TRef<CMembership> pMembership = ((CZoneSquadImpl*)(&squad))->FindMembership(*this);

  if (pMembership)
  {
    return pMembership->GetPosition();
  }
  else return POS_UNKNOWN;
}


/*-------------------------------------------------------------------------
 * GetLastPlayedDate()
 *-------------------------------------------------------------------------
 */
char * CZonePlayerImpl::GetLastPlayedDate(IZoneSquad & squad)
{
  TRef<CMembership> pMembership = ((CZoneSquadImpl*)(&squad))->FindMembership(*this);

  if (pMembership)
  {
    return pMembership->GetLastPlayedDate();
  }
  else return "        ";
}


/*-------------------------------------------------------------------------
 * SetLastPlayedDate()
 *-------------------------------------------------------------------------
 */
void   CZonePlayerImpl::SetLastPlayedDate(IZoneSquad & squad, PCC szLastPlayedDate)
{
  TRef<CMembership> pMembership = ((CZoneSquadImpl*)(&squad))->FindMembership(*this);

  ZAssert(pMembership); // Cannot set position if player in not a member of the squad

  if (pMembership)
  {
    pMembership->SetLastPlayedDate(szLastPlayedDate);
  }
}


/*-------------------------------------------------------------------------
 * GetHours()
 *-------------------------------------------------------------------------
 */
float CZonePlayerImpl::GetHours(IZoneSquad & squad)   
{
    ZAssert(0); // not impl

    return 0;
}


/*-------------------------------------------------------------------------
 * MakePlayerList()
 *-------------------------------------------------------------------------
 */
void CZoneSquadImpl::AddPlayersToList(XZonePlayers & list)
{
    XMembershipsIt it(m_listMemberships);

    while(!it.End())
    { 
        if (!list.Find(it.Value()->GetPlayer())) // no duplicates
            list.PushEnd(it.Value()->GetPlayer());
        it.Next();
    }
}




/*-------------------------------------------------------------------------
 * FindMembership()
 *-------------------------------------------------------------------------
 */
TRef<CMembership> CZoneSquadImpl::FindMembership(IZonePlayer & player)
{
    XMembershipsIt it(m_listMemberships);

    while(!it.End())
    { 
        if (it.Value()->GetPlayer() == &player)
        {
            return it.Value();
        }
        it.Next();
    }
    return NULL;
}


/*-------------------------------------------------------------------------
 * FindStatus()
 *-------------------------------------------------------------------------
 */
IMemberBase::DetailedStatus CZoneSquadImpl::FindStatus(IZonePlayer & player)
{
    XMembershipsIt it(m_listMemberships);

    while(!it.End())
    { 
        if (it.Value()->GetPlayer() == &player)
        {
            return it.Value()->GetStatus();
        }
        it.Next();
    }
    return IMemberBase::DSTAT_NO_ASSOCIATION;
}

/*-------------------------------------------------------------------------
 * AddPlayer()
 *-------------------------------------------------------------------------
 * Note: consider using SquadScreen::AddPlayerToSquad() instead of this
 * to ensure proper screen refresh
 */
void CZoneSquadImpl::SetStatus(IZonePlayer & player, IMemberBase::DetailedStatus detailedstatus)
{
    TRef<CMembership> pMembership = FindMembership(player);
    if(!pMembership)
    {
        pMembership = new CMembership;
        m_listMemberships.PushEnd(pMembership);
        pMembership->SetPlayer(&player);
    }

    if (detailedstatus == IMemberBase::DSTAT_LEADER)
        SetOwner(&player);
    else
        pMembership->SetStatus(detailedstatus);
}


/*-------------------------------------------------------------------------
 * QuitPlayer()
 *-------------------------------------------------------------------------
 * Note: consider using SquadScreen::RemovePlayerFromSquad() instead of this
 * to ensure proper screen refresh
 */
void CZoneSquadImpl::QuitPlayer(IZonePlayer & player)
{
    TRef<CMembership> pMembership = FindMembership(player);

    if(pMembership)
    {
        if (m_pOwner == &player)
        {
          m_pOwner = NULL; // for now set to nobody
          //
          // Server will update who is owner later
          //
        }
        m_listMemberships.Remove(pMembership);
    }
}

/*-------------------------------------------------------------------------
 * AddJoinRequest()
 *-------------------------------------------------------------------------
 */
void CZoneSquadImpl::AddJoinRequest(IZonePlayer & player)
{
    SetStatus(player, IZonePlayer::DSTAT_PENDING);
}

/*-------------------------------------------------------------------------
 * SetEmpty() -- Clears all membership
 *-------------------------------------------------------------------------
 */
void CZoneSquadImpl::SetEmpty()
{
    m_listMemberships.SetEmpty();
}
 

/*-------------------------------------------------------------------------
 * RejectPlayer()
 *-------------------------------------------------------------------------
 *
 *    Used to reject 
 * 
 */
void CZoneSquadImpl::RejectPlayer(IZonePlayer & player)
{
    QuitPlayer(player);
}


/*-------------------------------------------------------------------------
 * BootPlayer()
 *-------------------------------------------------------------------------
 *
 *    Used to boot
 * 
 */
void CZoneSquadImpl::BootPlayer(IZonePlayer & player)
{
    QuitPlayer(player);
}



/*-------------------------------------------------------------------------
 * CancelJoinRequest()
 *-------------------------------------------------------------------------
 */
void CZoneSquadImpl::CancelJoinRequest(IZonePlayer & player)
{
    QuitPlayer(player);
}


/*-------------------------------------------------------------------------
 * SetOwner()
 *-------------------------------------------------------------------------
 */
void CZoneSquadImpl::SetOwner(TRef<IZonePlayer> pOwner) 
{ 
  // demote existing owner
  if (m_pOwner && m_pOwner->IsMemberOf(*this)) 
    m_pOwner->SetPosition(*this, IZonePlayer::POS_MEMBER);

  // promote new guy
  m_pOwner = pOwner;
  m_pOwner->SetPosition(*this, IZonePlayer::POS_LEADER);
}



TRef<IZonePlayer> CreateZonePlayer(const char *szName, int nID)
{
    return new CZonePlayerImpl(szName, nID);
}

