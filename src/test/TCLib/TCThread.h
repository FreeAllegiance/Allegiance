#ifndef __TCThread_h__
#define __TCThread_h__

#include <zassert.h>
#include "AutoHandle.h"


/////////////////////////////////////////////////////////////////////////////
// TCThread.h | Declaration of the TCThread class
// 


/////////////////////////////////////////////////////////////////////////////
// Forward Declarations

/////////////////////////////////////////////////////////////////////////////
// Type definition/prototype for a user-defined callback function that
// represents a thread, specified as the /pfnThreadProc/ parameters of the
// TCThread::BeginThread method.
//
// See Also: TCThread, TCThread::BeginThread
typedef unsigned (__stdcall* TC_THREADPROC)(void*);


/////////////////////////////////////////////////////////////////////////////
// Remarks: This class is like CWinThread with all that Thread State and
// m_pMainWnd stuff removed from it.  It came about because many COM objects
// won't be able to use CWinThread because there won't be a MainWnd for the
// thread to be associated with.  This class does not use MFC.
//
// The Usage is almost identical to CWinThread.  Call the static function 
// TCThread::BeginThread() to create and start the thread.  When the
// controlling function returns, the thread is automatically cleaned up, just
// like CWinThread.  The following function may also be used on the thread:
//
//       int GetThreadPriority()
//       BOOL SetThreadPriority(int nPriority)
//       DWORD SuspendThread()
//       DWORD ResumeThread()
//       bool PostThreadMessage(UINT message, WPARAM wParam, LPARAM lParam)
class TCThread
{
// Construction / Destruction
public:
  TCThread();
  TCThread(TC_THREADPROC pfnThreadProc, LPVOID pParam);
  virtual ~TCThread();
  
  // Static functions to create and return a TCThread 
  static TCThread* BeginThread(TC_THREADPROC pfnThreadProc, LPVOID pParam,
    int nPriority = THREAD_PRIORITY_NORMAL, UINT nStackSize = 0,
    DWORD dwCreateFlags = 0, LPSECURITY_ATTRIBUTES pSecurityAttrs = NULL);
  static TCThread* BeginMsgThread(TC_THREADPROC pfnThreadProc, LPVOID pParam,
    int nPriority = THREAD_PRIORITY_NORMAL, UINT nStackSize = 0,
    DWORD dwCreateFlags = 0, LPSECURITY_ATTRIBUTES pSecurityAttrs = NULL);

  // Called by TCBeginThread
  bool CreateThread(bool bMsgQueue = false, DWORD dwCreateFlags = 0,
    UINT nStackSize = 0, LPSECURITY_ATTRIBUTES pSecurityAttrs = NULL);

  void EndThread(UINT nExitCode, bool bDelete = true);

  // 'delete this' only if m_bAutoDelete == true
  void Delete();

// Group=Attributes
public:
  HANDLE    m_hThread;       // This thread's HANDLE
  DWORD     m_nThreadID;     // This thread's ID
  bool      m_bAutoDelete:1; // Enables 'delete this' after thread termination
  TCHandle  m_hEventExit;    // Thread proc can wait for this

  int GetThreadPriority();
  bool SetThreadPriority(int nPriority);

// Operations
public:
  DWORD SuspendThread();
  DWORD ResumeThread();
  bool PostThreadMessage(UINT message, WPARAM wParam = 0, LPARAM lParam = 0);
  DWORD SignalExitAndWait(DWORD dwWait = INFINITE);

// Operators
public:
  operator HANDLE() const;

// Implementation
public:
  // These items aren't for user consumption.
  void CommonConstruct();

  // Actual entry point for all threads of this type.
  static UINT __stdcall TCThreadEntry(void* pParam);
  static UINT __stdcall TCMsgThreadEntry(void* pParam);

// Group=Data Members
public:
  // Parameters passed to starting function.  Used internally.
  LPVOID        m_pThreadParams; 
  TC_THREADPROC m_pfnThreadProc;
  DWORD         m_dwCreateFlags;
  
  // Events to coordinate thread startup. Used internally.
  TCHandle      m_hEvent, m_hEvent2;

  // Error checking. Used internally.
  bool          m_bError:1, m_bMsgQueue:1;
};


/////////////////////////////////////////////////////////////////////////////
// Group=Attributes

