#include "pch.h"
#include "mappreview.h"
#include "mappvmaker.h"

class MapPreviewImageImpl : public MapPreviewImage
{
private:

	TRef<Image>             m_pimageSectorBkgnd;
	TRef<Image>             m_pimageSectorEmpty;
	TRef<Image>             m_pimageOwnerHighlight;
	TRef<Image>             m_pimageSecondaryOwnerHighlight;
	TRef<Image>             m_pimageSectorHighlight;
	WinRect                 m_rectMap;
	float                   m_xMin;
	float                   m_xMax;
	float                   m_yMin;
	float                   m_yMax;
	float                   m_xClusterMin;
	float                   m_xClusterMax;
	float                   m_yClusterMin;
	float                   m_yClusterMax;
	float                   m_xDrag;
	float                   m_yDrag;
	bool                    m_bCanDrag;
    bool                    m_bDragging;
    Point                   m_pointLastDrag;

	bool					m_bShowDetails;
	bool					m_bShowSide;
	bool					m_bValidPreview;
	const char *            m_pszReason;
	ImissionPV				m_missionpv;
	MissionParams			m_mp;

	enum { c_nXBorder = 4, c_nYBorder = 4 };
public:
	// ImissionPV implementation


	// end of ImissionPV
	
	MapPreviewImageImpl(Modeler *pmodeler,Image *background) :
		m_xDrag(0),
		m_yDrag(0),
		m_bValidPreview(false),
		m_bCanDrag(false),
		m_bDragging(false),
		m_bShowDetails(false),
		m_bShowSide(false),
		m_missionpv(pmodeler)
    {
		m_pimageSectorBkgnd = background; //pmodeler->LoadImage("sectormapbkgndbmp", false);
        m_pimageSectorEmpty = pmodeler->LoadImage("sectoremptybmp", true);
		m_pimageOwnerHighlight = pmodeler->LoadImage("sectorownerbmp", true);
		m_pimageSecondaryOwnerHighlight = pmodeler->LoadImage("sectorinvaderbmp", true);
		m_pimageSectorHighlight = pmodeler->LoadImage("sectorhighlightbmp", true);

		m_rectMap = WinRect::Cast(m_pimageSectorBkgnd->GetBounds().GetRect());

		m_pszReason = "1";
		if(trekClient.MyMission())
		{
			ComputePreview(trekClient.MyMission()->GetMissionParams());
		}
    }

	void ShowDetails(bool bDetails)
	{
		if(m_bShowDetails != bDetails)
		{
			m_bShowDetails = bDetails;
			Changed();
		}

	}
	void ShowSide(bool bSide)
	{
		if(m_bShowSide != bSide)
		{
			m_bShowSide = bSide;
			Changed();
		}
	}
    void CalcBounds()
    {
        m_bounds = m_pimageSectorBkgnd->GetBounds();
    }

