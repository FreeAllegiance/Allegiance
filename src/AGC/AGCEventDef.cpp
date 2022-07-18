/////////////////////////////////////////////////////////////////////////////
// AGCEventDef.cpp : Implementation of CAGCEventDef
//

#include "pch.h"
#include <AGC.h>
#include "AGCEventDef.h"
#include "resource.h"
#include <..\TCLib\ObjectLock.h>


/////////////////////////////////////////////////////////////////////////////
// TCSimpleLock Adaptor

class TCSimpleLock
{
// Construction
public:
  TCSimpleLock(LONG& nSync) :
    m_nSync(nSync) {}

// Operations
public:
  void Lock()
  {
    while (InterlockedExchange(&m_nSync, 1))
      Sleep(10);
  }
  void Unlock()
  {
    InterlockedExchange(&m_nSync, 0);
  }

// Data Members
protected:
  LONG& m_nSync;
};


/////////////////////////////////////////////////////////////////////////////
// CAGCEventDef


/////////////////////////////////////////////////////////////////////////////
// Include the XML/XSL-generated files
#include <AGCEventsRCH.h>
#include <AGCEventsCPP.h>


/////////////////////////////////////////////////////////////////////////////
// Static Initialization

LONG                    CAGCEventDef::s_nInitSync = 0;
bool                    CAGCEventDef::s_bInitialized = false;
CAGCEventDef::XNameMap* CAGCEventDef::s_pNameMap = NULL;
CAGCEventDef            CAGCEventDef::s_Instance;


///////////////////////////////////////////////////////////////////////////
// Construction / Destruction

CAGCEventDef::CAGCEventDef()
{
  // Debug-only data verification
  #ifdef _DEBUG
  {
    // Validate that all table entries are in ascending order
    const XEventDef* a = begin();
    const XEventDef* b = a + 1;
    while (end() != b)
      ZAssert(XLess()(*a++, *b++));

    // Validate that all group scopes match
    int nIndent = 0;
    for (const XEventDef* it = begin(); end() != it; ++it)
    {
      ZAssert(-1 <= it->m_nIndent && it->m_nIndent <= 1);
      nIndent += it->m_nIndent;
      ZAssert(0 <= nIndent);
    }
    ZAssert(0 == nIndent);
  }
  #endif // _DEBUG
}

void CAGCEventDef::Initialize()
{
  TCSimpleLock spinLock(s_nInitSync);
  TCObjectLock<TCSimpleLock> lock(&spinLock);
  if (s_bInitialized)
    return;

  // Map all of the event names to ID's
  s_pNameMap = new XNameMap;
  for (const XEventDef* it = begin(); end() != it; ++it)
  {
    if (HIWORD(reinterpret_cast<DWORD>(it->m_pszName)))
      (*s_pNameMap)[it->m_pszName] = it->m_id;
    else
    {
      BSTR bstrName;
      ZSucceeded(GetString(it->m_pszName, &bstrName));
      (*s_pNameMap)[bstrName] = it->m_id;
    }
  }
  s_bInitialized = true;
}

void CAGCEventDef::Terminate()
{
  TCSimpleLock spinLock(s_nInitSync);
  TCObjectLock<TCSimpleLock> lock(&spinLock);
  if (!s_bInitialized)
    return;

  // Free each BSTR in the name map
  for (XNameMapIt it = s_pNameMap->begin(); it != s_pNameMap->end(); ++it)
  {
    const XEventDef* itFind = find(it->second);
    ZAssert(itFind != end());
    if (!HIWORD(reinterpret_cast<DWORD>(itFind->m_pszName)))
      SysFreeString(const_cast<BSTR>(it->first));
  }
  delete s_pNameMap;
  s_pNameMap = NULL;
  s_bInitialized = false;
}


/////////////////////////////////////////////////////////////////////////////
// Attributes

HRESULT CAGCEventDef::GetEventName(AGCEventID idEvent, BSTR* pbstrOut)
{
  // Initialize the [out] parameter
  *pbstrOut = NULL;

  // Find the definition of the event ID
  const XEventDef* it = find(idEvent);
  if (end() == it)
    return E_INVALIDARG;

  // Get the event name string
  HRESULT hr = GetString(it->m_pszName, pbstrOut);

  // Indicate success
  return S_OK;
}

