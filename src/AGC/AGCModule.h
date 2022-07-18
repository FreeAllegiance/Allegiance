#ifndef __AGCModule_h__
#define __AGCModule_h__

/////////////////////////////////////////////////////////////////////////////
// AGCModule : Declaration of the CAGCModule class.
//

extern "C" const CLSID CLSID_AGCGlobal;

#include <..\TCLib\AutoHandle.h>

#ifdef AGC_MODULE

  ///////////////////////////////////////////////////////////////////////////
  //
  class CAGCModule
  {
  // Attributes
  public:
    IAGCGlobalPtr GetAGCGlobal()
    {
      HRESULT hr;
      if (NULL == m_spcf)
      {
        hr = _Module.GetClassObject(CLSID_AGCGlobal, IID_IClassFactory,
          (void**)&m_spcf);
        ZAssert(SUCCEEDED(hr) && NULL != m_spcf);
      }
      IAGCGlobalPtr spGlobal;
      hr = m_spcf->CreateInstance(NULL, IID_IAGCGlobal, (void**)&spGlobal);
      ZAssert(NULL != spGlobal);
      return spGlobal;
    }

  // Operations
  public:
    void TriggerEvent(HAGCLISTENERS hListeners, AGCEventID idEvent,
      LPCOLESTR pszSubject, AGCUniqueID idSubject,
      AGCUniqueID idObject1, AGCUniqueID idObject2, long cArgTriplets, ...);
    void TriggerEvent(HAGCLISTENERS hListeners, AGCEventID idEvent,
      LPCSTR pszSubject, AGCUniqueID idSubject,
      AGCUniqueID idObject1, AGCUniqueID idObject2, long cArgTriplets, ...);
    void TriggerContextEvent(HAGCLISTENERS hListeners, AGCEventID idEvent,
      LPCSTR pszContext, LPCOLESTR pszSubject, AGCUniqueID idSubject,
      AGCUniqueID idObject1, AGCUniqueID idObject2, long cArgTriplets, ...);
    void TriggerContextEvent(HAGCLISTENERS hListeners, AGCEventID idEvent,
      LPCSTR pszContext, LPCSTR pszSubject, AGCUniqueID idSubject,
      AGCUniqueID idObject1, AGCUniqueID idObject2, long cArgTriplets, ...);
    HRESULT MakeAGCEvent(AGCEventID idEvent, IAGCEvent** ppEvent,
      LPCSTR pszContext, LPCOLESTR pszSubject, AGCUniqueID idSubject,
      long cArgTriplets, ...);
    HRESULT MakeAGCEvent(AGCEventID idEvent, IAGCEvent** ppEvent,
      LPCSTR pszContext, LPCSTR pszSubject, AGCUniqueID idSubject,
      long cArgTriplets, ...);

  // Implementation
  protected:
    void DoTriggerEvent(HAGCLISTENERS hListeners, AGCEventID idEvent,
      LPCSTR pszContext, LPCOLESTR pszSubject, AGCUniqueID idSubject,
      AGCUniqueID idObject1, AGCUniqueID idObject2,
      long cArgTriplets, void* pvArgs)
    {
      bool bIsError = GetAGCGlobal()->GetEventSeverity(idEvent) == EVENTLOG_ERROR_TYPE;

      // Always trigger events with severity level of Error synchronously 
      if (!bIsError)
        GetAGCGlobal()->TriggerEvent(hListeners, idEvent, pszContext,
          pszSubject, idSubject, idObject1, idObject2, cArgTriplets, pvArgs);
      else
      {
        GetAGCGlobal()->TriggerEventSynchronous(hListeners, idEvent, pszContext,
          pszSubject, idSubject, idObject1, idObject2, cArgTriplets, pvArgs);
        *(DWORD*)0 = 0; // make DrWatson happy
        //DebugBreak();
        exit(EXIT_FAILURE);
      }
    }

  // Data Members
  protected:
    IClassFactoryPtr m_spcf;
  };

