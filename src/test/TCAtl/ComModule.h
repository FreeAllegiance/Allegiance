#ifndef __ComModule_h__
#define __ComModule_h__

#pragma once

/////////////////////////////////////////////////////////////////////////////
// ComModule.h : Declaration and implementation of the TCComModule class.
//

#include <TCLib.h>
#include <..\TCLib\AutoHandle.h>
#include "UserAcct.h"


/////////////////////////////////////////////////////////////////////////////
//
template <class T>
class ATL_NO_VTABLE TCComModule : public CComModule
{
// Types
protected:
  typedef TCComModule<T> TCComModuleBase;
  typedef HRESULT (T::*XOptionProc)(int, TCHAR*[], int*);
  struct  XCmdLineOption
  {
    LPCSTR      m_pszOption;
    LPCSTR      m_pszOptionDesc;
    XOptionProc m_pfn;
    int         m_cMinParams;
    int         m_cMaxParams;
    bool        m_bContinue;
    bool        m_bReportSuccess;
    bool        m_bAllowForWin9x;
    bool        m_bAllowForWinNT;
  };
  typedef const XCmdLineOption* PXCmdLineOption;

// Construction / Destruction
public:
  TCComModule();
  ~TCComModule();
  void Init(_ATL_OBJMAP_ENTRY* p, HINSTANCE h, const GUID* plibid = NULL);

// Attributes
public:
  static UINT GetShutdownEventName(LPTSTR pszEventName, UINT cch,
    bool bGlobalObject = true);
  void SetMessageHwnd(HWND hwnd);
  HWND GetMessageHwnd() const;

// Operations
public:
  HRESULT ParseCommandLine(LPCTSTR pszCmdLine = NULL);
  HRESULT ParseCommandLine(int argc, TCHAR* argv[]);
  HRESULT OnRegServer  (int argc, TCHAR* argv[], int* pnArgs);
  HRESULT OnUnregServer(int argc, TCHAR* argv[], int* pnArgs);
  HRESULT OnNoExit     (int argc, TCHAR* argv[], int* pnArgs);
  HRESULT OnExit       (int argc, TCHAR* argv[], int* pnArgs);
  HRESULT OnHelp       (int argc, TCHAR* argv[], int* pnArgs);
  HRESULT ReportError(HRESULT hr, LPCTSTR pszDesc);
  void    ReportSuccess(LPCTSTR pszDesc);
  int     Echo(LPCTSTR pszFmt, ...);
  int     Echo(UINT idFmt, ...);
  int     EchoV(LPCTSTR pszFmt, va_list argptr);
  void    EchoFlush();
  void    ClearOutputText();
  HRESULT Syntax();
  HRESULT RegisterClassObjects(DWORD dwFlags = REGCLS_MULTIPLEUSE,
    DWORD dwClsContext = CLSCTX_LOCAL_SERVER);
  HRESULT RevokeClassObjects();
  HRESULT StartMonitor();
  LONG Unlock() throw();
  HRESULT OpenAppIDRegKey(CRegKey& key, REGSAM samDesired = KEY_ALL_ACCESS);

// Overrides
public:
  HRESULT InitializeSecurity();
  HRESULT RegisterSecurity();
  HRESULT GetAccessSecurity(PSECURITY_DESCRIPTOR* psd, DWORD* pcb);
  HRESULT GetLaunchSecurity(PSECURITY_DESCRIPTOR* psd, DWORD* pcb);
  HRESULT GetAuthenticationLevel(DWORD* pdwAuthnLevel);
  void    DisplaySyntax();
  void    OutputText(LPCTSTR pszText);
  bool    UseMessageBox() {return true;}
  LPCTSTR GetRegInfo(UINT& idResource, bool& bRegTypeLib);
  UINT    GetCmdLineOptionMap(const XCmdLineOption** ppMap);
  DWORD   GetIdleShutdownTimeout() {return 5000;}
  DWORD   GetThreadPauseTimeout () {return 1000;}
  _ATL_REGMAP_ENTRY* GetRegMapEntries() {return NULL;}

// Implementation
public:
  static void parse_cmdline(_TSCHAR* cmdstart, _TSCHAR** argv,
    _TSCHAR* args, int* numargs, int* numchars);
  static HANDLE CreateShutdownEvent();
  static BOOL WINAPI ConsoleCtrlHandler(DWORD dwCtrlType);
  static DWORD WINAPI MonitorThunk(void* pv);
  void MonitorProc();
  //PXCmdLineOption GetCurrentCmdLineOption() const;

// Data Members
protected:
  UINT                   m_cchOutputText, m_cchOutputTextMax;
  TCCoTaskPtr<TCHAR*>    m_spszOutputText;
  DWORD                  m_dwThreadID;
  HANDLE                 m_hEventShutdown;
  TCPtr<PXCmdLineOption> m_spfnCmdLineOption;
  HWND                   m_hwndMessage;
  bool                   m_bActivity  : 1;
  bool                   m_bPermanent : 1;
};


/////////////////////////////////////////////////////////////////////////////
// Construction / Destruction

template <class T>
TCComModule<T>::TCComModule() :
  m_cchOutputText(0),
  m_cchOutputTextMax(0),
  m_dwThreadID(0),
  m_hEventShutdown(NULL),
  m_hwndMessage(NULL),
  m_bActivity(false),
  m_bPermanent(false)
{
}

template <class T>
TCComModule<T>::~TCComModule()
{
  EchoFlush();
}

template <class T>
void TCComModule<T>::Init(_ATL_OBJMAP_ENTRY* p, HINSTANCE h, const GUID* plibid)
{
  // Perform default processing
  CComModule::Init(p, h);

  // Get the current thread id
  m_dwThreadID = GetCurrentThreadId();

  // Trim the initial working set
  #ifndef _WINDLL
    if (IsWinNT())
      SetProcessWorkingSetSize(GetCurrentProcess(), -1, -1);
  #endif // !_WINDLL
}


