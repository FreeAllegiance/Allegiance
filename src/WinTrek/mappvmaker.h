/*
**
**  File:	mappvmaker.h
**
**  Author: KGJV
**
**  Description:
**
**  History:
*/
// 
#pragma once

class CMapPVCluster;
class ImissionPV;

#include "mapmakerigc.h"

class CwarpPV : public IObject
{
private:
	ImissionPV *m_pMission;
	WarpDef m_warpDef;
	SectorID sectorID;
	CwarpPV* m_destination;
public:
	CwarpPV(ImissionPV *pmission,DataWarpIGC *ds,int dataSize);
    ~CwarpPV(void)
    {
    }
	CwarpPV* GetDestination();
	CMapPVCluster *GetCluster();
	void SetCluster(CMapPVCluster* cluster);
	ImissionPV *GetMyMission() {return m_pMission;}
	WarpID GetObjectID() {return m_warpDef.warpID;}
	void Terminate();
};

typedef  Slist_utl<CwarpPV*>  WarpPVList;
typedef  Slink_utl<CwarpPV*>  WarpPVLink;

class CMapPVCluster : public IObject
{
private:
	ImissionPV *m_pMission;
	DataClusterIGC      m_data;
	int nStationsPerSide[c_cSidesMax]; // = {0, 0, 0, 0, 0, 0};
	WarpPVList m_Warps;
public:
	CMapPVCluster(ImissionPV* pmission, DataClusterIGC *ds,int dataSize);
	~CMapPVCluster();
	WarpPVList *GetWarps() {return &m_Warps; }
	void AddWarp(CwarpPV *warp);
	void DeleteWarp(CwarpPV *warp);
	SectorID GetObjectID() {return m_data.clusterID;}
	float GetScreenX() { return m_data.screenX;}
	float GetScreenY() { return m_data.screenY;}
	int *GetStationsPerSide() { return nStationsPerSide;}
	bool GetHomeSector(void) const { return m_data.bHomeSector; }
};

typedef Slist_utl<CMapPVCluster*> ClusterPVList;
typedef Slink_utl<CMapPVCluster*> ClusterPVLink;

class ImissionPV
{
private:
	Modeler *       m_pmodeler;
	ClusterPVList   m_pClusters;
	WarpPVList      m_pWarps;
public:
		ImissionPV(Modeler * pmodeler);
		~ImissionPV();
		void Clear();
		Modeler *GetModeler() { return m_pmodeler; }
		ClusterPVList*	GetClusters(void) { return &m_pClusters; }
		CMapPVCluster*			GetCluster(SectorID id);
		WarpPVList*		GetWarps(void)  { return &m_pWarps; }
		CwarpPV*				GetWarp(WarpID id);
		void					DeleteWarp(CwarpPV*	pwarp);

		CMapPVCluster*			CreatePVCluster(DataClusterIGC *ds,int dataSize);
		CwarpPV*				CreatePVWrap(DataWarpIGC *ds,int dataSize,CMapPVCluster* pCluster);
};

// The CMapPVData class holds all the information needed to build
// a map preview
//
class CMapPVData
{
    protected:
        static const char * smszClusterName[c_NumClusterNames];

    protected:

		ImissionPV * mpMission;
        const MissionParams * mpMissionParams;


        // Members for storing the sector data.
        //
        CMapPVCluster * mpCluster[50];
        SideID msideIDCluster[50];

        //
        // Local storage.
        //
        SectorID mcTeamClustersPerTeam;
        SectorID mcNeutralClustersPerTeam;
        SectorID mcOtherClusters;
        WarpID mIDNextWarp;


    public:
        CMapPVData();
        ~CMapPVData();

        //
        // Reader functions.
        //
		ImissionPV *   GetMission()
            { return(mpMission); }
        const MissionParams * GetMissionParams()
            { return(mpMissionParams); }

        CMapPVCluster * GetCluster(SectorID sID)
            { return(mpCluster[sID]); }
        SideID GetClusterSide(SectorID sID)
            { return(msideIDCluster[sID]); }

