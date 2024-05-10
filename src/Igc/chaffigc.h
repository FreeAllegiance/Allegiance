/*
**  Copyright (C) 1996, 1997 Microsoft Corporation. All Rights Reserved.
**
**  File:	chaffIGC.h
**
**  Author: 
**
**  Description:
**      Header for the CchaffIGC class. This file was initially created by
**  the ATL wizard.
**
**  History:
*/
// chaffIGC.h : Declaration of the CchaffIGC

#pragma once

#include "igc.h"
#include "modelIGC.h"

class CchaffIGC : public TmodelIGC<IchaffIGC>
{
    public:
        CchaffIGC(void);
        ~CchaffIGC(void);

    public:
    // IbaseIGC
	    virtual HRESULT             Initialize(ImissionIGC* pMission, Time now, const void* data, int dataSize);
	    virtual void                Terminate(void);
        virtual void                Update(Time now);

        virtual ObjectType          GetObjectType(void) const
        {
            return OT_chaff;
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

            TmodelIGC<IchaffIGC>::SetCluster(cluster);

            if (cluster)
                cluster->AddModel(this);

            Release();
        }

        virtual void                 SetVisibleF(bool   vf) const
        {
        }

        virtual bool                 GetVisibleF(void) const
        {
            return true;
        }
        virtual void                 SetRender(unsigned char render)
        {
        }

    // IchaffIGC

    private:
        DataChaffTypeIGC*  m_pChaffTypeData;
        Time               m_timeExpire;
};