/*
**  Copyright (C) 1996, 1997 Microsoft Corporation. All Rights Reserved.
**
**  File:	mapmakerigc.cpp
**
**  Author: Jeff Fink (jfink@microsoft.com)
**
**  Description:
**
**  History:
*/

#include    "mapmakerigc.h"


//
// Static members.
// 08/30/04 -KGJV: added Titty Baby memorial sector name
// 12/14/11 -pkk: added Tj Butler memorial sector name
// 5/28/2019 - BT - Added MrChaos, Wasp to memorial sector list. 
const char * CMapData::smszClusterName[c_NumClusterNames] =
{
    "Aegis",        "Aeroflex",     "Ahto",         "Aredne",
    "Blenhem",      "Borsonis",     "Bragi",        "Caladonia",	
	"Cirt",			"Claytow",      "Denom",        "Dunatis",    
	"Eleesh",		"Enlil",        "Fejfar",       "Fenris",       
	"Fierro",		"Freya",        "Gala",         "Gery",
    "Gobo",         "Goron",        "Holokine",		"Jol",
    "Karlo",        "Kassano",      "Kassimier",    "Kik",
    "Kilvero",      "Kishoten",     "Kuat",         "L'Sau",
    "Larchis",      "Lemminakean",  "Leighton",     "Louhi",
    "Loviatar",     "Luerentia",	"Meesh",		"Mercor",
	"Miekko",       "Mielikki",     "Monmar",		"MrChaos",
    "Nadir",        "Night",        "Oblivion",		"Okanagon",     
	"Oxyl",			"Pohjola",	    "Raiko",        "Resnik",       
	"Rib'zki",		"Rimin",		"Sleipener",	"Solitaire",    
	"Soron",        "Sri Metsa",    "Surma",		"Tathlum",      
	"Tierankeen",   "Tjeerd",       "Torc",			"Touni",        
	"Tauceti",      "Tj Butler",    "Titty Baby",	"Turakeen",   
	"Udar",			"Virkinow",		"Wasp",			"Xuping"
} ;

const char * CmapMakerIGC::smszPlanetName[c_NumberOfPlanetPosters] =
{
    "nebplnt36bmp",
    "nebplnt35bmp",
    "nebplnt02bmp",
    "nebplnt23bmp",
    "nebplnt19bmp",
    "nebplnt01bmp",
	"nebplnt03bmp",
	"nebplnt04bmp",
	"nebplnt05bmp",
	"nebplnt06bmp",
	"nebplnt07bmp",
	"nebplnt08bmp",
	"nebplnt09bmp",
	"nebplnt10bmp",
	"nebplnt11bmp",
	"nebplnt12bmp",
	"nebplnt16bmp",
	"nebplnt19bmp",
	"nebplnt24bmp",
	"nebplnt25bmp",
	"nebplnt29bmp",
	"nebplnt30bmp",
	"nebplnt31bmp",
	"nebplnt33bmp",
	"nebplnt37bmp"
} ;

//
// Map data helper class.
//

CMapData::CMapData()
{
    SectorID sID;

    mpMission = NULL;
    mpMissionParams = NULL;

    ZeroMemory(mpCluster, sizeof(mpCluster));
    for(sID = 0; sID < c_NumClusterNames; sID++)
        mClusterNamesLeft[sID] = sID;
    mcClusterNamesLeft = c_NumClusterNames;


    mcTeamClustersPerTeam = 0;
    mcNeutralClustersPerTeam = 0;
    mcOtherClusters = 0;
    mIDNextWarp = 0;
}


CMapData::~CMapData()
{
    SectorID sID;

    for(sID = 0; sID < c_MaxClustersPerMap; sID++)
    {
        if (NULL != mpCluster[sID])
            mpCluster[sID]->Release();  
    }
}


VOID CMapData::SetCluster(SectorID sID, IclusterIGC * pCluster, SideID sideID)
{
    if (NULL != pCluster)
        pCluster->AddRef();
    if (NULL != mpCluster[sID])
        mpCluster[sID]->Release();
    mpCluster[sID] = pCluster;
    msideIDCluster[sID] = sideID;
}


VOID CMapData::GetNewClusterName(CHAR * szClusterName)
{
    INT n = randomInt(0, mcClusterNamesLeft - 1);

    strcpy(szClusterName, smszClusterName[mClusterNamesLeft[n]]);
    mcClusterNamesLeft--;
    mClusterNamesLeft[n] = mClusterNamesLeft[mcClusterNamesLeft];
}


const char* ImapMakerIGC::IsValid(const MissionParams* pmp)
{
    switch (pmp->mmMapType)
    {
        case c_mmSingleRing:
            return CmapMakerSingleRingIGC::IsValid(pmp);
        case c_mmDoubleRing:
            return CmapMakerDoubleRingIGC::IsValid(pmp);
        case c_mmPinWheel:
            return CmapMakerPinWheelIGC::IsValid(pmp);
        case c_mmDiamondRing:
            return CmapMakerDiamondRingIGC::IsValid(pmp);
        case c_mmSnowFlake:
            return CmapMakerSnowFlakeIGC::IsValid(pmp);
        case c_mmSplitBase:
            return CmapMakerSplitBaseIGC::IsValid(pmp);
        case c_mmLargeSplit:
            return CmapMakerLargeSplitIGC::IsValid(pmp);
        case c_mmBrawl:
            return CmapMakerBrawlIGC::IsValid(pmp);
        case c_mmBigRing:
            return CmapMakerBigRingIGC::IsValid(pmp);
        case c_mmHiLo:
            return CmapMakerHiLoIGC::IsValid(pmp);
        case c_mmHiHigher:
            return CmapMakerHiHigherIGC::IsValid(pmp);
        case c_mmStar:
            return CmapMakerStarIGC::IsValid(pmp);
        case c_mmInsideOut:
            return CmapMakerInsideOutIGC::IsValid(pmp);
        case c_mmGrid:
            return CmapMakerGridIGC::IsValid(pmp);
        case c_mmEastWest:
            return CmapMakerEastWestIGC::IsValid(pmp);
    }

    return "Invalid map type";
}

void    ImapMakerIGC::Create(Time                   now,
                             const MissionParams*   pmp,
                             ImissionIGC*           pmission)
{
    ZAssert (pmp->nTeams >= 2);
    ZAssert (pmp->nTeams <= c_cSidesMax);

#define MakeMap(x)  case c_mm##x:                                   \
                    {                                               \
                        CmapMaker##x##IGC mm;                       \
                        mm.GenerateMission(now, pmp, pmission);     \
                    }                                               \
                    break;                                          \

    switch(pmp->mmMapType)
    {
        MakeMap(SingleRing)
        MakeMap(DoubleRing)
        MakeMap(PinWheel)
        MakeMap(DiamondRing)
        MakeMap(SnowFlake)
        MakeMap(SplitBase)
        MakeMap(Brawl)
        MakeMap(BigRing)
        MakeMap(HiLo)
        MakeMap(HiHigher)
        MakeMap(Star)
        MakeMap(InsideOut)
        MakeMap(Grid)
        MakeMap(EastWest)
        MakeMap(LargeSplit)
        default:
            ZAssert (false);
    }
#undef MakeMap
}

//
// Base class for the map maker.
//
CmapMakerIGC::CmapMakerIGC()
{
    mMMID = c_mmSingleRing;
    this->SetName("Single Ring");
}


CmapMakerIGC::~CmapMakerIGC()
{
}

static bool    FindPath(IclusterIGC*    p2,
                        int*            pStart,
                        int*            pStop,
                        IclusterIGC**   pclustersAdjacent,
                        IclusterIGC**   pclusterBackTrack,
                        bool*           bVisited)
{
    if (*pStart == *pStop)
        return false;

    IclusterIGC*    p1 = pclustersAdjacent[*pStart];
    ZAssert (p1);

    if (p1 == p2)
        return true;

    (*pStart)++;

    for (WarpLinkIGC*   pwl = p1->GetWarps()->first(); (pwl != NULL); pwl = pwl->next())
    {
        IclusterIGC*    p = pwl->data()->GetDestination()->GetCluster();
        SectorID        id = p->GetObjectID();

        if ((pclusterBackTrack[id] == NULL) && !bVisited[id])
        {
            pclusterBackTrack[id] = p1;
            pclustersAdjacent[*pStop] = p;

            (*pStop)++;
        }
    }

    return FindPath(p2, pStart, pStop,
                    pclustersAdjacent,
                    pclusterBackTrack,
                    bVisited);
}

static bool    EnoughPaths(IclusterIGC**            pclusterBackTrack,
                           bool*                    bVisited,
                           const ClusterListIGC*    pclusters,
                           IwarpIGC*                pwarp,
                           int                      count)
{
    IclusterIGC*    pcluster1 = pwarp->GetCluster();
    IclusterIGC*    pcluster2 = pwarp->GetDestination()->GetCluster();

    for (int i = pclusters->n() - 1; (i >= 0); i--)
    {
        pclusterBackTrack[i] = NULL;
    }

    IclusterIGC**   pclustersAdjacent = (IclusterIGC**)alloca(pclusters->n() * pclusters->n() * sizeof(IclusterIGC*));
    int         nStart = 0;
    int         nStop = 0;
    for (WarpLinkIGC*   pwl = pcluster1->GetWarps()->first(); (pwl != NULL); pwl = pwl->next())
    {
        IwarpIGC*   pw = pwl->data();
        if (pw != pwarp)
        {
            IclusterIGC*    pcluster = pw->GetDestination()->GetCluster();
            SectorID        id = pcluster->GetObjectID();

            if (!bVisited[id])
            {
                pclustersAdjacent[nStop++] = pcluster;
                pclusterBackTrack[id] = pcluster1;
            }
        }
    }

    if (!FindPath(pcluster2, &nStart, &nStop, pclustersAdjacent, pclusterBackTrack, bVisited))
    {
        return false;
    }
    else if (count == 1)
    {
        return true;
    }
    else
    {
        //Mark everything on the path from p1 to p2 as visited so they can be used
        //by subsequent paths
        //Note: p2 is never marked as visited
        //      p1 is marked as visited (not that we care)
        IclusterIGC*    pcluster = pcluster2;
        SectorID    id = pcluster->GetObjectID();
        do
        {
            ZAssert (pclusterBackTrack[id] != NULL);

            pcluster = pclusterBackTrack[id];
            id = pcluster->GetObjectID();

            bVisited[id] = true;
        }
        while (pcluster != pcluster1);

        return EnoughPaths(pclusterBackTrack,
                           bVisited,
                           pclusters,
                           pwarp,
                           --count);
    }
}

static bool OkToRemove(const ClusterListIGC*    pclusters, IwarpIGC* pwarp)
{
    IclusterIGC**   pclusterBackTrack = (IclusterIGC**)alloca(pclusters->n() * sizeof(IclusterIGC*));
    bool*           bVisited = (bool*)alloca(pclusters->n() * sizeof(bool));

    {
        for (int i = pclusters->n() - 1; (i >= 0); i--)
            bVisited[i] = false;
    }

    bVisited[pwarp->GetCluster()->GetObjectID()] = true;

    return EnoughPaths(pclusterBackTrack, bVisited, pclusters, pwarp, 2);
}

