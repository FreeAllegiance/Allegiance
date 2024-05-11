/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	setcontrolconstraintsaction.h
**
**  Author:  
**
**  Description:
**      Header file for the training library "setcontrolconstraintsaction" interface.
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
    class SetControlConstraintsAction : public Action
    {
        public:
                    /* void */          SetControlConstraintsAction (void);
            virtual /* void */          ~SetControlConstraintsAction (void);
            virtual void                Execute (void);
                    void                EnableInputAction (int iInputAction);
                    void                DisableInputAction (int iInputAction);
                    void                ScaleInputControl (Axis axis, float fScalar);

        protected:
                    ControlData             m_inputControls;
                    int                     m_iEnableActionsMask;
                    int                     m_iDisableActionsMask;
    };

    //------------------------------------------------------------------------------
}