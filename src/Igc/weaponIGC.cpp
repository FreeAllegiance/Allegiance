/*
**  Copyright (C) 1996, 1997 Microsoft Corporation. All Rights Reserved.
**
**  File:    weaponIGC.cpp
**
**  Author: 
**
**  Description:
**      Implementation of the CweaponIGC class. This file was initially created by
**  the ATL wizard for the weapon object.
**
**  History:
*/
// weaponIGC.cpp : Implementation of CweaponIGC
#include "weaponIGC.h"

/////////////////////////////////////////////////////////////////////////////
// CweaponIGC
CweaponIGC::CweaponIGC(void)
:
    m_partType(NULL),
    m_projectileType(NULL),
    m_ship(NULL),
    m_pshipGunner(NULL),
    m_fActive(false),
    m_fFiringShot(false),
    m_fFiringBurst(false),
    m_mountID(c_mountNA)
{
}

HRESULT     CweaponIGC::Initialize(ImissionIGC* pMission, Time now, const void* data, int dataSize)
{
    ZAssert (pMission);
    m_pMission = pMission;

    ZRetailAssert (data && (dataSize == sizeof(DataPartIGC)));
    {
        m_partType = ((DataPartIGC*)data)->partType;
        ZAssert (m_partType);
        m_partType->AddRef();

        m_typeData = (const DataWeaponTypeIGC*)m_partType->GetData();

        m_projectileType = m_pMission->GetProjectileType(m_typeData->projectileTypeID);
        ZRetailAssert (m_projectileType);
        m_projectileType->AddRef();
    }

    return S_OK;
}

void        CweaponIGC::Terminate(void)
{
    AddRef();

    if (m_pshipGunner)
    {
        SetGunner(NULL);
    }

    SetShip(NULL, NA);

    if (m_partType)
    {
        m_partType->Release();
        m_partType = NULL;
    }

    if (m_projectileType)
    {
        m_projectileType->Release();
        m_projectileType = NULL;
    }

    Release();
}
void        CweaponIGC::SetShip(IshipIGC*       newVal, Mount mount)
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

void    CweaponIGC::SetMountID(Mount newVal)
{
    ZAssert (m_ship);

    if (newVal != m_mountID)
    {
        Deactivate();
        if (m_pshipGunner)
            SetGunner(NULL);

        m_ship->MountPart(this, newVal, &m_mountID);

        if (newVal >= 0)
        {
            //Get the corresponding hardpoint: we know one exists because the
            //ship allowed the part to be mounted.
            const IhullTypeIGC* pht = m_ship->GetBaseHullType();
            ZAssert (pht);

            m_pposition = &(pht->GetWeaponPosition(newVal));
            m_porientation = &(pht->GetWeaponOrientation(newVal));

            //Is there a turret gunner in this position (who does not have a gun)
            {
                for (ShipLinkIGC*   psl = m_ship->GetChildShips()->first(); (psl != NULL); psl = psl->next())
                {
                    IshipIGC*   pship = psl->data();
                    if (pship->GetTurretID() == newVal)
                    {
                        SetGunner(pship);
                        break;
                    }
                }
            }
        }
    }
}

