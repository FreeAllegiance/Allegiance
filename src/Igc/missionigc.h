/*
**  Copyright (C) 1996, 1997 Microsoft Corporation. All Rights Reserved.
**
**  File:	missionIGC.h
**
**  Author: 
**
**  Description:
**      Header for the CmissionIGC class. This file was initially created by
**  the ATL wizard.
**
**  History:
*/
// missionIGC.h : Declaration of the CmissionIGC

#ifndef __MISSIONIGC_H_
#define __MISSIONIGC_H_
#include "igc.h"


struct  Constants
{
    float   floatConstants[c_fcidMax];
    float   damageConstants[c_dmgidMax][c_defidMax];
};

//Define a class that will handle all of the static data (for all missions)
class   CstaticIGC
{
    public:
        CstaticIGC(void);

        ~CstaticIGC(void)
        {
        }

    // IstaticIGC
        void            Initialize(void)
        {
        }
        void            Terminate(void)
        {
			//Imago reordered for debugging purposes 8/17/09
            {
                TreasureSetLinkIGC*  l;
				debugf("Nuking %d treasureset(s):\n",m_treasureSets.n());
                while ((l = m_treasureSets.first()) != NULL)
                {
					debugf("\t%s (%i)\n",l->data()->GetName(), l->data()->GetObjectID());
                    l->data()->Terminate();
                }
            }
            {
                DroneTypeLinkIGC*  l;
				debugf("Nuking %d drone types:\n",m_droneTypes.n());
                while ((l = m_droneTypes.first()) != NULL)
                {
					debugf("\t%s (%i)\n",l->data()->GetName(), l->data()->GetObjectID());
                    l->data()->Terminate();
                }
            }
            {
                StationTypeLinkIGC*  l;
                while ((l = m_stationTypes.first()) != NULL)
                {
                    l->data()->Terminate();
                }
            }
            {
                HullTypeLinkIGC*  l;
                while ((l = m_hullTypes.first()) != NULL)
                {
                    l->data()->Terminate();
                }
            }
            {
                ExpendableTypeLinkIGC*  l;
                while ((l = m_expendableTypes.first()) != NULL)
                {
                    l->data()->Terminate();
                }
            }
            {
                PartTypeLinkIGC*  l;
                while ((l = m_partTypes.first()) != NULL)
                {
                    l->data()->Terminate();
                }
            }
            {
                ProjectileTypeLinkIGC*  l;
                while ((l = m_projectileTypes.first()) != NULL)
                {
                    l->data()->Terminate();
                }
            }
            {
                DevelopmentLinkIGC*  l;
                while ((l = m_developments.first()) != NULL)
                {
                    l->data()->Terminate();
                }
            }
            {
                CivilizationLinkIGC*  l;
                while ((l = m_civilizations.first()) != NULL)
                {
                    l->data()->Terminate();
                }
            }

            ZAssert (m_stationTypes.n() == 0);
            ZAssert (m_hullTypes.n() == 0);
            ZAssert (m_partTypes.n() == 0);
            ZAssert (m_projectileTypes.n() == 0);
            ZAssert (m_developments.n() == 0);
            ZAssert (m_droneTypes.n() == 0);
            ZAssert (m_civilizations.n() == 0);
            ZAssert (m_treasureSets.n() == 0);

            if (m_pptAmmoPack)
            {
                m_pptAmmoPack->Release();
                m_pptAmmoPack = NULL;
            }

            if (m_pptFuelPack)
            {
                m_pptFuelPack->Release();
                m_pptFuelPack = NULL;
            }
        }
        virtual float                   GetFloatConstant(FloatConstantID fcid) const
        {
            ZAssert (fcid >= 0);
            ZAssert (fcid < c_fcidMax);

            return m_constants.floatConstants[fcid];
        }
        virtual void                    SetFloatConstant(FloatConstantID fcid, float f)
        {
            ZAssert (fcid >= 0);
            ZAssert (fcid < c_fcidMax);

            m_constants.floatConstants[fcid] = f;
        }

