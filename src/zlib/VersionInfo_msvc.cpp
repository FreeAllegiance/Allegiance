/////////////////////////////////////////////////////////////////////////////
// VersionInfo.cpp : Implementation of the ZVersionInfo class.
//

#include "VersionInfo.h"

/////////////////////////////////////////////////////////////////////////////
// ZVersionInfo


/////////////////////////////////////////////////////////////////////////////
// Construction / Destruction

class ZVersionInfoPrivate {
public:
	uint8_t*            m_pVerInfo = nullptr;
	UINT                m_cbVerInfo = 0;
	VS_FIXEDFILEINFO*   m_pFixed = nullptr;
	LANGID              m_wLangID = -1;
	uint16_t            m_wCodePage = -1;
};

ZVersionInfo::ZVersionInfo(uintptr_t hinstance) :
	d(new ZVersionInfoPrivate)
{
  bool hr = Load(hinstance);
  ZAssert(hr);
}

ZVersionInfo::ZVersionInfo(bool) :
	d(new ZVersionInfoPrivate)
{
}

ZVersionInfo::ZVersionInfo(LPCTSTR szModule) :
	d(new ZVersionInfoPrivate)
{
  bool hr = Load(szModule);
  ZAssert(hr);
}

ZVersionInfo::~ZVersionInfo()
{
  // Ensure that there is no previous version information block allocated
  Unload();
}

bool ZVersionInfo::Load(LPCTSTR szModule)
{
//  // Check for a nullptr or empty string
  if (!szModule || TEXT('\0') == *szModule)
    return Load();

  // Ensure that there is no previous version information block allocated
  Unload();
  ZAssert(!d->m_cbVerInfo);
  ZAssert(!d->m_pVerInfo);
  ZAssert(!d->m_pFixed);

  // Get the size of the version information of the specified module
  uint8_t* pVerInfo = nullptr;
  uint32_t cbVerInfo, dummy;
  cbVerInfo = GetFileVersionInfoSize(const_cast<LPTSTR>(szModule), LPDWORD(&dummy));
  if (cbVerInfo)
  {
    // Allocate space to hold the version information
    pVerInfo = new uint8_t[cbVerInfo];
    if (!pVerInfo)
    {
      SetLastError(ERROR_NOT_ENOUGH_MEMORY);
      return false;
    }
  }
  else
    cbVerInfo = sizeof(*d->m_pFixed);

  // Attempt to load the version information block
  if (!GetFileVersionInfo(const_cast<LPTSTR>(szModule), 0, cbVerInfo, pVerInfo))
  {
    uint32_t dwLastError = GetLastError();
    delete [] pVerInfo;
    SetLastError(dwLastError ? dwLastError : ERROR_NO_MORE_ITEMS);
    return false;
  }

  // Attempt to get a pointer to the fixed version information
  VS_FIXEDFILEINFO* pFixed = nullptr;
  UINT cbFixed = 0;
  if (!VerQueryValue(pVerInfo, TEXT("\\"), (void**)&pFixed, &cbFixed) || cbFixed != sizeof(*pFixed))
  {
    uint32_t dwLastError = GetLastError();
    delete [] pVerInfo;
    SetLastError(dwLastError);
    return false;
  }

  // Save the size of the version info and the allocated pointers
  d->m_cbVerInfo = cbVerInfo;
  d->m_pVerInfo  = pVerInfo;
  d->m_pFixed    = pFixed;

  // Indicate success
  SetLastError(0);
  return true;
}

bool ZVersionInfo::Load(uintptr_t hinstance)
{
  // Get the module name of the specified instance
  TCHAR szModule[_MAX_PATH];
  if (!GetModuleFileName(HINSTANCE(hinstance), szModule, sizeofArray(szModule)))
    return false;

  // Pass on to the string overload of this function
  return Load(szModule);
}