        SectorID GetTeamClustersPerTeam()
            { return(mcTeamClustersPerTeam); }
        SectorID GetNeutralClustersPerTeam()
            { return(mcNeutralClustersPerTeam); }
        SectorID GetClustersPerTeam()
            { return(mcTeamClustersPerTeam + mcNeutralClustersPerTeam); }
        SideID GetTeams()
            { return(mpMissionParams->nTeams); }
        SectorID GetTeamClusters()
            { return(GetTeamClustersPerTeam() * GetTeams()); }
        SectorID GetNeutralClusters()
            { return(GetNeutralClustersPerTeam() * GetTeams()); }
        SectorID GetTotalClusters()
            { return(GetClustersPerTeam() * GetTeams() + mcOtherClusters); }
        SectorID GetOtherClusters()
            { return(mcOtherClusters); }

        short GetPlayerClusterMineableAsteroids()
            { return(mpMissionParams->nPlayerSectorMineableAsteroids); }
        short GetPlayerClusterSpecialAsteroids()
            { return(mpMissionParams->nPlayerSectorSpecialAsteroids); }
        short GetPlayerClusterGenericAsteroids()
            { return(mpMissionParams->nPlayerSectorAsteroids); }
        short GetPlayerClusterTreasure()
            { return(mpMissionParams->nPlayerSectorTreasures); }

        short GetNeutralClusterMineableAsteroids()
            { return(mpMissionParams->nNeutralSectorMineableAsteroids); }
        short GetNeutralClusterSpecialAsteroids()
            { return(mpMissionParams->nNeutralSectorSpecialAsteroids); }
        short GetNeutralClusterGenericAsteroids()
            { return(mpMissionParams->nNeutralSectorAsteroids); }
        short GetNeutralClusterTreasure()
            { return(mpMissionParams->nNeutralSectorTreasures); }

        short GetMinableAsteroids(void)
            {
                return mpMissionParams->nPlayerSectorMineableAsteroids * 
                       GetTeamClusters() +
                       mpMissionParams->nNeutralSectorMineableAsteroids *
                       (GetOtherClusters() + GetNeutralClusters());
            }

        short GetPlayerStartTSI()
            { return mpMissionParams->tsiPlayerStart; }
        short GetNeutralStartTSI()
            { return mpMissionParams->tsiNeutralStart; }

        short GetMaxPlayersOnTeam()
            { return(mpMissionParams->nMaxPlayersPerTeam); }
		short GetMapSize()
			{ return (mpMissionParams->iMapSize); }
        short GetMaxPlayersInMission()
            { return(GetMaxPlayersOnTeam() * GetTeams()); }

		WarpID GetNextWarpID()
            { return(mIDNextWarp++); }

        //
        // Writer functions.
        //
        VOID SetMission(ImissionPV * pMission)
            { mpMission = pMission; }
		VOID SetMissionParams(const MissionParams * pMissionParams)
            { mpMissionParams = pMissionParams; }
        VOID SetTeamClustersPerTeam(SectorID sID)
            { mcTeamClustersPerTeam = sID; }
        VOID SetNeutralClustersPerTeam(SectorID sID)
            { mcNeutralClustersPerTeam = sID; }
        VOID SetOtherClusters(SectorID sID)
            { mcOtherClusters = sID; }

        VOID SetCluster(SectorID sID, CMapPVCluster * pCluster, SideID sideID);
        VOID SetClusterSide(SectorID sID, SideID sideID)
            { msideIDCluster[sID] = sideID; }

} ;

class ImapPVMaker
{
    public:
        static const char*  IsValid(const MissionParams * pmp);
        static const char * Create(const MissionParams*   pmp,
                                   ImissionPV*           pmission);

        virtual MapMakerID  GetMapMakerID() const = 0;
        virtual VOID        SetMapMakerID(MapMakerID mbID) = 0;
        //virtual char const* GetName(void) const = 0;
        //virtual void        SetName(const char* newVal) = 0;

        virtual VOID        GenerateMission(const MissionParams* pmp,
                                            ImissionPV*           pmission) = 0;
};

//
// Base class for the map maker.
//
//
// Shape is a circle (2 alephs in and out of each sector).
// If Teams > 3, neutral sectors get extra aleph to another neutral sector.
// If Players On Team > 30, central player sectors get extra aleph to
//      another team's central player sector.
//
class CmapPVMaker : public ImapPVMaker
{
    protected:
        MapMakerID mMMID;
        char mszName[c_cbDescription + 4];

