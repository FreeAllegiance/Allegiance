/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	CreateProbeAction.h
**
**  Author:  
**
**  Description:
**      Header file for the training library "CreateProbeAction" interface.
**
**  History:
*/
#pragma once

#include "CreateObjectAction.h"

namespace Training
{
    //------------------------------------------------------------------------------
    // class definitions
    //------------------------------------------------------------------------------
    class CreateProbeAction : public CreateObjectAction
    {
        public:
                    /* void */          CreateProbeAction (ProbeID probeID, ExpendableTypeID typeID, const Vector& position, SideID sideID, SectorID sectorID);
            virtual /* void */          ~CreateProbeAction (void);
            virtual void                Execute (void);

        protected:
                    ProbeID             m_probeID;
                    ExpendableTypeID    m_typeID;
                    Vector              m_position;
                    SideID              m_sideID;
                    SectorID            m_sectorID;
                    DataProbeIGC        m_probeData;
    };

    //------------------------------------------------------------------------------
}