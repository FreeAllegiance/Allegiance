/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	setradarlodaction.h
**
**  Author:  
**
**  Description:
**      Header file for the training library "setradarlodaction" interface.
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
    class SetRadarLODAction : public Action
    {
        public:
                    /* void */          SetRadarLODAction (RadarImage::RadarLOD radarLOD);
            virtual /* void */          ~SetRadarLODAction (void);
            virtual void                Execute (void);

        protected:
                    RadarImage::RadarLOD    m_radarLOD;
    };

    //------------------------------------------------------------------------------
}