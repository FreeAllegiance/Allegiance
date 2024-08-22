/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	SetControlsAction.h
**
**  Author:  
**
**  Description:
**      Header file for the training library "SetControlsAction" interface.
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
    class SetControlsAction : public Action
    {
        public:
                    /* void */          SetControlsAction (void);
            virtual /* void */          ~SetControlsAction (void);
            virtual void                Execute (void);
                    void                SetInputAction (int inputAction, int iMask = 0);
                    void                SetInputControl (Axis axis, float fScalar);

        protected:
                    ControlData         m_inputControls;
                    int                 m_iInputAction;
    };

    //------------------------------------------------------------------------------
}