#else // AGC_MODULE

  #ifndef _assert_h_

    #ifndef ZAssert
      #define ZAssert(x) while (false) {}
    #endif // !ZAssert

    inline void debugf(const char* , ...) {}    
  #endif // !_assert_h_

  #ifndef RETURN_FAILED
    #define RETURN_FAILED(exp)  \
    {                           \
      HRESULT _hr = exp;        \
      if (FAILED(_hr))          \
        return _hr;             \
    }
  #endif // !RETURN_FAILED

  #ifndef sizeofArray
    ///////////////////////////////////////////////////////////////////////////
    // Counts the number of elements in a fixed-length array.
    // Parameters:  x - The name of the array of which to compute the size.
    #define sizeofArray(x) (sizeof(x) / sizeof(x[0]))
  #endif // !sizeofArray


  ///////////////////////////////////////////////////////////////////////////
  //
  class CAGCModule
  {
  // Construction
  public:
    CAGCModule();

  // Attributes
  public:
    IAGCGlobal* GetAGCGlobal();
    _ATL_REGMAP_ENTRY* GetRegMapEntries();
    void SetDebugBreakOnErrors(bool bBreak);
    bool GetDebugBreakOnErrors() const;

  // Operations
  public:
    HRESULT Init();
    void Term();
    bool IsRegistered();
    HRESULT Register();

  // Operations
  public:
    void TriggerEvent(HAGCLISTENERS hListeners, AGCEventID idEvent,
      LPCOLESTR pszSubject, AGCUniqueID idSubject,
      AGCUniqueID idObject1, AGCUniqueID idObject2, long cArgTriplets, ...);
    void TriggerEvent(HAGCLISTENERS hListeners, AGCEventID idEvent,
      LPCSTR pszSubject, AGCUniqueID idSubject,
      AGCUniqueID idObject1, AGCUniqueID idObject2, long cArgTriplets, ...);
    void TriggerContextEvent(HAGCLISTENERS hListeners, AGCEventID idEvent,
      LPCSTR pszContext, LPCOLESTR pszSubject, AGCUniqueID idSubject,
      AGCUniqueID idObject1, AGCUniqueID idObject2, long cArgTriplets, ...);
    void TriggerContextEvent(HAGCLISTENERS hListeners, AGCEventID idEvent,
      LPCSTR pszContext, LPCSTR pszSubject, AGCUniqueID idSubject,
      AGCUniqueID idObject1, AGCUniqueID idObject2, long cArgTriplets, ...);
    HRESULT MakeAGCEvent(AGCEventID idEvent, IAGCEvent** ppEvent,
      LPCSTR pszContext, LPCOLESTR pszSubject, AGCUniqueID idSubject,
      long cArgTriplets, ...);
    HRESULT MakeAGCEvent(AGCEventID idEvent, IAGCEvent** ppEvent,
      LPCSTR pszContext, LPCSTR pszSubject, AGCUniqueID idSubject,
      long cArgTriplets, ...);

  // Implementation
  protected:
    void DoTriggerEvent(HAGCLISTENERS hListeners, AGCEventID idEvent,
      LPCSTR pszContext, LPCOLESTR pszSubject, AGCUniqueID idSubject,
      AGCUniqueID idObject1, AGCUniqueID idObject2,
      long cArgTriplets, void* pvArgs);

  // Data Members
  protected:
    IAGCGlobalPtr m_spGlobal;
    bool          m_bDebugBreakOnErrors;
  };

  ///////////////////////////////////////////////////////////////////////////
  // Construction

  inline CAGCModule::CAGCModule() :
    m_bDebugBreakOnErrors(true)
  {
  }

  ///////////////////////////////////////////////////////////////////////////
  // Attributes

  inline IAGCGlobal* CAGCModule::GetAGCGlobal()
  {
    #ifdef _DEBUG
      if (NULL == m_spGlobal)
        debugf("CAGCModule::GetAGCGlobal(): Init not successfully called!\n");
      ZAssert(NULL != m_spGlobal);
    #endif // _DEBUG
    return m_spGlobal;
  }
  inline _ATL_REGMAP_ENTRY* CAGCModule::GetRegMapEntries()
  {
    static OLECHAR s_szModulePath[_MAX_PATH];
    static _ATL_REGMAP_ENTRY s_map[] =
    {
      {OLESTR("AGCModule"), s_szModulePath},
      {NULL               ,           NULL}
    };

    // Get the AGC resource module
    HINSTANCE hinst = GetAGCGlobal()->GetResourceInstance();
    ZAssert(hinst);

    // Warning: Not thread-safe,
    //   (but it shouldn't matter, since this function will typically only be
    //    called from an application's main thread during server registration.)
    TCHAR szModulePath[_MAX_PATH];
    GetModuleFileName(hinst, szModulePath, sizeofArray(szModulePath));
    USES_CONVERSION;
    wcscpy(s_szModulePath, T2COLE(szModulePath));

    // Return the array of registrar replacement entries
    return s_map;
  }

  inline void CAGCModule::SetDebugBreakOnErrors(bool bBreak)
  {
    m_bDebugBreakOnErrors = bBreak;
  }

  inline bool CAGCModule::GetDebugBreakOnErrors() const
  {
    return m_bDebugBreakOnErrors;
  }


  ///////////////////////////////////////////////////////////////////////////
  // Operations

  inline HRESULT CAGCModule::Init()
  {
    if (!IsRegistered())
    {
      HRESULT hr = Register();
      ZAssert(hr);
      if (FAILED(hr))
        return hr;
    }

    // Create the single instance of the AGCGlobal object
    RETURN_FAILED(m_spGlobal.CreateInstance(CLSID_AGCGlobal));

    // Initialize the AGCGlobal object
    m_spGlobal->Initialize();

    // Indicate success
    return S_OK;
  }

  inline void CAGCModule::Term()
  {
    // Terminate the AGCGlobal object
    if (NULL != m_spGlobal)
      m_spGlobal->Terminate();

    // Release the IAGCGlobal pointer
    m_spGlobal = NULL;
  }

  inline bool CAGCModule::IsRegistered()
  {
    // Obviously registered if the global has already been created
    if (NULL != m_spGlobal)
      return true;

    // Attempt to create an instance
    return SUCCEEDED(m_spGlobal.CreateInstance(CLSID_AGCGlobal));
  }

  inline HRESULT CAGCModule::Register()
  {
    // Get the current module name
    TCHAR szModule[_MAX_PATH];
    ::GetModuleFileName(NULL, szModule, sizeofArray(szModule));

    // Break the module name into pieces
    TCHAR szDrive[_MAX_DRIVE], szDir[_MAX_DIR], szPath[_MAX_PATH];
    _tsplitpath(szModule, szDrive, szDir, NULL, NULL);
    _tmakepath(szPath, szDrive, szDir, NULL, NULL);

    // Change the current directory
    TCHAR szCurDirPrev[_MAX_PATH] = TEXT("");
    GetCurrentDirectory(sizeofArray(szCurDirPrev), szCurDirPrev);
    SetCurrentDirectory(szPath);

    // Load the AGC module
    TCHAR szAGC[_MAX_PATH];
    _tmakepath(szAGC, szDrive, szDir, TEXT("AGC"), TEXT(".dll"));
    HINSTANCE hinst = ::LoadLibrary(szAGC);
    HRESULT hr = HRESULT_FROM_WIN32(::GetLastError());
    if (hinst)
    {
      // Get the specified export procedure address
      typedef HRESULT (CALLBACK *PFNREG)();
      PFNREG pfn = (PFNREG)GetProcAddress(hinst, "DllRegisterServer");

      // Call the function
      hr = pfn ? (*pfn)() : HRESULT_FROM_WIN32(::GetLastError());

      // Unload the specified module
      FreeLibrary(hinst);
    }

    // Restore the previous current directory
    SetCurrentDirectory(szCurDirPrev);

    // Return the last HRESULT
    return hr;
  }

  ///////////////////////////////////////////////////////////////////////////
  // Implementation
  inline void CAGCModule::DoTriggerEvent(HAGCLISTENERS hListeners,
    AGCEventID idEvent, LPCSTR pszContext,
    LPCOLESTR pszSubject, AGCUniqueID idSubject,
    AGCUniqueID idObject1, AGCUniqueID idObject2,
    long cArgTriplets, void* pvArgs)
  {
    bool bIsError;
    if (NULL != m_spGlobal)
    {
      // Get the event severity
      bIsError = m_spGlobal->GetEventSeverity(idEvent) == EVENTLOG_ERROR_TYPE;

      // Always trigger events with severity level of Error synchronously 
      if (!bIsError)
        m_spGlobal->TriggerEvent(hListeners, idEvent, pszContext,
          pszSubject, idSubject, idObject1, idObject2, cArgTriplets, pvArgs);
      else
        m_spGlobal->TriggerEventSynchronous(hListeners, idEvent, pszContext,
          pszSubject, idSubject, idObject1, idObject2, cArgTriplets, pvArgs);
    }
    else
    {
      // Get the name of the current module
      TCHAR szModule[_MAX_PATH], szName[_MAX_PATH];
      GetModuleFileName(NULL, szModule, sizeofArray(szModule));
      _tsplitpath(szModule, NULL, NULL, szName, NULL);

      // Open the local event log for the module's application
      TCEventSourceHandle sh = RegisterEventSource(NULL, szName);

      // Log an event
      const DWORD dwEventID = (3 << 30) | EventID_AGCNotInitialized;
      ReportEvent(sh, EVENTLOG_ERROR_TYPE, 0, dwEventID, NULL, 0, 0, NULL, NULL);

      // Set the event severity
      bIsError = true;
    }

    // Errors (other than Assert's) will break into debugger
    if (bIsError && EventID_AGCAssert != idEvent)
    {
      #ifdef _DEBUG
        if (GetDebugBreakOnErrors())
          *(DWORD*)0 = 0;
          //DebugBreak();
      #endif // _DEBUG

      // Harsh reality has hit us
      exit(EXIT_FAILURE);
    }
  }

