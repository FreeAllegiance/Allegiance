/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	setdisplaymodeaction.h
**
**  Author:  
**
**  Description:
**      Header file for the training library "setdisplaymodeaction" interface.
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
    class SetDisplayModeAction : public Action
    {
        public:
                    /* void */          SetDisplayModeAction (TrekWindow::CameraMode mode);
            virtual /* void */          ~SetDisplayModeAction (void);
            virtual void                Execute (void);

        protected:
                    TrekWindow::CameraMode  m_mode;
    };

    //------------------------------------------------------------------------------
}