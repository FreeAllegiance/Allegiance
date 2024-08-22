/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	PlayerShipTarget.h
**
**  Author:  
**
**  Description:
**      Header file for the training library "PlayerShipTarget" interface.
**
**  History:
*/
#pragma once

#include "AbstractTarget.h"

namespace Training
{
    //------------------------------------------------------------------------------
    // class definitions
    //------------------------------------------------------------------------------
    class PlayerShipTarget : public AbstractTarget
    {
        public:
            virtual /* void */          ~PlayerShipTarget (void);
            virtual /* ImodelIGC* */    operator ImodelIGC* (void);
    };

    //------------------------------------------------------------------------------
}