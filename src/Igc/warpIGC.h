/*
**  Copyright (C) 1996, 1997 Microsoft Corporation. All Rights Reserved.
**
**  File:	warpIGC.h
**
**  Author: 
**
**  Description:
**      Header for the CwarpIGC class. This file was initially created by
**  the ATL wizard.
**
**  History:
*/
// warpIGC.h : Declaration of the CwarpIGC

#ifndef __WARPIGC_H_
#define __WARPIGC_H_

#include "igc.h"
#include "modelIGC.h"

class CwarpIGC : public TmodelIGC<IwarpIGC>
{
    public:
        CwarpIGC(void)
        :
            m_destination(NULL),
			m_bFixedPosition(false) // KG- added
        {
        }

        ~CwarpIGC(void)
        {
        }

    // IbaseIGC
	    virtual HRESULT Initialize(ImissionIGC* pMission, Time now, const void* data, int length);
	    virtual void    Terminate(void)
        {
            AddRef();
            GetMyMission()->DeleteWarp(this);
            TmodelIGC<IwarpIGC>::Terminate();

            if (m_destination)
            {
                m_destination->Release();
                m_destination = NULL;
            }

            m_bombs.purge();

            Release();
        }
        virtual void            Update(Time   now)
        {
            WarpBombLink*   plink;

            while ((plink = m_bombs.first()) &&               //Intentional =
                   (plink->data().timeExplosion <= now))
            {
                ImissileTypeIGC*    pmt = plink->data().pmt;
                ZAssert (pmt);
                ZAssert (pmt->GetObjectType() == OT_missileType);

                DamageTypeID    dtid = pmt->GetDamageType();
                float           p    = pmt->GetPower();
                float           r    = pmt->GetBlastRadius();
				
                IclusterIGC*    pcluster = GetCluster();
				if (pmt->HasCapability(c_eabmWarpBombDual)) // KGJV- only Dual explode this side
                pcluster->CreateExplosion(dtid,
                                          p,
                                          r,
                                          c_etBigShip,
                                          plink->data().timeExplosion,
                                          GetPosition(),
                                          NULL);

                IwarpIGC*   pwarp = GetDestination();
                pcluster = pwarp->GetCluster();
                pcluster->CreateExplosion(dtid,
                                          p,
                                          r,
                                          c_etBigShip,
                                          plink->data().timeExplosion,
                                          pwarp->GetPosition(),
                                          NULL);

                delete plink;
            }

            TmodelIGC<IwarpIGC>::Update(now);
        }

        virtual int                 Export(void* data) const;

        virtual ObjectType          GetObjectType(void) const
        {
            return OT_warp;
        }

	    virtual ObjectID            GetObjectID(void) const
        {
            return m_warpDef.warpID;
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
                    c->DeleteWarp(this);
            }

            TmodelIGC<IwarpIGC>::SetCluster(cluster);

            if (cluster)
                cluster->AddWarp(this);

            Release();
        }

    // IwarpIGC
	    virtual IwarpIGC*       GetDestination(void)
        {
            if (!m_destination)
            {
                //Do a lazy evaluation of the destination so that all warps do not
                //need to be defined before defining any warps
                m_destination = GetMyMission()->GetWarp(m_warpDef.destinationID);
                if (m_destination)
                    m_destination->AddRef();
            }

            return m_destination;
        }

        virtual void         AddBomb(Time               timeExplosion,
                                     ImissileTypeIGC*   pmt)
        {
            WarpBombLink*   p = new WarpBombLink;
            p->data().timeExplosion = timeExplosion;
            p->data().pmt = pmt;

            // add the pulse effects
            Color       blastColor (0.6f, 0.8f, 1.0f);
            float       fExplodeTime = timeExplosion - Time::Now ();
            GetCluster ()->GetClusterSite ()->AddPulse (fExplodeTime, GetPosition (), pmt->GetBlastRadius (), blastColor);
            IwarpIGC*   pDestination = GetDestination();
            pDestination->GetCluster ()->GetClusterSite ()->AddPulse (fExplodeTime, m_destination->GetPosition (), pmt->GetBlastRadius (), blastColor);

            // this should tell the aleph rendering site about the pulse
            ThingSite*  pThingSite = GetThingSite ();
            pThingSite->AddPulse (fExplodeTime, GetPosition (), pmt->GetBlastRadius (), blastColor);
            pThingSite = pDestination->GetThingSite ();
            pThingSite->AddPulse (fExplodeTime, GetPosition (), pmt->GetBlastRadius (), blastColor);

            m_bombs.last(p);
        }

        virtual const WarpBombList* GetBombs(void) const
        {
            return &m_bombs;
        }
		// KG - added 
		virtual bool IsFixedPosition()
		{
			return m_bFixedPosition;
		}
		//Andon: Added for aleph mass limits
		virtual double MassLimit()
		{
			return m_MassLimit;
		}

    private:
        IwarpIGC*           m_destination;
        WarpDef             m_warpDef;
        WarpBombList        m_bombs;
		bool				m_bFixedPosition; // KG - added to prevent randomization
		double				m_MassLimit; //Andon: Added for aleph mass limts
};

#endif //__WARPIGC_H_
