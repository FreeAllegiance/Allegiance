/*
**  Copyright (C) 1996, 1997 Microsoft Corporation. All Rights Reserved.
**
**  File:    clusterIGC.h
**
**  Author: 
**
**  Description:
**      Header for the CclusterIGC class. This file was initially created by
**  the ATL wizard.
**
**  History:
*/
// clusterIGC.h : Declaration of the CclusterIGC

#ifndef __CLUSTERIGC_H_
#define __CLUSTERIGC_H_

#include "igc.h"

const int   c_maxExplosions = 20;
const int   c_nPassesPerUpdate = 10;

/////////////////////////////////////////////////////////////////////////////
// CclusterIGC
class       CclusterIGC : public IclusterIGC
{
    public:
        CclusterIGC(void)
        :
            m_pClusterSite(NULL),
            m_nExplosions(0),
            m_kdrStatic(true),
            m_kdrMoving(false),
            m_collisionID(0),
            m_fPendingTreasures(0.0f)
        {
        }

    // IbaseIGC
        virtual HRESULT                 Initialize(ImissionIGC* pMission, Time now, const void* data, int length);
        virtual void                    Terminate(void);
        virtual void                    Update(Time now);

        virtual int                     Export(void* data) const;
        virtual ObjectType              GetObjectType(void) const
        {
            return OT_cluster;
        }
        virtual ObjectID                GetObjectID(void) const
        {
            return m_data.clusterID;
        }

        virtual ImissionIGC*    GetMission(void) const
        {
            return m_pMission;
        }

    // IclusterIGC
        virtual const char*             GetName(void) const
        {
            return m_data.name;
        }

        virtual Time                    GetLastUpdate(void) const
        {
            return m_lastUpdate;
        }

        virtual void                    SetLastUpdate(Time now)
        {
            m_lastUpdate = now;
        }

        virtual void                    AddModel(ImodelIGC* modelNew);
        virtual void                    DeleteModel(ImodelIGC* modelOld);

        virtual ImodelIGC*              GetModel(const char* name) const;
        virtual const ModelListIGC*     GetModels(void) const
        {
            return &m_models;
        }
        virtual const ModelListIGC*     GetPickableModels(void) const
        {
            return &m_modelsPickable;
        }

        virtual void                    AddTreasure(ItreasureIGC* treasureNew)
        {
            AddIbaseIGC((BaseListIGC*)&m_treasures, treasureNew);
            AddModel(treasureNew);
        }
        virtual void                    DeleteTreasure(ItreasureIGC* treasureOld)
        {
            DeleteModel(treasureOld);
            DeleteIbaseIGC((BaseListIGC*)&m_treasures, treasureOld);
        }
        virtual ItreasureIGC*           GetTreasure(TreasureID  id) const
        {
            return (ItreasureIGC*)GetIbaseIGC((BaseListIGC*)&m_treasures, id);
        }
        virtual const TreasureListIGC*  GetTreasures(void) const
        {
            return &m_treasures;
        }

        virtual void                    AddWarp(IwarpIGC* warpNew)
        {
            AddIbaseIGC((BaseListIGC*)&m_warps, warpNew);
            AddModel(warpNew);
        }
        virtual void                    DeleteWarp(IwarpIGC* warpOld)
        {
            DeleteModel(warpOld);
            DeleteIbaseIGC((BaseListIGC*)&m_warps, warpOld);
        }
        virtual const WarpListIGC*      GetWarps(void) const
        {
            return &m_warps;
        }

        virtual void                    AddStation(IstationIGC* stationNew)
        {
            AddIbaseIGC((BaseListIGC*)&m_stations, stationNew);
            m_pClusterSite->AddScanner(stationNew->GetSide()->GetObjectID(), stationNew);
            AddModel(stationNew);
        }
        virtual void                    DeleteStation(IstationIGC* stationOld)
        {
            DeleteModel(stationOld);
            m_pClusterSite->DeleteScanner(stationOld->GetSide()->GetObjectID(), stationOld);
            DeleteIbaseIGC((BaseListIGC*)&m_stations, stationOld);
        }
        virtual IstationIGC*            GetStation(StationID   id) const
        {
            return (IstationIGC*)GetIbaseIGC((BaseListIGC*)&m_stations, id);
        }
        virtual const StationListIGC*   GetStations(void) const
        {
            return &m_stations;
        }

