/*
**  Copyright (C) 1996, 1997 Microsoft Corporation. All Rights Reserved.
**
**  File:	asteroidIGC.cpp
**
**  Author: 
**
**  Description:
**      Implementation of the CasteroidIGC class. This file was initially created by
**  the ATL wizard.
**
**  History:
*/
// asteroidIGC.cpp : Implementation of CasteroidIGC
#include "asteroidigc.h"

/////////////////////////////////////////////////////////////////////////////
// CasteroidIGC
HRESULT     CasteroidIGC::Initialize(ImissionIGC* pMission, Time now, const void* data, int dataSize)
{
	ZeroMemory(&m_builderseensides, sizeof(bool) * c_cSidesMax); //Imago 8/10
    TmodelIGC<IasteroidIGC>::Initialize(pMission, now, data, dataSize);

    ZRetailAssert (data && (dataSize == sizeof(DataAsteroidIGC)));
    {
        DataAsteroidIGC*  dataAsteroid = (DataAsteroidIGC*)data;
        m_asteroidDef = dataAsteroid->asteroidDef;
		
        IclusterIGC*    cluster = pMission->GetCluster(dataAsteroid->clusterID);
        ZRetailAssert (cluster);
        {
            HRESULT rc = Load(0,
                              m_asteroidDef.modelName,
                              m_asteroidDef.textureName,
                              m_asteroidDef.iconName,
                              c_mtStatic | c_mtHitable | c_mtDamagable | c_mtSeenBySide | c_mtPredictable);
            ZAssert (SUCCEEDED(rc));

            SetRadius((float)m_asteroidDef.radius);
            SetPosition(dataAsteroid->position);
            {
                Orientation o(dataAsteroid->forward, dataAsteroid->up);
                SetOrientation(o);
            }
            SetRotation(dataAsteroid->rotation);

            SetSignature(dataAsteroid->signature);

            m_fraction = dataAsteroid->fraction;

            if (dataAsteroid->name[0] != '\0')
                SetName(dataAsteroid->name);
            else
                SetSecondaryName(dataAsteroid->name + 1);

            SetCluster(cluster);

			//Xynth #100 7/2010 Need to initialize oreseenbyside for all sides
			for (SideLinkIGC* psl = this->GetMission()->GetSides()->first(); psl != NULL; psl = psl->next())
			{
				IsideIGC* pside = psl->data();									
				oreSeenBySide.Set(pside, 0.0);									
			}

			m_lastUpdateOre = 0.0;  //Xynth #132 7/2010 Should update first time this is mined
			m_inhibitUpdate = false; //Xynth #225 9/10 		
		    m_inhibitCounter = -1;
#ifdef DEBUG
            {
                //Verify that there is no pre-existing asteroid with the same ID
                for (AsteroidLinkIGC*   pal = pMission->GetAsteroids()->first(); (pal != NULL); pal = pal->next())
                {
                    ZAssert (pal->data()->GetObjectID() != m_asteroidDef.asteroidID);
                }
            }
#endif
            pMission->AddAsteroid(this);
        }
    }

    return S_OK;
}

int         CasteroidIGC::Export(void* data) const
{
    if (data)
    {
        DataAsteroidIGC*  dataAsteroid = (DataAsteroidIGC*)data;
        dataAsteroid->asteroidDef = m_asteroidDef;

        dataAsteroid->position = GetPosition();
        {
            const Orientation&  o = GetOrientation();
            dataAsteroid->forward = o.GetForward();
            dataAsteroid->up      = o.GetUp();
        }
        dataAsteroid->rotation = GetRotation();

        ZAssert (GetCluster());
        dataAsteroid->clusterID = GetCluster()->GetObjectID();
        dataAsteroid->signature = GetSignature();

        dataAsteroid->fraction = m_fraction;
        
        const char* pszName = GetName();
        if (*pszName == '\0')
            memcpy(dataAsteroid->name, GetName(), sizeof(dataAsteroid->name));
        else
            UTL::putName(dataAsteroid->name, GetName());
    }

    return sizeof(DataAsteroidIGC);
}


/////////////////////////////////////////////////////////////////////////////
// IasteroidIGC

struct AsteroidTypeRow
{
    const char* name;
    const char* prefix;
    AsteroidDef def;
};

