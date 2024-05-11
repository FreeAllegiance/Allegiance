/////////////////////////////////////////////////////////////////////////////
// PigBehaviorScriptMethods.h : Declaration of the CPigBehaviorScriptMethods
// class

#pragma once

#include <TCLib.h>
#include <..\TCLib\LookupTable.h>


/////////////////////////////////////////////////////////////////////////////
// Forward Declarations

class CPigBehaviorScriptType;


/////////////////////////////////////////////////////////////////////////////
// CPigBehaviorScriptMethods
class CPigBehaviorScriptMethods
{
// Types
public:
  enum EMethod
  {
    EMethod_OnActivate,
    EMethod_OnDeactivate,
    EMethod_OnStateNonExistant,
    EMethod_OnStateLoggingOn,
    EMethod_OnStateLoggingOff,
    EMethod_OnStateMissionList,
    EMethod_OnStateCreatingMission,
    EMethod_OnStateJoiningMission,
    EMethod_OnStateQuittingMission,
    EMethod_OnStateTeamList,
    EMethod_OnStateJoiningTeam,
    EMethod_OnStateWaitingForMission,
    EMethod_OnStateDocked,
    EMethod_OnStateLaunching,
    EMethod_OnStateFlying,
    EMethod_OnStateTerminated,
    EMethod_OnMissionStarted,
    EMethod_OnReceiveChat,
    EMethod_OnShipDamaged,
    EMethod_OnShipKilled,
    EMethod_OnSectorChange,
    EMethod_OnAlephHit,
    EMethodCount,
  };
protected:
  struct XMethodInfo
  {
    LPCOLESTR m_pszName;
    short     m_cParams;
  };

// Construction
public:
  CPigBehaviorScriptMethods();

// Attributes
public:
  DISPID GetMethodID(EMethod eMethod) const;

// Operations
public:
  static HRESULT ResolveMethod(REFCLSID clsid, ITypeComp* ptc,
    LPCOLESTR pszMethodName, short cParams, DISPID* pid,
    CPigBehaviorScriptType* pType);
  static HRESULT ResolveMethod(REFCLSID clsid, IDispatch* pd,
    LPCOLESTR pszMethodName, short cParams, DISPID* pid,
    CPigBehaviorScriptType* pType);
  HRESULT ResolveMethods(REFCLSID clsid, IDispatch* pd,
    CPigBehaviorScriptType* pType);

// Data Members
protected:
  DISPID m_dispids[EMethodCount];
  TCLookupTable_DECLARE(Methods, EMethod, XMethodInfo)
};


/////////////////////////////////////////////////////////////////////////////
// Construction

inline CPigBehaviorScriptMethods::CPigBehaviorScriptMethods()
{
  // Initialize the array of DISPID's
  for (DISPID* it = m_dispids; it < (m_dispids + EMethodCount); ++it)
    *it = DISPID_UNKNOWN;
}


/////////////////////////////////////////////////////////////////////////////
// Attributes

inline DISPID CPigBehaviorScriptMethods::GetMethodID(EMethod eMethod) const
{
  return m_dispids[eMethod];
}


/////////////////////////////////////////////////////////////////////////////