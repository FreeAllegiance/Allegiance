#if _MSC_VER > 1000
  #pragma once
#endif // _MSC_VER > 1000

#ifndef __AutoMutex_h__
#define __AutoMutex_h__


/////////////////////////////////////////////////////////////////////////////
// AutoMutex.h | Declaration of the TCAutoMutex class.
//

#include "AutoHandle.h"


/////////////////////////////////////////////////////////////////////////////
// TCAutoMutex provides methods for obtaining and releasing
// ownership of a Win32 mutex object. TCAutoMutex automatically initializes
// the mutex object in the constructor, and automatically deletes it in the
// destructor.
//
// See Also: TCAutoCriticalSection
class TCAutoMutex
{
// Construction / Destruction
public:
  TCAutoMutex();
  virtual ~TCAutoMutex();

// Operations
public:
  void Lock();
  DWORD Lock(DWORD dwMilliseconds);
  void Unlock();

// Data Members
protected:
  TCHandle m_hmtx;  // A Win32 mutex object.
};


/////////////////////////////////////////////////////////////////////////////
// Group=Construction / Destruction

/////////////////////////////////////////////////////////////////////////////
// Description: Constructor.
//
// The constructor. Calls the Win32 function InitializeCriticalSection,
// which initializes the mutex object contained in the m_sec data
// member.
//
// See Also: TCAutoMutex::Lock, TCAutoMutex::destructor
inline TCAutoMutex::TCAutoMutex()
{
  m_hmtx = CreateMutex(NULL, FALSE, NULL);
}

inline TCAutoMutex::~TCAutoMutex()
{
}


/////////////////////////////////////////////////////////////////////////////
// Group=Operations

/////////////////////////////////////////////////////////////////////////////
// Obtains ownership of the mutex object.
//
// Parameters:
//   dwMilliseconds - Specifies the time-out interval, in milliseconds. The
// function returns if the interval elapses, even if the object's state is
// nonsignaled. If /dwMilliseconds/ is zero, the function tests the object's
// state and returns immediately. If /dwMilliseconds/ is INFINITE or not
// specified, the function's time-out interval never elapses.
//
// Return Value: The parameter form of this method returns a DWORD:
//
// If the function fails, the return value is *WAIT_FAILED*.
//
// The return value on success is one of the following values: 
//
//   WAIT_ABANDONED - The specified object is a mutex object that was not
//     released by the thread that owned the mutex object before the owning
//     thread terminated. Ownership of the mutex object is granted to the
//     calling thread, and the mutex is set to nonsignaled.
//   WAIT_OBJECT_0 - The state of the specified object is signaled. 
//   WAIT_TIMEOUT - The time-out interval elapsed, and the object's state is
//     nonsignaled. 
//
// See Also: TCAutoMutex::Unlock
inline void TCAutoMutex::Lock()
{
  Lock(INFINITE);
}

inline DWORD TCAutoMutex::Lock(DWORD dwMilliseconds)
{
  ZAssert(!m_hmtx.IsNull());
  return WaitForSingleObject(m_hmtx, dwMilliseconds);
}


/////////////////////////////////////////////////////////////////////////////
// Releases ownership of the mutex object.
//
// See Also: TCAutoMutex::Lock
inline void TCAutoMutex::Unlock()
{
  ReleaseMutex(m_hmtx);
}


/////////////////////////////////////////////////////////////////////////////

#endif // !__AutoMutex_h__