VOID CmapMakerIGC::GenerateMission(Time now,
                                    const MissionParams * pmp,
                                    ImissionIGC * pMission)
{
    CMapData MapData;

    ZAssert(NULL == ImapMakerIGC::IsValid(pmp));

    MapData.SetTime(now);
    MapData.SetMissionParams(pmp);
    MapData.SetMission(pMission);

    //
    // First generate the sides.
    //
    //this->GenerateSides(&MapData);

    //
    // Next generate the sector layout.
    //
    this->GenerateLayout(&MapData);

    {
        //Adjust the position of all alephs in all clusters
        float   majorRadius = pMission->GetFloatConstant(c_fcidRadiusUniverse);
        for (ClusterLinkIGC*    pcl = pMission->GetClusters()->first(); (pcl != NULL); pcl = pcl->next())
        {
            IclusterIGC*    pcluster = pcl->data();

            const WarpListIGC*  pwarps = pcluster->GetWarps();
            if (pwarps->n() != 0)
            {
                float   nWarps = (float)(pwarps->n());

                const int c_maxWarps = 10;
                ZAssert (pwarps->n() <= c_maxWarps);

                float   offset[c_maxWarps] = {0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f};
                float   displacement;
                if (pwarps->n() > 1)
                {
                    displacement =  sin(pi/nWarps) * 2.0f / 3.0f;
                    for (int index = 0; (index < pwarps->n()); index++)
                    {
                        int swap = randomInt(0, pwarps->n() - 1);
                        if (swap != index)
                        {
                            float   t = offset[index];
                            offset[index] = offset[swap];
                            offset[swap] = t;
                        }
                    }
                }
                else
                    displacement = 2.0f / 3.0f;

                {
                    float   bias = random(0.0f, 2.0f * pi);
                    int     index = 0;

                    for (WarpLinkIGC*   pwl = pwarps->first(); (pwl != NULL); pwl = pwl->next())
                    {
                        IwarpIGC*   pwarp = pwl->data();

                        float       r = pwarp->GetPosition().z * majorRadius;

                        float       angle = bias + offset[index++] * (2.0f * pi) / nWarps;

                        Vector      position;
                        position = Vector::RandomPosition(r * displacement);

                        position.x  += cos(angle) * r;
                        position.y  += sin(angle) * r;
                        position.z  *= 1.2f;

                        pwarp->SetPosition(position);
                        Orientation o(-position);
                        pwarp->SetOrientation(o);

                        pwarp->GetHitTest()->UpdateStaticBB();
                    }
                }
            }
        }
    }

    //Randomize the map if appropriate
    if (pmp->iRandomEncounters > 1)
    {
        const WarpListIGC*      pwarpList = pMission->GetWarps();
        IwarpIGC**  pwarps = (IwarpIGC**)(alloca(pwarpList->n() * sizeof(IwarpIGC*)));

        //Actually allocate twice as much space as we need since we are only storing a single instance for
        //both sides of the aleph
        int nWarps = 0;
        {
            for (WarpLinkIGC*   pwl = pwarpList->first(); (pwl != NULL); pwl = pwl->next())
            {
                IwarpIGC*   pwarp = pwl->data();
                if ((pwarp->GetObjectID() < pwarp->GetDestination()->GetObjectID()) &&
                    (randomInt(0, pmp->iRandomEncounters) > 1))
                {
                    ZAssert (nWarps < pwarpList->n());
                    pwarps[nWarps++] = pwarp;
                }
            }
        }

        const ClusterListIGC*   pclusters = pMission->GetClusters();
        while (nWarps > 0)
        {
            int         index = randomInt(0, nWarps - 1);
            IwarpIGC*   pwarp = pwarps[index];

            //Can we remove pwarp without destroying the map's connectivity?
            if (OkToRemove(pclusters, pwarp))
            {
                pwarp->GetDestination()->Terminate();
                pwarp->Terminate();
            }

            pwarps[index] = pwarps[--nWarps];
        }
    }

    this->PopulateClusters(&MapData);

    //
    // If the show home sector flag is on, show it to the teams.
    //
    if (TRUE == pmp->bShowHomeSector)
        this->RevealHomeClusters(pMission);

    if (TRUE == pmp->bAllowAlliedViz)
        this->RevealAlliedClusters(pMission);

    if (TRUE == pmp->bShowMap)
        this->RevealMap(pMission);

    //
    // We're done with everything, so activate the sides.
    //
    this->ActivateSides(pMission);
}


//
// Base class implementation of the map maker.
//
/*
VOID CmapMakerIGC::GenerateSides(CMapData * pMapData)
{
    static const float sideColors[c_cSidesMax][3] =
                                { {200.0f/255.0f,  15.0f/255.0f, 200.0f/255.0f}, //purple
                                  {  8.0f/255.0f, 184.0f/255.0f, 184.0f/255.0f}, //teal
                                  {  0.0f/255.0f,   0.0f/255.0f, 233.0f/255.0f}, //blue
                                  {184.0f/255.0f, 184.0f/255.0f,  50.0f/255.0f}, //icky yellow
                                  {184.0f/255.0f,  92.0f/255.0f,   0.0f/255.0f}, //icky orange
                                  {123.0f/255.0f,  61.0f/255.0f,  61.0f/255.0f}};//icky magenta
    static const char* sideNames[c_cSidesMax] =
                        { "Crusaders",
                          "Unity",
                          "Survivors",
                          "Protectorate",
                          "Colossal Mining Corp",
                          "Midnight Runners" };

    for (SideID sid = 0; sid < pMapData->GetTeams(); sid++)
    {
        IcivilizationIGC*   pcivilization = pMapData->GetMission()->
            GetCivilization(pMapData->GetMissionParams()->rgCivID[sid]);

        DataSideIGC ds;
        ds.sideID = sid;
        ds.civilizationID = pMapData->GetCivID(sid);
        ds.color.SetRGBA(sideColors[sid][0],
                         sideColors[sid][1],
                         sideColors[sid][2]);
        strcpy(ds.name, sideNames[sid]);
        ds.ttbmDevelopmentTechs = pcivilization->GetBaseTechs();
        ds.conquest = 100 / pMapData->GetTeams();
        ds.nKills = ds.nDeaths = 0;

        IObject*    o = pMapData->GetMission()->CreateObject(
                                                pMapData->GetTime(),
                                                OT_side,
                                                &ds,
                                                sizeof(ds));
        ZAssert (o);
        o->Release();
    }

    ZAssert (pMapData->GetMission()->GetSides()->n() == pMapData->GetTeams());
}
*/


VOID CmapMakerIGC::GenerateTeamClusterScreenPosition(CMapData * pMapData,
                                                    DataClusterIGC * pdc,
                                                    SectorID sID)
{
    float flTheta;

    flTheta = (float) (2.0 * pi * sID / pMapData->GetTotalClusters());

    pdc->screenX = cos(flTheta) * 0.8f;
    pdc->screenY = sin(flTheta) * 0.8f;
}


IclusterIGC * CmapMakerIGC::GenerateTeamCluster(CMapData * pMapData,
                                                SectorID sID)
{
    DataClusterIGC dc;
    INT n; 

    dc.bHomeSector = true;

    dc.clusterID = sID;
    dc.activeF = true;

    dc.lightColor = 0xffffffff;
    dc.lightDirection = Vector::RandomDirection();

    pMapData->GetNewClusterName(dc.name);

    strcpy(dc.posterName, "globe");
    dc.posterName[5] = '0' + randomInt(1, 8);
    dc.posterName[6] = '\0';

    n = randomInt(0, c_NumberOfPlanetPosters - 1);
    strcpy(dc.planetName, smszPlanetName[n]);
    dc.planetSinLatitude = random(0.0f, 1.0f);
    dc.planetLongitude = random(0.0f, 1.0f);
    dc.planetRadius = (unsigned char)randomInt(50, 100);

    dc.starSeed = rand();
    dc.nDebris = randomInt(250, 750);
    dc.nStars = randomInt(250, 750);

    //
    // Position cluster on the screen.
    //
    this->GenerateTeamClusterScreenPosition(pMapData, &dc, sID);

    IclusterIGC * o = (IclusterIGC *) pMapData->GetMission()->CreateObject(
                                                    pMapData->GetTime(),
                                                    OT_cluster,
                                                    &dc,
                                                    sizeof(dc));
    ZAssert(o);
    return(o);
}


VOID CmapMakerIGC::GenerateNeutralClusterScreenPosition(CMapData * pMapData,
                                                    DataClusterIGC * pdc,
                                                    SectorID sID)
{
    float flTheta;

    flTheta = (float) (2.0 * pi * sID / pMapData->GetTotalClusters());
    pdc->screenX = cos(flTheta) * 0.4f;
    pdc->screenY = sin(flTheta) * 0.4f;
}


IclusterIGC * CmapMakerIGC::GenerateNeutralCluster(CMapData * pMapData,
                                                    SectorID sID)
{
    DataClusterIGC dc;

    dc.bHomeSector = false;

    dc.clusterID = sID;
    dc.activeF = true;

    dc.lightColor = 0xffffffff;
    dc.lightDirection = Vector::RandomDirection();

    pMapData->GetNewClusterName(dc.name);

    strcpy(dc.posterName, "globe");
    dc.posterName[5] = '0' + randomInt(1, 8);
    dc.posterName[6] = '\0';
    dc.planetName[0] = '\0';

    dc.starSeed = rand();
    dc.nDebris = randomInt(250, 750);
    dc.nStars = randomInt(250, 750);

    //
    // Position cluster on the screen.  Since we're doing all the
    // player clusters first, we need to space them appropriately.
    //
    this->GenerateNeutralClusterScreenPosition(pMapData, &dc, sID);

    IclusterIGC * o = (IclusterIGC *) pMapData->GetMission()->CreateObject(
                                                    pMapData->GetTime(),
                                                    OT_cluster,
                                                    &dc,
                                                    sizeof(dc));
    ZAssert(o);
    return(o);
}


VOID CmapMakerIGC::GenerateRingClusters(CMapData * pMapData)
{
    SectorID sID, cTotalRingClusters;
    IclusterIGC * pCluster;

    cTotalRingClusters = pMapData->GetTotalClusters() -
                            pMapData->GetOtherClusters();

    for(sID = 0; sID < cTotalRingClusters; sID++)
    {
        if (sID % pMapData->GetClustersPerTeam() <
            pMapData->GetTeamClustersPerTeam())
        {
            pCluster = this->GenerateTeamCluster(pMapData, sID);
            pMapData->SetCluster(sID,
                                pCluster,
                                sID / pMapData->GetClustersPerTeam());
        } else
        {
            pCluster = this->GenerateNeutralCluster(pMapData, sID);
            pMapData->SetCluster(sID, pCluster, NA);
        }
        pCluster->Release();
    }

    while(sID < pMapData->GetTotalClusters())
    {
        pCluster = this->GenerateNeutralCluster(pMapData, sID);
        pMapData->SetCluster(sID, pCluster, NA);
        sID++;
    }
}


VOID CmapMakerIGC::GenerateWarp(CMapData * pMapData,
                                IclusterIGC * pCluster,
                                DataWarpIGC * pdw)
{
    //
    // Add it to the cluster.
    //
    pdw->forward.x = pdw->forward.y = 0.0f;
    pdw->forward.z = 1.0f;

    pdw->rotation.axis(pdw->forward);
    pdw->rotation.angle(0.0f);

    pdw->warpDef.radius = 100;

    IObject * o = pMapData->GetMission()->CreateObject(pMapData->GetTime(),
                                                       OT_warp,
                                                       pdw,
                                                       sizeof(*pdw));
    ZAssert(o);
    o->Release();
}


