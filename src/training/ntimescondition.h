/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	ntimescondition.h
**
**  Author:  
**
**  Description:
**      Header file for the training library "ntimescondition" interface.
**
**  History:
*/
#pragma once

#include "Condition.h"

namespace Training
{
    //------------------------------------------------------------------------------
    // class definitions
    //------------------------------------------------------------------------------
    class NTimesCondition : public Condition
    {
        public:
                    /* void */          NTimesCondition (Condition* pCondition, unsigned int iTimes, bool bSticky = false);
            virtual /* void */          ~NTimesCondition (void);
            virtual bool                Start (void);
            virtual void                Stop (void);
            virtual bool                Evaluate (void);

        protected:
                    Condition*          m_pCondition;
                    unsigned int        m_iTimes;
                    unsigned int        m_iCount;
                    bool                m_bSticky;
    };

    //------------------------------------------------------------------------------
}