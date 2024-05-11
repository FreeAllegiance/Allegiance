/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	TypeIDTarget.h
**
**  Author:  
**
**  Description:
**      Header file for the training library "TypeIDTarget" interface.
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
    class TypeIDTarget : public AbstractTarget
    {
        public:
                    /* void */          TypeIDTarget (ObjectType targetType, ObjectID targetID);
            virtual /* void */          ~TypeIDTarget (void);
            virtual /* ImodelIGC* */    operator ImodelIGC* (void);

        protected:
                    ObjectType          m_targetType;
                    ObjectID            m_targetID;
    };

    //------------------------------------------------------------------------------
}