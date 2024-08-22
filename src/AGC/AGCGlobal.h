#pragma once

/////////////////////////////////////////////////////////////////////////////
// AGCGlobal : Declaration of the CAGCGlobal class.
//

#include <AGC.h>
#include "resource.h"
#include <..\TCLib\ObjectLock.h>
#include <..\TCLib\RangeSet.h>


/////////////////////////////////////////////////////////////////////////////
// Forward Declarations

class CAGCEvent;
class CAGCEventThread;


/////////////////////////////////////////////////////////////////////////////
//
class ATL_NO_VTABLE CAGCGlobal :
  public IAGCGlobal,
  public CComObjectRootEx<CComMultiThreadModel>, 
  public CComCoClass<CAGCGlobal, &CLSID_AGCGlobal>
{
// Declarations
public:
  DECLARE_CLASSFACTORY_SINGLETON(CAGCGlobal)
  DECLARE_REGISTRY_RESOURCEID(IDR_AGCGlobal)

// Interface Map
public:
  BEGIN_COM_MAP(CAGCGlobal)
    COM_INTERFACE_ENTRY(IAGCGlobal)
  END_COM_MAP()

// Construction / Destruction
public:
  CAGCGlobal();
  void FinalRelease();

// Implementation
protected:
  static void FreeListenersImpl(HAGCLISTENERS hListeners);
  static IGlobalInterfaceTablePtr& GetGIT();
  static TCINT64 MakeKey(AGCEventID idEvent, AGCUniqueID idUnique);
  static const IID* InterpretIID(const void* pvIgc, REFIID iid);

// IAGCGlobal Interface Methods
public:
  // Object Creation Registration
  STDMETHODIMP_(void) RegisterObjectCreator(REFIID riid,
    PFNAGCCreator pfnCreator);
  STDMETHODIMP_(void) RevokeObjectCreator(REFIID riid);
  // IGC to AGC Mapping
  STDMETHODIMP GetAGCObject(const void* pvIgc, REFIID riid, void** ppUnk);
  STDMETHODIMP_(void) AddAGCObject(const void* pvIgc, IUnknown* pUnkAGC);
  STDMETHODIMP_(void) RemoveAGCObject(const void* pvIgc,
    boolean bForceDestruct);
  // Event Registration
  STDMETHODIMP_(void)    SetEventSinksAreGITCookies(boolean bEventSinksAreGITCookies);
  STDMETHODIMP_(boolean) GetEventSinksAreGITCookies();
  STDMETHODIMP_(void) RegisterEvent(AGCEventID eventID, AGCUniqueID uniqueID, 
    IAGCEventSink* pEventSink);
  STDMETHODIMP_(boolean) RevokeEvent(AGCEventID eventID, AGCUniqueID uniqueID, 
    IAGCEventSink* pEventSink);
  STDMETHODIMP_(void) RegisterEventRanges(IAGCEventIDRanges* pRanges,
    AGCUniqueID uniqueID, IAGCEventSink* pEventSink);
  STDMETHODIMP_(void) RevokeEventRanges(IAGCEventIDRanges* pRanges,
    AGCUniqueID uniqueID, IAGCEventSink* pEventSink);
  STDMETHODIMP_(int) RevokeAllEvents(IAGCEventSink* pEventSink);
  STDMETHODIMP_(int) RegisterAllEvents(IAGCEventSink* pEventSink);
  STDMETHODIMP_(HAGCLISTENERS) EventListeners(AGCEventID idEvent,
    AGCUniqueID idSubject = -1,
    AGCUniqueID idObject1 = -1, AGCUniqueID idObject2 = -1);
  STDMETHODIMP_(void) TriggerEvent(HAGCLISTENERS hListeners, AGCEventID idEvent,
    LPCSTR pszContext, LPCOLESTR pszSubject, AGCUniqueID idSubject,
    AGCUniqueID idObject1, AGCUniqueID idObject2, long cArgTriplets, void* pvArgs);
  STDMETHODIMP_(void) TriggerEventSynchronous(HAGCLISTENERS hListeners, AGCEventID idEvent,
    LPCSTR pszContext, LPCOLESTR pszSubject, AGCUniqueID idSubject,
    AGCUniqueID idObject1, AGCUniqueID idObject2, long cArgTriplets, void* pvArgs);
  STDMETHODIMP_(void) FreeListeners(HAGCLISTENERS hListeners);
  STDMETHODIMP_(boolean) IsRegistered(AGCEventID eventID, AGCUniqueID uniqueID, 
    IAGCEventSink* pEventSink);
  STDMETHODIMP MakeAGCEvent(AGCEventID idEvent, LPCSTR pszContext, LPCOLESTR pszSubject,
    AGCUniqueID idSubject, long cArgTriplets, void* pvArgs, IAGCEvent** ppEvent);
  STDMETHODIMP_(void) SetAvailableEventIDRanges(IAGCEventIDRanges* pRanges);
  STDMETHODIMP_(void) GetAvailableEventIDRanges(IAGCEventIDRanges** ppRanges);
  STDMETHODIMP_(WORD) GetEventSeverity(AGCEventID idEvent);
  // Miscellaneous
  STDMETHODIMP_(HINSTANCE) GetResourceInstance();
  STDMETHODIMP MakeAGCVector(const void* pVectorRaw, IAGCVector** ppVector);
  STDMETHODIMP MakeAGCOrientation(const void* pOrientationRaw,
    IAGCOrientation** ppOrientation);
  STDMETHODIMP MakeAGCEventIDRange(AGCEventID lower, AGCEventID upper,
    IAGCEventIDRange** ppRange);
  // IGlobalInterfaceTable Interface Methods (delegate to single GIT instance)
  STDMETHODIMP RegisterInterfaceInGlobal(IUnknown* pUnk, REFIID riid, DWORD* pdwCookie);
  STDMETHODIMP RevokeInterfaceFromGlobal(DWORD dwCookie);
  STDMETHODIMP GetInterfaceFromGlobal(DWORD dwCookie, REFIID riid, void** ppv);
  // Object Initialization and Cleanup
  STDMETHODIMP_(void) Initialize();
  STDMETHODIMP_(void) Terminate();
  // Debugging Support
  STDMETHODIMP_(void) SetDebugHook(IAGCDebugHook* pdh);

// Types
protected:
  typedef TCObjectLock<CAGCGlobal>         XLock;
  // Object Creation Registration
  typedef std::map<CLSID, PFNAGCCreator>   XAGCCreatorMap;
  typedef XAGCCreatorMap::iterator         XAGCCreatorMapIt;
  // IGC to AGC Mapping
  typedef std::map<const void*, IUnknown*> XIgcToAGCMap;
  typedef XIgcToAGCMap::iterator           XIgcToAGCMapIt;
  // Event Registration
  typedef std::vector<IAGCEventSink*>      XEventSinks;
  typedef XEventSinks::iterator            XEventSinksIt;
  typedef rangeset<range<AGCEventID> >     XEventIDRanges;
  typedef XEventIDRanges::iterator         XEventIDRangeIt;

  // For XRegEvents, the map's key is 64 bits:
  // the lower DWORD (32 bits) is the AGCUniqueID
  // the upper DWORD (32 bits) is the AGCEventID
  typedef std::map<TCINT64, XEventSinks>   XRegEvents;
  typedef XRegEvents::iterator             XRegEventsIt;

// Data Members
protected:
  // Object Creation Registration
  XAGCCreatorMap   m_mapCreator;
  // IGC to AGC Mapping
  XIgcToAGCMap     m_map;
  // Event Registration
  XRegEvents       m_RegEvents;
  bool             m_bEventSinksAreGITCookies;
  XEventIDRanges   m_EventIDRanges;
  // Event Firing
  CAGCEventThread* m_pthEvents;

// Friends
protected:
  friend class CAGCEventThread;
};


/////////////////////////////////////////////////////////////////////////////
// Construction / Destruction

inline CAGCGlobal::CAGCGlobal() :
  m_bEventSinksAreGITCookies(false),
  m_pthEvents(NULL)
{
}


/////////////////////////////////////////////////////////////////////////////
// Implementation

inline void CAGCGlobal::FreeListenersImpl(HAGCLISTENERS hListeners)
{
  if (hListeners)
    delete reinterpret_cast<XEventSinks*>(hListeners);
}

inline IGlobalInterfaceTablePtr& CAGCGlobal::GetGIT()
{
  static IGlobalInterfaceTablePtr spGIT;
  if (NULL == spGIT)
    _SVERIFYE(spGIT.CreateInstance(CLSID_StdGlobalInterfaceTable));
  return spGIT;
}

inline TCINT64 CAGCGlobal::MakeKey(AGCEventID idEvent, AGCUniqueID idUnique)
{
  return (TCINT64(idEvent) << 32) | idUnique;
}


/////////////////////////////////////////////////////////////////////////////