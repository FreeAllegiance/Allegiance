/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	commandacknowledgedcondition.h
**
**  Author:  
**
**  Description:
**      Header file for the training library "commandacknowledgedcondition" interface.
**
**  History:
*/
#pragma once

#include "Condition.h"

namespace Training
{
    //------------------------------------------------------------------------------
    // class definitions
    //------------------------------------------------------------------------------
    class CommandAcknowledgedCondition : public Condition
    {
        public:
                    /* void */          CommandAcknowledgedCondition (char cTargetCommand);
            virtual /* void */          ~CommandAcknowledgedCondition (void);
            virtual bool                Evaluate (void);
            static  void                SetCommandAcknowledged (char cAcknowledged);

        protected:
                    char                m_cTargetCommand;
            static  char                m_cCommandAcknowledged;
    };

    //------------------------------------------------------------------------------
}