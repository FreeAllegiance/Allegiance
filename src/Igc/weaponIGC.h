/*
**  Copyright (C) 1996, 1997 Microsoft Corporation. All Rights Reserved.
**
**  File:    weaponIGC.h
**
**  Author: 
**
**  Description:
**      Header for the CweaponIGC class. This file was initially created by
**  the ATL wizard.
**
**  History:
*/
// weaponIGC.h : Declaration of the CweaponIGC

#ifndef __WEAPONIGC_H_
#define __WEAPONIGC_H_
#include "igc.h"

/////////////////////////////////////////////////////////////////////////////
// CweaponIGC
class CweaponIGC : public IweaponIGC
{
    public:
        CweaponIGC(void);

    // IbaseIGC
        virtual HRESULT Initialize(ImissionIGC* pMission, Time now, const void* data, int dataSize);
        virtual void    Terminate(void);
        virtual void    Update(Time now)
        {

            if (m_mountedFraction < 1.0f)
            {
                float dt = now - m_ship->GetLastUpdate();

                m_mountedFraction += dt * m_pMission->GetFloatConstant(c_fcidMountRate);
                if (m_mountedFraction < 1.0f)
                {
                    m_fFiringShot = false;
                    m_fFiringBurst = false;
                    return;
                }

                IIgcSite*   pigc = GetMission()->GetIgcSite();
                pigc->PlayNotificationSound(mountedSound, m_ship);
                pigc->PostNotificationText(m_ship, false, "%s ready.", GetPartType()->GetName());
                m_mountedFraction = 1.0f;
            }

            if ((!m_fActive) && fIsWeaponSelected())
                Activate();

            if (m_fActive)
            {
                FireWeapon(now);
            }
            else
            {
                m_fFiringShot = false;
                m_fFiringBurst = false;
            }
        }

        virtual ObjectType  GetObjectType(void) const
        {
            return OT_weapon;
        }

        virtual ImissionIGC*    GetMission(void) const
        {
            return m_pMission;
        }

    // IpartIGC
        virtual EquipmentType    GetEquipmentType(void) const
        {
            return ET_Weapon;
        }

        virtual IpartTypeIGC*    GetPartType(void) const
        {
            return m_partType;
        }

        virtual IshipIGC*        GetShip(void) const
        {
            return m_ship;
        }
        virtual void             SetShip(IshipIGC* newVal, Mount mount);

        virtual Mount            GetMountID(void) const
        {
            return m_mountID;
        }
        virtual void             SetMountID(Mount newVal);

        virtual bool             fActive(void) const
        {
            return m_fActive;
        }

        virtual void             Activate(void)
        {
            ZAssert (m_ship);

            if (!m_fActive)
            {
                m_ship->ChangeSignature(m_typeData->signature);

                m_nextFire = m_ship->GetLastUpdate() + m_typeData->dtimeBurst;

                m_fActive = true;
            }
        }
        virtual void             Deactivate(void)
        {
            ZAssert (m_ship);

            if (m_fActive)
            {
                m_ship->ChangeSignature(-m_typeData->signature);
                m_fActive = false;
            }

            m_fFiringBurst = false;
            m_fFiringShot = false;  // REVIEW: this may lead to a silent shot
        }
        virtual float            GetMass(void) const
        {
            return m_typeData->mass;
        }

        virtual Money            GetPrice(void) const
        {
            return m_typeData->price;
        }

        virtual float           GetMountedFraction(void) const
        {
            return m_mountedFraction;
        }

        virtual void            SetMountedFraction(float f)
        {
            m_mountedFraction = f;
        }

        virtual void            Arm(void)
        {
            m_mountedFraction = 1.0f;
            m_nextFire = m_ship->GetLastUpdate();
        }

    // IweaponIGC
        virtual IprojectileTypeIGC*  GetProjectileType(void) const
        {
            return m_projectileType;
        }
        virtual short                GetAmmoPerShot(void) const
        {
            return m_typeData->cAmmoPerShot;
        }

        virtual float               GetLifespan(void) const
        {
            float   ls = m_projectileType->GetLifespan();
            if (m_ship && (m_typeData->energyPerShot != 0.0f))
                ls *= m_ship->GetSide()->GetGlobalAttributeSet().GetAttribute(c_gaLifespanEnergy);

            return ls;
        }

        virtual const Vector&       GetPosition(void) const
        {
            return *m_pposition;
        }

        virtual float               GetDtBurst(void) const
        {
            return m_typeData->dtimeBurst;
        }

        virtual void                 SetGunner(IshipIGC* pship)
        {
            Deactivate();

            if (m_pshipGunner)
                m_pshipGunner->Release();

            m_pshipGunner = pship;

            if (pship)
            {
                ZAssert (m_ship);
                ZAssert (pship->GetParentShip() == m_ship);
                pship->AddRef();

                ZAssert (m_mountID >= 0);
                ZAssert (pship->GetParentShip());
                ZAssert (pship->GetParentShip()->GetHullType()->GetHardpointData(m_mountID).bFixed == false);
            }
        }

        virtual IshipIGC*            GetGunner(void) const
        {
            return m_pshipGunner;
        }

        virtual bool                 fFiringShot(void) const
        {
            return m_fFiringShot;
        }

        virtual bool                 fFiringBurst(void) const
        {
            return m_fFiringBurst;
        }

        virtual SoundID              GetActivateSound(void) const
        {
            return m_typeData->activateSound;
        }

        virtual SoundID              GetSingleShotSound(void) const
        {
            return m_typeData->singleShotSound;
        }

        virtual SoundID              GetBurstSound(void) const
        {
            return m_typeData->burstSound;
        }

    private:
        bool                        fIsWeaponSelected(void) const
        {
            bool    fSelected;
            if (m_ship->GetHullType()->GetHardpointData(m_mountID).bFixed)
            {
                int stateM = m_ship->GetStateM();

                fSelected = ((stateM & allWeaponsIGC) ||
                             ((stateM & oneWeaponIGC) &&
                              (m_mountID == ((stateM & selectedWeaponMaskIGC) >> selectedWeaponShiftIGC)))) &&
                            (m_ship->GetRipcordModel() == NULL);
            }
            else
            {
                fSelected = m_pshipGunner && (m_pshipGunner->GetStateM() & weaponsMaskIGC);
            }

            return fSelected;
        }

        void                        FireWeapon(Time   now);

        ImissionIGC*                m_pMission;

        IshipIGC*                   m_pshipGunner;

        const DataWeaponTypeIGC*    m_typeData;
        IpartTypeIGC*               m_partType;
        IshipIGC*                   m_ship;

        IprojectileTypeIGC*         m_projectileType;
        Time                        m_nextFire;

        const Vector*               m_pposition;
        const Orientation*          m_porientation;
        float                       m_mountedFraction;

        Mount                       m_mountID;
        bool                        m_fActive;
        bool                        m_fFiringShot;
        bool                        m_fFiringBurst;
};

#endif //__WEAPONIGC_H_
