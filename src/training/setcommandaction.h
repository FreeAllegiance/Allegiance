/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	setcommandaction.h
**
**  Author:  
**
**  Description:
**      Header file for the training library "setcommandaction" interface.
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
    class SetCommandAction : public Action
    {
        public:
                    /* void */          SetCommandAction (IshipIGC* pShip, Command command, ImodelIGC* pTarget, CommandID commandID);
                    /* void */          SetCommandAction (IshipIGC* pShip, Command command, ObjectType targetType, ObjectID targetID, CommandID commandID);
                    /* void */          SetCommandAction (ShipID shipID, Command command, ObjectType targetType, ObjectID targetID, CommandID commandID);
                    /* void */          SetCommandAction (ShipID shipID, Command command, ImodelIGC* pTarget, CommandID commandID);
            virtual /* void */          ~SetCommandAction (void);
            virtual void                Execute (void);

        protected:
                    AbstractTarget*     m_pShip;
                    Command             m_command;
                    AbstractTarget*     m_pTarget;
                    CommandID           m_commandID;
    };

    //------------------------------------------------------------------------------
}