bool ZVersionInfo::Load(const void* pvVerInfo, UINT cbVerInfo)
{
  // Ensure that a nullptr was not passed
  if (!pvVerInfo)
  {
    SetLastError(ERROR_INVALID_PARAMETER);
    return false;
  }

  // Ensure that there is no previous version information block allocated
  Unload();

  // Allocate space to hold the version information
  uint8_t* pVerInfo = new uint8_t[cbVerInfo];
  if (!pVerInfo)
  {
    SetLastError(ERROR_NOT_ENOUGH_MEMORY);
    return false;
  }

  // Copy the specified block
  CopyMemory(pVerInfo, pvVerInfo, cbVerInfo);

  // Attempt to get a pointer to the fixed version information
  VS_FIXEDFILEINFO* pFixed = nullptr;
  UINT cbFixed = 0;
  if (!VerQueryValue(pVerInfo, TEXT("\\"), (void**)&pFixed, &cbFixed) || cbFixed != sizeof(*pFixed))
  {
    uint32_t dwLastError = GetLastError();
    delete [] pVerInfo;
    SetLastError(dwLastError ? dwLastError : ERROR_NO_MORE_ITEMS);
    return false;
  }

  // Save the size of the version info and the allocated and fixed pointers
  d->m_cbVerInfo = cbVerInfo;
  d->m_pVerInfo  = pVerInfo;
  d->m_pFixed    = pFixed;

  // Indicate success
  SetLastError(0);
  return true;
}

void ZVersionInfo::Unload()
{
  // Deallocate the version information block
  if (d->m_pVerInfo)
    delete d->m_pVerInfo;

  d->m_pVerInfo  = nullptr;
  d->m_cbVerInfo = 0;
  d->m_pFixed    = nullptr;
}


/////////////////////////////////////////////////////////////////////////////
// Attributes


/////////////////////////////////////////////////////////////////////////////
// String Values

ZString ZVersionInfo::GetStringValue(LPCTSTR pszKey, bool* pbExists) const
{
  // Initialize the [out] parameter
  if (pbExists)
    *pbExists = false;

  // Ensure that we have a non-nullptr key
  if (!pszKey)
  {
    SetLastError(ERROR_INVALID_PARAMETER);
    return ZString();
  }

  // Determine if a LANGID was specified
  LANGID wLangID = LANGID((LANGID(-1) == GetLanguageID()) ?
    MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US) : GetLanguageID());

  // Determine if a code page was specified
  uint16_t rgwCodePages[] =
  {
    1200,  // Unicode
    1252,  // Windows Multilingual
    0000,  // Neutral
  };
  int cCodePages = sizeofArray(rgwCodePages);
  if (uint16_t(-1) != d->m_wCodePage)
  {
    rgwCodePages[0] = d->m_wCodePage;
    cCodePages = 1;
  }

  // Format the base sub-block string
  TCHAR szBase[32];
  _stprintf_s(szBase, 32, TEXT("\\StringFileInfo\\%04X"), wLangID);

  // Loop thru each code page
  for (int iCP = 0; iCP < cCodePages; ++iCP)
  {
    // Format a sub-block string
    TCHAR szSubBlock[_MAX_PATH * 2];
    _stprintf_s(szSubBlock, _MAX_PATH * 2, TEXT("%s%04X\\%s"), szBase, rgwCodePages[iCP], pszKey);

    // Query the value
    UINT cbValue = 0;
    LPCTSTR pszValue = nullptr;
    if (d->m_pVerInfo != nullptr && VerQueryValue(d->m_pVerInfo, szSubBlock, (void**)&pszValue, &cbValue))
    {
      // Indicate that the key exists
      if (pbExists)
        *pbExists = true;

      // Indicate success
      SetLastError(0);
      return ZString(pszValue);
    }
  }

  // Indicate success
  SetLastError(0);
  return ZString();  
}

ZString ZVersionInfo::GetFileVersionString() const
{
  ZAssert(d->m_pFixed);
  TCHAR szText[32];
  _stprintf_s(szText, 32, TEXT("%hu.%02hu.%02hu.%04hu"),
    GetFileVersionMSHigh(), GetFileVersionMSLow(),
    GetFileVersionLSHigh(), GetFileVersionLSLow());
  return ZString(szText);
}

uint16_t ZVersionInfo::GetFileVersionMSHigh() const
{
  ZAssert(d->m_pFixed);
  return HIWORD(d->m_pFixed->dwFileVersionMS);
}

uint16_t ZVersionInfo::GetFileVersionMSLow() const
{
  ZAssert(d->m_pFixed);
  return LOWORD(d->m_pFixed->dwFileVersionMS);
}

uint16_t ZVersionInfo::GetFileVersionLSHigh() const
{
  ZAssert(d->m_pFixed);
  return HIWORD(d->m_pFixed->dwFileVersionLS);
}

uint16_t ZVersionInfo::GetFileVersionLSLow() const
{
  ZAssert(d->m_pFixed);
  return LOWORD(d->m_pFixed->dwFileVersionLS);
}

