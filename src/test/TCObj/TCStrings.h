#pragma once

/////////////////////////////////////////////////////////////////////////////
// TCStrings.h | Declaration of the CTCStrings, which implements the
// CLSID_TCStrings component object.

#include "resource.h"       // main symbols
#include "SrcInc.h"
#include <TCObj.h>


/////////////////////////////////////////////////////////////////////////////
// CTCStrings

class ATL_NO_VTABLE CTCStrings :
  public IDispatchImpl<ITCStrings, &IID_ITCStrings, &LIBID_TCObj>,
  public IDispatchImpl<ITCCollectionPersistHelper, &IID_ITCCollectionPersistHelper, &LIBID_TCObj>,
  public TCPersistPropertyBagImpl<CTCStrings>,
  public IPersistStorageImpl<CTCStrings>,
  public TCPersistStreamInitImpl<CTCStrings>,
  public TCComPropertyClass<CTCStrings>,
  public CComCoClass<CTCStrings, &CLSID_TCStrings>,
  public CComObjectRoot
{
// Declarations
public:
  DECLARE_REGISTRY_RESOURCEID(IDR_TCStrings)
  DECLARE_GET_CONTROLLING_UNKNOWN()

// Category Map
public:
  BEGIN_CATEGORY_MAP(CTCStrings)
    IMPLEMENTED_CATEGORY(CATID_TCObj)
  END_CATEGORY_MAP()

// Interface Map
public:
  BEGIN_COM_MAP(CTCStrings)
    COM_INTERFACE_ENTRY(ITCStrings)
    COM_INTERFACE_ENTRY2(IDispatch, ITCStrings)
    COM_INTERFACE_ENTRY(ITCCollection)
    COM_INTERFACE_ENTRY(ITCCollectionPersistHelper)
    COM_INTERFACE_ENTRY2(IPersist, TCPersistPropertyBagImpl<CTCStrings>)
    COM_INTERFACE_ENTRY(IPersistPropertyBag)
    COM_INTERFACE_ENTRY(IPersistStorage)
    COM_INTERFACE_ENTRY(IPersistStreamInit)
// KGJV - to check: original was 'IPersistStream', changed to 'IPersistStreamInit'
    COM_INTERFACE_ENTRY2(IPersistStreamInit, TCPersistStreamInitImplBase)
    COM_INTERFACE_ENTRY_AUTOAGGREGATE(IID_IMarshal, m_punkMBV.p,
      CLSID_TCMarshalByValue)
  END_COM_MAP()

// Property Map
public:
  BEGIN_PROP_MAP(CTCStrings)
    PROP_ENTRY_EX("Strings", dispid_Collection1, CLSID_NULL,
      IID_ITCCollectionPersistHelper)
  END_PROP_MAP()

// Construction / Destruction
public:
  CTCStrings();

// ITCCollection Interface Methods
public:
  STDMETHODIMP get_Count(long* pnCount);
  STDMETHODIMP get__NewEnum(IUnknown** ppunkEnum);

// ITCStrings Interface Methods
public:
  STDMETHODIMP get_Item(VARIANT* pvIndex, BSTR* pbstr);
  STDMETHODIMP Add(BSTR bstr);
  STDMETHODIMP Remove(VARIANT* pvIndex);
  STDMETHODIMP RemoveAll();
  STDMETHODIMP AddDelimited(BSTR bstrDelimiter, BSTR bstrStrings);
  STDMETHODIMP get_DelimitedItems(BSTR bstrDelimiter, BSTR* pbstrStrings);
  STDMETHODIMP AddStrings(ITCStrings* pStrings);

// ITCCollectionPersistHelper Interface Methods
public:
  STDMETHODIMP put_Collection1(VARIANT* pvarSafeArray);
  STDMETHODIMP get_Collection1(VARIANT* pvarSafeArray);

// Types
protected:
  typedef std::vector<CComBSTR> CStringVector;
  typedef CStringVector::iterator CStringIterator;

// Data Members
public:
  CComPtr<IUnknown> m_punkMBV;
protected:
  CStringVector m_vecStrings;
};


/////////////////////////////////////////////////////////////////////////////