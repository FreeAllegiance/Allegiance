/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	CurrentTarget.h
**
**  Author:  
**
**  Description:
**      Header file for the training library "CurrentTarget" interface.
**
**  History:
*/
#pragma once

#include "AbstractTarget.h"

namespace Training
{
    //------------------------------------------------------------------------------
    // class definitions
    //------------------------------------------------------------------------------
    class CurrentTarget : public AbstractTarget
    {
        public:
                    /* void */          CurrentTarget (AbstractTarget* pTarget, bool bLockOnFirstFetch = false);
            virtual /* void */          ~CurrentTarget (void);
            virtual /* ImodelIGC* */    operator ImodelIGC* (void);

        protected:
                    AbstractTarget*     m_pTarget;
                    bool                m_bLockOnFirstFetch;
                    AbstractTarget*     m_pFetched;
    };

    //------------------------------------------------------------------------------
}