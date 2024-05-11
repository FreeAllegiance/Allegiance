/*
**  Copyright (C) 1999 Microsoft Corporation. All Rights Reserved.
**
**  File:	AbstractTarget.h
**
**  Author:  
**
**  Description:
**      Header file for the training library "AbstractTarget" interface.
**
**  History:
*/
#pragma once

namespace Training
{
    //------------------------------------------------------------------------------
    // class definitions
    //------------------------------------------------------------------------------
    class AbstractTarget
    {
        public:
            virtual /* void */          ~AbstractTarget (void);
            virtual /* ImodelIGC* */    operator ImodelIGC* (void) = 0;
                    ImodelIGC*          operator -> (void);
    };

    //------------------------------------------------------------------------------
}