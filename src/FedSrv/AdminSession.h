/*-------------------------------------------------------------------------
 * fedsrv\AdminSession.H
 * 
 * This houses the definition of AdminSession Class.  This class
 * exposes log-on methods allowing a Admin Session (using COM) with AllSrv.
 * 
 * Owner: 
 * 
 * Copyright 1986-1998 Microsoft Corporation, All Rights Reserved
 *-----------------------------------------------------------------------*/

#pragma once

#include <objsafe.h> // For CATID_SafeForScripting
#include <..\TCLib\AutoCriticalSection.h>
#include <..\TCLib\ObjectLock.h>
#include <..\TCAtl\DualEventsCP.h>
#include "AdminSessionClass.h"


/////////////////////////////////////////////////////////////////////////////
// External Declarations

extern "C" const IID DIID__IAdminSessionEvents;


/////////////////////////////////////////////////////////////////////////////
// Forward Declarations

class CAdminSessionEventSink;


/////////////////////////////////////////////////////////////////////////////
// IAdminSessionEventsCP
//
BEGIN_TCComDualEventsCP(IAdminSessionEventsCP, IAdminSessionEvents, DIID__IAdminSessionEvents)
  TCComDualEventCP_Fn1(OnEvent, dispid_OnEvent, IAGCEventPtr, VT_DISPATCH)
END_TCComDualEventsCP()


/////////////////////////////////////////////////////////////////////////////
//
class ATL_NO_VTABLE CAdminSession : 
  public IDispatchImpl<IAdminSession, &IID_IAdminSession, &LIBID_ALLEGIANCESERVERLib>,
  public IProvideClassInfo2Impl<&CLSID_AdminSession, &DIID__IAdminSessionEvents, &LIBID_ALLEGIANCESERVERLib>,
  public IConnectionPointContainerImpl<CAdminSession>,
  public IAdminSessionEventsCP<CAdminSession>,
  public CComObjectRootEx<CComMultiThreadModel>,
  public CComCoClass<CAdminSession, &CLSID_AdminSession>
{
// Declarations
public:
  DECLARE_REGISTRY_RESOURCEID(IDR_ADMINSESSION)
  DECLARE_PROTECT_FINAL_CONSTRUCT()
  DECLARE_CLASSFACTORY_EX(CAdminSessionClass)

// Interface Map
public:
  BEGIN_COM_MAP(CAdminSession)
    COM_INTERFACE_ENTRY(IAdminSession)
    COM_INTERFACE_ENTRY(IDispatch)
    COM_INTERFACE_ENTRY(IProvideClassInfo)
    COM_INTERFACE_ENTRY(IProvideClassInfo2)
    COM_INTERFACE_ENTRY(IConnectionPointContainer)
  END_COM_MAP()

// Category Map
public:
  BEGIN_CATEGORY_MAP(CAdminSession)
    IMPLEMENTED_CATEGORY(CATID_AllegianceAdmin)
    IMPLEMENTED_CATEGORY(CATID_SafeForScripting)
  END_CATEGORY_MAP()

// Connection Point Map
public:
  BEGIN_CONNECTION_POINT_MAP(CAdminSession)
    CONNECTION_POINT_ENTRY(DIID__IAdminSessionEvents)
    CONNECTION_POINT_ENTRY(IID_IAdminSessionEvents)
  END_CONNECTION_POINT_MAP()

// Construction / Destruction
public:
  CAdminSession();
  virtual ~CAdminSession();
  HRESULT FinalConstruct();
  void FinalRelease();

// Operations
public:
  static void  DestroyAllSessions();

// Overrides
protected:
  virtual IUnknown* OnGetUnknown();

// Implementation
public:
  static int GetSessionCount();
  static CAdminSession* GetLastSession();
  static CAdminSession* GetWhoStartedServer();

// IAdminSession Interface Methods
public:
  STDMETHODIMP put_SessionInfo(ITCSessionInfo* pSessionInfo);
  STDMETHODIMP get_SessionInfo(ITCSessionInfo** ppSessionInfo);
  STDMETHODIMP get_SessionInfos(ITCSessionInfos** ppSessionInfos);
  STDMETHODIMP get_Server(IAdminServer** ppAdminServer);
  STDMETHODIMP get_IsEventActivated(AGCEventID event, AGCUniqueID uniqueID, BOOL *pVal);
  STDMETHODIMP ActivateEvents(AGCEventID AGCEvent, AGCUniqueID uniqueID = -1);
  STDMETHODIMP DeactivateEvents(AGCEventID AGCEvent, AGCUniqueID uniqueID = -1);
  STDMETHODIMP ActivateAllEvents();
  STDMETHODIMP DeactivateAllEvents();
  STDMETHODIMP get_ProcessID(long* pdwProcessID);
  STDMETHODIMP get_Version(IAGCVersionInfo** ppVersion);
  STDMETHODIMP Stop();
  STDMETHODIMP Pause();
  STDMETHODIMP get_WhoStartedServer(IAdminSession** IAdminSession); 
  STDMETHODIMP get_EventLog(IAGCEventLogger** ppEventLogger);
  STDMETHODIMP get_PerfCounters(IAGCEvent** ppPerfCounters);
  STDMETHODIMP SendAdminChat(BSTR bstrText, long nUserID, DATE dateOriginal);
  STDMETHODIMP Continue();

// Types
protected:
  typedef TCObjectLock<CAdminSession>         XLock;
  typedef TCObjectLock<TCAutoCriticalSection> XLockStatic;
  typedef std::vector<CAdminSession*>         XSessions;
  typedef XSessions::iterator                 XSessionsIt;

// Data Members
protected:
  CAdminSessionEventSink*      m_pEventSink;
  static TCAutoCriticalSection s_cs;
  static XSessions             s_vecSessions;

  // this is temporary until launcher obj gets done
  static CAdminSession*   s_pWhoStartedServer; // This is NULL unless a session started the server
};


/////////////////////////////////////////////////////////////////////////////
// Implementation

inline int CAdminSession::GetSessionCount()
{
  XLockStatic lock(&s_cs);
  return s_vecSessions.size();
}

inline CAdminSession* CAdminSession::GetLastSession()
{
  XLockStatic lock(&s_cs);
  return *s_vecSessions.rbegin();
}

inline CAdminSession* CAdminSession::GetWhoStartedServer()
{
  XLockStatic lock(&s_cs);
  return s_pWhoStartedServer;
}


/////////////////////////////////////////////////////////////////////////////