VOID CmapMakerIGC::LinkClusters(CMapData* pMapData,
                                SectorID  sID1,
                                SectorID  sID2,
                                int       alephType)
{
    DataWarpIGC dw;

    static const char*  c_pszIconNeutral = "neutralaleph";
    static const char*  c_pszIconPlayer = "homealeph";

    static const char*  c_pszTextureNeutral = "plnt19";
    static const char*  c_pszTexturePlayer = "plnt42";

    float signature1 = 1.0f;
    float signature2 = 1.0f;

    float radius1 = 0.6f;
    float radius2 = 0.6f;

    if (alephType == NA)
    {
        SideID  side1 = pMapData->GetClusterSide(sID1);
        SideID  side2 = pMapData->GetClusterSide(sID2);
        if (NA == side1)
        {
            if (NA == side2)
            {
                alephType = c_NeutralAleph;
            }
            else
            {
                alephType = c_NeutralToPlayerAleph;
            }
        }
        else if (side1 == side2)
        {
            alephType = c_FriendlyAleph;
        }
        else if (NA != side2)
        {
            alephType = c_EnemyAleph;
        }
        else
        {
            alephType = c_PlayerToNeutralAleph;
        }
    }

    const char* pszTexture1;
    const char* pszTexture2;

    const char* pszIcon1;
    const char* pszIcon2;
    switch (alephType)
    {
        case c_FriendlyAleph:
            pszIcon1 = pszIcon2 = c_pszIconPlayer;
            pszTexture1 = pszTexture2 = c_pszTexturePlayer;
        break;

        case c_EnemyAleph:
            pszIcon1 = pszIcon2 = c_pszIconPlayer;
            pszTexture1 = pszTexture2 = c_pszTexturePlayer;
            signature1 = signature2 = 0.33f;
            radius1 = radius2 = 0.85f;
        break;

        case c_NeutralToPlayerAleph:
            pszIcon1 = c_pszIconPlayer;
            pszTexture1 = c_pszTexturePlayer;
            signature1 = 0.67f;

            pszIcon2 = c_pszIconNeutral;
            pszTexture2 = c_pszTextureNeutral;
        break;

        case c_PlayerToNeutralAleph:
            pszIcon2 = c_pszIconPlayer;
            pszTexture2 = c_pszTexturePlayer;
            signature2 = 0.67f;

            pszIcon1 = c_pszIconNeutral;
            pszTexture1 = c_pszTextureNeutral;
        break;

        case c_NeutralAleph:
            pszIcon1 = pszIcon2 = c_pszIconNeutral;
            pszTexture1 = pszTexture2 = c_pszTextureNeutral;
        break;

        case c_HiddenToPlayerAleph:
            pszIcon2 = c_pszIconPlayer;
            pszTexture2 = c_pszTexturePlayer;
            signature2 = 0.33f;
            radius2 = 0.85f;

            pszIcon1 = c_pszIconNeutral;
            pszTexture1 = c_pszTextureNeutral;
        break;

        default:
            ZAssert (false);
    }

    //
    // Build the alephs.
    //
    dw.warpDef.warpID = pMapData->GetNextWarpID();
    dw.clusterID = sID1;
    strcpy(dw.warpDef.textureName, pszTexture1);
    strcpy(dw.warpDef.iconName, pszIcon1);
    strcpy(dw.name, pMapData->GetCluster(sID2)->GetName());
    dw.signature = signature1;
    dw.position.x = dw.position.y = 0.0f;
    dw.position.z = radius1;
    dw.warpDef.destinationID = dw.warpDef.warpID + 1;

    this->GenerateWarp(pMapData, pMapData->GetCluster(sID1), &dw);

    dw.warpDef.warpID = pMapData->GetNextWarpID();
    dw.clusterID = sID2;
    strcpy(dw.warpDef.textureName, pszTexture2);
    strcpy(dw.warpDef.iconName, pszIcon2);
    strcpy(dw.name, pMapData->GetCluster(sID1)->GetName());
    dw.signature = signature2;
    dw.position.x = dw.position.y = 0.0f;
    dw.position.z = radius2;
    dw.warpDef.destinationID = dw.warpDef.warpID - 1;

    this->GenerateWarp(pMapData, pMapData->GetCluster(sID2), &dw);
}


VOID CmapMakerIGC::CrossLinkClusters(CMapData * pMapData, SectorID sFirstID)
{
    SectorID sMax, sWrap;

    sMax = pMapData->GetTotalClusters();
    if (pMapData->GetTeams() < 4)
        sMax = 0;
    else if (pMapData->GetTeams() == 4)
        sMax /= 2;

    while(sFirstID < sMax)
    {
        sWrap = (sFirstID + 2 * pMapData->GetClustersPerTeam()) %
                    pMapData->GetTotalClusters();
        LinkClusters(pMapData, sFirstID, sWrap);
        sFirstID += pMapData->GetClustersPerTeam();
    }
}


VOID CmapMakerIGC::LinkClusters(CMapData * pMapData)
{
    SectorID sID, cTotalSectors;

    //
    // Default implementation is a single ring.
    //
    cTotalSectors = pMapData->GetTotalClusters();

    for(sID = 0; sID < cTotalSectors - 1; sID++)
        this->LinkClusters(pMapData, sID, sID + 1);

    this->LinkClusters(pMapData, sID, 0);

    //
    // If more than 3 teams, link the neutral sectors.
    //
    this->CrossLinkClusters(pMapData, pMapData->GetTeamClustersPerTeam());
}


VOID CmapMakerIGC::GenerateLayout(CMapData * pMapData)
{
    //SectorID    s = pMapData->GetMaxPlayersOnTeam() >= 30 ? 2 : 1;
	//KGJV-: changed, use now MapSize parameter
    SectorID    s = pMapData->GetMapSize() != 0 ? 2 : 1;
    pMapData->SetTeamClustersPerTeam(s);
    pMapData->SetNeutralClustersPerTeam(1);

    this->GenerateRingClusters(pMapData);

    //
    // Now that we have the clusters, link them together.
    //
    this->LinkClusters(pMapData);
}


VOID CmapMakerIGC::GenerateStarbase(CMapData*       pMapData,
                                    IclusterIGC*    pcluster)
{
    //
    // Note that before calling this function, the alephs should
    // be created so that the side visibility takes effect when
    // the station is created.
    //
    DataStationIGC ds;

    SideID      sideID = pMapData->GetClusterSide(pcluster->GetObjectID());
    IsideIGC*   pSide = pMapData->GetMission()->GetSide(sideID);
    ZAssert(pSide);

    ds.clusterID = pcluster->GetObjectID();
    ds.position = Vector::GetZero();
    ds.forward.x = ds.forward.y = 0.0f;
    ds.forward.z = 1.0f;
    ds.up.x = 1.0f;
    ds.up.y = ds.up.z = 0.0f;

    ds.rotation.axis(ds.forward);
    ds.rotation.angle(0.0f);

    ds.bpHull = 1.0f;
    ds.bpShield = 1.0f;

    ds.sideID = sideID;
    ds.stationID = pMapData->GetMission()->GenerateNewStationID();

    IstationTypeIGC*    pst = pSide->GetCivilization()->GetInitialStationType();
    ZAssert (pst);
    //Allow for an upgrade to the station being available
    pst = pst->GetSuccessorStationType(pSide);

    ds.stationTypeID = pst->GetObjectID();
    strcpy(ds.name, pst->GetName());

    IObject * o = pMapData->GetMission()->CreateObject(
                                        pMapData->GetTime(),
                                        OT_station,
                                        &ds,
                                        sizeof(ds));
    ZAssert(o);
    o->Release();
}


VOID CmapMakerIGC::PopulateCluster(CMapData*     pMapData,
                                   IclusterIGC*  pcluster,
                                   float         amountHe3)
{
    ::PopulateCluster(pMapData->GetMission(), pMapData->GetMissionParams(), pcluster, amountHe3);
}


VOID CmapMakerIGC::PopulateClusters(CMapData* pMapData)
{
    float   amountHe3 = pMapData->GetMission()->GetFloatConstant(c_fcidAmountHe3) *
                        pMapData->GetMissionParams()->fHe3Density *
                        float(pMapData->GetTeams()) / float(pMapData->GetMinableAsteroids());


    for (ClusterLinkIGC*    pcl = pMapData->GetMission()->GetClusters()->first(); (pcl != NULL); pcl = pcl->next())
    {
        IclusterIGC*    pcluster = pcl->data();
        if (pcluster->GetHomeSector())
            GenerateStarbase(pMapData, pcluster);

        PopulateCluster(pMapData, pcluster, amountHe3);
    }
}

VOID CmapMakerIGC::RevealHomeClusters(ImissionIGC * pMission)
{

	    //
	    // Show every side everything about its home
	    //
	    for (SideLinkIGC * psl = pMission->GetSides()->first();
	        (psl != NULL);
	        psl = psl->next())
	    {
	        IsideIGC*   pside = psl->data();

	        for (StationLinkIGC * pstnl = pside->GetStations()->first();
	            (pstnl != NULL);
	            pstnl = pstnl->next())
			{
	            //
	            // Make every model in the station's cluster visible to the side
	            //
	            for (ModelLinkIGC * pml =
	                    pstnl->data()->GetCluster()->GetModels()->first();
	                (pml != NULL);
	                pml = pml->next())
	            {
	                pml->data()->SetSideVisibility(pside, true);
	            }
	        }
	    }
}

VOID CmapMakerIGC::RevealMap(ImissionIGC * pMission)
{
    //
    // Show every side every warp
    //
    for (WarpLinkIGC * pwl = pMission->GetWarps()->first();
        (pwl != NULL);
        pwl = pwl->next())
    {
        IwarpIGC*   pwarp = pwl->data();

        for (SideLinkIGC * psl = pMission->GetSides()->first();
            (psl != NULL);
            psl = psl->next())
        {
            pwarp->SetSideVisibility(psl->data(), true);

        }
    }
}

VOID CmapMakerIGC::RevealAlliedClusters(ImissionIGC * pMission)
{
    //
    // Show every ally everything about thier home
    //
	    for (SideLinkIGC * psl = pMission->GetSides()->first();
	        (psl != NULL);
	        psl = psl->next())
	    {
	        IsideIGC*   pside = psl->data();

	        for (StationLinkIGC * pstnl = pside->GetStations()->first();
	            (pstnl != NULL);
	            pstnl = pstnl->next())
			{
	            //
	            // Make every model in the station's cluster visible to the allies
	            //
	            for (ModelLinkIGC * pml =
	                    pstnl->data()->GetCluster()->GetModels()->first();
	                (pml != NULL);
	                pml = pml->next())
	            {								
					
					//lets get a list of allied sideIDs
				    for (SideLinkIGC* psidelink = pMission->GetSides()->first();
						(psidelink != NULL);
						psidelink = psidelink->next())
					{
						IsideIGC*   otherside = psidelink->data();
						//this side is ally...and not ours or an aleph (alephs must be replicated)
						if (pside->AlliedSides(otherside,pside) && otherside != pside && pml->data()->GetObjectType() != OT_warp) {
							pml->data()->SetSideVisibility(otherside, true);
						}
					}
	            }
	        }
	    }
}

