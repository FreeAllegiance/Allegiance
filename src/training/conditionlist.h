/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	conditionlist.h
**
**  Author:  
**
**  Description:
**      Header file for the training library "conditionlist" interface.
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
    class ConditionList : public Condition
    {
        public:
            virtual /* void */          ~ConditionList (void);
            virtual bool                Start (void);
            virtual void                Stop (void);
            virtual bool                Evaluate (void);
                    void                AddCondition (Condition* pCondition);

        protected:
                    std::list<Condition*>   m_conditionList;
    };

    //------------------------------------------------------------------------------
}