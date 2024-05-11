/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	turnshipaboutcondition.h
**
**  Author:  
**
**  Description:
**      Header file for the training library "turnshipaboutcondition" interface.
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
    class TurnShipAboutCondition : public Condition
    {
        public:
            /* void */                  TurnShipAboutCondition (const TRef<ImodelIGC>& ship);
            virtual /* void */          ~TurnShipAboutCondition (void);
            virtual bool                Start (void);
            virtual bool                Evaluate (void);

        protected:
                    TRef<ImodelIGC>     m_ship;
                    Vector              m_startingForward;
    };

    //------------------------------------------------------------------------------
}