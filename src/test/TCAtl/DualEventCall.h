#ifndef __DualEventCall_h__
#define __DualEventCall_h__

/////////////////////////////////////////////////////////////////////////////
// DualEventsCall.h | Declaration of the classes used in the for_each
// statements of the connection point sinks.

#ifndef _INC_STDARG
  #include <stdarg.h>
#endif // !_INC_STDARG

#ifndef __EventCall_h__
  #include "EventCall.h"
#endif // !__EventCall_h__


/////////////////////////////////////////////////////////////////////////////  
// TCComDualEventCall is used internally by the TCComDualEventCP_Fn macros as
// a base class for a function object that maintains the set of arguments
// passed to an IDispatch-based connection point sink.
//
// The function call operator() of this class will be called for each
// *IDispatch* connection point sink maintained by a connection point
// derived from TCComDualEventCP.
//
// Since this class is used internally by the TCComDualEventCP_Fn macros, it
// is unlikely that you will ever need to use it directly.
//
// Parameters:
//   If_vtbl - Specifies the interface name of the v-table connection point
// sink which is also implemented by the IDispatch-based connection point
// sink.
//
// See Also: TCComDualEventCP_Fn macros, TCComDualEventsCP, TCComEventCall,
// TCComEventsCP
template <class If_vtbl>
class ATL_NO_VTABLE TCComDualEventCall : public TCComEventCall<If_vtbl>
{
// Construction / Destruction
public:
  TCComDualEventCall();
  virtual ~TCComDualEventCall();

// Disallow copy constructor
private:
  TCComDualEventCall(const TCComDualEventCall&);

// Operators
public:
  HRESULT operator()(IDispatch* pdisp);

// Overrides
protected:
  virtual void OnCreateDispParams() = 0;

// Implementation
protected:
  void SetDispParams(DISPID dispid, UINT cArgs, ...);
  void SetDispParamsV(DISPID dispid, UINT cArgs, va_list argptr);

// Group=Data Members
protected:
  // A *DISPPARAMS* structure used in subsequent calls to
  // *IDispatch::Invoke*.
  //
  // See Also: TCComDualEventCall::operator()
  DISPPARAMS m_DispParams;
  // A dispatch ID used in subsequent calls to *IDispatch::Invoke*.
  //
  // See Also: TCComDualEventCall::operator()
  DISPID m_dispid;
};


/////////////////////////////////////////////////////////////////////////////
// Group=Construction / Destruction

/////////////////////////////////////////////////////////////////////////////
// Initializes the instance by setting m_dispid to *DISPID_UNKNOWN* and
// clearing m_DispParams to zeroes.
template <class If_vtbl>
inline TCComDualEventCall<If_vtbl>::TCComDualEventCall() :
  m_dispid(DISPID_UNKNOWN)
{
  // Initialize the DISPPARAMS structure
  ZeroMemory(&m_DispParams, sizeof(m_DispParams));
}

/////////////////////////////////////////////////////////////////////////////
// Deletes the dispatch arguments structure, if one was created.
//
// See Also: TCComDualEventCall::SetDispParams
template <class If_vtbl>
TCComDualEventCall<If_vtbl>::~TCComDualEventCall()
{
  if (NULL != m_DispParams.rgvarg)
    delete [] m_DispParams.rgvarg;
}


/////////////////////////////////////////////////////////////////////////////
// Group=Operators

