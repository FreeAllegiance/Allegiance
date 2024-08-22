/*
**  Copyright (C) 1996, 1997 Microsoft Corporation. All Rights Reserved.
**
**  File:    droneTypeIGC.h
**
**  Author: 
**
**  Description:
**      Header for the CdroneTypeIGC class. This file was initially created by
**  the ATL wizard.
**
**  History:
*/
// droneTypeIGC.h : Declaration of the CdroneTypeIGC

#pragma once

#include "igc.h"

/////////////////////////////////////////////////////////////////////////////
// CdroneTypeIGC
class       CdroneTypeIGC : public IdroneTypeIGC
{
    public:
        CdroneTypeIGC(void)
        {
        }

        ~CdroneTypeIGC(void)
        {
        }

    public:
    // IbaseIGC
        virtual HRESULT         Initialize(ImissionIGC* pMission, Time now, const void* data, int dataSize);
        virtual void            Terminate(void);

        virtual int             Export(void* data) const;

        virtual ObjectType      GetObjectType(void) const
        {
            return OT_droneType;
        }

        virtual ObjectID        GetObjectID(void) const
        {
            return m_data.droneTypeID;
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

        // IdroneTypeIGC
        virtual HullID                  GetHullTypeID(void) const
        {
            return m_data.hullTypeID;
        }
        virtual PilotType               GetPilotType(void) const
        {
            return m_data.pilotType;
        }

        virtual float                   GetShootSkill(void) const
        {
            return m_data.shootSkill;
        }
        virtual float                   GetMoveSkill(void) const
        {
            return m_data.moveSkill;
        }
        virtual float                   GetBravery(void) const
        {
            return m_data.bravery;
        }

        virtual IexpendableTypeIGC*     GetLaidExpendable(void) const
        {
            return m_petLaid;
        }

    private:
        ImissionIGC*                m_pMission;
        DataDroneTypeIGC            m_data;
        TRef<IexpendableTypeIGC>    m_petLaid;
};