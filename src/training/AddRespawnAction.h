#pragma once

#include "Action.h"

#include"CreateDroneAction.h"

namespace Training
{
    //------------------------------------------------------------------------------
    // class definitions
    //------------------------------------------------------------------------------
    class AddRespawnAction : public Action
    {
    public:
        /* void */          AddRespawnAction(CreateDroneAction* cda, float downtime, char spawnGroup=NA, bool spawnOnAdd = true);
        virtual /* void */          ~AddRespawnAction(void);
        virtual void                Execute(void);

    protected:
        CreateDroneAction*  m_cda;
        float               m_Downtime;
        char                m_SpawnGroup;
        bool                m_SpawnOnAdd;
    };

    //------------------------------------------------------------------------------
}