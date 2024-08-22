#pragma once

/////////////////////////////////////////////////////////////////////////////
// AdviseHolder.h | Declaration of the TCAdviseHolder class.
//

#pragma warning(disable: 4786)

#if !defined(_INC_COMDEF)
  #include <comdef.h>
#endif // !defined(_INC_COMDEF)

#ifndef __RefCountedData_h__
  #include "RefCountedData.h"
#endif // !__RefCountedData_h__


/////////////////////////////////////////////////////////////////////////////
// TCAdviseHolder provides an encapsulation of the semantics used to connect
// to and disconnect from a connectable component object. An object is
// connectable in this sense if it supports the IConnectionPointContainer
// interface, first introduced by Microsoft for use with ActiveX controls.
//
// The class simplifies the connecting to an object by providing the Advise
// method, which takes three parameters and performs the necessary sequence
// of interface method calls. In this process, the class saves the /cookie/
// returned from the connectable object, which identifies the connection when
// Unadvise is called to disconnect from the object.
//
// While the connection process can happen from the single method call, it
// can also be split into a two step process using the FindConnectionPoint
// method, followed by a single-parameter form of the Advise method. This
// allows for a more advanced usage of the class.
//
// The class is implemented using a reference counting scheme to avoid
// unnecessary connections and disconnections when passing the object by
// value. This makes it suitable for use within an STL container object, such
// as std::map or std::vector. Actually, that was the primary motivation of
// this class.
//
// When the Unadvise method is called, which happens automatically from the
// destructor, m_pData (a pointer to an XDataType instance) is released. Upon
// releasing the last reference, the XDataType instance will delete itself,
// which actually disconnects from the connectable object.
//
// See Also: TCAdviseHolder::XData, TCRefCountedData
class TCAdviseHolder
{
// Construction / Destruction
public:
  TCAdviseHolder();
  TCAdviseHolder(const TCAdviseHolder& that);
  virtual ~TCAdviseHolder();

// Attributes
public:
  IConnectionPointContainer* GetCPC();
  IConnectionPoint* GetCP();

// Operations
public:
  HRESULT FindConnectionPoint(IUnknown* punkCP, REFIID iid);
  HRESULT Advise(IUnknown* punk);
  HRESULT Advise(IUnknown* punkCP, IUnknown* punk, REFIID iid);
  HRESULT Unadvise();

// Operators
public:
  const TCAdviseHolder& operator=(const TCAdviseHolder& that);

// Group=Types
protected:
  ///////////////////////////////////////////////////////////////////////////
  // Description: Nested class.
  // Remarks: XData is a nested class of TCAdviseHolder which represents the
  // reference counted data used by that class. There may be multiple
  // instances of TCAdviseHolder that each point to the same instance of this
  // class. Actually, XData is the base class of the XDataType, which adds
  // the reference counting implementation to XData.
  class XData
  {
  // Construction / Destruction
  public:
    XData();
    virtual ~XData();

  // Operations
  public:
    HRESULT Unadvise();

  // Data Members:
  public:
    // Smart pointer that maintains the IConnectionPointContainer interface.
    IConnectionPointContainerPtr m_pCPC;
    // Smart pointer that maintains the IConnectionPoint interface.
    IConnectionPointPtr m_pCP;
    // Cookie provided by the connectable object to identify this connection.
    DWORD m_dwAdvise;
  };
  ///////////////////////////////////////////////////////////////////////////
  // Description: Reference counted derivation of XData.
  // See Also: TCRefCountedData, TCAdviseHolder::XData
  typedef TCRefCountedData<XData> XDataType;

// Group=Data Members
protected:
  XDataType* m_pData;  // Pointer to the reference counted data.
};


/////////////////////////////////////////////////////////////////////////////
// Group=Construction / Destruction

/////////////////////////////////////////////////////////////////////////////
inline TCAdviseHolder::TCAdviseHolder()
  : m_pData(NULL)
{
}

/////////////////////////////////////////////////////////////////////////////
// Parameters:
//   that - Another instance to be copied into this one. The m_pData member
//   is copied and TCRefCountedData::AddRef is called on the pointer.
//
// See Also: TCAdviseHolder::operator=, TCRefCountedData::AddRef
inline TCAdviseHolder::TCAdviseHolder(const TCAdviseHolder& that)
  : m_pData(NULL)
{
  operator=(that);
}

/////////////////////////////////////////////////////////////////////////////
// Remarks: The m_pData member is release by calling
// TCRefCountedData::Release.
//
// See Also: TCRefCountedData::Release
inline TCAdviseHolder::~TCAdviseHolder()
{
  m_pData->Release();
}


