#pragma once

/////////////////////////////////////////////////////////////////////////////
// AdminSessionEventSink.h : Declaration of the CAdminSessionEventSink


/////////////////////////////////////////////////////////////////////////////
// Forward Declarations

class CAdminSession;


/////////////////////////////////////////////////////////////////////////////
// CAdminSessionEventSink
class ATL_NO_VTABLE CAdminSessionEventSink : 
  public IAGCEventSink,
  public CComObjectRootEx<CComMultiThreadModel>
{
// Declarations
public:
  DECLARE_PROTECT_FINAL_CONSTRUCT()

// Interface Map
public:
  BEGIN_COM_MAP(CAdminSessionEventSink)
    COM_INTERFACE_ENTRY(IAGCEventSink)
  END_COM_MAP()

// Construction / Destruction
public:
  CAdminSessionEventSink();
  void Init(CAdminSession* pSession);
  void Term();

// Attributes:
public:
  DWORD GetGITCookie() const;

// IAGCEventSink Interface Methods
public:
  STDMETHODIMP OnEventTriggered(IAGCEvent* pEvent);

// Data Members
protected:
  CAdminSession* m_pSession;
  DWORD          m_dwGITCookie;
};


/////////////////////////////////////////////////////////////////////////////
// Attributes:

inline DWORD CAdminSessionEventSink::GetGITCookie() const
{
  return m_dwGITCookie;
}


/////////////////////////////////////////////////////////////////////////////