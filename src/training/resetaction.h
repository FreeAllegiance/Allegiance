/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	resetaction.h
**
**  Author:  
**
**  Description:
**      Header file for the training library "resetaction" interface.
**
**  History:
*/
#pragma once

#include "Action.h"

#include "Goal.h"

namespace Training
{
    //------------------------------------------------------------------------------
    // class definitions
    //------------------------------------------------------------------------------
    class ResetAction : public Action
    {
        public:
                    enum    ResetStage  {
                                            RESET_START,
                                            RESET_NOTIFY,
                                            RESET_SPECIFIC_NOTIFY,
                                            RESET_COMPLETE
                                        };
        public:
                    /* void */          ResetAction (Goal* pGoal, SoundID soundID = NA);
            virtual /* void */          ~ResetAction (void);
            virtual void                Execute (void);
            static  void                Initialize (void);

        protected:
                    void                PlaySound (SoundID soundID, ResetStage nextStage);

        protected:
                    Goal*               m_pGoal;
                    SoundID             m_soundID;
            static  int                 m_iResetCount;
                    ResetStage          m_resetStage;
    };

    //------------------------------------------------------------------------------
}