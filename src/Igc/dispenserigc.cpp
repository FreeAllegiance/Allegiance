/*
**  Copyright (C) 1996, 1997 Microsoft Corporation. All Rights Reserved.
**
**  File:	dispenserIGC.cpp
**
**  Author: 
**
**  Description:
**      Implementation of the CdispenserIGC class. This file was initially created by
**  the ATL wizard for the core object.
**
**  History:
*/
// dispenserIGC.cpp : Implementation of CdispenserIGC
#include "dispenserigc.h"

/////////////////////////////////////////////////////////////////////////////
// CdispenserIGC
CdispenserIGC::CdispenserIGC(void)
:
    m_ship(NULL),
    m_partType(NULL),
    m_expendableType(NULL),
    m_mountID(c_mountNA)
{
}

CdispenserIGC::~CdispenserIGC(void)
{
}

HRESULT     CdispenserIGC::Initialize(ImissionIGC* pMission, Time now, const void* data, int dataSize)
{
    ZAssert (pMission);
    m_pMission = pMission;

    ZRetailAssert (data && (dataSize == sizeof(DataPartIGC)));
    {
        m_partType = (IlauncherTypeIGC*)(((DataPartIGC*)data)->partType);
        ZAssert (m_partType);
        m_partType->AddRef();

        m_expendableType = (IexpendableTypeIGC*)(m_partType->GetExpendableType());
        ZAssert (m_expendableType);
        ZAssert ((m_expendableType->GetObjectType() == OT_mineType) ||
                (m_expendableType->GetObjectType() == OT_probeType) ||
                (m_expendableType->GetObjectType() == OT_chaffType));

		//m_expendableType->AddRef(); // Xynth -"Fix to avoid crash 8963864" 

		// Xynth -"Fix to avoid crash 8963864" 
		if (m_expendableType != NULL)
			m_expendableType->AddRef();
		else
			return S_FALSE;

        m_amount = 0;
    }

	return S_OK;
}

void        CdispenserIGC::Terminate(void)
{
    AddRef();

    SetShip(NULL, NA);

    if (m_partType)
    {
        m_partType->Release();
        m_partType = NULL;
    }

    if (m_expendableType)
    {
        m_expendableType->Release();
        m_expendableType = NULL;
    }

    Release();
}

void        CdispenserIGC::Update(Time   now)
{
    ZAssert (m_ship);

    if (m_mountedFraction < 1.0f)
    {
        float   dt = now - m_ship->GetLastUpdate();
        ZAssert (dt >= 0.0f);

        m_mountedFraction += dt * m_pMission->GetFloatConstant(c_fcidMountRate);
        if (m_mountedFraction < 1.0f)
            return;

        IIgcSite*   pigc = GetMission()->GetIgcSite();
        pigc->PlayNotificationSound(mountedSound, m_ship);
        pigc->PostNotificationText(m_ship, false, "%s ready.", GetPartType()->GetName());
        m_mountedFraction = 1.0f;
    }

    if ((m_timeLoaded < now) && (m_amount > 0))
    {
        static const int    c_maskFire[ET_MAX] =
            { chaffFireIGC,
              0, 0,
              mineFireIGC,
              0, 0, 0, 0 };

        ZAssert (c_maskFire[ET_ChaffLauncher] == chaffFireIGC);
        ZAssert (c_maskFire[ET_Dispenser] == mineFireIGC);

        EquipmentType   et = m_expendableType->GetEquipmentType();
        
        if ((m_ship->GetStateM() & c_maskFire[et]) &&
            ((et == ET_ChaffLauncher) || (m_ship->GetRipcordModel() == NULL)))
        {
            m_timeLoaded = now + m_expendableType->GetLoadTime();

            m_pMission->GetIgcSite()->FireExpendable(m_ship,
                                                     this,
                                                     now);
        }
    }
}

void        CdispenserIGC::SetShip(IshipIGC*       newVal, Mount mount)
{
    //There may be only a single reference to this part ... so make sure the part
    //does not get deleted midway through things
    AddRef();

    if (m_ship)
    {
        m_ship->DeletePart(this);
        m_ship->Release();

    }
    ZAssert (m_mountID == c_mountNA);

    m_ship = newVal;

    if (m_ship)
    {
        m_ship->AddRef();
        m_ship->AddPart(this);

        m_emissionPt = m_ship->GetHitTest()->GetFrameOffset("missemt");

        SetMountID(mount);
    }

    Release();
}
void    CdispenserIGC::SetMountID(Mount newVal)
{
    ZAssert (m_ship);

    if (newVal != m_mountID)
    {
        if ((newVal < 0) && (m_mountID == 0))
            Deactivate();                       //was active iff mounted

        m_ship->MountPart(this, newVal, &m_mountID);

        if (newVal == 0)
            Activate();                         //active iff mounted
    }
}
