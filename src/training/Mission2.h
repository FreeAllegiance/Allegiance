/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	Mission2.h
**
**  Author:  
**
**  Description:
**      Header file for the training library "Mission2" interface.
**
**  History:
*/
#pragma once

#include "TrainingMission.h"

namespace Training
{
    //------------------------------------------------------------------------------
    // class definitions
    //------------------------------------------------------------------------------
    class Mission2 : public TrainingMission
    {
        public:
            virtual /* void */              ~Mission2 (void);
            virtual int                     GetMissionID (void);
            virtual SectorID                GetStartSectorID (void);
            virtual bool                    ShipLanded (void);

        protected:
            virtual void                    CreateUniverse (void);
            virtual Condition*              CreateMission (void);

                    Goal*                   CreateGoal01 (void);
                    Goal*                   CreateGoal02 (void);
                    Goal*                   CreateGoal03 (void);
                    Goal*                   CreateGoal04 (void);
                    Goal*                   CreateGoal05 (void);
                    Goal*                   CreateGoal06 (void);
                    Goal*                   CreateGoal07 (void);
                    Goal*                   CreateGoal08 (void);
                    Goal*                   CreateGoal09 (void);
    };

    //------------------------------------------------------------------------------
}