/////////////////////////////////////////////////////////////////////////////
// Attributes

template <class T>
UINT TCComModule<T>::GetShutdownEventName(LPTSTR pszEventName, UINT cch,
  bool bGlobalObject)
{
  #ifdef _WINDLL
    ZAssert(!"defined(_WINDLL)");
    return E_UNEXPECTED;
  #endif // _WINDLL

  // Get the base file name of the module and make it lowercase
  TCHAR szFileName[_MAX_PATH], szBaseName[_MAX_FNAME];
  GetModuleFileName(NULL, szFileName, sizeofArray(szFileName));
  _tsplitpath(szFileName, NULL, NULL, szBaseName, NULL);
  _tcsupr(szBaseName);

  // Create the event name
  _tcscpy(szFileName, bGlobalObject ? TEXT("Global\\") : TEXT(""));
  _tcscat(szFileName, szBaseName);
  _tcscat(szFileName, TEXT("_TCComModule"));

  // Copy the name to the [out] parameter
  if (cch && pszEventName)
    lstrcpyn(pszEventName, szFileName, cch);

  // Return the length of the event name
  return _tcslen(szFileName);
}

template <class T>
inline void TCComModule<T>::SetMessageHwnd(HWND hwnd)
{
  m_hwndMessage = hwnd;
}

template <class T>
inline HWND TCComModule<T>::GetMessageHwnd() const
{
  return m_hwndMessage;
}


/////////////////////////////////////////////////////////////////////////////
// Operations

template <class T>
HRESULT TCComModule<T>::ParseCommandLine(LPCTSTR pszCmdLine)
{
  ///////////////////////////////////////////////////////////////////////////
  // This was blatantly ripped off from stdargv.c in the VC6 CRT library and
  // modified for use here.
  //

  // Define some local constants
  const TCHAR ZT = TCHAR('\0');
  const TCHAR SP = TCHAR(' ');
  const TCHAR DQ = TCHAR('\"');

  // Ensure that we have a command line
  if (!pszCmdLine || ZT == *pszCmdLine)
  {
    #ifdef _WINDLL
      ZAssert(pszCmdLine && ZT != *pszCmdLine);
      return E_UNEXPECTED;
    #else // _WINDLL
      pszCmdLine = GetCommandLine();
    #endif // _WINDLL
  }

  // Make a local copy of the command line
  int cb = (lstrlen(pszCmdLine) + 1) * sizeof(TCHAR);
  LPTSTR szCmdLine = (LPTSTR)_alloca(cb);
  lstrcpy(szCmdLine, pszCmdLine);

  // First find out how much space is needed to store args
  int numargs, numchars;
  parse_cmdline((_TSCHAR*)szCmdLine, NULL, NULL, &numargs, &numchars);

  // Allocate space for argv[] vector and strings
  cb = numargs * sizeof(LPTSTR) + numchars * sizeof(TCHAR);
  _TSCHAR* p = (_TSCHAR*)_alloca(cb);
  if (!p)
    return E_OUTOFMEMORY;

  // Store args and argv ptrs in just allocated block
  parse_cmdline((_TSCHAR*)szCmdLine,
    (_TSCHAR**)p, p + numargs * sizeof(_TSCHAR*), &numargs, &numchars);

  // Delegate to the other method
  int     argc = numargs - 1;
  TCHAR** argv = (TCHAR**)p;
  return ParseCommandLine(argc, argv);
}


template <class T>
HRESULT TCComModule<T>::ParseCommandLine(int argc, TCHAR* argv[])
{
  ZAssert(argc);
  ZAssert(argv);

  // Decrement to account for the module name
  --argc;
  ++argv;

  // Return if no arguments were specified
  if (argc < 1)
    return S_OK;

  // Get a pointer to the most-derived class
  T* pT = static_cast<T*>(this);

  // Continue while we have arguments
  bool bProcessed = false;
  bool bContinue = true;
  while (argc > 0 && bContinue)
  {
    // Option must begin with an option character
    if (TEXT('-') != *argv[0] && TEXT('/') != *argv[0])
      return Syntax();

    // Get the option name
    LPCTSTR pszOption = argv[0] + 1;

    // Advance argc and argv past the option name
    --argc;
    ++argv;

    // Loop thru map of options and determine which was specified
    HRESULT hr;
    int     nEaten;
    const XCmdLineOption* pEntries = NULL;
    UINT cEntries = pT->GetCmdLineOptionMap(&pEntries);
    UINT i;
	for (i = 0; i < cEntries; ++i)
    {
      if (0 == lstrcmpi(pszOption, pEntries[i].m_pszOption))
      {
        // Ignore if option not valid for current platform
        if (!pEntries[i].m_bAllowForWin9x && IsWin9x())
          continue;
        if (!pEntries[i].m_bAllowForWinNT && IsWinNT())
          continue;

        // Validate the specified number of option parameters
        if (pEntries[i].m_cMinParams >= 0)
        {
          if (argc < pEntries[i].m_cMinParams)
          {
            Echo(TEXT("Not enough parameters specified for \"%s\" option.\n"),
              pEntries[i].m_pszOption);
            return Syntax();
          }
          if (pEntries[i].m_cMaxParams >= 0 && argc > pEntries[i].m_cMaxParams)
          {
            Echo(TEXT("Too many parameters specified for \"%s\" option.\n"),
              pEntries[i].m_pszOption);
            return Syntax();
          }
        }

        // Process the option
        if (pEntries[i].m_pfn)
        {
          // Call the option handler function
          nEaten = 0;
          m_spfnCmdLineOption = pEntries + i;
          hr = (pT->*pEntries[i].m_pfn)(argc, argv, &nEaten);
          m_spfnCmdLineOption = NULL;

          // Report failure or success
          if (FAILED(hr))
            return ReportError(hr, pEntries[i].m_pszOptionDesc);
          if (pEntries[i].m_bReportSuccess && S_FALSE != hr)
            ReportSuccess(pEntries[i].m_pszOptionDesc);

          // Keep track that we successfully processed an option
          bProcessed = true;

          // Advance argc and argv past the parsed options
          argc -= nEaten;
          argv += nEaten;
        }
        bContinue = pEntries[i].m_bContinue;
        break;
      }
    }

    // Display the "invalid option" message
    if (i >= cEntries)
    {
      Echo(TEXT("\"%s\" is an invalid option.\n"), pszOption);
      return Syntax();
    }
  }

  // Return code based on bContinue flag
  return bContinue ? S_OK : S_FALSE;
}

