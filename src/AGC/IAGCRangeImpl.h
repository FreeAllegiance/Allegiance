#ifndef __IAGCRangeImpl_h__
#define __IAGCRangeImpl_h__

/////////////////////////////////////////////////////////////////////////////
// IAGCRangeImpl.h : Declaration of the IAGCRangeImpl class template.
//

#include <AGC.h>
#include <..\TCLib\Range.h>
#include <..\TCLib\ObjectLock.h>


/////////////////////////////////////////////////////////////////////////////
// Interface Map Macro
//
// Classes derived from IAGCRangeImpl should include this macro in their
// interface maps.
//
#define COM_INTERFACE_ENTRIES_IAGCRangeImpl()                               \
	  COM_INTERFACE_ENTRY(IAGCRangePrivate)                                   \
	  COM_INTERFACE_ENTRY(IDispatch)                                          \
	  COM_INTERFACE_ENTRY(ISupportErrorInfo)                                  \
    COM_INTERFACE_ENTRY(IPersistStreamInit)                                 \
    COM_INTERFACE_ENTRY(IPersistPropertyBag)                                \
    COM_INTERFACE_ENTRY2(IPersistStream, IPersistStreamInit)                \
    COM_INTERFACE_ENTRY2(IPersist, IPersistStreamInit)                      \
    COM_INTERFACE_ENTRY_AUTOAGGREGATE(IID_IMarshal, m_punkMBV.p,            \
      CLSID_TCMarshalByValue)


