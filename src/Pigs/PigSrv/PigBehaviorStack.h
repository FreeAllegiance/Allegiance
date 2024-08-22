#pragma once

/////////////////////////////////////////////////////////////////////////////
// PigBehaviorStack.h : Declaration of the CPigBehaviorStack class.

#include "PigSrv.h"
#include "resource.h"
#include "PigEngine.h"
#include "PigEventOwner.h"


/////////////////////////////////////////////////////////////////////////////
// Forward Declarations

class CPig;
class CPigBehaviorType;
class CPigBehaviorScriptType;


/////////////////////////////////////////////////////////////////////////////
// CPigBehaviorStack
class ATL_NO_VTABLE CPigBehaviorStack : 
  public IDispatchImpl<IPigBehaviorStack, &IID_IPigBehaviorStack, &LIBID_PigsLib>,
  public ISupportErrorInfo,
  public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CPigBehaviorStack, &CLSID_PigBehaviorStack>
{
// Declarations
public:
  DECLARE_REGISTRY_RESOURCEID(IDR_PigBehaviorStack)
  DECLARE_PROTECT_FINAL_CONSTRUCT()

// Category Map
public:
  BEGIN_CATEGORY_MAP(CPigBehaviorStack)
    IMPLEMENTED_CATEGORY(CATID_PigObjects)
  END_CATEGORY_MAP()

// Interface Map
public:
  BEGIN_COM_MAP(CPigBehaviorStack)
    COM_INTERFACE_ENTRY(IPigBehaviorStack)
    COM_INTERFACE_ENTRY(IDispatch)
    COM_INTERFACE_ENTRY(ISupportErrorInfo)
  END_COM_MAP()

// Construction / Destruction
public:
  CPigBehaviorStack();
  HRESULT Init(CPig* pPig);

// Implementation
protected:
  static CPigEngine& GetEngine();

// ISupportErrorInfo Interface Methods
public:
  STDMETHODIMP InterfaceSupportsErrorInfo(REFIID riid);

// ITCCollection Interface Methods
public:
  STDMETHODIMP get_Count(long* pnCount);
  STDMETHODIMP get__NewEnum(IUnknown** ppunkEnum);

// IPigBehaviorStack Interface Methods
public:
  STDMETHODIMP Push(BSTR bstrType, BSTR bstrCommandLine,
    IPigBehavior** ppBehavior);
  STDMETHODIMP Pop();
  STDMETHODIMP get_Top(long nFromTop, IPigBehavior** ppBehavior);
  STDMETHODIMP get_Pig(IPig** ppPig);

// Types
protected:
  typedef TCObjectLock<CPigBehaviorStack> XLock;

// Data Members
protected:
  CPig* m_pPig;
};


/////////////////////////////////////////////////////////////////////////////
// Implementation

inline CPigEngine& CPigBehaviorStack::GetEngine()
{
  return CPigEngine::GetEngine();
}


/////////////////////////////////////////////////////////////////////////////