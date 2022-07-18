#ifndef _LOBBYMODULE_
#define _LOBBYMODULE_

#ifndef STRICT
  #define STRICT
#endif


#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0A00
#endif

#define _ATL_APARTMENT_THREADED

#include <atlbase.h>

#include "LobbyAppSite.h" // Declaration of ILobbyAppSite

//You may derive a class from CComModule and use it if you want to override
//something, but do not change the name of _Module

class CServiceModule : public CComModule, public ILobbyAppSite
{
public:
    HRESULT RegisterServer(BOOL bRegTypeLib, BOOL bService, char * szAccount, char * szPassword);
    HRESULT UnregisterServer();
    void Init(_ATL_OBJMAP_ENTRY* p, HINSTANCE h, UINT nServiceNameID, UINT nServiceDescID, const GUID * plibid = NULL);
    void Start();
    void ServiceMain(DWORD dwArgc, LPTSTR* lpszArgv);
    void ExeMain();
    void Handler(DWORD dwOpcode);
    void Run();
    const char * GetModulePath();
    BOOL IsInstalled();
    BOOL InstallService(char * szAccount, char * szPassword);
    BOOL Install();
    BOOL Uninstall();
    LONG Unlock() throw();
    void SetServiceStatus(DWORD dwState);
    void SetupAsLocalServer();
    bool ReadFromRegistry(HKEY & hk, bool bIsString, const char * szItem, void * pValue, DWORD dwDefault, bool bWarnIfMissing = false);

// ILobbyAppSite
    virtual int LogEvent(WORD wType, int id, ...);


//Implementation
private:
    static void WINAPI _ServiceMain(DWORD dwArgc, LPTSTR* lpszArgv);
    static void WINAPI _Handler(DWORD dwOpcode);

// data members
public:
    TCHAR m_szServiceName[256];
    TCHAR m_szServiceDesc[256];
    SERVICE_STATUS_HANDLE m_hServiceStatus;
    SERVICE_STATUS m_status;
    DWORD dwThreadID;
    BOOL m_bService;
};

extern CServiceModule _Module;
#include <atlcom.h>

#endif
