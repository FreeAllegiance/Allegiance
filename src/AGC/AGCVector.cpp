/////////////////////////////////////////////////////////////////////////////
// AGCVector.cpp : Implementation of CAGCVector
//

#include "pch.h"
#include "AGCVector.h"


/////////////////////////////////////////////////////////////////////////////
// CVector

TC_OBJECT_EXTERN_IMPL(CAGCVector)


/////////////////////////////////////////////////////////////////////////////
// Implementation

HRESULT CAGCVector::GetRawVector(IAGCVector* pVector, Vector* pVectorRaw)
{
  __try
  {
    if (!pVector)
      return E_POINTER;
    IAGCVectorPrivate* pPrivate = NULL;
    RETURN_FAILED(pVector->QueryInterface(__uuidof(pPrivate), (void**)&pPrivate));
    ZAssert(pPrivate);
    if (!pPrivate)
      return E_INVALIDARG;
    HRESULT hr = pPrivate->CopyVectorTo(pVectorRaw);
    pPrivate->Release();
    return hr;
  }
  __except(1)
  {
    return E_POINTER;
  }
}

HRESULT CAGCVector::CreateResultVector(const Vector* pVectorRaw,
  IAGCVector** ppResult)
{
  CComObject<CAGCVector>* pVector = NULL;
  RETURN_FAILED(pVector->CreateInstance(&pVector));
  IAGCVectorPtr spVector(pVector);
  RETURN_FAILED(pVector->InitFromVector(pVectorRaw));
  CLEAROUT(ppResult, (IAGCVector*)spVector);
  spVector.Detach();
  return S_OK;
}


/////////////////////////////////////////////////////////////////////////////
// ISupportErrorInfo Interface Methods

STDMETHODIMP CAGCVector::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IAGCVector
	};
	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}


/////////////////////////////////////////////////////////////////////////////
// IPersist Interface Methods

STDMETHODIMP CAGCVector::GetClassID(CLSID* pClassID)
{
  __try
  {
    *pClassID = GetObjectCLSID();
  }
  __except(1)
  {
    return E_POINTER;
  }
  return S_OK;
}


/////////////////////////////////////////////////////////////////////////////
// IPersistStreamInit Interface Methods

STDMETHODIMP CAGCVector::IsDirty()
{
  // Return dirty flag
  XLock lock(this);
  return m_bDirty ? S_OK : S_FALSE;
}

STDMETHODIMP CAGCVector::Load(LPSTREAM pStm)
{
  // Read the number of floats being read
  long cDims;
  RETURN_FAILED(pStm->Read(&cDims, sizeof(cDims), NULL));
  if (DIMENSIONS != cDims)
  {
    ZAssert(DIMENSIONS == cDims);
    return ERROR_INVALID_DATA;
  }

  // Read each float from the stream
  float xArg, yArg, zArg;
  RETURN_FAILED(pStm->Read(&xArg, sizeof(xArg), NULL));
  RETURN_FAILED(pStm->Read(&yArg, sizeof(yArg), NULL));
  RETURN_FAILED(pStm->Read(&zArg, sizeof(zArg), NULL));

  // Save the values
  return InitXYZ(xArg, yArg, zArg);
}

STDMETHODIMP CAGCVector::Save(LPSTREAM pStm, BOOL fClearDirty)
{
  // Write out the number of floats being written
  long cDims = DIMENSIONS;
  RETURN_FAILED(pStm->Write(&cDims, sizeof(cDims), NULL));

  // Write each float to the stream
  XLock lock(this);
  float xArg = m_vector.X(), yArg = m_vector.Y(), zArg = m_vector.Z();
  RETURN_FAILED(pStm->Write(&xArg, sizeof(xArg), NULL));
  RETURN_FAILED(pStm->Write(&yArg, sizeof(yArg), NULL));
  RETURN_FAILED(pStm->Write(&zArg, sizeof(zArg), NULL));

  // Clear the dirty flag, if specified
  if (fClearDirty)
    m_bDirty = false;

  // Indicate success
  return S_OK;
}