void        CweaponIGC::FireWeapon(Time now)
{
    ZAssert (m_mountID >= 0);
    ZAssert (m_ship);

    bool    fFiredShot = false;

    if (m_fActive && (m_nextFire < now))
    {
        Time    lastUpdate = m_ship->GetLastUpdate();

        //Never fire retroactively.
        if (m_nextFire < lastUpdate)
            m_nextFire = lastUpdate;

        bool    fSelected = fIsWeaponSelected();

        float   energy = m_ship->GetEnergy();                   //the energy the ship would have at "now"
        if (energy >= m_typeData->energyPerShot)
        {
            //We'd be able to fire before now and would have enough energy at now to fire, so ...
            float   rechargeRate = m_ship->GetHullType()->GetRechargeRate();

            float   energyDeficit = (m_nextFire - now) *            //this is how much we are in the hole because we
                                    rechargeRate;                   //are shooting sooner than "now" (must be < 0)

            short     ammo = m_ship->GetAmmo();

            if ((ammo > 0) || (m_typeData->cAmmoPerShot == 0))
            {
                // we are firing at least once this frame
                fFiredShot = true;
                m_pMission->GetIgcSite()->PlayFFEffect(effectFire, m_pshipGunner ? m_pshipGunner : m_ship);

                //Get the stuff that won't change between shots
                IclusterIGC*    cluster = m_ship->GetCluster();
                ZAssert (cluster);

                const Orientation&  shipOrientation = m_ship->GetOrientation();

                //Orientation         orientationBfr;
                const Orientation*  pMyOrientation;
                if (m_pshipGunner)
                {
                    /*
                    orientationBfr = m_pshipGunner->GetOrientation() * (*m_porientation * shipOrientation);
                    pMyOrientation = &orientationBfr;
                    */
                    pMyOrientation = &(m_pshipGunner->GetOrientation());
                }
                else
                {
                    pMyOrientation = &shipOrientation;
                }
            
                //This is how much energy deficit recovers between shots
                float   dtimeBurst = GetDtBurst();

                float   recharge = rechargeRate * dtimeBurst;

                const Vector&       myVelocity      = m_ship->GetVelocity();
                Vector              myPosition      = m_ship->GetPosition() + *m_pposition * shipOrientation; //*pMyOrientation;


                /*
                m_ship->GetThingSite()->AddMuzzleFlare(
				    m_emissionPt, 
				    std::min(dtimeBurst * 0.5f, 0.05f)
			    );
                */

                DataProjectileIGC   dataProjectile;
                dataProjectile.projectileTypeID = m_projectileType->GetObjectID();

                float   lifespan = GetLifespan();

                float   speed = m_projectileType->GetSpeed();
                if (m_typeData->cAmmoPerShot)
                    speed *= m_ship->GetSide()->GetGlobalAttributeSet().GetAttribute(c_gaSpeedAmmo);
                bool    absoluteF = m_projectileType->GetAbsoluteF();

                const Vector*   ppositionTarget;
                const Vector*   pvelocityTarget;
                ImodelIGC*  ptarget = NULL;
                if (m_projectileType->GetBlastPower() != 0.0f)
                {
                    if (m_pshipGunner)
                        ptarget = m_pshipGunner->GetCommandTarget(c_cmdCurrent);
                    else
                        ptarget = m_ship->GetCommandTarget(c_cmdCurrent);

                    if (ptarget)
                    {
                        if ((ptarget->GetCluster() == cluster) && m_ship->CanSee(ptarget))
                        {
                            ppositionTarget = &(ptarget->GetPosition());
                            pvelocityTarget = &(ptarget->GetVelocity());
                        }
                        else
                            ptarget = NULL;
                    }
                }

                int nShots = fSelected ? 10 : 0;    //Only allow a single shot if the weapon is no longer selected

                do
                {
                    if (energy + energyDeficit < m_typeData->energyPerShot)
                    {
                        //We don't have enough energy to fire at our prefered time ... so wait until we do.
                        m_nextFire += (m_typeData->energyPerShot - (energy + energyDeficit)) / rechargeRate;
                    }

                    //Permute the "forward" direction slightly by a random amount
                    dataProjectile.forward = pMyOrientation->GetForward();
                    if (m_typeData->dispersion != 0.0f)
                    {
                        float   r = random(0.0f, m_typeData->dispersion);
                        float   a = random(0.0f, 2.0f * pi);
                        dataProjectile.forward += (r * cos(a)) * pMyOrientation->GetRight();
                        dataProjectile.forward += (r * sin(a)) * pMyOrientation->GetUp();

                        dataProjectile.forward.SetNormalize();
                    }

                    dataProjectile.velocity = speed * dataProjectile.forward;
                    if (!absoluteF)
                        dataProjectile.velocity += myVelocity;

                    Vector  position = myPosition + myVelocity * (m_nextFire - lastUpdate);
                    dataProjectile.lifespan = lifespan;
                    if (ptarget)
                    {
                        Vector      dV = *pvelocityTarget - dataProjectile.velocity;

                        float       dV2 = dV.LengthSquared();
                        if (dV2 != 0.0f)
                        {
                            Vector      dP = position - *ppositionTarget;  //reverse so time has right sense

                            dataProjectile.lifespan = (dV * dP) / dV2;
                            if (dataProjectile.lifespan > lifespan)
                                dataProjectile.lifespan = lifespan;
                            else if (dataProjectile.lifespan < 0.1f)
                                dataProjectile.lifespan = 0.1f;     //Don't let it explode in our face
                        }
                    }

                    IprojectileIGC*  p = (IprojectileIGC*)(m_pMission->CreateObject(m_nextFire, OT_projectile, 
                                                                                    &dataProjectile, sizeof(dataProjectile)));
                    if (p)
                    {
                        if (m_pshipGunner)
                            p->SetGunner(m_pshipGunner);
                        else
                            p->SetLauncher(m_ship);

                        p->SetPosition(position);

                        p->SetCluster(cluster);

                        p->Release();
                    }

                    energyDeficit += rechargeRate * dtimeBurst;
                    energy -= m_typeData->energyPerShot;
                    ammo -= m_typeData->cAmmoPerShot;

                    m_nextFire += dtimeBurst;
                }
                while ((nShots-- > 0) &&
                       (m_nextFire < now) &&
                       (energy + energyDeficit >= m_typeData->energyPerShot) &&
                       (ammo > 0));

                m_ship->SetAmmo(ammo > 0 ? ammo : 0);

                if ((ammo == 0) && (m_typeData->cAmmoPerShot != 0))
                    fSelected = false;

                m_ship->SetEnergy(energy);
            }
        }

        if (!fSelected)
            Deactivate();
    }

    // if we fired a shot, keep track of it (note - stored localy because the 
    // call to deactivate (above) clears the member variable).
    m_fFiringShot = fFiredShot;
 
    // if we are still firing and have the energy and ammo for the next shot,
    // assume we are firing a burst.
    if ((m_ship->GetEnergy() >= m_typeData->energyPerShot)
        && (m_ship->GetAmmo() >= m_typeData->cAmmoPerShot)
        && (m_fFiringBurst || (m_fActive && m_fFiringShot)) // a burst starts with a shot
        )
    {
        m_fFiringBurst = true;
    }
    else
    {
        m_fFiringBurst = false;
    }

}
