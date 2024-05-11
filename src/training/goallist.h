/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	goallist.h
**
**  Author:  
**
**  Description:
**      Header file for the training library "goallist" interface.
**
**  History:
*/
#pragma once

#include "Goal.h"

namespace Training
{
    //------------------------------------------------------------------------------
    // class definitions
    //------------------------------------------------------------------------------
    class GoalList : public Condition
    {
        public:
                    /* void */          GoalList (void);
            virtual /* void */          ~GoalList (void);
            virtual bool                Start (void);
            virtual void                Stop (void);
            virtual bool                Evaluate (void);
                    void                AddGoal (Goal* pGoal);

        protected:
                    std::list<Goal*>            m_goalList;
                    std::list<Goal*>::iterator  m_iterator;
    };

    //------------------------------------------------------------------------------
}