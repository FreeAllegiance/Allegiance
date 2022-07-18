/////////////////////////////////////////////////////////////////////////////
// AdminEventLoggerHook.cpp : Implementation of the CAdminEventLoggerHook
// class.

#include "pch.h"
#include "AdminEventLoggerHook.h"

//#if !defined(ALLSRV_STANDALONE) Imago enabled 6/10

/////////////////////////////////////////////////////////////////////////////
// CAdminEventLoggerHook


/////////////////////////////////////////////////////////////////////////////
// Implementation


/////////////////////////////////////////////////////////////////////////////
// Purpose: Callback from completion of a synchronous CQLogEvent query.
//
void CAdminEventLoggerHook::EventLogged(CQLogEvent* pquery)
{
  // Signal the event
  CQLogEventData* pqd = pquery->GetData();
  ZAssert(pqd->hevt);
  ::SetEvent(pqd->hevt);
}


/////////////////////////////////////////////////////////////////////////////
// IAGCEventLoggerHook Interface Methods

STDMETHODIMP CAdminEventLoggerHook::LogEvent(IAGCEvent* pEvent, VARIANT_BOOL bSynchronous)
{
	// BT - 9/17 - No longer used.
  // Do nothing if SQLCore has not been initialized
  //if (!g.sql.GetNotifyThreadID())
    return S_OK;

  // Get the fields of the specified event object
  AGCEventID idEvent;
  long       idSubject;
  CComBSTR   bstrComputerName, bstrSubjectName, bstrContext;
  ZSucceeded(pEvent->get_ID(&idEvent));
  ZSucceeded(pEvent->get_SubjectID   (&idSubject       ));
  ZSucceeded(pEvent->get_ComputerName(&bstrComputerName));
  ZSucceeded(pEvent->get_SubjectName (&bstrSubjectName ));
  ZSucceeded(pEvent->get_Context     (&bstrContext     ));

  // Use a callback if event is synchronous
  void (*pfnDataReady)(CQLogEvent* pquery) = bSynchronous ? EventLogged : NULL;

  // Create the database update query
  CQLogEvent*  pquery = new CQLogEvent(pfnDataReady);
  CQLogEventData* pqd = pquery->GetData();

  // Ensure that the callback does NOT go through the message queue
  pquery->SetCallbackOnMainThread(false);

  // Populate the query parameters from the event fields
  USES_CONVERSION;
	pqd->nEvent   = idEvent;
	pqd->nSubject = idSubject;
  if (bstrComputerName.Length())
    lstrcpyn(pqd->szComputerName, OLE2CT(bstrComputerName), sizeofArray(pqd->szComputerName));
  else
    pqd->szComputerName[0] = TEXT('\0');
  if (bstrSubjectName.Length())
    lstrcpyn(pqd->szSubjectName, OLE2CT(bstrSubjectName), sizeofArray(pqd->szSubjectName));
  else
    pqd->szSubjectName[0] = TEXT('\0');
  if (bstrContext.Length())
    lstrcpyn(pqd->szContext, OLE2CT(bstrContext), sizeofArray(pqd->szContext));
  else
    pqd->szContext[0] = TEXT('\0');

  // Get the event object as a string
  ZAssert(IPersistStreamInitPtr(pEvent) != NULL || IPersistStreamPtr(pEvent) != NULL);
  ZAssert(IMarshalPtr(pEvent) != NULL);
  CComBSTR bstrTemp;
  ZSucceeded(pEvent->SaveToString(&bstrTemp));
  WideCharToMultiByte(CP_ACP, 0, bstrTemp, -1,
    pqd->szObjectRef, sizeof(pqd->szObjectRef), 0, 0);

  // Create an event upon which to wait, if event is synchronous
  TCHandle shevt;
  if (bSynchronous)
    shevt = ::CreateEvent(NULL, false, false, NULL);
  pqd->hevt = shevt.GetHandle();

  // BT - 9/17 - No longer used.
  // Post the query for async completion
  //g.sql.PostQuery(pquery);

  // Wait for the event, if event is synchronous
  if (!shevt.IsNull())
    ::WaitForSingleObject(shevt, INFINITE);

  // Indicate that we handled it
  return S_OK;
}

//#endif // !defined(ALLSRV_STANDALONE) Imago 6/10
