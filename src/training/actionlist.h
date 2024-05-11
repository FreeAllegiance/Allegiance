/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	actionlist.h
**
**  Author:  
**
**  Description:
**      Header file for the training library "actionlist" interface.
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
    class ActionList : public Action
    {
        public:
                    /* void */          ActionList (void);
            virtual /* void */          ~ActionList (void);
            virtual void                Execute (void);
            virtual void                Stop (void);
                    void                AddAction (Action* pAction);

        protected:
                    std::list<Action*>              m_actionList;
    };

    //------------------------------------------------------------------------------
}