/////////////////////////////////////////////////////////////////////////////
// IAGCRangeImpl
//
template <class T, class RT, VARTYPE VT, class ITF, const GUID* plibid>
class ATL_NO_VTABLE IAGCRangeImpl :
  public IDispatchImpl<ITF, &__uuidof(ITF), plibid>,
  public IAGCRangePrivate,
	public ISupportErrorInfo,
  public IPersistStreamInit,
  public IPersistPropertyBag
{
// Types
public:
  typedef TCObjectLock<T>                            XLock;
  typedef range<RT>                                  XRange;
  typedef IDispatchImpl<ITF, &__uuidof(ITF), plibid> IAGCRangeImplBase;
  enum    {c_cbPersistentData = sizeof(DWORD) + sizeof(RT) * 2};

// Overrides
public:
  RT GetVariantValue(VARIANT* pvar);
  void GetPrefixString(LPOLESTR pszPrefix);
  void GetSuffixString(LPOLESTR pszSuffix);
  void GetValueDelimiter(LPOLESTR pszDelim);
  void FormatValueString(const RT& value, LPOLESTR pszOut);
  bool ReadValueString(LPCOLESTR pszValue, RT& value);

// IAGC<*>Range Interface Methods
public:
  STDMETHODIMP put_DisplayString(BSTR bstr)
  {
    // Initialize the object
    RETURN_FAILED(InitNew());

    // Do nothing more if string is empty
    UINT cch = BSTRLen(bstr);
    if (!cch)
      return S_OK;

    // Skip leading whitespace
    while (cch && iswspace(bstr[0]))
    {
      ++bstr;
      --cch;
    }

    // Skip trailing whitespace
    LPOLESTR pszEnd = bstr + cch - 1;
    while (cch && iswspace(pszEnd[0]))
    {
      --pszEnd;
      --cch;
    }

    // Get the prefix, delimiter, and suffix strings
    T* pThis = static_cast<T*>(this);
    OLECHAR szPrefix[_MAX_PATH], szDelim[_MAX_PATH], szSuffix[_MAX_PATH];
    pThis->GetPrefixString  (szPrefix);
    pThis->GetValueDelimiter(szDelim );
    pThis->GetSuffixString  (szSuffix);
    UINT cchPrefix = wcslen(szPrefix);
    UINT cchDelim  = wcslen(szDelim );
    UINT cchSuffix = wcslen(szSuffix);

    // Parse the specified string
    if (cchPrefix && 0 != wcsncmp(bstr, szPrefix, cchPrefix))
      return E_INVALIDARG;
    if (cchSuffix && 0 != wcsncmp(bstr + cch - cchSuffix, szSuffix, cchSuffix))
      return E_INVALIDARG;
    LPCOLESTR pszDelim = wcsstr(bstr, szDelim);
    if (!pszDelim || wcsstr(pszDelim + 1, szDelim))
      return E_INVALIDARG;

    // Skip past the prefix
    bstr += cchPrefix;
    cch -= cchPrefix;

    // Copy the two individual value strings
    UINT cchValue1 = pszDelim - bstr + 1;
    UINT cchValue2 = cch - cchValue1 - cchDelim - cchSuffix + 2;
    LPOLESTR pszValue1 = (LPOLESTR)_alloca(cchValue1 * sizeof(OLECHAR));
    LPOLESTR pszValue2 = (LPOLESTR)_alloca(cchValue2 * sizeof(OLECHAR));
    wcsncpy(pszValue1, bstr, cchValue1);
    wcsncpy(pszValue2, pszDelim + cchDelim, cchValue2);
    pszValue1[cchValue1 - 1] = L'\0';
    pszValue2[cchValue2 - 1] = L'\0';

    // Allow the most-derived class to interpret the strings as values
    RT value1, value2;
    if (!pThis->ReadValueString(pszValue1, value1) ||
        !pThis->ReadValueString(pszValue2, value2))
      return E_INVALIDARG;

    // Initialize the object with the values
    return Init(value1, value2);
  }
  STDMETHODIMP get_DisplayString(BSTR* pbstr)
  {
    // Copy the range and unlock the object
    T* pThis = static_cast<T*>(this);
    XLock lock(pThis);
    XRange range(m_range);
    lock.Unlock();

    // Format the range
    OLECHAR szText[_MAX_PATH];
    pThis->GetPrefixString(szText);
    UINT cch = wcslen(szText);
    LPOLESTR psz = szText + cch;
    pThis->FormatValueString(range.lower(), psz);
    psz += (cch = wcslen(psz)); // Intentional assignment
    pThis->GetValueDelimiter(psz);
    psz += (cch = wcslen(psz)); // Intentional assignment
    pThis->FormatValueString(range.upper(), psz);
    psz += (cch = wcslen(psz)); // Intentional assignment
    pThis->GetSuffixString(psz);
    psz += (cch = wcslen(psz)); // Intentional assignment

    // Create a BSTR from the formatted string
    CComBSTR bstr(szText);

    // Detach the BSTR to the [out] parameter
    CLEAROUT(pbstr, (BSTR)bstr);
    bstr.Detach();

    // Indicate success
    return S_OK;
  }
  STDMETHODIMP Init(RT lower, RT upper)
  {
    XLock lock(static_cast<T*>(this));
    m_range = XRange(lower, upper);
    return S_OK;
  }
  STDMETHODIMP get_Lower(RT* pValue)
  {
    XLock lock(static_cast<T*>(this));
    CLEAROUT(pValue, m_range.lower());
    return S_OK;
  }
  STDMETHODIMP get_Upper(RT* pValue)
  {
    XLock lock(static_cast<T*>(this));
    CLEAROUT(pValue, m_range.upper());
    return S_OK;
  }
  STDMETHODIMP get_IsEmpty(VARIANT_BOOL* pbIsEmpty)
  {
    XLock lock(static_cast<T*>(this));
    CLEAROUT(pbIsEmpty, VARBOOL(m_range.empty()));
    return S_OK;
  }
  STDMETHODIMP get_IntersectsWithValue(RT value,
    VARIANT_BOOL* pbIntersects)
  {
    XLock lock(static_cast<T*>(this));
    CLEAROUT(pbIntersects, VARBOOL(m_range.intersects(value)));
    return S_OK;
  }
  STDMETHODIMP get_IntersectsWithRangeValues(RT value1, RT value2,
    VARIANT_BOOL* pbIntersects)
  {
    XLock lock(static_cast<T*>(this));
    CLEAROUT(pbIntersects, VARBOOL(m_range.intersects(XRange(value1, value2))));
    return S_OK;
  }
  STDMETHODIMP get_IntersectsWithRange(ITF* pRange,
    VARIANT_BOOL* pbIntersects)
  {
    RT value1, value2;
    RETURN_FAILED(pRange->get_Lower(&value1));
    RETURN_FAILED(pRange->get_Upper(&value2));
    return get_IntersectsWithRangeValues(value1, value2, pbIntersects);
  }

// IAGCRangePrivate Interface Methods
public:
  STDMETHODIMP InitFromRange(const void* pvRange)
  {
    const XRange* pRange = reinterpret_cast<const XRange*>(pvRange);
    XLock lock(static_cast<T*>(this));
    m_range = pRange ? *pRange : XRange(RT(), RT());
    return S_OK;
  }
  STDMETHODIMP CopyRangeTo(void* pvRange)
  {
    XRange* pRange = reinterpret_cast<XRange*>(pvRange);
    XLock lock(static_cast<T*>(this));
    *pRange = m_range;
    return S_OK;
  }

// ISupportErrorInfo Interface Methods
public:
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid)
  {
	  static const IID* arr[] = 
	  {
		  &__uuidof(ITF),
	  };
	  for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	  {
		  if (InlineIsEqualGUID(*arr[i],riid))
			  return S_OK;
	  }
	  return S_FALSE;
  }

