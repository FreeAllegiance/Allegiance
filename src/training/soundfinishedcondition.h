/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	soundfinishedcondition.h
**
**  Author:  
**
**  Description:
**      Header file for the training library "soundfinishedcondition" interface.
**
**  History:
*/
#pragma once

#include "Condition.h"

#include "PlaySoundAction.h"

namespace Training
{
    //------------------------------------------------------------------------------
    // class definitions
    //------------------------------------------------------------------------------
    class SoundFinishedCondition : public Condition
    {
        public:
            /* void */                  SoundFinishedCondition (PlaySoundAction* pPlaySoundAction);
            virtual /* void */          ~SoundFinishedCondition (void);
            virtual bool                Evaluate (void);

        protected:
                    PlaySoundAction*    m_pPlaySoundAction;
                    bool                m_bHasStarted;
    };

    //------------------------------------------------------------------------------
}