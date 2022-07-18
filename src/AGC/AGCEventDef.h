#ifndef __AGCEventDef_H_
#define __AGCEventDef_H_

/////////////////////////////////////////////////////////////////////////////
// AGCEventDef.h : Declaration of the CAGCEventDef class.
//


/////////////////////////////////////////////////////////////////////////////
// CAGCEventDef
//
class CAGCEventDef
{
// Construction / Destruction
private:
  // A static instance is instantiated automatically upon startup
  CAGCEventDef();
public:
  static void Initialize();
  static void Terminate();

// Types
public:
  typedef std::vector<BSTR> XParamStrings;
  struct XEventDef
  {
    AGCEventID m_id;
    int        m_nIndent;
    LPCOLESTR  m_pszName;
    LPCOLESTR  m_pszEventDescription;  // Use MAKEINTRESOURCE for string ID
    LPCOLESTR  m_pszFormatDescription; // Use MAKEINTRESOURCE for string ID
    WORD       m_wSeverity;
    bool       m_bLogAsNTEvent : 1;
    bool       m_bLogAsDBEvent : 1;
  };
  struct XParseData;
  typedef HRESULT (__fastcall *XStateProc)(XParseData&);
  struct XParseData
  {
    IAGCEvent*     m_pEvent;
    IStream*       m_pStm;
    XParamStrings* m_pParams;
    LPCOLESTR      m_pszVarBegin;
    LPCOLESTR      m_pszVarEnd;
    LPCOLESTR      m_pszInput;
    XStateProc     m_pfnNextState;
    bool           m_bEndOfString;
  };
protected:
  struct XLess
  {
    bool operator()(const XEventDef& a, const XEventDef& b) const
    {
      return a.m_id < b.m_id;
    }
  };
  struct XLessName
  {
    bool operator()(const LPCOLESTR& a, const LPCOLESTR& b) const
    {
      return _wcsicmp(a, b) < 0;
    }
  };
  typedef std::map<LPCOLESTR, AGCEventID, XLessName> XNameMap;
  typedef XNameMap::iterator                         XNameMapIt;

// Attributes
public:
  static const XEventDef* begin();
  static const XEventDef* end();
  static const XEventDef* find(AGCEventID idEvent);
  static HRESULT GetEventName       (AGCEventID idEvent, BSTR* pbstrOut);
  static HRESULT GetEventDescription(AGCEventID idEvent, BSTR* pbstrOut);
  static HRESULT GetEventDescription(IAGCEvent*  pEvent, BSTR* pbstrOut,
    const XEventDef* pDefHint = NULL);
  static HRESULT GetEventParameters (IAGCEvent*  pEvent, XParamStrings&,
    const XEventDef* pDefHint = NULL);
  static bool IDFromName(LPCOLESTR pszEventName, AGCEventID& id);

// Implementation
public:
  static HRESULT GetString(LPCOLESTR psz, BSTR* pbstrOut);
  static HRESULT ExpandFmtString(BSTR bstrFmt, IAGCEvent* pEvent,
    BSTR* pbstrOut);
  static HRESULT ExpandParams(BSTR bstrFmt, IAGCEvent* pEvent,
    XParamStrings& rParams);
protected:
  static HRESULT __fastcall ParseState_WriteInput(XParseData& data);
  static HRESULT __fastcall ParseState_WriteVar  (XParseData& data);
  static HRESULT __fastcall ParseState_End       (XParseData& data);
  static HRESULT __fastcall ParseState_Base      (XParseData& data);
  static HRESULT __fastcall ParseState_InVar     (XParseData& data);
  static HRESULT __fastcall ParseState_ProcessVar(XParseData& data);

// Data Members
protected:
  const static XEventDef             s_EventDefs[];
  const static XEventDef*            s_EventDefs_end;
      #pragma pack(push, 4)
        static LONG                  s_nInitSync;
      #pragma pack(pop)
        static bool                  s_bInitialized;
        static XNameMap*             s_pNameMap;
        static CAGCEventDef          s_Instance;
};


/////////////////////////////////////////////////////////////////////////////
// Attributes

inline
const CAGCEventDef::XEventDef* CAGCEventDef::begin()
{
  return s_EventDefs;
}

inline
const CAGCEventDef::XEventDef* CAGCEventDef::end()
{
  return s_EventDefs_end;
}

inline
const CAGCEventDef::XEventDef* CAGCEventDef::find(AGCEventID idEvent)
{
  XEventDef value = {idEvent};
  const XEventDef* it = std::lower_bound(begin(), end(), value, XLess());
  return (it != end() && !XLess()(value, *it)) ? it : end();
}

inline bool CAGCEventDef::IDFromName(LPCOLESTR pszEventName, AGCEventID& id)
{
  Initialize();
  XNameMapIt it = s_pNameMap->find(pszEventName);
  if (s_pNameMap->end() == it)
    return false;
  id = it->second;
  return true;
}


/////////////////////////////////////////////////////////////////////////////

#endif //__AGCEventDef_H_
