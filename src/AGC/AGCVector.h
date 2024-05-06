/////////////////////////////////////////////////////////////////////////////
// AGCVector.h : Declaration of the CAGCVector
//

#pragma once

#include <AGC.h>
#include <..\TCLib\ObjectLock.h>
#include "resource.h"


/////////////////////////////////////////////////////////////////////////////
// CAGCVector
class ATL_NO_VTABLE CAGCVector : 
  public IDispatchImpl<IAGCVector, &IID_IAGCVector, &LIBID_AGCLib>,
  public IAGCVectorPrivate,
	public ISupportErrorInfo,
  public IPersistStreamInit,
  public IPersistPropertyBag,
  public AGCObjectSafetyImpl<CAGCVector>,
	public CComObjectRootEx<CComMultiThreadModel>, 
	public CComCoClass<CAGCVector, &CLSID_AGCVector>
{
// Declarations
public:
  DECLARE_REGISTRY_RESOURCEID(IDR_AGCVector)
  DECLARE_PROTECT_FINAL_CONSTRUCT()
  DECLARE_GET_CONTROLLING_UNKNOWN()

// Interface Map
public:
  BEGIN_COM_MAP(CAGCVector)
	  COM_INTERFACE_ENTRY(IAGCVector)
	  COM_INTERFACE_ENTRY(IAGCVectorPrivate)
	  COM_INTERFACE_ENTRY(IDispatch)
	  COM_INTERFACE_ENTRY(ISupportErrorInfo)
    COM_INTERFACE_ENTRY(IPersistStreamInit)
    COM_INTERFACE_ENTRY(IPersistPropertyBag)
    COM_INTERFACE_ENTRY2(IPersistStream, IPersistStreamInit)
    COM_INTERFACE_ENTRY2(IPersist, IPersistStreamInit)
    COM_INTERFACE_ENTRY_AUTOAGGREGATE(IID_IMarshal, m_punkMBV.p,
      CLSID_TCMarshalByValue)
    COM_INTERFACE_ENTRY(IObjectSafety)
  END_COM_MAP()

// Category Map
public:
  BEGIN_CATEGORY_MAP(CAGCVector)
    IMPLEMENTED_CATEGORY(CATID_AGC)
    IMPLEMENTED_CATEGORY(CATID_SafeForScripting)
    IMPLEMENTED_CATEGORY(CATID_SafeForInitializing)
  END_CATEGORY_MAP()

// Construction
public:
  CAGCVector() :
    m_bDirty(false)
	{
    m_vector.SetX(0);
    m_vector.SetY(0);
    m_vector.SetZ(0);
	}

// Implemenatation
protected:
  static HRESULT GetRawVector(IAGCVector* pVector, Vector* pVectorRaw);
  static HRESULT CreateResultVector(const Vector* pVectorRaw,
    IAGCVector** ppResult);

// ISupportsErrorInfo Interface Methods
public:
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// IAGCVector Interface Methods
public:
  STDMETHODIMP put_X(float Val);
  STDMETHODIMP get_X(float* pVal);
  STDMETHODIMP put_Y(float Val);
  STDMETHODIMP get_Y(float* pVal);
  STDMETHODIMP put_Z(float Val);
  STDMETHODIMP get_Z(float* pVal);
  STDMETHODIMP put_DisplayString(BSTR bstrDisplayString);
  STDMETHODIMP get_DisplayString(BSTR* pbstrDisplayString);
  STDMETHODIMP InitXYZ(float xArg, float yArg, float zArg);
  STDMETHODIMP InitCopy(IAGCVector* pVector);
  STDMETHODIMP InitRandomDirection();
  STDMETHODIMP InitRandomPosition(float fRadius);
  STDMETHODIMP get_IsZero(VARIANT_BOOL* pbIsZero);
  STDMETHODIMP get_IsEqual(IAGCVector* pVector, VARIANT_BOOL* pbIsEqual);
  STDMETHODIMP get_Length(float* pfLength);
  STDMETHODIMP get_LengthSquared(float* pfLengthSquared);
  STDMETHODIMP get_OrthogonalVector(IAGCVector** ppResult);
  STDMETHODIMP Add(IAGCVector* pVector,IAGCVector** ppResult);
  STDMETHODIMP Subtract(IAGCVector* pVector, IAGCVector** ppResult);
  STDMETHODIMP Multiply(float f, IAGCVector** ppResult);
  STDMETHODIMP Divide(float f, IAGCVector** ppResult);
  STDMETHODIMP Normalize(IAGCVector** ppResult);
  STDMETHODIMP CrossProduct(IAGCVector* pVector, IAGCVector** ppResult);
  STDMETHODIMP Interpolate(IAGCVector* pVector, float fValue, IAGCVector** ppResult);
  STDMETHODIMP AddInPlace(IAGCVector* pVector);
  STDMETHODIMP SubtractInPlace(IAGCVector* pVector);
  STDMETHODIMP MultiplyInPlace(float f);
  STDMETHODIMP DivideInPlace(float f);
  STDMETHODIMP NormalizeInPlace();
  STDMETHODIMP CrossProductInPlace(IAGCVector* pVector);
  STDMETHODIMP InterpolateInPlace(IAGCVector* pVector, float fValue);

// IAGCVectorPrivate Interface Methods
public:
  STDMETHODIMP InitFromVector(const void* pvVector);
  STDMETHODIMP CopyVectorTo(void* pvVector);

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

// IPersistPropertyBag Interface Methods
public:
  STDMETHODIMP Load(IPropertyBag* pPropBag, IErrorLog* pErrorLog);
  STDMETHODIMP Save(IPropertyBag* pPropBag, BOOL fClearDirty,
    BOOL fSaveAllProperties);

// Enumerations
protected:
  enum {DIMENSIONS = 3};

// Types
protected:
  typedef TCObjectLock<CAGCVector> XLock;

// Data Members
private:
  Vector m_vector; // the actual vector data goes here!
  bool   m_bDirty;
public:  
  CComPtr<IUnknown> m_punkMBV;
};


/////////////////////////////////////////////////////////////////////////////