#ifndef __IAGCCollectionImpl_h__
#define __IAGCCollectionImpl_h__

/////////////////////////////////////////////////////////////////////////////
// IAGCCollectionImpl.h : Declaration of the IAGCCollectionImpl class
// template.
//

#include "IAGCCommonImpl.h"


/////////////////////////////////////////////////////////////////////////////
// Interface Map Macro
//
// Classes derived from IAGCCollectionImpl should include this macro in their
// interface maps.
//
#define COM_INTERFACE_ENTRIES_IAGCCollectionImpl()                          \
    COM_INTERFACE_ENTRY(IAGCCollection)                                     \
    COM_INTERFACE_ENTRY(IDispatch)                                          \
    COM_INTERFACE_ENTRIES_IAGCCommonImpl()


/////////////////////////////////////////////////////////////////////////////
//
template <class T, class IGC, class ITF, class ITEMIGC, class ITEMITF,
  const GUID* plibid, class AGCIGC = IGC, class AGCITF = ITF,
  class ITEMAGCIGC = ITEMIGC, class ITEMAGCITF = ITEMITF>
class ATL_NO_VTABLE IAGCCollectionImpl:
  public IAGCCommonImpl<T, IGC, ITF, plibid, AGCIGC, AGCITF>
{
// Types
public:
  typedef IAGCCollectionImpl<T, IGC, ITF, ITEMIGC, ITEMITF, plibid, AGCIGC,
    AGCITF, ITEMAGCIGC, ITEMAGCITF> IAGCCollectionImplBase;

// IAGCCollection Interface Methods
public:
  STDMETHODIMP get_Count(long* pnCount)
  {
    ZAssert(GetIGC());
    CLEAROUT(pnCount, (long)GetIGC()->n());
    return S_OK;
  }

  STDMETHODIMP get__NewEnum(IUnknown** ppunkEnum)
  {
    ZAssert(GetIGC());

    // Create a new CComEnum enumerator object
    typedef CComObject<CComEnum<IEnumVARIANT, &IID_IEnumVARIANT, VARIANT,
      _Copy<VARIANT> > > CEnum;
    CEnum* pEnum = new CEnum;
    ZAssert(NULL != pEnum);

    // Get the number of items in the collection
    long cTotal = GetIGC()->n();
    
    // Create a temporary array of variants
    //std::vector<CComVariant> vecTemp(cTotal);  //Imago replaced /w below line 8/5/09
    CComVariant* pargTemp = new CComVariant[cTotal];

    // Get the IDispatch of each item of the collection
    long l2 = 0;
    for (CComVariant i(0L); V_I4(&i) < cTotal; ++V_I4(&i))
    {
      CComPtr<ITEMITF> spItem;
      RETURN_FAILED(get_Item(&i, &spItem));
      //vecTemp[V_I4(&i)] = (IDispatch*)spItem; //Imago replaced /w below line 8/5/09
      pargTemp[l2] = (IDispatch*)spItem;
      l2++;
    }

    // Initialize enumerator object with the temporary CComVariant vector
// VS.Net 2003 port - accomodate change in iterators under VC.Net 200x (see 'breaking changes' in vsnet doc)
   
    // Imago removed this 2003 port (kgjv), VS.Net 2003 projects were removed anyways.
    // The reason I'm here: we can't dereference iterators with invalid pointers, so this code has been redone
    // to not use iterators and this fixes server crashes for anything that used this collection! 8/5/09

    HRESULT hr = pEnum->Init(&pargTemp[0], &pargTemp[cTotal], NULL, AtlFlagCopy);

    delete [] pargTemp;

//#endif
    if (SUCCEEDED(hr))
      hr = pEnum->QueryInterface(IID_IEnumVARIANT, (void**)ppunkEnum);
    if (FAILED(hr))
      delete pEnum;

    // Return the last result
    return hr;
  }

// ITF Interface Methods
public:
  STDMETHODIMP get_Item(VARIANT* pvIndex, ITEMITF** ppItem)
  {
    // Initialize the [out] parameter
    CLEAROUT(ppItem, (ITEMITF*)NULL);

    // Attempt to convert the specified VARIANT to a long
    CComVariant var;
    RETURN_FAILED(VariantChangeType(&var, pvIndex, 0, VT_I4));
    long iIndex(V_I4(&var));

    // Bounds check
    ZAssert(GetIGC());
    long iMax = GetIGC()->n();
    if (0 > iIndex || iIndex >= iMax)
      return E_INVALIDARG; 

    // Get the item's IGC pointer from the item's host pointer
    ITEMAGCIGC* pIGC =
      Host2Igc<ITEMIGC, ITEMAGCIGC>((*GetIGC())[iIndex]->data());

    // Return the item
    return GetAGCGlobal()->GetAGCObject(pIGC, __uuidof(ITEMITF),
      (void**)ppItem);
  }
};


/////////////////////////////////////////////////////////////////////////////

#endif //__IAGCCollectionImpl_h__

