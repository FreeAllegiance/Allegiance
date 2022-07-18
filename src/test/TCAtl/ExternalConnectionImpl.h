#ifndef __ExternalConnectionImpl_h__
#define __ExternalConnectionImpl_h__

/////////////////////////////////////////////////////////////////////////////
// ExternalConnectionImpl.h | Declaration of the IExternalConnectionImpl
// template class.
//

#include <TCLib.h>
#include <comdef.h>


/////////////////////////////////////////////////////////////////////////////
// Interface Map Entries

#define COM_INTERFACE_ENTRIES_IExternalConnectionImpl()                     \
  COM_INTERFACE_ENTRY_FUNC(IID_IExternalConnection, 0,                      \
    QI_IExternalConnectionImpl)


/////////////////////////////////////////////////////////////////////////////
// IExternalConnectionImpl

template <class T>
class ATL_NO_VTABLE IExternalConnectionImpl : public IExternalConnection
{
// Construction
public:
  IExternalConnectionImpl();

// Overrides
public:
  void DoLastReleaseClosing();

// Diagnostics
protected:
  void DumpConnection(int nDirection, DWORD extconn, DWORD reserved,
    BOOL fLastReleaseCloses = FALSE);

// Implementation
protected:
  static HRESULT WINAPI QI_IExternalConnectionImpl(void* pvThis, REFIID,
    LPVOID* ppv, DWORD);

// IExternalConnection Interface Methods
public:
  STDMETHOD_(DWORD, AddConnection)(DWORD extconn, DWORD reserved);
  STDMETHOD_(DWORD, ReleaseConnection)(DWORD extconn, DWORD reserved,
    BOOL fLastReleaseCloses);

// Data Members
protected:
  DWORD m_dwStrong;
};


/////////////////////////////////////////////////////////////////////////////
// Inline Construction

template <class T>
IExternalConnectionImpl<T>::IExternalConnectionImpl()
  : m_dwStrong(0)
{
}


/////////////////////////////////////////////////////////////////////////////
// Overrides

template <class T>
void IExternalConnectionImpl<T>::DoLastReleaseClosing()
{
  BOOL bNoDisconnect = FALSE;
  #ifdef _DEBUG
    // Format the derived class name string
    const LPCSTR pszClass = TCTypeName(T);

    // Read the registry value
    CRegKey key;
    LPCTSTR pszKey = TEXT("Software\\MultiMedia Access");
    LONG lResult = key.Open(HKEY_LOCAL_MACHINE, pszKey);
    if (ERROR_SUCCESS == lResult)
    {
      DWORD dwValue = 0;
      LPCTSTR pszValue = TEXT("DisableLastReleaseClosing");
      if (ERROR_SUCCESS == (lResult = key.QueryValue(dwValue, pszValue)))
        if (dwValue)
          bNoDisconnect = TRUE;
    }
  #endif // _DEBUG

  // Get the derived class pointer
  T* pThis = static_cast<T*>(this);

  // Get the object's IUnknown pointer (without AddRef'ing it)
  IUnknown* punk = NULL;
  {
    TCObjectLock<T> lock(pThis);  
    punk = pThis->GetUnknown();

    // Report the object reference count upon entering
    #ifdef _DEBUG
      ATLTRACE1("IExternalConnectionImpl<%hs>::", pszClass);
      ATLTRACE0("DoLastReleaseClosing(): Entering, ");
      ATLTRACE1("m_dwRef = %d\n", pThis->m_dwRef);
    #endif // _DEBUG
  }

  // Disconnect all external connections to the object
  if (bNoDisconnect)
  {
    #ifdef _DEBUG
      ATLTRACE1("\tIExternalConnectionImpl<%hs>::", pszClass);
      ATLTRACE0("DoLastReleaseClosing(): Skipping CoDisconnectObject ");
      ATLTRACE0("due to registry value\n");
    #endif // _DEBUG
  }
  else
  {
    #ifdef _DEBUG
      ATLTRACE1("\tIExternalConnectionImpl<%hs>::", pszClass);
      ATLTRACE0("DoLastReleaseClosing(): Prior to CoDisconnectObject\n");
    #endif // _DEBUG

    // Disconnect the object
    HRESULT hr = CoDisconnectObject(punk, 0);

    #ifdef _DEBUG
      ATLTRACE1("\tIExternalConnectionImpl<%hs>::", pszClass);
      ATLTRACE0("DoLastReleaseClosing(): CoDisconnectObject ");
      ATLTRACE1("returned 0x%08X\n", hr);
    #endif // _DEBUG
  }

  // Report the object reference count upon exiting
  #ifdef _DEBUG
  {
    TCObjectLock<T> lock(pThis);  
    ATLTRACE1("IExternalConnectionImpl<%hs>::", TCTypeName(T));
    ATLTRACE0("DoLastReleaseClosing(): Exiting, ");
    ATLTRACE1("m_dwRef = %d\n", pThis->m_dwRef);
  }
  #endif
}


