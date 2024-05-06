/////////////////////////////////////////////////////////////////////////////
// AGCOrientation.h : Declaration of the CAGCOrientation
//

#pragma once

#include <AGC.h>
#include <..\TCLib\ObjectLock.h>
#include "resource.h"


/////////////////////////////////////////////////////////////////////////////
// CAGCOrientation
class ATL_NO_VTABLE CAGCOrientation : 
  public IDispatchImpl<IAGCOrientation, &IID_IAGCOrientation, &LIBID_AGCLib>,
  public IAGCOrientationPrivate,
	public ISupportErrorInfo,
  public IPersistStreamInit,
  public AGCObjectSafetyImpl<CAGCOrientation>,
	public CComObjectRootEx<CComMultiThreadModel>, 
	public CComCoClass<CAGCOrientation, &CLSID_AGCOrientation>
{
// Declarations
public:
  DECLARE_REGISTRY_RESOURCEID(IDR_AGCOrientation)
  DECLARE_PROTECT_FINAL_CONSTRUCT()
  DECLARE_GET_CONTROLLING_UNKNOWN()

// Interface Map
public:
  BEGIN_COM_MAP(CAGCOrientation)
	  COM_INTERFACE_ENTRY(IAGCOrientation)
	  COM_INTERFACE_ENTRY(IAGCOrientationPrivate)
	  COM_INTERFACE_ENTRY(IDispatch)
	  COM_INTERFACE_ENTRY(ISupportErrorInfo)
    COM_INTERFACE_ENTRY(IPersistStreamInit)
    COM_INTERFACE_ENTRY2(IPersistStream, IPersistStreamInit)
    COM_INTERFACE_ENTRY2(IPersist, IPersistStreamInit)
    COM_INTERFACE_ENTRY_AUTOAGGREGATE(IID_IMarshal, m_punkMBV.p,
      CLSID_TCMarshalByValue)
    COM_INTERFACE_ENTRY(IObjectSafety)
  END_COM_MAP()

// Category Map
public:
  BEGIN_CATEGORY_MAP(CAGCOrientation)
    IMPLEMENTED_CATEGORY(CATID_AGC)
    IMPLEMENTED_CATEGORY(CATID_SafeForScripting)
    IMPLEMENTED_CATEGORY(CATID_SafeForInitializing)
  END_CATEGORY_MAP()

// Construction
public:
  CAGCOrientation() :
    m_bDirty(false)
	{
	}

// Implemenatation
protected:
  static HRESULT GetRawOrientation(IAGCOrientation* pOrientation,
    Orientation* pOrientationRaw);
  static HRESULT CreateResultVector(const Vector* pVectorRaw,
    IAGCVector** ppResult);

// ISupportsErrorInfo Interface Methods
public:
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// IAGCOrientation Interface Methods
public:
  STDMETHODIMP InitCopy(IAGCOrientation* pOrientation);
  STDMETHODIMP get_Forward(IAGCVector** ppVector);
  STDMETHODIMP get_Backward(IAGCVector** ppVector);
  STDMETHODIMP get_Up(IAGCVector** ppVector);
  STDMETHODIMP get_Right(IAGCVector** ppVector);
  STDMETHODIMP get_IsEqual(IAGCOrientation* pOrientation,
    VARIANT_BOOL* pbIsEqual);
  STDMETHODIMP get_IsRoughlyEqual(IAGCOrientation* pOrientation,
    VARIANT_BOOL* pbIsEqual);

// IAGCOrientationPrivate Interface Methods
public:
  STDMETHODIMP InitFromOrientation(const void* pvOrientation);
  STDMETHODIMP CopyOrientationTo(void* pvOrientation);

// IPersist Interface Methods
public:
  STDMETHODIMP GetClassID(CLSID* pClassID);

// IPersistStreamInit Interface Methods
public:
  STDMETHODIMP IsDirty();
  STDMETHODIMP Load(LPSTREAM pStm);
  STDMETHODIMP Save(LPSTREAM pStm, BOOL fClearDirty);
  STDMETHODIMP GetSizeMax(ULARGE_INTEGER* pCbSize);
  STDMETHODIMP InitNew(void);

// Enumerations
protected:
  enum {DIMENSIONS = 9};

// Types
protected:
  typedef TCObjectLock<CAGCOrientation> XLock;

// Data Members
private:
  Orientation m_orientation; // the actual orientation data goes here!
  bool   m_bDirty;
public:  
  CComPtr<IUnknown> m_punkMBV;
};


/////////////////////////////////////////////////////////////////////////////