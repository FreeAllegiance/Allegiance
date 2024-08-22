/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	predicate.h
**
**  Author:  
**
**  Description:
**      Header file for the training library "predicate" interface.
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
    class Predicate : public Condition
    {
        public:
                    /* void */          Predicate (Condition* pCondition, bool bExpectedValue);
            virtual /* void */          ~Predicate (void);
            virtual bool                Start (void);
            virtual void                Stop (void);
            virtual bool                Evaluate (void);

        protected:
                    Condition*          m_pCondition;
                    bool                m_bExpectedValue;
    };

    //------------------------------------------------------------------------------
}