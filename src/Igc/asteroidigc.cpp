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
#include "pch.h"
#include "asteroidIGC.h"

/////////////////////////////////////////////////////////////////////////////
// CasteroidIGC
HRESULT     CasteroidIGC::Initialize(ImissionIGC* pMission, Time now, const void* data, int dataSize)
{
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
            assert (SUCCEEDED(rc));

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

#ifdef DEBUG
            {
                //Verify that there is no pre-existing asteroid with the same ID
                for (AsteroidLinkIGC*   pal = pMission->GetAsteroids()->first(); (pal != NULL); pal = pal->next())
                {
                    assert (pal->data()->GetObjectID() != m_asteroidDef.asteroidID);
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

        assert (GetCluster());
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

static const AsteroidTypeRow asteroidTypes[] =  //andon, added more 6/10
{
    { "asteroid",		"\0a",  { 0.0f, 0,        0,                      0, 25000, 400,  "bgrnd03", "", "meteoricon" } },
    { "asteroid",		"\0a",  { 0.0f, 0,        0,                      0, 25000, 400,  "bgrnd05", "", "meteoricon" } },
    { "asteroid",		"\0a",  { 0.0f, 0,        0,                      0, 25000, 400,  "bgrnd06", "", "meteoricon" } },
    { "asteroid",		"\0a",  { 0.0f, 0,        0,                      0, 25000, 400,  "bgrnd07", "", "meteoricon" } },
    { "asteroid",		"\0a",  { 0.0f, 0,        c_aabmBuildable,        0, 10000, 200,  "bgrnd03", "", "meteoricon" } },
    { "asteroid",		"\0a",  { 0.0f, 0,        c_aabmBuildable,        0, 10000, 200,  "bgrnd05", "", "meteoricon" } },
    { "asteroid",		"\0a",  { 0.0f, 0,        c_aabmBuildable,        0, 10000, 200,  "bgrnd06", "", "meteoricon" } },
    { "asteroid",		"\0a",  { 0.0f, 0,        c_aabmBuildable,        0, 10000, 200,  "bgrnd07", "", "meteoricon" } },
    { "Helium 3",		"He",   { 1.0f, 1.0f,     c_aabmMineHe3,          0, 25000, 100,  "bgrnd55", "", "heliumrock" } },
    { "Helium 3",		"He",   { 1.0f, 1.0f,     c_aabmMineHe3,          0, 25000, 100,  "bgrnd56", "", "heliumrock" } },
    { "Uranium",		"U",    { 0.0f, 0,        (c_aabmSpecial << 0),   0, 25000, 200,  "bgrnd51", "", "hotrock"    } },
    { "Uranium",		"U",    { 0.0f, 0,        (c_aabmSpecial << 0),   0, 25000, 200,  "bgrnd59", "", "hotrock"    } },
    { "Silicon",		"Si",   { 0.0f, 0,        (c_aabmSpecial << 1),   0, 25000, 200,  "bgrnd50", "", "copperrock" } },
    { "Silicon",		"Si",   { 0.0f, 0,        (c_aabmSpecial << 1),   0, 25000, 200,  "bgrnd58", "", "copperrock" } },
    { "Carbonaceous",	"C",    { 0.0f, 0,        (c_aabmSpecial << 2),   0, 25000, 200,  "bgrnd52", "", "carbonrock" } },
    { "Carbonaceous",	"C",    { 0.0f, 0,        (c_aabmSpecial << 2),   0, 25000, 200,  "bgrnd53", "", "carbonrock" } },
};

const int nFirstHugeType = 0;
const int nNumHugeTypes = 4; //andon increased, was 2 6/10
const int nFirstGenericType = nFirstHugeType + nNumHugeTypes;
const int nNumGenericTypes = 4; //andon increased, was 2 6/10
const int nFirstMinableType = nFirstGenericType + nNumGenericTypes;
const int nNumMinableTypes = 2;
const int nFirstSpecialType = nFirstMinableType + nNumMinableTypes;
const int nNumSpecialTypes = 6; //andon increased, was 3 6/10 - shouldn't we make this be 6? --Imago CHANGED to 6 6/10
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
        case 0:
        case 7:
            n = index % 6; //andon, was 3 6/10
        break;

        case 1:
            n = 0;
        break;

        case 2:
            n = 2;
        break;

        case 3:
            n = index % 4; //andon, was 2 6/10
        break;

        case 4:
            n = 4;
        break;

        case 5:
            n = ((index % 4) == 0) ? 0 : 4; //andon, was 2s 6/10
        break;

        case 6:
            n = 1 + (index % 4); //andon, 4 was 2 6/10
        break;
    }

    return n + nFirstSpecialType + randomInt(0, 1); //andon, added + randomInt(0, 1) 6/10
}

int IasteroidIGC::GetRandomType(AsteroidAbilityBitMask aabm)
{
    int index;

    switch (aabm)
    {
        case 0:
            index = nFirstHugeType + randomInt(0, 3); //andon, increased from 1 to 3 6/10
        break;

        case c_aabmBuildable:
            index = nFirstGenericType + randomInt(0, 3); //andon, increased from 1 to 3 6/10
        break;

        case c_aabmMineHe3:
            index = nFirstMinableType + randomInt(0, 1); //andon, added + randomInt(0, 1) 6/10
        break;

        default:
            assert (false);
    }

    return index;
}
