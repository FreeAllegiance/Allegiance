#include "pch.h"
#include "trekctrls.h"
#include "loadout.h"
#include "limits.h"
#include "trekmdl.h"

#include <button.h>
#include <controls.h>

// KG- EF5P - sept2008- 
// Enhanced F5 panel: added 'show all' and 'hide completed' options
// all changes related to this have 'EF5P' has comment
// also added a new member to igcstation (GetDirectSuccessorStationType) for this. see igc project.

//////////////////////////////////////////////////////////////////////////////
//
// PartListItem
//
//////////////////////////////////////////////////////////////////////////////

#define shipIDLoadout SHRT_MAX  //review: NA doesn't work here

class PartListItem : public ListItem
{
private:
    long                m_lData;
    TRef<IbucketIGC>    m_pBucket;
    TRef<IbuyableIGC>   m_pBuyable;
    Money               m_moneyCost;
    const char*         m_szModel;
    TRef<Image>         m_pImagePart;
    TRef<Image>         m_pImageBkgnd;
    TRef<Image>         m_pImageSelect;
    TRef<Image>         m_pImageBrackets;
	TList<IbucketIGC*>  m_pDepList; // EF5P
public:
    
    IbuyableIGC* GetBuyable()
    { return m_pBuyable;}
    
    PartListItem(IbucketIGC* pBucket)
    {
        m_lData = (long)pBucket;
        m_pBucket = pBucket;
        m_pBuyable = pBucket;
        m_moneyCost = m_pBuyable->GetPrice();
        Construct();
    }

    void Construct()
    {
        ZAssert(m_pBuyable);
        m_szModel = m_pBuyable->GetModelName();
        
        m_pImageBrackets = GetModeler()->LoadImage("conshadebracketsbmp", true);
        m_pImageBkgnd = GetModeler()->LoadImage(AWF_LOADOUT_ITEM, true);
        m_pImageSelect = GetModeler()->LoadImage("investhighlightbmp", true);
        m_pImagePart = GetModeler()->LoadImage(
            (ZString("i") + ZString(m_szModel) + "bmp").ToLower(), 
            false
            );
        
        BuildPrereqList();
    }
    
	// EF5P begin
    ~PartListItem()
	{
		m_pDepList.SetEmpty();
	}

	void BuildPrereqList()
	{
		m_pDepList.SetEmpty();
		IsideIGC* pSide =  trekClient.GetSide();
		if (!pSide) return;
		if (pSide->CanBuy(m_pBuyable)) return;

		TechTreeBitMask ttbside = pSide->GetTechs(); //GetDevelopmentTechs();
		TechTreeBitMask ttbo =  m_pBuyable->GetRequiredTechs();
		TechTreeBitMask ttdiff = ttbo - ttbside;
		TechTreeBitMask ttremain = ttdiff;
		debugf("%s red because:\n",m_pBuyable->GetName());

		const BucketListIGC* pList = pSide->GetBuckets();
		for (BucketLinkIGC* pBucketNode = pList->first();
			pBucketNode != NULL; pBucketNode = pBucketNode->next())
		{
			IbucketIGC*   pBucket  = pBucketNode->data();
			if (!pBucket->GetCompleteF())
			{
				if (pBucket->GetPredecessor())
					if (!pBucket->GetPredecessor()->GetCompleteF()) continue;
				TechTreeBitMask tteff = pBucket->GetEffectTechs();
				if (!((tteff & ttdiff).GetAllZero()))
				{
					ttremain -= tteff;
					m_pDepList.PushEnd(pBucket);
					continue;
					//debugf("  (%d)%s (pred=%s) type=%d\n",
					//	pBucket->GetBucketType(),
					//	pBucket->GetName(),
					//	pBucket->GetPredecessor() ? pBucket->GetPredecessor()->GetName(): "none",
					//	pBucket->GetBucketType());

				}
			}
		} // for

		// 2nd pass: remove prereq hierachy of stations (should we loop this till stable?)
		for (int i=m_pDepList.GetCount()-1;i>=0;i--)
		{
			TechTreeBitMask ttbi = ttbo & m_pDepList[i]->GetEffectTechs(); // reason for @i to be a prereq 
			if (m_pDepList[i]->GetBucketType() != OT_stationType) continue;
			for (int j=0;j<m_pDepList.GetCount();j++)
			{
				if (m_pDepList[j]->GetBucketType() != OT_stationType) continue;
				if (i==j) continue;

				TechTreeBitMask ttbj = ttbo & m_pDepList[j]->GetEffectTechs(); // reason for @j to be a prereq

				// is ttbi > ttbj ( '>' == '>=' then '!=')
				if (ttbi >= ttbj)
				if (ttbi != ttbj) continue; // @i is here for more reason than @j

				//@i can be removed if it depends on @j
				IstationTypeIGC* pStationtypeI; CastTo(pStationtypeI, m_pDepList[i]->GetBuyable());
				IstationTypeIGC* pStationtypeJ; CastTo(pStationtypeJ, m_pDepList[j]->GetBuyable());
				if (pStationtypeJ->GetDirectSuccessorStationType() != pStationtypeI) continue;

				m_pDepList.Remove(m_pDepList[i]);
				break;
			}
		}
		// 3rd pass: deplist is empty, some bits remain and bucket isnt a dev
		// then we're missing some local station bits or it's obsolete
		IbuyableIGC* b = m_pBucket->GetBuyable(); // get the real buyable object
		if (!b) return; // shouldnt happen but in case
		if (m_pDepList.GetCount()==0 && !ttremain.GetAllZero() && b->GetObjectType() != OT_development)
		{
			debugf("3rd pass for %s %d\n",b->GetName(),b->GetObjectType());
			// 1st check if this is an obsolete station
			if (b->GetObjectType() == OT_stationType)
			{
				IstationTypeIGC* pstation;
				CastTo(pstation,b);
				if (pstation->GetSuccessorStationType(pSide) != pstation) return; // it has a successor so it's obsoloete
			}
			// find prereq stations
			for (BucketLinkIGC* pBucketNode = pList->first();
				pBucketNode != NULL; pBucketNode = pBucketNode->next())
			{
				IbucketIGC*   pBucket  = pBucketNode->data();
				if (pBucket == m_pBucket) continue; // skip self

				//only consider stations
				if (pBucket->GetBucketType() != OT_stationType) continue;
				// get the station local ttbm
				TechTreeBitMask ttstation = ((IstationTypeIGC*)(pBucket->GetBuyable()))->GetLocalTechs();
				if (!((ttstation & ttremain).GetAllZero()))
				{
					m_pDepList.PushEnd(pBucket);
				}
			}
		}
	}
	// -EF5P

