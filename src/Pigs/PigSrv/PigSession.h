#pragma once

/////////////////////////////////////////////////////////////////////////////
// PigSession.h : Declaration of the CPigSession

#include "PigSrv.h"
#include "resource.h"       // main symbols
#include "PigEngine.h"
#include "PigSessionEventSink.h"
#include <objsafe.h>
#include <..\TCAtl\DualEventsCP.h>


/////////////////////////////////////////////////////////////////////////////
// External Declarations

extern "C" const IID DIID__IPigSessionEvents;


/////////////////////////////////////////////////////////////////////////////
// IPigSessionEventsCP
//
BEGIN_TCComDualEventsCP(IPigSessionEventsCP, IPigSessionEvents, DIID__IPigSessionEvents)
  TCComDualEventCP_Fn1(OnEvent, dispid_OnEvent, IAGCEventPtr, VT_DISPATCH)
END_TCComDualEventsCP()


/////////////////////////////////////////////////////////////////////////////
// CPigSession
class ATL_NO_VTABLE CPigSession : 
  public IDispatchImpl<IPigSession, &IID_IPigSession, &LIBID_PigsLib>,
  public ISupportErrorInfo,
  public IConnectionPointContainerImpl<CPigSession>,
  public IProvideClassInfo2Impl<&CLSID_PigSession, &DIID__IPigSessionEvents, &LIBID_PigsLib>,
  public IPigSessionEventsCP<CPigSession>,
  public CComObjectRootEx<CComMultiThreadModel>,
  public CComCoClass<CPigSession, &CLSID_PigSession>
{
// Declarations
public:
  DECLARE_REGISTRY_RESOURCEID(IDR_PigSession)
  DECLARE_PROTECT_FINAL_CONSTRUCT()

// Category Map
public:
  BEGIN_CATEGORY_MAP(CPigSession)
    IMPLEMENTED_CATEGORY(CATID_PigObjects)
    IMPLEMENTED_CATEGORY(CATID_SafeForScripting)
    IMPLEMENTED_CATEGORY(CATID_SafeForInitializing)
  END_CATEGORY_MAP()

// Interface Map
public:
  BEGIN_COM_MAP(CPigSession)
    COM_INTERFACE_ENTRY(IPigSession)
    COM_INTERFACE_ENTRY(IDispatch)
    COM_INTERFACE_ENTRY(ISupportErrorInfo)
    COM_INTERFACE_ENTRY(IConnectionPointContainer)
    COM_INTERFACE_ENTRY(IProvideClassInfo)
    COM_INTERFACE_ENTRY(IProvideClassInfo2)
  END_COM_MAP()

// Connection Point Map
public:
  BEGIN_CONNECTION_POINT_MAP(CPigSession)
    CONNECTION_POINT_ENTRY(IID_IPigSessionEvents)
    CONNECTION_POINT_ENTRY(DIID__IPigSessionEvents)
  END_CONNECTION_POINT_MAP()

// Construction / Destruction
public:
  CPigSession();
  HRESULT FinalConstruct();
  void FinalRelease();

// Implementation
protected:
  static CPigEngine& GetEngine();

// ISupportsErrorInfo Interface Methods
public:
  STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// IPigSession Interface Methods
public:
  STDMETHODIMP put_SessionInfo(ITCSessionInfo* pSessionInfo);
  STDMETHODIMP get_SessionInfo(ITCSessionInfo** ppSessionInfo);
  STDMETHODIMP get_SessionInfos(ITCSessionInfos** ppSessionInfos);
  STDMETHODIMP put_ScriptDir(BSTR bstrScriptDir);
  STDMETHODIMP get_ScriptDir(BSTR* pbstrScriptDir);
  STDMETHODIMP put_ArtPath(BSTR bstrArtPath);
  STDMETHODIMP get_ArtPath(BSTR* pbstrArtPath);
  STDMETHODIMP get_BehaviorTypes(IPigBehaviorTypes** ppBehaviorTypes);
  STDMETHODIMP get_Lobbies(IPigLobbies** ppLobbies);
  STDMETHODIMP get_Pigs(IPigs** ppPigs);
  STDMETHODIMP CreatePig(BSTR bstrType, BSTR bstrCommandLine, IPig** ppPig);
  STDMETHODIMP put_MissionServer(BSTR bstrServer);
  STDMETHODIMP get_MissionServer(BSTR* pbstrServer);
  STDMETHODIMP put_MaxPigs(long nMaxPigs);
  STDMETHODIMP get_MaxPigs(long* pnMaxPigs);
  STDMETHODIMP get_AccountDispenser(IPigAccountDispenser** ppDispenser);
  STDMETHODIMP get_Version(IAGCVersionInfo** ppVersion);
  STDMETHODIMP get_ProcessID(DWORD* pdwProcessID);
  STDMETHODIMP ActivateAllEvents();
  STDMETHODIMP DeactivateAllEvents();
  STDMETHODIMP get_EventLog(IAGCEventLogger** ppEventLogger);
  STDMETHODIMP put_AccountServer(BSTR bstrServer);
  STDMETHODIMP get_AccountServer(BSTR* pbstrServer);
  STDMETHODIMP put_ZoneAuthServer(BSTR bstrServer);
  STDMETHODIMP get_ZoneAuthServer(BSTR* pbstrServer);
  STDMETHODIMP put_ZoneAuthTimeout(long nMilliseconds);
  STDMETHODIMP get_ZoneAuthTimeout(long* pnMilliseconds);
  STDMETHODIMP put_LobbyMode(PigLobbyMode eMode);
  STDMETHODIMP get_LobbyMode(PigLobbyMode* peMode);

// Data Members
protected:
  CPigSessionEventSink* m_pEventSink;
};



/////////////////////////////////////////////////////////////////////////////
// Implementation

inline CPigEngine& CPigSession::GetEngine()
{
  return CPigEngine::GetEngine();
}




/////////////////////////////////////////////////////////////////////////////