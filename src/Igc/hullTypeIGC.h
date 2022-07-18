/*
**  Copyright (C) 1996, 1997 Microsoft Corporation. All Rights Reserved.
**
**  File:    hullTypeIGC.h
**
**  Author: 
**
**  Description:
**      Header for the ChullTypeIGC class. This file was initially created by
**  the ATL wizard.
**
**  History:
*/
// hullTypeIGC.h : Declaration of the ChullTypeIGC

#ifndef __HULLTYPEIGC_H_
#define __HULLTYPEIGC_H_
#include "igc.h"

/////////////////////////////////////////////////////////////////////////////
// ChullTypeIGC
class       ChullTypeIGC : public IhullTypeIGC
{
    public:
        ChullTypeIGC(void)
        :
            m_data(NULL)
        {
        }

        ~ChullTypeIGC(void)
        {
            delete [] (char*)m_data;
        }

    public:
    // IbaseIGC
        virtual HRESULT Initialize(ImissionIGC* pMission, Time now, const void* data, int dataSize);
        virtual void    Terminate(void);

        virtual int     Export(void* data) const;

        virtual ObjectType          GetObjectType(void) const
        {
            return OT_hullType;
        }

        virtual ObjectID            GetObjectID(void) const
        {
            return m_data->hullID;
        }

    // ItypeIGC
        virtual const void*         GetData(void) const
        {
            return m_data;
        }

    // IbuyableIGC
        virtual const char*          GetModelName(void) const
        {
            return m_data->modelName;
        }

        virtual const char*          GetName(void) const
        {
            return m_data->name;
        }
        virtual const char*          GetDescription(void) const
        {
            return m_data->description;
        }
        virtual Money                GetPrice(void) const
        {
            return m_data->price;
        }

        virtual const TechTreeBitMask&  GetRequiredTechs(void) const
        {
            return m_data->ttbmRequired;
        }

        virtual const TechTreeBitMask&  GetEffectTechs(void) const
        {
            return m_data->ttbmEffects;
        }

        virtual DWORD                   GetTimeToBuild(void) const
        {
            return m_data->timeToBuild;
        }

        virtual BuyableGroupID          GetGroupID(void) const
        {
            return m_data->groupID;
        }

       // IhullTypeIGC
        virtual float                GetMass(void) const
        {
            return m_data->mass;
        }

        virtual float               GetSignature(void) const
        {
            return m_data->signature;
        }

        virtual float                GetLength(void) const
        {
            return m_data->length;
        }
        virtual float                GetMaxSpeed(void) const
        {
            return m_data->speed;
        }
        virtual float                GetMaxTurnRate(Axis    axis) const
        {
            ZAssert (axis >= 0);
            ZAssert (axis <= 3);
            return m_data->maxTurnRates[axis];
        }
        virtual float                GetTurnTorque(Axis    axis) const
        {
            ZAssert (axis >= 0);
            ZAssert (axis <= 3);
            return m_data->turnTorques[axis];
        }
        virtual float                GetThrust(void) const
        {
            return m_data->thrust;
        }
        virtual float                GetSideMultiplier(void) const
        {
            return m_data->sideMultiplier;
        }
        virtual float                GetBackMultiplier(void) const
        {
            return m_data->backMultiplier;
        }
        virtual float                GetScannerRange(void) const
        {
            return m_data->scannerRange;
        }

        virtual float                GetMaxEnergy(void) const
        {
            return m_data->maxEnergy;
        }
        virtual float                GetRechargeRate(void) const
        {
            return m_data->rechargeRate;
        }

        virtual HitPoints            GetHitPoints(void) const
        {
            return m_data->hitPoints;
        }
        virtual DefenseTypeID       GetDefenseType(void) const
        {
            return m_data->defenseType;
        }
        virtual PartMask             GetPartMask(EquipmentType et, Mount  mountID) const
        {
            ZAssert (mountID >= 0);
            if (et == ET_Weapon)
            {
                return (mountID < m_data->maxWeapons)
                       ? (((HardpointData*)(((char*)m_data) + m_data->hardpointOffset))[mountID]).partMask
                       : 0;
            }
            else
            {
                return (mountID == 0) ? m_data->pmEquipment[et] : 0;
            }
        }

        virtual short                GetCapacity(EquipmentType et) const
        {
            short c;
            switch (et)
            {
                case ET_Magazine:
                    c = m_data->capacityMagazine;
                break;
                case ET_Dispenser:
                    c = m_data->capacityDispenser;
                break;
                case ET_ChaffLauncher:
                    c = m_data->capacityChaffLauncher;
                break;
                default:
                    c = 0;
            }

            return c;
        }

