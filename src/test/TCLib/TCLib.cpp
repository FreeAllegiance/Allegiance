/////////////////////////////////////////////////////////////////////////////
// TCLib.cpp | Implementation of extensions to the C++ Runtime Library.
// See TCLib.h for the intended usage of this file in your project.
//

#include "pch.h"
#include "..\Inc\TCLib.h"

// VS.Net 2003 port: StdComponentCategoriesMgr missing from VS.Net <comdef.h>
// 1st workaround: add the definition here ... but need to investigate further why it's missing.
#if _MSC_VER >= 1310
class __declspec(uuid("0002e005-0000-0000-c000-000000000046")) StdComponentCategoriesMgr;
#endif
/////////////////////////////////////////////////////////////////////////////
// Macro Support Functions

#ifdef _DEBUG
  void _cdecl TCTrace(LPCTSTR lpszFormat, ...)
  {
    va_list args;
    va_start(args, lpszFormat);

    int nBuf;
    TCHAR szBuffer[512];

    nBuf = _vstprintf(szBuffer, lpszFormat, args);
    ZAssert(nBuf < sizeof(szBuffer));

    USES_CONVERSION;
    ZDebugOutputImpl(T2CA(szBuffer));
    va_end(args);
  }
#endif // _DEBUG


/////////////////////////////////////////////////////////////////////////////
// Component Category Registration

HRESULT RegisterComponentCategory(REFGUID catid, LPCOLESTR pszDesc)
{
  ICatRegisterPtr pcr;
  HRESULT hr = pcr.CreateInstance(__uuidof(StdComponentCategoriesMgr));
  if (FAILED(hr))
  {
    // Since not all systems have the category manager installed, we'll allow
    // the registration to succeed even though we didn't register our
    // categories.  If you really want to register categories on a system
    // without the category manager, you can either manually add the
    // appropriate entries to your registry script (.rgs), or you can
    // redistribute comcat.dll.
    return S_OK;
  }

  // Determine the LCID for US English
  const LANGID langid = MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US);
  const LCID lcid = MAKELCID(langid, SORT_DEFAULT);

  // Register the component category
  CATEGORYINFO catinfo;
  catinfo.catid = catid;
  catinfo.lcid = lcid;
  wcscpy(catinfo.szDescription, pszDesc);
  return pcr->RegisterCategories(1, &catinfo);
}

HRESULT RegisterComponentCategory(LPCOLESTR pszCLSID, LPCOLESTR pszDesc)
{
  GUID catid;
  HRESULT hr = CLSIDFromString(const_cast<LPOLESTR>(pszCLSID), &catid);
  return SUCCEEDED(hr) ? RegisterComponentCategory(catid, pszDesc) : hr;
}

HRESULT UnregisterComponentCategory(REFGUID catid)
{
  ICatRegisterPtr pcr;
  HRESULT hr = pcr.CreateInstance(__uuidof(StdComponentCategoriesMgr));
  if (FAILED(hr))
  {
    // Since not all systems have the category manager installed, we'll allow
    // the unregistration to succeed even though we didn't register our
    // categories.  If you really want to unregister categories on a system
    // without the category manager, you can either manually add the
    // appropriate entries to your registry script (.rgs), or you can
    // redistribute comcat.dll.
    return S_OK;
  }

  // Determine the LCID for US English
  const LANGID langid = MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US);
  const LCID lcid = MAKELCID(langid, SORT_DEFAULT);

  // Unregister the component category
  CATID catids[1];
  catids[0] = catid;
  return pcr->UnRegisterCategories(1, catids);
}

HRESULT UnregisterComponentCategory(LPCOLESTR pszCLSID)
{
  GUID catid;
  HRESULT hr = CLSIDFromString(const_cast<LPOLESTR>(pszCLSID), &catid);
  return SUCCEEDED(hr) ? UnregisterComponentCategory(catid) : hr;
}


/////////////////////////////////////////////////////////////////////////////
// Date and Time Retrieval / Conversion

DATE GetVariantDateTime()
{
  // Get the current date/time in Universal Coordinated Time (UCT)
  SYSTEMTIME st;
  GetSystemTime(&st);

  // Convert the date/time to VariantTime
  DATE time = 0;
  _VERIFYE(SystemTimeToVariantTime(&st, &time));
  return time;
}

