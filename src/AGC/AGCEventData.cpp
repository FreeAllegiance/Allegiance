/////////////////////////////////////////////////////////////////////////////
// AGCEventData.cpp : Implementation of CAGCEventData
//

#include "pch.h"
#include "AGCEventData.h"
#include <..\TCAtl\SimpleStream.h>
#include <..\TCAtl\TCNullStreamImpl.h>


/////////////////////////////////////////////////////////////////////////////
// CAGCEventData


/////////////////////////////////////////////////////////////////////////////
// Construction / Destruction

/////////////////////////////////////////////////////////////////////////////
// Attributes

void CAGCEventData::GetVarData(BSTR* pbstrContext, BSTR* pbstrSubject,
  CAGCEventData::XProperties* pProperties) const
{
  // Get a pointer to the beginning of the variable data block
  ZAssert(m_pbData && m_cbData >= sizeof(XData));
  XData* pData = reinterpret_cast<XData*>(m_pbData);
  BYTE* pbData = reinterpret_cast<BYTE*>(pData + 1);

  // Read the context string
  pbData += CreateBSTRFromData_LPSTR(pbData, pbstrContext);

  // Read the subject name
  pbData += CreateBSTRFromData_LPWSTR(pbData, pbstrSubject);

  // Add each association to the specified property map
  CComBSTR    bstrKey;
  CComVariant varItem;
  for (long cArgTriplets = GetArgCount(); cArgTriplets; --cArgTriplets)
  {
    // Read the argument name
    pbData += CreateBSTRFromData_LPSTR(pbData, &bstrKey);

    // Read the argument value
    pbData += CreateVARIANTFromData(pbData, varItem);

    // Add the association to the property map
    pProperties->insert(XProperties::value_type(bstrKey, varItem));

    // Free the BSTR
    bstrKey.Empty();
  }
}


/////////////////////////////////////////////////////////////////////////////
// Implementation


/////////////////////////////////////////////////////////////////////////////
// Useful macro for implementing size calculations of variant types
//
#define VT_SIZE(vt, type)     \
  case vt:                    \
    va_arg(argptr, type);     \
    cbTotal += sizeof(type);  \
    break;


/////////////////////////////////////////////////////////////////////////////
//
UINT CAGCEventData::ComputeVariableDataSize(LPCSTR pszContext,
  LPCOLESTR pszSubject, long cArgTriplets, va_list argptr)
{
  // Start with the string length of the specified context string, if any
  UINT cbTotal = pszContext ? strlen(pszContext) : 0;
  cbTotal += sizeof(cbTotal);

  // Next is the string length of the specified subject string, if any
  cbTotal += pszSubject ? (wcslen(pszSubject) * sizeof(WCHAR)) : 0;
  cbTotal += sizeof(cbTotal);

  // Iterate through the argument triplets
  for (long i = 0; i < cArgTriplets; ++i)
  {
    // Get the argument name
    LPCSTR pszArgName = va_arg(argptr, LPCSTR);
    ZAssert(!IsBadStringPtrA(pszArgName, UINT(-1)));

    // Get the argument name length
    UINT cchName = strlen(pszArgName);
    cbTotal += sizeof(cchName) + cchName;

    // Get the argument variant type and value
    VARTYPE vt = va_arg(argptr, VARTYPE);
    cbTotal += sizeof(vt);
    switch (vt)
    {
      VT_SIZE(VT_BOOL   , VARIANT_BOOL)
      VT_SIZE(VT_I1     ,         CHAR)
      VT_SIZE(VT_I2     ,        SHORT)
      VT_SIZE(VT_I4     ,         LONG)
      VT_SIZE(VT_UI1    ,         BYTE)
      VT_SIZE(VT_UI2    ,       USHORT)
      VT_SIZE(VT_ERROR  ,        SCODE)
      case VT_R4:
      {
        // (...) pushes a float argument onto the stack as a double
        va_arg(argptr, DOUBLE);
        cbTotal += sizeof(float);
        break;
      }
      VT_SIZE(VT_R8     ,       DOUBLE)
      VT_SIZE(VT_DECIMAL,      DECIMAL)
      VT_SIZE(VT_CY     ,           CY)
      VT_SIZE(VT_DATE   ,         DATE)
      case VT_BSTR:
      {
        BSTR bstr = va_arg(argptr, BSTR);
        UINT cb = bstr ? SysStringByteLen(bstr) : 0;
        cbTotal += sizeof(cb) + cb;
        break;
      }
      case VT_UNKNOWN:
      case VT_DISPATCH:
      {
        // Object *must* support IPersistStream(Init)
        IUnknown* punk = va_arg(argptr, IUnknown*);
        IPersistStreamPtr sppstm(punk);
        if (punk && NULL == sppstm)
          ZSucceeded(punk->QueryInterface(IID_IPersistStreamInit,
            (void**)&sppstm));

        // Create a Null stream for calculating persisent size
        TCNullStreamImpl stm;

        // 'Persist' the object to the stream to determine its size
        OleSaveToStream(sppstm, &stm);

        // Get the object's persistent size
        ULARGE_INTEGER cbSize;
        LARGE_INTEGER dlibMove = {0, 0};
        ZSucceeded(stm.Seek(dlibMove, STREAM_SEEK_CUR, &cbSize));
        ZAssert(0 == cbSize.HighPart);
        cbTotal += sizeof(cbSize.LowPart) + cbSize.LowPart;
        break;
      }
      case VT_VARIANT:
        // TODO: Could recurse here to support VT_VARIANT
        ZError("Unsupported Variant Type");
        break;
      case VT_LPSTR:
      {
        LPCSTR psz = va_arg(argptr, LPCSTR);
        UINT cch = psz ? strlen(psz) : 0;
        cbTotal += sizeof(cch) + cch;
        break;
      }
      case VT_LPWSTR:
      {
        LPCWSTR psz = va_arg(argptr, LPCWSTR);
        UINT cch = psz ? wcslen(psz) : 0;
        cbTotal += sizeof(cch) + cch * sizeof(WCHAR);
        break;
      }
      default:
        ZError("Unsupported Variant Type");
    }
  }

  // Return the total size
  return cbTotal;
}


