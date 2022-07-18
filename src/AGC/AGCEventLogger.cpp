/////////////////////////////////////////////////////////////////////////////
// AGCEventLogger.cpp : Implementation of CAGCEventLogger
//

#include "pch.h"
#include <..\TCLib\AutoHandle.h>
#include <..\TCLib\UtilImpl.h>
#include <..\TCLib\WinApp.h>
#include <..\TCAtl\SimpleStream.h>
#include "AGCEventLogger.h"
#include "AGCEventDef.h"


/////////////////////////////////////////////////////////////////////////////
// CAGCEventLogger

TC_OBJECT_EXTERN_IMPL(CAGCEventLogger)


/////////////////////////////////////////////////////////////////////////////
// Construction / Destruction

CAGCEventLogger::CAGCEventLogger() :
  m_bInitializing(true),
  m_bstrID(L"id"),
  m_bstrLogAsNTEvent(L"LogAsNTEvent"),
  m_bstrLogAsDBEvent(L"LogAsDBEvent"),
  m_bLoggingToNTEnabled(true),
  m_bLoggingToDBEnabled(true)
{
  TCZeroMemory(&m_idTable);
}


/////////////////////////////////////////////////////////////////////////////
// Overrides

IUnknown* CAGCEventLogger::OnGetUnknown()
{
  return GetUnknown();
}

TC_WorkItemRelProc CAGCEventLogger::OnGetWorkItemRelProc()
{
  return ReleaseWorkItem;
}

void CAGCEventLogger::OnMessage(UINT idMsg, int cParams, LPARAM* rgParams)
{
  switch (idMsg)
  {
    case e_LogEvent:
    {
      PRIVATE_ASSERTE(2 <= cParams);
      IAGCEvent* pEvent = reinterpret_cast<IAGCEvent*>(rgParams[0]);
      HANDLE     hevt   = reinterpret_cast<HANDLE    >(rgParams[1]);
      LogEvent(pEvent, !!hevt);
      break;
    };
    case e_CloseNTEventLog:
    {
      PRIVATE_ASSERTE(2 <= cParams);
      HANDLE hevt  = reinterpret_cast<HANDLE  >(rgParams[0]);
      HRESULT* phr = reinterpret_cast<HRESULT*>(rgParams[1]);
      *phr = CloseNTEventLog();
      break;
    }
    case e_ChangeNTEventLog:
    {
      PRIVATE_ASSERTE(3 <= cParams);
      BSTR bstr       = reinterpret_cast<BSTR    >(rgParams[0]);
      HANDLE hevt     = reinterpret_cast<HANDLE  >(rgParams[1]);
      HRESULT* phr    = reinterpret_cast<HRESULT*>(rgParams[2]);
      *phr = ChangeNTEventLog(bstr);
      break;
    }
    case e_CloseDatabase:
    {
      PRIVATE_ASSERTE(2 <= cParams);
      HANDLE hevt  = reinterpret_cast<HANDLE  >(rgParams[0]);
      HRESULT* phr = reinterpret_cast<HRESULT*>(rgParams[1]);
      *phr = CloseDatabase();
      break;
    }
    case e_ChangeDatabase:
    {
      PRIVATE_ASSERTE(5 <= cParams);
      IDBInitialize* pInit = reinterpret_cast<IDBInitialize*>(rgParams[0]);
      DBID* pidTable       = reinterpret_cast<DBID*         >(rgParams[1]);
      CDBPropSet* pPropSet = reinterpret_cast<CDBPropSet*   >(rgParams[2]);
      HANDLE hevt          = reinterpret_cast<HANDLE        >(rgParams[3]);
      HRESULT* phr         = reinterpret_cast<HRESULT*      >(rgParams[4]);
      *phr = ChangeDatabase(pInit, pidTable, pPropSet);
      break;
    }
  }
}

void WINAPI CAGCEventLogger::ReleaseWorkItem(UINT idMsg, int cParams, LPARAM* rgParams)
{
  switch (idMsg)
  {
    case e_LogEvent:
    {
      PRIVATE_ASSERTE(2 <= cParams);
      IAGCEvent* pEvent = reinterpret_cast<IAGCEvent*>(rgParams[0]);
      HANDLE     hevt   = reinterpret_cast<HANDLE    >(rgParams[1]);
      pEvent->Release();
      if (hevt)
        ::SetEvent(hevt);
      break;
    };
    case e_CloseNTEventLog:
    {
      PRIVATE_ASSERTE(2 <= cParams);
      HANDLE hevt  = reinterpret_cast<HANDLE  >(rgParams[0]);
      HRESULT* phr = reinterpret_cast<HRESULT*>(rgParams[1]);
      SetEvent(hevt);
      break;
    }
    case e_ChangeNTEventLog:
    {
      PRIVATE_ASSERTE(3 <= cParams);
      BSTR bstr    = reinterpret_cast<BSTR    >(rgParams[0]);
      HANDLE hevt  = reinterpret_cast<HANDLE  >(rgParams[1]);
      HRESULT* phr = reinterpret_cast<HRESULT*>(rgParams[2]);
      SetEvent(hevt);
      break;
    }
    case e_CloseDatabase:
    {
      PRIVATE_ASSERTE(2 <= cParams);
      HANDLE hevt  = reinterpret_cast<HANDLE  >(rgParams[0]);
      HRESULT* phr = reinterpret_cast<HRESULT*>(rgParams[1]);
      SetEvent(hevt);
      break;
    }
    case e_ChangeDatabase:
    {
      PRIVATE_ASSERTE(5 <= cParams);
      IDBInitialize* pInit = reinterpret_cast<IDBInitialize*>(rgParams[0]);
      DBID* pidTable       = reinterpret_cast<DBID*         >(rgParams[1]);
      CDBPropSet* pPropSet = reinterpret_cast<CDBPropSet*   >(rgParams[2]);
      HANDLE hevt          = reinterpret_cast<HANDLE        >(rgParams[3]);
      HRESULT* phr         = reinterpret_cast<HRESULT*      >(rgParams[4]);
      pInit->Release();
      SetEvent(hevt);
      break;
    }
  }
}


/////////////////////////////////////////////////////////////////////////////
// Implementation

