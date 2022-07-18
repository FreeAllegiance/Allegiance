/*
**  Copyright (C) 1996, 1997 Microsoft Corporation. All Rights Reserved.
**
**  File:	buildingEffectIGC.cpp
**
**  Author: 
**
**  Description:
**      Implementation of the CclusterIGC class. This file was initially created by
**  the ATL wizard for the core object.
**
**  History:
*/
// buildingEffectIGC.cpp : Implementation of buildingEffectIGC

#ifndef __BUILDINGEFFECTIGC_H_
#define __BUILDINGEFFECTIGC_H_

const float c_dtShrink   = 10.0f;
const float c_dtOpaque   = 10.0f;
const float c_dtGrow     = 10.0f;
const float c_dtEnvelope = 10.0f;

class CbuildingEffectIGC : public TmodelIGC<IbuildingEffectIGC>
{
    public:
        CbuildingEffectIGC(void)
        {
        }
        ~CbuildingEffectIGC(void)
        {
        }

    public:
    // IbaseIGC
	    virtual HRESULT Initialize(ImissionIGC* pMission, Time now, const void* data, int dataSize)
        {
            TmodelIGC<IbuildingEffectIGC>::Initialize(pMission, now, data, dataSize);

            {
                ZRetailAssert (data && (dataSize > sizeof(DataBuildingEffectBase)));
                DataBuildingEffectBase*   dataBuilding =  (DataBuildingEffectBase*)data;

                m_positionStart = dataBuilding->positionStart;
                m_positionStop = dataBuilding->positionStop;

                m_radiusAsteroid = dataBuilding->radiusAsteroid;
                m_radiusStation = dataBuilding->radiusStation;

                m_timeEnvelope = pMission->GetIgcSite()->ClientTimeFromServerTime(dataBuilding->timeStart);
                m_timeGrow     = m_timeEnvelope + c_dtEnvelope;
                m_timeOpaque   = m_timeGrow     + c_dtGrow;
                m_timeShrink   = m_timeOpaque   + c_dtOpaque;
                m_timeComplete = m_timeShrink   + c_dtShrink;
            }

            ZAssert (sizeof(DataBuildingEffectIGC) != sizeof(DataBuildingEffectExport));
            IclusterIGC*    pcluster;
            const Color*    pcolor;
            if (dataSize == sizeof(DataBuildingEffectIGC))
            {
                DataBuildingEffectIGC*  dataBuilding = (DataBuildingEffectIGC*)data;

                m_pshipBuilder = dataBuilding->pshipBuilder;
                ZAssert (m_pshipBuilder);
                m_pshipBuilder->SetStateM(drillingMaskIGC | buildingMaskIGC);

                m_pside = m_pshipBuilder->GetSide();
                m_pstationType = (IstationTypeIGC*)(m_pshipBuilder->GetBaseData());

                m_pasteroid = dataBuilding->pasteroid;
                ZAssert (m_pasteroid);
                m_pasteroid->SetBuildingEffect(this);

                pcluster = dataBuilding->pcluster;

                pcolor = &(m_pshipBuilder->GetSide()->GetColor());
            }
            else
            {
                ZRetailAssert (dataSize == sizeof(DataBuildingEffectExport));
                DataBuildingEffectExport*  dataBuilding = (DataBuildingEffectExport*)data;

                pcluster = pMission->GetCluster(dataBuilding->clusterID);

                m_pasteroid = pcluster->GetAsteroid(dataBuilding->asteroidID);
                ZAssert (m_pasteroid);
                m_pasteroid->SetBuildingEffect(this);

                pcolor = &(dataBuilding->color);
            }

            LoadEffect(Color(50.0f/255.0f, 28.0f/255.0f, 146.0f/255.0f), //*pcolor,
                       (c_mtNotPickable | c_mtCastRay | c_mtHitable | c_mtDamagable | c_mtPredictable));

            m_radiusMax = 1.1f * (m_radiusAsteroid > m_radiusStation ? m_radiusAsteroid : m_radiusStation);
            SetRadius(1.0f);
            SetPosition(m_positionStart);

            GetHitTest()->SetNoHit(m_pasteroid->GetHitTest());

            ZAssert (GetVelocity().LengthSquared() == 0.0f);

            ZAssert (pcluster);
            SetCluster(pcluster);

            //Set the BB's effect, since it was probably created as a result of a collision.
            SetBB(now, now, 0.0f);

            return S_OK;
        }
	    virtual void    Terminate(void)
        {
            AddRef();

            if (m_pasteroid)
            {
                m_pasteroid->SetBuildingEffect(NULL);
                m_pasteroid = NULL;
            }

            if (m_pshipBuilder)
            {
                GetMyMission()->GetIgcSite()->KillShipEvent(GetMyLastUpdate(), m_pshipBuilder, NULL,
                                                            0.0f, m_pshipBuilder->GetPosition(), Vector::GetZero());
                m_pshipBuilder = NULL;
            }

            m_pside = NULL;
            m_pstationType = NULL;

	        TmodelIGC<IbuildingEffectIGC>::Terminate();
            Release();
        }

