#pragma once

/////////////////////////////////////////////////////////////////////////////
// EventsCall.h | Declaration of the classes used in the for_each statements
// of the connection point sinks.


/////////////////////////////////////////////////////////////////////////////  
// TCComEventCall is used internally by the TCComEventCP_Fn macros as a base
// class for a function object that maintains the set of arguments passed to
// a v-table connection point sink.
//
// The function call operator() of this class will be called for each
// v-table connection point sink maintained by a connection point derived
// from TCComEventCP.
//
// Since this class is used internally by the TCComEventCP_Fn macros, it is
// unlikely that you will ever need to use it directly.
//
// Parameters:
//   If_vtbl - Specifies the interface name of the v-table connection point
// sink.
//
// See Also: TCComEventCP_Fn macros, TCComEventsCP, TCComDualEventCall,
// TCComDualEventsCP
template <class If_vtbl>
class ATL_NO_VTABLE TCComEventCall
{
// Construction / Destruction
public:
  TCComEventCall();
  virtual ~TCComEventCall();

// Disallow copy constructor
private:
  TCComEventCall(const TCComEventCall&);

// Attributes
public:
  void SetEventName(LPCSTR pszName);
  LPCSTR GetEventName() const;

// Operators
public:
  HRESULT operator()(If_vtbl* pvtbl);

// Overrides
protected:
  virtual HRESULT OnCallEventSink(If_vtbl* pvtbl) = 0;

// Implementation
protected:
  void ResetErrorInfo();

// Data Members
protected:
  // A smart-pointer that maintains the *IErrorInfo* object of the thread for
  // each call to a connection point sink.
  //
  // See Also: TCComEventCall::ResetErrorInfo, TCComEventCall::operator()
  CComPtr<IErrorInfo> m_pei;
  // A pointer to the name of the event, used for diagnostic purposes.
  //
  // See Also: TCComEventCall::SetEventName, TCComEventCall::GetEventName,
  // TCComEventCall::operator()
  LPCSTR              m_pszName;
};


/////////////////////////////////////////////////////////////////////////////
// Group=Construction / Destruction

/////////////////////////////////////////////////////////////////////////////
// Initializes the instance by setting m_pszName to *NULL* and copying the
// current *IErrorInfo* of the thread.
//
// See Also: TCComEventCall::ResetErrorInfo
template <class If_vtbl>
inline TCComEventCall<If_vtbl>::TCComEventCall() :
  m_pszName(NULL)
{
  // Save the current error info object of this thread
  m_pei = NULL;
  _VERIFYE(SUCCEEDED(GetErrorInfo(0, &m_pei)));
  ResetErrorInfo();
}

template <class If_vtbl>
inline TCComEventCall<If_vtbl>::~TCComEventCall()
{
}


/////////////////////////////////////////////////////////////////////////////
// Group=Attributes

/////////////////////////////////////////////////////////////////////////////
// Description: Sets the name of the event used for diagnostic purposes.
//
// The TCComEventCP_Fn macros call this with the name of the event. It is
// used for diagnostic messages that indicate the success or failure of a
// call to an event sink.
//
// Parameters:
//   pszName - Points to the name of the event. This should be a constant
// string literal, since only the pointer is saved.
//
// See Also: TCComEventCall::GetEventName, TCComEventCall::m_pszName
template <class If_vtbl>
inline void TCComEventCall<If_vtbl>::SetEventName(LPCSTR pszName)
{
  m_pszName = pszName;
}

/////////////////////////////////////////////////////////////////////////////
// Description: Gets the name of the event used for diagnostic purposes.
//
// This is used for diagnostic messages that indicate the success or failure
// of a call to an event sink.
//
// Return Value: Returns the same pointer specified in a previous call to
// SetEventName, or *NULL* if the name has not been set.
//
// See Also: TCComEventCall::SetEventName, TCComEventCall::m_pszName
template <class If_vtbl>
inline LPCSTR TCComEventCall<If_vtbl>::GetEventName() const
{
  return m_pszName;
}


/////////////////////////////////////////////////////////////////////////////
// Group=Operators

/////////////////////////////////////////////////////////////////////////////
// Description: Function call operator.
//
// This function call operator calls the event method on the specified
// v-table interface pointer by delegating to the virtual OnCallEventSink
// override of the derived class. The derived class maintains the set of 
// arguments associated with the event call. Prior to each call, the
// *IErrorInfo* of the current thread is reset to that of the thread that
// initiated the event call.
//
// Parameters:
//   pvtbl - The interface pointer of the connected event sink.
//
// Return Value: The HRESULT of the event call is returned.
//
// See Also: TCComEventCall::OnCallEventSink, TCComEventCP_Fn macros
template <class If_vtbl>
HRESULT TCComEventCall<If_vtbl>::operator()(If_vtbl* pvtbl)
{
  HRESULT hr = S_OK;
  if (NULL != pvtbl)
  {
    // Format the class::function name
    char szFn[_MAX_PATH] = "";
    #ifdef _DEBUG
      sprintf(szFn, "TCComEventCall<%hs>::operator()", TCTypeName(If_vtbl));
    #endif

    __try
    {
      // Reset the IErrorInfo* for this thread and call the event
      ResetErrorInfo();
      hr = OnCallEventSink(pvtbl);

      #ifdef _DEBUG
        char szMsg[_MAX_PATH];
        sprintf(szMsg, "Event call %hs", GetEventName());
        if (SUCCEEDED(hr))
          _TRACE2("%hs: %hs succeeded\n", szFn, szMsg);
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
  // Description: Pure-virtual override that makes an event call to the
  // specified connection point sink.
  //
  // Overridden in derived classes to makes an event call to the specified
  // connection point sink. This is called by the function call operator()
  // for each connected event sink.
  //
  // Parameters:
  //   pvtbl - The interface pointer of the connected event sink.
  //
  // Return Value: The HRESULT of the event call should be returned.
  //
  // See Also: TCComEventCall::operator(), TCComEventCP_Fn macros
  template <class If_vtbl>
  virtual HRESULT TCComEventCall<If_vtbl>::OnCallEventSink(If_vtbl* pvtbl);
#endif // _DOCJET_ONLY


/////////////////////////////////////////////////////////////////////////////
// Group=Implementation

/////////////////////////////////////////////////////////////////////////////
// Description: Resets the *IErrorInfo* of the current thread.
//
// This method is called by the function call operator() to reset the
// *IErrorInfo* of the current thread to that of the thread that initiated
// the event call.
//
// See Also: TCComEventCall::operator(), TCComEventCall::m_pei
template <class If_vtbl>
inline void TCComEventCall<If_vtbl>::ResetErrorInfo()
{
  _SVERIFYE(::SetErrorInfo(0, m_pei));
}


/////////////////////////////////////////////////////////////////////////////