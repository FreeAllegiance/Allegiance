/////////////////////////////////////////////////////////////////////////////
// PigTimer.h : Declaration of the CPigTimer class

#pragma once

#include "resource.h"       // main symbols
#include <..\TCLib\ObjectLock.h>
#include "PigEvent.h"
#include "PigSrv.h"


/////////////////////////////////////////////////////////////////////////////
// CPigTimer
class ATL_NO_VTABLE CPigTimer : 
  public IPigEventImpl<CPigTimer, IPigTimer, &IID_IPigTimer>,
  public ISupportErrorInfo,
  public CComObjectRoot,
  public CComCoClass<CPigTimer, &CLSID_PigTimer>
{
// Declarations
public:
  DECLARE_REGISTRY_RESOURCEID(IDR_PigTimer)
  DECLARE_PROTECT_FINAL_CONSTRUCT()

// Category Map
public:
  BEGIN_CATEGORY_MAP(CPigTimer)
    IMPLEMENTED_CATEGORY(CATID_PigObjects)
  END_CATEGORY_MAP()

// Interface Map
public:
  BEGIN_COM_MAP(CPigTimer)
    COM_INTERFACE_ENTRY(IPigTimer)
    COM_INTERFACE_ENTRY(IPigEvent)
    COM_INTERFACE_ENTRY(IDispatch)
    COM_INTERFACE_ENTRY(ISupportErrorInfo)
  END_COM_MAP()

// Construction / Destruction
public:
  CPigTimer();
  HRESULT FinalConstruct();
  void FinalRelease();
  HRESULT Init(CPigEventOwner* pOwner, float fInterval,
    BSTR bstrEventExpression, long nRepetitions, BSTR bstrName);

// Overrides
public:
  // CPigEvent Overrides
  virtual bool IsExpired();
  virtual bool Pulse(bool bExpired);

// ISupportErrorInfo Interface Methods
public:
  STDMETHODIMP InterfaceSupportsErrorInfo(REFIID riid);

// IPigTimer Interface Methods
public:
  STDMETHODIMP put_RepetitionCount(long nRepetitionCount);
  STDMETHODIMP get_RepetitionCount(long* pnRepetitionCount);
  STDMETHODIMP put_Interval(float fInterval);
  STDMETHODIMP get_Interval(float* pfInterval);

// Types
protected:
  typedef TCObjectLock<CPigTimer> XLock;

// Data Members
protected:
  long    m_nIntervalMS;
  long    m_nRepetitionCount;
  DWORD   m_dwExpireNext;

// Friends
protected:
  friend class IPigEventImpl<CPigTimer, IPigTimer, &IID_IPigTimer>;
};


/////////////////////////////////////////////////////////////////////////////