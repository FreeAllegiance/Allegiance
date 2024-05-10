/*
**  Copyright (C) 1996, 1997 Microsoft Corporation. All Rights Reserved.
**
**  File:    missileTypeIGC.h
**
**  Author: 
**
**  Description:
**      Header for the CmissileTypeIGC class. This file was initially created by
**  the ATL wizard.
**
**  History:
*/
// missileTypeIGC.h : Declaration of the CmissileTypeIGC

#pragma once

#include "igc.h"

/////////////////////////////////////////////////////////////////////////////
// CmissileTypeIGC
class CmissileTypeIGC : public ImissileTypeIGC
{
    public:
        CmissileTypeIGC(void)
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
            return OT_missileType;
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
            return ET_Magazine;
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

    // ImissileTypeIGC
        virtual float           GetAcceleration(void) const
        {
            return m_data.acceleration;
        }
        virtual float           GetTurnRate(void) const
        {
            return m_data.turnRate;
        }
        virtual float           GetInitialSpeed(void) const
        {
            return m_data.initialSpeed;
        }
        //virtual float           GetArmTime(void) const
        //{
            //return m_data.armTime;
        //}

        virtual float           GetMaxLock(void) const
        {
            return m_data.maxLock;
        }
        virtual float           GetChaffResistance(void) const
        {
            return m_data.chaffResistance;
        }
        virtual float           GetLockTime(void) const
        {
            return m_data.lockTime;
        }
        virtual float           GetReadyTime(void) const
        {
            return m_data.readyTime;
        }
        virtual float           GetDispersion(void) const
        {
            return m_data.dispersion;
        }
        virtual float           GetLockAngle(void) const
        {
            return m_data.lockAngle;
        }
        virtual float           GetPower(void) const
        {
            return m_data.power;
        }
        virtual float           GetBlastPower(void) const
        {
            return m_data.blastPower;
        }
        virtual float           GetBlastRadius(void) const
        {
            return m_data.blastRadius;
        }
        virtual DamageTypeID  GetDamageType(void) const
        {
            return m_data.damageType;
        }

        virtual SoundID         GetLaunchSound(void) const
        {
            return m_data.launchSound;
        }
        virtual SoundID         GetAmbientSound(void) const
        {
            return m_data.ambientSound;
        }

    private:
        ImissionIGC*        m_pMission;
        DataMissileTypeIGC  m_data;
};