HRESULT CAGCEventDef::GetEventDescription(AGCEventID idEvent, BSTR* pbstrOut)
{
  // Initialize the [out] parameter
  *pbstrOut = NULL;

  // Find the definition of the event ID
  const XEventDef* it = find(idEvent);
  if (end() == it)
    return E_INVALIDARG;

  // Get the event description string
  HRESULT hr = GetString(it->m_pszEventDescription, pbstrOut);

  // If not specified, use the event name string
  return (S_FALSE != hr) ? hr : GetEventName(it->m_id, pbstrOut);
}

HRESULT CAGCEventDef::GetEventDescription(IAGCEvent* pEvent, BSTR* pbstrOut,
  const XEventDef* pDefHint)
{
  // Initialize the [out] parameter
  *pbstrOut = NULL;
  ZAssert(pEvent);

  // Find the definition of the event ID, if not specified
  if (!pDefHint)
  {
    // Get the specified event's ID
    AGCEventID idEvent;
    RETURN_FAILED(pEvent->get_ID(&idEvent));

    // Lookup the event ID in the table
    const XEventDef* it = find(idEvent);
    if (end() == it)
      return E_INVALIDARG;
    pDefHint = it;
  }
  else
  {
    #ifdef _DEBUG
      // Get the specified event's ID
      AGCEventID idEvent;
      RETURN_FAILED(pEvent->get_ID(&idEvent));
      ZAssert(pDefHint->m_id == idEvent);
    #endif // _DEBUG
  }

  // Get the event description formatting string
  CComBSTR bstrFmt;
  HRESULT hr = GetString(pDefHint->m_pszFormatDescription, &bstrFmt);
  RETURN_FAILED(hr);
  if (S_FALSE == hr)
  {
    // Use the static description
    return GetEventDescription(pDefHint->m_id, pbstrOut);
  }

  // Format the event
  ZAssert(pEvent);
  return ExpandFmtString(bstrFmt, pEvent, pbstrOut);
}

HRESULT CAGCEventDef::GetEventParameters (IAGCEvent* pEvent,
  CAGCEventDef::XParamStrings& rParamStrings, const XEventDef* pDefHint)
{
  ZAssert(pEvent);

  // Find the definition of the event ID, if not specified
  if (!pDefHint)
  {
    // Get the specified event's ID
    AGCEventID idEvent;
    RETURN_FAILED(pEvent->get_ID(&idEvent));

    // Lookup the event ID in the table
    const XEventDef* it = find(idEvent);
    if (end() == it)
      return E_INVALIDARG;
    pDefHint = it;
  }
  else
  {
    #ifdef _DEBUG
      // Get the specified event's ID
      AGCEventID idEvent;
      RETURN_FAILED(pEvent->get_ID(&idEvent));
      ZAssert(pDefHint->m_id == idEvent);
    #endif // _DEBUG
  }

  // Get the event description formatting string
  CComBSTR bstrFmt;
  HRESULT hr = GetString(pDefHint->m_pszFormatDescription, &bstrFmt);
  if (S_OK != hr)
    return hr;

  // Get the event parameters
  return ExpandParams(bstrFmt, pEvent, rParamStrings);
}


/////////////////////////////////////////////////////////////////////////////
// Implementation

HRESULT CAGCEventDef::GetString(LPCOLESTR psz, BSTR* pbstrOut)
{
  // Get the specified string as a DWORD
  DWORD dw = reinterpret_cast<DWORD>(psz);

  // If HIWORD is non-null, this is a string pointer
  if (HIWORD(dw))
  {
    USES_CONVERSION;
    *pbstrOut = SysAllocString(psz);
    return *pbstrOut ? (SysStringLen(*pbstrOut) ? S_OK : S_FALSE)
                     : E_OUTOFMEMORY;
  }
  return TCLoadBSTR(_Module.GetResourceInstance(), dw, pbstrOut);
}

