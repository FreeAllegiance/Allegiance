/////////////////////////////////////////////////////////////////////////////
// AGCGlobal.cpp : Implementation of the CAGCGlobal class.
//

#include "pch.h"
#include "AGCGlobal.h"
#include "AGCEvent.h"
#include "AGCEventDef.h"
#include "AGCEventData.h"
#include "AGCEventThread.h"
#include "AGCEventIDRange.h"
#include "AGCEventIDRanges.h"
#include "AGCVector.h"
#include "AGCOrientation.h"
#include "AGCWinApp.h"


/////////////////////////////////////////////////////////////////////////////
// CAGCGlobal

TC_OBJECT_EXTERN_IMPL(CAGCGlobal)


/////////////////////////////////////////////////////////////////////////////
// Construction / Destruction

void CAGCGlobal::FinalRelease()
{
  // Ensure that we're terminated
  Terminate();
}


/////////////////////////////////////////////////////////////////////////////
// Implementation

const IID* CAGCGlobal::InterpretIID(const void* pvIgc, REFIID iid)
{
  // Only interpret if pvIgc is non-NULL
  if (pvIgc)
  {
    // Determine if iid is IID_IAGCModel
    if (IID_IAGCModel == iid)
    {
      // Determine the corresponding IID from the model's type
      const ImodelIGC* pModel = reinterpret_cast<const ImodelIGC*>(pvIgc);
      switch (pModel->GetObjectType())
      {
        case OT_ship:        return &IID_IAGCShip    ;
        case OT_station:     return &IID_IAGCStation ;
        case OT_missile:     return &IID_IAGCModel   ;  // TODO: Implement specific object type in AGC
        case OT_mine:        return &IID_IAGCModel   ;  // TODO: Implement specific object type in AGC
        case OT_probe:       return &IID_IAGCProbe   ;
        case OT_asteroid:    return &IID_IAGCAsteroid;
        case OT_projectile:  return &IID_IAGCModel   ;  // TODO: Implement specific object type in AGC
        case OT_warp:        return &IID_IAGCAleph   ;
        case OT_treasure:    return &IID_IAGCModel   ;  // TODO: Implement specific object type in AGC
        case OT_buoy:        return &IID_IAGCModel   ;  // TODO: Implement specific object type in AGC
        case OT_chaff:       return &IID_IAGCModel   ;  // TODO: Implement specific object type in AGC
        default:
          ZError("CAGCGlobal::InterpretIID(): non-model type specified\n");
      }
    }
  }

  // No interpretation
  return &iid;
}


/////////////////////////////////////////////////////////////////////////////
// IAGCGlobal Interface Methods

STDMETHODIMP_(void) CAGCGlobal::RegisterObjectCreator(REFIID riid,
  PFNAGCCreator pfnCreator)
{
  ZAssert(pfnCreator);

  // Lock the object for the duration of this method
  XLock lock(this);

  // Insert the association into the map
  std::pair<XAGCCreatorMapIt, bool> result =
    m_mapCreator.insert(std::make_pair(riid, pfnCreator));
  if (!result.second)
    result.first->second = pfnCreator;
}

STDMETHODIMP_(void) CAGCGlobal::RevokeObjectCreator(REFIID riid)
{
  ZAssert(!"NOT IMPLEMENTED!");
}

STDMETHODIMP CAGCGlobal::GetAGCObject(const void* pvIgc, REFIID riid,
  void** ppUnk)
{
  // Lock the object for the duration of this method
  XLock lock(this);

  // Interpret the specified IID
  const IID* piid = InterpretIID(pvIgc, riid);

  // Find the specified Igc pointer in the map
  XIgcToAGCMapIt it = m_map.find(const_cast<void*>(pvIgc));
  if (m_map.end() != it)
  {
    // Simply QueryInterface the associated AGC pointer
    return it->second->QueryInterface(*piid, ppUnk);
  }

  // Specified object does not exist in the map, create a wrapper for it

  // Find the specified IID in the AGC object creator map
  XAGCCreatorMapIt itCreator = m_mapCreator.find(*piid);
  if (m_mapCreator.end() == itCreator)
  {
    ZAssert(false);
    return E_UNEXPECTED;
  }

  // Call the creator function to create an AGC wrapper for the Igc object
  PFNAGCCreator pfn = itCreator->second;
  ZAssert(pfn);
  RETURN_FAILED((*pfn)(const_cast<void*>(pvIgc), *piid, ppUnk));

  // Add the AGC wrapper to the map
  AddAGCObject(const_cast<void*>(pvIgc), IUnknownPtr((IUnknown*)*ppUnk));

  // Indicate success
  return S_OK;
}