DATE GetVariantDate()
{
  // Get the current date/time in Universal Coordinated Time (UCT)
  SYSTEMTIME st;
  GetSystemTime(&st);

  // Clear the time fields (00:00 midnight by convention)
  st.wHour = 0;
  st.wMinute = 0;
  st.wSecond = 0;
  st.wMilliseconds = 0;

  // Convert the date to VariantTime
  DATE time = 0;
  _VERIFYE(SystemTimeToVariantTime(&st, &time));
  return time;
}

DATE GetVariantTime()
{
  // Get the current date/time in Universal Coordinated Time (UCT)
  SYSTEMTIME st;
  GetSystemTime(&st);

  // Clear the date fields (30 December, 1899 by convention)
  st.wYear = 1899;
  st.wMonth = 12;
  st.wDay = 30;

  // Convert the time to VariantTime
  DATE time = 0;
  _VERIFYE(SystemTimeToVariantTime(&st, &time));
  return time;
}

DATE GetLocalVariantDateTime()
{
  // Get the current date/time in the local time zone
  SYSTEMTIME st;
  GetLocalTime(&st);

  // Convert the date/time to VariantTime
  DATE time = 0;
  _VERIFYE(SystemTimeToVariantTime(&st, &time));
  return time;
}

DATE GetLocalVariantDate()
{
  // Get the current date/time in Universal Coordinated Time (UCT)
  SYSTEMTIME st;
  GetLocalTime(&st);

  // Clear the time fields (00:00 midnight by convention)
  st.wHour = 0;
  st.wMinute = 0;
  st.wSecond = 0;
  st.wMilliseconds = 0;

  // Convert the date to VariantTime
  DATE time = 0;
  _VERIFYE(SystemTimeToVariantTime(&st, &time));
  return time;
}

DATE GetLocalVariantTime()
{
  // Get the current date/time in Universal Coordinated Time (UCT)
  SYSTEMTIME st;
  GetLocalTime(&st);

  // Clear the date fields (30 December, 1899 by convention)
  st.wYear = 1899;
  st.wMonth = 12;
  st.wDay = 30;

  // Convert the time to VariantTime
  DATE time = 0;
  _VERIFYE(SystemTimeToVariantTime(&st, &time));
  return time;
}

bool TCSystemTimeToTzSpecificLocalTime(TIME_ZONE_INFORMATION* pTimeZone,
  SYSTEMTIME* pUniversalTime, SYSTEMTIME* pLocalTime)
{
  // Convert the SYSTEMTIME from UTC to the local time zone
  if (!::SystemTimeToTzSpecificLocalTime(NULL, pUniversalTime, pLocalTime))
  {
    TIME_ZONE_INFORMATION tz;
    DWORD zone = GetTimeZoneInformation(&tz);
    if (zone != TIME_ZONE_ID_UNKNOWN)
    {
      LONG lBias = tz.Bias;
      if (zone == TIME_ZONE_ID_STANDARD)
        lBias += tz.StandardBias;
      else //TIME_ZONE_ID_DAYLIGHT:
        lBias += tz.DaylightBias;

      FILETIME ft;
      _VERIFYE(::SystemTimeToFileTime(pUniversalTime, &ft));
      __int64 time64 = ((__int64)ft.dwHighDateTime << 32) | ft.dwLowDateTime;
      __int64 bias = (__int64)lBias * (__int64)600000000;
      time64 -= bias;
      ft.dwHighDateTime = (DWORD)(time64 >> 32);
      ft.dwLowDateTime = (DWORD)time64;
      _VERIFYE(::FileTimeToSystemTime(&ft, pLocalTime));
    }
  }
  return true;
}

bool VariantTimeToLocalTime(DATE date, SYSTEMTIME* psystime)
{
  // Convert the VARIANT time to a SYSTEMTIME structure
  SYSTEMTIME systimeUTC, systime;
  if (!VariantTimeToSystemTime(date, &systimeUTC))
    return false;

  // Convert the SYSTEMTIME from UTC to the local time zone
  _VERIFYE(TCSystemTimeToTzSpecificLocalTime(NULL, &systimeUTC, &systime));

  // Copy the SYSTEMTIME structure to the specified [out] parameter
  __try
  {
    CopyMemory(psystime, &systime, sizeof(*psystime));
    return true;
  }
  __except(1)
  {
    return false;
  }
}

bool FileTimeToVariantTime(FILETIME* pft, DATE* pdate)
{
  SYSTEMTIME systime;
  if (!FileTimeToSystemTime(pft, &systime))
    return false;
  return !!SystemTimeToVariantTime(&systime, pdate);
}

