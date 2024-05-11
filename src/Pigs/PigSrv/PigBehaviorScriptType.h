#pragma once

/////////////////////////////////////////////////////////////////////////////
// PigBehaviorScriptType.h : Declaration of the CPigBehaviorScriptType class

#include "PigSrv.h"
#include "resource.h"       // main symbols
#include "PigEngine.h"
#include "PigBehaviorType.h"
#include "PigBehaviorScriptMethods.h"
#include "IActiveScriptSiteImpl.h"


/////////////////////////////////////////////////////////////////////////////
// Forward Declarations


/////////////////////////////////////////////////////////////////////////////
// CPigBehaviorScriptType
class ATL_NO_VTABLE CPigBehaviorScriptType : 
  public IDispatchImpl<IPigBehaviorScriptType, &IID_IPigBehaviorScriptType, &LIBID_PigsLib>,
  public ISupportErrorInfo,
  public IActiveScriptSiteImpl<CPigBehaviorScriptType>,
  public CPigBehaviorType,
  public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CPigBehaviorScriptType, &CLSID_PigBehaviorScriptType>
{
// Declarations
public:
  DECLARE_REGISTRY_RESOURCEID(IDR_PigBehaviorScriptType)
  DECLARE_PROTECT_FINAL_CONSTRUCT()

// Category Map
public:
  BEGIN_CATEGORY_MAP(CPigBehaviorScriptType)
    IMPLEMENTED_CATEGORY(CATID_PigObjects)
  END_CATEGORY_MAP()

// Interface Map
public:
  BEGIN_COM_MAP(CPigBehaviorScriptType)
    COM_INTERFACE_ENTRY(IPigBehaviorScriptType)
    COM_INTERFACE_ENTRY(IDispatch)
    COM_INTERFACE_ENTRY(IPigBehaviorType)
    COM_INTERFACE_ENTRY(ISupportErrorInfo)
    COM_INTERFACE_ENTRY(IActiveScriptSite)
  END_COM_MAP()

// Construction / Destruction
public:
  CPigBehaviorScriptType();
  HRESULT FinalConstruct();
  void FinalRelease();
  HRESULT Load(const WIN32_FIND_DATA* pffd, tstring strFileName);
  HRESULT Reload(const WIN32_FIND_DATA* pffd, tstring strFileName);
  void Unload();

// Attributes
public:
  bool IsModified(const WIN32_FIND_DATA* pffd) const;
  HRESULT GetLastScriptError(IActiveScriptError** ppAse);
  tstring GetName();
  tstring GetBaseBehaviorName();
  void SetRecursionError();
  void SetBaseError();
  void SetBaseNonExistant();
  bool IsValid();

// Operations
public:
  void AddBehavior(DWORD dwGITCookie);
  void RemoveBehavior(DWORD dwGITCookie);

// Implementation
protected:
  static CPigEngine& GetEngine();
  HRESULT ParseScriptFile(BSTR bstrFileName, ITCStrings** ppScripts = NULL);
  HRESULT ProcessRootAttributes(IXMLElement* pRoot, bool bValidate);
  HRESULT AddInvokeCommand(IXMLElement* pCmd);
  HRESULT AddInvokeCommand(BSTR bstr);
  HRESULT ProcessScriptElement(IXMLElement* pScr, ITCStrings* pScripts,
    bool bValidate);
  HRESULT CreateScriptingEngine(BSTR bstrProgID);
  void ShutdownScriptingEngine();
  void TriggerBaseError(AGCEventID idEvent);

// IPigBehaviorType Interface Methods
public:
  STDMETHODIMP get_InvokeCommands(ITCStrings** ppStrings);
  STDMETHODIMP get_Name(BSTR* pbstrName);
  STDMETHODIMP get_Description(BSTR* pbstrDescription);
  STDMETHODIMP get_Behaviors(IPigBehaviors** ppBehaviors);
  STDMETHODIMP get_IsScript(VARIANT_BOOL* pbIsScript);
  STDMETHODIMP get_BaseBehaviorType(IPigBehaviorType** ppBaseType);
  STDMETHODIMP put_AppearsValid(VARIANT_BOOL bAppearsValid);
  STDMETHODIMP get_AppearsValid(VARIANT_BOOL* pbAppearsValid);

// IPigBehaviorScriptType Interface Methods
public:
  STDMETHODIMP get_FilePathName(BSTR* pbstrFilePathName);
  STDMETHODIMP get_FileName(BSTR* pbstrFileName);
  STDMETHODIMP get_FileAttributes(FileAttributes* peFileAttributes);
  STDMETHODIMP get_FileModifiedTime(DATE* pdate);
  STDMETHODIMP get_FileText(BSTR* pbstrFileText);
  STDMETHODIMP get_ScriptText(ITCStrings** ppScriptStrings);
  STDMETHODIMP get_ScriptEngineProgID(BSTR* pbstrProgID);
  STDMETHODIMP get_ScriptEngineCLSID(BSTR* pbstrCLSID);

// ISupportErrorInfo Interface Methods
public:
  STDMETHODIMP InterfaceSupportsErrorInfo(REFIID riid);

// IActiveScriptSite Interface Methods
public:
  STDMETHODIMP GetItemInfo(LPCOLESTR, DWORD, IUnknown**, ITypeInfo**);
  STDMETHODIMP OnScriptError(IActiveScriptError* pScriptError);

// Types
protected:
  typedef TCObjectLock<CPigBehaviorScriptType> XLock;
  typedef std::set<DWORD>                      XBehaviors;
  typedef XBehaviors::iterator                 XBehaviorIt;

// Data Members
protected:
  ITCStringsPtr             m_spInvokeCommands;
  tstring                   m_strFileName, m_strName;
  tstring                   m_strBaseBehavior;
  DWORD                     m_dwAttributes;
  FILETIME                  m_ftModified;
  CLSID                     m_clsidEngine;
  IActiveScript* 		    m_spAs;
  IActiveScriptParse*	    m_spAsp;
  IActiveScriptError*	    m_spAse;
  XBehaviors                m_Behaviors;
  bool                      m_bAppearsValid:1;
  CPigBehaviorScriptMethods m_Methods;
};