VOID CmapMakerIGC::ActivateSides(ImissionIGC * pMission)
{
    const SideListIGC * psideList = pMission->GetSides();

    for (SideLinkIGC * psidelink = psideList->first();
        NULL != psidelink;
        psidelink = psidelink->next())
    {
        psidelink->data()->SetActiveF(true);
    }
}


//
// Double ring implementation.
//
CmapMakerDoubleRingIGC::CmapMakerDoubleRingIGC()
{
    mMMID = c_mmDoubleRing;
    this->SetName("Double Ring");
}

VOID CmapMakerDoubleRingIGC::GenerateTeamClusterScreenPosition(
                                                    CMapData * pMapData,
                                                    DataClusterIGC * pdc,
                                                    SectorID sID)
{
    float   flTheta = (float) (2.0 * pi * sID / pMapData->GetTotalClusters());
    pdc->screenX = cos(flTheta) * 0.9f;
    pdc->screenY = sin(flTheta) * 0.9f;
}


VOID CmapMakerDoubleRingIGC::GenerateNeutralClusterScreenPosition(
                                                    CMapData * pMapData,
                                                    DataClusterIGC * pdc,
                                                    SectorID sID)
{
    float   flTheta = (float) (2.0 * pi * sID / pMapData->GetTotalClusters());
    float   flRadius = (pMapData->GetTeams() == 2) && (pMapData->GetTeamClustersPerTeam() == 2)
                       ? 0.2f
                       : 0.3f;
    pdc->screenX = cos(flTheta) * flRadius;
    pdc->screenY = sin(flTheta) * flRadius;
}


VOID CmapMakerDoubleRingIGC::LinkClusters(CMapData * pMapData)
{
    SectorID sID, cTotalSectors, sWrapID, sIndex;

    //
    // Link each team's group of sectors.
    //
    cTotalSectors = pMapData->GetTotalClusters();
    for(sID = 0; sID < cTotalSectors; sID += pMapData->GetClustersPerTeam())
    {
        sWrapID = (sID + cTotalSectors - 1) % cTotalSectors;

        for(sIndex = 0; sIndex < pMapData->GetTeamClustersPerTeam(); sIndex++)
        {
            CmapMakerIGC::LinkClusters(pMapData,
                                    sID + sIndex,
                                    sID + pMapData->GetTeamClustersPerTeam());
            CmapMakerIGC::LinkClusters(pMapData,
                                    sID + sIndex,
                                    sWrapID);
        }
    }

    //
    // Link the ring around the outside.
    //
    sWrapID = cTotalSectors - 1 - pMapData->GetNeutralClustersPerTeam();
    for(sID = 0; sID < cTotalSectors; sID++)
    {
        if (NA != pMapData->GetClusterSide(sID))
        {
            CmapMakerIGC::LinkClusters(pMapData, sID, sWrapID);
            sWrapID = sID;
        }
    }

    //
    // Link the ring around the inside.
    //
    sWrapID = cTotalSectors - 1;
    for(sID = 0; sID < cTotalSectors; sID++)
    {
        if (NA == pMapData->GetClusterSide(sID))
        {
            CmapMakerIGC::LinkClusters(pMapData, sID, sWrapID);
            sWrapID = sID;
        }
    }
}

//
// Big ring implementation.
//
CmapMakerBigRingIGC::CmapMakerBigRingIGC()
{
    mMMID = c_mmBigRing;
    this->SetName("Big Ring");
}

VOID CmapMakerBigRingIGC::GenerateLayout(CMapData * pMapData)
{
    //SectorID    s = pMapData->GetMaxPlayersOnTeam() >= 30 ? 2 : 1;
	//KGJV-: changed, use now MapSize parameter
    SectorID    s = pMapData->GetMapSize() != 0 ? 2 : 1;
    pMapData->SetTeamClustersPerTeam(s);
    pMapData->SetNeutralClustersPerTeam(2);

    this->GenerateRingClusters(pMapData);

    //
    // Now that we have the clusters, link them together.
    //
    this->LinkClusters(pMapData);
}

VOID CmapMakerBigRingIGC::GenerateTeamClusterScreenPosition(CMapData * pMapData,
                                                    DataClusterIGC * pdc,
                                                    SectorID sID)
{
    SectorID    s = (pMapData->GetTeamClustersPerTeam() == 1) ? (sID / 3) : ((sID + 1) / 2);
    SectorID    n = pMapData->GetTeamClusters();
    float flTheta = (float) (2.0 * pi * s / n);

    pdc->screenX = cos(flTheta) * 0.9f;
    pdc->screenY = sin(flTheta) * 0.9f;
}
VOID CmapMakerBigRingIGC::GenerateNeutralClusterScreenPosition(CMapData * pMapData,
                                                    DataClusterIGC * pdc,
                                                    SectorID sID)
{
    float       flTheta;

    SectorID    n = pMapData->GetNeutralClusters();
    if (pMapData->GetTeamClustersPerTeam() == 1)
    {
        SectorID    s = (sID - 1 - sID/3);
        flTheta = (float) (2.0 * pi * (s + 0.5f) / n);
    }
    else
    {
        SectorID    s =  ((sID + 1) / 2);
        flTheta = (float) (2.0 * pi * s / n);
    }

    pdc->screenX = cos(flTheta) * 0.6f;
    pdc->screenY = sin(flTheta) * 0.6f;
}

VOID CmapMakerBigRingIGC::LinkClusters(CMapData * pMapData)
{
    //
    // Link the ring
    //
    SectorID    cTotalSectors = pMapData->GetTotalClusters();
    {
        SectorID    sWrapID = cTotalSectors - 1;
        for(SectorID    sID = 0; sID < cTotalSectors; sID++)
        {
            CmapMakerIGC::LinkClusters(pMapData, sID, sWrapID);
            sWrapID = sID;
        }
    }


    if (pMapData->GetTeams() > 2)
    {
        SectorID    n = pMapData->GetTeamClustersPerTeam();
        for (SideID i = 0; (i < pMapData->GetTeams()); i++)
        {
            SectorID    sID = i * pMapData->GetClustersPerTeam();
            CmapMakerIGC::LinkClusters(pMapData, sID + n, (sID + 3 + 2*n) % cTotalSectors);
            if (pMapData->GetTeams() > 3)
                CmapMakerIGC::LinkClusters(pMapData, sID + n + 1, (cTotalSectors + (sID - 2)) % cTotalSectors);
        }
    }
}
//
// HiLo implementation.
//
CmapMakerHiLoIGC::CmapMakerHiLoIGC()
{
    mMMID = c_mmHiLo;
    this->SetName("HiLo");
}

VOID CmapMakerHiLoIGC::GenerateLayout(CMapData * pMapData)
{
    //pMapData->SetTeamClustersPerTeam(pMapData->GetMaxPlayersOnTeam() >= 30 ? 2 : 1);
	//KGJV-: changed, use now MapSize parameter
    pMapData->SetTeamClustersPerTeam(pMapData->GetMapSize() != 0 ? 2 : 1);
                                    
    pMapData->SetNeutralClustersPerTeam(3);

    pMapData->SetOtherClusters(1);

    this->GenerateRingClusters(pMapData);

    //
    // Now that we have the clusters, link them together.
    //
    this->LinkClusters(pMapData);
}

VOID CmapMakerHiLoIGC::GenerateTeamClusterScreenPosition(
                                                    CMapData * pMapData,
                                                    DataClusterIGC * pdc,
                                                    SectorID sID)
{
    float flTheta, flRadius;
    SectorID sTeam;

    sTeam = sID / pMapData->GetClustersPerTeam();
    sID = sID % pMapData->GetClustersPerTeam();

    flTheta = (float) (2.0f * pi * float(sTeam) / float(pMapData->GetTeams()));
    flRadius = 0.5f + 0.4f * sID;

    pdc->screenX = cos(flTheta) * flRadius;
    pdc->screenY = sin(flTheta) * flRadius;
}

VOID CmapMakerHiLoIGC::GenerateNeutralClusterScreenPosition(
                                                    CMapData * pMapData,
                                                    DataClusterIGC * pdc,
                                                    SectorID sID)
{
    SectorID sTeam;

    sTeam = sID / pMapData->GetClustersPerTeam();
    if (sTeam < pMapData->GetTeams())
    {
        sID = (sID % pMapData->GetClustersPerTeam()) - pMapData->GetTeamClustersPerTeam();

        float flTheta1 = (float) (2.0 * pi * sTeam / pMapData->GetTeams());
        float flTheta2 = (float) (2.0 * pi * (sTeam + 1) / pMapData->GetTeams());

        float   flTheta;
        float   flRadius;
        switch (sID)
        {
            case 0:
                flTheta = (flTheta1 + flTheta2) / 2.0f;
                flRadius = 0.4f;
            break;
            case 1:
                flTheta = (2.0f * flTheta1 + flTheta2) / 3.0f;
                flRadius = 0.8f;
            break;
            case 2:
                flTheta = (flTheta1 + 2.0f * flTheta2) / 3.0f;
                flRadius = 0.8f;
            break;
        }

        pdc->screenX = cos(flTheta) * flRadius;
        pdc->screenY = sin(flTheta) * flRadius;
    }
    else
        pdc->screenX = pdc->screenY = 0.0f;
}

VOID CmapMakerHiLoIGC::LinkClusters(CMapData * pMapData)
{
    if (pMapData->GetTeamClustersPerTeam() != 1)
    {
        //Link the split base clusters for each team
        for (SideID i = 0; (i < pMapData->GetTeams()); i++)
        {
            SectorID    sID = i * pMapData->GetClustersPerTeam();
            CmapMakerIGC::LinkClusters(pMapData, sID, sID + 1);
        }
    }

    {
        SectorID    sidCenter = pMapData->GetTotalClusters() - 1;

        for (SideID i = 0; (i < pMapData->GetTeams()); i++)
        {
            SectorID    sID = i * pMapData->GetClustersPerTeam() +
                              pMapData->GetTeamClustersPerTeam();

            //Inner ring to center
            CmapMakerIGC::LinkClusters(pMapData, sID, sidCenter);

            //Inner ring to inner previous team cluster
            SectorID    sidPrevious = i * pMapData->GetClustersPerTeam();
            CmapMakerIGC::LinkClusters(pMapData, sID, sidPrevious);

            //Inner ring to inner next team cluster
            SectorID    sidNext = ((i+1)%pMapData->GetTeams()) * pMapData->GetClustersPerTeam();
            CmapMakerIGC::LinkClusters(pMapData, sID, sidNext);

            //Outer ring 1 to outer ring 2
            CmapMakerIGC::LinkClusters(pMapData, sID + 1, sID + 2);

            //Outer ring 1 to previous team
            CmapMakerIGC::LinkClusters(pMapData, sID + 1, sidPrevious + pMapData->GetTeamClustersPerTeam() - 1);

            //Outer ring 2 to previous team
            CmapMakerIGC::LinkClusters(pMapData, sID + 2, sidNext + pMapData->GetTeamClustersPerTeam() - 1);
        }
    }
}

//
// HiHigher implementation.
//
CmapMakerHiHigherIGC::CmapMakerHiHigherIGC()
{
    mMMID = c_mmHiHigher;
    this->SetName("HiHigher");
}

