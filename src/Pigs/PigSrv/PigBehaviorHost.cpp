/////////////////////////////////////////////////////////////////////////////
// PigBehaviorHost.cpp : Implementation of the CPigBehaviorHost class.

#include "pch.h"
#include <TCLib.h>
#include <..\TCAtl\ObjectMap.h>
#include "Pig.h"
#include "PigBehaviorHost.h"


/////////////////////////////////////////////////////////////////////////////
// CPigBehaviorHost

TC_OBJECT_EXTERN_NON_CREATEABLE_IMPL(CPigBehaviorHost)


/////////////////////////////////////////////////////////////////////////////
// Construction / Destruction

CPigBehaviorHost::CPigBehaviorHost() :
  m_pPig(NULL)
{
}

HRESULT CPigBehaviorHost::FinalConstruct()
{
  // Indicate success
  return S_OK;
}

void CPigBehaviorHost::FinalRelease()
{
}

HRESULT CPigBehaviorHost::Init(CPig* pPig)
{
  ZAssert(!m_pPig);
  ZAssert(pPig);
  m_pPig = pPig;
  return S_OK;
}


/////////////////////////////////////////////////////////////////////////////
// Implementation

void CPigBehaviorHost::TraceOutput(LPCOLESTR pszName, LPCOLESTR pszText,
  LPCOLESTR pszTextFormatted)
{
  // Send the specified string to the debugger output
  if (IsWinNT())
  {
    OutputDebugStringW(pszTextFormatted);
  }
  else
  {
    USES_CONVERSION;
    OutputDebugString(OLE2CT(pszTextFormatted));
  }

  // Also send the string to the console window, if any
  fputws(pszTextFormatted, stdout);

  // Trigger an event
  _AGCModule.TriggerEvent(NULL, PigEventID_Trace, pszName, -1, -1, -1, 1,
    "Text", VT_LPWSTR, pszText);
}


/////////////////////////////////////////////////////////////////////////////
// ISupportErrorInfo Interface Methods

STDMETHODIMP CPigBehaviorHost::InterfaceSupportsErrorInfo(REFIID riid)
{
  static const IID* arr[] = 
  {
    &IID_IPigBehaviorHost,
  };
  for (int i = 0; i < sizeofArray(arr); ++i)
  {
    if (InlineIsEqualGUID(*arr[i], riid))
      return S_OK;
  }
  return S_FALSE;
}


/////////////////////////////////////////////////////////////////////////////
// IPigBehaviorHost Interface Methods

STDMETHODIMP CPigBehaviorHost::Beep(long nFrequency, long nDuration)
{
  // Note: If this is not scoped to the Win32 global scope, a stack fault
  // will eventually occur. I know, I did it once :)
  ::Beep(nFrequency, nDuration);
  return S_OK;
}

STDMETHODIMP CPigBehaviorHost::CreatePig(BSTR bstrType, BSTR bstrCommandLine,
  IPig** ppPig)
{
  // Delegate to the engine
  return GetEngine().CreatePig(bstrType, bstrCommandLine, ppPig);
}

STDMETHODIMP CPigBehaviorHost::Trace(BSTR bstrText)
{
  // Do nothing if the specified string is empty
  UINT cchText = BSTRLen(bstrText);
  if (!cchText)
    return S_OK;

  // Get the name of the pig, if any
  BSTR bstrName = m_pPig ? m_pPig->InternalGetName() : NULL;
  UINT cchName = BSTRLen(bstrName);
  if (cchName)
  {
    // Allocate a buffer for a formatted string
    UINT cch = cchName + 2 + cchText;
    LPOLESTR pszText = (LPOLESTR)_alloca(sizeof(OLECHAR) * (cch + 1));

    // Format the message
    swprintf(pszText, L"%ls: %ls", bstrName, bstrText);

    // Output the trace message
    TraceOutput(bstrName, bstrText, pszText);
  }
  else
  {
    // Output the trace message
    TraceOutput(bstrName, bstrText, bstrText);
  }

  // Indicate success
  return S_OK;
}

STDMETHODIMP CPigBehaviorHost::get_StateName(PigState ePigState,
  BSTR* pbstrPigState)
{
  // Delegate to the engine
  return GetEngine().get_StateName(ePigState, pbstrPigState);
}

STDMETHODIMP CPigBehaviorHost::Random(short* pnRandom)
{
  // Copy a random number to the [out] parameter
  CLEAROUT(pnRandom, (short)rand());
  return S_OK;
}
