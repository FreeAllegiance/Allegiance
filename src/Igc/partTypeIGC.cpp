/*
**  Copyright (C) 1996, 1997 Microsoft Corporation. All Rights Reserved.
**
**  File:	partTypeIGC.cpp
**
**  Author: 
**
**  Description:
**      Implementation of the CpartTypeIGC class. This file was initially created by
**  the ATL wizard for the core object.
**
**  History:
*/
// partTypeIGC.cpp : Implementation of CpartTypeIGC
#include "partTypeIGC.h"

/////////////////////////////////////////////////////////////////////////////
// CpartTypeIGC
HRESULT         CpartTypeIGC::Initialize(ImissionIGC*   pMission,
                                         Time           now,
                                         const void*    data,
                                         int            dataSize)
{
    ZAssert (pMission);
    m_pMission = pMission;

    ZRetailAssert (data && (dataSize >= sizeof(DataPartTypeIGC)));
    {
        //Note ... we need to allocate & copy the DataHullTypeIGC + the variable length fields
        m_dataSize = dataSize;
        m_data = (DataPartTypeIGC*)(new char [m_dataSize]);
        ZAssert (m_data);
        memcpy(m_data, data, m_dataSize);

        if (m_data->successorPartID != NA)
        {
            m_pptSuccessor = pMission->GetPartType(m_data->successorPartID);
            ZAssert (m_pptSuccessor);
        }

        pMission->AddPartType(this);
    }

    return S_OK;
}


void        CpartTypeIGC::Terminate(void)
{
    m_pMission->DeletePartType(this);
}

int         CpartTypeIGC::Export(void*  data) const
{
    if (data)
        memcpy(data, m_data, m_dataSize);

    return m_dataSize;
}


/////////////////////////////////////////////////////////////////////////////
// IpartTypeIGC

const char* IpartTypeIGC::GetEquipmentTypeName(EquipmentType et)
{
    switch (et)
    {
    case ET_ChaffLauncher:
        return "chaff";

    case ET_Weapon:
        return "weapon";

    case ET_Magazine:
        return "missile";

    case ET_Dispenser:
        return "mine";

    case ET_Shield:
        return "shield";

    case ET_Cloak:
        return "cloak";

    case ET_Pack:
        return "ammo";

    case ET_Afterburner:
        return "afterburner";

    default:
        ZAssert(false);
        return "";
    }
}