        int        Export(void*  data) const
        {
            if (data)
            {
                DataBuildingEffectExport*   pdbe = (DataBuildingEffectExport*)data;

                pdbe->timeStart = GetMyMission()->GetIgcSite()->ServerTimeFromClientTime(m_timeEnvelope);

                pdbe->asteroidID = m_pasteroid ? m_pasteroid->GetObjectID() : NA;
                pdbe->clusterID = GetCluster()->GetObjectID();

                pdbe->radiusAsteroid = m_radiusAsteroid;
                pdbe->radiusStation = m_radiusStation;

                pdbe->positionStart = m_positionStart;
                pdbe->positionStop = m_positionStop;

                ZAssert (m_pshipBuilder);
                pdbe->color = m_pshipBuilder->GetSide()->GetColor();
            }

            return sizeof(DataBuildingEffectExport);
        }

        virtual void        Update(Time now)
        {
            if (now >= m_timeComplete)
            {
                Terminate();
            }
            else
            {
                Vector  position;
                float   radius;
                float   aInner;
                float   fInner;
                float   fOuter;

                if (now >= m_timeShrink)
                {
                    float   f = (now - m_timeShrink) / c_dtShrink;

                    position = m_positionStop;
                    radius = m_radiusStation * f + m_radiusMax * (1.0f - f);

                    aInner = 1.0f - f;
                    fInner = 1.0f - f;
                    fOuter = 1.0f - f;
                }
                else if (now >= m_timeOpaque)
                {
                    float   f = (now - m_timeOpaque) / c_dtOpaque;

                    position = m_positionStop;
                    radius = m_radiusMax;

                    aInner = 0.5f + 0.5f * f;
                    fInner = 1.0f;
                    fOuter = 1.0f;
                }
                else if (now >= m_timeGrow)
                {
                    float   f = (now - m_timeGrow) / c_dtGrow;

                    position = m_positionStop;
                    radius = m_radiusMax * f + m_radiusAsteroid * (1.0f - f);
                    aInner = 0.5f;
                    fInner = 1.0f;
                    fOuter = 1.0f;
                }
                else
                {
                    float   dt = (now - m_timeEnvelope);
                    float   f = (dt > 0.0f) ? (dt / c_dtEnvelope) : 0.0f;

                    position = m_positionStop * f + m_positionStart * (1.0f - f);
                    radius = f * m_radiusAsteroid + 1.0f - f;

                    aInner = 0.5f;
                    fInner = 1.0f;
                    fOuter = 1.0f;
                }

                SetPosition(position);
                SetRadius(radius);

                ZAssert (GetThingSite());
                GetThingSite()->SetColors(aInner, fInner, fOuter);
            }
        }

        virtual ObjectType  GetObjectType(void) const
        {
            return OT_buildingEffect;
        }

    // ImodelIGC
        virtual void    SetCluster(IclusterIGC* cluster)
        {
            AddRef();

            {
                IclusterIGC*    c = GetCluster();
                if (c)
                    c->DeleteModel(this);
            }

            TmodelIGC<IbuildingEffectIGC>::SetCluster(cluster);

            if (cluster)
                cluster->AddModel(this);

            Release();
        }
        virtual void                 HandleCollision(Time       timeCollision,
                                                     float                  tCollision,
                                                     const CollisionEntry&  entry,
                                                     ImodelIGC* pModel)
        {
            ObjectType  type = pModel->GetObjectType();
            if ((type != OT_buildingEffect) &&
                (type != OT_asteroid) &&
                (type != OT_station) &&
                (type != OT_probe))
            {
                pModel->HandleCollision(timeCollision, tCollision, entry, this);
            }
        }