STDMETHODIMP_(void) CAGCGlobal::AddAGCObject(const void* pvIgc,
  IUnknown* pUnkAGC)
{
  // Lock the object for the duration of this method
  XLock lock(this);

  // Ensure that the specified Igc pointer is not already in the map
  ZAssert(m_map.end() == m_map.find(const_cast<void*>(pvIgc)));

  // Ensure that the specified AGC pointer supports IAGCPrivate
  ZAssert(NULL != IAGCPrivatePtr(pUnkAGC));

  // Insert the pair into the map
  #ifdef _DEBUG
    std::pair<XIgcToAGCMapIt, bool> result =
  #endif
    m_map.insert(std::make_pair(const_cast<void*>(pvIgc), pUnkAGC));
  ZAssert(result.second);
}


STDMETHODIMP_(void) CAGCGlobal::RemoveAGCObject(const void* pvIgc,
  boolean bForceDestruct)
{
  // Lock the object for the duration of this method
  XLock lock(this);

  // Find the specified Igc pointer in the map
  XIgcToAGCMapIt it = m_map.find(pvIgc);

  // Remove the association from the map
  if (m_map.end() != it)
  {
    // Store and AddRef the associated AGC interface
    IUnknownPtr spUnk(it->second);

    // Remove the association from the map
    m_map.erase(it);

    // Disconnect the object's proxy, if specified
    if (bForceDestruct)
      ::CoDisconnectObject(spUnk, 0);

    // spUnk Release'd upon leaving scope
  }
}


/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP_(void) CAGCGlobal::SetEventSinksAreGITCookies(
  boolean bEventSinksAreGITCookies)
{
  XLock lock(this);

  // Can't change mode when event sinks are already registered
  #ifdef _DEBUG
    if (m_RegEvents.size())
      ZAssert(!m_bEventSinksAreGITCookies == !bEventSinksAreGITCookies);
  #endif // _DEBUG

  m_bEventSinksAreGITCookies = !!bEventSinksAreGITCookies;
}


/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP_(boolean) CAGCGlobal::GetEventSinksAreGITCookies()
{
  XLock lock(this);
  return m_bEventSinksAreGITCookies;
}


/////////////////////////////////////////////////////////////////////////////
// Description: Registers an event sink for the specified event.
//
// Remarks:
// If or when the specified event is fired, the specified event sink will be
// notified.
//
// A uniqueID of  causes global firing of an event, that is, an event will
// fire without being limited to a single IGC object.
//
STDMETHODIMP_(void) CAGCGlobal::RegisterEvent(AGCEventID eventID,
  AGCUniqueID uniqueID, IAGCEventSink* pEventSink)
{
  // Verify that the specified event sink is as required
  #ifdef _DEBUG
  {
    if (GetEventSinksAreGITCookies())
    {
      // This will ZAssert if pEventSink is not a cookie in the GIT
      IAGCEventSinkPtr spEventSink;
      GetInterfaceFromGlobal(reinterpret_cast<DWORD>(pEventSink),
        IID_IAGCEventSink, (void**)&spEventSink);
    }
  }
  #endif // _DEBUG

  // Find the array for the specified id
  XLock lock(this);
  TCINT64 nKey = MakeKey(eventID, uniqueID);
  XRegEventsIt itID = m_RegEvents.find(nKey);
  if (m_RegEvents.end() != itID)
  {
    // Find the specified event sink in the array
    XEventSinks& vecEvents = itID->second;
    for (XEventSinksIt it = vecEvents.begin(); it != vecEvents.end(); ++it)
      if (*it == pEventSink)
        return;
  }
  else
  {
    // Insert a new array into the map
    itID = m_RegEvents.insert(std::make_pair(nKey, XEventSinks())).first;
  }

  // Add the event sink to the array for this event
  XEventSinks& vecEvents = itID->second;
  vecEvents.push_back(pEventSink);
}