	static int CountDronesOfType(IdroneTypeIGC* pDroneType)
    {
        DroneTypeID dtid = pDroneType->GetObjectID();
        SideID      sid = trekClient.GetSideID();
        
        int nCount = 0;
        for (PlayerLink*    ppl = trekClient.GetPlayerList()->first();
        (ppl != NULL);
        ppl = ppl->next())
        {
            //We only know about drones in our mission so only check the side
            const PlayerInfo& pi = ppl->data();
            
            if ((pi.GetDroneTypeID() == dtid) && (pi.SideID() == sid))
            {
                nCount++;
            }
            
        }
        
        return nCount;
    }
    
    static int CountStationsOfType(IstationTypeIGC* pStationtypeTest)
    {
        IsideIGC* pSide =  trekClient.GetSide();
        const StationListIGC* stationList = pSide->GetStations();
        StationClassID classID = pStationtypeTest->GetClassID();
        int nCount = 0;
        
        for (StationLinkIGC* stationLink = (stationList ? stationList->first() : NULL); 
        stationLink; stationLink=stationLink->next())
        {
            IstationTypeIGC* pstationtype = stationLink->data()->GetBaseStationType();
            
            // if this station is in the same class of station
            if (pstationtype->GetClassID() == classID)
                
                nCount++;
        }
        
        return nCount;
    }
    
    static bool BuildSpotFoundForStationType(IstationTypeIGC* pStationtype)
    {
        AsteroidAbilityBitMask aabm = pStationtype->GetBuildAABM();
        
        for (AsteroidLinkIGC* asteriodLink = trekClient.GetCore()->GetAsteroids()->first(); 
        asteriodLink != NULL; asteriodLink = asteriodLink->next())
        {
            if (asteriodLink->data()->HasCapability(aabm))
                return true;
        }
        
        return false;
    }
    
    static IshipIGC* FindBuilderForStationType(IstationTypeIGC* pStationtype)
    {
        if (trekClient.GetSide())
        {
            for (ShipLinkIGC* shipLink = trekClient.GetSide()->GetShips()->first(); 
            shipLink != NULL; shipLink = shipLink->next())
            {
                if (shipLink->data()->GetBaseData() == pStationtype)
                    return shipLink->data();
            }
        }
        
        return NULL;
    }
    
    ZString GetBuildingStatusText()
    {
        IstationTypeIGC* pStationtype;
        CastTo(pStationtype, m_pBucket->GetBuyable());
        IshipIGC* pshipBuilder = FindBuilderForStationType(pStationtype);
        
        if (m_pBucket->GetPercentBought() >= 100)
        {
            return "Preparing builder ...";
        }
        if (pshipBuilder)
        {
            if (pshipBuilder->GetCommandTarget(c_cmdAccepted) &&
                (pshipBuilder->GetCommandID(c_cmdAccepted) == c_cidBuild))
            {
                return "Constructor en route ...";
            }
            else
            {
                return "Constructor waiting ...";
            }
        }
        else if (BuildSpotFoundForStationType(pStationtype))
        {
            return "";
        }
        else
        {
            return ZString("no ") 
                + IasteroidIGC::GetTypeName(pStationtype->GetBuildAABM()) 
                + " asteroid available";
        }
    }
    
    void DrawText(Surface* pSurface, const WinRect& rect)
    {
        Color colorShadow(0, 0, 33.0f / 256.0f);

        char szPrice[20];
		// EF5P - if partialled, displays residual cost instead of full cost
		Money cost = m_moneyCost;
		if (m_pBucket)
		{
			if (m_pBucket->GetPercentBought()>0 && m_pBucket->GetPercentBought() <100)
				cost = cost - m_pBucket->GetMoney();
		}
		// -EF5P
		sprintf(szPrice, "$ %d", cost); // EF5P

        pSurface->DrawStringWithShadow(
            TrekResources::SmallFont(),
            Color::White(),
            colorShadow,
            rect.Min() + WinPoint(147,1), 
            szPrice
        );
        
        ZString strName = m_pBuyable->GetName();
        
        if (m_pBucket)
        {
            ObjectType  otBucket = m_pBucket->GetBucketType();
            if (otBucket != OT_development)
            {
                int nTotalNumber;
                switch (otBucket)
                {
                    case OT_stationType:
                    {
                        pSurface->DrawStringWithShadow(
                            TrekResources::SmallFont(),
                            Color::White(),
                            colorShadow,
                            rect.Min() + WinPoint(11,42), 
                            GetBuildingStatusText()
                        );
                
                        IstationTypeIGC* pStationtype;
                        CastTo(pStationtype, m_pBucket->GetBuyable());
                        nTotalNumber = CountStationsOfType(pStationtype);
                        strName = "Build " + strName + " (" + ZString(nTotalNumber) + ")";
                    }
                    break;

                    case OT_droneType:
                    {
                        IdroneTypeIGC* pDronetype;
                        int nMaxDrones = trekClient.MyMission()->GetMissionDef().misparms.nMaxDronesPerTeam;
                        CastTo(pDronetype, m_pBucket->GetBuyable());
                        nTotalNumber = CountDronesOfType(pDronetype);

                        if (nTotalNumber >= nMaxDrones){
                            pSurface->DrawStringWithShadow(
                                TrekResources::SmallFont(),
                                Color::White(),
                                colorShadow,
                                rect.Min() + WinPoint(11,42), 
                                "At maximum " + strName + "s"
                           );
                        }

                        strName = "Build " + strName + " (" + ZString(nTotalNumber) 
                            + "/" + ZString(nMaxDrones) + ")";
                    }
                    break;

                    case OT_partType:
                    case OT_hullType:
                    {
                        nTotalNumber = m_pBucket->GetSide()->GetStockpile(m_pBucket->GetBuyable());
                        strName = "Build " + strName + " (" + ZString(nTotalNumber) + ")";
                    }
                }
            }
        }
        pSurface->DrawStringWithShadow(
            TrekResources::SmallFont(),
            Color::White(),
            colorShadow,
            rect.Min() + WinPoint(11,1), 
            strName
        );

		// EF5P
		if (m_pDepList.GetCount()==0) 
		{
			if (!trekClient.GetSide()->CanBuy(m_pBucket->GetBuyable()))
			{
				pSurface->DrawStringWithShadow(
						TrekResources::SmallBoldFont(),
						Color::White(),
						colorShadow,
						rect.Min() + WinPoint(11+5,1+9), 
						"obsolete" 
				   );
				return;
			}
			// remaining time
			if (m_pBucket->GetPercentComplete() == 0) return;
			if (m_pBucket->GetPercentComplete() == 100) return;
			int seconds = m_pBucket->GetTimeToBuild() - m_pBucket->GetTime()/1000;
			int minutes = seconds / 60;
            seconds -= minutes * 60;
			pSurface->DrawStringWithShadow(
					TrekResources::SmallFont(),
					Color::White(),
					colorShadow,
					rect.Min() + WinPoint(11+5,1+9+9), 
					ZString(minutes) + "m:" 
                    + ((seconds > 9) ? ZString(seconds) : ("0" + ZString(seconds))) +"s"
			   );
			return;
		}
		int y = 1+9;
		for (int i=0; i < m_pDepList.GetCount() ; i++)
		{
			if (i>4) 
			{
				pSurface->DrawStringWithShadow(
                    TrekResources::SmallFont(),
                    Color::White(),
                    colorShadow,
                    rect.Min() + WinPoint(156,y-9), 
					"......");
				break;
			}

			pSurface->DrawStringWithShadow(
                    TrekResources::SmallFont(),
					Color::White(),
                    colorShadow,
                    rect.Min() + WinPoint(11+5,y), 
					(m_pDepList[i])->GetName()
               );
			y+=9;
		}
		// -EF5P
    }
    
