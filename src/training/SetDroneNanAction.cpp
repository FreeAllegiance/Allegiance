/*
**
**  File:	setdronenanaction.cpp
**
**  Author:  
**
**  Description:
**      Implementation of the training library "setdronenanaction" interface.
**
**  History:
*/
#include    "pch.h"

#include "TrainingMission.h"

#include    "SetDroneNanAction.h"

namespace Training
{
    //------------------------------------------------------------------------------
    // class methods
    //------------------------------------------------------------------------------
    /* void */  SetDroneNanAction::SetDroneNanAction (ShipID theShip)
    {
       nanShipID = theShip;
    }

    //------------------------------------------------------------------------------
    /* void */  SetDroneNanAction::~SetDroneNanAction (void)
    {
    }

    //------------------------------------------------------------------------------
    void        SetDroneNanAction::Execute (void)
    {
		IshipIGC*		nanShip = trekClient.GetSide()->GetShip(nanShipID);
		IpartIGC*       pPart = nanShip->GetMountedPart (ET_Weapon, 0);
		if (pPart)
			pPart->Terminate ();

		// Stick Nanite 2 on the front
		PartData        pd;
        pd.partID = 190;
        pd.mountID = 0;
        pd.amount = 0;
        nanShip->CreateAndAddPart (&pd);

    }
}
