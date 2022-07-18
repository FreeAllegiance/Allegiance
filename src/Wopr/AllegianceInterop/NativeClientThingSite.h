#pragma once

#include "igc.h"

// Direct copy of PigsThingSite.h
class NativeClientThingSite : public ThingSite
{
private:
	//IclusterIGC * currentCluster = nullptr;
	//ImodelIGC*   currentModel = nullptr;
	//SideVisibility              m_sideVisibility[c_cSidesMax];

	IsideIGC*					m_clientSide;

	SideVisibility              m_sideVisibility;
	SideVisibility              m_enemySideVisibility;
	bool                        m_bSideVisibility;
	bool                        m_bIsShip;

public:

	NativeClientThingSite(IsideIGC* clientSide) : ThingSite()
	{
		m_clientSide = clientSide;
	}

	// Taken from trekigc's implementation.
	//bool GetSideVisibility(IsideIGC* side)
	//{
	//	bool currentEye = false;

	//	if (currentCluster == nullptr)
	//		return false;

	//	NativeClientClusterSite *clusterSite = (NativeClientClusterSite *)currentCluster->GetClusterSite();
	//	const ScannerListIGC * scanners = clusterSite->GetScanners(side->GetObjectID());

	//	if (currentCluster == nullptr || clusterSite == nullptr || scanners == nullptr || scanners->n() < 1)
	//		return currentEye;

	//	IscannerIGC * lastSpotter = m_sideVisibility[side->GetObjectID()].pLastSpotter();

	//	if (lastSpotter && lastSpotter->InScannerRange(currentModel))
	//	{
	//		currentEye = true;
	//	}
	//	else
	//	{
	//		//do it the hard way
	//		for (ScannerLinkIGC* l = currentCluster->GetClusterSite()->GetScanners(side->GetObjectID())->first(); (l != NULL); l = l->next())
	//		{
	//			IscannerIGC*   s = l->data();

	//			if (s->InScannerRange(currentModel))
	//			{
	//				currentEye = true;
	//				m_sideVisibility[side->GetObjectID()].pLastSpotter(s);
	//				break;
	//			}
	//		}
	//	}

	//	return currentEye;
	//}

	//void        SetCluster(ImodelIGC*   pmodel, IclusterIGC* pcluster)
	//{
	//	currentCluster = pcluster;
	//	currentModel = pmodel;
	//}



	// Taken from most recent trekigc version.
	void SetCluster(ImodelIGC* pmodel, IclusterIGC* pcluster)
	{
		ModelAttributes ma = pmodel->GetAttributes();

		m_bSideVisibility = (ma & c_mtSeenBySide) != 0;
		m_bIsShip = (pmodel->GetObjectType() == OT_ship);

		if (pcluster)
		{
			if ((ma & c_mtPredictable) || !m_bSideVisibility)
			{
				m_sideVisibility.fVisible(true);
				m_sideVisibility.CurrentEyed(true);  //Xynth #100 7/2010
				m_enemySideVisibility.fVisible(true);
				m_enemySideVisibility.CurrentEyed(true);

				switch (pmodel->GetObjectType())
				{
				case OT_asteroid:
					/*trekClient.GetClientEventSource()->
						OnDiscoveredAsteroid((IasteroidIGC*)pmodel);*/
					break;

				case OT_station:
					/*trekClient.GetClientEventSource()->
						OnDiscoveredStation((IstationIGC*)pmodel);*/
					break;

				case OT_probe:
				{
					if (((IprobeIGC*)pmodel)->GetProbeType()->HasCapability(c_eabmRipcord))
					{
						IsideIGC* pside = pmodel->GetSide();

						if ((pside != m_clientSide) && (!pside->AlliedSides(pside, m_clientSide))) //ALLY - imago 7/3/09
						{
							ZAssert(pside);
							ClusterSite*    pcs = pcluster->GetClusterSite();
							pcs->SetClusterAssetMask(pcs->GetClusterAssetMask() | c_amEnemyTeleport);

							/*trekClient.PostText(true, START_COLOR_STRING "%s %s" END_COLOR_STRING " spotted in %s",
								(PCC)ConvertColorToString(pside->GetColor()), pside->GetName(), GetModelName(pmodel), pcluster->GetName());*/
						}
						else
						{
							/*trekClient.PostText(true, "%s deployed in %s", GetModelName(pmodel), pcluster->GetName());*/
						}
					}
				}
				}
			}
			else
				UpdateSideVisibility(pmodel, pcluster);
		}
		else
		{
			/*if (m_pSoundSite)
				m_pSoundSite->StopSounds();*/
		}
	}

