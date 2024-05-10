/////////////////////////////////////////////////////////////////////////////
// WorkerThread.cpp | Implementation of the TCWorkerThread class.
//

#include "pch.h"
#include "..\Inc\TCLib.h"
#include "WorkerThread.h"
#include "TCThread.h"


/////////////////////////////////////////////////////////////////////////////
// 
// #define XWorkItem_TRACE

#if defined(XWorkItem_TRACE)
  #include <typeinfo.h>
  #include <TlsValue.h>
  #include "AsyncDebugOutput.h"

  static TCAsyncDebugOutput* GetDebugOutput()
  {
    static TlsValue<TCAsyncDebugOutput*> s_pDebug;
    if (!s_pDebug.GetValue())
      s_pDebug = new TCAsyncDebugOutput(TEXT("%temp%"), TEXT("TCWorkerThread"));
    return s_pDebug;    
  }
#endif // defined(XWorkItem_TRACE)


/////////////////////////////////////////////////////////////////////////////
// TCWorkerThread


/////////////////////////////////////////////////////////////////////////////
// Group=Types

/////////////////////////////////////////////////////////////////////////////
// Description: Nested class that manages a variable-length array of
// work item.
//
// Used by TCWorkerThread to manage a variable-length array of LPARAM
// arguments associated with an element of work in the worker thread's
// queue.
//
// This class represents a single element of work in the worker thread's
// queue. As such, it manages an identifier to indicate (to the
// TCWorkerThread-derived class) what type of work element is represented.
// Also, any data items associated with the element of work are managed as
// a variable-length array of LPARAM arguments. Since only the derived
// class understands the contents of the arguments, it is responsible for
// releasing them when the element of work is complete (or has failed).
// A callback function pointer is stored, which is called to perform any
// necessary cleanup of the arguments. Finally, if the derived class is a
// COM object, a reference to it will be held so that the object can not be
// released until the element of work completes (or fails).
//
// See Also: TCWorkerThread, TCWorkerThread::PostMessage,
// TCWorkerThread::PostMessageV, TCWorkerThread::PostMessageEx,
//
class TCWorkerThread::XWorkItem
{
// Group=Construction / Destruction
public:
  ///////////////////////////////////////////////////////////////////////////
  // Simply initializes the data members from the specified parameters.
  //
  // Parameters:
  //   punkOwner - The IUnknown of the TCWorkerThread-derived class. This
  // may be NULL if the derived-class is not a COM object. By copying this to
  // the m_punkOwner data member, AddRef is implicitly called since the data
  // member is a smart pointer.
  //   pfnRelease - A callback function pointer of type TC_WorkItemRelProc,
  // called to release any resources associated with the arguments.
  //   idMsg - An identifier, meaningful only in the context of the derive
  // class, used to identify the type of a queued element of work.
  //   cParams - The number of LPARAM arguments pointed to by the
  // /rgParams/ parameter.
  //   rgParams - An array of LPARAM arguments specified when the element of
  // work was queued to the worker thread. These arguments are only
  // meaninful in the context of the derived class.
  //
  // See Also: TCWorkerThread::XWorkItem::destructor, TCWorkerThread,
  // TCWorkerThread::PostMessage, TCWorkerThread::PostMessageV,
  // TCWorkerThread::PostMessageEx, TCWorkerThread_ArgumentReleaseProc,
  // TC_WorkItemRelProc
  XWorkItem(IUnknown* punkOwner, TC_WorkItemRelProc pfnRelease, UINT idMsg,
    int cParams, LPARAM* rgParams) :
    m_punkOwner(punkOwner),
    m_pfnRelease(pfnRelease),
    m_idMsg(idMsg),
    m_vec(rgParams, rgParams + cParams)
  {
    #if defined(XWorkItem_TRACE)
      char szParams[_MAX_PATH] = "";
      for (int i = 0; i < cParams; ++i)
      {
        char szParam[16];
        sprintf(szParam, "%08X ", rgParams[i]);
        strcat(szParams, szParam);
      }
      GetDebugOutput()->Write(
        "XWorkItem\t%08X\tconstructor: typeid(*punkOwner)=%hs, idMsg=%u, params=%hs\n",
        this, typeid(*punkOwner).name(), idMsg, szParams);
    #endif // defined(XWorkItem_TRACE)
  }
  ///////////////////////////////////////////////////////////////////////////
  // If a callback function was specified for releasing the arguments, it is
  // called when the object is destroyed.
  //
  // If an IUnknown* of the derived-class was specified, an implicit Release
  // is performed.
  //
  // See Also: TCWorkerThread::XWorkItem::constructor, TCWorkerThread,
  // TCWorkerThread::PostMessage, TCWorkerThread::PostMessageV,
  // TCWorkerThread::PostMessageEx, TCWorkerThread_ArgumentReleaseProc,
  // TC_WorkItemRelProc
  XWorkItem::~XWorkItem()
  {
    #if defined(XWorkItem_TRACE)
      GetDebugOutput()->WriteLen(70,
        "XWorkItem\t%08X\tdestructor: m_pfnRelease=%08X\n",
        this, m_pfnRelease);
    #endif // defined(XWorkItem_TRACE)

    if (m_pfnRelease)
	    (*m_pfnRelease)(m_idMsg, m_vec.size(), &(*m_vec.begin()));
  }

// Group=Data Members
public:
  // IUnknown* of the owner object, if it's a COM object
  IUnknownPtr         m_punkOwner;
  // Callback function used to release the arguments.
  TC_WorkItemRelProc  m_pfnRelease;
  // Identifies the type of a queued element of work in the worker thread.
  UINT                m_idMsg;
  // A variable-length array of the arguments associated with a queued
  // element of work.
  std::vector<LPARAM> m_vec;
};


