/*
**  Copyright (C) 1996, 1997 Microsoft Corporation. All Rights Reserved.
**
**  File:	missileIGC.h
**
**  Author: 
**
**  Description:
**      Header for the CmissileIGC class. This file was initially created by
**  the ATL wizard.
**
**  History:
*/
// missileIGC.h : Declaration of the CmissileIGC

#ifndef __MISSILEIGC_H_
#define __MISSILEIGC_H_

#include "igc.h"
#include "modelIGC.h"

class CmissileIGC : public TmodelIGC<ImissileIGC>
{
    public:
        CmissileIGC(void);
        ~CmissileIGC(void);

    public:
    // IbaseIGC
	    virtual HRESULT Initialize(ImissionIGC* pMission, Time now, const void* data, int dataSize);
	    virtual void    Terminate(void);
        virtual void    Update(Time now);

        virtual ObjectType  GetObjectType(void) const
        {
            return OT_missile;
        }

        virtual ObjectID    GetObjectID(void) const
        {
            return m_missileID;
        }

    // ImodelIGC
        virtual void    SetCluster(IclusterIGC* cluster)
        {
            AddRef();

            {
                IclusterIGC*    c = GetCluster();
                if (c)
                    c->DeleteMissile(this);
            }

            TmodelIGC<ImissileIGC>::SetCluster(cluster);

            if (cluster)
                cluster->AddMissile(this);

            Release();
        }

        virtual void                 HandleCollision(Time       timeCollision,
                                                     float                  tCollision,
                                                     const CollisionEntry&  entry,
                                                     ImodelIGC* pModel);

        /*
        virtual void                SetSide(IsideIGC*   pside)  //override the default SetSide method
        {
            TmodelIGC<ImissileIGC>::SetSide(pside);

            if (pside)
                GetThingSite()->SetTrailColor(pside->GetColor());
        }
        */

    // IdamageIGC
       virtual DamageResult        ReceiveDamage(DamageTypeID            type,
                                                  float                   amount,
                                                  Time                    timeCollision,
                                                  const Vector&           position1,
                                                  const Vector&           position2,
                                                  ImodelIGC*              launcher)
        {
			IsideIGC*   pside = GetSide();

			if (launcher &&
				(!GetMyMission()->GetMissionParams()->bAllowFriendlyFire) &&
				((pside == launcher->GetSide()) || IsideIGC::AlliedSides(pside,launcher->GetSide())) && // TheRock 3/8/09 this check wasn't here
				(amount >= 0.0f))
			{
				return c_drNoDamage;
			}


            DamageResult dr = c_drHullDamage;
            amount *= GetMyMission()->GetDamageConstant(type, m_missileType->GetDefenseType());

            float   maxHP = m_missileType->GetHitPoints();
            
            //Student 8/3/2022 applying experience modifier (kb) to apply to missiles
            if (launcher && (launcher->GetObjectType() == OT_ship))
            {
                amount *= ((IshipIGC*)launcher)->GetExperienceMultiplier();
            }

            if (amount < 0.0f)
            {
                if (maxHP > 0.0f)
                {
                    m_fraction -= amount / maxHP;
                    if (m_fraction > 1.0f)
                        m_fraction = 1.0f;
                }
            }
            else if (amount != 0.0)
            {
                float   oldFraction = m_fraction;
                m_fraction = (maxHP > 0.0f) ? (m_fraction - amount / maxHP) : 0.0f;
                if (m_fraction <= 0.0f)
                {
                    m_fraction = 0.0f;
                    if (oldFraction > 0.0f)
                    {
                        GetMyMission()->GetIgcSite()->KillMissileEvent(this, position1);
                        dr = c_drKilled;
                    }
                }
            }

            return dr;
        }
        virtual float   GetFraction(void) const
        {
            return m_fraction;
        }
        virtual void    SetFraction(float newVal)
        {
            m_fraction = newVal;
        }

        virtual float               GetHitPoints(void) const
        {
            return m_fraction * m_missileType->GetHitPoints();
        }

    // ImissileIGC //Student NOTE, give possibility of reaquiring target?
        virtual ImissileTypeIGC*    GetMissileType(void) const
        {
            return m_missileType;
        }
        virtual IshipIGC*           GetLauncher(void) const
        {
            return m_launcher;
        }
        virtual ImodelIGC*          GetTarget(void) const
        {
            return m_target;
        }
        virtual void                SetTarget(ImodelIGC* newVal)
        {
            if (m_target)
                m_target->Release();

            m_target = newVal;

            if (newVal)
                newVal->AddRef();

            m_lock = 1.0f;

            //Assume m_tImpact does not change (not unreasonable given that the only reason to call SetTarget
            //is when the missile is spoofed and then the chaff is on top of the target.
        }

        virtual float               GetLock(void) const
        {
            return m_lock;
        }

        virtual void                Explode(const Vector& position);

        virtual void                Disarm(void)
        {
            m_bDisarmed = true;
            if (m_launcher->GetLastMissileFired() == this)
                m_launcher->SetLastMissileFired(NULL);
        }

    private:
        ImissileTypeIGC*    m_missileType;
        IshipIGC*           m_launcher;
        ImodelIGC*          m_target;
        Time                m_timeActivate;
        Time                m_timeExpire;
        float               m_tImpact;     //current estimated time of impact
        float               m_fraction;
        float               m_lock;
        MissileID           m_missileID;
        Vector              m_explodePosition;
        bool                m_bDisarmed;
};

#endif //__MISSILEIGC_H_