    public:
        static const char*  IsValid(const MissionParams* pmp)
        {
            return NULL;
        }

        CmapPVMaker();
        virtual ~CmapPVMaker();

        virtual MapMakerID  GetMapMakerID() const           { return(mMMID); }
        virtual VOID        SetMapMakerID(MapMakerID mmID)  { mMMID = mmID; }
        virtual char const* GetName() const
            { return(mszName); }
        virtual VOID        SetName(const char * newVal)
            { strcpy(mszName, newVal); }

        virtual VOID        GenerateMission(const MissionParams * pmp,
											ImissionPV*    pmission);

        //
        // Our own custom helper methods.
        //
        //virtual VOID        GenerateSides(CMapPVData * pMapData);

        virtual VOID        GenerateTeamClusterScreenPosition(
                                            CMapPVData * pMapData,
                                            DataClusterIGC * pdc,
                                            SectorID sID);
        virtual CMapPVCluster * GenerateTeamCluster(CMapPVData * pMapData,
                                            SectorID sID);
        virtual VOID        GenerateNeutralClusterScreenPosition(
                                            CMapPVData * pMapData,
                                            DataClusterIGC * pdc,
                                            SectorID sID);
        virtual CMapPVCluster * GenerateNeutralCluster(CMapPVData * pMapData,
                                            SectorID sID);
        virtual VOID        GenerateRingClusters(CMapPVData * pMapData);

        virtual VOID        GenerateWarp(CMapPVData*      pMapData,
                                         CMapPVCluster*   pCluster,
                                         DataWarpIGC*   pdw);
        virtual VOID        LinkClusters(CMapPVData* pMapData,
                                         SectorID  sID1,
                                         SectorID  sID2,
                                         int       alephType = NA);
        virtual VOID        CrossLinkClusters(CMapPVData * pMapData,
                                            SectorID sFirstID);
        virtual VOID        LinkClusters(CMapPVData * pMapData);
        virtual VOID        GenerateLayout(CMapPVData * pMapData);
} ;

class CmapPVMakerFromIGCFile
{
public:
	const char * GenerateMission(const char * igcfile,
					ImissionPV*    pmission);
};
//
// Derived classes.
// Just use the base class without changes
class   CmapPVMakerSingleRing  :   public CmapPVMaker
{
    public:
        static const char*  IsValid(const MissionParams* pmp)
        {
            return NULL;
        }
};

//

//
// Double ring.
//
// Shape is two concentric circles.  The center circle contains 1
// neutral sector for each team.  The outer circle contains 1
// sector for each 15 players.  Each player sector is connected
// to two neutral sectors.
//

class CmapPVMakerDoubleRing : public CmapPVMaker
{
    public:
        static const char*  IsValid(const MissionParams* pmp)
        {
            return NULL;
        }

        CmapPVMakerDoubleRing();

        virtual VOID        GenerateTeamClusterScreenPosition(
                                            CMapPVData * pMapData,
                                            DataClusterIGC * pdc,
                                            SectorID sID);
        virtual VOID        GenerateNeutralClusterScreenPosition(
                                            CMapPVData * pMapData,
                                            DataClusterIGC * pdc,
                                            SectorID sID);
        virtual VOID        LinkClusters(CMapPVData * pMapData);
} ;

//
// HiLo
//

class CmapPVMakerHiLo : public CmapPVMaker
{
    public:
        static const char*  IsValid(const MissionParams* pmp)
        {
            return NULL;
        }

        CmapPVMakerHiLo();
        virtual VOID        GenerateLayout(CMapPVData * pMapData);
        virtual VOID        GenerateTeamClusterScreenPosition(
                                            CMapPVData * pMapData,
                                            DataClusterIGC * pdc,
                                            SectorID sID);
        virtual VOID        GenerateNeutralClusterScreenPosition(
                                            CMapPVData * pMapData,
                                            DataClusterIGC * pdc,
                                            SectorID sID);
        virtual VOID        LinkClusters(CMapPVData * pMapData);
} ;

