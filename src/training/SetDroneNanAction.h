/*
**
**  File:	setdronennaaction.h
**
**  Author:  
**
**  Description:
**      Header file for the training library "setdronenanaction" interface.
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
    class SetDroneNanAction : public Action
    {
        public:
                    /* void */          SetDroneNanAction (ShipID theShip);
            virtual /* void */          ~SetDroneNanAction (void);
            virtual void                Execute (void);

        protected:
                    ShipID nanShipID;
    };

    //------------------------------------------------------------------------------
}