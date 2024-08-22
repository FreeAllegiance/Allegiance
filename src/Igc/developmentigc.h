/*
**  Copyright (C) 1996, 1997 Microsoft Corporation. All Rights Reserved.
**
**  File:    developmentIGC.h
**
**  Author: 
**
**  Description:
**      Header for the CdevelopmentIGC class. This file was initially created by
**  the ATL wizard.
**
**  History:
*/
// developmentIGC.h : Declaration of the CdevelopmentIGC

#pragma once

#include "igc.h"

/////////////////////////////////////////////////////////////////////////////
// CdevelopmentIGC
class       CdevelopmentIGC : public IdevelopmentIGC
{
    public:
        CdevelopmentIGC(void)
        {
        }

        ~CdevelopmentIGC(void)
        {
        }

    public:
    // IbaseIGC
        virtual HRESULT     Initialize(ImissionIGC* pMission, Time now, const void* data, int dataSize);
        virtual void        Terminate(void);

        virtual int         Export(void* data) const;

        virtual ObjectType  GetObjectType(void) const
        {
            return OT_development;
        }
        virtual ObjectID    GetObjectID(void) const
        {
            return m_data.developmentID;
        }

    // ItypeIGC
        virtual const void*   GetData(void) const
        {
            return &m_data;
        }

    // IbuyableIGC
        virtual const char*          GetModelName(void) const
        {
            return m_data.modelName;
        }

        virtual const char*          GetIconName(void) const
        {
            return m_data.iconName;
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

        // IdevelopmentIGC
        virtual const GlobalAttributeSet&   GetGlobalAttributeSet(void) const
        {
            return m_data.gas;
        }

        virtual bool                        GetTechOnly(void) const
        {
            return m_techOnly;
        }

        virtual bool                        IsObsolete(const TechTreeBitMask&  ttbm) const
        {
            return m_techOnly && (m_data.ttbmEffects <= ttbm);
        }

        virtual SoundID                     GetCompletionSound() const
        {
            return m_data.completionSound;
        }

    private:
        ImissionIGC*        m_pMission;
        DataDevelopmentIGC  m_data;
        bool                m_techOnly;
};