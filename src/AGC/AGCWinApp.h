#pragma once

/////////////////////////////////////////////////////////////////////////////
// AGCWinApp.h : Declaration of the CAGCWinApp class.
//

#include <AGC.h>
#include <..\TCLib\WinApp.h>


/////////////////////////////////////////////////////////////////////////////
//
class CAGCWinApp : public TCWinAppDLL
{
// Attributes
public:
  void SetDebugHook(IAGCDebugHook* pdh)
  {
    m_spDebugHook = pdh;
  }

// Overrides
public:
  #ifdef _DEBUG
    virtual void DebugOutput(const char* psz)
    {
      if (NULL != m_spDebugHook)
        m_spDebugHook->DebugOutput(psz);
      else
        TCWinAppDLL::DebugOutput(psz);
    }

    virtual bool OnAssert(const char* psz, const char* pszFile, int line, const char* pszModule)
    {
      return (NULL != m_spDebugHook) ?
        !!m_spDebugHook->OnAssert(psz, pszFile, line, pszModule) :
        TCWinAppDLL::OnAssert(psz, pszFile, line, pszModule);
    }

    virtual void OnAssertBreak()
    {
      if (NULL != m_spDebugHook)
        m_spDebugHook->OnAssertBreak();
      else
        TCWinAppDLL::OnAssertBreak();
    }
  #endif // _DEBUG

// Data Members
protected:
  IAGCDebugHookPtr m_spDebugHook;
};


/////////////////////////////////////////////////////////////////////////////
// External Reference

extern CAGCWinApp g_app;


/////////////////////////////////////////////////////////////////////////////