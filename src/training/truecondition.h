/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	truecondition.h
**
**  Author:  
**
**  Description:
**      Header file for the training library "truecondition" interface.
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
    class TrueCondition : public Condition
    {
        public:
            virtual /* void */          ~TrueCondition (void);
            virtual bool                Start (void);
            virtual bool                Evaluate (void);
    };

    //------------------------------------------------------------------------------
}