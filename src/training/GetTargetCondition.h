/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	gettargetcondition.h
**
**  Author:  
**
**  Description:
**      Header file for the training library "gettargetcondition" interface.
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
    class GetTargetCondition : public Condition
    {
        public:
                    /* void */          GetTargetCondition (IshipIGC* pShip, ImodelIGC* pExpectedTarget);
                    /* void */          GetTargetCondition (IshipIGC* pShip, ObjectType targetType, ObjectID targetID);
                    /* void */          GetTargetCondition (IshipIGC* pShip, AbstractTarget* pTarget);
            virtual /* void */          ~GetTargetCondition (void);
            virtual bool                Evaluate (void);

        protected:
                    AbstractTarget*     m_pShip;
                    AbstractTarget*     m_pTarget;
    };

    //------------------------------------------------------------------------------
}