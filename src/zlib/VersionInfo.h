#pragma once

/////////////////////////////////////////////////////////////////////////////
// VersionInfo.h : Declaration of the ZVersionInfo class.
//

#include <windows.h>
#include <winver.h>
#include <tchar.h>

#include <memory>
#include "zassert.h"
#include "zstring.h"

#ifndef sizeofArray
  ///////////////////////////////////////////////////////////////////////////
  // Counts the number of elements in a fixed-length array.
  // Parameters:  x - The name of the array of which to compute the size.
  #define sizeofArray(x) (sizeof(x) / sizeof(x[0]))
#endif // !sizeofArray


/////////////////////////////////////////////////////////////////////////////
// ZVersionInfo
class ZVersionInfoPrivate;
class ZVersionInfo
{
// Construction / Destruction
public:
  ZVersionInfo(bool);
  ZVersionInfo(LPCTSTR szModule);
  ZVersionInfo(uintptr_t hinstance = 0);
  virtual ~ZVersionInfo();
  bool Load(LPCTSTR szModule);
  bool Load(uintptr_t hinstance = 0);
  bool Load(const void* pvVerInfo, UINT cbVerInfo);
  void Unload();

// Attributes
public:
  // File Version
  ZString GetFileVersionString   () const;
  uint16_t    GetFileVersionMSHigh   () const;
  uint16_t    GetFileVersionMSLow    () const;
  uint16_t    GetFileVersionLSHigh   () const;
  uint16_t    GetFileVersionLSLow    () const;
  uint16_t    GetFileBuildNumber     () const;
  // Product Version
  ZString GetProductVersionString() const;
  uint16_t    GetProductVersionMSHigh() const;
  uint16_t    GetProductVersionMSLow () const;
  uint16_t    GetProductVersionLSHigh() const;
  uint16_t    GetProductVersionLSLow () const;
  uint16_t    GetProductBuildNumber  () const;

  // File Flags
  bool IsDebug            () const;
  bool IsInfoInferred     () const;
  bool IsPatched          () const;
  bool IsPreRelease       () const;
  bool IsPrivateBuild     () const;
  bool IsSpecialBuild     () const;

  // Target Operating Systems
  bool IsTargetUnknown    () const;
  bool IsTargetDOS        () const;
  bool IsTargetOS216      () const;
  bool IsTargetOS232      () const;
  bool IsTargetNT         () const;
  bool IsTargetBase       () const;
  bool IsTargetWin16      () const;
  bool IsTargetPM16       () const;
  bool IsTargetPM32       () const;
  bool IsTargetWin32      () const;
  bool IsTargetDOSWin16   () const;
  bool IsTargetDOSWin32   () const;
  bool IsTargetOS216PM16  () const;
  bool IsTargetOS232PM32  () const;
  bool IsTargetNTWin32    () const;

  // File Types
  bool IsApp              () const;
  bool IsDll              () const;
  bool IsDriver           () const;
  bool IsFont             () const;
  bool IsVxd              () const;
  bool IsStaticLib        () const;

  // Driver Types
  bool IsDriverUnknown    () const;
  bool IsDriverPrinter    () const;
  bool IsDriverKeyboard   () const;
  bool IsDriverLanguage   () const;
  bool IsDriverDisplay    () const;
  bool IsDriverMouse      () const;
  bool IsDriverNetwork    () const;
  bool IsDriverSystem     () const;
  bool IsDriverInstallable() const;
  bool IsDriverSound      () const;
  bool IsDriverComm       () const;
  bool IsDriverInputMethod() const;

  // Font Types
  bool IsFontRaster       () const;
  bool IsFontVector       () const;
  bool IsFontTrueType     () const;

  // String Values
  void   SetLanguageID    (LANGID wLangID = -1);
  LANGID GetLanguageID    () const;
  void   SetCodePage      (uint16_t wCodePage = -1);
  uint16_t   GetCodePage      () const;
  ZString GetStringValue(LPCTSTR pszKey, bool* pbExists = NULL) const;

  // Common String Values
  ZString GetCompanyName     (bool* pbExists = NULL) const;
  ZString GetFileDescription (bool* pbExists = NULL) const;
  ZString GetFileVersion     (bool* pbExists = NULL) const;
  ZString GetInternalName    (bool* pbExists = NULL) const;
  ZString GetLegalCopyright  (bool* pbExists = NULL) const;
  ZString GetOriginalFilename(bool* pbExists = NULL) const;
  ZString GetProductName     (bool* pbExists = NULL) const;
  ZString GetProductVersion  (bool* pbExists = NULL) const;
  ZString GetSpecialBuild    (bool* pbExists = NULL) const;
  ZString GetOLESelfRegister (bool* pbExists = NULL) const;

  // Raw Data Access
  const uint8_t*             GetVerInfo    () const;
  uint32_t                   GetVerInfoSize() const;
  const uintptr_t GetFixed      () const;

// Data Members
protected:
  std::unique_ptr<ZVersionInfoPrivate> d;
};


/////////////////////////////////////////////////////////////////////////////
// Attributes


/////////////////////////////////////////////////////////////////////////////
// Versions

/////////////////////////////////////////////////////////////////////////////