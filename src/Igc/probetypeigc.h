/*
**  Copyright (C) 1996, 1997 Microsoft Corporation. All Rights Reserved.
**
**  File:    probeTypeIGC.h
**
**  Author: 
**
**  Description:
**      Header for the CprobeTypeIGC class. This file was initially created by
**  the ATL wizard.
**
**  History:
*/
// probeTypeIGC.h : Declaration of the CprobeTypeIGC

#ifndef __PROBETYPEIGC_H_
#define __PROBETYPEIGC_H_
#include "igc.h"

/////////////////////////////////////////////////////////////////////////////
// CprobeTypeIGC
class CprobeTypeIGC : public IprobeTypeIGC
{
    #ifdef _DEBUG_CprobeTypeIGC_AddRef_Release
        public:
            DWORD __stdcall  AddRef(void)
            {
                DWORD dwRefs = IprobeTypeIGC::AddRef();
                debugf("CprobeTypeIGC::AddRef(): m_count = %d\n", dwRefs);
                return dwRefs;
            }

            DWORD __stdcall  Release(void)
            {
                DWORD dwRefs = IprobeTypeIGC::Release();
                debugf("CprobeTypeIGC::Release(): m_count = %d\n", dwRefs);
                return dwRefs;
            }
    #endif // _DEBUG

    public:
        CprobeTypeIGC(void)
        :
            m_projectileType(NULL)
        {
        }

        ~CprobeTypeIGC(void)
        {
            ZAssert (m_projectileType == NULL);
        }

    // IbaseIGC
        virtual HRESULT         Initialize(ImissionIGC* pMission, Time now, const void* data, int dataSize);
        virtual void            Terminate(void)
        {
            if (m_projectileType)
            {
                m_projectileType->Release();
                m_projectileType = NULL;
            }

            m_pMission->DeleteExpendableType(this);
        }
        virtual void            Update(Time now)
        {
        }

        virtual int             Export(void* data) const;

        virtual ObjectType  GetObjectType(void) const
        {
            return OT_probeType;
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

        // IprobeTypeIGC
        virtual float           GetScannerRange(void) const
        {
            return m_data.scannerRange;
        }

        virtual float               GetDtBurst(void) const
        {
            return m_data.dtimeBurst;
        }

        virtual float               GetDispersion(void) const
        {
            return m_data.dispersion;
        }

        virtual float               GetAccuracy(void) const
        {
            return m_data.accuracy;
        }

        virtual short               GetAmmo(void) const
        {
            return m_data.ammo;
        }

        virtual IprojectileTypeIGC* GetProjectileType(void) const
        {
            return m_projectileType;
        }
        virtual const Vector&       GetEmissionPt(void) const
        {
            return m_emissionPt;
        }
        virtual float               GetRipcordDelay(void) const
        {
            return m_data.dtRipcord;
        }

        virtual SoundID             GetAmbientSound(void) const
        {
            return m_data.ambientSound;
        }

    private:
        ImissionIGC*        m_pMission;
        DataProbeTypeIGC    m_data;
        Vector              m_emissionPt;
        IprojectileTypeIGC* m_projectileType;
};

#endif //__PROBETYPEIGC_H_