/////////////////////////////////////////////////////////////////////////////
// Diagnostics

template <class T>
void IExternalConnectionImpl<T>::DumpConnection(int nDirection,
  DWORD extconn, DWORD reserved, BOOL fLastReleaseCloses)
{
  #ifdef _DEBUG
  {
    // Lock the object to get the connection count
    TCObjectLock<T> lock(static_cast<T*>(this));
    int nStrong = (int)m_dwStrong;

    // Determine if the extconn parameter specifies a strong connection
    bool bStrong = (extconn & EXTCONN_STRONG);

    // Format the derived class name string
    const LPCSTR pszClass = TCTypeName(T);

    // Format the method name string
    ZAssert(1 == nDirection || -1 == nDirection);
    const LPCSTR pszMethod =
      (nDirection > 0) ? "AddConnection" : "ReleaseConnection";

    // Dump the method name
    ATLTRACE2("IExternalConnectionImpl<%hs>::%hs(): ", pszClass, pszMethod);

    // Dump the strong connection count
    ATLTRACE1("m_dwStrong= %d", nStrong + (bStrong ? nDirection : 0));

    // Dump the method parameters
    if (nDirection < 0)
      ATLTRACE1(" fLastReleaseCloses= %hs",
        fLastReleaseCloses ? "TRUE" : "FALSE");
    if (!bStrong)
      ATLTRACE1(" extconn= 0x%08X (!=EXTCONN_STRONG)", extconn);

    // Finish with a line-end character
    ATLTRACE0("\n");
  }
  #endif // _DEBUG
}


/////////////////////////////////////////////////////////////////////////////
// Implementation

template <class T>
HRESULT WINAPI IExternalConnectionImpl<T>::QI_IExternalConnectionImpl(
  void* pvThis, REFIID, LPVOID* ppv, DWORD)
{
  static DWORD dwDisableIExternalConnection = -1;
  if (-1 == dwDisableIExternalConnection)
  {
    // Initialize the value
    dwDisableIExternalConnection = 0;

    #ifdef _DEBUG
      // Read the registry value
      CRegKey key;
      const LPCTSTR pszKey = TEXT("Software\\MultiMedia Access");
      LONG lResult = key.Open(HKEY_LOCAL_MACHINE, pszKey);
      if (ERROR_SUCCESS == lResult)
      {
        const LPCTSTR pszValue = TEXT("DisableIExternalConnection");
        lResult = key.QueryValue(dwDisableIExternalConnection, pszValue);
      }
    #endif // _DEBUG
  }

  // Clear the specified [out] parameter
  CLEAROUT(ppv, (void*)NULL);

  // Do not support the interface if disabled in the registry
  if (dwDisableIExternalConnection)
    return E_NOINTERFACE;

  // Cast to the IExternalConnection interface
  T* pThis = static_cast<T*>(pvThis);
  IExternalConnection* pec = static_cast<IExternalConnection*>(pThis);
  if (!pec)
    return E_NOINTERFACE;

  // Copy the interface pointer to the [out] parameter
  CLEAROUT(ppv, (void*)pec);
  pec->AddRef();

  // Indicate success
  return S_OK;
}


/////////////////////////////////////////////////////////////////////////////
// IExternalConnection Interface Methods

template <class T>
STDMETHODIMP_(DWORD) IExternalConnectionImpl<T>::AddConnection(DWORD extconn,
   DWORD reserved)
{
  TCObjectLock<T> lock(static_cast<T*>(this));
  DumpConnection(1, extconn, reserved);

  return (extconn & EXTCONN_STRONG) ? ++m_dwStrong : 0;
}

template <class T>
STDMETHODIMP_(DWORD) IExternalConnectionImpl<T>::ReleaseConnection(
  DWORD extconn, DWORD reserved, BOOL fLastReleaseCloses)
{
  TCObjectLock<T> lock(static_cast<T*>(this));
  DumpConnection(-1, extconn, reserved, fLastReleaseCloses);

  if (extconn & EXTCONN_STRONG)
  {
    DWORD nStrong = --m_dwStrong;
    lock.Unlock();
    if (0 == nStrong && fLastReleaseCloses)
      static_cast<T*>(this)->DoLastReleaseClosing();
    return nStrong;
  }
  else
    return 0;
}


/////////////////////////////////////////////////////////////////////////////

#endif // !__ExternalConnectionImpl_h__
