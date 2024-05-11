/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	GetPickedCondition.h
**
**  Author:  
**
**  Description:
**      Header file for the training library "GetPickedCondition" interface.
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
    class GetPickedCondition : public Condition
    {
        public:
                    /* void */          GetPickedCondition (ObjectType targetType, ObjectID targetID);
            virtual /* void */          ~GetPickedCondition (void);
            virtual bool                Evaluate (void);

        protected:
                    AbstractTarget*     m_pTarget;
    };

    //------------------------------------------------------------------------------
}