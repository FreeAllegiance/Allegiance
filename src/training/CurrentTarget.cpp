/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	CurrentTarget.cpp
**
**  Author:  
**
**  Description:
**      Implementation of the training library "CurrentTarget" interface.
**
**  History:
*/
#include    "pch.h"
#include    "CurrentTarget.h"
#include    "TypeIDTarget.h"

namespace Training
{
    //------------------------------------------------------------------------------
    // class methods
    //------------------------------------------------------------------------------
    /* void */  CurrentTarget::CurrentTarget (AbstractTarget* pTarget, bool bLockOnFirstFetch) :
    m_pTarget (pTarget),
    m_bLockOnFirstFetch (bLockOnFirstFetch),
    m_pFetched (0)
    {
    }

    //------------------------------------------------------------------------------
    /* void */  CurrentTarget::~CurrentTarget (void)
    {
        delete m_pTarget;
        delete m_pFetched;
    }

    //------------------------------------------------------------------------------
    /* ImodelIGC* */    CurrentTarget::operator ImodelIGC* (void)
    {
        // first, get the ship and its target
        IshipIGC*   pShip = static_cast<IshipIGC*> (static_cast<ImodelIGC*> (*m_pTarget));
        ZAssert (pShip);
        ImodelIGC*  pTarget = pShip->GetCommandTarget (c_cmdCurrent);

        // there is an option to cache the target the first time this target is evaluated.
        // if that option is set...
        if (m_bLockOnFirstFetch)
        {

            // first check to see if we have done a fetch at all
            if (not m_pFetched)
            {
                // we haven't cached a target yet, so we need to get the target
                m_pFetched = new TypeIDTarget (pTarget ? pTarget->GetObjectType () : NA, pTarget ? pTarget->GetObjectID () : NA);
            }
            else
            {
                // we have cached a target, but the reason we want to cache the target is to 
                // determine if a kill has happened, e.g. has the target been eliminated. The
                // problem is that the player may choose to change the target before 
                // eliminating it, and then we are stuck waiting on the original target. Here
                // we check to see if the target has been changed w/o eliminating the
                // original target, and if so, adjust to compensate.
                ImodelIGC*  pFetched = *m_pFetched;
                if ((pFetched != pTarget) and pFetched and pTarget)
                {
                    // there has been such a change, so set up a new abstract target
                    delete m_pFetched;
                    m_pFetched = new TypeIDTarget (pTarget ? pTarget->GetObjectType () : NA, pTarget ? pTarget->GetObjectID () : NA);
                }
            }
            return (*m_pFetched);
        }

        // that option wasn't requested, so we just return the actual found target
        return pTarget;
    }

    //------------------------------------------------------------------------------
}
