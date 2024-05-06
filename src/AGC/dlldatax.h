#pragma once

#ifdef _MERGE_PROXYSTUB

extern "C" 
{
  BOOL WINAPI PrxDllMain(HINSTANCE hInstance, DWORD dwReason, 
    LPVOID lpReserved);
  STDAPI PrxDllCanUnloadNow(void);
  STDAPI PrxDllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv);
  STDAPI PrxDllRegisterServer(void);
  STDAPI PrxDllUnregisterServer(void);
}

#endif

/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.
/////////////////////////////////////////////////////////////////////////////
