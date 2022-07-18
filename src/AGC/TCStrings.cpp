/////////////////////////////////////////////////////////////////////////////
// TCStrings.cpp | Implementation of the CTCStrings class.
//

#include "pch.h"
#include "TCStrings.h"


/////////////////////////////////////////////////////////////////////////////
// CTCStrings

TC_OBJECT_EXTERN_IMPL(CTCStrings)


/////////////////////////////////////////////////////////////////////////////
// Construction / Destruction

CTCStrings::CTCStrings()
{
}


/////////////////////////////////////////////////////////////////////////////
// ITCCollection Interface Methods

STDMETHODIMP CTCStrings::get_Count(long* pnCount)
{
  // Lock the object
  CLock lock(this);

  // Copy the size of the vector to the specified [out] parameter
  CLEAROUT(pnCount, (long)m_vecStrings.size());

  // Indicate success
  return S_OK;
}

STDMETHODIMP CTCStrings::get__NewEnum(IUnknown** ppunkEnum)
{
  // Clear the [out] parameter
  CLEAROUT(ppunkEnum, (IUnknown*)NULL);

  // Create a new CComEnum enumerator object
  typedef CComObject<CComEnum<IEnumVARIANT, &IID_IEnumVARIANT, VARIANT,
    _Copy<VARIANT> > > CEnum;
  CEnum* pEnum = new CEnum;
  ZAssert(NULL != pEnum);

  // Lock the object
  CLock lock(this);

  // Copy the elements of stored vector to a temporary CComVariant vector
  std::vector<CComVariant> vecTemp(m_vecStrings.size(), CComVariant());
  for (UINT i = 0; i < m_vecStrings.size(); ++i)
    vecTemp[i] = m_vecStrings[i].m_T.Copy();

  // Initialize enumerator object with the temporary CComVariant vector
  // VS.Net 2003 port - accomodate change in iterators under VC.Net 200x (see 'breaking changes' in vsnet doc)

   //Imago 8/5/09 Is this code even used?  If so it may be incorrect...
   // see IAGCCollectionImpl.h(81)
#if _MSC_VER >= 1310
  HRESULT hr = pEnum->Init(&(*vecTemp.begin()), &(*vecTemp.end()), NULL, AtlFlagCopy);
#else
  HRESULT hr = pEnum->Init(vecTemp.begin(), vecTemp.end(), NULL, AtlFlagCopy);
#endif
  if (SUCCEEDED(hr))
    hr = pEnum->QueryInterface(IID_IEnumVARIANT, (void**)ppunkEnum);
  if (FAILED(hr))
    delete pEnum;

  // Return the last result
  return hr;
}


/////////////////////////////////////////////////////////////////////////////
// ITCStrings Interface Methods

STDMETHODIMP CTCStrings::get_Item(VARIANT* pvIndex, BSTR* pbstr)
{
  // Initialize the [out] parameter
  CLEAROUT(pbstr, (BSTR)NULL);

  // Attempt to convert the specified VARIANT to a long
  CComVariant var;
  HRESULT hr = VariantChangeType(&var, pvIndex, 0, VT_I4);
  if (FAILED(hr))
    return hr;

  // Lock the object
  CLock lock(this);

  // Ensure that the specified index is in range
  if (V_I4(&var) < 0 || V_I4(&var) >= (int)m_vecStrings.size())
    return E_INVALIDARG;

  // Copy the item at the specified index into the vector
  *pbstr = m_vecStrings[V_I4(&var)].m_T.Copy();

  // Indicate success
  return S_OK;
}

STDMETHODIMP CTCStrings::Add(BSTR bstr)
{
  // Lock the object
  CLock lock(this);

  // Add the string to the vector
  m_vecStrings.push_back(CComBSTR(bstr));

  // Indicate success
  return S_OK;
}

STDMETHODIMP CTCStrings::Remove(VARIANT* pvIndex)
{
  // Attempt to convert the specified VARIANT to a long
  CComVariant var;
  RETURN_FAILED(VariantChangeType(&var, pvIndex, 0, VT_I4));

  // Lock the object
  CLock lock(this);

  // Ensure that the specified index is in range
  if (V_I4(&var) < 0 || V_I4(&var) >= (int)m_vecStrings.size())
    return E_INVALIDARG;

  // Use the long as an index into the vector
  m_vecStrings.erase(m_vecStrings.begin() + V_I4(&var));

  // Indicate success
  return S_OK;
}

STDMETHODIMP CTCStrings::RemoveAll()
{
  // Lock the object
  CLock lock(this);

  // Clear the vector
  m_vecStrings.clear();

  // Indicate success
  return S_OK;
}

STDMETHODIMP CTCStrings::AddDelimited(BSTR bstrDelimiter, BSTR bstrStrings)
{
  // Get the character length of the specified delimiter
  UINT cchDelim = SysStringLen(bstrDelimiter);

  // Ensure that the bstrDelimiter parameter is not an empty string
  if (!cchDelim)
    return E_INVALIDARG;

  // Get a pointer to the specified strings
  LPCOLESTR psz = bstrStrings;
  while (psz)
  {
    // Find the delimiter in the specified string
    LPCOLESTR pszFound = wcsstr(psz, bstrDelimiter);
    if (pszFound)
    {
      m_vecStrings.push_back(CComBSTR(pszFound - psz, psz));
      psz = pszFound + cchDelim;
    }
    else
    {
      m_vecStrings.push_back(CComBSTR(psz));
      psz = NULL;
    }
  };

  // Indicate success
  return S_OK;
}

