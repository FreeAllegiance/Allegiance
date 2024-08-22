/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	RepairRearmAction.h
**
**  Author:  
**
**  Description:
**      Header file for the training library "RepairRearmAction" interface.
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
    class RepairRearmAction : public Action
    {
        public:
            virtual /* void */          ~RepairRearmAction (void);
            virtual void                Execute (void);
    };

    //------------------------------------------------------------------------------
}