/////////////////////////////////////////////////////////////////////////////
// Description: Revokes an event sink for the specified event. 
//
// Return Value:
// *true* if the specified event sink was registered for the specified event
// prior to the method being called. Otherwise, *false*.
//
STDMETHODIMP_(boolean) CAGCGlobal::RevokeEvent(AGCEventID eventID,
  AGCUniqueID uniqueID, IAGCEventSink* pEventSink)
{
  // Verify that the specified event sink is as required
  #ifdef _DEBUG
  {
    if (GetEventSinksAreGITCookies())
    {
      // This will ZAssert if pEventSink is not a cookie in the GIT
      IAGCEventSinkPtr spEventSink;
      GetInterfaceFromGlobal(reinterpret_cast<DWORD>(pEventSink),
        IID_IAGCEventSink, (void**)&spEventSink);
    }
  }
  #endif // _DEBUG

  // Find the array for the specified id
  XLock lock(this);
  XRegEventsIt itID = m_RegEvents.find(MakeKey(eventID, uniqueID));
  if (m_RegEvents.end() != itID)
  {
    // Find the specified object pointer in the array
    XEventSinks& vecEvents = itID->second;
    for (XEventSinksIt it = vecEvents.begin(); it != vecEvents.end(); ++it)
    {
      if (*it == pEventSink)
      {
        vecEvents.erase(it);
        if (vecEvents.empty())
          m_RegEvents.erase(itID);
        return true;
      }
    }
  }
  // Specified object was not previously registered
  return false;
}


/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP_(void) CAGCGlobal::RegisterEventRanges(
  IAGCEventIDRanges* pRanges, AGCUniqueID uniqueID,
  IAGCEventSink* pEventSink)
{
  ZAssert(pRanges);

  // Register all event ID's that are not group ID's
  XLock lock(this);
  for (const CAGCEventDef::XEventDef* it = CAGCEventDef::begin();
    CAGCEventDef::end() != it; ++it)
  {
    if (0 == it->m_nIndent)
    {
      range<AGCEventID> theRange(it->m_id, it->m_id);
      if (m_EventIDRanges.find(theRange) != m_EventIDRanges.end())
      {
        VARIANT_BOOL bIntersects;
        ZSucceeded(pRanges->get_IntersectsWithValue(it->m_id, &bIntersects));
        if (bIntersects)
          RegisterEvent(it->m_id, uniqueID, pEventSink);
      }
    }
  }
}

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP_(void) CAGCGlobal::RevokeEventRanges(IAGCEventIDRanges* pRanges,
  AGCUniqueID uniqueID, IAGCEventSink* pEventSink)
{
  ZAssert(pRanges);

  // Revoke all event ID's that are not group ID's
  XLock lock(this);
  for (const CAGCEventDef::XEventDef* it = CAGCEventDef::begin();
    CAGCEventDef::end() != it; ++it)
  {
    if (0 == it->m_nIndent)
    {
      range<AGCEventID> theRange(it->m_id, it->m_id);
      if (m_EventIDRanges.find(theRange) != m_EventIDRanges.end())
      {
        VARIANT_BOOL bIntersects;
        ZSucceeded(pRanges->get_IntersectsWithValue(it->m_id, &bIntersects));
        if (bIntersects)
          RevokeEvent(it->m_id, uniqueID, pEventSink);
      }
    }
  }
}


