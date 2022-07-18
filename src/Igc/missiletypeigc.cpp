/*
**  Copyright (C) 1996, 1997 Microsoft Corporation. All Rights Reserved.
**
**  File:	missileTypeIGC.cpp
**
**  Author: 
**
**  Description:
**      Implementation of the CmissileTypeIGC class. This file was initially created by
**  the ATL wizard for the core object.
**
**  History:
*/
// missileTypeIGC.cpp : Implementation of CmissileTypeIGC
#include "missiletypeigc.h"

/////////////////////////////////////////////////////////////////////////////
// CmissileTypeIGC
HRESULT     CmissileTypeIGC::Initialize(ImissionIGC* pMission, Time now, const void* data, int dataSize)
{
    ZAssert (pMission);
    m_pMission = pMission;

    ZRetailAssert (data && (dataSize == sizeof(DataMissileTypeIGC)));
    {
        m_data = *((DataMissileTypeIGC*)data);

        if (iswalpha(m_data.modelName[0]))
        {
            pMission->GetIgcSite()->Preload(m_data.modelName, iswalpha(m_data.textureName[0])
                                                              ? m_data.textureName
                                                              : NULL);
            HitTest::Load(m_data.modelName);
        }
        else
            pMission->GetIgcSite()->Preload(NULL, m_data.textureName);

        pMission->AddExpendableType(this);
    }

    return S_OK;
}

int         CmissileTypeIGC::Export(void* data) const
{
    if (data)
        *((DataMissileTypeIGC*)data) = m_data;

    return sizeof(DataMissileTypeIGC);
}

