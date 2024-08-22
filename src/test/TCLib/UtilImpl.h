#pragma once

/////////////////////////////////////////////////////////////////////////////
// UtilImpl.h : Declaration of the TCUtilImpl class.
//


/////////////////////////////////////////////////////////////////////////////
//
class TCUtilImpl
{
// Group=Construction
private:
  // This is private since class is not intended to be instantiated.
  TCUtilImpl() {} 

// Group=Operations
public:
  static STDMETHODIMP CreateObject(BSTR bstrProgID, BSTR bstrComputer,
    IUnknown** ppUnk);
  static STDMETHODIMP ObjectReference(IUnknown* pUnk,
    BSTR* pbstrObjectReference);
  static STDMETHODIMP Sleep(long nDurationMS);
  static STDMETHODIMP GetObject(BSTR bstrMoniker, VARIANT_BOOL bAllowUI,
    long nTimeoutMS, IUnknown** ppUnk);
};


/////////////////////////////////////////////////////////////////////////////