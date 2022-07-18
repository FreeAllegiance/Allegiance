/*
**  Copyright (C) 1996, 1997 Microsoft Corporation. All Rights Reserved.
**
**  File:	projectileTypeIGC.cpp
**
**  Author: 
**
**  Description:
**      Implementation of the CprojectileTypeIGC class. This file was initially created by
**  the ATL wizard for the core object.
**
**  History:
*/
// projectileTypeIGC.cpp : Implementation of CprojectileTypeIGC
#include "projectileTypeIGC.h"

/////////////////////////////////////////////////////////////////////////////
// CprojectileTypeIGC
HRESULT     CprojectileTypeIGC::Initialize(ImissionIGC* pMission, Time now, const void* data, int dataSize)
{
    ZAssert (pMission);
    m_pMission = pMission;

    ZRetailAssert (data && (dataSize == sizeof(DataProjectileTypeIGC)));
    {
        m_data = *((DataProjectileTypeIGC*)data);

        if (iswalpha(m_data.modelName[0]))
            pMission->GetIgcSite()->Preload(m_data.modelName, iswalpha(m_data.textureName[0])
                                                              ? m_data.textureName
                                                              : NULL);
        else
            pMission->GetIgcSite()->Preload(NULL, m_data.textureName);

        pMission->AddProjectileType(this);
    }

    return S_OK;
}

int         CprojectileTypeIGC::Export(void* data) const
{
    if (data)
        *((DataProjectileTypeIGC*)data) = m_data;

    return sizeof(DataProjectileTypeIGC);
}

