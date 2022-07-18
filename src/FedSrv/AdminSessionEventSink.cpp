/////////////////////////////////////////////////////////////////////////////
// AdminSessionEventSink.cpp : Implementation of the CAdminSessionEventSink
// class.

#include "pch.h"
#include "AdminSession.h"
#include "AdminSessionEventSink.h"


/////////////////////////////////////////////////////////////////////////////
// CAdminSessionEventSink


/////////////////////////////////////////////////////////////////////////////
// Construction / Destruction

CAdminSessionEventSink::CAdminSessionEventSink() :
  m_pSession(NULL),
  m_dwGITCookie(0)
{
  // #define CAdminSessionEventSink_TRACE_CONSTRUCTION
  #ifdef CAdminSessionEventSink_TRACE_CONSTRUCTION
    _TRACE_BEGIN
      DWORD id = GetCurrentThreadId();
      _TRACE_PART2("CAdminSessionEventSink::CAdminSessionEventSink(): ThreadId = %d (0x%X)\n", id, id);
      _TRACE_PART1("\tRaw pointer = 0x%08X", this);
      _TRACE_PART1(", IAGCEventSink* = 0x%08X\n", static_cast<IAGCEventSink*>(this));
    _TRACE_END
  #endif // CAdminSessionEventSink_TRACE_CONSTRUCTION
}

void CAdminSessionEventSink::Init(CAdminSession* pSession)
{
  // Save the owning session object
  ZAssert(pSession);
  ZAssert(!m_pSession);
  m_pSession = pSession;

  // Add ourself to the GIT
  GetAGCGlobal()->RegisterInterfaceInGlobal(
    static_cast<IAGCEventSink*>(this), IID_IAGCEventSink, &m_dwGITCookie);
}

void CAdminSessionEventSink::Term()
{
  ZAssert(m_dwGITCookie);

  // Revoke all events for which we are registered
  GetAGCGlobal()->RevokeAllEvents(reinterpret_cast<IAGCEventSink*>(this));

  // Revoke ourself from the GIT
  GetAGCGlobal()->RevokeInterfaceFromGlobal(m_dwGITCookie);
  m_dwGITCookie = 0;
}


/////////////////////////////////////////////////////////////////////////////
// IAGCEventSink Interface Methods

STDMETHODIMP CAdminSessionEventSink::OnEventTriggered(IAGCEvent* pEvent)
{
  ZAssert(m_pSession);
  // #define CAdminSessionEventSink_TRACE_OnEventTriggered
  #ifdef CAdminSessionEventSink_TRACE_OnEventTriggered
    _TRACE_BEGIN
      DWORD id = GetCurrentThreadId();
      _TRACE_PART2("CAdminSessionEventSink::OnEventTriggered(): ThreadId = %d (0x%X)\n", id, id);
      _TRACE_PART1("\tRaw pointer = 0x%08X", this);
      _TRACE_PART1(", IAGCEventSink* = 0x%08X\n", static_cast<IAGCEventSink*>(this));
      _TRACE_PART1("\tIAGCEvent*     = 0x%08X", pEvent);
      _TRACE_PART1(", refcount = %d\n", m_dwRef);
    _TRACE_END
  #endif // CAdminSessionEventSink_OnEventTriggered

  m_pSession->Fire_OnEvent(pEvent);
  return S_OK;
}