int FormatDateTime(SYSTEMTIME* psystime, LPTSTR psz, int nLength)
{
  __try
  {
    // Get the number of characters needed for the date and time strings
    LCID lcid = GetThreadLocale();
    int cchDate, cchTime;
    cchDate = GetDateFormat(lcid, DATE_SHORTDATE, psystime, NULL, NULL, 0);
    cchTime = GetTimeFormat(lcid, 0, psystime, NULL, NULL, 0);

    // Compute buffer size for date, space, time, and null-terminator
    int cchTotal = cchDate + cchTime;

    // Return buffer size, if specified length is zero
    if (!nLength)
      return cchTotal;

    // Allocate a buffer for date, space, time, and null-terminator
    LPTSTR pszFmt = LPTSTR(_alloca(cchTotal * sizeof(TCHAR)));

    // Convert the date and time to strings
    GetDateFormat(lcid, DATE_SHORTDATE, psystime, NULL, pszFmt, cchDate);
    GetTimeFormat(lcid, 0, psystime, NULL, pszFmt + cchDate, cchTime);

    // Ensure that a space separates the two strings
    pszFmt[cchDate - 1] = TEXT(' ');

    // Copy the formatted string to the specified buffer
    lstrcpyn(psz, pszFmt, nLength);

    // Indicate success
    return cchTotal;
  }
  __except(1)
  {
    _TRACE0("FormatDateTime(): Caught an unknown exception\n");
    return 0;
  }
}

int FormatVariantDateTimeAsLocal(DATE date, LPTSTR psz, int nLength)
{
  // Convert the specified DATE to a localized SYSTEMTIME
  SYSTEMTIME systime;
  if (!VariantTimeToLocalTime(date, &systime))
    return 0;

  // Format the date and time string
  return FormatDateTime(&systime, psz, nLength);
}

int FormatDate(SYSTEMTIME* psystime, LPTSTR psz, int nLength)
{
  __try
  {
    // Get the number of characters needed for the date string
    LCID lcid = GetThreadLocale();
    int cchDate;
    cchDate = GetDateFormat(lcid, DATE_SHORTDATE, psystime, NULL, NULL, 0);

    // Return buffer size, if specified length is zero
    if (!nLength)
      return cchDate;

    // Allocate a buffer for date and null-terminator
    LPTSTR pszFmt = LPTSTR(_alloca(cchDate * sizeof(TCHAR)));

    // Convert the date to a string
    GetDateFormat(lcid, DATE_SHORTDATE, psystime, NULL, pszFmt, cchDate);

    // Copy the formatted string to the specified buffer
    lstrcpyn(psz, pszFmt, nLength);

    // Indicate success
    return cchDate;
  }
  __except(1)
  {
    _TRACE0("FormatDate(): Caught an unknown exception\n");
    return 0;
  }
}

int FormatVariantDateAsLocal(DATE date, LPTSTR psz, int nLength)
{
  // Convert the specified DATE to a localized SYSTEMTIME
  SYSTEMTIME systime;
  if (!VariantTimeToLocalTime(date, &systime))
    return 0;

  // Format the date and time string
  return FormatDate(&systime, psz, nLength);
}

int FormatTime(SYSTEMTIME* psystime, LPTSTR psz, int nLength)
{
  __try
  {
    // Get the number of characters needed for the time string
    LCID lcid = GetThreadLocale();
    int cchTime;
    cchTime = GetTimeFormat(lcid, 0, psystime, NULL, NULL, 0);

    // Return buffer size, if specified length is zero
    if (!nLength)
      return cchTime;

    // Allocate a buffer for time and null-terminator
    LPTSTR pszFmt = LPTSTR(_alloca(cchTime * sizeof(TCHAR)));

    // Convert the time to a string
    GetTimeFormat(lcid, 0, psystime, NULL, pszFmt, cchTime);

    // Copy the formatted string to the specified buffer
    lstrcpyn(psz, pszFmt, nLength);

    // Indicate success
    return cchTime;
  }
  __except(1)
  {
    _TRACE0("FormatTime(): Caught an unknown exception\n");
    return 0;
  }
}

