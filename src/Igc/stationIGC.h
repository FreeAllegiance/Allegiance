/*
**  Copyright (C) 1996, 1997 Microsoft Corporation. All Rights Reserved.
**
**  File:	stationIGC.h
**
**  Author: 
**
**  Description:
**      Header for the CstationIGC class. This file was initially created by
**  the ATL wizard.
**
**  History:
*/
// shipIGC.h : Declaration of the CshipIGC

#ifndef __STATIONIGC_H_
#define __STATIONIGC_H_
#include "igc.h"

#include "modelIGC.h"

class       CstationIGC;

class       MyStationType : public IstationTypeIGC
{
    public:
        MyStationType(CstationIGC*  pstation)
        :
            m_pstation(pstation),
            m_pStationType(NULL)
        {
        }

        ~MyStationType(void)
        {
            if (m_pStationType)
                m_pStationType->Release();
        }

        //IbaseIGC
        virtual HRESULT         Initialize(ImissionIGC* pMission, Time now, const void* data, int length);
        virtual void            Terminate(void);
        virtual void            Update(Time   now);

        virtual ObjectType      GetObjectType(void) const;
        virtual ObjectID        GetObjectID(void) const;

        //ItypeIGC
        virtual const void*     GetData(void) const;

        //IbuyableIGC
        virtual const char*             GetName(void) const;
        virtual const char*             GetDescription(void) const;
        virtual const char*             GetModelName(void) const;
        virtual const TechTreeBitMask&  GetRequiredTechs(void) const;
        virtual const TechTreeBitMask&  GetEffectTechs(void) const;

        virtual Money                   GetPrice(void) const;
        virtual DWORD                   GetTimeToBuild(void) const;
        virtual BuyableGroupID          GetGroupID(void) const;

        // IstationTypeIGC
        virtual HitPoints               GetMaxArmorHitPoints(void) const;
        virtual DefenseTypeID           GetArmorDefenseType(void) const;
        virtual HitPoints               GetMaxShieldHitPoints(void) const;
        virtual DefenseTypeID           GetShieldDefenseType(void) const;
        virtual float                   GetArmorRegeneration(void) const;
        virtual float                   GetShieldRegeneration(void) const;
        virtual float                   GetSignature(void) const;
        virtual float                   GetRadius(void) const;
        virtual float                   GetScannerRange(void) const;
        virtual Money                   GetIncome(void) const;
        virtual const TechTreeBitMask&  GetLocalTechs(void) const;
        virtual StationAbilityBitMask   GetCapabilities(void) const;
        virtual bool                    HasCapability(StationAbilityBitMask sabm) const;
        virtual const char*             GetTextureName(void) const;
        virtual const char*             GetBuilderName(void) const;
        virtual const char*             GetIconName(void) const;
        virtual IstationTypeIGC*        GetSuccessorStationType(const IsideIGC*   pside);
		virtual IstationTypeIGC*		GetDirectSuccessorStationType(void);
        virtual AsteroidAbilityBitMask  GetBuildAABM(void) const;

        virtual int                     GetLaunchSlots(void) const;
        virtual const Vector&           GetLaunchPosition(int   slotID) const;
        virtual const Vector&           GetLaunchDirection(int   slotID) const;

        virtual int                     GetLandSlots(void) const;
        virtual int                     GetCapLandSlots(void) const;
        virtual int                     GetLandPlanes(int   slotID) const;
        virtual const Vector&           GetLandPosition(int slotID, int planeID) const;
        virtual const Vector&           GetLandDirection(int slotID, int planeID) const;

        virtual SoundID                 GetInteriorSound() const;
        virtual SoundID                 GetInteriorAlertSound() const;
        virtual SoundID                 GetExteriorSound() const;

        virtual SoundID                 GetConstructorNeedRockSound() const;
        virtual SoundID                 GetConstructorUnderAttackSound() const;
        virtual SoundID                 GetConstructorDestroyedSound() const;
        virtual SoundID                 GetCompletionSound() const;

        virtual SoundID                 GetUnderAttackSound() const;
        virtual SoundID                 GetCriticalSound() const;
        virtual SoundID                 GetDestroyedSound() const;
        virtual SoundID                 GetCapturedSound() const;
        virtual SoundID                 GetEnemyCapturedSound() const;
        virtual SoundID                 GetEnemyDestroyedSound() const;

