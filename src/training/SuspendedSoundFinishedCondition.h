/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	suspendedsoundfinishedcondition.h
**
**  Author:  
**
**  Description:
**      Header file for the training library "suspendedsoundfinishedcondition" interface.
**
**  History:
*/
#pragma once

#include "SoundFinishedCondition.h"

namespace Training
{
    //------------------------------------------------------------------------------
    // class definitions
    //------------------------------------------------------------------------------
    class SuspendedSoundFinishedCondition : public SoundFinishedCondition
    {
        public:
                    /* void */          SuspendedSoundFinishedCondition (PlaySoundAction* pPlaySoundAction, bool bDeleteWhenDone = true);
            virtual /* void */          ~SuspendedSoundFinishedCondition (void);

        protected:
                    bool                m_bDeleteWhenDone;
    };

    //------------------------------------------------------------------------------
}