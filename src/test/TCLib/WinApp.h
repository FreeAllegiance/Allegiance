#pragma once


/////////////////////////////////////////////////////////////////////////////
// WinApp.h | Declaration of the TCWinApp class.
//

#include <TCLib.h>

extern Win32App* g_papp;

#ifdef _DEBUG

  ///////////////////////////////////////////////////////////////////////////
  //
  class TCWinApp : public Win32App
  {
  // Construction
  public:
    TCWinApp()
    {
      const int modeWarn   = _CRTDBG_MODE_DEBUG | _CRTDBG_MODE_FILE;
      const int modeError  = _CRTDBG_MODE_DEBUG | _CRTDBG_MODE_FILE;
      const int modeAssert = _CRTDBG_MODE_DEBUG | _CRTDBG_MODE_FILE;

      // Report WARNINGS to Debugger and stdout
      _CrtSetReportMode(_CRT_WARN  , modeWarn);
      _CrtSetReportFile(_CRT_WARN  , _CRTDBG_FILE_STDOUT);

      // Report ERRORS to Debugger and stdout
      _CrtSetReportMode(_CRT_ERROR , modeError);
      _CrtSetReportFile(_CRT_ERROR , _CRTDBG_FILE_STDOUT);

      // Report ASSERTS to Debugger and stdout
      _CrtSetReportMode(_CRT_ASSERT, modeAssert);
      _CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDOUT);

      // Set a hook function
      AccessPrevHook() = _CrtSetReportHook(ReportHook);

      // Get the module filename of the current process
      char szModule[_MAX_PATH];
      GetModuleFileNameA(NULL, szModule, sizeofArray(szModule));

      // Break out just the file name part
      _splitpath(szModule, NULL, NULL, m_szModuleName, NULL);

      // Add a colon and a space
      strcat(m_szModuleName, ": ");

      // Compute the address of the terminating null
      m_cchModuleName = strlen(m_szModuleName);
      m_pszModuleNameEnd = m_szModuleName + m_cchModuleName;
    }
    /////////////////////////////////////////////////////////////////////////
    ~TCWinApp()
    {
      // Restore the previous hook function, if any
      _CrtSetReportHook(AccessPrevHook());
    }

  // Overrides
  public:
    /////////////////////////////////////////////////////////////////////////
    virtual void DebugOutput(const char *psz)
    {
      int cch = strlen(psz);
      if (cch < (int)(sizeof(m_szModuleName) - (m_cchModuleName + 1))) //Imago 8/10 added (int)
      {
        strcpy(m_pszModuleNameEnd, psz);
        _CrtDbgReport(_CRT_WARN, NULL, NULL, NULL, m_szModuleName);
      }
      else
      {
        char* pszCopy = (char*)_alloca(cch + m_cchModuleName + 1);
        strncpy(pszCopy, m_szModuleName, m_cchModuleName);
        strcpy(pszCopy + m_cchModuleName, psz);
        _CrtDbgReport(_CRT_WARN, NULL, NULL, NULL, pszCopy);
      }
    }
    /////////////////////////////////////////////////////////////////////////
    virtual bool OnAssert(const char* psz, const char* pszFile, int line,
      const char* pszModule)
    {
      // Save the current _CRT_ASSERT report mode, if we are interactive
      int nReportModePrev;
      bool bInteractive = IsInteractiveDesktop();
      if (bInteractive)
      {
        nReportModePrev = _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_REPORT_MODE);
        _CrtSetReportMode(_CRT_ASSERT, nReportModePrev | _CRTDBG_MODE_WNDW);
      }

      // Report the assertion
      int n = _CrtDbgReport(_CRT_ASSERT, pszFile, line, NULL, psz);

      // Restore the previous report mode, if interactive
      if (bInteractive)
        _CrtSetReportMode(_CRT_ASSERT, nReportModePrev);

      // Perform the necessary break
      if (1 == n || !bInteractive)
        DebugBreak();

      // Return false, since we already took care of the break
      return false;
    }

  // Implementation
  protected:
    /////////////////////////////////////////////////////////////////////////
    static _CRT_REPORT_HOOK& AccessPrevHook()
    {
      static _CRT_REPORT_HOOK s_pfnPrevHook = NULL;
      return s_pfnPrevHook;
    }
    /////////////////////////////////////////////////////////////////////////
    static bool IsNoise(const char* pszText)
    {
      if ('\0' != pszText[1])
        return false;
      if ('.'  == pszText[0])
        return true;
      if ('{'  == pszText[0])
        return true;
      if ('}'  == pszText[0])
        return true;
      if ('+'  == pszText[0])
        return true;
      if ('-'  == pszText[0])
        return true;
      if ('['  == pszText[0])
        return true;
      if (']'  == pszText[0])
        return true;
      if ('<'  == pszText[0])
        return true;
      if ('>'  == pszText[0])
        return true;
      return false;
    }
    /////////////////////////////////////////////////////////////////////////
    static int ReportHook(int reportType, char* message, int* returnValue)
    {
      static bool s_bInHook = false;
      if (s_bInHook)
        return false;

      static bool s_bIsNoise = false;
      bool bWasNoise = s_bIsNoise;
      s_bIsNoise = IsNoise(message);

      if (bWasNoise && !s_bIsNoise)
      {
        s_bInHook = true;
        int nReportModePrev = _CrtSetReportMode(reportType, _CRTDBG_REPORT_MODE);
        _CrtSetReportMode(reportType, nReportModePrev & ~_CRTDBG_MODE_WNDW);
        _CrtDbgReport(reportType, NULL, NULL, NULL, "\n");
        _CrtSetReportMode(reportType, nReportModePrev);
        s_bInHook = false;
      }

      // Perform default processing
      return false;
    }

  // Data Members
  protected:
    char  m_szModuleName[_MAX_PATH * 4];
    char* m_pszModuleNameEnd;
    int   m_cchModuleName;
  };

  class TCWinAppDLL : public TCWinApp
  {
  // Construction
  public:
    TCWinAppDLL()
    {
      // Do NOT report WARNINGS or ERRORS to stdout
      _CrtSetReportMode(_CRT_WARN , _CRTDBG_MODE_DEBUG);
      _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);
    }
  };
#else // _DEBUG
  ///////////////////////////////////////////////////////////////////////////
  //
  class TCWinApp : public Win32App
  {
  };
  typedef TCWinApp TCWinAppDLL;
#endif // _DEBUG


/////////////////////////////////////////////////////////////////////////////