	void UpdateSideVisibility(ImodelIGC* pmodel, IclusterIGC* pcluster)
	{
		//Xynth #100 7/2010
		bool currentEye = false;
		//We can only update it if we have one & if the client is actually on a side.
		if (m_bSideVisibility && m_clientSide)
		{
			//Update the visibility of hidden or non-static objects
			//(visibile static objects stay visible)
			//if (!(m_sideVisibility.fVisible() && (pmodel->GetAttributes() & c_mtPredictable))) Xynth #100 we need to run test for static objects
			//{
			//We, trivially, see anything on our side. beyond that ...  Imago ALLY VISIBILITY 7/11/09
			//does the ship that saw the object last still see it
			//(if such a ship exists)
			if (
				(m_clientSide == pmodel->GetSide()) ||
				//(m_clientSide->AlliedSides(pmodel->GetSide(), m_clientSide) && trekClient.MyMission()->GetMissionParams().bAllowAlliedViz) ||
				(m_sideVisibility.pLastSpotter() && m_sideVisibility.pLastSpotter()->InScannerRange(pmodel) && m_clientSide == m_sideVisibility.pLastSpotter()->GetSide()) 
				//(m_sideVisibility.pLastSpotter() && (m_clientSide->AlliedSides(m_sideVisibility.pLastSpotter()->GetSide(), m_clientSide) && m_clientSide != m_sideVisibility.pLastSpotter()->GetSide()) && m_sideVisibility.pLastSpotter()->InScannerRange(pmodel) && trekClient.MyMission()->GetMissionParams().bAllowAlliedViz)
				)
			{
				//yes
				currentEye = true; //Xynth #100 7/2010
				if (!m_sideVisibility.fVisible())
				{
					if (m_bIsShip)
					{
						IsideIGC* pside = pmodel->GetSide();

						//this is where you would put in rate limiting for the sound effects if someone "eye" spams };-) -imago
						if ((pside != m_clientSide) && (!pside->AlliedSides(pside, m_clientSide))) //ALLY Imago 7/3/09
						{
							//trekClient.PlaySoundEffect(newShipSound, pmodel);
						}
						else
						{
							//trekClient.PlaySoundEffect(newEnemySound, pmodel);
						}
					}
					//Xynth #100 7/2010 m_sideVisibility.fVisible(true);
				}
			}
			else
			{
				//do it the hard way
				currentEye = false; //Xynth #100 7/2010 m_sideVisibility.fVisible(false);
				for (ScannerLinkIGC*   l = pcluster->GetClusterSite()->GetScanners(0)->first();
					(l != NULL);
					l = l->next())
				{
					IscannerIGC*   s = l->data();
					ZAssert(s->GetCluster() == pcluster);

					if (s->InScannerRange(pmodel))
					{
						//Ship s's side does not see the ship but this ship does
						/*if (m_bIsShip)
							trekClient.PlaySoundEffect(newShipSound, pmodel);*/

						currentEye = true; //Xynth #100 7/2010 m_sideVisibility.fVisible(true);
						m_sideVisibility.pLastSpotter(s);
						//if (trekClient.MyMission()->GetMissionParams().bAllowAlliedViz) //ALLY should be SCAN Imago 7/13/09
						//{
						//	//lets get a list of allied sideIDs
						//	for (SideLinkIGC* psidelink = trekClient.GetCore()->GetSides()->first();
						//		(psidelink != NULL);
						//		psidelink = psidelink->next())
						//	{
						//		IsideIGC*   otherside = psidelink->data();
						//		//this spotter's side is ally...and not ours...and we dont already see it
						//		if (s->GetSide()->AlliedSides(s->GetSide(), otherside) && s->GetSide() != otherside && !pmodel->SeenBySide(otherside))
						//			pmodel->SetSideVisibility(otherside, true);
						//	}
						//}
					}
				}
			}
			//}

			/*if (!trekClient.m_fm.IsConnected()) {
				if (pmodel->GetObjectType() == OT_ship) {
					IshipIGC* pShip = (IshipIGC*)pmodel;
					if (!m_sideVisibility.CurrentEyed() && currentEye) {
						debugf(">>UpdateSideVis for %s\n", pmodel->GetName());
						PlayerInfo* ppi = (PlayerInfo*)(pShip->GetPrivateData());
						if (ppi) {
							ShipStatus ss = ppi->GetShipStatus();
							ss.SetStateTime(trekClient.m_lastUpdate.clock());
							ss.SetDetected(true);
							ss.SetSectorID(pcluster->GetObjectID());
							ss.SetState(c_ssFlying);
							ss.SetHullID(((IshipIGC*)pmodel)->GetHullType()->GetObjectID());
							if (pmodel->GetCluster())
								ss.SetState(c_ssFlying);
							ppi->SetShipStatus(ss);
						}
					}
					else if (m_sideVisibility.CurrentEyed() && !currentEye) {
						debugf(">>UpdateSideVis for %s - hide\n", pmodel->GetName());
						PlayerInfo* ppi = (PlayerInfo*)(pShip->GetPrivateData());
						if (ppi) {
							ShipStatus ss = ppi->GetShipStatus();
							ss.SetDetected(false);
							ss.SetUnknown();
							ppi->SetShipStatus(ss);
						}
					}
				}
			}*/

			m_sideVisibility.CurrentEyed(currentEye);
			currentEye = currentEye || (m_sideVisibility.fVisible() && (pmodel->GetAttributes() & c_mtPredictable));
			m_sideVisibility.fVisible(currentEye);

		}
		//if (!trekClient.m_fm.IsConnected()) {
		//	// Done with player side, now update visibilities for the enemy -- ignore allies for now
		//	currentEye = false;
		//	if ((pmodel->GetSide() != m_clientSide) ||
		//		(m_enemySideVisibility.pLastSpotter() && m_enemySideVisibility.pLastSpotter()->InScannerRange(pmodel))) //&& m_clientSide != m_enemySideVisibility.pLastSpotter()->GetSide()) ||
		//	{
		//		currentEye = true;
		//	}
		//	else
		//	{
		//		//do it the hard way
		//		for (ScannerLinkIGC* l = pcluster->GetClusterSite()->GetScanners(1)->first(); (l != NULL); l = l->next())
		//		{
		//			IscannerIGC*   s = l->data();

		//			if (s->InScannerRange(pmodel))
		//			{
		//				currentEye = true;
		//				m_enemySideVisibility.pLastSpotter(s);
		//				break;
		//			}
		//		}
		//	}
		//	m_enemySideVisibility.CurrentEyed(currentEye);
		//	currentEye = currentEye || (m_enemySideVisibility.fVisible() && (pmodel->GetAttributes() & c_mtPredictable));
		//	m_enemySideVisibility.fVisible(currentEye);


		//	if (pmodel->GetObjectType() == OT_ship) {
		//		IshipIGC* pShip = (IshipIGC*)pmodel;
		//		if (pShip == trekClient.GetShip()) {
		//			// let the player know if he is eyed
		//			PlayerInfo* ppi = (PlayerInfo*)(pShip->GetPrivateData());
		//			if (ppi) {
		//				ShipStatus ss = ppi->GetShipStatus();
		//				ss.SetDetected(currentEye); // For UI eye
		//				ppi->SetShipStatus(ss);
		//			}
		//		}
		//	}
		//}

	}

