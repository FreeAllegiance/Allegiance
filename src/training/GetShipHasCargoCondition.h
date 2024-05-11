/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	getshiphascargocondition.h
**
**  Author:  
**
**  Description:
**      Header file for the training library "getshiphascargocondition" interface.
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
    class GetShipHasCargoCondition : public Condition
    {
        public:
                    /* void */          GetShipHasCargoCondition (const TRef<IshipIGC>& ship, EquipmentType part);
            virtual /* void */          ~GetShipHasCargoCondition (void);
            virtual bool                Evaluate (void);

        protected:
                    TRef<IshipIGC>      m_ship;
                    EquipmentType       m_part;
    };

    //------------------------------------------------------------------------------
}