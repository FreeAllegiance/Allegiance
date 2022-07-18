#pragma once

#ifndef __SimpleStream_h__
#define __SimpleStream_h__

/////////////////////////////////////////////////////////////////////////////
// SimpleStream.h : Declaration of the CTCSimpleStreamImpl class.


/////////////////////////////////////////////////////////////////////////////
// Description: A class that implements an object that supports the IStream
// interface and, therefore, also ISequentialStream. The implementation is
// very simple in that the stream is a fixed size (non-growable), and is
// implemented on a specified pointer.
//
// This object provides an implementation of the IStream interface that is
// imlemented on a fixed, non-growable, memory block.
//
// Note: This implementation does *not* provide any thread synchronization.
// It also does not keep track of any of the time fields. The Stat method
// will always return the current time for all of the time fields.
//
class ATL_NO_VTABLE CTCSimpleStreamImpl :
  public IStream,
  public CComObjectRoot
{
/////////////////////////////////////////////////////////////////////////////
// Interface Map
public:
  BEGIN_COM_MAP(CTCSimpleStreamImpl)
    COM_INTERFACE_ENTRY(IStream)
    COM_INTERFACE_ENTRY(ISequentialStream)
  END_COM_MAP()

/////////////////////////////////////////////////////////////////////////////
// Construction
public:
  ///////////////////////////////////////////////////////////////////////////
  CTCSimpleStreamImpl() :
    m_pbBegin(NULL),
    m_pbPosition(NULL),
    m_pbEnd(NULL)
  {
  }

  ///////////////////////////////////////////////////////////////////////////
  void Init(ULONG cbData, void* pvData)
  {
    ZAssert(!m_pbBegin && !m_pbPosition && !m_pbEnd);
    ZAssert(cbData);
    ZAssert(pvData);
    ZAssert(!IsBadReadPtr(pvData, cbData));
    m_pbBegin = m_pbPosition = reinterpret_cast<BYTE*>(pvData);
    m_pbEnd   = m_pbBegin + cbData;
  }

/////////////////////////////////////////////////////////////////////////////
// ISequentialStream Interface Methods
public:
  ///////////////////////////////////////////////////////////////////////////
  STDMETHODIMP Read(void* pv, ULONG cb, ULONG* pcbRead)
  {
    ZAssert(m_pbBegin && m_pbPosition && m_pbEnd);
    ZAssert(!IsBadWritePtr(pv, cb));
    ZAssert(!pcbRead || !IsBadWritePtr(pcbRead, sizeof(*pcbRead)));

    // Compute the number of bytes that can be read
    ULONG nMax = m_pbEnd - m_pbPosition;
    nMax = std::min(nMax, cb);

    // Copy the maximum number of bytes to the specified buffer
    ZAssert(!IsBadReadPtr(m_pbPosition, nMax));
    CopyMemory(pv, m_pbPosition, nMax);

    // Update the current position
    m_pbPosition += nMax;

    // Return the number of bytes read, if requested
    if (pcbRead)
      *pcbRead = nMax;

    // Indicate success
    return S_OK;
  }

  ///////////////////////////////////////////////////////////////////////////
  STDMETHODIMP Write(const void* pv, ULONG cb, ULONG* pcbWritten)
  {
    ZAssert(m_pbBegin && m_pbPosition && m_pbEnd);
    ZAssert(!IsBadReadPtr(pv, cb));
    ZAssert(!pcbWritten || !IsBadWritePtr(pcbWritten, sizeof(*pcbWritten)));

    // Compute the number of bytes that can be written
    ULONG nMax = m_pbEnd - m_pbPosition;
    nMax = std::min(nMax, cb);

    // Copy the maximum number of bytes from the specified buffer
    ZAssert(!IsBadWritePtr(m_pbPosition, nMax));
    CopyMemory(m_pbPosition, pv, nMax);

    // Update the current position
    m_pbPosition += nMax;

    // Return the number of bytes written, if requested
    if (pcbWritten)
      *pcbWritten = nMax;

    // Indicate success
    return S_OK;
  }


/////////////////////////////////////////////////////////////////////////////
// IStream Interface Methods
public:
  ///////////////////////////////////////////////////////////////////////////
  STDMETHODIMP Seek(LARGE_INTEGER dlibMove, DWORD dwOrigin,
    ULARGE_INTEGER* plibNewPosition)
  {
    ZAssert(m_pbBegin && m_pbPosition && m_pbEnd);
    ZAssert(!plibNewPosition || !IsBadWritePtr(plibNewPosition,
      sizeof(*plibNewPosition)));

    // Clear the specified [out] parameter, if not NULL
    if (plibNewPosition)
      TCZeroMemory(plibNewPosition);

    switch (dwOrigin)
    {
      case STREAM_SEEK_SET:
      {
        // Validate the position pointer as unsigned and in 32-bit range
        if (0 != dlibMove.HighPart)
          return STG_E_INVALIDFUNCTION;

        // Compute the new position
        BYTE* pbNewPosition = m_pbBegin + dlibMove.LowPart;
        if (m_pbBegin > pbNewPosition || pbNewPosition > m_pbEnd)
          return STG_E_INVALIDFUNCTION;
        m_pbPosition = pbNewPosition;
        break;
      }

      case STREAM_SEEK_CUR:
      {
        // Validate the position pointer as signed and in 32-bit range
        if (0 != dlibMove.HighPart && -1 != dlibMove.HighPart)
          return STG_E_INVALIDFUNCTION;

        // Compute the new position
        BYTE* pbNewPosition = m_pbPosition + LONG(dlibMove.LowPart);
        if (m_pbBegin > pbNewPosition || pbNewPosition > m_pbEnd)
          return STG_E_INVALIDFUNCTION;
        m_pbPosition = pbNewPosition;
        break;
      }

      case STREAM_SEEK_END:
      {
        // Validate the position pointer as signed and in 32-bit range
        if (0 != dlibMove.HighPart && -1 != dlibMove.HighPart)
          return STG_E_INVALIDFUNCTION;

        // Compute the new position
        BYTE* pbNewPosition = m_pbEnd + LONG(dlibMove.LowPart);
        if (m_pbBegin > pbNewPosition || pbNewPosition > m_pbEnd)
          return STG_E_INVALIDFUNCTION;
        m_pbPosition = pbNewPosition;
        break;
      }

      default:
        return STG_E_INVALIDFUNCTION;
    }

    // Return the new position, if requested
    if (plibNewPosition)
      plibNewPosition->LowPart = m_pbPosition - m_pbBegin;

    // Indicate success
    return S_OK;
  }

  ///////////////////////////////////////////////////////////////////////////
  STDMETHODIMP SetSize(ULARGE_INTEGER libNewSize)
  {
    ZAssert(m_pbBegin && m_pbPosition && m_pbEnd);

    // Validate the specified size as in 32-bit range
    if (0 != libNewSize.HighPart)
      return STG_E_MEDIUMFULL;

    // This stream can only shrink, never grow
    if ((BYTE)libNewSize.LowPart > (m_pbEnd - m_pbBegin))
      return STG_E_MEDIUMFULL;

    // Set the new size of the stream
    m_pbEnd = m_pbBegin + libNewSize.LowPart;
    m_pbPosition = std::min(m_pbPosition, m_pbEnd);

    // Indicate success
    return S_OK;
  }

  ///////////////////////////////////////////////////////////////////////////
  STDMETHODIMP CopyTo(IStream* pstm, ULARGE_INTEGER cb,
    ULARGE_INTEGER* pcbRead, ULARGE_INTEGER* pcbWritten)
  {
    ZAssert(m_pbBegin && m_pbPosition && m_pbEnd);

    // Initialize the [out] parameters
    if (pcbRead)
      TCZeroMemory(pcbRead);
    if (pcbWritten)
      TCZeroMemory(pcbWritten);

    // Ensure that the specified stream pointer is valid
    IStreamPtr spstm;
    RETURN_FAILED(TCSafeQI(pstm, &spstm));

    // Compute the number of bytes that can be copied
    ULONG nMax = m_pbEnd - m_pbPosition;
    nMax = std::min(nMax, cb.LowPart);

    // Copy from the current seek pointer, the maximum number of bytes
    ULONG cbWritten;
    RETURN_FAILED(spstm->Write(m_pbPosition, nMax, &cbWritten));

    // Update the current position
    m_pbPosition += cbWritten;

    // Return the number of bytes read, if requested
    if (pcbRead)
      pcbRead->LowPart = cbWritten;

    // Return the number of bytes written, if requested
    if (pcbWritten)
      pcbWritten->LowPart = cbWritten;

    // Indicate success
    return S_OK;
  }

  ///////////////////////////////////////////////////////////////////////////
  STDMETHODIMP Commit(DWORD grfCommitFlags)
  {
    ZAssert(m_pbBegin && m_pbPosition && m_pbEnd);
    UNUSED(grfCommitFlags);
    return S_OK;
  }

  ///////////////////////////////////////////////////////////////////////////
  STDMETHODIMP Revert(void)
  {
    ZAssert(m_pbBegin && m_pbPosition && m_pbEnd);
    return S_OK;
  }

  ///////////////////////////////////////////////////////////////////////////
  STDMETHODIMP LockRegion(ULARGE_INTEGER libOffset,
    ULARGE_INTEGER cb, DWORD dwLockType)
  {
    ZAssert(m_pbBegin && m_pbPosition && m_pbEnd);
    UNUSED(libOffset);
    UNUSED(cb);
    UNUSED(dwLockType);
    return STG_E_INVALIDFUNCTION;
  }

  ///////////////////////////////////////////////////////////////////////////
  STDMETHODIMP UnlockRegion(ULARGE_INTEGER libOffset,
    ULARGE_INTEGER cb, DWORD dwLockType)
  {
    ZAssert(m_pbBegin && m_pbPosition && m_pbEnd);
    UNUSED(libOffset);
    UNUSED(cb);
    UNUSED(dwLockType);
    return STG_E_INVALIDFUNCTION;
  }

  ///////////////////////////////////////////////////////////////////////////
  STDMETHODIMP Stat(STATSTG* pstatstg, DWORD grfStatFlag)
  {
    ZAssert(m_pbBegin && m_pbPosition && m_pbEnd);
    UNUSED(grfStatFlag);

    // Initialize the specified structures
    TCZeroMemory(pstatstg);

    // Uset the current time for all time fields
    FILETIME ft;
    GetSystemTimeAsFileTime(&ft);

    // Populate the fields of the specified structure
    pstatstg->pwcsName          = NULL;
    pstatstg->type              = STGTY_STREAM; 
    pstatstg->cbSize.HighPart   = 0;
    pstatstg->cbSize.LowPart    = m_pbEnd - m_pbBegin;
    pstatstg->mtime             = ft;
    pstatstg->ctime             = ft;
    pstatstg->atime             = ft;
    pstatstg->grfMode           = STGM_READWRITE | STGM_SHARE_DENY_NONE;
    pstatstg->grfLocksSupported = 0;
    pstatstg->clsid             = CLSID_NULL;

    // Indicate success
    return S_OK;
  }

  ///////////////////////////////////////////////////////////////////////////
  STDMETHODIMP Clone(IStream** ppstm)
  {
    ZAssert(m_pbBegin && m_pbPosition && m_pbEnd);

    // Initialize the [out] parameter
    CLEAROUT(ppstm, (IStream*)NULL);

    // Do not support this function
    return STG_E_INSUFFICIENTMEMORY;
  }

/////////////////////////////////////////////////////////////////////////////
// Data Members
protected:
  BYTE* m_pbBegin;
  BYTE* m_pbEnd;
  BYTE* m_pbPosition;
};


/////////////////////////////////////////////////////////////////////////////
// CTCSimpleStream

typedef CComObjectGlobal<CTCSimpleStreamImpl> TCSimpleStream;


/////////////////////////////////////////////////////////////////////////////

#endif // !__SimpleStream_h__

