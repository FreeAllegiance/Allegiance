#ifndef __RangeValueImpl_h__
#define __RangeValueImpl_h__

/////////////////////////////////////////////////////////////////////////////
// RangeValueImpl.h | Declaration of the ITCRangeValueImpl template class.
//


/////////////////////////////////////////////////////////////////////////////
// Range Value Macros

#define DECLARE_RANGE_VALUE_INTERPRET_CHANGE(fn)                            \
public:                                                                     \
  bool InterpretChange(bool bChanged)                                       \
  {                                                                         \
    return fn##(bChanged);                                                  \
  }

#define BEGIN_RANGE_VALUE_MAP(T)                                            \
public:                                                                     \
  typedef HRESULT (T::*XRangeValueApplyProc)(long, long);                   \
  struct XRangeValueEntry                                                   \
  {                                                                         \
    DISPID               dispid;                                            \
    const IID*           piid;                                              \
    long                 nValueMin;                                         \
    long                 nValueMax;                                         \
    long                 nValueDefault;                                     \
    long                 nGranularity;                                      \
    long                 nTickMarks;                                        \
    XRangeValueApplyProc pfnApply;                                          \
  };                                                                        \
  static UINT GetPageFieldTable(const XRangeValueEntry** ppTable)           \
  {                                                                         \
    static const XRangeValueEntry table[] =                                 \
    {

#define RANGE_VALUE_ENTRY_EX(dispid, iid, minv, maxv, defv, gran, tics,     \
  fnApply)                                                                  \
      {dispid, &iid, minv, maxv, defv, gran, tics,                          \
        (static_cast<XRangeValueApplyProc>(fnApply))},

#define RANGE_VALUE_ENTRY(name, minv, maxv, defv, gran, tics)               \
      {TCDISPID_##name, &IID_ITC##name, minv, maxv, defv, gran, tics,       \
        (static_cast<XRangeValueApplyProc>(OnApply_##name))},

#define END_RANGE_VALUE_MAP()                                               \
      {0, 0, NULL, NULL, NULL}                                              \
    };                                                                      \
    if (ppTable)                                                            \
      *ppTable = table;                                                     \
    return sizeofArray(table) - 1;                                          \
  }


/////////////////////////////////////////////////////////////////////////////
// ITCRangeValueImpl

template <class T, class _I, const IID* piid, const GUID* plibid,
  WORD wMajor = 1, WORD wMinor = 0>
class ATL_NO_VTABLE ITCRangeValueImpl :
  public IDispatchImpl<_I, piid, plibid, wMajor, wMinor>
{
// Construction
public:
  ITCRangeValueImpl() : m_pEntry(NULL)
  {
    // Get the derived class pointer
    T* pThis = static_cast<T*>(this);

    // Get the table of range value
    const T::XRangeValueEntry* pTable;
    UINT nCount = pThis->GetPageFieldTable(&pTable);

    // Find the piid in the table
    for (UINT i = 0; i < nCount; i++)
    {
      if (*pTable[i].piid == *piid)
      {
        // Save the range value entry
        m_pEntry = pTable + i;

        // Initialize the value to the default
        T::XRangeValueEntry* pEntry = (T::XRangeValueEntry*)m_pEntry;
        m_nValue = pEntry->nValueDefault;
        break;
      }
    }

    // Ensure that the piid was found in the table
    ZAssert(m_pEntry);
  }

// Overrides
public:
  bool InterpretChange(bool bChanged)
  {
    return bChanged;
  }

// ITCRangeValue Interface Methods
public:
  STDMETHOD(get_MinValue)(long* pnValue)
  {
    ZAssert(m_pEntry);
    T::XRangeValueEntry* pEntry = (T::XRangeValueEntry*)m_pEntry;
    CLEAROUT(pnValue, pEntry->nValueMin);
    return S_OK;
  }

  STDMETHOD(get_MaxValue)(long* pnValue)
  {
    ZAssert(m_pEntry);
    T::XRangeValueEntry* pEntry = (T::XRangeValueEntry*)m_pEntry;
    CLEAROUT(pnValue, pEntry->nValueMax);
    return S_OK;
  }

  STDMETHOD(get_DefaultValue)(long* pnValue)
  {
    ZAssert(m_pEntry);
    T::XRangeValueEntry* pEntry = (T::XRangeValueEntry*)m_pEntry;
    CLEAROUT(pnValue, pEntry->nValueDefault);
    return S_OK;
  }

  STDMETHOD(get_Granularity)(long* pnGranularity)
  {
    ZAssert(m_pEntry);
    T::XRangeValueEntry* pEntry = (T::XRangeValueEntry*)m_pEntry;
    CLEAROUT(pnGranularity, pEntry->nGranularity);
    return S_OK;
  }

  STDMETHOD(get_TickMarks)(long* pnTickMarks)
  {
    ZAssert(m_pEntry);
    T::XRangeValueEntry* pEntry = (T::XRangeValueEntry*)m_pEntry;

    // Interpret the tick mark count value
    long nTickMarks = pEntry->nTickMarks;
    if (-1 == nTickMarks)
      nTickMarks =
        DetermineTickMarks(pEntry->nValueMax + 1 - pEntry->nValueMin);
    CLEAROUT(pnTickMarks, nTickMarks);
    return S_OK;
  }

  STDMETHOD(put_RangeValue)(long nValue)
  {
    ZAssert(m_pEntry);
    T::XRangeValueEntry* pEntry = (T::XRangeValueEntry*)m_pEntry;

    // Load the specified value into a variant
    CComVariant var(nValue);

    // Call Invoke to set the current value
    DISPID dispid = pEntry->dispid;
    HRESULT hr = CComDispatchDriver::PutProperty(this, dispid, &var);
    if (FAILED(hr))
    {
      ATLTRACE2("ITCRangeValueImpl<%hs, %hs>::put_RangeValue(): ",
        TCTypeName(T), TCTypeName(_I));
      ATLTRACE1("Invoke failed on DISPID %08X\n", dispid);
    }

    // Return the last HRESULT
    return hr;
  }

  STDMETHOD(get_RangeValue)(long* pnValue)
  {
    ZAssert(m_pEntry);
    T::XRangeValueEntry* pEntry = (T::XRangeValueEntry*)m_pEntry;

    // Call Invoke to get the current value
    CComVariant var;
    DISPID dispid = pEntry->dispid;
    HRESULT hr = CComDispatchDriver::GetProperty(this, dispid, &var);
    if (FAILED(hr))
    {
      ATLTRACE2("ITCRangeValueImpl<%hs, %hs>::get_RangeValue(): ",
        TCTypeName(T), TCTypeName(_I));
      ATLTRACE1("Invoke failed on DISPID %08X\n", dispid);
      return hr;
    }

    // Ensure that the type is a long
    if (FAILED(hr = var.ChangeType(VT_I4)))
      return hr;

    // Copy the value to the specified [out] parameter
    CLEAROUT(pnValue, V_I4(&var));

    // Indicate success
    return S_OK;
  }

  STDMETHOD(put_CurrentValue)(long nValue)
  {
    ZAssert(m_pEntry);
    T::XRangeValueEntry* pEntry = (T::XRangeValueEntry*)m_pEntry;

    // Validate the specified parameter
    if (nValue < pEntry->nValueMin || nValue > pEntry->nValueMax)
      return E_INVALIDARG;

    // Get the derived class pointer
    T* pThis = static_cast<T*>(this);

    // Lock the object
    pThis->Lock();

    // Set the property value
    long nValuePrev = m_nValue;
    HRESULT hr = TCComPropertyPut(pThis, m_nValue, nValue);
    if (SUCCEEDED(hr) && pThis->InterpretChange(nValuePrev != m_nValue))
    {
      // Apply the changed value
      __try
      {
        T::XRangeValueApplyProc pfnApply = pEntry->pfnApply;
        hr = (pThis->*pfnApply)(nValue, nValuePrev);
      }
      __except(1)
      {
        hr = RPC_E_SERVERFAULT;
      }

      // Unlock the object
      pThis->Unlock();

      // Fire the property change notification, if value was applied
      if (S_OK == hr)
        pThis->FireOnChanged(pEntry->dispid);
    }
    else
    {
      // Unlock the object
      pThis->Unlock();
    }

    // Return the last result
    return hr;
  }

  STDMETHOD(get_CurrentValue)(long* pnValue)
  {
    // Get the derived class pointer
    T* pThis = static_cast<T*>(this);

    // Get the property value
    return TCComPropertyGet(pThis, pnValue, m_nValue);
  }

// Implementation
protected:
  static long DetermineTickMarks(long nSteps)
  {
    // Declare an array of the preferred tick mark counts
    const static long nFreqs[] = {16, 16, 10, 12, 8};
    const static long nFreqCount = sizeofArray(nFreqs);
    ZAssert(nFreqCount);

    // Use 1 if specified step count <= the largest preferred tick mark count
    if (nSteps <= nFreqs[0])
      return nSteps;

    // Loop thru preferred frequencies
    long nBestMod = nFreqs[0], nBestFreq = nFreqs[0];
    for (int i = 1; i < nFreqCount; ++i)
    {
      long nMod = nSteps % nFreqs[i];
      if (0 == nMod)
        return nFreqs[i];

      // Save this remainder and frequency if the best so far
      if (nMod < nBestMod)
      {
        nBestMod = nMod;
        nBestFreq = nFreqs[i];
      }
    }

    // If no even divisor amongst the preferred frequencies, check all others
    for (long iFreq = nFreqs[0]; iFreq > nFreqs[nFreqCount - 1]; --iFreq)
    {
      long nMod = nSteps % iFreq;
      if (0 == nMod)
        return nFreqs[i];

      // Save this remainder and frequency if the best so far
      if (nMod < nBestMod)
      {
        nBestMod = nMod;
        nBestFreq = nFreqs[i];
      }
    }

    // If still no even divisor, return the best match
    return nBestFreq;
  }

// Data Members
protected:
  long m_nValue;
  const void* m_pEntry;
};


/////////////////////////////////////////////////////////////////////////////

#endif // !__RangeValueImpl_h__
