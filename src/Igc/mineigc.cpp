/*
**  Copyright (C) 1996, 1997 Microsoft Corporation. All Rights Reserved.
**
**  File:	mineIGC.cpp
**
**  Author: 
**
**  Description:
**      Implementation of the CmineIGC class. This file was initially created by
**  the ATL wizard for the core object.
**
**  History:
*/
// mineIGC.cpp : Implementation of CmineIGC
#include "mineigc.h"

/////////////////////////////////////////////////////////////////////////////
// CmineIGC
CmineIGC::CmineIGC(void)
:
    m_launcher(NULL),
    m_mineType(NULL),
    m_bCreateNow (false)
{
}

CmineIGC::~CmineIGC(void)
{
}

HRESULT CmineIGC::Initialize(ImissionIGC* pMission, Time now, const void* data, int dataSize)
{
    ZRetailAssert (data && (dataSize > sizeof(DataMineBase)));
    {
        DataMineBase*  dataMineBase = (DataMineBase*)data;

        if ((dataMineBase->exportF) && 
            ((DataMineExport*)dataMineBase)->createNow)
        {
            m_time0 = now;
        }
        else
        {
            m_time0 = pMission->GetIgcSite()->ClientTimeFromServerTime(dataMineBase->time0);
        }

        TmodelIGC<ImineIGC>::Initialize(pMission, now, data, dataSize);

        IshipIGC*         pshipLauncher;
        IclusterIGC*      pcluster;
        DataMineTypeIGC*  dataMineType;
        if (dataMineBase->exportF)
        {
            ZAssert (dataSize >= sizeof(DataMineExport));

            DataMineExport* dataMineExport = (DataMineExport*)dataMineBase;

            m_mineType = (ImineTypeIGC*)(pMission->GetExpendableType(dataMineExport->minetypeID));
            ZAssert (m_mineType);
            ZAssert (m_mineType->GetObjectType() == OT_mineType);

            pcluster = pMission->GetCluster(dataMineExport->clusterID);

            ZAssert (m_mineType);
            dataMineType = (DataMineTypeIGC*)(m_mineType->GetData());

            pshipLauncher = pMission->GetShip(dataMineExport->launcherID);
            SetSide(pMission->GetSide(dataMineExport->sideID));

            m_fraction = dataMineExport->fraction;
        }
        else
        {
            ZAssert (dataSize == sizeof(DataMineIGC));

            DataMineIGC*    dataMine = (DataMineIGC*)dataMineBase;

            m_mineType = dataMine->pminetype;
            pcluster = dataMine->pcluster;

            ZAssert (m_mineType);
            ZAssert (m_mineType->GetObjectType() == OT_mineType);
            dataMineType = (DataMineTypeIGC*)(m_mineType->GetData());

            pshipLauncher = dataMine->pshipLauncher;
            SetSide(dataMine->psideLauncher);

            m_fraction = 1.0f;
        }

        if (pshipLauncher && (dataMineType->launcherDef.price == 0))
        {
            m_launcher = pshipLauncher;
            m_launcher->AddRef();
        }

        ZAssert (GetSide());
        ZAssert (pcluster);

        m_mineType->AddRef();

        //Load the model for the mine
        LoadMine(dataMineType->textureName, 1.0f, dataMineType->radius, dataMineType->iconName,
                 c_mtSeenBySide | c_mtPredictable | c_mtHitable | c_mtStatic);

        SetRadius(dataMineType->radius);
        SetSignature(dataMineType->signature);
        SetSecondaryName(dataMineType->launcherDef.name);

        {
            //lifespan == 0 => immortal mine that can hit until it gets terminated on the next update; this is bad
            ZAssert (dataMineType->lifespan > 0.0f);
            m_timeExpire = m_time0 + dataMineType->lifespan;
            ZAssert (m_timeExpire != m_time0);
        }
        SetMass(0.0f);

        m_mineID = dataMineBase->mineID;

        SetPosition(dataMineBase->p0);
        SetCluster(pcluster);

        pMission->AddMine(this);
    }

    return S_OK;
}

void    CmineIGC::Terminate(void)
{
    AddRef();

    GetMyMission()->DeleteMine(this);
	TmodelIGC<ImineIGC>::Terminate();

    if (m_launcher)
    {
        m_launcher->Release();
        m_launcher = NULL;
    }

    if (m_mineType)
    {
        m_mineType->Release();
        m_mineType = NULL;
    }
    Release();
}

void    CmineIGC::Update(Time now)
{
    if (now > m_timeExpire)
        Terminate();
    else
        TmodelIGC<ImineIGC>::Update(now);
}

int                 CmineIGC::Export(void*    data) const
{
    int size = sizeof(DataMineExport);

    if (data)
    {
        DataMineExport* pdme = (DataMineExport*)data;

        pdme->p0 = GetPosition();
        pdme->time0 = m_time0;

        pdme->mineID = GetObjectID();
        pdme->exportF = true;

        pdme->clusterID = GetCluster()->GetObjectID();
        pdme->minetypeID = m_mineType->GetObjectID();

        pdme->launcherID = m_launcher ? m_launcher->GetObjectID() : NA;

        pdme->sideID = GetSide()->GetObjectID();

        pdme->fraction = m_fraction;

        pdme->createNow = m_bCreateNow;
    }

    return size;
}
