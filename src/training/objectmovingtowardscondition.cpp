/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	objectmovingtowardscondition.cpp
**
**  Author:  
**
**  Description:
**      Implementation of the training library "objectmovingtowardscondition" interface.
**
**  History:
*/
#include    "pch.h"
#include    "ObjectMovingTowardsCondition.h"

namespace Training
{
    //------------------------------------------------------------------------------
    // class methods
    //------------------------------------------------------------------------------
    /* void */  ObjectMovingTowardsCondition::ObjectMovingTowardsCondition (const TRef<ImodelIGC>& object, const TRef<ImodelIGC>& target) :
    m_object (object),
    m_target (target)
    {
    }

    //------------------------------------------------------------------------------
    /* void */  ObjectMovingTowardsCondition::~ObjectMovingTowardsCondition (void)
    {
        m_object = 0;
        m_target = 0;
    }

    //------------------------------------------------------------------------------
    bool        ObjectMovingTowardsCondition::Evaluate (void)
    {
        // check that both objects in this condition still exist. This is mostly
        // to avoid unwanted crashes, not to provide any kind of desired behavior.
        // Note that if the objects don't exist, a constraint should have caught
        // the situation and appropriately handled it, so this shouldn't fail.
        ZAssert (m_object->GetMission () and m_target->GetMission ());
        if (m_object->GetMission () and m_target->GetMission ())
        {
            // The algorithm here is to get the vector of the motion and the vector towards
            // the target (the look vector). If the object is moving towards the target, 
            // then the angle from its velocity vector to the look vector is less than 
            // the angle subtended by the target's radius around the look vector.
            Vector  forward = m_object->GetVelocity ();
            float   fSpeed = forward.Length ();
            if (fSpeed > 0.01f)
            {
                Vector  lookAtTarget = m_target->GetPosition () - m_object->GetPosition ();
                float   fDistanceToTarget = lookAtTarget.Length ();
                lookAtTarget /= fDistanceToTarget;
                forward /= fSpeed;
                float   fCosLookAngle = forward * lookAtTarget; // dot product
                if (fCosLookAngle > 0.0f)
                {
                    float   fLookAngle = acosf (fCosLookAngle);
                    float   fCoverageRadius = m_target->GetRadius () * 0.75f;
                    float   fCoverageAngle = asinf (fCoverageRadius / fDistanceToTarget);
                    return (fLookAngle < fCoverageAngle) ? true : false;
                }
            }
        }
        return false;
    }

    //------------------------------------------------------------------------------
}