        virtual void                    AddShip(IshipIGC* shipNew)
        {
            AddIbaseIGC((BaseListIGC*)&m_ships, shipNew);
            if (shipNew->GetParentShip() == NULL)
                m_pClusterSite->AddScanner(shipNew->GetSide()->GetObjectID(), shipNew);
            AddModel(shipNew);

			//debugf("ShipCount cluster: %s, adding ship %s, new ship count: %ld\n", this->GetName(), shipNew->GetName(), m_ships.n());
        }
        virtual void                    DeleteShip(IshipIGC* shipOld)
        {
            ZAssert (shipOld);
            DeleteModel(shipOld);
            if (shipOld->GetParentShip() == NULL)
                m_pClusterSite->DeleteScanner(shipOld->GetSide()->GetObjectID(), shipOld);
            DeleteIbaseIGC((BaseListIGC*)&m_ships, shipOld);

			//debugf("cluster: %s, deleting ship %s, updated ship count: \n", this->GetName(), shipOld->GetName(), m_ships.n());
        }
        virtual IshipIGC*               GetShip(ShipID   id) const
        {
            return (IshipIGC*)GetIbaseIGC((BaseListIGC*)&m_ships, id);
        }
        virtual const ShipListIGC*      GetShips(void) const
        {
			//debugf("cluster: %s, GetShips Count: %ld\n", this->GetName(), m_ships.n());

            return &m_ships;
        }

        virtual void                    AddMine(ImineIGC* mineNew)
        {
            AddIbaseIGC((BaseListIGC*)&m_mines, mineNew);
            AddModel(mineNew);
        }
        virtual void                    DeleteMine(ImineIGC* mineOld)
        {
            ZAssert (mineOld);
            DeleteModel(mineOld);

            DeleteIbaseIGC((BaseListIGC*)&m_mines, mineOld);
        }
        virtual ImineIGC*               GetMine(MineID   id) const
        {
            return (ImineIGC*)GetIbaseIGC((BaseListIGC*)&m_mines, id);
        }
        virtual const MineListIGC*      GetMines(void) const
        {
            return &m_mines;
        }

        virtual void                    AddMissile(ImissileIGC* missileNew)
        {
            AddIbaseIGC((BaseListIGC*)&m_missiles, missileNew);
            AddModel(missileNew);
        }
        virtual void                    DeleteMissile(ImissileIGC* missileOld)
        {
            ZAssert (missileOld);
            DeleteModel(missileOld);

            DeleteIbaseIGC((BaseListIGC*)&m_missiles, missileOld);
        }
        virtual ImissileIGC*               GetMissile(MissileID   id) const
        {
            return (ImissileIGC*)GetIbaseIGC((BaseListIGC*)&m_missiles, id);
        }
        virtual const MissileListIGC*      GetMissiles(void) const
        {
            return &m_missiles;
        }

        virtual void                    AddProbe(IprobeIGC* probeNew)
        {
            AddIbaseIGC((BaseListIGC*)&m_probes, probeNew);
            m_pClusterSite->AddScanner(probeNew->GetSide()->GetObjectID(), probeNew);
            AddModel(probeNew);
        }
        virtual void                    DeleteProbe(IprobeIGC* probeOld)
        {
            ZAssert (probeOld);
            DeleteModel(probeOld);
            m_pClusterSite->DeleteScanner(probeOld->GetSide()->GetObjectID(), probeOld);
            DeleteIbaseIGC((BaseListIGC*)&m_probes, probeOld);
        }
        virtual IprobeIGC*               GetProbe(ProbeID   id) const
        {
            return (IprobeIGC*)GetIbaseIGC((BaseListIGC*)&m_probes, id);
        }
        virtual const ProbeListIGC*      GetProbes(void) const
        {
            return &m_probes;
        }

        virtual void                    AddAsteroid(IasteroidIGC* asteroidNew)
        {
            AddIbaseIGC((BaseListIGC*)&m_asteroids, asteroidNew);

            AddModel(asteroidNew);
        }

        virtual void                    DeleteAsteroid(IasteroidIGC* asteroidOld)
        {
            DeleteModel(asteroidOld);
            DeleteIbaseIGC((BaseListIGC*)&m_asteroids, asteroidOld);
        }
        virtual IasteroidIGC*           GetAsteroid(AsteroidID   id) const
        {
            return (IasteroidIGC*)GetIbaseIGC((BaseListIGC*)&m_asteroids, id);
        }
        virtual const AsteroidListIGC*  GetAsteroids(void) const
        {
            return &m_asteroids;
        }
		//Xynth #132 New function to get known He3 in a cluster for a side
		virtual float                   GetHeliumSeen(IsideIGC* pside)
		{
			float fOre = 0.0;
			for (AsteroidLinkIGC* asteriodLink = this->GetAsteroids()->first();
            asteriodLink != NULL; asteriodLink = asteriodLink->next())
			{
				AsteroidAbilityBitMask aabm = asteriodLink->data()->GetCapabilities();

				// if we can mine helium at this asteroid
				if ((aabm & c_aabmMineHe3) != 0)
				{
					// count it.								
					fOre += asteriodLink->data()->GetOreSeenBySide(pside);
				}
			}

			return fOre;
		}

