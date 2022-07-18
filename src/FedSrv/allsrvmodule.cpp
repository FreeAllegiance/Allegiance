
/*-------------------------------------------------------------------------
 * fedsrv\AllSrvModule.CPP
 * 
 * This part of AllSrv contains Service-related functions and COM/ATL support.  
 * 
 * Owner: 
 * 
 * Copyright 1986-1998 Microsoft Corporation, All Rights Reserved
 *-----------------------------------------------------------------------*/



#include "pch.h"
#include "AllSrvModuleIDL_i.c"
#include <AGC.h>
#include <..\TCAtl\ObjectMap.h>
#include <..\TCLib\CoInit.h>

#include "AdminEventLoggerHook.h"
CComObjectGlobal<CAdminEventLoggerHook> g_DBLoggingHook;

#ifdef _DEBUG
  #include "FedSrvApp.h"
  #include <AGCDebugHook.h>
  CComObjectGlobal<CAGCDebugHook> g_DebugHook;
#endif // _DEBUG

// {E1A86098-DD81-11d2-8B45-00C04F681633}
static const CATID CATID_AllegianceAdmin = 
  {0xe1a86098,0xdd81,0x11d2,{0x8b,0x45,0x0,0xc0,0x4f,0x68,0x16,0x33}};

// The following two APP IDs must be the same
static const GUID APPID_AllSrv =
  {0xE4E8767E,0xDFDB,0x11d2,{0x8B,0x46,0x00,0xC0,0x4F,0x68,0x16,0x33}};

static const char *c_szAPPID_AllSrv = "{E4E8767E-DFDB-11d2-8B46-00C04F681633}";


/////////////////////////////////////////////////////////////////////////////
// Global Initialization

CAGCModule _AGCModule;
CServiceModule _Module;


/////////////////////////////////////////////////////////////////////////////
// External Declarations

extern Global g;


TC_OBJECT_EXTERN(CAdminSession)
TC_OBJECT_EXTERN_NON_CREATEABLE(CAdminGame)
TC_OBJECT_EXTERN_NON_CREATEABLE(CAdminGames)
TC_OBJECT_EXTERN_NON_CREATEABLE(CAdminServer)
TC_OBJECT_EXTERN_NON_CREATEABLE(CAdminUser)
TC_OBJECT_EXTERN_NON_CREATEABLE(CAdminUsers)
TC_OBJECT_EXTERN_NON_CREATEABLE(CAdminShip)
// End: External Declarations


/////////////////////////////////////////////////////////////////////////////
// Object Map

BEGIN_OBJECT_MAP(ObjectMap)
  TC_OBJECT_ENTRY_STD(AdminSession)
  TC_OBJECT_ENTRY_STD_NON_CREATEABLE(AdminGame)
  TC_OBJECT_ENTRY_STD_NON_CREATEABLE(AdminGames)
  TC_OBJECT_ENTRY_STD_NON_CREATEABLE(AdminServer)
  TC_OBJECT_ENTRY_STD_NON_CREATEABLE(AdminUser)
  TC_OBJECT_ENTRY_STD_NON_CREATEABLE(AdminUsers)
  TC_OBJECT_ENTRY_STD_NON_CREATEABLE(AdminShip)
END_OBJECT_MAP()




#ifdef SHOW_SPONSOR_INFO
  int cSponsor = 0;    // for debugging in AdminUtil.h
  int cLimb = 0;
  CDebugClass DebugClass;
#endif



/*-------------------------------------------------------------------------
 * PrintSystemErrorMessage()
 *-------------------------------------------------------------------------
 * Paramters:
 *    dwErrorCode: take a dwErrorCode and print what it means as text    
 * 
 */
void PrintSystemErrorMessage(LPCTSTR szText, DWORD dwErrorCode)
{
  LPVOID lpMsgBuf;

  FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | 
                FORMAT_MESSAGE_ALLOCATE_BUFFER | 
                FORMAT_MESSAGE_IGNORE_INSERTS, 
                NULL, 
                dwErrorCode, 
                MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), 
                (LPTSTR) &lpMsgBuf,
                0,
                NULL 
                );

  printf("%s", (LPCTSTR)szText);
  printf("%s", (LPCTSTR)lpMsgBuf);

  _AGCModule.TriggerEvent(NULL, AllsrvEventID_SystemError, "", -1, -1, -1, 2,
      "Text", VT_LPSTR, szText,
      "Message", VT_LPSTR, lpMsgBuf);

  LocalFree( lpMsgBuf );
}


                                                 


/*-------------------------------------------------------------------------
 * IsInstalled()
 *-------------------------------------------------------------------------
 * Returns:
 *    TRUE iff AllSrv was properly installed (either as Service or EXE)
 * 
 */
BOOL CServiceModule::IsInstalled()
{
    // Are we Service or Local Server
    CRegKey keyAppID;
    LONG lRes = keyAppID.Open(HKEY_CLASSES_ROOT, _T("AppID"), KEY_READ);
    if (lRes != ERROR_SUCCESS)
        return FALSE;

    CRegKey key;
    lRes = key.Open(keyAppID, c_szAPPID_AllSrv, KEY_READ);
    if (lRes != ERROR_SUCCESS)
        return FALSE;

    return TRUE;
}


/*-------------------------------------------------------------------------
 * IsInstalledAsService()
 *-------------------------------------------------------------------------
 * Returns:
 *    TRUE iff AllSrv is installed as an NT service.
 * 
 */
