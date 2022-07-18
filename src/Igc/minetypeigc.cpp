/*
**  Copyright (C) 1996, 1997 Microsoft Corporation. All Rights Reserved.
**
**  File:	mineTypeIGC.cpp
**
**  Author: 
**
**  Description:
**      Implementation of the CmineTypeIGC class. This file was initially created by
**  the ATL wizard for the core object.
**
**  History:
*/
// mineTypeIGC.cpp : Implementation of CmineTypeIGC
#include "minetypeigc.h"

/////////////////////////////////////////////////////////////////////////////
// CmineTypeIGC
HRESULT     CmineTypeIGC::Initialize(ImissionIGC* pMission, Time now, const void* data, int dataSize)
{
    ZAssert (pMission);
    m_pMission = pMission;

    ZRetailAssert (data && (dataSize == sizeof(DataMineTypeIGC)));
    {
        m_data = *((DataMineTypeIGC*)data);

        pMission->AddExpendableType(this);
    }

    return S_OK;
}

int         CmineTypeIGC::Export(void* data) const
{
    if (data)
        *((DataMineTypeIGC*)data) = m_data;

    return sizeof(DataMineTypeIGC);
}

