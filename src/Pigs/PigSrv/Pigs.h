// Pigs.h : Declaration of the CPigs class

#pragma once

#include "resource.h"       // main symbols
#include "PigEngine.h"
#include "PigSrv.h"
#include <..\TCLib\tstring.h>


/////////////////////////////////////////////////////////////////////////////
// CPigs
class ATL_NO_VTABLE CPigs : 
  public IDispatchImpl<IPigs, &IID_IPigs, &LIBID_PigsLib>,
  public ISupportErrorInfo,
  public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CPigs, &CLSID_Pigs>
{
// Declarations
public:
  DECLARE_REGISTRY_RESOURCEID(IDR_Pigs)
  DECLARE_PROTECT_FINAL_CONSTRUCT()

// Category Map
public:
  BEGIN_CATEGORY_MAP(CPigs)
    IMPLEMENTED_CATEGORY(CATID_PigObjects)
  END_CATEGORY_MAP()

// Interface Map
public:
  BEGIN_COM_MAP(CPigs)
    COM_INTERFACE_ENTRY(IPigs)
    COM_INTERFACE_ENTRY(IDispatch)
    COM_INTERFACE_ENTRY(ITCCollection)
    COM_INTERFACE_ENTRY(ISupportErrorInfo)
  END_COM_MAP()

// Construction / Destruction
public:
  CPigs();
  HRESULT FinalConstruct();
  void FinalRelease();

// Attributes
public:
  bool Exists(_bstr_t bstrName);

// Operations
public:
  void AddPig(DWORD dwGITCookie, _bstr_t bstrName);
  void RemovePig(DWORD dwGITCookie);
  void RenamePig(DWORD dwGITCookie, _bstr_t bstrName);
  DWORD Find(_bstr_t bstrName);

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

// IPigs Interface Methods
public:
  STDMETHODIMP get_Item(VARIANT* pvIndex, IPig** ppPig);

// Types
protected:
  typedef TCObjectLock<CPigs>                      XLock;
  typedef std::map<DWORD, _bstr_t>                 XMapByCookie;
  typedef XMapByCookie::iterator                   XMapByCookieIt;
  typedef std::map<_bstr_t, DWORD, ci_less_bstr_t> XMapByName;
  typedef XMapByName::iterator                     XMapByNameIt;

// Data Members
protected:
  XMapByCookie m_mapByCookie;
  XMapByName   m_mapByName;
};


/////////////////////////////////////////////////////////////////////////////
// Attributes

inline bool CPigs::Exists(_bstr_t bstrName)
{
  return 0 != Find(bstrName);
}


/////////////////////////////////////////////////////////////////////////////
// Operations

inline DWORD CPigs::Find(_bstr_t bstrName)
{
  // Lookup the specified name in the 'by name' map
  XLock lock(this);
  XMapByNameIt itName = m_mapByName.find(bstrName);
  return (m_mapByName.end() == itName) ? 0 : itName->second;
}


/////////////////////////////////////////////////////////////////////////////
// Implementation

inline CPigEngine& CPigs::GetEngine()
{
  return CPigEngine::GetEngine();
}


/////////////////////////////////////////////////////////////////////////////