/////////////////////////////////////////////////////////////////////////////
// Construction / Destruction

/////////////////////////////////////////////////////////////////////////////
// Increments the reference count of the worker thread. If this is
// the first instance, the worker thread is created.
//
// The copy constructor is declared private to disallow copying of objects of
// this type. It has only a declaration and no implementation.
//
// See Also: TCWorkerThread::m_nRefs, TCWorkerThread::m_pth
TCWorkerThread::TCWorkerThread() :
  m_bClosed(false),
  m_pth(NULL)
{
  // Create the event for signaling when items are availabe on the queue
  m_shevtQueueNotEmpty = CreateEvent(NULL, true, false, NULL);
  ZVerify(!m_shevtQueueNotEmpty.IsNull());

  // Create the event for signaling when the thread should shutdown
  m_shevtShutdown = CreateEvent(NULL, true, false, NULL);
  ZVerify(!m_shevtShutdown.IsNull());

  // Create the thread object and save its pointer
  int nPriority = THREAD_PRIORITY_NORMAL;
  TCThread* pth = TCThread::BeginThread(ThreadThunk, this, nPriority, 8192);
  InterlockedExchange((long*)&m_pth, (long)pth);
}

/////////////////////////////////////////////////////////////////////////////
// Calls Close to shutdown the worker thread.
//
// See Also: TCWorkerThread::constructor, TCWorkerThread::Close
TCWorkerThread::~TCWorkerThread()
{
  Close();
}


/////////////////////////////////////////////////////////////////////////////
// Group=Operations

