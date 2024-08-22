/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	GetSectorCondition.h
**
**  Author:  
**
**  Description:
**      Header file for the training library "GetSectorCondition" interface.
**
**  History:
*/
#pragma once

#include "Condition.h"

#include "AbstractTarget.h"

namespace Training
{
    //------------------------------------------------------------------------------
    // class definitions
    //------------------------------------------------------------------------------
    class GetSectorCondition : public Condition
    {
        public:
                    /* void */          GetSectorCondition (ImodelIGC* pModel, SectorID sector);
                    /* void */          GetSectorCondition (ObjectType objectType, ObjectID objectID, SectorID sector);
                    /* void */          GetSectorCondition (AbstractTarget* pTarget, SectorID sector);
            virtual /* void */          ~GetSectorCondition (void);
            virtual bool                Evaluate (void);

        protected:
                    AbstractTarget*     m_pModel;
                    SectorID            m_sector;
    };

    //------------------------------------------------------------------------------
}