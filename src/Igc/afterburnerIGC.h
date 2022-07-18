/*
**  Copyright (C) 1996, 1997 Microsoft Corporation. All Rights Reserved.
**
**  File:	afterburnerIGC.h
**
**  Author: 
**
**  Description:
**      Header for the CafterburnerIGC class. This file was initially created by
**  the ATL wizard.
**
**  History:
*/

// afterburnerIGC.h : Declaration of the CafterburnerIGC

#ifndef __AFTERBURNERIGC_H_
#define __AFTERBURNERIGC_H_

#include "igc.h"

/////////////////////////////////////////////////////////////////////////////
// CafterburnerIGC
class CafterburnerIGC : public IafterburnerIGC
{
    public:
        CafterburnerIGC(void);

// IbaseIGC
        virtual HRESULT Initialize(ImissionIGC* pMission, Time now, const void* data, int dataSize);
        virtual void    Terminate(void);
        virtual void    Update(Time now);

        virtual ObjectType  GetObjectType(void) const
        {
            return OT_afterburner;
        }

        virtual ImissionIGC*    GetMission(void) const
        {
            return m_pMission;
        }

// IpartIGC
        virtual EquipmentType    GetEquipmentType(void) const
        {
            return ET_Afterburner;
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
            return m_fActive;
        }

        virtual void             Activate(void)
        {
            ZAssert (m_ship);

            if (!m_fActive)
            {
                m_ship->ChangeSignature(m_typeData->signature);
                m_fActive = true;
                m_power = 0.0f;
            }
        }
        virtual void             Deactivate(void)
        {
            ZAssert (m_ship);

            if (m_fActive)
            {
                m_ship->ChangeSignature(-m_typeData->signature);
                m_fActive = false;
                m_power = 0.0f;
            }
        }
        virtual void             Reset()
        {
        }

        virtual float            GetMass(void) const
        {
            return m_typeData->mass;
        }

        virtual Money            GetPrice(void) const
        {
            return m_typeData->price;
        }

    // IafterburnerIGC
        virtual float    GetFuelConsumption(void) const
        {
            return m_typeData->fuelConsumption;
        }
		virtual float	 GetMaxThrustWithGA(void) const //TheRock 15-8-2009
		{
			return m_typeData->maxThrust * GetShip()->GetSide()->GetGlobalAttributeSet().GetAttribute(c_gaThrust);
		}
        virtual float    GetMaxThrust(void) const
        {
            return m_typeData->maxThrust;
        }
        virtual float    GetOnRate(void) const
        {
            return m_typeData->onRate;
        }
        virtual float    GetOffRate(void) const
        {
            return m_typeData->offRate;
        }

        virtual float    GetPower(void) const
        {
            return m_power;
        }

        virtual void     SetPower(float p)
        {
            ZAssert (p >= 0.0f);
            ZAssert (p <= 1.0f);
            if (p != 0.0f)
                Activate();

            m_power = p;
        }

        virtual void     IncrementalUpdate(Time lastUpdate, Time now, bool bUseFuel);

        virtual SoundID  GetInteriorSound(void) const
        {
            return m_typeData->interiorSound;
        }
        virtual SoundID  GetExteriorSound(void) const
        {
            return m_typeData->exteriorSound;
        }

        virtual float    GetMountedFraction(void) const
        {
            return m_mountedFraction;
        }
        virtual void     SetMountedFraction(float f)
        {
            m_mountedFraction = f;
            if (f != 1.0f)
                Deactivate();
        }

        virtual void    Arm(void)
        {
            m_mountedFraction = 1.0f;
        }

    private:
        ImissionIGC*                    m_pMission;
        const DataAfterburnerTypeIGC*   m_typeData;
        IpartTypeIGC*                   m_partType;
        IshipIGC*                       m_ship;
        float                           m_power;
        float                           m_mountedFraction;

        Mount                           m_mountID;
        bool                            m_fActive;
};

#endif //__AFTERBURNERIGC_H_
