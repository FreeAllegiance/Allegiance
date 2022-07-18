/*
**  Copyright (C) 1996, 1997 Microsoft Corporation. All Rights Reserved.
**
**  File:	KDnode.cpp
**
**  Author: 
**
**  Description:
**
**  History:
*/
#include "pch.h"

//Allocate space for the node's hitTest and endpoint arrays
//(as a single allocation)
void    KDnode::allocHitTestsEndpoints(int n)
{
    if (m_maxHitTests < n)
    {
        //Not enough space ... realloc
        delete [] m_ppHitTests;
        void**  v = new void* [n * (1 + 2 * c_nAxes)];
        ZAssert (v);

        m_maxHitTests = n;
        m_ppHitTests = (HitTest**)v;
        m_ppEndpoints[0] = (Endpoint**)(v + n);

        for (int i = 1; (i < c_nAxes); i++)
            m_ppEndpoints[i] = m_ppEndpoints[i - 1] + (n << 1);
    }
}

KDnode::KDnode(KDnode*          parent)
:
    m_parent(parent),
    m_pivotAxis(-1),
    m_pivotValue(0.0f),
    m_nHitTests(0),
    m_maxHitTests(0),
    m_ppHitTests(NULL)
{
    m_ppEndpoints[0] = NULL;
    m_children[0] = m_children[1] = NULL;
}

KDnode::~KDnode(void)
{
    delete m_children[1];
    delete m_children[0];
    delete [] m_ppHitTests;
}

void    KDnode::reset(bool             highF)
{
    ZAssert (m_parent);

    //Set asside enough space for the same number of hitTests as the parent
    //(pessimistic, but fast)
    allocHitTestsEndpoints(m_parent->m_nHitTests);

    int     pivotAxis = m_parent->m_pivotAxis;
    ZAssert (pivotAxis >= 0);
    ZAssert (pivotAxis < c_nAxes);
    float   pivotValue = m_parent->m_pivotValue;

    //Extract and mark the subset of the parent's hitTests that belong to this node
    {
        m_nHitTests = 0;
        for (HitTest** ppHitTest = &m_parent->m_ppHitTests[m_parent->m_nHitTests - 1];
             (ppHitTest >= m_parent->m_ppHitTests);
             ppHitTest--)
        {
            HitTest* pHitTest = *ppHitTest;
            ZAssert (pHitTest);

            if (highF)
            {
                //Creating the high child ... anything extending above the pivot value is accepted
                if (pHitTest->m_boundingBox.axes[pivotAxis].values[1] > pivotValue)
                {
                    pHitTest->m_activeF = true;
                    m_ppHitTests[m_nHitTests++] = pHitTest;
                }
                else
                    pHitTest->m_activeF = false;
            }
            else
            {
                //Creating the low child ... anything extending below the pivot value is accepted
                if (pHitTest->m_boundingBox.axes[pivotAxis].values[0] < pivotValue)
                {
                    pHitTest->m_activeF = true;
                    m_ppHitTests[m_nHitTests++] = pHitTest;
                }
                else
                    pHitTest->m_activeF = false;
            }
        }

        ZAssert (m_nHitTests != 0);
        ZAssert (m_nHitTests != m_parent->m_nHitTests);
    }

    if (m_nHitTests >= c_minNodeSize)
    {
        //Generate the sorted endpoint lists for this node
        //use the fact that the parent's endpoint lists are sorted
        //and that the endpoint lists for this node will be a subset
        //(consisting of only the endpoints for the "active" hitTests)
        for (int axis = 0; (axis < c_nAxes); axis++)
        {
            //Work backwards to so the conditionals test vs. a non-equation.
            int thisID = (m_nHitTests << 1) - 1;

            //Note: the condition tests thisID so that the loop will stop as soon as
            //the endpoint list is filled (the remaining endpoints in the parent's
            //endpoint list must not be active).
            for (int parentID = (m_parent->m_nHitTests << 1) - 1;
                 (thisID >= 0);
                 parentID--)
            {
                ZAssert (parentID >= 0);

                HitTest* pHitTest = m_parent->m_ppEndpoints[axis][parentID]->pHitTest;
                if (pHitTest->m_activeF)
                    m_ppEndpoints[axis][thisID--] = m_parent->m_ppEndpoints[axis][parentID];
            }
            //There's no need to sort the endpoint list (since is was sorted in the parent).
        }

        pivot();
    }
    else
        m_pivotAxis = -1;
}

