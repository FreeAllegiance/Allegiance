/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	getshipisdestroyedcondition.h
**
**  Author:  
**
**  Description:
**      Header file for the training library "getshipisdestroyedcondition" interface.
**
**  History:
*/
#pragma once

#include "GetShipIsDamagedCondition.h"

#include "NotCondition.h"

namespace Training
{
    #define GetShipIsDestroyedCondition(ship)               NotCondition (new GetShipIsDamagedCondition (ship, 0.0f))
    #define GetShipIsDestroyedCondition2(shipType, shipID)   NotCondition (new GetShipIsDamagedCondition (shipType, shipID, 0.0f))
}