/////////////////////////////////////////////////////////////////////////////
// Group=Attributes

/////////////////////////////////////////////////////////////////////////////
// Gets the IConnectionPointContainer interface pointer of the current
// connection, if any.
//
// Return Value: The IConnectionPointContainer interface pointer of the
// current connection, if any.
//
// See Also: TCAdviseHolder::GetCP, TCAdviseHolder::Advise,
// TCAdviseHolder::FindConnectionPoint
inline IConnectionPointContainer* TCAdviseHolder::GetCPC()
{
  return m_pData ? m_pData->m_pCPC : NULL;
}

/////////////////////////////////////////////////////////////////////////////
// Gets the IConnectionPoint interface pointer of the current connection, if
// any.
//
// Return Value: The IConnectionPoint interface pointer of the current
// connection, if any.
//
// See Also: TCAdviseHolder::GetCPC, TCAdviseHolder::Advise
inline IConnectionPoint* TCAdviseHolder::GetCP()
{
  return m_pData ? m_pData->m_pCP : NULL;
}


/////////////////////////////////////////////////////////////////////////////
// Group=Operations


/////////////////////////////////////////////////////////////////////////////
// Description: Finds the specified outgoing interface in an object.
//
// Parameters:
//   punkCP - An interface pointer of an object for which the outgoing
//   interface, /iid/, is to be found.
//   iid - The outgoing interface GUID which represents the connection point
//   to be found.
//
// Remarks: This method is used internally by the three-parameter Advise
// method. It is made public to enable a more advanced usage of the class. Do
// not use this method directly unless you have a need to find a connection
// point seperately from setting up the advisory connection.
//
// This method first queries /punkCP/ for the IConnectionPointContainer
// interface. If the object supports this interface, then the
// *IConnectionPointContainer::FindConnectionPoint* method is called to get
// the connection point for the interface specified by /iid/. The interface
// pointers for both the connection point and its container are stored.
// Follow this method call by calling the single-parameter Advise
// method to actually complete the advisory connection to the object.
//
// Return Value: This method supports the standard return values
// E_OUTOFMEMORY and E_UNEXPECTED, as well as the following:
//
//   S_OK - The object supports the specified connection point interface.
//   E_NOINTERFACE - The object does not support connection points.
//   CONNECT_E_NOCONNECTION - The object does not support the specified
//   outgoing connection point interface. 
//
// See Also: TCAdviseHolder::GetCP, TCAdviseHolder::GetCPC,
// TCAdviseHolder::Advise
inline
HRESULT TCAdviseHolder::FindConnectionPoint(IUnknown* punkCP, REFIID iid)
{
  // Allocate a new reference counted data object
  HRESULT hr = S_OK;
  XDataType* pData = NULL;
  __try
  {
    // Release any previous reference counted data object
    m_pData->Release();
    m_pData = NULL;

    // Allocate a new reference counted data object
    if (!(pData = new XDataType))
      return E_OUTOFMEMORY;

    // Query for the specified object's IConnectionPointContainer
    pData->m_pCPC = punkCP;
    if (NULL != pData->m_pCPC)
    {
      // Get the object's IID_IPropertyNotifySink connection point
      hr = pData->m_pCPC->FindConnectionPoint(iid, &pData->m_pCP);
      if (SUCCEEDED(hr))
      {
        // Save the new reference counted data object
        m_pData = pData;

        // Indicate success
        return S_OK;
      }
    }
    else
    {
      hr = E_NOINTERFACE;
    }
  }
  __except(1)
  {
    hr = E_POINTER;
  }

  // Release the shared data object
  pData->Release();

  // Return the last HRESULT
  return hr;
}

