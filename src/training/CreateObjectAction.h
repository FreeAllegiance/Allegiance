/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	CreateObjectAction.h
**
**  Author:  
**
**  Description:
**      Header file for the training library "CreateObjectAction" interface.
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
    class CreateObjectAction : public Action
    {
        public:
                    /* void */          CreateObjectAction (ObjectType objectType, const void* pData, int iDataSize);
            virtual /* void */          ~CreateObjectAction (void);
            virtual void                Execute (void);

        protected:
                    ObjectType          m_objectType;
                    const void*         m_pData;
                    int                 m_iDataSize;
                    TRef<IbaseIGC>      m_result;
    };

    //------------------------------------------------------------------------------
}