VOID CmapMakerHiHigherIGC::GenerateLayout(CMapData * pMapData)
{
    pMapData->SetTeamClustersPerTeam((mMMID == c_mmHiHigher)
                                     //? (pMapData->GetMaxPlayersOnTeam() >= 30 ? 2 : 1)
									 //KGJV-: changed, use now MapSize parameter
									 ? (pMapData->GetMapSize() != 0 ? 2 : 1)
                                     : ((mMMID == c_mmLargeSplit)
                                        ? 2
                                        : 1));
                                    
    pMapData->SetNeutralClustersPerTeam(5);

    pMapData->SetOtherClusters(1);

    this->GenerateRingClusters(pMapData);

    //
    // Now that we have the clusters, link them together.
    //
    this->LinkClusters(pMapData);
}

VOID CmapMakerHiHigherIGC::GenerateTeamClusterScreenPosition(
                                                    CMapData * pMapData,
                                                    DataClusterIGC * pdc,
                                                    SectorID sID)
{
    SectorID sTeam = sID / pMapData->GetClustersPerTeam();
    sID = (sID % pMapData->GetClustersPerTeam());

    float flTheta;
    float flRadius;
    if (mMMID == c_mmInsideOut)
    {
        flTheta = (float) (2.0 * pi * (sTeam + 0.5f) / pMapData->GetTeams());
        flRadius = 0.3f;
    }
    else
    {
        flTheta = (float) (2.0 * pi * sTeam / pMapData->GetTeams());
        flRadius = (pMapData->GetTeamClustersPerTeam() == 1) ? 1.0f : (0.75f + 0.25f * sID);
    }

    pdc->screenX = cos(flTheta) * flRadius;
    pdc->screenY = sin(flTheta) * flRadius;
}

VOID CmapMakerHiHigherIGC::GenerateNeutralClusterScreenPosition(
                                                    CMapData * pMapData,
                                                    DataClusterIGC * pdc,
                                                    SectorID sID)
{
    SectorID sTeam;

    sTeam = sID / pMapData->GetClustersPerTeam();
    if (sTeam < pMapData->GetTeams())
    {
        sID = (sID % pMapData->GetClustersPerTeam()) - pMapData->GetTeamClustersPerTeam();

        float flTheta1 = (float) (2.0 * pi * sTeam / pMapData->GetTeams());
        float flTheta2 = (float) (2.0 * pi * (sTeam + 1) / pMapData->GetTeams());

        float   flTheta;
        float   flRadius;
        switch (sID)
        {
            case 0:
                flTheta = (3.0f * flTheta1 + flTheta2) / 4.0f;
                flRadius = 0.6f;
            break;
            case 1:
                if (mMMID == c_mmInsideOut)
                {
                    flTheta = flTheta1;
                    flRadius = 1.0f;
                }
                else
                {
                    flTheta = (flTheta1 + flTheta2) / 2.0f;
                    flRadius = 0.3f;
                }
            break;
            case 2:
                flTheta = (flTheta1 + 3.0f * flTheta2) / 4.0f;
                flRadius = 0.6f;
            break;
            case 3:
                flTheta = (2.0f * flTheta1 + flTheta2) / 3.0f;
                flRadius = 1.0f;
            break;
            case 4:
                flTheta = (flTheta1 + 2.0f * flTheta2) / 3.0f;
                flRadius = 1.0f;
            break;
        }

        pdc->screenX = cos(flTheta) * flRadius;
        pdc->screenY = sin(flTheta) * flRadius;
    }
    else
        pdc->screenX = pdc->screenY = 0.0f;
}

VOID CmapMakerHiHigherIGC::LinkClusters(CMapData * pMapData)
{
    if ((pMapData->GetTeamClustersPerTeam() != 1) && (mMMID != c_mmLargeSplit))
    {
        //Link the split base clusters for each team
        for (SideID i = 0; (i < pMapData->GetTeams()); i++)
        {
            SectorID    sID = i * pMapData->GetClustersPerTeam();
            CmapMakerIGC::LinkClusters(pMapData, sID, sID + 1);
        }
    }

    {
        SectorID    sidCenter = pMapData->GetTotalClusters() - 1;

        for (SideID i = 0; (i < pMapData->GetTeams()); i++)
        {
            SectorID    sIDp = i * pMapData->GetClustersPerTeam();
            SectorID    sID0 = sIDp +
                               pMapData->GetTeamClustersPerTeam();

            SectorID    sID1 = sID0 + 1;
            SectorID    sID2 = sID0 + 2;
            SectorID    sID3 = sID0 + 3;
            SectorID    sID4 = sID0 + 4;

            SectorID    sIDn = ((i+1)%pMapData->GetTeams()) * pMapData->GetClustersPerTeam();

            if (mMMID == c_mmInsideOut)
            {
                ZAssert (pMapData->GetTeamClustersPerTeam() == 1);
                sID1 = sIDp;
                sIDp = sID0 + 1;
                sIDn = ((i+1)%pMapData->GetTeams()) * pMapData->GetClustersPerTeam() + 1 + 1;
            }

            //Inner ring to center
            CmapMakerIGC::LinkClusters(pMapData, sID1, sidCenter,
                                       (mMMID == c_mmInsideOut) ? c_HiddenToPlayerAleph : NA);

            CmapMakerIGC::LinkClusters(pMapData, sID0, sID1);
            CmapMakerIGC::LinkClusters(pMapData, sID1, sID2);

            CmapMakerIGC::LinkClusters(pMapData, sID0, sID3);
            CmapMakerIGC::LinkClusters(pMapData, sID2, sID4);

            CmapMakerIGC::LinkClusters(pMapData, sID3, sID4);

            CmapMakerIGC::LinkClusters(pMapData, sID0, sIDp);
            CmapMakerIGC::LinkClusters(pMapData, sID2, sIDn);

            CmapMakerIGC::LinkClusters(pMapData, sID3, sIDp + pMapData->GetTeamClustersPerTeam() - 1);
            CmapMakerIGC::LinkClusters(pMapData, sID4, sIDn + pMapData->GetTeamClustersPerTeam() - 1);

            if (pMapData->GetMissionParams()->iRandomEncounters != 0)
                CmapMakerIGC::LinkClusters(pMapData, sID2, 
                                           (mMMID == c_mmInsideOut)
                                           ? (sIDn - 1)
                                           : (sIDn + pMapData->GetTeamClustersPerTeam()));
        }
    }
}

//
// Star implementation.
//
CmapMakerStarIGC::CmapMakerStarIGC()
{
    mMMID = c_mmStar;
    this->SetName("Star");
}

VOID CmapMakerStarIGC::GenerateLayout(CMapData * pMapData)
{
    //pMapData->SetTeamClustersPerTeam(pMapData->GetMaxPlayersOnTeam() >= 30 ? 2 : 1);
	//KGJV-: changed, use now MapSize parameter
    pMapData->SetTeamClustersPerTeam(pMapData->GetMapSize() != 0 ? 2 : 1);
                                    
    pMapData->SetNeutralClustersPerTeam(5);

    this->GenerateRingClusters(pMapData);

    //
    // Now that we have the clusters, link them together.
    //
    this->LinkClusters(pMapData);
}

VOID CmapMakerStarIGC::GenerateTeamClusterScreenPosition(
                                                    CMapData * pMapData,
                                                    DataClusterIGC * pdc,
                                                    SectorID sID)
{
    SectorID sTeam = sID / pMapData->GetClustersPerTeam();
    sID = sID % pMapData->GetClustersPerTeam();

    float   flDelta = (2.0f * pi / float(pMapData->GetTeams()));
    float   flTheta = flDelta * float(sTeam);

    if (pMapData->GetTeamClustersPerTeam() == 2)
        flTheta += flDelta * (float(sID) - 0.5f) * 0.5f;

    pdc->screenX = cos(flTheta);
    pdc->screenY = sin(flTheta);
}

VOID CmapMakerStarIGC::GenerateNeutralClusterScreenPosition(
                                                    CMapData * pMapData,
                                                    DataClusterIGC * pdc,
                                                    SectorID sID)
{
    SectorID sTeam = sID / pMapData->GetClustersPerTeam();
    sID = sID % pMapData->GetClustersPerTeam() - pMapData->GetTeamClustersPerTeam();

    float   flRadius;
    float   flDelta = (2.0f * pi / float(pMapData->GetTeams()));
    float   flTheta = flDelta * float(sTeam);
    switch (sID)
    {
        case 0:
        case 1:
        {
            flRadius = 0.7f;
            flTheta += flDelta * (float(sID) - 0.5f) * 0.5f;
        }
        break;
        case 3:
        {
            flTheta += flDelta * 0.5f;
        }
        //no break
        case 2:
        {
            flRadius = 0.5f;
        }
        break;

        case 4:
        {
            flRadius = 0.2f;
        }
        break;
    }

    pdc->screenX = cos(flTheta) * flRadius;
    pdc->screenY = sin(flTheta) * flRadius;
}

VOID CmapMakerStarIGC::LinkClusters(CMapData * pMapData)
{
    {
        SectorID    sidCenter = pMapData->GetTotalClusters() - 1;

        for (SideID i = 0; (i < pMapData->GetTeams()); i++)
        {
            SectorID    sID = i * pMapData->GetClustersPerTeam() +
                              pMapData->GetTeamClustersPerTeam();

            //Inner starting sectors to neutral sectors
            if (pMapData->GetTeamClustersPerTeam() == 1)
            {
                CmapMakerIGC::LinkClusters(pMapData, sID,     sID - 1);
                CmapMakerIGC::LinkClusters(pMapData, sID + 1, sID - 1);
            }
            else
            {
                CmapMakerIGC::LinkClusters(pMapData, sID - 1, sID - 2);
                CmapMakerIGC::LinkClusters(pMapData, sID,     sID - 2);
                CmapMakerIGC::LinkClusters(pMapData, sID + 1, sID - 1);
            }

            CmapMakerIGC::LinkClusters(pMapData, sID, sID + 2);
            CmapMakerIGC::LinkClusters(pMapData, sID + 1, sID + 2);
            CmapMakerIGC::LinkClusters(pMapData, sID + 1, sID + 3);

            CmapMakerIGC::LinkClusters(pMapData, sID + 2, sID + 3);
            CmapMakerIGC::LinkClusters(pMapData, sID + 2, sID + 4);

            {
                SectorID    s = (sID + pMapData->GetTotalClusters() - pMapData->GetClustersPerTeam()) % pMapData->GetTotalClusters();
                CmapMakerIGC::LinkClusters(pMapData, s + 3, sID);
                CmapMakerIGC::LinkClusters(pMapData, s + 3, sID + 2);
            }

            {
                SideID      j = (i + pMapData->GetTeams()/2) % pMapData->GetTeams();
                SectorID    s = j * pMapData->GetClustersPerTeam() +
                                pMapData->GetTeamClustersPerTeam();
                if (pMapData->GetTeams() % 2 == 0)
                {
                    CmapMakerIGC::LinkClusters(pMapData, sID + 4, s + 4);
                }
                else
                {
                    CmapMakerIGC::LinkClusters(pMapData, sID + 4, s + 4);
                    CmapMakerIGC::LinkClusters(pMapData, sID + 4, (s + 4 + pMapData->GetClustersPerTeam()) % pMapData->GetTotalClusters());
                }
            }
        }
    }
}