        virtual StationClassID          GetClassID() const;
        virtual IdroneTypeIGC*          GetConstructionDroneType(void) const;

    private:
        IstationTypeIGC*    GetStationType(void) const
        {
            return m_pStationType;
        }
        void SetStationType(IstationTypeIGC*  pst)
        {
            if (m_pStationType)
                m_pStationType->Release();

            m_pStationType = pst;

            if (pst)
            {
                pst->AddRef();
                m_pStationData = (const DataStationTypeIGC*)(m_pStationType->GetData());
            }
            else
                m_pStationData = NULL;
        }

        CstationIGC*                m_pstation;
        IstationTypeIGC*            m_pStationType;
        const DataStationTypeIGC*   m_pStationData;

        friend class CstationIGC;
};

class       CstationIGC : public TmodelIGC<IstationIGC>
{
    public:
        CstationIGC(void)
        :
            m_myStationType(this),
            m_hullFraction(1.0f),
            m_shieldFraction(0.0f),
			//Imago #121 7/10
			m_roidID(NA),
			m_roidSig(0.0f),
			m_roidRadius(0.0f),
			m_roidAabm(0)
        {
			m_roidPos = Vector(0.0f,0.0f,0.0f);
			ZeroMemory(&m_roidSides,sizeof(bool) * c_cSidesMax);
			//end Imago
        }

        /*
        DWORD   AddRef(void)
        {
            return TmodelIGC<IstationIGC>::AddRef();
        }

        DWORD   Release(void)
        {
            return TmodelIGC<IstationIGC>::Release();
        }
        */
    // IbaseIGC
	    virtual HRESULT Initialize(ImissionIGC* pMission, Time now, const void* data, int length);
	    virtual void    Terminate(void)
        {
            AddRef();
            {
                //Force all of the ships docked at the station to undock
                //NYI terminate all of the ships docked at the station?
                ShipLinkIGC*    l;
                while (l = m_shipsDocked.first())   //intentional assignment
                    l->data()->SetStation(NULL);
            }

            GetMyMission()->DeleteStation(this);
            TmodelIGC<IstationIGC>::Terminate();

            m_myStationType.SetStationType(NULL);
            Release();
        }
        virtual void    Update(Time now)
        {
            float   dt = now - GetMyLastUpdate();

            if (dt > 0.0f)
            {
                if (m_hullFraction != 1.0f)
                {
                    m_hullFraction += dt * m_myStationType.GetArmorRegeneration() / m_myStationType.GetMaxArmorHitPoints();
                    if (m_hullFraction > 1.0f)
                        m_hullFraction = 1.0f;
                }
                else
                {
                    float   m = m_myStationType.GetMaxShieldHitPoints();
                    if ((m_shieldFraction != 1.0f) && (m != 0.0f))
                    {
                        //Use SetShieldFraction so the station's shape will be adjusted as appropriate.
                        SetShieldFraction(m_shieldFraction + dt * m_myStationType.GetShieldRegeneration() / m);
                    }
                }

                TmodelIGC<IstationIGC>::Update(now);
            }
        }
        virtual int             Export(void* data) const;

        virtual ObjectType  GetObjectType(void) const
        {
            return OT_station;
        }

        virtual ObjectID    GetObjectID(void) const
        {
            return m_stationID;
        }

    // ImodelIGC
        virtual void    SetCluster(IclusterIGC* cluster)
        {
            AddRef();

            //Overrride the model's cluster method so that we can maintain the cluster's warp list
            //(as well as letting the model maintain its model list)
            {
                IclusterIGC*    c = GetCluster();
                if (c)
                {
                    ZAssert (cluster == NULL);
                    c->DeleteStation(this);
                    c->GetClusterSite()->MoveStation();
                }
            }

            TmodelIGC<IstationIGC>::SetCluster(cluster);

            if (cluster)
            {
                cluster->AddStation(this);
                cluster->GetClusterSite()->MoveStation();
            }
            Release();
        }