template <class T>
HRESULT TCComModule<T>::OnRegServer(int argc, TCHAR* argv[], int* pnArgs)
{
  // Eat all the parameters
  *pnArgs = argc;

  // Prepare to manipulate the specified user account
  bool bInteractiveUser = false;
  TCUserAccount acct;
  if (IsWinNT())
  {
    bInteractiveUser = 0 == _tcsicmp(argv[0], TEXT("Interactive User"));
    if (!bInteractiveUser)
      RETURN_FAILED(acct.Init(argv[0]))
    else if (argc > 1)
    {
      Echo("Password not allowed for \"Interactive User\".\n");
      Syntax();
      return S_FALSE;
    }
  }

  // Get a pointer to the most-derived class
  T* pT = static_cast<T*>(this);

  // Get registration info from the most-derived class
  UINT idr = 0;
  bool bRegTypeLib = false;
  LPCTSTR pszAppID = pT->GetRegInfo(idr, bRegTypeLib);

  // Get registrar map entries from the most-derived class
  _ATL_REGMAP_ENTRY* pRegMap = pT->GetRegMapEntries();

  // Register our AppID
  if (idr)
    RETURN_FAILED(_Module.UpdateRegistryFromResource(idr, true, pRegMap));

  // Register our components
  #ifndef _WINDLL
    RETURN_FAILED(_Module.RegisterServer(bRegTypeLib));
  #else // !_WINDLL
    RETURN_FAILED(DllRegisterServer());
  #endif // !_WINDLL

  // Register security options
  RETURN_FAILED(pT->RegisterSecurity());

  // Ensure that the specified user has the "Logon as a batch job" right
  if (IsWinNT())
  {
    if (bInteractiveUser)
    {
      // Set the Interactive User as the "RunAs" value for the AppID
      RETURN_FAILED(acct.SetRunAsInteractiveUser(pszAppID));
    }
    else
    {
      // Ensure that the specified user has the "Logon as a batch job" right
      HRESULT hr = acct.HasRight(SE_BATCH_LOGON_NAME);
      if (S_OK != hr)
      {
        RETURN_FAILED(acct.SetRight(SE_BATCH_LOGON_NAME));
        Echo(TEXT("The account %ls\\%ls has been granted the Logon As A Batch Job right."),
          acct.GetDomainNameW(), acct.GetUserNameW());
        EchoFlush();
      }

      // Set the specified user account as the "RunAs" value for the AppID
      LPCTSTR pszPassword = (2 == argc) ? argv[1] : TEXT("");
      RETURN_FAILED(acct.SetRunAsUser(pszAppID, pszPassword));
    }
  }

  // Indicate success
  return S_OK;
}

template <class T>
HRESULT TCComModule<T>::OnUnregServer(int argc, TCHAR* argv[], int* pnArgs)
{
  #ifndef _WINDLL

    // Get a pointer to the most-derived class
    T* pT = static_cast<T*>(this);

    // Get registration info from the most-derived class
    UINT idr = 0;
    bool bRegTypeLib;
    pT->GetRegInfo(idr, bRegTypeLib);

    // Unregister the AppID
    if (idr)
      _Module.UpdateRegistryFromResource(idr, FALSE);

    // Unregister the components
    _Module.UnregisterServer(TRUE);

    // Indicate success
    return S_OK;

  #else // !_WINDLL

    // Just use the exported function
    return DllUnregisterServer();

  #endif // !_WINDLL
}

template <class T>
HRESULT TCComModule<T>::OnNoExit(int argc, TCHAR* argv[], int* pnArgs)
{
  m_bPermanent = true;
  return S_OK;
}

template <class T>
HRESULT TCComModule<T>::OnExit(int argc, TCHAR* argv[], int* pnArgs)
{
  // Open the shutdown event
  TCHandle hevt(CreateShutdownEvent());
  if (hevt.IsNull())
    return HRESULT_FROM_WIN32(GetLastError());
  if (GetLastError() != ERROR_ALREADY_EXISTS)
  {
    Echo("No server is currently running.");
    return S_FALSE;
  }

  // Signal the shutdown event
  SetEvent(hevt);

  // Display status
  Echo("Server has been signaled to exit.");
  EchoFlush();

  // Indicate success
  return S_OK;
}

template <class T>
HRESULT TCComModule<T>::OnHelp(int argc, TCHAR* argv[], int* pnArgs)
{
  Syntax();
  return S_OK;
}

template <class T>
HRESULT TCComModule<T>::Syntax()
{
  // Call on the most-derived class to display its syntax
  static_cast<T*>(this)->DisplaySyntax();
  return S_FALSE;
}

