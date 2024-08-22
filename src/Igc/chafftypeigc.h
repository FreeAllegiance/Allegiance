/*
**  Copyright (C) 1996, 1997 Microsoft Corporation. All Rights Reserved.
**
**  File:    chaffTypeIGC.h
**
**  Author: 
**
**  Description:
**      Header for the CchaffTypeIGC class. This file was initially created by
**  the ATL wizard.
**
**  History:
*/
// chaffTypeIGC.h : Declaration of the CchaffTypeIGC

#pragma once

#include "igc.h"

/////////////////////////////////////////////////////////////////////////////
// CchaffTypeIGC
class CchaffTypeIGC : public IchaffTypeIGC
{
    public:
        CchaffTypeIGC(void)
        {
        }

        ~CchaffTypeIGC(void)
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
            return OT_chaffType;
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
            return ET_ChaffLauncher;
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

        // IchaffTypeIGC
        virtual float               GetChaffStrength(void) const
        {
            return m_data.chaffStrength;
        }

    private:
        ImissionIGC*        m_pMission;
        DataChaffTypeIGC    m_data;
};