HRESULT CAGCEventDef::ExpandFmtString(BSTR bstrFmt, IAGCEvent* pEvent,
  BSTR* pbstrOut)
{
  ZAssert(BSTRLen(bstrFmt));
  ZAssert(pbstrOut);

  // Create a growable stream into which we'll write
  IStreamPtr spStm;
  RETURN_FAILED(CreateStreamOnHGlobal(NULL, true, &spStm));

  // Initialize the parsing data
  XParseData data =
    {pEvent, spStm, NULL, NULL, NULL, bstrFmt, NULL, false};

  // Iterate through the characters of the format string
  XStateProc pfnState = ParseState_Base;
  for (UINT cch = SysStringLen(bstrFmt); cch && pfnState; --cch)
  {
    RETURN_FAILED(pfnState(data));
    pfnState = data.m_pfnNextState;
    ++data.m_pszInput;
  }
  data.m_bEndOfString = true;
  while (pfnState)
  {
    RETURN_FAILED(pfnState(data));
    pfnState = data.m_pfnNextState;
  }

  // Get the current seek pointer of the stream (which is it's size)
  LARGE_INTEGER li = {0};
  ULARGE_INTEGER uli;
  RETURN_FAILED(spStm->Seek(li, STREAM_SEEK_CUR, &uli));
  UINT cchStream = (UINT)uli.QuadPart / sizeof(OLECHAR);

  // Get the HGLOBAL underlying the stream
  HGLOBAL hGlobal = NULL;
  RETURN_FAILED(GetHGlobalFromStream(spStm, &hGlobal));
  ZAssert(hGlobal);

  // Lock the HGLOBAL
  LPCOLESTR pszOut = reinterpret_cast<LPCOLESTR>(GlobalLock(hGlobal));
  ZAssert(pszOut);

  // Create a BSTR from the byte stream
  *pbstrOut = SysAllocStringLen(pszOut, cchStream);

  // Unlock the HGLOBAL
  GlobalUnlock(hGlobal);

  // Indicate success or failure
  return *pbstrOut ? S_OK : E_OUTOFMEMORY;
}

HRESULT CAGCEventDef::ExpandParams(BSTR bstrFmt, IAGCEvent* pEvent,
  CAGCEventDef::XParamStrings& rParams)
{
  ZAssert(BSTRLen(bstrFmt));

  // Initialize the parsing data
  XParseData data =
    {pEvent, NULL, &rParams, NULL, NULL, bstrFmt, NULL, false};

  // Iterate through the characters of the format string
  XStateProc pfnState = ParseState_Base;
  for (UINT cch = SysStringLen(bstrFmt); cch && pfnState; --cch)
  {
    RETURN_FAILED(pfnState(data));
    pfnState = data.m_pfnNextState;
    ++data.m_pszInput;
  }
  data.m_bEndOfString = true;
  while (pfnState)
  {
    RETURN_FAILED(pfnState(data));
    pfnState = data.m_pfnNextState;
  }

  // Indicate success
  return S_OK;
}

HRESULT __fastcall
CAGCEventDef::ParseState_WriteInput(CAGCEventDef::XParseData& data)
{
  HRESULT hr = S_OK;
  if (data.m_pStm)
    hr = data.m_pStm->Write(data.m_pszInput, sizeof(*data.m_pszInput), NULL);
  data.m_pfnNextState = SUCCEEDED(hr) ? ParseState_Base : NULL;
  return hr;
}

HRESULT __fastcall
CAGCEventDef::ParseState_WriteVar(CAGCEventDef::XParseData& data)
{
  UINT cch = data.m_pszVarEnd - data.m_pszVarBegin;
  UINT cb  = cch * sizeof(*data.m_pszVarEnd);
  HRESULT hr = S_OK;
  if (data.m_pStm)
    hr = data.m_pStm->Write(data.m_pszVarBegin, cb, NULL);
  data.m_pfnNextState = SUCCEEDED(hr) ? ParseState_Base : NULL;
  return hr;
}

HRESULT __fastcall
CAGCEventDef::ParseState_End(CAGCEventDef::XParseData& data)
{
  ZAssert(data.m_bEndOfString);
  data.m_pfnNextState = NULL;
  return S_OK;
}

