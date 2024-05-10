/*
**  Copyright (C) 1996, 1997 Microsoft Corporation. All Rights Reserved.
**
**  File:	mineIGC.h
**
**  Author: 
**
**  Description:
**      Header for the CmineIGC class. This file was initially created by
**  the ATL wizard.
**
**  History:
*/
// mineIGC.h : Declaration of the CmineIGC

#pragma once

#include "igc.h"
#include "modelIGC.h"

const char  c_msStart = 0;
const char  c_msExploded = 1;
const char  c_msScattered = 2;

class CmineIGC : public TmodelIGC<ImineIGC>
{
    public:
        CmineIGC(void);
        ~CmineIGC(void);

    public:
    // IbaseIGC
	    virtual HRESULT Initialize(ImissionIGC* pMission, Time now, const void* data, int dataSize);
	    virtual void    Terminate(void);
        virtual void    Update(Time now);
        virtual int     Export(void*    data) const;

        virtual ObjectType  GetObjectType(void) const
        {
            return OT_mine;
        }

        virtual ObjectID    GetObjectID(void) const
        {
            return m_mineID;
        }

    // ImodelIGC
        virtual void    SetCluster(IclusterIGC* cluster)
        {
            AddRef();

            {
                IclusterIGC*    c = GetCluster();
                if (c)
                    c->DeleteMine(this);
            }

            TmodelIGC<ImineIGC>::SetCluster(cluster);

            if (cluster)
                cluster->AddMine(this);

            Release();
        }

        virtual void                 HandleCollision(Time                   timeCollision,
                                                     float                  tCollision,
                                                     const CollisionEntry&  entry,
                                                     ImodelIGC*             pmodel)
        {
            if (pmodel->GetObjectType() == OT_ship)
            {
                const float c_dtArmTime = 1.0f;

				if (((pmodel->GetSide() != GetSide()) && !IsideIGC::AlliedSides(pmodel->GetSide(),GetSide())) && (timeCollision >= m_time0 + c_dtArmTime)) // #ALLY - was : (pmodel->GetSide() != GetSide())  Imago fixed!
                {
                    //Find the length of the ship's path within the minefield
                    const Vector&   dV = pmodel->GetVelocity();
                    double  a = dV * dV;
                    if (a > 1.0)        //ya got to be moving at least a little to get nailed
                    {
                        const Vector&   pMe = GetPosition();

                        const Vector&   pHim = pmodel->GetPosition();

                        Vector  pEnter = pHim + tCollision * dV;

                        //What time do we leave the minefield?
                        //R^2(t) = |pHim + t * vHim - pMe|^2 = (myRadius + hisRadius)^2

                        Vector  dP = pMe - pHim;    //Note: sense is reversed wrt dV to eliminate a - sign below

                        float    rMe = GetRadius();
                        float    rHim = pmodel->GetRadius();
                        double   r = rMe + rHim;

                        double  halfB = dP * dV;

                        double  c = dP * dP - r * r;
                        double  b2ac = halfB * halfB - a * c;

                        if (b2ac >= 0.0f)
                        {
                            double  s = sqrt(b2ac);

                            float   tExit = (float)((halfB + s) / a);
                            float   tStop = GetHitTest()->GetTstop();
                            if (tExit > tStop)
                                tExit = tStop;

                            float   dt = tExit - tCollision;
                            if (dt > 0.0f)
                            {
                                const float c_damageMultiplier = 1.0f / (5.0f * 5.0f * 100.0f);
                                float   speed2 = float(a);
                                float   amount = dt * speed2 * m_mineType->GetPower() * rHim * rHim * c_damageMultiplier / (2.0f * rMe);

                                float   endurance = m_mineType->GetEndurance();

                                float   oldEndurance = endurance * m_fraction;
                                float   newEndurance = oldEndurance * float(exp(-amount / endurance));

                                float   damage = oldEndurance - newEndurance;

                                m_fraction = newEndurance / endurance;

                                Vector position1 = pHim + dV * tCollision;

                                const float c_radiusPlacement = 10.0f;
                                Vector position2 = position1 + dV * ((c_radiusPlacement + rHim) / float(sqrt(a))) + Vector::RandomPosition(c_radiusPlacement);

                                ((IshipIGC*)pmodel)->AddMineDamage(m_mineType->GetDamageType(), damage, timeCollision, 
                                                                   m_launcher,
                                                                   position1,
                                                                   position2);

                                if (newEndurance < 1.0f)
                                    GetMyMission()->GetIgcSite()->KillMineEvent(this);
                                else
                                    GetThingSite()->SetMineStrength(m_fraction);
                            }
                        }
                    }
                }
            }
        }

    // ImineIGC
        virtual ImineTypeIGC*   GetMineType(void) const
        {
            return m_mineType;
        }
        virtual IshipIGC*       GetLauncher(void) const
        {
            return m_launcher;
        }

        virtual void            SetCreateNow (void)
        {
            m_bCreateNow = true;
        }

        virtual float            GetStrength(void) const
        {
            return m_fraction;
        }

        virtual void            ReduceStrength(float    amount)
        {
            float   endurance = m_mineType->GetEndurance();
            float   newEndurance = endurance * m_fraction * float(exp(-amount / endurance));

            m_fraction = (newEndurance / endurance);

            if (m_fraction < 0.25f)
                GetMyMission()->GetIgcSite()->KillMineEvent(this);
            else
                GetThingSite()->SetMineStrength(m_fraction);
        }

        virtual float           GetTimeFraction(void) const
        {
            Time    timeNow = GetMyLastUpdate();
            float   f = (m_timeExpire - timeNow) / (m_timeExpire - m_time0);
            if (f < 0.0f)
                f = 0.0f;
            else if (f > 1.0f)
                f = 1.0f;

            return f;
        }

    private:
        ImineTypeIGC*       m_mineType;
        IshipIGC*           m_launcher;
        Time                m_time0;
        Time                m_timeExpire;

        float               m_fraction;

        MineID              m_mineID;
        bool                m_bCreateNow;
};