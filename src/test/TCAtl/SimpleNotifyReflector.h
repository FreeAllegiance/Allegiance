#pragma once

/////////////////////////////////////////////////////////////////////////////
// SimpleNotifyReflector.h | Declaration of the TCSimpleNotifyReflector
// class.
//


/////////////////////////////////////////////////////////////////////////////
// TCSimpleNotifyReflector_Base

class ATL_NO_VTABLE TCSimpleNotifyReflector_Base :
  public IPropertyNotifySink
{
// Construction
public:
  TCSimpleNotifyReflector_Base(DISPID dispid) :
    m_dispid(dispid)
  {
  }

// IUnknown Interface Methods
public:
  STDMETHODIMP QueryInterface(REFIID riid, void** ppvObject)
  {
    CLEAROUT(ppvObject, (void*)NULL);
    if (IID_IUnknown == riid || IID_IPropertyNotifySink == riid)
    {
      *ppvObject = (IPropertyNotifySink*)this;
      return S_OK;
    }
    return E_NOINTERFACE;
  }
  STDMETHODIMP_(ULONG) AddRef()
  {
    return 1;
  }
  STDMETHODIMP_(ULONG) Release()
  {
    return 1;
  }

// IPropertyNotifySink Interface Methods
public:
  STDMETHODIMP OnChanged(DISPID dispID) = 0;
  STDMETHODIMP OnRequestEdit(DISPID dispID) = 0;

// Data Members
protected:
  DISPID m_dispid;
};


/////////////////////////////////////////////////////////////////////////////
// TCSimpleNotifyReflector

template <class T>
class TCSimpleNotifyReflector :
  public TCSimpleNotifyReflector_Base
{
// Construction
public:
  TCSimpleNotifyReflector(T* pThis, DISPID dispid) :
    TCSimpleNotifyReflector_Base(dispid),
    m_pThis(pThis)
  {
  }

// IPropertyNotifySink Interface Methods
public:
  STDMETHODIMP OnChanged(DISPID)
  {
    return m_pThis->FireOnChanged(m_dispid);
  }
  STDMETHODIMP OnRequestEdit(DISPID)
  {
    return m_pThis->FireOnRequestEdit(m_dispid);
  }

// Types
public:
  typedef TCSimpleNotifyReflector<T> TCSimpleNotifyReflectorBase;

// Data Members
protected:
  T* m_pThis;
};


/////////////////////////////////////////////////////////////////////////////