template <class T>
HRESULT TCComModule<T>::ReportError(HRESULT hr, LPCTSTR pszDesc)
{
  // Declare an array of modules in which to find the system message
  TCHAR* szModules[] = {TEXT("NetMsg.dll"), NULL};

  // Loop through each module in the array
  LPTSTR pszBuffer = NULL;
  DWORD dwLangID = MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT);
  DWORD dwFormatFlags = FORMAT_MESSAGE_ALLOCATE_BUFFER |
    FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_FROM_SYSTEM;
  for (int i = 0; i < sizeofArray(szModules); ++i)
  {
    // Attempt to load the library for messages
    TCHinstance shInst = (szModules[i] && *szModules[i]) ?
      LoadLibraryEx(szModules[i], NULL, LOAD_LIBRARY_AS_DATAFILE) : NULL;

    // Determine the format flags
    DWORD dwFlags = dwFormatFlags |
      (shInst.IsNull() ? 0 : FORMAT_MESSAGE_FROM_HMODULE);

    // Allow the system to format the message
    DWORD cch = FormatMessage(dwFlags, shInst.GetHandle(), hr, dwLangID,
      (LPTSTR)&pszBuffer, 0, NULL);
    if (cch)
      break;
  }

  // Echo the base message first
  static_cast<T*>(this)->Echo(TEXT("%s failed with return code 0x%08X.\n"),
    pszDesc, hr);
  if (pszBuffer)
  {
    static_cast<T*>(this)->Echo(TEXT("    %s\n"), pszBuffer);
    LocalFree(pszBuffer);
  }

  // Return the specified result
  return hr;
}

template <class T>
void TCComModule<T>::ReportSuccess(LPCTSTR pszDesc)
{
  if (pszDesc && TEXT('\0') != *pszDesc)
    static_cast<T*>(this)->Echo(TEXT("%s succeeded.\n"), pszDesc);
}

template <class T>
inline int TCComModule<T>::Echo(LPCTSTR pszFmt, ...)
{
  va_list argptr;
  va_start(argptr, pszFmt);
  int cch = static_cast<T*>(this)->EchoV(pszFmt, argptr);
  va_end(argptr);
  return cch;
}

template <class T>
inline int TCComModule<T>::Echo(UINT idFmt, ...)
{
  // Load the specified string resource
  TCHAR szFmt[_MAX_PATH * 4];
  LoadString(GetResourceInstance(), idFmt, szFmt, sizeofArray(szFmt));

  // Get a pointer to the variable argument list and delegate
  va_list argptr;
  va_start(argptr, idFmt);
  int cch = static_cast<T*>(this)->EchoV(szFmt, argptr);
  va_end(argptr);
  return cch;
}

template <class T>
int TCComModule<T>::EchoV(LPCTSTR pszFmt, va_list argptr)
{
  // Format the text
  TCHAR szText[_MAX_PATH * 6];
  int cch = _vsntprintf(szText, sizeofArray(szText), pszFmt, argptr);
  if (-1 == cch)
    return cch;

  // Ensure that the buffer is big enough to concatenate the string
  int cchTotal = cch + 1;
  int cchRemain = m_cchOutputTextMax - m_cchOutputText;
  if (cchRemain < cchTotal || m_spszOutputText.IsNull())
  {
    // Allocate or reallocate enough memory to concetenate the string
    int cchNeeded        = cchTotal - cchRemain;
    int cBlocksNeeded    = 1 + (cchNeeded / _MAX_PATH);
    cchNeeded            = cBlocksNeeded * _MAX_PATH;
    m_cchOutputTextMax  += cchNeeded;
    int cbAlloc          = m_cchOutputTextMax * sizeof(TCHAR);
    LPTSTR pszOld        = m_spszOutputText.Detach();
    m_spszOutputText     = (LPTSTR)CoTaskMemRealloc(pszOld, cbAlloc);
    if (m_spszOutputText.IsNull())
    {
      m_spszOutputText    = pszOld;
      m_cchOutputTextMax -= cchNeeded;
      ZAssert(!"CoTaskMemRealloc returned NULL");
      return -1;
    }
    if (!m_cchOutputText)
      *m_spszOutputText = TEXT('\0');
  }

  // Concatenate the string to the buffer
  _tcscat(m_spszOutputText, szText);
  m_cchOutputText += cchTotal;

  // Return the number of characters formatted (excluding terminating NULL)
  return cch;
}

template <class T>
inline void TCComModule<T>::EchoFlush()
{
  if (!m_spszOutputText.IsNull())
  {
    static_cast<T*>(this)->OutputText(m_spszOutputText);
    static_cast<T*>(this)->ClearOutputText();
  }
}

template <class T>
void TCComModule<T>::ClearOutputText()
{
  // Release the text pointer
  m_spszOutputText = NULL;
  m_spszOutputText = NULL;
  m_cchOutputText  = m_cchOutputTextMax = 0;
}

template <class T>
HRESULT TCComModule<T>::RegisterClassObjects(DWORD dwFlags, DWORD dwClsContext)
{
  #ifdef _WINDLL
    ZAssert(!"defined(_WINDLL)");
    return E_UNEXPECTED;
  #endif // _WINDLL

  // Start the monitor thread
  _Module.StartMonitor();

  // Register our class (factory) objects
  #if (_WIN32_WINNT >= 0x0400 || defined(_WIN32_DCOM)) & defined(_ATL_FREE_THREADED)
    RETURN_FAILED(CComModule::RegisterClassObjects(dwClsContext,
      dwFlags | REGCLS_SUSPENDED));
    RETURN_FAILED(CoResumeClassObjects());
  #else
    RETURN_FAILED(CComModule::RegisterClassObjects(dwFlags, dwClsContext));
  #endif
  return S_OK;
}

template <class T>
HRESULT TCComModule<T>::RevokeClassObjects()
{
  #ifdef _WINDLL
    ZAssert(!"defined(_WINDLL)");
    return E_UNEXPECTED;
  #endif // _WINDLL

  // Perform default processing
  HRESULT hr = CComModule::RevokeClassObjects();

  // Wait for any threads to finish
  Sleep(static_cast<T*>(this)->GetThreadPauseTimeout());

  // Indicate success
  return S_OK;
}

