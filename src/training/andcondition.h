/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	andcondition.h
**
**  Author:  
**
**  Description:
**      Header file for the training library "andcondition" interface.
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
    class AndCondition : public Condition
    {
        public:
                    /* void */          AndCondition (Condition* pConditionA, Condition* pConditionB);
            virtual /* void */          ~AndCondition (void);
            virtual bool                Start (void);
            virtual void                Stop (void);
            virtual bool                Evaluate (void);

        protected:
                    Condition*          m_pConditionA;
                    Condition*          m_pConditionB;
    };

    //------------------------------------------------------------------------------
}