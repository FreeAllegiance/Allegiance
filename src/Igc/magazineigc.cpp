/*
**  Copyright (C) 1996, 1997 Microsoft Corporation. All Rights Reserved.
**
**  File:	magazineIGC.cpp
**
**  Author: 
**
**  Description:
**      Implementation of the CmagazineIGC class. This file was initially created by
**  the ATL wizard for the core object.
**
**  History:
*/
// magazineIGC.cpp : Implementation of CmagazineIGC
#include "magazineigc.h"

/////////////////////////////////////////////////////////////////////////////
// CmagazineIGC
CmagazineIGC::CmagazineIGC(void)
:
    m_ship(NULL),
    m_partType(NULL),
    m_missileType(NULL),
    m_mountID(c_mountNA),
    m_lock(0.0f)
{
}

CmagazineIGC::~CmagazineIGC(void)
{
}

HRESULT     CmagazineIGC::Initialize(ImissionIGC* pMission, Time now, const void* data, int dataSize)
{
    ZAssert (pMission);
    m_pMission = pMission;

    ZRetailAssert (data && (dataSize == sizeof(DataPartIGC)));
    {
        m_partType = (IlauncherTypeIGC*)(((DataPartIGC*)data)->partType);
        ZAssert (m_partType);
        if (m_partType) //Xynth for good meaure
			m_partType->AddRef();

        m_missileType = (ImissileTypeIGC*)(m_partType->GetExpendableType());
        ZAssert (m_missileType);
        ZAssert (m_missileType->GetObjectType() == OT_missileType);
        if (m_missileType) //Xynth Fix for error 8986029
			m_missileType->AddRef();
        m_amount = 0;
    }

	return S_OK;
}

void        CmagazineIGC::Terminate(void)
{
    AddRef();

    SetShip(NULL, NA);

    if (m_partType)
    {
        m_partType->Release();
        m_partType = NULL;
    }

    if (m_missileType)
    {
        m_missileType->Release();
        m_missileType = NULL;
    }

    Release();
}

void        CmagazineIGC::Update(Time   now)
{
    ZAssert (m_ship);

    Time    lastUpdate = m_ship->GetLastUpdate();
    float   dt = now - lastUpdate;
    ZAssert (dt >= 0.0f);

    if (m_mountedFraction < 1.0f)
    {

        m_mountedFraction += dt * m_pMission->GetFloatConstant(c_fcidMountRate);
        if (m_mountedFraction < 1.0f)
        {
            m_lock = 0.0f;
            return;
        }

        IIgcSite*   pigc = GetMission()->GetIgcSite();
        pigc->PlayNotificationSound(mountedSound, m_ship);
        pigc->PostNotificationText(m_ship, false, "%s ready.", GetPartType()->GetName());
        m_mountedFraction = 1.0f;
    }

    ZAssert (m_ship);
    if ((m_timeLoaded < now) && (m_amount > 0))
    {
        if (m_ship->GetRipcordModel() != NULL)
            m_lock = 0.0f;
        else
        {
            ImodelIGC*  ptarget = m_ship->GetCommandTarget(c_cmdCurrent);

            float   maxLock = m_missileType->GetMaxLock();
            if (m_ship->GetStateM() & missileFireIGC)
            {
                //Twizzle anything now so that nothing needs to be adjusted after shooting (which may cause a terminate)
                float   oldLock = m_lock;
                m_lock = 0.0f;
                m_timeLoaded = now + m_missileType->GetLoadTime();

                m_pMission->GetIgcSite()->FireMissile(m_ship,
                                                      this,
                                                      now,
                                                      ptarget,
                                                      oldLock * maxLock);

            }
            else
            {
                if (ptarget && (ptarget->GetCluster() == m_ship->GetCluster()) &&   //have a target,
                    ((ptarget->GetSide() != m_ship->GetSide()) && (!IsideIGC::AlliedSides(ptarget->GetSide(),m_ship->GetSide()))) &&  //and not on friendly sides #ALLY mmf 11/08 added !   IMAGO FIXED 7/8/09
                    (m_missileType->HasCapability(c_eabmWarpBomb)                   //is a warp bomb
                     ? (ptarget->GetObjectType() == OT_warp)                        //and target is a warp
                     : (ptarget->GetAttributes() & c_mtDamagable)) &&               //is not a warp bomb and we can do damage to it
                    m_ship->CanSee(ptarget))                                        //and we can see it,
                {
                    float   lifespan = m_missileType->GetLifespan();
                    float   maxRange = m_missileType->GetInitialSpeed() * lifespan + 0.5f * m_missileType->GetAcceleration() * lifespan * lifespan;

                    Vector  dP = (ptarget->GetPosition() - m_ship->GetPosition());
                    float   l2 = dP.LengthSquared();

                    float   oldLock = m_lock;
                    m_lock = 0.0f;

                    if ((l2 > 1.0f) && (l2 < maxRange * maxRange))
                    {
                        if (maxLock == 0.0f)
                            m_lock = 1.0f;
                        else
                        {
                            dP.SetNormalize();

                            double  dot = -(dP * m_ship->GetOrientation().GetBackward());
                            float   angleOffBow = (dot >= 1.0)
                                                  ? 0.0f
                                                  : ((dot <= -1.0f)
                                                     ? pi
                                                     : (float)acos(dot));

                            float   lockTime = m_missileType->GetLockTime();
                            float   lockAngle = m_missileType->GetLockAngle();

                            float   v = (lockAngle - angleOffBow) / lockAngle;
                            if (v < 0.0)
                                v *= lockTime;

                            m_lock = oldLock + v * dt / (ptarget->GetObjectType() == OT_ship 
                                                         ? (lockTime * ((IshipIGC*)ptarget)->GetHullType()->GetECM())
                                                         : lockTime);
                        }

                        if (m_lock >= 1.0f)
                        {
                            m_lock = 1.0f;
                        }
                        else if (m_lock < 0.0f)
                            m_lock = 0.0f;
                    }
                }
            }
        }
    }
}
void        CmagazineIGC::SetShip(IshipIGC*       newVal, Mount mount)
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
void    CmagazineIGC::SetMountID(Mount newVal)
{
    ZAssert (m_ship);

    if (newVal != m_mountID)
    {
        if ((newVal < 0) && (m_mountID == 0))
            Deactivate();

        m_ship->MountPart(this, newVal, &m_mountID);

        if (newVal == 0)
            Activate();
    }
}
