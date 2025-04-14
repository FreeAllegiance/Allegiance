/////////////////////////////////////////////////////////////////////////////
// UtilityThread.cpp | Implementation of the TCUtilityThread class.
//

#include "pch.h"
#include "..\Inc\TCLib.h"
#include "UtilityThread.h"
#include "TCThread.h"


/////////////////////////////////////////////////////////////////////////////
// TCUtilityThread


/////////////////////////////////////////////////////////////////////////////
// Group=Types

/////////////////////////////////////////////////////////////////////////////
// Description: Nested class that manages a variable-length array of
// message arguments.
//
// Used by TCUtilityThread to manage a variable-length array of LPARAM
// arguments associated with an element of work in the utility thread's
// queue.
//
// This class represents a single element of work in the utility thread's
// queue. As such, it manages an identifier to indicate (to the
// TCUtilityThread-derived class) what type of work element is represented.
// Also, any data items associated with the element of work are managed as
// a variable-length array of LPARAM arguments. Since only the derived
// class understands the contents of the arguments, it is responsible for
// releasing them when the element of work is complete (or has failed).
// A callback function pointer is stored, which is called to perform any
// necessary cleanup of the arguments. Finally, if the derived class is a
// COM object, a reference to it will be held so that the object can not be
// released until the element of work completes (or fails).
//
// See Also: TCUtilityThread, TCUtilityThread::PostMessage,
// TCUtilityThread::PostMessageV, TCUtilityThread::PostMessageEx,
//
class TCUtilityThread::XWorkItem
{
// Group=Construction / Destruction
public:
  ///////////////////////////////////////////////////////////////////////////
  // Simply initializes the data members from the specified parameters.
  //
  // Parameters:
  //   punkOwner - The IUnknown of the TCUtilityThread-derived class. This
  // may be NULL if the derived-class is not a COM object. By copying this to
  // the m_punkOwner data member, AddRef is implicitly called since the data
  // member is a smart pointer.
  //   pfnRelease - A callback function pointer of type TC_UtilArgRelProc,
  // called to release any resources associated with the arguments.
  //   idMsg - An identifier, meaningful only in the context of the derive
  // class, used to identify the type of a queued element of work.
  //   cParams - The number of LPARAM arguments pointed to by the
  // /rgParams/ parameter.
  //   rgParams - An array of LPARAM arguments specified when the element of
  // work was queued to the utility thread. These arguments are only
  // meaninful in the context of the derived class.
  //
  // See Also: TCUtilityThread::XWorkItem::destructor, TCUtilityThread,
  // TCUtilityThread::PostMessage, TCUtilityThread::PostMessageV,
  // TCUtilityThread::PostMessageEx, TCUtilityThread_ArgumentReleaseProc,
  // TC_UtilArgRelProc
  XWorkItem(IUnknown* punkOwner, TC_UtilArgRelProc pfnRelease, UINT idMsg,
    int cParams, LPARAM* rgParams) :
    m_punkOwner(punkOwner),
    m_pfnRelease(pfnRelease),
    m_idMsg(idMsg),
    m_vec(rgParams, rgParams + cParams)
  {
  }
  ///////////////////////////////////////////////////////////////////////////
  // If a callback function was specified for releasing the arguments, it is
  // called when the object is destroyed.
  //
  // If an IUnknown* of the derived-class was specified, an implicit Release
  // is performed.
  //
  // See Also: TCUtilityThread::XWorkItem::constructor, TCUtilityThread,
  // TCUtilityThread::PostMessage, TCUtilityThread::PostMessageV,
  // TCUtilityThread::PostMessageEx, TCUtilityThread_ArgumentReleaseProc,
  // TC_UtilArgRelProc
  XWorkItem::~XWorkItem()
  {
    if (m_pfnRelease)
	    (*m_pfnRelease)(m_idMsg, m_vec.size(), &(*m_vec.begin()));
  }

// Group=Data Members
public:
  // IUnknown* of the owner object, if it's a COM object
  IUnknownPtr         m_punkOwner;
  // Callback function used to release the arguments.
  TC_UtilArgRelProc   m_pfnRelease;
  // Identifies the type of a queued element of work in the utility thread.
  UINT                m_idMsg;
  // A variable-length array of the arguments associated with a queued
  // element of work.
  std::vector<LPARAM> m_vec;
};


