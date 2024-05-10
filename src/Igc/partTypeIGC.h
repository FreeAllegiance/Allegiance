/*
**  Copyright (C) 1996, 1997 Microsoft Corporation. All Rights Reserved.
**
**  File:  partTypeIGC.h
**
**  Author: 
**
**  Description:
**      Header for the CpartTypeIGC class. This file was initially created by
**  the ATL wizard.
**
**  History:
*/
// partTypeIGC.h : Declaration of the CpartTypeIGC

#pragma once

#include "igc.h"

/////////////////////////////////////////////////////////////////////////////
// CpartTypeIGC

class   CpartTypeIGC : public IpartTypeIGC
{
    public:
        CpartTypeIGC(void)
        :
            m_data(NULL)
        {
        }

        ~CpartTypeIGC(void)
        {
            delete [] m_data;
        }

// IbaseIGC
        virtual HRESULT                 Initialize(ImissionIGC* pMission, Time now, const void* data, int dataSize);
        virtual void                    Terminate(void);

        virtual int                     Export(void* data) const;    

        virtual ObjectType              GetObjectType(void) const
        {
            return OT_partType;
        }

        virtual ObjectID                GetObjectID(void) const
        {
            return m_data->partID;
        }
// ItypeIGC
        virtual const void*             GetData(void) const
        {
            return m_data;
        }

// IbuyableIGC
        virtual const char*             GetModelName(void) const
        {
            return m_data->modelName;
        }

        virtual const char*          GetIconName(void) const
        {
            return m_data->iconName;
        }

        virtual const char*             GetName(void) const
        {
            return m_data->name;
        }
        virtual const char*             GetDescription(void) const
        {
            return m_data->description;
        }
        virtual Money                   GetPrice(void) const
        {
            return m_data->price;
        }

        virtual DWORD                   GetTimeToBuild(void) const
        {
            return m_data->timeToBuild;
        }

        virtual BuyableGroupID          GetGroupID(void) const
        {
            return m_data->groupID;
        }

        // IpartTypeIGC
        virtual float                   GetMass(void) const
        {
            return m_data->mass;
        }

        virtual float                   GetSignature(void) const
        {
            return m_data->signature;
        }

        virtual EquipmentType           GetEquipmentType(void) const
        {
            return m_data->equipmentType;
        }
        virtual PartMask                GetPartMask(void) const
        {
            return m_data->partMask;
        }

        virtual const TechTreeBitMask&  GetRequiredTechs(void) const
        {
            return m_data->ttbmRequired;
        }

        virtual const TechTreeBitMask&  GetEffectTechs(void) const
        {
            return m_data->ttbmEffects;
        }

        virtual IpartTypeIGC*           GetSuccessorPartType(void) const
        {
            return m_pptSuccessor;
        }

        const char*             GetInventoryLineMDLName(void) const
        {
            return m_data->inventoryLineMDL;
        }

    private:
        ImissionIGC*        m_pMission;
        TRef<IpartTypeIGC>  m_pptSuccessor;
        DataPartTypeIGC*    m_data;
        int                 m_dataSize;
};