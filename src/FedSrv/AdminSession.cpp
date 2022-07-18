/*-------------------------------------------------------------------------
 * fedsrv\AdminSession.CPP
 * 
 * This houses the implementation of CAdminSession Class.  This class
 * exposes log-on methods allowing an Admin Session (using COM) with AllSrv.
 * 
 * Owner: , 
 * 
 * Copyright 1986-1998 Microsoft Corporation, All Rights Reserved
 *-----------------------------------------------------------------------*/



#include "pch.h"
#include "AdminSessionEventSink.h"
#include "zreg.h"


/////////////////////////////////////////////////////////////////////////////
// CAdminSession

TC_OBJECT_EXTERN_IMPL(CAdminSession)


/////////////////////////////////////////////////////////////////////////////
// Static Initialization

/*static*/ TCAutoCriticalSection    CAdminSession::s_cs;
/*static*/ CAdminSession::XSessions CAdminSession::s_vecSessions;

// this is temporary until launcher obj gets done
/*static*/ CAdminSession*           CAdminSession::s_pWhoStartedServer = NULL; // This is NULL unless a session started the server


/////////////////////////////////////////////////////////////////////////////
// Construction / Destruction

/*-------------------------------------------------------------------------
 * CAdminSession()
 *-------------------------------------------------------------------------
 */
CAdminSession::CAdminSession() :
  m_pEventSink(NULL)
{
  XLockStatic lock(&s_cs);
  if (_Module.WasCOMStarted() && !s_pWhoStartedServer)
    s_pWhoStartedServer = this;

  // Add ourself to the static collection
  s_vecSessions.push_back(this);
}


/*-------------------------------------------------------------------------
 * ~CAdminSession()
 *-------------------------------------------------------------------------
 */
CAdminSession::~CAdminSession()
{
  XLockStatic lock(&s_cs);
  if (this == s_pWhoStartedServer) 
    s_pWhoStartedServer = NULL;

  // Remove ourself from the static collection
  XSessionsIt it = std::find(s_vecSessions.begin(), s_vecSessions.end(), this);
  if (it != s_vecSessions.end())
    s_vecSessions.erase(it);
  lock.Unlock();

  #if defined(ALLSRV_STANDALONE)
    // Possibly shutdown the standalone server if no more sessions
    if (0 == GetSessionCount())
    {
      // Get the Server property
      IAdminServerPtr spServer;
      if (SUCCEEDED(get_Server(&spServer)))
      {
        IAdminGamesPtr spGames;
        if (SUCCEEDED(spServer->get_Games(&spGames)))
        {
          // Shutdown the server if no games exist
          long cGames = 0;
          spGames->get_Count(&cGames);
		  // KGJV #114 - if lobbied then dont shutdown if create game allowed on this server
		  bool bSupposedToConnectToLobby = !(FEDSRV_GUID != g.fm.GetHostApplicationGuid());
		  if ((0 == cGames) && (bSupposedToConnectToLobby ? (g.cStaticCoreInfo==0) : true))
            PostThreadMessage(g.idReceiveThread, WM_QUIT, 0, 0);
        }
      }
    }
  #endif // defined(ALLSRV_STANDALONE)
}


HRESULT CAdminSession::FinalConstruct()
{
  // #define CAdminSession_TRACE_CONSTRUCTION
  #ifdef CAdminSession_TRACE_CONSTRUCTION
    _TRACE_BEGIN
      DWORD id = GetCurrentThreadId();
      _TRACE_PART2("CAdminSession::FinalConstruct(): ThreadId = %d (0x%X)\n", id, id);
      _TRACE_PART1("\tRaw pointer = 0x%08X\n", this);
    _TRACE_END
  #endif // CAdminSession_TRACE_CONSTRUCTION

  // Create the event sink object
  CComObject<CAdminSessionEventSink>* pEventSink = NULL;
  RETURN_FAILED(pEventSink->CreateInstance(&pEventSink));
  pEventSink->AddRef();
  pEventSink->Init(this);
  m_pEventSink = pEventSink;

  // Indicate success
  return S_OK;
}