/////////////////////////////////////////////////////////////////////////////
// Static Initialization

/////////////////////////////////////////////////////////////////////////////
// Description: The number of TCUtilityThread (or derived class)
// instances referencing the shared utility thread.
//
// This static data member contains the number of TCUtilityThread (or derived
// class) instances referencing the shared utility thread.
//
// This member is packed on a DWORD boundary since it is specified as a
// parameter to the Win32 ::InterlockedIncrement and ::InterlockedDecrement
// API's.
//
// TODO: Currently, the interactions with this data member and m_pth does not
// appear to be completely thread safe. There should probably be a static
// TCAutoCriticalSection to control access to both of these data members.
// When that change is made, the data members will no longer need to be
// packed on DWORD boundaries, since the critical section would be used
// instead of the Win32 ::Interlocked... API's.
//
// See Also: TCUtilityThread::constructor, TCUtilityThread::destructor,
// TCUtilityThread::m_pth
long TCUtilityThread::m_nRefs = 0;

/////////////////////////////////////////////////////////////////////////////
// Description: A pointer to the shared utility thread.
//
// This static data member contains a pointer to the shared utility thread,
// or NULL if the thread has not yet been created.
//
// For each module (DLL or EXE) of a process, there can be, at most, one
// shared utility thread. This means, however, that a process may have
// multiple shared utility threads since the EXE can have one, and each DLL
// can have one.
//
// TODO: This could be alleviated by changing the implementation to use a
// process-wide shared utility thread. The static members could be put into a
// DLL and either a [local] COM object or (C-style) API exports could be
// provided to access (and ref-count) it. The [local] attribute of the COM
// object approach would make it possible to pass the pointer value *without*
// any marshaling. The [local] interface would *not* need to be
// [oleautomation], [dual], or even included in the type libary, although
// that would make it difficult to use the #import directive.
//
// This member is packed on a DWORD boundary since it may need to be
// specified as a parameter to the Win32 ::InterlockedExchange API.
//
// TODO: Currently, the interactions with this data member and m_nRefs does
// not appear to be completely thread safe. There should probably be a static
// TCAutoCriticalSection to control access to both of these data members.
// When that change is made, the data members will no longer need to be
// packed on DWORD boundaries, since the critical section would be used
// instead of the Win32 ::Interlocked... API's.
//
// See Also: TCThread, TCUtilityThread::constructor,
// TCUtilityThread::ThreadProc, TCUtilityThread::m_nRefs
TCThread* TCUtilityThread::m_pth = NULL;


/////////////////////////////////////////////////////////////////////////////
// Construction / Destruction

/////////////////////////////////////////////////////////////////////////////
// Increments the reference count of the shared utility thread. If this is
// the first instance, the utility thread is created.
//
// The copy constructor is declared private to disallow copying of objects of
// this type. It has only a declaration and no implementation.
//
// See Also: TCUtilityThread::m_nRefs, TCUtilityThread::m_pth
TCUtilityThread::TCUtilityThread() :
  m_bClosed(false)
{
  // Increment the static thread's reference count
  InterlockedIncrement(&m_nRefs);

  // If this is the first reference, create the thread
  if (1 == m_nRefs)
  {
    int nPriority = THREAD_PRIORITY_NORMAL;
    m_pth = TCThread::BeginThread(ThreadProc, NULL, nPriority, 4096);
  }
}

/////////////////////////////////////////////////////////////////////////////
// Calls Close to decrement the reference count of the utility thread.
//
// See Also: TCUtilityThread::constructor, TCUtilityThread::Close
TCUtilityThread::~TCUtilityThread()
{
  Close();
}


/////////////////////////////////////////////////////////////////////////////
// Group=Operations