STDMETHODIMP CAGCVector::GetSizeMax(ULARGE_INTEGER* pCbSize)
{
  __try
  {
    // NOTE: This needs to exactly match what's written in Save
    pCbSize->LowPart  = sizeof(long) + sizeof(float) * DIMENSIONS;
    pCbSize->HighPart = 0;
  }
  __except(1)
  {
    return E_POINTER;
  }

  // Indicate success
  return S_OK;
}

STDMETHODIMP CAGCVector::InitNew( void)
{
  // Initialize the vector object
  XLock lock(this);
  m_vector = Vector::GetZero();

  // Indicate success
  return S_OK;
}


/////////////////////////////////////////////////////////////////////////////
// IPersistPropertyBag Interface Methods

STDMETHODIMP CAGCVector::Load(IPropertyBag* pPropBag, IErrorLog* pErrorLog)
{
  // Load each dimension (column) of the vector object
  CComVariant varX(0.f), varY(0.f), varZ(0.f);
  RETURN_FAILED(pPropBag->Read(L"X", &varX, pErrorLog));
  RETURN_FAILED(pPropBag->Read(L"Y", &varY, pErrorLog));
  RETURN_FAILED(pPropBag->Read(L"Z", &varZ, pErrorLog));

  // Save the values
  return InitXYZ(V_R4(&varX), V_R4(&varY), V_R4(&varZ));
}

STDMETHODIMP CAGCVector::Save(IPropertyBag* pPropBag, BOOL fClearDirty, BOOL)
{
  // Save each dimension (column) of the vector object
  XLock lock(this);
  RETURN_FAILED(pPropBag->Write(L"X", &CComVariant(m_vector.X())));
  RETURN_FAILED(pPropBag->Write(L"Y", &CComVariant(m_vector.Y())));
  RETURN_FAILED(pPropBag->Write(L"Z", &CComVariant(m_vector.Z())));

  // Clear the dirty flag, if specified
  if (fClearDirty)
    m_bDirty = false;

  // Indicate success
  return S_OK;
}


/////////////////////////////////////////////////////////////////////////////
// IAGCVector Interface Methods

STDMETHODIMP CAGCVector::put_X(float Val) 
{
  XLock lock(this);
  if (m_vector.X() != Val)
  {
    m_vector.SetX(Val);
    m_bDirty = true;
  }
  return S_OK;
}

STDMETHODIMP CAGCVector::get_X(float* pVal) 
{
  XLock lock(this);
  CLEAROUT(pVal, m_vector.X());
  return S_OK;
}

STDMETHODIMP CAGCVector::put_Y(float Val) 
{
  XLock lock(this);
  if (m_vector.Y() != Val)
  {
    m_vector.SetY(Val);
    m_bDirty = true;
  }
  return S_OK;
}

STDMETHODIMP CAGCVector::get_Y(float* pVal) 
{
  XLock lock(this);
  CLEAROUT(pVal, m_vector.Y());
  return S_OK;
}

STDMETHODIMP CAGCVector::put_Z(float Val) 
{
  XLock lock(this);
  if (m_vector.Z() != Val)
  {
    m_vector.SetZ(Val);
    m_bDirty = true;
  }
  return S_OK;
}

STDMETHODIMP CAGCVector::get_Z(float* pVal) 
{
  XLock lock(this);
  CLEAROUT(pVal, m_vector.Z());
  return S_OK;
}

STDMETHODIMP CAGCVector::put_DisplayString(BSTR bstrDisplayString)
{
  // Invalid if string is empty
  if (!BSTRLen(bstrDisplayString))
    return E_INVALIDARG;

  // Load the format string
  CComBSTR bstrFmt;
  if (!bstrFmt.LoadString(IDS_FMT_VECTOR))
    return HRESULT_FROM_WIN32(::GetLastError());

  // Extract the values from the string
  float xArg, yArg, zArg;
  if (3 != swscanf(bstrDisplayString, bstrFmt, &xArg, &yArg, &zArg))
    return E_INVALIDARG;

  // Save the values
  return InitXYZ(xArg, yArg, zArg);
}

