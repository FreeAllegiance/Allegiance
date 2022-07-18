/*
**  Copyright (C) 1996, 1997 Microsoft Corporation. All Rights Reserved.
**
**  File:    stationTypeIGC.h
**
**  Author: 
**
**  Description:
**      Header for the CstationTypeIGC class. This file was initially created by
**  the ATL wizard.
**
**  History:
*/
// stationTypeIGC.h : Declaration of the CstationTypeIGC

#ifndef __STATIONTYPEIGC_H_
#define __STATIONTYPEIGC_H_
#include "igc.h"

/////////////////////////////////////////////////////////////////////////////
// CstationTypeIGC
class       CstationTypeIGC : public IstationTypeIGC
{
    public:
        CstationTypeIGC(void)
        :
            m_pstSuccessor(NULL)
        {
        }

        ~CstationTypeIGC(void)
        {
        }

        /*
        DWORD __stdcall  AddRef(void)
        {
            return IstationTypeIGC::AddRef();
        }

        DWORD __stdcall  Release(void)
        {
            return IstationTypeIGC::Release();
        }
        */

    public:
    // IbaseIGC
        virtual HRESULT             Initialize(ImissionIGC* pMission, Time now, const void* data, int dataSize);
        virtual void                Terminate(void);

        virtual int                 Export(void* data) const;

        virtual ObjectType          GetObjectType(void) const
        {
            return OT_stationType;
        }

        virtual ObjectID            GetObjectID(void) const
        {
            return m_data.stationTypeID;
        }

    // ItypeIGC
        virtual const void*         GetData(void) const
        {
            return &m_data;
        }

    // IbuyableIGC
        virtual const char*          GetModelName(void) const
        {
            return m_data.modelName;
        }

        virtual const char*          GetName(void) const
        {
            return m_data.name;
        }
        virtual const char*          GetDescription(void) const
        {
            return m_data.description;
        }
        virtual Money                GetPrice(void) const
        {
            return m_data.price;
        }
        virtual BuyableGroupID       GetGroupID(void) const 
        { 
            return m_data.groupID; 
        };

        virtual const TechTreeBitMask&  GetRequiredTechs(void) const
        {
            return m_data.ttbmRequired;
        }

        virtual const TechTreeBitMask&  GetEffectTechs(void) const
        {
            return m_data.ttbmEffects;
        }

        virtual DWORD                   GetTimeToBuild(void) const
        {
            return m_data.timeToBuild;
        }

        // IstationTypeIGC
        virtual float               GetSignature(void) const
        {
            return m_data.signature;
        }

        virtual float                GetRadius(void) const
        {
            return m_data.radius;
        }

        virtual float                GetScannerRange(void) const
        {
            return m_data.scannerRange;
        }

        virtual HitPoints            GetMaxArmorHitPoints(void) const
        {
            return m_data.maxArmorHitPoints;
        }

        virtual DefenseTypeID        GetArmorDefenseType(void) const
        {
            return m_data.defenseTypeArmor;
        }

        virtual HitPoints                GetMaxShieldHitPoints(void) const
        {
            return m_data.maxShieldHitPoints;
        }

        virtual DefenseTypeID        GetShieldDefenseType(void) const
        {
            return m_data.defenseTypeShield;
        }

        virtual float                GetArmorRegeneration(void) const
        {
            return m_data.armorRegeneration;
        }

        virtual float                GetShieldRegeneration(void) const
        {
            return m_data.shieldRegeneration;
        }

        virtual Money                GetIncome(void) const
        {
            return m_data.income;
        }

        virtual const TechTreeBitMask&  GetLocalTechs(void) const
        {
            return m_data.ttbmLocal;
        }

        virtual StationAbilityBitMask   GetCapabilities(void) const
        {
            return m_data.sabmCapabilities;
        }
        bool                            HasCapability(StationAbilityBitMask sabm) const
        {
            return (m_data.sabmCapabilities & sabm) != 0;
        }
        virtual const char*             GetTextureName(void) const
        {
            return m_data.textureName;
        }
        virtual const char*             GetBuilderName(void) const
        {
            return m_data.builderName;
        }
        virtual const char*             GetIconName(void) const
        {
            return m_data.iconName;
        }

