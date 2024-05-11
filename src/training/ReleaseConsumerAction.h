/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	ReleaseConsumerAction.h
**
**  Author:  
**
**  Description:
**      Header file for the training library "ReleaseConsumerAction" interface.
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
    class ReleaseConsumerAction : public Action
    {
        public:
                    /* void */          ReleaseConsumerAction (const TRef<IbuoyIGC>& buoy);
            virtual /* void */          ~ReleaseConsumerAction (void);
            virtual void                Execute (void);

        protected:
                    TRef<IbuoyIGC>      m_buoy;
    };

    //------------------------------------------------------------------------------
}