int FormatVariantTimeAsLocal(DATE date, LPTSTR psz, int nLength)
{
  // Convert the specified DATE to a localized SYSTEMTIME
  SYSTEMTIME systime;
  if (!VariantTimeToLocalTime(date, &systime))
    return 0;

  // Format the date and time string
  return FormatTime(&systime, psz, nLength);
}

int FormatTimeSpan(SYSTEMTIME* psystime, LPTSTR psz, int nLength)
{
  __try
  {
    // Get the number of characters needed for the time string
    LCID lcid = GetThreadLocale();

    int cchTime;
    DWORD dwFlags = TIME_NOTIMEMARKER | TIME_FORCE24HOURFORMAT;
    cchTime = GetTimeFormat(lcid, dwFlags, psystime, NULL, NULL, 0);

    // Return buffer size, if specified length is zero
    if (!nLength)
      return cchTime;

    // Allocate a buffer for time and null-terminator
    LPTSTR pszFmt = LPTSTR(_alloca(cchTime * sizeof(TCHAR)));

    // Convert the time to a string
    GetTimeFormat(lcid, dwFlags, psystime, NULL, pszFmt, cchTime);

    // Copy the formatted string to the specified buffer
    lstrcpyn(psz, pszFmt, nLength);

    // Indicate success
    return cchTime;
  }
  __except(1)
  {
    _TRACE0("FormatTimeSpan(): Caught an unknown exception\n");
    return 0;
  }
}

int FormatVariantTimeSpan(DATE date, LPTSTR psz, int nLength)
{
  // Convert the specified DATE to a SYSTEMTIME
  SYSTEMTIME systime;
  if (!VariantTimeToSystemTime(date, &systime))
    return 0;

  // Get the number of days in the time span and compute extra hours
  int nDays = int(date);
  int nNegative = (nDays < 0) ? -1 : 1;
  int nHoursExtra = (nDays * nNegative) * 24;
  if ((nHoursExtra + systime.wHour) > USHRT_MAX)
    return 0; 

  // Add the extra hours into the systime.wHour field
  systime.wHour += nHoursExtra;

  // Format the date and time string
  return FormatTimeSpan(&systime, psz, nLength);
}



/////////////////////////////////////////////////////////////////////////////
// MFC Date and Time Wrappers

#ifdef _AFX

  CString FormatDateTime(SYSTEMTIME* psystime)
  {
    CString str;
    FormatDateTime(psystime, str);
    return str;
  }

  BOOL FormatDateTime(SYSTEMTIME* psystime, CString& str)
  {
    // Empty the specified string
    str.Empty();

    // Get the number of characters needed to format the date and time
    int cch = FormatDateTime(psystime, NULL, 0);

    // Format the date and time
    int cchFmt = FormatDateTime(psystime, str.GetBuffer(cch), cch);
    str.ReleaseBuffer();

    // Return the previous result
    return 0 != cchFmt;
  }

  CString FormatVariantDateTimeAsLocal(DATE date)
  {
    CString str;
    FormatVariantDateTimeAsLocal(date, str);
    return str;
  }

  BOOL FormatVariantDateTimeAsLocal(DATE date, CString& str)
  {
    // Convert the specified DATE to a localized SYSTEMTIME
    SYSTEMTIME systime;
    if (!VariantTimeToLocalTime(date, &systime))
      return FALSE;

    // Format the date and time string
    return FormatDateTime(&systime, str);
  }

  CString FormatDate(SYSTEMTIME* psystime)
  {
    CString str;
    FormatDate(psystime, str);
    return str;
  }

  BOOL FormatDate(SYSTEMTIME* psystime, CString& str)
  {
    // Empty the specified string
    str.Empty();

    // Get the number of characters needed to format the date
    int cch = FormatDate(psystime, NULL, 0);

    // Format the date
    int cchFmt = FormatDate(psystime, str.GetBuffer(cch), cch);
    str.ReleaseBuffer();

    // Return the previous result
    return 0 != cchFmt;
  }

  CString FormatVariantDateAsLocal(DATE date)
  {
    CString str;
    FormatVariantDateAsLocal(date, str);
    return str;
  }

  BOOL FormatVariantDateAsLocal(DATE date, CString& str)
  {
    // Convert the specified DATE to a localized SYSTEMTIME
    SYSTEMTIME systime;
    if (!VariantTimeToLocalTime(date, &systime))
      return FALSE;

    // Format the date and time string
    return FormatDate(&systime, str);
  }

  CString FormatTime(SYSTEMTIME* psystime)
  {
    CString str;
    FormatTime(psystime, str);
    return str;
  }

  BOOL FormatTime(SYSTEMTIME* psystime, CString& str)
  {
    // Empty the specified string
    str.Empty();

    // Get the number of characters needed to format the time
    int cch = FormatTime(psystime, NULL, 0);

    // Format the time
    int cchFmt = FormatTime(psystime, str.GetBuffer(cch), cch);
    str.ReleaseBuffer();

    // Return the previous result
    return 0 != cchFmt;
  }

  CString FormatVariantTimeAsLocal(DATE date)
  {
    CString str;
    FormatVariantTimeAsLocal(date, str);
    return str;
  }

  BOOL FormatVariantTimeAsLocal(DATE date, CString& str)
  {
    // Convert the specified DATE to a localized SYSTEMTIME
    SYSTEMTIME systime;
    if (!VariantTimeToLocalTime(date, &systime))
      return FALSE;

    // Format the date and time string
    return FormatTime(&systime, str);
  }

  CString FormatTimeSpan(SYSTEMTIME* psystime)
  {
    CString str;
    FormatTimeSpan(psystime, str);
    return str;
  }

  BOOL FormatTimeSpan(SYSTEMTIME* psystime, CString& str)
  {
    // Empty the specified string
    str.Empty();

    // Get the number of characters needed to format the time span
    int cch = FormatTimeSpan(psystime, NULL, 0);

    // Format the time span
    int cchFmt = FormatTimeSpan(psystime, str.GetBuffer(cch), cch);
    str.ReleaseBuffer();

    // Return the previous result
    return 0 != cchFmt;
  }

  CString FormatVariantTimeSpan(DATE date)
  {
    CString str;
    FormatVariantTimeSpan(date, str);
    return str;
  }

  BOOL FormatVariantTimeSpan(DATE date, CString& str)
  {
    // Empty the specified string
    str.Empty();

    // Get the number of characters needed to format the time span
    int cch = FormatVariantTimeSpan(date, NULL, 0);

    // Format the time span
    int cchFmt = FormatVariantTimeSpan(date, str.GetBuffer(cch), cch);
    str.ReleaseBuffer();

    // Return the previous result
    return 0 != cchFmt;
  }

