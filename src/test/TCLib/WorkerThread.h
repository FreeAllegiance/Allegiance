#ifndef __WorkerThread_h__
#define __WorkerThread_h__

/////////////////////////////////////////////////////////////////////////////
// WorkerThread.h | Declaration of the TCWorkerThread class.
//

#include <list>
#include <queue>
#include "TCThread.h"
#include "AutoHandle.h"
#include "AutoCriticalSection.h"
#include "ObjectLock.h"


/////////////////////////////////////////////////////////////////////////////
// Type definition for a user-defined callback function that is called by the
// implementation of TCWorkerThread. The callback function is expected to
// release any resources associated with the specified thread arguments.
//
// A class derived from TCWorkerThread should override the virtual
// TCWorkerThread::OnGetWorkItemRelProc method and return a function pointer
// of this type. Typically, the class will specify a static class method.
//
// See Also: TCWorkerThread, TCWorkerThread::OnGetWorkItemRelProc,
// TCWorkerThread::XWorkItem, TCWorkerThread_ArgumentReleaseProc
typedef void (WINAPI *TC_WorkItemRelProc)(UINT, int, LPARAM*);

#ifdef _DOCJET_ONLY
  ///////////////////////////////////////////////////////////////////////////
  // The TCWorkerThread_ArgumentReleaseProc function is an
  // application-defined callback function used with the TCWorkerThread
  // class. It receives notification that a queued element has finished
  // executing in the worker thread. The callback function should release
  // any resources associated with the specified arguments. The
  // TC_WorkItemRelProc type defines a pointer to this callback function.
  // TCWorkerThread_ArgumentReleaseProc is a placeholder for the
  // application-defined function name.
  //
  // Parameters:
  //   idMsg - The message identifier used to identify different elements of
  // work that the TCWorkerThread-derived class has posted to the worker
  // thread. Note that this is *not* a window message and can be any UINT
  // value meaningful to the derived class. Usually, the derived class will
  // declare an enumeration in the class to define these identifiers.
  //   cParams - The number of LPARAM arguments pointed to by the
  // /rgParams/ parameter.
  //   rgParams - An array of LPARAM arguments specified when the element of
  // work was queued to the worker thread.
  //
  // See Also: TC_WorkItemRelProc, TCWorkerThread,
  // TCWorkerThread::OnGetWorkItemRelProc, TCWorkerThread::PostMessage,
  // TCWorkerThread::PostMessageV, TCWorkerThread::PostMessageEx,
  // TCWorkerThread::XWorkItem
void WINAPI TCWorkerThread_ArgumentReleaseProc(UINT idMsg, int cParams,
  LPARAM* rgParams);
#endif // _DOCJET_ONLY


