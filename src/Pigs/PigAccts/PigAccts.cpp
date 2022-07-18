/////////////////////////////////////////////////////////////////////////////
// PigAccts.cpp : Implementation of WinMain
//
// Note: Proxy/Stub Information
//      To build a separate proxy/stub DLL, 
//      run nmake -f PigAcctsps.mk in the project directory.

#include "pch.h"
#include "SrcInc.h"
#include <TCLib.h>
#include <..\TCLib\CoInit.h>
#include <..\TCLib\WinApp.h>
#include "resource.h"
#include "PigAccts.h"


/////////////////////////////////////////////////////////////////////////////
// Global Initialization

CExeModule _Module;
TCWinApp  g_app;


/////////////////////////////////////////////////////////////////////////////
// External Declarations

TC_OBJECT_EXTERN(CPigAccountDispenser)
TC_OBJECT_EXTERN_NON_CREATEABLE(CPigAccount)
// End: External Declarations


/////////////////////////////////////////////////////////////////////////////
// Object Map

BEGIN_OBJECT_MAP(ObjectMap)
  TC_OBJECT_ENTRY_STD(PigAccountDispenser)
  TC_OBJECT_ENTRY_STD_NON_CREATEABLE(PigAccount)
END_OBJECT_MAP()


/////////////////////////////////////////////////////////////////////////////
// Program Entry Point
//
extern "C" int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
  // Initialize the application, if needed
  g_papp->Initialize(GetCommandLine());

  // Just display syntax when the command-line is empty
  if (argc < 2)
    return _Module.Syntax();

  // Enter this thread into a COM apartment
  #if (_WIN32_WINNT >= 0x0400 || defined(_WIN32_DCOM)) & defined(_ATL_FREE_THREADED)
    TCCoInit coInit(COINIT_MULTITHREADED);
  #else
    TCCoInit coInit();
  #endif
  ZAssert(coInit.Succeeded());

  // Initialize the ATL _Module object
  _Module.Init(ObjectMap, GetModuleHandle(NULL), &LIBID_PigsLib);

  // Parse the command line options
  HRESULT hr = _Module.ParseCommandLine(argc, argv);
  if (FAILED(hr))
    return hr;
  if (S_FALSE == hr)
    return S_OK;

  // Initialize security
  if (FAILED(hr = _Module.InitializeSecurity()))
    return _Module.ReportError(hr, TEXT("Security Initialization"));

  // Register our class (factory) objects
  if (FAILED(hr = _Module.RegisterClassObjects()))
  {
    _Module.Term();
    return _Module.ReportError(hr, TEXT("Class Object Registration"));
  }

  // Display status
  _Module.Echo(TEXT("The server is running. Ctrl+C to exit."));
  _Module.EchoFlush();

  // Handle Ctrl+C and other important exiting events
  SetConsoleCtrlHandler(_Module.ConsoleCtrlHandler, true);

  // Enter a message loop (very last Unlock will post WM_QUIT message)
  MSG msg;
  while (GetMessage(&msg, 0, 0, 0))
    DispatchMessage(&msg);

  // Revoke our class (factory) objects
  _Module.RevokeClassObjects();

  // Terminate the ATL _Module object
  _Module.Term();
  
  // Indicate success
  return S_OK;
}



/////////////////////////////////////////////////////////////////////////////
// Overrides

void CExeModule::DisplaySyntax()
{
  Echo(IsWin9x()? IDS_SYNTAX_9X : IDS_SYNTAX_NT);
  EchoFlush();
}


LPCTSTR CExeModule::GetRegInfo(UINT& idResource, bool& bRegTypeLib)
{
  idResource  = IDR_PigAccts;
  bRegTypeLib = false;
  return TEXT("Pigs.AccountDispenser");
}

