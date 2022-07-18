/*-------------------------------------------------------------------------
  allegdb.cpp
  
  Implementation of OLE DB layer for Allegiance
  
  Owner: 
  
  Copyright 1986-2000 Microsoft Corporation, All Rights Reserved
 *-----------------------------------------------------------------------*/

#include "pch.h"
#include "allegdb.h"

unsigned CALLBACK SQLQueueProc(void * pvSQLQueueThread)
{
  CSQLQueueThread * pSQLQueueThread = (CSQLQueueThread *) pvSQLQueueThread;
  HANDLE rgeventHandles[1];
  rgeventHandles[0] = pSQLQueueThread->GetEventDie();
  DWORD cHandles = sizeof(rgeventHandles) / sizeof(rgeventHandles[0]);
  DWORD dwWait = WAIT_TIMEOUT;

  do
  {
    //Wait until either we get a message or we ran out of time
    dwWait = MsgWaitForMultipleObjects(cHandles, rgeventHandles, FALSE, INFINITE, QS_ALLINPUT);

    // Process the message queue, if any messages were received
    static MSG msg;
    while (WAIT_OBJECT_0 != dwWait && PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
      // dispatch Windows Messages to allow for the admin tool's COM to work
      TranslateMessage(&msg);
      switch (msg.message)
      {
        case wm_sql_addquery:
        {
          pSQLQueueThread->AddQuery((CSQLQuery*) msg.lParam);
          break;
        }
        
        case WM_QUIT:
          dwWait = WAIT_OBJECT_0; // let the thread be shutdown by sending a quit, or signalling pSqlThread->m_hEventExit
          break;

        default:
          ZError("SQLThreadProc: Unexpected thread message.\n");
      }
    }
  } while (WAIT_OBJECT_0 != dwWait); 
  
  return 0;
}


unsigned CALLBACK SQLThreadProc(void * pvsqlThread)
{
  CSQLThread * pSqlThread = (CSQLThread *) pvsqlThread;
  CSQLCore * psql = pSqlThread->GetSQLCore();
  HRESULT hr = E_FAIL;

  hr = pSqlThread->Open();
  if (FAILED(hr))
    return hr;
  
  HANDLE rgeventHandles[2];
  rgeventHandles[0] = pSqlThread->GetEventDie();
  rgeventHandles[1] = pSqlThread->GetNotify() ? psql->GetNotifySemaphore() : psql->GetSilentSemaphore();
  DWORD cHandles = sizeof(rgeventHandles) / sizeof(rgeventHandles[0]);
  DWORD dwWait = WAIT_TIMEOUT;

  do
  {
    //Wait until either we get a message or we ran out of time
    dwWait = WaitForMultipleObjects(cHandles, rgeventHandles, FALSE, INFINITE);

    switch (dwWait)
    {
      case WAIT_OBJECT_0: // pSqlThread->m_hEventExit
        break;

      case WAIT_OBJECT_0 + 1: // spemaphore for the queue we're servicing
        // We may not actually get the query that we were signaled for, but that doesn't matter
        hr = pSqlThread->ServiceQuery(pSqlThread->GetNotify() ? psql->GetNotifyQuery() : psql->GetSilentQuery());
        break;

      default:
        ZError("Unexpected object signaled in SQLThreadProc.\n");
    }

  } while (WAIT_OBJECT_0 != dwWait);

  return 0;
}


HRESULT CSQLCore::Init(LPCOLESTR strSQLConfig, DWORD nThreadIDNotify, DWORD cSilentThreads, 
                       DWORD cNotifyThreads)

{
  HRESULT hr = E_FAIL;
  if (cSilentThreads + cNotifyThreads < 1 || 
      cSilentThreads + cNotifyThreads > 20) // must have between 1 and 20 threads
    return E_INVALIDARG;

  m_nThreadIDNotify = nThreadIDNotify;
  m_cSilentThreads = cSilentThreads;
  m_cNotifyThreads = cNotifyThreads;

  // create event used to signal that ALL sql threads should exit
  m_hKillSQLEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
  ZAssert(m_hKillSQLEvent);

  hr = m_connection.OpenFromInitializationString(strSQLConfig);
  if (FAILED(hr))
  {
    DumpErrorInfo(m_connection.m_spInit, IID_IDBInitialize, NULL);
    return hr;
  }

  m_hQueryNotify = CreateSemaphore(NULL, 0, MAXLONG, NULL);
  m_hQuerySilent = CreateSemaphore(NULL, 0, MAXLONG, NULL);

  m_pthdQueue = new CSQLQueueThread(m_hKillSQLEvent, this);
  ZAssert (m_pthdQueue);

  m_pargSilentThreads = (CSQLThread **) new char[sizeof(CSQLThread *) * m_cSilentThreads];
  m_pargNotifyThreads = (CSQLThread **) new char[sizeof(CSQLThread *) * m_cNotifyThreads];

  DWORD i = 0;
  for (i = 0; i < m_cNotifyThreads; i++)
  {
    m_pargNotifyThreads[i] = new CSQLThread(m_hKillSQLEvent, true, this);
  }

  for (i = 0; i < m_cSilentThreads; i++)
  {
    m_pargSilentThreads[i] = new CSQLThread(m_hKillSQLEvent, false, this);
  }

  return hr;
}


