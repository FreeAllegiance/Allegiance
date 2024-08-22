/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	TurnToAction.h
**
**  Author:  
**
**  Description:
**      Header file for the training library "TurnToAction" interface.
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
    class TurnToAction : public Action
    {
        public:
            /* void */                  TurnToAction (ImodelIGC* pObject, ImodelIGC* pTarget);
            /* void */                  TurnToAction (ImodelIGC* pObject, ObjectType targetType, ObjectID targetID);
            /* void */                  TurnToAction (ImodelIGC* pObject, AbstractTarget* pTarget);
            virtual /* void */          ~TurnToAction (void);
            virtual void                Execute (void);

        protected:
                    AbstractTarget*     m_pObject;
                    AbstractTarget*     m_pTarget;
    };

    //------------------------------------------------------------------------------
}