/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	messageaction.h
**
**  Author:  
**
**  Description:
**      Header file for the training library "messageaction" interface.
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
    class MessageAction : public Action
    {
        public:
                    /* void */          MessageAction (void);
                    /* void */          MessageAction (const ZString message, const SoundID soundID = NA); //TheBored 06-JUL-07: Bahdohday's sound w/text edit.
            virtual /* void */          ~MessageAction (void);
            virtual void                Execute (void);
                    void                AddMessage (const ZString message, const SoundID soundID = NA); //TheBored 06-JUL-07: Bahdohday's sound w/text edit.

        protected:
                    std::list<ZString>              m_messageList;
                    std::list<ZString>::iterator    m_iterator;
					//TheBored 06-JUL-07: Bahdohday's sound w/text edit.
					std::list<SoundID>				m_soundList;
					std::list<SoundID>::iterator	m_soundIterator;
					//End TB
    };

    //------------------------------------------------------------------------------
}