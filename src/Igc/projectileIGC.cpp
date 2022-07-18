/*
**  Copyright (C) 1996, 1997 Microsoft Corporation. All Rights Reserved.
**
**  File:	projectileIGC.cpp
**
**  Author: 
**
**  Description:
**      Implementation of the CprojectileIGC class. This file was initially created by
**  the ATL wizard for the core object.
**
**      This particular implementation of projectiles supports non-tracking missiles with
**  a fixed run (time at which they expire).
**
**  History:
*/
// projectileIGC.cpp : Implementation of CprojectileIGC
#include "projectileIGC.h"

/////////////////////////////////////////////////////////////////////////////
// CprojectileIGC
CprojectileIGC::CprojectileIGC(void)
:
    m_launcher(NULL),
    m_projectileType(NULL),
    m_pExplosionData(NULL)
{
}

CprojectileIGC::~CprojectileIGC(void)
{
}

HRESULT CprojectileIGC::Initialize(ImissionIGC* pMission, Time now, const void* data, int dataSize)
{
    TmodelIGC<IprojectileIGC>::Initialize(pMission, now, data, dataSize);

    ZRetailAssert (data && (dataSize == sizeof(DataProjectileIGC)));
    {
        DataProjectileIGC*  dataProjectile = (DataProjectileIGC*)data;

        m_projectileType = pMission->GetProjectileType(dataProjectile->projectileTypeID);
        if (m_projectileType)
        {
            m_projectileType->AddRef();

            DataProjectileTypeIGC*  dataProjectileType = (DataProjectileTypeIGC*)m_projectileType->GetData();
            ZAssert (dataProjectileType);

            //Load the model for the projectile
            if (iswalpha(dataProjectileType->modelName[0]))
            {
                HRESULT hr = Load(0, dataProjectileType->modelName, dataProjectileType->textureName, NULL, c_mtCastRay | c_mtNotPickable);
                ZAssert (SUCCEEDED(hr));
                SetRadius(dataProjectileType->radius);
            }
            else
            {
                HRESULT hr = LoadDecal(dataProjectileType->textureName, NULL,
                               Color((float)dataProjectileType->color.r,
                                     (float)dataProjectileType->color.g,
                                     (float)dataProjectileType->color.b,
                                     (float)dataProjectileType->color.a),
                               dataProjectileType->bDirectional,
                               dataProjectileType->width,
                               c_mtCastRay | c_mtNotPickable);
                ZAssert (SUCCEEDED(hr));
                GetHitTest()->SetRadius(0.0f);
                GetThingSite()->SetRadius(dataProjectileType->radius);
            }

            //position set after being initialized
            SetVelocity(dataProjectile->velocity);
            {
                Orientation o(dataProjectile->forward);
                SetOrientation(o);
            }
            {
                Rotation    r(dataProjectile->forward, dataProjectileType->rotation);
                SetRotation(r);
            }
            {
                HitTest*    ht = GetHitTest();

                //lifespan == 0 => immortal projectile that can hit until it gets terminated on the next update; this is bad
                ZAssert (dataProjectile->lifespan > 0.0f);
                ht->SetTimeStart(now);
                ht->SetTimeStop(m_timeExpire = now + dataProjectile->lifespan); //intentional assignment
                ZAssert (m_timeExpire != now);
            }

            SetMass(0.0f);
        }
    }

    return S_OK;
}

void    CprojectileIGC::Terminate(void)
{
    AddRef();

	TmodelIGC<IprojectileIGC>::Terminate();

    if (m_launcher)
    {
        m_launcher->Release();
        m_launcher = NULL;
    }

    if (m_projectileType)
    {
        m_projectileType->Release();
        m_projectileType = NULL;
    }

    Release();
}

void    CprojectileIGC::Update(Time now)
{
    if (GetMyLastUpdate() >= m_timeExpire)
        Terminate();
    else
    {
        if (now >= m_timeExpire)
        {
            float   p = m_projectileType->GetBlastPower();
            if (p != 0.0f)
            {
                m_pExplosionData = GetCluster()->CreateExplosion(m_projectileType->GetDamageType(),
                                                                 p * GetSide()->GetGlobalAttributeSet().GetAttribute(c_gaDamageGuns),
                                                                 m_projectileType->GetBlastRadius(),
                                                                 c_etProjectile,
                                                                 m_timeExpire,
                                                                 GetPosition() +
                                                                 GetVelocity() * (m_timeExpire - GetMyLastUpdate()),
                                                                 m_launcher);
            }
        }

        TmodelIGC<IprojectileIGC>::Update(now);
    }
}

int     CprojectileIGC::Export(void*    data)
{
    if (data)
    {
        DataProjectileIGC*  dataProjectile = (DataProjectileIGC*)data;

        dataProjectile->projectileTypeID = m_projectileType->GetObjectID();

        dataProjectile->velocity = GetVelocity();
        dataProjectile->forward = GetOrientation().GetForward();
    }

    return sizeof(DataProjectileIGC);
}

void                 CprojectileIGC::HandleCollision(Time       timeCollision,
                                                     float                  tCollision,
                                                     const CollisionEntry&  entry,
                                                     ImodelIGC* pModel)
{
    if (pModel->GetObjectType() == OT_mine)
    {
        //Ignore collisions with minefields
        return;
    }

    //A projectile hit something other than a minefield
    bool    fExplosion;
    Vector position1 = GetPosition() + GetVelocity() * tCollision;
    Vector position2 = pModel->GetPosition() + pModel->GetVelocity()  * tCollision;

    DamageTypeID type = m_projectileType->GetDamageType();
    if (pModel->GetAttributes() & c_mtDamagable)
    {
        //The target can take damage
        IdamageIGC*   pDamage = (IdamageIGC*)pModel;

        pModel->AddRef();

        DamageResult dr = pDamage->ReceiveDamage(type,
                                                 m_projectileType->GetPower() * GetSide()->GetGlobalAttributeSet().GetAttribute(c_gaDamageGuns),
                                                 timeCollision,
                                                 position2,
                                                 position1,
                                                 m_launcher);

        //No explosion because no hull damage, not visible or we killed it.
        fExplosion = (dr <= c_drHullDamage) && pModel->GetVisibleF() && (pModel->GetCluster() != NULL);

        pModel->Release();
    }
    else
         fExplosion = false;

    if (m_pExplosionData)
    {
        //This projectile would have exploded anyway -- change the explosion position & time.
        m_pExplosionData->time = timeCollision;
        m_pExplosionData->position = position1;
    }
    else
    {
        float   p = m_projectileType->GetBlastPower();
        if (p != 0.0f)
        {
            //Area effect weapon: create an explosion (no matter what)
            GetCluster()->CreateExplosion(type,
                                          p,
                                          m_projectileType->GetBlastRadius(),
                                          c_etProjectile,
                                          timeCollision,
                                          position1,
                                          m_launcher);
        }
        else if (fExplosion && ((type & c_dmgidNoDebris) == 0))
        {
            Vector pos = position1 - position2;

            pModel->GetThingSite()->AddHullHit(pos,
                                               pos.Normalize());
        }
    }

    //In any case, kill the projectile
    Terminate();
}
