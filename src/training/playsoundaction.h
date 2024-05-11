/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	playsoundaction.h
**
**  Author:  
**
**  Description:
**      Header file for the training library "playsoundaction" interface.
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
    class PlaySoundAction : public Action
    {
        public:
                    /* void */              PlaySoundAction (SoundID soundID);
            virtual /* void */              ~PlaySoundAction (void);
            virtual void                    Execute (void);
            virtual void                    Stop (void);
                    bool                    HasStarted (void) const;
                    bool                    HasStopped (void) const;

        protected:
                    SoundID                 m_soundID;
                    TRef<ISoundInstance>    m_soundInstance;
                    bool                    m_bHasStarted;
    };

    //------------------------------------------------------------------------------
}