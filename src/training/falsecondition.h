/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	falsecondition.h
**
**  Author:  
**
**  Description:
**      Header file for the training library "falsecondition" interface.
**
**  History:
*/
#pragma once

#include "NotCondition.h"

#include "TrueCondition.h"

namespace Training
{
    #define FalseCondition  NotCondition (new TrueCondition)
}