#endif // _AFX


/////////////////////////////////////////////////////////////////////////////
// Object Persistence

HRESULT SaveObjectToNewStream(IUnknown* punk, IStream** ppStream)
{
  // Initialize the [out] parameter
  CLEAROUT(ppStream, (IStream*)NULL);

  // Do nothing if specified object is NULL
  if (!punk)
    return E_POINTER;

  // Get the IPersistStream(Init) interface of the specified object
  IPersistStreamPtr sppstm;
  HRESULT hr = TCSafeQI(punk, &sppstm);
  if (E_NOINTERFACE == hr)
    hr = punk->QueryInterface(IID_IPersistStreamInit, (void**)&sppstm);
  RETURN_FAILED(hr);

  // Create an IStream in memory
  IStreamPtr spstm;
  RETURN_FAILED(CreateStreamOnHGlobal(NULL, TRUE, &spstm));

  // Preset the size of the IStream buffer
  ULARGE_INTEGER cbSize = {0, 0};
  if (SUCCEEDED(sppstm->GetSizeMax(&cbSize)))
    spstm->SetSize(cbSize);

  // Save the specified options object to the IStream
  RETURN_FAILED(sppstm->Save(spstm, FALSE))

  // Return the new IStream object
  *ppStream = spstm;
  spstm.Detach();

  // Indicate success
  return S_OK;
}

HRESULT LoadObjectFromStream(IUnknown* punk, IStream* pStream)
{
  // Do nothing if specified object is NULL
  if (!punk)
    return E_POINTER;

  // Get the IPersistStream(Init) interface of the specified object
  IPersistStreamPtr sppstm;
  HRESULT hr = TCSafeQI(punk, &sppstm);
  if (E_NOINTERFACE == hr)
    hr = punk->QueryInterface(IID_IPersistStreamInit, (void**)&sppstm);
  RETURN_FAILED(hr);

  // Ensure that the specified pStream is valid
  IStreamPtr spStream;
  RETURN_FAILED(TCSafeQI(pStream, &spStream));

  // Rewind the specified stream
  LARGE_INTEGER lSeek = {0, 0};
  RETURN_FAILED(spStream->Seek(lSeek, STREAM_SEEK_SET, NULL));

  // Load the current Call Options object from the IStream
  return sppstm->Load(spStream);
}

