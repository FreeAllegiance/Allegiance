/////////////////////////////////////////////////////////////////////////////
// PigEvent.h : Declaration of the CPigEvent class

#pragma once

#include "resource.h"       // main symbols
#include "PigSrv.h"


/////////////////////////////////////////////////////////////////////////////
// Forward Declarations

class CPigEventOwner;
class ImodelIGC;


/////////////////////////////////////////////////////////////////////////////
// CPigEvent
class ATL_NO_VTABLE CPigEvent
{
// Construction / Destruction
public:
  CPigEvent();
  HRESULT Init(CPigEventOwner* pOwner);
  void Term();

// Attributes
protected:
  static long GetNextEventID();

// Overrides
public:
  virtual void LockT() = 0;
  virtual void UnlockT() = 0;
  virtual long GetEventID() = 0;
  virtual void* GetMostDerivedPointer() = 0;
  virtual bool IsExpired() = 0;
  virtual bool Pulse(bool bExpired) = 0;
  virtual void DoDefaultExpiration() {};
  virtual bool ModelTerminated(ImodelIGC* pModelIGC) {return false;}

// IUnknown Interface Methods
public:
  STDMETHOD_(ULONG,AddRef)() = 0;
  STDMETHOD_(ULONG,Release)() = 0;

// IPigEvent Interface Methods
public:
  STDMETHOD(put_Name)(BSTR bstrName);
  STDMETHOD(get_Name)(BSTR* pbstrName);
  STDMETHOD(put_ExpirationExpression)(BSTR bstrExpr);
  STDMETHOD(get_ExpirationExpression)(BSTR* pbstrExpr);
  STDMETHOD(get_IsExpired)(VARIANT_BOOL* pbExpired);
  STDMETHOD(Kill)();
  STDMETHOD(put_InterruptionExpression)(BSTR bstrExpr);
  STDMETHOD(get_InterruptionExpression)(BSTR* pbstrExpr);

// Data Members
protected:
  CPigEventOwner* m_pOwner;
  CComBSTR        m_bstrName;
  CComBSTR        m_bstrExpirationExpression;
  CComBSTR        m_bstrInterruptionExpression;
  bool            m_bKilled;
};


/////////////////////////////////////////////////////////////////////////////
// Construction / Destruction

inline CPigEvent::CPigEvent() :
  m_pOwner(NULL),
  m_bKilled(false)
{
}


/////////////////////////////////////////////////////////////////////////////
// Attributes

inline long CPigEvent::GetNextEventID()
{
  static long id = 0;
  long idNext = id;
  InterlockedIncrement(&id);
  return idNext;
}