    void FillClippedRect(Context* pcontext, const WinRect& rect, const WinRect& rectClip, const Color& color)
    {
        WinRect rectClipped = rect;

        rectClipped.Intersect(rectClip);

        if (!rectClipped.IsEmpty()) {
            pcontext->FillRect(Rect::Cast(rect), color);
        }
    }

    void DrawShade(Surface* pSurface, const WinRect& rect)
    {
        int nWidth = (int)(rect.XSize() - 23);
        int nPaidShadeWidth = (int)(nWidth * m_pBucket->GetPercentBought()/100);
        int nCompletedShadeWidth = (int)(nWidth * m_pBucket->GetPercentComplete()/100);
        
        if (nPaidShadeWidth > nWidth)
            nPaidShadeWidth  = nWidth;

        if (nCompletedShadeWidth > nWidth)
            nCompletedShadeWidth  = nWidth;

        pSurface->BitBlt(rect.Min(), m_pImageBrackets->GetSurface());

        Point offset = Point::Cast(pSurface->GetOffset() + rect.Min() + WinPoint(8, 2));
        const WinPoint& size = pSurface->GetSize();

        WinRect rectClip = pSurface->GetClipRect();
        rectClip.Offset(pSurface->GetOffset());
        rectClip =
            WinRect(
                rectClip.XMin(),
                size.Y() - rectClip.YMax(),
                rectClip.XMax(),
                size.Y() - rectClip.YMin()
            );

        TRef<Context> pcontext = pSurface->GetContext();

        if (pcontext) {
            pcontext->SetBlendMode(BlendModeSourceAlphaPreMultiplied);

            float bright = 0.5f;
			// EF5P
			float cantbuy = 0.0f; // overlay color for 'show all'
			IsideIGC* pSide =  trekClient.GetSide();
			// if nCompletedShadeWidth isnt 0 then the devel is fully paid (tech pickup)
			// otherwise we check if we can buy it, if not we shade it in red
			if (nCompletedShadeWidth == 0 && !pSide->CanBuy(m_pBuyable))
			{
				cantbuy = 0.5f;
				bright = 0.0f;
				nCompletedShadeWidth = nWidth;
				nPaidShadeWidth = nWidth;
			}
			// end- EF5P
            FillClippedRect(
                pcontext,
                WinRect(
                    offset.X(),
                    size.Y() - (offset.Y() + rect.YSize()-13),
                    offset.X() + nCompletedShadeWidth,
                    size.Y() - offset.Y()
                ),
                rectClip,
                Color(cantbuy, bright, 0.0, bright+cantbuy)// EF5P
            );

            Point offsetPaid = offset + Point(nCompletedShadeWidth, 0);
            FillClippedRect(
                pcontext,
                WinRect(
                    offsetPaid.X(),
                    size.Y() - (offsetPaid.Y() + rect.YSize()-13),
                    offsetPaid.X() + nPaidShadeWidth - nCompletedShadeWidth,
                    size.Y() - offsetPaid.Y()
                ),
                rectClip,
                Color(cantbuy+bright, cantbuy+bright, 0.0, bright+cantbuy)// EF5P
            );

            pSurface->ReleaseContext(pcontext);
        }
    }
    
    short GetItemHeight()
    {
        return 1;
    }
    
    long GetItemData()
    {
        return m_lData;
    };
    
    bool Update()
    {
		BuildPrereqList();
        return true;
    }
    
    void SetSortOrder(long lSortOrder)
    {
    }
    
    bool SetFilter(long lFilter)
    {
        return true;
    }
    
    void DrawItem(Surface* pSurface, const WinRect& rect, bool fSelected, int iFirstSlot)
    {   if (m_pImagePart) {
            pSurface->BitBlt(rect.Min() + WinPoint(10, 2), m_pImagePart->GetSurface());
        } else {
            pSurface->BitBlt(rect.Min(), m_pImageBkgnd->GetSurface());
        }
        
        if (fSelected)
        {
            pSurface->BitBlt(rect.Min() + WinPoint(10, 2), m_pImageSelect->GetSurface());
            
            /*Point offset = Point::Cast(pSurface->GetOffset() + rect.Min() + WinPoint(8, 2));
            const WinPoint& size = pSurface->GetSize();
            TRef<Context> pcontext = pSurface->GetContext();

            pcontext->SetBlendMode(BlendModeSourceAlpha);

            pcontext->SetColor(Color(1.0, 0.0, 0.0, 0.5));
            pcontext->FillRect(
                Rect(
                    offset.X(),
                    size.Y() - (offset.Y() + rect.YSize()-13),
                    offset.X() + rect.XSize() - 23,
                    size.Y() - offset.Y()
                )
            );

            pSurface->ReleaseContext(pcontext);
            */
        }       

        if (m_pBucket)
            DrawShade(pSurface, rect);
        
        DrawText(pSurface, rect);
        
    }
    
};

//////////////////////////////////////////////////////////////////////////////
//
// PurchasesPane
//
//////////////////////////////////////////////////////////////////////////////

