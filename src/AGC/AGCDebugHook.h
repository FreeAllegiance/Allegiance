#pragma once

/////////////////////////////////////////////////////////////////////////////
// AGCDebugHook.h : Declaration of the CAGCDebugHook class.
//
/////////////////////////////////////////////////////////////////////////////
// CAGCDebugHook
//
class ATL_NO_VTABLE CAGCDebugHook : 
  public IAGCDebugHook,
  public CComObjectRootEx<CComMultiThreadModel>
{
// Interface Map
public:
  BEGIN_COM_MAP(CAGCDebugHook)
    COM_INTERFACE_ENTRY(IAGCDebugHook)
  END_COM_MAP()

// IDebugHook Interface Methods
public:
  STDMETHODIMP_(void) DebugOutput(LPCSTR psz)
  {
    g_app.DebugOutput(psz);
  }
  STDMETHODIMP_(boolean) OnAssert(LPCSTR psz, LPCSTR pszFile, int line,
    LPCSTR pszModule)
  {
    return g_app.OnAssert(psz, pszFile, line, pszModule);
  }
  STDMETHODIMP_(void) OnAssertBreak()
  {
    g_app.OnAssertBreak();
  }
};


/////////////////////////////////////////////////////////////////////////////