/////////////////////////////////////////////////////////////////////////////
// 
template <class T, class I, const IID* piid>
class ATL_NO_VTABLE IPigEventImpl :
  public IDispatchImpl<I, piid, &LIBID_PigsLib>,
  public CPigEvent
{
// Attributes
protected:
  static long InternalGetEventID()
  {
    static long id = -1;
    if (-1 == id)
      InterlockedExchange(&id, CPigEvent::GetNextEventID());
    return id;
  }
public:
  static bool IsEventOfThisType(CPigEvent* pEvent)
  {
    return pEvent->GetEventID() == InternalGetEventID();
  }

  static T* DynamicCast(CPigEvent* pEvent)
  {
    return reinterpret_cast<T*>(IsEventOfThisType(pEvent) ?
      pEvent->GetMostDerivedPointer() : NULL);
  }

// Overrides
public:
  virtual void LockT()
  {
    static_cast<T*>(this)->Lock();
  }

  virtual void UnlockT()
  {
    static_cast<T*>(this)->Unlock();
  }

  virtual long GetEventID()
  {
    return InternalGetEventID();
  }

  virtual void* GetMostDerivedPointer()
  {
    return static_cast<T*>(this);
  }

// IPigEvent Interface Methods
public:
  STDMETHODIMP put_Name(BSTR bstrName)
  {
    // Get a pointer to the most-derived class
    T* pT = static_cast<T*>(this);

    // Validate that a name was specified
    if (!BSTRLen(bstrName))
      return pT->Error(IDS_E_TIMER_NONAME, IID_IPigEvent);

    // Only do the next part if we're attached to an owner
    T::XLock lock(pT);
    if (pT->m_pOwner)
    {
      // Get the Properties dictionary
      IDictionaryPtr spProperties;
      RETURN_FAILED(pT->m_pOwner->get_Properties(&spProperties));
      if (NULL != spProperties)
      {
        // Note: Do NOT call VariantClear on this since we only copy the pointer
        VARIANT varName;
        VariantInit(&varName);
        varName.vt = VT_BSTR;

        // Get the currently associated item for the specified name, if any
        varName.bstrVal = bstrName;
        VARIANT_BOOL bExists = VARIANT_FALSE;
        spProperties->Exists(&varName, &bExists);
        if (bExists)
        {
          CComVariant varObj;
          spProperties->get_Item(&varName, &varObj);
          if (V_VT(&varObj) != VT_EMPTY &&
            (!V_ISOBJECT(&varObj) || V_UNKNOWN(&varObj) != pT->GetUnknown()))
              return pT->Error(IDS_E_TIMER_EXISTS, IID_IPigEvent);
        }

        // Remove any prior association, if any
        if (pT->m_bstrName.Length())
        {
          // Remove the named object
          varName.bstrVal = pT->m_bstrName;
          RETURN_FAILED(spProperties->Remove(&varName));
          pT->m_bstrName.Empty();
          varName.bstrVal = bstrName;
        }

        // Add the object to our owner's property dictionary
        _variant_t varTimer((IDispatch*)IDispatchPtr(pT->GetUnknown()));
        if (bExists)
          RETURN_FAILED(spProperties->Remove(&varName));
        RETURN_FAILED(spProperties->Add(&varName, &varTimer));
      }
    }

    // Save the event's new name
    pT->m_bstrName = bstrName;  

    // Indicate success
    return S_OK;
  }

  STDMETHODIMP get_Name(BSTR* pbstrName)
  {
    // Get a pointer to the most-derived class
    T* pT = static_cast<T*>(this);

    CLEAROUT(pbstrName, (BSTR)NULL);
    pT->Lock();
    *pbstrName = pT->m_bstrName.Copy();
    pT->Unlock();
    return S_OK;
  }

  STDMETHODIMP put_ExpirationExpression(BSTR bstrExpr)
  {
    // Get a pointer to the most-derived class
    T* pT = static_cast<T*>(this);

    HRESULT hr;
    pT->Lock();
    __try
    {
      pT->m_bstrExpirationExpression = bstrExpr;
      hr = S_OK;
    }
    __except(1)
    {
      hr = E_POINTER;
    }
    pT->Unlock();
    return hr;
  }

  STDMETHODIMP get_ExpirationExpression(BSTR* pbstrExpr)
  {
    // Get a pointer to the most-derived class
    T* pT = static_cast<T*>(this);

    CLEAROUT(pbstrExpr, (BSTR)NULL);
    pT->Lock();
    *pbstrExpr = pT->m_bstrExpirationExpression.Copy();
    pT->Unlock();
    return S_OK;
  }

  STDMETHODIMP get_IsExpired(VARIANT_BOOL* pbExpired)
  {
    // Get the killed flag
    T* pT = static_cast<T*>(this);
    pT->Lock();
    bool bKilled = m_bKilled;
    pT->Unlock();
    CLEAROUT(pbExpired, VARBOOL(bKilled || IsExpired()));
    return S_OK;
  }

  STDMETHODIMP Kill()
  {
    // Get a pointer to the most-derived class
    T* pT = static_cast<T*>(this);

    // Set the killed flag
    pT->Lock();
    m_bKilled = true;

    // Tell the owner to release us (indirectly through the Term method)
    if (m_pOwner)
    {
      // Remove any association
      if (pT->m_bstrName.Length())
      {
        // Get the Properties dictionary
        IDictionaryPtr spProperties;
        RETURN_FAILED(pT->m_pOwner->get_Properties(&spProperties));
        if (NULL != spProperties)
        {
          // Note: Do NOT call VariantClear on this since we only copy the pointer
          VARIANT varName;
          VariantInit(&varName);
          varName.vt = VT_BSTR;

          // Remove the named object
          varName.bstrVal = pT->m_bstrName;
          spProperties->Remove(&varName);
          pT->m_bstrName.Empty();
        }
      }
      Term();
    }
  
    // Indicate success
    pT->Unlock();
    return S_OK;
  }

  STDMETHODIMP put_InterruptionExpression(BSTR bstrExpr)
  {
    // Get a pointer to the most-derived class
    T* pT = static_cast<T*>(this);

    HRESULT hr;
    pT->Lock();
    __try
    {
      pT->m_bstrInterruptionExpression = bstrExpr;
      hr = S_OK;
    }
    __except(1)
    {
      hr = E_POINTER;
    }
    pT->Unlock();
    return hr;
  }

  STDMETHODIMP get_InterruptionExpression(BSTR* pbstrExpr)
  {
    // Get a pointer to the most-derived class
    T* pT = static_cast<T*>(this);

    CLEAROUT(pbstrExpr, (BSTR)NULL);
    pT->Lock();
    *pbstrExpr = pT->m_bstrInterruptionExpression.Copy();
    pT->Unlock();
    return S_OK;
  }

// Types
public:
  typedef IPigEventImpl<T, I, piid> IPigEventImplBase;
};


/////////////////////////////////////////////////////////////////////////////