/////////////////////////////////////////////////////////////////////////////
// TCWorkerThread provides a base class from which a class can inherit to
// perform elements of work within a seperate worker thread.
//
// Possible uses for this class include:
// + Performing delayed garbage collection.
// + Performing less time-critical tasks outside of a main thread.
//
// To use this class, first include it as a public base in your
// class declaration:
//
//      class CMyClass : public TCWorkerThread
//      {
//
// It should rarely conflict with other base class method names, so it is
// also suitable for use with multiple inheritance:
//
//      class CMIClass :
//        public IUnknown,
//        public TCWorkerThread
//      {
//
// Next, declare the overrides for the pure virtual methods of the class:
//
//      // Overrides
//      protected:
//        virtual IUnknown* OnGetUnknown();
//        virtual TC_WorkItemRelProc OnGetWorkItemRelProc();
//        virtual void OnMessage(UINT idMsg, int cParams, LPARAM* rgParams);
//        
// Following that, you should declare a *static* method used to release
// message arguments:
//
//      // Implementation
//      protected:
//        static void WINAPI ArgumentReleaseProc(UINT idMsg, int cParams,
//          LPARAM* rgParams);
//
// Then, declare an unnamed enumeration in your class, naming the type (or
// types) of work that your class needs to perform from the worker thread:
//
//      // Types
//      protected:
//        enum {e_NotifyEngineer, e_NotifyManager, e_NotifyExecutive};
//      };
//
// Next, implement OnGetUnknown. If your class is not a COM object, this can
// simply return NULL:
//
//      IUnknown* CMyClass::OnGetUnknown()
//      {
//        return NULL;
//      }
//
// But, if your class *does* implement a component object, you must return
// an IUnknown pointer on your class. Do *not* AddRef the interface pointer:
//
//      IUnknown* CMIClass::OnGetUnknown()
//      {
//        // ATL objects should use GetUnknown() instead of 'this'
//        return static_cast<IUnknown*>(this);
//      }
//
// The implementation of OnGetWorkItemRelProc is just as easy:
//
//      TC_WorkItemRelProc CMyClass::OnGetWorkItemRelProc()
//      {
//        return ArgumentReleaseProc;
//      }
//
// Note: The OnGetWorkItemRelProc can return NULL only if *all* the arguments
// passed to the PostMessage, PostMessageV, or PostMessageEx methods do not
// represent open resources. Open resources include things such as pointers,
// handles, registry keys, etc. This is rarely the case, but if such
// conditions do exists for your class, OnGetWorkItemRelProc can return NULL
// and the ArgumentReleaseProc static callback would not need to be declared
// or implemented.
//
// The last of the overrides is where you actually perform the work for a
// queued element. Usually, however, if multiple types of work are defined,
// the method will just switch, unpack the arguments, and call a function to
// peform one specific type of work:
//
//      void CMyClass::OnMessage(UINT idMsg, int cParams, LPARAM* rgParams)
//      {
//        switch (idMsg)
//        {
//          case e_NotifyEngineer:
//          {
//            ZAssert(2 == cParams);
//            CBonus* psi = reinterpret_cast<CBonus*>(rgParams[0]);
//            LPCTSTR psz = reinterpret_cast<LPCTSTR>(rgParams[1]);
//            NotifyEngineer(psi, psz);
//            return;
//          }
//          case e_NotifyManager:
//          {
//            ZAssert(1 == cParams);
//            CWeatherRpt* pwr = reinterpret_cast<CWeatherRpt*>(rgParams[0]);
//            NotifyManager(pwr);
//            return;
//          }
//          case e_NotifyExecutive:
//          {
//            ZAssert(1 == cParams);
//            CBottomLine* pbl = reinterpret_cast<CBottomLine*>(rgParams[0]);
//            NotifyExecutive(pbl);
//            return;
//          }
//        }
//      }
//
// Note: If you *really* dislike switch statements, you could specify a
// class function pointer as the /idMsg/ parameter to the PostMessage
// methods. Then, the switch statement (and the enum) could be eliminated.
// Notice, though, that this approach would push the parameter unpacking down
// to the individual member functions, which may also be considered more
// elegant.
//
//      void CMyClass::OnMessage(UINT idMsg, int cParams, LPARAM* rgParams)
//      {
//        // Declare a typedef for a member function pointer
//        typedef void (CMyClass::*WORK_PROC)(int, LPARAM*);
//        …
//        // Cast the idMsg back to a WORK_PROC
//        WORK_PROC pfn = (WORK_PROC)idMsg;
//        …
//        // Call the member function
//        (this->*pfn)(cParams, rgParams);
//      }
//
// Finally, the static method needs to be implemented to release the memory
// used by the parameters:
//
//      void WINAPI CMyClass::ArgumentReleaseProc(UINT idMsg, int cParams,
//        LPARAM* rgParams)
//      {
//        switch (idMsg)
//        {
//          case e_NotifyEngineer:
//          {
//            ZAssert(2 == cParams);
//            CBonus* psi = reinterpret_cast<CBonus*>(rgParams[0]);
//            LPCTSTR psz = reinterpret_cast<LPCTSTR>(rgParams[1]);
//            delete psi;
//            delete [] psz;
//            return;
//          }
//          case e_NotifyManager:
//          {
//            ZAssert(1 == cParams);
//            CWeatherRpt* pwr = reinterpret_cast<CWeatherRpt*>(rgParams[0]);
//            delete pwr;
//            return;
//          }
//          case e_NotifyExecutive:
//          {
//            ZAssert(1 == cParams);
//            CBottomLine* pbl = reinterpret_cast<CBottomLine*>(rgParams[0]);
//            delete pbl;
//            return;
//          }
//        }
//      }
//
// You probably noticed that this method does most of the same things that
// the virtual OnMessage method has to perform. It would seem likely that the
// two operations (process and release) on the work element could be done in
// the same method. A flag would indicate which operation to be performed on
// the arguments once they're unpacked. This /could/ be done, but with one
// minor caveat. Since the release method is static, the common function
// would have to take great care to not reference any instance data or call
// any non-static method. If this sounds like a reasonable tradeoff, the
// static release method could be coded as follows:
//
//      void WINAPI CMyClass::ArgumentReleaseProc(UINT idMsg, int cParams,
//        LPARAM* rgParams)
//      {
//        // Declare a typedef for a member function pointer
//        typedef void CMyClass::*WORK_PROC(int, LPARAM*);
//        …
//        // Cast the idMsg back to a WORK_PROC
//        WORK_PROC* pfn = (WORK_PROC*)idMsg;
//        …
//        // Create a NULL instance pointer (be careful with it!)
//        CMyClass* pThis = (CMyClass*)(NULL);
//        …
//        // Call the member function
//        (pThis->*pfn)(cParams, rgParams);
//      }
//
// For this to work correctly, the member function must *not* be virtual, and
// it needs to check the *this* pointer to determine if it is to release the
// arguments or to process them:
//
//      void CMyClass::NotifyExecutive(int cParams, LPARAM* rgParams)
//      {
//        // Unpack the arguments
//        ZAssert(1 == cParams);
//        CBottomLine* pbl = reinterpret_cast<CBottomLine*>(rgParams[0]);
//        …
//        // Delete or process the arguments
//        if (this)
//        {
//          // Notify the executive of the bottom line...
//        }
//        else
//        {
//          delete pbl;
//        }
//      }
//
// Note: For a short discussion of the reason that the release method must be
// static, refer to the documentation for the OnGetWorkItemRelProc override.
//
// Finally, in the line of normal processing, when the derived object needs
// to queue the element of work to the worker thread, a simple call to one
// of the PostMessage methods is all that it takes:
//
//      HRESULT CMyClass::ChangeBottomLine(double dRevenue, double dExpenses)
//      {
//        // Save the values, etc...
//        // ...
//        …
//        // Notify the executives of the change
//        CBottomLine* pbl = new CBottomLine(dRevenue, dExpenses);
//        PostMessage(e_NotifyExecutive, pbl);
//        …
//        // Indicate success
//        return S_OK;
//      }
//
// Of course, the PostMessage call would look slightly different if the
// non-switch approach were being used:
//
//        PostMessage(UINT(NotifyExecutive), pbl);
//
// ToDo: The majority of this class's functionality would be useful outside
// of the singleton scenario. The argument handling and thread processing
// should be moved into either a base class or another stand-alone class.
//
// See Also: TCWorkerThread_ArgumentReleaseProc,
// TCWorkerThread::XWorkItem
class TCWorkerThread
{
// Group=Types
protected:
  class XWorkItem;

// Construction / Destruction
public:
  TCWorkerThread();
  virtual ~TCWorkerThread();

// Disallow copy constructor
private:
  TCWorkerThread(const TCWorkerThread&);

// Attributes
public:
  bool IsCurrentThread();

// Operations
public:
  void Close();
  void PostMessage(UINT idMsg, int cParams, ...);
  void PostMessageV(UINT idMsg, int cParams, va_list argptr);
  void PostMessageEx(UINT idMsg, int cParams, LPARAM* rgParams);

// Overrides
protected:
  virtual IUnknown* OnGetUnknown() = 0;
  virtual TC_WorkItemRelProc OnGetWorkItemRelProc() = 0;
  virtual void OnMessage(UINT idMsg, int cParams, LPARAM* rgParams) = 0;

// Implementation
protected:
  static unsigned WINAPI ThreadThunk(void*);
  void ThreadProc();
  void DispatchWorkItem(XWorkItem* pArgs);
  void DestroyWorkItem(XWorkItem* pArgs);

// Group=Types
protected:
	typedef std::queue<XWorkItem*, std::list<XWorkItem*> > XQueue;
	typedef TCObjectLock<TCAutoCriticalSection>            XLockQueue;

// Group=Data Members
protected:
  #pragma pack(push, 4)
    /////////////////////////////////////////////////////////////////////////
    // Description: Pointer to the thread object.
    //
    // See Also: TCThread
    TCThread* m_pth;

