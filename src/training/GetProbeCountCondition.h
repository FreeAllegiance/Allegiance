/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	GetProbeCountCondition.h
**
**  Author:  
**
**  Description:
**      Header file for the training library "GetProbeCountCondition" interface.
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
    class GetProbeCountCondition : public Condition
    {
        public:
                    /* void */          GetProbeCountCondition (int iProbeCount);
            virtual /* void */          ~GetProbeCountCondition (void);
            virtual bool                Evaluate (void);

        protected:
                    int                 m_iProbeCount;
    };

    //------------------------------------------------------------------------------
}