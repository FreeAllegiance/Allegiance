/*
**  Copyright (C) 1996, 1997 Microsoft Corporation. All Rights Reserved.
**
**  File:	treasureIGC.cpp
**
**  Author: 
**
**  Description:
**      Implementation of the CtreasureIGC class. This file was initially created by
**  the ATL wizard.
**
**  History:
*/
// treasureIGC.cpp : Implementation of CtreasureIGC
#include "treasureIGC.h"
#include <stdio.h>

const float c_deceleration = 10.0f;

/////////////////////////////////////////////////////////////////////////////
// CtreasureIGC
HRESULT     CtreasureIGC::Initialize(ImissionIGC* pMission, Time now, const void* data, int dataSize)
{
    //Does this treasure already exist? If so, abort which will prevent it from being terminated
    if (pMission->GetTreasure(((DataTreasureIGC*)data)->objectID))
        return E_ABORT;

    ZRetailAssert (data && (dataSize == sizeof(DataTreasureIGC)));
    {
        DataTreasureIGC*  dataTreasure = (DataTreasureIGC*)data;

        m_data = *dataTreasure;
        m_data.createNow = false;

        if (dataTreasure->createNow)
        {
            m_data.time0 = now;
        }
        else
        {
            m_data.time0 = pMission->GetIgcSite()->ClientTimeFromServerTime(m_data.time0);
        }

        TmodelIGC<ItreasureIGC>::Initialize(pMission, now, &m_data, sizeof(m_data));

        if (m_data.treasureCode == c_tcPart)
        {
            IpartTypeIGC*   pt = pMission->GetPartType(m_data.treasureID);
            ZAssert (pt);

            m_buyable = (IbuyableIGC*)(pt);

            if (m_data.amount == NA)
            {
                switch (pt->GetEquipmentType())
                {
                    case ET_Dispenser:
                    case ET_ChaffLauncher:
                    case ET_Magazine:
                    {
                        m_data.amount = randomInt(1, pt->GetAmount(NULL));
                    }
                    break;

                    case ET_Pack:
                    {
                        short a = ((DataPackTypeIGC*)(pt->GetData()))->amount;
                        ZAssert (a > 2);
                        m_data.amount = randomInt(a/2, a);
                    }
                    break;

                    default:
                        m_data.amount = 0;
                }
            }

            ZAssert (m_buyable);
            m_buyable->AddRef();
        }
        else if (m_data.treasureCode == c_tcDevelopment)
        {
            m_buyable = (IbuyableIGC*)(pMission->GetDevelopment(m_data.treasureID));
            ZAssert (m_buyable);
            m_buyable->AddRef();
        }
        else
        {
            m_buyable = NULL;

            if (m_data.treasureCode == c_tcCash)
                m_data.amount = m_data.treasureID;                
        }

        Vector  p0;
        const Vector* pposition;
        const Vector* pvelocity;
        m_speed0 = m_data.v0.Length();
        m_stationaryF = (m_speed0 == 0.0f);
        {
            float   dt = now - m_data.time0;
            if (m_speed0 <= c_deceleration * dt)
            {
                //drifted to a stop
                m_stationaryF = true;

                p0 = m_data.p0 + m_data.v0 * (0.5f * m_speed0 / c_deceleration);
                pposition = &p0;
                pvelocity = &(Vector::GetZero());
            }
            else
            {
                pposition = &(m_data.p0);
                pvelocity = &(m_data.v0);
            }
        }

        static const char*  cName = "acs14";

        const char* name;
        const char* texture = NULL;
        const char* icon;
        if (m_buyable)
        {
            name = m_buyable->GetModelName();
            icon = m_buyable->GetIconName();
        }
        else if (m_data.treasureCode == c_tcFlag)
        {
            if (m_data.treasureID != SIDE_TEAMLOBBY)
            {
                name = NULL;
                texture = "biosflag";
                icon = "flag";

                SetName("Flag");
                IsideIGC*   pside = pMission->GetSide(m_data.treasureID);
                ZAssert (pside);
                SetSide(pside);
            }
            else
            {
                name = "artifact";
                icon = "articon";
                SetName("Artifact");
            }

            SetSignature(1.5f);
        }
        else
        {
            ZAssert ((m_data.treasureCode == c_tcCash) || (m_data.treasureCode == c_tcPowerup));
            name = cName;
            icon = (m_data.treasureCode == c_tcCash) ? "cashicon" : "iconpu";
        }

        if (texture)
        {
            HRESULT rc = LoadDecal(texture, icon,
                           Color(1.0f,
                                 1.0f,
                                 1.0f,
                                 1.0f),
                           false,
                           1.0f,
                           m_stationaryF
                           ? (c_mtStatic | c_mtHitable | c_mtSeenBySide | c_mtPredictable)
                           : (c_mtCastRay | c_mtSeenBySide | c_mtPredictable));

            ZAssert (SUCCEEDED(rc));
        }
        else
        {
            HRESULT rc = Load(0, name, texture, icon,
                      m_stationaryF
                      ? (c_mtStatic | c_mtHitable | c_mtSeenBySide | c_mtPredictable)
                      : (c_mtCastRay | c_mtSeenBySide | c_mtPredictable));
            ZAssert (SUCCEEDED(rc));

            Orientation o;
            Rotation    r;
            //Parts get a random orientation
            o.Set(Vector::RandomDirection());

            //and a random rotation
            r.axis(Vector::RandomDirection());
            r.angle(random(-6, 6));

            SetOrientation(o);
            SetRotation(r);
        }

        SetRadius(5.0f);		//NYI -- needs to be gotten from the DB

        SetMass(0.0f);

        m_expire = m_data.time0 + m_data.lifespan;

        SetPosition(*pposition);
        SetVelocity(*pvelocity);
        SetCluster(pMission->GetCluster(m_data.clusterID));
        pMission->AddTreasure(this);

        //Fun and games time ... treasures can be created in the cluster update
        //so initialize everything as if we were in the resolve collisions part
        //of the cluster update before adding the object to the cluster.
        //Fortunately, the KD tree is no longer used in the update by this point
        //and all we need to do is set the bounding box 
        SetBB(now, now, 0.0f);
    }

    return S_OK;
}