        virtual void    SetSide(IsideIGC*   psideNew)
        {
            IsideIGC*   psideOld = GetSide();
            if (psideOld != psideNew)
            {
                IclusterIGC*    pcluster = GetCluster();

                if (psideOld != NULL)
                {
                    //Remove the station from previous side's list of stations
                    if (pcluster)
                        pcluster->GetClusterSite()->DeleteScanner(psideOld->GetObjectID(), this);
                    psideOld->DeleteStation(this);
                }

                TmodelIGC<IstationIGC>::SetSide(psideNew);

                if (psideNew != NULL)
                {
                    //Add the station to the new side's list of stations
                    psideNew->AddStation(this);
                    if (pcluster)
                        pcluster->GetClusterSite()->AddScanner(psideNew->GetObjectID(), this);

                    GetHitTest()->SetUseTrueShapeSelf(psideNew);
                }

                if (pcluster != NULL)
                    pcluster->GetClusterSite()->MoveStation();
            }
        }

    // IdamageIGC
        virtual DamageResult        ReceiveDamage(DamageTypeID            type,
                                                  float                   amount,
                                                  Time                    timeCollision,
                                                  const Vector&           position1,
                                                  const Vector&           position2,
                                                  ImodelIGC*              launcher);

        virtual float   GetFraction(void) const
        {
            return m_hullFraction;
        }
        virtual void    SetFraction(float newVal)
        {
            m_hullFraction = newVal;
        }

        virtual float               GetHitPoints(void) const
        {
            return m_hullFraction * (float)m_myStationType.GetMaxArmorHitPoints();
        }

    // IscannerIGC
        bool            InScannerRange(ImodelIGC*   pModel) const
        {
            ZAssert (pModel);

            bool    rc;
            IclusterIGC*    pcluster = GetCluster();
            if (pModel->GetCluster() == pcluster)
            {
                if (pModel->GetFlag() == NA)
                {
                    float   m = m_myStationType.GetScannerRange() * pModel->GetSignature();
                    {
                        IsideIGC*   pside = pModel->GetSide();
                        if (pside)
                            m /= pside->GetGlobalAttributeSet().GetAttribute(c_gaSignature);
                    }
                    float   r = GetRadius() + pModel->GetRadius() + m;

                    rc = ((GetPosition() - pModel->GetPosition()).LengthSquared() <= r * r) &&
                         LineOfSightExist(pcluster, this, pModel);
                }
                else
                    rc = true;
            }
            else
                rc = false;

            return rc;
        }

        bool             CanSee(ImodelIGC*   pModel) const
        {
            ZAssert (pModel);

            IsideIGC*   side = GetSide();

            return (pModel->GetSide() == side) ||                               //Always see things on our side
                   pModel->SeenBySide(side) ||
                   InScannerRange(pModel);                                       //or we can see it ourselves
        }

    // IstationIGC
        virtual const IstationTypeIGC*  GetStationType(void) const
        {
            return &m_myStationType;
        }

        virtual IstationTypeIGC*        GetBaseStationType(void) const
        {
            return m_myStationType.GetStationType();
        }

        virtual void                SetBaseStationType(IstationTypeIGC* pst);

        virtual float   GetShieldFraction(void) const
        {
            return m_shieldFraction;
        }
        virtual void    SetShieldFraction(float newVal)
        {
            if (newVal < 0.0f)
                newVal = 0.0f;
            else if (newVal > 1.0f)
                newVal = 1.0f;

            m_shieldFraction = newVal;
        }

        virtual bool                    CanBuy(const IbuyableIGC* b) const
        {
            IsideIGC*   s = GetSide();
            ZAssert (s);

            ZAssert (b->GetObjectType() != OT_bucket);

            return b->GetRequiredTechs() <= (m_myStationType.GetLocalTechs() | s->GetTechs());
        }

        virtual bool                    IsObsolete(IbuyableIGC* b) const
        {
            IbuyableIGC*    pSuccessor = GetSuccessor(b);
            return (pSuccessor != b);
        }

        virtual IbuyableIGC*    GetSuccessor(IbuyableIGC* b) const
        {
            ZAssert (b);

            IbuyableIGC*    pSuccessor = b;

            if (CanBuy(b))
            {
                IbuyableIGC*    pNext = pSuccessor;
                ObjectType  type = b->GetObjectType();
                while (true)
                {
                    switch (type)
                    {
                        case OT_partType:
                        {
                            pNext = ((IpartTypeIGC*)pNext)->GetSuccessorPartType();
                        }
                        break;
                        case OT_hullType:
                        {
                            pNext = ((IhullTypeIGC*)pNext)->GetSuccessorHullType();
                        }
                        break;
                        default:
                        {
                            pNext = NULL;
                        }
                    }

                    if (pNext == NULL)
                        break;

                    if (CanBuy(pNext))
                        pSuccessor = pNext;
                }

                ZAssert (pSuccessor);
            }

            return pSuccessor;                   
        }