//
// Brawl
//
CmapMakerBrawlIGC::CmapMakerBrawlIGC()
{
    mMMID = c_mmBrawl;
    this->SetName("Brawl");
}


const char* CmapMakerBrawlIGC::IsValid(const MissionParams * pmp)
{
	// yp your_persona removed march 24 2006
    //if (pmp->nMaxPlayersPerTeam * pmp->nTeams > 36)
    //    return("Brawl maps must have less than 37 players; "
    //            "please change the map type, number of players per team, "
    //            "or number of teams.");

    return(NULL);
}


VOID CmapMakerBrawlIGC::GenerateLayout(CMapData * pMapData)
{
    pMapData->SetTeamClustersPerTeam(0);

    pMapData->SetOtherClusters(1);

    this->GenerateRingClusters(pMapData);

    //
    // Now that we have the clusters, link them together.
    //
    this->LinkClusters(pMapData);
}


VOID CmapMakerBrawlIGC::GenerateNeutralClusterScreenPosition(
                                                    CMapData * pMapData,
                                                    DataClusterIGC * pdc,
                                                    SectorID sID)
{
    pdc->screenX = 0.0f;
    pdc->screenY = 0.0f;
}


VOID CmapMakerBrawlIGC::LinkClusters(CMapData * pMapData)
{
}


VOID CmapMakerBrawlIGC::PopulateCluster(CMapData*    pMapData,
                                        IclusterIGC* pcluster,
                                        float        amountHe3)
{
    SectorID    sID = pcluster->GetObjectID();

    //Place all the space stations ... 1/side
    for (SideID sideID = 0; (sideID < pMapData->GetTeams()); sideID++)
    {
        DataStationIGC ds;

        IsideIGC*  pSide = pMapData->GetMission()->GetSide(sideID);
        ZAssert(pSide);

        ds.clusterID = sID;
        {
            const float c_startRadius = 2500.0f;

            float   angle = 2.0f * pi * float(sideID) / float(pMapData->GetTeams());
            ds.position.x = c_startRadius * cos(angle);
            ds.position.y = c_startRadius * sin(angle);
            ds.position.z = 0.0f;
        }
        ds.forward.x = ds.forward.y = 0.0f;
        ds.forward.z = 1.0f;
        ds.up.x = 1.0f;
        ds.up.y = ds.up.z = 0.0f;

        ds.rotation.axis(ds.forward);
        ds.rotation.angle(0.0f);

        ds.bpHull = 1.0f;
        ds.bpShield = 1.0f;

        ds.sideID = sideID;
        ds.stationID = pMapData->GetMission()->GenerateNewStationID();

        IstationTypeIGC*    pst = pSide->GetCivilization()->GetInitialStationType();
        ZAssert (pst);

        ds.stationTypeID = pst->GetObjectID();
        strcpy(ds.name, pst->GetName());

        IstationIGC*    pstation = (IstationIGC*)pMapData->GetMission()->CreateObject(pMapData->GetTime(),
                                                                                       OT_station,
                                                                                       &ds,
                                                                                       sizeof(ds));
        ZAssert(pstation);

        pstation->Release();

    }

    for (int i = randomInt(3, 5); (i >= 0); i--)
    {
        CreateAsteroid(pMapData->GetMission(), pcluster, IasteroidIGC::GetRandomType(0), 0.0f);
    }

    ::PopulateCluster(pMapData->GetMission(), pMapData->GetMissionParams(), pcluster, amountHe3,
                    true, true,
                    1, -3 * (pMapData->GetTeams()/2));
}

//
// PinWheel
//
CmapMakerPinWheelIGC::CmapMakerPinWheelIGC()
{
    mMMID = c_mmPinWheel;
    this->SetName("PinWheel");
}


const char* CmapMakerPinWheelIGC::IsValid(const MissionParams * pmp)
{
	// yp your_persona removed march 24 2006
    //if (pmp->nMaxPlayersPerTeam > 200)
    //  return("Pinwheel maps must have less than 201 player per team; "
    //"please change the map type, or number of players per team.");
	//
    return(NULL);
}


VOID CmapMakerPinWheelIGC::GenerateLayout(CMapData * pMapData)
{
    pMapData->SetTeamClustersPerTeam(1);

	//if (pMapData->GetMaxPlayersInMission() <= 30)
    //KGJV-: changed, use now MapSize parameter
    if (pMapData->GetMapSize() == 0)
        pMapData->SetOtherClusters(1);
    else
        pMapData->SetNeutralClustersPerTeam(1);

    this->GenerateRingClusters(pMapData);

    //
    // Now that we have the clusters, link them together.
    //
    this->LinkClusters(pMapData);
}


VOID CmapMakerPinWheelIGC::GenerateNeutralClusterScreenPosition(
                                                    CMapData * pMapData,
                                                    DataClusterIGC * pdc,
                                                    SectorID sID)
{
    float flTheta;

    if (0 != pMapData->GetNeutralClustersPerTeam())
    {
        sID = sID / pMapData->GetClustersPerTeam();
        flTheta = (float) (2.0 * pi * sID / pMapData->GetNeutralClusters());
        pdc->screenX = cos(flTheta) * 0.4f;
        pdc->screenY = sin(flTheta) * 0.4f;
    } else
        pdc->screenX = pdc->screenY = 0.0f;
}


VOID CmapMakerPinWheelIGC::LinkClusters(CMapData * pMapData)
{
    SectorID sID, cTotalClusters, sWrapID, sFirstNA;

    cTotalClusters = pMapData->GetTotalClusters();
    if (0 != pMapData->GetNeutralClustersPerTeam())
    {
        //
        // Multiple sectors in the center.
        //
        sWrapID = NA;
        for(sID = 0; sID < cTotalClusters; sID++)
        {
            if (NA != pMapData->GetClusterSide(sID))
                CmapMakerIGC::LinkClusters(pMapData, sID, sID + 1);
            else
            {
                if (NA != sWrapID)
                    CmapMakerIGC::LinkClusters(pMapData, sID, sWrapID);
                else
                    sFirstNA = sID;
                sWrapID = sID;
            }
        }

        if (pMapData->GetTeams() > 2)
            CmapMakerIGC::LinkClusters(pMapData, sWrapID, sFirstNA);
        this->CrossLinkClusters(pMapData, pMapData->GetTeamClustersPerTeam());
    }
    else
    {
        //
        // Single sector in the middle.
        //
        for(sID = 0; sID < cTotalClusters; sID++)
        {
            if (NA != pMapData->GetClusterSide(sID))
                CmapMakerIGC::LinkClusters(pMapData, sID, cTotalClusters - 1);
        }
    }
}

VOID CmapMakerPinWheelIGC::PopulateCluster(CMapData*    pMapData,
                                           IclusterIGC* pcluster,
                                           float        amountHe3)
{
    short   k;
    if (pcluster->GetHomeSector() ||
        (pMapData->GetNeutralClustersPerTeam() != 0))
    {
        k = 1;
    }
    else
        k = 2;

    ::PopulateCluster(pMapData->GetMission(), pMapData->GetMissionParams(), pcluster, amountHe3,
                    true, true,
                    k, k, k);
}


//
// Diamond ring.
//
CmapMakerDiamondRingIGC::CmapMakerDiamondRingIGC()
{
    mMMID = c_mmDiamondRing;
    this->SetName("Diamond Ring");
}


const char* CmapMakerDiamondRingIGC::IsValid(const MissionParams * pmp)
{
    if (pmp->nTeams < 3)
        return("Diamond ring maps must have more than 2 teams; "
               "please change the map type, or number of teams.");

    return(NULL);
}


VOID CmapMakerDiamondRingIGC::GenerateLayout(CMapData * pMapData)
{
    //pMapData->SetTeamClustersPerTeam(pMapData->GetMaxPlayersOnTeam() >= 30 ? 2 : 1);
	//KGJV-: changed, use now MapSize parameter
    pMapData->SetTeamClustersPerTeam(pMapData->GetMapSize() != 0 ? 2 : 1);
    pMapData->SetNeutralClustersPerTeam(2);

    this->GenerateRingClusters(pMapData);

    //
    // Now that we have the clusters, link them together.
    //
    this->LinkClusters(pMapData);
}


VOID CmapMakerDiamondRingIGC::GenerateTeamClusterScreenPosition(
                                                    CMapData * pMapData,
                                                    DataClusterIGC * pdc,
                                                    SectorID sID)
{
    float flTheta, flAdjust, flRadius;
    SectorID sTeam;

    sTeam = sID / pMapData->GetClustersPerTeam();
    sID = sID % pMapData->GetTeamClustersPerTeam();

    flTheta = (float) (2.0 * pi * sTeam / pMapData->GetTeams());
    switch(pMapData->GetTeamClustersPerTeam())
    {
        case 2:
            flAdjust = (float) (pi / 8.0f);
            if (1 == sID)
                flAdjust = -flAdjust;
            break;
        case 3:
            flAdjust = (sID - 1) * (float) (pi / 6.0f);
            break;
        default:
            flAdjust = 0.0f;
            break;
    }
    flTheta += flAdjust;
    flRadius = 0.55f;
    pdc->screenX = cos(flTheta) * flRadius;
    pdc->screenY = sin(flTheta) * flRadius;
}


VOID CmapMakerDiamondRingIGC::GenerateNeutralClusterScreenPosition(
                                                    CMapData * pMapData,
                                                    DataClusterIGC * pdc,
                                                    SectorID sID)
{
    float flTheta, flRadius;
    SectorID sTeam;

    sTeam = sID / pMapData->GetClustersPerTeam();
    sID = (sID % pMapData->GetClustersPerTeam()) -
            pMapData->GetTeamClustersPerTeam();
    if (0 == (sID & 0x01))
    {
        flRadius = 0.3f;
    } else
        flRadius = 0.8f;

    flTheta = (float) (2.0 * pi * sTeam / pMapData->GetTeams());
    pdc->screenX = cos(flTheta) * flRadius;
    pdc->screenY = sin(flTheta) * flRadius;
}


VOID CmapMakerDiamondRingIGC::LinkClusters(CMapData * pMapData)
{
    SectorID sID, cTotalSectors, sNeutralID, sIndex;

    //
    // Link each team's group of sectors.
    //
    cTotalSectors = pMapData->GetTotalClusters();
    for(sID = 0; sID < cTotalSectors; sID += pMapData->GetClustersPerTeam())
    {
        sNeutralID = sID + pMapData->GetTeamClustersPerTeam();

        for(sIndex = 0; sIndex < pMapData->GetTeamClustersPerTeam(); sIndex++)
        {
            CmapMakerIGC::LinkClusters(pMapData,
                                        sID + sIndex,
                                        sNeutralID);
            CmapMakerIGC::LinkClusters(pMapData,
                                        sID + sIndex,
                                        sNeutralID + 1);
        }
    }

    //
    // Link the rings.
    //
    for(sID = 0; sID < cTotalSectors; sID++)
    {
        if (NA == pMapData->GetClusterSide(sID))
        {
            sNeutralID = (sID + pMapData->GetClustersPerTeam()) % cTotalSectors;
            CmapMakerIGC::LinkClusters(pMapData, sID, sNeutralID);
        }
    }

    //
    // Link the center sectors.
    //
    this->CrossLinkClusters(pMapData, pMapData->GetTeamClustersPerTeam());
}