        virtual float                           GetDamageConstant(DamageTypeID dmgid, DefenseTypeID defid) const
        {
            ZAssert ((dmgid & c_dmgidMask) >= 0);
            ZAssert ((dmgid & c_dmgidMask) < c_dmgidMax);
            ZAssert (defid >= 0);
            ZAssert (defid < c_defidMax);

            return m_constants.damageConstants[dmgid & c_dmgidMask][defid];
        }
        virtual void                            SetDamageConstant(DamageTypeID dmgid, DefenseTypeID defid, float f)
        {
            ZAssert ((dmgid & c_dmgidMask) >= 0);
            ZAssert ((dmgid & c_dmgidMask) < c_dmgidMax);
            ZAssert (defid >= 0);
            ZAssert (defid < c_defidMax);

            m_constants.damageConstants[dmgid & c_dmgidMask][defid] = f;
        }

        virtual const void*             GetConstants(void) const
        {
            return &m_constants;
        }
        virtual void                    SetConstants(const void* pf)
        {
            memcpy(&m_constants, pf, sizeof(m_constants));
        }
        virtual int                     GetSizeOfConstants(void) const
        {
            return sizeof(m_constants);
        }
        void                            AddDroneType(IdroneTypeIGC*                 dt)
        {
            AddIbaseIGC((BaseListIGC*)&m_droneTypes, dt);
        }
        void                            DeleteDroneType(IdroneTypeIGC*              dt)
        {
            DeleteIbaseIGC((BaseListIGC*)&m_droneTypes, dt);
        }
        IdroneTypeIGC*                  GetDroneType(DroneTypeID                    id) const
        {
            return (IdroneTypeIGC*)GetIbaseIGC((BaseListIGC*)&m_droneTypes, id);
        }
        const DroneTypeListIGC*         GetDroneTypes(void) const
        {
            return &m_droneTypes;
        }

        void                            AddDevelopment(IdevelopmentIGC*             d)
        {
            AddIbaseIGC((BaseListIGC*)&m_developments, d);
        }
        void                            DeleteDevelopment(IdevelopmentIGC*          d)
        {
            DeleteIbaseIGC((BaseListIGC*)&m_developments, d);
        }
        IdevelopmentIGC*                GetDevelopment(DevelopmentID                id) const
        {
            return (IdevelopmentIGC*)GetIbaseIGC((BaseListIGC*)&m_developments, id);
        }
        const DevelopmentListIGC*       GetDevelopments(void) const
        {
            return &m_developments;
        }

        void                            AddStationType(IstationTypeIGC*             st)
        {
            AddIbaseIGC((BaseListIGC*)&m_stationTypes, st);
        }
        void                            DeleteStationType(IstationTypeIGC*          st)
        {
            DeleteIbaseIGC((BaseListIGC*)&m_stationTypes, st);
        }
        IstationTypeIGC*                GetStationType(StationTypeID                id) const
        {
            return (IstationTypeIGC*)GetIbaseIGC((BaseListIGC*)&m_stationTypes, id);
        }
        const StationTypeListIGC*       GetStationTypes(void) const
        {
            return &m_stationTypes;
        }

        void                            AddCivilization(IcivilizationIGC*           c)
        {
            AddIbaseIGC((BaseListIGC*)&m_civilizations, c);
        }
        void                            DeleteCivilization(IcivilizationIGC*        c)
        {
            DeleteIbaseIGC((BaseListIGC*)&m_civilizations, c);
        }
        IcivilizationIGC*               GetCivilization(CivID                       id) const
        {
            return (IcivilizationIGC*)GetIbaseIGC((BaseListIGC*)&m_civilizations, id);
        }
        const CivilizationListIGC*      GetCivilizations(void) const
        {
            return &m_civilizations;
        }

        void                            AddHullType(IhullTypeIGC*                   ht)
        {
            AddIbaseIGC((BaseListIGC*)&m_hullTypes, ht);
        }
        void                            DeleteHullType(IhullTypeIGC*                ht)
        {
            DeleteIbaseIGC((BaseListIGC*)&m_hullTypes, ht);
        }
        IhullTypeIGC*                   GetHullType(HullID                          id) const
        {
            return (IhullTypeIGC*)GetIbaseIGC((BaseListIGC*)&m_hullTypes, id);
        }
        const HullTypeListIGC*          GetHullTypes(void) const
        {
            return &m_hullTypes;
        }

