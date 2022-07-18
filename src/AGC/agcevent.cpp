/////////////////////////////////////////////////////////////////////////////
// AGCEvent.cpp : Implementation of CAGCEvent
//

#include "pch.h"
#include "AGCEvent.h"
#include "AGCEventDef.h"
#include "AGCEventData.h"
#include <..\TCLib\UtilImpl.h>
#include <..\TCLib\BinString.h>
#include <..\TCAtl\SimpleStream.h>


/////////////////////////////////////////////////////////////////////////////
// CAGCEvent

TC_OBJECT_EXTERN_IMPL(CAGCEvent)


/////////////////////////////////////////////////////////////////////////////
// Construction

HRESULT CAGCEvent::FinalConstruct()
{
  // #define CAGCEvent_TRACE_CONSTRUCTION
  #ifdef CAGCEvent_TRACE_CONSTRUCTION
    _TRACE_BEGIN
      DWORD id = GetCurrentThreadId();
      _TRACE_PART2("CAGCEvent::FinalConstruct(): ThreadId = %d (0x%X)\n", id, id);
      _TRACE_PART1("\tRaw pointer = 0x%08X", this);
      _TRACE_PART1(", IAGCEvent* = 0x%08X\n", static_cast<IAGCEvent*>(this));
    _TRACE_END
  #endif // !CAGCEvent_TRACE_CONSTRUCTION

  // Indicate success
  return S_OK;
}


void CAGCEvent::FinalRelease()
{
  #if 0
    #pragma pack(push, 4)
    static long s_cIterations = 0;
    #pragma pack(pop)
    debugf("E:%06d\n", InterlockedIncrement(&s_cIterations));
  #endif
}


HRESULT CAGCEvent::Init(AGCEventID idEvent, LPCSTR pszContext,
  LPCOLESTR pszSubject, long idSubject, long cArgTriplets, va_list argptr)
{
  XLock lock(this);
  m_id = idEvent;
  m_time = GetVariantDateTime();
  m_idSubject = idSubject;
  m_bstrSubjectName = pszSubject;
  m_bstrContext = pszContext;

  // Clear the property map
  m_Properties.clear();

  // Add the static properties to the dictionary
  RETURN_FAILED(AddStaticFields());

  // Add the specified properties to the dictionary
  return AddToDictionaryV(cArgTriplets, argptr);
}


HRESULT CAGCEvent::Init(const CAGCEventData& data)
{
  XLock lock(this);

  // Get the fixed data
  m_id = data.GetEventID();
  m_time = data.GetTime();
  m_idSubject = data.GetSubjectID();

  // Clear the dictionary
  m_Properties.clear();

  // Get the variable data
  data.GetVarData(&m_bstrContext, &m_bstrSubjectName, &m_Properties);

  // Add the static properties to the dictionary
  return AddStaticFields();
}


/////////////////////////////////////////////////////////////////////////////
// Operations

HRESULT CAGCEvent::AddToDictionary(long cArgTriplets, ...)
{
  va_list argptr;
  va_start(argptr, cArgTriplets);
  HRESULT hr = AddToDictionaryV(cArgTriplets, argptr);
  va_end(argptr);
  return hr;
}


