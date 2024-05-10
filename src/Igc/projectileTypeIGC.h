/*
**  Copyright (C) 1996, 1997 Microsoft Corporation. All Rights Reserved.
**
**  File:    projectileTypeIGC.h
**
**  Author: 
**
**  Description:
**      Header for the CprojectileTypeIGC class. This file was initially created by
**  the ATL wizard.
**
**  History:
*/
// projectileTypeIGC.h : Declaration of the CprojectileTypeIGC

#pragma once

#include "igc.h"

/////////////////////////////////////////////////////////////////////////////
// CprojectileTypeIGC
class CprojectileTypeIGC : public IprojectileTypeIGC
{
    public:
        CprojectileTypeIGC(void)
        {
        }

    // IbaseIGC
        virtual HRESULT         Initialize(ImissionIGC* pMission, Time now, const void* data, int dataSize);
        virtual void            Terminate(void)
        {
            m_pMission->DeleteProjectileType(this);
        }

        virtual int             Export(void* data) const;

        virtual ObjectType      GetObjectType(void) const
        {
            return OT_projectileType;
        }

        virtual ObjectID        GetObjectID(void) const
        {
            return m_data.projectileTypeID;
        }
    // ItypeIGC
        virtual const void*     GetData(void) const
        {
            return &m_data;
        }

    // IprojectileTypeIGC
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
        virtual float           GetSpeed(void) const
        {
            return m_data.speed;
        }
        virtual bool            GetAbsoluteF(void) const
        {
            return m_data.absoluteF;
        }
        virtual const char*     GetModelName(void) const
        {
            return m_data.modelName;
        }
        virtual const char*     GetModelTexture(void) const
        {
            return m_data.textureName;
        }
        virtual float           GetLifespan(void) const
        {
            return m_data.lifespan;
        }
        virtual float           GetRadius(void) const
        {
            return m_data.radius;
        }
        virtual float           GetRotation(void) const
        {
            return m_data.rotation;
        }
        virtual COLORVALUE   GetColor(void) const // was D3DCOLORVALUE
        {
            return m_data.color;
        }
        virtual DamageTypeID    GetDamageType(void) const
        {
            return m_data.damageType;
        }

        virtual SoundID          GetAmbientSound(void) const
        {
            return m_data.ambientSound;
        }
    private:
        ImissionIGC*            m_pMission;
        DataProjectileTypeIGC   m_data;
};