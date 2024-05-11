/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	getcontrolactivecondition.h
**
**  Author:  
**
**  Description:
**      Header file for the training library "getcontrolactivecondition" interface.
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
    class GetControlActiveCondition : public Condition
    {
        public:
                    /* void */          GetControlActiveCondition (const TRef<IshipIGC>& ship, Axis axis, float fExpectedValue, bool bUseInputControls = false);
            virtual /* void */          ~GetControlActiveCondition (void);
            virtual bool                Evaluate (void);

        protected:
                    TRef<IshipIGC>      m_ship;
                    Axis                m_axis;
                    float               m_fExpectedValue;
                    bool                m_bUseInputControls;
    };

    //------------------------------------------------------------------------------
}