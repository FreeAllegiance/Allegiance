/////////////////////////////////////////////////////////////////////////////
// Pigs.cpp : Implementation of the CPigs class.

#include "pch.h"
#include <TCLib.h>
#include <..\TCAtl\ObjectMap.h>

#include "Pigs.h"


/////////////////////////////////////////////////////////////////////////////
// CPigs

TC_OBJECT_EXTERN_NON_CREATEABLE_IMPL(CPigs)


/////////////////////////////////////////////////////////////////////////////
// Construction / Destruction

CPigs::CPigs()
{
}

HRESULT CPigs::FinalConstruct()
{
  // Indicate success
  return S_OK;
}

void CPigs::FinalRelease()
{
}


/////////////////////////////////////////////////////////////////////////////
// Operations

void CPigs::AddPig(DWORD dwGITCookie, _bstr_t bstrName)
{
  XLock lock(this);

  // Ensure that the specified pig is not already in either collection
  ZAssert(m_mapByCookie.end() == m_mapByCookie.find(dwGITCookie));
  ZAssert(m_mapByName.end() == m_mapByName.find(bstrName));

  // Add the association to the collections
  m_mapByCookie.insert(std::make_pair(dwGITCookie, bstrName));
  m_mapByName.insert(std::make_pair(bstrName, dwGITCookie));
}

void CPigs::RemovePig(DWORD dwGITCookie)
{
  XLock lock(this);

  // Find the specified pig in the collection
  XMapByCookieIt itCookie = m_mapByCookie.find(dwGITCookie);
  ZAssert(m_mapByCookie.end() != itCookie);

  // Find the associated name in the other collection
  XMapByNameIt itName = m_mapByName.find(itCookie->second);
  ZAssert(m_mapByName.end() != itName);
  ZAssert(itCookie->first == itName->second);

  // Remove the pig from both collections
  m_mapByCookie.erase(itCookie);
  m_mapByName.erase(itName);
}

void CPigs::RenamePig(DWORD dwGITCookie, _bstr_t bstrName)
{
  XLock lock(this);

  // Find the specified pig in the collection
  XMapByCookieIt itCookie = m_mapByCookie.find(dwGITCookie);
  ZAssert(m_mapByCookie.end() != itCookie);

  // Find the pig's old name in the other collection
  XMapByNameIt itName = m_mapByName.find(itCookie->second);
  ZAssert(m_mapByName.end() != itName);
  ZAssert(itCookie->first == itName->second);

  // Remove the pig from the 'by name' collection
  m_mapByName.erase(itName);

  // Add the new association to the 'by name' collection
  m_mapByName.insert(std::make_pair(bstrName, dwGITCookie));

  // Change the value of in the 'by cookie' collection
  itCookie->second = bstrName;
}


/////////////////////////////////////////////////////////////////////////////
// Implementation


/////////////////////////////////////////////////////////////////////////////
// ISupportErrorInfo Interface Methods

STDMETHODIMP CPigs::InterfaceSupportsErrorInfo(REFIID riid)
{
  static const IID* arr[] = 
  {
    &IID_IPigs,
  };
  for (int i = 0; i < sizeofArray(arr); ++i)
  {
    if (InlineIsEqualGUID(*arr[i], riid))
      return S_OK;
  }
  return S_FALSE;
}


/////////////////////////////////////////////////////////////////////////////
// ITCCollection Interface Methods

STDMETHODIMP CPigs::get_Count(long* pnCount)
{
  XLock lock(this);

  ZAssert(m_mapByCookie.size() == m_mapByName.size());
  CLEAROUT(pnCount, (long)m_mapByCookie.size());
  return S_OK;
}

STDMETHODIMP CPigs::get__NewEnum(IUnknown** ppunkEnum)
{
  // Clear the [out] parameter
  CLEAROUT(ppunkEnum, (IUnknown*)NULL);

  // Lock this object for the duration of the copy operation
  XLock lock(this);

  // Copy the elements of 'by name' map to a temporary CComVariant vector
  ZAssert(m_mapByCookie.size() == m_mapByName.size());
  CComVariant var;
  // KG- switch to array instead of STL vector
  // we could keep STL vector and use CComEnumOnSTL later but something asserts at debug doing so.
  //std::vector<CComVariant> vecTemp(m_mapByName.size(), CComVariant());
  //std::vector<CComVariant>::iterator itVec = vecTemp.begin();
  CComVariant* vecTemp = new CComVariant[m_mapByName.size()]; // allocate the array
  int itVec = 0; // will be the total size at the end

  for (XMapByNameIt it = m_mapByName.begin(); it != m_mapByName.end(); ++it)
  {
    // Get an apartment-safe interface pointer for the located object
    IPigPtr spPig;
    RETURN_FAILED(GetEngine().GetInterfaceFromGlobal(it->second,
      IID_IPig, (void**)&spPig));

    // Initialize the variant with the IUnknown pointer
    var = (IDispatch*)spPig;

    // Place it into the pre-created slot of the temporary vector
	// KG- array instead of vector
    //*itVec++ = var;
	vecTemp[itVec++] = var;
  }
  // KG-  ZAssert(itVec == m_mapByName.size());

  // Unlock the object after the copy
  lock.Unlock();

  // Create a new CComEnum enumerator object
  typedef CComObject<CComEnum<IEnumVARIANT, &IID_IEnumVARIANT, VARIANT,
    _Copy<VARIANT> > > CEnum;
  CEnum* pEnum = new CEnum;
  ZAssert(NULL != pEnum);

  // Initialize enumerator object with the temporary CComVariant vector
  // VS.Net 2003 port - accomodate change in iterators under VC.Net 200x (see 'breaking changes' in vsnet doc)
  // VS.Net 2010 port - switch to array instead of STL vector

  //HRESULT hr = pEnum->Init(&(*vecTemp.begin()), &(*vecTemp.end()), NULL, AtlFlagCopy);
  HRESULT hr = pEnum->Init(&vecTemp[0], &vecTemp[itVec], NULL, AtlFlagCopy);
  
  // kg- we copied the content, safe to delete
  delete [] vecTemp;

  if (SUCCEEDED(hr))
    hr = pEnum->QueryInterface(IID_IEnumVARIANT, (void**)ppunkEnum);
  if (FAILED(hr))
    delete pEnum;

  // Return the last result
  return hr;
}


/////////////////////////////////////////////////////////////////////////////
// IPigs Interface Methods

HRESULT CPigs::get_Item(VARIANT* pvIndex, IPig** ppPig)
{
  ZAssert(m_mapByCookie.size() == m_mapByName.size());

  // Initialize the [out] parameter
  CLEAROUT(ppPig, (IPig*)NULL);

  // Ensure that the specified index is a VT_BSTR
  if (VT_BSTR != V_VT(pvIndex))
    return DISP_E_TYPEMISMATCH;

  // Lookup the specified name in the 'by name' map
  DWORD dwGIT = Find(V_BSTR(pvIndex));
  if (!dwGIT)
    return S_FALSE;

  // Get an apartment-safe interface pointer for the located object
  return GetEngine().GetInterfaceFromGlobal(dwGIT, IID_IPig, (void**)ppPig);
}

