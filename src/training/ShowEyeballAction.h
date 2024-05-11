/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	ShowEyeballAction.h
**
**  Author:  
**
**  Description:
**      Header file for the training library "ShowEyeballAction" interface.
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
    class ShowEyeballAction : public Action
    {
        public:
                    /* void */          ShowEyeballAction (bool showEyeball = false);
            virtual /* void */          ~ShowEyeballAction (void);
            virtual void                Execute (void);

        protected:
                    bool                m_showEyeball;
    };

    //------------------------------------------------------------------------------
}