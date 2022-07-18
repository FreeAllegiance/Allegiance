/*
**  Copyright (C) 1996, 1997 Microsoft Corporation. All Rights Reserved.
**
**  File:	clusterIGC.cpp
**
**  Author: 
**
**  Description:
**      Implementation of the CclusterIGC class. This file was initially created by
**  the ATL wizard for the core object.
**
**      Clusters are distinct places in the trek universe: nothing happening in one
**  cluster can directly affect things in another cluster. Clusters (in addition to
**  the ships, missiles, etc.) have a collection of static objects that represent things
**  like planets.
**
**  History:
*/
// clusterIGC.cpp : Implementation of CclusterIGC
#include "clusterIGC.h"
#include "modelIGC.h"
#include <stdlib.h>
#include <math.h>

/////////////////////////////////////////////////////////////////////////////
// CclusterIGC
HRESULT CclusterIGC::Initialize(ImissionIGC* pMission, Time   now, const void* data, int dataSize)
{
    ZAssert (pMission);
    m_pMission = pMission;


    ZRetailAssert (data && (dataSize == sizeof(m_data)));
    m_data = *((DataClusterIGC*)data);

    m_nPass = m_data.clusterID;
    m_lastUpdate = now;

    pMission->AddCluster(this);

    m_pClusterSite = pMission->GetIgcSite()->CreateClusterSite(this);
    if (m_data.posterName[0] != '\0')
        m_pClusterSite->SetEnvironmentGeo(m_data.posterName);

    if (m_data.planetName[0] != '\0')
    {
        Vector  position;

        double  sinLatitude = double(m_data.planetSinLatitude - 0.5);
        double  cosLatitude = sqrt(1.0 - sinLatitude * sinLatitude);
        double  longitude = double(m_data.planetLongitude * (2.0f * pi));
        position.x = float(cos(longitude) * cosLatitude);
        position.y = float(sin(longitude) * cosLatitude);
        position.z = float(sinLatitude);
        m_pClusterSite->AddPoster(m_data.planetName,
                                  position,
                                  float(m_data.planetRadius));
    }

	m_highlight = false;  //Xynth #208

    return S_OK;
}

void        CclusterIGC::Terminate(void)
{
    {
        //Models remove themselves from the list when terminated
        ModelLinkIGC*    l;
        while (l = m_models.first())  //Not ==
        {
            l->data()->Terminate();
        }
    }

    m_kdrStatic.flush();
    m_kdrMoving.flush();

    ZAssert (m_modelsPickable.n() == 0);
    ZAssert (m_modelsCastRay.n() == 0);

    ZAssert (m_stations.n() == 0);
    ZAssert (m_models.n() == 0);
    ZAssert (m_probes.n() == 0);
    ZAssert (m_warps.n() == 0);
    ZAssert (m_treasures.n() == 0);
    ZAssert (m_asteroids.n() == 0);
    ZAssert (m_mines.n() == 0);

    ZAssert (m_pClusterSite);
    m_pClusterSite->Terminate();
    m_pClusterSite = NULL;

    m_pMission->DeleteCluster(this);
}