BOOL CServiceModule::IsInstalledAsService()
{
    if (IsWin9x())
      return false;

    // Are we Service or Local Server
    CRegKey keyAppID;
    LONG lRes = keyAppID.Open(HKEY_CLASSES_ROOT, _T("AppID"), KEY_READ);
    if (lRes != ERROR_SUCCESS)
        return FALSE;

    CRegKey key;
    lRes = key.Open(keyAppID, c_szAPPID_AllSrv, KEY_READ);
    if (lRes != ERROR_SUCCESS)
        return FALSE;

    TCHAR szValue[_MAX_PATH];
    DWORD dwLen = _MAX_PATH;
	// mdvalley: QueryStringValue isn't in my ATL, so edited slightly
    lRes = key.QueryValue(szValue, _T("LocalService"), &dwLen);

    if (lRes == ERROR_SUCCESS)
        return TRUE;

    return FALSE;
}

/*-------------------------------------------------------------------------
 * IsInServiceControlManager()
 *-------------------------------------------------------------------------
 * Returns:
 *    TRUE iff AllSrv is an NT service (by checking the Service Control 
 *    Manager).
 * 
 */
BOOL CServiceModule::IsInServiceControlManager()
{
    if (IsWin9x())
      return false;

    BOOL bResult = FALSE;

    SC_HANDLE hSCM = ::OpenSCManager(NULL, NULL, GENERIC_READ);

    if (hSCM != NULL)
    {
        SC_HANDLE hService = ::OpenService(hSCM, c_szSvcName, SERVICE_QUERY_CONFIG);
        if (hService != NULL)
        {
            bResult = TRUE;
            ::CloseServiceHandle(hService);
        }
        ::CloseServiceHandle(hSCM);
    }
    return bResult;
}



/*-------------------------------------------------------------------------
 * CServiceModule::Init
 *-------------------------------------------------------------------------
 * Purpose:
 *    Init some COM/ATL/AGC stuff
 * 
 */
HRESULT CServiceModule::Init(HINSTANCE hInst)
{
  // Initialize the ATL module
  HRESULT hr;
  ZSucceeded(hr = CComModule::Init(ObjectMap, hInst, &LIBID_ALLEGIANCESERVERLib));
  RETURN_FAILED(hr);

  // Indicate success
  return S_OK;
}


/*-------------------------------------------------------------------------
 * CServiceModule::Term
 *-------------------------------------------------------------------------
 * Purpose:
 *    Terminate some COM/ATL/AGC stuff
 * 
 */
void CServiceModule::Term()
{
  CComModule::Term();
}


HRESULT CServiceModule::InitAGC()
{
  // Initialize AGC
  HRESULT hr;
  ZSucceeded(hr = _AGCModule.Init());
  RETURN_FAILED(hr);

  _AGCModule.SetDebugBreakOnErrors(g.fWantInt3);

  #ifdef _DEBUG
    _AGCModule.GetAGCGlobal()->SetDebugHook(&g_DebugHook);
  #endif // _DEBUG

  // Create the set of available AGCEventID ranges
  CComPtr<IAGCEventIDRanges> spRanges;
  ZSucceeded(hr = spRanges.CoCreateInstance(L"AGC.EventIDRanges"));
  RETURN_FAILED(hr);

  // Add our ranges to it
  ZSucceeded(spRanges->AddByValues(EventID_AGC_LowerBound, EventID_AGC_UpperBound));
  ZSucceeded(spRanges->AddByValues(AllsrvEventID_Allsrv_LowerBound, AllsrvEventID_Allsrv_UpperBound));
  ZSucceeded(spRanges->AddByValues(EventID_Admin_LowerBound, EventID_Admin_UpperBound));

  // Set the ranges of available events
  GetAGCGlobal()->SetAvailableEventIDRanges(spRanges);

  // Create the event logger object
  ZSucceeded(hr = m_spEventLogger.CreateInstance("AGC.EventLogger"));
  RETURN_FAILED(hr);

  // Initialize the event logger object
  CComBSTR bstrEventSource(__MODULE__);
  CComBSTR bstrRegKey("HKLM\\" HKLM_FedSrv);
  IAGCEventLoggerPrivatePtr spPrivate(m_spEventLogger);
  hr = spPrivate->Initialize(bstrEventSource, bstrRegKey);
  ZSucceeded(hr);
  RETURN_FAILED(hr);

  // Hook the event logger for DB event logging
  ZSucceeded(hr = spPrivate->put_HookForDBLogging(&g_DBLoggingHook));

  // Indicate success
  return S_OK;
}

void CServiceModule::TermAGC()
{
  // Shutdown the event logger object
  if (NULL != m_spEventLogger)
  {
    IAGCEventLoggerPrivatePtr spPrivate(m_spEventLogger);
    ZAssert(NULL != spPrivate);

    // Terminate the event logger
    ZSucceeded(spPrivate->Terminate());

    // Unhook the event logger for DB event logging
    {
      ZSucceeded(spPrivate->put_HookForDBLogging(NULL));
    }

    // Release the event logger
    spPrivate = NULL;
    m_spEventLogger = NULL;
  }

  // Terminate AGC
  _AGCModule.Term();
}



#if defined(SRV_PARENT)
/*-------------------------------------------------------------------------
 * CServiceModule::xOutgoingPort Imago 6/24/08
 *-------------------------------------------------------------------------
 * Purpose:
 *    Gets/Sets the last used outgoing port number.
 * 
 */
DWORD CServiceModule::GetOutgoingPort() {
	return this->m_dwMPort;
}
void CServiceModule::SetOutgoingPort(DWORD dwPort) {
	this->m_dwMPort = dwPort;
}

/*-------------------------------------------------------------------------
 * InitPIDs	Imago 6/25/08
 *-------------------------------------------------------------------------
 * Purpose:
 *    removes stale process identifiers and re-keys the PID array
 */

