/////////////////////////////////////////////////////////////////////////////
// PigEvent.cpp : Implementation of the CPigEvent class.

#include "pch.h"
#include <TCLib.h>

#include "PigEventOwner.h"
#include "PigEvent.h"


/////////////////////////////////////////////////////////////////////////////
// CPigEvent


/////////////////////////////////////////////////////////////////////////////
// Construction / Destruction

HRESULT CPigEvent::Init(CPigEventOwner* pOwner)
{
  // Ensure that we have a unique event id
  GetEventID();

  // Save the specified owner
  ZAssert(pOwner);
  m_pOwner = pOwner;

  // Add ourself to our owner's collection
  m_pOwner->AddEvent(this);

  // Indicate success
  return S_OK;
}

void CPigEvent::Term()
{
  if (m_pOwner)
  {
    // Remove ourself from our owner's collection
    m_pOwner->RemoveEvent(this);

    // Detach from our owner
    m_pOwner = NULL;
  }
}

