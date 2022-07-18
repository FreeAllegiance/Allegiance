/*
**  Copyright (C) 1996, 1997 Microsoft Corporation. All Rights Reserved.
**
**  File:	packIGC.h
**
**  Author: 
**
**  Description:
**      Header for the CpackIGC class. This file was initially created by
**  the ATL wizard.
**
**  History:
*/
// packIGC.h : Declaration of the CpackIGC

#ifndef __PACKIGC_H_
#define __PACKIGC_H_
#include "igc.h"

/////////////////////////////////////////////////////////////////////////////
// CpackIGC
class CpackIGC : public IpackIGC
{
    public:
        CpackIGC(void);
        ~CpackIGC(void);

// IbaseIGC
        virtual HRESULT Initialize(ImissionIGC* pMission, Time now, const void* data, int dataSize);
        virtual void    Terminate(void);

        virtual ObjectType  GetObjectType(void) const
        {
            return OT_pack;
        }

        virtual ImissionIGC*    GetMission(void) const
        {
            return m_pMission;
        }

// IpartIGC
        virtual EquipmentType    GetEquipmentType(void) const
        {
            return ET_Pack;
        }

        virtual IpartTypeIGC*    GetPartType(void) const
        {
            return m_partType;
        }

        virtual IshipIGC*        GetShip(void) const
        {
            return m_pship;
        }
        virtual void             SetShip(IshipIGC* newVal, Mount mount);

        virtual Mount            GetMountID(void) const
        {
            return m_mountID;
        }
        virtual void             SetMountID(Mount newVal);

        virtual bool             fActive(void) const
        {
            return false;
        }
        virtual void             Activate(void)
        {
        }
        virtual void             Deactivate(void)
        {
        }
        virtual float            GetMass(void) const
        {
            return 0.0f;
        }

        virtual Money           GetPrice(void) const
        {
            return 0;
        }

        virtual float           GetMountedFraction(void) const
        {
            return 1.0f;
        }

        virtual void           SetMountedFraction(float f)
        {
        }
        virtual void           Arm(void)
        {
        }

    // IpackIGC
        virtual PackType    GetPackType(void) const
        {
            return m_typeData->packType;
        }
        virtual void        SetAmount(short a)
        {
            if (m_pship)
            {
                const IhullTypeIGC*   pht = m_pship->GetHullType();

                short   maxAmount;
                if (m_typeData->packType == c_packAmmo)
                {
                    maxAmount = pht->GetMaxAmmo();
                }
                else
                {
                    ZAssert (m_typeData->packType == c_packFuel);
                    maxAmount = short(pht->GetMaxFuel());
                }

                if (a > maxAmount)
                    a = maxAmount;
            }

            m_amount = a;
        }
        virtual short       GetAmount(void) const
        {
            return m_amount;
        }

    private:
        ImissionIGC*                    m_pMission;
        const DataPackTypeIGC*          m_typeData;
        IpartTypeIGC*                   m_partType;
        IshipIGC*                       m_pship;
        short                           m_amount;
        Mount                           m_mountID;
};

#endif //__PACKIGC_H_
