/*
**  Copyright (C) 1996, 1997 Microsoft Corporation. All Rights Reserved.
**
**  File:	KDroot.cpp
**
**  Author: 
**
**  Description:
**
**  History:
*/
#include "pch.h"

KDroot::KDroot(bool bStatic)
:
    KDnode(NULL),
    m_nAdded(0),
    m_nDeleted(0),
    m_bStatic(bStatic)
{
    //allocate enough space for everything we'll ever want
    allocHitTestsEndpoints((c_maxHitTests * 3) >> 1);
}

KDroot::~KDroot(void)
{
    //everything should be marked dead before the root destructor is called.
    flush();
    ZAssert (m_nHitTests == 0);
}

void    KDroot::addHitTest(HitTest*   pHitTest)
{
    ZAssert (m_nHitTests <= m_maxHitTests);

    //Not allowed to add a node that was marked dead but not yet pruned.
    //Danger: we are performing an operation on another KDRoot which,
    //theoretically, could have been freed. The only reason it works is
    //that -- if it were freed -- then it would have been flushed and all
    //of the hittests that refered back to the it would have had their
    //KDRoot's cleared.
    if (pHitTest->GetKDRoot() != NULL)
        pHitTest->GetKDRoot()->flush();

    ZAssert (pHitTest->GetKDRoot() == NULL);

    pHitTest->SetKDRoot(this);
    pHitTest->SetDeadF(false);
    pHitTest->SetDeletedF(false);

    if (m_nHitTests == m_maxHitTests)
    {
        //Need more space but, unlike a KDnode, we can't simply nuke the
        //object/endpoint arrays: we need to create, copy and then nuke.
        ZAssert (m_ppHitTests);

        //Double the existing space
        int n = m_maxHitTests << 1;
        void**  v = new void* [n * (1 + 2 * c_nAxes)];
        ZAssert (v);

        m_maxHitTests = n;
        for (int i = 0; (i < c_nAxes); i++)
        {
            Endpoint**  pOldEndpoints = m_ppEndpoints[i];
            m_ppEndpoints[i] = (i == 0)
                               ? ((Endpoint**)(v + n))
                               : m_ppEndpoints[i - 1] + (m_maxHitTests << 1);

            for (int j = 0; (j < (m_nHitTests << 1)); j++)
                m_ppEndpoints[i][j] = pOldEndpoints[j];
        }
        {
            HitTest**    ppOldHitTests = m_ppHitTests;
            m_ppHitTests = (HitTest**)v;

            for (int i = 0; (i < m_nHitTests); i++)
                m_ppHitTests[i] = ppOldHitTests[i];

            delete ppOldHitTests;
        }
    }
    ZAssert (m_nHitTests < m_maxHitTests);

    if (m_bStatic)
        pHitTest->UpdateStaticBB();

    //Add a reference to the hitTest's data object
    pHitTest->AddRef();

    //Add the object to the list of 
    m_ppHitTests[m_nHitTests] = pHitTest;

    //create and sort the endpoint lists for each axis.
    {
        int ht2 = m_nHitTests << 1;

        for (int axis = 0; (axis < c_nAxes); axis++)
        {
            m_ppEndpoints[axis][ht2    ] = &pHitTest->m_endpoints[axis][0];
            m_ppEndpoints[axis][ht2 + 1] = &pHitTest->m_endpoints[axis][1];
        }
    }
    
    m_nHitTests++;

    //Increment the changed count (which determines which sort is used in the update).
    m_nAdded++;
}

void    KDroot::deleteHitTest(HitTest*    pHitTest)
{
    //Actually defer the delete until the next update
    m_nDeleted++;
    pHitTest->SetDeadF(true);
    pHitTest->SetDeletedF(true);

    //If we are deleting a hit test ... it must preserve the pointer to the old hit test
    ZAssert (pHitTest->GetKDRoot() == this);
}

void    KDroot::flush(void)
{
    if (m_nDeleted > 0)
    {
        //One or more objects were deleted ... scan through the object and endpoint arrays, nuking the dead objects.
        //Endpoint arrays first
        for (int axis = 0; (axis < c_nAxes); axis++)
        {
            int dest = 0;
            for (int source = 0; (source < (m_nHitTests << 1)); source++)
            {
                if (!m_ppEndpoints[axis][source]->pHitTest->m_deletedF)
                {
                    m_ppEndpoints[axis][dest++] = m_ppEndpoints[axis][source];
                }
            }
        }

        {
            //Do the same for the objects ... but also release the object as well
            int dest = 0;
            for (int source = 0; (source < m_nHitTests); source++)
            {
                HitTest*    pht = m_ppHitTests[source];
                if (pht->m_deletedF)
                {
                    pht->SetKDRoot(NULL);   //We are not allowed to add this hit test to another KD tree until after this point
                                            //so mark the node as not being dead (and acceptable to add) here.

                    pht->Release();
                }
                else
                {
                    m_ppHitTests[dest++] = pht;
                }
            }

            m_nHitTests = dest;
        }

        m_nDeleted = 0;
    }
}

void    KDroot::update(void)
{
    //Only works from the root
    ZAssert (!m_parent);

    bool    bSort = (!m_bStatic) || (m_nAdded > 0);

    if (bSort || ((m_nDeleted << 4) > m_nHitTests))
    {
        flush();
        if (m_nHitTests >= c_minRootSize)
        {
            if (bSort)
            {
                //Resort the list of endpoints.
                //Use the short or long sort depending on how things have changed
                for (int    axis = 0; (axis < c_nAxes); axis++)
                {
                    if ((m_nAdded << 2) > m_nHitTests)
                    {
                        //Lots of things have changed ... use the long sort.
                        Endpoint::longSort(&m_ppEndpoints[axis][0],
                                           &m_ppEndpoints[axis][(m_nHitTests << 1) - 1]);
                    }
                    else
                    {
                        //Only minor changes ... use the short sort.
                        Endpoint::shortSort(&m_ppEndpoints[axis][0],
                                            &m_ppEndpoints[axis][(m_nHitTests << 1) - 1]);
                    }
                }
                m_nAdded = 0;
            }

            pivot();
        }
        else
            m_pivotAxis = -1;
    }
}