/////////////////////////////////////////////////////////////////////////////
// Description: Shuts down the worker thread.
//
// Posts a WM_QUIT message to the thread and the thread is waited upon to exit.
//
// Note: This method is used internally, as noted above, by the destructor.
// It is made public to allow an advanced usage of the class. Most often,
// this method will *not* need to be called directly.
//
// TODO: The PostMessage methods should also check the m_bClosed flag prior
// to posting a message, but currently they don't. Not an issue until
// someone pokes an eye out calling PostMessage *after* calling close.
//
// See Also: TCWorkerThread::destructor, TCWorkerThread::m_bClosed,
// TCWorkerThread::m_pth
void TCWorkerThread::Close()
{
  if (!m_bClosed)
  {
    // Indicate that we have closed
    InterlockedExchange((long*)&m_bClosed, (long)true);

    // Get the thread handle and ID
    assert(m_pth);
    HANDLE hth     = m_pth->m_hThread;
    DWORD idThread = m_pth->m_nThreadID;

    // Signal the thread to exit
    SetEvent(m_shevtShutdown);

    // Wait for the thread to exit
    if (GetCurrentThreadId() != idThread)
      WaitForSingleObject(hth, INFINITE);
  }
}

/////////////////////////////////////////////////////////////////////////////
// Description: Posts an element of work to the worker thread's queue.
//
// Parameters:
//   idMsg - An identifier, meaningful only in the context of the derived
// class, used to identify the type of a queued element of work.
//   cParams - The number of LPARAM arguments pointed to by the either the
// /rgParams/ parameter, the /argptr/ parameter, or the variable argument
// list.
//   argptr - A variable argument list specifying the LPARAM arguments
// associated with the element of work. The number of parameters passed in
// must match the /cParams/ parameter.
//   rgParams - An array of LPARAM arguments specifying the LPARAM arguments
// associated with the element of work. This pointer must be valid for the
// number of LPARAM's specified by the /cParams/ parameter. These arguments
// are only meaninful in the context of the derived class.
//
// Remarks:
// These methods are used to post an element of work to the worker thread's
// message queue. An element of work consists of an identifier,
// /idMsg/, meaningful only in the context of the derived class, used to
// identify what type of work is represented by the message. Also, a variable
// number of LPARAM arguments can be associated with the element of work.
// Again, these arguments are only meaningful in the context of the derived
// class.
//
// Note: Rather than using function overloading and using the same name for
// all three methods, these *must* be named differently since the argument
// lists would be ambiguous.
//
// TODO: Create another set of these methods that take a timeout value as a
// parameter. This would allow an element of work to be delayed for a
// specified amount of time (in milliseconds). This could be implemented
// using the Win32 SetTimer/KillTimer API's with a static timer proc. The
// ThreadProc could capture the WM_TIMER message and map the timer ID to the
// XWorkItem instance. The functions should probably be prototyped as
// follows:
//
//      void PostTimedMessage(UINT uElapse, UINT idMsg, int cParams, ...);
//      void PostTimedMessageV(UINT uElapse, UINT idMsg, int cParams,
//        va_list argptr);
//      void PostTimedMessageEx(UINT uElapse, UINT idMsg, int cParams,
//        LPARAM* rgParams);
//
// TODO: Another useful feature would be to specify that an element of work
// is only to be processed if another element with the same message ID (and 
// owner instance) has not already been posted. This 'last of type' concept
// would be especially useful when combined with the timeout method. The
// implementation (in ThreadProc) could simply check a std::map for the
// owner/ID pair and, if found, ignore any XWorkItem instances other than
// the one mapped to the owner/ID pair. This would imply that such Post
// methods would add the most recent work element to the map. Possible
// prototypes for these might be as follows, where a /uElapse/ of zero would:
// indicate that the work element is not to be delayed, as in the original
// methods:
//
//      void PostLastOfMessage(UINT uElapse, UINT idMsg, int cParams, ...);
//      void PostLastOfMessageV(UINT uElapse, UINT idMsg, int cParams,
//        va_list argptr);
//      void PostLastOfMessageEx(UINT uElapse, UINT idMsg, int cParams,
//        LPARAM* rgParams);
//
// See Also: TCWorkerThread::XWorkItem, TCWorkerThread::ThreadProc
void TCWorkerThread::PostMessage(UINT idMsg, int cParams, ...)
{
  va_list argptr;
  va_start(argptr, cParams);
  PostMessageV(idMsg, cParams, argptr);
  va_end(argptr);
}