/////////////////////////////////////////////////////////////////////////////
// Description: Decrements the reference count of the shared utility thread.
//
// Decrements the reference count of the shared utility thread. If the
// resulting reference count is zero, a WM_QUIT message is posted to the
// thread and the thread is waited upon to exit.
//
// The m_bClosed data member is used to disallow the class instance from
// decrementing the reference count more than once. This is important since
// the destructor calls Close.
//
// Note: This method is used internally, as noted above, by the destructor.
// It is made public to allow an advanced usage of the class. Most often,
// this method will *not* need to be called directly.
//
// TODO: The PostMessage methods should also check the m_bClosed flag prior
// to posting a message, but currently they don't. Not an issue until
// someone pokes an eye out calling PostMessage *after* calling close.
//
// See Also: TCUtilityThread::destructor, TCUtilityThread::m_bClosed,
// TCUtilityThread::m_nRefs, TCUtilityThread::m_pth
void TCUtilityThread::Close()
{
  if (!m_bClosed)
  {
    // Indicate that we have closed
    m_bClosed = true;

    // Decrement the reference count
    if (0 == InterlockedDecrement(&m_nRefs))
    {
      // Get the thread handle and ID
      assert(m_pth);
      HANDLE hth     = m_pth->m_hThread;
      DWORD idThread = m_pth->m_nThreadID;

      // Signal the thread to exit
      m_pth->PostThreadMessage(WM_QUIT, 0, 0);

      // Wait for the thread to exit
      if (GetCurrentThreadId() != idThread)
        WaitForSingleObject(hth, INFINITE);
    }
  }
}

/////////////////////////////////////////////////////////////////////////////
// Description: Posts an element of work to the shared utility thread's
// queue.
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
// These methods are used to post an element of work to the shared utility
// thread's message queue. An element of work consists of an identifier,
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
// See Also: TCUtilityThread::XWorkItem, TCUtilityThread::ThreadProc
void TCUtilityThread::PostMessage(UINT idMsg, int cParams, ...)
{
  va_list argptr;
  va_start(argptr, cParams);
  PostMessageV(idMsg, cParams, argptr);
  va_end(argptr);
}

