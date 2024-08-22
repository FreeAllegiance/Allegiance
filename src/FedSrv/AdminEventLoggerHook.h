#pragma once

/////////////////////////////////////////////////////////////////////////////
// AdminEventLoggerHook.h : Declaration of the CAdminEventLoggerHook


/////////////////////////////////////////////////////////////////////////////
// CAdminEventLoggerHook
class ATL_NO_VTABLE CAdminEventLoggerHook : 
  public IAGCEventLoggerHook,
  public CComObjectRootEx<CComMultiThreadModel>
{
// Declarations
public:
  DECLARE_PROTECT_FINAL_CONSTRUCT()

// Interface Map
public:
  BEGIN_COM_MAP(CAdminEventLoggerHook)
    COM_INTERFACE_ENTRY(IAGCEventLoggerHook)
  END_COM_MAP()

// Implementation
private:
  static void EventLogged(CQLogEvent* pquery);

// IAGCEventLoggerHook Interface Methods
public:
  STDMETHODIMP LogEvent(IAGCEvent* pEvent, VARIANT_BOOL bSynchronous);
};


/////////////////////////////////////////////////////////////////////////////