    void ComputePreview(const MissionParams& p)
    {
		// KGJV fix: reset dragging
		m_xDrag = 0;
		m_yDrag = 0;
		m_bCanDrag = false;
		m_bDragging = false;

		m_mp = p;
		m_missionpv.Clear();
		m_bValidPreview = true;
		m_pszReason = ImapPVMaker::IsValid(&m_mp);
		if (m_pszReason != NULL)
		{
			m_pszReason = "1";
		}

		if (m_pszReason == NULL)
			m_pszReason = ImapPVMaker::Create(&m_mp, &m_missionpv);
		if (m_pszReason != NULL)
			m_bValidPreview = false;
		
		// force update to call render
		Changed();

		// debug stuff
		//ClusterPVList* clusters = m_missionpv.GetClusters();
		//debugf("ImapPVMaker::Create done: %d sectors\n",clusters->n());
		//for (ClusterPVLink* cLink = clusters->first(); cLink != NULL; cLink = cLink->next())
		//{
		//	CMapPVCluster* pCluster = cLink->data();
		//	debugf("  cluster %d at %f %f - sides: ",(int)pCluster->GetObjectID(),(double)pCluster->GetScreenX(),(double)pCluster->GetScreenY());
		//	for (int i =0; i < c_cSidesMax; i++) debugf("%d",pCluster->GetStationsPerSide()[i]);
		//	debugf("\n");
		//}
		//WarpPVList* warps = m_missionpv.GetWarps();
		//debugf("ImapPVMaker:: %d warps\n",warps->n());
		//for (WarpPVLink* cLink = warps->first(); cLink != NULL; cLink = cLink->next())
		//{
		//	CwarpPV* pWarp = cLink->data();
		//	debugf("  warp %d in sector %d to warp %d\n",(int)pWarp->GetObjectID(),(int)pWarp->GetCluster()->GetObjectID(),(int)pWarp->GetDestination()->GetObjectID());
		//}
    }
private:
	WinPoint GetClusterPoint(CMapPVCluster* pcluster)
	{
		WinPoint pt = WinPoint(
			(int)(m_rectMap.XMin() + ((pcluster->GetScreenX() - m_xMin)/(m_xMax - m_xMin))*(m_rectMap.XSize() - 2*c_nXBorder) + c_nXBorder), 
			(int)(m_rectMap.YMin() + ((pcluster->GetScreenY() - m_yMin)/(m_yMax - m_yMin))*(m_rectMap.YSize() - 2*c_nYBorder) + c_nYBorder)
			);

		return pt;
	}
	void CalculateScreenMinAndMax()
	{
		// map the sector data
		bool    bFirstCluster = true;
		const ClusterPVList* clusters = m_missionpv.GetClusters();
		for (ClusterPVLink* cLink = clusters->first(); cLink != NULL; cLink = cLink->next())
		{
			CMapPVCluster* pCluster = cLink->data();
			if (true) // pCluster->GetModels()->n() != 0
			{
				float   x = pCluster->GetScreenX();
				float   y = pCluster->GetScreenY();
				if (bFirstCluster)
				{
					bFirstCluster = false;

					m_xClusterMin = m_xClusterMax = x;
					m_yClusterMin = m_yClusterMax = y;
				}
				else
				{
					if (x < m_xClusterMin)
						m_xClusterMin = x;
					else if (x > m_xClusterMax)
						m_xClusterMax = x;

					if (y < m_yClusterMin)
						m_yClusterMin = y;
					else if (y > m_yClusterMax)
						m_yClusterMax = y;
				}
			}
		}

		const   float   c_bfr = 0.1f * 
			std::max(0.0001f, std::max(m_xClusterMax - m_xClusterMin,
				m_yClusterMax - m_yClusterMin));
		m_xClusterMin -= c_bfr;
		m_xClusterMax += c_bfr;
		m_yClusterMin -= c_bfr;
		m_yClusterMax += c_bfr;

		// figure out the minimum and maximum screen coordinates
		m_xMin = m_xClusterMin;
		m_xMax = m_xClusterMax;
		m_yMin = m_yClusterMin;
		m_yMax = m_yClusterMax;

		float fDesiredAspectRatio = float(m_rectMap.XSize() - 2*c_nXBorder)/(m_rectMap.YSize() - 2*c_nYBorder);

		// scale it so that a map with height 2 will fit exactly
		const float fMapmakerHeight = 2.2f; // 10% fudge factor
		const float fMaxHeight = fMapmakerHeight;
		const float fMaxWidth = fMaxHeight * fDesiredAspectRatio;

		// if the map is bigger than we want to display, clip the view to the max size
		if (m_xMax - m_xMin > fMaxWidth || m_yMax - m_yMin > fMaxHeight)
		{
			m_bCanDrag = true;
			m_xMax = m_xMin + std::min(m_xMax - m_xMin, fMaxWidth);
			m_yMax = m_yMin + std::min(m_yMax - m_yMin, fMaxHeight);
		}
		else
		{
			m_bCanDrag = false;
		}

		//Preserve the aspect ratio
		float fAspectRatio = (m_xMax - m_xMin) / (m_yMax - m_yMin);

		if (fAspectRatio < fDesiredAspectRatio)
		{
			// grow the X size to correct the aspect ratio
			float fXGrowth = (fDesiredAspectRatio / fAspectRatio - 1) * (m_xMax - m_xMin);
			m_xMax += fXGrowth / 2;
			m_xMin -= fXGrowth / 2;
		}
		else if (fAspectRatio > fDesiredAspectRatio)
		{
			// grow the Y size to correct the aspect ratio
			float fYGrowth = (fAspectRatio / fDesiredAspectRatio - 1) * (m_yMax - m_yMin);
			m_yMax += fYGrowth / 2;
			m_yMin -= fYGrowth / 2;
		}

		// translate by the current drag offset
		m_xMin += m_xDrag;
		m_xMax += m_xDrag;
		m_yMin += m_yDrag;
		m_yMax += m_yDrag;
	}

