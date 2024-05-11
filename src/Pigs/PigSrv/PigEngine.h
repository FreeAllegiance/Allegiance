/////////////////////////////////////////////////////////////////////////////
// PigEngine.h: Declaration of the CPigEngine class.
//

#pragma once

#include <..\TCLib\TCThread.h>
#include <..\TCLib\Lockable.h>
#include <..\TCLib\tstring.h>
#include <..\TCLib\LookupTable.h>
#include "PigSrv.h"


/////////////////////////////////////////////////////////////////////////////
// Forward Declarations

class CPigBehaviorScriptType;
class CPigBehaviorType;
class CPig;
class CPigs;
class CPigSession;


/////////////////////////////////////////////////////////////////////////////

class CPigEngine
{
// Types
protected:
  typedef std::map<tstring, CPigBehaviorScriptType*, ci_less> XScriptMap;
  typedef XScriptMap::iterator                                XScriptMapIt;
  typedef std::map<tstring, CPigBehaviorScriptType*, ci_less> XBehaviorMap;
  typedef XBehaviorMap::iterator                              XBehaviorMapIt;
  typedef std::set<DWORD>                                     XPigSet;
  typedef XPigSet::iterator                                   XPigIt;

// Construciton / Destruction
private:
  CPigEngine();
  ~CPigEngine();
  HRESULT Construct();

// Attributes
public:
  static CPigEngine& GetEngine();
  CPigs& GetPigs();
  CPigBehaviorScriptType* GetBehaviorType(BSTR bstrType);
  BSTR GetMissionServer();
  BSTR GetZoneAuthServer();
  DWORD GetZoneAuthTimeout();

// Operations
public:
  void Lock();
  void Unlock();
  HRESULT Initialize(CPigSession* pSession);
  DWORD Uninitialize(CPigSession* pSession);
  DWORD AddRef();
  DWORD Release();

// Implementation
protected:
  static IGlobalInterfaceTablePtr& GetGIT();
  static unsigned CALLBACK ScriptDirThunk(void* pv);
  void ScriptDirMonitor(HANDLE hevtExit);
  HRESULT EnsureScriptsAreLoaded();
  void ProcessScriptDirChanges();
  void AddScriptFile(const WIN32_FIND_DATA* pffd, tstring strFileName);
  void LoadScriptFile(CPigBehaviorScriptType* pScript,
    const WIN32_FIND_DATA* pffd, tstring strFileName);
  void ReloadScriptFile(const WIN32_FIND_DATA* pffd, XScriptMapIt& it);
  void UnloadScriptFile(tstring strFileName);
  void UnloadScriptFile(XScriptMapIt& it, bool bErase = true);
  HRESULT GetInvokeCommands(CPigBehaviorScriptType* pType,
    XBehaviorMap& mapCommands);
  HRESULT AddInvokeCommands(CPigBehaviorScriptType* pType);
  void RemoveInvokeCommands(CPigBehaviorScriptType* pType);

// IPigSession Interface Methods (delegated from CPigSession)
public:
  HRESULT get_SessionInfos(ITCSessionInfos** ppSessionInfos);
  HRESULT put_ScriptDir(BSTR bstrScriptDir);
  HRESULT get_ScriptDir(BSTR* pbstrScriptDir);
  HRESULT put_ArtPath(BSTR bstrArtPath);
  HRESULT get_ArtPath(BSTR* pbstrArtPath);
  HRESULT get_BehaviorTypes(IPigBehaviorTypes** ppBehaviorTypes);
  HRESULT get_Lobbies(IPigLobbies** ppLobbies);
  HRESULT get_Pigs(IPigs** ppPigs);
  HRESULT CreatePig(BSTR bstrType, BSTR bstrCommandLine, IPig** ppPig);
  HRESULT put_MissionServer(BSTR bstrServer);
  HRESULT get_MissionServer(BSTR* pbstrServer);
  HRESULT put_MaxPigs(long nMaxPigs);
  HRESULT get_MaxPigs(long* pnMaxPigs);
  HRESULT get_AccountDispenser(IPigAccountDispenser** ppDispenser);
  HRESULT get_EventLog(IAGCEventLogger** ppEventLogger);
  HRESULT put_AccountServer(BSTR bstrServer);
  HRESULT get_AccountServer(BSTR* pbstrServer);
  HRESULT put_ZoneAuthServer(BSTR bstrServer);
  HRESULT get_ZoneAuthServer(BSTR* pbstrServer);
  HRESULT put_ZoneAuthTimeout(long nMilliseconds);
  HRESULT get_ZoneAuthTimeout(long* pnMilliseconds);
  HRESULT put_LobbyMode(PigLobbyMode eMode);
  HRESULT get_LobbyMode(PigLobbyMode* peMode);

// IPigHost Interface Methods (delegated from CPigBehaviorHost)
public:
  HRESULT get_StateName(PigState ePigState, BSTR* pbstrPigState);

// IGlobalInterfaceTable Interface Methods (delegate to single GIT instance)
public:
  HRESULT RegisterInterfaceInGlobal(IUnknown* pUnk, REFIID riid, DWORD* pdwCookie);
  HRESULT RevokeInterfaceFromGlobal(DWORD dwCookie);
  HRESULT GetInterfaceFromGlobal(DWORD dwCookie, REFIID riid, void** ppv);

// Types
protected:
  typedef TCObjectLock<CPigEngine> XLock;
  typedef std::set<CPigSession*>   XSessions;
  typedef XSessions::iterator      XSessionIt;

// Data Members
protected:
  #pragma pack(push, 4)
    static CPigEngine*    m_pInstance;
    TCThread*             m_pth;
  #pragma pack(pop)
  TCLookupTable_DECLARE(StateNames, PigState, LPCOLESTR)
  DWORD                   m_dwRefs;
  CComBSTR                m_bstrMissionServer;
  CComBSTR                m_bstrAccountServer;
  CComBSTR                m_bstrZoneAuthServer;
  _bstr_t                 m_bstrScriptDir;
  TCChangeNotifyHandle    m_hDirChange;
  XScriptMap              m_mapScripts;
  XBehaviorMap            m_mapBehaviors;
  CPigs*                  m_pPigs;
  XPigSet                 m_setPigs;
  IPigAccountDispenserPtr m_spDispenser;
  TCAutoCriticalSection   m_cs;
  XSessions               m_Sessions;
  IAGCEventLoggerPtr      m_spEventLogger;
  long                    m_nZoneAuthTimeout;
  PigLobbyMode            m_eLobbyMode;
};


