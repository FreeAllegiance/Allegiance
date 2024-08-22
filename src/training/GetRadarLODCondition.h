/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	getradarlodcondition.h
**
**  Author:  
**
**  Description:
**      Header file for the training library "getradarlodcondition" interface.
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
    class GetRadarLODCondition : public Condition
    {
        public:
                    /* void */          GetRadarLODCondition (RadarImage::RadarLOD expectedRadarLOD);
            virtual /* void */          ~GetRadarLODCondition (void);
            virtual bool                Evaluate (void);

        protected:
                    RadarImage::RadarLOD    m_expectedRadarLOD;
    };

    //------------------------------------------------------------------------------
}