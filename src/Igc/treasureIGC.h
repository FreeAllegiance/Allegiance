/*
**  Copyright (C) 1996, 1997 Microsoft Corporation. All Rights Reserved.
**
**  File:	treasureIGC.h
**
**  Author: 
**
**  Description:
**      Header for the CtreasureIGC class. This file was initially created by
**  the ATL wizard.
**
**  History:
*/
// treasureIGC.h : Declaration of the CtreasureIGC

#pragma once

#include "igc.h"
#include "modelIGC.h"

class CtreasureIGC : public TmodelIGC<ItreasureIGC>
{
    public:
    // IbaseIGC
        virtual HRESULT         Initialize(ImissionIGC* pMission, Time now, const void* data, int length);
        virtual void    Terminate(void)
        {
            AddRef();

            GetMyMission()->DeleteTreasure(this);
            TmodelIGC<ItreasureIGC>::Terminate();

            if (m_buyable)
            {
                m_buyable->Release();
                m_buyable = NULL;
            }

            Release();
        }

        virtual void            Update(Time now);

        virtual int             Export(void* data) const;

        virtual ObjectType      GetObjectType(void) const
        {
            return OT_treasure;
        }

        virtual ObjectID        GetObjectID(void) const
        {
            return m_data.objectID;
        }

    // ImodelIGC
        virtual void    SetCluster(IclusterIGC* cluster)
        {
            AddRef();

            //Overrride the model's cluster method so that we can maintain the cluster's treasure list
            //(as well as letting the model maintain its model list)
            {
                IclusterIGC*    c = GetCluster();
                if (c)
                    c->DeleteTreasure(this);
            }

            TmodelIGC<ItreasureIGC>::SetCluster(cluster);

            if (cluster)
                cluster->AddTreasure(this);

            Release();
        }

        virtual void                 HandleCollision(Time       timeCollision,
                                                     float                  tCollision,
                                                     const CollisionEntry&  entry,
                                                     ImodelIGC* pModel)
        {
            if (pModel->GetObjectType() == OT_ship)
            {
                //A treasure hitting a ship ... can the ship pick treasures up?
                if ((((IshipIGC*)pModel)->GetPilotType() >= c_ptPlayer) &&
                    ((IshipIGC*)pModel)->GetBaseHullType()->HasCapability(c_habmRescue))
                {
                    //yes ...let the gamesite deal with it.
                    IIgcSite*   igc = GetMyMission()->GetIgcSite();

                    if (igc->HitTreasureEvent(timeCollision,
                                              (IshipIGC*)pModel,
                                              this))
                    {
                        igc->KillTreasureEvent(this);
                    }
                }
            }
            else if (pModel->GetAttributes() & c_mtStatic)
            {
                GetMyMission()->GetIgcSite()->KillTreasureEvent(this);
            }
        }

    // ItreasureIGC
        virtual TreasureCode    GetTreasureCode(void) const
        {
            return m_data.treasureCode;
        }

        virtual IbuyableIGC*   GetBuyable(void) const
        {
            return m_buyable;
        }
        virtual ObjectID       GetTreasureID(void) const
        {
            return m_data.treasureID;
        }
        virtual void            SetTreasureID(ObjectID newVal)
        {
            m_data.treasureID = newVal;
        }
        virtual short           GetAmount(void) const
        {
            return m_data.amount;
        }
        virtual void            SetAmount(short a)
        {
            m_data.amount = a;
        }
        virtual void            ResetExpiration(Time now)
        {
            m_expire = now + m_data.lifespan;
        }

        virtual void            SetCreateNow (void)
        {
            m_data.createNow = true;
        }

    private:
        DataTreasureIGC     m_data;
        IbuyableIGC*        m_buyable;
        Time                m_expire;
        float               m_speed0;

        bool                m_stationaryF;
};