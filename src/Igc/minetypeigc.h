/*
**  Copyright (C) 1996, 1997 Microsoft Corporation. All Rights Reserved.
**
**  File:    mineTypeIGC.h
**
**  Author: 
**
**  Description:
**      Header for the CmineTypeIGC class. This file was initially created by
**  the ATL wizard.
**
**  History:
*/
// mineTypeIGC.h : Declaration of the CmineTypeIGC

#pragma once

#include "igc.h"

/////////////////////////////////////////////////////////////////////////////
// CmineTypeIGC
class CmineTypeIGC : public ImineTypeIGC
{
    public:
        CmineTypeIGC(void)
        {
        }

    // IbaseIGC
        virtual HRESULT         Initialize(ImissionIGC* pMission, Time now, const void* data, int dataSize);
        virtual void            Terminate(void)
        {
            m_pMission->DeleteExpendableType(this);
        }
        virtual void            Update(Time now)
        {
        }

        virtual int             Export(void* data) const;

        virtual ObjectType  GetObjectType(void) const
        {
            return OT_mineType;
        }

        virtual ObjectID        GetObjectID(void) const
        {
            return m_data.expendabletypeID;
        }

    // ItypeIGC
        virtual const void*     GetData(void) const
        {
            return &m_data;
        }

    // IexpendableTypeIGC
        virtual const LauncherDef*  GetLauncherDef(void) const
        {
            return &(m_data.launcherDef);
        }
        virtual EquipmentType       GetEquipmentType(void) const
        {
            return ET_Dispenser;
        }
        virtual float           GetLoadTime(void) const
        {
            return m_data.loadTime;
        }
        virtual float               GetMass(void) const
        {
            return m_data.launcherDef.mass;
        }
        virtual float           GetLifespan(void) const
        {
            return m_data.lifespan;
        }

        virtual float           GetSignature(void) const
        {
            return m_data.signature;
        }
        virtual HitPoints       GetHitPoints(void) const
        {
            return m_data.hitPoints;
        }
        virtual DefenseTypeID   GetDefenseType(void) const
        {
            return m_data.defenseType;
        }
        virtual char const*     GetName(void) const
        {
            return m_data.launcherDef.name;
        }

        virtual char const*     GetModelName(void) const
        {
            return m_data.modelName;
        }
        virtual char const*     GetModelTexture(void) const
        {
            return m_data.textureName;
        }
        virtual float           GetRadius(void) const
        {
            return m_data.radius;
        }

        virtual ExpendableAbilityBitMask    GetCapabilities(void) const
        {
            return m_data.eabmCapabilities;
        }

        virtual bool                        HasCapability(ExpendableAbilityBitMask eabm) const
        {
            return (m_data.eabmCapabilities & eabm) != 0;
        }

        // ImineTypeIGC
        virtual float               GetPower(void) const
        {
            return m_data.power;
        }
        virtual float               GetEndurance(void) const
        {
            return m_data.endurance;
        }
        virtual DamageTypeID        GetDamageType(void) const
        {
            return m_data.damageType;
        }

    private:
        ImissionIGC*        m_pMission;
        DataMineTypeIGC     m_data;
};