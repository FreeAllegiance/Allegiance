/*-------------------------------------------------------------------------
  server.h
  
  Per server stuff on lobby side
  
  Owner: 
  
  Copyright 1986-2000 Microsoft Corporation, All Rights Reserved
 *-----------------------------------------------------------------------*/

#ifndef _SERVER_H_
#define _SERVER_H_

// mmf added this to support lobby checking for allowed server ips
bool IsServerAllowed(const char *ip);

const int c_GameFactor = 5; // factor to multiply count of games by when adding to player count ot determine server load

class CFLServer : public IObject
{
public:
  CFLServer(CFMConnection * pcnxn);
  ~CFLServer();

  CFMConnection * GetConnection()
  {
    return m_pcnxn;
  }

  static CFLServer * FromConnection(CFMConnection & cnxn) 
  {
    return (CFLServer *)cnxn.GetPrivateData();
  }

  CFLMission * CreateMission(CFLClient * pClientCreator)
  {
    CFLMission * pMission = new CFLMission(this, pClientCreator);
    m_missions.PushFront(pMission);
    return pMission;
  }
  
  void DeleteMission(const CFLMission * pMission);

  MissionList * GetMissions()
  {
    return &m_missions;
  }
    
  int GetPlayerCount()
  {
    return m_cPlayers;
  }

  void AddPlayer()
  {
    ++m_cPlayers;
  }

  void RemovePlayer()
  {
    --m_cPlayers;
    ZAssert (m_cPlayers >= 0);
  }

  PER_SERVER_COUNTERS * GetCounters()
  {
    return m_pCounters;
  }

  int GetPercentLoad()
  {
    return (m_missions.GetCount() * c_GameFactor + m_cPlayers) / m_maxLoad;
  }

  void SetHere()
  {
    m_bHere = true;
  }

  bool GetHere()
  {
    return m_bHere;
  }

  bool GetPaused()
  {
    return m_fPaused;
  }

  void SetServerPort(DWORD dwPort)		// mdvalley: functions for setting and getting server port
  {
	  m_sPort = dwPort;
  }

  DWORD GetServerPort()
  {
	  return m_sPort;
  }

  // KGJV #114
  void FreeStaticCoreInfo()
  {
	  if (m_vStaticCoreInfo)
		  delete [] m_vStaticCoreInfo;
	  m_vStaticCoreInfo = NULL;
	  m_cStaticCoreInfo = 0;
  }
  void SetStaticCoreInfo(int cStaticCoreInfo, StaticCoreInfo *vStaticCoreInfo)
  {
		FreeStaticCoreInfo(); // free in case of reconnect
		m_cStaticCoreInfo = cStaticCoreInfo;
		if (cStaticCoreInfo)
		{
			m_vStaticCoreInfo = new StaticCoreInfo[cStaticCoreInfo];
			memcpy(m_vStaticCoreInfo,vStaticCoreInfo,cStaticCoreInfo*sizeof(StaticCoreInfo));
		}
  }
  int GetcStaticCoreInfo() { return m_cStaticCoreInfo; }
  StaticCoreInfo* GetvStaticCoreInfo() {return m_vStaticCoreInfo;}
  void SetStaticCoreMask(DWORD dwStaticCoreMask) { m_dwStaticCoreMask = dwStaticCoreMask; }
  DWORD GetStaticCoreMask() { return m_dwStaticCoreMask; }
 
  //Imago - allow setting of Current Games w/o effecting normal usage
  int GetCurrentGamesCount() { if (m_iCurGames != m_missions.GetCount() && m_iCurGames != 0) {return m_iCurGames;} else {return m_missions.GetCount();}}
  void SetCurrentGamesCount(int CurGames) {m_iCurGames = CurGames;}
  int GetMaxGamesAllowed()   { return m_iMaxGames; }
  void SetMaxGamesAllowed(int max) {m_iMaxGames = max;}
  char *GetLocation() { return m_szLocation; }
  void SetLocation(char *loc) { 
	  Strncpy(m_szLocation,loc,sizeof(m_szLocation)); //Imago use Safe string copy 6/10
	  m_szLocation[sizeof(m_szLocation)-1]='\0';
  };
  
  //Imago 6/10 #2
  char *GetPrivilegedUsers() {return m_szPrivilegedUsers;} 
  void SetPrivilegedUsers(char *szPrivilegedUsers) {
	  Strncpy(m_szPrivilegedUsers,szPrivilegedUsers,sizeof(m_szPrivilegedUsers));
	  m_szPrivilegedUsers[sizeof(m_szPrivilegedUsers)-1]='\0';
  }
  //Imago 7/10 #62
  char *GetVersion() {return m_szVersion;} 
  void SetVersion(char *szVersion) {
	  Strncpy(m_szVersion,szVersion,sizeof(m_szVersion));
	  m_szPrivilegedUsers[sizeof(m_szVersion)-1]='\0';
  }

  void Pause(bool fPause);

private:
  
  static const CFLMission * c_AllMissions;
  static const DWORD c_dwID;
  DWORD m_dwID;
  CFMConnection * m_pcnxn;
  DWORD m_sPort;					// mdvalley: track the port number
  PER_SERVER_COUNTERS * m_pCounters;
  MissionList     m_missions;
  int             m_cPlayers;
  int             m_maxLoad; // some combination of games/players
  bool            m_bHere; // don't count them for periodic roll call until they show up for the 1st time.
  bool            m_fPaused;
  // KGJV #114
  StaticCoreInfo *m_vStaticCoreInfo;
  int m_cStaticCoreInfo;
  DWORD m_dwStaticCoreMask;
  int  m_iMaxGames;
  int  m_iCurGames;  // Imago
  char m_szLocation[c_cbFileName]; // it's not a filename but we want it short
  char m_szPrivilegedUsers[512]; // Imago 6/10 #2
  char m_szVersion[18]; // Imago 6/10 #62
};

#endif
