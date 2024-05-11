/*
**
**  File:	setcargonanaction.cpp
**
**  Author:  
**
**  Description:
**      Implementation of the training library "setcargonanaction" interface.
**
**  History:
*/
#include    "pch.h"

#include "TrainingMission.h"

#include    "SetCargoNanAction.h"

namespace Training
{
    //------------------------------------------------------------------------------
    // class methods
    //------------------------------------------------------------------------------
    /* void */  SetCargoNanAction::SetCargoNanAction (ShipID theShip)
    {
       nanShipID = theShip;
    }

    //------------------------------------------------------------------------------
    /* void */  SetCargoNanAction::~SetCargoNanAction (void)
    {
    }

    //------------------------------------------------------------------------------
    void        SetCargoNanAction::Execute (void)
    {
		IshipIGC*		nanShip = trekClient.GetSide()->GetShip(nanShipID);

		PartData        nan2;
        nan2.partID = 190;
        nan2.mountID = 0;
        nan2.amount = 0;

		PartData        gat1;
        gat1.partID = 33;
        gat1.mountID = 0;
        gat1.amount = 0;
		
		PartData        prox1;
        prox1.partID = 59;
        prox1.mountID = 0;
        prox1.amount = 0x7fff;

        IpartIGC*       pPart = nanShip->GetMountedPart (ET_Weapon, 0);
		IpartIGC*       pPart2 = nanShip->GetMountedPart(ET_Dispenser, 0);

		if (pPart2)
			pPart2->Terminate();
	
		nanShip->CreateAndAddPart(&prox1);

        if (!pPart)
			nanShip->CreateAndAddPart(&nan2);

		pPart = nanShip->GetMountedPart (ET_Weapon, 0);

		if (pPart && pPart->GetPartType()->GetObjectID() == 190)
		{
			for (int i = -1; i >= -c_maxCargo; i--) {
				pPart = nanShip->GetMountedPart(NA, i);
				if (pPart)
					pPart->Terminate();

				if (i == -1) {
					// Put gat in cargo
					gat1.mountID = i;
					nanShip->CreateAndAddPart(&gat1);
				} 
				else 
				{
					PartData newProx;
					newProx.partID = 59;
					newProx.mountID = i;
					newProx.amount = 0x7fff;
					nanShip->CreateAndAddPart(&newProx);
				}
			}
		}
		else if (pPart)
		{
			for (int i = -1; i >= -c_maxCargo; i--) {
				pPart = nanShip->GetMountedPart(NA, i);
				if (pPart)
					pPart->Terminate();

				if (i == -1) {
					// Put nan in cargo
					nan2.mountID = i;
					nanShip->CreateAndAddPart(&nan2);
				} 
				else 
				{
					PartData newProx;
					newProx.partID = 59;
					newProx.mountID = i;
					newProx.amount = 0x7fff;
					nanShip->CreateAndAddPart(&newProx);
				}
			}
		}
    }
}