        virtual IstationTypeIGC*        GetSuccessorStationType(const IsideIGC*   pside)
        {
            //The successor can be built if:
            //    we have one, and
            //    the required bits are a subset of the side tech bits and the required bits for this station
            //    (on the logic that, if we have this station, we had the tech bits at one time)
            if (m_pstSuccessor != NULL)
            {
                if (m_pstSuccessor->GetRequiredTechs() <= (m_data.ttbmRequired |
                                                           pside->GetDevelopmentTechs()))
                    return m_pstSuccessor->GetSuccessorStationType(pside);
            }

            return this;
        }
		// EF5P
		// get the direct successor as stored in the core
		virtual IstationTypeIGC*        GetDirectSuccessorStationType()
		{
			if (m_pstSuccessor != NULL) return m_pstSuccessor;
			return this;
		}
        virtual AsteroidAbilityBitMask   GetBuildAABM(void) const
        {
            return m_data.aabmBuild;
        }

        virtual int                     GetLaunchSlots(void) const
        {
            return m_nLaunchSlots;
        }
        virtual const Vector&           GetLaunchPosition(int   slotID) const
        {
            ZAssert (slotID >= 0);
            ZAssert (slotID < m_nLaunchSlots);

            return m_positionLaunches[slotID];
        }
        virtual const Vector&           GetLaunchDirection(int   slotID) const
        {
            ZAssert (slotID >= 0);
            ZAssert (slotID < m_nLaunchSlots);

            return m_directionLaunches[slotID];
        }

        virtual int                     GetLandSlots(void) const
        {
            return m_nLandSlots;
        }
        virtual int                     GetCapLandSlots(void) const
        {
            return m_nCapLandSlots;
        }
        virtual int                     GetLandPlanes(int   slotID) const
        {
            ZAssert (slotID >= 0);
            ZAssert (slotID < m_nLandSlots);

            return m_nLandPlanes[slotID];
        }
        virtual const Vector&           GetLandPosition(int slotID, int planeID) const
        {
            ZAssert (slotID >= 0);
            ZAssert (slotID < m_nLandSlots);
            ZAssert (planeID >= 0);
            ZAssert (planeID <= m_nLandPlanes[slotID]);

            return m_positionLandPlanes[slotID][planeID];
        }
        virtual const Vector&           GetLandDirection(int slotID, int planeID) const
        {
            ZAssert (slotID >= 0);
            ZAssert (slotID < m_nLandSlots);
            ZAssert (planeID >= 0);
            ZAssert (planeID <= m_nLandPlanes[slotID]);

            return m_directionLandPlanes[slotID][planeID];
        }

        virtual SoundID                 GetInteriorSound() const
        {
            return m_data.interiorSound;
        }
        virtual SoundID                 GetInteriorAlertSound() const
        {
            return m_data.interiorAlertSound;
        }
        virtual SoundID                 GetExteriorSound() const
        {
            return m_data.exteriorSound;
        }

        virtual SoundID                 GetConstructorNeedRockSound() const
        {
            return m_data.constructorNeedRockSound;
        }

        virtual SoundID                 GetConstructorUnderAttackSound() const
        {
            return m_data.constructorUnderAttackSound;
        }

        virtual SoundID                 GetConstructorDestroyedSound() const
        {
            return m_data.constructorDestroyedSound;
        }

        virtual SoundID                 GetCompletionSound() const
        {
            return m_data.completionSound;
        }

        virtual SoundID                 GetUnderAttackSound() const
        {
            return m_data.underAttackSound;
        }

        virtual SoundID                 GetCriticalSound() const
        {
            return m_data.criticalSound;
        }

        virtual SoundID                 GetDestroyedSound() const
        {
            return m_data.destroyedSound;
        }

        virtual SoundID                 GetCapturedSound() const
        {
            return m_data.capturedSound;
        }

        virtual SoundID                 GetEnemyCapturedSound() const
        {
            return m_data.enemyCapturedSound;
        }

        virtual SoundID                 GetEnemyDestroyedSound() const
        {
            return m_data.enemyDestroyedSound;
        }

        virtual StationClassID          GetClassID() const
        {
            return m_data.classID;
        }

        virtual IdroneTypeIGC*              GetConstructionDroneType(void) const
        {
            return m_pdtConstruction;
        }

    private:
        ImissionIGC*        m_pMission;
        DataStationTypeIGC  m_data;

        IdroneTypeIGC*      m_pdtConstruction;

        IstationTypeIGC*    m_pstSuccessor;

        int                 m_nLaunchSlots;
        Vector              m_positionLaunches[c_maxLaunchSlots];
        Vector              m_directionLaunches[c_maxLaunchSlots];

        int                 m_nLandSlots;
        int                 m_nCapLandSlots;
        int                 m_nLandPlanes[c_maxLandSlots];
        Vector              m_positionLandPlanes[c_maxLandSlots][c_maxLandPlanes];
        Vector              m_directionLandPlanes[c_maxLandSlots][c_maxLandPlanes];
};

#endif //__STATIONTYPEIGC_H_