HRESULT CAGCEvent::AddToDictionaryV(long cArgTriplets, va_list argptr)
{
  XLock lock(this);

  // Iterate through the argument triplets
  for (long i = 0; i < cArgTriplets; ++i)
  {
    // Get the argument name
    LPCSTR pszArgName = va_arg(argptr, LPCSTR);
    ZAssert(!IsBadStringPtrA(pszArgName, UINT(-1)));

    // Get the argument variant type and value
    CComVariant var;
    VARTYPE vt = va_arg(argptr, VARTYPE);
    switch (V_VT(&var) = vt)
    {
      case VT_BOOL:
        V_BOOL(&var) = va_arg(argptr, VARIANT_BOOL);
        break;
      case VT_I1:
        V_I1(&var) = va_arg(argptr, CHAR);
        break;
      case VT_I2:
        V_I2(&var) = va_arg(argptr, SHORT);
        break;
      case VT_I4:
        V_I4(&var) = va_arg(argptr, LONG);
        break;
      case VT_UI1:
        V_UI1(&var) = va_arg(argptr, BYTE);
        break;
      case VT_UI2:
        V_UI2(&var) = va_arg(argptr, USHORT);
        break;
      case VT_ERROR:
        V_ERROR(&var) = va_arg(argptr, SCODE);
        break;
      case VT_R4:
      {
        // (...) pushes a float argument onto the stack as a double
        DOUBLE dblTemp = va_arg(argptr, DOUBLE);
        V_R4(&var) = (float)dblTemp;
        break;
      }
      case VT_R8:
        V_R8(&var) = va_arg(argptr, DOUBLE);
        break;
      case VT_DECIMAL:
        V_DECIMAL(&var) = va_arg(argptr, DECIMAL);
        break;
      case VT_CY:
        V_CY(&var) = va_arg(argptr, CY);
        break;
      case VT_DATE:
        V_DATE(&var) = va_arg(argptr, DATE);
        break;
      case VT_BSTR:
        V_BSTR(&var) = SysAllocString(va_arg(argptr, BSTR));
        break;
      case VT_UNKNOWN:
        V_UNKNOWN(&var) = va_arg(argptr, IUnknown*);
        V_UNKNOWN(&var)->AddRef();
        break;
      case VT_DISPATCH:
        V_DISPATCH(&var) = va_arg(argptr, IDispatch*);
        V_DISPATCH(&var)->AddRef();
        break;
      case VT_VARIANT:
        var.Copy(va_arg(argptr, VARIANT*));
        break;
      case VT_LPSTR:
      {
        V_VT(&var) = VT_BSTR;
        LPCSTR psz = va_arg(argptr, LPCSTR);
        USES_CONVERSION;
        V_BSTR(&var) = (psz && '\0' != *psz) ? ::SysAllocString(A2COLE(psz)) : NULL;
        break;
      }
      case VT_LPWSTR:
      {
        V_VT(&var) = VT_BSTR;
        LPCWSTR psz = va_arg(argptr, LPCWSTR);
        V_BSTR(&var) = (psz && L'\0' != *psz) ? ::SysAllocString(psz) : NULL;
        break;
      }
      default:
        if (vt & VT_BYREF)
        {
          V_BYREF(&var) = va_arg(argptr, void*);
          break;
        }
        debugf("CAGCEvent::Init(): Specified VARTYPE %hu (0x%04X) is unsupported\n", vt, vt);
        ZAssert(false);
    }

    // Add the named value to the property map
    m_Properties.insert(XProperties::value_type(CComBSTR(pszArgName), var));
  }

  // Indicate success
  return S_OK;
}


HRESULT CAGCEvent::AddStaticFields()
{
  // Add the static properties to the dictionary
  XLock lock(this);
  RETURN_FAILED(get_ComputerName(NULL));
  return AddToDictionary(6,
    ".ID"          , VT_I4  , m_id,
    ".Time"        , VT_DATE, m_time,
    ".ComputerName", VT_BSTR, (BSTR)m_bstrComputerName,
    ".SubjectID"   , VT_I4  , m_idSubject,
    ".SubjectName" , VT_BSTR, (BSTR)m_bstrSubjectName,
    ".Context"     , VT_BSTR, (BSTR)m_bstrContext);
}


/////////////////////////////////////////////////////////////////////////////
// Implementation

bool CAGCEvent::IsStaticField(BSTR bstr)
{
  if (!BSTRLen(bstr))
    return false;
  if (!_wcsicmp(bstr, OLESTR(".ID")))
    return true;
  if (!_wcsicmp(bstr, OLESTR(".Time")))
    return true;
  if (!_wcsicmp(bstr, OLESTR(".ComputerName")))
    return true;
  if (!_wcsicmp(bstr, OLESTR(".SubjectID")))
    return true;
  if (!_wcsicmp(bstr, OLESTR(".SubjectName")))
    return true;
  if (!_wcsicmp(bstr, OLESTR(".Context")))
    return true;
  return false;
}