        void                            AddExpendableType(IexpendableTypeIGC*             mt)
        {
            AddIbaseIGC((BaseListIGC*)&m_expendableTypes, mt);
        }
        void                            DeleteExpendableType(IexpendableTypeIGC*          mt)
        {
            DeleteIbaseIGC((BaseListIGC*)&m_expendableTypes, mt);
        }
        IexpendableTypeIGC*             GetExpendableType(ExpendableTypeID                id) const
        {
            return (IexpendableTypeIGC*)GetIbaseIGC((BaseListIGC*)&m_expendableTypes, id);
        }
        const ExpendableTypeListIGC*    GetExpendableTypes(void) const
        {
            return &m_expendableTypes;
        }

        void                            AddPartType(IpartTypeIGC*                   pt)
        {
            AddIbaseIGC((BaseListIGC*)&m_partTypes, pt);

            if (pt->GetEquipmentType() == ET_Pack)
            {
                DataPackTypeIGC*    pdpt = (DataPackTypeIGC*)(pt->GetData());
                if (pdpt->packType == c_packAmmo)
                {
                    if (m_pptAmmoPack == NULL)
                    {
                        m_pptAmmoPack = pt;
                        pt->AddRef();
                    }
                }
                else if (pdpt->packType == c_packFuel)
                {
                    if (m_pptFuelPack == NULL)
                    {
                        m_pptFuelPack = pt;
                        pt->AddRef();
                    }
                }
            }
        }
        void                            DeletePartType(IpartTypeIGC*                pt)
        {
            DeleteIbaseIGC((BaseListIGC*)&m_partTypes, pt);
        }
        IpartTypeIGC*                   GetPartType(PartID                          id) const
        {
            return (IpartTypeIGC*)GetIbaseIGC((BaseListIGC*)&m_partTypes, id);
        }
        const PartTypeListIGC*          GetPartTypes(void) const
        {
            return &m_partTypes;
        }

        virtual void                            AddTreasureSet(ItreasureSetIGC*                   pt)
        {
            AddIbaseIGC((BaseListIGC*)&m_treasureSets, pt);
        }
        virtual void                            DeleteTreasureSet(ItreasureSetIGC*                pt)
        {
            DeleteIbaseIGC((BaseListIGC*)&m_treasureSets, pt);
        }
        virtual ItreasureSetIGC*                GetTreasureSet(TreasureSetID                      id) const
        {
            return (ItreasureSetIGC*)GetIbaseIGC((BaseListIGC*)&m_treasureSets, id);
        }
        virtual const TreasureSetListIGC*       GetTreasureSets(void) const
        {
            return &m_treasureSets;
        }

        void                            AddProjectileType(IprojectileTypeIGC*       pt)
        {
            AddIbaseIGC((BaseListIGC*)&m_projectileTypes, pt);
        }
        void                            DeleteProjectileType(IprojectileTypeIGC*    pt)
        {
            DeleteIbaseIGC((BaseListIGC*)&m_projectileTypes, pt);
        }
        IprojectileTypeIGC*             GetProjectileType(ProjectileTypeID  id) const
        {
            return (IprojectileTypeIGC*)GetIbaseIGC((BaseListIGC*)&m_projectileTypes, id);
        }
        const ProjectileTypeListIGC*    GetProjectileTypes(void) const
        {
            return &m_projectileTypes;
        }

        IpartTypeIGC*           GetAmmoPack(void) const
        {
            return m_pptAmmoPack;
        }

        IpartTypeIGC*           GetFuelPack(void) const
        {
            return m_pptFuelPack;
        }

    private:
        HullTypeListIGC          m_hullTypes;
        PartTypeListIGC          m_partTypes;
        ProjectileTypeListIGC    m_projectileTypes;
        ExpendableTypeListIGC    m_expendableTypes;
        StationTypeListIGC       m_stationTypes;
        CivilizationListIGC      m_civilizations;
        DroneTypeListIGC         m_droneTypes;
        DevelopmentListIGC       m_developments;
        TreasureSetListIGC       m_treasureSets;