#endif // !AGC_MODULE


/////////////////////////////////////////////////////////////////////////////
// Operations

inline void CAGCModule::TriggerEvent(HAGCLISTENERS hListeners,
  AGCEventID idEvent, LPCOLESTR pszSubject, AGCUniqueID idSubject,
  AGCUniqueID idObject1, AGCUniqueID idObject2, long cArgTriplets, ...)
{
  va_list argptr;
  va_start(argptr, cArgTriplets);
  DoTriggerEvent(hListeners, idEvent, NULL, pszSubject, idSubject,
    idObject1, idObject2, cArgTriplets, argptr);
  va_end(argptr);
}

inline void CAGCModule::TriggerEvent(HAGCLISTENERS hListeners,
  AGCEventID idEvent, LPCSTR pszSubject, AGCUniqueID idSubject,
  AGCUniqueID idObject1, AGCUniqueID idObject2, long cArgTriplets, ...)
{
  USES_CONVERSION;
  va_list argptr;
  va_start(argptr, cArgTriplets);
  DoTriggerEvent(hListeners, idEvent, NULL, A2COLE(pszSubject), idSubject,
    idObject1, idObject2, cArgTriplets, argptr);
  va_end(argptr);
}


inline void CAGCModule::TriggerContextEvent(HAGCLISTENERS hListeners,
  AGCEventID idEvent, LPCSTR pszContext,
  LPCOLESTR pszSubject, AGCUniqueID idSubject,
  AGCUniqueID idObject1, AGCUniqueID idObject2, long cArgTriplets, ...)
{
  va_list argptr;
  va_start(argptr, cArgTriplets);
  DoTriggerEvent(hListeners, idEvent, pszContext, pszSubject, idSubject,
    idObject1, idObject2, cArgTriplets, argptr);
  va_end(argptr);
}