void CAdminSession::FinalRelease()
{
  // Terminate the event sink object
  if (m_pEventSink)
  {
    m_pEventSink->Term();
    m_pEventSink->Release();
    m_pEventSink = NULL;
  }
}


/////////////////////////////////////////////////////////////////////////////
// Overrides

IUnknown* CAdminSession::OnGetUnknown()
{
  return NULL;
}


/*-------------------------------------------------------------------------
 * DestroyAllSessions()
 *-------------------------------------------------------------------------
 * Purpose:
 *    Kill all active (and hosed) sessions
 * 
 */
/* static*/void  CAdminSession::DestroyAllSessions()
{
  int cSessions;
  while (cSessions = GetSessionCount()) // Intentional assignment
  {
    // Keep a reference while disconnecting object
    CAdminSession* pSession = GetLastSession();
    IUnknownPtr spUnk(pSession);

    // Disconnect all external references
    if (NULL != spUnk)
      ::CoDisconnectObject(spUnk, 0);
    
    // Release the IUnknown
    spUnk = NULL;

    // That /should/ have forced a deletion. If not, remove from list
    XLockStatic lock(&s_cs);
    if (GetSessionCount() == cSessions)
    {
      XSessionsIt it = std::find(s_vecSessions.begin(), s_vecSessions.end(), pSession);
      ZAssert(it != s_vecSessions.end());
      s_vecSessions.erase(it);
    }
  }
}




/*-------------------------------------------------------------------------
 * put_SessionInfo()
 *-------------------------------------------------------------------------
 * Purpose:
 *    
 * 
 */
STDMETHODIMP CAdminSession::put_SessionInfo(ITCSessionInfo* pSessionInfo)
{
//  GetAGCGlobal()->TriggerEvent(ppListeners, EventID_NewSession, 0);
  return S_OK;
}


/*-------------------------------------------------------------------------
 * get_SessionInfo()
 *-------------------------------------------------------------------------
 * Purpose:
 *    
 * 
 */
STDMETHODIMP CAdminSession::get_SessionInfo(ITCSessionInfo** ppSessionInfo)
{
  return S_OK;
}

/*-------------------------------------------------------------------------
 * get_SessionInfos()
 *-------------------------------------------------------------------------
 * Purpose:
 *    
 * 
 */
STDMETHODIMP CAdminSession::get_SessionInfos(ITCSessionInfos** ppSessionInfos)
{

  return S_OK;
}

/*-------------------------------------------------------------------------
 * get_SessionInfos()
 *-------------------------------------------------------------------------
 * Purpose:
 *   Use this to find out of an event of a certain type is firing 
 *
 *   A uniqueID of AGC_Any_Objects causes the check to be done for global event firing,
 *   otherwise this id is used to check for specialized event firing for the AGC
 *   object with this id.
 * 
 */
STDMETHODIMP CAdminSession::get_IsEventActivated(AGCEventID event, AGCUniqueID uniqueID, BOOL *pVal)
{
  if (uniqueID == -1)
    uniqueID = AGC_Any_Objects;

  ZAssert(m_pEventSink);
  *pVal = GetAGCGlobal()->IsRegistered(event, uniqueID,
    reinterpret_cast<IAGCEventSink*>(m_pEventSink)); 
  return S_OK;
}



/*-------------------------------------------------------------------------
 * get_Server()
 *-------------------------------------------------------------------------
 * Purpose:
 *    
 * 
 */
STDMETHODIMP CAdminSession::get_Server(IAdminServer** ppAdminServer)
{
  // Get the CAdminServer instance from the GIT
  HRESULT hr;
  ZAssert(g.dwServerGITCookie);
  ZSucceeded(hr = GetAGCGlobal()->GetInterfaceFromGlobal(
    g.dwServerGITCookie, IID_IAdminServer, (void**)ppAdminServer));
  return hr;
}


