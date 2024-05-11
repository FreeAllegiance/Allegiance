/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	notcondition.h
**
**  Author:  
**
**  Description:
**      Header file for the training library "notcondition" interface.
**
**  History:
*/
#pragma once

#include "Predicate.h"

namespace Training
{
    #define NotCondition(pCondition)  Predicate (pCondition, false)
}