void CServiceModule::InitPIDs()
{ 
	//NYI first chcek should be the other way, (enum the process list for AllSrv.exe(s), delete any unmatched pid files)
	//	then do this:

	DWORD dwNewPIDs[98] = {0};
	int iNewKey = 0;
	for (int i=0;i < 98;i++) {
		DWORD pid = this->m_dwPIDs[i];
		if(pid) {
 			char strFilename[10] = "\0";
			sprintf(strFilename,"%d.pid",pid);
			HANDLE hFile = (HANDLE)CreateFile(strFilename, 0, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_NO_BUFFERING, NULL);
			DWORD dwError = GetLastError();
			if (hFile != INVALID_HANDLE_VALUE && dwError != ERROR_FILE_NOT_FOUND) {
				dwNewPIDs[iNewKey] = pid;
				iNewKey++;
			}
			CloseHandle(hFile);
		} 
	}
	this->m_iPIDID = iNewKey;
	memcpy(this->m_dwPIDs, dwNewPIDs, sizeof(dwNewPIDs)); 

}

void CServiceModule::BreakChildren()
{ 
	debugf("intentionally stopping any games...\n");
	for (int i=0;i < 98;i++) {
		DWORD pid = this->m_dwPIDs[i];
		if(pid) {
			FreeConsole();
			AttachConsole(pid);
			GenerateConsoleCtrlEvent(CTRL_BREAK_EVENT,pid);
			debugf("bye bye %d...\n",pid);
		}
	}
}
#endif


/*-------------------------------------------------------------------------
 * CServiceModule::get_EventLog
 *-------------------------------------------------------------------------
 * Purpose:
 *    Gets the previously created event logger object.
 * 
 */
HRESULT CServiceModule::get_EventLog(IAGCEventLogger** ppEventLogger)
{
  CLEAROUT(ppEventLogger, (IAGCEventLogger*)NULL);
  (*ppEventLogger = m_spEventLogger)->AddRef();
  return S_OK;
}



/*-------------------------------------------------------------------------
 * RegisterCOMObjects
 *-------------------------------------------------------------------------
 * Purpose:
 *    Let COM know about our class objects 
 * 
 */
void CServiceModule::RegisterCOMObjects() 
{
  // Make sure we haven't already been here
  ZAssert(m_shevtMTAReady.IsNull());
  ZAssert(m_shevtMTAExit.IsNull());
  ZAssert(m_shthMTA.IsNull());

  // Create thread synchronization objects
  m_shevtMTAReady = CreateEvent(NULL, false, false, NULL);
  m_shevtMTAExit  = CreateEvent(NULL, false, false, NULL);
  ZAssert(!m_shevtMTAReady.IsNull());
  ZAssert(!m_shevtMTAExit.IsNull());

  // Create the thread to keep the MTA alive and register the class objects
  DWORD dwID = 0;
  m_shthMTA = CreateThread(NULL, 8192, MTAKeepAliveThunk, this, 0, &dwID);
  ZAssert(!m_shthMTA.IsNull());

  // Wait for the new thread to do its initialization work
  WaitForSingleObject(m_shevtMTAReady, INFINITE);

  // Close the 'Ready' event since we're done with it
  m_shevtMTAReady = NULL;

  // Check for failure
  if (FAILED(m_hrMTAKeepAlive))
    PrintSystemErrorMessage("Failed to register COM class objects for Admin Session.", m_hrMTAKeepAlive);

  ZSucceeded(m_hrMTAKeepAlive);
}


/*-------------------------------------------------------------------------
 * RevokeCOMObjects
 *-------------------------------------------------------------------------
 * Purpose:
 *    Let COM 'un-know' about our class objects.
 * 
 */
void CServiceModule::RevokeCOMObjects() 
{
  // Make sure that we have registered correctly
  ZAssert(m_shevtMTAReady.IsNull());
  ZAssert(!m_shevtMTAExit.IsNull());
  ZAssert(!m_shthMTA.IsNull());
  ZAssert(WAIT_TIMEOUT == WaitForSingleObject(m_shthMTA, 0));

  // Signal the MTA keep-alive thread to exit
  SetEvent(m_shevtMTAExit);

  // Wait for the MTA keep-alive thread to exit
  HANDLE hth = m_shthMTA;
  while (WAIT_OBJECT_0 != MsgWaitForMultipleObjects(1, &hth, false, INFINITE, QS_ALLINPUT))
  {
    MSG msg;
    while (PeekMessage(&msg, NULL, 0, 0, true))
      DispatchMessage(&msg);
  }

  // Clear the thread handle
  m_shthMTA = NULL;

  // Close the 'Exit' event since we're done with it
  m_shevtMTAExit = NULL;
}


#ifdef MONOLITHIC_DPLAY
  void RegisterMonolithicDPlay()
  {
    HKEY hKey = NULL;
    const char * szKey = "CLSID\\{DA9CABC6-C724-4265-A61D-6E78EB2042B4}\\InprocServer32";
    // check to see if it's already registered, and if so, do nothing, otherwise register it
    if (ERROR_SUCCESS != RegOpenKeyEx(HKEY_CLASSES_ROOT, szKey, 0, KEY_READ, &hKey))
    {
      // Get the module path
      TCHAR szFilePath[_MAX_PATH * 2], szDrive[_MAX_DRIVE], szDir[_MAX_DIR * 2];
      ::GetModuleFileName(NULL, szFilePath, sizeofArray(szFilePath));
      _tsplitpath(szFilePath, szDrive, szDir, NULL, NULL);
      _tmakepath(szFilePath, szDrive, szDir, NULL, NULL);
      int cch = _tcslen(szFilePath);
      if (TEXT('\\') == szFilePath[cch - 1])
        szFilePath[cch - 1] = TEXT('\0');

      // Update the registry from the DPMono.rgs
      USES_CONVERSION;
      _ATL_REGMAP_ENTRY regmap[] =
      {
        {L"MODULE_PATH", T2COLE(szFilePath)},
        {NULL          ,               NULL},
      };
      _Module.UpdateRegistryFromResource(IDR_DPMONO, true, regmap);
    }
    else
    {
      RegCloseKey(hKey);
    }
  }