CSQLCore::~CSQLCore()
{
  DWORD i = 0;
  for (i = 0; i < m_cNotifyThreads && m_pargNotifyThreads; i++)
  {
    // TODO: cleanup m_listQuries
    delete m_pargNotifyThreads[i];
  }
  delete [] m_pargNotifyThreads;

  for (i = 0; i < m_cSilentThreads && m_pargSilentThreads; i++)
  {
    // TODO: cleanup m_listQuries
    delete m_pargSilentThreads[i];
  }
  delete [] m_pargSilentThreads;

  CloseHandle(m_hQueryNotify);
  CloseHandle(m_hQuerySilent);
}


void CSQLCore::AddQuery(CSQLQuery * pquery)
{
  HANDLE h = NULL;
  if (pquery->GetNotify())
  {
    m_listQueriesNotify.last(pquery);
    h = GetNotifySemaphore();
  }
  else 
  {
    m_listQueriesSilent.last(pquery);
    h = GetSilentSemaphore();
  }
  ReleaseSemaphore(h, 1, NULL); // signal some thread to pick up the query
}


void CSQLCore::PostQuery(CSQLQuery * pquery)
{
  PostThreadMessage(m_pthdQueue->GetThreadID(), wm_sql_addquery, (WPARAM) NULL, (LPARAM) pquery);
}


HRESULT CSQLThread::ServiceQuery(CSQLQuery * pqueryNew)
{
  CSQLQuery * pqueryCache = NULL;
  REFGUID guid = pqueryNew->GetGuid();
  HRESULT hrQuery = E_FAIL; // it is important that ONLY results from calls to pqueryCache be assigned here

  // See if this query is already in the per-thread cache
  for (SLinkQueries * plinkQuery = m_listQueries.first(); plinkQuery && !pqueryCache; plinkQuery = plinkQuery->next())
  {
    CSQLQuery * pqueryT = plinkQuery->data();
    if (pqueryT->GetGuid() == guid)
    {
      // make sure we didn't accidentally use the same guid twice for different queries
      ZAssert(!lstrcmp(pqueryNew->GetStrQuery(), pqueryT->GetStrQuery()));
      pqueryCache = pqueryT;
    }
  }

  if (!pqueryCache) // the first time this thread has seen this query, so let's add it to our reportoire
  {
    //pqueryCache = pqueryNew;
    pqueryCache = pqueryNew->Copy(NULL);
    m_listQueries.first(pqueryCache);
    hrQuery = pqueryCache->OnPrepare(m_session);
  }
  else
  {
    hrQuery = S_OK;
    pqueryNew->Copy(pqueryCache);
  }

  bool fRetry = SUCCEEDED(hrQuery);
  int cRetries = 0;
  while (fRetry)
  {
    hrQuery = pqueryCache->OnExecute();
    if (SUCCEEDED(hrQuery))
    {
      fRetry = false;
    }
    else
    {
      m_psql->DumpErrorInfo(pqueryCache->GetIUnknown(), IID_ICommand, &fRetry);
      if (fRetry)
      {
        debugf("Query deadlocked or timed-out. Retry #%d\n", ++cRetries);
        Sleep(50 * cRetries);
      }
    }
  }

  if (FAILED(hrQuery))
    m_psql->DumpErrorInfo(pqueryCache->GetIUnknown(), IID_ICommand, NULL);

  pqueryCache->Copy(pqueryNew);
  // We post it whether the client requested notification or not, because at least we need to let the query clean up itself
  if (pqueryNew->GetCallbackOnMainThread())
  {
    BOOL bPosted = PostThreadMessage(GetSQLCore()->GetNotifyThreadID(), wm_sql_querydone, (WPARAM) NULL, (LPARAM) pqueryNew);
    if (!bPosted)
    {
      // If the PostThreadMessage failed because the thread id is gone, we still need to clean up
      DWORD dwLastError = ::GetLastError();
      if (ERROR_INVALID_THREAD_ID == dwLastError)
      {
        pqueryNew->DataReady();
      }
    }
  }
  else
  {
    pqueryNew->DataReady();
  }

  return hrQuery;
}


