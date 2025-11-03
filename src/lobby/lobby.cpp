// Lobby.cpp : Implementation of WinMain


// Note: Proxy/Stub Information
//      To build a separate proxy/stub DLL, 
//      run nmake -f Lobbyps.mk in the project directory.

#include "pch.h"

CServiceModule _Module;

BEGIN_OBJECT_MAP(ObjectMap)
END_OBJECT_MAP()

LPCTSTR FindOneOf(LPCTSTR p1, LPCTSTR p2)
{
    while (p1 != NULL && *p1 != NULL)
    {
        LPCTSTR p = p2;
        while (p != NULL && *p != NULL)
        {
            if (*p1 == *p)
                return CharNext(p1);
            p = CharNext(p);
        }
        p1 = CharNext(p1);
    }
    return NULL;
}

// Although some of these functions are big they are declared inline since they are only used once

inline HRESULT CServiceModule::RegisterServer(BOOL bRegTypeLib, BOOL bService, char * szAccount, char * szPassword)
{
    HRESULT hr = CoInitialize(NULL);
    if (FAILED(hr))
        return hr;

    // Remove any previous service since it may point to
    // the incorrect file
    Uninstall();

    // Add service entries
    UpdateRegistryFromResource(IDR_Lobby, TRUE);

    // Adjust the AppID for Local Server or Service
    CRegKey keyAppID;
    LONG lRes = keyAppID.Open(HKEY_CLASSES_ROOT, _T("AppID"), KEY_WRITE);
    if (lRes != ERROR_SUCCESS)
        return lRes;

    CRegKey key;
    lRes = key.Open(keyAppID, _T("{EFD52202-45CB-454D-B477-33BC5C29BDF1}"), KEY_WRITE);
    if (lRes != ERROR_SUCCESS)
        return lRes;
    key.DeleteValue(_T("LocalService"));
    
    if (bService)
    {
		// mdvalley: SetStringValue not in my ATL
        key.SetValue(_T("AllLobby"), _T("LocalService"));
        key.SetValue(_T("-Service"), _T("ServiceParameters"));
        // Create service
        //Install();
        InstallService(szAccount, szPassword);
    }

    // Add object entries
    hr = CComModule::RegisterServer(bRegTypeLib);

    CoUninitialize();
    return hr;
}




/*-------------------------------------------------------------------------
 * GetModulePath
 *-------------------------------------------------------------------------
 * Returns:
 *    path of AllLobby.exe
 */
const char * CServiceModule::GetModulePath()
{
    static char * pszLast = NULL;

    if (pszLast == NULL)
    {
        static char szFileName[MAX_PATH + 16];
        GetModuleFileName(NULL, szFileName, MAX_PATH);
        char*   p = strrchr(szFileName, '\\');
        if (p)
            *(p+1) = 0;
        else 
        {
            szFileName[0] = '\\';
            szFileName[1] = 0;
        }

        pszLast = szFileName;
    }
    return pszLast;
}
 


/*-------------------------------------------------------------------------
 * ReadFromRegistry
 *-------------------------------------------------------------------------
 * Purpose:
 *    Read info from the registry
 * 
 * Parameters:
 *    hk:        which key to read from
 *    bIsString: if false, then item is cosnidered to be a DWORD
 *    szItem:    name of item to read
 *    pValue:    destination of read
 *    dwDefault: default value can be an int or a pointer to a string
 *
 * Returns:
 *    true: iff pValue was set
 */
bool CServiceModule::ReadFromRegistry(HKEY & hk, bool bIsString, const char * szItem, void * pValue, DWORD dwDefault, bool bWarnIfMissing)
{
    char psz[MAX_PATH] = {""};
    DWORD dwSize = (bIsString ? sizeof(psz): sizeof(DWORD));

    if (RegQueryValueEx(hk, szItem, NULL, NULL, (BYTE *)psz, &dwSize) != ERROR_SUCCESS ||
        (bIsString && psz[0] == 0)) // if blank like what SrvConfig.exe makes
    {
        if (bIsString)
        {
            if (dwDefault)
            {
                Strcpy((char*)pValue, (char*)dwDefault);
                if(bWarnIfMissing)
                    LogEvent(EVENTLOG_INFORMATION_TYPE, LE_RegStrMissingDef, szItem, dwDefault);
                return true;
            }
            if(bWarnIfMissing)
                LogEvent(EVENTLOG_ERROR_TYPE, LE_RegStrMissingNoDef, szItem);
            return false;
        }
        if(bWarnIfMissing)
            LogEvent(EVENTLOG_ERROR_TYPE, LE_RegIntMissingDef, szItem, (DWORD)dwDefault);
        *(DWORD*)pValue = dwDefault;
        return true;
    }

    if (bIsString)
        Strcpy((char*)pValue, psz);
    else
        *(DWORD*)pValue = *(DWORD*)psz;

    return true;
}