#endif


/*-------------------------------------------------------------------------
 * RegisterServer
 *-------------------------------------------------------------------------
 * Purpose:
 *    Put module into the registry.
 *
 * Paramters: 
 *    bReRegister:  are we registering after an autoupdate?
 *    bRegTypeLib:  should we register the COM Type Lib?
 *    bService:     if TRUE, then module is placed in the service control manager
 *    argc, argv:   arguments passed via command-line (used by Install Service)
 */

HRESULT CServiceModule::RegisterServer(BOOL bReRegister, BOOL bRegTypeLib, BOOL bService, int argc, char * argv[])
{
    // Enter the COM MTA
    TCCoInit init(COINIT_MULTITHREADED);
    if (init.Failed())
      return init;

    // Initialize the ATL module
    HRESULT hr = _Module.Init(g.hInst); // init COM and/or NT service stuff
    ZSucceeded(hr);

    if (IsWin9x())
      bService = false; // Windows 9x doesn't support services

    // Remove any previous service since it may point to
    // the incorrect file
    if (IsInServiceControlManager() && !bReRegister)
      RemoveService();

    // Add service entries
    _Module.UpdateRegistryFromResource(IDR_AllSrv, true,
      _AGCModule.GetRegMapEntries());

    #ifdef MONOLITHIC_DPLAY
      // Register custom dplay bits
      RegisterMonolithicDPlay();
    #endif

    // Create the component category manager
    CComPtr<ICatRegister> spCatReg;
    hr = CoCreateInstance(CLSID_StdComponentCategoriesMgr, NULL, CLSCTX_ALL,
      IID_ICatRegister, (void**)&spCatReg);
    ZSucceeded(hr);

    if (SUCCEEDED(hr))
    {
      // Determine the LCID for US English
      const LANGID langid = MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US);
      const LCID lcid = MAKELCID(langid, SORT_DEFAULT);

      // Register the component category
      CATEGORYINFO catinfo;
      catinfo.catid = CATID_AllegianceAdmin;
      catinfo.lcid = lcid;
      wcscpy(catinfo.szDescription, L"Allegiance Admin Objects");
      hr = spCatReg->RegisterCategories(1, &catinfo);
      ZSucceeded(hr);

      // Explicitly release the smart pointer
      spCatReg = NULL;
    }

    if(!bReRegister)
    {
      // Adjust the AppID for Local Server or Service
      CRegKey keyAppID;

      DWORD lRes = keyAppID.Open(HKEY_CLASSES_ROOT, _T("AppID"), KEY_WRITE);
      if (lRes != ERROR_SUCCESS)
          return lRes;

      CRegKey key;
      lRes = key.Open(keyAppID, _T("{E4E8767E-DFDB-11d2-8B46-00C04F681633}"), KEY_WRITE);
      if (lRes != ERROR_SUCCESS)
          return lRes;

      key.DeleteValue(_T("LocalService"));
    
      if (bService)
      {
		  // mdvalley: I hate my ATL libraries sometimes (SetStringValue)
          key.SetValue(__MODULE__, _T("LocalService"));
          key.SetValue(_T("-Service"), _T("ServiceParameters"));
          // Create service
          InstallService(argc, argv);
      }
    }

    // Add object entries
    hr = _Module.CComModule::RegisterServer(bRegTypeLib);

    // Terminate the ATL module
    _Module.Term();
    return hr;
}

/*-------------------------------------------------------------------------
 * UnregisterServer
 *-------------------------------------------------------------------------
 * Purpose:
 *    remove module from registry and remove service
 *    from service control manager.
 *
 */

HRESULT CServiceModule::UnregisterServer()
{
    // Enter the COM MTA
    TCCoInit init(COINIT_MULTITHREADED);
    if (init.Failed())
      return init;

    // Initialize the ATL module
    HRESULT hr = _Module.Init(g.hInst); // init COM and/or NT service stuff
    ZSucceeded(hr);

    // Create the component category manager
    CComPtr<ICatRegister> spCatReg;
    hr = CoCreateInstance(CLSID_StdComponentCategoriesMgr, NULL, CLSCTX_ALL,
      IID_ICatRegister, (void**)&spCatReg);
    ZSucceeded(hr);

    if (SUCCEEDED(hr))
    {
      // Unregister our component category
      CATID catid = CATID_AllegianceAdmin;
      spCatReg->UnRegisterCategories(1, &catid);
      spCatReg = NULL;
    }

    // Remove service entries
    UpdateRegistryFromResource(IDR_AllSrv, FALSE);
    // uninstall service
    if (IsInServiceControlManager())
    {
      RemoveService();
    }
    // Remove object entries
    CComModule::UnregisterServer(TRUE);

    // Terminate the ATL module
    _Module.Term();
    return hr;
}



////////////////////////////////////////////////////////////////////////
// InstallService - installs this service into the NT service manager
//
//

