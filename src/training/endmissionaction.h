/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	endmissionaction.h
**
**  Author:  
**
**  Description:
**      Header file for the training library "endmissionaction" interface.
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
    class EndMissionAction : public Action
    {
        public:
            virtual /* void */          ~EndMissionAction (void);
            virtual void                Execute (void);
    };

    //------------------------------------------------------------------------------
}

#endif  //_END_MISSION_ACTION_H_