void DisplayOLEDBErrorRecords(HRESULT hrErr = S_OK)
{
	CDBErrorInfo ErrorInfo;
	ULONG        cRecords;
	HRESULT      hr;
	ULONG        i;
	CComBSTR     bstrDesc, bstrHelpFile, bstrSource;
	GUID         guid;
	DWORD        dwHelpContext;
	WCHAR        wszGuid[40];
	USES_CONVERSION;

  TCERRLOG_BEGIN_SIZE(_MAX_PATH * 8);

	// If the user passed in an HRESULT then trace it
	if (hrErr != S_OK)
  {
		TCERRLOG_PART1("OLE DB Error Record dump for hr = 0x%x\n", hrErr);
  }

	LCID lcLocale = GetSystemDefaultLCID();

	hr = ErrorInfo.GetErrorRecords(&cRecords);
	if (FAILED(hr) && ErrorInfo.m_spErrorInfo == NULL)
	{
		TCERRLOG_PART1("No OLE DB Error Information found: hr = 0x%x\n", hr);
	}
	else
	{
		for (i = 0; i < cRecords; i++)
		{
			hr = ErrorInfo.GetAllErrorInfo(i, lcLocale, &bstrDesc, &bstrSource, &guid,
										&dwHelpContext, &bstrHelpFile);
			if (FAILED(hr))
			{
				TCERRLOG_PART1("OLE DB Error Record dump retrieval failed: hr = 0x%x\n", hr );
				return;
			}
			StringFromGUID2(guid, wszGuid, sizeof(wszGuid) / sizeof(WCHAR));
			TCERRLOG_PART5(
				"Source:\"%ls\"\nDescription:\"%ls\"\nHelp File:\"%ls\"\nHelp Context:%4d\nGUID:%ls\n",
				bstrSource, bstrDesc, bstrHelpFile, dwHelpContext, wszGuid);
			bstrSource.Empty();
			bstrDesc.Empty();
			bstrHelpFile.Empty();
		}
	}
  TCERRLOG_END
}

void CAGCEventLogger::LogEvent(IAGCEvent* pEvent, bool bSynchronous)
{
  PRIVATE_ASSERTE(pEvent);
  XLock lock(this);

  // Determine if we can log to either event log type
  bool bLogToNT = m_bLoggingToNTEnabled && !m_shEventLog.IsNull();
  bool bLogToDB = m_bLoggingToDBEnabled && m_ds.m_spInit != NULL;

  // Get the event ID
  AGCEventID idEvent;
  PRIVATE_VERIFYE(SUCCEEDED(pEvent->get_ID(&idEvent)));

  // Determine if the event is enabled for each event log type
  VARIANT_BOOL bEnabled;
  if (bLogToNT && NULL != m_spRangesNT
    && SUCCEEDED(m_spRangesNT->get_IntersectsWithValue(idEvent, &bEnabled)))
      bLogToNT = !!bEnabled;
  if (bLogToDB && NULL != m_spRangesDB
    && SUCCEEDED(m_spRangesDB->get_IntersectsWithValue(idEvent, &bEnabled)))
      bLogToDB = !!bEnabled;

  // Unlock the CritSec
  lock.Unlock();

  // Do nothing if we're not logging this event anywhere
  if (!bLogToNT && !bLogToDB)
    return;

  // Find the event definition
  const CAGCEventDef::XEventDef* pEventDef = CAGCEventDef::find(idEvent);
  PRIVATE_ASSERTE(pEventDef);

  // Output the event to the debug monitor
  #if 0 && defined(_DEBUG)
  {
    CComBSTR bstrDescription, bstrSubjectName;
    PRIVATE_VERIFYE(SUCCEEDED(pEvent->get_Description(&bstrDescription)));
    PRIVATE_VERIFYE(SUCCEEDED(pEvent->get_SubjectName (&bstrSubjectName)));
    int cchDbg = bstrSubjectName.Length() + bstrDescription.Length() + 16;
    _TRACE_BEGIN_SIZE(cchDbg)
      if (bstrSubjectName.Length())
        _TRACE_PART1("%ls:\n", (BSTR)bstrSubjectName);
      if (bstrDescription.Length())
        _TRACE_PART1("%ls\n" , (BSTR)bstrDescription);
    _TRACE_END
  }
  #endif // defined(_DEBUG)

  // Log the event to the NT Event Log, if specified
  if (bLogToNT)
  {
    PRIVATE_ASSERTE(IsWinNT());

    IAGCEventLoggerHookPtr spHook;
    get_HookForNTLogging(&spHook);
    if (NULL == spHook || S_FALSE == spHook->LogEvent(pEvent, VARBOOL(bSynchronous)))
    {
      XLock lockNT(this);
      if (m_bLoggingToNTEnabled && !m_shEventLog.IsNull())
      {
        // Get the event severity
        WORD wSeverity = pEventDef->m_wSeverity;

        // Create the full event id (regarding the MC-generated id's)
        UINT nBits;
        switch (wSeverity)
        {
          case EVENTLOG_INFORMATION_TYPE: nBits = 0x01 << 30; break;
          case EVENTLOG_WARNING_TYPE    : nBits = 0x02 << 30; break;
          case EVENTLOG_ERROR_TYPE      : nBits = 0x03 << 30; break;
          case EVENTLOG_SUCCESS         : nBits = 0x00 << 30; break;
          default                       : PRIVATE_ASSERTE(!"Bad severity code.");
        }
        DWORD dwEventID = nBits | idEvent;

        // Get the event replacement parameters
        CAGCEventDef::XParamStrings vecParamStrings;
        PRIVATE_VERIFYE(SUCCEEDED(CAGCEventDef::GetEventParameters(pEvent,
          vecParamStrings, pEventDef)));
	
		// mdvalley: prevent crashes when no %Parameter% in strings.
		// That took so long to figure out.
		WORD NumStrings = vecParamStrings.size();
		LPCWSTR* lpStrings = NULL;
		if(NumStrings)
			lpStrings = (LPCWSTR*)(&(*vecParamStrings.begin()));

        // Report the event to the NT Event log
        ReportEventW(m_shEventLog, wSeverity, 0, dwEventID, NULL,
          vecParamStrings.size(), 0,
// VS.Net 2003 port
#if _MSC_VER >= 1310
        lpStrings, NULL);
//		(LPCWSTR*)(&(*vecParamStrings.begin())), NULL);
#else
		(LPCWSTR*)(vecParamStrings.begin()), NULL);
#endif

        // Free the replacement parameters BSTR's
        for (CAGCEventDef::XParamStrings::iterator it = vecParamStrings.begin();
          it != vecParamStrings.end(); ++it)
            SysFreeString(*it);
      }
    }
  }

  // Log the event to the DB Event Log, if specified
  if (bLogToDB)
  {
    IAGCEventLoggerHookPtr spHook;
    get_HookForDBLogging(&spHook);
    if (NULL == spHook || S_FALSE == spHook->LogEvent(pEvent, VARBOOL(bSynchronous)))
    {
      XLock lockDB(this);
      if (m_bLoggingToDBEnabled && m_ds.m_spInit != NULL)
      {
        // Get the fields of the specified event object
        long       idSubject;
        CComBSTR   bstrComputerName, bstrSubjectName, bstrContext;
        PRIVATE_VERIFYE(SUCCEEDED(pEvent->get_SubjectID   (&idSubject       )));
        PRIVATE_VERIFYE(SUCCEEDED(pEvent->get_ComputerName(&bstrComputerName)));
        PRIVATE_VERIFYE(SUCCEEDED(pEvent->get_SubjectName (&bstrSubjectName )));
        PRIVATE_VERIFYE(SUCCEEDED(pEvent->get_Context     (&bstrContext     )));

        // Open the session and table, and insert the row
        HRESULT hr;
        if (SUCCEEDED(hr = m_session.Open(m_ds)))
        {
          if (SUCCEEDED(hr = m_table.Open(m_session, m_idTable, &m_propset)))
          {
            // Populate the fields of the new row
            USES_CONVERSION;
            // Event
            m_table.m_idEvent   = idEvent;
            // Subject
            m_table.m_idSubject = idSubject;
            // ComputerName
            if (bstrComputerName.Length())
              lstrcpyn(m_table.m_szComputerName, OLE2CT(bstrComputerName),
                sizeofArray(m_table.m_szComputerName));
            else
              *m_table.m_szComputerName = TEXT('\0');
            // SubjectName
            if (bstrSubjectName.Length())
              lstrcpyn(m_table.m_szSubjectName, OLE2CT(bstrSubjectName),
                sizeofArray(m_table.m_szSubjectName));
            else
              *m_table.m_szSubjectName = TEXT('\0');
            // Context
            if (bstrContext.Length())
              lstrcpyn(m_table.m_szContext, OLE2CT(bstrContext),
                sizeofArray(m_table.m_szContext));
            else
              *m_table.m_szContext = TEXT('\0');

            // Get the event object as a string
            PRIVATE_ASSERTE(IPersistStreamInitPtr(pEvent) != NULL || IPersistStreamPtr(pEvent) != NULL);
            PRIVATE_ASSERTE(IMarshalPtr(pEvent) != NULL);
            CComBSTR bstrTemp;
            PRIVATE_VERIFYE(SUCCEEDED(pEvent->SaveToString(&bstrTemp)));
            WideCharToMultiByte(CP_ACP, 0, bstrTemp, -1,
              m_table.m_szObjRef, sizeof(m_table.m_szObjRef), 0, 0);

            // Insert the row into the table
            hr = m_table.Insert();
          }
        }

        // Report any errors
        if (FAILED(hr))
          DisplayOLEDBErrorRecords(hr);

        // Close the table
        m_table.Close();

        // Close the session
        m_session.Close();
      }
    }
  }
}