class PurchasesPaneImpl :   public PurchasesPane, 
public EventTargetContainer<PurchasesPaneImpl>, 
public TrekClientEventSink
{
private:
    //TRef<TabPane>       m_pTabPaneTeamPurchases;
    TRef<ListPaneOld>   m_pListPane;
    int                 m_nTabSel;
    TRef<PartInfoPane>  m_pPartInfoPane;
    int                 m_nLastHoverIndex;
    TRef<IstationIGC>   m_pStation;
    TRef<ButtonPane>    m_pButtonInvest;
    TRef<ButtonPane>    m_pButtonTab0;
    TRef<ButtonPane>    m_pButtonTab1;
    TRef<ButtonPane>    m_pButtonTab2;
    TRef<ButtonPane>    m_pButtonTab3;
    TRef<ButtonPane>    m_pButtonTab4;
    TRef<ButtonPane>    m_pButtonTab5;
    TRef<ButtonPane>    m_pButtonClose;
	TRef<ButtonPane>	m_pButtonShowAll; // EF5P
	TRef<ButtonPane>	m_pButtonShowComplete; // EF5P
    Window*             m_pPaneWindow;
    TRef<Pane>          m_pBlankPane;
    TRef<Pane>          m_pBlankPane0;
    TRef<Pane>          m_pBlankPane1;
    TRef<Pane>          m_pBlankPane2;
    TRef<Pane>          m_pBlankPane3;
    TRef<Pane>          m_pBlankPane4;
    TRef<Pane>          m_pBlankPane5;
    
    
public:
    PurchasesPaneImpl(PartInfoPane* pPartInfoPane) :
          m_nTabSel(-1),
          m_nLastHoverIndex(-1),
          m_pPartInfoPane(pPartInfoPane),
          m_pStation(NULL),
          m_pPaneWindow(NULL)
      {
          TRef<ImagePane> pImagePane = new ImagePane(GetModeler()->LoadImage(
              AWF_LOADOUT_FLIGHT_PURCHASE_PANEL, false));
          InsertAtBottom(pImagePane);
          
          // main loadout column pane
          TRef<ColumnPane> pColumnPane = new ColumnPane();
          TRef<RowPane> pRowPane = new RowPane();
          

          const int nTabWidth = 180;

          m_pBlankPane5 = new Pane();
          InternalSetSize(m_pBlankPane5, WinPoint(nTabWidth, 3));
          pColumnPane->InsertAtBottom(m_pBlankPane5);

          m_pButtonTab0 = 
            CreateTrekButton(
              CreateButtonFacePane(
                  GetModeler()->LoadSurface("btndronesbmp", true),
                  ButtonNormal
                  ),
              false, mouseclickSound
              );
          pColumnPane->InsertAtBottom(m_pButtonTab0);
          //m_pBlankPane0 = new Pane();
          //InternalSetSize(m_pBlankPane0, WinPoint(nTabWidth, 1));
          //pColumnPane->InsertAtBottom(m_pBlankPane0);

          m_pButtonTab1 = 
            CreateTrekButton(
              CreateButtonFacePane(
                  GetModeler()->LoadSurface("btnstarbasebmp", true),
                  ButtonNormal
                  ),
              false, mouseclickSound
              );
          pColumnPane->InsertAtBottom(m_pButtonTab1);
          m_pBlankPane1 = new Pane();
          InternalSetSize(m_pBlankPane1, WinPoint(nTabWidth, 1));
          pColumnPane->InsertAtBottom(m_pBlankPane1);

          m_pButtonTab2 = 
            CreateTrekButton(
              CreateButtonFacePane(
                  GetModeler()->LoadSurface("btnordnancebmp", true),
                  ButtonNormal
                  ),
              false, mouseclickSound
              );
          pColumnPane->InsertAtBottom(m_pButtonTab2);
          m_pBlankPane2 = new Pane();
          InternalSetSize(m_pBlankPane2, WinPoint(nTabWidth, 1));
          pColumnPane->InsertAtBottom(m_pBlankPane2);

          m_pButtonTab3 = 
            CreateTrekButton(
              CreateButtonFacePane(
                  GetModeler()->LoadSurface("btnelectronicsbmp", true),
                  ButtonNormal
                  ),
              false, mouseclickSound
              );
          pColumnPane->InsertAtBottom(m_pButtonTab3);
          m_pBlankPane3 = new Pane();
          InternalSetSize(m_pBlankPane3, WinPoint(nTabWidth, 1));
          pColumnPane->InsertAtBottom(m_pBlankPane3);

          m_pButtonTab4 = 
            CreateTrekButton(
              CreateButtonFacePane(
                  GetModeler()->LoadSurface("btnresearchbmp", true),
                  ButtonNormal
                  ),
              false, mouseclickSound
              );
          pColumnPane->InsertAtBottom(m_pButtonTab4);
          m_pBlankPane4 = new Pane();
          InternalSetSize(m_pBlankPane4, WinPoint(nTabWidth, 1));
          pColumnPane->InsertAtBottom(m_pBlankPane4);

          m_pButtonTab5 = 
            CreateTrekButton(
              CreateButtonFacePane(
                  GetModeler()->LoadSurface("btnshipyardbmp", true),
                  ButtonNormal
                  ),
              false, mouseclickSound
              );
          pColumnPane->InsertAtBottom(m_pButtonTab5);
              

          pRowPane->InsertAtBottom(pColumnPane);
          
          m_pBlankPane = new Pane();
          InternalSetSize(m_pBlankPane, WinPoint(30, 10));
          pRowPane->InsertAtBottom(m_pBlankPane);

          // close button
          m_pButtonClose = 
              CreateTrekButton(
                  CreateButtonFacePane(
                      GetModeler()->LoadSurface("btnclosepanebmp", true),
                      ButtonNormal
                  ),
                  false
              );

          // parts list        
          m_pListPane = ListPaneOld::Create(WinPoint(203, 432), 72, true, m_pButtonClose),
              pRowPane->InsertAtBottom(m_pListPane);
          
          // invest button
          m_pButtonInvest = 
              CreateTrekButton(
                  CreateButtonFacePane(
                      GetModeler()->LoadSurface(AWF_LOADOUT_INVEST_BUTTON, true),
                      ButtonNormal
                  ),
                  false
              );
          
          pImagePane->InsertAtBottom(m_pButtonInvest);
          m_pButtonInvest->SetOffset(WinPoint(63,315));
            
#pragma region EF5P
          // EF5P new buttons 
          m_pButtonShowAll = 
              CreateTrekButton(
                  CreateButtonFacePane(
                      GetModeler()->LoadSurface("btnshowallbmp", true),
                      ButtonNormalCheckBox
                  ),
                  true
              );
		  m_pButtonShowAll->SetChecked(false);
		  pImagePane->InsertAtBottom(m_pButtonShowAll);
		  m_pButtonShowAll->SetOffset(WinPoint(30,360)); //25,370

		  m_pButtonShowComplete = 
              CreateTrekButton(
                  CreateButtonFacePane(
                      GetModeler()->LoadSurface("btnshowcpltbmp", true),
                      ButtonNormalCheckBox
                  ),
                  true
              );
		  m_pButtonShowComplete->SetChecked(true);
		  pImagePane->InsertAtBottom(m_pButtonShowComplete);
		  m_pButtonShowComplete->SetOffset(WinPoint(30,375)); //110,370
#pragma endregion

          pImagePane->InsertAtBottom(pRowPane);
          pRowPane->SetOffset(WinPoint(17,9));
          
          //initialize tab selection to Starbase Tab
          m_pButtonTab0->SetChecked(true);
          OnTabSelect0();
          

		  // mdvalley: needs the class name and a pointer.
          AddEventTarget(&PurchasesPaneImpl::OnTabSelect0, m_pButtonTab0->GetEventSource());
          AddEventTarget(&PurchasesPaneImpl::OnTabSelect1, m_pButtonTab1->GetEventSource());
          AddEventTarget(&PurchasesPaneImpl::OnTabSelect2, m_pButtonTab2->GetEventSource());
          AddEventTarget(&PurchasesPaneImpl::OnTabSelect3, m_pButtonTab3->GetEventSource());
          AddEventTarget(&PurchasesPaneImpl::OnTabSelect4, m_pButtonTab4->GetEventSource());
          AddEventTarget(&PurchasesPaneImpl::OnTabSelect5, m_pButtonTab5->GetEventSource());


          //AddEventTarget(OnTabSelect, m_pTabPaneTeamPurchases->GetEventSource());


          AddEventTarget(&PurchasesPaneImpl::OnListSelect, m_pListPane->GetEventSource());
          AddEventTarget(&PurchasesPaneImpl::OnInvest, m_pListPane->GetDoubleClickEventSource());
          AddEventTarget(&PurchasesPaneImpl::OnPartial, m_pListPane->GetRightClickEventSource());	// TE: hooked up RightClickEvent to OnPartial method
          AddEventTarget(&PurchasesPaneImpl::OnListHover, m_pListPane->GetMouseOverEvent());
          AddEventTarget(&PurchasesPaneImpl::OnInvest, m_pButtonInvest->GetEventSource());
          AddEventTarget(&PurchasesPaneImpl::OnButtonBack, m_pButtonClose->GetEventSource());
          AddEventTarget(&PurchasesPaneImpl::OnMouseEnterInvestButton, m_pButtonInvest->GetMouseEnterEventSource());
          AddEventTarget(&PurchasesPaneImpl::OnMouseLeaveInvestButton, m_pButtonInvest->GetMouseLeaveEventSource());
          
		  AddEventTarget(&PurchasesPaneImpl::OnShowAll, m_pButtonShowAll->GetEventSource()); // EF5P
		  AddEventTarget(&PurchasesPaneImpl::OnHideCompleted, m_pButtonShowComplete->GetEventSource()); // EF5P
          //m_pTabPaneTeamPurchases->ShowSelPane();
      }
      
      ~PurchasesPaneImpl()
      {
          GetWindow()->SetInvestAsteroid(0);
      }
      
      bool OnInvest()
      {
          if (trekClient.MyMissionInProgress())
          {
              TRef<ListItem> pItem = m_pListPane->GetSelItem();
              if (pItem)
              {
                  IbucketIGC* pBucket =  (IbucketIGC*)m_pListPane->GetSelItem()->GetItemData();

                  if (CanInvest(pBucket))
                  {
                      Money bucketCost = pBucket->GetPrice() - pBucket->GetMoney();

                      Money investmentAmount = std::min(bucketCost, trekClient.GetMoney());
                      trekClient.AddMoneyToBucket(pBucket, investmentAmount);
                  }
              }                    
          }
          return true;
      }
#pragma region EF5P
	  // EF5P
      bool OnShowAll()
	  {
			m_pListPane->UpdateAll();
			UpdateTechList(); //Imago limit the amount of tab switching when you hit the new buttons 7/17/09
			switch (m_nTabSel) {
			case 0: 
				OnTabSelect0();
				break;
			case 1:
				OnTabSelect1();
				break;
			case 2: {
				if (m_pButtonTab2->GetEnabled())
					OnTabSelect2();
				else 
					OnTabSelect0();
				break;
					}
			case 3: {
				if (m_pButtonTab3->GetEnabled())
					OnTabSelect3();
				else 
					OnTabSelect0();
				break;
					}
			case 4: {
				if (m_pButtonTab4->GetEnabled())
					OnTabSelect4();
				else 
					OnTabSelect0();
				break;
					}
			case 5: {
				if (m_pButtonTab5->GetEnabled())
					OnTabSelect5();
				else 
					OnTabSelect0();
				break;
					}
				default:
					OnTabSelect0();
					break;
			}
			UpdateSpendButtons();
			return true;
	  }
	  // EF5P
	  bool OnHideCompleted()
	  {
		  return OnShowAll();
	  }
#pragma endregion
      
	  // TE: Added OnPartial method to handle partial-investing
      bool OnPartial()
      {
          if (trekClient.MyMissionInProgress())
          {
              TRef<ListItem> pItem = m_pListPane->GetSelItem();
              if (pItem)
              {
                  IbucketIGC* pBucket =  (IbucketIGC*)m_pListPane->GetSelItem()->GetItemData();

                  if (CanInvest(pBucket))
                  {
                      Money bucketCost = pBucket->GetPrice() - pBucket->GetMoney();	// Get required money to finish development
					  
					  // Prevent finishing the development with the right-click
					  if (bucketCost == 1)
						  return true;

                      Money TenPercent = pBucket->GetPrice() * 0.1;					// Get 10% of the total research cost

					  Money investmentAmount = std::min(TenPercent, trekClient.GetMoney());	// Get what's less: Your money? or the reqdevcost

					  // If we're about to finish this development, remove 1cr so we don't!!
					  while (pBucket->GetMoney() + investmentAmount >= pBucket->GetPrice())
						  investmentAmount -= 1;

					  // Perform the investment
					  trekClient.AddMoneyToBucket(pBucket, investmentAmount);
                  }
              }                    
          }
          return true;
     }

     bool OnButtonBack()
     {
         
        GetWindow()->TurnOffOverlayFlags(ofInvestment);
        return true;
     }
      
      void OnBucketChange(BucketChange bc, IbucketIGC* b)
      {
		  bool updatedeps = false; // EF5P
          if (bc == c_bcTerminated)
		  {
			  updatedeps = true; // EF5P
              m_pListPane->RemoveItemByData((long)b);
		  }
          else
		  {
			  // EF5P - hide if completed and m_pButtonShowComplete isnt checked
			  if (b->GetCompleteF() && !m_pButtonShowComplete->GetChecked())
				  m_pListPane->RemoveItemByData((long)b);
			  else
				m_pListPane->UpdateItemByData((long)b);
		  }
		  // EF5P - update all deps when a bucket is completed or terminated and 'show all'
		  updatedeps = updatedeps || b->GetCompleteF();
		  if (updatedeps && m_pButtonShowAll->GetChecked())
			m_pListPane->UpdateAll();
          UpdateSpendButtons();
      }
      
      void OnModelTerminated(ImodelIGC* pmodel) 
      {
          // make sure we correct the number of existing stations on the investment list
          if ((pmodel->GetObjectType() == OT_station) && (pmodel->GetSide() != NULL)
              && (pmodel->GetSide()->GetObjectID() == trekClient.GetSideID()))
          {   
              m_pListPane->UpdateAll();
              
              UpdateSpendButtons();
          }
      }
      
      void OnStationCaptured(StationID stationID, SideID sideID) 
      {
          // this may change our station count
          m_pListPane->UpdateAll();
          
          UpdateSpendButtons();
      }
      
      void OnDiscoveredStation(IstationIGC* pstation)
      {
          // this may change our station count
          m_pListPane->UpdateAll();
          
          UpdateSpendButtons();
      }
      
      void OnDiscoveredAsteroid(IasteroidIGC* pasteroid)
      {
          // this may change our station status text
          m_pListPane->UpdateAll();
          
          UpdateSpendButtons();
      }
      
      
      void UpdateTechList()
      {
		  bool bShowAll = m_pButtonShowAll->GetChecked(); // EF5P
		  bool bHideCompleted = !m_pButtonShowComplete->GetChecked();

          // Update the selected tech list
          // HACK: This is currently O(n^2), but it seems to work well enough.  
          int nInsertionIndex = 0;
          BuyableGroupID group = (BuyableGroupID)m_nTabSel;
          
          IsideIGC* pSide = trekClient.GetSide();
          if (pSide)
          {
              const BucketListIGC* pList = pSide->GetBuckets();
              for (BucketLinkIGC* pBucketNode = pList->first();
              pBucketNode != NULL; pBucketNode = pBucketNode->next())
              {
                  IbucketIGC*   pBucket  = pBucketNode->data();

                  if (pBucket->GetGroupID() == group)
                  {
					  bool bDisplay = pSide->CanBuy(pBucket) || bShowAll;

					  if (bHideCompleted && pBucket->GetCompleteF()) bDisplay = false;
                      if (bDisplay)
                      {
                          IbucketIGC*   pbucketPredecessor = pBucket->GetPredecessor();
                          if ((pbucketPredecessor == NULL) || pbucketPredecessor->GetCompleteF() || bShowAll) // EF5P

                          {
                              int nExistingIndex = m_pListPane->GetItemIdxByData((long)pBucket);
                          
                              if (nExistingIndex == NA)
                                  m_pListPane->InsertItem(nInsertionIndex++, new PartListItem(pBucket));
                              else
							  {
								  m_pListPane->GetItemByIdx(nExistingIndex)->Update(); // EF5P - update the prereqs
                                  nInsertionIndex = nExistingIndex + 1;
							  }
                          }
                      }
                      else
                      {
                          m_pListPane->RemoveItemByData((long)pBucket);
                      }
                  }
              }
          }

          m_pButtonTab0->SetEnabled(BuyableTechExistsInGroup(0));
          m_pButtonTab1->SetEnabled(BuyableTechExistsInGroup(1));
          m_pButtonTab2->SetEnabled(BuyableTechExistsInGroup(2));
          m_pButtonTab3->SetEnabled(BuyableTechExistsInGroup(3));
          m_pButtonTab4->SetEnabled(BuyableTechExistsInGroup(4));
          m_pButtonTab5->SetEnabled(BuyableTechExistsInGroup(5));

          UpdateSpendButtons();
      }

      bool BuyableTechExistsInGroup(BuyableGroupID group)
      {
		  // EF5P
		  // dont 'show all' the path(s) not allowed in game settings.
		  bool bShowAll = m_pButtonShowAll->GetChecked(); 
		  if (trekClient.MyMission())
		  {
			  if (!trekClient.MyMission()->GetMissionParams().bAllowShipyardPath)
				  if (group == 5) return false;
			  if (!trekClient.MyMission()->GetMissionParams().bAllowSupremacyPath)
				  if (group == 2) return false;
			  if (!trekClient.MyMission()->GetMissionParams().bAllowTacticalPath)
				  if (group == 3) return false;
			  if (!trekClient.MyMission()->GetMissionParams().bAllowExpansionPath)
				  if (group == 4) return false;

		  }
		  // end- EF5P

          IsideIGC* pSide = trekClient.GetSide();
          if (pSide)
          {
              const BucketListIGC* pList = pSide->GetBuckets();
              for (BucketLinkIGC* pBucketNode = pList->first();
              pBucketNode != NULL; pBucketNode = pBucketNode->next())
              {
                  IbucketIGC*  pBucket  = pBucketNode->data();
                  if (pBucket->GetGroupID() == group)
                  {
                      if (pSide->CanBuy(pBucket) || bShowAll) // EF5P
                      {
                          IbucketIGC*   pbucketPredecessor = pBucket->GetPredecessor();
                          if ((pbucketPredecessor == NULL) || pbucketPredecessor->GetCompleteF() || bShowAll) // EF5P
                          {
                              return true;
                          }
                      }
                  }
              }
          }

          return false;
      }
      
      void OnTechTreeChanged(SideID sid)
      {
          if (trekClient.GetSideID() == sid)
          {
			  debugf("TT Changed\n");
              UpdateTechList();
          }
      }
      
      bool CanInvest(IbucketIGC* pBucket)
      {
          // if the game has not started yet, don't let them invest
          if (!trekClient.MyMissionInProgress())
              return false;

          if (!trekClient.GetSide())
              return false;

		  // EF5P:
		  IsideIGC* pSide = trekClient.GetSide();
		  if (!pSide->CanBuy(pBucket)) return false;

		  //if (pside->CanBuy())
          // if this is a drone, make sure that we have less than 
          // 4 of that type of drone.
          if (pBucket->GetBucketType() == OT_droneType)
          {
              IdroneTypeIGC* pDronetype; 
              CastTo(pDronetype, pBucket->GetBuyable());
              if (PartListItem::CountDronesOfType(pDronetype) 
                  >= trekClient.MyMission()->GetMissionDef().misparms.nMaxDronesPerTeam)
              {
                  return false;
              }
          }
          
          // if this is a station, make sure we have the 
          // appropriate rock and don't have a builder.
          if (pBucket->GetBucketType() == OT_stationType)
          {
              IstationTypeIGC* pStationtype; 
              CastTo(pStationtype, pBucket->GetBuyable());
              if (!PartListItem::BuildSpotFoundForStationType(pStationtype)
                  || PartListItem::FindBuilderForStationType(pStationtype) != NULL)
                  return false;
          }

          // they need to have money to invest
          if (trekClient.GetMoney() <= 0)
              return false;

          // there needs to be something in which to invest
          if (pBucket->GetPercentBought() >= 100)
              return false;

          return true;
      }

      void UpdateSpendButtons()
      {
          TRef<ListItem> pItem = m_pListPane->GetSelItem();
          
          if (pItem) 
          {
              IbucketIGC* pBucket = (IbucketIGC*)(pItem->GetItemData());
              
              // if the mission has started, the player has money to invest, and the investment
              // is not at 100%...
              m_pButtonInvest->SetEnabled(CanInvest(pBucket));
          } 
          else 
          {
              // no item to buy or invest in
              m_pButtonInvest->SetEnabled(false);
          }
      }
      
      void OnMoneyChange(PlayerInfo* pPlayerInfo)
      {
          UpdateSpendButtons();
      }
      
      bool OnListHover(int nSel)
      {
          if (nSel == -1)
          {
              m_pPartInfoPane->DisplayNothing();
          }
          else
          {
              IbucketIGC* pBucket =  (IbucketIGC*)m_pListPane->GetItemByIdx(nSel)->GetItemData();
              if (m_nLastHoverIndex != nSel)
                  trekClient.PlaySoundEffect(mouseoverSound);
              m_pPartInfoPane->DisplayTeamPurchase(pBucket, false);
          }
          
          m_nLastHoverIndex = nSel;
          
          return true;
      }
      
      bool OnMouseEnterInvestButton()
      {
          OnListHover(m_pListPane->GetSelItemIdx());
          return true;
      }
      
      bool OnMouseLeaveInvestButton()
      {
          OnListHover(-1);
          return true;
      }
      
      bool OnListSelect(int nSel)
      {
          UpdateSpendButtons();
          return true;
      }
      
      void OnAddPlayer(MissionInfo* pMissionInfo, SideID sideID, PlayerInfo* pPlayerInfo)
      {
          // update the drone counts
          m_pListPane->UpdateAll();
          
          UpdateSpendButtons();
      }
      
      void OnDelPlayer(MissionInfo* pMissionInfo, SideID sideID, PlayerInfo* pPlayerInfo, QuitSideReason reason, const char* szMessageParam)
      {
          // update the drone counts
          m_pListPane->UpdateAll();
          
          UpdateSpendButtons();
      }
      
      bool OnTabSelect0()
      {
          m_pListPane->RemoveAll();
          m_nTabSel = 0;
          m_pButtonTab0->SetChecked(true);
          m_pButtonTab1->SetChecked(false);
          m_pButtonTab2->SetChecked(false);
          m_pButtonTab3->SetChecked(false);
          m_pButtonTab4->SetChecked(false);
          m_pButtonTab5->SetChecked(false);
          UpdateTechList();
          m_pListPane->SetSelItemByIdx(0);
          return true;     
      }
      
      bool OnTabSelect1()
      {
          m_pListPane->RemoveAll();
          m_nTabSel = 1;
          m_pButtonTab0->SetChecked(false);
          m_pButtonTab1->SetChecked(true); 
          m_pButtonTab2->SetChecked(false);
          m_pButtonTab3->SetChecked(false);
          m_pButtonTab4->SetChecked(false);
          m_pButtonTab5->SetChecked(false);
          UpdateTechList();
          m_pListPane->SetSelItemByIdx(0);
          return true;     
      }

      bool OnTabSelect2()
      {
          m_pListPane->RemoveAll();
          m_nTabSel = 2;
          m_pButtonTab0->SetChecked(false);
          m_pButtonTab1->SetChecked(false);
          m_pButtonTab2->SetChecked(true);
          m_pButtonTab3->SetChecked(false);
          m_pButtonTab4->SetChecked(false);
          m_pButtonTab5->SetChecked(false);
          UpdateTechList();
          m_pListPane->SetSelItemByIdx(0);
          return true;     
      }

      bool OnTabSelect3()
      {
          m_pListPane->RemoveAll();
          m_nTabSel = 3;
          m_pButtonTab0->SetChecked(false);
          m_pButtonTab1->SetChecked(false);
          m_pButtonTab2->SetChecked(false);
          m_pButtonTab3->SetChecked(true);
          m_pButtonTab4->SetChecked(false);
          m_pButtonTab5->SetChecked(false);
          UpdateTechList();
          m_pListPane->SetSelItemByIdx(0);
          return true;     
      }

      bool OnTabSelect4()
      {
          m_pListPane->RemoveAll();
          m_nTabSel = 4;
          m_pButtonTab0->SetChecked(false);
          m_pButtonTab1->SetChecked(false);
          m_pButtonTab2->SetChecked(false);
          m_pButtonTab3->SetChecked(false);
          m_pButtonTab4->SetChecked(true);
          m_pButtonTab5->SetChecked(false);
          UpdateTechList();
          m_pListPane->SetSelItemByIdx(0);
          return true;     
      }

      bool OnTabSelect5()
      {
          m_pListPane->RemoveAll();
          m_nTabSel = 5;
          m_pButtonTab0->SetChecked(false);
          m_pButtonTab1->SetChecked(false);
          m_pButtonTab2->SetChecked(false);
          m_pButtonTab3->SetChecked(false);
          m_pButtonTab4->SetChecked(false);
          m_pButtonTab5->SetChecked(true);
          UpdateTechList();
          m_pListPane->SetSelItemByIdx(0);
          return true;     
      }
      
      void UpdateLayout()
      {
          DefaultUpdateLayout();
      }
      
      void SetPaneWindow(Window* pWindow)
      {
          m_pPaneWindow = pWindow;
      }
};