/////////////////////////////////////////////////////////////////////////////
// {partof:PostMessage}
void TCWorkerThread::PostMessageV(UINT idMsg, int cParams, va_list argptr)
{
  LPARAM* pParams = NULL;
  if (cParams)
    pParams = (LPARAM*)_alloca(cParams * sizeof(LPARAM));
  for (int i = 0; i < cParams; ++i)
    pParams[i] = va_arg(argptr, LPARAM);
  PostMessageEx(idMsg, cParams, pParams);
}

/////////////////////////////////////////////////////////////////////////////
// {partof:PostMessage}
void TCWorkerThread::PostMessageEx(UINT idMsg, int cParams, LPARAM* rgParams)
{
  IUnknown* punk = OnGetUnknown();
  TC_WorkItemRelProc pfnRelease = OnGetWorkItemRelProc();
  XWorkItem* pItem = new XWorkItem(punk, pfnRelease, idMsg, cParams, rgParams);
  assert(pItem);

  #if defined(XWorkItem_TRACE)
    GetDebugOutput()->WriteLen(60,
      "XWorkItem\t%08X\tTCWorkerThread::PostMessageEx\n", pItem);
  #endif // defined(XWorkItem_TRACE)

  // Put the work item onto the queue
  {
    XLockQueue lock(&m_csQueue);
    m_queue.push(pItem);
  }
  SetEvent(m_shevtQueueNotEmpty);
}


/////////////////////////////////////////////////////////////////////////////
// Group=Overrides
#ifdef _DOCJET_ONLY
  ///////////////////////////////////////////////////////////////////////////
  // Description: Pure-virtual override to specify the derived-class's
  // IUnknown if it's a COM object.
  //
  // If the derived class is a COM object, its override of this method should
  // specify an IUnknown pointer on itself. This interface pointer is
  // AddRef'd and stored when a XWorkItem instance is created, and
  // Release'd when the instance is destroyed. This ensures that the derived
  // class instance is not released while elements of its work remain on the
  // queue.
  //
  // See Also: TCWorkerThread::ThreadProc, TCWorkerThread::XWorkItem,
  // TCWorkerThread::XWorkItem::m_punkOwner,
  // TCWorkerThread::PostMessage, TCWorkerThread::PostMessageV,
  // TCWorkerThread::PostMessageEx
  IUnknown* TCWorkerThread::OnGetUnknown();

  ///////////////////////////////////////////////////////////////////////////
  // Description: Pure-virtual override to specify a callback function used
  // to destroy the contents of a work element's arguments.
  //
  // The derived class defines the types of work that it will perform from
  // the worker thread. Because of this, it must provide the address
  // of a callback function that will release any resources associated with
  // the arguments of a queued element of work. When an element of work is
  // queued to the worker thread, this override is called to get the
  // callback function used to release the arguments.
  //
  // Note: Since the instance of the derived class that posted the element of
  // work may be destroyed before the work is performed, it was observed that
  // this function could *not* be virtual, but instead should be a
  // *static* class method. This was so that the callback function would
  // still be valid if the object were destroyed. If a virtual method were
  // used, accessing the the virtual table pointer would cause an exceptions.
  //
  // Return Value: The address of a callback function used to release the
  // arguments of a queued element of work. See
  // TCWorkerThread_ArgumentReleaseProc for the prototype of this function.
  //
  // See Also: TCWorkerThread_ArgumentReleaseProc, TC_WorkItemRelProc,
  // TCWorkerThread::ThreadProc, TCWorkerThread::XWorkItem,
  // TCWorkerThread::XWorkItem::m_pfnRelease,
  // TCWorkerThread::PostMessage, TCWorkerThread::PostMessageV,
  // TCWorkerThread::PostMessageEx
  TC_WorkItemRelProc TCWorkerThread::OnGetWorkItemRelProc();

  ///////////////////////////////////////////////////////////////////////////
  // Description: Pure-virtual override to process a queued element of work.
  //
  // The derived class must override this pure-virtual method to process a
  // queued element of work. The parameters represent the same parameters
  // that were specified in one of the PostMessage calls.
  //
  // See the Class Overview for detailed information on how to implement
  // this.
  //
  // Parameters:
  //   idMsg - An identifier, meaningful only in the context of the derived
  // class, used to identify the type of a queued element of work.
  //   cParams - The number of LPARAM arguments pointed to by the
  // /rgParams/ parameter.
  //   rgParams - An array of LPARAM arguments specifying the LPARAM
  // arguments associated with the element of work. This pointer is valid
  // only for the number of LPARAM's specified by the /cParams/ parameter.
  // These arguments are only meaninful in the context of the derived class.
  //
  // See Also: TCWorkerThread::ThreadProc, TCWorkerThread::XWorkItem,
  // TCWorkerThread::XWorkItem::m_idMsg,
  // TCWorkerThread::XWorkItem::m_vec, TCWorkerThread::PostMessage,
  // TCWorkerThread::PostMessageV, TCWorkerThread::PostMessageEx
  void TCWorkerThread::OnMessage(UINT idMsg, int cParams, LPARAM* rgParams);
