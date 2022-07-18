/*
**  Copyright (C) 1996, 1997 Microsoft Corporation. All Rights Reserved.
**
**  File:	cloakIGC.cpp
**
**  Author: 
**
**  Description:
**      Implementation of the CcloakIGC class. This file was initially created by
**  the ATL wizard for the core object.
**
**  History:
*/
// cloakIGC.cpp : Implementation of CcloakIGC
#include "cloakigc.h"

/////////////////////////////////////////////////////////////////////////////
// CcloakIGC
CcloakIGC::CcloakIGC(void)
:
    m_partType(NULL),
    m_ship(NULL),
    m_mountID(c_mountNA),
    m_fActive(false)
{
}

HRESULT     CcloakIGC::Initialize(ImissionIGC* pMission, Time now, const void* data, int dataSize)
{
    m_pMission = pMission;

    ZRetailAssert (data && (dataSize == sizeof(DataPartIGC)));
    {
        m_partType = ((DataPartIGC*)data)->partType;
        ZAssert (m_partType);
        m_partType->AddRef();

        m_typeData = (const DataCloakTypeIGC*)m_partType->GetData();
    }

	return S_OK;
}

void        CcloakIGC::Terminate(void)
{
    AddRef();

    SetShip(NULL, NA);

    if (m_partType)
    {
        m_partType->Release();
        m_partType = NULL;
    }

    Release();
}

void        CcloakIGC::Update(Time now)
{
    ZAssert (m_ship);
    ZAssert (m_mountID == 0);

    Time    lastUpdate = m_ship->GetLastUpdate();
    ZAssert (now >= lastUpdate);

    float dt = now - lastUpdate;
    if (m_mountedFraction < 1.0f)
    {
        m_mountedFraction += dt * m_pMission->GetFloatConstant(c_fcidMountRate);
        if (m_mountedFraction > 1.0f)
            m_mountedFraction = 1.0f;

        IIgcSite*   pigc = GetMission()->GetIgcSite();
        pigc->PlayNotificationSound(mountedSound, m_ship);
        pigc->PostNotificationText(m_ship, false, "%s ready.", GetPartType()->GetName());
        m_mountedFraction = 1.0f;
    }

    bool    bActive = (m_ship->GetStateM() & cloakActiveIGC) &&
                      (m_mountedFraction >= 1.0f);
    if (bActive)
        Activate();

    if (m_fActive)
    {
        float   maxCloaking = bActive ? m_typeData->maxCloaking : 0.0f;

        float   energyNeeded = m_typeData->energyConsumption * dt;
        float   energy = m_ship->GetEnergy();
        if (energy >= energyNeeded)
        {
            //Have enough energy to run the cloak at full power
            energy -= energyNeeded;
        }
        else
        {
            //Not enough energy to run the cloak at full power
            float   r = (energy / energyNeeded);
            maxCloaking *= r * r;
            energy = 0.0f;
        }
        m_ship->SetEnergy(energy);

        if (maxCloaking > m_cloaking)
        {
            m_cloaking += dt * m_typeData->onRate;
            if (m_cloaking > maxCloaking)
                m_cloaking = maxCloaking;
        }
        else
        {
            m_cloaking -= dt * m_typeData->offRate;
            if (m_cloaking < maxCloaking)
                m_cloaking = maxCloaking;
        }

        if ((!bActive) && (m_cloaking == 0.0f))
            Deactivate();
        else
            m_ship->SetCloaking(1.0f - m_cloaking);
    }
}

void        CcloakIGC::SetShip(IshipIGC*       newVal, Mount mount)
{
    //There may be only a single reference to this part ... so make sure the part
    //does not get deleted midway through things
    AddRef();

    if (m_ship)
    {
        m_ship->DeletePart(this);
        m_ship->Release();
    }
    ZAssert (m_mountID == c_mountNA);

    m_ship = newVal;

    if (m_ship)
    {
        m_ship->AddRef();
        m_ship->AddPart(this);

        SetMountID(mount);
    }

    Release();
}

void    CcloakIGC::SetMountID(Mount newVal)
{
    ZAssert (m_ship);

    if (newVal != m_mountID)
    {
        Deactivate();

        m_ship->MountPart(this, newVal, &m_mountID);
    }
}