/////////////////////////////////////////////////////////////////////////////
// Description: Revokes an event sink for all its events.
//
// Return Value: The count of events unregistered for the specified event
// sink.
//
STDMETHODIMP_(int) CAGCGlobal::RevokeAllEvents(IAGCEventSink* pEventSink)
{
  // Verify that the specified event sink is as required
  #ifdef _DEBUG
  {
    if (GetEventSinksAreGITCookies())
    {
      // This will ZAssert if pEventSink is not a cookie in the GIT
      IAGCEventSinkPtr spEventSink;
      GetInterfaceFromGlobal(reinterpret_cast<DWORD>(pEventSink),
        IID_IAGCEventSink, (void**)&spEventSink);
    }
  }
  #endif // _DEBUG

  int cRemoved = 0;
  XLock lock(this);
  for (XRegEventsIt itID = m_RegEvents.begin(); itID != m_RegEvents.end(); ++itID)
  {
    XEventSinks& vecEvents = itID->second;
    for (XEventSinksIt it = vecEvents.begin(); it != vecEvents.end(); ++it)
    {
      if (*it == pEventSink)
      {
        vecEvents.erase(it);
        ++cRemoved;
        break;
      }
    }
  }
  return cRemoved;
}


/////////////////////////////////////////////////////////////////////////////
// Description: Registers an event sink for all its events.
//
// Return Value: The count of events registered for the specified event
// sink.
//
STDMETHODIMP_(int) CAGCGlobal::RegisterAllEvents(IAGCEventSink* pEventSink)
{
  // Verify that the specified event sink is as required
  #ifdef _DEBUG
  {
    if (GetEventSinksAreGITCookies())
    {
      // This will ZAssert if pEventSink is not a cookie in the GIT
      IAGCEventSinkPtr spEventSink;
      GetInterfaceFromGlobal(reinterpret_cast<DWORD>(pEventSink),
        IID_IAGCEventSink, (void**)&spEventSink);
    }
  }
  #endif // _DEBUG

  // Register all event ID's that are not group ID's
  XLock lock(this);
  int cEventIDs = 0;
  for (const CAGCEventDef::XEventDef* it = CAGCEventDef::begin();
    CAGCEventDef::end() != it; ++it)
  {
    if (0 == it->m_nIndent)
    {
      range<AGCEventID> theRange(it->m_id, it->m_id);
      if (m_EventIDRanges.find(theRange) != m_EventIDRanges.end())
      {
        RegisterEvent(it->m_id, AGC_Any_Objects, pEventSink);
        ++cEventIDs;
      }
    }
  }

  // Return the number of event ID's registered
  return cEventIDs;
}


