/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	objectwithinradiuscondition.h
**
**  Author:  
**
**  Description:
**      Header file for the training library "objectwithinradiuscondition" interface.
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
    class ObjectWithinRadiusCondition : public Condition
    {
        public:
            /* void */                  ObjectWithinRadiusCondition (ImodelIGC* pObject, ImodelIGC* pTarget, float fRadius);
            /* void */                  ObjectWithinRadiusCondition (ImodelIGC* pObject, ObjectType targetType, ObjectID targetID, float fRadius);
            /* void */                  ObjectWithinRadiusCondition (ImodelIGC* pObject, AbstractTarget* pTarget, float fRadius);
            /* void */                  ObjectWithinRadiusCondition (ObjectType objectType, ObjectID objectID, ObjectType targetType, ObjectID targetID, float fRadius);
            virtual /* void */          ~ObjectWithinRadiusCondition (void);
            virtual bool                Evaluate (void);
                    float               GetRadarRadius (void) const;

        protected:
                    bool                Initialized (void);

        protected:
                    AbstractTarget*     m_pObject;
                    AbstractTarget*     m_pTarget;
                    float               m_fRadiusSquared;
                    bool                m_bConditionInitialized;
    };

    //------------------------------------------------------------------------------
}