TRef<PurchasesPane> PurchasesPane::Create(PartInfoPane* pPartInfoPane)
{
    return new PurchasesPaneImpl(pPartInfoPane);
}

TRef<IObject> PurchasesPaneFactory::Apply(ObjectStack& stack)
{
    TRef<PartInfoPane> pinfoPane; CastTo(pinfoPane, (Pane*)(IObject*)stack.Pop());
    return (Pane*)PurchasesPane::Create(pinfoPane);
}

//////////////////////////////////////////////////////////////////////////////
//
// PartInfoPane
//
//////////////////////////////////////////////////////////////////////////////

static WinPoint s_rgPtLabels[] =
{
    WinPoint(9,25), WinPoint(9,42), WinPoint(9,59),
        WinPoint(9,76), WinPoint(9,92),
        WinPoint(120,25), WinPoint(120,42), WinPoint(120,59),
        WinPoint(120,76), WinPoint(120,92),
};

static WinPoint s_rgPtStats[] =
{
    WinPoint(67,25), WinPoint(67,42), WinPoint(67,59),
        WinPoint(67,76), WinPoint(67,92),
        WinPoint(178,25), WinPoint(178,42), WinPoint(178,59),
        WinPoint(178,76), WinPoint(178,92),
};



class PartInfoPaneImpl : public PartInfoPane
{
private:
    TRef<IbuyableIGC>   m_pBuyable;
    TRef<IhullTypeIGC>  m_pHullType;
    TRef<IpartTypeIGC>  m_pPartType;
    Money               m_moneyCost;
    const char*         m_szModel;
    TRef<Image>         m_pImagePart;
    TRef<Image>         m_pImageBkgnd;
    TRef<ConnectorPane> m_pConnectorPaneInventory;
    TRef<ConnectorPane> m_pConnectorPanePurchases;
    
public:
    