HRESULT CAGCEventLogger::CloseDatabase()
{
  XLock lock(this);

  // Close the previous database connection
  m_table.Close();
  m_session.Close();
  m_ds.Close();

  // Indicate success
  return S_OK;
}

HRESULT CAGCEventLogger::ChangeDatabase(IDBInitialize* pInit,
  DBID* pidTable, CDBPropSet* pPropSet)
{
  XLock lock(this);

  // Close the previous database connection
  CloseDatabase();

  // Set the specified database connection
  m_ds.m_spInit            = pInit;
  m_bstrTableName          = pidTable->uName.pwszName;
  m_idTable.eKind          = DBKIND_NAME;
  m_idTable.uName.pwszName = m_bstrTableName;
  m_propset                = *pPropSet;

  // Test the database by opening it
  m_session.Open(m_ds);
  HRESULT hr = m_table.Open(m_session, m_idTable, &m_propset);
  if (FAILED(hr))
    m_bstrTableName.Empty();
  m_table.Close();
  m_session.Close();
  return hr;
}


HRESULT CAGCEventLogger::CloseNTEventLog()
{
  XLock lock(this);

  // Close the previous event log
  m_shEventLog = NULL;

  // Indicate success
  return S_OK;
}

HRESULT CAGCEventLogger::ChangeNTEventLog(BSTR bstrComputer)
{
  XLock lock(this);

  // Close the previous event log
  CloseNTEventLog();

  // Open the NT event log on the specified computer
  BSTR bstr = BSTRLen(bstrComputer) ? bstrComputer : NULL;
  m_shEventLog = RegisterEventSourceW(bstr, m_bstrSourceApp);

  // Indicate success or failure
  return !m_shEventLog.IsNull() ? S_OK : HRESULT_FROM_WIN32(GetLastError());
}


HKEY CAGCEventLogger::RootKeyFromString(BSTR bstrRegKey, DWORD* cchEaten)
{
  // Define a static lookup table
  struct XRootKeys
  {
    LPCOLESTR pszName;
    DWORD     cchName;
    HKEY      hkey;
  };
  const static XRootKeys s_table[] =
  {
    {L"HKCR\\"                 ,  5, HKEY_CLASSES_ROOT    },
    {L"HKCU\\"                 ,  5, HKEY_CURRENT_USER    },
    {L"HKLM\\"                 ,  5, HKEY_LOCAL_MACHINE   },
    {L"HKCC\\"                 ,  5, HKEY_CURRENT_CONFIG  },
    {L"HKPD\\"                 ,  5, HKEY_PERFORMANCE_DATA},
    {L"HKDD\\"                 ,  5, HKEY_DYN_DATA        },

    {L"HKEY_CLASSES_ROOT\\"    , 18, HKEY_CLASSES_ROOT    },
    {L"HKEY_CURRENT_USER\\"    , 18, HKEY_CURRENT_USER    },
    {L"HKEY_LOCAL_MACHINE\\"   , 19, HKEY_LOCAL_MACHINE   },
    {L"HKEY_CURRENT_CONFIG\\"  , 20, HKEY_CURRENT_CONFIG  },
    {L"HKEY_PERFORMANCE_DATA\\", 22, HKEY_PERFORMANCE_DATA},
    {L"HKEY_DYN_DATA\\"        , 14, HKEY_DYN_DATA        },
  };
  const static long s_cEntries = sizeofArray(s_table);

  // Search for an allowed root key name
  if (BSTRLen(bstrRegKey))
  {
    for (long i = 0; i < s_cEntries; ++i)
    {
      if (!_wcsnicmp(s_table[i].pszName, bstrRegKey, s_table[i].cchName))
      {
        *cchEaten = s_table[i].cchName;
        return s_table[i].hkey;
      }
    }
  }

  // Could not find the specified string
  *cchEaten = 0;
  return NULL;
}

