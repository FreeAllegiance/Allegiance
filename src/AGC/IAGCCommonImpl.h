#ifndef __IAGCCommonImpl_h__
#define __IAGCCommonImpl_h__

/*-------------------------------------------------------------------------
 * AGC\AGCCommon.h
 * 
 * CAGCCommon is inherited by all AGC Objects (except low-level ones like 
 * AGCVector).  
 *
 * Note that AGCBase is inherited by most AGC objects (except low-level ones,
 * AGC Collections, and AGCGame).
 * 
 * Owner: 
 * 
 * Copyright 1986-2000 Microsoft Corporation, All Rights Reserved
 *-----------------------------------------------------------------------*/

#include <AGC.h>


/////////////////////////////////////////////////////////////////////////////
// Conversion Function Templates

template <class HOSTIGC, class AGCIGC>
inline AGCIGC* Host2Igc(HOSTIGC* p)
{
  return p;
}

template <class HOSTIGC, class AGCIGC>
inline HOSTIGC* Igc2Host(AGCIGC* p)
{
  return p;
}


/////////////////////////////////////////////////////////////////////////////
// Interface Map Macro
//
// Classes derived from IAGCCommonImpl should include this macro in their
// interface maps.
//
#define COM_INTERFACE_ENTRIES_IAGCCommonImpl()                              \
    COM_INTERFACE_ENTRY(IAGCCommon)                                         \
    COM_INTERFACE_ENTRY(IAGCPrivate)


/////////////////////////////////////////////////////////////////////////////
// Type Declaration Macro
//
#define DECLARE_AGC_TYPE(type)                                              \
  LPCOLESTR GetType()                                                       \
  {                                                                         \
    return OLESTR(#type);                                                   \
  }


/////////////////////////////////////////////////////////////////////////////
//
template <class T, class IGC, class ITF, const GUID* plibid, class AGCIGC = IGC, class AGCITF = ITF>
class ATL_NO_VTABLE IAGCCommonImpl :
  public IDispatchImpl<ITF, &__uuidof(ITF), plibid>,
  public IAGCPrivate,
  public CComObjectRootEx<CComSingleThreadModel>
{
// Declarations
public:
  DECLARE_AGC_TYPE(agc)

// Types
public:
  typedef IAGCCommonImpl<T, IGC, ITF, plibid, AGCIGC, AGCITF> IAGCCommonImplBase;

// Construction / Destruction
public:
  IAGCCommonImpl() :
    m_pIGC(NULL)
  {
  }
  virtual ~IAGCCommonImpl()
  {
    GetAGCGlobal()->RemoveAGCObject(m_pIGC, false);
  }
  void Init(AGCIGC* pIGC)
  {
    // Save the specified Igc pointer
    m_pIGC = pIGC;
  }

// Attributes
public:
  AGCIGC* GetIGC()
  {
    return m_pIGC;
  }

  IGC* GetHostIGC()
  {
    return Igc2Host<IGC, AGCIGC>(m_pIGC);
  }

// Implementation
public:
	static void WINAPI ObjectMain(bool bStarting)
  {
    if (bStarting)
    {
      // Register our creator function for the AGCITF's IID
      IAGCGlobalPtr sp(GetAGCGlobal());
      sp->RegisterObjectCreator(__uuidof(AGCITF), T::CreateAGCObject);

      // Register the same creator function for the IGC's IID, if different
      if (__uuidof(ITF) != __uuidof(AGCITF))
        sp->RegisterObjectCreator(__uuidof(ITF), T::CreateAGCObject);
    }
  }
  static HRESULT WINAPI CreateAGCObject(void* pIgc, REFIID riid, void** ppUnk)
  {
    CComObject<T>* pObj = NULL;
    RETURN_FAILED(pObj->CreateInstance(&pObj));
    pObj->Init(reinterpret_cast<AGCIGC*>(pIgc));
    HRESULT hr = pObj->QueryInterface(riid, ppUnk);
    if (FAILED(hr))
      delete pObj;
    return hr;
  }

// IAGCCommon Interface Methods
public:
  ///////////////////////////////////////////////////////////////////////////
  // Name: get_UniqueID
  //
  // Description: Gets this object's unique AGC id number ID.
  //
  // Remarks: We need to be able to generate a unique ID for all AGC objects.
  // IGC creates unique ID's for most IbaseIGC objects.  Unfortunately, many
  // IGC objects do not derive from IbaseIGC, like ImissionIGC and all the
  // collections. 
  //
  // Note: This is overrided in IAGCBaseImpl. This version of get_UniqueID()
  // is for  AGC objects that represent IGC objects not derived from
  // IbaseIgc (such as ImissionIgc and all of the IGC collections).
  //
/*  STDMETHODIMP get_UniqueID(AGCUniqueID* pObjectID)
  {
    debugf("IAGCCommonImpl::get_UniqueID:()\n");
    ZAssert(GetIGC());
    CLEAROUT(pObjectID, (AGCUniqueID)GetIGC()); // return IGC ptr for now, TODO: make better
    return S_OK;
  }
*/
  STDMETHODIMP get_Type(BSTR* pbstrType)
  {
    // Get a pointer to the most-derived type
    T* pT = static_cast<T*>(this);

    // Get the object's type string
    CComBSTR bstrType(pT->GetType());

    // Detach the string to the [out] parameter
    CLEAROUT(pbstrType, (BSTR)bstrType);
    bstrType.Detach();

    // Indicate success
    return S_OK;
  }

// IAGCPrivate Interface Methods
public:
  STDMETHODIMP_(void*) GetIGCVoid()
  {
    const void* p = m_pIGC;
    return const_cast<void*>(p);
  } 

// Data Members
protected:
  AGCIGC* m_pIGC;
};


/////////////////////////////////////////////////////////////////////////////

#endif // !__IAGCCommonImpl_h__