HRESULT CAGCEvent::WriteStringToStream(IStream* pStm, BSTR bstr)
{
  // Get the character length of the specified BSTR
  ULONG cchBSTR = BSTRLen(bstr);

  // Determine the number of bytes needed for conversion to ANSI
  ULONG cchAnsi = cchBSTR ?
    WideCharToMultiByte(CP_ACP, 0, bstr, cchBSTR, NULL, 0, NULL, NULL) : 0;

  // Write an indicator of how many following bytes represent the length
  BYTE bIndicator = GetSizePrefix(cchAnsi);
  RETURN_FAILED(pStm->Write(&bIndicator, sizeof(bIndicator), NULL));

  // Write the number of ANSI characters that follow
  switch (bIndicator)
  {
    case LengthZero:
      return S_OK;
    case Length1Byte:
    {
      BYTE cch = (BYTE)cchAnsi;
      RETURN_FAILED(pStm->Write(&cch, sizeof(cch), NULL));
      break;
    }
    case Length2Bytes:
    {
      WORD cch = (WORD)cchAnsi;
      RETURN_FAILED(pStm->Write(&cch, sizeof(cch), NULL));
      break;
    }
    case Length4Bytes:
    {
      RETURN_FAILED(pStm->Write(&cchAnsi, sizeof(cchAnsi), NULL));
      break;
    }
    default:
      ZError("bad switch case in WriteStringToStream");
  }

  // Allocate the buffer for conversion
  LPSTR psz = (LPSTR)_alloca(cchAnsi);

  // Convert the BSTR to ANSI
  if (!WideCharToMultiByte(CP_ACP, 0, bstr, cchBSTR, psz, cchAnsi, NULL, NULL))
    return HRESULT_FROM_WIN32(GetLastError());

  // Write the ANSI characters
  RETURN_FAILED(pStm->Write(psz, cchAnsi, NULL));

  // Indicate success
  return S_OK;
}

HRESULT CAGCEvent::WriteVariantToStream(IStream* pStm, CComVariant& var)
{
  // Special processing for VT_BSTR
  if (VT_BSTR == V_VT(&var))
  {
    // Delegate to WriteStringToStream
    RETURN_FAILED(WriteStringToStream(pStm, V_BSTR(&var)));
  }
  else
  {
    // Indicate a non-custom variant
    BYTE bCustom = 0x00;
    RETURN_FAILED(pStm->Write(&bCustom, sizeof(bCustom), NULL));

    // Delegate non-custom variant types to CComVariant's implementation
    RETURN_FAILED(var.WriteToStream(pStm));
  }

  // Indicate success
  return S_OK;
}


HRESULT CAGCEvent::ReadStringFromStream(IStream* pStm, BSTR* pbstr, BYTE bIndicator)
{
  ZAssert(!*pbstr);

  // Read the size indicator if zero was specified
  if (!bIndicator)
  {
    RETURN_FAILED(pStm->Read(&bIndicator, sizeof(bIndicator), NULL));
  }

  // Read the number of ANSI character to follow
  ULONG cchAnsi;
  switch (bIndicator)
  {
    case LengthZero:
      return S_OK;
    case Length1Byte:
    {
      BYTE cch;
      RETURN_FAILED(pStm->Read(&cch, sizeof(cch), NULL));
      cchAnsi = cch;
      break;
    }
    case Length2Bytes:
    {
      WORD cch;
      RETURN_FAILED(pStm->Read(&cch, sizeof(cch), NULL));
      cchAnsi = cch;
      break;
    }
    case Length4Bytes:
    {
      RETURN_FAILED(pStm->Read(&cchAnsi, sizeof(cchAnsi), NULL));
      break;
    }
    default:
      ZError("bad switch case in ReadStringFromStream");
  }

  if (cchAnsi)
  {
    // Allocate a buffer for the ANSI characters and read them in
    LPSTR pszAnsi = (LPSTR)_alloca(cchAnsi);
    RETURN_FAILED(pStm->Read(pszAnsi, cchAnsi, NULL));

    // Determine the number of bytes needed for conversion to Unicode
    ULONG cchWide = MultiByteToWideChar(CP_ACP, 0, pszAnsi, cchAnsi, NULL, 0);

    // Allocate a buffer for the conversion and convert it
    LPWSTR pszWide = (LPWSTR)_alloca(cchWide);
    if (!MultiByteToWideChar(CP_ACP, 0, pszAnsi, cchAnsi, pszWide, cchWide))
      return HRESULT_FROM_WIN32(GetLastError());

    // Allocate the BSTR
    *pbstr = SysAllocStringLen(pszWide, cchWide);
    if (!*pbstr)
      return E_OUTOFMEMORY;
  }

  // Indicate success
  return S_OK;
}

