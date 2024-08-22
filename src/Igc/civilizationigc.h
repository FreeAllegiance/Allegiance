/*
**  Copyright (C) 1996, 1997 Microsoft Corporation. All Rights Reserved.
**
**  File:    civilizationIGC.h
**
**  Author: 
**
**  Description:
**      Header for the CcivilizationIGC class. This file was initially created by
**  the ATL wizard.
**
**  History:
*/
// civilizationIGC.h : Declaration of the CcivilizationIGC

#pragma once

#include "igc.h"

/////////////////////////////////////////////////////////////////////////////
// CcivilizationIGC
class       CcivilizationIGC : public IcivilizationIGC
{
    public:
        CcivilizationIGC(void)
        {
        }

    public:
    // IbaseIGC
        virtual HRESULT                     Initialize(ImissionIGC* pMission, Time now, const void* data, int dataSize);
        virtual void                        Terminate(void);

        virtual int                         Export(void* data) const;

        virtual ObjectType                  GetObjectType(void) const
        {
            return OT_civilization;
        }

        virtual ObjectID                    GetObjectID(void) const
        {
            return m_data.civilizationID;
        }

    // IcivilizationIGC
        virtual const char*                 GetName(void) const
        {
            return m_data.name;
        }

        virtual const char*                 GetIconName(void) const
        {
            return m_data.iconName;
        }

        virtual IhullTypeIGC*               GetLifepod(void) const
        {
            return m_phtLifepod;
        }

        virtual const TechTreeBitMask&      GetBaseTechs(void) const
        {
            return m_data.ttbmBaseTechs;
        }
        virtual const TechTreeBitMask&      GetNoDevTechs(void) const
        {
            return m_data.ttbmNoDevTechs;
        }
        virtual const GlobalAttributeSet&   GetBaseAttributes(void) const
        {
            return m_data.gasBaseAttributes;
        }
        virtual IstationTypeIGC*            GetInitialStationType(void) const
        {
            return m_pstInitial;
        }
        virtual const char*                 GetHUDName(void) const
        {
            return m_data.hudName;
        }

        virtual float                       GetBonusMoney(void) const
        {
            return m_data.bonusMoney;
        }

        virtual float                       GetIncomeMoney(void) const
        {
            return m_data.incomeMoney;
        }

    private:
        ImissionIGC*        m_pMission;
        DataCivilizationIGC m_data;

        IhullTypeIGC*       m_phtLifepod;
        IstationTypeIGC*    m_pstInitial;
};