/////////////////////////////////////////////////////////////////////////////
// Description: Function call operator.
//
// This function call operator calls *Invoke* on the specified
// *IDispatch* pointer, specifying m_dispid as the dispatch ID, and passing
// the arguments specified in a prior call to SetDispParams. If the arguments
// have not been specified yet, the virtual OnCreateDispParams override is
// called to specify the arguments.
//
// The current *IErrorInfo* for the thread is saved/restored before/after the
// *Invoke* method call so that it stays intact.
//
// Parameters:
//   pdisp - The *IDispatch* pointer on which to call *Invoke*.
//
// Return Value: The HRESULT of the *IDispatch::Invoke* call is returned.
//
// See Also: TCComDualEventCall::SetDispParams,
// TCComDualEventCall::OnCreateDispParams
template <class If_vtbl>
HRESULT TCComDualEventCall<If_vtbl>::operator()(IDispatch* pdisp)
{
  HRESULT hr = S_OK;
  if (NULL != pdisp)
  {
    // Format the class::function name
    char szFn[_MAX_PATH] = "";
    #ifdef _DEBUG
      sprintf(szFn, "TCComDualEventCall<%hs>::operator()",
        TCTypeName(If_vtbl));
    #endif

    __try
    {
      // Create the arguments array the first time only
      if (NULL == m_DispParams.rgvarg)
        OnCreateDispParams();

      // Reset the IErrorInfo* for this thread and invoke the event method
      _VERIFYE(SUCCEEDED(::SetErrorInfo(0, m_pei)));
      hr = pdisp->Invoke(m_dispid, IID_NULL, LOCALE_USER_DEFAULT,
        DISPATCH_METHOD, &m_DispParams, NULL, NULL, NULL);

      #ifdef _DEBUG
        char szMsg[_MAX_PATH];
        sprintf(szMsg, "Invoke for event call %hs", GetEventName());
        if (SUCCEEDED(hr))
        {
          //_TRACE2("%hs: %hs succeeded\n", szFn, szMsg);
        }
        else
        {
          strcat(szMsg, " returned");
          TCReportHR(szFn, szMsg, hr);
        }
      #endif _DEBUG
    }
    __except(1)
    {
      _TRACE1("%hs: Caught an unknown exception\n", szFn);
      return E_UNEXPECTED;
    }
  }

  // Return the last HRESULT
  return hr;
}


/////////////////////////////////////////////////////////////////////////////
// Group=Overrides

#ifdef _DOCJET_ONLY
  ///////////////////////////////////////////////////////////////////////////
  // Description: Pure-virtual override that creates the dispatch parameters.
  //
  // Overridden in derived classes to call SetDispParams. This is called only
  // once by the function call operator(), just prior to the first time that
  // the operator calls *IDispatch::Invoke*.
  //
  // See Also: TCComDualEventCall::SetDispParams,
  // TCComDualEventCall::SetDispParamsV, TCComDualEventCall::operator()
  template <class If_vtbl>
  virtual void TCComDualEventCall<If_vtbl>::OnCreateDispParams() = 0;
#endif // _DOCJET_ONLY


/////////////////////////////////////////////////////////////////////////////
// Group=Implementation

/////////////////////////////////////////////////////////////////////////////
// Description: Sets the dispatch ID and dispatch parameters for subsequent
// *Invoke* calls.
//
// Sets the dispatch ID and dispatch parameters for subsequent
// *Invoke* calls. This method simply accesses the variable argument list and
// passes it to SetDispParamsV. Please see that method for more information.
//
// Parameters:
//   dispid - The dispatch ID used in subsequent *Invoke* calls.
//   cArgs - The number of dispatch arguments specified in the variable
// argument list.
//
// See Also: TCComDualEventCall::SetDispParamsV,
// TCComDualEventCall::operator(), TCComDualEventCall::OnCreateDispParams
template <class If_vtbl>
void TCComDualEventCall<If_vtbl>::SetDispParams(DISPID dispid, UINT cArgs,
  ...)
{
  va_list argptr;
  va_start(argptr, cArgs);
  SetDispParamsV(dispid, cArgs, argptr);
  va_end(argptr);
}