// IPersist Interface Methods
public:
  STDMETHODIMP GetClassID(CLSID* pClassID)
  {
    __try
    {
      *pClassID = T::GetObjectCLSID();
    }
    __except(1)
    {
      return E_POINTER;
    }
    return S_OK;
  }

// IPersistStreamInit Interface Methods
public:
  STDMETHODIMP IsDirty()
  {
    // We don't maintain a dirty flag
    return S_OK;
  }
  STDMETHODIMP Load(LPSTREAM pStm)
  {
    // Read the number of bytes to be read
    DWORD cbData;
    RETURN_FAILED(pStm->Read(&cbData, sizeof(cbData), NULL));
    if (c_cbPersistentData != cbData)
    {
      ZAssert(c_cbPersistentData == cbData);
      return ERROR_INVALID_DATA;
    }

    // Read each value from the stream
    RT value1, value2;
    RETURN_FAILED(pStm->Read(&value1, sizeof(value1), NULL));
    RETURN_FAILED(pStm->Read(&value2, sizeof(value2), NULL));

    // Save the values
    XLock lock(static_cast<T*>(this));
    m_range = XRange(value1, value2);

    // Indicate success
    return S_OK;
  }
  STDMETHODIMP Save(LPSTREAM pStm, BOOL fClearDirty)
  {
    UNUSED_ALWAYS(fClearDirty);

    // Write out the number of bytes of data to follow
    DWORD cbData = c_cbPersistentData;
    RETURN_FAILED(pStm->Write(&cbData, sizeof(cbData), NULL));

    // Write each float to the stream
    XLock lock(static_cast<T*>(this));
    RT value1(m_range.lower()), value2(m_range.upper());
    lock.Unlock();
    RETURN_FAILED(pStm->Write(&value1, sizeof(value1), NULL));
    RETURN_FAILED(pStm->Write(&value2, sizeof(value2), NULL));

    // Indicate success
    return S_OK;
  }
  STDMETHODIMP GetSizeMax(ULARGE_INTEGER* pCbSize)
  {
    pCbSize->HighPart = 0;
    pCbSize->LowPart  = c_cbPersistentData;
    return S_OK;
  }
  STDMETHODIMP InitNew()
  {
    XLock lock(static_cast<T*>(this));
    m_range = XRange(RT(), RT());
    return S_OK;
  }

// IPersistPropertyBag Interface Methods
public:
  STDMETHODIMP Load(IPropertyBag* pPropBag, IErrorLog* pErrorLog)
  {
    // Load the persistent string value of the range
    CComVariant var(BSTR(NULL));
    RETURN_FAILED(pPropBag->Read(L"Value", &var, pErrorLog));

    // Load the string into the object
    return put_DisplayString(V_BSTR(&var));
  }
  STDMETHODIMP Save(IPropertyBag* pPropBag, BOOL fClearDirty,
    BOOL fSaveAllProperties)
  {
    UNUSED_ALWAYS(fClearDirty);
    UNUSED_ALWAYS(fSaveAllProperties);

    // Create a display string of the range
    CComBSTR bstr;
    RETURN_FAILED(get_DisplayString(&bstr));

    // Save the persistent string value of the range
    return pPropBag->Write(L"Value", &CComVariant(bstr));
  }

// Data Members
protected:
  XRange m_range;
// Data Members
public:  
  CComPtr<IUnknown> m_punkMBV;
};


/////////////////////////////////////////////////////////////////////////////

#endif //__IAGCRangeImpl_h__
