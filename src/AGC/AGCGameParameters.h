#pragma once

/*-------------------------------------------------------------------------
 * fedsrv\AGCAGCGameParameters.h
 * 
 * Implementation of CAGCGameParameters
 * 
 * 
 * Owner: 
 * 
 * Copyright 1986-1999 Microsoft Corporation, All Rights Reserved
 *-----------------------------------------------------------------------*/

#include <AGC.h>
#include "resource.h"
#include "GameParamData.h"

/////////////////////////////////////////////////////////////////////////////
// CAGCGameParameters
class ATL_NO_VTABLE CAGCGameParameters : 
  public IDispatchImpl<IAGCGameParameters, &IID_IAGCGameParameters, &LIBID_AGCLib>,
  public ISupportErrorInfo,
  public IPersistStreamInit,
  public IAGCPrivate,
  public AGCObjectSafetyImpl<CAGCGameParameters>,
  public CComCoClass<CAGCGameParameters, &CLSID_AGCGameParameters>,
  public CComObjectRootEx<CComMultiThreadModel>
{
// Declarations
public:
  DECLARE_REGISTRY_RESOURCEID(IDR_AGCGameParameters)
  DECLARE_PROTECT_FINAL_CONSTRUCT()
  DECLARE_GET_CONTROLLING_UNKNOWN()

// Interface Map
public:
  BEGIN_COM_MAP(CAGCGameParameters)
    COM_INTERFACE_ENTRY(IAGCGameParameters)
    COM_INTERFACE_ENTRY(IDispatch)
    COM_INTERFACE_ENTRY(IAGCPrivate)
    COM_INTERFACE_ENTRY(ISupportErrorInfo)
    COM_INTERFACE_ENTRY(IPersist)
    COM_INTERFACE_ENTRY(IPersistStreamInit)
    COM_INTERFACE_ENTRY2(IPersistStream, IPersistStreamInit)
    COM_INTERFACE_ENTRY_AUTOAGGREGATE(IID_IMarshal, m_punkMBV.p, CLSID_TCMarshalByValue)
    COM_INTERFACE_ENTRY(IObjectSafety)
  END_COM_MAP()

// Category Map
public:
  BEGIN_CATEGORY_MAP(CAGCGameParameters)
    IMPLEMENTED_CATEGORY(CATID_AGC)
    IMPLEMENTED_CATEGORY(CATID_SafeForScripting)
    IMPLEMENTED_CATEGORY(CATID_SafeForInitializing)
  END_CATEGORY_MAP()

// Construction / Destruction
public:
  CAGCGameParameters();
  ~CAGCGameParameters();
  void Init(CGameParamData* pMissionParams)
  {
    m_mp = *pMissionParams;
  }

// IAGCPrivate Interface Methods
public:
  STDMETHODIMP_(void*) GetIGCVoid() 
  {
    return &m_mp;
  } 

// IPersist Interface Methods
public:
  STDMETHODIMP GetClassID(CLSID* pClassID);

// IPersistStreamInit Interface Methods
public:
  STDMETHODIMP IsDirty();
  STDMETHODIMP Load(IStream* pStm);
  STDMETHODIMP Save(IStream* pStm, BOOL fClearDirty);
  STDMETHODIMP GetSizeMax(ULARGE_INTEGER* pCbSize);
  STDMETHODIMP InitNew(void);

// ISupportsErrorInfo Interface Methods
public:
  STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// IAGCGameParameters Interface Methods
public:
  STDMETHODIMP Validate();
  STDMETHODIMP put_MinPlayers(BYTE pVal);
  STDMETHODIMP get_MinPlayers(BYTE *pVal);
  STDMETHODIMP put_MaxPlayers(BYTE pVal);
  STDMETHODIMP get_MaxPlayers(BYTE *pVal);
  STDMETHODIMP get_NeutralSectorSpecialAsteroids(short* pVal);
  STDMETHODIMP put_NeutralSectorSpecialAsteroids(short pVal);
  STDMETHODIMP get_PlayerSectorSpecialAsteroids(short* pVal);
  STDMETHODIMP put_PlayerSectorSpecialAsteroids(short pVal);
  STDMETHODIMP get_NeutralSectorMineableAsteroids(short* pVal);
  STDMETHODIMP put_NeutralSectorMineableAsteroids(short pVal);
  STDMETHODIMP get_PlayerSectorMineableAsteroids(short* pVal);
  STDMETHODIMP put_PlayerSectorMineableAsteroids(short pVal);
  STDMETHODIMP get_NeutralSectorTreasureRate(FLOAT* pVal);
  STDMETHODIMP put_NeutralSectorTreasureRate(FLOAT pVal);
  STDMETHODIMP get_PlayerSectorTreasureRate(FLOAT* pVal);
  STDMETHODIMP put_PlayerSectorTreasureRate(FLOAT pVal);
  STDMETHODIMP get_NeutralSectorTreasures(short* pVal);
  STDMETHODIMP put_NeutralSectorTreasures(short pVal);
  STDMETHODIMP get_PlayerSectorTreasures(short* pVal);
  STDMETHODIMP put_PlayerSectorTreasures(short pVal);
  STDMETHODIMP get_NeutralSectorAsteroids(short* pVal);
  STDMETHODIMP put_NeutralSectorAsteroids(short pVal);
  STDMETHODIMP get_PlayerSectorAsteroids(short* pVal);
  STDMETHODIMP put_PlayerSectorAsteroids(short pVal);
  STDMETHODIMP get_MaxRank(short* pVal);
  STDMETHODIMP put_MaxRank(short pVal);
  STDMETHODIMP get_MinRank(short* pVal);
  STDMETHODIMP put_MinRank(short pVal);
  STDMETHODIMP get_Teams(short* pVal);
  STDMETHODIMP put_Teams(short pVal);
  STDMETHODIMP get_AlephPositioning(short* pVal);
  STDMETHODIMP put_AlephPositioning(short pVal);
  STDMETHODIMP get_AlephsPerSector(short* pVal);
  STDMETHODIMP put_AlephsPerSector(short pVal);
  STDMETHODIMP get_NeutralSectors(VARIANT_BOOL* pVal);
  STDMETHODIMP put_NeutralSectors(VARIANT_BOOL pVal);
  STDMETHODIMP get_RandomEncounters(short* pVal);
  STDMETHODIMP put_RandomEncounters(short pVal);
  STDMETHODIMP get_MapSize(short* pVal);
  STDMETHODIMP put_MapSize(short pVal);
  STDMETHODIMP get_MapType(short* pVal);
  STDMETHODIMP put_MapType(short pVal);
  STDMETHODIMP get_GoalTeamKills(short* pVal);
  STDMETHODIMP put_GoalTeamKills(short pVal);
  STDMETHODIMP get_Lives(short* pVal);
  STDMETHODIMP put_Lives(short pVal);
  STDMETHODIMP get_StartingMoney(float* pVal);
  STDMETHODIMP put_StartingMoney(float pVal);
  STDMETHODIMP get_TsiNeutralRegenerate(short* pVal);
  STDMETHODIMP put_TsiNeutralRegenerate(short pVal);
  STDMETHODIMP get_TsiPlayerRegenerate(short* pVal);
  STDMETHODIMP put_TsiPlayerRegenerate(short pVal);
  STDMETHODIMP get_TsiNeutralStart(short* pVal);
  STDMETHODIMP put_TsiNeutralStart(short pVal);
  STDMETHODIMP get_TsiPlayerStart(short* pVal);
  STDMETHODIMP put_TsiPlayerStart(short pVal);
  STDMETHODIMP get_GoalTeamMoney(AGCMoney* pVal);
  STDMETHODIMP put_GoalTeamMoney(AGCMoney pVal);
  STDMETHODIMP get_CivIDs(short element, short* pVal);
  STDMETHODIMP put_CivIDs(short element, short pVal);
  STDMETHODIMP get_ShowHomeSector(VARIANT_BOOL* pVal);
  STDMETHODIMP put_ShowHomeSector(VARIANT_BOOL pVal);
  STDMETHODIMP get_NoTeams(VARIANT_BOOL* pVal);
  STDMETHODIMP put_NoTeams(VARIANT_BOOL pVal);
  STDMETHODIMP get_RandomWormholes(VARIANT_BOOL* pVal);
  STDMETHODIMP put_RandomWormholes(VARIANT_BOOL pVal);
  STDMETHODIMP get_ResourceAmountsVisible(VARIANT_BOOL* pVal);
  STDMETHODIMP put_ResourceAmountsVisible(VARIANT_BOOL pVal);
  STDMETHODIMP get_Resources(VARIANT_BOOL* pVal);
  STDMETHODIMP put_Resources(VARIANT_BOOL pVal);
  STDMETHODIMP get_IsGoalTeamKills(VARIANT_BOOL* pVal);
  STDMETHODIMP put_IsGoalTeamKills(VARIANT_BOOL pVal);
  STDMETHODIMP get_IsGoalTeamMoney(VARIANT_BOOL* pVal);
  STDMETHODIMP put_IsGoalTeamMoney(VARIANT_BOOL pVal);
  STDMETHODIMP get_IsGoalArtifacts(VARIANT_BOOL* pVal);
  STDMETHODIMP put_IsGoalArtifacts(VARIANT_BOOL pVal);
  STDMETHODIMP get_IsGoalConquest(VARIANT_BOOL* pVal);
  STDMETHODIMP put_IsGoalConquest(VARIANT_BOOL pVal);
  STDMETHODIMP get_Drones(VARIANT_BOOL* pVal);
  STDMETHODIMP put_Drones(VARIANT_BOOL pVal);
  STDMETHODIMP get_ScoresCount(VARIANT_BOOL* pVal);
  STDMETHODIMP put_ScoresCount(VARIANT_BOOL pVal);
  STDMETHODIMP get_Stations(VARIANT_BOOL* pVal);
  STDMETHODIMP put_Stations(VARIANT_BOOL pVal);
  STDMETHODIMP get_AllowJoiners(VARIANT_BOOL* pVal);
  STDMETHODIMP put_AllowJoiners(VARIANT_BOOL pVal);
  STDMETHODIMP get_PowerUps(VARIANT_BOOL* pVal);
  STDMETHODIMP put_PowerUps(VARIANT_BOOL pVal);
  STDMETHODIMP get_AllowPrivateTeams(VARIANT_BOOL* pVal);
  STDMETHODIMP put_AllowPrivateTeams(VARIANT_BOOL pVal);
  STDMETHODIMP get_EjectPods(VARIANT_BOOL* pVal);
  STDMETHODIMP put_EjectPods(VARIANT_BOOL pVal);
  STDMETHODIMP get_GameName(BSTR* pVal);
  STDMETHODIMP put_GameName(BSTR pVal);
  STDMETHODIMP get_IGCStaticFile(BSTR *pVal);
  STDMETHODIMP put_IGCStaticFile(BSTR newVal);
  STDMETHODIMP get_GamePassword(BSTR *pVal);
  STDMETHODIMP put_GamePassword(BSTR newVal);
  STDMETHODIMP get_InvulnerableStations(VARIANT_BOOL *pVal);
  STDMETHODIMP put_InvulnerableStations(VARIANT_BOOL newVal);
  STDMETHODIMP get_ShowMap(VARIANT_BOOL *pVal);
  STDMETHODIMP put_ShowMap(VARIANT_BOOL newVal);
  STDMETHODIMP get_AllowDevelopments(VARIANT_BOOL *pVal);
  STDMETHODIMP put_AllowDevelopments(VARIANT_BOOL newVal);
  STDMETHODIMP get_AllowDefections(VARIANT_BOOL *pVal);
  STDMETHODIMP put_AllowDefections(VARIANT_BOOL newVal);
  STDMETHODIMP get_SquadGame(VARIANT_BOOL *pVal);
  STDMETHODIMP put_SquadGame(VARIANT_BOOL newVal);
  STDMETHODIMP get_AllowFriendlyFire(VARIANT_BOOL *pVal);
  STDMETHODIMP put_AllowFriendlyFire(VARIANT_BOOL newVal);
  STDMETHODIMP get_IGCcoreVersion(long *pVal);
  STDMETHODIMP get_GameLength(float *pVal);
  STDMETHODIMP put_GameLength(float newVal);
  STDMETHODIMP get_He3Density(float *pVal);
  STDMETHODIMP put_He3Density(float newVal);
  STDMETHODIMP get_KillPenalty(AGCMoney *pVal);
  STDMETHODIMP put_KillPenalty(AGCMoney newVal);
  STDMETHODIMP get_KillReward(AGCMoney *pVal);
  STDMETHODIMP put_KillReward(AGCMoney newVal);
  STDMETHODIMP get_EjectPenalty(AGCMoney *pVal);
  STDMETHODIMP put_EjectPenalty(AGCMoney newVal);
  STDMETHODIMP get_EjectReward(AGCMoney *pVal);
  STDMETHODIMP put_EjectReward(AGCMoney newVal);
  STDMETHODIMP get_TimeStart(double *pVal);
  STDMETHODIMP get_TimeStartClock(long *pVal);
  STDMETHODIMP get_GoalArtifactsCount(short *pVal);
  STDMETHODIMP put_GoalArtifactsCount(short newVal);
  STDMETHODIMP get_AutoRestart(VARIANT_BOOL *pVal);
  STDMETHODIMP put_AutoRestart(VARIANT_BOOL newVal);   
  STDMETHODIMP get_DefaultCountdown(float *pVal); 
  STDMETHODIMP put_DefaultCountdown(float newVal); 
  STDMETHODIMP get_InitialMinersPerTeam(unsigned char *pVal); 
  STDMETHODIMP put_InitialMinersPerTeam(unsigned char newVal); 
  STDMETHODIMP get_MaxDronesPerTeam(unsigned char *pVal); 
  STDMETHODIMP put_MaxDronesPerTeam(unsigned char newVal); 
  STDMETHODIMP get_CustomMap(BSTR* pVal);
  STDMETHODIMP put_CustomMap(BSTR pVal);
  STDMETHODIMP get_RestartCountdown(float *pVal);
  STDMETHODIMP put_RestartCountdown(float newVal);
  STDMETHODIMP get_TotalMaxPlayers(short* pVal);
  STDMETHODIMP put_TotalMaxPlayers(short newVal);
  STDMETHODIMP get_LockTeamSettings(VARIANT_BOOL* pVal);
  STDMETHODIMP put_LockTeamSettings(VARIANT_BOOL newVal);
  STDMETHODIMP get_InvitationListID(int* pVal);
  STDMETHODIMP put_InvitationListID(int newVal);
  STDMETHODIMP get_IsSquadGame(VARIANT_BOOL* pVal);
  STDMETHODIMP put_IsSquadGame(VARIANT_BOOL newVal);
  STDMETHODIMP get_LockGameOpen(VARIANT_BOOL* pVal);
  STDMETHODIMP put_LockGameOpen(VARIANT_BOOL newVal);
  STDMETHODIMP get_TeamName(short iTeam, BSTR* pVal);
  STDMETHODIMP put_TeamName(short iTeam, BSTR pVal);
  STDMETHODIMP get_IsTechBitOverridden(short iTeam, short iBitID, VARIANT_BOOL* pVal);
  STDMETHODIMP get_OverriddenTechBit(short iTeam, short iBitID, VARIANT_BOOL* pVal);
  STDMETHODIMP put_OverriddenTechBit(short iTeam, short iBitID, VARIANT_BOOL newVal);
  STDMETHODIMP put_SetOverriddenTechBitRange(short iTeam, short iBitID_First, short iBitID_Last, VARIANT_BOOL newVal);
  STDMETHODIMP get_IsGoalFlags(VARIANT_BOOL* pVal);
  STDMETHODIMP put_IsGoalFlags(VARIANT_BOOL newVal);
  STDMETHODIMP get_GoalFlagsCount(short* pVal);
  STDMETHODIMP put_GoalFlagsCount(short pVal);
  STDMETHODIMP get_StoryText(BSTR* pbstrStory);
  STDMETHODIMP put_StoryText(BSTR bstrStory);
  STDMETHODIMP get_AllowEmptyTeams(VARIANT_BOOL* pVal);
  STDMETHODIMP put_AllowEmptyTeams(VARIANT_BOOL pVal);
  STDMETHODIMP get_AutoStart(VARIANT_BOOL *pVal);
  STDMETHODIMP put_AutoStart(VARIANT_BOOL newVal);   
  STDMETHODIMP get_AllowRestart(VARIANT_BOOL *pVal);
  STDMETHODIMP put_AllowRestart(VARIANT_BOOL newVal);
  STDMETHODIMP get_Experimental(VARIANT_BOOL *pVal);    // mmf 10/07 Experimental game type
  STDMETHODIMP put_Experimental(VARIANT_BOOL newVal);   // mmf 10/07 Experimental game type
  STDMETHODIMP get_AllowShipyardPath(VARIANT_BOOL* pVal);
  STDMETHODIMP put_AllowShipyardPath(VARIANT_BOOL pVal);
  STDMETHODIMP get_AllowSupremacyPath(VARIANT_BOOL* pVal);
  STDMETHODIMP put_AllowSupremacyPath(VARIANT_BOOL pVal);
  STDMETHODIMP get_AllowTacticalPath(VARIANT_BOOL* pVal);
  STDMETHODIMP put_AllowTacticalPath(VARIANT_BOOL pVal);
  STDMETHODIMP get_AllowExpansionPath(VARIANT_BOOL* pVal);
  STDMETHODIMP put_AllowExpansionPath(VARIANT_BOOL pVal);
  STDMETHODIMP get_MaxImbalance(short* pVal);
  STDMETHODIMP put_MaxImbalance(short Val);

// Types
protected:
  typedef TCObjectLock<CAGCGameParameters> XLock;

// Data Members
public:
  CComPtr<IUnknown> m_punkMBV; // for custom marshalling

// Enumerations
protected:
  // Indicates that the version is specified in the stream
  enum {c_cHasVersion = -1};
  // Increment this if stream format changes
  enum {c_cCurrentVersion = 0};

// Data Members
protected:
  CGameParamData    m_mp;
  DATE              m_timestart;
  CComBSTR          m_bstrStoryText;
};


/////////////////////////////////////////////////////////////////////////////