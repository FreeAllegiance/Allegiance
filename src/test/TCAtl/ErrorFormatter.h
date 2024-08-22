#pragma once

/////////////////////////////////////////////////////////////////////////////
// ErrorFormatter.h : Declaration of the TCErrorFormatter class template and
// of TCErrorFormatterBase from which it is derived.
//


/////////////////////////////////////////////////////////////////////////////
// TCErrorFormatterBase
//
class ATL_NO_VTABLE TCErrorFormatterBase
{
// Implementation
public:
  static HRESULT FormattedErrorV(REFCLSID clsid, REFIID iid, UINT idFmt,
    va_list argptr)
  {
    // Load the formatting string
    TCHAR szFmt[_MAX_PATH * 2];
    if (!LoadString(_Module.GetResourceInstance(), idFmt, szFmt,
      sizeofArray(szFmt)))
        return E_UNEXPECTED;

    // Format the string
    TCHAR szText[_MAX_PATH * 4];
    if (-1 == _vsntprintf(szText, sizeofArray(szText), szFmt, argptr))
      return E_OUTOFMEMORY;

    // Delegate to the Atl method
    return AtlReportError(clsid, szText, iid, MAKE_HRESULT(1, FACILITY_ITF,
      idFmt));
  }
  static HRESULT FormattedBSTRV(BSTR* pbstr, UINT idFmt, va_list argptr)
  {
    // Do nothing if the specified [out] parameter is NULL
    if (!pbstr)
      return S_OK;

    // Load the formatting string (always in Unicode under NT)
    LPCWSTR pszFmt;
    #if !defined(_UNICODE) && !defined(UNICODE)
      if (IsWin9x())
      {
        char szFmt[_MAX_PATH * 2];
        if (!LoadString(_Module.GetResourceInstance(), idFmt, szFmt,
          sizeof(szFmt)))
            return E_UNEXPECTED;
        USES_CONVERSION;
        pszFmt = A2CW(szFmt);
      }
      else
    #endif
      {
        WCHAR szFmt[_MAX_PATH * 2];
        if (!LoadStringW(_Module.GetResourceInstance(), idFmt, szFmt,
          sizeofArray(szFmt)))
            return E_UNEXPECTED;
        pszFmt = szFmt;
      }

    // Format the string
    WCHAR szText[_MAX_PATH * 4];
    int cch = _vsnwprintf(szText, sizeofArray(szText), pszFmt, argptr);
    if (-1 == cch)
      return E_OUTOFMEMORY;

    // Allocate a BSTR
    CComBSTR bstr(cch, szText);
    CLEAROUT(pbstr, (BSTR)bstr);
    return S_OK;
  }
};


/////////////////////////////////////////////////////////////////////////////
// TCErrorFormatter<T, const IID*>
//
template <class T, const IID* piid>
class ATL_NO_VTABLE TCErrorFormatter :
  protected TCErrorFormatterBase
{
// Operations
public:
  HRESULT FormattedError(UINT idFmt, ...)
  {
    va_list argptr;
    va_start(argptr, idFmt);
    HRESULT hr = TCErrorFormatterBase::FormattedErrorV(
      static_cast<T*>(this)->GetObjectCLSID(), *piid, idFmt, argptr);
    va_end(argptr);
    return hr;
  }
  HRESULT FormattedErrorIID(REFIID iid, UINT idFmt, ...)
  {
    va_list argptr;
    va_start(argptr, idFmt);
    HRESULT hr = TCErrorFormatterBase::FormattedErrorV(
      static_cast<T*>(this)->GetObjectCLSID(), iid, idFmt, argptr);
    va_end(argptr);
    return hr;
  }
  static HRESULT FormattedBSTR(BSTR* pbstr, UINT idFmt, ...)
  {
    va_list argptr;
    va_start(argptr, idFmt);
    HRESULT hr = TCErrorFormatterBase::FormattedBSTRV(pbstr, idFmt, argptr);
    va_end(argptr);
    return hr;
  }
};


/////////////////////////////////////////////////////////////////////////////