void CtreasureIGC::Update(Time now)
{
    if (now >= m_expire)
        GetMyMission()->GetIgcSite()->KillTreasureEvent(this);
    else
    {
        if ((!m_stationaryF) && (now > m_data.time0))
        {
            float   dt1 = GetMyLastUpdate() - m_data.time0;
            float   v1 = m_speed0 - c_deceleration * dt1;
            if (v1 <= 0.0f)
            {
                //The object came, previously, to a complete stop
                m_stationaryF = true;

                SetPosition(m_data.p0 + m_data.v0 * (0.5f * m_speed0 / c_deceleration));
                SetVelocity(Vector::GetZero());

                MakeStatic();
            }
            else
            {
                //Still moving ... calculate the object's position at the start and stop
                ZAssert (m_speed0 != 0.0f);
                Vector p1 = m_data.p0 + 
                            m_data.v0 *
                            (dt1 - 0.5f * dt1 * dt1 * c_deceleration / m_speed0);

                SetPosition(p1);
                if (now == GetMyLastUpdate())
                {
                    SetVelocity(m_data.v0 * (v1 / m_speed0));
                }
                else
                {
                    float   dt2 = now - m_data.time0;
                    float   v2 = m_speed0 - c_deceleration * dt2;

                    Vector  p2 = (v2 > 0.0f)
                                 ? (m_data.p0 + 
                                    m_data.v0 *
                                    (dt2 - 0.5f * dt2 * dt2 * c_deceleration / m_speed0))
                                 : (m_data.p0 + m_data.v0 * (0.5f * m_speed0 / c_deceleration));

                    SetVelocity((p2 - p1) / (now - GetMyLastUpdate()));
                }
            }
        }

        TmodelIGC<ItreasureIGC>::Update(now);
    }
}

int CtreasureIGC::Export(void* data) const
{
    if (data)
    {
        DataTreasureIGC*  dataTreasure = (DataTreasureIGC*)data;
        *dataTreasure = m_data;
        dataTreasure->time0 = GetMyMission()->GetIgcSite()->ServerTimeFromClientTime(m_data.time0);
    }

    return sizeof(DataTreasureIGC);
}