/////////////////////////////////////////////////////////////////////////////
// Useful macros for implementing data copying
//
#define COPY_DATA(pData, cb)           \
  {                                    \
    int _cb = (cb);                    \
    if (_cb)                           \
    {                                  \
      ZAssert(cbData >= _cb);           \
      CopyMemory(pbData, pData, _cb);  \
      cbData -= _cb;                   \
      pbData += _cb;                   \
    }                                  \
  }

#define COPY_VAR(var)                  \
  COPY_DATA(&var, sizeof(var))

#define VT_COPY(vt, type)              \
  case vt:                             \
  {                                    \
    type var = va_arg(argptr, type);   \
    COPY_VAR(var);                     \
    break;                             \
  }


/////////////////////////////////////////////////////////////////////////////
//
void CAGCEventData::CopyVariableData(LPCSTR pszContext, LPCOLESTR pszSubject,
  long cArgTriplets, va_list argptr)
{
  // Get initial pointers and byte count
  UINT  cbData = m_cbData - sizeof(XData);
  BYTE* pbData = m_pbData + sizeof(XData);

  // Start with the specified context string, if any
  UINT cb = pszContext ? strlen(pszContext) : 0;
  COPY_VAR(cb);
  COPY_DATA(pszContext, cb);

  // Next is the specified subject string, if any
  cb = pszSubject ? (wcslen(pszSubject) * sizeof(WCHAR)) : 0;
  COPY_VAR(cb);
  COPY_DATA(pszSubject, cb);

  // Iterate through the argument triplets
  for (long i = 0; i < cArgTriplets; ++i)
  {
    // Get the argument name
    LPCSTR pszArgName = va_arg(argptr, LPCSTR);
    cb = strlen(pszArgName);
    COPY_VAR(cb);
    COPY_DATA(pszArgName, cb);

    // Get the argument variant type and value
    VARTYPE vt = va_arg(argptr, VARTYPE);
    COPY_VAR(vt);
    switch (vt)
    {
      VT_COPY(VT_BOOL   , VARIANT_BOOL)
      VT_COPY(VT_I1     ,         CHAR)
      VT_COPY(VT_I2     ,        SHORT)
      VT_COPY(VT_I4     ,         LONG)
      VT_COPY(VT_UI1    ,         BYTE)
      VT_COPY(VT_UI2    ,       USHORT)
      VT_COPY(VT_ERROR  ,        SCODE)
      case VT_R4:
      {
        // (...) pushes a float argument onto the stack as a double
        float var = (float)va_arg(argptr, DOUBLE);
        COPY_VAR(var);
        break;
      }
      VT_COPY(VT_R8     ,       DOUBLE)
      VT_COPY(VT_DECIMAL,      DECIMAL)
      VT_COPY(VT_CY     ,           CY)
      VT_COPY(VT_DATE   ,         DATE)
      case VT_BSTR:
      {
        BSTR bstr = va_arg(argptr, BSTR);
        cb = bstr ? SysStringByteLen(bstr) : 0;
        COPY_VAR(cb);
        COPY_DATA(bstr, cb);
        break;
      }
      case VT_UNKNOWN:
      case VT_DISPATCH:
      {
        // Object *must* support IPersistStream(Init)
        IUnknown* punk = va_arg(argptr, IUnknown*);
        IPersistStreamPtr sppstm(punk);
        if (punk && NULL == sppstm)
          ZSucceeded(punk->QueryInterface(IID_IPersistStreamInit,
            (void**)&sppstm));

        // Create a simple stream on the memory block
        TCSimpleStream stm;
        stm.Init(cbData - sizeof(ULONG), pbData + sizeof(ULONG));

        // Persist the object to the memory block stream
        OleSaveToStream(sppstm, &stm);

        // Get the object's persistent size
        ULARGE_INTEGER cbSize;
        LARGE_INTEGER dlibMove = {0, 0};
        ZSucceeded(stm.Seek(dlibMove, STREAM_SEEK_CUR, &cbSize));
        ZAssert(0 == cbSize.HighPart);

        // Save the size of the persistent object
        COPY_VAR(cbSize.LowPart);

        // Just skip past bytes already written as stream
        cbData -= cbSize.LowPart;
        pbData += cbSize.LowPart;
        break;
      }
      case VT_VARIANT:
        // TODO: Could recurse here to support VT_VARIANT
        ZError("Unsupported Variant Type");
        break;
      case VT_LPSTR:
      {
        LPCSTR psz = va_arg(argptr, LPCSTR);
        cb = psz ? strlen(psz) : 0;
        COPY_VAR(cb);
        COPY_DATA(psz, cb);
        break;
      }
      case VT_LPWSTR:
      {
        LPCWSTR psz = va_arg(argptr, LPCWSTR);
        cb = psz ? (wcslen(psz) * sizeof(WCHAR)) : 0;
        COPY_VAR(cb);
        COPY_DATA(psz, cb);
        break;
      }
      default:
        ZError("Unsupported Variant Type");
    }
  }
}

