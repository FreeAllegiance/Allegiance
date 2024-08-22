/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	condition.h
**
**  Author:  
**
**  Description:
**      Header file for the training library "condition" interface.
**
**  History:
*/
#pragma once

namespace Training
{
    //------------------------------------------------------------------------------
    // class definitions
    //------------------------------------------------------------------------------
    class Condition
    {
        public:
            virtual /* void */          ~Condition (void);
            virtual bool                Start (void);
            virtual void                Stop (void);
            virtual bool                Evaluate (void) = 0;
    };

    //------------------------------------------------------------------------------
}