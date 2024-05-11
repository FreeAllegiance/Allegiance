#pragma once

/////////////////////////////////////////////////////////////////////////////
// InvokeArgs.h | Declaration of the classes used in the for_each
// statements of the connection point sinks.

#ifndef _INC_STDARG
  #include <stdarg.h>
#endif // !_INC_STDARG


/////////////////////////////////////////////////////////////////////////////  
// TCInvokeArgs is used to maintains the set of arguments passed to an
// IDispatch::Invoke method.
//
class TCInvokeArgs : public tagDISPPARAMS
{
// Construction / Destruction
public:
  TCInvokeArgs();
  TCInvokeArgs(DISPID dispid, UINT nArgs, ...);
  virtual ~TCInvokeArgs();
  void CommonConstruct();

// Disallow copy constructor
private:
  TCInvokeArgs(const TCInvokeArgs&);

// Attributes
public:
  void SetDispParams(DISPID dispid, UINT nArgs, ...);
  void SetDispParamsV(DISPID dispid, UINT nArgs, va_list argptr);

// Operators
public:
  HRESULT Invoke(IDispatch* pdisp, VARIANT* pvarResult = NULL);
  void Empty();

// Group=Data Members
protected:
  // A dispatch ID used in subsequent calls to *IDispatch::Invoke*.
  //
  // See Also: TCInvokeArgs::Invoke, TCInvokeArgs::SetDispParams
  DISPID m_dispid;
};


/////////////////////////////////////////////////////////////////////////////
// Group=Construction / Destruction

/////////////////////////////////////////////////////////////////////////////
// Initializes the instance by setting m_dispid to *DISPID_UNKNOWN* and
// clearing the base structure to zeroes.
inline TCInvokeArgs::TCInvokeArgs()
{
  CommonConstruct();
}

inline TCInvokeArgs::TCInvokeArgs(DISPID dispid, UINT nArgs, ...)
{
  CommonConstruct();
  va_list argptr;
  va_start(argptr, nArgs);
  SetDispParamsV(dispid, nArgs, argptr);
  va_end(argptr);
}

inline void TCInvokeArgs::CommonConstruct()
{
  // Reset the DISPID
  m_dispid = DISPID_UNKNOWN;

  // Initialize the DISPPARAMS structure
  ZeroMemory(static_cast<tagDISPPARAMS*>(this), sizeof(tagDISPPARAMS));
}

/////////////////////////////////////////////////////////////////////////////
// Group=Attributes

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
// See Also: TCInvokeArgs::SetDispParamsV, TCInvokeArgs::Invoke,
// TCInvokeArgs::OnCreateDispParams, TCInvokeArgs::m_dispid
inline void TCInvokeArgs::SetDispParams(DISPID dispid, UINT nArgs, ...)
{
  va_list argptr;
  va_start(argptr, nArgs);
  SetDispParamsV(dispid, nArgs, argptr);
  va_end(argptr);
}


/////////////////////////////////////////////////////////////////////////////
// Group=Operations

/////////////////////////////////////////////////////////////////////////////
// Empties the instance by setting m_dispid to *DISPID_UNKNOWN* and clearing
// the base structure, releasing any memory previously allocated.
inline void TCInvokeArgs::Empty()
{
  // Release any previously allocated memory
  if (NULL != rgvarg)
  {
    delete [] rgvarg;
    CommonConstruct();
  }
}


/////////////////////////////////////////////////////////////////////////////