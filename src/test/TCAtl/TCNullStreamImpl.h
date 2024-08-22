#pragma once

/////////////////////////////////////////////////////////////////////////////
// CTCNullStreamImpl.h | Declaration of the CTCNullStreamImpl class, which
// implements the CLSID_CTCNullStreamImpl component object.

#include <..\TCLib\ObjectLock.h>


/////////////////////////////////////////////////////////////////////////////
// Description: A class that implements an object that supports the IStream
// and, therefore, also ISequentialStream, to do nothing but keep track of
// the number of bytes having been written to it.
//
// This object provides an implementation of the IStream interface that does
// nothing more that count the number of bytes being written to it.  Since
// ATL (currently 3.0) implements IPersistStreamInitImpl::GetSizeMax by
// returning E_NOTIMPL, this object can be used in an override of that
// method. This is accomplished by creating an instance of this null stream
// and 'persisting' an object to it, thereby counting the number of bytes
// needed to persist the object. This object object performs this without
// allocating any memory to back up the stream.
//
// Care should be taken to ensure that the object that thinks it is being
// persisted does not change state from the time the byte count is taken
// until the when the object is written to a real stream.
//
// See Also: CTCNullStreamImpl, CTCNullStream
//
class ATL_NO_VTABLE CTCNullStreamImpl :
  public IStream,
  public CComObjectRoot
{
// Interface Map
public:
  BEGIN_COM_MAP(CTCNullStreamImpl)
    COM_INTERFACE_ENTRY(IStream)
    COM_INTERFACE_ENTRY(ISequentialStream)
  END_COM_MAP()

// Construction
public:
  CTCNullStreamImpl();

// ISequentialStream Interface Methods
public:
  STDMETHODIMP Read(void* pv, ULONG cb, ULONG* pcbRead);
  STDMETHODIMP Write(const void* pv, ULONG cb, ULONG* pcbWritten);

// IStream Interface Methods
public:
  STDMETHODIMP Seek(LARGE_INTEGER dlibMove, DWORD dwOrigin,
    ULARGE_INTEGER* plibNewPosition);
  STDMETHODIMP SetSize(ULARGE_INTEGER libNewSize);
  STDMETHODIMP CopyTo(IStream* pstm, ULARGE_INTEGER cb,
    ULARGE_INTEGER* pcbRead, ULARGE_INTEGER* pcbWritten);
  STDMETHODIMP Commit(DWORD grfCommitFlags);
  STDMETHODIMP Revert(void);
  STDMETHODIMP LockRegion(ULARGE_INTEGER libOffset, ULARGE_INTEGER cb,
    DWORD dwLockType);
  STDMETHODIMP UnlockRegion(ULARGE_INTEGER libOffset, ULARGE_INTEGER cb,
    DWORD dwLockType);
  STDMETHODIMP Stat(STATSTG* pstatstg, DWORD grfStatFlag);
  STDMETHODIMP Clone(IStream** ppstm);

// Types
protected:
  typedef TCObjectLock<CTCNullStreamImpl> CLock;

// Data Members
protected:
  ULONG    m_nPosition;
  ULONG    m_nSize;
  FILETIME m_ftCreated, m_ftModified, m_ftAccessed;
};


/////////////////////////////////////////////////////////////////////////////
// CTCNullStreamImpl

typedef CComObjectGlobal<CTCNullStreamImpl> TCNullStreamImpl;


/////////////////////////////////////////////////////////////////////////////