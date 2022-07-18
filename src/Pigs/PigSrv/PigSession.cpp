/////////////////////////////////////////////////////////////////////////////
// PigSession.cpp : Implementation of the CPigSession class.

#include "pch.h"
#include <TCLib.h>
#include <..\TCAtl\ObjectMap.h>
#include "PigSession.h"


/////////////////////////////////////////////////////////////////////////////
// CPigSession

TC_OBJECT_EXTERN_IMPL(CPigSession)


/////////////////////////////////////////////////////////////////////////////
// Construction / Destruction

CPigSession::CPigSession() :
  m_pEventSink(NULL)
{
}

HRESULT CPigSession::FinalConstruct()
{
  #define CPigSession_TRACE_CONSTRUCTION
  #ifdef CPigSession_TRACE_CONSTRUCTION
    _TRACE_BEGIN
      DWORD id = GetCurrentThreadId();
      _TRACE_PART2("CPigSession::FinalConstruct(): ThreadId = %d (0x%X)\n", id, id);
      _TRACE_PART1("\tRaw pointer = 0x%08X\n", this);
    _TRACE_END
  #endif // !CPigSession_TRACE_CONSTRUCTION

  // Initialize the pig engine
  HRESULT hr;
  _SVERIFYE(hr = GetEngine().Initialize(this));
  RETURN_FAILED(hr);

  // TODO: Move this into it's own method
  // Create the event sink object
  CComObject<CPigSessionEventSink>* pEventSink = NULL;
  RETURN_FAILED(pEventSink->CreateInstance(&pEventSink));
  pEventSink->AddRef();
  pEventSink->Init(this);
  m_pEventSink = pEventSink;

  // Indicate success
  return S_OK;
}

void CPigSession::FinalRelease()
{
  // TODO: Move this into it's own method
  // Terminate the event sink object
  if (m_pEventSink)
  {
    m_pEventSink->Term();
    m_pEventSink->Release();
    m_pEventSink = NULL;
  }

  // Dereference the pig engine
  GetEngine().Uninitialize(this);
}


/////////////////////////////////////////////////////////////////////////////
// ISupportErrorInfo Interface Methods

STDMETHODIMP CPigSession::InterfaceSupportsErrorInfo(REFIID riid)
{
  static const IID* arr[] = 
  {
    &IID_IPigSession,
  };
  for (int i = 0; i < sizeof(arr) / sizeof(arr[0]) ; ++i)
  {
    if (InlineIsEqualGUID(*arr[i], riid))
      return S_OK;
  }
  return S_FALSE;
}


/////////////////////////////////////////////////////////////////////////////
// IPigSession Interface Methods

STDMETHODIMP CPigSession::put_SessionInfo(ITCSessionInfo* pSessionInfo)
{
  return S_OK;
}

STDMETHODIMP CPigSession::get_SessionInfo(ITCSessionInfo** ppSessionInfo)
{
  return S_OK;
}

STDMETHODIMP CPigSession::get_SessionInfos(ITCSessionInfos** ppSessionInfos)
{
  // Delegate to the engine object
  return GetEngine().get_SessionInfos(ppSessionInfos);
}

STDMETHODIMP CPigSession::put_ScriptDir(BSTR bstrScriptDir)
{
  // Delegate to the engine object
  return GetEngine().put_ScriptDir(bstrScriptDir);
}

STDMETHODIMP CPigSession::get_ScriptDir(BSTR* pbstrScriptDir)
{
  // Delegate to the engine object
  return GetEngine().get_ScriptDir(pbstrScriptDir);
}

STDMETHODIMP CPigSession::put_ArtPath(BSTR bstrArtPath)
{
  // Delegate to the engine object
  return GetEngine().put_ArtPath(bstrArtPath);
}

STDMETHODIMP CPigSession::get_ArtPath(BSTR* pbstrArtPath)
{
  // Delegate to the engine object
  return GetEngine().get_ArtPath(pbstrArtPath);
}

STDMETHODIMP CPigSession::get_BehaviorTypes(
  IPigBehaviorTypes** ppBehaviorTypes)
{
  // Delegate to the engine object
  return GetEngine().get_BehaviorTypes(ppBehaviorTypes);
}

STDMETHODIMP CPigSession::get_Lobbies(IPigLobbies** ppLobbies)
{
  // Delegate to the engine object
  return GetEngine().get_Lobbies(ppLobbies);
}

STDMETHODIMP CPigSession::get_Pigs(IPigs** ppPigs)
{
  // Delegate to the engine object
  return GetEngine().get_Pigs(ppPigs);
}

