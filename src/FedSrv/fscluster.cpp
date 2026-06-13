/*-------------------------------------------------------------------------
 * fscluster.cpp
 * 
 * Implementation of FedSrv class Cluster (aka Sector)
 * 
 * Maker: a-markcu
 * 
 * Copyright 1986-1999 Microsoft Corporation, All Rights Reserved
 *-----------------------------------------------------------------------*/

#include "pch.h"

/*-------------------------------------------------------------------------
 * CFSCluster::CFSCluster()
 *-------------------------------------------------------------------------
 */
CFSCluster::CFSCluster(TRef<IclusterIGC> pCluster) :

  m_pIclusterIGC(pCluster),
  m_pClusterGroups(NULL)
{
  pCluster->SetPrivateData((DWORD)this);  // link to IGC CLuster
}


/*-------------------------------------------------------------------------
 * CFSCluster::~CFSCluster()
 *-------------------------------------------------------------------------
 */
CFSCluster::~CFSCluster()
{
  m_pIclusterIGC->SetPrivateData((DWORD)NULL);  // unlink from IGC CLuster

  // Free the per-cluster dplay groups created in CFSMission::CreateDPGroups.
  // FedMessaging::DeleteGroup destroys the dplay group and deletes the CFMGroup
  // object (CFMGroup::Delete -> DestroyGroup + 'delete this'). FedMessaging does
  // NOT auto-free groups at session teardown (Shutdown() only does
  // m_groupMap.clear() on a std::map of raw pointers), so this does not double-free.
  if (m_pClusterGroups)
  {
    g.fm.DeleteGroup(m_pClusterGroups->pgrpClusterDocked);
    g.fm.DeleteGroup(m_pClusterGroups->pgrpClusterFlying);
    delete m_pClusterGroups;
    m_pClusterGroups = NULL;
  }
}