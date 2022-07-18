#include "pch.h"

//////////////////////////////////////////////////////////////////////////////
//
// TeleportPane
//
//////////////////////////////////////////////////////////////////////////////

class TeleportPane :
    public IItemEvent::Sink,
    public EventTargetContainer<TeleportPane>,
    public TrekClientEventSink
{
protected:
    TRef<ButtonPane>         m_pbuttonTeleport;
    TRef<ButtonPane>         m_pbuttonBack;
    TRef<ButtonBarPane>      m_pbuttonbarDestinations;
    TRef<ListPane>           m_plistPaneDestinations;
    TRef<IItemEvent::Source> m_peventDestinations;
    TRef<IItemEvent::Sink>   m_psinkDestinations;

    TVector<int>             m_viColumns;

    TRef<TListListWrapper<ImodelIGC*> > m_plistDestinations;

    enum 
    {
        sortName,
        sortType,
        sortSector,
    } m_sortDestination;

public:
    class DestinationPainter : public ItemPainter
    {
        const TVector<int>& m_viColumns;

    public:

        DestinationPainter(const TVector<int>& viColumns)
            : m_viColumns(viColumns) {};

        int GetXSize()
        {
            return m_viColumns[3];
        }

        int GetYSize()
        {
            return 14;
        }

        void Paint(ItemID pitemArg, Surface* psurface, bool bSelected, bool bFocus)
        {
            ImodelIGC* pDestination = (ImodelIGC*)pitemArg;
            IshipIGC* pship = NULL;
            IstationIGC* pstation = NULL;

            if (pDestination == NULL || trekClient.MyMission() == NULL)
                return;

            if (pDestination->GetObjectType() == OT_ship)
            {
                CastTo(pship, pDestination);
            }
            else
            {
                ZAssert(pDestination->GetObjectType() == OT_station);
                CastTo(pstation, pDestination);
            }



            // draw the selection bar

            if (bSelected) {
                psurface->FillRect(
                    WinRect(0, 0, GetXSize(), GetYSize()),
                    Color::Red()
                );
            }

            TRef<IEngineFont> pfont;

            // show the place we currently are in bold
            if (pship && pship == trekClient.GetShip()->GetParentShip()
                || pstation && pstation == trekClient.GetShip()->GetStation())
            {
                pfont = TrekResources::SmallBoldFont();
            }
            else
            {
                pfont = TrekResources::SmallFont();
            }
         
            Color color;

            if (CanKeepCurrentShip(pDestination))
            {
                color = Color::White();
            }
            else
            {
                color = 0.75f * Color::White();
            }

            // draw the name

            psurface->DrawString(
                pfont, 
                color,
                WinPoint(0, 0),
                pship ? pship->GetName() : pstation->GetName()
            );


            // draw the ship type (if any)
            if (pship)
            {
                psurface->DrawString(
                    pfont, 
                    color,
                    WinPoint(m_viColumns[0], 0),
                    HullName(pDestination)
                );
            }
            
            // draw the cluster
            psurface->DrawString(
                pfont, 
                color,
                WinPoint(m_viColumns[1] + 2, 0),
                SectorName(pDestination)
            );

            // draw the total number of turrets (if appropriate)
            if (pship && NumTurrets(pship))
            {
                int nNumTurrets = NumTurrets(pship);

                if (nNumTurrets != 0)
                {
                    psurface->DrawString(
                        pfont,
                        color,
                        WinPoint(m_viColumns[2] + 2, 0), 
                        ZString((int)MannedTurrets(pship)) + ZString("/") + ZString(nNumTurrets)
                    );
                }
            }
        }
    };


public:
    TeleportPane(Modeler* pmodeler)
        : m_peventDestinations(NULL), m_sortDestination(sortName)
    {
        TRef<IObject> pobjColumns;

        // a team pane is meaningless without a team
        if (trekClient.MyMission() == NULL)
            return;

        // Load the members from MDL

        TRef<INameSpace> pns = pmodeler->GetNameSpace("teleportpane");

        CastTo(m_pbuttonTeleport,               pns->FindMember("teleportTeleportButtonPane"));
        CastTo(m_pbuttonBack,                   pns->FindMember("teleportBackButtonPane"));
        CastTo(m_plistPaneDestinations,  (Pane*)pns->FindMember("teleportPaneDestinationListPane"));
        CastTo(pobjColumns,                     pns->FindMember("teleportPaneDestinationColumns"));
        CastTo(m_pbuttonbarDestinations,        pns->FindMember("teleportPaneDestinationListHeader"));

        //
        // Buttons
        //

		// mdvalley: Pointers and class names to shut up error C3867
        AddEventTarget(&TeleportPane::OnButtonTeleport,    m_pbuttonTeleport->GetEventSource());
        AddEventTarget(&TeleportPane::OnButtonBack,        m_pbuttonBack->GetEventSource());
        AddEventTarget(&TeleportPane::OnButtonBar,         m_pbuttonbarDestinations->GetEventSource());

        //
        // The Destination list
        //

        ParseIntVector(pobjColumns, m_viColumns);
        m_plistPaneDestinations->SetItemPainter(new DestinationPainter(m_viColumns));
        m_peventDestinations = m_plistPaneDestinations->GetSelectionEventSource();
        if (m_psinkDestinations)
            m_peventDestinations->RemoveSink(m_psinkDestinations);
        m_peventDestinations->AddSink(m_psinkDestinations = new IItemEvent::Delegate(this));

		// mdvalley: It's a pointer.
        AddEventTarget(&TeleportPane::OnButtonTeleport, m_plistPaneDestinations->GetDoubleClickEventSource());

        RebuildList();
    }

    ~TeleportPane()
    {
        if (m_peventDestinations)
        {
            m_peventDestinations->RemoveSink(m_psinkDestinations);
        }
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Event handlers
    //
    //////////////////////////////////////////////////////////////////////////////

    bool OnEvent(IItemEvent::Source *pevent, ItemID pitem)
    {
        if (pevent == m_peventDestinations) {
            OnSelectDestination(pitem);
        }

        return true;
    }

    void OnShipStatusChange(PlayerInfo* pPlayer)
    {
        // this might have been someone buying a ship with turrets.
        RebuildList();
    }

    void OnAddPlayer(MissionInfo* pMissionInfo, SideID sideID, PlayerInfo* pPlayerInfo)
    {
        // this might become someone in our list.
        RebuildList();
    }

    void OnDelPlayer(MissionInfo* pMissionInfo, SideID sideID, PlayerInfo* pPlayerInfo, QuitSideReason reason, const char* szMessageParam)
    {
        // this might be someone in our list.
        RebuildList(pPlayerInfo->GetShip());
    }

    void OnStationCaptured(StationID stationID, SideID sideID)
    {
        // this might be or have been one of our stations
        RebuildList();
    }

    void OnTechTreeChanged(SideID sideID)
    {
        if (sideID == trekClient.GetSideID())
        {
            RebuildList();
        }
    }

    void OnModelTerminated(ImodelIGC* pmodel)
    {
        // this might a station being destroyed
        if (pmodel->GetSide() == trekClient.GetSide())
            RebuildList(pmodel);
    }

    void OnDiscoveredStation(IstationIGC* pstation)
    {
        // this might be a new station we have built
        if (pstation->GetSide() == trekClient.GetSide())
            RebuildList();
    }

    bool OnSelectDestination(ItemID pitem)
    {
        return true;
    }


    void OnBoardFailed(IshipIGC* pshipRequestedParent)
    {
        if (trekClient.GetShip()->GetCluster() == NULL)
        {
            if (pshipRequestedParent 
                && MannedTurrets(pshipRequestedParent) < NumTurrets(pshipRequestedParent))
            {
                TRef<IMessageBox> pmsgBox = CreateMessageBox(
                    "Teleportation canceled - no turret positions were open.");
                trekClient.PlaySoundEffect(noTurretsSound);
                GetWindow()->GetPopupContainer()->OpenPopup(pmsgBox, false);
            }
            else
            {
                TRef<IMessageBox> pmsgBox = CreateMessageBox("Teleportation failed.");
                GetWindow()->GetPopupContainer()->OpenPopup(pmsgBox, false);
            }
        }
    }

    bool OnButtonTeleport()
    {
        ImodelIGC* pDestination = (ImodelIGC*)(m_plistPaneDestinations->GetSelection());
        IstationIGC* pstation = NULL;

        if (pDestination == NULL || trekClient.MyMission() == NULL 
            || trekClient.GetShip()->GetCluster() != NULL)
            return true;


        if (pDestination->GetObjectType() == OT_ship)
        {
            // if this is a ship, try boarding it.
            IshipIGC* pship;
            CastTo(pship, pDestination);

            if (trekClient.GetShip()->GetParentShip() != NULL)
            {
                trekClient.DisembarkAndBoard(pship);
            }
            else
            {
                trekClient.BoardShip(pship);
            }
            trekClient.PlaySoundEffect(personalJumpSound);
        }
        else
        {
            // try teleporting to that station
            IstationIGC* pstation;
            CastTo(pstation, pDestination);

            if (trekClient.GetShip()->GetParentShip() != NULL)
            {
                // if they are already there, just get off of the ship
                if (pstation == trekClient.GetShip()->GetParentShip()->GetStation())
                {
                    trekClient.BoardShip(NULL);
                }
                else
                {
                    trekClient.DisembarkAndTeleport(pstation);
                }
            }
            else
            {
                if (pstation != trekClient.GetShip()->GetStation())
                {
                    trekClient.SetMessageType(BaseClient::c_mtGuaranteed);
                    BEGIN_PFM_CREATE(trekClient.m_fm, pfmDocked, C, DOCKED)
                    END_PFM_CREATE

                    pfmDocked->stationID = pstation->GetObjectID();

                    trekClient.StartLockDown(
                        "Teleporting to " + ZString(pstation->GetName()) + "....", 
                        BaseClient::lockdownTeleporting);
                }
            }
        }

        // dismiss the teleport pane.
        GetWindow()->TurnOffOverlayFlags(ofTeleportPane);

        return true;
    }

    bool OnButtonBack()
    {
        GetWindow()->TurnOffOverlayFlags(ofTeleportPane);

        return true;
    }

    bool OnButtonBar(int iButton)
    {
        return true;
    }

    static bool CanBoard(IshipIGC* pship)
    {
        PlayerInfo* pPlayer = (PlayerInfo*)pship->GetPrivateData();

        if (pship != trekClient.GetShip() 
            && pship->GetSide() == trekClient.GetSide()
            && pPlayer->LastSeenState() == c_ssDocked 
            && pship->GetPilotType() >= c_ptPlayer)
        {
            HullID          hid = pPlayer->LastSeenShipType();
            ZAssert (hid != NA);
            IhullTypeIGC*   pht = trekClient.m_pCoreIGC->GetHullType(hid);
            ZAssert (pht);

            if ((trekClient.GetShip()->GetPilotType() == c_ptCheatPlayer) ||
                (pht->GetMaxFixedWeapons() != pht->GetMaxWeapons()))
            {
                return true;
            }
        }

        return false;
    }

    void RebuildList(ImodelIGC* pmodelExclude = NULL)
    {
        TRef<TListListWrapper<ImodelIGC*> > plistNew = new TListListWrapper<ImodelIGC*>();

        IsideIGC* pside = trekClient.GetSide();

        if (pside && trekClient.GetSideID() != SIDE_TEAMLOBBY)
        {
            for (ShipLinkIGC*   pshiplink = pside->GetShips()->first();
                 (pshiplink != NULL);
                 pshiplink = pshiplink->next())
            {
                IshipIGC* pship = pshiplink->data();
            
                if (pship != pmodelExclude && CanBoard(pship))
                {
                    plistNew->PushEnd(pship);
                }
            }

            for (StationLinkIGC*   pstationlink = pside->GetStations()->first();
                 (pstationlink != NULL);
                 pstationlink = pstationlink->next())
            {
                IstationIGC* pstation = pstationlink->data();
            
                if (pstation != pmodelExclude 
                    && pstation->GetBaseStationType()->HasCapability(c_sabmRestart))
                {
                    plistNew->PushEnd(pstation);
                }
            }
        }

        m_plistDestinations = plistNew;
        m_plistPaneDestinations->SetList(m_plistDestinations);
    }

    static ZString HullName(ImodelIGC* pmodel)
    {
        if (pmodel->GetObjectType() == OT_ship)
        {
            PlayerInfo* pPlayer = (PlayerInfo*)((IshipIGC*)pmodel)->GetPrivateData();

            IhullTypeIGC* phull = trekClient.GetCore()->GetHullType(pPlayer->LastSeenShipType());
   
            if (phull == NULL)
                return "<bug>";
            else
                return phull->GetName();
        }
        else
        {
            return "";
        }
    }

    static bool CanKeepCurrentShip(ImodelIGC* pmodel)
    {
        if (pmodel->GetObjectType() == OT_ship)
        {
            // they can keep their ship only if they don't have one or are in a lifepod
            return trekClient.GetShip()->GetParentShip() != NULL
                || trekClient.GetShip()->GetBaseHullType()->HasCapability(c_habmLifepod);
        }
        else
        {
            IstationIGC* pstation;
            CastTo(pstation, pmodel);
            IhullTypeIGC* pht = trekClient.GetShip()->GetBaseHullType();

            // they can keep their current ship if they are already in this station or 
            // if the station can build the hull which they currently have.  
            return trekClient.GetShip()->GetStation() == pstation 
                ||  pht && pstation->CanBuy(pht);
        }
    }

    static ZString SectorName(ImodelIGC* pmodel)
    {
        IclusterIGC* psector;

        if (pmodel->GetObjectType() == OT_ship)
        {
            PlayerInfo* pplayer = (PlayerInfo*)((IshipIGC*)pmodel)->GetPrivateData();

            psector = trekClient.GetCore()->GetCluster(pplayer->LastSeenSector());
        }
        else
        {
            psector = pmodel->GetCluster();
        }


        if (psector == NULL)
            return "unknown";
        else
            return psector->GetName();
    }

    static int MannedTurrets(IshipIGC* pshipParent)
    {
        // loop through all of the ships on that side and count the ones 
        // that are turrets of this.
        int cMannedTurrets = 0;
        
        const ShipListIGC* shipList = pshipParent->GetSide()->GetShips();
        for (const ShipLinkIGC* lShip = shipList->first(); lShip; lShip = lShip->next())
        {
            IshipIGC* pship = lShip->data();
            PlayerInfo* pplayer = (PlayerInfo*)pship->GetPrivateData();

            if (pplayer->LastSeenState() == c_ssTurret)                
            {
                PlayerInfo* pplayerParent = trekClient.FindPlayer(pplayer->LastSeenParent());

                if (pplayerParent && pplayerParent->GetShip() == pshipParent)
                    cMannedTurrets++;
            }
        }

        return cMannedTurrets;
    }

    static int NumTurrets(IshipIGC* pship)
    {
        PlayerInfo* pPlayer = (PlayerInfo*)pship->GetPrivateData();
        const IhullTypeIGC* pHullType = trekClient.GetCore()->GetHullType(pPlayer->LastSeenShipType());
         
        // if we have a real ship...
        if (pHullType)
            return pHullType->GetMaxWeapons() - pHullType->GetMaxFixedWeapons();
        else
            return 0;
    }
};

//////////////////////////////////////////////////////////////////////////////
//
// Constructor
//
//////////////////////////////////////////////////////////////////////////////

TRef<IObject> CreateTeleportPane(Modeler* pmodeler)
{
    return (IItemEvent::Sink*)new TeleportPane(pmodeler);
}

