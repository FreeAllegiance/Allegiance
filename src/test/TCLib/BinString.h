#pragma once

/////////////////////////////////////////////////////////////////////////////
// BinString.h : Declaration of the TCBinString<> template.
//


/////////////////////////////////////////////////////////////////////////////
template <class TCH>
struct TCBinStringAlgorithm
{
// Types
public:
  typedef       TCH* _PTSTR;
  typedef const TCH* _CPTSTR;

// Overrides
public:
  // Encoding
  static DWORD GetEncodedStringLength(const void* pvData, DWORD cbData);
  static HRESULT EncodeToString(const void* pvData, DWORD cbData, _PTSTR pszOut);
  // Decoding
  static bool IsAlgorithmSignature(_CPTSTR psz);
  static DWORD GetDecodedDataSize(_CPTSTR pszText);
  static HRESULT DecodeFromString(_CPTSTR pszText, void* pvData, DWORD cbOut);
};


/////////////////////////////////////////////////////////////////////////////
// Macro for declaring string literals in templates.
//
#define TCBinString_Literal(name, str)                                      \
  static _CPTSTR Get##name()                                                \
  {                                                                         \
    if (1 == sizeof(TCH))                                                   \
    {                                                                       \
      const static CHAR  s_sz##name##A[] = str;                             \
      return (_CPTSTR)s_sz##name##A;                                        \
    }                                                                       \
    else                                                                    \
    {                                                                       \
      const static WCHAR s_sz##name##W[] = L##str;                          \
      return (_CPTSTR)s_sz##name##W;                                        \
    }                                                                       \
  }                                                                         \
  static DWORD Get##name##Length()                                          \
  {                                                                         \
    return sizeof(str) - 1;                                                 \
  }


/////////////////////////////////////////////////////////////////////////////
template <class TCH>
struct TCBinStringAlgorithm_0_0 : public TCBinStringAlgorithm<TCH>
{
  static bool IsByteDisplayable(BYTE byte)
  {
    static const BYTE s_table[] =
    {
      0x00, 0x00, 0x00, 0x00,  // 0x00 - 0x1F
      0xFD, 0xF7, 0xFF, 0xF5,  // 0x20 - 0x3F &, (comma), <, and > excluded
      0xFF, 0xFF, 0xFF, 0xFF,  // 0x40 - 0x5F
      0xFF, 0xFF, 0xFF, 0xFF,  // 0x60 - 0x7F
      0x00, 0x00, 0x00, 0x00,  // 0x80 - 0x9F
      0x00, 0x00, 0x00, 0x00,  // 0xA0 - 0xBF
      0x00, 0x00, 0x00, 0x00,  // 0xC0 - 0xDF
      0x00, 0x00, 0x00, 0x00,  // 0xE0 - 0xFF
    };

    int  nIndex = byte / 8;
    BYTE nMask  = 0x80 >> (byte % 8);
    return !!(s_table[nIndex] & nMask);
  }
  static DWORD GetCharCode(BYTE byte, _PTSTR pszOut)
  {
    if (pszOut)
    {
      _CPTSTR pszCode;
      if (1 == sizeof(TCH))
      {
        const static CHAR  s_szCodeA[] = "0123456789ABCDEF";
        pszCode = (_CPTSTR)s_szCodeA;
      }
      else
      {
        const static WCHAR s_szCodeW[] = L"0123456789ABCDEF";
        pszCode = (_CPTSTR)s_szCodeW;
      }
      pszOut[0] = pszCode[byte / 0x10];
      pszOut[1] = pszCode[byte % 0x10];
    }
    return 2;
  }

  // Constants
  TCBinString_Literal(Signature  , "tc00:")
  TCBinString_Literal(EscSeqBegin, "&"    )
  TCBinString_Literal(EscSeqEnd  , ";"    )