CmapMakerSnowFlakeIGC::CmapMakerSnowFlakeIGC()
{
    mMMID = c_mmSnowFlake;
    this->SetName("SnowFlake");
}


VOID CmapMakerSnowFlakeIGC::GenerateLayout(CMapData * pMapData)
{
    //SectorID    s = pMapData->GetMaxPlayersOnTeam() >= 30 ? 2 : 1;
	//KGJV-: changed, use now MapSize parameter
    SectorID    s = pMapData->GetMapSize() != 0 ? 2 : 1;
    pMapData->SetTeamClustersPerTeam(s);
    pMapData->SetNeutralClustersPerTeam(3);

    this->GenerateRingClusters(pMapData);

    //
    // Now that we have the clusters, link them together.
    //
    this->LinkClusters(pMapData);
}


VOID CmapMakerSnowFlakeIGC::GenerateTeamClusterScreenPosition(
                                                    CMapData * pMapData,
                                                    DataClusterIGC * pdc,
                                                    SectorID sID)
{
    float flRadius, flTheta, flAdjust;
    SectorID sTeam;

    sTeam = sID / pMapData->GetClustersPerTeam();
    sID = sID % pMapData->GetClustersPerTeam();

    flTheta = (float) (2.0 * pi * sTeam / pMapData->GetTeams());
    flAdjust = pMapData->GetTeamClustersPerTeam() == 1 ? 0.0f : (float) (pi / (2.0f * pMapData->GetTeams()));

    flRadius = 0.9f;

    if (pMapData->GetTeamClustersPerTeam() == sID + 1)
        flAdjust = -flAdjust;
    flTheta += flAdjust;
    pdc->screenX = cos(flTheta) * flRadius;
    pdc->screenY = sin(flTheta) * flRadius;
}


VOID CmapMakerSnowFlakeIGC::GenerateNeutralClusterScreenPosition(
                                                    CMapData * pMapData,
                                                    DataClusterIGC * pdc,
                                                    SectorID sID)
{
    float flRadius, flTheta, flAdjust;
    SectorID sTeam;

    sTeam = sID / pMapData->GetClustersPerTeam();
    sID = (sID % pMapData->GetClustersPerTeam()) -
            pMapData->GetTeamClustersPerTeam();

    flTheta = (float) (2.0 * pi * sTeam / pMapData->GetTeams());
    if (sID % 3 < 2)
    {
        //
        // This cluster is in the second ring.
        //
        flRadius = 0.65f;

        flAdjust = (float) (pi / (2.0f * pMapData->GetTeams()));
        if (0 == sID)
            flAdjust = -flAdjust;
    }
    else
    {
        flRadius = 0.3f;
        flAdjust = 0.0f;
    }

    flTheta += flAdjust;
    pdc->screenX = cos(flTheta) * flRadius;
    pdc->screenY = sin(flTheta) * flRadius;
}


VOID CmapMakerSnowFlakeIGC::LinkClusters(CMapData * pMapData)
{
    SectorID sID, cTotalClusters, cTeamClustersPerTeam, sIndex;
    SectorID cClustersPerTeam;

    cTotalClusters = pMapData->GetTotalClusters();
    cTeamClustersPerTeam = pMapData->GetTeamClustersPerTeam();
    cClustersPerTeam = pMapData->GetClustersPerTeam();

    for(sID = 0; sID < cTotalClusters; sID += cClustersPerTeam)
    {
        //
        // Link friendly clusters.
        //
        if (cTeamClustersPerTeam > 1)
        {
            for(sIndex = 0; sIndex < cTeamClustersPerTeam - 1; sIndex++)
            {
                CmapMakerIGC::LinkClusters(pMapData,
                                            sID + sIndex,
                                            sID + sIndex + 1);
            }
            CmapMakerIGC::LinkClusters(pMapData, sID + sIndex, sID);
        }

        //
        // Link Neutral clusters.
        //
        sIndex = sID + cTeamClustersPerTeam;
        CmapMakerIGC::LinkClusters(pMapData, sIndex, sIndex + 2);
        CmapMakerIGC::LinkClusters(pMapData, sIndex + 1, sIndex + 2);
        
        //
        // Now we need to link the friendly to the neutral.
        //
        switch(cTeamClustersPerTeam)
        {
            case 3:
                CmapMakerIGC::LinkClusters(pMapData, sID + 1, sIndex);
                CmapMakerIGC::LinkClusters(pMapData, sID + 1, sIndex + 1);
                CmapMakerIGC::LinkClusters(pMapData, sID + 2, sIndex);
                CmapMakerIGC::LinkClusters(pMapData, sID + 2, sIndex + 1);
                break;
            case 2:
                CmapMakerIGC::LinkClusters(pMapData, sID, sIndex);
                CmapMakerIGC::LinkClusters(pMapData, sID, sIndex + 1);
                CmapMakerIGC::LinkClusters(pMapData, sID + 1, sIndex);
                CmapMakerIGC::LinkClusters(pMapData, sID + 1, sIndex + 1);
                break;
            default:
                CmapMakerIGC::LinkClusters(pMapData, sID, sIndex);
                CmapMakerIGC::LinkClusters(pMapData, sID, sIndex + 1);
                break;
        }
    }

    //
    // Now we need to link the second neutral ring.
    //
    sIndex = NA;
    for(sID = cTeamClustersPerTeam;
        sID < cTotalClusters;
        sID += cClustersPerTeam)
    {
        CmapMakerIGC::LinkClusters(pMapData, sID, sID + 1);
        if (NA != sIndex)
            CmapMakerIGC::LinkClusters(pMapData, sIndex, sID);
        sIndex = sID + 1;
    }
    CmapMakerIGC::LinkClusters(pMapData, sIndex, cTeamClustersPerTeam);

    //
    // Cross link the center sectors.
    //
    CmapMakerIGC::CrossLinkClusters(pMapData, cTeamClustersPerTeam + 2);

    //
    // If less than 4 teams, cross linking doesn't do anything.  So
    // make a ring out of them.
    //
    if (pMapData->GetTeams() < 4)
    {
        sIndex = NA;
        for(sID = cTeamClustersPerTeam + 2;
            sID < cTotalClusters;
            sID += cClustersPerTeam)
        {
            if (NA != sIndex)
                CmapMakerIGC::LinkClusters(pMapData, sID, sIndex);
            sIndex = sID;
        }
        if (pMapData->GetTeams() > 2)
        {
            CmapMakerIGC::LinkClusters(pMapData,
                                        sIndex,
                                        cTeamClustersPerTeam + 2);
        }
    }
}


//
// Split Base
//

const char* CmapMakerLargeSplitIGC::IsValid(const MissionParams * pmp)
{
	// yp your_persona removed march 24 2006
    //if (pmp->nMinPlayersPerTeam < 10)
    //  return("Large split maps must have at least 10 players per team; "
    //          "please change the map type, or number of players per team.");
	//
    return(NULL);
}

CmapMakerLargeSplitIGC::CmapMakerLargeSplitIGC()
{
    mMMID = c_mmLargeSplit;
    this->SetName("LargeSplit");
}

CmapMakerInsideOutIGC::CmapMakerInsideOutIGC()
{
    mMMID = c_mmInsideOut;
    this->SetName("Inside out");
}

CmapMakerGridIGC::CmapMakerGridIGC()
{
    mMMID = c_mmGrid;
    this->SetName("Grid");
}


VOID CmapMakerGridIGC::GenerateLayout(CMapData * pMapData)
{
    //pMapData->SetTeamClustersPerTeam(pMapData->GetMaxPlayersOnTeam() >= 30 ? 2 : 1);
	//KGJV-: changed, use now MapSize parameter
    pMapData->SetTeamClustersPerTeam(pMapData->GetMapSize() != 0 ? 2 : 1);
                                    
    pMapData->SetNeutralClustersPerTeam(4);

    pMapData->SetOtherClusters(0);

    this->GenerateRingClusters(pMapData);

    //
    // Now that we have the clusters, link them together.
    //
    this->LinkClusters(pMapData);
}

VOID CmapMakerGridIGC::GenerateTeamClusterScreenPosition(
                                                    CMapData * pMapData,
                                                    DataClusterIGC * pdc,
                                                    SectorID sID)
{
    SectorID sTeam = sID / pMapData->GetClustersPerTeam();
    sID = (sID % pMapData->GetClustersPerTeam());

    float flTheta;
    float flRadius;
    if (pMapData->GetTeamClustersPerTeam() == 1)
    {
        flTheta = (float) (2.0 * pi * sTeam / pMapData->GetTeams());

        static const float  radii[c_cSidesMax + 1] = {0.0f, 0.0f, 0.55f, 0.6f, 0.65f, 0.70f, 0.75f};
        ZAssert (pMapData->GetTeams() <= c_cSidesMax);
        flRadius = radii[pMapData->GetTeams()];
    }
    else
    {
        flTheta = (float) (2.0 * pi * sTeam / pMapData->GetTeams());

        static const float  radii0[c_cSidesMax + 1] = {0.0f, 0.0f, 0.475f, 0.525f, 0.575f, 0.625f, 0.675f};
        static const float  radii1[c_cSidesMax + 1] = {0.0f, 0.0f, 0.625f, 0.675f, 0.725f, 0.775f, 0.825f};

        ZAssert (pMapData->GetTeams() <= c_cSidesMax);
        flRadius = (sID == 0)
                   ? radii0[pMapData->GetTeams()]
                   : radii1[pMapData->GetTeams()];
    }

    pdc->screenX = cos(flTheta) * flRadius;
    pdc->screenY = sin(flTheta) * flRadius;
}

VOID CmapMakerGridIGC::GenerateNeutralClusterScreenPosition(
                                                    CMapData * pMapData,
                                                    DataClusterIGC * pdc,
                                                    SectorID sID)
{
    SectorID sTeam;

    sTeam = sID / pMapData->GetClustersPerTeam();
    sID = (sID % pMapData->GetClustersPerTeam()) - pMapData->GetTeamClustersPerTeam();

    float flTheta1 = (float) (2.0 * pi * sTeam / pMapData->GetTeams());
    float flTheta2 = (float) (2.0 * pi * (sTeam + 1) / pMapData->GetTeams());

    float   flTheta;
    float   flRadius;
    switch (sID)
    {
        case 0:
            flTheta = (5.0f * flTheta1 + flTheta2) / 6.0f;
            flRadius = 1.0f;
        break;
        case 1:
            flTheta = (3.0f * flTheta1 + flTheta2) / 4.0f;
            flRadius = 0.5f;
        break;
        case 2:
            flTheta = (3.0f * flTheta2 + flTheta1) / 4.0f;
            flRadius = 0.5f;
        break;
        case 3:
            flTheta = (5.0f * flTheta2 + flTheta1) / 6.0f;
            flRadius = 1.0f;
        break;
    }

    pdc->screenX = cos(flTheta) * flRadius;
    pdc->screenY = sin(flTheta) * flRadius;
}

