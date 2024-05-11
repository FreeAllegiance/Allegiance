/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	objectmovingtowardscondition.h
**
**  Author:  
**
**  Description:
**      Header file for the training library "objectmovingtowardscondition" interface.
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
    class ObjectMovingTowardsCondition : public Condition
    {
        public:
            /* void */                  ObjectMovingTowardsCondition (const TRef<ImodelIGC>& object, const TRef<ImodelIGC>& target);
            virtual /* void */          ~ObjectMovingTowardsCondition (void);
            virtual bool                Evaluate (void);

        protected:
                    TRef<ImodelIGC>     m_object;
                    TRef<ImodelIGC>     m_target;
    };

    //------------------------------------------------------------------------------
}