  // Encoding
  static DWORD GetEncodedStringLength(const void* pvData, DWORD cbData)
  {
    bool bLastWasBin = false;
    DWORD cch = GetSignatureLength();
    const BYTE* pDataEnd = (BYTE*)pvData + cbData;
    for (const BYTE* pByte = (BYTE*)pvData; pByte < pDataEnd; ++pByte)
    {
      if (IsByteDisplayable(*pByte))
      {
        if (bLastWasBin)
          cch += GetEscSeqEndLength();
        cch += 1;
        bLastWasBin = false;
      }
      else
      {
        if (!bLastWasBin)
          cch += GetEscSeqBeginLength();
        cch += GetCharCode(*pByte, NULL);
        bLastWasBin = true;
      }
    }
    if (bLastWasBin)
      cch += GetEscSeqEndLength();
    return cch + sizeof(TCH); // Allow for null-terminator
  }
  static HRESULT EncodeToString(const void* pvData, DWORD cbData, _PTSTR pszOut)
  {
    // Null-terminate the specified string
    if (1 == sizeof(TCH))
      *pszOut = '\0';
    else
      *pszOut = L'\0';

    bool bLastWasBin = false;
    CopyMemory(pszOut, GetSignature(), GetSignatureLength() * sizeof(TCH));
    _PTSTR psz = pszOut + GetSignatureLength();
    const BYTE* pDataEnd = (BYTE*)pvData + cbData;
    for (const BYTE* pByte = (BYTE*)pvData; pByte < pDataEnd; ++pByte)
    {
      if (IsByteDisplayable(*pByte))
      {
        if (bLastWasBin)
        {
          DWORD cch = GetEscSeqEndLength();
          CopyMemory(psz, GetEscSeqEnd(), cch * sizeof(TCH));
          psz += cch;
        }
        *psz++ = (TCH)*pByte;
        bLastWasBin = false;
      }
      else
      {
        if (!bLastWasBin)
        {
          DWORD cch = GetEscSeqBeginLength();
          CopyMemory(psz, GetEscSeqBegin(), cch * sizeof(TCH));
          psz += cch;
        }
        psz += GetCharCode(*pByte, psz);
        bLastWasBin = true;
      }
    }
    if (bLastWasBin)
    {
      CopyMemory(psz, GetEscSeqEnd(), GetEscSeqEndLength() * sizeof(TCH));
      psz += GetEscSeqEndLength();
    }

    // Null-terminate the specified string
    if (1 == sizeof(TCH))
      *psz = '\0';
    else
      *psz = L'\0';
    return S_OK;
  }
  // Decoding
  static bool IsAlgorithmSignature(_CPTSTR psz)
  {
    return 0 == memcmp(psz, GetSignature(), GetSignatureLength() * sizeof(TCH));
  }
  static DWORD GetDecodedDataSize(_CPTSTR pszText)
  {
    TCH chNul, chAmp, chSemi;
    if (1 == sizeof(TCH))
    {
      chNul  = (TCH)'\0';
      chAmp  = (TCH)'&';
      chSemi = (TCH)';';
    }
    else
    {
      chNul  = (TCH)L'\0';
      chAmp  = (TCH)L'&';
      chSemi = (TCH)L';';
    }

    // Account for the encoding signature
    if (!IsAlgorithmSignature(pszText))
      return -1;

    bool bInEsc = false;
    DWORD cbData = 0, cbEsc = 0;
    for (_CPTSTR psz = pszText + GetSignatureLength(); *psz != chNul; ++psz)
    {
      if (!bInEsc)
      {
        if (chAmp == *psz)
          bInEsc = true;
        else
          ++cbData;
      }
      else
      {
        if (chSemi == *psz)
        {
          bInEsc = false;
          cbData += cbEsc / 2;
          cbEsc = 0;
        }
        else
        {
          ++cbEsc;
        }
      }
    }

    return cbData;
  }
  static HRESULT DecodeFromString(_CPTSTR pszText, void* pvData, DWORD* pcbOut)
  {
    // Constants
    const static BYTE s_Codes[] =
    {                                                  // When offset by '0'
      0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,  // 01234567
      0x08, 0x09, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,  // 89:;<=>?
      0xFF, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0xFF,  // @ABCDEFG
      0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,  // HIJKLMNO
    };
    const BYTE chNul  = BYTE('\0');
    const BYTE chAmp  = BYTE('&');
    const BYTE chSemi = BYTE(';');
    const BYTE chZero = BYTE('0');

    // Account for the encoding signature
    if (!IsAlgorithmSignature(pszText))
      return E_INVALIDARG;

    // Loop through the input string
    bool bInEsc = false;
    BYTE* pb = (BYTE*)pvData;
    for (_CPTSTR psz = pszText + GetSignatureLength(); *psz != chNul; ++psz)
    {
      BYTE bChar = LOBYTE(*psz);
      if (!bInEsc)
      {
        if (chAmp == bChar)
          bInEsc = true;
        else
          *pb++ = bChar;
      }
      else
      {
        if (chSemi != bChar)
        {
          // OR against 0xF0 to check for invalid escapes
          BYTE bHiBitSum = 0;

          // First character of escape code
          BYTE bIndex = (bChar - chZero);
          bHiBitSum |= bIndex;
          BYTE bValue = s_Codes[bIndex & 0x1F];
          bHiBitSum |= bValue;
          *pb = bValue << 4;

          // Second character of escape code
          bChar = LOBYTE(*++psz);
          bIndex = (bChar - chZero);
          bHiBitSum |= bIndex;
          bValue = s_Codes[bIndex & 0x1F];
          bHiBitSum |= bValue;
          *pb++ |= bValue;

          // Invalid input string if any of the high three bits set
          if (bHiBitSum & 0xE0)
            return E_INVALIDARG;
        }
        else
        {
          bInEsc = false;
        }
      }
    }

    // Compute the length, if requested
    if (pcbOut)
      *pcbOut = pb - (BYTE*)pvData;

    // Indicate success
    return S_OK;
  }
};