        virtual bool                 CanMount(IpartTypeIGC* ppt, Mount  mountID) const
        {
            ZAssert (ppt);
            bool    bMountable;

            if (mountID < 0)
                bMountable = (mountID >= -c_maxCargo);
            else
            {
                EquipmentType   et = ppt->GetEquipmentType();
                bMountable = (ppt->GetPartMask() & ~GetPartMask(et, mountID)) == 0;
            }

            return bMountable;
        }            
        virtual Mount                GetMaxWeapons(void) const
        {
            return m_data->maxWeapons;
        }
        virtual Mount                GetMaxFixedWeapons(void) const
        {
            return m_data->maxFixedWeapons;
        }
        virtual const HardpointData& GetHardpointData(Mount hardpointID) const
        {
            ZAssert ((hardpointID >= 0) && (hardpointID < m_data->maxWeapons));
            return ((HardpointData*)(((char*)m_data) + m_data->hardpointOffset))[hardpointID];
        }

        virtual const char*          GetTextureName(void) const
        {
            return m_data->textureName;
        }

        virtual const char*          GetIconName(void) const
        {
            return m_data->iconName;
        }

        virtual HullAbilityBitMask   GetCapabilities(void) const
        {
            return m_data->habmCapabilities;
        }

        virtual bool                 HasCapability(HullAbilityBitMask habm) const
        {
            return (m_data->habmCapabilities & habm) != 0;
        }

        virtual const Vector&        GetCockpit(void) const
        {
            return m_cockpit;
        }

		// TurkeyXIII 11/09 #94
		virtual const Vector&		 GetChaffPosition(void) const
		{
			return m_chaff;
		}

        virtual const Vector&        GetWeaponPosition(Mount mount) const
        {
            ZAssert (mount >= 0);
            ZAssert (mount < m_data->maxWeapons);

            return m_positionWeapons[mount];
        }
        virtual const Orientation&   GetWeaponOrientation(Mount mount) const
        {
            ZAssert (mount >= 0);
            ZAssert (mount < m_data->maxWeapons);

            return m_orientationWeapons[mount];
        }

        virtual float   GetMaxFuel(void) const
        {
            return m_data->maxFuel;
        }

        virtual float   GetECM(void) const
        {
            return m_data->ecm;
        }

        virtual float   GetRipcordSpeed(void) const
        {
            return m_data->ripcordSpeed;
        }

        virtual float   GetRipcordCost(void) const
        {
            return m_data->ripcordCost;
        }

        virtual short   GetMaxAmmo(void) const
        {
            return m_data->maxAmmo;
        }

        virtual IhullTypeIGC*   GetSuccessorHullType(void) const
        {
            return m_phtSuccessor;
        }

        /*
        virtual const char*          GetPilotHUDName(void) const
        {
            return m_data->pilotHUDName;
        }
        virtual const char*          GetObserverHUDName(void) const
        {
            return m_data->observerHUDName;
        }
        */
    
        virtual SoundID              GetInteriorSound(void) const
        {
            return m_data->interiorSound;
        }
        virtual SoundID              GetExteriorSound(void) const
        {
            return m_data->exteriorSound;
        }

        virtual SoundID              GetMainThrusterInteriorSound(void) const
        {
            return m_data->mainThrusterInteriorSound;
        }
        virtual SoundID              GetMainThrusterExteriorSound(void) const
        {
            return m_data->mainThrusterExteriorSound;
        }

        virtual SoundID              GetManuveringThrusterInteriorSound(void) const
        {
            return m_data->manuveringThrusterInteriorSound;
        }
        virtual SoundID              GetManuveringThrusterExteriorSound(void) const
        {
            return m_data->manuveringThrusterExteriorSound;
        }
        virtual const PartTypeListIGC*      GetPreferredPartTypes(void) const
        {
            return &m_preferredPartTypes;
        }
        virtual float GetScale() const
        {
            return m_scale;
        }
        virtual IObject*            GetIcon(void) const
        {
            return m_poIcon;
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

    private:
        ImissionIGC*        m_pMission;
        TRef<IhullTypeIGC>  m_phtSuccessor;
        TRef<IObject>       m_poIcon;
        DataHullTypeIGC*    m_data;
        PartTypeListIGC     m_preferredPartTypes;
        Vector              m_cockpit;
		Vector				m_chaff;	 // TurkeyXIII 11/09 #94
        Vector              m_positionWeapons[c_maxMountedWeapons];
        Orientation         m_orientationWeapons[c_maxMountedWeapons];
        float               m_scale;

        int                 m_nLaunchSlots;
        Vector              m_positionLaunches[c_maxShipLaunchSlots];
        Vector              m_directionLaunches[c_maxShipLaunchSlots];

        int                 m_nLandSlots;
        int                 m_nCapLandSlots;
        int                 m_nLandPlanes[c_maxLandSlots];
        Vector              m_positionLandPlanes[c_maxShipLandSlots][c_maxLandPlanes];
        Vector              m_directionLandPlanes[c_maxShipLandSlots][c_maxLandPlanes];
};

#endif //__HULLTYPEIGC_H_
