/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	getshipisstoppedcondition.h
**
**  Author:  
**
**  Description:
**      Header file for the training library "getshipisstoppedcondition" interface.
**
**  History:
*/
#pragma once

#include "Condition.h"

namespace Training
{
    //------------------------------------------------------------------------------
    // class definitions
    //------------------------------------------------------------------------------
    class GetShipIsStoppedCondition : public Condition
    {
        public:
                    /* void */          GetShipIsStoppedCondition (const TRef<IshipIGC>& ship);
					/* void */          GetShipIsStoppedCondition (const ShipID shipID);
            virtual /* void */          ~GetShipIsStoppedCondition (void);
            virtual bool                Evaluate (void);

        protected:
                    TRef<IshipIGC>      m_ship;
					ShipID				m_shipID;
    };

    //------------------------------------------------------------------------------
}