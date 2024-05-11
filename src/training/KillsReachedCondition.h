#pragma once

#include "Condition.h"

namespace Training
{
    //------------------------------------------------------------------------------
    // class definitions
    //------------------------------------------------------------------------------
    class KillsReachedCondition : public Condition
    {
    public:
                /* void */          KillsReachedCondition(int kills);
                /* void */          ~KillsReachedCondition(void);
                bool                Evaluate(void);

    protected:
        int m_kills;
    };

    //------------------------------------------------------------------------------
}