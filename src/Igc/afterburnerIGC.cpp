/*
**  Copyright (C) 1996, 1997 Microsoft Corporation. All Rights Reserved.
**
**  File:	afterburnerIGC.cpp
**
**  Author: 
**
**  Description:
**      Implementation of the CafterburnerIGC class. This file was initially created by
**  the ATL wizard for the core object.
**
**  History:
*/
// afterburnerIGC.cpp : Implementation of CafterburnerIGC
#include "afterburnerIGC.h"

/////////////////////////////////////////////////////////////////////////////
// CafterburnerIGC
CafterburnerIGC::CafterburnerIGC(void)
:
    m_partType(NULL),
    m_ship(NULL),
    m_fActive(false),
    m_power(0.0f),
    m_mountID(c_mountNA)
{
}

HRESULT     CafterburnerIGC::Initialize(ImissionIGC* pMission, Time now, const void* data, int dataSize)
{
    m_pMission = pMission;

    ZRetailAssert (data && (dataSize == sizeof(DataPartIGC)));
    {
        m_partType = ((DataPartIGC*)data)->partType;
        ZAssert (m_partType);
        m_partType->AddRef();

        m_typeData = (const DataAfterburnerTypeIGC*)m_partType->GetData();
    }

	return S_OK;
}

void        CafterburnerIGC::Terminate(void)
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

void        CafterburnerIGC::Update(Time now)
{
    IncrementalUpdate(m_ship->GetLastUpdate(), now, true);
}

void        CafterburnerIGC::SetShip(IshipIGC*       newVal, Mount mount)
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

void    CafterburnerIGC::SetMountID(Mount newVal)
{
    ZAssert (m_ship);

    if (newVal != m_mountID)
    {
        Deactivate();

        m_ship->MountPart(this, newVal, &m_mountID);
    }
}

void    CafterburnerIGC::IncrementalUpdate(Time lastUpdate, Time now, bool bUseFuel)
{
    ZAssert (m_ship);
    ZAssert (now >= lastUpdate);

    {
        float   dt = now - lastUpdate;

        if (m_mountedFraction < 1.0f)
        {
            if (bUseFuel)
                m_mountedFraction += dt * m_pMission->GetFloatConstant(c_fcidMountRate);

            if (m_mountedFraction >= 1.0f)
            {
                IIgcSite*   pigc = GetMission()->GetIgcSite();
                pigc->PlayNotificationSound(mountedSound, m_ship);
                pigc->PostNotificationText(m_ship, false, "%s ready.", GetPartType()->GetName());
                m_mountedFraction = 1.0f;
            }
            else
                return;
        }

        {
            float   fuel = m_ship->GetFuel();
            bool    bActivated = (m_ship->GetStateM() & afterburnerButtonIGC) && (fuel != 0.0f);
            if (bActivated)
                Activate();

            if (m_fActive)
            {
                if (bActivated)
                {
                    m_power += dt * m_typeData->onRate;
                    if (m_power > 1.0f)
                        m_power = 1.0f;
                }
                else
                {
                    m_power -= dt * m_typeData->offRate;
                    if (m_power <= 0.0f)
                        Deactivate();
                }

                if ((m_power != 0.0f) && bUseFuel)
                {
                    float   fuelUsed = m_power * m_typeData->fuelConsumption * m_typeData->maxThrust * dt;

                    if (fuelUsed < fuel)
                        m_ship->SetFuel(fuel - fuelUsed);
                    else if (fuel != 0.0f)
                    {
                        //Out of gas
                        m_ship->SetFuel(0.0f);
                        Deactivate();
                    }
                }
            }
        }
    }
}