/////////////////////////////////////////////////////////////////////////////
// Description: Returns true if someone wants to know about an event
//              Use this to see if you should fire an event.
//
// Note: Calling this method is optional since the TriggerEvent method will
// make this check if NULL is specified as its hListeners parameter. This
// method is used for cases when other processing may be involved with
// sending an event.
//
// Parameters:
//   idEvent - The event id for which to lookup the list of listeners.
//   idSubject - The subject for which the event represents an action being
// taken. This is optional and should be set to -1 if no such object is
// appropriate.
//   idObject1 - An additional object which is affected by the event. This is
// optional and should be set to -1 if not such object is appropriate.
//   idObject2 - An additional object which is affected by the event. This is
// optional and should be set to -1 if not such object is appropriate.
//
// Return Value: NULL if no event sinks are registered for the specified
// event and subject/objects. Otherwise a non-NULL handle that contains an
// internal list of event sinks registerd for the specified event and
// subject/objects. This handle should be passed to TriggerEvent to fire the
// event to those sinks. If the caller decides to not fire the events, it
// should pass this handle to FreeListeners.
//
STDMETHODIMP_(HAGCLISTENERS) CAGCGlobal::EventListeners(AGCEventID idEvent,
  AGCUniqueID idSubject, AGCUniqueID idObject1, AGCUniqueID idObject2)
{
  XLock lock(this);
  XEventSinks* pSinks = NULL;

  //
  // A key with the uniqueID component as AGC_Any_Objects causes stands for
  // global firing of an event, that is, an event will fire without being
  // limited to a single IGC object. So first, we see if any global event
  // firing keys are in the map.
  //
  XRegEventsIt itID_AllObjects = m_RegEvents.find(MakeKey(idEvent, AGC_Any_Objects));
  if (itID_AllObjects != m_RegEvents.end())
  {
    XEventSinksIt itBegin = itID_AllObjects->second.begin();
    XEventSinksIt itEnd   = itID_AllObjects->second.end();
    if (itEnd != itBegin)
    {
      pSinks = new XEventSinks(itBegin, itEnd);
    }
  }

  if (-1 != idSubject)
  {
    XRegEventsIt itID1 = m_RegEvents.find(MakeKey(idEvent, idSubject));
    if (itID1 != m_RegEvents.end())
    {
      XEventSinksIt itBegin = itID_AllObjects->second.begin();
      XEventSinksIt itEnd   = itID_AllObjects->second.end();
      if (itEnd != itBegin)
      {
        if (!pSinks)
          pSinks = new XEventSinks(itBegin, itEnd);
        else
          pSinks->insert(pSinks->end(), itBegin, itEnd);
      }
    }
  }

  if (-1 != idObject1)
  {
    XRegEventsIt itID1 = m_RegEvents.find(MakeKey(idEvent, idObject1));
    if (itID1 != m_RegEvents.end())
    {
      XEventSinksIt itBegin = itID_AllObjects->second.begin();
      XEventSinksIt itEnd   = itID_AllObjects->second.end();
      if (itEnd != itBegin)
      {
        if (!pSinks)
          pSinks = new XEventSinks(itBegin, itEnd);
        else
          pSinks->insert(pSinks->end(), itBegin, itEnd);
      }
    }
  }

  if (-1 != idObject2)
  {
    XRegEventsIt itID1 = m_RegEvents.find(MakeKey(idEvent, idObject2));
    if (itID1 != m_RegEvents.end())
    {
      XEventSinksIt itBegin = itID_AllObjects->second.begin();
      XEventSinksIt itEnd   = itID_AllObjects->second.end();
      if (itEnd != itBegin)
      {
        if (!pSinks)
          pSinks = new XEventSinks(itBegin, itEnd);
        else
          pSinks->insert(pSinks->end(), itBegin, itEnd);
      }
    }
  }

  return pSinks;
}


/////////////////////////////////////////////////////////////////////////////
// Description: Notifies registered event sinks that the specified event
// occurred
//
STDMETHODIMP_(void) CAGCGlobal::TriggerEvent(HAGCLISTENERS hListeners,
  AGCEventID idEvent, LPCSTR pszContext, LPCOLESTR pszSubject,
  AGCUniqueID idSubject, AGCUniqueID idObject1, AGCUniqueID idObject2,
  long cArgTriplets, void* pvArgs)
{
  XLock lock(this);

  // Create the event thread, if not already created
  // Note: I've put this outside of the timer, since it only happens once.
  if (!m_pthEvents)
    m_pthEvents = new CAGCEventThread(this);
  ZAssert(m_pthEvents);

  // Initialize the timer
  static CTempTimer timerTriggerEvent("in TriggerEvent", .01f);

  // Start the timer
  timerTriggerEvent.Start();

  // Get the event sinks for this event/subject/objects, if not specified
  if (!hListeners)
  {
    hListeners = EventListeners(idEvent, idSubject, idObject1, idObject2);
    if (!hListeners)
      goto stopTimer;
  }

  // Create an AGCEventData object
  {
    CAGCEventData data(idEvent, pszContext, pszSubject, idSubject,
      cArgTriplets, reinterpret_cast<va_list>(pvArgs));

    // Queue the event to the event thread
    m_pthEvents->QueueEvent(hListeners, data);
  }

  // Stop the timer
stopTimer:
  timerTriggerEvent.Stop();
}