    PartInfoPaneImpl(ConnectorPane* pConnectorPanePurchases, ConnectorPane* pConnectorPaneInventory) :
      m_pConnectorPaneInventory(pConnectorPaneInventory), 
          m_pConnectorPanePurchases(pConnectorPanePurchases)
      {
          m_pImageBkgnd = GetModeler()->LoadImage(AWF_LOADOUT_INFO_PANEL, false);
          DisplayNothing();
      }
      
      void DisplayTeamPurchase(IbucketIGC* pBucket, bool bPartInInventory)
      {
          if (m_pConnectorPaneInventory)
              m_pConnectorPaneInventory->SetVisible(bPartInInventory);
          
          if (m_pConnectorPanePurchases)
              m_pConnectorPanePurchases->SetVisible(!bPartInInventory);
          
          m_pPartType = NULL;
          m_pHullType = NULL;
          m_pBuyable = pBucket;
          
          InternalSetSize(m_pImageBkgnd->GetSurface()->GetSize());
          
          NeedPaint();

          GetWindow()->SetInvestAsteroid(0);
          IbuyableIGC*    pbuyable = pBucket->GetBuyable();
          if (pbuyable->GetObjectType() == OT_stationType)
          {
              IstationTypeIGC*    pst = (IstationTypeIGC*)pbuyable;

              AsteroidAbilityBitMask  aabm = pst->GetBuildAABM();
              GetWindow()->SetInvestAsteroid(aabm);
          } 
      }
      
