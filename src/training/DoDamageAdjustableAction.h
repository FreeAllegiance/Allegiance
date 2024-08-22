/*
**  File:	DoDamageAdjustableAction.h
**
**  Author:  Adam Davison
**
**  Description:
**      Header file for the training library "DoDamageAdjustableAction" interface.
**
**  History:
*/

#pragma once

#include "Action.h"

namespace Training
{
    class DoDamageAdjustableAction : public Action
    {
        public:
                    /* void */          DoDamageAdjustableAction (ShipID ship, float amount);
            virtual /* void */          ~DoDamageAdjustableAction (void);
            virtual void                Execute (void);

        protected:
					ShipID				m_shipID;
					float				m_amount;
    };
}