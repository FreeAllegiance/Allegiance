/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	objectpointingatcondition.cpp
**
**  Author:  
**
**  Description:
**      Implementation of the training library "objectpointingatcondition" interface.
**
**  History:
*/
#include    "pch.h"
#include    "ObjectPointingAtCondition.h"
#include    "TypeIDTarget.h"

namespace Training
{
    //------------------------------------------------------------------------------
    // class methods
    //------------------------------------------------------------------------------
    /* void */  ObjectPointingAtCondition::ObjectPointingAtCondition (ImodelIGC* pObject, ImodelIGC* pTarget) :
    m_pObject (new TypeIDTarget (pObject->GetObjectType (), pObject->GetObjectID ())),
    m_pTarget (new TypeIDTarget (pTarget->GetObjectType (), pTarget->GetObjectID ())),
    m_vectorMask (fNA, fNA, fNA),
    m_fMinimumAngle (fNA)
    {
    }

    //------------------------------------------------------------------------------
    /* void */  ObjectPointingAtCondition::ObjectPointingAtCondition (ImodelIGC* pObject, ObjectType targetType, ObjectID targetID) :
    m_pObject (new TypeIDTarget (pObject->GetObjectType (), pObject->GetObjectID ())),
    m_pTarget (new TypeIDTarget (targetType, targetID)),
    m_vectorMask (fNA, fNA, fNA),
    m_fMinimumAngle (fNA)
    {
    }

    //------------------------------------------------------------------------------
    /* void */  ObjectPointingAtCondition::ObjectPointingAtCondition (ImodelIGC* pObject, AbstractTarget* pTarget) :
    m_pObject (new TypeIDTarget (pObject->GetObjectType (), pObject->GetObjectID ())),
    m_pTarget (pTarget),
    m_vectorMask (fNA, fNA, fNA),
    m_fMinimumAngle (fNA)
    {
    }

    //------------------------------------------------------------------------------
    /* void */  ObjectPointingAtCondition::~ObjectPointingAtCondition (void)
    {
        delete m_pObject;
        delete m_pTarget;
    }

    //------------------------------------------------------------------------------
    bool        ObjectPointingAtCondition::Evaluate (void)
    {
        // check that both objects in this condition still exist. This is mostly
        // to avoid unwanted crashes, not to provide any kind of desired behavior.
        // Note that if the objects don't exist, a constraint should have caught
        // the situation and appropriately handled it, so this shouldn't fail.
        ZAssert (*m_pObject and *m_pTarget);
        if (*m_pObject and *m_pTarget)
        {
            // The algorithm here is to get the vector of the forward orientation and 
            // the vector towards the target (the look vector). 
            // If the object is looking towards the target, then the angle from its 
            // forward vector to the look vector has some characteristic we can measure.
            Vector  targetPosition = MaskedVector ((*m_pTarget)->GetPosition ());
            Vector  objectPosition = MaskedVector ((*m_pObject)->GetPosition ());
            Vector  forward = MaskedVector ((*m_pObject)->GetOrientation ().GetForward ()).Normalize ();
            Vector  lookAtTarget = targetPosition - objectPosition;
            float   fDistanceToTarget = lookAtTarget.Length ();
            lookAtTarget /= fDistanceToTarget;
            float   fCosLookAngle = forward * lookAtTarget; // dot product
            if (fCosLookAngle > 0.0f)
            {
                // here is where we do the comparison against some measurable criteria
                float   fLookAngle = acosf (fCosLookAngle); // dot product
                if (m_fMinimumAngle >= 0.0f)
                {
                    // in this case, I care about a specific angle that I set, so we just
                    // compare against that
                    float   fTargetLookAngle = RadiansFromDegrees (m_fMinimumAngle);
                    return (fLookAngle < fTargetLookAngle) ? true : false;
                }
                else
                {
                    // here, we check to see if the look angle is less than the angle 
                    // subtended by the target's radius around the look vector.
                    // this can be confused by long, skinny ships, such as the builder.
                    float   fCoverageRadius = (*m_pTarget)->GetRadius () * 0.75f;
                    float   fCoverageAngle = asinf (fCoverageRadius / fDistanceToTarget);
                    float   fMinimumCoverageAngle = RadiansFromDegrees (3.0f); // settled on 3 degrees
                    return (fLookAngle < ((fCoverageAngle < fMinimumCoverageAngle) ? fMinimumCoverageAngle : fCoverageAngle)) ? true : false;
                }
            }
        }
        return false;
    }

    //------------------------------------------------------------------------------
    void        ObjectPointingAtCondition::SetVectorMask (const Vector& vectorMask)
    {
        ZAssert ((vectorMask.X () == 0.0f) or (vectorMask.X () == NA));
        ZAssert ((vectorMask.Y () == 0.0f) or (vectorMask.Y () == NA));
        ZAssert ((vectorMask.Z () == 0.0f) or (vectorMask.Z () == NA));
        m_vectorMask = vectorMask;
    }

    //------------------------------------------------------------------------------
    void        ObjectPointingAtCondition::ClearVectorMask (void)
    {
        m_vectorMask = Vector (fNA, fNA, fNA);
    }

    //------------------------------------------------------------------------------
    Vector      ObjectPointingAtCondition::MaskedVector (const Vector& vector)
    {
        // this masking of the vectors allows me to use this condition along any 
        // pair of axes, so that I could examine how much to the right or left any
        // target is, not just relationship to the center of the screen
        float   x = (m_vectorMask.X () == NA) ? vector.X () : m_vectorMask.X ();
        float   y = (m_vectorMask.Y () == NA) ? vector.Y () : m_vectorMask.Y ();
        float   z = (m_vectorMask.Z () == NA) ? vector.Z () : m_vectorMask.Z ();
        return Vector (x, y, z);
    }

    //------------------------------------------------------------------------------
    void        ObjectPointingAtCondition::SetMinimumAngle (float fMinimumAngle)
    {
        m_fMinimumAngle = fMinimumAngle;
    }

    //------------------------------------------------------------------------------
}