/////////////////////////////////////////////////////////////////////////////
// Description: Notifies registered event sinks that the specified event
// occurred
//
STDMETHODIMP_(void) CAGCGlobal::TriggerEventSynchronous(HAGCLISTENERS hListeners,
  AGCEventID idEvent, LPCSTR pszContext, LPCOLESTR pszSubject,
  AGCUniqueID idSubject, AGCUniqueID idObject1, AGCUniqueID idObject2,
  long cArgTriplets, void* pvArgs)
{
  XLock lock(this);

  // Create the event thread, if not already created
  if (!m_pthEvents)
    m_pthEvents = new CAGCEventThread(this);
  ZAssert(m_pthEvents);

  // Get the event sinks for this event/subject/objects, if not specified
  if (!hListeners)
  {
    hListeners = EventListeners(idEvent, idSubject, idObject1, idObject2);
    if (!hListeners)
      return;
  }
  lock.Unlock();

  // Create an AGCEventData object
  CAGCEventData data(idEvent, pszContext, pszSubject, idSubject,
    cArgTriplets, reinterpret_cast<va_list>(pvArgs));

  // Queue the event to the event thread
  m_pthEvents->QueueEventSynchronous(hListeners, data);
}


STDMETHODIMP_(void) CAGCGlobal::FreeListeners(HAGCLISTENERS hListeners)
{
  // Delegate to static method
  FreeListenersImpl(hListeners);
}


/////////////////////////////////////////////////////////////////////////////
// Description: Determines if the specified object has a callback interface
// registered for the specified event.
//
// Return Value:
// *true* if the specified object was registered for the specified event.
// Otherwise, *false*.
//
STDMETHODIMP_(boolean) CAGCGlobal::IsRegistered(AGCEventID eventID,
  AGCUniqueID uniqueID, IAGCEventSink* pEventSink)
{
  // Find the array for the specified id
  XLock lock(this);
  XRegEventsIt itID = m_RegEvents.find(MakeKey(eventID, uniqueID));
  if (m_RegEvents.end() != itID)
  {
    // Loop thru each array element until pEventSink is found
    XEventSinks& vecEvents = itID->second;
    for (XEventSinksIt it = vecEvents.begin(); it != vecEvents.end(); ++it)
      if (*it == pEventSink)
        return true;
  }
  return false;
}


/////////////////////////////////////////////////////////////////////////////
// Description: Creates an event object.
//
STDMETHODIMP CAGCGlobal::MakeAGCEvent(AGCEventID idEvent,
  LPCSTR pszContext, LPCOLESTR pszSubject, AGCUniqueID idSubject,
  long cArgTriplets, void* pvArgs, IAGCEvent** ppEvent)
{
  // Create an event object
  CComObject<CAGCEvent>* pEvent = NULL;
  RETURN_FAILED(pEvent->CreateInstance(&pEvent));
  IAGCEventPtr spEvent(pEvent);
  RETURN_FAILED(pEvent->Init(idEvent, pszContext, pszSubject, idSubject,
    cArgTriplets, (va_list)pvArgs));

  // Detach the object to the [out] parameter
  *ppEvent = spEvent.Detach();
  return S_OK;
}


STDMETHODIMP_(void) CAGCGlobal::SetAvailableEventIDRanges(
  IAGCEventIDRanges* pRanges)
{
  // Copy the specified rangeset
  ZAssert(pRanges);
  XLock lock(this);
  IAGCRangesPrivatePtr spPrivate(pRanges);
  ZSucceeded(spPrivate->CopyRangesTo(&m_EventIDRanges));
}


STDMETHODIMP_(void) CAGCGlobal::GetAvailableEventIDRanges(
  IAGCEventIDRanges** ppRanges)
{
  // Create an AGC.EventIDRanges object
  CComObject<CAGCEventIDRanges>* pRanges = NULL;
  ZSucceeded(pRanges->CreateInstance(&pRanges));
  IAGCRangesPrivatePtr spPrivate(pRanges);
  ZAssert(NULL != spPrivate);

  // Copy the rangeset into the new object
  {
    XLock lock(this);
    ZSucceeded(spPrivate->InitFromRanges(&m_EventIDRanges));
  }

  // AddRef the object to the [out] parameter
  (*ppRanges = pRanges)->AddRef();
}


