/////////////////////////////////////////////////////////////////////////////
// PigEventOwner.cpp : Implementation of the CPigEventOwner class.

#include "pch.h"
#include <TCLib.h>

#include "PigEvent.h"
#include "PigEventOwner.h"


/////////////////////////////////////////////////////////////////////////////
// CPigEventOwner


/////////////////////////////////////////////////////////////////////////////
// Attributes

long CPigEventOwner::GetNextEventNumber()
{
  LockT();
  long nNext = m_nNextEventNumber++;
  UnlockT();
  return nNext;
}


/////////////////////////////////////////////////////////////////////////////
// Operations

void CPigEventOwner::AddEvent(CPigEvent* pEvent)
{
  LockT();
  m_Events.push_back(pEvent);
  pEvent->AddRef();
  UnlockT();
}

void CPigEventOwner::RemoveEvents()
{
  // Remove all timer objects
  LockT();
  XEvents events(m_Events);
  for (XEventIt it = events.begin(); it != events.end(); ++it)
    (*it)->Term();
  UnlockT();
}

void CPigEventOwner::RemoveEvent(CPigEvent* pEvent)
{
  // Find the specified event in our collection
  LockT();
  XEventIt it = std::find(m_Events.begin(), m_Events.end(), pEvent);
  ZAssert(m_Events.end() != it);

  // Remove the event from the collection
  m_Events.erase(it);

  // Release our ref count on the event
  pEvent->Release();
  UnlockT();
}

void CPigEventOwner::PulseEvents()
{
  // Copy the collection of events (while locked)
  XEvents events(m_Events);
  {
    LockT();
    for (XEventIt it = events.begin(); it != events.end(); ++it)
      (*it)->AddRef();
    UnlockT();
  }

  // Iterate through each event
  for (XEventIt it = events.begin(); it != events.end(); ++it)
  {
    // Call the OnCompletion method if this event has expired
    bool bExpired = (*it)->IsExpired();
    if (bExpired)
      OnExpiration(*it);

    // If Pulse returns true, we will terminate the event
    if ((*it)->Pulse(bExpired))
      (*it)->Term();

    // Release the event (possibly destroying it)
    (*it)->Release();
  }
}

void CPigEventOwner::ModelTerminated(ImodelIGC* pModelIGC)
{
  // Copy the collection of events (while locked)
  XEvents events(m_Events);
  {
    LockT();
    for (XEventIt it = events.begin(); it != events.end(); ++it)
      (*it)->AddRef();
    UnlockT();
  }

  // Iterate through each event, calling the ModelTerminated method
  for (XEventIt it = events.begin(); it != events.end(); ++it)
  {
    // Call the OnInterruption method if this event has been interrupted
    bool bExpired = (*it)->ModelTerminated(pModelIGC);
    if (bExpired)
      OnInterruption(*it);

    // If Pulse returns true, we will terminate the event
    if ((*it)->Pulse(bExpired))
      (*it)->Term();

    // Release the event (possibly destroying it)
    (*it)->Release();
  }
}


/////////////////////////////////////////////////////////////////////////////
// Overrides

void CPigEventOwner::OnExpiration(CPigEvent* pEvent)
{
}

void CPigEventOwner::OnInterruption(CPigEvent* pEvent)
{
}

STDMETHODIMP CPigEventOwner::get_Properties(IDictionary** ppProperties)
{
  *ppProperties = NULL;
  return S_FALSE;
}


