#ifndef __WindowThreadImpl_h__
#define __WindowThreadImpl_h__

#if _MSC_VER > 1000
  #pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable: 4786)
#include <map>
#include <malloc.h>

#include "WindowThreadBase.h"


/////////////////////////////////////////////////////////////////////////////
// Description: Implements a thread with a message loop that can create
// windows owned by the thread.
//
// Parameters:
//   T - Your class, derived from TCWindowThreadImpl
//
// See Also: TCWindowThread, TCWindowThreadBase
//
template <class T>
class TCWindowThreadImpl : public TCWindowThreadBase
{
// Group=Types
protected:
  typedef TCWindowThreadImpl<T> TCWindowThreadImplBase;

// Group=Construction / Destruction
public:
  TCWindowThreadImpl(DWORD dwCoInit =
    COINIT_MULTITHREADED | COINIT_DISABLE_OLE1DDE);
  ~TCWindowThreadImpl();

// Group=Attributes
public:
  virtual TCThread* GetOrCreateThread();

// Group=Implementation
protected:
  virtual void InternalCreateWindow(XArgs& args);
  virtual void InternalDestroyWindow(XArgs& args);
  static unsigned CALLBACK ThreadProc(void* pvParam);
};


/////////////////////////////////////////////////////////////////////////////
// Group=Construction / Destruction

template <class T>
inline TCWindowThreadImpl<T>::TCWindowThreadImpl(DWORD dwCoInit) :
  TCWindowThreadBase(dwCoInit)
{
}

template <class T>
inline TCWindowThreadImpl<T>::~TCWindowThreadImpl()
{
  // Force all windows to be destroyed and the thread to be shut down
  ShutdownWindowThread();
}


/////////////////////////////////////////////////////////////////////////////
// Group=Attributes

template <class T>
TCThread* TCWindowThreadImpl<T>::GetOrCreateThread()
{
  // Create the thread, if not already created
  CLock lock(this);
  if (!m_pth)
  {
    // Create the thread parameters structure
    TCHandle hevt = CreateEvent(NULL, FALSE, FALSE, NULL);
    XThreadArgs args = {static_cast<T*>(this), hevt};
    m_pth = TCThread::BeginThread(ThreadProc, &args);

    // Wait for the thread to start its message queue
    WaitForSingleObject(hevt, INFINITE);
  }
  return m_pth;
}


/////////////////////////////////////////////////////////////////////////////
// Group=Implementation

template <class T>
void TCWindowThreadImpl<T>::InternalCreateWindow(
  TCWindowThreadBase::XArgs& args)
{
  // Get the derived-class pointer
  T* pThis = static_cast<T*>(this);

  // Attempt to create the window as specified
  args.hwnd = pThis->DoCreateWindowOnThread(args);
  args.dwLastError = args.hwnd ? 0 : ::GetLastError();
  AddWindow(args);
  SubclassWindow(args.hwnd);
}

template <class T>
void TCWindowThreadImpl<T>::InternalDestroyWindow(
  TCWindowThreadBase::XArgs& args)
{
  // Get the derived-class pointer
  T* pThis = static_cast<T*>(this);

  // Call the method of the derived class
  if (!pThis->DoDestroyWindowOnThread(args.hwnd))
    args.hwnd = NULL;
  args.dwLastError = args.hwnd ? 0 : ::GetLastError();
}

template <class T>
unsigned CALLBACK TCWindowThreadImpl<T>::ThreadProc(void* pvParam)
{
  // Reinterpret the parameter as an XThreadArgs*
  XThreadArgs* pArgs = reinterpret_cast<XThreadArgs*>(pvParam);
  ZAssert(pArgs);

  // Get the instance pointer of the derived class
  T* pThis = reinterpret_cast<T*>(pArgs->pvThis);

  // Initialize the thread and delegate to the non-static method
  if (pThis->DoInitThread())
  {
    // Create the message queue
    MSG msg;
    PeekMessage(&msg, NULL, WM_USER, WM_USER, PM_NOREMOVE);

    // Signal the thread creator that the message queue is ready
    ::SetEvent(pArgs->hevt);

    // Service the message queue
    pThis->MessageLoop();
  }

  // Lock the object until the thread exits
  CLock lock(pThis);

  // Clear the thread pointer data member
  pThis->m_pth = NULL;

  // Uninitialize the thread
  pThis->DoExitThread();

  // Obligatory return code
  return 0;
}


/////////////////////////////////////////////////////////////////////////////

#endif // !__WindowThreadImpl_h__