/////////////////////////////////////////////////////////////////////////////
// Definition of the current coding algorithm version.
//
#define TCBinStringAlgorithm_Current TCBinStringAlgorithm_0_0


/////////////////////////////////////////////////////////////////////////////
// Provides a set of static methods used to convert between a binary stream
// and an ANSI text string that uses only displayable, database-compatible
// characters.
//
// This implementation 
//
template <class TCH, class ALGO = TCBinStringAlgorithm_Current<TCH> >
class TCBinString
{
// Types
public:
  typedef       TCH* _PTSTR;
  typedef const TCH* _CPTSTR;

// Encoding
public:
  static DWORD GetEncodedStringLength(const void* pvData, DWORD cbData)
  {
    return ALGO::GetEncodedStringLength(pvData, cbData);
  }

  static HRESULT EncodeToString(const void* pvData, DWORD cbData, _PTSTR pszOut)
  {
    return ALGO::EncodeToString(pvData, cbData, pszOut);
  }

// Decoding
public:
  static DWORD GetDecodedDataSize(_CPTSTR pszText)
  {
    if (TCBinStringAlgorithm_0_0<TCH>::IsAlgorithmSignature(pszText))
      return TCBinStringAlgorithm_0_0<TCH>::GetDecodedDataSize(pszText);
    // else if (LESS_CURRENT_ALGORITHM::IsAlgorithmSignature(pszText))
    //   return LESS_CURRENT_ALGORITHM::GetDecodedDataSize(pszText);
    // else if ... etc.
    return 0;
  }

  static HRESULT DecodeFromString(_CPTSTR pszText, void* pvData, DWORD* pcbOut)
  {
    if (TCBinStringAlgorithm_0_0<TCH>::IsAlgorithmSignature(pszText))
      return TCBinStringAlgorithm_0_0<TCH>::DecodeFromString(pszText, pvData, pcbOut);
    // else if (LESS_CURRENT_ALGORITHM::IsAlgorithmSignature(pszText))
    //   return LESS_CURRENT_ALGORITHM::GetDecodedDataSize(pszText);
    // else if ... etc.
    return E_INVALIDARG;
  }
};

/////////////////////////////////////////////////////////////////////////////