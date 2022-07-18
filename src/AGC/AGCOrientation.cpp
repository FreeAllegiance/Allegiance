/////////////////////////////////////////////////////////////////////////////
// AGCOrientation.cpp : Implementation of CAGCOrientation
//

#include "pch.h"
#include "AGCVector.h"
#include "AGCOrientation.h"


/////////////////////////////////////////////////////////////////////////////
// COrientation

TC_OBJECT_EXTERN_IMPL(CAGCOrientation)


/////////////////////////////////////////////////////////////////////////////
// Implementation

HRESULT CAGCOrientation::GetRawOrientation(IAGCOrientation* pOrientation,
  Orientation* pOrientationRaw)
{
  __try
  {
    if (!pOrientation)
      return E_POINTER;
    IAGCOrientationPrivate* pPrivate = NULL;
    RETURN_FAILED(pOrientation->QueryInterface(__uuidof(pPrivate),
      (void**)&pPrivate));
    ZAssert(pPrivate);
    if (!pPrivate)
      return E_INVALIDARG;
    HRESULT hr = pPrivate->CopyOrientationTo(pOrientationRaw);
    pPrivate->Release();
    return hr;
  }
  __except(1)
  {
    return E_POINTER;
  }
}

HRESULT CAGCOrientation::CreateResultVector(const Vector* pVectorRaw,
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

STDMETHODIMP CAGCOrientation::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IAGCOrientation
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

STDMETHODIMP CAGCOrientation::GetClassID(CLSID* pClassID)
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

STDMETHODIMP CAGCOrientation::IsDirty()
{
  // Return dirty flag
  XLock lock(this);
  return m_bDirty ? S_OK : S_FALSE;
}

STDMETHODIMP CAGCOrientation::Load(LPSTREAM pStm)
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
  float r[3][3];
  for (int i = 0; i < DIMENSIONS; ++i)
    RETURN_FAILED(pStm->Read(&r[i/3][i%3], sizeof(float), NULL));

  // Save the values
  Orientation orientation(r);
  return InitFromOrientation(&orientation);
}

STDMETHODIMP CAGCOrientation::Save(LPSTREAM pStm, BOOL fClearDirty)
{
  // Write out the number of floats being written
  long cDims = DIMENSIONS;
  RETURN_FAILED(pStm->Write(&cDims, sizeof(cDims), NULL));

  // Write each float to the stream
  XLock lock(this);
  for (int i = 0; i < DIMENSIONS; ++i)
  {
    float r = m_orientation[i/3][i%3];
    RETURN_FAILED(pStm->Write(&r, sizeof(r), NULL));
  }

  // Clear the dirty flag, if specified
  if (fClearDirty)
    m_bDirty = false;

  // Indicate success
  return S_OK;
}

STDMETHODIMP CAGCOrientation::GetSizeMax(ULARGE_INTEGER* pCbSize)
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

STDMETHODIMP CAGCOrientation::InitNew( void)
{
  // Initialize the vector object
  XLock lock(this);
  m_orientation.Reset();

  // Indicate success
  return S_OK;
}


/////////////////////////////////////////////////////////////////////////////
// IAGCOrientation Interface Methods

STDMETHODIMP CAGCOrientation::InitCopy(IAGCOrientation* pOrientation)
{
  Orientation orientation;
  RETURN_FAILED(GetRawOrientation(pOrientation, &orientation));

  // Save the values
  return InitFromOrientation(&orientation);
}

STDMETHODIMP CAGCOrientation::get_Forward(IAGCVector** ppVector)
{
  XLock lock(this);
  return CreateResultVector(&m_orientation.GetForward(), ppVector);
}

STDMETHODIMP CAGCOrientation::get_Backward(IAGCVector** ppVector)
{
  XLock lock(this);
  return CreateResultVector(&m_orientation.GetBackward(), ppVector);
}

STDMETHODIMP CAGCOrientation::get_Up(IAGCVector** ppVector)
{
  XLock lock(this);
  return CreateResultVector(&m_orientation.GetUp(), ppVector);
}

STDMETHODIMP CAGCOrientation::get_Right(IAGCVector** ppVector)
{
  XLock lock(this);
  return CreateResultVector(&m_orientation.GetRight(), ppVector);
}

STDMETHODIMP CAGCOrientation::get_IsEqual(IAGCOrientation* pOrientation,
  VARIANT_BOOL* pbIsEqual)
{
  // Get the raw orientation
  Orientation orientation;
  RETURN_FAILED(GetRawOrientation(pOrientation, &orientation));

  // Determine absolute equality
  XLock lock(this);
  // mdvalley: type now explicit size_t
  const size_t cbData = sizeof(*m_orientation[0]) * DIMENSIONS;
  bool bEqual = !memcmp(m_orientation[0], orientation[0], cbData);

  // Initialize the [out] parameter
  CLEAROUT(pbIsEqual, VARBOOL(bEqual));
  return S_OK;
}

STDMETHODIMP CAGCOrientation::get_IsRoughlyEqual(
  IAGCOrientation* pOrientation, VARIANT_BOOL* pbIsEqual)
{
  // Get the raw orientation
  Orientation orientation;
  RETURN_FAILED(GetRawOrientation(pOrientation, &orientation));

  // Determine 'fuzzy' equality
  XLock lock(this);
  bool bEqual = m_orientation == orientation;

  // Initialize the [out] parameter
  CLEAROUT(pbIsEqual, VARBOOL(bEqual));
  return S_OK;
}


/////////////////////////////////////////////////////////////////////////////
// IAGCOrientationPrivate Interface Methods

STDMETHODIMP CAGCOrientation::InitFromOrientation(const void* pvOrientation)
{
  Orientation orientationTemp;
  const Orientation* pOrientation =
    reinterpret_cast<const Orientation*>(pvOrientation);
  if (!pOrientation)
    pOrientation = &orientationTemp;

  // Determine absolute equality
  XLock lock(this);
  // mdvalley: type now explicit
  const size_t cbData = sizeof(*m_orientation[0]) * DIMENSIONS;
  bool bEqual = !memcmp(m_orientation[0], (*pOrientation)[0], cbData);
  if (!bEqual)
  {
    m_orientation = *pOrientation;
    m_bDirty = true;
  }

  // Indicate success
  return S_OK;
}

STDMETHODIMP CAGCOrientation::CopyOrientationTo(void* pvOrientation)
{
  XLock lock(this);
  Orientation* pOrientation = reinterpret_cast<Orientation*>(pvOrientation);
  *pOrientation = m_orientation;
  return S_OK;
}