HRESULT CopyObjectThruStream(IUnknown* punkDest, IUnknown* punkSrc)
{
  // Do nothing if either interface pointer is NULL
  if (!punkDest || !punkSrc)
    return E_POINTER;

  IStreamPtr pstm;
  RETURN_FAILED(SaveObjectToNewStream(punkSrc, &pstm));
  return LoadObjectFromStream(punkDest, pstm);
}

HRESULT CreateCopyObjectThruStream(IUnknown* punk, REFIID riid, void** ppv)
{
  // Initialize the [out] parameter
  CLEAROUT(ppv, (void*)NULL);

  // Do nothing else if specified object is NULL
  if (!punk)
    return S_OK;

  // Get the CLSID of the specified object
  IPersistPtr spPersist;
  RETURN_FAILED(TCSafeQI(punk, &spPersist));

  // Get the CLSID of the specified object
  CLSID clsid;
  RETURN_FAILED(spPersist->GetClassID(&clsid));
  if (CLSID_NULL == clsid)
    return REGDB_E_CLASSNOTREG;

  // Save the specified object to a new stream
  IStreamPtr spstm;
  RETURN_FAILED(SaveObjectToNewStream(punk, &spstm));

  // Create a new instance of the specified object
  IUnknownPtr spunkDest;
  RETURN_FAILED(spunkDest.CreateInstance(clsid));

  // Initialize the new instance from the stream data
  RETURN_FAILED(LoadObjectFromStream(spunkDest, spstm));

  // QueryInterface the new instance for the specified IID
  return spunkDest->QueryInterface(riid, ppv);
}

HRESULT CompareObjectsThruStream(IUnknown* punk1, IUnknown* punk2)
{
  // Objects are equal if both interfaces are NULL
  if (!punk1 && !punk2)
    return S_OK;

  // CLSID's must be equal
  HRESULT hr = CompareObjectCLSIDs(punk1, punk2);
  if (S_OK != hr)
    return hr;

  // Save the first object to a new stream
  IStreamPtr spstm1;
  RETURN_FAILED(SaveObjectToNewStream(punk1, &spstm1));

  // Delegate to CompareObjectsThruThisStream
  return CompareObjectsThruThisStream(spstm1, punk2);
}

HRESULT CompareObjectsThruThisStream(IStream* pstm1, IUnknown* punk2)
{
  // Objects are equal if both interfaces are NULL
  if (!pstm1 && !punk2)
    return S_OK;

  // Object are inequal if either interface is NULL
  if (!pstm1 || !punk2)
    return S_FALSE;

  // Save the second object to a new stream
  IStreamPtr spstm2;
  RETURN_FAILED(SaveObjectToNewStream(punk2, &spstm2));

  // Size of each stream must be equal
  LARGE_INTEGER liMove = {0, 0};
  ULARGE_INTEGER uliPos1 = {0, 0}, uliPos2 = {0, 0};
  RETURN_FAILED(pstm1->Seek(liMove, STREAM_SEEK_END, &uliPos1));
  RETURN_FAILED(spstm2->Seek(liMove, STREAM_SEEK_END, &uliPos2));
  ZAssert(!uliPos1.HighPart && !uliPos2.HighPart);
  if (uliPos1.LowPart != uliPos2.LowPart)
    return S_FALSE;

  // Rewind each stream to the beginning
  RETURN_FAILED(pstm1->Seek(liMove, STREAM_SEEK_SET, NULL));
  RETURN_FAILED(spstm2->Seek(liMove, STREAM_SEEK_SET, NULL));

  // Compare the data of each stream in chunks
  const int cbBuffer = 4096;
  BYTE pBuffer1[cbBuffer], pBuffer2[cbBuffer];
  ULONG nRemaining = uliPos1.LowPart;
  for (; nRemaining > cbBuffer; nRemaining -= cbBuffer)
  {
    RETURN_FAILED(pstm1->Read(pBuffer1, cbBuffer, NULL));
    RETURN_FAILED(spstm2->Read(pBuffer2, cbBuffer, NULL));
    if (0 != memcmp(pBuffer1, pBuffer2, cbBuffer))
      return S_FALSE;
  }

  // Compare any remaining bytes
  RETURN_FAILED(pstm1->Read(pBuffer1, nRemaining, NULL));
  RETURN_FAILED(spstm2->Read(pBuffer2, nRemaining, NULL));
  return (0 != memcmp(pBuffer1, pBuffer2, nRemaining)) ? S_FALSE : S_OK;
}