	bool GetSideVisibility(IsideIGC* side)
	{
		ZAssert(side);

		//if (Training::IsTraining())
		//{
		//	//if ((trekClient.GetShip()->GetSide() != side) && (Training::GetTrainingMissionID() != Training::c_TM_6_Practice_Arena) && (Training::GetTrainingMissionID() != Training::c_TM_10_Free_Flight))
		//	//    return false;
		//	if (side != m_clientSide)
		//		return m_enemySideVisibility.fVisible();
		//}
		return m_sideVisibility.fVisible();
	}

	bool GetCurrentEye(IsideIGC* side)
	{
		ZAssert(side);
		/*if (!trekClient.m_fm.IsConnected() && side != m_clientSide)
			return m_enemySideVisibility.CurrentEyed();*/
		return m_sideVisibility.CurrentEyed();
	}

	void SetSideVisibility(IsideIGC* side, bool fVisible)
	{
		if (m_bSideVisibility) {
			if (side == m_clientSide) // || (side->AlliedSides(side,m_clientSide) && trekClient.MyMission()->GetMissionParams().bAllowAlliedViz) ) ) //imago viz ALLY VISIBILITY 7/11/09
			{
				m_sideVisibility.fVisible(fVisible);
				m_sideVisibility.CurrentEyed(fVisible);
			}
			/*else if (!trekClient.m_fm.IsConnected())
			{
				m_enemySideVisibility.fVisible(fVisible);
				m_enemySideVisibility.CurrentEyed(fVisible);
			}*/
		}
		/*if (fVisible)
		{
			switch (m_pmodel->GetObjectType())
			{
			case OT_asteroid:
				trekClient.GetClientEventSource()->
					OnDiscoveredAsteroid((IasteroidIGC*)m_pmodel);
				break;

			case OT_station:
				trekClient.GetClientEventSource()->
					OnDiscoveredStation((IstationIGC*)m_pmodel);
				break;
			}
		}*/
	}

};