/////////////////////////////////////////////////////////////////////////////
// Description: Gets the priority of the current thread.
//
// Return Value: The current thread priority level within its priority class.
// The value returned will be one of the following, listed from highest
// priority to lowest:
//
// + THREAD_PRIORITY_TIME_CRITICAL
// + THREAD_PRIORITY_HIGHEST
// + THREAD_PRIORITY_ABOVE_NORMAL
// + THREAD_PRIORITY_NORMAL
// + THREAD_PRIORITY_BELOW_NORMAL
// + THREAD_PRIORITY_LOWEST
// + THREAD_PRIORITY_IDLE 
//
// Sets the priority of the current thread.
//
// See Also: TCThread::SetThreadPriority
inline int TCThread::GetThreadPriority()                 
{
  ZAssert(NULL != m_hThread);
  return ::GetThreadPriority(m_hThread);
}

/////////////////////////////////////////////////////////////////////////////
// Description: Sets the priority of the current thread.
//
// Return Value: Nonzero if function was successful; otherwise 0.
//
// Parameters:
//   nPriority - Specifies the new thread priority level within its priority
//   class. This parameter must be one of the following values, listed from
//   highest priority to lowest:
//
// THREAD_PRIORITY_TIME_CRITICAL
//
// THREAD_PRIORITY_HIGHEST
//
// THREAD_PRIORITY_ABOVE_NORMAL
//
// THREAD_PRIORITY_NORMAL
//
// THREAD_PRIORITY_BELOW_NORMAL
//
// THREAD_PRIORITY_LOWEST
//
// THREAD_PRIORITY_IDLE
//
// This function sets the priority level of the current thread within its
// priority class. It can only be called after CreateThread successfully
// returns.
//
// See Also: TCThread::GetThreadPriority
inline bool TCThread::SetThreadPriority(int nPriority)
{
  ZAssert(NULL != m_hThread);
  return !!::SetThreadPriority(m_hThread, nPriority);
}


/////////////////////////////////////////////////////////////////////////////
// Group=Operations

/////////////////////////////////////////////////////////////////////////////
// Description: Increments a thread's suspend count.
//
// Increments the current thread's suspend count. If any thread has a suspend
// count above zero, that thread does not execute. The thread can be resumed
// by calling the ResumeThread member function.
//
// Return Value: The thread's previous suspend count if successful;
// 0xFFFFFFFF otherwise.
//
// See Also: TCThread::ResumeThread
inline DWORD TCThread::SuspendThread()
{
  ZAssert(NULL != m_hThread);
  return ::SuspendThread(m_hThread);
}

/////////////////////////////////////////////////////////////////////////////
// Description: Decrements a thread's suspend count.
//
// Called to resume execution of a thread that was suspended by the
// SuspendThread member function, or a thread created with the
// CREATE_SUSPENDED flag. The suspend count of the current thread is reduced
// by one. If the suspend count is reduced to zero, the thread resumes
// execution; otherwise the thread remains suspended.
//
// Return Value: The thread's previous suspend count if successful;
// 0xFFFFFFFF otherwise. If the return value is zero, the current thread was
// not suspended. If the return value is one, the thread was suspended, but
// is now restarted. Any return value greater than one means the thread
// remains suspended.
inline DWORD TCThread::ResumeThread()
{
  ZAssert(NULL != m_hThread);
  return ::ResumeThread(m_hThread);
}

/////////////////////////////////////////////////////////////////////////////
// Description: Posts a message to the message queue of this thread.
//
// Return Value: Nonzero if successful; otherwise 0.
//
// Parameters:
//   message - ID of the user-defined message.
//   wParam - First message parameter.
//   lParam - Second message parameter.
//
// Called to post a message to the message queue of this object. 
inline bool TCThread::PostThreadMessage(UINT message, WPARAM wParam,
  LPARAM lParam)
{
  ZAssert(NULL != m_hThread);
  ZAssert(0 != m_nThreadID);
  return !!::PostThreadMessage(m_nThreadID, message, wParam, lParam);
}


inline DWORD TCThread::SignalExitAndWait(DWORD dwWait)
{
  HANDLE hth = m_hThread;
  ::SetEvent(m_hEventExit);
  if (m_bMsgQueue)
    PostThreadMessage(WM_QUIT, 0, 0);
  return WaitForSingleObject(hth, dwWait);
}


/////////////////////////////////////////////////////////////////////////////
// Group=Operators

inline TCThread::operator HANDLE() const
{
  return (NULL == this) ? NULL : m_hThread;
}


/////////////////////////////////////////////////////////////////////////////

#endif // __TCThread_h__
