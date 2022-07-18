/*-------------------------------------------------------------------------
 * fscluster.h
 * 
 * Declaration of FedSrv class Cluster (aka Sector)
 * 
 * Maker: a-markcu
 * 
 * Copyright 1986-1999 Microsoft Corporation, All Rights Reserved
 *-----------------------------------------------------------------------*/

struct ClusterGroups;

class CFSPlayer;

typedef Slist_utl<CFSPlayer*>   PlayerList;
typedef Slink_utl<CFSPlayer*>   PlayerLink;

class CFSCluster
{
public:

  CFSCluster(TRef<IclusterIGC>  pCluster);
  virtual ~CFSCluster();
  
  IclusterIGC *                 GetClusterIGC()                   {return m_pIclusterIGC;}
  ClusterGroups *               GetClusterGroups()                {return m_pClusterGroups;}
  void                          SetClusterGroups(ClusterGroups *p){m_pClusterGroups=p;}

  void                          AddFlyingPlayer(CFSPlayer* p)
  {
      ZAssert (p);
      ZAssert (m_playersFlying.find(p) == NULL);

      m_playersFlying.last(p);
  }

  void                          RemoveFlyingPlayer(CFSPlayer* p)
  {
      ZAssert (p);
      PlayerLink*   ppl = m_playersFlying.find(p);
      ZAssert (ppl);
      delete ppl;
  }

  const PlayerList*             GetFlyingPlayers(void) const
  {
      return& m_playersFlying;
  }

private:
  
  IclusterIGC *                 m_pIclusterIGC;
  ClusterGroups *               m_pClusterGroups;   // not user here yet, always NULL currently
  PlayerList                    m_playersFlying;
};