/*-------------------------------------------------------------------------
 * ActivateEvents()
 *-------------------------------------------------------------------------
 * Purpose:
 *   Use this to activate event firing for events of a certain type 
 *
 * Parameters:
 *    event: id of the event to be deactivated
 *    uniqueID: if not AGC_Any_Objects, then this is AGC unqiue object id telling who the event
 *              should be fired for.  AGC_Any_Objects means for all applicable objects
 */
STDMETHODIMP CAdminSession::ActivateEvents(AGCEventID event, AGCUniqueID uniqueID)
{
  if (-1 == uniqueID)
    uniqueID = AGC_Any_Objects;

  ZAssert(m_pEventSink);
  GetAGCGlobal()->RegisterEvent(event, uniqueID,
    reinterpret_cast<IAGCEventSink*>(m_pEventSink));
  return S_OK;
}


/*-------------------------------------------------------------------------
 * DeactivateEvents()
 *-------------------------------------------------------------------------
 * Purpose:
 *    Use this to activate event firing for events of a certain type
 *
 * Parameters:
 *    event: id of the event to be deactivated
 *    uniqueID: if not AGC_Any_Objects, then this is AGC unqiue object id telling who the event
 *              should be fired for.  AGC_Any_Objects means for all applicable objects
 *
 * Remarks:
 *    There should be a DeactivateEvents() call for each ActivateEvents() call.
 *    A single DeactivateEvents() call with AGC_Any_Objects as the unique ID does not turn off 
 *    all event firing for the AGCEventID event.
 */
STDMETHODIMP CAdminSession::DeactivateEvents(AGCEventID event, AGCUniqueID uniqueID)
{
  if (-1 == uniqueID)
    uniqueID = AGC_Any_Objects;

  ZAssert(m_pEventSink);
  GetAGCGlobal()->RevokeEvent(event, uniqueID,
    reinterpret_cast<IAGCEventSink*>(m_pEventSink));
  return S_OK;
}


/*-------------------------------------------------------------------------
 * ActivateAllEvents()
 *-------------------------------------------------------------------------
 * Purpose:
 *   Use this to activate event firing all events for this session
 * 
 */
STDMETHODIMP CAdminSession::ActivateAllEvents()
{
  ZAssert(m_pEventSink);
  GetAGCGlobal()->RegisterAllEvents(
    reinterpret_cast<IAGCEventSink*>(m_pEventSink));
  return S_OK;
}



/*-------------------------------------------------------------------------
 * DeactivateAllEvents()
 *-------------------------------------------------------------------------
 * Purpose:
 *   Use this to deactivate event firing all events for this session
 * 
 */
STDMETHODIMP CAdminSession::DeactivateAllEvents()
{
  ZAssert(m_pEventSink);
  GetAGCGlobal()->RevokeAllEvents(
    reinterpret_cast<IAGCEventSink*>(m_pEventSink));
  return S_OK;
}


/*-------------------------------------------------------------------------
 * get_ProcessID()
 *-------------------------------------------------------------------------
 * Purpose:
 *    Determine the process identifier of this AllSrv process.
 * 
 */
STDMETHODIMP CAdminSession::get_ProcessID(long* pdwProcessID)
{
  CLEAROUT(pdwProcessID, long(GetCurrentProcessId()));
  return S_OK;
}


/*-------------------------------------------------------------------------
 * get_ProductVersion()
 *-------------------------------------------------------------------------
 * Purpose:
 *    Determine the version number of AllSrv
 * 
 */
STDMETHODIMP CAdminSession::get_Version(IAGCVersionInfo** ppVersion)
{
  // Initialize the [out] parameter
  CLEAROUT(ppVersion, (IAGCVersionInfo*)NULL);

  // Create an instance of the version object
  IAGCVersionInfoPtr spVersion;
  RETURN_FAILED(spVersion.CreateInstance(CLSID_AGCVersionInfo));

  // Initialize the version object
  TCHAR szModule[_MAX_PATH];
  GetModuleFileName(_Module.GetModuleInstance(), szModule, sizeofArray(szModule));
  RETURN_FAILED(spVersion->put_FileName(CComBSTR(szModule)));

  // Detach the object to the [out] parameter
  *ppVersion = spVersion.Detach();

  // Indicate success
  return S_OK;
}