inline HRESULT CServiceModule::UnregisterServer()
{
    HRESULT hr = CoInitialize(NULL);
    if (FAILED(hr))
        return hr;

    // Remove service entries
    UpdateRegistryFromResource(IDR_Lobby, FALSE);
    // Remove service
    Uninstall();
    // Remove object entries
    CComModule::UnregisterServer(TRUE);
    CoUninitialize();
    return S_OK;
}

inline void CServiceModule::Init(_ATL_OBJMAP_ENTRY* p, HINSTANCE h, UINT nServiceNameID, UINT nServiceDescID, const GUID* plibid)
{
    CComModule::Init(p, h, plibid);

    m_bService = TRUE;

    LoadString(h, nServiceNameID, m_szServiceName, sizeof(m_szServiceName) / sizeof(TCHAR));
    LoadString(h, nServiceDescID, m_szServiceDesc, sizeof(m_szServiceDesc) / sizeof(TCHAR));

    // set up the initial service status 
    m_hServiceStatus = NULL;
    m_status.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
    m_status.dwCurrentState = SERVICE_STOPPED;
    m_status.dwControlsAccepted = SERVICE_ACCEPT_STOP;
    m_status.dwWin32ExitCode = 0;
    m_status.dwServiceSpecificExitCode = 0;
    m_status.dwCheckPoint = 0;
    m_status.dwWaitHint = 0;
}

LONG CServiceModule::Unlock()
{
    LONG l = CComModule::Unlock();
    if (l == 0 && !m_bService)
        PostThreadMessage(dwThreadID, WM_QUIT, 0, 0);
    return l;
}

BOOL CServiceModule::IsInstalled()
{
    BOOL bResult = FALSE;

    SC_HANDLE hSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

    if (hSCM != NULL)
    {
        SC_HANDLE hService = ::OpenService(hSCM, m_szServiceName, SERVICE_QUERY_CONFIG);
        if (hService != NULL)
        {
            bResult = TRUE;
            ::CloseServiceHandle(hService);
        }
        ::CloseServiceHandle(hSCM);
    }
    return bResult;
}

inline BOOL CServiceModule::Install()
{
    if (IsInstalled())
        return TRUE;

    SC_HANDLE hSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    if (hSCM == NULL)
    {
        MessageBox(NULL, _T("Couldn't open service manager"), m_szServiceName, MB_OK);
        return FALSE;
    }

    // Get the executable file path
    TCHAR szFilePath[_MAX_PATH];
    ::GetModuleFileName(NULL, szFilePath, _MAX_PATH);

    SC_HANDLE hService = ::CreateService(
        hSCM, m_szServiceName, m_szServiceDesc,
        SERVICE_ALL_ACCESS, SERVICE_WIN32_OWN_PROCESS,
        SERVICE_DEMAND_START, SERVICE_ERROR_NORMAL,
        szFilePath, NULL, NULL, _T("RPCSS\0"), NULL, NULL);

    if (hService == NULL)
    {
        ::CloseServiceHandle(hSCM);
        MessageBox(NULL, _T("Couldn't create service"), m_szServiceName, MB_OK);
        return FALSE;
    }

    ::CloseServiceHandle(hService);
    ::CloseServiceHandle(hSCM);
    return TRUE;
}

////////////////////////////////////////////////////////////////////////
// InstallService - installs this service into the NT service manager
//
//

BOOL CServiceModule::InstallService(char * szAccount, char * szPassword)
{
    SC_HANDLE schMgr;
    SC_HANDLE schSvc;
    char szPath[512];

    schMgr = OpenSCManager(NULL,NULL,SC_MANAGER_CREATE_SERVICE);

    if (!schMgr)
    {
        printf("Unable to open SCManager.  Service not installed.\n");
        return FALSE;
    }

    GetModuleFileName(NULL,szPath,sizeof(szPath));

    schSvc = CreateService(schMgr,
                           m_szServiceName,
                           m_szServiceDesc,
                           SERVICE_ALL_ACCESS,
                           SERVICE_WIN32_OWN_PROCESS,
                           SERVICE_AUTO_START,
                           SERVICE_ERROR_NORMAL,
                           szPath,
                           NULL,
                           NULL,
                           _T("RPCSS\0"),
                           szAccount,
                           szPassword);

    if (!schSvc)
    {
        ::CloseServiceHandle(schMgr);
        MessageBox(NULL, _T("Couldn't create service"), m_szServiceName, MB_OK);
        return FALSE;
    }

    CloseServiceHandle(schSvc);
    CloseServiceHandle(schMgr);
    printf("%s service installed.\n", m_szServiceName);

    if (szAccount)
    {
        TCUserAccount acct;
        acct.Init(szAccount);  // example: szAccount == Redmond\federat which was passed in on the cmdline
        if (S_OK != acct.HasRight(SE_SERVICE_LOGON_NAME))
        {
          acct.SetRight(SE_SERVICE_LOGON_NAME);
          printf("The account %ls\\%ls has been granted the Logon As A Service right.", acct.GetDomainNameW(), acct.GetUserNameW());
        }
    }

    return TRUE;
}