BOOL CServiceModule::InstallService(int argc, char * argv[])
{
    SC_HANDLE schMgr;
    SC_HANDLE schSvc;
    char szPath[512];
    char * szUserName;
    char * szPassword;


    schMgr = OpenSCManager(NULL,NULL,SC_MANAGER_CREATE_SERVICE);

    if (!schMgr)
    {
        printf("Unable to open SCManager.  Service not installed.\n");
        return FALSE;
    }

    GetModuleFileName(NULL,szPath,sizeof(szPath));

    if (argc > 3)
    {
        szUserName = argv[2];
        szPassword = argv[3];
    } else
    {
        szUserName = NULL;
        szPassword = NULL;
    }

    schSvc = CreateService(schMgr,
                           c_szSvcName,
                           "MS Allegiance Game Server",
                           SERVICE_ALL_ACCESS,
                           SERVICE_WIN32_OWN_PROCESS,
                           SERVICE_AUTO_START,
                           SERVICE_ERROR_NORMAL,
                           szPath,
                           NULL,
                           NULL,
                           _T("RPCSS\0"),
                           szUserName,
                           szPassword);

    if (!schSvc)
    {
      char szBuf[MAX_PATH];
      DWORD dwErrorCode(GetLastError());
      sprintf(szBuf, "Unable to create service [0x%08x].  Service not installed.\n", dwErrorCode);
      PrintSystemErrorMessage(szBuf, dwErrorCode);

      CloseServiceHandle(schMgr);
      return FALSE;
    }

	//Imago #167
	/*
    schSvc = OpenService(schMgr, c_szSvcName, SERVICE_CHANGE_CONFIG);  
    if (!schSvc)
    {
      char szBuf[MAX_PATH];
      DWORD dwErrorCode(GetLastError());
      sprintf(szBuf, "Unable to open to modify service [0x%08x].  Service not installed corectly.\n", dwErrorCode);
      PrintSystemErrorMessage(szBuf, dwErrorCode);
      CloseServiceHandle(schMgr);
      return FALSE;
    }
	*/
    SERVICE_FAILURE_ACTIONS failureActions;
	failureActions.cActions = 2;
	SC_ACTION actions[2];
	actions[0].Type = SC_ACTION_RESTART;
	actions[0].Delay = 15000;
	actions[1].Type = SC_ACTION_RESTART;
	actions[1].Delay = 15000;
	//actions[2].Type = SC_ACTION_RESTART;
	//actions[2].Delay = 15000;
	failureActions.lpsaActions = actions;
	failureActions.dwResetPeriod = 120;
    failureActions.lpRebootMsg = "";
    failureActions.lpCommand = NULL;
    if( !ChangeServiceConfig2(schSvc, SERVICE_CONFIG_FAILURE_ACTIONS,&failureActions) )
    {
      char szBuf[MAX_PATH];
      DWORD dwErrorCode(GetLastError());
      sprintf(szBuf, "Unable to modify service [0x%08x].  Service not installed.\n", dwErrorCode);
      PrintSystemErrorMessage(szBuf, dwErrorCode);

      CloseServiceHandle(schMgr);
      return FALSE;
    }

    CloseServiceHandle(schSvc);
    CloseServiceHandle(schMgr);
    printf("%s service installed.\n", c_szSvcName);

    if (szUserName)
    {
        TCUserAccount acct;
        acct.Init(szUserName);  // example: szUserName == Redmond\federat which was passed in on the cmdline
        if (S_OK != acct.HasRight(SE_SERVICE_LOGON_NAME))
        {
          acct.SetRight(SE_SERVICE_LOGON_NAME);
          printf("The account %ls\\%ls has been granted the Logon As A Service right.", acct.GetDomainNameW(), acct.GetUserNameW());
        }
    }

    return TRUE;
}

////////////////////////////////////////////////////////////////////////
// RemoveService - uninstalls this service from the NT service manager
//
//

BOOL CServiceModule::RemoveService(void)
{
    SC_HANDLE schMgr;
    SC_HANDLE schSvc;
    SERVICE_STATUS ss;

    schMgr = OpenSCManager(NULL,NULL,SC_MANAGER_ALL_ACCESS);

    if (!schMgr)
    {
        printf("Unable to open SCManager.  Service not removed.\n");
        return FALSE;
    }

    schSvc = OpenService(schMgr, c_szSvcName, SERVICE_ALL_ACCESS);

    if (!schSvc)
    {
        printf("Unable to open %s service.  Service not removed.\n", c_szSvcName);
        CloseServiceHandle(schMgr);
        return FALSE;
    }

    QueryServiceStatus(schSvc,&ss);

    if (ss.dwCurrentState!=SERVICE_STOPPED)
    {
        printf("Unable to remove %s service while it is running.\n", c_szSvcName);
        CloseServiceHandle(schSvc);
        CloseServiceHandle(schMgr);
        return FALSE;
    }

    if (DeleteService(schSvc))
      printf("%s removed (as an NT Service).\n", c_szSvcName);
    else
    {
      char szBuf[MAX_PATH];
      sprintf(szBuf, "Unable to delete %s service.\n", c_szSvcName);

      DWORD dwErrorCode(GetLastError());
      PrintSystemErrorMessage(szBuf, dwErrorCode);
    }

    CloseServiceHandle(schSvc);
    CloseServiceHandle(schMgr);

    return TRUE;
}


// for when running as EXE
BOOL WINAPI HandlerRoutine(
  DWORD dwCtrlType   //  control signal type
)
{
    if (dwCtrlType == CTRL_CLOSE_EVENT || dwCtrlType == CTRL_BREAK_EVENT || dwCtrlType == CTRL_SHUTDOWN_EVENT || dwCtrlType == CTRL_C_EVENT) 
		//imago added 7/3/08
    {
		debugf("shutting down, got control code:%d\n",dwCtrlType);
        SetEvent(g.hKillReceiveEvent);
        return TRUE;
    }

    return FALSE;
}