template <class T>
HRESULT TCComModule<T>::StartMonitor()
{
  #ifdef _WINDLL
    ZAssert(!"defined(_WINDLL)");
    return E_UNEXPECTED;
  #endif // _WINDLL

  // Create the shutdown event
  m_hEventShutdown = CreateShutdownEvent();
  if (!m_hEventShutdown)
    return HRESULT_FROM_WIN32(GetLastError());

  // Create the monitor thread
  DWORD dwThreadID;
  HANDLE hth = CreateThread(NULL, 0, MonitorThunk, static_cast<T*>(this),
    0, &dwThreadID);
  return hth ? S_OK : HRESULT_FROM_WIN32(GetLastError());
}

template <class T>
LONG TCComModule<T>::Unlock() throw()
{
  // Perform default processing
  LONG l = CComModule::Unlock();
//  _TRACE2("TCComModule<%hs>::Unlock(): l = %d\n", TCTypeName(T), l);

  #ifndef _WINDLL
    // Shutdown the app on last unlock
    if (!m_bPermanent && 0 == l)
    {
      m_bActivity = true;
      SetEvent(m_hEventShutdown); // tell monitor that we transitioned to zero
    }
  #endif // !_WINDLL

  // Return the result of the default processing
  return l;
}


template <class T>
HRESULT TCComModule<T>::OpenAppIDRegKey(CRegKey& key, REGSAM samDesired)
{
  // Get the AppID string (may be a CLSID or ProgID)
  USES_CONVERSION;
  UINT idResource;
  bool bRegTypeLib;
  WCHAR szAppID[_MAX_PATH];
  wcscpy(szAppID, T2CW(_Module.GetRegInfo(idResource, bRegTypeLib)));

  // Resolve and open the registry key of the AppID string
  HKEY hkey = NULL;
  RETURN_FAILED(TCUserAccount::ResolveAppID(szAppID, &hkey, samDesired));
  key.Close();
  key.Attach(hkey);
  return S_OK;
}


/////////////////////////////////////////////////////////////////////////////
// Overrides

template <class T>
HRESULT TCComModule<T>::InitializeSecurity()
{
  // Default is to use the AppID registry settings
  return S_OK;
}


template <class T>
HRESULT TCComModule<T>::RegisterSecurity()
{
  // Get a pointer to the most-derived class
  T* pT = static_cast<T*>(this);

  // Get the AppID string (may be a CLSID or ProgID)
  USES_CONVERSION;
  UINT idResource;
  bool bRegTypeLib;
  WCHAR szAppID[_MAX_PATH];
  wcscpy(szAppID, T2CW(pT->GetRegInfo(idResource, bRegTypeLib)));

  // Resolve and open the registry key of the AppID string
  HKEY hkey = NULL;
  RETURN_FAILED(TCUserAccount::ResolveAppID(szAppID, &hkey));
  CRegKey key;
  key.Attach(hkey);

  // Allow the most-derived class to specify access permissions
  HRESULT             hr;
  TCArrayPtr<BYTE*> spSD;
  DWORD             cbSD;
  RETURN_FAILED(hr = pT->GetAccessSecurity((PSECURITY_DESCRIPTOR*)&spSD, &cbSD));
  if (!spSD.IsNull() && (S_FALSE != hr))
  {
    // Write the self-relative security descriptor to the registry value
    LONG lr = RegSetValueEx(key, TEXT("AccessPermission"), 0, REG_BINARY,
      spSD, cbSD);
    if (ERROR_SUCCESS != lr)
      return HRESULT_FROM_WIN32(lr);
  }

  // Allow the most-derived class to specify launch permissions
  RETURN_FAILED(hr = pT->GetLaunchSecurity((PSECURITY_DESCRIPTOR*)&spSD, &cbSD));
  if (!spSD.IsNull() && (S_FALSE != hr))
  {
    // Write the self-relative security descriptor to the registry value
    LONG lr = RegSetValueEx(key, TEXT("LaunchPermission"), 0, REG_BINARY,
      spSD, cbSD);
    if (ERROR_SUCCESS != lr)
      return HRESULT_FROM_WIN32(lr);
  }

  // Allow the most-derived class to specify authentication level
  DWORD dwAuthnLevel = 0;
  RETURN_FAILED(hr = pT->GetAuthenticationLevel(&dwAuthnLevel));
  if (RPC_C_AUTHN_LEVEL_NONE <= dwAuthnLevel &&
    dwAuthnLevel <= RPC_C_AUTHN_LEVEL_PKT_PRIVACY && (S_FALSE != hr))
  {
    // Write the authentication level
    LONG lr = key.SetValue(dwAuthnLevel, TEXT("AuthenticationLevel"));
    if (ERROR_SUCCESS != lr)
      return HRESULT_FROM_WIN32(lr);
  }

  // Indicate success
  return S_OK;
}