/////////////////////////////////////////////////////////////////////////////
// Description: Sets the dispatch ID and dispatch parameters for subsequent
// *Invoke* calls.
//
// This method saves the specified /dispid/ and creates an array of
// *VARIANTARG* structures suitable for use with the
// *IDispatch::Invoke* method. This method is to be called by the
// derived-class implementation of the virtual OnCreateDispParams override.
//
// For each pair of arguments in the specified variable argument list,
// /argptr/, a *VARIANTARG* is created and initialized. The *VARIANTARG* is
// then assigned the variant type and variant value specified in the argument
// pair. So, for example, the following would set three dispatch parameters,
// of types *VT_BOOL*, *VT_I4*, and *VT_BSTR* to *VARIANT_TRUE*, 137, and
// "Hello", respectively:
//
//      CComBSTR bstrText("Hello");
//      pCall->SetDispParams(DISPID_HELLO, 3, VT_BOOL, VARIANT_TRUE,
//        VT_I4, 137, VT_BSTR, bstrText);
//
// Note: Notice that although 3 is specified as the /cArgs/ parameter, the
// variable argument list contains six actual parameters. This is because for
// each argument specified in /cArgs/, an argument /pair/ is implied, since
// each dispatch argument has a variant type and value.
//
// Parameters:
//   dispid - The dispatch ID used in subsequent *Invoke* calls.
//   cArgs - The number of dispatch arguments specified in the variable
// argument list.
//   argptr - The standardized representation of a variable argument list.
//
// See Also: TCComDualEventCall::SetDispParams,
// TCComDualEventCall::operator(), TCComDualEventCall::OnCreateDispParams
template <class If_vtbl>
void TCComDualEventCall<If_vtbl>::SetDispParamsV(DISPID dispid, UINT cArgs,
  va_list argptr)
{
  // This function should only be called, at most, once per instance
  ZAssert(NULL == m_DispParams.rgvarg);

  // Save the specified dispatch id
  m_dispid = dispid;

  // Return immediately if 0 arguments specified
  if (!cArgs)
    return;

  // Create the VARIANTARG array with the specified number of arguments
  m_DispParams.rgvarg = new VARIANTARG[m_DispParams.cArgs = cArgs];

  // Loop thru each argument and initialize the VARIANTARG array element
  while (cArgs--)
  {
    VariantInit(&m_DispParams.rgvarg[cArgs]);
    VARTYPE vt = va_arg(argptr, VARTYPE);
    switch (V_VT(&m_DispParams.rgvarg[cArgs]) = vt)
    {
      case VT_BOOL:
        V_BOOL(&m_DispParams.rgvarg[cArgs]) = va_arg(argptr, VARIANT_BOOL);
        break;
      case VT_I1:
        V_I1(&m_DispParams.rgvarg[cArgs]) = va_arg(argptr, CHAR);
        break;
      case VT_I2:
        V_I2(&m_DispParams.rgvarg[cArgs]) = va_arg(argptr, SHORT);
        break;
      case VT_I4:
        V_I4(&m_DispParams.rgvarg[cArgs]) = va_arg(argptr, LONG);
        break;
      case VT_UI1:
        V_UI1(&m_DispParams.rgvarg[cArgs]) = va_arg(argptr, BYTE);
        break;
      case VT_UI2:
        V_UI2(&m_DispParams.rgvarg[cArgs]) = va_arg(argptr, USHORT);
        break;
      case VT_ERROR:
        V_ERROR(&m_DispParams.rgvarg[cArgs]) = va_arg(argptr, SCODE);
        break;
      case VT_R4:
        V_R4(&m_DispParams.rgvarg[cArgs]) = va_arg(argptr, FLOAT);
        break;
      case VT_R8:
        V_R8(&m_DispParams.rgvarg[cArgs]) = va_arg(argptr, DOUBLE);
        break;
      case VT_DECIMAL:
        V_DECIMAL(&m_DispParams.rgvarg[cArgs]) = va_arg(argptr, DECIMAL);
        break;
      case VT_CY:
        V_CY(&m_DispParams.rgvarg[cArgs]) = va_arg(argptr, CY);
        break;
      case VT_DATE:
        V_DATE(&m_DispParams.rgvarg[cArgs]) = va_arg(argptr, DATE);
        break;
      case VT_BSTR:
        V_BSTR(&m_DispParams.rgvarg[cArgs]) = va_arg(argptr, BSTR);
        break;
      case VT_UNKNOWN:
        V_UNKNOWN(&m_DispParams.rgvarg[cArgs]) = va_arg(argptr, IUnknown*);
        break;
      case VT_DISPATCH:
        V_DISPATCH(&m_DispParams.rgvarg[cArgs]) = va_arg(argptr, IDispatch*);
        break;
      case VT_VARIANT:
        V_VARIANTREF(&m_DispParams.rgvarg[cArgs]) = va_arg(argptr, VARIANT*);
        break;
      default:
        _TRACE_BEGIN
          _TRACE_PART0("TCComDualEventCall<If_vtbl>::SetDispParamsV(): ");
          _TRACE_PART2("Specified VARTYPE %hu (0x%04X) is unsupported\n", vt, vt);
        _TRACE_END
        _ASSERT(false);
    }
  }
} 


/////////////////////////////////////////////////////////////////////////////

#endif // !__DualEventCall_h__