void        CclusterIGC::Update(Time now)
{
    if (now > m_lastUpdate)
    {
        float   dt = now - m_lastUpdate;

        bool    bStarted = m_pMission->GetMissionStage() == STAGE_STARTED;
        if (bStarted)
        {
            {
                //Have any stations launch docked drones
                for (StationLinkIGC*   l = m_stations.first();
                     (l != NULL);
                     l = l->next())
                {
                    IstationIGC*    pstation = l->data();

                    ShipLinkIGC*    pslNext;
                    for (ShipLinkIGC*   psl = pstation->GetShips()->first();
                         (psl != NULL);
                         psl = pslNext)
                    {
                        IshipIGC*   pship = psl->data();
                        pslNext = psl->next();             //Get the next link now since the ship may launch
                        // const MissionParams* pmp = m_pMission->GetMissionParams(); 04/08 commented out as not needed // mmf 10/07 added so we can get at bExperimental game type
                        if (pship->GetAutopilot() && (pship->GetPilotType() < c_ptPlayer))
                        {
                            //Docked non-players on autopilot never are observers/parents
                            ZAssert (pship->GetParentShip() == NULL);
                            ZAssert (pship->GetChildShips()->n() == 0);

							// mmf/yp 10/07 added this so drones launch when ordered to even if OkToLaunch might be false
							// intentionally left c_cidMine out of the list otherwise miners would launch with their AI
							// 'order' to mine
	
							if (pship->OkToLaunch(now) || (pship->GetCommandID(c_cmdAccepted) == c_cidGoto) ||
							   (pship->GetCommandID(c_cmdAccepted) == c_cidBuild) )
                                pship->SetStation(NULL);
							// if (pship->OkToLaunch(now))  // mmf orig code
							//	  pship->SetStation(NULL);
							
                        }
                    }
					
					//Are any ships buzzing around the stations that a side has yet to eye? #121 #120 Imago 8/10
					if (GetShips()->n() > 0 && pstation->GetRoidID() != NA) {
						Vector pos = pstation->GetRoidPos();
						float Sig = pstation->GetRoidSig();
						float Radius = pstation->GetRoidRadius();
						if (Sig != 0.0f && Radius != 0.0f) {
							//check if they have a ship eying where the rock would be
							for (ShipLinkIGC*   psl1 = GetShips()->first(); (psl1 != NULL); psl1 = psl1->next()) {
								IshipIGC*   pship = psl1->data();
								if (pship->GetParentShip() || pship->GetSide()->GetObjectID() == pstation->GetSide()->GetObjectID() || pstation->SeenBySide(pship->GetSide()) || !pstation->GetRoidSide(pship->GetSide()->GetObjectID()))
									continue;
								bool bEye = bSimpleEye(pship->GetHullType()->GetScannerRange(),GetMission()->GetModel(OT_ship,pship->GetObjectID()),Sig,pstation->GetSide()->GetGlobalAttributeSet().GetAttribute(c_gaSignature),Radius,pos);
								if (bEye) {
									pstation->SetRoidSide(pship->GetSide()->GetObjectID(),false);
									GetMission()->GetIgcSite()->KillAsteroidEvent(pstation->GetRoidID(),GetObjectID(),pship->GetSide());
								}
							}
						}
					}
                }
            }
            {
                m_fCost = m_pMission->GetFloatConstant(c_fcidBaseClusterCost);

                float   costLifepod = m_pMission->GetFloatConstant(c_fcidLifepodCost);
                float   costTurret = m_pMission->GetFloatConstant(c_fcidTurretCost);
                float   costPlayer = m_pMission->GetFloatConstant(c_fcidPlayerCost);
                float   costDrone = m_pMission->GetFloatConstant(c_fcidDroneCost);

                //Have miners and builders do any pre-plotted moves. Allow ships to suicide.
                ShipLinkIGC*        lNext;
                for (ShipLinkIGC*   l = m_ships.first();
                     (l != NULL);
                     l = lNext)
                {
                    IshipIGC*   s = l->data();
                    lNext = l->next();

                    if (s->GetPilotType() < c_ptPlayer)
                        m_fCost += costDrone;
                    else if (s->GetParentShip() != NULL)
                        m_fCost += costTurret;
                    else
                    {
                        IhullTypeIGC*   pht = s->GetBaseHullType();
                        ZAssert (pht);
                        m_fCost += pht->HasCapability(c_habmLifepod)
                                  ? costLifepod
                                  : costPlayer;
                    }

                    s->PreplotShipMove(now);
                }

                if (m_fCost > 0.0f)
                {
                    m_fCost *= dt / m_pMission->GetFloatConstant(c_fcidClusterDivisor);
                }

                {
                    //Have all ships on autopilot plot their moves. Allow ships to suicide.
                    ShipLinkIGC*        lNext;
                    for (ShipLinkIGC*   l = m_ships.first();
                         (l != NULL);
                         l = lNext)
                    {
                        IshipIGC*   s = l->data();
                        lNext = l->next();

                        s->PlotShipMove(now);
                    }
                }
            }

            {
                //Have all ships execute their moves
                for (ShipLinkIGC*   l = m_ships.first();
                     (l != NULL);
                     l = l->next())
                {
                    IshipIGC*   s = l->data();

                    if (s->GetParentShip() == NULL)
                    {
                        s->ExecuteShipMove(now);
                    }
                }
            }
        }
        else
            m_fCost = 0.0f;

        {
            //Call the update method on all the contained models
            //models might self-terminate in the update and nuke earlier models in the update loop

            //NYI debugging variables
            //ObjectType  oldObjectType = NA;
            //ObjectType  newObjectType = NA;

            ModelLinkIGC*       lNext;
            for (ModelLinkIGC*     l = m_models.first();
                 (l != NULL);
                 l = lNext)
            {
                //oldObjectType = newObjectType;
                //newObjectType = l->data()->GetObjectType();

                lNext = l->next();

                l->data()->Update(now);
            }
        }

        if (m_data.activeF && bStarted)
        {
            {
                //Update the bounding boxes for all moving objects & projectiles
                for (ModelLinkIGC*     l = m_models.first();
                     (l != NULL);
                     l = l->next())
                {
                    l->data()->SetBB(m_lastUpdate, now, dt);
                }

                m_tMax = dt;
            }

            m_kdrStatic.update();
            m_kdrMoving.update();

            {
                //Cast rays through the KD tree for each object
                for (ModelLinkIGC*     l = m_modelsCastRay.first();
                     (l != NULL);
                     l = l->next())
                {
                    ImodelIGC*  m = l->data();

                    HitTest*    ht = m->GetHitTest();

                    if (!ht->GetDeadF())
                    {
                        m_kdrStatic.test(ht, &m_collisions);
                        m_kdrMoving.test(ht, &m_collisions);
                    }
                }
            }

            //Sort the collisions by the time they occur
            m_collisions.sort(0);

            //Process each collision (in order)
            {
                m_tOffset = 0.0f;
                for (m_collisionID = 0; (m_collisionID < m_collisions.n()); m_collisionID++)
                {
                    const CollisionEntry& entry = m_collisions[m_collisionID];

                    if (!(entry.m_pHitTest1->GetDeadF() || entry.m_pHitTest2->GetDeadF()))
                    {
                        Time    timeCollision = m_lastUpdate + (m_tOffset + entry.m_tCollision);

                        ImodelIGC*  pModelHitTest1 = (ImodelIGC*)(entry.m_pHitTest1->GetData());
                        ZAssert (pModelHitTest1);

                        ImodelIGC*  pModelHitTest2 = (ImodelIGC*)(entry.m_pHitTest2->GetData());
                        ZAssert (pModelHitTest2);

                        //Give each participant in the collision a chance to handle the collision
                        //but give the "1st" model first dibs.
                        if ((pModelHitTest1->GetCluster() == this) &&
                            (pModelHitTest2->GetCluster() == this))
                        {
                            pModelHitTest1->HandleCollision(timeCollision, entry.m_tCollision, entry, pModelHitTest2);
                        }
                    }
                }

                m_collisions.purge();
            }
            {
                //Apply any damage from mines
                //Kids always follow parents in the ship list, so go from back to front
                //so that the killing a parent doesn't mean hitting dead elements in the list
                ShipLinkIGC*        lTxen;
                for (ShipLinkIGC*   l = m_ships.last();
                     (l != NULL);
                     l = lTxen)
                {
                    IshipIGC*   s = l->data();
                    lTxen = l->txen();

                    s->ApplyMineDamage();
                }
            }

            //Move each object & projectile
            {
                for (ModelLinkIGC*  l = m_models.first();
                     (l != NULL);
                     l = l->next())
                {
                    l->data()->Move();
                }
            }

            if ((m_nPass++) % c_nPassesPerUpdate == 0)
            {
                for (ModelLinkIGC*  l = m_models.first();
                     (l != NULL);
                     l = l->next())
                {
                    l->data()->UpdateSeenBySide();
                }

                m_pMission->GetIgcSite()->ClusterUpdateEvent(this);
            }
        }


        //Draw and resolve any explosions
        if (m_nExplosions != 0)
        {
            const int c_maxDmgs = 500;
            IdamageIGC* pdmgs[c_maxDmgs];
            int         nDmgs = 0;

            //Copy the list of models in the sector that can be damaged into
            for (ModelLinkIGC*  l = m_modelsPickable.first();
                 (l != NULL);
                 l = l->next())
            {
                ImodelIGC*  pmodel = l->data();
                ObjectType  type = pmodel->GetObjectType();

                //Not everything that can take damage can be affected by an explosion.
                if ((type == OT_ship) || (type == OT_asteroid) ||
                    (type == OT_station) || (type == OT_missile) || (type == OT_probe))
                {
                    pmodel->AddRef();
                    pdmgs[nDmgs++] = (IdamageIGC*)pmodel;

                    if (nDmgs == c_maxDmgs)
                        break;
                }
            }

            ImineIGC*   pmines[c_maxDmgs];
            int         nMines = 0;
            {
                for (MineLinkIGC*   l = m_mines.first(); (l != NULL); l = l->next())
                {
                    ImineIGC*   pm = l->data();
                    pm->AddRef();
                    pmines[nMines++] = pm;

                    if (nMines == c_maxDmgs)
                        break;
                }
            }

            int i = 0;
            do
            {
                ExplosionData&  e = m_explosions[i];
                m_pClusterSite->AddExplosion(e.position, e.radius, e.explosionType);

                float   dt = (e.time - m_lastUpdate) - m_tOffset;

                //Now, the painful part: applying damage to everything in the sector that could be hit
                {
                    for (int j = 0; (j < nDmgs); j++)
                    {
                        IdamageIGC*  pTarget = pdmgs[j];
                        if (pTarget->GetCluster() == this)      //Make sure it wasn't already destroyed
                        {
                            //The target is still around
                            Vector  p = pTarget->GetPosition() + dt * pTarget->GetVelocity();
                            float   d = (e.position - p).Length() - pTarget->GetRadius();

                            if (d < e.radius)
                            {
                                float   amount = e.amount;
                                if (d > 0.0f)
                                {
                                    float   f = 1.0f - (d / e.radius);
                                    amount *= f * f;
                                }

                                pTarget->ReceiveDamage(e.damageType | c_dmgidNoWarn | c_dmgidNoDebris,
                                                       amount,
                                                       e.time,
                                                       p, e.position,
                                                       e.launcher);
                            }
                        }
                    }
                }
                {
                    for (int j = 0; (j < nMines); j++)
                    {
                        ImineIGC*  pTarget = pmines[j];
                        if (pTarget->GetCluster() == this)      //Make sure it wasn't already destroyed
                        {
                            //The target is still around
                            const Vector&   p = pTarget->GetPosition();
                            float           d = (e.position - p).Length() - pTarget->GetRadius();

                            if (d < e.radius)
                            {
                                float   amount = e.amount;
                                if (d > 0.0f)
                                {
                                    float   f = 1.0f - (d / e.radius);
                                    amount *= f * f;
                                }
                                pTarget->ReduceStrength(amount);
                            }
                        }
                    }
                }

                if (e.launcher)
                    e.launcher->Release();
            }
            while (++i < m_nExplosions);

            //Release all the cached pointers
            {
                while (--nDmgs >= 0)
                    pdmgs[nDmgs]->Release();
            }

            {
                while (--nMines >= 0)
                    pmines[nMines]->Release();
            }

            m_nExplosions = 0;
        }

        m_lastUpdate = now;
    }
}

