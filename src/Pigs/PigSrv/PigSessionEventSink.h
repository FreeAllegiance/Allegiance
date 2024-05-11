#pragma once

/////////////////////////////////////////////////////////////////////////////
// PigSessionEventSink.h : Declaration of the CPigSessionEventSink

#include "PigSrv.h"


/////////////////////////////////////////////////////////////////////////////
// Forward Declarations

class CPigSession;


/////////////////////////////////////////////////////////////////////////////
// CPigSessionEventSink
class ATL_NO_VTABLE CPigSessionEventSink : 
  public IAGCEventSink,
  public CComObjectRootEx<CComMultiThreadModel>
{
// Declarations
public:
  DECLARE_PROTECT_FINAL_CONSTRUCT()

// Interface Map
public:
  BEGIN_COM_MAP(CPigSessionEventSink)
    COM_INTERFACE_ENTRY(IAGCEventSink)
  END_COM_MAP()

// Construction / Destruction
public:
  CPigSessionEventSink();
  void Init(CPigSession* pSession);
  void Term();

// Attributes:
public:
  DWORD GetGITCookie() const;

// IAGCEventSink Interface Methods
public:
  STDMETHODIMP OnEventTriggered(IAGCEvent* pEvent);

// Data Members
protected:
  CPigSession* m_pSession;
  DWORD        m_dwGITCookie;
};


/////////////////////////////////////////////////////////////////////////////
// Attributes:

inline DWORD CPigSessionEventSink::GetGITCookie() const
{
  return m_dwGITCookie;
}


/////////////////////////////////////////////////////////////////////////////