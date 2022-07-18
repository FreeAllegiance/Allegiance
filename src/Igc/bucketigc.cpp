/*
**  Copyright (C) 1996, 1997 Microsoft Corporation. All Rights Reserved.
**
**  File:	bucketIGC.cpp
**
**  Author: 
**
**  Description:
**      Implementation of the CbucketIGC class. This file was initially created by
**  the ATL wizard for the core object.
**
**  History:
*/
// bucketIGC.cpp : Implementation of CbucketIGC
#include "bucketigc.h"

/////////////////////////////////////////////////////////////////////////////
// CbucketIGC
HRESULT         CbucketIGC::Initialize(ImissionIGC* pMission,
                                       Time         now,
                                       const void*  data,
                                       int          dataSize)
{
    ZAssert (pMission);
    m_pMission = pMission;

	ZRetailAssert (data && (dataSize == sizeof(DataBucketIGC)));
    m_data = *((DataBucketIGC*)data);

    ZAssert (m_data.side);
    m_data.side->AddRef();

    m_price = m_data.buyable->GetPrice();
    m_timeToBuild = m_data.buyable->GetTimeToBuild();

    //See if there is a predecessor bucket
    if (m_data.buyable->GetObjectType() == OT_development)
    {
        IdevelopmentIGC*    pdevelopment = (IdevelopmentIGC*)(m_data.buyable);
        if ((!pdevelopment->GetTechOnly()) && pdevelopment->GetEffectTechs().GetAllZero())
        {
            //A tech that affects only a side's global attributes.
            //Check for other buckets which do exactly the same thing
            Money                       price = pdevelopment->GetPrice();
            const GlobalAttributeSet&   gas = pdevelopment->GetGlobalAttributeSet();

            //Go backwards to get the last possible development
            for (BucketLinkIGC* pbl = m_data.side->GetBuckets()->last(); (pbl != NULL); pbl = pbl->txen())
            {
                IbucketIGC*     pbucket = pbl->data();
                IbuyableIGC*    pbuyable = pbucket->GetBuyable();
                if (pbuyable->GetObjectType() == OT_development)
                {
                    IdevelopmentIGC*    pd = (IdevelopmentIGC*)pbuyable;
                    if ((!pd->GetTechOnly()) && (pd->GetPrice() <= price) && pd->GetEffectTechs().GetAllZero())
                    {
                        //Look at that ... a equally or less expensive development that affects only global attributes
                        //do they affect the same global attributes the same way?
                        if (pd->GetGlobalAttributeSet() == gas)
                        {
                            pbucket->AddRef();
                            m_pbucketPredecessor = pbucket;
                            break;
                        }
                    }
                }
            }
        }

        if (pdevelopment->GetObjectID() != c_didTeamMoney)
        {
            m_price = (Money)(m_price * m_data.side->GetGlobalAttributeSet().GetAttribute(c_gaDevelopmentCost));
            m_timeToBuild = (DWORD)(m_timeToBuild * m_data.side->GetGlobalAttributeSet().GetAttribute(c_gaDevelopmentTime));
        }
    }

    m_data.side->AddBucket(this);

    ZAssert (m_data.buyable);
    m_data.buyable->AddRef();

    m_lastUpdate = now;

    return S_OK;
}

void            CbucketIGC::Terminate(void)
{
    AddRef();

    ZAssert (m_data.side);

    if (m_pbucketPredecessor)
    {
        m_pbucketPredecessor->Release();
        m_pbucketPredecessor = NULL;
    }
    m_pMission->GetIgcSite()->BucketChangeEvent(c_bcTerminated, this);

    ZAssert (m_data.side);
    m_data.side->DeleteBucket(this);
    m_data.side->Release();
    m_data.side = NULL;

    ZAssert (m_data.buyable);
    m_data.buyable->Release();
    m_data.buyable = NULL;

    Release();
}

int             CbucketIGC::Export(void*  data) const
{
    return NA;
}