        IpartTypeIGC*            m_pptAmmoPack;
        IpartTypeIGC*            m_pptFuelPack;

        Constants                m_constants;
};

/////////////////////////////////////////////////////////////////////////////
// CmissionIGC
class   CmissionIGC : public ImissionIGC
{
    public:
        CmissionIGC(void)
        :
            m_pIgcSite(NULL),
            m_dwPrivate(0),
            m_nextBuoyID(0),
            m_nextShipID(0),
            m_nextAsteroidID(0),
            m_nextTreasureID(0),
            m_nextMineID(0),
            m_nextProbeID(0),
            m_nextMissileID(0),
            m_nextStationID(0),
            m_stageMission(STAGE_NOTSTARTED),
            m_missionID(0),
            m_pStatic(NULL),
            m_bHasGenerated(false),
            m_nReplayCount(0)
        {
            // Record the creation time
            SYSTEMTIME st;
            ::GetLocalTime(&st);
            ::SystemTimeToFileTime(&st, &m_ftCreated);

			ZeroMemory(rgTechs, sizeof(rgTechs));
			ZeroMemory(rgParts, sizeof(rgParts));
        }

        virtual ~CmissionIGC(void);

    // IstaticIGC
        virtual void                            Initialize(void)
        {
            //Never called ... should use the initialize of ImissionIGC
            ZAssert (false);
        }
        virtual void                            Terminate(void);

        virtual float                           GetFloatConstant(FloatConstantID fcid) const
        {
            return m_pStatic->GetFloatConstant(fcid);
        }
        virtual void                    SetFloatConstant(FloatConstantID fcid, float f)
        {
            m_pStatic->SetFloatConstant(fcid, f);
        }

        virtual float                           GetDamageConstant(DamageTypeID dmgid, DefenseTypeID defid) const
        {
            return  m_pStatic->GetDamageConstant(dmgid, defid);
        }
        virtual void                            SetDamageConstant(DamageTypeID dmgid, DefenseTypeID defid, float f)
        {
             m_pStatic->SetDamageConstant(dmgid, defid, f);
        }
        virtual const void*                     GetConstants(void) const
        {
            return m_pStatic->GetConstants();
        }
        virtual void                            SetConstants(const void* pf)
        {
            m_pStatic->SetConstants(pf);
        }
        virtual int                             GetSizeOfConstants(void) const
        {
            return m_pStatic->GetSizeOfConstants();
        }
        virtual void                            AddDroneType(IdroneTypeIGC*                 dt)
        {
            m_pStatic->AddDroneType(dt);
        }
        virtual void                            DeleteDroneType(IdroneTypeIGC*              dt)
        {
            m_pStatic->DeleteDroneType(dt);
        }
        virtual IdroneTypeIGC*                  GetDroneType(DroneTypeID                    id) const
        {
            return m_pStatic->GetDroneType(id);
        }
        virtual const DroneTypeListIGC*         GetDroneTypes(void) const
        {
            return m_pStatic->GetDroneTypes();
        }

        virtual void                            AddDevelopment(IdevelopmentIGC*             d)
        {
            m_pStatic->AddDevelopment(d);
        }
        virtual void                            DeleteDevelopment(IdevelopmentIGC*          d)
        {
            m_pStatic->DeleteDevelopment(d);
        }
        virtual IdevelopmentIGC*                GetDevelopment(DevelopmentID                id) const
        {
            return m_pStatic->GetDevelopment(id);
        }
        virtual const DevelopmentListIGC*       GetDevelopments(void) const
        {
            return m_pStatic->GetDevelopments();
        }

        virtual void                            AddStationType(IstationTypeIGC*             st)
        {
            m_pStatic->AddStationType(st);
        }
        virtual void                            DeleteStationType(IstationTypeIGC*          st)
        {
            m_pStatic->DeleteStationType(st);
        }
        virtual IstationTypeIGC*                GetStationType(StationTypeID                id) const
        {
            return m_pStatic->GetStationType(id);
        }
        virtual const StationTypeListIGC*       GetStationTypes(void) const
        {
            return m_pStatic->GetStationTypes();
        }

