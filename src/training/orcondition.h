/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	orcondition.h
**
**  Author:  
**
**  Description:
**      Header file for the training library "orcondition" interface.
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
    class OrCondition : public Condition
    {
        public:
                    /* void */          OrCondition (Condition* pConditionA, Condition* pConditionB);
            virtual /* void */          ~OrCondition (void);
            virtual bool                Start (void);
            virtual void                Stop (void);
            virtual bool                Evaluate (void);

        protected:
                    Condition*          m_pConditionA;
                    Condition*          m_pConditionB;
    };

    //------------------------------------------------------------------------------
}