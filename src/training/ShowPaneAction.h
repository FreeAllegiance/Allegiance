/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	ShowPaneAction.h
**
**  Author:  
**
**  Description:
**      Header file for the training library "ShowPaneAction" interface.
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
    class ShowPaneAction : public Action
    {
        public:
                    /* void */          ShowPaneAction (OverlayMask overlayMask = 0);
            virtual /* void */          ~ShowPaneAction (void);
            virtual void                Execute (void);

        protected:
                    OverlayMask         m_overlayMask;
    };

    //------------------------------------------------------------------------------
}