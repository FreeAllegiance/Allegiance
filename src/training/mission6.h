/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	Mission6.h
**
**  Author:  
**
**  Description:
**      Header file for the training library "Mission6" interface.
**
**  History:
*/
#pragma once

#include "TrainingMission.h"

#include "Goal.h"

namespace Training
{
    //------------------------------------------------------------------------------
    // class definitions
    //------------------------------------------------------------------------------
    class Mission6 : public TrainingMission
    {
        public:
                                            Mission6(void);
            virtual /* void */              ~Mission6 (void);
            virtual int                     GetMissionID (void);
            virtual SectorID                GetStartSectorID (void);
            virtual bool                    RestoreShip (void);
            virtual bool                    ShipLanded(void);
            virtual bool                    HandlePickDefaultOrder(IshipIGC* pShip);

        protected:
            virtual void                    CreateUniverse (void);
            virtual Condition*              CreateMission (void);

        bool    bUndockHintShown;
    };

    //------------------------------------------------------------------------------
}