/////////////////////////////////////////////////////////////////////////////
// Attributes

inline CPigEngine& CPigEngine::GetEngine()
{
  if (!m_pInstance)
    InterlockedExchange((long*)&m_pInstance, (long)(new CPigEngine));
  return *m_pInstance;
}

inline CPigs& CPigEngine::GetPigs()
{
  return *m_pPigs;
}

inline BSTR CPigEngine::GetMissionServer()
{
  return m_bstrMissionServer;
}

inline BSTR CPigEngine::GetZoneAuthServer()
{
  return m_bstrZoneAuthServer;
}

inline DWORD CPigEngine::GetZoneAuthTimeout()
{
  XLock lock(this);
  return static_cast<DWORD>(m_nZoneAuthTimeout);
}


/////////////////////////////////////////////////////////////////////////////
// Operations

inline void CPigEngine::Lock()
{
  m_cs.Lock();
}

inline void CPigEngine::Unlock()
{
  m_cs.Unlock();
}

inline DWORD CPigEngine::AddRef()
{
  return DWORD(Initialize(NULL));
}

inline DWORD CPigEngine::Release()
{
  return Uninitialize(NULL);
}


/////////////////////////////////////////////////////////////////////////////
// Implementation

inline IGlobalInterfaceTablePtr& CPigEngine::GetGIT()
{
  static IGlobalInterfaceTablePtr spGIT;
  if (NULL == spGIT)
    _SVERIFYE(spGIT.CreateInstance(CLSID_StdGlobalInterfaceTable));
  return spGIT;
}


/////////////////////////////////////////////////////////////////////////////
// IGlobalInterfaceTable Interface Methods (delegate to single GIT instance)

inline HRESULT CPigEngine::RegisterInterfaceInGlobal(IUnknown* pUnk,
  REFIID riid, DWORD* pdwCookie)
{
  return GetGIT()->RegisterInterfaceInGlobal(pUnk, riid, pdwCookie);
}

inline HRESULT CPigEngine::RevokeInterfaceFromGlobal(DWORD dwCookie)
{
  return GetGIT()->RevokeInterfaceFromGlobal(dwCookie);
}

inline HRESULT CPigEngine::GetInterfaceFromGlobal(DWORD dwCookie,
  REFIID riid, void** ppv)
{
  return GetGIT()->GetInterfaceFromGlobal(dwCookie, riid, ppv);
}


/////////////////////////////////////////////////////////////////////////////