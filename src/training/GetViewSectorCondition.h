/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	GetViewSectorCondition.h
**
**  Author:  
**
**  Description:
**      Header file for the training library "GetViewSectorCondition" interface.
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
    class GetViewSectorCondition : public Condition
    {
        public:
                    /* void */          GetViewSectorCondition (SectorID sector);
            virtual /* void */          ~GetViewSectorCondition (void);
            virtual bool                Evaluate (void);

        protected:
                    SectorID            m_sector;
    };

    //------------------------------------------------------------------------------
}