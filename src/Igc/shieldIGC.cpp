/*
**  Copyright (C) 1996, 1997 Microsoft Corporation. All Rights Reserved.
**
**  File:	shieldIGC.cpp
**
**  Author: 
**
**  Description:
**      Implementation of the CshieldIGC class. This file was initially created by
**  the ATL wizard for the core object.
**
**  History:
*/
// shieldIGC.cpp : Implementation of CshieldIGC
#include "shieldIGC.h"

/////////////////////////////////////////////////////////////////////////////
// CshieldIGC
CshieldIGC::CshieldIGC(void)
:
    m_partType(NULL),
    m_ship(NULL),
    m_fraction(0.0f),
    m_mountID(c_mountNA)
{
}

HRESULT     CshieldIGC::Initialize(ImissionIGC* pMission, Time now, const void* data, int dataSize)
{
    m_pMission = pMission;

    ZRetailAssert (data && (dataSize == sizeof(DataPartIGC)));
    {
        m_partType = ((DataPartIGC*)data)->partType;
        ZAssert (m_partType);
        m_partType->AddRef();

        m_typeData = (const DataShieldTypeIGC*)m_partType->GetData();
    }

	return S_OK;
}

void        CshieldIGC::Terminate(void)
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

void        CshieldIGC::Update(Time now)
{
    ZAssert (m_ship);
    ZAssert (m_mountID == 0);

    Time    lastUpdate = m_ship->GetLastUpdate();
    ZAssert (now >= lastUpdate);

    float dt = now - lastUpdate;


    if (m_mountedFraction < 1.0f)
    {
        //We can't ZAssert that the shield fraction is 0 because
        //the shield fraction is directly set via updates from the server
        //and the server times for mounting/dismounting might be different
        //from this client
        
        m_mountedFraction += dt * m_pMission->GetFloatConstant(c_fcidMountRate);
        if (m_mountedFraction < 1.0f)
            return;

        IIgcSite*   pigc = m_pMission->GetIgcSite();
        pigc->PlayNotificationSound(mountedSound, m_ship);
        pigc->PostNotificationText(m_ship, false, "%s ready.", GetPartType()->GetName());
        m_mountedFraction = 1.0f;
    }

    float previousFraction = m_fraction;

    //Use set fraction so that the ship's shape will change if appropriate
    SetFraction(m_fraction + GetRegeneration() * dt / GetMaxStrength());

    // if we have managed to recharge above 5%, play a shield-powerup sound
    if (previousFraction < 0.05 && m_fraction >= 0.05)
        m_pMission->GetIgcSite()->PlaySoundEffect(m_typeData->activateSound, m_ship);

}
void        CshieldIGC::SetShip(IshipIGC*       newVal, Mount mount)
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
void    CshieldIGC::SetMountID(Mount newVal)
{
    ZAssert (m_ship);

    if (newVal != m_mountID)
    {
        if ((newVal < 0) && (m_mountID == 0))
            Deactivate();

        m_ship->MountPart(this, newVal, &m_mountID);

        if (newVal == 0)
        {
            m_fraction = 0.0f;
            Activate();         //If it is mounted ... it is active
        }
    }
}

float   CshieldIGC::ApplyDamage(Time           timeCollision,
                                DamageTypeID   type,
                                float          amount,
                                const Vector&  deltaP)
{
    float   dtm = m_pMission->GetDamageConstant(type, m_typeData->defenseType);
    amount *= dtm;

    float leakage;

    if (amount > 0.0)
    {
        ZAssert (dtm > 0.0f);

        //how much damage will the shield actually stop?
        float   maxStrength = GetMaxStrength();
        float   absorbed = amount / maxStrength;

        float down = m_pMission->GetFloatConstant(c_fcidDownedShield);

        if (m_fraction <= down)
        {
            //The shield was previously knocked negative ... it doesn't absorb anything
            leakage = amount / dtm;
        }
        else
        {
            float   usefulShieldFraction = m_fraction - down;
            ZAssert (usefulShieldFraction >= 0.0f);

            if ((type & c_dmgidNoFlare) == 0)
                m_ship->GetThingSite()->AddFlare(timeCollision, deltaP, 0, m_ship->GetHitTest()->GetEllipseEquation());

            if (absorbed <= usefulShieldFraction)
            {
                //Normal case ... the shield is strong enough to abosrb the entire hit
                leakage = 0.0f;
            }
            else
            {
                //Abnormal case ... the shield isn't strong enough: so use normal shield behavior on the
                //portion that can be absorbed and apply the rest as leakage.
                leakage = (absorbed - usefulShieldFraction) * maxStrength / dtm;
                ZAssert (leakage >= 0.0f);

                m_pMission->GetIgcSite()->PlaySoundEffect(m_typeData->deactivateSound, m_ship);
                m_pMission->GetIgcSite()->PlayNotificationSound(salShieldsOfflineSound, m_ship);
            }
        }
        SetFraction(m_fraction - absorbed);
    }
    else
        leakage = 0.0f;

    return leakage;
}
