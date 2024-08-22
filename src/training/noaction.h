/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	noaction.h
**
**  Author:  
**
**  Description:
**      Header file for the training library "noaction" interface.
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
    class NoAction : public Action
    {
        public:
            virtual /* void */          ~NoAction (void);
            virtual void                Execute (void);
    };

    //------------------------------------------------------------------------------
}