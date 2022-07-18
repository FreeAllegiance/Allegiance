/////////////////////////////////////////////////////////////////////////////
// PigBehaviorScript.cpp : Implementation of the CPigBehaviorScript class.

#include "pch.h"
#include <TCLib.h>
#include <..\TCAtl\ObjectMap.h>

#include "Pig.h"
#include "PigBehaviorType.h"
#include "PigBehaviorScriptType.h"
#include "PigBehaviorStack.h"


/////////////////////////////////////////////////////////////////////////////
// CPigBehaviorStack

TC_OBJECT_EXTERN_NON_CREATEABLE_IMPL(CPigBehaviorStack)


/////////////////////////////////////////////////////////////////////////////
// Construction / Destruction

CPigBehaviorStack::CPigBehaviorStack() :
  m_pPig(NULL)
{
}

HRESULT CPigBehaviorStack::Init(CPig* pPig)
{
  // Save the specified pig reference
  ZAssert(!m_pPig);
  m_pPig = pPig;

  // Indicate success
  return S_OK;
}


/////////////////////////////////////////////////////////////////////////////
// Implementation


/////////////////////////////////////////////////////////////////////////////
// ISupportErrorInfo Interface Methods

STDMETHODIMP CPigBehaviorStack::InterfaceSupportsErrorInfo(REFIID riid)
{
  static const IID* arr[] = 
  {
    &IID_IPigBehaviorStack,
  };
  for (int i = 0; i < sizeofArray(arr); ++i)
  {
    if (InlineIsEqualGUID(*arr[i], riid))
      return S_OK;
  }
  return S_FALSE;
}


/////////////////////////////////////////////////////////////////////////////
// ITCCollection Interface Methods

STDMETHODIMP CPigBehaviorStack::get_Count(long* pnCount)
{
  // Delegate to the pig object
  ZAssert(m_pPig);
  return m_pPig->Stack_get_Count(pnCount);
}

STDMETHODIMP CPigBehaviorStack::get__NewEnum(IUnknown** ppunkEnum)
{
  // Delegate to the pig object
  ZAssert(m_pPig);
  return m_pPig->Stack_get__NewEnum(ppunkEnum);
}


/////////////////////////////////////////////////////////////////////////////
// IPigBehaviorStack Interface Methods

STDMETHODIMP CPigBehaviorStack::Push(BSTR bstrType, BSTR bstrCommandLine,
  IPigBehavior** ppBehavior)
{
  // Delegate to the pig object
  ZAssert(m_pPig);
  return m_pPig->Stack_Push(bstrType, bstrCommandLine, ppBehavior);
}

STDMETHODIMP CPigBehaviorStack::Pop()
{
  // Delegate to the pig object
  ZAssert(m_pPig);
  return m_pPig->Stack_Pop();
}

STDMETHODIMP CPigBehaviorStack::get_Top(long nFromTop,
  IPigBehavior** ppBehavior)
{
  // Delegate to the pig object
  ZAssert(m_pPig);
  return m_pPig->Stack_get_Top(nFromTop, ppBehavior);
}

STDMETHODIMP CPigBehaviorStack::get_Pig(IPig** ppPig)
{
  CLEAROUT(ppPig, (IPig*)NULL);
  ZAssert(m_pPig);
  IPigPtr spPig(m_pPig);
  *ppPig = spPig.Detach();
  return S_OK;
}

