#pragma once

/////////////////////////////////////////////////////////////////////////////
// AGCEventLogger.h : Declaration of the CAGCEventLogger class

#include <AGC.h>
#include <atldbcli.h>
#include <..\TCLib\AutoHandle.h>
#include <..\TCLib\ObjectLock.h>
#include <..\TCLib\RangeSet.h>
#include <..\TCLib\WorkerThread.h>
#include <XMLPtrs.h>
#include "resource.h"


/////////////////////////////////////////////////////////////////////////////
// CAGCEventLogger
class ATL_NO_VTABLE CAGCEventLogger : 
  public IAGCEventSink,
  public IAGCEventSinkSynchronous,
  public IDispatchImpl<IAGCEventLogger, &IID_IAGCEventLogger, &LIBID_AGCLib>,
  public IAGCEventLoggerPrivate,
  public TCWorkerThread,
  public AGCObjectSafetyImpl<CAGCEventLogger>,
  public CComObjectRootEx<CComMultiThreadModel>,
  public CComCoClass<CAGCEventLogger, &CLSID_AGCEventLogger>
{
// Types
protected:
  typedef std::vector<IXMLDOMNodePtr> XNodes;
  typedef XNodes::iterator            XNodeIt;

// Declarations
public:
  DECLARE_REGISTRY_RESOURCEID(IDR_AGCEventLogger)
  DECLARE_PROTECT_FINAL_CONSTRUCT()

// Interface Map
public:
  BEGIN_COM_MAP(CAGCEventLogger)
    COM_INTERFACE_ENTRY(IAGCEventSink)
    COM_INTERFACE_ENTRY(IAGCEventSinkSynchronous)
    COM_INTERFACE_ENTRY(IAGCEventLogger)
    COM_INTERFACE_ENTRY(IDispatch)
    COM_INTERFACE_ENTRY(IAGCEventLoggerPrivate)
    COM_INTERFACE_ENTRY(IObjectSafety)
  END_COM_MAP()

// Category Map
public:
  BEGIN_CATEGORY_MAP(CAGCEvent)
    IMPLEMENTED_CATEGORY(CATID_AGC)
    IMPLEMENTED_CATEGORY(CATID_SafeForScripting)
    IMPLEMENTED_CATEGORY(CATID_SafeForInitializing)
  END_CATEGORY_MAP()

// Construction / Destruction
public:
  CAGCEventLogger();

// Overrides
protected:
  virtual IUnknown* OnGetUnknown();
  virtual TC_WorkItemRelProc OnGetWorkItemRelProc();
  virtual void OnMessage(UINT idMsg, int cParams, LPARAM* rgParams);
  static void WINAPI ReleaseWorkItem(UINT idMsg, int cParams, LPARAM* rgParams);

// Implementation
protected:
  void LogEvent(IAGCEvent* pEvent, bool bSynchronous);
  HRESULT CloseDatabase();
  HRESULT ChangeDatabase(IDBInitialize* pInit, DBID* pidTable,
    CDBPropSet* pPropSet);
  HRESULT CloseNTEventLog();
  HRESULT ChangeNTEventLog(BSTR bstrComputer);
  static HKEY RootKeyFromString(BSTR bstrRegKey, DWORD* cchEaten);
  HRESULT ReadStringValueFromRegistry(LPCTSTR pszValueName,
    CComBSTR& bstrOut);
  HRESULT OpenWriteableRegKey(CRegKey& keyWrite);
  void FireNTEventLogStopped();
  void FireDBEventLogStopped();
  HRESULT CopyChildNodes(IXMLDOMNode* pNode, XNodes& nodes);
  HRESULT RemoveStyleSheetPIs(IXMLDOMNode* pNode);
  HRESULT ProcessXMLNode(IXMLDOMNode* pNode, IXMLDOMNode* pNodeParent);

// IAGCEventSink Interface Methods
public:
  STDMETHODIMP OnEventTriggered(IAGCEvent* pEvent);

// IAGCEventSinkSynchronous Interface Methods
public:
  STDMETHODIMP OnEventTriggeredSynchronous(IAGCEvent* pEvent);

// IAGCEventLogger Interface Methods
public:
  STDMETHODIMP get_EventList(BSTR* pbstrEventListXML);
  STDMETHODIMP put_NTEventLog(BSTR bstrComputer);
  STDMETHODIMP get_NTEventLog(BSTR* pbstrComputer);
  STDMETHODIMP put_DBEventLog(IAGCDBParams* pDBParams);
  STDMETHODIMP get_DBEventLog(IAGCDBParams** ppDBParams);
  STDMETHODIMP put_EnabledNTEvents(IAGCEventIDRanges* pEvents);
  STDMETHODIMP get_EnabledNTEvents(IAGCEventIDRanges** ppEvents);
  STDMETHODIMP put_EnabledDBEvents(IAGCEventIDRanges* pEvents);
  STDMETHODIMP get_EnabledDBEvents(IAGCEventIDRanges** ppEvents);
  STDMETHODIMP get_DefaultEnabledNTEvents(IAGCEventIDRanges** ppEvents);
  STDMETHODIMP get_DefaultEnabledDBEvents(IAGCEventIDRanges** ppEvents);

// IAGCEventLoggerPrivate Interface Methods
public:
  STDMETHODIMP Initialize(BSTR bstrSourceApp, BSTR bstrRegKey);
  STDMETHODIMP Terminate();
  STDMETHODIMP put_LoggingToNTEnabled(VARIANT_BOOL bEnabled);
  STDMETHODIMP get_LoggingToNTEnabled(VARIANT_BOOL* pbEnabled);
  STDMETHODIMP put_LoggingToDBEnabled(VARIANT_BOOL bEnabled);
  STDMETHODIMP get_LoggingToDBEnabled(VARIANT_BOOL* pbEnabled);
  STDMETHODIMP put_HookForNTLogging(IAGCEventLoggerHook* pHook);
  STDMETHODIMP get_HookForNTLogging(IAGCEventLoggerHook** ppHook);
  STDMETHODIMP put_HookForDBLogging(IAGCEventLoggerHook* pHook);
  STDMETHODIMP get_HookForDBLogging(IAGCEventLoggerHook** ppHook);

// Types
protected:
  typedef TCObjectLock<CAGCEventLogger> XLock;
  typedef rangeset< range<AGCEventID> > XRangeSet;
  typedef XRangeSet::iterator           XRangeSetIt;
  enum
  {
    e_LogEvent,
    e_CloseNTEventLog,
    e_ChangeNTEventLog,
    e_CloseDatabase,
    e_ChangeDatabase,
  };
  struct XEvent
  {
  // Column Map
  public:
    BEGIN_COLUMN_MAP(XEvent)
      COLUMN_ENTRY_TYPE(2, DBTYPE_I4 , m_idEvent       )
      COLUMN_ENTRY_TYPE(4, DBTYPE_STR, m_szComputerName)
      COLUMN_ENTRY_TYPE(5, DBTYPE_I4 , m_idSubject     )
      COLUMN_ENTRY_TYPE(6, DBTYPE_STR, m_szSubjectName )
      COLUMN_ENTRY_TYPE(7, DBTYPE_STR, m_szContext     )
    	COLUMN_ENTRY_TYPE(8, DBTYPE_STR, m_szObjRef      )
    END_COLUMN_MAP()

  // Data Members
  public:
    long     m_idEvent;
    char     m_szComputerName[16];
    long     m_idSubject;
    char     m_szSubjectName[32];
    char     m_szContext[24];
    char     m_szObjRef[4000];
  };

// Data Members
protected:
  CDataSource                m_ds;
  CSession                   m_session;
  CTable<CAccessor<XEvent> > m_table;
  CComBSTR                   m_bstrTableName;
  DBID                       m_idTable;
  CDBPropSet                 m_propset;
  CComBSTR                   m_bstrSourceApp;
  CRegKey                    m_key;
  CComBSTR                   m_bstrNTEventLog;
  TCEventSourceHandle        m_shEventLog;
  IAGCEventIDRangesPtr       m_spRangesNT;
  IAGCEventIDRangesPtr       m_spRangesDB;
  bool                       m_bInitializing;
  CComBSTR                   m_bstrID;
  CComBSTR                   m_bstrLogAsNTEvent;
  CComBSTR                   m_bstrLogAsDBEvent;
  bool                       m_bLoggingToNTEnabled;  // NOT persisted to reg!
  bool                       m_bLoggingToDBEnabled;  // NOT persisted to reg!
  IAGCEventLoggerHookPtr     m_spHookNT;
  IAGCEventLoggerHookPtr     m_spHookDB;
};


/////////////////////////////////////////////////////////////////////////////