/////////////////////////////////////////////////////////////////////////////
// Useful macro for implementing VARIANT creations of simple variant types.
//
#define VT_CREATE(_vt, type)                          \
  case VT##_vt:                                       \
    V##_vt(&var) = *reinterpret_cast<type*>(pbData);  \
    cbData = sizeof(type);                            \
    break;

/////////////////////////////////////////////////////////////////////////////
//
// Return Value: The number of bytes consumed from the input buffer. The
// caller should increment its pointer by this many bytes.
//
UINT CAGCEventData::CreateVARIANTFromData(BYTE* pbData, CComVariant& var)
{
  // Clear the specified VARIANT
  ZSucceeded(var.Clear());

  // Get the VARTYPE from the specified buffer
  V_VT(&var) = *reinterpret_cast<VARTYPE*>(pbData);
  pbData += sizeof(V_VT(&var));

  // Switch on the VARTYPE
  UINT cbData = 0;
  switch (V_VT(&var))
  {
    VT_CREATE(_BOOL   , VARIANT_BOOL)
    VT_CREATE(_I1     ,         CHAR)
    VT_CREATE(_I2     ,        SHORT)
    VT_CREATE(_I4     ,         LONG)
    VT_CREATE(_UI1    ,         BYTE)
    VT_CREATE(_UI2    ,       USHORT)
    VT_CREATE(_ERROR  ,        SCODE)
    VT_CREATE(_R4     ,        FLOAT)
    VT_CREATE(_R8     ,       DOUBLE)
    VT_CREATE(_DECIMAL,      DECIMAL)
    VT_CREATE(_CY     ,           CY)
    VT_CREATE(_DATE   ,         DATE)
    case VT_BSTR:
      cbData = CreateBSTRFromData(pbData, &V_BSTR(&var));
      break;
    case VT_UNKNOWN:
      cbData = CreateObjectFromData(pbData, IID_IUnknown,
        (void**)&V_UNKNOWN(&var));
      break;
    case VT_DISPATCH:
      cbData = CreateObjectFromData(pbData, IID_IDispatch,
        (void**)&V_DISPATCH(&var));
      break;
    case VT_VARIANT:
      // TODO: Could recurse here to support VT_VARIANT
      ZError("Unsupported Variant Type");
      break;
    case VT_LPSTR:
      cbData = CreateBSTRFromData_LPSTR(pbData, &V_BSTR(&var));
      V_VT(&var) = VT_BSTR;
      break;
    case VT_LPWSTR:
      cbData = CreateBSTRFromData_LPWSTR(pbData, &V_BSTR(&var));
      V_VT(&var) = VT_BSTR;
      break;
    default:
      ZError("Unsupported Variant Type");
  }

  // Return the number of bytes consumed from the buffer
  return sizeof(V_VT(&var)) + cbData;
}