void RunAsWindow()
{
  // Create an invisible window
  HWND hwnd = ::CreateWindow("static", "AllSrv", WS_OVERLAPPED,
    0, 0, 0, 0, NULL, NULL, ::GetModuleHandle(NULL), NULL);

  // Load the small application icon
  HICON hIcon = reinterpret_cast<HICON>(::LoadImage(::GetModuleHandle(NULL),
    MAKEINTRESOURCE(ICO_FEDSRV), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON),
    ::GetSystemMetrics(SM_CYSMICON), 0));

  // Set the taskbar icon
  const UINT wm_ShellNotifyIcon = WM_APP;
  NOTIFYICONDATA nid = {sizeof(nid), hwnd, 0, NIF_ICON | NIF_MESSAGE | NIF_TIP,
    wm_ShellNotifyIcon, hIcon, "Allegiance Server"};
  Shell_NotifyIcon(NIM_ADD, &nid);

  // Enter a message loop
  while (true)
  {
    DWORD dwWait = ::MsgWaitForMultipleObjects(1, &g.hKillReceiveEvent,
      false, INFINITE, QS_ALLINPUT);
    if (WAIT_OBJECT_0 == dwWait)
      break;

    MSG msg;
    while (::PeekMessage(&msg, NULL, 0, 0, true))
    {
      ::TranslateMessage(&msg);
      switch (msg.message)
      {
        case WM_QUIT:
          ::SetEvent(g.hKillReceiveEvent);
          break;
        default:
          ::DispatchMessage(&msg);
      }
    }
  }

  // Remove the taskbar icon
  Shell_NotifyIcon(NIM_DELETE, &nid);

  // Shutdown the application
  FedSrv_Terminate();
}


////////////////////////////////////////////////////////////////////////
// RunAsExecutable - for debugging, this service can be run from the
//                   command-line.
//
//

VOID CServiceModule::RunAsExecutable()
{
	//Imago: we parents needs to keep track of our children
#if defined(SRV_PARENT)
	InitPIDs();
#endif

    HRESULT hr;
#if defined(SRV_PARENT)
	if (g.bRestarting == false) {
		ZVersionInfo vi;
		printf("%s\n%s\n\n",(LPCSTR)vi.GetFileDescription(), (LPCSTR)vi.GetLegalCopyright());
	}
#else
#if !defined(SRV_CHILD)
    ZVersionInfo vi;
    printf("%s\n%s\n\n",
      (LPCSTR)vi.GetFileDescription(), (LPCSTR)vi.GetLegalCopyright());
#endif
#endif

    if (IsWinNT())
      printf("Running as an executable.\n");

    printf("Initializing...\n");

    hr = FedSrv_Init();

    #if !defined(_CONSOLE)
        if (SUCCEEDED(hr))
        {
            RunAsWindow();
            return;
        }
    #endif


    if (SUCCEEDED(hr))
    {
        HANDLE hConsole;
                                  
        printf("\rType 'Q' to Quit.\n");
        printf("\rType 'P' to Pause.\n");
        printf("\rType 'C' to Continue.\n");

        hConsole = GetStdHandle(STD_INPUT_HANDLE);
        SetConsoleMode(hConsole,0);

        SetConsoleCtrlHandler(HandlerRoutine, true);

        ZAssert(g.hKillReceiveEvent);
		//Imago - parents restart everything after tricking the lobby into sending the client to our child
#if defined(SRV_PARENT)
		g.bRestarting = false;
#endif

        HANDLE hEventArray[] = { g.hKillReceiveEvent, hConsole };

        DWORD dwAwaker;

        //
        // Wait until Receive Thread dies; if Q is pressed, kill Receive Thread
        //
        do
        {
            dwAwaker = MsgWaitForMultipleObjects(2, hEventArray, FALSE, INFINITE, 0);

            if (dwAwaker == WAIT_OBJECT_0 + 1)
            {
                INPUT_RECORD Key;
                DWORD dwRead;

                ReadConsoleInput(hConsole, &Key, 1, &dwRead);

                if (Key.EventType == KEY_EVENT && 
                    Key.Event.KeyEvent.wVirtualKeyCode == 'Q')
                {
                     debugf("Q pressed\n");
                     SetEvent(g.hKillReceiveEvent);
                }
                if (Key.EventType == KEY_EVENT && 
                    Key.Event.KeyEvent.wVirtualKeyCode == 'P')
                {
                     debugf("P pressed\n");
                     FedSrv_Pause();
                }
                if (Key.EventType == KEY_EVENT && 
                    Key.Event.KeyEvent.wVirtualKeyCode == 'C')
                {
                     debugf("C pressed\n");
                     FedSrv_Continue();
                }
            }

        } while (dwAwaker != WAIT_OBJECT_0);

        FedSrv_Terminate();

		// here is a good spot to restart -Imago
#if defined(SRV_PARENT)
		if (g.bRestarting)
			RunAsExecutable();
#endif

    } 
    else
    {
        printf("\rInitialization failed. (%x)\n", hr);
    }
}



////////////////////////////////////////////////////////////////////////
// SetServiceStatus - more useful version of this function than the NT
// one.
//
//