        virtual void                            AddCivilization(IcivilizationIGC*           c)
        {
            m_pStatic->AddCivilization(c);
        }
        virtual void                            DeleteCivilization(IcivilizationIGC*        c)
        {
            m_pStatic->DeleteCivilization(c);
        }
        virtual IcivilizationIGC*               GetCivilization(CivID                       id) const
        {
            return m_pStatic->GetCivilization(id);
        }
        virtual const CivilizationListIGC*      GetCivilizations(void) const
        {
            return m_pStatic->GetCivilizations();
        }

        virtual void                            AddHullType(IhullTypeIGC*                   ht)
        {
            m_pStatic->AddHullType(ht);
        }
        virtual void                            DeleteHullType(IhullTypeIGC*                ht)
        {
            m_pStatic->DeleteHullType(ht);
        }
        virtual IhullTypeIGC*                   GetHullType(HullID                          id) const
        {
            return m_pStatic->GetHullType(id);
        }
        virtual const HullTypeListIGC*          GetHullTypes(void) const
        {
            return m_pStatic->GetHullTypes();
        }

        virtual void                            AddExpendableType(IexpendableTypeIGC*             mt)
        {
            m_pStatic->AddExpendableType(mt);
        }
        virtual void                            DeleteExpendableType(IexpendableTypeIGC*          mt)
        {
            m_pStatic->DeleteExpendableType(mt);
        }
        virtual IexpendableTypeIGC*             GetExpendableType(ExpendableTypeID                id) const
        {
            return m_pStatic->GetExpendableType(id);
        }
        virtual const ExpendableTypeListIGC*    GetExpendableTypes(void) const
        {
            return m_pStatic->GetExpendableTypes();
        }

        virtual void                            AddPartType(IpartTypeIGC*                   pt)
        {
            m_pStatic->AddPartType(pt);

            {
                //Preload the art for the corresponding model
                m_pIgcSite->Preload(pt->GetModelName(), NULL);
            }
        }
        virtual void                            DeletePartType(IpartTypeIGC*                pt)
        {
            m_pStatic->DeletePartType(pt);
        }
        virtual IpartTypeIGC*                   GetPartType(PartID                          id) const
        {
            return m_pStatic->GetPartType(id);
        }
        virtual const PartTypeListIGC*          GetPartTypes(void) const
        {
            return m_pStatic->GetPartTypes();
        }

        virtual void                            AddTreasureSet(ItreasureSetIGC*                   pt)
        {
            m_pStatic->AddTreasureSet(pt);
        }
        virtual void                            DeleteTreasureSet(ItreasureSetIGC*                pt)
        {
            m_pStatic->DeleteTreasureSet(pt);
        }
        virtual ItreasureSetIGC*                GetTreasureSet(TreasureSetID                      id) const
        {
            return m_pStatic->GetTreasureSet(id);
        }
        virtual const TreasureSetListIGC*       GetTreasureSets(void) const
        {
            return m_pStatic->GetTreasureSets();
        }

        virtual void                            AddProjectileType(IprojectileTypeIGC*       pt)
        {
            m_pStatic->AddProjectileType(pt);
        }
        virtual void                            DeleteProjectileType(IprojectileTypeIGC*    pt)
        {
            m_pStatic->DeleteProjectileType(pt);
        }
        virtual IprojectileTypeIGC*             GetProjectileType(ProjectileTypeID              id) const
        {
            return m_pStatic->GetProjectileType(id);
        }
        virtual const ProjectileTypeListIGC*    GetProjectileTypes(void) const
        {
            return m_pStatic->GetProjectileTypes();
        }

    // ImissionIGC
        virtual void                            Initialize(Time now, IIgcSite* pIgcSite);
        virtual void                            Update(Time now);

        virtual MissionID                       GetMissionID(void) const
        {
            return m_missionID;
        }

        virtual int                     Export(int64_t  maskTypes,
                                               char*    pdata) const;
        virtual void                    Import(Time     now,
                                               int64_t  maskTypes,
                                               char*    pdata,
                                               int      datasize);
		