	static void GetClusterOwners(CMapPVCluster* pcluster, SideID& sideOwner, SideID& sideSecondaryOwner)
	{
		ZAssert (c_cSidesMax == 6);
		int nStations[c_cSidesMax] = {0, 0, 0, 0, 0, 0};

		// count the stations by side
		int *side = pcluster->GetStationsPerSide();
		for (int i =0; i < c_cSidesMax; i++)
		{
			nStations[i]+= side[i];
		}

		// the owner is the one with the most stations
		int     nStationsOwner = 0;
		int     nStationsSecondaryOwner = 0;
		sideOwner = NA;
		sideSecondaryOwner = NA;
		SideID  sideID;
		for (sideID = 0; sideID < c_cSidesMax; sideID++)
		{
			if (nStations[sideID] > nStationsOwner)
			{
				// demote the current top owner
				sideSecondaryOwner = sideOwner;
				nStationsSecondaryOwner = nStationsOwner;

				// track the new top owner
				sideOwner = sideID;
				nStationsOwner = nStations[sideID];
			}
			else if (nStations[sideID] > nStationsSecondaryOwner)
			{
				sideSecondaryOwner = sideID;
				nStationsSecondaryOwner = nStations[sideID];
			}
		}    
	}

public:
	void Render(Context* pcontext)
    {
		
		if (m_bValidPreview)
		{
			RealRender(pcontext);
			if (m_bShowDetails)
			{
				int nbsect = m_missionpv.GetClusters()->n();
				int nbaleph = m_missionpv.GetWarps()->n() / 2;
				ZString str1 = ZString(nbsect) + (nbsect>1? ZString(" sectors"): ZString(" sector"));
				ZString str2 = ZString(nbaleph) + (nbaleph>1? ZString(" alephs"): ZString(" aleph"));

				Rect r = m_bounds.GetRect();
				WinPoint p1 = TrekResources::LargeBoldFont()->GetTextExtent(str1);
				WinPoint p2 = TrekResources::LargeBoldFont()->GetTextExtent(str2);

				debugf("show details: %s - %s\n",(const char *)str1,(const char *)str2);
				int x1 = r.Size().X()-p1.X(); if (x1<0) x1 = 0;
				int x2 = r.Size().X()-p2.X(); if (x2<0) x2 = 0;

				int y2 = r.Size().Y()-p1.Y()-p2.Y(); if (y2<0) y2 = 0;
				y2 = y2/2;
				int y1 = y2+p2.Y(); if (y1<0) y1 = 0;

				pcontext->DrawString(TrekResources::LargeBoldFont(),Color::White(),Point(x1/2,y1),str1);
				pcontext->DrawString(TrekResources::LargeBoldFont(),Color::White(),Point(x2/2,y2),str2);

			}
			return;
		}

		// draw the minimap background
        pcontext->SetShadeMode(ShadeModeFlat);
		pcontext->DrawImage(m_pimageSectorBkgnd->GetSurface());

		if (m_pszReason[0] == '1')
			DrawStringCentered(pcontext,
				TrekResources::SmallFont(),
				Color::White(),
//					Point(20, 96/2-14),
				"invalid settings");
		else if (m_pszReason != NULL)
			DrawStringCentered(pcontext,
				TrekResources::SmallFont(),
				Color::White(),
				m_pszReason);

	}
private:
	// mouse/draging
    MouseResult HitTest(IInputProvider* pprovider, const Point& point, bool bCaptured)
    {
		// KGJV fix: remove capture if map was changed while dragging
		MouseResult mr = m_pimageSectorBkgnd->HitTest(pprovider, point, bCaptured);
		if (bCaptured && !m_bDragging) // we were capturing mouse but we're not dragging anymore
		{
			// reset the mouse cursor
			// if can drag and inside image then set 'hand' cursor
            if (m_bCanDrag && mr.Test(MouseResultHit()))
            {
                GetWindow()->SetCursor(AWF_CURSOR_DRAG);
            }
            else // otherwise default cursor
            {
                GetWindow()->SetCursor(AWF_CURSOR_DEFAULT);
            }

			// release the mouse capture
			mr.Set(MouseResultRelease());
		}
		return mr;
    }
    void MouseMove(IInputProvider* pprovider, const Point& point, bool bCaptured, bool bInside)
    {
        if (bCaptured)
        {
            ZAssert(m_bDragging && m_bCanDrag);

            float fScale = (m_xMax - m_xMin)/m_rectMap.XSize();

            float fDeltaX = fScale * (m_pointLastDrag.X() - point.X());
            float fDeltaY = fScale * (m_pointLastDrag.Y() - point.Y());

            // make sure we don't drag the map off of the screen
            m_xDrag = std::max(std::min((m_xClusterMax - m_xClusterMin) - (m_xMax - m_xMin), m_xDrag + fDeltaX), 0.0f);
            m_yDrag = std::max(std::min((m_yClusterMax - m_yClusterMin) - (m_yMax - m_yMin), m_yDrag + fDeltaY), 0.0f);

            m_pointLastDrag = point;
            GetWindow()->SetCursor(AWF_CURSOR_DRAG);
            Changed();
        }
        else
        {
 
            if (m_bCanDrag)
            {
                GetWindow()->SetCursor(AWF_CURSOR_DRAG);
            }
            else
            {
                GetWindow()->SetCursor(AWF_CURSOR_DEFAULT);
            }
        }

        Changed();
    }

