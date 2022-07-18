/*
**  Copyright (C) 1996, 1997 Microsoft Corporation. All Rights Reserved.
**
**  File:	civilizationIGC.cpp
**
**  Author: 
**
**  Description:
**      Implementation of the CcivilizationIGC class. This file was initially created by
**  the ATL wizard for the core object.
**
**  History:
*/
// civilizationIGC.cpp : Implementation of CcivilizationIGC
#include "civilizationigc.h"

/////////////////////////////////////////////////////////////////////////////
// CcivilizationIGC
HRESULT         CcivilizationIGC::Initialize(ImissionIGC*   pMission,
                                             Time           now,
                                             const void*    data,
                                             int            dataSize)
{
    ZAssert (pMission);
    m_pMission = pMission;

	ZRetailAssert (data && (dataSize == sizeof(DataCivilizationIGC)));
    m_data = *((DataCivilizationIGC*)data);

    pMission->AddCivilization(this);

    m_pstInitial = pMission->GetStationType(m_data.initialStationTypeID);
    ZAssert (m_pstInitial);
    m_pstInitial->AddRef();
    ZAssert (m_pstInitial->HasCapability(c_sabmRestart));

    m_phtLifepod = pMission->GetHullType(m_data.lifepod);
    ZAssert (m_phtLifepod);
    m_phtLifepod->AddRef();

    return S_OK;
}

void            CcivilizationIGC::Terminate(void)
{
    ZAssert (m_pstInitial);
    m_pstInitial->Release();
    m_pstInitial = NULL;

    ZAssert (m_phtLifepod);
    m_phtLifepod->Release();
    m_phtLifepod = NULL;

    m_pMission->DeleteCivilization(this);
}

int             CcivilizationIGC::Export(void*  data) const
{
    if (data)
    {
        *((DataCivilizationIGC*)data) = m_data;
    }

    return sizeof(DataCivilizationIGC);
}

