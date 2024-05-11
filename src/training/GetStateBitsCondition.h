/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	getstatebitscondition.h
**
**  Author:  
**
**  Description:
**      Header file for the training library "getstatebitscondition" interface.
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
    class GetStateBitsCondition : public Condition
    {
        public:
                    /* void */          GetStateBitsCondition (const TRef<IshipIGC>& ship, int iBits);
            virtual /* void */          ~GetStateBitsCondition (void);
            virtual bool                Evaluate (void);

        protected:
                    TRef<IshipIGC>      m_ship;
                    int                 m_iBits;
    };

    //------------------------------------------------------------------------------
}