/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	ShipSeesObjectCondition.h
**
**  Author:  
**
**  Description:
**      Header file for the training library "ShipSeesObjectCondition" interface.
**
**  History:
*/
#pragma once

#include "Condition.h"

#include "AbstractTarget.h"

namespace Training
{
    //------------------------------------------------------------------------------
    // class definitions
    //------------------------------------------------------------------------------
    class ShipSeesObjectCondition : public Condition
    {
        public:
            /* void */                  ShipSeesObjectCondition (IshipIGC* pShip, ImodelIGC* pTarget);
            /* void */                  ShipSeesObjectCondition (IshipIGC* pShip, ObjectType targetType, ObjectID targetID);
            /* void */                  ShipSeesObjectCondition (ObjectID shipID, ObjectType targetType, ObjectID targetID);
            virtual /* void */          ~ShipSeesObjectCondition (void);
            virtual bool                Evaluate (void);

        protected:
                    AbstractTarget*     m_pShip;
                    AbstractTarget*     m_pTarget;
    };

    //------------------------------------------------------------------------------
}