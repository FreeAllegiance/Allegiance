#include "pch.h"
#include "trekctrls.h"
#include "screen.h"

#include <button.h>
#include <controls.h>
#include <frameimage.h>

TRef<ModifiableNumber>  g_pnumberInTurretNumber;

//////////////////////////////////////////////////////////////////////////////
//
// HangarScreen
//
//////////////////////////////////////////////////////////////////////////////

class HangarScreen :
    public Screen,
    public EventTargetContainer<HangarScreen>,
    public TrekClientEventSink
{
private:
    TRef<FrameImageButtonPane> m_pbuttonLaunch;
    TRef<ButtonPane> m_pbuttonCommand;
    TRef<ButtonPane> m_pbuttonLoadout;
    TRef<ButtonPane> m_pbuttonInvest;
    TRef<ButtonPane> m_pbuttonTeleport;
    TRef<ButtonPane> m_pbuttonTeam;
    TRef<ButtonPane> m_pbuttonHelp;
    TRef<ButtonPane> m_pbuttonExit;
    TRef<Pane>                 m_ppane;
    Modeler*                   m_pmodeler;
static  int                    m_iInTurretNumber;

    enum
    {
        hoverNone,
        hoverLaunch,
        hoverCommand,
        hoverLoadout,
        hoverInvest,
        hoverHelp,
        hoverTeleport,
        hoverTeam,
        hoverExit
    };

    TRef<ModifiableNumber>  m_pnumberCurrentHover;

public:
    HangarScreen(Modeler* pmodeler, const ZString& strNamespace) :
        m_pmodeler(pmodeler)
    {
        m_pmodeler->SetColorKeyHint(false);
        TRef<INameSpace> pnsHangarData = m_pmodeler->CreateNameSpace("hangardata", m_pmodeler->GetNameSpace("gamepanes"));

        pnsHangarData->AddMember("hoverNone",    new Number(hoverNone   ));
        pnsHangarData->AddMember("hoverLaunch",  new Number(hoverLaunch ));
        pnsHangarData->AddMember("hoverCommand", new Number(hoverCommand));
        pnsHangarData->AddMember("hoverLoadout", new Number(hoverLoadout));
        pnsHangarData->AddMember("hoverInvest",  new Number(hoverInvest ));
        pnsHangarData->AddMember("hoverHelp",    new Number(hoverHelp   ));
        pnsHangarData->AddMember("hoverTeleport",new Number(hoverTeleport));
        pnsHangarData->AddMember("hoverTeam",    new Number(hoverTeam   ));
        pnsHangarData->AddMember("hoverExit",    new Number(hoverExit   ));

        pnsHangarData->AddMember("CurrentHover", m_pnumberCurrentHover = new ModifiableNumber(hoverNone));
        pnsHangarData->AddMember("InTurretNumber", g_pnumberInTurretNumber = new ModifiableNumber(m_iInTurretNumber));

        // Load the members from MDL

        TRef<INameSpace> pns = pmodeler->GetNameSpace(strNamespace);

        CastTo(m_ppane,          pns->FindMember("screen"));
        CastTo(m_pbuttonCommand, pns->FindMember("commandButtonPane"));
        CastTo(m_pbuttonLoadout, pns->FindMember("loadoutButtonPane"));
        CastTo(m_pbuttonInvest,  pns->FindMember("investButtonPane"));
        CastTo(m_pbuttonHelp,    pns->FindMember("helpButtonPane"));
        CastTo(m_pbuttonTeleport,pns->FindMember("teleportButtonPane"));
        CastTo(m_pbuttonTeam,    pns->FindMember("teamButtonPane"));

        CastTo(m_pbuttonLaunch,  pns->FindMember("launchButtonPane"));
        CastTo(m_pbuttonExit,  pns->FindMember("exitButtonPane"));

        pmodeler->UnloadNameSpace(strNamespace);

        //
        // Buttons
        //

		// mdvalley: pointers and class.
        AddEventTarget(&HangarScreen::OnButtonLaunch,  m_pbuttonLaunch->GetEventSource());
        AddEventTarget(&HangarScreen::OnButtonCommand, m_pbuttonCommand->GetEventSource());
        AddEventTarget(&HangarScreen::OnButtonLoadout, m_pbuttonLoadout->GetEventSource());
        AddEventTarget(&HangarScreen::OnButtonInvest,  m_pbuttonInvest->GetEventSource());
        AddEventTarget(&HangarScreen::OnButtonHelp,    m_pbuttonHelp->GetEventSource());
        AddEventTarget(&HangarScreen::OnButtonTeleport,  m_pbuttonTeleport->GetEventSource());
        AddEventTarget(&HangarScreen::OnButtonTeam,    m_pbuttonTeam->GetEventSource());
        AddEventTarget(&HangarScreen::OnButtonExit,    m_pbuttonExit->GetEventSource());

        AddEventTarget(&HangarScreen::OnHoverLaunch,   m_pbuttonLaunch->GetMouseEnterEventSource());
        AddEventTarget(&HangarScreen::OnHoverCommand,  m_pbuttonCommand->GetMouseEnterEventSource());
        AddEventTarget(&HangarScreen::OnHoverLoadout,  m_pbuttonLoadout->GetMouseEnterEventSource());
        AddEventTarget(&HangarScreen::OnHoverInvest,   m_pbuttonInvest->GetMouseEnterEventSource());
        AddEventTarget(&HangarScreen::OnHoverHelp,     m_pbuttonHelp->GetMouseEnterEventSource());
        AddEventTarget(&HangarScreen::OnHoverTeleport,   m_pbuttonTeleport->GetMouseEnterEventSource());
        AddEventTarget(&HangarScreen::OnHoverTeam,     m_pbuttonTeam->GetMouseEnterEventSource());
        AddEventTarget(&HangarScreen::OnHoverExit,     m_pbuttonExit->GetMouseEnterEventSource());

        AddEventTarget(&HangarScreen::OnHoverNone,     m_pbuttonLaunch->GetMouseLeaveEventSource());
        AddEventTarget(&HangarScreen::OnHoverNone,     m_pbuttonCommand->GetMouseLeaveEventSource());
        AddEventTarget(&HangarScreen::OnHoverNone,     m_pbuttonLoadout->GetMouseLeaveEventSource());
        AddEventTarget(&HangarScreen::OnHoverNone,     m_pbuttonInvest->GetMouseLeaveEventSource());
        AddEventTarget(&HangarScreen::OnHoverNone,     m_pbuttonHelp->GetMouseLeaveEventSource());
        AddEventTarget(&HangarScreen::OnHoverNone,     m_pbuttonTeleport->GetMouseLeaveEventSource());
        AddEventTarget(&HangarScreen::OnHoverNone,     m_pbuttonTeam->GetMouseLeaveEventSource());
        AddEventTarget(&HangarScreen::OnHoverNone,     m_pbuttonExit->GetMouseLeaveEventSource());

        if (!trekClient.MyMission()->AllowDevelopments())
            m_pbuttonInvest->SetHidden(true);
    }

    ~HangarScreen()
    {
        m_pmodeler->UnloadNameSpace("hangardata");
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Screen Methods
    //
    //////////////////////////////////////////////////////////////////////////////

    Pane* GetPane()
    {
        return m_ppane;
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Event handlers
    //
    //////////////////////////////////////////////////////////////////////////////

    bool OnButtonLaunch()
    {
        if (!trekClient.IsLockedDown())
        {
            if (trekClient.GetShip()->GetParentShip())
                trekClient.DisembarkAndLaunch();
            else
                trekClient.BuyLoadout(trekClient.GetShip(), true);
        }
        else
        {
            ZAssert(false);
        }

        return true;
    }

    void OnBoardShip(IshipIGC* pshipChild, IshipIGC* pshipParent)
    {
        if (pshipChild == trekClient.GetShip())
        {
            if (pshipParent)
            {
                trekClient.PostText(false, "You have boarded %s's ship as %s.",
                        pshipParent->GetName(),
                        ((pshipChild->GetTurretID() == NA) ? "an observer" : "a turret gunner"));
                g_pnumberInTurretNumber->SetValue (1);
            }
            else
            {
                trekClient.PostText(false, "You have disembarked.");
                g_pnumberInTurretNumber->SetValue (0);
            }
        }
    }

    bool OnButtonCommand()
    {
        IstationIGC*    pstation = trekClient.GetShip()->GetStation();
        ZAssert (pstation);

        IclusterIGC*    pcluster = pstation->GetCluster();
        ZAssert (pcluster);

        trekClient.RequestViewCluster(pcluster);
        GetWindow()->SetViewMode(TrekWindow::vmCommand);
        return true;
    }

    bool OnButtonLoadout()
    {
        GetWindow()->SetViewMode(TrekWindow::vmLoadout);

        return true;
    }

    bool OnButtonInvest()
    {
        GetWindow()->TurnOffOverlayFlags(c_omBanishablePanes & ~ofInvestment);
        GetWindow()->ToggleOverlayFlags(ofInvestment);
        return true;
    }

    bool OnButtonHelp()
    {
        GetWindow()->OnHelp(true);

        return true;
    }

    bool OnButtonTeleport()
    {
        GetWindow()->TurnOffOverlayFlags(c_omBanishablePanes & ~ofTeleportPane);
        GetWindow()->ToggleOverlayFlags(ofTeleportPane);

        return true;
    }

    bool OnButtonTeam()
    {
        GetWindow()->TurnOffOverlayFlags(c_omBanishablePanes & ~ofExpandedTeam);
        GetWindow()->ToggleOverlayFlags(ofExpandedTeam);
        return true;
    }

    bool OnButtonExit()
    {
        GetWindow()->StartQuitMission();
        
        return true;
    }

    bool OnHoverLaunch()
    {
        m_pnumberCurrentHover->SetValue(hoverLaunch);
        return true;
    }

    bool OnHoverCommand()
    {
        m_pnumberCurrentHover->SetValue(hoverCommand);
        return true;
    }

    bool OnHoverLoadout()
    {
        m_pnumberCurrentHover->SetValue(hoverLoadout);
        return true;
    }

    bool OnHoverInvest()
    {
        m_pnumberCurrentHover->SetValue(hoverInvest);
        return true;
    }

    bool OnHoverHelp()
    {
        m_pnumberCurrentHover->SetValue(hoverHelp);
        return true;
    }

    bool OnHoverTeleport()
    {
        m_pnumberCurrentHover->SetValue(hoverTeleport);
        return true;
    }

    bool OnHoverTeam()
    {
        m_pnumberCurrentHover->SetValue(hoverTeam);
        return true;
    }

    bool OnHoverExit()
    {
        m_pnumberCurrentHover->SetValue(hoverExit);
        return true;
    }

    bool OnHoverNone()
    {
        m_pnumberCurrentHover->SetValue(hoverNone);
        return true;
    }
};

//////////////////////////////////////////////////////////////////////////////
//
// static variable initializer
//
//////////////////////////////////////////////////////////////////////////////
int             HangarScreen::m_iInTurretNumber = 0;

//////////////////////////////////////////////////////////////////////////////
//
// Constructor
//
//////////////////////////////////////////////////////////////////////////////

TRef<Screen> CreateHangarScreen(Modeler* pmodeler, const ZString& strNamespace)
{
    return new HangarScreen(pmodeler, strNamespace);
}