HRESULT CAGCEventLogger::ReadStringValueFromRegistry(LPCTSTR pszValueName,
  CComBSTR& bstrOut)
{
  // Initialize the [out] parameter
  bstrOut.Empty();

  // Get the size of data from the specified value
  DWORD cbData = 0;
  long lr = RegQueryValueEx(m_key, pszValueName, NULL, NULL, NULL, &cbData);
  if (ERROR_SUCCESS != lr)
    return HRESULT_FROM_WIN32(lr);

  // Do nothing else if data size is zero
  if (!cbData)
    return S_OK;

  // Allocate an automatic block of memory
  LPTSTR pszValue = (LPTSTR)_alloca(cbData);

  // Attempt to read the specified value
  // mdvalley: No damn QueryStringValue
  // Swap first 2 params.
  lr = m_key.QueryValue(pszValue, pszValueName, &cbData);

  // Save the value string to the [out] parameter
  bstrOut = pszValue;

  // Indicate success
  return S_OK;
}

HRESULT CAGCEventLogger::OpenWriteableRegKey(CRegKey& keyWrite)
{
  // Ensure that the specified key is closed
  keyWrite.Close();

  // Do nothing if we are initializing
  XLock lock(this);
  if (m_bInitializing)
    return S_FALSE;

  // Ensure that the main registry key is open
  if (!m_key.m_hKey)
    return E_UNEXPECTED;

  // Open/create the writeable registry key
  long lr = keyWrite.Create(m_key, TEXT(""));
  return (ERROR_SUCCESS == lr) ? S_OK : HRESULT_FROM_WIN32(lr);
}

void CAGCEventLogger::FireNTEventLogStopped()
{
  // Fire an 'event log stopped' event if a log is open
  bool bLogIsOpen;
  {
    XLock lock(this);
    bLogIsOpen = !m_shEventLog.IsNull();
  }
  if (bLogIsOpen)
  {
    // Fire an event log stopped event
    GetAGCGlobal()->TriggerEventSynchronous(NULL, EventID_NTEventLogStopped,
      NULL, NULL, -1, -1, -1, 0, NULL);
  }
}

void CAGCEventLogger::FireDBEventLogStopped()
{
  // Fire an 'event log stopped' event if a log is open
  bool bLogIsOpen;
  {
    XLock lock(this);
    bLogIsOpen = m_ds.m_spInit != NULL;
  }
  if (bLogIsOpen)
  {
    // Fire an event log stopped event
    GetAGCGlobal()->TriggerEventSynchronous(NULL, EventID_DBEventLogStopped,
      NULL, NULL, -1, -1, -1, 0, NULL);
  }
}

HRESULT CAGCEventLogger::CopyChildNodes(IXMLDOMNode* pNode,
  CAGCEventLogger::XNodes& nodes)
{
  // Get the list of child nodes
  IXMLDOMNodeListPtr spChildren;
  RETURN_FAILED(pNode->get_childNodes(&spChildren));

  // Get the count of child nodes
  long cChildren;
  RETURN_FAILED(spChildren->get_length(&cChildren));

  // Set the size of the array
  nodes.resize(cChildren);

  // Copy each child node to the array
  for (long nIndex = 0; nIndex < cChildren; ++nIndex)
  {
    IXMLDOMNodePtr spChild;
    RETURN_FAILED(spChildren->nextNode(&spChild))
    ZAssert(NULL != spChild);
    nodes[nIndex] = spChild;
  }

  // Indicate success
  return S_OK;
}

HRESULT CAGCEventLogger::RemoveStyleSheetPIs(IXMLDOMNode* pNode)
{
  // Get an array of child nodes
  XNodes nodes;
  RETURN_FAILED(CopyChildNodes(pNode, nodes));

  // Process each child node
  for (XNodeIt it = nodes.begin(); it != nodes.end(); ++it)
  {
    // Get the node's type
    DOMNodeType type;
    RETURN_FAILED((*it)->get_nodeType(&type));
    if (NODE_PROCESSING_INSTRUCTION == type)
    {
      // Get the processing instruction's name
      CComBSTR bstrName;
      RETURN_FAILED((*it)->get_nodeName(&bstrName));
      if (0 == wcscmp(bstrName, L"xml-stylesheet"))
      {
        // Remove it if it's a stylesheet PI
        IXMLDOMNodePtr spNodeRemoved;
        RETURN_FAILED(pNode->removeChild(*it, &spNodeRemoved));
      }
    }
  }

  // Indicate success
  return S_OK;
}

HRESULT CAGCEventLogger::ProcessXMLNode(IXMLDOMNode* pNode,
  IXMLDOMNode* pNodeParent)
{
  // Get an array of child nodes
  XNodes nodes;
  RETURN_FAILED(CopyChildNodes(pNode, nodes));

  // Get the available ranges for the host application
  IAGCEventIDRangesPtr spRangesHost;
  GetAGCGlobal()->GetAvailableEventIDRanges(&spRangesHost);

  // Process each child node
  for (XNodeIt it = nodes.begin(); it != nodes.end(); ++it)
  {
    // Get the node as an element
    IXMLDOMElementPtr spElement(*it);
    if (spElement == NULL)
    {
      // Get the node as a comment
      IXMLDOMCommentPtr spComment(*it);
      if (spComment != NULL)
      {
        // Remove the comment
        IXMLDOMNodePtr spNodeRemoved;
        RETURN_FAILED(pNode->removeChild(spComment, &spNodeRemoved));
      }

      // Leave the node alone
      continue;
    }

    // Get the element's tag name
    CComBSTR bstrTag;
    RETURN_FAILED(spElement->get_tagName(&bstrTag));

    // Process depending on the tag name
    if (0 == wcscmp(bstrTag, L"Event"))
    {
      // Get the element's "id" attribute
      CComVariant varValue;
      RETURN_FAILED(spElement->getAttribute(m_bstrID, &varValue));
      RETURN_FAILED(varValue.ChangeType(VT_I4));
      AGCEventID idEvent = static_cast<AGCEventID>(V_I4(&varValue));

      // Determine if the event id is available for the host application
      VARIANT_BOOL bInRange;
      ZSucceeded(spRangesHost->get_IntersectsWithValue(idEvent, &bInRange));
      if (!bInRange)
      {
        IXMLDOMNodePtr spNodeRemoved;
        RETURN_FAILED(pNode->removeChild(spElement, &spNodeRemoved));
      }
      else
      {
        // Determine if the event id is enabled for NT event logging
        ZSucceeded(m_spRangesNT->get_IntersectsWithValue(idEvent, &bInRange));
        varValue = !!bInRange;
        RETURN_FAILED(spElement->setAttribute(m_bstrLogAsNTEvent, varValue));

        // Determine if the event id is enabled for DB event logging
        ZSucceeded(m_spRangesDB->get_IntersectsWithValue(idEvent, &bInRange));
        varValue = !!bInRange;
        RETURN_FAILED(spElement->setAttribute(m_bstrLogAsDBEvent, varValue));
      }
    }
    else if (0 == wcscmp(bstrTag, L"EventGroup"))
    {
      // Recursively process the group
      RETURN_FAILED(ProcessXMLNode(*it, pNode));
    }
  }

  // Possibly remove the node, unless parent is the document
  if (pNodeParent)
  {
    // Get the list of child nodes
    IXMLDOMNodeListPtr spChildren;
    RETURN_FAILED(pNode->get_childNodes(&spChildren));

    // Get the count of child nodes
    long cChildren;
    RETURN_FAILED(spChildren->get_length(&cChildren));

    // If the node has no remaining children, delete it from the parent
    if (!cChildren)
    {
      // Remove the node from its parent
      IXMLDOMNodePtr spNodeRemoved;
      RETURN_FAILED(pNodeParent->removeChild(pNode, &spNodeRemoved));
    }
  }

  // Indicate success
  return S_OK;
}


