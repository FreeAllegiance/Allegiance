/*
**
**  File:	setcargonanaction.h
**
**  Author:  
**
**  Description:
**      Header file for the training library "setcargonanaction" interface.
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
    class SetCargoNanAction : public Action
    {
        public:
                    /* void */          SetCargoNanAction (ShipID theShip);
            virtual /* void */          ~SetCargoNanAction (void);
            virtual void                Execute (void);

        protected:
                    ShipID nanShipID;
    };

    //------------------------------------------------------------------------------
}