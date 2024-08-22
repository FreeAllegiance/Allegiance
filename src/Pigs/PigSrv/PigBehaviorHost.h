#pragma once

/////////////////////////////////////////////////////////////////////////////
// PigBehaviorHost.h : Declaration of the CPigBehaviorHost class

#include "resource.h"       // main symbols
#include "PigEngine.h"
#include "PigSrv.h"


/////////////////////////////////////////////////////////////////////////////
// Forward Declarations

class CPig;


/////////////////////////////////////////////////////////////////////////////
// CPigBehaviorHost
class ATL_NO_VTABLE CPigBehaviorHost : 
  public IDispatchImpl<IPigBehaviorHost, &IID_IPigBehaviorHost, &LIBID_PigsLib>,
  public ISupportErrorInfo,
  public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CPigBehaviorHost, &CLSID_PigBehaviorHost>
{
// Declarations
public:
  DECLARE_REGISTRY_RESOURCEID(IDR_PigBehaviorHost)
  DECLARE_PROTECT_FINAL_CONSTRUCT()

// Category Map
public:
  BEGIN_CATEGORY_MAP(CPigBehaviorHost)
    IMPLEMENTED_CATEGORY(CATID_PigObjects)
  END_CATEGORY_MAP()

// Interface Map
public:
  BEGIN_COM_MAP(CPigBehaviorHost)
    COM_INTERFACE_ENTRY(IPigBehaviorHost)
    COM_INTERFACE_ENTRY(IDispatch)
    COM_INTERFACE_ENTRY(ISupportErrorInfo)
  END_COM_MAP()

// Construction / Destruction
public:
  CPigBehaviorHost();
  HRESULT FinalConstruct();
  void FinalRelease();
  HRESULT Init(CPig* pPig);

// Attributes
public:

// Implementation
protected:
  static CPigEngine& GetEngine();
  void TraceOutput(LPCOLESTR pszName, LPCOLESTR pszText,
    LPCOLESTR pszTextFormatted);

// ISupportErrorInfo Interface Methods
public:
  STDMETHODIMP InterfaceSupportsErrorInfo(REFIID riid);

// IPigBehaviorHost Interface Methods
public:
  STDMETHODIMP Beep(long nFrequency, long nDuration);
  STDMETHODIMP CreatePig(BSTR bstrType, BSTR bstrCommandLine, IPig** ppPig);
  STDMETHODIMP Trace(BSTR bstrText);
  STDMETHODIMP get_StateName(PigState ePigState, BSTR* pbstrPigState);
  STDMETHODIMP Random(short* pnRandom);

// Types
protected:
  typedef TCObjectLock<CPigBehaviorHost> XLock;

// Data Members
protected:
  CPig* m_pPig;
};


/////////////////////////////////////////////////////////////////////////////
// Attributes


/////////////////////////////////////////////////////////////////////////////
// Implementation

inline CPigEngine& CPigBehaviorHost::GetEngine()
{
  return CPigEngine::GetEngine();
}


/////////////////////////////////////////////////////////////////////////////