/////////////////////////////////////////////////////////////////////////////
// {partof:PostMessage}
void TCUtilityThread::PostMessageV(UINT idMsg, int cParams, va_list argptr)
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
void TCUtilityThread::PostMessageEx(UINT idMsg, int cParams, LPARAM* rgParams)
{
  assert(m_pth);
  IUnknown* punk = OnGetUnknown();
  TC_UtilArgRelProc pfnRelease = OnGetArgRelProc();
  XWorkItem* pArgs =
    new XWorkItem(punk, pfnRelease, idMsg, cParams, rgParams);
  if (!m_pth || !m_pth->PostThreadMessage(wm_message, WPARAM(this), LPARAM(pArgs)))
    DispatchWorkItem(pArgs);
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
  // See Also: TCUtilityThread::ThreadProc, TCUtilityThread::XWorkItem,
  // TCUtilityThread::XWorkItem::m_punkOwner,
  // TCUtilityThread::PostMessage, TCUtilityThread::PostMessageV,
  // TCUtilityThread::PostMessageEx
  IUnknown* TCUtilityThread::OnGetUnknown();

  ///////////////////////////////////////////////////////////////////////////
  // Description: Pure-virtual override to specify a callback function used
  // to destroy the contents of a work element's arguments.
  //
  // The derived class defines the types of work that it will perform from
  // the shared utility thread. Because of this, it must provide the address
  // of a callback function that will release any resources associated with
  // the arguments of a queued element of work. When an element of work is
  // queued to the utility thread, this override is called to get the
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
  // TCUtilityThread_ArgumentReleaseProc for the prototype of this function.
  //
  // See Also: TCUtilityThread_ArgumentReleaseProc, TC_UtilArgRelProc,
  // TCUtilityThread::ThreadProc, TCUtilityThread::XWorkItem,
  // TCUtilityThread::XWorkItem::m_pfnRelease,
  // TCUtilityThread::PostMessage, TCUtilityThread::PostMessageV,
  // TCUtilityThread::PostMessageEx
  TC_UtilArgRelProc TCUtilityThread::OnGetArgRelProc();

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
  // See Also: TCUtilityThread::ThreadProc, TCUtilityThread::XWorkItem,
  // TCUtilityThread::XWorkItem::m_idMsg,
  // TCUtilityThread::XWorkItem::m_vec, TCUtilityThread::PostMessage,
  // TCUtilityThread::PostMessageV, TCUtilityThread::PostMessageEx
  void TCUtilityThread::OnMessage(UINT idMsg, int cParams, LPARAM* rgParams);
#endif // _DOCJET_ONLY


/////////////////////////////////////////////////////////////////////////////
// Group=Implementation

/////////////////////////////////////////////////////////////////////////////
// Description: The shared utility thread's main procedure.
//
// This static class method is the entry point for the shared utility thread.
// It's main role is to service the message queue, which is used to
// implement the queue of work elements to be processed by the shared utility
// thread.
//
// Aside from WM_QUIT, used to exit the message loop (and the thread), the
// only message expected to be posted to the message queue is wm_message,
// which is declared in a class enumeration as WM_APP. Upon receiving this
// message, the /wParam/ is cast as a pointer to the instance of
// TCUtilityThread that posted the element of work. Next, the /lParam/ is
// cast as a pointer to the XWorkItem instance that represents the queued
// element of work. The virtual OnMessage override is then called to allow
// the derived class to perform the work. Following the virtual method call,
// the XWorkItem instance is deleted which, in turn, will allow the
// derived class to release any resources represented by the arguments. Keep
// in mind that although the element of work is processed by the OnMessage
// override of the derived class, *the* *method* *is* *called* *in* *the* „
// *processing* *context* *of* *the* *shared* *utility* *thread.* This may
// pose little or no problem for many situations, but it deserves to be
// mentioned here.
//
// Note: The virtual method is called within a *__try* block in case the
// derived class instance has been destroyed. /If/ /the/ /derived/ /class/ „
// /is/ /a/ /COM/ /object,/ /the/ „
// /TCUtilityThread::XWorkItem::m_punkOwner/ /data/ /member/ /should/ „
// /have/ /been/ /set,/ /causing/ /an/ /AddRef./ /This/ /should/ „
// /circumvent/ /the/ /possibility/ /of/ /the/ /instance/ /being/ „
// /destroyed/ /while/ /it/ /still/ /has/ /elements/ /of/ /work/ /in/ /the/ „
// /queue./ However, this will also catch an ill-behaved override, so as to
// not unexpectedly crash the thread. The associated *__except* block simply
// sends a text message to the debug monitor but, mainly, serves the purpose
// of catching the exception in a consistent, well-defined manner. The
// message arguments instance is deleted *after* the entire exception block,
// to guarantee that it gets destructed properly.
// 
// Prior to entering the message loop, the thread is entered into the
// process's Multi-Threaded Apartment (MTA), as defined by the COM subsystem.
// The thread is removed from the Apartment after exiting the message loop,
// just prior to returning from the thread procedure.
//
// Parameters:
//   The void* paramter of the thread procedure prototype is not needed. It
// therefore is neither used nor declared in the parameter list.//
//
// Return Value: The return value is always zero and is only provided to
// satisfy the prototype of a thread procedure.
//
// See Also: TCUtilityThread::OnMessage, TCUtilityThread::PostMessage,
// TCUtilityThread::PostMessageV, TCUtilityThread::PostMessageEx,
// TCUtilityThread::XWorkItem
unsigned TCUtilityThread::ThreadProc(void*)
{
  TCERRLOG0("TCUtilityThread::ThreadProc(): Entering ThreadProc\n");

  // Enter this thread into the MTA
  HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
  assert((SUCCEEDED(hr)));

  // Pump the message queue
  MSG msg;
  while (GetMessage(&msg, NULL, 0, 0))
  {
    switch (msg.message)
    {
      case wm_message:
      {
        TCUtilityThread* pThis = (TCUtilityThread*)msg.wParam;
        XWorkItem* pArgs = (XWorkItem*)msg.lParam;
        pThis->DispatchWorkItem(pArgs);
        break;
      }
    }   
  }

  // Clear the thread pointer since we're about to die
  m_pth = NULL;

  // Remove this thread from the MTA
  CoUninitialize();

  TCERRLOG0("TCUtilityThread::ThreadProc(): Exiting ThreadProc\n");
  return 0;
}

void TCUtilityThread::DispatchWorkItem(TCUtilityThread::XWorkItem* pArgs)
{
///  __try
	try
  {
    OnMessage(pArgs->m_idMsg, pArgs->m_vec.size(), &(*(pArgs->m_vec.begin())));
  }
///  __except(1)
	catch(...)
  {
    TCERRLOG0("TCUtilityThread::DispatchThreadMessage: Exception Caught! Message loop continuing...\n");
  }
  delete pArgs;
}