/////////////////////////////////////////////////////////////////////////////
// Description: Sets up an advisory connection to an outgoing interface of a
// connectable object.
//
// Parameters:
//   punk - An interface pointer that is to receive the advisory
//   notifications (events) from the source object. This interface must
//   support the outgoing interface through QueryInterface.
//   punkCP - An interface pointer of an object for which the outgoing
//   interface, /iid/, is to be found.
//   iid - The outgoing interface GUID which represents the connection point
//   to be found.
//
// Remarks: The three-parameter form of this method is the only method that
// needs to be called on a new instance of this class. This method will
// perform all the steps necessary to set up an advisory connection to an
// outgoing interface of a connectable object. This method is the recommended
// way of using this class for most cases.
//
// The single-parameter form of this method must be used in conjunction with
// the FindConnectionPoint method and is provided for a more advanced usage
// of the class.
//
// Return Value: This method supports the standard return values
// E_OUTOFMEMORY and E_UNEXPECTED, as well as the following: 
//
//   S_OK - The object supports the specified connection point interface.
//   E_NOINTERFACE - The object does not support connection points.
//   CONNECT_E_NOCONNECTION - The object does not support the specified
//   outgoing connection point interface. 
//
// See Also: TCAdviseHolder::GetCP, TCAdviseHolder::GetCPC,
// TCAdviseHolder::FindConnectionPoint, TCAdviseHolder::XData::m_pCPC,
// TCAdviseHolder::XData::m_pCP
inline HRESULT TCAdviseHolder::Advise(IUnknown* punk)
{
  // Ensure that FindConnectionPoint has been called previously
  if (!m_pData || NULL == m_pData->m_pCPC || NULL == m_pData->m_pCP ||
    m_pData->m_dwAdvise)
  {
    _ASSERT(false);
    return E_UNEXPECTED;
  }

  // Set the advise
  HRESULT hr = m_pData->m_pCP->Advise(punk, &m_pData->m_dwAdvise);
  if (FAILED(hr))
  {
    _TRACE1("TCAdviseHolder::Advise(): CP->Advise returneed 0x%08X\n", hr);
    Unadvise();
  }

  // Return the last HRESULT
  return hr;
}

inline
HRESULT TCAdviseHolder::Advise(IUnknown* punkCP, IUnknown* punk, REFIID iid)
{
  HRESULT hr = FindConnectionPoint(punkCP, iid);
  if (SUCCEEDED(hr))
    hr = Advise(punk);
  return hr;
}

/////////////////////////////////////////////////////////////////////////////
// Description: Terminates the advisory connection.
//
// Return Value: This method supports the standard return value E_UNEXPECTED,
// as well as the following:
//
//   S_OK - The connection was successfully terminated.
//   CONNECT_E_NOCONNECTION - No advisory connection exists.
//
// Remarks: Terminates the advisory connection previously established through
// Advise.
//
// See Also: TCAdviseHolder::Advise, TCAdviseHolder::XData::Unadvise
inline HRESULT TCAdviseHolder::Unadvise()
{
  return m_pData ? m_pData->Unadvise() : CONNECT_E_NOCONNECTION;
}


/////////////////////////////////////////////////////////////////////////////
// Group=Operators

/////////////////////////////////////////////////////////////////////////////
// Parameters:
//   that - Another instance to be copied into this one. The m_pData member
//   is copied and TCRefCountedData::AddRef is called on the pointer.
//
// TODO: This method /should/ call TCRefCountedData::Release on the current
// m_pData, if not NULL, prior to assigning it the new pointer value.
//
// See Also: TCAdviseHolder::constructor, TCRefCountedData::AddRef
inline
const TCAdviseHolder& TCAdviseHolder::operator=(const TCAdviseHolder& that)
{
  m_pData = that.m_pData;
  m_pData->AddRef();
  return *this;
}


/////////////////////////////////////////////////////////////////////////////
// TCAdviseHolder::XData


/////////////////////////////////////////////////////////////////////////////
// Group=Construction / Destruction

inline TCAdviseHolder::XData::XData()
  : m_dwAdvise(0)
{
}

inline TCAdviseHolder::XData::~XData()
{
  Unadvise();
}


/////////////////////////////////////////////////////////////////////////////
// Group=Operations

/////////////////////////////////////////////////////////////////////////////
// Description: Terminates the advisory connection.
//
// Return Value: This method supports the standard return value E_UNEXPECTED,
// as well as the following:
//
//   S_OK - The connection was successfully terminated.
//   CONNECT_E_NOCONNECTION - No advisory connection exists.
//
// Remarks: Terminates the advisory connection previously established through
// Advise.
//
// See Also: TCAdviseHolder::m_pCPC, TCAdviseHolder::m_pCP,
// TCAdviseHolder::Unadvise
inline HRESULT TCAdviseHolder::XData::Unadvise()
{
  // Release any IConnectionPointContainer
  if (m_pCPC)
    m_pCPC = NULL;

  // Disconnect the advisory connection, if any
  HRESULT hr = CONNECT_E_NOCONNECTION;
  if (m_dwAdvise)
  {
    hr = m_pCP->Unadvise(m_dwAdvise);
    m_dwAdvise = 0;
  }

  // Release any IConnectionPoint
  if (m_pCP)
    m_pCP = NULL;

  // Return the last HRESULT
  return hr;
}


/////////////////////////////////////////////////////////////////////////////