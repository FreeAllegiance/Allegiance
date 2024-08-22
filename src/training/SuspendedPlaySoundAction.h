/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	suspendedplaysoundaction.h
**
**  Author:  
**
**  Description:
**      Header file for the training library "suspendedplaysoundaction" interface.
**
**  History:
*/
#pragma once

#include    "PlaySoundAction.h"

namespace Training
{
    //------------------------------------------------------------------------------
    // class definitions
    //------------------------------------------------------------------------------
    class SuspendedPlaySoundAction : public PlaySoundAction
    {
        public:
                    /* void */          SuspendedPlaySoundAction (SoundID soundID, bool bDeleteWhenDone = true);
            virtual /* void */          ~SuspendedPlaySoundAction (void);
            virtual void                Execute (void);

        protected:
                    bool                m_bDeleteWhenDone;
    };

    //------------------------------------------------------------------------------
}