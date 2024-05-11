/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	DoDamageAction.h
**
**  Author:  
**
**  Description:
**      Header file for the training library "DoDamageAction" interface.
**
**  History:
*/
#pragma once

#include "Action.h"

namespace Training
{
    //------------------------------------------------------------------------------
    // class definitions
    //------------------------------------------------------------------------------
    class DoDamageAction : public Action
    {
        public:
                    /* void */          DoDamageAction (bool bHull);
            virtual /* void */          ~DoDamageAction (void);
            virtual void                Execute (void);

        protected:
                    bool                m_bHull;
    };

    //------------------------------------------------------------------------------
}