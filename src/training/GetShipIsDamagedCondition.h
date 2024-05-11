/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	getshipisdamagedcondition.h
**
**  Author:  
**
**  Description:
**      Header file for the training library "getshipisdamagedcondition" interface.
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
    class GetShipIsDamagedCondition : public Condition
    {
        public:
                    /* void */          GetShipIsDamagedCondition (IdamageIGC* pShip, float fFraction = 1.0f);
                    /* void */          GetShipIsDamagedCondition (ObjectType objectType, ObjectID objectID, float fFraction = 1.0f);
                    /* void */          GetShipIsDamagedCondition (AbstractTarget* pTarget, float fFraction = 1.0f);
            virtual /* void */          ~GetShipIsDamagedCondition (void);
            virtual bool                Evaluate (void);

        protected:
                    AbstractTarget*     m_pTarget;
                    float               m_fFraction;
    };

    //------------------------------------------------------------------------------
}