		// Imago
		virtual ZString					BitsToTechsList(TechTreeBitMask & ttbm);
		virtual ZString					BitsToPartsList(PartMask & pm, EquipmentType et);
		
		virtual void					TechsListToBits(const char * szTechs, TechTreeBitMask & ttbm);
		virtual int						TechBitFromToken(const char * szToken);
		
		virtual PartMask				PartMaskFromToken(const char * szToken, EquipmentType et);
		virtual PartMask				PartsListToMask(const char * szParts, EquipmentType et);
		
		virtual bool					LoadTechBitsList(void);
		virtual bool					LoadPartsBitsList(void);
		
		virtual void					ExportStaticIGCObjs();
		virtual void					ImportStaticIGCObjs();
		// ^

        virtual void                            SetMissionID(MissionID mid)
        {
            m_missionID = mid;
        }

        virtual IIgcSite*                       GetIgcSite(void) const
        {
            return m_pIgcSite;
        }

        virtual IbaseIGC*                       CreateObject(Time now, ObjectType objecttype,
                                                             const void* data, int dataSize);

        virtual ImodelIGC*                      GetModel(ObjectType type, ObjectID id) const;

        virtual IbaseIGC*                       GetBase(ObjectType type, ObjectID id) const;

        virtual DamageTrack*                    CreateDamageTrack(void)
        {
            return m_damageTracks.Create();
        }
        
        //Global list of stations
        virtual void                            AddStation(IstationIGC* s);
        virtual void                            DeleteStation(IstationIGC* s);
        virtual IstationIGC*                    GetStation(StationID stationID) const;
        virtual const StationListIGC*           GetStations(void) const;

        //Global list of warps
        virtual void                            AddWarp(IwarpIGC* w);
        virtual void                            DeleteWarp(IwarpIGC* w);
        virtual IwarpIGC*                       GetWarp(WarpID warpID) const;
        virtual const WarpListIGC*              GetWarps(void) const;

        //Global list of asteroids
        virtual void                            AddAsteroid(IasteroidIGC* s);
        virtual void                            DeleteAsteroid(IasteroidIGC* s);
        virtual IasteroidIGC*                   GetAsteroid(AsteroidID id) const;
        virtual const AsteroidListIGC*          GetAsteroids(void) const;

        //Global list of mines
        virtual void                            AddMine(ImineIGC* ms);
        virtual void                            DeleteMine(ImineIGC* m);
        virtual ImineIGC*                       GetMine(MineID mineID) const;
        virtual const MineListIGC*              GetMines(void) const;

        //Global list of probes
        virtual void                            AddProbe(IprobeIGC* ms);
        virtual void                            DeleteProbe(IprobeIGC* m);
        virtual IprobeIGC*                      GetProbe(ProbeID probeID) const;
        virtual const ProbeListIGC*             GetProbes(void) const;

        //Global list of treasures
        virtual void                            AddTreasure(ItreasureIGC* t);
        virtual void                            DeleteTreasure(ItreasureIGC* t);
        virtual ItreasureIGC*                   GetTreasure(TreasureID treasureID) const;
        virtual const TreasureListIGC*          GetTreasures(void) const;

        //Global list of sides
        virtual void                            AddSide(IsideIGC* s);
        virtual void                            DeleteSide(IsideIGC* s);
        virtual IsideIGC*                       GetSide(SideID sideID) const;
        virtual const SideListIGC*              GetSides(void) const;
		virtual void                            GetSeenSides(ImodelIGC * pmodel,bool (&bseensides)[c_cSidesMax], ImodelIGC * poptionalmodel = NULL); //Imago #120 #121 8/10

        //Global list of clusters
        virtual void                            AddCluster(IclusterIGC* c);
        virtual void                            DeleteCluster(IclusterIGC* c);
        virtual IclusterIGC*                    GetCluster(SectorID clusterID) const;
        virtual const ClusterListIGC*           GetClusters(void) const;

        //Global list of ships
        virtual void                            AddShip(IshipIGC* s);
        virtual void                            DeleteShip(IshipIGC* s);
        virtual IshipIGC*                       GetShip(ShipID shipID) const;
        virtual const ShipListIGC*              GetShips(void) const;

