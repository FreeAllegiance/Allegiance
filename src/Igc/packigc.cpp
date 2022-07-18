/*
**  Copyright (C) 1996, 1997 Microsoft Corporation. All Rights Reserved.
**
**  File:	packIGC.cpp
**
**  Author: 
**
**  Description:
**      Implementation of the CpackIGC class. This file was initially created by
**  the ATL wizard for the core object.
**
**  History:
*/
// packIGC.cpp : Implementation of CpackIGC
#include "packigc.h"

/////////////////////////////////////////////////////////////////////////////
// CpackIGC
CpackIGC::CpackIGC(void)
:
    m_partType(NULL),
    m_pship(NULL),
    m_mountID(c_mountNA)
{
}

CpackIGC::~CpackIGC(void)
{
}

HRESULT     CpackIGC::Initialize(ImissionIGC* pMission, Time now, const void* data, int dataSize)
{
    ZAssert (pMission);
    m_pMission = pMission;

    ZRetailAssert (data && (dataSize == sizeof(DataPartIGC)));
    {
        m_partType = ((DataPartIGC*)data)->partType;
        ZAssert (m_partType);
        m_partType->AddRef();

        m_typeData = (const DataPackTypeIGC*)m_partType->GetData();
        m_amount = m_typeData->amount;
    }

	return S_OK;
}

void        CpackIGC::Terminate(void)
{
    AddRef();

    SetShip(NULL, NA);

    if (m_partType)
    {
        m_partType->Release();
        m_partType = NULL;
    }

    Release();
}

void        CpackIGC::SetShip(IshipIGC*       newVal, Mount mount)
{
    //There may be only a single reference to this part ... so make sure the part
    //does not get deleted midway through things
    AddRef();

    if (m_pship)
    {
        m_pship->DeletePart(this);
        m_pship->Release();
    }
    ZAssert (m_mountID == c_mountNA);

    m_pship = newVal;

    if (m_pship)
    {
        m_pship->AddRef();
        m_pship->AddPart(this);

        SetMountID(mount);
    }

    Release();
}


void    CpackIGC::SetMountID(Mount newVal)
{
    ZAssert (m_pship);

    if (newVal != m_mountID)
    {
        m_pship->MountPart(this, newVal, &m_mountID);
    }
}