    // IdamageIGC
        virtual DamageResult        ReceiveDamage(DamageTypeID            type,
                                                  float                   amount,
                                                  Time                    timeCollision,
                                                  const Vector&           position1,
                                                  const Vector&           position2,
                                                  ImodelIGC*              launcher)
        {
            if (m_pasteroid)
            {
                m_pasteroid->ReceiveDamage(type, amount, timeCollision, m_pasteroid->GetPosition(), position2, launcher);
            }

            return c_drNoDamage;
        }
        virtual float   GetFraction(void) const
        {
            return 1.0f;
        }
        virtual void    SetFraction(float newVal)
        {
        }

        virtual float   GetHitPoints(void) const
        {
            return m_pasteroid ? m_pasteroid->GetHitPoints() : 1.0f;
        }

    // IbuildingEffectIGC
        virtual void    BuilderTerminated(void)
        {
            m_pshipBuilder = NULL;
        }

        virtual IasteroidIGC*    GetAsteroid(void) const
        {
            return m_pasteroid;
        }

        virtual void    MakeUnhitable(void)
        {
            ZAssert (m_pasteroid);
            m_pasteroid = NULL;

            TmodelIGC<IbuildingEffectIGC>::MakeUnhitable();        //Virtual wrapper for a non-virtual function
        }

        virtual void                AsteroidUpdate(Time now)
        {
            if ((now >= m_timeShrink) && m_pshipBuilder)
            {
                //transform the asteroid into the station
                ZAssert (m_pstationType);
                ZAssert (m_pside);

                GetMyMission()->GetIgcSite()->SendChatf(m_pshipBuilder, CHAT_TEAM, m_pside->GetObjectID(),
                                                        m_pstationType->GetCompletionSound(), 
                                                        "Finished building %s", m_pstationType->GetName());

				//Imago #120 #121 8/10
				bool bseenside[c_cSidesMax] = {false};
				GetMyMission()->GetSeenSides(GetMyMission()->GetModel(m_pshipBuilder->GetObjectType(),m_pshipBuilder->GetObjectID()),bseenside,GetMyMission()->GetModel(m_pasteroid->GetObjectType(),m_pasteroid->GetObjectID()));
								
                //Quietly kill the ship (after nuking its parts to prevent treasure from being created)
                {
                    const PartListIGC*  parts = m_pshipBuilder->GetParts();
                    PartLinkIGC*    plink;
                    while (plink = parts->first())  //Not ==
                        plink->data()->Terminate();
                }
                m_pshipBuilder->SetAmmo(0);
                m_pshipBuilder->SetFuel(0.0f);

                m_pshipBuilder->SetStateM(0);

                GetMyMission()->GetIgcSite()->KillShipEvent(now, m_pshipBuilder, NULL, 0.0f, m_pshipBuilder->GetPosition(), Vector::GetZero());

                m_pshipBuilder = NULL;

                IasteroidIGC*   pasteroid = m_pasteroid;
                m_pasteroid->SetBuildingEffect(NULL);     //Clear the building effect so it isn't nuked along with the asteroid
                ZAssert (m_pasteroid == NULL);

                GetMyMission()->GetIgcSite()->BuildStation(pasteroid,
                                                           m_pside,
                                                           m_pstationType,
                                                           now, bseenside); //Imago #121 - Selective pop rocks
            }
        }

    private:
        TRef<IshipIGC>           m_pshipBuilder;
        TRef<IasteroidIGC>       m_pasteroid;
        TRef<IstationTypeIGC>    m_pstationType;
        TRef<IsideIGC>           m_pside;
        Time                     m_timeComplete;
        Time                     m_timeShrink;
        Time                     m_timeOpaque;
        Time                     m_timeGrow;
        Time                     m_timeEnvelope;

        Vector                   m_positionStart;
        Vector                   m_positionStop;
        float                    m_radiusAsteroid;
        float                    m_radiusStation;
        float                    m_radiusMax;
};

#endif