inline BOOL CServiceModule::Uninstall()
{
    if (!IsInstalled())
        return TRUE;

    SC_HANDLE hSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

    if (hSCM == NULL)
    {
        MessageBox(NULL, _T("Couldn't open service manager"), m_szServiceName, MB_OK);
        return FALSE;
    }

    SC_HANDLE hService = ::OpenService(hSCM, m_szServiceName, SERVICE_STOP | DELETE);

    if (hService == NULL)
    {
        ::CloseServiceHandle(hSCM);
        MessageBox(NULL, _T("Couldn't open service"), m_szServiceName, MB_OK);
        return FALSE;
    }
    SERVICE_STATUS status;
    ::ControlService(hService, SERVICE_CONTROL_STOP, &status);

    BOOL bDelete = ::DeleteService(hService);
    ::CloseServiceHandle(hService);
    ::CloseServiceHandle(hSCM);

    if (bDelete)
        return TRUE;

    MessageBox(NULL, _T("Service could not be deleted"), m_szServiceName, MB_OK);
    return FALSE;
}

///////////////////////////////////////////////////////////////////////////////////////
// Logging functions
int CServiceModule::LogEvent(WORD wType, int id, ...)
{
    TCHAR    chMsg[256];
    HANDLE  hEventSource;
    LPTSTR  lpszStrings[1];
    va_list pArg;

    va_start(pArg, id);
    _vsnprintf(chMsg, sizeof(chMsg), g_rgszLobbyEvents[id], pArg);
    va_end(pArg);

    lpszStrings[0] = chMsg;
    debugf("%s\n", lpszStrings[0]);

    if (m_bService)
    {
        /* Get a handle to use with ReportEvent(). */
        hEventSource = RegisterEventSource(NULL, m_szServiceName);
        if (hEventSource != NULL)
        {
            /* Write to event log. */
            ReportEvent(hEventSource, wType, 0, LobbyEventBaseID + id, NULL, 1, 0, (LPCTSTR*) &lpszStrings[0], NULL);
            DeregisterEventSource(hEventSource);
        }
    }
    else
    {
        // As we are not running as a service, just write the error to the console.
        _putts(chMsg);
    }
    return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////
// Service startup and registration
inline void CServiceModule::Start()
{
    SERVICE_TABLE_ENTRY st[] =
    {
        { m_szServiceName, _ServiceMain },
        { NULL, NULL }
    };
    if (m_bService && !::StartServiceCtrlDispatcher(st))
    {
        m_bService = FALSE;
    }
    if (m_bService == FALSE)
        ExeMain();
}

inline void CServiceModule::ServiceMain(DWORD /* dwArgc */, LPTSTR* /* lpszArgv */)
{
    // Register the control request handler
    m_status.dwCurrentState = SERVICE_START_PENDING;
    m_hServiceStatus = RegisterServiceCtrlHandler(m_szServiceName, _Handler);
    if (m_hServiceStatus == NULL)
    {
        LogEvent(EVENTLOG_ERROR_TYPE, LE_NoServiceHandler);
        return;
    }
    SetServiceStatus(SERVICE_START_PENDING);

    m_status.dwWin32ExitCode = S_OK;
    m_status.dwCheckPoint = 0;
    m_status.dwWaitHint = 0;

    // When the Run function returns, the service has stopped.
    Run();

    SetServiceStatus(SERVICE_STOPPED);
    LogEvent(EVENTLOG_INFORMATION_TYPE, LE_ServiceStopped);
}


void CServiceModule::ExeMain()
{
    _putts("Running as an executable.");
    Run();
}


inline void CServiceModule::Handler(DWORD dwOpcode)
{
    switch (dwOpcode)
    {
    case SERVICE_CONTROL_STOP:
        SetServiceStatus(SERVICE_STOP_PENDING);
        PostThreadMessage(dwThreadID, WM_QUIT, 0, 0);
        break;
    case SERVICE_CONTROL_PAUSE:
        break;
    case SERVICE_CONTROL_CONTINUE:
        break;
    case SERVICE_CONTROL_INTERROGATE:
        break;
    case SERVICE_CONTROL_SHUTDOWN:
        break;
    default:
        LogEvent(EVENTLOG_ERROR_TYPE, LE_BadServiceReq);
    }
}

void WINAPI CServiceModule::_ServiceMain(DWORD dwArgc, LPTSTR* lpszArgv)
{
    _Module.ServiceMain(dwArgc, lpszArgv);
}
void WINAPI CServiceModule::_Handler(DWORD dwOpcode)
{
    _Module.Handler(dwOpcode); 
}

void CServiceModule::SetServiceStatus(DWORD dwState)
{
    m_status.dwCurrentState = dwState;
    ::SetServiceStatus(m_hServiceStatus, &m_status);
}

void CServiceModule::Run()
{
    _Module.dwThreadID = GetCurrentThreadId();

    //HRESULT hr = CoInitialize(NULL);
//  If you are running on NT 4.0 or higher you can use the following call
//  instead to make the EXE free threaded.
//  This means that calls come in on a random RPC thread

	//Imago changed 6/25/08
	HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);

    _ASSERTE(SUCCEEDED(hr));

    // This provides a NULL DACL which will allow access to everyone.
    CSecurityDescriptor sd;
    sd.InitializeFromThreadToken();
    hr = CoInitializeSecurity(sd, -1, NULL, NULL,
        RPC_C_AUTHN_LEVEL_PKT, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE, NULL);
    _ASSERTE(SUCCEEDED(hr));

    hr = _Module.RegisterClassObjects(CLSCTX_LOCAL_SERVER | CLSCTX_REMOTE_SERVER, REGCLS_MULTIPLEUSE);
    _ASSERTE(SUCCEEDED(hr));

    LogEvent(EVENTLOG_INFORMATION_TYPE, LE_Started);
    if (m_bService)
        SetServiceStatus(SERVICE_RUNNING);

    {
      TRef<CLobbyApp> plobbyapp = new CLobbyApp(this);
      if SUCCEEDED(plobbyapp->Init())
        plobbyapp->Run();
      else
        LogEvent(EVENTLOG_ERROR_TYPE, LE_StartFailed);
    }

    _Module.RevokeClassObjects();

    CoUninitialize();
}