STDMETHODIMP CPigSession::CreatePig(BSTR bstrType, BSTR bstrCommandLine,
  IPig** ppPig)
{
  // Delegate to the engine object
  return GetEngine().CreatePig(bstrType, bstrCommandLine, ppPig);
}

STDMETHODIMP CPigSession::put_MissionServer(BSTR bstrServer)
{
  // Delegate to the engine object
  return GetEngine().put_MissionServer(bstrServer);
}

STDMETHODIMP CPigSession::get_MissionServer(BSTR* pbstrServer)
{
  // Delegate to the engine object
  return GetEngine().get_MissionServer(pbstrServer);
}

STDMETHODIMP CPigSession::put_MaxPigs(long nMaxPigs)
{
  // Delegate to the engine object
  return GetEngine().put_MaxPigs(nMaxPigs);
}

STDMETHODIMP CPigSession::get_MaxPigs(long* pnMaxPigs)
{
  // Delegate to the engine object
  return GetEngine().get_MaxPigs(pnMaxPigs);
}

STDMETHODIMP CPigSession::get_AccountDispenser(IPigAccountDispenser** ppDispenser)
{
  // Delegate to the engine object
  return GetEngine().get_AccountDispenser(ppDispenser);
}

STDMETHODIMP CPigSession::get_Version(IAGCVersionInfo** ppVersion)
{
  // Initialize the [out] parameter
  CLEAROUT(ppVersion, (IAGCVersionInfo*)NULL);

  // Create an instance of the version object
  IAGCVersionInfoPtr spVersion;
  RETURN_FAILED(spVersion.CreateInstance("AGC.AGCVersionInfo"));

  // Initialize the version object
  TCHAR szModule[_MAX_PATH];
  GetModuleFileName(_Module.GetModuleInstance(), szModule, sizeofArray(szModule));
  RETURN_FAILED(spVersion->put_FileName(CComBSTR(szModule)));

  // Detach the object to the [out] parameter
  *ppVersion = spVersion.Detach();

  // Indicate success
  return S_OK;
}

STDMETHODIMP CPigSession::get_ProcessID(DWORD* pdwProcessID)
{
  CLEAROUT(pdwProcessID, GetCurrentProcessId());
  return S_OK;
}

STDMETHODIMP CPigSession::ActivateAllEvents()
{
  ZAssert(m_pEventSink); // TODO: Create in lazy fashion
  GetAGCGlobal()->RegisterAllEvents(
    reinterpret_cast<IAGCEventSink*>(m_pEventSink->GetGITCookie()));
  return S_OK;
}

STDMETHODIMP CPigSession::DeactivateAllEvents()
{
  ZAssert(m_pEventSink); // TODO: Create in lazy fashion
  GetAGCGlobal()->RevokeAllEvents(
    reinterpret_cast<IAGCEventSink*>(m_pEventSink->GetGITCookie()));
  return S_OK;
}

STDMETHODIMP CPigSession::get_EventLog(IAGCEventLogger** ppEventLogger)
{
  // Delegate to the engine object
  return GetEngine().get_EventLog(ppEventLogger);
}

STDMETHODIMP CPigSession::put_AccountServer(BSTR bstrServer)
{
  // Delegate to the engine object
  return GetEngine().put_AccountServer(bstrServer);
}

STDMETHODIMP CPigSession::get_AccountServer(BSTR* pbstrServer)
{
  // Delegate to the engine object
  return GetEngine().get_AccountServer(pbstrServer);
}

STDMETHODIMP CPigSession::put_ZoneAuthServer(BSTR bstrServer)
{
  // Delegate to the engine object
  return GetEngine().put_ZoneAuthServer(bstrServer);
}

STDMETHODIMP CPigSession::get_ZoneAuthServer(BSTR* pbstrServer)
{
  // Delegate to the engine object
  return GetEngine().get_ZoneAuthServer(pbstrServer);
}

STDMETHODIMP CPigSession::put_ZoneAuthTimeout(long nMilliseconds)
{
  // Delegate to the engine object
  return GetEngine().put_ZoneAuthTimeout(nMilliseconds);
}

STDMETHODIMP CPigSession::get_ZoneAuthTimeout(long* pnMilliseconds)
{
  // Delegate to the engine object
  return GetEngine().get_ZoneAuthTimeout(pnMilliseconds);
}

STDMETHODIMP CPigSession::put_LobbyMode(PigLobbyMode eMode)
{
  // Delegate to the engine object
  return GetEngine().put_LobbyMode(eMode);
}

STDMETHODIMP CPigSession::get_LobbyMode(PigLobbyMode* peMode)
{
  // Delegate to the engine object
  return GetEngine().get_LobbyMode(peMode);
}