/////////////////////////////////////////////////////////////////////////////
// Attributes

inline bool CPigBehaviorScriptType::IsModified(const WIN32_FIND_DATA* pffd) const
{
  return pffd->ftLastWriteTime.dwHighDateTime != m_ftModified.dwHighDateTime
    || pffd->ftLastWriteTime.dwLowDateTime != m_ftModified.dwLowDateTime
    || pffd->dwFileAttributes != m_dwAttributes;
}

inline HRESULT CPigBehaviorScriptType::GetLastScriptError(IActiveScriptError** ppAse)
{
  XLock lock(this);
  if (NULL == m_spAse)
    return S_FALSE;
  *ppAse = m_spAse; //m_spAse.Detach();
  return S_OK;
}

inline tstring CPigBehaviorScriptType::GetName()
{
  XLock lock(this);
  return m_strName;
}

inline tstring CPigBehaviorScriptType::GetBaseBehaviorName()
{
  XLock lock(this);
  return m_strBaseBehavior;
}

inline void CPigBehaviorScriptType::SetRecursionError()
{
  XLock lock(this);
  m_bAppearsValid = false;
  TriggerBaseError(PigEventID_ScriptBaseRecursive);
}

inline void CPigBehaviorScriptType::SetBaseError()
{
  XLock lock(this);
  m_bAppearsValid = false;
  TriggerBaseError(PigEventID_ScriptBaseError);
}

inline void CPigBehaviorScriptType::SetBaseNonExistant()
{
  XLock lock(this);
  m_bAppearsValid = false;
  TriggerBaseError(PigEventID_ScriptBaseNonExistant);
}

inline bool CPigBehaviorScriptType::IsValid()
{
  XLock lock(this);
  return m_bAppearsValid;
}


/////////////////////////////////////////////////////////////////////////////
// Implementation

inline CPigEngine& CPigBehaviorScriptType::GetEngine()
{
  return CPigEngine::GetEngine();
}



/////////////////////////////////////////////////////////////////////////////