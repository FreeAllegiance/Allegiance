#ifndef __IAGCSectorImpl_h__
#define __IAGCSectorImpl_h__

/////////////////////////////////////////////////////////////////////////////
// IAGCSectorImpl.h : Declaration of the IAGCSectorImpl class template.

#include "IAGCBaseImpl.h"
#include "AGCChat.h"


/////////////////////////////////////////////////////////////////////////////
// Interface Map Macro
//
// Classes derived from IAGCSectorImpl should include this macro in their
// interface maps.
//
#define COM_INTERFACE_ENTRIES_IAGCSectorImpl()                              \
    COM_INTERFACE_ENTRY(IAGCSector)                                         \
    COM_INTERFACE_ENTRY(IDispatch)                                          \
    COM_INTERFACE_ENTRIES_IAGCBaseImpl()


/////////////////////////////////////////////////////////////////////////////
// IAGCSectorImpl
//
template <class T, class IGC, class ITF, const GUID* plibid>
class ATL_NO_VTABLE IAGCSectorImpl :
  public IAGCBaseImpl<T, IGC, ITF, plibid, IclusterIGC, IAGCSector>
{
// Types
public:
  typedef IAGCSectorImpl<T, IGC, ITF, plibid> IAGCSectorImplBase;

// IAGCSector Interface Methods
public:
  STDMETHODIMP get_Name(BSTR* pbstr)
  {
    if (!GetIGC()) 
      return T::Error("Sector not initialized");
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

  STDMETHODIMP get_Alephs(IAGCAlephs** ppAlephs)
  {
    ZAssert(GetIGC());
    ZAssert(GetIGC()->GetWarps());
    return GetAGCGlobal()->GetAGCObject(GetIGC()->GetWarps(), IID_IAGCAlephs,
      (void**)ppAlephs);
  }

  STDMETHODIMP get_Asteroids(IAGCAsteroids** ppAsteroids)
  {
    ZAssert(GetIGC());
    ZAssert(GetIGC()->GetAsteroids());
    return GetAGCGlobal()->GetAGCObject(GetIGC()->GetAsteroids(),
      IID_IAGCAsteroids, (void**)ppAsteroids);
  }

  STDMETHODIMP get_ScreenX(float * pVal)
  {
    ZAssert(GetIGC());
    CLEAROUT(pVal, GetIGC()->GetScreenX());
    return S_OK;
  }

  STDMETHODIMP get_ScreenY(float * pVal)
  {
    ZAssert(GetIGC());
    CLEAROUT(pVal, GetIGC()->GetScreenY());
    return S_OK;
  }

  ///////////////////////////////////////////////////////////////////////////  
  STDMETHODIMP SendChat(BSTR bstrText, VARIANT_BOOL bIncludeEnemies,
    AGCSoundID idSound)
  {
    ZAssert(GetIGC());
    ZAssert(GetIGC()->GetMission()->GetIgcSite());

    // Do nothing if string is empty
    if (!BSTRLen(bstrText))
      return S_FALSE;

    // Determine the specified chat recipient
    ChatTarget eRecipient = bIncludeEnemies ?
      CHAT_ALL_SECTOR : CHAT_FRIENDLY_SECTOR;

    // Send the chat
    USES_CONVERSION;
    GetIGC()->GetMission()->GetIgcSite()->SendChat(NULL, eRecipient,
      GetIGC()->GetObjectID(), idSound, OLE2CA(bstrText), c_cidNone, NA, NA, NULL, true); 

    // Indicate success
    return S_OK;
  }


  ///////////////////////////////////////////////////////////////////////////  
  STDMETHODIMP SendCommand(BSTR bstrCommand, IAGCModel* pModelTarget,
    VARIANT_BOOL bIncludeEnemies, AGCSoundID idSound)
  {
    ZAssert(GetIGC());
    ZAssert(GetIGC()->GetMission()->GetIgcSite());

    // Convert the command string to a command ID
    CommandID idCmd;
    RETURN_FAILED(FindCommandName(bstrCommand, &idCmd, T::GetObjectCLSID(),
      IID_IAGCSector));

    // Verify that the specified model supports IAGCPrivate
    IAGCPrivatePtr spPrivate(pModelTarget);
    if (NULL == spPrivate)
      return E_INVALIDARG;

    // Get the target's IGC pointer
    ImodelIGC* pModel = reinterpret_cast<ImodelIGC*>(spPrivate->GetIGCVoid());

    // Determine the specified chat recipient
    ChatTarget eRecipient = bIncludeEnemies ?
      CHAT_ALL_SECTOR : CHAT_FRIENDLY_SECTOR;

    // Send the chat
    GetIGC()->GetMission()->GetIgcSite()->SendChat(NULL, eRecipient,
      GetIGC()->GetObjectID(), idSound, "", idCmd, pModel->GetObjectType(),
      pModel->GetObjectID(), pModel, true);
      
    // Indicate success
    return S_OK;
  }

  STDMETHODIMP get_Mines(IAGCModels** ppMines)
  {
    ZAssert(GetIGC());
    ZAssert(GetIGC()->GetMines());
    return GetAGCGlobal()->GetAGCObject(GetIGC()->GetMines(),
      IID_IAGCModels, (void**)ppMines);
  }

  STDMETHODIMP get_Missiles(IAGCModels** ppMissiles)
  {
    ZAssert(GetIGC());
    ZAssert(GetIGC()->GetMissiles());
    return GetAGCGlobal()->GetAGCObject(GetIGC()->GetMissiles(),
      IID_IAGCModels, (void**)ppMissiles);
  }

  STDMETHODIMP get_Probes(IAGCProbes** ppProbes)
  {
    ZAssert(GetIGC());
    ZAssert(GetIGC()->GetProbes());
    return GetAGCGlobal()->GetAGCObject(GetIGC()->GetProbes(),
      IID_IAGCProbes, (void**)ppProbes);
  }

  STDMETHODIMP get_Models(IAGCModels** ppModels)
  {
    ZAssert(GetIGC());
    ZAssert(GetIGC()->GetModels());
    return GetAGCGlobal()->GetAGCObject(GetIGC()->GetModels(),
      IID_IAGCModels, (void**)ppModels);
  }

  STDMETHODIMP get_SelectableModels(IAGCModels** ppModels)
  {
    ZAssert(GetIGC());
    ZAssert(GetIGC()->GetPickableModels());
    return GetAGCGlobal()->GetAGCObject(GetIGC()->GetPickableModels(),
      IID_IAGCModels, (void**)ppModels);
  }

  STDMETHODIMP get_Treasures(IAGCModels** ppTreasures)
  {
    ZAssert(GetIGC());
    ZAssert(GetIGC()->GetTreasures());
    return GetAGCGlobal()->GetAGCObject(GetIGC()->GetTreasures(),
      IID_IAGCModels, (void**)ppTreasures);
  }
};


/////////////////////////////////////////////////////////////////////////////

#endif //__IAGCSectorImpl_h__
