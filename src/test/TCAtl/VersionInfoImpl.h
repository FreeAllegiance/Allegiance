#pragma once

/////////////////////////////////////////////////////////////////////////////
// VersionInfoImpl.h: Declaration of the TCVersionInfoImpl class.template.
//

#include "..\TCLib\ObjectLock.h"
#include "..\TCLib\AutoHandle.h"


/////////////////////////////////////////////////////////////////////////////
// Interface Map Macro
//
// Classes derived from TCVersionInfoImpl should include this macro in their
// interface maps.
//
#define COM_INTERFACE_ENTRIES_TCVersionInfoImpl()                           \
    COM_INTERFACE_ENTRY(IDispatch)                                          \
    COM_INTERFACE_ENTRY(ISupportErrorInfo)                                  \
    COM_INTERFACE_ENTRY(IPersistStreamInit)                                 \
    COM_INTERFACE_ENTRY2(IPersistStream, IPersistStreamInit)                \
    COM_INTERFACE_ENTRY2(IPersist, IPersistStreamInit)                      \
    COM_INTERFACE_ENTRY(IMarshal)


/////////////////////////////////////////////////////////////////////////////
// Macros to make implementation less tedious

#define TCVersionInfoImpl_Flag(flagName)                                    \
  STDMETHODIMP get_Is##flagName(VARIANT_BOOL* pFlag)                        \
  {                                                                         \
    XLock lock(static_cast<T*>(this));                                      \
    RETURN_FAILED(VerifyInit(pFlag));                                       \
    *pFlag = VARBOOL(m_vi.Is##flagName());                                  \
    return S_OK;                                                            \
  }

#define TCVersionInfoImpl_CommonValue(valueName)                            \
  STDMETHODIMP get_##valueName(BSTR* pbstrValue)                            \
  {                                                                         \
    XLock lock(static_cast<T*>(this));                                      \
    RETURN_FAILED(VerifyInit(pbstrValue));                                  \
    USES_CONVERSION;                                                        \
    ZString strValue(m_vi.Get##valueName());                                \
    HRESULT hr = HRESULT_FROM_WIN32(GetLastError());                        \
    *pbstrValue = CComBSTR(strValue).Detach();                              \
    return hr;                                                              \
  }


/////////////////////////////////////////////////////////////////////////////
// TCVersionInfoImpl
//
template <class T, class I, const GUID* pTLB>
class TCVersionInfoImpl : 
  public IDispatchImpl<I, &__uuidof(I), pTLB>,
  public ISupportErrorInfo,
  public IPersistStreamInit,
  public IMarshal,
  public CComObjectRootEx<CComMultiThreadModel>
{
// Types
protected:
  typedef TCObjectLock<T> XLock;

// Construction / Destruction
public:
  TCVersionInfoImpl() :
    m_vi(false),
    m_bDirty(false)
  {
  }

// Implementation
protected:
  HRESULT VerifyInit()
  {
    // Verify that we have been initialized
    XLock lock(static_cast<T*>(this));
    return m_vi.GetFixed() ? S_OK :
      T::Error(L"Version object has not been initialized.", __uuidof(I));
  }
  HRESULT VerifyInit(VARIANT_BOOL* pb)
  {
    CLEAROUT(pb, VARIANT_FALSE);
    return VerifyInit();
  }
  HRESULT VerifyInit(BSTR* pbstr)
  {
    CLEAROUT(pbstr, (BSTR)NULL);
    return VerifyInit();
  }
  HRESULT VerifyInit(WORD* pw)
  {
    CLEAROUT(pw, (WORD)0);
    return VerifyInit();
  }

// ISupportErrorInfo Interface Methods
public:
  STDMETHODIMP InterfaceSupportsErrorInfo(REFIID riid)
  {
    return (__uuidof(I) == riid) ? S_OK : S_FALSE;
  }

// IPersist Interface Methods
public:
  STDMETHODIMP GetClassID(CLSID* pClassID)
  {
    __try
    {
      *pClassID = T::GetObjectCLSID();
    }
    __except(1)
    {
      return E_POINTER;
    }
    return S_OK;
  }

// IPersistStreamInit Interface Methods
public:
  STDMETHODIMP IsDirty()
  {
    // Return dirty flag
    XLock lock(static_cast<T*>(this));
    return m_bDirty ? S_OK : S_FALSE;
  }
  STDMETHODIMP Load(LPSTREAM pStm)
  {
    // Initialize the object
    XLock lock(static_cast<T*>(this));
    RETURN_FAILED(InitNew());

    // Read the number of bytes of version data
    UINT cbVerInfo = 0;
    RETURN_FAILED(pStm->Read(&cbVerInfo, sizeof(cbVerInfo), NULL));
    if (!cbVerInfo)
      return S_OK;

    // Allocate the memory
    TCArrayPtr<void*> spvVerInfo = new BYTE[cbVerInfo];
    if (spvVerInfo.IsNull())
      return E_OUTOFMEMORY;

    // Read the version info from the stream
    RETURN_FAILED(pStm->Read(spvVerInfo, cbVerInfo, NULL));

    // Initialize the ZVersionInfo object
    m_vi.Load(spvVerInfo, cbVerInfo);

    // Read the module filename
    m_bstrFileName.Empty();
    RETURN_FAILED(m_bstrFileName.ReadFromStream(pStm));

    // Indicate success
    return S_OK;
  }

  STDMETHODIMP Save(LPSTREAM pStm, BOOL fClearDirty)
  {
    // Write the number of bytes of version data
    XLock lock(static_cast<T*>(this));
    UINT cbVerInfo = m_vi.GetVerInfoSize();
    RETURN_FAILED(pStm->Write(&cbVerInfo, sizeof(cbVerInfo), NULL));

    // Write the version data
    if (cbVerInfo)
      RETURN_FAILED(pStm->Write(m_vi.GetVerInfo(), cbVerInfo, NULL));

    // Write the module filename
    RETURN_FAILED(m_bstrFileName.WriteToStream(pStm));

    // Clear the dirty flag, if specified
    if (fClearDirty)
      m_bDirty = false;

    // Indicate success
    return S_OK;
  }
  STDMETHODIMP GetSizeMax(ULARGE_INTEGER* pCbSize)
  {
    XLock lock(static_cast<T*>(this));
    return TCGetPersistStreamSize(static_cast<T*>(this)->GetUnknown(), pCbSize);
  }
  STDMETHODIMP InitNew(void)
  {
    // Set the dirty flag
    XLock lock(static_cast<T*>(this));
    m_bDirty = true;

    // Unload the ZVersionInfo object
    m_vi.Unload();

    // Reset the filename
    m_bstrFileName.Empty();

    // Indicate success
    return S_OK;
  }

// IMarshal Interface Methods
public:
  STDMETHODIMP GetUnmarshalClass(REFIID riid, void* pv, DWORD dwDestContext,
    void* pvDestContext, DWORD mshlflags, CLSID* pCid)
  {
    UNUSED(riid);
    UNUSED(pv);
    UNUSED(dwDestContext);
    UNUSED(pvDestContext);
    UNUSED(mshlflags);

    // Delegate to IPersistStream::GetClassID
    return GetClassID(pCid);
  }
  STDMETHODIMP GetMarshalSizeMax(REFIID riid, void* pv, DWORD dwDestContext,
    void* pvDestContext, DWORD mshlflags, DWORD* pSize)
  {
    UNUSED(riid);
    UNUSED(pv);
    UNUSED(dwDestContext);
    UNUSED(pvDestContext);
    UNUSED(mshlflags);

    // Initialize the [out] parameter
    *pSize = 0;

    // Delegate to IPersistStream::GetSizeMax
    ULARGE_INTEGER uli;
    HRESULT hr = hr = GetSizeMax(&uli);
    if (SUCCEEDED(hr))
    {
      // Include the size of an endian indicator DWORD value
      *pSize = uli.LowPart + sizeof(m_dwEndian);
    }

    // Return the last HRESULT
    return hr;
  }
  STDMETHODIMP MarshalInterface(IStream* pStm, REFIID riid, void* pv,
    DWORD dwDestContext, void* pvDestContext, DWORD mshlflags)
  {
    UNUSED(riid);
    UNUSED(pv);
    UNUSED(dwDestContext);
    UNUSED(pvDestContext);
    UNUSED(mshlflags);

    // Write an endian indicator DWORD value to the stream
    DWORD dwEndian = m_dwEndianOriginal;
    HRESULT hr = pStm->Write(&dwEndian, sizeof(dwEndian), NULL);

    // Delegate to IPersistStream::Save
    if (SUCCEEDED(hr))
      hr = Save(pStm, FALSE);

    // Return the last HRESULT
    return hr;
  }
  STDMETHODIMP UnmarshalInterface(IStream* pStm, REFIID riid, void** ppv)
  {
    // Read the endian indicator DWORD value from the stream
    XLock lock(static_cast<T*>(this));
    HRESULT hr = pStm->Read(&m_dwEndian, sizeof(m_dwEndian), NULL);
    if (SUCCEEDED(hr))
    {
      // Delegate to IPersistStream::Load
      if (SUCCEEDED(hr = Load(pStm)))
        hr = static_cast<T*>(this)->QueryInterface(riid, ppv);
    }

    // Return the last HRESULT
    return hr;
  }
  STDMETHODIMP ReleaseMarshalData(IStream* pStm)
  {
    // Marshaling by value does not cause us to acquire any resources
    return S_OK;
  }
  STDMETHODIMP DisconnectObject(DWORD dwReserved)
  {
    // Marshaling by value has no notion of connected-ness
    return S_OK;
  }

// <I> Interface Methods
public:
  // Initialization
  STDMETHODIMP put_FileName(BSTR bstrFileName)
  {
    // Use the current module, if an empty string was specified
    LPTSTR pszFileName = NULL;
    if (!BSTRLen(bstrFileName))
    {
      DWORD cch = _MAX_PATH * sizeof(TCHAR);
      pszFileName = (LPTSTR)_alloca(cch);
      GetModuleFileName(NULL, pszFileName, cch);
    }
    else
    {
      USES_CONVERSION;
      pszFileName = OLE2T(bstrFileName);
    }

    // Set the dirty flag
    XLock lock(static_cast<T*>(this));
    m_bDirty = true;

    // Load the version information resource
    if (!m_vi.Load(pszFileName))
      return HRESULT_FROM_WIN32(GetLastError());

    // Save the specified file name
    m_bstrFileName = pszFileName;

    // Indicate success
    return S_OK;
  }
  STDMETHODIMP get_FileName(BSTR* pbstrFileName)
  {
    CLEAROUT(pbstrFileName, (BSTR)NULL);
    *pbstrFileName = m_bstrFileName.Copy();
    return S_OK;
  }

  // File Version
  STDMETHODIMP get_FileVersionString(BSTR* pbstrString)
  {
    XLock lock(static_cast<T*>(this));
    RETURN_FAILED(VerifyInit(pbstrString));
    CComBSTR bstrFmt(m_vi.GetFileVersionString());
    *pbstrString = bstrFmt.Detach();
    return S_OK;
  }
  STDMETHODIMP get_FileVersionMSHigh(WORD* pwMSHigh)
  {
    XLock lock(static_cast<T*>(this));
    RETURN_FAILED(VerifyInit(pwMSHigh));
    *pwMSHigh = m_vi.GetFileVersionMSHigh();
    return S_OK;
  }
  STDMETHODIMP get_FileVersionMSLow(WORD* pwMSLow)
  {
    XLock lock(static_cast<T*>(this));
    RETURN_FAILED(VerifyInit(pwMSLow));
    *pwMSLow = m_vi.GetFileVersionMSLow();
    return S_OK;
  }
  STDMETHODIMP get_FileVersionLSHigh(WORD* pwLSHigh)
  {
    XLock lock(static_cast<T*>(this));
    RETURN_FAILED(VerifyInit(pwLSHigh));
    *pwLSHigh = m_vi.GetFileVersionLSHigh();
    return S_OK;
  }
  STDMETHODIMP get_FileVersionLSLow(WORD* pwLSLow)
  {
    XLock lock(static_cast<T*>(this));
    RETURN_FAILED(VerifyInit(pwLSLow));
    *pwLSLow = m_vi.GetFileVersionLSLow();
    return S_OK;
  }
  STDMETHODIMP get_FileBuildNumber(WORD* pwBuildNumber)
  {
    XLock lock(static_cast<T*>(this));
    RETURN_FAILED(VerifyInit(pwBuildNumber));
    *pwBuildNumber = m_vi.GetFileBuildNumber();
    return S_OK;
  }

  // Product Version
  STDMETHODIMP get_ProductVersionString(BSTR* pbstrString)
  {
    XLock lock(static_cast<T*>(this));
    RETURN_FAILED(VerifyInit(pbstrString));
    CComBSTR bstrFmt(m_vi.GetProductVersionString());
    *pbstrString = bstrFmt.Detach();
    return S_OK;
  }
  STDMETHODIMP get_ProductVersionMSHigh(WORD* pwMSHigh)
  {
    XLock lock(static_cast<T*>(this));
    RETURN_FAILED(VerifyInit(pwMSHigh));
    *pwMSHigh = m_vi.GetProductVersionMSHigh();
    return S_OK;
  }
  STDMETHODIMP get_ProductVersionMSLow(WORD* pwMSLow)
  {
    XLock lock(static_cast<T*>(this));
    RETURN_FAILED(VerifyInit(pwMSLow));
    *pwMSLow = m_vi.GetProductVersionMSLow();
    return S_OK;
  }
  STDMETHODIMP get_ProductVersionLSHigh(WORD* pwLSHigh)
  {
    XLock lock(static_cast<T*>(this));
    RETURN_FAILED(VerifyInit(pwLSHigh));
    *pwLSHigh = m_vi.GetProductVersionLSHigh();
    return S_OK;
  }
  STDMETHODIMP get_ProductVersionLSLow(WORD* pwLSLow)
  {
    XLock lock(static_cast<T*>(this));
    RETURN_FAILED(VerifyInit(pwLSLow));
    *pwLSLow = m_vi.GetProductVersionLSLow();
    return S_OK;
  }
  STDMETHODIMP get_ProductBuildNumber(WORD* pwBuildNumber)
  {
    XLock lock(static_cast<T*>(this));
    RETURN_FAILED(VerifyInit(pwBuildNumber));
    *pwBuildNumber = m_vi.GetProductBuildNumber();
    return S_OK;
  }

  // File Flags
  TCVersionInfoImpl_Flag(Debug       )
  TCVersionInfoImpl_Flag(InfoInferred)
  TCVersionInfoImpl_Flag(Patched     )
  TCVersionInfoImpl_Flag(PreRelease  )
  TCVersionInfoImpl_Flag(PrivateBuild)
  TCVersionInfoImpl_Flag(SpecialBuild)

  // Target Operating Systems
  TCVersionInfoImpl_Flag(TargetUnknown  )
  TCVersionInfoImpl_Flag(TargetDOS      )
  TCVersionInfoImpl_Flag(TargetOS216    )
  TCVersionInfoImpl_Flag(TargetOS232    )
  TCVersionInfoImpl_Flag(TargetNT       )
  TCVersionInfoImpl_Flag(TargetBase     )
  TCVersionInfoImpl_Flag(TargetWin16    )
  TCVersionInfoImpl_Flag(TargetPM16     )
  TCVersionInfoImpl_Flag(TargetPM32     )
  TCVersionInfoImpl_Flag(TargetWin32    )
  TCVersionInfoImpl_Flag(TargetDOSWin16 )
  TCVersionInfoImpl_Flag(TargetDOSWin32 )
  TCVersionInfoImpl_Flag(TargetOS216PM16)
  TCVersionInfoImpl_Flag(TargetOS232PM32)
  TCVersionInfoImpl_Flag(TargetNTWin32  )

  // File Types
  TCVersionInfoImpl_Flag(App      )
  TCVersionInfoImpl_Flag(Dll      )
  TCVersionInfoImpl_Flag(Driver   )
  TCVersionInfoImpl_Flag(Font     )
  TCVersionInfoImpl_Flag(Vxd      )
  TCVersionInfoImpl_Flag(StaticLib)

  // Driver Types
  TCVersionInfoImpl_Flag(DriverUnknown    )
  TCVersionInfoImpl_Flag(DriverPrinter    )
  TCVersionInfoImpl_Flag(DriverKeyboard   )
  TCVersionInfoImpl_Flag(DriverLanguage   )
  TCVersionInfoImpl_Flag(DriverDisplay    )
  TCVersionInfoImpl_Flag(DriverMouse      )
  TCVersionInfoImpl_Flag(DriverNetwork    )
  TCVersionInfoImpl_Flag(DriverSystem     )
  TCVersionInfoImpl_Flag(DriverInstallable)
  TCVersionInfoImpl_Flag(DriverSound      )
  TCVersionInfoImpl_Flag(DriverComm       )
  TCVersionInfoImpl_Flag(DriverInputMethod)

  // Font Types
  TCVersionInfoImpl_Flag(FontRaster  )
  TCVersionInfoImpl_Flag(FontVector  )
  TCVersionInfoImpl_Flag(FontTrueType)

  // String Values
  STDMETHODIMP put_LanguageID(WORD wLangID)
  {
    XLock lock(static_cast<T*>(this));
    if (wLangID != m_vi.GetLanguageID())
    {
      m_vi.SetLanguageID(wLangID);
      m_bDirty = true;
    }
    return S_OK;
  }
  STDMETHODIMP get_LanguageID(WORD* pwLangID)
  {
    XLock lock(static_cast<T*>(this));
    CLEAROUT(pwLangID, (WORD)m_vi.GetLanguageID());
    return S_OK;
  }
  STDMETHODIMP put_CodePage(WORD wCodePage)
  {
    XLock lock(static_cast<T*>(this));
    if (wCodePage != m_vi.GetCodePage())
    {
      m_vi.SetCodePage(wCodePage);
      m_bDirty = true;
    }
    return S_OK;
  }
  STDMETHODIMP get_CodePage(WORD* pwCodePage)
  {
    XLock lock(static_cast<T*>(this));
    CLEAROUT(pwCodePage, (WORD)m_vi.GetCodePage());
    return S_OK;
  }
  STDMETHODIMP get_Exists(BSTR bstrKey, VARIANT_BOOL* pbExists)
  {
    XLock lock(static_cast<T*>(this));
    RETURN_FAILED(VerifyInit(pbExists));
    bool bExists;
    USES_CONVERSION;
    m_vi.GetStringValue(bstrKey ? OLE2CT(bstrKey) : "", &bExists);
    *pbExists = VARBOOL(bExists);
    return HRESULT_FROM_WIN32(GetLastError());
  }
  STDMETHODIMP get_Value(BSTR bstrKey, BSTR* pbstrValue)
  {
    XLock lock(static_cast<T*>(this));
    RETURN_FAILED(VerifyInit(pbstrValue));
    USES_CONVERSION;
    ZString strValue(m_vi.GetStringValue(bstrKey ? OLE2CT(bstrKey) : ""));
    HRESULT hr = HRESULT_FROM_WIN32(GetLastError());
    *pbstrValue = CComBSTR(strValue).Detach();
    return hr;
  }

  // Common String Values
  TCVersionInfoImpl_CommonValue(CompanyName     )
  TCVersionInfoImpl_CommonValue(FileDescription )
  TCVersionInfoImpl_CommonValue(FileVersion     )
  TCVersionInfoImpl_CommonValue(InternalName    )
  TCVersionInfoImpl_CommonValue(LegalCopyright  )
  TCVersionInfoImpl_CommonValue(OriginalFilename)
  TCVersionInfoImpl_CommonValue(ProductName     )
  TCVersionInfoImpl_CommonValue(ProductVersion  )
  TCVersionInfoImpl_CommonValue(SpecialBuild    )
  TCVersionInfoImpl_CommonValue(OLESelfRegister )

// Data Members
protected:
  CComBSTR     m_bstrFileName;
  ZVersionInfo m_vi;
  bool         m_bDirty:1;
  DWORD        m_dwEndian;
  static const DWORD m_dwEndianOriginal, m_dwEndianInverted;
};


/////////////////////////////////////////////////////////////////////////////
// Static Initialization

template <class T, class I, const GUID* pTLB>
const DWORD TCVersionInfoImpl<T, I, pTLB>::m_dwEndianOriginal = 0xFF669900;

template <class T, class I, const GUID* pTLB>
const DWORD TCVersionInfoImpl<T, I, pTLB>::m_dwEndianInverted = 0x009966FF;


/////////////////////////////////////////////////////////////////////////////