    virtual void MouseLeave(IInputProvider* pprovider)
    { 
        if (!m_bDragging)
            GetWindow()->SetCursor(AWF_CURSOR_DEFAULT);

        Changed();
    }

    MouseResult Button(IInputProvider* pprovider, const Point& point, int button, bool bCaptured, bool bInside, bool bDown)
    {
        if (bDown)
        {
            if (0 == button)
            {
				if (m_bCanDrag)
                {
                    // start a drag
                    m_bDragging = true;
                    m_pointLastDrag = point;
                    GetWindow()->SetCursor(AWF_CURSOR_DRAG);
                    Changed();
                    return MouseResultCapture();
                }
            }
        }
        else
        {
            if (m_bDragging && 0 == button)
            {
                m_bDragging = false;

				if (!bInside)
                {
                    GetWindow()->SetCursor(AWF_CURSOR_DEFAULT);
                }
                else
                {
                    GetWindow()->SetCursor(AWF_CURSOR_DRAG);
                }

                Changed();
                return MouseResultRelease();
            }
        }
    
        Changed();
        return MouseResult();
    }


	void DrawStringCentered(Context* pcontext,IEngineFont*  pfont,  const Color&   color,  const ZString& str)
	{
		Rect r = m_bounds.GetRect();
		WinPoint p = pfont->GetTextExtent(str);
		int x = r.Size().X()-p.X(); if (x<0) x = 0;
		int y = r.Size().Y()-p.Y(); if (y<0) y = 0;

		pcontext->DrawString(pfont,color,Point(x/2,y/2),str);
	}

