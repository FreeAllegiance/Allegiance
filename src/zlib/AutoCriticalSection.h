// BT - mutex rollback 9/24

#ifndef __ZAutoCriticalSection_h__
#define __ZAutoCriticalSection_h__

#include <Windows.h>

/////////////////////////////////////////////////////////////////////////////
// ZAutoCriticalSection.h | Declaration of the ZAutoCriticalSection class.
//


/////////////////////////////////////////////////////////////////////////////
// ZAutoCriticalSection provides methods for obtaining and releasing
// ownership of a Win32 critical section object. ZAutoCriticalSection
// automatically initializes critical section objects in the constructor, and
// automatically deletes the critical section in the destructor.
class ZAutoCriticalSection
{
// Construction / Destruction
public:
  ZAutoCriticalSection();
  ~ZAutoCriticalSection();

// Operations
public:
  void Lock();
  void Unlock();

// Data Members
protected:
  CRITICAL_SECTION m_sec;  // A Win32 CRITICAL_SECTION object.
};


/////////////////////////////////////////////////////////////////////////////
// Group=Construction / Destruction

/////////////////////////////////////////////////////////////////////////////
// Description: Constructor.
//
// The constructor. Calls the Win32 function InitializeCriticalSection,
// which initializes the critical section object contained in the m_sec data
// member.
//
// See Also: ZAutoCriticalSection::Lock, ZAutoCriticalSection::destructor
inline ZAutoCriticalSection::ZAutoCriticalSection()
{
  InitializeCriticalSection(&m_sec);
}

/////////////////////////////////////////////////////////////////////////////
// Description: Destructor
//
// The destructor. calls DeleteCriticalSection, which releases all system
// resources used by the critical section object.
//
// See Also: ZAutoCriticalSection::Unlock,
// ZAutoCriticalSection::constructor
inline ZAutoCriticalSection::~ZAutoCriticalSection()
{
  DeleteCriticalSection(&m_sec);
}


/////////////////////////////////////////////////////////////////////////////
// Group=Operations

/////////////////////////////////////////////////////////////////////////////
// Obtains ownership of the critical section object.
//
// See Also: ZAutoCriticalSection::Unlock, ZAutoCriticalSection::destructor
inline void ZAutoCriticalSection::Lock()
{
  EnterCriticalSection(&m_sec);
}

/////////////////////////////////////////////////////////////////////////////
// Releases ownership of the critical section object.
//
// See Also: ZAutoCriticalSection::Lock, ZAutoCriticalSection::constructor
inline void ZAutoCriticalSection::Unlock()
{
  LeaveCriticalSection(&m_sec);
}


/////////////////////////////////////////////////////////////////////////////

#endif // !__ZAutoCriticalSection_h__