void CServiceModule::SetSvcStatus(
  DWORD state,
  DWORD exitcode
)
{
   if (g.ssh)
   {
       SERVICE_STATUS ss;
       static DWORD s_cp = 1;

       ss.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
       ss.dwCurrentState = state;
       ss.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_PAUSE_CONTINUE;
       ss.dwWin32ExitCode = exitcode;
       ss.dwServiceSpecificExitCode = 0;

       switch (state)
       {
           case SERVICE_RUNNING:
           case SERVICE_STOPPED:
           case SERVICE_PAUSED:
               ss.dwCheckPoint = s_cp++;
               ss.dwWaitHint = 1000;
               break;

         case SERVICE_START_PENDING:
               ss.dwCheckPoint = s_cp++;
               ss.dwWaitHint = 30000;       // we take about 30 seconds to start
            break;

         case SERVICE_STOP_PENDING:
               ss.dwCheckPoint = s_cp++;
               ss.dwWaitHint = 30000;       // we take about 30 seconds to shutdown
            break;

           default:
               ss.dwCheckPoint = 0;
               ss.dwWaitHint = 0;
               break;
       }

       SetServiceStatus(g.ssh,&ss);
   }
}
       
DWORD WINAPI CServiceModule::ServerTerminateThread(DWORD dwUnused)
{
    FedSrv_Terminate();
    SetSvcStatus(SERVICE_STOPPED,0);
    return 0;
}      

void WINAPI CServiceModule::StartServerTerminateThread()
{
    DWORD dwId;
    HANDLE hthrPending = CreateThread(NULL, 8192, (LPTHREAD_START_ROUTINE) ServerTerminateThread, 0, 0, &dwId);
    CloseHandle(hthrPending);
}
       
////////////////////////////////////////////////////////////////////////
// ServiceControl
//
// Service control handler which is registered with NT (using
// RegisterServiceCtrlHandler) as the callback for the service events.
// Right now the service does not support PAUSE/CONTINUE functionality,
// but this could be added here if required.
//
//
void WINAPI CServiceModule::ServiceControl(DWORD dwCode)
{
    switch (dwCode)
    {
        case SERVICE_CONTROL_STOP:
            // If it can take us a long time to stop our service as we wait for operations to complete,
            // the SCM doesn't like the ServiceControl() function to take a long time, so we spin up another
            // thread to terminate us.
            SetSvcStatus(SERVICE_STOP_PENDING, 0);
            StartServerTerminateThread();
            break;

        case SERVICE_CONTROL_PAUSE:
            FedSrv_Pause();
            SetSvcStatus(SERVICE_PAUSED, 0);
            
            break;

        case SERVICE_CONTROL_CONTINUE:
            FedSrv_Continue();
            SetSvcStatus(SERVICE_RUNNING, 0);
            break;

        case SERVICE_CONTROL_SHUTDOWN:
            // System is shutting down.  Memory and resources should *not*
            // be freed in this case due to limited time available for
            // shutdown.  Instead, only urgent non-volatile (i.e. disk)
            // structures should be flushed.
            break;

        case SERVICE_CONTROL_INTERROGATE:
            break;

    }

}




////////////////////////////////////////////////////////////////////////
// ServiceMain
//
// Pointer to this function is in the SERVICE_TABLE_ENTRY of the
// array passed to StartServiceCtrlDispatcher.

void WINAPI CServiceModule::ServiceMain(
  DWORD dwArgc,
  LPSTR *lpszArgv)
{
    HRESULT hr;

    g.ssh = RegisterServiceCtrlHandler(c_szSvcName, ServiceControl);

    if (!g.ssh)
    {
        SetSvcStatus(SERVICE_STOPPED,GetLastError());
        return;
    }

    SetSvcStatus(SERVICE_START_PENDING,NO_ERROR);

    hr = FedSrv_Init();

    if (SUCCEEDED(hr))
        SetSvcStatus(SERVICE_RUNNING,NO_ERROR);
    else
        SetSvcStatus(SERVICE_STOPPED,GetLastError());

}



#if defined(SRV_PARENT)
static SERVICE_STATUS_HANDLE hServiceStatus=NULL;
static SERVICE_STATUS ServiceStatus;
static HANDLE hShutdownEvent=NULL;
static HANDLE hThread=NULL;
DWORD MainThreadId=0;

void SetServiceStatus(DWORD status)
{
	ServiceStatus.dwCurrentState = status;
	::SetServiceStatus(hServiceStatus,&ServiceStatus);
}

static void WINAPI ServiceHandler(DWORD dwOpcode)
{
    switch (dwOpcode)
    {
    case SERVICE_CONTROL_STOP:
    case SERVICE_CONTROL_SHUTDOWN:
        SetServiceStatus(SERVICE_STOP_PENDING);
        PostThreadMessage(MainThreadId, WM_QUIT, 0, 0);
        break;
    case SERVICE_CONTROL_PAUSE:
    case SERVICE_CONTROL_CONTINUE:
    case SERVICE_CONTROL_INTERROGATE:
    default:
        break;
    }
}

static DWORD WINAPI EXEThreadProc(LPVOID lpParameter){
	HRESULT hr;
	_Module.InitPIDs();
    debugf("Initializing...\n");
    hr = FedSrv_Init();
    if (SUCCEEDED(hr))
    {
		g.bRestarting = false;
		HANDLE hEventArray[] = { g.hKillReceiveEvent, hThread };
        DWORD dwAwaker;

        do {
            dwAwaker = MsgWaitForMultipleObjects(2, hEventArray, FALSE, INFINITE, 0);
            if (dwAwaker == WAIT_OBJECT_0 + 1)
            {
				debugf("!!! oh no!\n");
                break;
			}
        } while (dwAwaker != WAIT_OBJECT_0);

        FedSrv_Terminate();
    } 
    else
    {
        debugf("\rInitialization failed. (%x)\n", hr);
		return (DWORD)-1;
    }
	return (DWORD)0;
}

