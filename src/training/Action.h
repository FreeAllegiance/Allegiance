/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	action.h
**
**  Author:  
**
**  Description:
**      Header file for the training library "action" interface.
**
**  History:
*/
#pragma once

namespace Training
{
    //------------------------------------------------------------------------------
    // class definitions
    //------------------------------------------------------------------------------
    class Action
    {
        public:
            virtual /* void */          ~Action (void);
            virtual void                Execute (void) = 0;
            virtual void                Stop (void);
    };

    //------------------------------------------------------------------------------
}