#pragma once

/////////////////////////////////////////////////////////////////////////////

#include <igc.h>

/////////////////////////////////////////////////////////////////////////////
//


template <class T>
class CAGCSite : public T
{
public:

  /*-------------------------------------------------------------------------
   * DestroySideEvent()
   *-------------------------------------------------------------------------
   */
  void DestroySideEvent(IsideIGC* pIsideIGC) 
  {
    // Destroy any existing AGC object associated with the IGC object
    GetAGCGlobal()->RemoveAGCObject(pIsideIGC, true);

    // Allow the server/client site class to terminate the team
    T::DestroySideEvent(pIsideIGC);
  }

  /*-------------------------------------------------------------------------
   * TerminateModelEvent()
   *-------------------------------------------------------------------------
   */
  void TerminateModelEvent(ImodelIGC* model)
  {
    // Destroy any existing AGC object associated with the IGC object
    GetAGCGlobal()->RemoveAGCObject(model, true);

    // Allow the server/client site class to terminate the model
    T::TerminateModelEvent(model);
  }                                      

  /*-------------------------------------------------------------------------
   * TerminateMissionEvent()
   *-------------------------------------------------------------------------
   */
  void TerminateMissionEvent(ImissionIGC* pMission) 
  {
    // Destroy any existing AGC object associated with the IGC object
    GetAGCGlobal()->RemoveAGCObject(pMission, true);

    // Allow the server/client site class to terminate the mission
    T::TerminateMissionEvent(pMission);
  }

  /*-------------------------------------------------------------------------
   * ChangeCluster()
   *-------------------------------------------------------------------------
   */
  void ChangeCluster(IshipIGC* pship,
                     IclusterIGC* pclusterOld,
                     IclusterIGC* pclusterNew)
  {
    LPCSTR      pszShip       = pship       ?       pship->GetName    () : "";
    AGCUniqueID shipID        = pship       ?       pship->GetUniqueID() : -1;
    AGCUniqueID newClusterID  = pclusterOld ? pclusterOld->GetUniqueID() : -1;
    AGCUniqueID oldClusterID  = pclusterNew ? pclusterNew->GetUniqueID() : -1;
    LPCSTR      pszClusterOld = pclusterOld ? pclusterOld->GetName()     : "";
    LPCSTR      pszClusterNew = pclusterNew ? pclusterNew->GetName()     : "";

    LPCSTR pszContext = pship ? pship->GetMission()->GetContextName() : NULL;

    _AGCModule.TriggerContextEvent(NULL, EventID_ShipChangesSectors, pszContext,
      pszShip, shipID, newClusterID, oldClusterID, 4,
      "New Sector"     , VT_I4   , newClusterID,
      "Old Sector"     , VT_I4   , oldClusterID,
      "New Sector Name", VT_LPSTR, pszClusterNew,
      "Old Sector Name", VT_LPSTR, pszClusterOld);

    // Allow the server/client site class to invoke event
    T::ChangeCluster(pship, pclusterOld, pclusterNew);
  }

  /*-------------------------------------------------------------------------
   * KillShipEvent()
   *-------------------------------------------------------------------------
   */
  void KillShipEvent(Time now, IshipIGC* ship, ImodelIGC* launcher,
    float amount, const Vector& p1, const Vector& p2)
  {
    LPCSTR      pszShip     = ship     ?     ship->GetName    () : "";
    AGCUniqueID shipID      = ship     ?     ship->GetUniqueID() : -1;
    AGCUniqueID launcherID  = launcher ? launcher->GetUniqueID() : -1;
    LPCSTR      pszLauncher = launcher ? GetModelName(launcher)  : "";

    HAGCLISTENERS hListeners = GetAGCGlobal()->EventListeners(
      EventID_ShipKilled, shipID, -1, -1);
    if (hListeners)
    {
      // Create AGCVector objects
      IAGCVectorPtr spVector1, spVector2;
      _SVERIFYE(GetAGCGlobal()->MakeAGCVector(&p1, &spVector1));
      _SVERIFYE(GetAGCGlobal()->MakeAGCVector(&p2, &spVector2));

	  // TE 7 lines added to support added zLifepod below
      int IsLifepod = 0;
      IhullTypeIGC*   pht = ship->GetBaseHullType();
	  if (pht != NULL)
	  {
		if (pht->HasCapability(c_habmLifepod)) IsLifepod = 1; 
	  }

      LPCSTR pszContext = ship ? ship->GetMission()->GetContextName() : NULL;

      // Trigger the event - TE add MissionId and zLifepod
	  _AGCModule.TriggerContextEvent(hListeners, EventID_ShipKilled, pszContext,
        pszShip, shipID, -1, -1, 7,                // TE: Added 2 params
        "Launcher"     , VT_I4      , launcherID,
        "LauncherName" , VT_LPSTR   , pszLauncher,
        "Amount"       , VT_R4      , amount,
		"MissionID"    , VT_I4      , ship->GetMission()->GetMissionID(),  // TE: Add the GameID
        "Vector1"      , VT_DISPATCH, (IAGCVector*)spVector1,
        "Vector2"      , VT_DISPATCH, (IAGCVector*)spVector2,
		"zLifepod"     , VT_I4      , IsLifepod);                          // TE: 1 if this ship is a lifepod
    }

    // Allow the server/client site class to invoke event
    T::KillShipEvent(now, ship, launcher, amount, p1, p2);
  }

  /*-------------------------------------------------------------------------
   * UseRipcord()
   *-------------------------------------------------------------------------
   */
  bool UseRipcord(IshipIGC* pship, ImodelIGC* pmodel)
  {
    LPCSTR      pszShip     = pship    ? pship->GetName    () : "";
    AGCUniqueID shipID      = pship    ? pship->GetUniqueID() : -1;
    AGCUniqueID modelID     = pmodel   ? pmodel->GetUniqueID() : -1;
    LPCSTR      pszModel    = pmodel   ? GetModelName(pmodel)  : "";

    HAGCLISTENERS hListeners = GetAGCGlobal()->EventListeners(
      EventID_ShipUsesRipcord, shipID, -1, -1);
    if (hListeners)
    {
      LPCSTR pszContext = pship ? pship->GetMission()->GetContextName() : NULL;

      // Trigger the event
      _AGCModule.TriggerContextEvent(hListeners, EventID_ShipUsesRipcord,
        pszContext, pszShip, shipID, -1, -1, 2,
        "Model"     , VT_I4      , modelID,
        "ModelName" , VT_LPSTR   , pszModel);
    }

    // Allow the server/client site class to invoke event
    return T::UseRipcord(pship, pmodel);
  }

};


/////////////////////////////////////////////////////////////////////////////