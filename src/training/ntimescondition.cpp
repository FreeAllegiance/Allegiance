/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	ntimescondition.cpp
**
**  Author:  
**
**  Description:
**      Implementation of the training library "ntimescondition" interface.
**
**  History:
*/
#include    "pch.h"
#include    "NTimesCondition.h"

namespace Training
{
    //------------------------------------------------------------------------------
    // class methods
    //------------------------------------------------------------------------------
    /* void */  NTimesCondition::NTimesCondition (Condition* pCondition, unsigned int iTimes, bool bSticky) :
    m_pCondition (pCondition),
    m_iTimes (iTimes),
    m_bSticky (bSticky)
    {
        ZAssert (iTimes > 1);
    }

    //------------------------------------------------------------------------------
    /* void */  NTimesCondition::~NTimesCondition (void)
    {
        delete m_pCondition;
    }

    //------------------------------------------------------------------------------
    bool        NTimesCondition::Start (void)
    {
        m_iCount = m_pCondition->Start () ? 1 : 0;
        return false;
    }

    //------------------------------------------------------------------------------
    void        NTimesCondition::Stop (void)
    {
        m_pCondition->Stop ();
    }

    //------------------------------------------------------------------------------
    bool        NTimesCondition::Evaluate (void)
    {
        if (m_iCount < m_iTimes)
            m_iCount += m_pCondition->Evaluate () ? 1 : 0;
        if (m_iCount == m_iTimes)
        {
            m_iCount = m_bSticky ? m_iCount : 0;
            return true;
        }
        return false;
    }

    //------------------------------------------------------------------------------
}