static DWORD WINAPI ThreadProc(LPVOID lpParameter)
{
	SetServiceStatus(SERVICE_RUNNING);
RESTART:
	DWORD EXEThreadId=0;
	HANDLE hEXEThread = ::CreateThread(NULL,NULL,EXEThreadProc,NULL,NULL,&EXEThreadId);
	HANDLE events[2];
	events[0] = hEXEThread;
	events[1] = hShutdownEvent;
	DWORD r = ::WaitForMultipleObjects(2,events,FALSE,INFINITE);
	if (r != WAIT_OBJECT_0)
	{
		debugf("sending exe thread a quit message\n");
		SetEvent(g.hKillReceiveEvent);
		//::PostThreadMessage(EXEThreadId,WM_QUIT,0,0);

		debugf("scm is still rolling, good!\n");
		DWORD r1 = ::WaitForSingleObject(hEXEThread,10000);
		if (r1 == WAIT_TIMEOUT)
		{
			debugf("terminating thread abnormally\n");
			::TerminateThread(hEXEThread,-1);
		}
	}
	if (r != WAIT_OBJECT_0 + 1)
	{
		if(g.bRestarting) {
			CloseHandle(hEXEThread);
			goto RESTART;
		}
		debugf("terminating main thread\n");
		::PostThreadMessage(MainThreadId, WM_QUIT, 0, 0);
	}
	DWORD ec=-1;
	::GetExitCodeThread(hEXEThread,&ec);
	CloseHandle(hEXEThread);
	return ec;
}

void WINAPI MPServiceMain(DWORD dwArgc, LPTSTR* lpszArgv)
{
	MainThreadId = ::GetCurrentThreadId();
	ZeroMemory(&ServiceStatus,sizeof(ServiceStatus));
    ServiceStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
    ServiceStatus.dwCurrentState = SERVICE_STOPPED;
    ServiceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP|SERVICE_ACCEPT_SHUTDOWN;
	hServiceStatus = RegisterServiceCtrlHandler(c_szSvcName, ServiceHandler);
	SetServiceStatus(SERVICE_START_PENDING);
	
	hShutdownEvent = ::CreateEvent(NULL,TRUE,FALSE,NULL);
	DWORD ThreadId=0;
	hThread = ::CreateThread(NULL,NULL,ThreadProc,NULL,NULL,&ThreadId);
	MSG msg;
	while (GetMessage(&msg, 0, 0, 0))
		DispatchMessage(&msg);
	SetEvent(hShutdownEvent);
	WaitForSingleObject(hThread,INFINITE);
	::CloseHandle(hThread);
	::CloseHandle(hShutdownEvent);
	SetServiceStatus(SERVICE_STOPPED);
	_Module.BreakChildren();
}


#else 
void WINAPI _ServiceMain(DWORD dwArgc, LPTSTR* lpszArgv)
{
    _Module.ServiceMain(dwArgc, lpszArgv);
}
#endif


/*-------------------------------------------------------------------------
 * StopAllsrv
 *-------------------------------------------------------------------------
 * Purpose:
 *    cause allsrv to terminate 
 */

void CServiceModule::StopAllsrv()
{ 
#if defined(SRV_PARENT) || defined(SRV_CHILD)
    if(g.hKillReceiveEvent)
      SetEvent(g.hKillReceiveEvent);
#else
  if(_Module.IsInstalledAsService())
    _Module.ServiceControl(SERVICE_CONTROL_STOP);
  else
  {
    if(g.hKillReceiveEvent)
      SetEvent(g.hKillReceiveEvent);
//    if(m_hEventStopRunningAsEXE)
//        SetEvent(m_hEventStopRunningAsEXE);
  }
#endif
}


/*-------------------------------------------------------------------------
 * MTAKeepAliveThunk
 *-------------------------------------------------------------------------
 * Purpose:
 *    static thunk function to reinterpret the thread parameter and call
 *    through it as a 'this' pointer.
 */
DWORD WINAPI CServiceModule::MTAKeepAliveThunk(void* pvThis)
{
  CServiceModule* pThis = reinterpret_cast<CServiceModule*>(pvThis);
  pThis->MTAKeepAliveThread();
  return 0;
}


/*-------------------------------------------------------------------------
 * MTAKeepAliveThread
 *-------------------------------------------------------------------------
 * Purpose:
 *    Thread controlling-function that initializes the COM MTA, registers
 *    the server's externally-creatable class objects and sleeps. When the
 *    quit event is signaled, the thread wakes up, revokes the class
 *    objects, and uninitializes the COM MTA.
 */
void CServiceModule::MTAKeepAliveThread()
{
  // Enter the COM MTA
  TCCoInit init(COINIT_MULTITHREADED);
  m_hrMTAKeepAlive = init;

  // Register the externally-creatable class objects
  if (init.Succeeded())
    m_hrMTAKeepAlive = _Module.RegisterClassObjects(CLSCTX_LOCAL_SERVER,
      REGCLS_MULTIPLEUSE);

  // Signal the 'Ready' event
  ZAssert(!m_shevtMTAReady.IsNull());
  SetEvent(m_shevtMTAReady);
  RETURN_FAILED_VOID(m_hrMTAKeepAlive);

  // Wait for the 'Exit' event
  ZAssert(!m_shevtMTAExit.IsNull());
  WaitForSingleObject(m_shevtMTAExit, INFINITE);

  // Note: the following call was moved here from the ReceiveThread function
  // in FedSrv.cpp so that it will always be called from within the MTA.
  // Destroy all sessions now--before GetAGCGlobal() becomes invalid
  CAdminSession::DestroyAllSessions();

  // Revoke the class objects
  _Module.RevokeClassObjects();

  // CoUninitialize called automatically upon leaving scope (using TCCoInit)
}

