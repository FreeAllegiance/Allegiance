/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	objectwithinradiuscondition.cpp
**
**  Author:  
**
**  Description:
**      Implementation of the training library "objectwithinradiuscondition" interface.
**
**  History:
*/
#include    "pch.h"
#include    "ObjectWithinRadiusCondition.h"
#include    "TypeIDTarget.h"

namespace Training
{
    //------------------------------------------------------------------------------
    // class methods
    //------------------------------------------------------------------------------
    /* void */  ObjectWithinRadiusCondition::ObjectWithinRadiusCondition (ImodelIGC* pObject, ImodelIGC* pTarget, float fRadius) :
    m_pObject (new TypeIDTarget (pObject->GetObjectType (), pObject->GetObjectID ())),
    m_pTarget (new TypeIDTarget (pTarget->GetObjectType (), pTarget->GetObjectID ())),
    m_fRadiusSquared (fRadius),
    m_bConditionInitialized (false)
    {
    }

    //------------------------------------------------------------------------------
    /* void */  ObjectWithinRadiusCondition::ObjectWithinRadiusCondition (ImodelIGC* pObject, ObjectType targetType, ObjectID targetID, float fRadius) :
    m_pObject (new TypeIDTarget (pObject->GetObjectType (), pObject->GetObjectID ())),
    m_pTarget (new TypeIDTarget (targetType, targetID)),
    m_fRadiusSquared (fRadius),
    m_bConditionInitialized (false)
    {
    }

    //------------------------------------------------------------------------------
    /* void */  ObjectWithinRadiusCondition::ObjectWithinRadiusCondition (ImodelIGC* pObject, AbstractTarget* pTarget, float fRadius) :
    m_pObject (new TypeIDTarget (pObject->GetObjectType (), pObject->GetObjectID ())),
    m_pTarget (pTarget),
    m_fRadiusSquared (fRadius),
    m_bConditionInitialized (false)
    {
    }

    //------------------------------------------------------------------------------
    /* void */  ObjectWithinRadiusCondition::ObjectWithinRadiusCondition (ObjectType objectType, ObjectID objectID, ObjectType targetType, ObjectID targetID, float fRadius) :
    m_pObject (new TypeIDTarget (objectType, objectID)),
    m_pTarget (new TypeIDTarget (targetType, targetID)),
    m_fRadiusSquared (fRadius),
    m_bConditionInitialized (false)
    {
    }

    //------------------------------------------------------------------------------
    /* void */  ObjectWithinRadiusCondition::~ObjectWithinRadiusCondition (void)
    {
        delete m_pObject;
        delete m_pTarget;
    }

    //------------------------------------------------------------------------------
    bool        ObjectWithinRadiusCondition::Evaluate (void)
    {
        if (Initialized ())
        {
            // check that both objects in this condition still exist. This is mostly
            // to avoid unwanted crashes, not to provide any kind of desired behavior.
            // Note that if the objects don't exist, a constraint should have caught
            // the situation and appropriately handled it, so this shouldn't fail.
            ZAssert (*m_pObject and *m_pTarget);
            if (*m_pObject and *m_pTarget)
            {
                // Check to see if the objects are closer than the specified distance,
                // after accounting for the radii of the two objects. We use the squared
                // lengths to avoid the unnecessary square root operation.
                Vector  delta = (*m_pObject)->GetPosition () - (*m_pTarget)->GetPosition ();
                float   fLengthSquared = delta.LengthSquared ();
                return (fLengthSquared > m_fRadiusSquared) ? false : true;
            }
        }
        return false;
    }

    //------------------------------------------------------------------------------
    float       ObjectWithinRadiusCondition::GetRadarRadius (void) const
    {
        // make sure the condition is initialized (if possible)
        const_cast<ObjectWithinRadiusCondition*> (this) ->Initialized ();
        // This returns the number that the radar will show at the moment the
        // condition becomes true. If the condition is not initialized, we assume 
        // a target radius of zero
        return m_bConditionInitialized ? sqrtf (m_fRadiusSquared) : (*m_pObject)->GetRadius () + m_fRadiusSquared;
    }

    //------------------------------------------------------------------------------
    bool        ObjectWithinRadiusCondition::Initialized (void)
    {
        if (not m_bConditionInitialized)
        {
            // fetch the target object, if we can
            if (*m_pTarget)
            {
                // we want the radius to represent distance from outer edge to outer edge,
                // not an absolute distance value.
                m_fRadiusSquared += (*m_pObject)->GetRadius () + (*m_pTarget)->GetRadius ();
                m_fRadiusSquared *= m_fRadiusSquared;
                m_bConditionInitialized = true;
            }
        }
        return m_bConditionInitialized;
    }

    //------------------------------------------------------------------------------
}
