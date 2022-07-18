/*
**  Copyright (C) 1996, 1997 Microsoft Corporation. All Rights Reserved.
**
**  File:	CollisionQueue.cpp
**
**  Author: 
**
**  Description:
**
**  History:
*/
#include    "pch.h"

CollisionQueue::CollisionQueue(void)
:
    m_nCollisions(0),
    m_maxCollisions(c_maxHitTests * 20),  //? best guess
    m_fDelete(true)
{
    m_pCollisions = new CollisionEntry [m_maxCollisions];
    ZAssert (m_pCollisions);
}

CollisionQueue::CollisionQueue(int              maxCollisions,
                               CollisionEntry*  pCollisions)
:
    m_nCollisions(0),
    m_pCollisions(pCollisions),
    m_maxCollisions(maxCollisions),
    m_fDelete(false)
{
}

CollisionQueue::~CollisionQueue(void)
{
    if (m_fDelete)
        delete [] m_pCollisions;
}

void    CollisionQueue::sort(int    start)
{
    if ((m_nCollisions - 1) > start)
    {
        if (start == 0)
        {
            CollisionEntry::longSort(m_pCollisions,
                                     &m_pCollisions[m_nCollisions - 1]);
        }
        else
        {
            CollisionEntry::shortSort(&m_pCollisions[start],
                                      &m_pCollisions[m_nCollisions - 1]);
        }
    }
}

void    CollisionQueue::flush(int       n,
                              HitTest*  pHitTest1,
                              HitTest*  pHitTest2)
{
    ZAssert (n > 0);
    ZAssert (n <= m_nCollisions);

    //Cheat and temporarily mark pHitTest1 & 2 as dead
    bool    oldDead1;
    if (pHitTest1)
    {
        oldDead1 = pHitTest1->GetDeadF();
        pHitTest1->AddRef();
        pHitTest1->SetDeadF(true);
    }

    bool    oldDead2;
    if (pHitTest2)
    {
        oldDead2 = pHitTest2->GetDeadF();
        pHitTest2->AddRef();
        pHitTest2->SetDeadF(true);
    }

    float   t = m_pCollisions[n - 1].m_tCollision;
    int dest = n;
    for (int source = n; (source < m_nCollisions); source++)
    {
        CollisionEntry* pentrySource = &m_pCollisions[source];

        if (pentrySource->m_pHitTest1->GetDeadF() || pentrySource->m_pHitTest2->GetDeadF())
        {
            pentrySource->m_pHitTest1->Release();
            pentrySource->m_pHitTest2->Release();

            pentrySource->m_pHitTest1 = NULL;   //NYI shouldn't be needed
            pentrySource->m_pHitTest2 = NULL;
        }
        else
        {
            CollisionEntry* pentryDest = &m_pCollisions[dest++];

            pentryDest->m_pHitTest1  = pentrySource->m_pHitTest1;
            pentryDest->m_pHitTest2  = pentrySource->m_pHitTest2;

            pentryDest->m_hts1 = pentrySource->m_hts1;
            pentryDest->m_hts2 = pentrySource->m_hts2;

            pentryDest->m_tCollision = pentrySource->m_tCollision - t;
        }
    }

    if (pHitTest2)
    {
        pHitTest2->SetDeadF(oldDead2);
        pHitTest2->Release();
    }

    if (pHitTest1)
    {
        pHitTest1->SetDeadF(oldDead1);
        pHitTest1->Release();
    }

    m_nCollisions = dest;
}

void    CollisionQueue::purge(void)
{
    int i = m_nCollisions;
    while (--i >= 0)
    {
        CollisionEntry* pentry = &m_pCollisions[i];

        pentry->m_pHitTest1->Release();
        pentry->m_pHitTest2->Release();
    }

    m_nCollisions = 0;
}

void    CollisionQueue::addCollision(float          tCollision,
                                     HitTest*       pHitTest1,
                                     HitTestShape   hts1,
                                     HitTest*       pHitTest2,
                                     HitTestShape   hts2)
{
    ZAssert (hts1 <= pHitTest1->GetTrueShape());
    ZAssert (hts2 <= pHitTest2->GetTrueShape());

    ZAssert (m_nCollisions <= m_maxCollisions);
    if (m_nCollisions == m_maxCollisions)
    {
        m_maxCollisions = (m_maxCollisions << 1);
        debugf("Extending collision queue from %d to %d entries\n", m_nCollisions, m_maxCollisions);

        CollisionEntry* p = new CollisionEntry[m_maxCollisions];
        ZAssert (p);
        
        for (int i = 0; (i < m_nCollisions); i++)
        {
            p[i] = m_pCollisions[i];
        }

        delete [] m_pCollisions;
        m_pCollisions = p;
    }

    ZAssert (m_nCollisions < m_maxCollisions);

    ZAssert (pHitTest1);
    ZAssert (pHitTest2);

    pHitTest1->AddRef();
    pHitTest2->AddRef();

    CollisionEntry* pentry = &m_pCollisions[m_nCollisions++];

    pentry->m_tCollision = tCollision;

    pentry->m_pHitTest1 = pHitTest1;
    pentry->m_hts1 = hts1;
    pentry->m_pHitTest2 = pHitTest2;
    pentry->m_hts2 = hts2;
}