//
// HiHigher
//

class CmapPVMakerHiHigher : public CmapPVMaker
{
    public:
        static const char*  IsValid(const MissionParams* pmp)
        {
            return NULL;
        }

        CmapPVMakerHiHigher();
        virtual VOID        GenerateLayout(CMapPVData * pMapData);
        virtual VOID        GenerateTeamClusterScreenPosition(
                                            CMapPVData * pMapData,
                                            DataClusterIGC * pdc,
                                            SectorID sID);
        virtual VOID        GenerateNeutralClusterScreenPosition(
                                            CMapPVData * pMapData,
                                            DataClusterIGC * pdc,
                                            SectorID sID);
        virtual VOID        LinkClusters(CMapPVData * pMapData);
} ;

//
// Star
//

class CmapPVMakerStar : public CmapPVMaker
{
    public:
        static const char*  IsValid(const MissionParams* pmp)
        {
            return NULL;
        }

        CmapPVMakerStar();
        virtual VOID        GenerateLayout(CMapPVData * pMapData);
        virtual VOID        GenerateTeamClusterScreenPosition(
                                            CMapPVData * pMapData,
                                            DataClusterIGC * pdc,
                                            SectorID sID);
        virtual VOID        GenerateNeutralClusterScreenPosition(
                                            CMapPVData * pMapData,
                                            DataClusterIGC * pdc,
                                            SectorID sID);
        virtual VOID        LinkClusters(CMapPVData * pMapData);
} ;

//
// Big ring
//
// Shape is a single ring with 2 neutral sectors between home sectors
//

class CmapPVMakerBigRing : public CmapPVMaker
{
    public:
        static const char*  IsValid(const MissionParams* pmp)
        {
            return NULL;
        }

        CmapPVMakerBigRing();
        virtual VOID        GenerateLayout(CMapPVData * pMapData);
        virtual VOID        GenerateTeamClusterScreenPosition(
                                            CMapPVData * pMapData,
                                            DataClusterIGC * pdc,
                                            SectorID sID);
        virtual VOID        GenerateNeutralClusterScreenPosition(
                                            CMapPVData * pMapData,
                                            DataClusterIGC * pdc,
                                            SectorID sID);
        virtual VOID        LinkClusters(CMapPVData * pMapData);
} ;

//
// Brawl
//
// Shape is a single sector containing all of the starbases.
//

class CmapPVMakerBrawl : public CmapPVMaker
{
    public:
        static const char*  IsValid(const MissionParams* pmp);

        CmapPVMakerBrawl();

        virtual VOID        GenerateLayout(CMapPVData * pMapData);
        virtual VOID        GenerateNeutralClusterScreenPosition(
                                                    CMapPVData * pMapData,
                                                    DataClusterIGC * pdc,
                                                    SectorID sID);
        virtual VOID        LinkClusters(CMapPVData * pMapData);
} ;

//
// Pin wheel.
//
// Shape is one or more neutral sectors with 1 player sector per
// team outstretched from the center.  If TotalPlayers > 30,
// center contains 1 neutral sector per team.  Otherwise, center 
// is 1 neutral sector.  If Teams > 3, neutral sectors are cross
// connected.
//

class CmapPVMakerPinWheel : public CmapPVMaker
{
    public:
        static const char*  IsValid(const MissionParams* pmp);

        CmapPVMakerPinWheel();

        virtual VOID        GenerateLayout(CMapPVData * pMapData);
        virtual VOID        GenerateNeutralClusterScreenPosition(
                                                    CMapPVData * pMapData,
                                                    DataClusterIGC * pdc,
                                                    SectorID sID);
        virtual VOID        LinkClusters(CMapPVData * pMapData);
} ;


//
// Diamond ring.
//
// Shape is one ring of neutral sectors in the center and one ring of
// neutral sectors around the outside.  Each team gets 1 to 3 sectors,
// each one connected to the outside and inside rings with 1 aleph.
//
class CmapPVMakerDiamondRing : public CmapPVMaker
{
    public:
        static const char*  IsValid(const MissionParams* pmp);

        CmapPVMakerDiamondRing();