	// this render the map preview like the in game minimap (mostly it's the same code)
	void RealRender(Context* pcontext)
    {

		// copied from CmissionIGC::UpdateSides .. this should be a global , one day, may be ;)
		static const float sideColors[c_cSidesMax][3] =
                            { {188.0f/255.0f, 160.0f/255.0f,   0.0f/255.0f}, //Gold
                              {  0.0f/255.0f, 138.0f/255.0f, 217.0f/255.0f}, //Blue
                              {156.0f/255.0f,  16.0f/255.0f, 102.0f/255.0f}, //Purple
                              { 50.0f/255.0f, 140.0f/255.0f,  20.0f/255.0f}, //icky yellow
                              {255.0f/255.0f, 145.0f/255.0f, 145.0f/255.0f}, //icky orange
                              { 50.0f/255.0f, 200.0f/255.0f, 125.0f/255.0f}};//icky magenta

		// draw the minimap background
        pcontext->SetShadeMode(ShadeModeFlat);
		pcontext->SetBlendMode(BlendModeSourceAlpha); //imago 7/17/09
		pcontext->DrawImage(m_pimageSectorBkgnd->GetSurface());

		if (m_missionpv.GetClusters()->n() == 0) return; // no cluster so no map preview

		Rect rectClip = m_bounds.GetRect();
		rectClip.Expand(-1);
		pcontext->Clip(rectClip);
		CalculateScreenMinAndMax();


		// draw all the connecting lines for warps
		{
			const WarpPVList* warps =  m_missionpv.GetWarps();
			TVector<VertexL>    vertices(warps->n(), 0);
			TVector<WORD>       indices(warps->n(), 0);

			for (WarpPVLink* warpLink = warps->first(); warpLink != NULL; warpLink = warpLink->next())
			{
				CwarpPV* pWarp = warpLink->data();
				CwarpPV* pwarpDestination = pWarp->GetDestination();
				ZAssert (pwarpDestination != NULL);
				if (pWarp->GetObjectID() > pwarpDestination->GetObjectID())
				{
					Color colorWarp = 0.5f * Color::White(); // upped from 0.5f to 0.7f //imago put back to match sectormap.cpp 7/17/09
					WinPoint point1 = GetClusterPoint(pWarp->GetCluster());
					WinPoint point2 = GetClusterPoint(pwarpDestination->GetCluster());

					indices.PushEnd(vertices.GetCount());
					vertices.PushEnd(VertexL(Vector(point1.x, point1.y, 0), colorWarp));
					indices.PushEnd(vertices.GetCount());
					vertices.PushEnd(VertexL(Vector(point2.x, point2.y, 0), colorWarp));
				}
			}
            
			if (vertices.GetCount() != 0)
			{
				pcontext->SetLineWidth(1.0f);
				pcontext->DrawLines(vertices, indices);
			}
		}

		// draw the data for each sector
		const ClusterPVList* clusters = m_missionpv.GetClusters();
		for (ClusterPVLink* cLink = clusters->first(); cLink != NULL; cLink = cLink->next())
		{
			CMapPVCluster* pCluster = cLink->data();
			//if (pCluster->GetModels()->n() != 0)
			{
				Point    xy = Point::Cast(GetClusterPoint(pCluster));

				// draw the sector outline
				pcontext->DrawImage3D(m_pimageSectorEmpty->GetSurface(), Color::White(), true, xy);

				// color it by the owner(s), if any
				SideID sideOwner;
				SideID sideSecondaryOwner;
				GetClusterOwners(pCluster, sideOwner, sideSecondaryOwner);
				
				// highlight it if it is ours
				if (m_bShowSide)
                if (trekClient.GetSideID() >= 0 && sideOwner >= 0)
				{
					if (sideOwner == trekClient.GetSideID())
						pcontext->DrawImage3D(m_pimageSectorHighlight->GetSurface(), Color::White(), true, xy);
				}
				if (sideOwner != NA)
				{
					Color color;
					color.SetRGBA(sideColors[sideOwner][0], sideColors[sideOwner][1], sideColors[sideOwner][2]);
					pcontext->DrawImage3D(
						m_pimageOwnerHighlight->GetSurface(), 
						color,
						true, 
						xy
					);
				}
			    if (sideSecondaryOwner != NA)
				{
					Color color;
					color.SetRGBA(sideColors[sideSecondaryOwner][0], sideColors[sideSecondaryOwner][1], sideColors[sideSecondaryOwner][2]);
					pcontext->DrawImage3D(
						m_pimageSecondaryOwnerHighlight->GetSurface(), 
						color,
						true, 
						xy
					);
				}
			}
		}
    }
};


TRef<MapPreviewImage> CreateMapPreviewImage(
    Modeler*         pmodeler,
	Image * bkgnd)
{
	return new MapPreviewImageImpl(pmodeler,bkgnd);
}


// MapPreviewPane
void  MapPreviewPane::MouseEnter(IInputProvider* pprovider, const Point& point)
{
	if(m_pMapPreviewImage && m_bShowDetails)
		m_pMapPreviewImage->ShowDetails(true);
	if (m_pMapPreviewImage) m_pMapPreviewImage->MouseEnter(pprovider, point);

}
void  MapPreviewPane::MouseLeave(IInputProvider* pprovider)
{
	if(m_pMapPreviewImage && m_bShowDetails)
		m_pMapPreviewImage->ShowDetails(false);
	if (m_pMapPreviewImage) m_pMapPreviewImage->MouseLeave(pprovider);
	
}