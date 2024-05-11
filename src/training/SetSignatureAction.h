/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	setsignatureaction.h
**
**  Author:  
**
**  Description:
**      Header file for the training library "setsignatureaction" interface.
**
**  History:
*/
#pragma once

#include "Action.h"

#include "AbstractTarget.h"

namespace Training
{
    //------------------------------------------------------------------------------
    // class definitions
    //------------------------------------------------------------------------------
    class SetSignatureAction : public Action
    {
        public:
                    /* void */          SetSignatureAction (ImodelIGC* object, float fSignature);
                    /* void */          SetSignatureAction (ObjectType objectType, ObjectID objectID, float fSignature);
                    /* void */          SetSignatureAction (AbstractTarget* pTarget, float fSignature);
            virtual /* void */          ~SetSignatureAction (void);
            virtual void                Execute (void);

        protected:
                    AbstractTarget*     m_pTarget;
                    float               m_fSignature;
    };

    //------------------------------------------------------------------------------
}