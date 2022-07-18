#ifndef __IAGCTeamImpl_h__
#define __IAGCTeamImpl_h__

/////////////////////////////////////////////////////////////////////////////
// IAGCTeamImpl.h : Declaration of the IAGCTeamImpl class template.
//

#include "IAGCBaseImpl.h"
#include "AGCChat.h"


/////////////////////////////////////////////////////////////////////////////
// Interface Map Macro
//
// Classes derived from IAGCTeamImpl should include this macro in their
// interface maps.
//
#define COM_INTERFACE_ENTRIES_IAGCTeamImpl()                                \
    COM_INTERFACE_ENTRY(IAGCTeam)                                           \
    COM_INTERFACE_ENTRY(IDispatch)                                          \
    COM_INTERFACE_ENTRIES_IAGCBaseImpl()


/////////////////////////////////////////////////////////////////////////////
// IAGCTeamImpl
//
template <class T, class IGC, class ITF, const GUID* plibid>
class ATL_NO_VTABLE IAGCTeamImpl :
  public IAGCBaseImpl<T, IGC, ITF, plibid, IsideIGC, IAGCTeam>
{
// Types
public:
  typedef IAGCTeamImpl<T, IGC, ITF, plibid> IAGCTeamImplBase;

// IAGCTeam Interface Methods
public:
  STDMETHODIMP get_Name(BSTR* pbstr)
  {
    ZAssert(GetIGC());
    CComBSTR bstrTemp(GetIGC()->GetName()); 
    CLEAROUT(pbstr, (BSTR)bstrTemp);
    bstrTemp.Detach();
    return S_OK;
  }

  STDMETHODIMP get_Stations(IAGCStations** ppStations)
  {
    ZAssert(GetIGC());
    ZAssert(GetIGC()->GetStations());
    return GetAGCGlobal()->GetAGCObject(GetIGC()->GetStations(),
      IID_IAGCStations, (void**)ppStations);
  }

  STDMETHODIMP get_Ships(IAGCShips** ppShips)
  {
    ZAssert(GetIGC());
    ZAssert(GetIGC()->GetShips());
    return GetAGCGlobal()->GetAGCObject(GetIGC()->GetShips(), IID_IAGCShips,
      (void**)ppShips);
  }

  STDMETHODIMP get_Civ(BSTR* pbstr)
  {
    ZAssert(GetIGC());

    const char * pCivName;
    IcivilizationIGC * p = GetIGC()->GetCivilization();
    if(p)
        pCivName = p->GetName();
    else 
        pCivName = "";
       
    CComBSTR bstrTemp(pCivName); 
    CLEAROUT(pbstr, (BSTR)bstrTemp);
    bstrTemp.Detach();
    return S_OK;
  }

  ///////////////////////////////////////////////////////////////////////////  
  STDMETHODIMP SendChat(BSTR bstrText, short idWing, AGCSoundID idSound)
  {
    ZAssert(GetIGC());
    ZAssert(GetIGC()->GetMission()->GetIgcSite());

    // Do nothing if string is empty
    if (!BSTRLen(bstrText))
      return S_FALSE;

    // Determine the specified chat recipient
    ChatTarget eRecipient;
    ObjectID  idRecipient;
    if (-1 == idWing)
    {
      eRecipient = CHAT_TEAM;
      idRecipient = GetIGC()->GetObjectID();
    }
    else
    {
      eRecipient = CHAT_WING;
      idRecipient = idWing;
    }

    // Send the chat
    USES_CONVERSION;
    GetIGC()->GetMission()->GetIgcSite()->SendChat(NULL, eRecipient,
      idRecipient, idSound, OLE2CA(bstrText), c_cidNone, NA, NA, NULL, true);

    // Indicate success
    return S_OK;
  }


  ///////////////////////////////////////////////////////////////////////////  
  STDMETHODIMP SendCommand(BSTR bstrCommand, IAGCModel* pModelTarget,
    short idWing, AGCSoundID idSound)
  {
    ZAssert(GetIGC());
    ZAssert(GetIGC()->GetMission()->GetIgcSite());

    // Convert the command string to a command ID
    CommandID idCmd;
    RETURN_FAILED(FindCommandName(bstrCommand, &idCmd, T::GetObjectCLSID(),
      IID_IAGCTeam));

    // Verify that the specified model supports IAGCPrivate
    IAGCPrivatePtr spPrivate(pModelTarget);
    if (NULL == spPrivate)
      return E_INVALIDARG;

    // Get the target's IGC pointer
    ImodelIGC* pModel = reinterpret_cast<ImodelIGC*>(spPrivate->GetIGCVoid());

    // Determine the specified chat recipient
    ChatTarget eRecipient;
    ObjectID  idRecipient;
    if (-1 == idWing)
    {
      eRecipient = CHAT_TEAM;
      idRecipient = GetIGC()->GetObjectID();
    }
    else
    {
      eRecipient = CHAT_WING;
      idRecipient = idWing;
    }

    // Send the chat
    GetIGC()->GetMission()->GetIgcSite()->SendChat(NULL, eRecipient,
      idRecipient, idSound, "", idCmd, pModel->GetObjectType(),
      pModel->GetObjectID(), pModel, true);
      
    // Indicate success
    return S_OK;
  }

};


/////////////////////////////////////////////////////////////////////////////

#endif //__IAGCTeamImpl_h__