        virtual ClusterSite*            GetClusterSite(void) const
        {
            return m_pClusterSite;
        }
        virtual IbuildingEffectIGC*     CreateBuildingEffect(Time           now,
                                                             IasteroidIGC*  pasteroid,
                                                             IstationIGC*   pstation,
                                                             IshipIGC*      pshipBuilder,
                                                             float          radiusAsteroid,
                                                             float          radiusStation,
                                                             const Vector&  positionStart,
                                                             const Vector&  positionStop);

        virtual short                   GetNdebris(void) const
        {
            return m_data.nDebris;
        }

        virtual short                   GetNstars(void) const
        {
            return m_data.nStars;
        }

        virtual unsigned int            GetStarSeed(void) const
        {
            return m_data.starSeed;
        }

        virtual const Vector&           GetLightDirection(void) const
        {
            return m_data.lightDirection;
        }

        virtual Color                GetLightColor(void) const
        {
            return
                Color(
                    ((float)RGB_GETRED(  m_data.lightColor)) / 255.0f,
                    ((float)RGB_GETGREEN(m_data.lightColor)) / 255.0f,
                    ((float)RGB_GETBLUE( m_data.lightColor)) / 255.0f
                );
        }

        virtual Color                GetLightColorAlt(void) const
        {
            return Color(0, 0.5f, 1.0f);
        }

        virtual float                GetAmbientLevel(void) const
        {
            return 0.5f;
        }

        virtual float                GetBidirectionalAmbientLevel(void) const
        {
            return 0.25f;
        }

        virtual void                    RecalculateCollisions(float        tOffset,
                                                              ImodelIGC*   pModel1,
                                                              ImodelIGC*   pModel2);

        virtual ExplosionData*          CreateExplosion(DamageTypeID   damageType,
                                                        float          amount,
                                                        float          radius,
                                                        int            explosionType,
                                                        Time           time,
                                                        const Vector&  position,
                                                        ImodelIGC*     launcher)
        {
            if (m_nExplosions == c_maxExplosions)
                return NULL;
            else
            {
                ExplosionData&  e = m_explosions[m_nExplosions++];

                e.damageType    = damageType;
                e.amount        = amount;
                e.radius        = radius;
                e.explosionType = explosionType;
                e.time          = time;
                e.position      = position;
                e.launcher      = launcher;

                if (launcher)
                    launcher->AddRef();

                return &e;
            }
        }

        virtual float                   GetScreenX(void) const
        {
            return m_data.screenX;
        }

        virtual float                   GetScreenY(void) const
        {
            return m_data.screenY;
        }

        virtual void                    SetActive(bool bActive)
        {
            m_data.activeF = bActive;
        }

        virtual const char*             GetPosterName(void) const
        {
            return m_data.posterName;
        }

        virtual void                    SetPrivateData(DWORD dwPrivate)
        {
            m_dwPrivate = dwPrivate;
        }

        virtual DWORD                   GetPrivateData(void) const
        {
            return m_dwPrivate;
        }

        virtual void                    FlushCollisionQueue(void)
        {
            if (m_collisionID < m_collisions.n())
            {
                //Called from inside the collision detection loop
                //so this object might just be switching clusters
                //(which would add it to another cluster). This probably
                //isn't the case ... but let's be safe
                m_collisions.flush(m_collisionID + 1, NULL, NULL);
            }
        }

        virtual void                    MakeModelStatic(ImodelIGC* pmodel)
        {
            ZAssert (pmodel);

            HitTest*    ht = pmodel->GetHitTest();
            ZAssert (ht);

            ZAssert (pmodel->GetCluster() == this);
            ZAssert (pmodel->GetAttributes() & c_mtHitable);
            ZAssert (pmodel->GetAttributes() & c_mtStatic);
            ZAssert ((pmodel->GetAttributes() & c_mtCastRay) == 0);

            DeleteIbaseIGC((BaseListIGC*)&m_modelsCastRay, pmodel);

            m_kdrStatic.addHitTest(ht);
        }

        virtual void                    MakeModelUnhitable(ImodelIGC* pmodel)
        {
            ZAssert (pmodel);

            HitTest*    ht = pmodel->GetHitTest();
            ZAssert (ht);
            m_kdrMoving.deleteHitTest(ht);

            ZAssert (pmodel->GetCluster() == this);
            ZAssert ((pmodel->GetAttributes() & c_mtHitable) == 0);
            ZAssert ((pmodel->GetAttributes() & c_mtCastRay) == 0);

            DeleteIbaseIGC((BaseListIGC*)&m_modelsCastRay, pmodel);
        }