static bool findPivot(int               nHitTests,
                      Endpoint* const * ppEndpoints,
                      float*            pValue,
                      int*              pCost)
{
    ZAssert (nHitTests >= c_minNodeSize);
    ZAssert (ppEndpoints);
    ZAssert (pValue);
    ZAssert (pCost);

    bool    optimalF = false;

    int nEndpoints = nHitTests << 1;

    //costBias is used to guarantee that the pivot is worthwhile. Must be
    //in the range [nHitTests, nHitTests * 2].
    int costBias = (5 * nHitTests) >> 2;

    //This is used to generate the minCost (below) and test for the when it's time to quit
    int bestBaseCost = costBias - (nHitTests << 1);

    //Theoretical lowest cost for this node ... account for the fact that it is impossible to
    //perfectly balance a node with an odd number of hitTests
    int minCost = bestBaseCost + (nHitTests & 0x01);   //fast mod 2

    //The endpoints start with a low and end with a high, meaning we can skip the first endpoint
    //in the loop (hence the pre-increment) and only test for exit on the high endpoints.
    ZAssert (!ppEndpoints[0]->highF);
    ZAssert (ppEndpoints[(nHitTests << 1) - 1]->highF);

    int nLow = 0;               //Number of intervals completely below of the current endpoint
    int nHigh = nHitTests - 1;   //Number of intervales completely above the current endpoint

    //Only pivots with a cost less than zero will be considered by pivot, so set the best cost to
    //zero so the only the positives costs pivots will be ignored.
    float   bestValue = 0.0f;  // was 0, changed to 0.0f
    int     bestCost = 0;

    while (true)
    {
        ZAssert (nLow < nHitTests);
        const Endpoint* pEndpoint = *(++ppEndpoints);

        if (pEndpoint->highF)
        {
            //crossed a high endpoint, meaning there is one less
            //interval completely below the current point
            nLow++;

            //Calculate the unbalanced and totals here since this will give a lower
            //overall cost than calculating it before nLow is incremented.
            int unbalanced = nLow - nHigh;
            int baseCost = costBias - ((nLow + nHigh) << 1);

            //thisCost == base + |unbalanced| ... just do it fast
            int thisCost = (unbalanced >= 0)
                           ? (baseCost + unbalanced)
                           : (baseCost - unbalanced);

            if (thisCost < bestCost)
            {
                bestCost = thisCost;
                bestValue = pEndpoint->value;

                if (bestCost == minCost)
                {
                    optimalF = true;
                    break;
                }
            }
            else if ((bestCost <= bestBaseCost + unbalanced) ||
                     (nLow == nHitTests))
            {
                //Since unbalanced only increases, we can give up when the best cost
                //is <= the best cost we can hope to achieve

                //we can also give up if we've reached the end of the endpoint list
                break;
            }

        }
        else
        {
            //No need to calculate costs ... either the previous endpoint was a right
            //endpoint (and the cost wouldn't change since nHigh would be decremented
            //after caclulating the cost) or it was a left endpoint (in which case the
            //cost could only be higher than it was before).

            //Crossed a low endpoint, meaning there is one less
            //interval completely above the current point
            nHigh--;
        }
    }

    *pCost = bestCost;
    *pValue = bestValue;

    return optimalF;
}

void    KDnode::pivot(void)
{
    //Select the best pivot axis and value for this node
    //based on the endpoint data

    ZAssert (m_nHitTests >= c_minNodeSize);
    //Rotate the axes to encourage (in the event of a tie for best cost)
    //that children pivot on different axes than their parents.
    int     startAxis = m_parent ? ((m_parent->m_pivotAxis + 1) % c_nAxes) : 0;

    int     bestAxis = startAxis;
    float   bestValue;
    int     bestCost;

    bool optimalF = findPivot(m_nHitTests,
                              m_ppEndpoints[bestAxis],
                              &bestValue,
                              &bestCost);

    for (int i = 1; ((!optimalF) && (i < c_nAxes)); i++)
    {
        int     axis = (i + startAxis) % c_nAxes;
        float   value;
        int     cost;
        
        optimalF = findPivot(m_nHitTests,
                             m_ppEndpoints[axis],
                             &value, &cost);

        if (cost < bestCost)
        {
            bestAxis = axis;
            bestValue = value;
            bestCost = cost;
        }
    }

    if (bestCost < 0)
    {
        //Found a pivot value that did some good ... repivot on the new pivot point
        m_pivotAxis = bestAxis;
        m_pivotValue = bestValue;

        if (!m_children[0])
        {
            ZAssert (!m_children[1]);

            //This node has no children ... create some
            m_children[0] = new KDnode(this);
            m_children[1] = new KDnode(this);
        }

        ZAssert (m_children[0] && m_children[1]);

        //Reset both children based on the new pivot axis/value.
        m_children[0]->reset(false);
        m_children[1]->reset(true);
    }
    else
        m_pivotAxis = -1;
}

void   KDnode::test(HitTest*        pHitTest1,
                    CollisionQueue* pQueue) const
{
    if (m_pivotAxis >= 0)
    {
        //Pass the hit tests along to one or both children
        const Interval&  interval = pHitTest1->m_boundingBox.axes[m_pivotAxis];

        if (interval.values[0] < m_pivotValue)
            m_children[0]->test(pHitTest1, pQueue);

        if (interval.values[1] > m_pivotValue)
            m_children[1]->test(pHitTest1, pQueue);
    }
    else
    {
        //this is a terminal node: test bb against each contained hitTest's bounding box
        for (HitTest** ppHitTest = &m_ppHitTests[m_nHitTests - 1];
             (ppHitTest >= m_ppHitTests);
             ppHitTest--)
        {
            HitTest*    pHitTest2 = *ppHitTest;
            if ((pHitTest1->m_id < pHitTest2->m_id) &&
                (pHitTest2->m_lastTest != pHitTest1) &&
                (pHitTest1->m_noHit != pHitTest2) &&
                (pHitTest2->m_noHit != pHitTest1))
            {
                pHitTest2->m_lastTest = pHitTest1;
                pHitTest1->Collide(pHitTest2, pQueue);
            }
        }
    }
}