        virtual VOID        GenerateLayout(CMapPVData * pMapData);
        virtual VOID        GenerateTeamClusterScreenPosition(
                                                    CMapPVData * pMapData,
                                                    DataClusterIGC * pdc,
                                                    SectorID sID);
        virtual VOID        GenerateNeutralClusterScreenPosition(
                                                    CMapPVData * pMapData,
                                                    DataClusterIGC * pdc,
                                                    SectorID sID);
        virtual VOID        LinkClusters(CMapPVData * pMapData);
} ;


//
// SnowFlake.
//
// 3 neutral sectors per team connected in a triangle.  Each triangle
// is then connected to form a double ring (or with teams > 3, a single
// ring with the center sectors cross connected).  Each team gets 1 to 3
// sectors connected to the outside of the neutral ring.
//
class CmapPVMakerSnowFlake : public CmapPVMaker
{
    public:
        static const char*  IsValid(const MissionParams* pmp)
        {
            return NULL;
        }

        CmapPVMakerSnowFlake();

        virtual VOID        GenerateLayout(CMapPVData * pMapData);
        virtual VOID        GenerateTeamClusterScreenPosition(
                                                    CMapPVData * pMapData,
                                                    DataClusterIGC * pdc,
                                                    SectorID sID);
        virtual VOID        GenerateNeutralClusterScreenPosition(
                                                    CMapPVData * pMapData,
                                                    DataClusterIGC * pdc,
                                                    SectorID sID);
        virtual VOID        LinkClusters(CMapPVData * pMapData);
} ;


//
// LargeSplit.
//
// Must have an even number of player sectors.  Each team sector is
// connected to 2 neutral sectors.  Each team has its sectors spread out
// so that they are not connected.
//
class CmapPVMakerLargeSplit : public CmapPVMakerHiHigher
{
    public:
        static const char*  IsValid(const MissionParams* pmp);

        CmapPVMakerLargeSplit();
};

class CmapPVMakerInsideOut : public CmapPVMakerHiHigher
{
    public:
        static const char*  IsValid(const MissionParams* pmp)
        {
            return NULL;
        }

        CmapPVMakerInsideOut();
};

class CmapPVMakerGrid : public CmapPVMaker
{
    public:
        static const char*  IsValid(const MissionParams* pmp)
        {
            return NULL;
        }

        CmapPVMakerGrid();

        virtual VOID        GenerateLayout(CMapPVData * pMapData);
        virtual VOID        GenerateTeamClusterScreenPosition(
                                            CMapPVData * pMapData,
                                            DataClusterIGC * pdc,
                                            SectorID sID);
        virtual VOID        GenerateNeutralClusterScreenPosition(
                                            CMapPVData * pMapData,
                                            DataClusterIGC * pdc,
                                            SectorID sID);
        virtual VOID        LinkClusters(CMapPVData * pMapData);
};

class CmapPVMakerEastWest : public CmapPVMaker
{
    public:
        static const char*  IsValid(const MissionParams* pmp);

        CmapPVMakerEastWest();

        virtual VOID        GenerateLayout(CMapPVData * pMapData);
        virtual VOID        GenerateTeamClusterScreenPosition(
                                            CMapPVData * pMapData,
                                            DataClusterIGC * pdc,
                                            SectorID sID);
        virtual VOID        GenerateNeutralClusterScreenPosition(
                                            CMapPVData * pMapData,
                                            DataClusterIGC * pdc,
                                            SectorID sID);
        virtual VOID        LinkClusters(CMapPVData * pMapData);
};

class CmapPVMakerSplitBase : public CmapPVMaker
{
    public:
        static const char*  IsValid(const MissionParams* pmp);

        CmapPVMakerSplitBase();
        virtual VOID        GenerateLayout(CMapPVData * pMapData);
        virtual VOID        GenerateTeamClusterScreenPosition(
                                            CMapPVData * pMapData,
                                            DataClusterIGC * pdc,
                                            SectorID sID);
        virtual VOID        GenerateNeutralClusterScreenPosition(
                                            CMapPVData * pMapData,
                                            DataClusterIGC * pdc,
                                            SectorID sID);
        virtual VOID        LinkClusters(CMapPVData * pMapData);
} ;