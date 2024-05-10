/*
**  Copyright (C) 1996, 1997 Microsoft Corporation. All Rights Reserved.
**
**  File:  launcherTypeIGC.h
**
**  Author: 
**
**  Description:
**      Header for the CpartTypeIGC class. This file was initially created by
**  the ATL wizard.
**
**  History:
*/
// launcherTypeIGC.h : Declaration of the CpartTypeIGC

#pragma once

#include "igc.h"

/////////////////////////////////////////////////////////////////////////////
// ClauncherTypeIGC

class   ClauncherTypeIGC : public IlauncherTypeIGC
{
    public:
        ClauncherTypeIGC(void)
        :
            m_pexpendabletype(NULL)
        {
        }

        ~ClauncherTypeIGC(void)
        {
        }

// IbaseIGC
        virtual HRESULT Initialize(ImissionIGC* pMission, Time now, const void* data, int dataSize);
        virtual void    Terminate(void);

        virtual int             Export(void* data) const;    

        virtual ObjectType      GetObjectType(void) const
        {
            return OT_partType;
        }

        virtual ObjectID        GetObjectID(void) const
        {
            return m_data.partID;
        }

 // ItypeIGC
        virtual const void*   GetData(void) const
        {
            return m_launcherDef;
        }

// IbuyableIGC
        virtual const char*          GetModelName(void) const
        {
            return m_launcherDef->modelName;
        }

        virtual const char*          GetIconName(void) const
        {
            return m_launcherDef->iconName;
        }

        virtual const char*          GetName(void) const
        {
            return m_launcherDef->name;
        }
        virtual const char*          GetDescription(void) const
        {
            return m_launcherDef->description;
        }
        virtual Money                GetPrice(void) const
        {
            return m_launcherDef->price;
        }

        virtual DWORD               GetTimeToBuild(void) const
        {
            return m_launcherDef->timeToBuild;
        }

        virtual BuyableGroupID      GetGroupID(void) const
        {
            return m_launcherDef->groupID;
        }

        // IpartTypeIGC
        virtual float                GetMass(void) const
        {
            return m_launcherDef->mass;
        }

        virtual float               GetSignature(void) const
        {
            return m_launcherDef->signature;
        }
        virtual EquipmentType       GetEquipmentType(void) const
        {
            return m_equipmenttype;
        }
        virtual PartMask            GetPartMask(void) const
        {
            return m_launcherDef->partMask;
        }
        virtual const TechTreeBitMask&  GetRequiredTechs(void) const
        {
            return m_launcherDef->ttbmRequired;
        }

        virtual const TechTreeBitMask&  GetEffectTechs(void) const
        {
            return m_launcherDef->ttbmEffects;
        }

        virtual IpartTypeIGC*           GetSuccessorPartType(void) const
        {
            return m_pptSuccessor;
        }

        const char*             GetInventoryLineMDLName(void) const
        {
            return m_data.inventoryLineMDL;
        }

        virtual short               GetAmount(IshipIGC* pship) const
        {
            short amount;
            if (pship)
            {
                amount = (pship->GetHullType()->GetCapacity(m_equipmenttype) / m_launcherDef->expendableSize);
                if (amount == 0)
                    amount = 1;
            }
            else
                amount = m_data.amount;

            return amount;
        }

// IlauncherTypeIGC
        virtual IexpendableTypeIGC* GetExpendableType(void) const
        {
            return m_pexpendabletype;
        }

        virtual short               GetLaunchCount (void) const
        {
            return m_data.launchCount;
        }

    private:
        ImissionIGC*                m_pMission;
        TRef<IpartTypeIGC>          m_pptSuccessor;
        IexpendableTypeIGC*         m_pexpendabletype;
        DataLauncherTypeIGC         m_data;
        const LauncherDef*          m_launcherDef;
        EquipmentType               m_equipmenttype;
};