/////////////////////////////////////////////////////////////////////////////
// IAGCEventSink Interface Methods

STDMETHODIMP CAGCEventLogger::OnEventTriggered(IAGCEvent* pEvent)
{
  pEvent->AddRef();
  PostMessage(e_LogEvent, 2, pEvent, NULL);
  return S_OK;
}


/////////////////////////////////////////////////////////////////////////////
// IAGCEventSinkSynchronous Interface Methods

STDMETHODIMP CAGCEventLogger::OnEventTriggeredSynchronous(IAGCEvent* pEvent)
{
  TCHandle shEvent = ::CreateEvent(NULL, false, false, NULL);
  pEvent->AddRef();
  PostMessage(e_LogEvent, 2, pEvent, shEvent.GetHandle());
  WaitForSingleObject(shEvent, INFINITE);
  return S_OK;
}


/////////////////////////////////////////////////////////////////////////////
// IAGCEventLogger Interface Methods

STDMETHODIMP CAGCEventLogger::get_EventList(BSTR* pbstrEventListXML)
{
  // Create an instance of the free-threaded MSXML component
  IXMLDOMDocumentPtr spXMLDoc;
  RETURN_FAILED(spXMLDoc.CreateInstance(L"Microsoft.FreeThreadedXMLDOM"));

  // Load the XML from the AGCEventsXML resource
  void* pXMLData = NULL;
  DWORD cbXMLData = 0;
  RETURN_FAILED(LockAndLoadResource(GetAGCGlobal()->GetResourceInstance(),
    TEXT("AGCEventsXML"), MAKEINTRESOURCE(1), &pXMLData, &cbXMLData));

  // Create a simple stream on the resource memory block
  TCSimpleStream stm;
  stm.Init(cbXMLData, pXMLData);

  // Load the XML from the resource data IStream
  CComVariant varXML((IStream*)&stm);
  VARIANT_BOOL bLoaded;
  ZSucceeded(spXMLDoc->load(varXML, &bLoaded));

  // Remove xml-stylesheet processing instructions, if any
  ZSucceeded(RemoveStyleSheetPIs(spXMLDoc));

  // Get the root element
  IXMLDOMElementPtr spRoot;
  ZSucceeded(spXMLDoc->get_documentElement(&spRoot));
  #ifdef _DEBUG
  {
    // Ensure that the root element tag is <AGCEvents>
    CComBSTR bstrRoot;
    ZSucceeded(spRoot->get_tagName(&bstrRoot));
    ZAssert(0 == wcscmp(bstrRoot, L"AGCEvents"));
  }
  #endif // _DEBUG

  // Recursively process the children of each group
  RETURN_FAILED(ProcessXMLNode(spRoot, NULL));

  // Return the XML text as a string
  return spXMLDoc->get_xml(pbstrEventListXML);
}

STDMETHODIMP CAGCEventLogger::put_NTEventLog(BSTR bstrComputer)
{
  // This property can only be set on WinNT machines
  if (!IsWinNT())
    return Error(IDS_E_NTEVENTLOG_WIN9X, IID_IAGCEventLogger);

  // Open a writeable registry key, unless we're initializing
  CRegKey keyWrite;
  RETURN_FAILED(OpenWriteableRegKey(keyWrite));

  // Fire an 'event log stopped' event if a log is open
  FireNTEventLogStopped();

  // Save the new computer name to the registry, unless we're initializing
  if (keyWrite.m_hKey)
  {
    USES_CONVERSION;
	// mdvalley: No SetStringValue
    long lr = keyWrite.SetValue(OLE2CT(bstrComputer), TEXT("NTEventLog"));
    ZAssert(ERROR_SUCCESS == lr);
  }

  // Create an event to be signaled when new NT event log is open
  TCHandle hevt = CreateEvent(NULL, false, false, NULL);
  ZAssert(!hevt.IsNull());

  // Open the new NT event log
  HRESULT hr = S_OK;
  PostMessage(e_ChangeNTEventLog, 3, bstrComputer, hevt, &hr);

  // Wait for the NT event log to be opened (or failure)
  WaitForSingleObject(hevt, INFINITE);
  RETURN_FAILED(hr);

  // Save the specified computer name name
  {
    XLock lock(this);
    m_bstrNTEventLog = bstrComputer;
  }

  // Get the AGC version information
  IAGCVersionInfoPtr spVersionInfo;
  ZSucceeded(spVersionInfo.CreateInstance("AGC.AGCVersionInfo"));

  // Get some product version information from the object
  WORD     wProductBuildNumber;
  CComBSTR bstrProductVersion;
  ZSucceeded(spVersionInfo->get_ProductBuildNumber(&wProductBuildNumber));
  ZSucceeded(spVersionInfo->get_ProductVersionString(&bstrProductVersion));

  // Fire an event log started event
  _AGCModule.TriggerEvent(NULL, EventID_NTEventLogStarted,
    bstrProductVersion, wProductBuildNumber, -1, -1, 0);

  // Indicate success
  return S_OK;
}