int        CclusterIGC::Export(void*  data) const
{
    if (data)
    {
        *((DataClusterIGC*)data) = m_data;

        //Selectively lie here: exported clusters are always inactive by default.
        ((DataClusterIGC*)data)->activeF = false;
    }

    return sizeof(m_data);
}

void        CclusterIGC::AddModel(ImodelIGC* modelNew)
{
    ZAssert (modelNew);

    ZVerify(m_models.first(modelNew));
    modelNew->AddRef();

    m_pClusterSite->AddThingSite(modelNew->GetThingSite());

    {
        //Add the model to the collision set
        HitTest*    ht = modelNew->GetHitTest();
        ModelAttributes mt = modelNew->GetAttributes();

        if ((mt & c_mtNotPickable) == 0)
        {
            ZVerify(m_modelsPickable.last(modelNew));
            modelNew->AddRef();
        }   

        if (mt & c_mtCastRay)
        {
            ZVerify(m_modelsCastRay.last(modelNew));
            modelNew->AddRef();
        }   

        if (mt & c_mtHitable)
        {
            ((mt & c_mtStatic)
             ? m_kdrStatic
             : m_kdrMoving).addHitTest(ht);
        }
        else
            ht->SetDeadF(false);

        ZRetailAssert(!ht->GetDeadF());
    }
}

