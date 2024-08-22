#pragma once

/////////////////////////////////////////////////////////////////////////////
#ifndef RETURN_FAILED
  ///////////////////////////////////////////////////////////////////////////
  // Evaluates an HRESULT expression and returns it if it indicates failure.
  // This should only be used from a function that has an HRESULT (or
  // compatible) return type.
  //
  // Parameters:
  //   exp - The HRESULT expression to be evaluated.
  //
  // See Also: RETURN_FAILED_VOID, ThrowError, ThrowErrorFAILED
  //
  #define RETURN_FAILED(exp)  \
  do                          \
  {                           \
    HRESULT _hr = exp;        \
    if (FAILED(_hr))          \
      return _hr;             \
  } while (false)
#endif // !RETURN_FAILED


//////////////////////////////////////////////////////////////////////////////
// 
// xhost
//
//////////////////////////////////////////////////////////////////////////////
struct AdminSessionSecureHost : public IAdminSessionHost
{
    // IUnknown Interface Methods
    STDMETHODIMP QueryInterface(REFIID riid, void** ppUnk)
    {
      if (IID_IUnknown == riid || __uuidof(IAdminSessionHost) == riid)
      {
        *ppUnk = this;
        return S_OK;
      }
      return E_NOINTERFACE;
    }

    STDMETHODIMP_(ULONG) AddRef(void)
    {
      return 1;
    }

    STDMETHODIMP_(ULONG) Release(void)
    {
      return 1;
    }

    // IAdminSessionHost Interface Methods
    STDMETHODIMP GetIdentity(long nCookie, IUnknown** ppHost)
    {
      // Ignore the cookie - it's only there in an attempt to foil hackers
      nCookie;

      // Create an AGCVersionInfo object
      IAGCVersionInfoPtr spVersion;
      RETURN_FAILED(spVersion.CreateInstance(__uuidof(AGCVersionInfo)));

      // Load the version info from this module
      RETURN_FAILED(spVersion->put_FileName(NULL));

      // Persist the version info object to a memory stream
      IPersistStreamPtr spPersist(spVersion);
      if (NULL == spPersist)
        return E_NOINTERFACE;
      IStreamPtr spStm;
      RETURN_FAILED(CreateStreamOnHGlobal(NULL, true, &spStm));
      RETURN_FAILED(spPersist->Save(spStm, false));

      // Rewind the stream
      LARGE_INTEGER li = {0};
      RETURN_FAILED(spStm->Seek(li, STREAM_SEEK_SET, NULL));

      // Create an encoded copy of the specified stream
      IStreamPtr spStmOut;
      RETURN_FAILED(CAdminSessionSecure<
        &__uuidof(AdminInterfaces)>::CreateDecryptedStream(spStm, &spStmOut));

      // Detach the output stream to the [out] parameter
      *ppHost = spStmOut.Detach();

      // Indicate success, but intentionally not S_OK
      return nCookie & 0x7FFFFFFF;
    }
};


/////////////////////////////////////////////////////////////////////////////