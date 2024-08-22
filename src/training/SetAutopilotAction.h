/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	setautopilotaction.h
**
**  Author:  
**
**  Description:
**      Header file for the training library "setautopilotaction" interface.
**
**  History:
*/
#pragma once

#include "Action.h"

#include "AbstractTarget.h"

namespace Training
{
    //------------------------------------------------------------------------------
    // class definitions
    //------------------------------------------------------------------------------
    class SetAutopilotAction : public Action
    {
        public:
                    /* void */          SetAutopilotAction (IshipIGC* pShip, bool bNewValue = true);
                    /* void */          SetAutopilotAction (ShipID shipID, bool bNewValue = true);
            virtual /* void */          ~SetAutopilotAction (void);
            virtual void                Execute (void);

        protected:
                    AbstractTarget*     m_pShip;
                    bool                m_bNewValue;
    };

    //------------------------------------------------------------------------------
}