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


/////////////////////////////////////////////////////////////////////////////
//
template <const GUID* pguid>
struct CAdminSessionSecure
{
  static HRESULT CreateEncryptedStream(IStream* pstmIn, IStream** ppstmOut)
  {
    // Rewind the input stream
    LARGE_INTEGER li = {0};
    RETURN_FAILED(pstmIn->Seek(li, STREAM_SEEK_SET, NULL));

    // Create a new memory stream
    IStreamPtr spStmOut;
    RETURN_FAILED(CreateStreamOnHGlobal(NULL, true, &spStmOut));

    // XOR the stream with specified GUID
    const BYTE* pbGUID = reinterpret_cast<const BYTE*>(pguid);
    const BYTE* pbEnd = pbGUID + sizeof(GUID);
    while (true)
    {
      // Read in a GUID-sized chunk from the stream
      BYTE pBuffer[sizeof(GUID)];
      ULONG cbRead;
      RETURN_FAILED(pstmIn->Read(pBuffer, sizeof(GUID), &cbRead));
      if (!cbRead)
        break;

      // Loop through each DWORD and XOR it with the GUID
      BYTE* pbData = pBuffer;
      for (const BYTE* pb = pbGUID; pb < pbEnd; pb += sizeof(DWORD),
        pbData += sizeof(DWORD))
      {
        const DWORD* pdwGUID = reinterpret_cast<const DWORD*>(pb);
              DWORD* pdwData = reinterpret_cast<      DWORD*>(pbData);
        *pdwData ^= *pdwGUID;
      }

      // Write the encoded chunk to the output stream
      RETURN_FAILED(spStmOut->Write(pBuffer, cbRead, NULL));
    }

    // Detach the new stream to the [out] parameter
    *ppstmOut = spStmOut.Detach();

    // Indicate success
    return S_OK;
  }
  static HRESULT CreateDecryptedStream(IStream* pstmIn, IStream** ppstmOut)
  {
    // Current algorithm is the same for encrypt and decrypt
    return CreateEncryptedStream(pstmIn, ppstmOut);
  }
};


/////////////////////////////////////////////////////////////////////////////