/*
**  Copyright (C) 1996, 1997 Microsoft Corporation. All Rights Reserved.
**
**  File:	magazineIGC.h
**
**  Author: 
**
**  Description:
**      Header for the CmagazineIGC class. This file was initially created by
**  the ATL wizard.
**
**  History:
*/
// magazineIGC.h : Declaration of the CmagazineIGC

#ifndef __MAGAZINEIGC_H_
#define __MAGAZINEIGC_H_
#include "igc.h"

/////////////////////////////////////////////////////////////////////////////
// CmagazineIGC
class CmagazineIGC : public ImagazineIGC
{
    public:
        CmagazineIGC(void);
        ~CmagazineIGC(void);

// IbaseIGC
        virtual HRESULT         Initialize(ImissionIGC* pMission, Time now, const void* data, int dataSize);
        virtual void            Terminate(void);
        virtual void            Update(Time now);

        virtual ObjectType      GetObjectType(void) const
        {
            return OT_magazine;
        }

        virtual ImissionIGC*    GetMission(void) const
        {
            return m_pMission;
        }

// IpartIGC
        virtual EquipmentType    GetEquipmentType(void) const
        {
            return ET_Magazine;
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
            return (m_mountID == 0);
        }

        virtual void             Activate(void)
        {
            m_timeLoaded = m_ship->GetLastUpdate() + ((1.0f/m_pMission->GetFloatConstant(c_fcidMountRate)) + m_missileType->GetLoadTime());
            m_ship->ChangeSignature(m_missileType->GetLauncherDef()->signature);
        }
        virtual void             Deactivate(void)
        {
            m_ship->ChangeSignature(-m_missileType->GetLauncherDef()->signature);
        }
        virtual float               GetMass(void) const
        {
            return m_amount * m_missileType->GetMass();
        }

        virtual Money            GetPrice(void) const
        {
            return ((Money)m_amount) * m_missileType->GetLauncherDef()->price;
        }

        virtual float           GetMountedFraction(void) const
        {
            return m_mountedFraction;
        }

        virtual void           SetMountedFraction(float f)
        {
            m_mountedFraction = f;
        }

        virtual void            Arm(void)
        {
            m_mountedFraction = 1.0f;
            m_timeLoaded = m_ship->GetLastUpdate();
        }

    // IlauncherIGC
        virtual void                SetAmount(short a)
        {
            {
                short   maxAmount = m_partType->GetAmount(m_ship);
                if (a > maxAmount)
                    a = maxAmount;
            }

            if (a != m_amount)
            {
                //Adjust the mass of the ship (if there is one) to account for the change in the number of missiles
                if (m_ship)
                {
                    m_ship->SetMass(m_ship->GetMass() +
                                    (a - m_amount) * m_missileType->GetMass());
                    m_pMission->GetIgcSite()->LoadoutChangeEvent(m_ship, this, c_lcQuantityChange);
                }

                m_amount = a;
            }
        }
        virtual short               GetAmount(void) const
        {
            return m_amount;
        }

        virtual void                SetTimeFired(Time   now)
        {
            m_timeLoaded = now + m_missileType->GetLoadTime();
        }

        virtual Time                GetTimeLoaded(void) const
        {
            return m_timeLoaded;
        }
        virtual void                SetTimeLoaded(Time  tl)
        {
            m_timeLoaded = tl;
        }
        virtual void                ResetTimeLoaded(void)
        {
            ZAssert (m_ship);
                m_timeLoaded = m_ship->GetLastUpdate() + ((1.0f/m_pMission->GetFloatConstant(c_fcidMountRate)) + m_missileType->GetLoadTime());
        }

        virtual const Vector&        GetEmissionPt(void) const
        {
            return m_emissionPt;
        }
        virtual float               GetArmedFraction(void) 
        {
            if (GetMountedFraction() < 1.0f)
            {
                return 0;
            }
            else
            {
                return 1.0f - std::max(0.0f, (m_timeLoaded - Time::Now()) / m_missileType->GetLoadTime());
            }
        }

    // ImagazineIGC
        virtual ImissileTypeIGC*    GetMissileType(void) const
        {
            return m_missileType;
        }
        virtual short               GetLaunchCount (void) const
        {
            return m_partType->GetLaunchCount ();
        }
        virtual float               GetLock(void) const
        {
            return m_lock;
        }
        virtual void                SetLock(float l)
        {
            m_lock = l;
        }

    private:
        ImissionIGC*                    m_pMission;
        IlauncherTypeIGC*               m_partType;
        ImissileTypeIGC*                m_missileType;
        IshipIGC*                       m_ship;

        Vector                          m_emissionPt;

        float                           m_mountedFraction;
        float                           m_lock;
        Time                            m_timeLoaded;

        short                           m_amount;

        Mount                           m_mountID;
};

#endif //__MAGAZINEIGC_H_
