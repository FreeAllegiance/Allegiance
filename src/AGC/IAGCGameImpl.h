#pragma once

/////////////////////////////////////////////////////////////////////////////
// IAGCGameImpl.h : Declaration of the IAGCGameImpl class template.
//

#include "IAGCCommonImpl.h"
#include "AGCChat.h"


/////////////////////////////////////////////////////////////////////////////
// Interface Map Macro
//
// Classes derived from IAGCGameImpl should include this macro in their
// interface maps.
//
#define COM_INTERFACE_ENTRIES_IAGCGameImpl()                                \
    COM_INTERFACE_ENTRY(IAGCGame)                                           \
    COM_INTERFACE_ENTRY(IDispatch)                                          \
    COM_INTERFACE_ENTRIES_IAGCCommonImpl()


/////////////////////////////////////////////////////////////////////////////
//
template <class T, class IGC, class ITF, const GUID* plibid>
class ATL_NO_VTABLE IAGCGameImpl:
  public IAGCCommonImpl<T, IGC, ITF, plibid, ImissionIGC, IAGCGame>
{
// Types
public:
  typedef IAGCGameImpl<T, IGC, ITF, plibid> IAGCGameImplBase;

// IAGCGame Interface Methods
public:
  /*-------------------------------------------------------------------------
   * get_Name()
   *-------------------------------------------------------------------------
   * Purpose:
   *   Return the name (or description of the mission)  
   * 
   */
  STDMETHODIMP get_Name(BSTR* pbstr)
  {
    assert(GetIGC());
    assert(GetIGC()->GetMissionParams());
    CComBSTR bstrTemp(GetIGC()->GetMissionParams()->strGameName);
    CLEAROUT(pbstr, (BSTR)bstrTemp);
    bstrTemp.Detach();
    return S_OK;
  }

  /*-------------------------------------------------------------------------
   * CAdminGame::get_Sectors()
   *-------------------------------------------------------------------------
   * Purpose:
   *   Returns the collection of sectors currently in the game.   
   * 
   */
  STDMETHODIMP get_Sectors(IAGCSectors** ppAGCSectors)
  {
    assert(GetIGC());
    assert(GetIGC()->GetClusters());
    return GetAGCGlobal()->GetAGCObject(GetIGC()->GetClusters(),
      IID_IAGCSectors, (void**)ppAGCSectors);
  }

  /*-------------------------------------------------------------------------
   * CAdminGame::get_Teams()
   *-------------------------------------------------------------------------
   * Purpose:
   *   Returns the collection of teams currently in the game.   
   * 
   */
  STDMETHODIMP get_Teams(IAGCTeams** ppAGCTeams)
  {
    assert(GetIGC());
    assert(GetIGC()->GetSides());
    return GetAGCGlobal()->GetAGCObject(GetIGC()->GetSides(), IID_IAGCTeams,
      (void**)ppAGCTeams);
  }

  /*-------------------------------------------------------------------------
   * get_LookupShip()
   *-------------------------------------------------------------------------
   * Purpose:
   *   Converts an AGC ID to a CAGCShip Object.
   *
   * Input:
   *   idAGC: ship id number of the igc object.
   *
   * Output:
   *   a pointer to an AGC object that cooresponds to the id number supplied.
   *
   * Remarks:
   *   The lower 16 bits of the id refers to the object type, the upper bits
   *   refer to the "object id" of that type.
   */
  STDMETHODIMP get_LookupShip(AGCUniqueID idAGC, IAGCShip** ppAGCObject)
  {
    // TODO: Ensure that the specified id IS a ship type

    // Initialize the [out] parameter
    CLEAROUT(ppAGCObject, (IAGCShip*)NULL);

    if ((idAGC>>16) != AGC_Ship)
      return S_OK;

    assert(GetIGC());
    IbaseIGC* pIbaseIGC = GetIGC()->GetBase(OT_ship, short(idAGC));

    if (!pIbaseIGC)
      return S_OK;

    return GetAGCGlobal()->GetAGCObject(pIbaseIGC, IID_IAGCShip,
      (void**)ppAGCObject);
  }

                                                                         
  /*-------------------------------------------------------------------------
   * get_Ships()
   *-------------------------------------------------------------------------
   * Purpose:
   *   Returns the collection of Ships currently in the game.   
   * 
   */
  STDMETHODIMP get_Ships(IAGCShips** ppAGCShips)
  {
    assert(GetIGC());
    assert(GetIGC()->GetShips());
    return GetAGCGlobal()->GetAGCObject(GetIGC()->GetShips(), IID_IAGCShips,
      (void**)ppAGCShips);
  }

  STDMETHODIMP get_Alephs(IAGCAlephs** ppAlephs)
  {
    assert(GetIGC());
    assert(GetIGC()->GetWarps());
    return GetAGCGlobal()->GetAGCObject(GetIGC()->GetWarps(), IID_IAGCAlephs,
      (void**)ppAlephs);
  }

  /*-------------------------------------------------------------------------
   * get_Asteroids()
   *-------------------------------------------------------------------------
   * Purpose:
   *   Returns the collection of Asteroids currently in the game.   
   * 
   */
  STDMETHODIMP get_Asteroids(IAGCAsteroids** ppAsteroids)
  {
    assert(GetIGC());
    assert(GetIGC()->GetAsteroids());
    return GetAGCGlobal()->GetAGCObject(GetIGC()->GetAsteroids(),
      IID_IAGCAsteroids, (void**)ppAsteroids);
  }

  /*-------------------------------------------------------------------------
   * get_GameParameters()
   *-------------------------------------------------------------------------
   */
  STDMETHODIMP get_GameParameters(IAGCGameParameters** ppParams)
  {
    // Initialize the [out] parameter
    CLEAROUT(ppParams, (IAGCGameParameters*)NULL);

    // Create an AGCGameParameters object
    IAGCPrivatePtr spPrivate;
    RETURN_FAILED(spPrivate.CreateInstance(CLSID_AGCGameParameters));
    assert(NULL != spPrivate);

    // Initialize the MissionParams structure of the new object
    assert(GetIGC());
    CopyMemory(spPrivate->GetIGCVoid(), GetIGC()->GetMissionParams(),
      sizeof(*GetIGC()->GetMissionParams()));

    // Return the new object
    return spPrivate->QueryInterface(IID_IAGCGameParameters, (void**)ppParams);
  }


  /*-------------------------------------------------------------------------
   * get_GameId()
   *-------------------------------------------------------------------------
   */
  STDMETHODIMP get_GameID(AGCGameID *pMissionId)
  {
    assert(GetIGC());
    *pMissionId  = GetIGC()->GetMissionID();
    return S_OK;
  }

  /*-------------------------------------------------------------------------
   * get_LookupTeam()
   *-------------------------------------------------------------------------
   * Purpose:
   *   Converts an AGC ID to a CAGCTeam Object.
   *
   * Input:
   *   idAGC: team id number of the igc object.
   *
   * Output:
   *   a pointer to an AGC object that cooresponds to the id number supplied.
   *
   * Remarks:
   *   The lower 16 bits of the id refers to the object type, the upper bits
   *   refer to the "object id" of that type.
   */
  STDMETHODIMP get_LookupTeam(AGCObjectID idAGC, IAGCTeam** ppAGCObject)
  {
    // TODO: Ensure that the specified id IS a ship type

    // Initialize the [out] parameter
    CLEAROUT(ppAGCObject, (IAGCTeam*)NULL);

    assert(GetIGC());
    IbaseIGC* pIbaseIGC = GetIGC()->GetBase(OT_side, idAGC);
    if (!pIbaseIGC)
      return T::Error("ID for Team not found");

    return GetAGCGlobal()->GetAGCObject(pIbaseIGC, IID_IAGCTeam,
      (void**)ppAGCObject);
  }
  
  
  ///////////////////////////////////////////////////////////////////////////  
  STDMETHODIMP SendChat(BSTR bstrText, AGCSoundID idSound)
  {
    assert(GetIGC());
    assert(GetIGC()->GetIgcSite());

    // Do nothing if string is empty
    if (!BSTRLen(bstrText))
      return S_FALSE;

    // Send the chat
    USES_CONVERSION;
    GetIGC()->GetIgcSite()->SendChat(NULL, CHAT_EVERYONE, NA, idSound,
      OLE2CA(bstrText), c_cidNone, NA, NA, NULL, true); 

    // Indicate success
    return S_OK;
  }


  ///////////////////////////////////////////////////////////////////////////  
  STDMETHODIMP SendCommand(BSTR bstrCommand, IAGCModel* pModelTarget,
    AGCSoundID idSound)
  {
    assert(GetIGC());
    assert(GetIGC()->GetIgcSite());

    // Convert the command string to a command ID
    CommandID idCmd;
    RETURN_FAILED(FindCommandName(bstrCommand, &idCmd, T::GetObjectCLSID(),
      IID_IAGCGame));

    // Verify that the specified model supports IAGCPrivate
    IAGCPrivatePtr spPrivate(pModelTarget);
    if (NULL == spPrivate)
      return E_INVALIDARG;

    // Get the target's IGC pointer
    ImodelIGC* pModel = reinterpret_cast<ImodelIGC*>(spPrivate->GetIGCVoid());

    // Send the chat
    GetIGC()->GetIgcSite()->SendChat(NULL, CHAT_EVERYONE, NA, idSound, "",
      idCmd, pModel->GetObjectType(), pModel->GetObjectID(), pModel, true);
      
    // Indicate success
    return S_OK;
  }


  /*-------------------------------------------------------------------------
   * get_GameStage()
   *-------------------------------------------------------------------------
   */
  STDMETHODIMP get_GameStage(AGCGameStage* pStage)
  {
    assert(GetIGC());
    *pStage  = AGCGameStage(GetIGC()->GetMissionStage());
    return S_OK;
  }


  STDMETHODIMP get_Probes(IAGCProbes** ppProbes)
  {
    assert(GetIGC());
    assert(GetIGC()->GetProbes());
    return GetAGCGlobal()->GetAGCObject(GetIGC()->GetProbes(),
      IID_IAGCProbes, (void**)ppProbes);
  }

  STDMETHODIMP get_Buoys(IAGCModels** ppBuoys)
  {
    assert(GetIGC());
    assert(GetIGC()->GetBuoys());
    return GetAGCGlobal()->GetAGCObject(GetIGC()->GetBuoys(),
      IID_IAGCModels, (void**)ppBuoys);
  }

  STDMETHODIMP get_Treasures(IAGCModels** ppTreasures)
  {
    assert(GetIGC());
    assert(GetIGC()->GetTreasures());
    return GetAGCGlobal()->GetAGCObject(GetIGC()->GetTreasures(),
      IID_IAGCModels, (void**)ppTreasures);
  }

  STDMETHODIMP get_Mines(IAGCModels** ppMines)
  {
    assert(GetIGC());
    assert(GetIGC()->GetMines());
    return GetAGCGlobal()->GetAGCObject(GetIGC()->GetMines(),
      IID_IAGCModels, (void**)ppMines);
  }

  STDMETHODIMP get_ReplayCount(short* pnReplays)
  {
    assert(GetIGC());
    CLEAROUT(pnReplays, GetIGC()->GetReplayCount());
    return S_OK;
  }

  STDMETHODIMP get_ContextName(BSTR* pbstrContextName)
  {
    assert(GetIGC());
    CComBSTR bstrTemp(GetIGC()->GetContextName());
    CLEAROUT(pbstrContextName, (BSTR)bstrTemp);
    bstrTemp.Detach();
    return S_OK;
  }

};


/////////////////////////////////////////////////////////////////////////////