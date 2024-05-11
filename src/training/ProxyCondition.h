/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	ProxyCondition.h
**
**  Author:  
**
**  Description:
**      Header file for the training library "ProxyCondition" interface.
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
    class ProxyCondition : public Condition
    {
        public:
                    /* void */          ProxyCondition (Condition* pCondition);
            virtual /* void */          ~ProxyCondition (void);
            virtual bool                Evaluate (void);

        protected:
                    Condition*          m_pCondition;
    };

    //------------------------------------------------------------------------------
}