void        CclusterIGC::DeleteModel(ImodelIGC* modelOld)
{
    ZAssert (modelOld);

    {
        //Add the model to the collision set
        HitTest*        ht = modelOld->GetHitTest();
        ModelAttributes mt = modelOld->GetAttributes();

        if ((mt & c_mtNotPickable) == 0)
        {
            DeleteIbaseIGC((BaseListIGC*)&m_modelsPickable, modelOld);
        }

        if (mt & c_mtCastRay)
        {
            DeleteIbaseIGC((BaseListIGC*)&m_modelsCastRay, modelOld);
        }

        if (mt & c_mtHitable)
        {
            ((mt & c_mtStatic)
             ? m_kdrStatic
             : m_kdrMoving).deleteHitTest(ht);
        }
        else
            ht->SetDeadF(true);

        ZRetailAssert(ht->GetDeadF());
    }

    m_pClusterSite->DeleteThingSite(modelOld->GetThingSite());

    DeleteIbaseIGC((BaseListIGC*)&m_models, modelOld);
}

ImodelIGC*  CclusterIGC::GetModel(const char* name) const
{
    ZAssert (name);
    for (ModelLinkIGC*     l = m_models.first();
         (l != NULL);
         l = l->next())
    {
        ImodelIGC*  m = l->data();
        if (_stricmp(m->GetName(), name) == 0)
        {
            return m;
        }
    }

    return NULL;
}