uint16_t ZVersionInfo::GetFileBuildNumber() const
{
  return GetFileVersionLSLow();
}

ZString ZVersionInfo::GetProductVersionString() const
{
  ZAssert(d->m_pFixed);
  TCHAR szText[32];
  _stprintf_s(szText, 32, TEXT("%hd.%02hd.%02hd.%04hd"),
    GetProductVersionMSHigh(), GetProductVersionMSLow(),
    GetProductVersionLSHigh(), GetProductVersionLSLow());
  return ZString(szText);
}

uint16_t ZVersionInfo::GetProductVersionMSHigh() const
{
  ZAssert(d->m_pFixed);
  return HIWORD(d->m_pFixed->dwProductVersionMS);
}

uint16_t ZVersionInfo::GetProductVersionMSLow() const
{
  ZAssert(d->m_pFixed);
  return LOWORD(d->m_pFixed->dwProductVersionMS);
}

uint16_t ZVersionInfo::GetProductVersionLSHigh() const
{
  ZAssert(d->m_pFixed);
  return HIWORD(d->m_pFixed->dwProductVersionLS);
}

uint16_t ZVersionInfo::GetProductVersionLSLow() const
{
  ZAssert(d->m_pFixed);
  return LOWORD(d->m_pFixed->dwProductVersionLS);
}

uint16_t ZVersionInfo::GetProductBuildNumber() const
{
  return GetProductVersionLSLow();
}


/////////////////////////////////////////////////////////////////////////////
// File Flags

bool ZVersionInfo::IsDebug() const
{
  ZAssert(d->m_pFixed);
  return !!(VS_FF_DEBUG & d->m_pFixed->dwFileFlagsMask & d->m_pFixed->dwFileFlags);
}

bool ZVersionInfo::IsInfoInferred() const
{
  ZAssert(d->m_pFixed);
  return !!(VS_FF_INFOINFERRED & d->m_pFixed->dwFileFlagsMask & d->m_pFixed->dwFileFlags);
}

bool ZVersionInfo::IsPatched() const
{
  ZAssert(d->m_pFixed);
  return !!(VS_FF_PATCHED & d->m_pFixed->dwFileFlagsMask & d->m_pFixed->dwFileFlags);
}

bool ZVersionInfo::IsPreRelease() const
{
  ZAssert(d->m_pFixed);
  return !!(VS_FF_PRERELEASE & d->m_pFixed->dwFileFlagsMask & d->m_pFixed->dwFileFlags);
}

bool ZVersionInfo::IsPrivateBuild() const
{
  ZAssert(d->m_pFixed);
  return !!(VS_FF_PRIVATEBUILD & d->m_pFixed->dwFileFlagsMask & d->m_pFixed->dwFileFlags);
}

bool ZVersionInfo::IsSpecialBuild() const
{
  ZAssert(d->m_pFixed);
  return !!(VS_FF_SPECIALBUILD & d->m_pFixed->dwFileFlagsMask & d->m_pFixed->dwFileFlags);
}


/////////////////////////////////////////////////////////////////////////////
// Target Operating Systems

bool ZVersionInfo::IsTargetUnknown() const
{
  ZAssert(d->m_pFixed);
  return VOS_UNKNOWN == d->m_pFixed->dwFileOS;
}

bool ZVersionInfo::IsTargetDOS() const
{
  ZAssert(d->m_pFixed);
  return HIWORD(VOS_DOS) == HIWORD(d->m_pFixed->dwFileOS);
}

bool ZVersionInfo::IsTargetOS216() const
{
  ZAssert(d->m_pFixed);
  return HIWORD(VOS_OS216) == HIWORD(d->m_pFixed->dwFileOS);
}

bool ZVersionInfo::IsTargetOS232() const
{
  ZAssert(d->m_pFixed);
  return HIWORD(VOS_OS232) == HIWORD(d->m_pFixed->dwFileOS);
}

bool ZVersionInfo::IsTargetNT() const
{
  ZAssert(d->m_pFixed);
  return HIWORD(VOS_NT) == HIWORD(d->m_pFixed->dwFileOS);
}

bool ZVersionInfo::IsTargetBase() const
{
  ZAssert(d->m_pFixed);
  return VOS__BASE == LOWORD(d->m_pFixed->dwFileOS);
}

