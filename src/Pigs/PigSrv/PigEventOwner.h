/////////////////////////////////////////////////////////////////////////////
// PigEventOwner.h : Declaration of the CPigEventOwner class.

#pragma once

#include "PigSrv.h"


/////////////////////////////////////////////////////////////////////////////
// Forward Declarations

class CPigEvent;
class ImodelIGC;


/////////////////////////////////////////////////////////////////////////////
// 
class ATL_NO_VTABLE CPigEventOwner
{
// Construction
public:
  CPigEventOwner();

// Attributes
public:
  long GetNextEventNumber();

// Operations
public:
  void AddEvent(CPigEvent* pEvent);
  void RemoveEvent(CPigEvent* pEvent);
  void RemoveEvents();
  void PulseEvents();
  void ModelTerminated(ImodelIGC* pModelIGC);

// Overrides
public:
  virtual void LockT() = 0;
  virtual void UnlockT() = 0;
  virtual void OnExpiration(CPigEvent* pEvent);
  virtual void OnInterruption(CPigEvent* pEvent);
  virtual STDMETHODIMP get_Properties(IDictionary** ppProperties);

// Types
protected:
  typedef std::vector<CPigEvent*> XEvents;
  typedef XEvents::iterator       XEventIt;

// Data Members
protected:
  XEvents m_Events;
  long    m_nNextEventNumber;
};


/////////////////////////////////////////////////////////////////////////////
// Construction

inline CPigEventOwner::CPigEventOwner() :
  m_nNextEventNumber(0)
{
}


/////////////////////////////////////////////////////////////////////////////
// 
template <class T>
class ATL_NO_VTABLE CPigEventOwnerImpl :
  public CPigEventOwner
{
// Overrides
public:
  virtual void LockT();
  virtual void UnlockT();

// Types
public:
  typedef CPigEventOwnerImpl<T> CPigEventOwnerImplBase;
};


/////////////////////////////////////////////////////////////////////////////
// Overrides

template <class T>
void CPigEventOwnerImpl<T>::LockT  ()
{
  static_cast<T*>(this)->Lock();
}

template <class T>
void CPigEventOwnerImpl<T>::UnlockT()
{
  static_cast<T*>(this)->Unlock();
}


/////////////////////////////////////////////////////////////////////////////