STDMETHODIMP CAGCEventLogger::get_NTEventLog(BSTR* pbstrComputer)
{
  CComBSTR bstr(m_bstrNTEventLog);
  CLEAROUT(pbstrComputer, (BSTR)bstr);
  bstr.Detach();
  return S_OK;
}

STDMETHODIMP CAGCEventLogger::put_DBEventLog(IAGCDBParams* pDBParams)
{
  USES_CONVERSION;

  // Open a writeable registry key, unless we're initializing
  CRegKey keyWrite;
  RETURN_FAILED(OpenWriteableRegKey(keyWrite));

  // Get the specified connection string and table name, if any
  CComBSTR bstrConnectionString;
  CComBSTR bstrTableName;
  if (pDBParams)
  {
    // Get the specified connection string
    RETURN_FAILED(pDBParams->get_ConnectionString(&bstrConnectionString));

    // Get the specified table name
    RETURN_FAILED(pDBParams->get_TableName(&bstrTableName));
  }

  // Attemp to establish a connection
  CDataSource ds;
  CSession session;
	DBID idTable;
	CDBPropSet propset(DBPROPSET_ROWSET);
  if (bstrConnectionString.Length() && bstrTableName.Length())
  {
    // Attempt to establish a connection to the specified data source
    RETURN_FAILED(ds.OpenFromInitializationString(bstrConnectionString));

    // Attempt to open a session on the data source
    RETURN_FAILED(session.Open(ds));

    // Setup the DBID parameter
	  idTable.eKind          = DBKIND_NAME;
	  idTable.uName.pwszName = bstrTableName;

    // Setup the property set for allowing updates to the rowset
	  propset.AddProperty(DBPROP_IRowsetChange, true);
	  propset.AddProperty(DBPROP_UPDATABILITY, DBPROPVAL_UP_INSERT);

    // Attempt to open the specified table
    CTable<CAccessor<XEvent> > table;
    RETURN_FAILED(table.Open(session, idTable, &propset));
    table.Close();
    session.Close();

    // TODO: Check the specified table for the needed columns
  }

  // Fire an 'event log stopped' event if a log is open
  FireDBEventLogStopped();

  // Save the new DB params to the registry, unless we're initializing
  if (keyWrite.m_hKey)
  {
    USES_CONVERSION;
	// mdvalley: SetStringValue
    long lr = keyWrite.SetValue(OLE2CT(bstrConnectionString), TEXT("ConnectionString"));
    ZAssert(ERROR_SUCCESS == lr);
    lr = keyWrite.SetValue(OLE2CT(bstrTableName), TEXT("TableName"));
    ZAssert(ERROR_SUCCESS == lr);
  }

  // Create an event to be signaled when new DB is open
  TCHandle hevt = CreateEvent(NULL, false, false, NULL);
  ZAssert(!hevt.IsNull());

  // Establish new connection
  HRESULT hr = S_OK;
  if (bstrConnectionString.Length() && bstrTableName.Length())
    PostMessage(e_ChangeDatabase, 5, ds.m_spInit.Detach(),
      &idTable, &propset, hevt, &hr);
  else
    PostMessage(e_CloseDatabase, 2, hevt, &hr);

  // Wait for the DB to be opened (or failure)
  WaitForSingleObject(hevt, INFINITE);
  RETURN_FAILED(hr);

  // Save the specified table name
  {
    XLock lock(this);
    m_bstrTableName = bstrTableName;
  }

  // Return now if no database was specified
  if (!bstrConnectionString.Length() || !bstrTableName.Length())
    return S_OK;

  // Get the AGC version information
  IAGCVersionInfoPtr spVersionInfo;
  ZSucceeded(spVersionInfo.CreateInstance("AGC.AGCVersionInfo"));

  // Get some product version information from the object
  WORD     wProductBuildNumber;
  CComBSTR bstrProductVersion;
  ZSucceeded(spVersionInfo->get_ProductBuildNumber(&wProductBuildNumber));
  ZSucceeded(spVersionInfo->get_ProductVersionString(&bstrProductVersion));

  // Fire an event log started event
  _AGCModule.TriggerEvent(NULL, EventID_DBEventLogStarted,
    bstrProductVersion, wProductBuildNumber, -1, -1, 0);

  // Indicate success
  return S_OK;
}

STDMETHODIMP CAGCEventLogger::get_DBEventLog(IAGCDBParams** ppDBParams)
{
  IAGCDBParamsPtr spParams;
  RETURN_FAILED(spParams.CreateInstance("AGC.DBParams"));
  CComBSTR bstrConnectionString;
  XLock lock(this);
  if (m_ds.m_spInit != NULL)
    RETURN_FAILED(m_ds.GetInitializationString(&bstrConnectionString, true))
  lock.Unlock();
  RETURN_FAILED(spParams->put_ConnectionString(bstrConnectionString));
  RETURN_FAILED(spParams->put_TableName(m_bstrTableName));
  CLEAROUT(ppDBParams, (IAGCDBParams*)spParams);
  spParams.Detach();
  return S_OK;
}

STDMETHODIMP CAGCEventLogger::put_EnabledNTEvents(IAGCEventIDRanges* pEvents)
{
  // This property can only be set on WinNT machines
  if (!IsWinNT())
    return Error(IDS_E_NTEVENTLOG_WIN9X, IID_IAGCEventLogger);

  // Open a writeable registry key, unless we're initializing
  XLock lock(this);
  CRegKey keyWrite;
  RETURN_FAILED(OpenWriteableRegKey(keyWrite));

  // Revoke the current set of event ID ranges, if any
  if (NULL != m_spRangesNT)
    GetAGCGlobal()->RevokeEventRanges(m_spRangesNT, AGC_Any_Objects,
      static_cast<IAGCEventSink*>(this));

  // Use the default enabled events, if NULL was specified
  IAGCEventIDRangesPtr spRanges;
  if (!pEvents)
  {
    RETURN_FAILED(get_EnabledNTEvents(&spRanges));
    pEvents = spRanges;
  }

  // Save the specified set of event ID ranges
  m_spRangesNT = pEvents;

  // Register the specified set of event ID ranges, if any
  CComBSTR bstrEventRanges;
  if (NULL != m_spRangesNT)
  {
    GetAGCGlobal()->RegisterEventRanges(m_spRangesNT, AGC_Any_Objects,
      static_cast<IAGCEventSink*>(this));

    // Persist the set of event ID ranges to a string
    ZSucceeded(m_spRangesNT->get_DisplayString(&bstrEventRanges));
  }

  // Save the new set of event ID ranges to the registry, unless initializing
  if (keyWrite.m_hKey)
  {
    USES_CONVERSION;
    LPCTSTR pszValue = bstrEventRanges ? OLE2CT(bstrEventRanges) : TEXT("");
	// mdvalley: SetStringValue
    long lr = keyWrite.SetValue(pszValue, TEXT("EnabledNTEvents"));
    ZAssert(ERROR_SUCCESS == lr);
  }

  // Indicate success
  return S_OK;
}

