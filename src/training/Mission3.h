/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	Mission3.h
**
**  Author:  
**
**  Description:
**      Header file for the training library "Mission3" interface.
**
**  History:
*/
#pragma once

#include "TrainingMission.h"

namespace Training
{
    //------------------------------------------------------------------------------
    // a needed value
    //------------------------------------------------------------------------------
    #define TM_NUMBER_OF_DERELICTS  10

    //------------------------------------------------------------------------------
    // class definitions
    //------------------------------------------------------------------------------
    class Mission3 : public TrainingMission
    {
        public:
            virtual /* void */              ~Mission3 (void);
            virtual int                     GetMissionID (void);
            virtual SectorID                GetStartSectorID (void);
            virtual bool                    RecordKeyPress (TrekKey key);
            virtual bool                    ShipLanded (void);

        protected:
                    ShipID                  m_derelict_hulls[TM_NUMBER_OF_DERELICTS];

        protected:
            virtual void                    CreateUniverse (void);
            virtual Condition*              CreateMission (void);

                    Condition*              CreateGotTargetCondition (void);

                    Goal*                   CreateGoal01 (void);
                    Goal*                   CreateGoal02 (void);
                    Goal*                   CreateGoal03 (void);
                    Goal*                   CreateGoal04 (void);
                    Goal*                   CreateGoal05 (void);
                    Goal*                   CreateGoal06 (void);
                    Goal*                   CreateGoal07 (void);
                    Goal*                   CreateGoal08 (void);
                    Goal*                   CreateGoal09 (void);
                    Goal*                   CreateGoal10 (void);
                    Goal*                   CreateGoal11 (void);
                    Goal*                   CreateGoal12 (void);
                    Goal*                   CreateGoal13 (void);
    };

    //------------------------------------------------------------------------------
}