template <class T>
HRESULT TCComModule<T>::GetAccessSecurity(PSECURITY_DESCRIPTOR* psd,
  DWORD* pcb)
{
  // Initialize the [out] parameter
  if (pcb)
    *pcb = 0;

  // Returning S_FALSE will avoid writing the AccessPermission value
  if (IsWin9x())
    return S_FALSE;

  // Create a security descriptor, owned by the thread owner
  CSecurityDescriptor sd;
  sd.InitializeFromThreadToken();

  // Define a table of Authority/SubAuthorities to be added to the DACL
  static struct
  {
    SID_IDENTIFIER_AUTHORITY m_auth;
    DWORD m_authSub1, m_authSub2;
  } auths[] =
  {
    {SECURITY_NT_AUTHORITY       , SECURITY_LOCAL_SYSTEM_RID    },
    {SECURITY_NT_AUTHORITY       , SECURITY_INTERACTIVE_RID     },
    {SECURITY_NT_AUTHORITY       , SECURITY_BUILTIN_DOMAIN_RID,
                                   DOMAIN_ALIAS_RID_ADMINS      },
    {SECURITY_WORLD_SID_AUTHORITY, SECURITY_WORLD_RID           },
  };

  // Compute the number of bytes taken by an ACE, in addtion to the SID
  const DWORD cbACE = sizeof(ACCESS_ALLOWED_ACE) - sizeof(DWORD);

  // Compute the number of bytes needed for the DACL, in addition to the ACEs
  DWORD cbACL = sizeof(ACL);

  // Create a SID for each Authority/SubAuthorities in the table
  TCSid psids[sizeofArray(auths)];
  UINT i;
  for (i = 0; i < sizeofArray(auths); ++i)
  {
    // Create a SID for the next Authority/SubAuthorities in the table
    SID_IDENTIFIER_AUTHORITY auth = auths[i].m_auth;
    if (!AllocateAndInitializeSid(&auth, auths[i].m_authSub2 ? 2 : 1,
      auths[i].m_authSub1, auths[i].m_authSub2, 0, 0, 0, 0, 0, 0, &psids[i]))
        return HRESULT_FROM_WIN32(GetLastError());

    // Add the size of an ACE for this SID to the DACL size
    cbACL += cbACE + GetLengthSid(psids[i]);
  }

  // Free the current DACL and allocate a new one
  if (sd.m_pDACL)
    free(sd.m_pDACL);
  sd.m_pDACL = (ACL*)malloc(cbACL);

  // Initialize the DACL
  if (!InitializeAcl(sd.m_pDACL, cbACL, ACL_REVISION))
    return HRESULT_FROM_WIN32(GetLastError());

  // Add the SIDs to the security descriptor
  for (i = 0; i < sizeofArray(psids); ++i)
  	if (!AddAccessAllowedAce(sd.m_pDACL, ACL_REVISION,
      COM_RIGHTS_EXECUTE, psids[i]))
        return HRESULT_FROM_WIN32(GetLastError());

  // Set the new DACL of the security descriptor
  if (!SetSecurityDescriptorDacl(sd.m_pSD, true, sd.m_pDACL, false))
    return HRESULT_FROM_WIN32(GetLastError());

  // Get the size of the security descriptor, represented self-relatively
  DWORD cbSD = 0;
  MakeSelfRelativeSD(sd, NULL, &cbSD);

  // Allocate a self-relative security descriptor (delete'd by the caller)
  PSECURITY_DESCRIPTOR psdTemp = new BYTE[cbSD];
  if (!psdTemp)
    return E_OUTOFMEMORY;
  ZeroMemory(psdTemp, cbSD);
  if (MakeSelfRelativeSD(sd, psdTemp, &cbSD))
  {
    *psd = psdTemp;
    if (pcb)
      *pcb = cbSD;
    return S_OK;
  }

  // Free the allocated self-relative security descriptor
  DWORD dwLastError = GetLastError();
  delete psdTemp;
  return HRESULT_FROM_WIN32(dwLastError);
}


template <class T>
HRESULT TCComModule<T>::GetLaunchSecurity(PSECURITY_DESCRIPTOR* psd,
  DWORD* pcb)
{
  // Initialize the [out] parameter
  if (pcb)
    *pcb = 0;

  // Returning S_FALSE will avoid writing the LaunchPermission value
  if (IsWin9x())
    return S_FALSE;

  // Use the access permissions of this class (not the most-derived class)
  return GetAccessSecurity(psd, pcb);
}


template <class T>
HRESULT TCComModule<T>::GetAuthenticationLevel(DWORD* pdwAuthnLevel)
{
  // Returning S_FALSE will avoid writing the AuthenticationLevel value
  return S_FALSE;
}


template <class T>
UINT TCComModule<T>::GetCmdLineOptionMap(const XCmdLineOption** ppMap)
{
  if (ppMap)
    *ppMap = NULL;
  return 0;
}


template <class T>
void TCComModule<T>::OutputText(LPCTSTR pszText)
{
  // Define some local constants
  const TCHAR ZT = TEXT('\0');
  const TCHAR CR = TEXT('\r');
  const TCHAR LF = TEXT('\n');

  // Do nothing if the specified string is empty
  if (!pszText || ZT == *pszText)
    return;

  // Output the text as-is to the debug monitor
  OutputDebugString(pszText);
  OutputDebugString(TEXT("\n"));

  // Write the text to StdOut, expanding LF's into CRLF's
  HANDLE hStdOut = ::GetStdHandle(STD_OUTPUT_HANDLE);
  DWORD cbWritten = sizeof(TCHAR);
  for (LPCTSTR psz = pszText, pszPrev = NULL; ZT != *psz; pszPrev = psz++)
  {
    if (LF == *psz && (!pszPrev || CR != *pszPrev))
      ::WriteFile(hStdOut, &CR, sizeof(TCHAR), &cbWritten, NULL);
    ::WriteFile(hStdOut, psz, sizeof(TCHAR), &cbWritten, NULL);
    if (sizeof(TCHAR) != cbWritten)
    {
      // Get the module name
      TCHAR szModule[_MAX_PATH], szFname[_MAX_FNAME], szExt[_MAX_EXT];
      ::GetModuleFileName(GetModuleInstance(), szModule,
        sizeofArray(szModule));
      _tsplitpath(szModule, NULL, NULL, szFname, szExt);
      _tmakepath(szModule, NULL, NULL, szFname, szExt);

      // Use MessageBox to output the specified text
      T* pT = static_cast<T*>(this);
      if (pT->UseMessageBox())
      {
        ::MessageBox(GetMessageHwnd(), pszText, szModule,
          MB_OK | MB_ICONINFORMATION);
      }
      return;
    }
  }
  // Write out a trailing CRLF for good measure
  ::WriteFile(hStdOut, &CR, sizeof(TCHAR), &cbWritten, NULL);
  ::WriteFile(hStdOut, &LF, sizeof(TCHAR), &cbWritten, NULL);
//  ::CloseHandle(hStdOut);
}


/////////////////////////////////////////////////////////////////////////////
// Implementation

