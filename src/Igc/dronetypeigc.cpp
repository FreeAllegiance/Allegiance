/*
**  Copyright (C) 1996, 1997 Microsoft Corporation. All Rights Reserved.
**
**  File:	droneTypeIGC.cpp
**
**  Author: 
**
**  Description:
**      Implementation of the CdroneTypeIGC class. This file was initially created by
**  the ATL wizard for the core object.
**
**  History:
*/
// droneTypeIGC.cpp : Implementation of CdroneTypeIGC
#include "dronetypeigc.h"

/////////////////////////////////////////////////////////////////////////////
// CdroneTypeIGC
HRESULT         CdroneTypeIGC::Initialize(ImissionIGC*  pMission,
                                         Time           now,
                                         const void*    data,
                                         int            dataSize)
{
    ZAssert (pMission);
    m_pMission = pMission;

	ZRetailAssert (data && (dataSize == sizeof(DataDroneTypeIGC)));

    memcpy(&m_data, data, dataSize);

    m_petLaid = (m_data.etidLaid == NA)
                ? NULL
                : pMission->GetExpendableType(m_data.etidLaid);

	// KGJV - fix for debug mode - to remove later
    //ZAssert ((m_petLaid != NULL) == (m_data.pilotType == c_ptLayer));

    pMission->AddDroneType(this);

    return S_OK;
}

void            CdroneTypeIGC::Terminate(void)
{
    ZAssert (m_pMission);

    m_pMission->DeleteDroneType(this);
}

int             CdroneTypeIGC::Export(void*  data) const
{
    if (data)
        *((DataDroneTypeIGC*)data) = m_data;

    return sizeof(DataDroneTypeIGC);
}