      void DisplayNothing()
      {
          GetWindow()->SetInvestAsteroid(0);

          if (m_pConnectorPaneInventory)
              m_pConnectorPaneInventory->SetVisible(false);
          
          if (m_pConnectorPanePurchases)
              m_pConnectorPanePurchases->SetVisible(false);
          
          m_pImagePart = NULL;
          m_pPartType = NULL;
          m_pHullType = NULL;
          m_pBuyable = NULL;
          m_szModel = NULL;
          NeedPaint();
      }
      
private:
    
    void UpdateLayout()
    {
        InternalSetSize(m_pImageBkgnd->GetSurface()->GetSize());
        Pane::UpdateLayout();
    }
    
    void DrawName(Surface* pSurface, IbuyableIGC* pBuyable) const
    {
        pSurface->DrawString(
            TrekResources::SmallBoldFont(),
            Color(64.0f / 255.0f, 70.0f / 255.0f, 1.0f),
            WinPoint(15,10), 
            pBuyable->GetName()
        );
    }
    
    void DrawDescription(Surface* pSurface, IbuyableIGC* pBuyable) const
    {
        const int nBorderWidth = 13;
        const int nLineHeight = 12;
        const int nFirstLineY = 4*nLineHeight;
        
        const int nWidth = (int)m_pImageBkgnd->GetBounds().GetRect().XSize() - nBorderWidth*2;
        int nCurrentYPos = nFirstLineY + nBorderWidth;
        
        TRef<IEngineFont> pfont = TrekResources::SmallFont();
        Color             color = MakeColorFromCOLORREF(RGB(180,180,180));

        pSurface->DrawString(pfont, color, WinPoint(nBorderWidth, 15 + nLineHeight),   ZString("Cost:           $") + ZString(pBuyable->GetPrice()));
        pSurface->DrawString(pfont, color, WinPoint(nBorderWidth, 15 + 2*nLineHeight), ZString("Time:          ") + ZString((long)pBuyable->GetTimeToBuild()) + ZString(" Sec."));

        ZString strDescription(pBuyable->GetDescription() + ZString(" "));
        
        
        while (strDescription.GetLength() != 0)
        {
            // calculate the number of characters we can display on this line
            int nLineLength = TrekResources::SmallFont()->GetMaxTextLength(strDescription, nWidth, true);
            
            // try doing word breaking
            int nWordBreakLength = nLineLength;
            while (nWordBreakLength > 0 && strDescription[nWordBreakLength] != ' ' 
                && strDescription[nWordBreakLength-1] != ' ')
                nWordBreakLength--;
            if (nWordBreakLength != 0)
                nLineLength = nWordBreakLength;
            
            if (nLineLength <= 0)
            {
                // skip the unknown character
                nLineLength = 1;
            }
            else
            {
                pSurface->DrawString(pfont, color, WinPoint(nBorderWidth,nCurrentYPos), 
                    strDescription.Left(nLineLength));
                nCurrentYPos += nLineHeight;
            }
            
            strDescription = strDescription.RightOf(nLineLength);
        }
    }
    
    void Paint(Surface* pSurface)
    {
        pSurface->BitBlt(WinPoint(0, 0), 
            m_pImageBkgnd->GetSurface(),
            WinRect(0, 0, XSize(), YSize()));
        
        if (m_pBuyable)
        {
            DrawName(pSurface, m_pBuyable);
            DrawDescription(pSurface, m_pBuyable);
        }
    }
    
    
};

TRef<PartInfoPane> PartInfoPane::Create(ConnectorPane* pConnectorPanePurchases, ConnectorPane* pConnectorPaneInventory)
{
    return new PartInfoPaneImpl(pConnectorPanePurchases, pConnectorPaneInventory);
}

TRef<IObject> PartInfoPaneFactory::Apply(ObjectStack& stack)
{
    return (Pane*)PartInfoPane::Create(NULL, NULL);
}
