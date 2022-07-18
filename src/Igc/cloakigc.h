/*
**  Copyright (C) 1996, 1997 Microsoft Corporation. All Rights Reserved.
**
**  File:	cloakIGC.h
**
**  Author: 
**
**  Description:
**      Header for the CcloakIGC class. This file was initially created by
**  the ATL wizard.
**
**  History:
*/
// cloakIGC.h : Declaration of the CcloakIGC

#ifndef __CLOAKIGC_H_
#define __CLOAKIGC_H_

#include "igc.h"

/////////////////////////////////////////////////////////////////////////////
// CcloakIGC
class CcloakIGC : public IcloakIGC
{
    public:
        CcloakIGC(void);

// IbaseIGC
        virtual HRESULT Initialize(ImissionIGC* pMission, Time now, const void* data, int dataSize);
        virtual void    Terminate(void);
        virtual void    Update(Time now);

        virtual ObjectType  GetObjectType(void) const
        {
            return OT_cloak;
        }

        virtual ImissionIGC*    GetMission(void) const
        {
            return m_pMission;
        }

// IpartIGC
        virtual EquipmentType    GetEquipmentType(void) const
        {
            return ET_Cloak;
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
            ZAssert (m_ship);
            if (!m_fActive)
            {
                m_ship->ChangeSignature(m_typeData->signature);
                m_fActive = true;

                m_cloaking = 0.0f;
				ZAssert (m_ship->GetCloaking() == 1.0f);
            }
        }
        virtual void             Deactivate(void)
        {
            ZAssert (m_ship);

            if (m_fActive)
            {
                m_ship->ChangeSignature(-m_typeData->signature);
                m_fActive = false;

                m_ship->SetCloaking(1.0f);  //1.0 - m_cloaking
            }
        }
        virtual float            GetMass(void) const
        {
            return m_typeData->mass;
        }

        virtual Money            GetPrice(void) const
        {
            return m_typeData->price;
        }

        virtual float    GetMountedFraction(void) const
        {
            return m_mountedFraction;
        }
        virtual void     SetMountedFraction(float f)
        {
            m_mountedFraction = f;
        }

        virtual void    Arm(void)
        {
            m_mountedFraction = 1.0f;
        }

    // IcloakIGC
        virtual float    GetEnergyConsumption(void) const
        {
            return m_typeData->energyConsumption;
        }
        virtual float    GetMaxCloaking(void) const
        {
            return m_typeData->maxCloaking; 
            /*
            return m_ship
                   ? (m_typeData->rateRegen * m_ship->GetSide()->GetGlobalAttributeSet().GetAttribute(c_gaRegenerationShield))
                   : m_typeData->rateRegen;
            */
        }
        virtual float    GetOnRate(void) const
        {
            return m_typeData->onRate; 
        }
        virtual float    GetOffRate(void) const
        {
            return m_typeData->offRate; 
        }

        virtual SoundID  GetEngageSound() const
        {
            return m_typeData->engageSound;
        }
        virtual SoundID  GetDisengageSound() const
        {
            return m_typeData->disengageSound;
        }

    private:
        const DataCloakTypeIGC*     m_typeData;
        IpartTypeIGC*               m_partType;
        IshipIGC*                   m_ship;
        ImissionIGC*                m_pMission;

        float                       m_cloaking;
        float                       m_mountedFraction;

        Mount                       m_mountID;
        bool                        m_fActive;
};

#endif //__CLOAKIGC_H_