HRESULT CompareObjectCLSIDs(IUnknown* punk1, IUnknown* punk2)
{
  // Objects are equal if both interfaces are NULL
  if (!punk1 && !punk2)
    return S_OK;

  // Object are inequal if either interface is NULL
  if (!punk1 || !punk2)
    return S_FALSE;

  // Each object must support IPersist
  IPersistPtr spPersist1, spPersist2;
  RETURN_FAILED(TCSafeQI(punk1, &spPersist1));
  RETURN_FAILED(TCSafeQI(punk2, &spPersist2));

  // Compare the CLSID's of the objects
  CLSID clsid1, clsid2;
  RETURN_FAILED(spPersist1->GetClassID(&clsid1));
  RETURN_FAILED(spPersist2->GetClassID(&clsid2));
  return (clsid1 == clsid2) ? S_OK : S_FALSE;
}


/////////////////////////////////////////////////////////////////////////////
// User Interface Helpers

bool FixSliderThumbSize(HWND hwndSlider)
{
  // Get the window style
  DWORD dwStyle = ::GetWindowLong(hwndSlider, GWL_STYLE);

  // Get the default thumb size
  long nDefault = ::SendMessage(hwndSlider, TBM_GETTHUMBLENGTH, 0, 0);

  // Adjust the style of the slider control, if neccessary
  if (!(dwStyle & TBS_FIXEDLENGTH))
    ::SetWindowLong(hwndSlider, GWL_STYLE, dwStyle | TBS_FIXEDLENGTH);

  // Get the client rectangle of the slider control
  RECT rc;
  ::GetClientRect(hwndSlider, &rc);

  // Determine the length available for the thumb
  const long nFocus = 1;
  long nThumb = ((dwStyle & TBS_VERT) ?
    (rc.right - rc.left) : (rc.bottom - rc.top)) - nFocus * 2;

  // Determine the number of sides with tick marks
  long nTickSides = (dwStyle & TBS_BOTH) ? 2 : 1;
  nTickSides = (dwStyle & TBS_NOTICKS) ? 0 : nTickSides;
  if (2 == nTickSides || (1 == nTickSides && (dwStyle & TBS_TOP)))
    --nThumb;

  // Allow space for the tick marks
  const long nTicks = 3 + 4;
  nThumb -= nTickSides * nTicks;

  // Allow the thumb to be no shorter than 6
  nThumb = std::max(nThumb, 6L);

  // Allow the thumb to be no longer than the original
  nThumb = std::min(nThumb, nDefault);

  // Indicate success if the thumb is already the computed size
  if (nThumb == nDefault)
    return true;

  // Set the new thumb length
  ::SendMessage(hwndSlider, TBM_SETTHUMBLENGTH, nThumb, 0);

  // Indicate success if thumb length changed
  return ::SendMessage(hwndSlider, TBM_GETTHUMBLENGTH, 0, 0) == nThumb;
}

#ifdef _AFX
  bool FixSliderThumbSize(CWnd* pwndSlider)
  {
    HWND hwndSlider = pwndSlider->GetSafeHwnd();
    return hwndSlider ? FixSliderThumbSize(hwndSlider) : false;
  }
#endif // _AFX


/////////////////////////////////////////////////////////////////////////////
// Finds a resource, loads it, and locks it.
//
HRESULT LockAndLoadResource(HINSTANCE hinst, LPCTSTR pszType,
  LPCTSTR pszName, void** ppvData, DWORD* pcbData)
{
  // Initialize the [out] parameters
  CLEAROUT_ALLOW_NULL(ppvData, (void*)NULL);
  CLEAROUT_ALLOW_NULL(pcbData, (DWORD)0);

  // Find the specified resource
  HRSRC hrsrc = ::FindResource(hinst, pszName, pszType);
  if (!hrsrc)
    return HRESULT_FROM_WIN32(::GetLastError());

  // Get the size of the resource
  DWORD cb = ::SizeofResource(hinst, hrsrc);

  // Load the specified resource
  HGLOBAL hData = ::LoadResource(hinst, hrsrc);
  if (!hData)
    return HRESULT_FROM_WIN32(::GetLastError());

  // Lock the resource
  void* pvData = LockResource(hData);
  if (!pvData)
    return HRESULT_FROM_WIN32(::GetLastError());

  // Copy values to the [out] parameters
  *ppvData = pvData;
  *pcbData = cb;

  // Indicate success
  return S_OK;
}


