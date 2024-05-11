/////////////////////////////////////////////////////////////////////////////
// PigHullTypes.h : Declaration of the CPigHullTypes class

#pragma once

#include "resource.h"       // main symbols
#include "PigSrv.h"
#include <Allegiance.h>
#include <..\TCLib\ObjectLock.h>


/////////////////////////////////////////////////////////////////////////////
// CPigHullTypes
class ATL_NO_VTABLE CPigHullTypes : 
  public IDispatchImpl<IPigHullTypes, &IID_IPigHullTypes, &LIBID_PigsLib>,
  public ISupportErrorInfo,
  public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CPigHullTypes, &CLSID_PigHullTypes>
{
// Declarations
public:
  DECLARE_REGISTRY_RESOURCEID(IDR_PigHullTypes)
  DECLARE_PROTECT_FINAL_CONSTRUCT()

// Category Map
public:
  BEGIN_CATEGORY_MAP(CPigHullTypes)
    IMPLEMENTED_CATEGORY(CATID_PigObjects)
  END_CATEGORY_MAP()

// Interface Map
public:
  BEGIN_COM_MAP(CPigHullTypes)
    COM_INTERFACE_ENTRY(IPigHullTypes)
    COM_INTERFACE_ENTRY(IDispatch)
    COM_INTERFACE_ENTRY(ITCCollection)
    COM_INTERFACE_ENTRY(ISupportErrorInfo)
  END_COM_MAP()

// Construction / Destruction
public:
  CPigHullTypes();
  HRESULT Init(const HullTypeListIGC* pslistIGC, IstationIGC* pStation);

// ISupportErrorInfo Interface Methods
public:
  STDMETHODIMP InterfaceSupportsErrorInfo(REFIID riid);

// ITCCollection Interface Methods
public:
  STDMETHODIMP get_Count(long* pnCount);
  STDMETHODIMP get__NewEnum(IUnknown** ppunkEnum);

// IPigHullTypes Interface Methods
public:
  STDMETHODIMP get_Item(VARIANT* pvIndex, IAGCHullType** ppHullType);

// Types
protected:
  typedef TCObjectLock<CPigHullTypes>  XLock;
  typedef std::vector<IhullTypeIGC*>   XHullTypesIGC;

// Data Members
protected:
  XHullTypesIGC m_HullTypesIGC;
};


/////////////////////////////////////////////////////////////////////////////