/////////////////////////////////////////////////////////////////////////////
// Description: Parses the command line and sets up the argv[] array.
//
// Remarks: On entry, cmdstart should point to the command line, argv should
// point to memory for the argv array, args points to memory to place the
// text of the arguments. If these are NULL, then no storing (only coujting)
// is done.
//
// On exit, *numargs has the number of arguments (plus one for a final NULL
// argument), and *numchars has the number of bytes used in the buffer
// pointed to by args.
//
// Parameters:
//   cmdstart - pointer to command line of the form <progname><sp><args><nul>
//   argv - where to build argv array; NULL means don't build array
//   args - where to place argument text; NULL means don't store text
//
// Note: This was blatantly ripped off from stdargv.c in the VC6 CRT library
// and modified for use here. I didn't reformat it or change variable names
// so as to not introduce any new bugs.
//
template <class T>
void TCComModule<T>::parse_cmdline(_TSCHAR* cmdstart, _TSCHAR** argv,
  _TSCHAR* args, int* numargs, int* numchars)
{
  // Define some local constants
  const _TSCHAR NULCHAR    = _T('\0');
  const _TSCHAR SPACECHAR  = _T(' ');
  const _TSCHAR TABCHAR    = _T('\t');
  const _TSCHAR DQUOTECHAR = _T('\"');
  const _TSCHAR SLASHCHAR  = _T('\\');

        _TSCHAR *p;
        _TUCHAR c;
        int inquote;                    /* 1 = inside quotes */
        int copychar;                   /* 1 = copy char to *args */
        unsigned numslash;              /* num of backslashes seen */

        *numchars = 0;
        *numargs = 1;                   /* the program name at least */

        /* first scan the program name, copy it, and count the bytes */
        p = cmdstart;
        if (argv)
            *argv++ = args;

        /* A quoted program name is handled here. The handling is much
           simpler than for other arguments. Basically, whatever lies
           between the leading double-quote and next one, or a terminal null
           character is simply accepted. Fancier handling is not required
           because the program name must be a legal NTFS/HPFS file name.
           Note that the double-quote characters are not copied, nor do they
           contribute to numchars. */
        if ( *p == DQUOTECHAR ) {
            /* scan from just past the first double-quote through the next
               double-quote, or up to a null, whichever comes first */
            while ( (*(++p) != DQUOTECHAR) && (*p != NULCHAR) ) {

#ifdef _MBCS
                if (_ismbblead(*p)) {
                    ++*numchars;
                    if ( args )
                        *args++ = *p++;
                }
#endif  /* _MBCS */
                ++*numchars;
                if ( args )
                    *args++ = *p;
            }
            /* append the terminating null */
            ++*numchars;
            if ( args )
                *args++ = NULCHAR;

            /* if we stopped on a double-quote (usual case), skip over it */
            if ( *p == DQUOTECHAR )
                p++;
        }
        else {
            /* Not a quoted program name */
            do {
                ++*numchars;
                if (args)
                    *args++ = *p;

                c = (_TUCHAR) *p++;
#ifdef _MBCS
                if (_ismbblead(c)) {
                    ++*numchars;
                    if (args)
                        *args++ = *p;   /* copy 2nd byte too */
                    p++;  /* skip over trail byte */
                }
#endif  /* _MBCS */

            } while ( c != SPACECHAR && c != NULCHAR && c != TABCHAR );

            if ( c == NULCHAR ) {
                p--;
            } else {
                if (args)
                    *(args-1) = NULCHAR;
            }
        }

        inquote = 0;

        /* loop on each argument */
        for(;;) {

            if ( *p ) {
                while (*p == SPACECHAR || *p == TABCHAR)
                    ++p;
            }

            if (*p == NULCHAR)
                break;              /* end of args */

            /* scan an argument */
            if (argv)
                *argv++ = args;     /* store ptr to arg */
            ++*numargs;

        /* loop through scanning one argument */
        for (;;) {
            copychar = 1;
            /* Rules: 2N backslashes + " ==> N backslashes and begin/end quote
               2N+1 backslashes + " ==> N backslashes + literal "
               N backslashes ==> N backslashes */
            numslash = 0;
            while (*p == SLASHCHAR) {
                /* count number of backslashes for use below */
                ++p;
                ++numslash;
            }
            if (*p == DQUOTECHAR) {
                /* if 2N backslashes before, start/end quote, otherwise
                    copy literally */
                if (numslash % 2 == 0) {
                    if (inquote) {
                        if (p[1] == DQUOTECHAR)
                            p++;    /* Double quote inside quoted string */
                        else        /* skip first quote char and copy second */
                            copychar = 0;
                    } else
                        copychar = 0;       /* don't copy quote */

                    inquote = !inquote;
                }
                numslash /= 2;          /* divide numslash by two */
            }

            /* copy slashes */
            while (numslash--) {
                if (args)
                    *args++ = SLASHCHAR;
                ++*numchars;
            }

            /* if at end of arg, break loop */
            if (*p == NULCHAR || (!inquote && (*p == SPACECHAR || *p == TABCHAR)))
                break;

            /* copy character into argument */
#ifdef _MBCS
            if (copychar) {
                if (args) {
                    if (_ismbblead(*p)) {
                        *args++ = *p++;
                        ++*numchars;
                    }
                    *args++ = *p;
                } else {
                    if (_ismbblead(*p)) {
                        ++p;
                        ++*numchars;
                    }
                }
                ++*numchars;
            }
            ++p;
#else  /* _MBCS */
            if (copychar) {
                if (args)
                    *args++ = *p;
                ++*numchars;
            }
            ++p;
#endif  /* _MBCS */
            }

            /* null-terminate the argument */

            if (args)
                *args++ = NULCHAR;          /* terminate string */
            ++*numchars;
        }

        /* We put one last argument in -- a null ptr */
        if (argv)
            *argv++ = NULL;
        ++*numargs;
}

