/*
**  Copyright (C) 1996, 1997 Microsoft Corporation. All Rights Reserved.
**
**  File:	probeTypeIGC.cpp
**
**  Author: 
**
**  Description:
**      Implementation of the CprobeTypeIGC class. This file was initially created by
**  the ATL wizard for the core object.
**
**  History:
*/
// probeTypeIGC.cpp : Implementation of CprobeTypeIGC
#include "probetypeigc.h"

/////////////////////////////////////////////////////////////////////////////
// CprobeTypeIGC
HRESULT     CprobeTypeIGC::Initialize(ImissionIGC* pMission, Time now, const void* data, int dataSize)
{
    ZAssert (pMission);
    m_pMission = pMission;

    ZRetailAssert (data && (dataSize == sizeof(DataProbeTypeIGC)));
    {
        m_data = *((DataProbeTypeIGC*)data);

        ZAssert (iswalpha(m_data.modelName[0]));
        pMission->GetIgcSite()->Preload(m_data.modelName, iswalpha(m_data.textureName[0])
                                                          ? m_data.textureName
                                                          : NULL);

        MultiHullBase*  pmhb = HitTest::Load(m_data.modelName);
        if (m_data.projectileTypeID != NA)
        {
            m_projectileType = pMission->GetProjectileType(m_data.projectileTypeID);
            ZAssert (m_projectileType);
            m_projectileType->AddRef();

            if (pmhb)
            {
                float   scale = (m_data.radius / pmhb->GetOriginalRadius());
                m_emissionPt = pmhb->GetFrameOffset("wepemt") * scale;
            }
            else
            {
                m_emissionPt = Vector::GetZero();
            }
        }

        pMission->AddExpendableType(this);
    }

    return S_OK;
}

int         CprobeTypeIGC::Export(void* data) const
{
    if (data)
        *((DataProbeTypeIGC*)data) = m_data;

    return sizeof(DataProbeTypeIGC);
}

