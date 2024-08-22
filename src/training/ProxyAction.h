/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	proxyaction.h
**
**  Author:  
**
**  Description:
**      Header file for the training library "proxyaction" interface.
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
    class ProxyAction : public Action
    {
        public:
                    /* void */          ProxyAction (Action* pAction);
            virtual /* void */          ~ProxyAction (void);
            virtual void                Execute (void);

        protected:
                    Action*             m_pAction;
    };

    //------------------------------------------------------------------------------
}