inline void CAGCModule::TriggerContextEvent(HAGCLISTENERS hListeners,
  AGCEventID idEvent, LPCSTR pszContext,
  LPCSTR pszSubject, AGCUniqueID idSubject,
  AGCUniqueID idObject1, AGCUniqueID idObject2, long cArgTriplets, ...)
{
  USES_CONVERSION;
  va_list argptr;
  va_start(argptr, cArgTriplets);
  DoTriggerEvent(hListeners, idEvent, pszContext, A2COLE(pszSubject), idSubject,
    idObject1, idObject2, cArgTriplets, argptr);
  va_end(argptr);
}



inline HRESULT CAGCModule::MakeAGCEvent(AGCEventID idEvent,
  IAGCEvent** ppEvent, LPCSTR pszContext,
  LPCOLESTR pszSubject, AGCUniqueID idSubject, long cArgTriplets, ...)
{
  va_list argptr;
  va_start(argptr, cArgTriplets);
  HRESULT hr = GetAGCGlobal()->MakeAGCEvent(idEvent, pszContext,
    pszSubject, idSubject, cArgTriplets, argptr, ppEvent);
  va_end(argptr);
  return hr;
}

inline HRESULT CAGCModule::MakeAGCEvent(AGCEventID idEvent,
  IAGCEvent** ppEvent, LPCSTR pszContext,
  LPCSTR pszSubject, AGCUniqueID idSubject, long cArgTriplets, ...)
{
  USES_CONVERSION;
  va_list argptr;
  va_start(argptr, cArgTriplets);
  HRESULT hr = GetAGCGlobal()->MakeAGCEvent(idEvent, pszContext,
    A2COLE(pszSubject), idSubject, cArgTriplets, argptr, ppEvent);
  va_end(argptr);
  return hr;
}



/////////////////////////////////////////////////////////////////////////////

#endif // !__AGCModule_h__