VOID CmapMakerGridIGC::LinkClusters(CMapData * pMapData)
{
    if (pMapData->GetTeamClustersPerTeam() != 1)
    {
        //Link the split base clusters for each team
        for (SideID i = 0; (i < pMapData->GetTeams()); i++)
        {
            SectorID    sID = i * pMapData->GetClustersPerTeam();
            CmapMakerIGC::LinkClusters(pMapData, sID, sID + 1);
        }
    }

    {
        SectorID    sidCenter = pMapData->GetTotalClusters() - 1;

        for (SideID i = 0; (i < pMapData->GetTeams()); i++)
        {
            SectorID    sIDh = i * pMapData->GetClustersPerTeam();
            SectorID    sID0 = sIDh +
                               pMapData->GetTeamClustersPerTeam();

            SectorID    sID1 = sID0 + 1;
            SectorID    sID2 = sID0 + 2;
            SectorID    sID3 = sID0 + 3;

            SectorID    sIDn  = ((i+1)%pMapData->GetTeams()) * pMapData->GetClustersPerTeam();
            SectorID    sIDn0 = sIDn +
                                pMapData->GetTeamClustersPerTeam();

            CmapMakerIGC::LinkClusters(pMapData, sID0, sID1);
            CmapMakerIGC::LinkClusters(pMapData, sID1, sID2);
            CmapMakerIGC::LinkClusters(pMapData, sID2, sID3);
            CmapMakerIGC::LinkClusters(pMapData, sID3, sID0);

            CmapMakerIGC::LinkClusters(pMapData, sID2, sIDn0 + 1);
            CmapMakerIGC::LinkClusters(pMapData, sID3, sIDn0 + 0);

            CmapMakerIGC::LinkClusters(pMapData, sID0, sIDh + pMapData->GetTeamClustersPerTeam() - 1);
            CmapMakerIGC::LinkClusters(pMapData, sID1, sIDh);

            CmapMakerIGC::LinkClusters(pMapData, sID2, sIDn);
            CmapMakerIGC::LinkClusters(pMapData, sID3, sIDn + pMapData->GetTeamClustersPerTeam() - 1);

            if (pMapData->GetMissionParams()->iRandomEncounters != 0)
            {
                if (pMapData->GetTeams() == 2)
                    CmapMakerIGC::LinkClusters(pMapData, sID1 + i, sIDn0 + 1 + i);
            }
        }
    }
}

const char* CmapMakerEastWestIGC::IsValid(const MissionParams * pmp)
{
    if (pmp->nTeams != 2) 
        return("East West maps must have exactly two teams; "
                "please change the map type, or the number of teams.");
	// yp your_persona removed march 24 2006
    // else if (pmp->nMinPlayersPerTeam < 10)
    //     return("East West maps must have at least 10 players per team; "
    //            "please change the map type, or the number or players per team.");

    return(NULL);
}

CmapMakerEastWestIGC::CmapMakerEastWestIGC()
{
    mMMID = c_mmEastWest;
    this->SetName("EastWest");
}


VOID CmapMakerEastWestIGC::GenerateLayout(CMapData * pMapData)
{
    pMapData->SetTeamClustersPerTeam(2);
                                    
    pMapData->SetNeutralClustersPerTeam(11);

    pMapData->SetOtherClusters(0);

    this->GenerateRingClusters(pMapData);

    //
    // Now that we have the clusters, link them together.
    //
    this->LinkClusters(pMapData);
}

VOID CmapMakerEastWestIGC::GenerateTeamClusterScreenPosition(
                                                    CMapData * pMapData,
                                                    DataClusterIGC * pdc,
                                                    SectorID sID)
{
    SectorID sTeam = sID / pMapData->GetClustersPerTeam();
    sID = (sID % pMapData->GetClustersPerTeam());

    pdc->screenX = sTeam == 0     ?  0.70f : -0.70f;
    pdc->screenY = (sID == sTeam) ? -0.55f :  0.55f;
}

VOID CmapMakerEastWestIGC::GenerateNeutralClusterScreenPosition(
                                                    CMapData * pMapData,
                                                    DataClusterIGC * pdc,
                                                    SectorID sID)
{
    SectorID sTeam;

    sTeam = sID / pMapData->GetClustersPerTeam();
    sID = (sID % pMapData->GetClustersPerTeam()) - pMapData->GetTeamClustersPerTeam();

    static const float  x[11] = { 1.0f,  1.0f,  0.4f,  0.4f, 1.0f, 1.0f, 0.4f, 0.4f,  0.15f, 0.15f, 0.15f };
    static const float  y[11] = {-0.9f, -0.2f, -0.9f, -0.2f, 0.2f, 0.9f, 0.2f, 0.9f, -0.55f, 0.00f, 0.55f };

    if (sTeam == 0)
    {
        pdc->screenX = x[sID];
        pdc->screenY = y[sID];
    }
    else
    {
        pdc->screenX = -x[sID];
        pdc->screenY = -y[sID];
    }
}

VOID CmapMakerEastWestIGC::LinkClusters(CMapData * pMapData)
{
    ZAssert (pMapData->GetTeams() == 2);
    ZAssert (pMapData->GetClustersPerTeam() == 13);
    ZAssert (pMapData->GetTeamClustersPerTeam() == 2);

    for (SideID i = 0; (i < 2); i++)
    {
        SectorID    sIDf0 = i * 13;
        SectorID    sIDf1 = sIDf0 + 1;
        SectorID    sID0  = sIDf1 + 1;

        CmapMakerIGC::LinkClusters(pMapData, sIDf0, sID0 + 0);
        CmapMakerIGC::LinkClusters(pMapData, sIDf0, sID0 + 1);
        CmapMakerIGC::LinkClusters(pMapData, sIDf0, sID0 + 2);
        CmapMakerIGC::LinkClusters(pMapData, sIDf0, sID0 + 3);



        CmapMakerIGC::LinkClusters(pMapData, sIDf1, sID0 + 4);
        CmapMakerIGC::LinkClusters(pMapData, sIDf1, sID0 + 5);
        CmapMakerIGC::LinkClusters(pMapData, sIDf1, sID0 + 6);
        CmapMakerIGC::LinkClusters(pMapData, sIDf1, sID0 + 7);

        CmapMakerIGC::LinkClusters(pMapData, sID0 + 0, sID0 + 1);
        CmapMakerIGC::LinkClusters(pMapData, sID0 + 1, sID0 + 3);
        CmapMakerIGC::LinkClusters(pMapData, sID0 + 3, sID0 + 2);
        CmapMakerIGC::LinkClusters(pMapData, sID0 + 2, sID0 + 0);

        CmapMakerIGC::LinkClusters(pMapData, sID0 + 4, sID0 + 5);
        CmapMakerIGC::LinkClusters(pMapData, sID0 + 5, sID0 + 7);
        CmapMakerIGC::LinkClusters(pMapData, sID0 + 7, sID0 + 6);
        CmapMakerIGC::LinkClusters(pMapData, sID0 + 6, sID0 + 4);


        CmapMakerIGC::LinkClusters(pMapData, sID0 + 4, sID0 + 1);

        CmapMakerIGC::LinkClusters(pMapData, sID0 + 8, sID0 + 2);
        CmapMakerIGC::LinkClusters(pMapData, sID0 + 8, sID0 + 3);

        CmapMakerIGC::LinkClusters(pMapData, sID0 + 9, sID0 + 3);
        CmapMakerIGC::LinkClusters(pMapData, sID0 + 9, sID0 + 6);

        CmapMakerIGC::LinkClusters(pMapData, sID0 + 10, sID0 + 6);
        CmapMakerIGC::LinkClusters(pMapData, sID0 + 10, sID0 + 7);
    }

    CmapMakerIGC::LinkClusters(pMapData, 10, 25);
    CmapMakerIGC::LinkClusters(pMapData, 11, 24);
    CmapMakerIGC::LinkClusters(pMapData, 12, 23);
}

VOID CmapMakerEastWestIGC::PopulateCluster(CMapData*    pMapData,
                                           IclusterIGC* pcluster,
                                           float        amountHe3)
{
    SectorID    localID = (pcluster->GetObjectID() % pMapData->GetClustersPerTeam());

    static const short  cMineable[13] = {2, 0, 2, 2, 2, 2, 0, 0, 0, 0, 1, 1, 1};
    static const short  cSpecial[13]  = {0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 0, 0, 0};

    ::PopulateCluster(pMapData->GetMission(), pMapData->GetMissionParams(), pcluster, amountHe3,
                    true, true,
                    cMineable[localID], cSpecial[localID]);
}

const char* CmapMakerSplitBaseIGC::IsValid(const MissionParams * pmp)
{
	// yp your_persona removed  march 24 2006
    //if (pmp->nTeams * pmp->nMinPlayersPerTeam < 30)
    //    return("Split base maps must have at least 30 players; "
    //           "please change the map type, the number of teams, or number of players per team.");

    return(NULL);
}
CmapMakerSplitBaseIGC::CmapMakerSplitBaseIGC()
{
    mMMID = c_mmSplitBase;
    this->SetName("Split Base");
}
VOID CmapMakerSplitBaseIGC::GenerateLayout(CMapData * pMapData)
{
    pMapData->SetTeamClustersPerTeam(2);
    pMapData->SetNeutralClustersPerTeam(2);

    this->GenerateRingClusters(pMapData);

    //
    // Now that we have the clusters, link them together.
    //
    this->LinkClusters(pMapData);
}

VOID CmapMakerSplitBaseIGC::GenerateTeamClusterScreenPosition(CMapData * pMapData,
                                                    DataClusterIGC * pdc,
                                                    SectorID sID)
{
    SectorID    n = pMapData->GetTeamClusters();
    SectorID    s = (sID + 1) / 2;
    float flTheta = (float) (2.0 * pi * s / n);
    //float flRadius = pMapData->GetTotalClusters() <= 18 ? 0.8f : 1.0f;
    pdc->screenX = cos(flTheta) * 0.9f;
    pdc->screenY = sin(flTheta) * 0.9f;
}

VOID CmapMakerSplitBaseIGC::GenerateNeutralClusterScreenPosition(CMapData * pMapData,
                                                    DataClusterIGC * pdc,
                                                    SectorID sID)
{
    SectorID    n = pMapData->GetTeamClusters();
    SectorID    s = (sID + 1) / 2;
    float flTheta = (float) (2.0 * pi * s / n);
    //float flRadius = pMapData->GetTotalClusters() <= 18 ? 0.5f : 0.7f;
    pdc->screenX = cos(flTheta) * 0.6f;
    pdc->screenY = sin(flTheta) * 0.6f;
}

VOID CmapMakerSplitBaseIGC::LinkClusters(CMapData * pMapData)
{
    SectorID    cTotalSectors = pMapData->GetTotalClusters();
    for (SideID i = 0; (i < pMapData->GetTeams()); i++)
    {
        SectorID    sID = i * pMapData->GetClustersPerTeam();

        CmapMakerIGC::LinkClusters(pMapData, sID + 1, sID + 2);
        CmapMakerIGC::LinkClusters(pMapData, sID + 2, sID + 3);
        CmapMakerIGC::LinkClusters(pMapData, sID + 3, (sID + 4) % cTotalSectors);
        CmapMakerIGC::LinkClusters(pMapData, sID + 2, (cTotalSectors + (sID - 1)) % cTotalSectors);

        if (pMapData->GetTeams() > 2)
        {
            CmapMakerIGC::LinkClusters(pMapData, sID + 2, (sID + 7) % cTotalSectors);
            if (pMapData->GetTeams() > 3)
                CmapMakerIGC::LinkClusters(pMapData, sID + 3, (cTotalSectors + (sID - 2)) % cTotalSectors);
        }
    }
}