#endif // _DOCJET_ONLY


/////////////////////////////////////////////////////////////////////////////
// Group=Implementation

/////////////////////////////////////////////////////////////////////////////
// Description: The worker thread's main procedure.
//
// This static class method is the entry point for the worker thread.
// Its main role is to cast the thread parameter as the class instance
// pointer and delegate to the non-static ThreadProc method.
//
// Prior to delegating to the non-static method, the thread is entered into
// the process's Multi-Threaded Apartment (MTA), as defined by the COM
// subsystem. The thread is removed from the Apartment after returning from
// the non-static method, just prior to returning from the thread procedure.
//
// Return Value: The return value is always zero and is only provided to
// satisfy the prototype of a thread procedure.
//
// See Also: TCWorkerThread::ThreadProc, TCWorkerThread::OnMessage,
// TCWorkerThread::PostMessage, TCWorkerThread::PostMessageV,
// TCWorkerThread::PostMessageEx, TCWorkerThread::XWorkItem
//
unsigned TCWorkerThread::ThreadThunk(void* pvThis)
{
  TCERRLOG0("TCWorkerThread::ThreadThunk(): Entering ThreadProc\n");

  // Enter this thread into the MTA
  HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
  PRIVATE_ASSERTE((SUCCEEDED(hr)));

  // Typecast the specified parameter
  TCWorkerThread* pThis = reinterpret_cast<TCWorkerThread*>(pvThis);

  // Delegate to the non-static method
  pThis->ThreadProc();

  // Remove this thread from the MTA
  CoUninitialize();

  TCERRLOG0("TCWorkerThread::ThreadProc(): Exiting ThreadProc\n");
  return 0;
}

