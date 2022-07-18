#ifndef __AGCEventThread_h__
#define __AGCEventThread_h__

/////////////////////////////////////////////////////////////////////////////
// AGCEventThread : Declaration of the CAGCEventThread class.
//

#include <AGC.h>
#include <..\TCLib\AutoHandle.h>
#include <..\TCLib\WorkerThread.h>
#include "AGCGlobal.h"
#include "AGCEventData.h"


/////////////////////////////////////////////////////////////////////////////
//
class CAGCEventThread :
  public TCWorkerThread
{
// Construction
private:
  CAGCEventThread(const CAGCEventThread&); // Disable copy constructor
public:
  CAGCEventThread(CAGCGlobal* pGlobal);

// Operations
public:
  void QueueEvent(HAGCLISTENERS hListeners, CAGCEventData& data);
  void QueueEventSynchronous(HAGCLISTENERS hListeners, CAGCEventData& data);

// Overrides
protected:
  virtual IUnknown* OnGetUnknown();
  virtual TC_WorkItemRelProc OnGetWorkItemRelProc();
  virtual void OnMessage(UINT idMsg, int cParams, LPARAM* rgParams);

// Implementation
protected:
  static void WINAPI ArgumentReleaseProc(UINT idMsg, int cParams,
    LPARAM* rgParams);

// Types
protected:
  // Event Registration
  typedef CAGCGlobal::XEventSinks   XEventSinks;
  typedef CAGCGlobal::XEventSinksIt XEventSinksIt;
  // Worker Thread Identifiers
  enum {e_TriggerEvent};

// Data Members
protected:
  CAGCGlobal* m_pGlobal;
};


/////////////////////////////////////////////////////////////////////////////
// Construction

inline CAGCEventThread::CAGCEventThread(CAGCGlobal* pGlobal) :
  m_pGlobal(pGlobal)
{
}


/////////////////////////////////////////////////////////////////////////////
// Operations

inline void CAGCEventThread::QueueEvent(HAGCLISTENERS hListeners,
  CAGCEventData& data)
{
  // Detach the data and post it to the thread
  UINT cbData;
  BYTE* pbData = data.Detach(&cbData);
  PostMessage(e_TriggerEvent, 4, hListeners, cbData, pbData, NULL);
}

inline void CAGCEventThread::QueueEventSynchronous(HAGCLISTENERS hListeners,
  CAGCEventData& data)
{
  TCHandle shEvent = ::CreateEvent(NULL, false, false, NULL);

  // Detach the data and post it to the thread
  UINT cbData;
  BYTE* pbData = data.Detach(&cbData);
  PostMessage(e_TriggerEvent, 4, hListeners, cbData, pbData, shEvent.GetHandle());

  WaitForSingleObject(shEvent, INFINITE);  
}


/////////////////////////////////////////////////////////////////////////////
// Overrides

inline IUnknown* CAGCEventThread::OnGetUnknown()
{
  return m_pGlobal->GetUnknown();
}

inline TC_WorkItemRelProc CAGCEventThread::OnGetWorkItemRelProc()
{
  return ArgumentReleaseProc;
}

inline void CAGCEventThread::OnMessage(UINT idMsg, int cParams,
  LPARAM* rgParams)
{
  switch (idMsg)
  {
    case e_TriggerEvent:
    {
      ZAssert(4 <= cParams);
      XEventSinks* pSinks = reinterpret_cast<XEventSinks*>(rgParams[0]);
      UINT         cbData =      static_cast<UINT        >(rgParams[1]);
      BYTE*        pbData = reinterpret_cast<BYTE*       >(rgParams[2]);
      HANDLE       hevt   = reinterpret_cast<HANDLE      >(rgParams[3]);

      // Attach the parameters to a CAGCEventData object
      CAGCEventData data(cbData, pbData);

      // Create an event object
      CComObject<CAGCEvent>* pEvent = NULL;
      ZSucceeded(pEvent->CreateInstance(&pEvent));
      IAGCEventPtr spEvent(pEvent);

      // Initialize the AGCEvent object from the CAGCEventData object
      ZSucceeded(pEvent->Init(data));

      // Notify each registered event sink
      for (XEventSinksIt it = pSinks->begin(); it != pSinks->end(); ++it)
      {
        IAGCEventSinkPtr spSink;
        if (m_pGlobal->GetEventSinksAreGITCookies())
        {
          ZSucceeded(m_pGlobal->GetInterfaceFromGlobal(
            reinterpret_cast<DWORD>(*it), IID_IAGCEventSink, (void**)&spSink));
        }
        else
        {
          spSink = *it;
        }
        if (hevt)
        {
          IAGCEventSinkSynchronousPtr spSinkSync(spSink);
          if (spSinkSync != NULL)
          {
            spSinkSync->OnEventTriggeredSynchronous(spEvent);
            continue;
          }
        }
        spSink->OnEventTriggered(spEvent);
      }

      // Detach the parameters from the CAGCEventData object
      //   (since they'll be properly destructed in ArgumentReleaseProc)
      data.Detach();
      return;
    }
  }
}


/////////////////////////////////////////////////////////////////////////////
// Implementation

inline void WINAPI CAGCEventThread::ArgumentReleaseProc(UINT idMsg,
  int cParams, LPARAM* rgParams)
{
  switch (idMsg)
  {
    case e_TriggerEvent:
    {
      ZAssert(4 <= cParams);
      XEventSinks* pSinks = reinterpret_cast<XEventSinks*>(rgParams[0]);
      UINT         cbData =      static_cast<UINT        >(rgParams[1]);
      BYTE*        pbData = reinterpret_cast<BYTE*       >(rgParams[2]);
      HANDLE       hevt   = reinterpret_cast<HANDLE      >(rgParams[3]);

      // Free the listeners array
      CAGCGlobal::FreeListenersImpl(reinterpret_cast<HAGCLISTENERS>(pSinks));

      // Attach the parameters to a CAGCEventData object
      //   (deleted when CAGCEventData is destructed)
      CAGCEventData(cbData, pbData);

      // Signal the event, if synchronous
      if (hevt)
        ::SetEvent(hevt);
      return;
    }
  }
}



/////////////////////////////////////////////////////////////////////////////

#endif // !__AGCEventThread_h__
