/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	onetimeaction.h
**
**  Author:  
**
**  Description:
**      Header file for the training library "onetimeaction" interface.
**
**  History:
*/
#pragma once

#include "Action.h"

namespace Training
{
    //------------------------------------------------------------------------------
    // class definitions
    //------------------------------------------------------------------------------
    class OneTimeAction : public Action
    {
        public:
                    /* void */          OneTimeAction (Action* pAction);
            virtual /* void */          ~OneTimeAction (void);
            virtual void                Execute (void);
            virtual void                Stop (void);

        protected:
                    Action*             m_pAction;
                    bool                m_bHasExecuted;
    };

    //------------------------------------------------------------------------------
}