    /////////////////////////////////////////////////////////////////////////
    // Description: A flag indicating that the Close method has been called.
    //
    // A flag indicating that the Close method has been called. This is used
    // by the Close method to ensure that the reference count of the worker
    // thread does not get decremented more than once by an instance. This is
    // important since the destructor calls Close.
    //
    // See Also: TCWorkerThread::destructor, TCWorkerThread::Close
    BOOL      m_bClosed;
  #pragma pack(pop)

  ///////////////////////////////////////////////////////////////////////////
  // Description: An event handle used to signal when new work items are
  // available on the queue.
  //
  // See Also: TCHandle, TCAutoHandle
  TCHandle m_shevtQueueNotEmpty;

  ///////////////////////////////////////////////////////////////////////////
  // Description: An event handle used to signal when the thread should be
  // shutdown.
  //
  // See Also: TCHandle, TCAutoHandle
  TCHandle m_shevtShutdown;

  ///////////////////////////////////////////////////////////////////////////
  // Description: Controls synchronized access to the Queue collection.
  //
  // See Also: TCAutoCriticalSection
  TCAutoCriticalSection m_csQueue;

  ///////////////////////////////////////////////////////////////////////////
  // Description: The queue of work items.
  //
  // See Also: TCWorkerThread::XQueue
  XQueue m_queue;
};


/////////////////////////////////////////////////////////////////////////////
// Group=Attributes

inline bool TCWorkerThread::IsCurrentThread()
{
  return !m_pth || GetCurrentThreadId() == m_pth->m_nThreadID;
}


/////////////////////////////////////////////////////////////////////////////

#endif // !__WorkerThread_h__