/////////////////////////////////////////////////////////////////////////////
//
// Return Value: The number of bytes consumed from the input buffer. The
// caller should increment its pointer by this many bytes.
//
UINT CAGCEventData::CreateBSTRFromData(BYTE* pbData, BSTR* pbstr)
{
  // Get the size of the following persistence block
  UINT cbData = *reinterpret_cast<UINT*>(pbData);
  pbData += sizeof(cbData);

  // Create the BSTR from the persistence block
  *pbstr = SysAllocStringByteLen(reinterpret_cast<LPCSTR>(pbData), cbData);
  ZAssert(*pbstr);

  // Return the number of bytes consumed
  return sizeof(cbData) + cbData;
}

/////////////////////////////////////////////////////////////////////////////
//
// Return Value: The number of bytes consumed from the input buffer. The
// caller should increment its pointer by this many bytes.
//
UINT CAGCEventData::CreateBSTRFromData_LPSTR(BYTE* pbData, BSTR* pbstr)
{
  // Get the size of the following persistence block
  UINT cch = *reinterpret_cast<UINT*>(pbData);
  pbData += sizeof(cch);

  // Convert the ANSI persistence block to UNICODE
  LPOLESTR pwsz = (LPOLESTR)_alloca((cch + 1) * sizeof(OLECHAR));
  MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED,
    reinterpret_cast<LPCSTR>(pbData), cch, pwsz, cch + 1);

  // Create the BSTR from the converted persistence block
  *pbstr = SysAllocStringLen(pwsz, cch);
  ZAssert(*pbstr);

  // Return the number of bytes consumed
  return sizeof(cch) + cch;
}

/////////////////////////////////////////////////////////////////////////////
//
// Return Value: The number of bytes consumed from the input buffer. The
// caller should increment its pointer by this many bytes.
//
UINT CAGCEventData::CreateBSTRFromData_LPWSTR(BYTE* pbData, BSTR* pbstr)
{
  // Get the size of the following persistence block
  UINT cb = *reinterpret_cast<UINT*>(pbData);
  pbData += sizeof(cb);

  // Create the BSTR from the persistence block
  UINT cch = cb / sizeof(OLECHAR);
  *pbstr = SysAllocStringLen(reinterpret_cast<LPOLESTR>(pbData), cch);
  ZAssert(*pbstr);

  // Return the number of bytes consumed
  return sizeof(cb) + cb;
}

/////////////////////////////////////////////////////////////////////////////
//
// Return Value: The number of bytes consumed from the input buffer. The
// caller should increment its pointer by this many bytes.
//
UINT CAGCEventData::CreateObjectFromData(BYTE* pbData, REFIID riid,
  void** ppvUnk)
{
  // Get the size of the following persistence block
  UINT cbData = *reinterpret_cast<UINT*>(pbData);
  pbData += sizeof(cbData);

  // Create a simple stream on the persistence block
  TCSimpleStream stm;
  stm.Init(cbData, pbData);

  // Create the object from the data
  ZSucceeded(OleLoadFromStream(&stm, riid, ppvUnk));

  // Return the number of bytes consumed
  return sizeof(cbData) + cbData;
}