STDMETHODIMP CAGCVector::get_DisplayString(BSTR* pbstrDisplayString)
{
  // Load the format string
  CComBSTR bstrFmt;
  if (!bstrFmt.LoadString(IDS_FMT_VECTOR))
    return HRESULT_FROM_WIN32(::GetLastError());

  // Format the string
  XLock lock(this);
  OLECHAR szText[_MAX_PATH];
  swprintf(szText, bstrFmt, m_vector.X(), m_vector.Y(), m_vector.Z());

  // Detach the string to the [out] parameter
  CComBSTR bstr(szText);
  CLEAROUT(pbstrDisplayString, (BSTR)bstr);
  bstr.Detach();

  // Indicate success
  return S_OK;
}

STDMETHODIMP CAGCVector::InitXYZ(float xArg, float yArg, float zArg)
{
  XLock lock(this);
  if (m_vector.X() != xArg || m_vector.Y() != yArg || m_vector.Z() != zArg)
  {
    m_vector.SetX(xArg);
    m_vector.SetY(yArg);
    m_vector.SetZ(zArg);
    m_bDirty = true;
  }
  return S_OK;
}

STDMETHODIMP CAGCVector::InitCopy(IAGCVector* pVector)
{
  // Get the values of the specified vector
  float xArg, yArg, zArg;
  RETURN_FAILED(pVector->get_X(&xArg));
  RETURN_FAILED(pVector->get_Y(&yArg));
  RETURN_FAILED(pVector->get_Z(&zArg));

  // Save the values
  return InitXYZ(xArg, yArg, zArg);
}

STDMETHODIMP CAGCVector::InitRandomDirection()
{
  // Create a random direction
  return InitFromVector(&Vector::RandomDirection());
}

STDMETHODIMP CAGCVector::InitRandomPosition(float fRadius)
{
  // Create a random position
  return InitFromVector(&Vector::RandomPosition(fRadius));
}

STDMETHODIMP CAGCVector::get_IsZero(VARIANT_BOOL* pbIsZero)
{
  XLock lock(this);
  CLEAROUT(pbIsZero, VARBOOL(m_vector.IsZero()));
  return S_OK;
}

STDMETHODIMP CAGCVector::get_IsEqual(IAGCVector* pVector,
  VARIANT_BOOL* pbIsEqual)
{
  Vector vector;
  RETURN_FAILED(GetRawVector(pVector, &vector));
  XLock lock(this);
  CLEAROUT(pbIsEqual, VARBOOL(vector == m_vector));
  return S_OK;
}

STDMETHODIMP CAGCVector::get_Length(float* pfLength)
{
  XLock lock(this);
  CLEAROUT(pfLength, m_vector.Length());
  return S_OK;
}

STDMETHODIMP CAGCVector::get_LengthSquared(float* pfLengthSquared)
{
  XLock lock(this);
  CLEAROUT(pfLengthSquared, m_vector.LengthSquared());
  return S_OK;
}

STDMETHODIMP CAGCVector::get_OrthogonalVector(IAGCVector** ppResult)
{
  XLock lock(this);
  return CreateResultVector(&m_vector.GetOrthogonalVector(), ppResult);
}

STDMETHODIMP CAGCVector::Add(IAGCVector* pVector, IAGCVector** ppResult)
{
  Vector vector;
  RETURN_FAILED(GetRawVector(pVector, &vector));
  XLock lock(this);
  return CreateResultVector(&(m_vector + vector), ppResult);
}