HRESULT __fastcall
CAGCEventDef::ParseState_Base(CAGCEventDef::XParseData& data)
{
  if (data.m_bEndOfString)
    return ParseState_End(data);

  if (OLESTR('%') != *data.m_pszInput)
    return ParseState_WriteInput(data);

  data.m_pszVarBegin = data.m_pszInput + 1;
  data.m_pfnNextState = ParseState_InVar;
  return S_OK;
}

HRESULT __fastcall
CAGCEventDef::ParseState_InVar(CAGCEventDef::XParseData& data)
{
  if (data.m_bEndOfString)
  {
    --data.m_pszVarBegin;
    data.m_pszVarEnd = data.m_pszInput;
    RETURN_FAILED(ParseState_WriteVar(data));
    return ParseState_End(data);
  }
  if (OLESTR('%') == *data.m_pszInput)
  {
    if (data.m_pszInput == data.m_pszVarBegin)
      return ParseState_WriteInput(data);

    data.m_pszVarEnd = data.m_pszInput;
    return ParseState_ProcessVar(data);
  }
  return S_OK;
}

HRESULT __fastcall
CAGCEventDef::ParseState_ProcessVar(CAGCEventDef::XParseData& data)
{
  // Get the variable as a VT_BSTR variant
  UINT cch = data.m_pszVarEnd - data.m_pszVarBegin;
  CComVariant varKey;
  V_VT(&varKey) = VT_BSTR;
  V_BSTR(&varKey) = ::SysAllocStringLen(data.m_pszVarBegin, cch);
  if (!V_BSTR(&varKey))
  {
    data.m_pfnNextState = NULL;
    return E_OUTOFMEMORY;
  }

  // Lookup the variable in the event's properties
  CComVariant varValue;
  HRESULT hr = data.m_pEvent->get_Property(&varKey, &varValue);
  if (FAILED(hr))
  {
    data.m_pfnNextState = NULL;
    return hr;
  }

  // Just write the variable name as-is if it doesn't exist in the event
  if (VT_EMPTY == V_VT(&varValue))
  {
    // Include the leading and trailing '%' characters
    --data.m_pszVarBegin;
    ++data.m_pszVarEnd;

    if (data.m_pParams)
    {
      cch = data.m_pszVarEnd - data.m_pszVarBegin;
      BSTR bstrValue = SysAllocStringLen(data.m_pszVarBegin, cch);
      if (!bstrValue)
      {
        data.m_pfnNextState = NULL;
        return E_OUTOFMEMORY;
      }
      data.m_pParams->push_back(bstrValue);
    }
    return ParseState_WriteVar(data);
  }

  // Convert the variant to a string  
  if (VT_BSTR != V_VT(&varValue))
  {
    VariantChangeTypeEx(&varValue, &varValue, GetThreadLocale(),
      VARIANT_LOCALBOOL | VARIANT_ALPHABOOL, VT_BSTR);
    if (VT_BSTR != V_VT(&varValue))
    {
      data.m_pfnNextState = ParseState_Base;
      return S_OK;
    }
  }
  UINT cchValue = BSTRLen(V_BSTR(&varValue));
  if (!cchValue)
  {
    if (data.m_pParams)
    {
      BSTR bstrValue = SysAllocString(L"");
      if (!bstrValue)
      {
        data.m_pfnNextState = NULL;
        return E_OUTOFMEMORY;
      }
      data.m_pParams->push_back(bstrValue);
    }
    data.m_pfnNextState = ParseState_Base;
    return S_OK;
  }

  // Write the value string
  data.m_pszVarBegin = V_BSTR(&varValue);
  data.m_pszVarEnd   = data.m_pszVarBegin + cchValue;
  RETURN_FAILED(ParseState_WriteVar(data));
  if (data.m_pParams)
  {
    data.m_pParams->push_back(V_BSTR(&varValue));
    V_VT(&varValue) = VT_EMPTY; // Detaches the BSTR from the VARIANT
  }

  // Indicate success
  return S_OK;
}

