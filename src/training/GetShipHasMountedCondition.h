/*
**
**  File:	getshiphasmountedcondition.h
**
**  Author:  
**
**  Description:
**      Header file for the training library "getshiphasmountedcondition" interface.
**
**  History:
*/
#pragma once

#include "Condition.h"

namespace Training
{
    //------------------------------------------------------------------------------
    // class definitions
    //------------------------------------------------------------------------------
    class GetShipHasMountedCondition : public Condition
    {
        public:
                    /* void */          GetShipHasMountedCondition (const TRef<IshipIGC>& ship, PartID pPart);
            virtual /* void */          ~GetShipHasMountedCondition (void);
            virtual bool                Evaluate (void);

        protected:
                    TRef<IshipIGC>      m_ship;
                    PartID	            m_part;
    };

    //------------------------------------------------------------------------------
}