/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	SwapWeaponAction.h
**
**  Author:  
**
**  Description:
**      Header file for the training library "SwapWeaponAction" interface.
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
    class SwapWeaponAction : public Action
    {
        public:
                    /* void */          SwapWeaponAction (EquipmentType equipmentType, Mount mount);
            virtual /* void */          ~SwapWeaponAction (void);
            virtual void                Execute (void);

        protected:
                    EquipmentType       m_equipmentType;
                    Mount               m_mount;
    };

    //------------------------------------------------------------------------------
}