/////////////////////////////////////////////////////////////////////////////
//
int __cdecl main(int argc, char *argv[])
{ 
    HINSTANCE hInstance = GetModuleHandle(NULL);

    LPSTR lpCmdLine = GetCommandLine(); //this line necessary for _ATL_MIN_CRT

    // {5B5BE9E8-F1C7-4b95-960A-542A495CCE20}
    static const GUID LIBID_LOBBY = 
        {0x5b5be9e8, 0xf1c7, 0x4b95, {0x96, 0xa, 0x54, 0x2a, 0x49, 0x5c, 0xce, 0x20}};

    _Module.Init(ObjectMap, hInstance, IDS_SERVICENAME, IDS_SERVICEDESC, &LIBID_LOBBY);
    _Module.m_bService = TRUE;

    if (argc > 1 && lstrcmpi(argv[1], _T("-UnregServer"))==0)
        return _Module.UnregisterServer();

    // Register as Local Server
    if (argc > 1 && lstrcmpi(argv[1], _T("-RegServer"))==0)
        return _Module.RegisterServer(FALSE, FALSE, NULL, NULL);
    
    // Register as Service
    if (argc > 1 && lstrcmpi(argv[1], _T("-Service"))==0)
    {
        if (argc == 4) 
            return _Module.RegisterServer(FALSE, TRUE, argv[2], argv[3]);
        else
            return _Module.RegisterServer(FALSE, TRUE, NULL, NULL);
    }

    // Are we Service or Local Server
    CRegKey keyAppID;
    LONG lRes = keyAppID.Open(HKEY_CLASSES_ROOT, _T("AppID"), KEY_READ);
    if (lRes != ERROR_SUCCESS)
    {
        _Module.LogEvent(EVENTLOG_ERROR_TYPE, LE_NoAppIDKey);
        return lRes;
    }

    CRegKey key;
    lRes = key.Open(keyAppID, _T("{EFD52202-45CB-454D-B477-33BC5C29BDF1}"), KEY_READ);
    if (lRes != ERROR_SUCCESS)
    {
        _Module.LogEvent(EVENTLOG_ERROR_TYPE, LE_NotRegistered);
        return lRes;
    }
    
    TCHAR szValue[_MAX_PATH];
    DWORD dwLen = _MAX_PATH;
	
    lRes = key.QueryValue(szValue, _T("LocalService"), &dwLen);

    _Module.m_bService = FALSE;
    if (lRes == ERROR_SUCCESS)
        _Module.m_bService = TRUE;

    _Module.Start();

    // When we get here, the service has been stopped
    return _Module.m_status.dwWin32ExitCode;
}

