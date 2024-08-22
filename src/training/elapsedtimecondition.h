/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	elapsedtimecondition.h
**
**  Author:  
**
**  Description:
**      Header file for the training library "elapsedtimecondition" interface.
**
**  History:
*/
#pragma once

#include "PeriodicCondition.h"

#include "NTimesCondition.h"

#include "TrueCondition.h"

namespace Training
{
    #define ElapsedTimeCondition(fTime)  NTimesCondition (new PeriodicCondition (new TrueCondition, fTime), 2, true)
}