        //Global list of buoys
        virtual void                            AddBuoy(IbuoyIGC* s);
        virtual void                            DeleteBuoy(IbuoyIGC* s);
        virtual IbuoyIGC*                       GetBuoy(BuoyID buoyID) const;
        virtual const BuoyListIGC*              GetBuoys(void) const;

        virtual IpartTypeIGC*                   GetAmmoPack(void) const
        {
            return m_pStatic->GetAmmoPack();
        }
        virtual IpartTypeIGC*                   GetFuelPack(void) const
        {
            return m_pStatic->GetFuelPack();
        }

        IpartIGC*   CreatePart(Time             now,
                               IpartTypeIGC*    ppt)
        {
            static const ObjectType    objectTypes[ET_MAX] =
                                        {OT_dispenser,        
                                         OT_weapon,
                                         OT_magazine,
                                         OT_dispenser,
                                         OT_shield,
                                         OT_cloak,
                                         OT_pack,
                                         OT_afterburner
                                        };
            ZAssert (objectTypes[ET_ChaffLauncher] == OT_dispenser);
            ZAssert (objectTypes[ET_Weapon] == OT_weapon);
            ZAssert (objectTypes[ET_Magazine] == OT_magazine);
            ZAssert (objectTypes[ET_Dispenser] == OT_dispenser);
            ZAssert (objectTypes[ET_Shield] == OT_shield);
            ZAssert (objectTypes[ET_Cloak] == OT_cloak);
            ZAssert (objectTypes[ET_Pack] == OT_pack);
            ZAssert (objectTypes[ET_Afterburner] == OT_afterburner);

            //The "data" used to create a part is a pointer to the part type.
            return (IpartIGC*)CreateObject(now, objectTypes[ppt->GetEquipmentType()], &ppt, sizeof(ppt));
        }

        void                    SetMissionStage(STAGE  st)
        {
            m_stageMission = st;
        }

        STAGE                   GetMissionStage()
        {
            return m_stageMission;
        }

        void    EnterGame(void)
        {
            if (m_missionParams.IsProsperityGame())
            {
                IdevelopmentIGC*    pdTeamMoney = GetDevelopment(c_didTeamMoney);
                ZAssert (pdTeamMoney);

                ZAssert (m_missionParams.fGoalTeamMoney > 0);

                for (SideLinkIGC*   psl = m_sides.first(); (psl != NULL); psl = psl->next())
                {
                    //It is something we might be able to build and it will actually
                    //do something useful
                    DataBucketIGC   db = {pdTeamMoney, psl->data()};
                    IbucketIGC*     b = (IbucketIGC*)(CreateObject(m_lastUpdate,
                                                                   OT_bucket,
                                                                   &db,
                                                                   sizeof(db)));
                    ZAssert (b);

                    b->SetPrice((Money)(m_missionParams.fGoalTeamMoney * GetFloatConstant(c_fcidWinTheGameMoney)));

                    b->Release();   //Creating the bucket adds it to the side's list of buckets
                }
            }

            //Go through all sides and create the prosperity development (adjusting the price as needed)
            //Go through all the treasures and reset their expiration time
            for (TreasureLinkIGC*   ptl = m_treasures.first(); (ptl != NULL); ptl = ptl->next())
                ptl->data()->ResetExpiration(m_lastUpdate);

            //Go through all stations and reset their start time
            for (StationLinkIGC*  psl = m_stations.first(); (psl != NULL); psl = psl->next())
                psl->data()->SetLastUpdate(m_lastUpdate);
        }

        void                    UpdateSides(Time now,
                                            const MissionParams * pmp,
                                            const char  sideNames[c_cSidesMax][c_cbSideName]);
		void                    UpdateAllies(const char  Allies[c_cSidesMax]); //#ALLY
        void                    ResetMission();

        void                    GenerateMission(Time                   now,
                                                const MissionParams *  pmp,
                                                TechTreeBitMask *      pttbmShouldOverride,
                                                TechTreeBitMask *      pttbmOverrideValue);

        void                    GenerateTreasure(Time           now,
                                                 IclusterIGC*   pcluster,
                                                 short          tsi);
        virtual void            SetPrivateData(DWORD dwPrivate)
        {
            m_dwPrivate = dwPrivate;
        }