/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP_(WORD) CAGCGlobal::GetEventSeverity(AGCEventID idEvent)
{
  const CAGCEventDef::XEventDef* it = CAGCEventDef::find(idEvent);
  return (it != CAGCEventDef::end()) ? it->m_wSeverity : 0;
}


/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP_(HINSTANCE) CAGCGlobal::GetResourceInstance()
{
  return _Module.GetResourceInstance();
}


/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CAGCGlobal::MakeAGCVector(const void* pVectorRaw,
  IAGCVector** ppVector)
{
  CComObject<CAGCVector>* pVector = NULL;
  RETURN_FAILED(pVector->CreateInstance(&pVector));
  IAGCVectorPtr spVector(pVector);
  RETURN_FAILED(pVector->InitFromVector(pVectorRaw));
  CLEAROUT(ppVector, (IAGCVector*)spVector);
  spVector.Detach();
  return S_OK;
}


/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CAGCGlobal::MakeAGCOrientation(const void* pOrientationRaw,
  IAGCOrientation** ppOrientation)
{
  CComObject<CAGCOrientation>* pOrientation = NULL;
  RETURN_FAILED(pOrientation->CreateInstance(&pOrientation));
  IAGCOrientationPtr spOrientation(pOrientation);
  RETURN_FAILED(pOrientation->InitFromOrientation(pOrientationRaw));
  CLEAROUT(ppOrientation, (IAGCOrientation*)spOrientation);
  spOrientation.Detach();
  return S_OK;
}


/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CAGCGlobal::MakeAGCEventIDRange(AGCEventID lower,
  AGCEventID upper, IAGCEventIDRange** ppRange)
{
  CComObject<CAGCEventIDRange>* pRange = NULL;
  RETURN_FAILED(pRange->CreateInstance(&pRange));
  IAGCEventIDRangePtr spRange(pRange);
  RETURN_FAILED(pRange->InitFromRange(&make_range(lower, upper)));
  CLEAROUT(ppRange, (IAGCEventIDRange*)spRange);
  spRange.Detach();
  return S_OK;
}


/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CAGCGlobal::RegisterInterfaceInGlobal(IUnknown* pUnk,
  REFIID riid, DWORD* pdwCookie)
{
  HRESULT hr = GetGIT()->RegisterInterfaceInGlobal(pUnk, riid, pdwCookie);
  if (FAILED(hr))
  {
    ZAssert(!"CAGCGlobal::RegisterInterfaceInGlobal()");
  }
  return hr;
}


/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CAGCGlobal::RevokeInterfaceFromGlobal(DWORD dwCookie)
{
  HRESULT hr = GetGIT()->RevokeInterfaceFromGlobal(dwCookie);
  if (FAILED(hr))
  {
    ZAssert(!"CAGCGlobal::RevokeInterfaceFromGlobal()");
  }
  return hr;
}


/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CAGCGlobal::GetInterfaceFromGlobal(DWORD dwCookie,
  REFIID riid, void** ppv)
{
  HRESULT hr = GetGIT()->GetInterfaceFromGlobal(dwCookie, riid, ppv);
  if (FAILED(hr))
  {
    ZAssert(!"CAGCGlobal::GetInterfaceFromGlobal()");
  }
  return hr;
}

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP_(void) CAGCGlobal::Initialize()
{
  // Initialize the CAGCEventDef static class
  CAGCEventDef::Initialize();
}


/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP_(void) CAGCGlobal::Terminate()
{
  // Shutdown the AGCEventThread, if it is running
  CAGCEventThread* pthEvents;
  {
    XLock lock(this);
    pthEvents = m_pthEvents;
  }
  if (pthEvents)
  {
    delete pthEvents;
    XLock lock(this);
    m_pthEvents = NULL;
  }

  // Terminate the CAGCEventDef static class
  CAGCEventDef::Terminate();
}


/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP_(void) CAGCGlobal::SetDebugHook(IAGCDebugHook* pdh)
{
  g_app.SetDebugHook(pdh);
}