STDMETHODIMP CAGCEventLogger::get_EnabledNTEvents(IAGCEventIDRanges** ppEvents)
{
  XLock lock(this);
  CLEAROUT(ppEvents, (IAGCEventIDRanges*)m_spRangesNT);
  (*ppEvents)->AddRef();
  return S_OK;
}

STDMETHODIMP CAGCEventLogger::put_EnabledDBEvents(IAGCEventIDRanges* pEvents)
{
  // Open a writeable registry key, unless we're initializing
  XLock lock(this);
  CRegKey keyWrite;
  RETURN_FAILED(OpenWriteableRegKey(keyWrite));

  // Revoke the current set of event ID ranges, if any
  if (NULL != m_spRangesDB)
    GetAGCGlobal()->RevokeEventRanges(m_spRangesDB, AGC_Any_Objects,
      static_cast<IAGCEventSink*>(this));

  // Use the default enabled events, if NULL was specified
  IAGCEventIDRangesPtr spRanges;
  if (!pEvents)
  {
    RETURN_FAILED(get_EnabledDBEvents(&spRanges));
    pEvents = spRanges;
  }

  // Save the specified set of event ID ranges
  m_spRangesDB = pEvents;

  // Register the specified set of event ID ranges, if any
  CComBSTR bstrEventRanges;
  if (NULL != m_spRangesDB)
  {
    GetAGCGlobal()->RegisterEventRanges(m_spRangesDB, AGC_Any_Objects,
      static_cast<IAGCEventSink*>(this));

    // Persist the set of event ID ranges to a string
    ZSucceeded(m_spRangesDB->get_DisplayString(&bstrEventRanges));
  }

  // Save the new set of event ID ranges to the registry, unless initializing
  if (keyWrite.m_hKey)
  {
    USES_CONVERSION;
    LPCTSTR pszValue = bstrEventRanges ? OLE2CT(bstrEventRanges) : TEXT("");
	// mdvalley: SetStringValue
    long lr = keyWrite.SetValue(pszValue, TEXT("EnabledDBEvents"));
    ZAssert(ERROR_SUCCESS == lr);
  }

  // Indicate success
  return S_OK;
}

STDMETHODIMP CAGCEventLogger::get_EnabledDBEvents(IAGCEventIDRanges** ppEvents)
{
  XLock lock(this);
  CLEAROUT(ppEvents, (IAGCEventIDRanges*)m_spRangesDB);
  (*ppEvents)->AddRef();
  return S_OK;
}

STDMETHODIMP CAGCEventLogger::get_DefaultEnabledNTEvents(
  IAGCEventIDRanges** ppEvents)
{
  // Initialize the [out] parameter
  CLEAROUT(ppEvents, (IAGCEventIDRanges*)NULL);

  // Create an event ID ranges object
  IAGCEventIDRangesPtr spRanges;
  RETURN_FAILED(spRanges.CreateInstance("AGC.EventIDRanges"));

  // Get the available ranges
  IAGCEventIDRangesPtr spRangesHost;
  GetAGCGlobal()->GetAvailableEventIDRanges(&spRangesHost);

  // Loop through each event ID
  for (const CAGCEventDef::XEventDef* it = CAGCEventDef::begin();
    it != CAGCEventDef::end(); ++it)
  {
    // Ignore group ID's and those not enabled by default
    if (0 == it->m_nIndent && it->m_bLogAsNTEvent)
    {
      VARIANT_BOOL bInRange;
      ZSucceeded(spRangesHost->get_IntersectsWithValue(it->m_id, &bInRange));
      if (bInRange)
        ZSucceeded(spRanges->AddByValues(it->m_id, AGCEventID(it->m_id + 1)));
    }
  }

  // Detach the new object to the [out] parameter
  *ppEvents = spRanges.Detach();

  // Indicate success
  return S_OK;
}

STDMETHODIMP CAGCEventLogger::get_DefaultEnabledDBEvents(
  IAGCEventIDRanges** ppEvents)
{
  // Initialize the [out] parameter
  CLEAROUT(ppEvents, (IAGCEventIDRanges*)NULL);

  // Create an event ID ranges object
  IAGCEventIDRangesPtr spRanges;
  RETURN_FAILED(spRanges.CreateInstance("AGC.EventIDRanges"));

  // Get the available ranges
  IAGCEventIDRangesPtr spRangesHost;
  GetAGCGlobal()->GetAvailableEventIDRanges(&spRangesHost);

  // Loop through each event ID
  for (const CAGCEventDef::XEventDef* it = CAGCEventDef::begin();
    it != CAGCEventDef::end(); ++it)
  {
    // Ignore group ID's and those not enabled by default
    if (0 == it->m_nIndent && it->m_bLogAsDBEvent)
    {
      VARIANT_BOOL bInRange;
      ZSucceeded(spRangesHost->get_IntersectsWithValue(it->m_id, &bInRange));
      if (bInRange)
        ZSucceeded(spRanges->AddByValues(it->m_id, AGCEventID(it->m_id + 1)));
    }
  }

  // Detach the new object to the [out] parameter
  *ppEvents = spRanges.Detach();

  // Indicate success
  return S_OK;
}


/////////////////////////////////////////////////////////////////////////////
// IAGCEventLoggerPrivate Interface Methods