void        CclusterIGC::RecalculateCollisions(float        tOffset,
                                               ImodelIGC*   pModel1,
                                               ImodelIGC*   pModel2)
{
    //Update the stop positions for the hit tests (& update their bounding boxes)
    ZAssert ((pModel1->GetAttributes() & c_mtStatic) == 0);
    HitTest*    pHitTest1 = pModel1->GetHitTest();
    if (pHitTest1)
    {
        if (pHitTest1->GetDeadF())
            pHitTest1 = NULL;
        else
            pHitTest1->SetStopPosition();
    }

    HitTest*    pHitTest2;
    if ((pModel2 == NULL) || (pModel2->GetAttributes() & c_mtStatic))
        pHitTest2 = NULL;
    else
    {
        pHitTest2 = pModel2->GetHitTest();
        if (pHitTest2)
        {
            if (pHitTest2->GetDeadF())
                pHitTest2 = NULL;
            else
                pHitTest2->SetStopPosition();
        }
    }

    if (pHitTest1 || pHitTest2)
    {
        m_tOffset += tOffset;

        //Don't bother recalculating collisions if we are already outside the window in which collisions can occur.
        if (m_tOffset <= m_tMax)
        {
            {
                //Move all objects to their positions at the time of the collision
                for (ModelLinkIGC*  l = m_models.first();
                     (l != NULL);
                     l = l->next())
                {
                    ImodelIGC*  m = l->data();

                    HitTest*    pht = m->GetHitTest();

                    //Move the models not involved in the collision
                    if ((m != pModel1) && (m != pModel2))
                        m->Move(tOffset);
                    else
                        pht->AdjustTimes(tOffset);

                    //Update the bounding boxes for everything
                    pht->UpdateBB();
                }
            }

            //Remove any collisions that involved either object involved in this collision
            m_collisions.flush(m_collisionID + 1, pHitTest1, pHitTest2);

            {
                //Check for any collisions between either ship and the rest of the stuff in the cluster
                for (ModelLinkIGC*     l = m_models.first();
                     (l != NULL);
                     l = l->next())
                {
                    ImodelIGC*  m = l->data();

                    if ((m != pModel1) && (m != pModel2) && (m->GetAttributes() & (c_mtCastRay | c_mtHitable)))
                    {
                        HitTest*    ht = m->GetHitTest();

                        if (!ht->GetDeadF())
                        {
                            if (ht->GetID() != c_htidStaticObject)
                            {
                                //ht is not a static object, so it can always go first (which also handles
                                //the problem that projectiles (which ht might be) go before ships).

                                //prevent collisions between projectiles and their launcher.
                                if (pHitTest1 &&
                                    (ht->GetNoHit() != pHitTest1) &&
                                    (pHitTest1->GetNoHit() != ht))
                                    ht->Collide(pHitTest1, &m_collisions);

                                if (pHitTest2 &&
                                    (ht->GetNoHit() != pHitTest2) &&
                                    (pHitTest2->GetNoHit() != ht))
                                    ht->Collide(pHitTest2, &m_collisions);
                            }
                            else
                            {
                                //in collisions between static objects and non-static objects, the
                                //non-static object is always first
                                if (pHitTest1 &&
                                    (ht->GetNoHit() != pHitTest1) &&
                                    (pHitTest1->GetNoHit() != ht))
                                    pHitTest1->Collide(ht, &m_collisions);

                                if (pHitTest2 &&
                                    (ht->GetNoHit() != pHitTest2) &&
                                    (pHitTest2->GetNoHit() != ht))
                                    pHitTest2->Collide(ht, &m_collisions);
                            }
                        }
                    }
                }
            }

            //Resort the collisions of the yet to be handled collisions
            m_collisions.sort(m_collisionID + 1);
        }
    }
}
IbuildingEffectIGC*      CclusterIGC::CreateBuildingEffect(Time           now,
                                                          IasteroidIGC*  pasteroid,
                                                          IstationIGC*   pstation,
                                                          IshipIGC*      pshipBuilder,
                                                          float          radiusAsteroid,
                                                          float          radiusStation,
                                                          const Vector&  positionStart,
                                                          const Vector&  positionStop)
{
    DataBuildingEffectIGC   dbe;

    dbe.timeStart = now;

    dbe.pasteroid = pasteroid;
    dbe.pstation = pstation;
    dbe.pcluster = this;
    dbe.pshipBuilder = pshipBuilder;
    dbe.radiusAsteroid = radiusAsteroid;
    dbe.radiusStation = radiusStation;

    dbe.positionStart = positionStart;
    dbe.positionStop  = positionStop;

    IbuildingEffectIGC*     pbe = (IbuildingEffectIGC*)(m_pMission->CreateObject(now, OT_buildingEffect, &dbe, sizeof(dbe)));
    ZAssert (pbe);
    pbe->Release();

    return pbe;
}