/*-------------------------------------------------------------------------
 * CAdminSession::Stop()
 *-------------------------------------------------------------------------
 * Purpose:
 *   Use this to kill the server.  
 * 
 */

STDMETHODIMP CAdminSession::Stop()
{
  OutputDebugString("\nSomeone is shutting down the server (possibly remotely) using the Admin Object Model.\n");

  _Module.StopAllsrv();

  return S_OK;
}


/*-------------------------------------------------------------------------
 * CAdminSession::Pause()
 *-------------------------------------------------------------------------
 * Purpose:
 *   Use this to pause the server.  When paused, no new users may join 
 *   and once everyone logs off, the server shuts down.
 *
 *   This method does nothing if the server is already paused.
 * 
 */

STDMETHODIMP CAdminSession::Pause()
{
  FedSrv_Pause();
  return S_OK;
}


/*-------------------------------------------------------------------------
 * get_WhoStartedServer()
 *-------------------------------------------------------------------------
 * Purpose:
 *    Use this to find out who started the server
 * 
 */
STDMETHODIMP CAdminSession::get_WhoStartedServer(IAdminSession** pIAdminSession)
{
  XLockStatic lock(&s_cs);
  if (s_pWhoStartedServer == NULL)
  {
    *pIAdminSession = NULL;
    return S_OK;
  }

  // TODO: NOT THREAD-SAFE!!!
  return s_pWhoStartedServer->QueryInterface(IID_IAdminSession, (void**)pIAdminSession);
}


/*-------------------------------------------------------------------------
 * CAdminSession::get_EventLog
 *-------------------------------------------------------------------------
 * Purpose:
 *    Gets the previously created event logger object.
 * 
 */
STDMETHODIMP CAdminSession::get_EventLog(IAGCEventLogger** ppEventLogger)
{
  // Delegate to the module object
  return _Module.get_EventLog(ppEventLogger);
}


/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CAdminSession::get_PerfCounters(IAGCEvent** ppPerfCounters)
{
  // Create an AGCEvent object to store the named performance counters
  IAGCEventCreatePtr spEvent;
  RETURN_FAILED(spEvent.CreateInstance(CLSID_AGCEvent));
  RETURN_FAILED(spEvent->Init());

  // Populate the event with the named performance counters
  CComVariant var(0L);
  V_I4(&var) = g.pServerCounters->cPacketsIn;
  RETURN_FAILED(spEvent->AddProperty(CComBSTR(L"PacketsIn"), &var));

  V_I4(&var) = g.pServerCounters->cPlayersOnline;
  RETURN_FAILED(spEvent->AddProperty(CComBSTR(L"PlayersOnline"), &var));

  V_I4(&var) = g.pServerCounters->timeBetweenInnerLoops;
  RETURN_FAILED(spEvent->AddProperty(CComBSTR(L"TimeInnerLoop"), &var));

  // Return the new object
  return spEvent->QueryInterface(IID_IAGCEvent, (void**)ppPerfCounters);
}


/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CAdminSession::SendAdminChat(BSTR bstrText, long nUserID, DATE dateOriginal)
{
  // This just triggers an event
  _AGCModule.TriggerEvent(NULL, EventID_AdminChat, "", nUserID, -1, -1, 2,
    "Message"      , VT_BSTR , bstrText,
    "OriginalTime" , VT_DATE , dateOriginal);
  
  // Indicate success
  return S_OK;
}


/*-------------------------------------------------------------------------
 * CAdminSession::Continue()
 *-------------------------------------------------------------------------
 * Purpose:
 *   Use this to continue the server from a paused state.
 *
 *   This method does nothing if the server is not paused.
 * 
 */

STDMETHODIMP CAdminSession::Continue()
{
  FedSrv_Continue();
  return S_OK;
}