/////////////////////////////////////////////////////////////////////////////
// Description: The worker thread's (non-static) main procedure.
//
// This class method is called from the static ThreadThunk method entry
// point for the worker thread. Its main role is to service the work item
// queue.
//
// The thread simply waits for either of two events to be signaled. When the
// m_shevtShutdown event is signaled, the thread cleans-up any remaing work
// items in the queue and exits. When the m_shevtQueueNotEmpty event is
// signaled, the next work item is popped from the queue. The work items are
// each instances of the XWorkItem class. The virtual OnMessage override is
// then called to allow the derived class to perform the work. Following the
// virtual method call, the XWorkItem instance is deleted which, in turn,
// will allow the derived class to release any resources represented by the
// arguments. Keep in mind that although the element of work is processed by
// the OnMessage override of the derived class, *the* *method* *is* „
// *called* *in* *the* *processing* *context* *of* *the* *worker* „
// *thread.* This may pose little or no problem for most situations, but it
// deserves to be mentioned here.
//
// Note: The virtual method is called within a *__try* block in case the
// derived class instance has been destroyed or throws an uncaught exception
// for any other reason. /If/ /the/ /derived/ /class/ /is/ /a/ /COM/ „
// /object,/ /the/ /TCWorkerThread::XWorkItem::m_punkOwner/ /data/ /member/ „
// /should/ /have/ /been/ /set,/ /causing/ /an/ /AddRef./ /This/ /should/ „
// /circumvent/ /the/ /possibility/ /of/ /the/ /instance/ /being/ „
// /destroyed/ /while/ /it/ /still/ /has/ /elements/ /of/ /work/ /in/ /the/ „
// /queue./ However, this will also catch an ill-behaved override, so as to
// not unexpectedly crash the thread. The associated *__except* block simply
// sends a text message to the debug monitor but, mainly, serves the purpose
// of catching the exception in a consistent, well-defined manner. The
// work item instance is deleted *after* the entire exception block,
// to guarantee that it gets destructed properly.
// 
// See Also: TCWorkerThread::ThreadThunk, TCWorkerThread::OnMessage,
// TCWorkerThread::PostMessage, TCWorkerThread::PostMessageV,
// TCWorkerThread::PostMessageEx, TCWorkerThread::XWorkItem
//
void TCWorkerThread::ThreadProc()
{
  bool fWinNT = ::IsWinNT();

  // Define the enumeration and array of objects upon which to wait
  enum
  {
    e_Shutdown = WAIT_OBJECT_0,
    e_QueueNotEmpty,
  };
  HANDLE hObjs[] =
  {
    m_shevtShutdown,
    m_shevtQueueNotEmpty
  };
  const DWORD cObjs = sizeofArray(hObjs);

  // Continuously wait for the 'shutdown' or 'queue-not-empty' event
  DWORD dwWait;
  do
  {
    if (fWinNT)
      dwWait = WaitForMultipleObjectsEx(cObjs, hObjs, 0, INFINITE, true);
    else
      dwWait = WaitForMultipleObjects(cObjs, hObjs, 0, INFINITE);

    if (e_QueueNotEmpty == dwWait)
    {
      // Get the next item of work from the queue
      XWorkItem* pWorkItem = NULL;
      {
        XLockQueue lock(&m_csQueue);
        if (!m_queue.empty())
        {
          pWorkItem = m_queue.front();
          m_queue.pop();
        }
        if (m_queue.empty())
        {
          lock.Unlock();
          ResetEvent(m_shevtQueueNotEmpty);
        }
      }

      // Process the item of work (deletes it for us)
      if (pWorkItem)
        DispatchWorkItem(pWorkItem);
    }

  } while (e_Shutdown != dwWait);

  // Clear the remaining items from the queue
  XLockQueue lock(&m_csQueue);
  while (!m_queue.empty())
  {
    XWorkItem* pWorkItem = m_queue.front();
    m_queue.pop();
    DestroyWorkItem(pWorkItem);
  }

  // Clear the thread pointer since we're about to die
  InterlockedExchange(reinterpret_cast<long*>(&m_pth), 0L);
}

/////////////////////////////////////////////////////////////////////////////
//
void TCWorkerThread::DispatchWorkItem(TCWorkerThread::XWorkItem* pItem)
{
  #if defined(XWorkItem_TRACE)
    GetDebugOutput()->WriteLen(60,
      "XWorkItem\t%08X\tTCWorkerThread::DispatchWorkItem\n", pItem);
  #endif // defined(XWorkItem_TRACE)
  OnMessage(pItem->m_idMsg, pItem->m_vec.size(), &(*(pItem->m_vec.begin())));
  DestroyWorkItem(pItem);
}

/////////////////////////////////////////////////////////////////////////////
//
void TCWorkerThread::DestroyWorkItem(TCWorkerThread::XWorkItem* pItem)
{
  #if defined(XWorkItem_TRACE)
    GetDebugOutput()->WriteLen(60,
      "XWorkItem\t%08X\tTCWorkerThread::DestroyWorkItem\n", pItem);
  #endif // defined(XWorkItem_TRACE)
  delete pItem;
}

