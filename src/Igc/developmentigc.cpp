/*
**  Copyright (C) 1996, 1997 Microsoft Corporation. All Rights Reserved.
**
**  File:	developmentIGC.cpp
**
**  Author: 
**
**  Description:
**      Implementation of the CdevelopmentIGC class. This file was initially created by
**  the ATL wizard for the core object.
**
**  History:
*/
// developmentIGC.cpp : Implementation of CdevelopmentIGC
#include "developmentigc.h"

/////////////////////////////////////////////////////////////////////////////
// CdevelopmentIGC
HRESULT         CdevelopmentIGC::Initialize(ImissionIGC* pMission,
                                            Time         now,
                                            const void*  data,
                                            int          dataSize)
{
    ZAssert (pMission);
    m_pMission = pMission;

	ZRetailAssert (data && (dataSize == sizeof(DataDevelopmentIGC)));
    m_data = *((DataDevelopmentIGC*)data);

    pMission->AddDevelopment(this);

    if (m_data.developmentID == c_didTeamMoney)
    {
        m_techOnly = false;     //Wining the game is not "tech only"
    }
    else
    {
        m_techOnly = true;
        for (int i = 0; (i < c_gaMax); i++)
        {
            if (m_data.gas.GetAttribute(i) != 1.0f)
            {
                m_techOnly = false;
                break;
            }
        }
    }

    return S_OK;
}

void            CdevelopmentIGC::Terminate(void)
{
    m_pMission->DeleteDevelopment(this);
}

int             CdevelopmentIGC::Export(void*  data) const
{
    if (data)
        *((DataDevelopmentIGC*)data) = m_data;

    return sizeof(DataDevelopmentIGC);
}

