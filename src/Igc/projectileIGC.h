/*
**  Copyright (C) 1996, 1997 Microsoft Corporation. All Rights Reserved.
**
**  File:	projectileIGC.h
**
**  Author: 
**
**  Description:
**      Header for the CprojectileIGC class. This file was initially created by
**  the ATL wizard.
**
**  History:
*/
// projectileIGC.h : Declaration of the CprojectileIGC

#ifndef __PROJECTILEIGC_H_
#define __PROJECTILEIGC_H_

#include "igc.h"
#include "modelIGC.h"

class CprojectileIGC : public TmodelIGC<IprojectileIGC>
{
    public:
        CprojectileIGC(void);
        ~CprojectileIGC(void);

    public:
    // IbaseIGC
	    virtual HRESULT             Initialize(ImissionIGC* pMission, Time now, const void* data, int dataSize);
	    virtual void                Terminate(void);
        virtual void                Update(Time now);
        virtual int                 Export(void* data);

        virtual ObjectType          GetObjectType(void) const
        {
            return OT_projectile;
        }

    // ImodelIGC
        virtual void                SetCluster(IclusterIGC* cluster)
        {
            AddRef();

            {
                IclusterIGC*    c = GetCluster();
                if (c)
                    c->DeleteModel(this);
            }

            TmodelIGC<IprojectileIGC>::SetCluster(cluster);

            if (cluster)
                cluster->AddModel(this);

            Release();
        }

        virtual void                HandleCollision(Time       timeCollision,
                                                    float                  tCollision,
                                                    const CollisionEntry&  entry,
                                                    ImodelIGC* pModel);

    // IprojectileIGC
        virtual IprojectileTypeIGC* GetProjectileType(void) const
        {
            return m_projectileType;
        }
        virtual ImodelIGC*           GetLauncher(void) const
        {
            return m_launcher;
        }
        virtual void                SetLauncher(ImodelIGC* newVal)
        {
            ZAssert (!m_launcher);       //Only set once
            ZAssert (newVal);

            newVal->AddRef();
            m_launcher = newVal;

            GetHitTest()->SetNoHit(m_launcher->GetHitTest());
            SetSide(m_launcher->GetSide());
        }
        virtual void                SetGunner(IshipIGC* newVal)
        {
            ZAssert (!m_launcher);       //Only set once
            ZAssert (newVal);

            newVal->AddRef();
            m_launcher = newVal;

            GetHitTest()->SetNoHit(newVal->GetParentShip()->GetHitTest());
            SetSide(m_launcher->GetSide());
        }

    private:
        IprojectileTypeIGC* m_projectileType;
        ImodelIGC*          m_launcher;
        ExplosionData*      m_pExplosionData;
        Time                m_timeExpire;
};

#endif //__PROJECTILEIGC_H_
