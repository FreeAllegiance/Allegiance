#pragma once

/////////////////////////////////////////////////////////////////////////////
// AutoCriticalSection.h | Declaration of the TCAutoCriticalSection class.
//


/////////////////////////////////////////////////////////////////////////////
// TCAutoCriticalSection provides methods for obtaining and releasing
// ownership of a Win32 critical section object. TCAutoCriticalSection
// automatically initializes critical section objects in the constructor, and
// automatically deletes the critical section in the destructor.
class TCAutoCriticalSection
{
// Construction / Destruction
public:
  TCAutoCriticalSection();
  ~TCAutoCriticalSection();

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
// See Also: TCAutoCriticalSection::Lock, TCAutoCriticalSection::destructor
inline TCAutoCriticalSection::TCAutoCriticalSection()
{
  InitializeCriticalSection(&m_sec);
}

/////////////////////////////////////////////////////////////////////////////
// Description: Destructor
//
// The destructor. calls DeleteCriticalSection, which releases all system
// resources used by the critical section object.
//
// See Also: TCAutoCriticalSection::Unlock,
// TCAutoCriticalSection::constructor
inline TCAutoCriticalSection::~TCAutoCriticalSection()
{
  DeleteCriticalSection(&m_sec);
}


/////////////////////////////////////////////////////////////////////////////
// Group=Operations

/////////////////////////////////////////////////////////////////////////////
// Obtains ownership of the critical section object.
//
// See Also: TCAutoCriticalSection::Unlock, TCAutoCriticalSection::destructor
inline void TCAutoCriticalSection::Lock()
{
  EnterCriticalSection(&m_sec);
}

/////////////////////////////////////////////////////////////////////////////
// Releases ownership of the critical section object.
//
// See Also: TCAutoCriticalSection::Lock, TCAutoCriticalSection::constructor
inline void TCAutoCriticalSection::Unlock()
{
  LeaveCriticalSection(&m_sec);
}


/////////////////////////////////////////////////////////////////////////////