static const AsteroidTypeRow asteroidTypes[] = 
{
    { "asteroid",       "\0a",  { 0.0f, 0,        0,                      0, 25000, 400,  "bgrnd03", "", "meteoricon" } },
    { "asteroid",       "\0a",  { 0.0f, 0,        0,                      0, 25000, 400,  "bgrnd05", "", "meteoricon" } },
    { "asteroid",       "\0a",  { 0.0f, 0,        c_aabmBuildable,        0, 10000, 200,  "bgrnd03", "", "meteoricon" } },
    { "asteroid",       "\0a",  { 0.0f, 0,        c_aabmBuildable,        0, 10000, 200,  "bgrnd05", "", "meteoricon" } },
    { "Helium 3",       "He",   { 1.0f, 1.0f,     c_aabmMineHe3,          0, 25000, 100,  "bgrnd55", "", "heliumrock" } }, //new he rock #92
    { "Helium 3",       "He",   { 1.0f, 1.0f,     c_aabmMineHe3,          0, 25000, 100,  "bgrnd56", "", "heliumrock" } },
    { "Uranium",        "U",    { 0.0f, 0,        (c_aabmSpecial << 0),   0, 25000, 200,  "bgrnd51", "", "hotrock"    } },
    { "Silicon",        "Si",   { 0.0f, 0,        (c_aabmSpecial << 1),   0, 25000, 200,  "bgrnd52", "", "copperrock" } },
    { "Carbonaceous",   "C",    { 0.0f, 0,        (c_aabmSpecial << 2),   0, 25000, 200,  "bgrnd53", "", "carbonrock" } }
};

// Change number of asteroids below and within functions GetSpecialAsterioid and GetRandomType #92
const int nFirstHugeType = 0;
const int nNumHugeTypes = 2;
const int nFirstGenericType = nFirstHugeType + nNumHugeTypes;
const int nNumGenericTypes = 2;
const int nFirstMinableType = nFirstGenericType + nNumGenericTypes;
const int nNumMinableTypes = 2;
const int nFirstSpecialType = nFirstMinableType + nNumMinableTypes;
const int nNumSpecialTypes = 3;
const int numAsteroidTypes = sizeof(asteroidTypes) / sizeof(AsteroidTypeRow);

static const AsteroidTypeRow& FindAsteroidRow(AsteroidAbilityBitMask aabm)
{
    for (int i = 0; i < numAsteroidTypes; i++)
    {
        if (aabm == asteroidTypes[i].def.aabmCapabilities)
        {
            return asteroidTypes[i];
        }
    }

    ZAssert(false); // asteroid type not found
    return asteroidTypes[0];
}

const char*              IasteroidIGC::GetTypeName(AsteroidAbilityBitMask aabm)
{
    return FindAsteroidRow(aabm).name;
}

const char*              IasteroidIGC::GetTypePrefix(int    index)
{
    return asteroidTypes[index].prefix;
}

const AsteroidDef&       IasteroidIGC::GetTypeDefaults(int index)
{
    return asteroidTypes[index].def;
}

int IasteroidIGC::NumberSpecialAsteroids(const MissionParams*  pmp)
{
    static const int c_nSpecialAsteroidTypes[8] = {3, 1, 1, 2, 1, 2, 2, 3};

    int c = (pmp->bAllowSupremacyPath ? 4 : 0) +
            (pmp->bAllowTacticalPath  ? 2 : 0) +
            (pmp->bAllowExpansionPath ? 1 : 0);

    return c_nSpecialAsteroidTypes[c];
}

int IasteroidIGC::GetSpecialAsterioid(const MissionParams*  pmp, int index)
{
    int n;
    switch ((pmp->bAllowSupremacyPath ? 4 : 0) +
            (pmp->bAllowTacticalPath  ? 2 : 0) +
            (pmp->bAllowExpansionPath ? 1 : 0))
    {
        // Number of special asteroids is also hardcoded here, see #92
        case 0: 
        case 7: 
            n = index % 3; 
        break; 

        case 1: 
            n = 0; 
        break; 

        case 2: 
            n = 1; 
        break; 

        case 3: 
            n = index % 2; 
        break; 

        case 4: 
            n = 2; 
        break; 

        case 5: 
            n = ((index % 2) == 0) ? 0 : 2; 
        break; 

        case 6: 
            n = 1 + (index % 2); 
        break; 
        }

    return n + nFirstSpecialType;
}

int IasteroidIGC::GetRandomType(AsteroidAbilityBitMask aabm)
{
    int index;

    switch (aabm)
    {
	// Number of regular and he3 asteroids is also hardcoded here, see #92
        case 0:
            index = nFirstHugeType + randomInt(0, 1);
        break;

        case c_aabmBuildable:
            index = nFirstGenericType + randomInt(0, 1);
        break;

        case c_aabmMineHe3:
            index = nFirstMinableType + randomInt(0, 1);
        break;

        default:
            ZAssert (false);
    }

    return index;
}
