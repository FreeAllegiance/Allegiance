/*
**  Copyright (C) 1996, 1997 Microsoft Corporation. All Rights Reserved.
**
**  File:	treasureSetIGC.cpp
**
**  Author: 
**
**  Description:
**      Implementation of the CtreasureSetIGC class. This file was initially created by
**  the ATL wizard for the core object.
**
**  History:
*/
// treasureSetIGC.cpp : Implementation of CtreasureSetIGC
#include "treasuresetigc.h"

/////////////////////////////////////////////////////////////////////////////
// CtreasureSetIGC
HRESULT     CtreasureSetIGC::Initialize(ImissionIGC* pMission, Time now, const void* data, int dataSize)
{
    ZAssert (pMission);
    m_pMission = pMission;

    ZRetailAssert (data && (dataSize >= sizeof(DataTreasureSetIGC)));

    int size;
    if (((DataTreasureSetIGC*)data)->nTreasureData == 0)
    {
        ZAssert (dataSize == sizeof(DataTreasureSetIGC));

        const short c_maxDefault = 10;
        m_maxTreasureData = c_maxDefault;
        size = sizeof(DataTreasureSetIGC) +
               c_maxDefault * sizeof(TreasureData);
    }
    else
    {
        m_maxTreasureData = ((DataTreasureSetIGC*)data)->nTreasureData;

        ZAssert (dataSize == int(sizeof(DataTreasureSetIGC) +
                                m_maxTreasureData *
                                sizeof(TreasureData)));

        size = dataSize;
    }
    ZAssert (size >= dataSize);

    m_data = (DataTreasureSetIGC*)(new char [size]);
    memcpy(m_data, data, dataSize);

    m_totalTreasureChance = 0;
    {
        TreasureData*   ptd = m_data->treasureData0();
        for (short i = 0; (i < m_data->nTreasureData); i++)
            m_totalTreasureChance += (ptd + i)->chance;
    }

    pMission->AddTreasureSet(this);

    return S_OK;
}

int         CtreasureSetIGC::Export(void* data) const
{
    int size = sizeof(DataTreasureSetIGC) + sizeof(TreasureData) *
                                            m_data->nTreasureData;

    if (data)
        memcpy(data, m_data, size);

    return size;
}