template <class T>
HANDLE TCComModule<T>::CreateShutdownEvent()
{
  #ifdef _WINDLL
    ZAssert(!"defined(_WINDLL)");
    return E_UNEXPECTED;
  #endif // _WINDLL

  // Set security under WinNT
  SECURITY_ATTRIBUTES* psa = NULL;
  if (IsWinNT())
  {
    // Create a NULL dacl to give "everyone" access
    SECURITY_DESCRIPTOR sd;
    SECURITY_ATTRIBUTES sa = {sizeof(sa), &sd, false};
    InitializeSecurityDescriptor(&sd, SECURITY_DESCRIPTOR_REVISION);
    SetSecurityDescriptorDacl(&sd, true, NULL, FALSE);
    psa = &sa;
  }

  // Get the name of the shutdown event
  TCHAR szEventName[_MAX_PATH];
  GetShutdownEventName(szEventName, sizeofArray(szEventName));

  // Create the shutdown event
  HANDLE hevt = CreateEvent(psa, false, false, szEventName);
  if (!hevt)
  {
    GetShutdownEventName(szEventName, sizeofArray(szEventName), false);
    hevt = CreateEvent(psa, false, false, szEventName);
  }
  return hevt;
}


template <class T>
BOOL WINAPI TCComModule<T>::ConsoleCtrlHandler(DWORD)
{
  TCHandle hevt(CreateShutdownEvent());
  SetEvent(hevt);
  _Module.Echo(TEXT("The server has been signaled to exit."));
  _Module.EchoFlush();
  return TRUE;
}


template <class T>
DWORD WINAPI TCComModule<T>::MonitorThunk(void* pv)
{
  // Delegate to the non-static method
  T* pT = reinterpret_cast<T*>(pv);
  pT->MonitorProc();
  return 0;
}

template <class T>
void TCComModule<T>::MonitorProc()
{
  DWORD dwTimeOut = static_cast<T*>(this)->GetIdleShutdownTimeout();
  while (true)
  {
    WaitForSingleObject(m_hEventShutdown, INFINITE);
    DWORD dwWait=0;
    do
    {
      m_bActivity = false;
      dwWait = WaitForSingleObject(m_hEventShutdown, dwTimeOut);
    } while (dwWait == WAIT_OBJECT_0);
    // timed out
    if (!m_bActivity && m_nLockCnt == 0) // if no activity let's really bail
    {
      #if (_WIN32_WINNT >= 0x0400 || defined(_WIN32_DCOM)) & defined(_ATL_FREE_THREADED)
        CoSuspendClassObjects();
        if (!m_bActivity && m_nLockCnt == 0)
      #endif
          break;
    }
  }
  CloseHandle(m_hEventShutdown);
  PostThreadMessage(m_dwThreadID, WM_QUIT, 0, 0);
}

//template <class T>
//inline TCComModule<T>::PXCmdLineOption TCComModule<T>::GetCurrentCmdLineOption() const
//{
//  return m_spfnCmdLineOption;
//}


/////////////////////////////////////////////////////////////////////////////
// Command Line Option Map Macros

#define BEGIN_CMDLINE_OPTION_MAP()                                          \
  static UINT GetCmdLineOptionMap(const XCmdLineOption** ppMap)             \
  {                                                                         \
    static XCmdLineOption map[] =                                           \
    {                                                                       

#define CMDLINE_OPTION_ENTRY_EX(sz, szDesc, fn, cMin, cMax, cont, reportSuccess, win9x, winNT) \
      {TEXT(sz), TEXT(szDesc), fn, cMin, cMax, cont, reportSuccess, win9x, winNT},             

#define CMDLINE_OPTION_ENTRY(sz, szDesc, fn, cont, reportSuccess)           \
      CMDLINE_OPTION_ENTRY_EX(sz, szDesc, fn, -1, 0, cont, reportSuccess,   \
        true, true)                                                         

#define CMDLINE_OPTION_Automation()                                         \
      CMDLINE_OPTION_ENTRY("Automation", NULL, NULL, true, false)           \
      CMDLINE_OPTION_ENTRY("Embedding", NULL, NULL, true, false)            

#define CMDLINE_OPTION_RegServer9x(T)                                        \
      CMDLINE_OPTION_ENTRY_EX("RegServer", "Server Registration",           \
        &TCComModule<T>::OnRegServer, 0, 0, false, true, true, false)                        

#define CMDLINE_OPTION_RegServerNT(T)                                        \
      CMDLINE_OPTION_ENTRY_EX("RegServer", "Server Registration",           \
        &TCComModule<T>::OnRegServer, 1, 2, false, true, false, true)                        

#define CMDLINE_OPTION_UnregServer(T)                                        \
      CMDLINE_OPTION_ENTRY_EX("UnregServer", "Server Unregistration",       \
        &TCComModule<T>::OnUnregServer, 0, 0, false, true, true, true)                       

#define CMDLINE_OPTION_NoExit(T)                                             \
      CMDLINE_OPTION_ENTRY_EX("NoExit", NULL, &TCComModule<T>::OnNoExit, 0, 0, true, false,  \
        true, false)                                                        

#define CMDLINE_OPTION_Exit(T)                                               \
      CMDLINE_OPTION_ENTRY_EX("Exit", NULL, &TCComModule<T>::OnExit, 0, 0, false, false,     \
        true, true)                                                        

#define CMDLINE_OPTION_Help(T)                                               \
      CMDLINE_OPTION_ENTRY_EX("?", NULL, &TCComModule<T>::OnHelp, 0, 0, false, false,        \
        true, true)                                                         \
      CMDLINE_OPTION_ENTRY_EX("Help", NULL, &TCComModule<T>::OnHelp, 0, 0, false, false,     \
        true, true)                                                        

#define END_CMDLINE_OPTION_MAP()                                            \
      {NULL, NULL, NULL, false, false}                                      \
    };                                                                      \
    if (ppMap)                                                              \
      *ppMap = map;                                                         \
    return sizeofArray(map) - 1;                                            \
  };                                                                        


/////////////////////////////////////////////////////////////////////////////

#endif // !__ComModule_h__