		//Imago 6/10 #14
        virtual Time                    GetLastUpdate(void) const
        {
            return m_lastLaunch;
        }

        virtual void                    SetLastUpdate(Time now)
        {
            m_lastLaunch = now;
        }

        virtual IpartTypeIGC*           GetSimilarPart(IpartTypeIGC* ppt) const
        {
            //Walk through the predecessor parts until one is found that can be bought
            //We need to make only a single pass because sucessors always precede their predecessors
            for (PartTypeLinkIGC*   pptl = GetMission()->GetPartTypes()->first(); (pptl != NULL); pptl = pptl->next())
            {
                IpartTypeIGC*   pptTry = pptl->data();
                if (pptTry->GetSuccessorPartType() == ppt)
                {
                    if (CanBuy(pptTry))
                        return pptTry;
                    else
                        ppt = pptTry;
                }
            }

            return NULL;
        }
        
        virtual void                    AddShip(IshipIGC* s);
        virtual void                    DeleteShip(IshipIGC* s);
        virtual IshipIGC*               GetShip(ShipID shipID) const;
        virtual const ShipListIGC*      GetShips(void) const;

        virtual void                    Launch(IshipIGC* pship);
        virtual void                    RepairAndRefuel(IshipIGC* pship) const;
        virtual bool                    InGarage(IshipIGC* pship, const Vector& position);

        virtual void                    SetLastDamageReport(Time timeLastDamage)
        {
            m_timeLastDamageReport = timeLastDamage;
        }
        virtual Time                    GetLastDamageReport(void) const
        {
            return m_timeLastDamageReport;
        }
		//Imago 6/10 #14
        virtual void                    SetLastLaunch(Time timeLastLaunch)
        {
           m_lastLaunch = timeLastLaunch;
        }
        virtual Time                   GetLastLaunch(void) const
        {
            return m_lastLaunch;
        }
		//
		//Imago #121 7/10
		virtual ObjectID GetRoidID() const {
			return m_roidID;
		}
		virtual void SetRoidID(ObjectID id) {
			m_roidID = id;
		}

		virtual Vector					GetRoidPos() const {
			return m_roidPos;
		}
		virtual void SetRoidPos(Vector pos) {
			m_roidPos = pos;
		}

		virtual float					GetRoidSig() const {
			return m_roidSig;
		}
		virtual void SetRoidSig(float Sig) {
			m_roidSig = Sig;
		}

		virtual float					GetRoidRadius() const {
			return m_roidRadius;
		}
		virtual void SetRoidRadius(float Radius) {
			m_roidRadius = Radius;
		}
		
		virtual AsteroidAbilityBitMask	GetRoidCaps() const {
			return m_roidAabm;
		}
		virtual void SetRoidCaps(AsteroidAbilityBitMask aabm) {
			m_roidAabm = aabm;
		}
		virtual void SetRoidSide(SideID sid, bool bset = true) {
			m_roidSides[sid] = bset;
		}
		virtual bool GetRoidSide(SideID sid) {
			return (m_roidSides[sid]);
		}
 		//

        virtual SoundID                 GetInteriorSound() const
        {
            return (GetFraction() > 0.8) ? m_myStationType.GetInteriorSound() 
                : m_myStationType.GetInteriorAlertSound();
        }
        virtual SoundID                 GetExteriorSound() const
        {
            return m_myStationType.GetExteriorSound();
        }

    private:
        MyStationType               m_myStationType;

        float                       m_hullFraction;
        float                       m_shieldFraction;

        Time                        m_timeLastDamageReport;

        ShipListIGC                 m_shipsDocked;
        StationID                   m_stationID;
        unsigned char               m_undockPosition;
		Time						m_lastLaunch; //Imago 6/10 #16
		 //Imago #121 7/10 #120 8/10
		ObjectID					m_roidID;
		float						m_roidSig;
		float						m_roidRadius;
		Vector						m_roidPos;
		AsteroidAbilityBitMask		m_roidAabm;
		bool						m_roidSides[c_cSidesMax];
};

#endif //__STATIONIGC_H_