        virtual void             FreeThingSite(ModelAttributes  ma, ImodelIGC*   pmodel, HitTest* pht)
        {
            if (ma & c_mtHitable)
            {
                ZAssert ((ma & c_mtStatic) == 0);
                m_kdrMoving.deleteHitTest(pht);
            }

            if ((ma & c_mtNotPickable) == 0)
            {
                DeleteIbaseIGC((BaseListIGC*)&m_modelsPickable, pmodel);
            }

            if (ma & c_mtCastRay)
            {
                DeleteIbaseIGC((BaseListIGC*)&m_modelsCastRay, pmodel);
            }
        }

        virtual bool             GetHomeSector(void) const
        {
            return m_data.bHomeSector;
        }

        virtual float             GetPendingTreasures(void) const
        {
            return m_fPendingTreasures;
        }
        virtual void             SetPendingTreasures(float  fpt)
        {
            m_fPendingTreasures = fpt;
        }

        virtual float            GetCost(void) const
        {
            return m_fCost;
        }

		//Xynth #208
		virtual void			SetHighlight(bool hl)
		{
			m_highlight = hl;
		}

		virtual bool			GetHighlight() const
		{
			return m_highlight;
		}

		//Imago 8/10 #121 #120 (adapted from common)
		virtual bool bSimpleEye(float RangeA, ImodelIGC* pmodelA, float Sig, float SigMod, float Radius, Vector pos) {
			float   m = RangeA * Sig * pmodelA->GetSide()->GetGlobalAttributeSet().GetAttribute(c_gaScanRange);
             m /= SigMod;
			float   r = pmodelA->GetRadius() + Radius + m;
			const Vector&   P1 = pmodelA->GetPosition();
			const Vector&   P2 = pos;

			Vector          V12 = P2 - P1;
			float           fLengthSquaredV12 = V12.LengthSquared (),
							fOverLengthV12 = 1.0f / sqrtf (fLengthSquaredV12);
			V12 *= fOverLengthV12;

			float           fVisibleAngle = asinf (Radius * fOverLengthV12);

			for (ModelLinkIGC* pml = ((ModelListIGC*)(GetAsteroids()))->first(); (pml != NULL); pml = pml->next())
			{
				ImodelIGC*  pmodel = pml->data();
				if (P1 != P2)
				{
					const Vector&   P3 = pmodel->GetPosition();

					Vector          V13 = P3 - P1;
					float           fLengthSquaredV13 = V13.LengthSquared ();

					if (fLengthSquaredV13 < fLengthSquaredV12)
					{
						float   dot = (V12 * V13);
						if (dot > 0.0f)
						{

							float   fOverLengthV13 = 1.0f / sqrtf (fLengthSquaredV13);
							float   fCosineSeparationAngle = dot * fOverLengthV13;

							{

								float   fRadius3 = pmodel->GetRadius () * 0.5f;
								float   fCoveredAngle = asinf (fRadius3 * fOverLengthV13);

								float   fSeparationAngle = acosf (fCosineSeparationAngle);
								float   fMaximumSeparationAngle = fSeparationAngle + fVisibleAngle;

								if (fMaximumSeparationAngle < fCoveredAngle) {
									return false;
								}
							}
						}
					}
				}
			}
			float t1 = (pmodelA->GetPosition() - pos).LengthSquared();
			float t2 = r * r;
			return (t1 <= t2);
		}
		// End Imago #121 #120 8/10

    private:
        ImissionIGC*        m_pMission;
        DWORD               m_dwPrivate; // private data for consumer
        DataClusterIGC      m_data;
        ModelListIGC        m_models;
        ModelListIGC        m_modelsPickable;
        ModelListIGC        m_modelsCastRay;
        ProbeListIGC        m_probes;
        WarpListIGC         m_warps;
        StationListIGC      m_stations;
        TreasureListIGC     m_treasures;
        AsteroidListIGC     m_asteroids;
        MissileListIGC      m_missiles;
        MineListIGC         m_mines;
        ShipListIGC         m_ships;
        Time                m_lastUpdate;
        KDroot              m_kdrStatic;
        KDroot              m_kdrMoving;
        CollisionQueue      m_collisions;
        TRef<ClusterSite>   m_pClusterSite;
        ExplosionData       m_explosions[c_maxExplosions];

        int                 m_collisionID;
        float               m_tOffset;
        float               m_tMax;
        float               m_fPendingTreasures;
        float               m_fCost;

        int                 m_nExplosions;

        int                 m_nPass;

		bool				m_highlight;  //Xynth #208 Highlight in minimap
};

#endif //__CLUSTERIGC_H_