		//imago 7/30/08
        virtual ZString            GetTechFlagName(int id) const
        {
            return ZString(rgTechs[id]);
        }
        
		virtual void            SetTechFlagName(int id, ZString name)
        {
            Strcpy(rgTechs[id],name);
        }

        virtual ZString            GetPartFlagName(int id, EquipmentType et) const
        {
            return ZString(rgParts[et][id]);
        }
        
		virtual void            SetPartFlagName(int id, EquipmentType et, ZString name)
        {
            Strcpy(rgParts[et][id],name);
        }
		// /imago

        virtual DWORD           GetPrivateData(void) const
        {
            return m_dwPrivate;
        }

        BuoyID                  GenerateNewBuoyID(void)
        {
            return m_nextBuoyID++;
        }

        ShipID                  GenerateNewShipID(void)
        {
            return m_nextShipID++;
        }

        AsteroidID              GenerateNewAsteroidID(void)
        {
            return m_nextAsteroidID++;
        }

        TreasureID              GenerateNewTreasureID(void)
        {
            return m_nextTreasureID++;
        }

        MineID                  GenerateNewMineID(void)
        {
            return m_nextMineID++;
        }
        ProbeID                 GenerateNewProbeID(void)
        {
            return m_nextProbeID++;
        }
        MissileID                  GenerateNewMissileID(void)
        {
            return m_nextMissileID++;
        }
        StationID                  GenerateNewStationID(void)
        {
            return m_nextStationID++;
        }

        Time                    GetLastUpdate(void) const
        {
            return m_lastUpdate;
        }

        const MissionParams*    GetMissionParams(void) const
        {
            return &m_missionParams;
        }
        void                    SetMissionParams(const MissionParams* pmp)
        {
            m_missionParams = *pmp;
        }
        virtual void                    SetStartTime(Time timeStart)
        {
            m_missionParams.timeStart = timeStart;
        }

        CstaticIGC*             GetStaticCore(void) const
        {
            return m_pStatic;
        }

        void                    SetStaticCore(CstaticIGC*   pStatic)
        {
            ZAssert (m_pStatic == NULL);

            if (pStatic)
                m_pStatic = pStatic;
            else
            {
                m_pStatic = new CstaticIGC;
                m_pStatic->Initialize();
            }
        }

        short                   GetReplayCount(void) const;
        const char*             GetContextName(void);

    private:
        MissionParams            m_missionParams;
        DWORD                    m_dwPrivate;
        MissionID                m_missionID;
        CstaticIGC*              m_pStatic;        

        IIgcSite*                m_pIgcSite;

        DamageTrackSet           m_damageTracks;
        
        ClusterListIGC           m_clusters;
        BuoyListIGC              m_buoys;
        ShipListIGC              m_ships;
        WarpListIGC              m_warps;
        StationListIGC           m_stations;
        AsteroidListIGC          m_asteroids;
        ProbeListIGC             m_probes;
        MineListIGC              m_mines;
        TreasureListIGC          m_treasures;
        SideListIGC              m_sides;
        IsideIGC*                m_sideTeamLobby;
        Time                     m_lastUpdate;

        BuoyID                   m_nextBuoyID;
        ShipID                   m_nextShipID;
        AsteroidID               m_nextAsteroidID;
        TreasureID               m_nextTreasureID;
        MineID                   m_nextMineID;
        ProbeID                  m_nextProbeID;
        MissileID                m_nextMissileID;
        StationID                m_nextStationID;

        STAGE                    m_stageMission;

        FILETIME                 m_ftCreated;
        bool                     m_bHasGenerated;
        short                    m_nReplayCount;
        ZString                  m_strContextName;
		char					 rgTechs[cTechs][CbTechBitName+1];
		char					 rgParts[ET_MAX][16][CbTechBitName+1];
};

// Read missions from plain-text Imago 8/3/08 NYI: XML
#pragma warning(disable:4530)
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
void readCSV(std::istream &input, std::vector< std::vector<std::string> > &output);

#endif //__MISSIONIGC_H_
