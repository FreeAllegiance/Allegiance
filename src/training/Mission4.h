/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	Mission4.h
**
**  Author:  
**
**  Description:
**      Header file for the training library "Mission4" interface.
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
    class Mission4 : public TrainingMission
    {
        public:
            virtual /* void */              ~Mission4 (void);
            virtual int                     GetMissionID (void);
            virtual SectorID                GetStartSectorID (void);
            virtual bool                    ShipLanded (void);

        protected:
                    ShipID                  m_enemyScoutID;
                    ShipID                  m_enemyMiner1ID;
                    ShipID                  m_enemyMiner2ID;
                    ShipID                  m_enemyMiner3ID;
                    ShipID                  m_enemyMiner4ID;
                    ShipID                  m_builderID;

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
                    Goal*                   CreateGoal10 (void);
    };

    //------------------------------------------------------------------------------
}