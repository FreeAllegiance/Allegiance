/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	getautopilotcondition.h
**
**  Author:  
**
**  Description:
**      Header file for the training library "getautopilotcondition" interface.
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
    class GetAutopilotCondition : public Condition
    {
        public:
                    /* void */          GetAutopilotCondition (const TRef<IshipIGC>& ship);
            virtual /* void */          ~GetAutopilotCondition (void);
            virtual bool                Evaluate (void);

        protected:
                    TRef<IshipIGC>      m_ship;
    };

    //------------------------------------------------------------------------------
}