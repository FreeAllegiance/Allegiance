/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	ToggleWeaponAction.h
**
**  Author:  
**
**  Description:
**      Header file for the training library "ToggleWeaponAction" interface.
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
    class ToggleWeaponAction : public Action
    {
        public:
                    /* void */          ToggleWeaponAction (Mount mount);
            virtual /* void */          ~ToggleWeaponAction (void);
            virtual void                Execute (void);

        protected:
                    Mount               m_mount;
    };

    //------------------------------------------------------------------------------
}