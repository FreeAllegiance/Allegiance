/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	conditionalaction.h
**
**  Author:  
**
**  Description:
**      Header file for the training library "conditionalaction" interface.
**
**  History:
*/
#pragma once

#include "Condition.h"

#include "Action.h"

namespace Training
{
    //------------------------------------------------------------------------------
    // class definitions
    //------------------------------------------------------------------------------
    class ConditionalAction : public Condition
    {
        public:
                    enum    {IMMEDIATE_EXECUTION = true, DELAYED_EXECUTION = false};

        public:
                    /* void */          ConditionalAction (Condition* pcondition, Action* paction, bool bImmediateExecute = IMMEDIATE_EXECUTION, bool bRunOnce = false); //AEM 7.9.07 Added parameter to only allow the conditional action to be executed once
            virtual /* void */          ~ConditionalAction (void);
            virtual bool                Start (void);
            virtual void                Stop (void);
            virtual bool                Evaluate (void);

        protected:
                    Condition*          m_pCondition;
                    Action*             m_pAction;
                    bool                m_bImmediateExecute;
                    bool                m_bExecute;
					bool				m_bRunOnce;
					bool				m_bRanOnce;
    };

    //------------------------------------------------------------------------------
}