STDMETHODIMP CAGCVector::Subtract(IAGCVector* pVector, IAGCVector** ppResult)
{
  Vector vector;
  RETURN_FAILED(GetRawVector(pVector, &vector));
  XLock lock(this);
  return CreateResultVector(&(m_vector - vector), ppResult);
}

STDMETHODIMP CAGCVector::Multiply(float f, IAGCVector** ppResult)
{
  XLock lock(this);
  return CreateResultVector(&(m_vector * f), ppResult);
}

STDMETHODIMP CAGCVector::Divide(float f, IAGCVector** ppResult)
{
  XLock lock(this);
  return CreateResultVector(&(m_vector / f), ppResult);
}

STDMETHODIMP CAGCVector::Normalize(IAGCVector** ppResult)
{
  XLock lock(this);
  return CreateResultVector(&m_vector.Normalize(), ppResult);
}

STDMETHODIMP CAGCVector::CrossProduct(IAGCVector* pVector,
  IAGCVector** ppResult)
{
  Vector vector;
  RETURN_FAILED(GetRawVector(pVector, &vector));
  XLock lock(this);
  return CreateResultVector(&::CrossProduct(m_vector, vector), ppResult);
}

STDMETHODIMP CAGCVector::Interpolate(IAGCVector* pVector, float fValue,
  IAGCVector** ppResult)
{
  Vector vector;
  RETURN_FAILED(GetRawVector(pVector, &vector));
  XLock lock(this);
  return CreateResultVector(&::Interpolate(m_vector, vector, fValue), ppResult);
}

STDMETHODIMP CAGCVector::AddInPlace(IAGCVector* pVector)
{
  Vector vector;
  RETURN_FAILED(GetRawVector(pVector, &vector));
  XLock lock(this);
  return InitFromVector(&(m_vector + vector));
}

STDMETHODIMP CAGCVector::SubtractInPlace(IAGCVector* pVector)
{
  Vector vector;
  RETURN_FAILED(GetRawVector(pVector, &vector));
  XLock lock(this);
  return InitFromVector(&(m_vector - vector));
}

STDMETHODIMP CAGCVector::MultiplyInPlace(float f)
{
  XLock lock(this);
  return InitFromVector(&(m_vector * f));
}

STDMETHODIMP CAGCVector::DivideInPlace(float f)
{
  XLock lock(this);
  return InitFromVector(&(m_vector / f));
}

STDMETHODIMP CAGCVector::NormalizeInPlace()
{
  XLock lock(this);
  m_vector.SetNormalize();
  return S_OK;
}

STDMETHODIMP CAGCVector::CrossProductInPlace(IAGCVector* pVector)
{
  Vector vector;
  RETURN_FAILED(GetRawVector(pVector, &vector));
  XLock lock(this);
  return InitFromVector(&::CrossProduct(m_vector, vector));
}

STDMETHODIMP CAGCVector::InterpolateInPlace(IAGCVector* pVector, float fValue)
{
  Vector vector;
  RETURN_FAILED(GetRawVector(pVector, &vector));
  XLock lock(this);
  return InitFromVector(&::Interpolate(m_vector, vector, fValue));
}


/////////////////////////////////////////////////////////////////////////////
// IAGCVectorPrivate Interface Methods

STDMETHODIMP CAGCVector::InitFromVector(const void* pvVector)
{
  Vector vectorTemp;
  const Vector* pVector = reinterpret_cast<const Vector*>(pvVector);
  if (!pVector)
  {
    vectorTemp = Vector::GetZero();
    pVector = &vectorTemp;
  }
  return InitXYZ(pVector->X(), pVector->Y(), pVector->Z());
}

STDMETHODIMP CAGCVector::CopyVectorTo(void* pvVector)
{
  XLock lock(this);
  Vector* pVector = reinterpret_cast<Vector*>(pvVector);
  pVector->SetX(m_vector.X());
  pVector->SetY(m_vector.Y());
  pVector->SetZ(m_vector.Z());
  return S_OK;
}