STDMETHODIMP CAGCEventLogger::Initialize(BSTR bstrSourceApp, BSTR bstrRegKey)
{
  // Validate the specified parameters
  ZAssert(BSTRLen(bstrSourceApp));
  ZAssert(BSTRLen(bstrRegKey));

  // Interpret the first part of the specified registry key string
  DWORD cchEaten;
  HKEY hkeyRoot = RootKeyFromString(bstrRegKey, &cchEaten);
  if (!hkeyRoot)
  {
    ZError("CAGCEventLogger::Initialize(): Invalid registry key name specified.");
    return E_INVALIDARG;
  }

  // Attempt to open the specified registry key for read access
  USES_CONVERSION;
  LONG lr = m_key.Open(hkeyRoot, OLE2CT(bstrRegKey + cchEaten), KEY_READ);

  // Attempt to read the NT Event Logging properties from the registry
  CComBSTR             bstrNTEventLog;
  IAGCEventIDRangesPtr spRangesNT;
  if (IsWinNT())
  {
    // Attempt to read the NTEventLog computer name
    ReadStringValueFromRegistry(TEXT("NTEventLog"), bstrNTEventLog);

    // Attempt to read the EnabledNTEvents value
    CComBSTR bstrEnabledEvents;
    HRESULT hr = ReadStringValueFromRegistry(TEXT("EnabledNTEvents"),
      bstrEnabledEvents);
    if (SUCCEEDED(hr) &&
      (!bstrEnabledEvents.Length() ||
      _wcsicmp(bstrEnabledEvents, L"default")))
    {
      // Create and populate the range object
      RETURN_FAILED(spRangesNT.CreateInstance("AGC.EventIDRanges"));
      RETURN_FAILED(spRangesNT->put_DisplayString(bstrEnabledEvents));
    }
    else
    {
      // Get the default set of enabled events
      RETURN_FAILED(get_DefaultEnabledNTEvents(&spRangesNT));
    }
  }

  // Attempt to read the DBEventLog connection string and table name
  CComBSTR             bstrConnectionString;
  CComBSTR             bstrTableName;
  IAGCEventIDRangesPtr spRangesDB;
  ReadStringValueFromRegistry(TEXT("ConnectionString"),
    bstrConnectionString);
  ReadStringValueFromRegistry(TEXT("TableName"), bstrTableName);

  // Create and populate the DBParams object
  IAGCDBParamsPtr spDBParams;
  RETURN_FAILED(spDBParams.CreateInstance("AGC.DBParams"));
  RETURN_FAILED(spDBParams->put_ConnectionString(bstrConnectionString));
  RETURN_FAILED(spDBParams->put_TableName(bstrTableName));

  // Attempt to read the EnabledDBEvents value
  CComBSTR bstrEnabledEvents;
  HRESULT hr = ReadStringValueFromRegistry(TEXT("EnabledDBEvents"),
    bstrEnabledEvents);
  if (SUCCEEDED(hr) &&
    (!bstrEnabledEvents.Length() ||
    _wcsicmp(bstrEnabledEvents, L"default")))
  {
    // Create and populate the range object
    RETURN_FAILED(spRangesDB.CreateInstance("AGC.EventIDRanges"));
    RETURN_FAILED(spRangesDB->put_DisplayString(bstrEnabledEvents));
  }
  else
  {
    // Get the default set of enabled events
    RETURN_FAILED(get_DefaultEnabledDBEvents(&spRangesDB));
  }

  // Register as an event source if logging any events to NT event log
  if (IsWinNT())
  {
    ZSucceeded(put_EnabledNTEvents(spRangesNT));
  }

  // Register as an event source if logging any events to DB event log
  ZSucceeded(put_EnabledDBEvents(spRangesDB));

  // Save the specified Event Source App name
  m_bstrSourceApp = bstrSourceApp;

  // Open the NT Event log specified in the registry
  if (IsWinNT())
  {
    RETURN_FAILED(put_NTEventLog(bstrNTEventLog));
  }

  // Open the DB Event log specified in the registry
  RETURN_FAILED(put_DBEventLog(spDBParams));

  // Indicate success
  m_bInitializing = false;
  return S_OK;
}

STDMETHODIMP CAGCEventLogger::Terminate()
{
  // Create an event to be signaled when event log is closed
  TCHandle hevt = CreateEvent(NULL, false, false, NULL);
  ZAssert(!hevt.IsNull());

  // Fire 'event log stopped' event if log is open
  FireDBEventLogStopped();

  // Close the current NT event log
  HRESULT hr = S_OK;
  PostMessage(e_CloseDatabase, 2, hevt, &hr);

  // Wait for the DB event log to be closed (or failure)
  WaitForSingleObject(hevt, INFINITE);
  RETURN_FAILED(hr);

  // Fire 'event log stopped' event if log is open
  FireNTEventLogStopped();

  // Close the current NT event log
  PostMessage(e_CloseNTEventLog, 2, hevt, &hr);

  // Wait for the NT event log to be closed (or failure)
  WaitForSingleObject(hevt, INFINITE);
  RETURN_FAILED(hr);

  // Keep a reference on ourself for the duration of this method
  IUnknownPtr spUnk(GetUnknown());

  // Revoke all events for which we are registered
  {
    XLock lock(this);
    ZSucceeded(GetAGCGlobal()->RevokeAllEvents(
      static_cast<IAGCEventSink*>(this)));
  }

  // Close the worker thread
  TCWorkerThread::Close();

  // Indicate success
  return S_OK;
}

STDMETHODIMP CAGCEventLogger::put_LoggingToNTEnabled(VARIANT_BOOL bEnabled)
{
  XLock lock(this);
  m_bLoggingToNTEnabled = !!bEnabled;
  return S_OK;
}

STDMETHODIMP CAGCEventLogger::get_LoggingToNTEnabled(VARIANT_BOOL* pbEnabled)
{
  XLock lock(this);
  CLEAROUT(pbEnabled, VARBOOL(m_bLoggingToNTEnabled));
  return S_OK;
}

STDMETHODIMP CAGCEventLogger::put_LoggingToDBEnabled(VARIANT_BOOL bEnabled)
{
  XLock lock(this);
  m_bLoggingToDBEnabled = !!bEnabled;
  return S_OK;
}

STDMETHODIMP CAGCEventLogger::get_LoggingToDBEnabled(VARIANT_BOOL* pbEnabled)
{
  XLock lock(this);
  CLEAROUT(pbEnabled, VARBOOL(m_bLoggingToDBEnabled));
  return S_OK;
}

STDMETHODIMP CAGCEventLogger::put_HookForNTLogging(IAGCEventLoggerHook* pHook)
{
  XLock lock(this);
  m_spHookNT = pHook;
  return S_OK;
}

STDMETHODIMP CAGCEventLogger::get_HookForNTLogging(IAGCEventLoggerHook** ppHook)
{
  XLock lock(this);
  CLEAROUT(ppHook, (IAGCEventLoggerHook*)m_spHookNT);
  return S_OK;
}

STDMETHODIMP CAGCEventLogger::put_HookForDBLogging(IAGCEventLoggerHook* pHook)
{
  XLock lock(this);
  m_spHookDB = pHook;
  return S_OK;
}

STDMETHODIMP CAGCEventLogger::get_HookForDBLogging(IAGCEventLoggerHook** ppHook)
{
  XLock lock(this);
  CLEAROUT(ppHook, (IAGCEventLoggerHook*)m_spHookDB);
  return S_OK;
}