STDMETHODIMP CTCStrings::get_DelimitedItems(BSTR bstrDelimiter,
  BSTR* pbstrStrings)
{
  // Initialize the [out] parameter
  CLEAROUT(pbstrStrings, (BSTR)NULL);

  // Get the character length of the specified delimiter
  UINT cchDelim = SysStringLen(bstrDelimiter);

  // Ensure that the bstrDelimiter parameter is not an empty string
  if (!cchDelim)
    return E_INVALIDARG;

  // Loop thru the collection and calculate the total number of characters
  UINT cchTotal = 1;
  UINT nCount = m_vecStrings.size();
  for (UINT i = 0; i < nCount; ++i)
  {
    if (i)
      cchTotal += cchDelim;
    cchTotal += m_vecStrings[i].m_T.Length();
  }

  // Allocate a buffer for the entire array
  LPOLESTR psz = new OLECHAR[cchTotal];
  *psz = OLESTR('\0');

  // Loop thru the collection and concatenate the strings
  for (UINT i = 0; i < nCount; ++i)
  {
    if (i)
      wcscat(psz, bstrDelimiter);
    if (m_vecStrings[i].m_T.Length())
      wcscat(psz, CComBSTR(m_vecStrings[i]));     
  }

  // Allocate a BSTR and assign it to the [out] parameter
  *pbstrStrings = SysAllocStringLen(psz, cchTotal - 1);

  // Delete the temporary string
  delete [] psz;

  // Indicate success
  return S_OK;
}

STDMETHODIMP CTCStrings::AddStrings(ITCStrings* pStrings)
{
  // Lock the object
  Lock();

  HRESULT hr = S_OK;
  BSTR bstr = NULL;
  __try
  {
    // Loop thru the strings of the specified collection
    long nCount = 0;
    if (!pStrings || FAILED(hr = pStrings->get_Count(&nCount)))
    {
      VARIANT v;
      VariantInit(&v);
      V_VT(&v) = VT_I4;
      V_I4(&v) = 0;
      for (; V_I4(&v) < nCount; ++V_I4(&v))
      {
        if (FAILED(hr = pStrings->get_Item(&v, &bstr)))
          break;
        if (FAILED(hr = Add(bstr)))
          break;
      }
    }
  }
  __except(1)
  {
    hr = E_POINTER;
  }

  // Unlock the object
  Unlock();

  // Free the BSTR
  if (bstr)
    SysFreeString(bstr);

  // Return the last result
  return hr;
}


/////////////////////////////////////////////////////////////////////////////
// ITCCollectionPersistHelper Interface Methods

STDMETHODIMP CTCStrings::put_Collection1(VARIANT* pvarSafeArray)
{
  // Lock the object
  CLock lock(this);

  // Validate the specified VARIANT
  VARTYPE vt = V_VT(pvarSafeArray);
  if ((VT_BSTR | VT_ARRAY) != vt)
  {
    m_vecStrings.clear();
    return E_INVALIDARG;
  }

  // Get the array pointer and check dimensions of the array
  SAFEARRAY* psa = V_ARRAY(pvarSafeArray);
  if (1 != SafeArrayGetDim(psa))
  {
    m_vecStrings.clear();
    return E_INVALIDARG;
  }

  // Get array bounds.      
  LONG cElements, lLBound, lUBound;
  HRESULT hr = SafeArrayGetLBound(psa, 1, &lLBound);
  if (SUCCEEDED(hr))
    hr = SafeArrayGetUBound(psa, 1, &lUBound);
  if (FAILED(hr))
  {
    m_vecStrings.clear();
    return hr;
  }
  cElements = lUBound - lLBound + 1;

  // Get a pointer to the elements of the array.
  BSTR* pbstr = NULL;
  hr = SafeArrayAccessData(psa, (void **)&pbstr);
  if (FAILED(hr))
  {
    m_vecStrings.clear();
    return hr;
  }

  // This object will call SafeArrayUnaccessData when it is destructed
  TCSafeArrayAccess saa(psa);
  
  // Erase the current contents of the vector
  m_vecStrings.clear();

  // Add each BSTR to the vector
  m_vecStrings.resize(cElements);
  for (long i = 0; i < cElements; i++)
    m_vecStrings[i] = pbstr[i];

  // Indicate success
  return S_OK;
}

STDMETHODIMP CTCStrings::get_Collection1(VARIANT* pvarSafeArray)
{
  // Initialize the specified VARIANT
  __try
  {
    VariantInit(pvarSafeArray);
  }
  __except(1)
  {
    return E_POINTER;
  }

  // Lock the object
  Lock();

  HRESULT hr = S_OK;
  __try
  {
    // Create a safe array to copy the BSTRs into
    long nCount = m_vecStrings.size();
    SAFEARRAY* psa = SafeArrayCreateVector(VT_BSTR, 0, nCount);
    if (!psa)
      hr = E_OUTOFMEMORY;
    else
    {
      // Attach the new safe array to the VARIANT
      V_VT(pvarSafeArray) = VT_ARRAY | VT_BSTR;
      V_ARRAY(pvarSafeArray) = psa;

      // Copy each BSTR to the safe array
      for (long i = 0; i < nCount; ++i)
        SafeArrayPutElement(psa, &i, m_vecStrings[i].m_T.Copy());
    }
  }
  __except(1)
  {
    hr = RPC_E_SERVERFAULT;
  }
  
  // Unlock the object
  Unlock();

  // Clear the [out] variant if an error occurred
  if (FAILED(hr))
    VariantClear(pvarSafeArray);

  // Return the last result
  return hr;
}

