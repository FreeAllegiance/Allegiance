/*
**  Copyright (C) 1996, 1997 Microsoft Corporation. All Rights Reserved.
**
**  File:	shieldIGC.h
**
**  Author: 
**
**  Description:
**      Header for the CshieldIGC class. This file was initially created by
**  the ATL wizard.
**
**  History:
*/
// shieldIGC.h : Declaration of the CshieldIGC

#pragma once

#include "igc.h"

/////////////////////////////////////////////////////////////////////////////
// CshieldIGC
class CshieldIGC : public IshieldIGC
{
    public:
        CshieldIGC(void);

// IbaseIGC
        virtual HRESULT Initialize(ImissionIGC* pMission, Time now, const void* data, int dataSize);
        virtual void    Terminate(void);
        virtual void    Update(Time now);

        virtual ObjectType  GetObjectType(void) const
        {
            return OT_shield;
        }

        virtual ImissionIGC*    GetMission(void) const
        {
            return m_pMission;
        }

// IpartIGC
        virtual EquipmentType    GetEquipmentType(void) const
        {
            return ET_Shield;
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
            m_ship->ChangeSignature(m_typeData->signature);
        }
        virtual void             Deactivate(void)
        {
            m_ship->ChangeSignature(-m_typeData->signature);
            m_ship->GetHitTest()->SetShape(c_htsConvexHullMax);
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
            m_fraction = 1.0f;
        }

    // IshieldIGC
        virtual float           GetRegeneration(void) const
        {
            return m_ship 
                   ? (m_typeData->rateRegen * m_ship->GetSide()->GetGlobalAttributeSet().GetAttribute(c_gaShieldRegenerationShip))
                   : m_typeData->rateRegen;
        }
	    virtual float           GetMaxStrength(void) const
        {
            return m_ship 
                   ? (m_typeData->maxStrength * m_ship->GetSide()->GetGlobalAttributeSet().GetAttribute(c_gaMaxShieldShip))
                   : m_typeData->maxStrength;
        }
        virtual DefenseTypeID   GetDefenseType(void) const
        {
            return m_typeData->defenseType;
        }
	    virtual float           GetFraction(void) const
        {
            return m_fraction;
        }
	    virtual void           SetFraction(float newVal)
        {
            float down = m_pMission->GetFloatConstant(c_fcidDownedShield);
            if (newVal <= down)
            {
                if (newVal < 0.0f)
                    newVal = 0.0f;

                if (m_fraction > down)
                    m_ship->GetHitTest()->SetShape(c_htsConvexHullMax);
            }
            else
            {
                if (newVal > 1.0f)
                    newVal = 1.0f;

                if (m_fraction <= down)
                    m_ship->GetHitTest()->SetShape(c_htsEllipse);
            }

            m_fraction = newVal;
        }

	    virtual float           ApplyDamage(Time           timeCollision,
                                            DamageTypeID   type,
                                            float          amount,
                                            const Vector&  deltaP);

    private:
        const DataShieldTypeIGC*    m_typeData;
        IpartTypeIGC*               m_partType;
        IshipIGC*                   m_ship;
        ImissionIGC*                m_pMission;

        float                       m_fraction;
        float                       m_mountedFraction;

        Mount                       m_mountID;
};