bool ZVersionInfo::IsTargetWin16() const
{
  ZAssert(d->m_pFixed);
  return VOS__WINDOWS16 == LOWORD(d->m_pFixed->dwFileOS);
}

bool ZVersionInfo::IsTargetPM16() const
{
  ZAssert(d->m_pFixed);
  return VOS__PM16 == LOWORD(d->m_pFixed->dwFileOS);
}

bool ZVersionInfo::IsTargetPM32() const
{
  ZAssert(d->m_pFixed);
  return VOS__PM32 == LOWORD(d->m_pFixed->dwFileOS);
}

bool ZVersionInfo::IsTargetWin32() const
{
  ZAssert(d->m_pFixed);
  return VOS__WINDOWS32 == LOWORD(d->m_pFixed->dwFileOS);
}

bool ZVersionInfo::IsTargetDOSWin16() const
{
  ZAssert(d->m_pFixed);
  return VOS_DOS_WINDOWS16 == d->m_pFixed->dwFileOS;
}

bool ZVersionInfo::IsTargetDOSWin32() const
{
  ZAssert(d->m_pFixed);
  return VOS_DOS_WINDOWS32 == d->m_pFixed->dwFileOS;
}

bool ZVersionInfo::IsTargetOS216PM16() const
{
  ZAssert(d->m_pFixed);
  return VOS_OS216_PM16 == d->m_pFixed->dwFileOS;
}

bool ZVersionInfo::IsTargetOS232PM32() const
{
  ZAssert(d->m_pFixed);
  return VOS_OS232_PM32 == d->m_pFixed->dwFileOS;
}

bool ZVersionInfo::IsTargetNTWin32() const
{
  ZAssert(d->m_pFixed);
  return VOS_NT_WINDOWS32 == d->m_pFixed->dwFileOS;
}


/////////////////////////////////////////////////////////////////////////////
// File Types

bool ZVersionInfo::IsApp() const
{
  ZAssert(d->m_pFixed);
  return VFT_APP == d->m_pFixed->dwFileType;
}

bool ZVersionInfo::IsDll() const
{
  ZAssert(d->m_pFixed);
  return VFT_DLL == d->m_pFixed->dwFileType;
}

bool ZVersionInfo::IsDriver() const
{
  ZAssert(d->m_pFixed);
  return VFT_DRV == d->m_pFixed->dwFileType;
}

bool ZVersionInfo::IsFont() const
{
  ZAssert(d->m_pFixed);
  return VFT_FONT == d->m_pFixed->dwFileType;
}

bool ZVersionInfo::IsVxd() const
{
  ZAssert(d->m_pFixed);
  return VFT_VXD == d->m_pFixed->dwFileType;
}

bool ZVersionInfo::IsStaticLib() const
{
  ZAssert(d->m_pFixed);
  return VFT_STATIC_LIB == d->m_pFixed->dwFileType;
}


/////////////////////////////////////////////////////////////////////////////
// Driver Types

bool ZVersionInfo::IsDriverUnknown() const
{
  ZAssert(d->m_pFixed);
  return IsDriver() && VFT2_UNKNOWN == d->m_pFixed->dwFileSubtype;
}

bool ZVersionInfo::IsDriverPrinter() const
{
  ZAssert(d->m_pFixed);
  return IsDriver() && VFT2_DRV_PRINTER == d->m_pFixed->dwFileSubtype;
}

bool ZVersionInfo::IsDriverKeyboard() const
{
  ZAssert(d->m_pFixed);
  return IsDriver() && VFT2_DRV_KEYBOARD == d->m_pFixed->dwFileSubtype;
}

bool ZVersionInfo::IsDriverLanguage() const
{
  ZAssert(d->m_pFixed);
  return IsDriver() && VFT2_DRV_LANGUAGE == d->m_pFixed->dwFileSubtype;
}

bool ZVersionInfo::IsDriverDisplay() const
{
  ZAssert(d->m_pFixed);
  return IsDriver() && VFT2_DRV_DISPLAY == d->m_pFixed->dwFileSubtype;
}

bool ZVersionInfo::IsDriverMouse() const
{
  ZAssert(d->m_pFixed);
  return IsDriver() && VFT2_DRV_MOUSE == d->m_pFixed->dwFileSubtype;
}

bool ZVersionInfo::IsDriverNetwork() const
{
  ZAssert(d->m_pFixed);
  return IsDriver() && VFT2_DRV_NETWORK == d->m_pFixed->dwFileSubtype;
}

