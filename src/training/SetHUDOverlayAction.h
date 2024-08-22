/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	SetHUDOverlayAction.h
**
**  Author:  
**
**  Description:
**      Header file for the training library "SetHUDOverlayAction" interface.
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
    class SetHUDOverlayAction : public Action
    {
        public:
                    /* void */          SetHUDOverlayAction (TrainingOverlay overlay);
            virtual /* void */          ~SetHUDOverlayAction (void);
            virtual void                Execute (void);

        protected:
                    TrainingOverlay     m_overlay;
    };

    //------------------------------------------------------------------------------
}