/////////////////////////////////////////////////////////////////////////////
// Determines if the current process/thread is running on the interactive
// desktop.
//
bool IsInteractiveDesktop()
{
  // Windows 9x is always interactive
  if (IsWin9x())
    return true;

  // Get the process window station
  HWINSTA hWinSta = ::GetProcessWindowStation();
  if (!hWinSta)
    return false;

  // Get the name of the process window station
  DWORD cbNeeded = 0;
  TCHAR szName[_MAX_PATH] = TEXT("");
  ::GetUserObjectInformation(hWinSta, UOI_NAME, szName, sizeof(szName),
    &cbNeeded);

  // If not "WinSta0", return false
  if (lstrcmp(TEXT("WinSta0"), szName))
    return false;

  // Get the current thread's desktop
  HDESK hDesk = ::GetThreadDesktop(::GetCurrentThreadId());
  if (!hDesk)
    return false;

  // Get the name of the thread's desktop
  ::GetUserObjectInformation(hDesk, UOI_NAME, szName, sizeof(szName),
    &cbNeeded);

  // If not "Default", return false
  return !lstrcmp(TEXT("Default"), szName);
}


/////////////////////////////////////////////////////////////////////////////
// Called as follows: If IDS_TEST is the string you wish to load and
// WCHAR Buffer[128] is your buffer, the call would be
// MyLoadStringW(IDS_TEST,Buffer,128);
// If it succeeds, the function returns the number of characters copied
// into the buffer, not including the NULL terminating character, or
// zero if the string resource does not exist.
//
// See KB article Q196899 for more information on why this function exists.
//
int TCLoadStringW(HINSTANCE hInstance, UINT wID, LPWSTR wzBuf, int cchBuf)
{
  // Null-terminate the specified buffer
  if (wzBuf && cchBuf)
    wzBuf[0] = L'\0';

  // Compute the block number and offset of the string
  UINT block = (wID >> 4) + 1;   // Compute block number.
  UINT num = wID & 0xf;          // Compute offset into block.

  // Find, lock, and load the string table resource
  HRSRC hRC = FindResourceEx(hInstance, RT_STRING,
    MAKEINTRESOURCE(block), MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL));
  HGLOBAL hgl = LoadResource(hInstance, hRC);
  LPWSTR str = (LPWSTR)LockResource(hgl);
  if (!str)
    return 0;

  // Increment to the correct offect into the string block
  for (UINT i = 0; i < num; i++)
    str += *str + 1;

  // Copy the string to the specified buffer
  if (wzBuf && cchBuf)
  {
    wcsncpy(wzBuf, str + 1, std::min((WCHAR)(cchBuf - 1), *str));
    wzBuf[std::min(WCHAR(cchBuf-1), *str) ] = '\0';
  }

  // Return the length of the string resource
  return *str;
}

HRESULT TCLoadBSTR(HINSTANCE hInstance, UINT wID, BSTR* pbstrOut)
{
  // Get the length of the string in the string table resource
  int cch = TCLoadStringW(hInstance, wID, NULL, 0);
  if (!cch)
    return S_FALSE;

  // Allocate space for the string
  *pbstrOut = SysAllocStringLen(NULL, cch);
  if (!*pbstrOut)
    return E_OUTOFMEMORY;

  // Load the string into the new BSTR
  return TCLoadStringW(hInstance, wID, *pbstrOut, cch + 1) ? S_OK : S_FALSE;
}


/////////////////////////////////////////////////////////////////////////////
// Error Reporting Functions/Macros
// Note: These methods will output even in RELEASE builds!

void _cdecl TCErrLog(LPCTSTR lpszFormat, ...)
{
  va_list args;
  va_start(args, lpszFormat);

  int nBuf;
  TCHAR szBuffer[512];

  nBuf = _vstprintf(szBuffer, lpszFormat, args);
  ZAssert(nBuf < sizeof(szBuffer));

  #ifdef _DEBUG
    ZDebugOutputImpl(szBuffer);
  #else // _DEBUG
    OutputDebugString(szBuffer);
  #endif // _DEBUG
  va_end(args);
}


/////////////////////////////////////////////////////////////////////////////