bool ZVersionInfo::IsDriverSystem() const
{
  ZAssert(d->m_pFixed);
  return IsDriver() && VFT2_DRV_SYSTEM == d->m_pFixed->dwFileSubtype;
}

bool ZVersionInfo::IsDriverInstallable() const
{
  ZAssert(d->m_pFixed);
  return IsDriver() && VFT2_DRV_INSTALLABLE == d->m_pFixed->dwFileSubtype;
}

bool ZVersionInfo::IsDriverSound() const
{
  ZAssert(d->m_pFixed);
  return IsDriver() && VFT2_DRV_SOUND == d->m_pFixed->dwFileSubtype;
}

bool ZVersionInfo::IsDriverComm() const
{
  ZAssert(d->m_pFixed);
  return IsDriver() && VFT2_DRV_COMM == d->m_pFixed->dwFileSubtype;
}

bool ZVersionInfo::IsDriverInputMethod() const
{
  ZAssert(d->m_pFixed);
  return IsDriver() && VFT2_DRV_INPUTMETHOD == d->m_pFixed->dwFileSubtype;
}


/////////////////////////////////////////////////////////////////////////////
// Font Types

bool ZVersionInfo::IsFontRaster() const
{
  ZAssert(d->m_pFixed);
  return IsFont() && VFT2_FONT_RASTER == d->m_pFixed->dwFileSubtype;
}

bool ZVersionInfo::IsFontVector() const
{
  ZAssert(d->m_pFixed);
  return IsFont() && VFT2_FONT_VECTOR == d->m_pFixed->dwFileSubtype;
}

bool ZVersionInfo::IsFontTrueType() const
{
  ZAssert(d->m_pFixed);
  return IsFont() && VFT2_FONT_TRUETYPE == d->m_pFixed->dwFileSubtype;
}


/////////////////////////////////////////////////////////////////////////////
// String Values

void ZVersionInfo::SetLanguageID(LANGID wLangID)
{
  d->m_wLangID = wLangID;
}

LANGID ZVersionInfo::GetLanguageID() const
{
  return d->m_wLangID;
}

void ZVersionInfo::SetCodePage(uint16_t wCodePage)
{
  d->m_wCodePage = wCodePage;
}

uint16_t ZVersionInfo::GetCodePage() const
{
  return d->m_wCodePage;
}


/////////////////////////////////////////////////////////////////////////////
// Common String Values

ZString ZVersionInfo::GetCompanyName(bool* pbExists) const
{
  return GetStringValue(TEXT("CompanyName"), pbExists);
}

ZString ZVersionInfo::GetFileDescription(bool* pbExists) const
{
  return GetStringValue(TEXT("FileDescription"), pbExists);
}

ZString ZVersionInfo::GetFileVersion(bool* pbExists) const
{
  return GetStringValue(TEXT("FileVersion"), pbExists);
}

ZString ZVersionInfo::GetInternalName(bool* pbExists) const
{
  return GetStringValue(TEXT("InternalName"), pbExists);
}

ZString ZVersionInfo::GetLegalCopyright(bool* pbExists) const
{
  return GetStringValue(TEXT("LegalCopyright"), pbExists);
}

ZString ZVersionInfo::GetOriginalFilename(bool* pbExists) const
{
  return GetStringValue(TEXT("OriginalFilename"), pbExists);
}

ZString ZVersionInfo::GetProductName(bool* pbExists) const
{
  return GetStringValue(TEXT("ProductName"), pbExists);
}

ZString ZVersionInfo::GetProductVersion(bool* pbExists) const
{
  return GetStringValue(TEXT("ProductVersion"), pbExists);
}

ZString ZVersionInfo::GetSpecialBuild(bool* pbExists) const
{
  return GetStringValue(TEXT("SpecialBuild"), pbExists);
}

ZString ZVersionInfo::GetOLESelfRegister(bool* pbExists) const
{
  return GetStringValue(TEXT("OLESelfRegister"), pbExists);
}


/////////////////////////////////////////////////////////////////////////////
// Raw Data Access

const uint8_t* ZVersionInfo::GetVerInfo() const
{
  return d->m_pVerInfo;
}

uint32_t ZVersionInfo::GetVerInfoSize() const
{
  return d->m_cbVerInfo;
}

const uintptr_t ZVersionInfo::GetFixed() const
{
  return uintptr_t(d->m_pFixed);
}