HRESULT CAGCEvent::ReadVariantFromStream(IStream* pStm, CComVariant& var)
{
  // Clear the specified variant
  var.Clear();

  // Read the custom byte indicator
  BYTE bCustom;
  RETURN_FAILED(pStm->Read(&bCustom, sizeof(bCustom), NULL));
  if (bCustom)
  {
    // Delegate to ReadStringFromStream
    V_VT(&var) = VT_BSTR;
    V_BSTR(&var) = NULL;
    RETURN_FAILED(ReadStringFromStream(pStm, &V_BSTR(&var), bCustom));
  }
  else
  {
    // Delegate non-custom variant types to CComVariant's implementation
    RETURN_FAILED(var.ReadFromStream(pStm));
  }

  // Indicate success
  return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
// This method is much like our IPersistStreamInit::Save implementation, but
// it ensures that all strings are written out as ANSI characters to save
// space.
//
// However, the IPersistStreamInit::Save implementation should not be changed
// as long as support is needed for the old format, which uses the ObjRef
// moniker.
//
HRESULT CAGCEvent::SaveToStream(IStream* pStm, BOOL fClearDirty)
{
  XLock lock(this);

  // Write out a short signature string (allows for future version checking)
  LPCSTR pszSignature = "1:";
  RETURN_FAILED(pStm->Write(pszSignature, strlen(pszSignature), NULL));

  // Write out the ID and time
  RETURN_FAILED(pStm->Write(&m_id, sizeof(m_id), NULL));
  RETURN_FAILED(pStm->Write(&m_time, sizeof(m_time), NULL));

  // Count the number of non-static fields
  long cPairs = 0;
  // mdvalley: again with the variable declarations
  XPropertyIt it;
  for (it = m_Properties.begin(); m_Properties.end() != it; ++it)
    if (!IsStaticField(it->first))
      ++cPairs;

  // Write out the number of non-static Key/Item pairs in the property map
  RETURN_FAILED(pStm->Write(&cPairs, sizeof(cPairs), NULL));

  // Write out each non-static Key/Item pair in the property map
  for (it = m_Properties.begin(); m_Properties.end() != it; ++it)
  {
    CComBSTR&    bstrKey = const_cast<CComBSTR&>(it->first);
    CComVariant& varItem = it->second;
    if (!IsStaticField(bstrKey))
    {
      RETURN_FAILED(WriteStringToStream(pStm, bstrKey));
      RETURN_FAILED(WriteVariantToStream(pStm, varItem));
    }
  }

  // Write out the computer name
  RETURN_FAILED(get_ComputerName(NULL));
  RETURN_FAILED(WriteStringToStream(pStm, m_bstrComputerName));

  // Write out the subject ID and name
  RETURN_FAILED(pStm->Write(&m_idSubject, sizeof(m_idSubject), NULL));
  RETURN_FAILED(WriteStringToStream(pStm, m_bstrSubjectName));

  // Write out the Context string
  RETURN_FAILED(WriteStringToStream(pStm, m_bstrContext));

  // Indicate success
  return S_OK;
}

HRESULT CAGCEvent::LoadFromStream(IStream* pStm)
{
  XLock lock(this);

  // Read the signature string (allows for future version checking)
  char szSignature[2];
  RETURN_FAILED(pStm->Read(szSignature, sizeof(szSignature), NULL));

  // Check the signature string
  int iSignature = -1;
  const static LPCSTR szSignatures[] = {"0:", "1:"};
  for (int i = 0; i < sizeofArray(szSignatures); ++i)
  {
    if (0 == strncmp(szSignature, szSignatures[i], strlen(szSignatures[i])))
    {
      iSignature = i;
      break;
    }
  }
  if (-1 == iSignature)
    return E_INVALIDARG;

  // Read the ID and time
  RETURN_FAILED(pStm->Read(&m_id, sizeof(m_id), NULL));
  RETURN_FAILED(pStm->Read(&m_time, sizeof(m_time), NULL));

  // Read the number of key/item pairs being read
  long cCount = 0;
  RETURN_FAILED(pStm->Read(&cCount, sizeof(cCount), NULL));

  // Clear all items from the property map
  m_Properties.clear();

  // Read each pair from the stream
  while (cCount--)
  {
    // Read the Key/Item pair from the stream
    CComBSTR    bstrKey;
    CComVariant varItem;
    RETURN_FAILED(ReadStringFromStream(pStm, &bstrKey));
    RETURN_FAILED(ReadVariantFromStream(pStm, varItem));

    // Add the Key/Item pair to the property map
    m_Properties.insert(XProperties::value_type(bstrKey, varItem));
  }

  // Read the computer name
  m_bstrComputerName.Empty();
  RETURN_FAILED(ReadStringFromStream(pStm, &m_bstrComputerName));

  // Read the subject ID and name
  RETURN_FAILED(pStm->Read(&m_idSubject, sizeof(m_idSubject), NULL));
  m_bstrSubjectName.Empty();
  RETURN_FAILED(ReadStringFromStream(pStm, &m_bstrSubjectName));

  // Read the context string
  m_bstrContext.Empty();
  if (iSignature >= 1)
  {
    RETURN_FAILED(ReadStringFromStream(pStm, &m_bstrContext));
  }

  // Add the static fields
  RETURN_FAILED(AddStaticFields());

  // Indicate success
  return S_OK;
}



/////////////////////////////////////////////////////////////////////////////
// ISupportErrorInfo Interface Methods

STDMETHODIMP CAGCEvent::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IAGCEvent
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

STDMETHODIMP CAGCEvent::GetClassID(CLSID* pClassID)
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

STDMETHODIMP CAGCEvent::IsDirty()
{
  // Always is dirty, since we don't keep a dirty flag
  return S_OK;
}


STDMETHODIMP CAGCEvent::Load(LPSTREAM pStm)
{
  XLock lock(this);

  // Read the ID and time
  RETURN_FAILED(pStm->Read(&m_id, sizeof(m_id), NULL));
  RETURN_FAILED(pStm->Read(&m_time, sizeof(m_time), NULL));

  // Read the number of key/item pairs being read
  long cCount = 0;
  RETURN_FAILED(pStm->Read(&cCount, sizeof(cCount), NULL));

  // Clear all items from the property map
  m_Properties.clear();

  // Read each pair from the stream
  while (cCount--)
  {
    // Read the Key/Item pair from the stream
    CComBSTR    bstrKey;
    CComVariant varItem;
    RETURN_FAILED(bstrKey.ReadFromStream(pStm));
    RETURN_FAILED(varItem.ReadFromStream(pStm));

    // Add the Key/Item pair to the property map
    m_Properties.insert(XProperties::value_type(bstrKey, varItem));
  }

  // Read the computer name
  m_bstrComputerName.Empty();
  RETURN_FAILED(m_bstrComputerName.ReadFromStream(pStm));

  // Read the subject ID and name
  RETURN_FAILED(pStm->Read(&m_idSubject, sizeof(m_idSubject), NULL));
  m_bstrSubjectName.Empty();
  RETURN_FAILED(m_bstrSubjectName.ReadFromStream(pStm));

  // Read the context string
  m_bstrContext.Empty();
  RETURN_FAILED(m_bstrContext.ReadFromStream(pStm));

  // Add the static fields
  RETURN_FAILED(AddStaticFields());

  // Indicate success
  return S_OK;
}


STDMETHODIMP CAGCEvent::Save(LPSTREAM pStm, BOOL fClearDirty)
{
  XLock lock(this);

  // Write out the ID and time
  RETURN_FAILED(pStm->Write(&m_id, sizeof(m_id), NULL));
  RETURN_FAILED(pStm->Write(&m_time, sizeof(m_time), NULL));

  // Count the number of non-static fields
  long cPairs = 0;
  // mdvalley: Declaration motion makes me seasick.
  XPropertyIt it;
  for (it = m_Properties.begin(); m_Properties.end() != it; ++it)
    if (!IsStaticField(it->first))
      ++cPairs;

  // Write out the number of non-static Key/Item pairs in the property map
  RETURN_FAILED(pStm->Write(&cPairs, sizeof(cPairs), NULL));

  // Write out each non-static Key/Item pair in the property map
  for (it = m_Properties.begin(); m_Properties.end() != it; ++it)
  {
    CComBSTR&    bstrKey = const_cast<CComBSTR&>(it->first);
    CComVariant& varItem = it->second;
    if (!IsStaticField(bstrKey))
    {
      RETURN_FAILED(bstrKey.WriteToStream(pStm));
      RETURN_FAILED(varItem.WriteToStream(pStm));
    }
  }

  // Write out the computer name
  RETURN_FAILED(get_ComputerName(NULL));
  RETURN_FAILED(m_bstrComputerName.WriteToStream(pStm));

  // Write out the subject ID and name
  RETURN_FAILED(pStm->Write(&m_idSubject, sizeof(m_idSubject), NULL));
  RETURN_FAILED(m_bstrSubjectName.WriteToStream(pStm));

  // Write out the context string
  RETURN_FAILED(m_bstrContext.WriteToStream(pStm));

  // Indicate success
  return S_OK;
}


STDMETHODIMP CAGCEvent::GetSizeMax(ULARGE_INTEGER* pCbSize)
{
  return TCGetPersistStreamSize(GetUnknown(), pCbSize);
}


STDMETHODIMP CAGCEvent::InitNew()
{
  // Initialize the Event object
  XLock lock(this);
  m_id = EventID_Unknown;
  m_time = GetVariantDateTime();
  m_Properties.clear();

  // Indicate success
  return S_OK;
}
      

/////////////////////////////////////////////////////////////////////////////
// IAGCEvent Interface Methods

STDMETHODIMP CAGCEvent::get_Description(BSTR* pbstrDisplayString)
{
  // Delegate to the CAGCEventDef class
  return CAGCEventDef::GetEventDescription(this, pbstrDisplayString);
}


STDMETHODIMP CAGCEvent::get_ID(AGCEventID* pVal)
{
  XLock lock(this);
  CLEAROUT(pVal, m_id);
  return S_OK;
}


STDMETHODIMP CAGCEvent::get_Time(DATE* pVal)
{
  XLock lock(this);
  CLEAROUT(pVal, m_time);
  return S_OK;
}


STDMETHODIMP CAGCEvent::get_PropertyCount(long* pnCount)
{
  XLock lock(this);
  CLEAROUT(pnCount, (long)m_Properties.size());
  return S_OK;
}


STDMETHODIMP CAGCEvent::get_PropertyExists(BSTR bstrKey,
  VARIANT_BOOL* pbExists)
{
  XLock lock(this);
  XPropertyIt it = m_Properties.find(bstrKey);
  CLEAROUT(pbExists, VARBOOL(m_Properties.end() != it));
  return S_OK;
}


STDMETHODIMP CAGCEvent::get_Property(VARIANT* pvKey, VARIANT* pvValue)
{
  // Check for default value
  CComVariant var;
  if (pvKey)
  {
    CComVariant varKey(*pvKey);

    // Check for an index first
    if (SUCCEEDED(varKey.ChangeType(VT_UI4)))
    {
      XLock lock(this);
      ULONG iIndex = V_UI4(&varKey);
      bool bGetKey = !!(iIndex & 0x80000000);
      iIndex &= 0x7FFFFFFF;
      if (iIndex >= m_Properties.size())
        return E_INVALIDARG;

      if (iIndex < (m_Properties.size() + 1) / 2)
      {
        XPropertyIt it = m_Properties.begin();
        for (ULONG i = 0; i < iIndex; ++i)
          ++it;
        if (bGetKey)
          var = it->first;
        else
          var = it->second;
      }
      else
      {
        XPropertyRevIt rit = m_Properties.rbegin();
        for (ULONG i = m_Properties.size() - 1; i > iIndex; --i)
          ++rit;
        if (bGetKey)
          var = rit->first;
        else
          var = rit->second;
      }
    }
    else
    {
      // Only support VT_BSTR at this time
      CComVariant varKey(*pvKey);
      RETURN_FAILED(varKey.ChangeType(VT_BSTR));
      CComBSTR bstrKey(V_BSTR(&varKey));

      // Find the specified string or not
      XLock lock(this);
      XPropertyIt it = m_Properties.find(bstrKey);
      if (m_Properties.end() != it)
      {
        // Get the value associated with the key
        var = it->second;
      }
      else
      {
        // Add it since it wasn't there
        m_Properties.insert(XProperties::value_type(bstrKey, var));
      }
    }
  }
  else
  {
    RETURN_FAILED(get_Description(&V_BSTR(&var)));
    V_VT(&var) = VT_BSTR;
  }
  return var.Detach(pvValue);
}


STDMETHODIMP CAGCEvent::get_ComputerName(BSTR* pbstrComputerName)
{
  // Initialize the [out] parameter
  if (pbstrComputerName)
    CLEAROUT(pbstrComputerName, (BSTR)NULL);

  // Copy the stored name, if any
  XLock lock(this);
  if (!m_bstrComputerName.Length())
  {
    // Get the computer name
    TCHAR szName[MAX_COMPUTERNAME_LENGTH + 1];
    DWORD cchName = sizeofArray(szName);
    GetComputerName(szName, &cchName);

    // Save the computer name
    m_bstrComputerName = szName;
  }

  // Copy the name to the [out] parameter
  if (pbstrComputerName)
  {
    USES_CONVERSION;
    *pbstrComputerName = m_bstrComputerName.Copy();
  }

  // Indicate success
  return S_OK;
}


STDMETHODIMP CAGCEvent::get_SubjectID(long* pidSubject)
{
  XLock lock(this);
  CLEAROUT(pidSubject, m_idSubject);
  return S_OK;
}


STDMETHODIMP CAGCEvent::get_SubjectName(BSTR* pbstrSubject)
{
  XLock lock(this);
  CComBSTR bstrTemp(m_bstrSubjectName);
  CLEAROUT(pbstrSubject, (BSTR)bstrTemp);
  bstrTemp.Detach();
  return S_OK;
}


STDMETHODIMP CAGCEvent::SaveToString(BSTR* pbstr)
{
  // Create a memory stream
  IStreamPtr spStm;
  RETURN_FAILED(CreateStreamOnHGlobal(NULL, true, &spStm));

  // Persist this object to the stream
  RETURN_FAILED(SaveToStream(spStm, false));

  // Get the size of the binary data in the stream
  ULARGE_INTEGER uli;
  LARGE_INTEGER liMove = {0, 0};
  ZSucceeded(spStm->Seek(liMove, STREAM_SEEK_CUR, &uli));
  ZAssert(0 == uli.HighPart);
  DWORD cbData = uli.LowPart;

  // Get the HGLOBAL from the stream
  HGLOBAL hGlobal;
  RETURN_FAILED(GetHGlobalFromStream(spStm, &hGlobal));

  // Lock the HGLOBAL to resolve it to a pointer
  void* pvData = GlobalLock(hGlobal);

  // Determine the length of the string representation of binary data
  DWORD cch = TCBinString<OLECHAR>::GetEncodedStringLength(pvData, cbData);

  // Allocate a buffer for the encoded string
  LPOLESTR psz = (LPOLESTR)_alloca((cch + 1) * sizeof(OLECHAR));

  // Encode the binary data to a string
  HRESULT hr = TCBinString<OLECHAR>::EncodeToString(pvData, cbData, psz);

  // Unlock the HGLOBAL
  GlobalUnlock(hGlobal);

  // Return if the encoding failed
  RETURN_FAILED(hr);

  // Allocate the BSTR
  *pbstr = SysAllocStringLen(psz, cch);
  if (!*pbstr)
    return E_OUTOFMEMORY;

  // Indicate success
  return S_OK;
}


STDMETHODIMP CAGCEvent::LoadFromString(BSTR bstr)
{
  ///////////////////////////////////////////////////////////////////////////
  // Support the old format
  const static OLECHAR szObjRef[] = OLESTR("objref:");
  const static int cchObjRef = sizeofArray(szObjRef) - 1;
  if (0 == wcsncmp(bstr, szObjRef, cchObjRef))
  {
    // Create an object from the specified BSTR
    IUnknownPtr spUnk;
    RETURN_FAILED(TCUtilImpl::GetObject(bstr, VARIANT_FALSE, INFINITE, &spUnk));

    // Load ourself as a copy of the new object
    return CopyObjectThruStream(GetUnknown(), spUnk);
  }

  ///////////////////////////////////////////////////////////////////////////
  // Must be the newer format
  ///////////////////////////////////////////////////////////////////////////

  // Get the number of bytes the encoded string will decode to
  DWORD cbData = TCBinString<OLECHAR>::GetDecodedDataSize(bstr);
  if (0xFFFFFFFF == cbData)
    return E_INVALIDARG;

  // Allocate a buffer for the decoded binary data
  void* pvData = _alloca(cbData);

  // Decode the data
  RETURN_FAILED(TCBinString<OLECHAR>::DecodeFromString(bstr, pvData, NULL));

  // Create a simple stream on the data buffer
  TCSimpleStream stm;
  stm.Init(cbData, pvData);

  // Load the object from the stream
  RETURN_FAILED(LoadFromStream(&stm));

  // Indicate success
  return S_OK;
}


STDMETHODIMP CAGCEvent::get_Context(BSTR* pbstrContext)
{
  XLock lock(this);
  CComBSTR bstrTemp(m_bstrContext);
  CLEAROUT(pbstrContext, (BSTR)bstrTemp);
  bstrTemp.Detach();
  return S_OK;
}


/////////////////////////////////////////////////////////////////////////////
// IAGCEventCreate Interface Methods

STDMETHODIMP CAGCEvent::Init()
{
  // Delegate to InitNew
  return InitNew();
}

STDMETHODIMP CAGCEvent::put_ID(AGCEventID Val)
{
  XLock lock(this);
  m_id = Val;

  // Indicate success
  return S_OK;
}

STDMETHODIMP CAGCEvent::put_Time(DATE Val)
{
  XLock lock(this);
  m_time = Val;

  // Indicate success
  return S_OK;
}

STDMETHODIMP CAGCEvent::SetTimeNow()
{
  XLock lock(this);
  m_time = GetVariantDateTime();

  // Indicate success
  return S_OK;
}

STDMETHODIMP CAGCEvent::AddProperty(BSTR pbstrKey, VARIANT* pvValue)
{
  if (!pbstrKey)
    return E_INVALIDARG;

  XLock lock(this);
  m_Properties[pbstrKey] = *pvValue;

  // Indicate success
  return S_OK;
}

STDMETHODIMP CAGCEvent::RemoveProperty(BSTR pbstrKey, VARIANT* pvValue)
{
  if (!pbstrKey)
    return E_INVALIDARG;

  XLock lock(this);
  XPropertyIt it = m_Properties.find(*pbstrKey);
  if (m_Properties.end() != it)
    m_Properties.erase(it);

  // Indicate success
  return S_OK;
}

STDMETHODIMP CAGCEvent::put_SubjectID(long idSubject)
{
  XLock lock(this);
  m_idSubject = idSubject;

  // Indicate success
  return S_OK;
}

STDMETHODIMP CAGCEvent::put_SubjectName(BSTR bstrSubject)
{
  XLock lock(this);
  m_bstrSubjectName = bstrSubject;

  // Indicate success
  return S_OK;
}

STDMETHODIMP CAGCEvent::put_Context(BSTR bstrContext)
{
  XLock lock(this);
  m_bstrContext = bstrContext;

  // Indicate success
  return S_OK;
}

