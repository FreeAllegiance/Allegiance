#include "pch.h"
#include "cmdview.h"

#include <button.h>
#include <controls.h>

#include <stdarg.h>
#include <map>
#include <string>

//////////////////////////////////////////////////////////////////////////////
//
// CommandGeo
//
//////////////////////////////////////////////////////////////////////////////
/*
float xMin, float yMin, 
                       float xMax, float yMax, 
*/
//Surface* top
CommandGeo::CommandGeo(float radius, float zGrid, int nSegments) 
    :
    m_vertices(
        4 * (nSegments + 1),
        4 * (nSegments + 1)
    ),
    m_indices(
        4 * (nSegments + 1),
        4 * (nSegments + 1)
    ),
    m_zGrid(zGrid),
    m_radius(radius)
    //m_top(top)
{
    int index = 0;
    float radius2 = radius * radius;

    for (int segment = 0; segment <= nSegments; segment++) 
    {
        float x = (((float)segment / (float)nSegments) - 0.5f) * 2.0f * radius;

        float yMax = (float)sqrt(radius2 - x * x);
        float yMin = -yMax;

        const Color& color = (segment%5 == 0) ? s_colorGridMajor : s_colorGrid;
        
        //Vertical lines
        m_indices.Set(index, index);
        m_vertices.Set(
            index, 
            VertexL(
                x,
                yMin,
                zGrid,
                color.R(),
                color.G(),
                color.B(),
                1,
                0,
                0
            )
        );
        index++;
        
        m_indices.Set(index, index);
        m_vertices.Set(
            index,
            VertexL(
                x,
                yMax,
                zGrid,
                color.R(),
                color.G(),
                color.B(),
                1,
                0,
                0
            )
        );
        index++;

        //Horizontal lines
        m_indices.Set(index, index);
        m_vertices.Set(
            index,
            VertexL(
                yMin,
                x,
                zGrid,
                color.R(),
                color.G(),
                color.B(),
                1,
                0,
                0
            )
        );
        index++;
        
        m_indices.Set(index, index);
        m_vertices.Set(
            index,
            VertexL(
                yMax,
                x,
                zGrid,
                color.R(),
                color.G(),
                color.B(),
                1,
                0,
                0
            )
        );
        index++;
    }

    ZAssert(index == m_indices.GetCount());
}

void CommandGeo::DrawShips(Context* pcontext)
{

    assert (m_pcluster);

    int                 index = 0;
    TVector<VertexL>    vertices;
    TVector<WORD>       indices;
    const ModelListIGC* models = m_pcluster->GetPickableModels();

    assert (models);

    // Draw the ship for every model in the cluster

    ImodelIGC*  pmodelOrders[3] =
                { trekClient.GetShip()->GetCommandTarget(c_cmdCurrent),
                  trekClient.GetShip()->GetCommandTarget(c_cmdAccepted),
                  trekClient.GetShip()->GetCommandTarget(c_cmdQueued)
                };

    for (ModelLinkIGC* l = models->first(); (l != NULL); l = l->next())
    {
        ImodelIGC* pmodel = l->data();

        if (pmodel->GetVisibleF())
        {
            ObjectType    type = pmodel->GetObjectType();
            switch (type)
            {
            default:
            {
                if ((pmodel != pmodelOrders[0]) &&
                    (pmodel != pmodelOrders[1]) &&
                    (pmodel != pmodelOrders[2]))
                {
                    break;
                }
            }

            case OT_ship:
            case OT_station:
            case OT_asteroid:
            case OT_warp:
            {
                const Vector& vecPosition = pmodel->GetPosition();

                // Add a drop line for the ship
                const Color& color = vecPosition.Z() > m_zGrid ? s_colorDropLineUp : s_colorDropLineDown;

                indices.PushEnd(index);
                vertices.PushEnd(VertexL(
                    vecPosition.X(),
                    vecPosition.Y(),
                    vecPosition.Z(),
                    color.R(),
                    color.G(),
                    color.B(),
                    1,
                    0,
                    0
                ));
                index++;

                indices.PushEnd(index);
                vertices.PushEnd(VertexL(
                    vecPosition.X(),
                    vecPosition.Y(),
                    m_zGrid,
                    color.R(),
                    color.G(),
                    color.B(),
                    1,
                    0,
                    0
                ));
                index++;

                // add some feet
                static int sizeFeet = 15;

                indices.PushEnd(index);
                vertices.PushEnd(VertexL(
                    vecPosition.X() + sizeFeet,
                    vecPosition.Y(),
                    m_zGrid,
                    s_colorFeet.R(),
                    s_colorFeet.G(),
                    s_colorFeet.B(),
                    1,
                    0,
                    0
                ));
                index++;

                indices.PushEnd(index);
                vertices.PushEnd(VertexL(
                    vecPosition.X() - sizeFeet,
                    vecPosition.Y(),
                    m_zGrid,
                    s_colorFeet.R(),
                    s_colorFeet.G(),
                    s_colorFeet.B(),
                    1,
                    0,
                    0
                ));
                index++;

                indices.PushEnd(index);
                vertices.PushEnd(VertexL(
                    vecPosition.X(),
                    vecPosition.Y() + sizeFeet,
                    m_zGrid,
                    s_colorFeet.R(),
                    s_colorFeet.G(),
                    s_colorFeet.B(),
                    1,
                    0,
                    0
                ));
                index++;

                indices.PushEnd(index);
                vertices.PushEnd(VertexL(
                    vecPosition.X(),
                    vecPosition.Y() - sizeFeet,
                    m_zGrid,
                    s_colorFeet.R(),
                    s_colorFeet.G(),
                    s_colorFeet.B(),
                    1,
                    0,
                    0
                ));
                index++;
            }
            }
        }

        // draw the drop lines
        if (index)
            pcontext->DrawLines(vertices, indices);

        // After existing per-ship draw work, add waypoint line drawing for selected allied player ships on autopilot.

    }

}

void CommandGeo::DrawTop(Context* pcontext)
{
    TRef<IEngineFont> pfont = TrekResources::HugeBoldFont();

    char* topString = "NORTH";
    float xShiftStr = pfont->GetTextExtent(topString).X();
    //float yShiftStr = pfont->GetHeight();
    

    //float yShiftArrow = m_top->GetSize().Y();
    //float xShiftArrow = m_top->GetSize().X();

    
    Point offset(0, 3 * m_radius / 4);
    //pcontext->DrawImage3D(m_top, s_colorNeutral, true, offset); //Something is going wrong with this image.
    
    offset.SetX(offset.X() - xShiftStr * 0.5f);

    pcontext->DrawString(pfont, s_colorNeutral, offset, topString);
}

// Debug logging below runs for every selected ship on every frame, which floods the
// log (17k lines in eight minutes of play) and buries anything interesting. Emit one
// consolidated line per ship, and only when what it would say actually changes.
static void LogSelectedPath(ShipID shipID, const char* format, ...)
{
    static std::map<ShipID, std::string> s_lastLine;

    char bfr[1024];
    va_list vl;
    va_start(vl, format);
    _vsnprintf_s(bfr, sizeof(bfr), sizeof(bfr) - 1, format, vl);
    va_end(vl);

    std::string& strLast = s_lastLine[shipID];
    if (strLast == bfr)
        return;

    strLast = bfr;
    debugf("%s\n", bfr);
}

// A ship that is running away routes only through friendly space; if that yields
// nothing, the route through hostile space is still what it would have to fly, and
// is still worth drawing.
static PathList* FindPathFrom(IclusterIGC*  pclusterOrigin,
                              const Vector& positionOrigin,
                              IsideIGC*     pside,
                              ImodelIGC*    ptarget,
                              bool          bCoward)
{
    PathList* ppath = FindPathList(pclusterOrigin, positionOrigin, pside, ptarget, bCoward);

    if ((ppath == NULL) && bCoward)
        ppath = FindPathList(pclusterOrigin, positionOrigin, pside, ptarget, false);

    return ppath;
}

// The route to draw for a ship.
//
// The first hop is not ours to choose. The AI picks one aleph when the plan is set and
// flies that leg to the end (GotoPlan::SetControls caches it in m_wpWarp), and the server
// tells us which one. Searching for it again from the ship's current position finds
// whichever aleph is cheapest from *here*, which stops being the one it committed to as
// soon as it has moved - and then we draw a route it was never going to fly. So take the
// committed hop as given and only search the remainder, starting from where that warp
// comes out.
static PathList* BuildRoute(IshipIGC*    pship,
                            IclusterIGC* pclusterOrigin,
                            ImodelIGC*   poriginModel,
                            IsideIGC*    pside,
                            ImodelIGC*   ptarget,
                            bool         bCoward)
{
    const Vector& positionOrigin = poriginModel->GetPosition();

    IwarpIGC* pwarpCommitted = pship->GetWaypointWarp();

    // Only meaningful for the ship itself (not a ripcord model), and only while it is
    // still in the cluster that warp leaves from - once through, the server picks again.
    if ((poriginModel != (ImodelIGC*)pship) ||
        (pwarpCommitted == NULL) ||
        (pwarpCommitted->GetCluster() != pclusterOrigin))
    {
        return FindPathFrom(pclusterOrigin, positionOrigin, pside, ptarget, bCoward);
    }

    IwarpIGC*    pwarpExit    = pwarpCommitted->GetDestination();
    IclusterIGC* pclusterNext = pwarpExit ? pwarpExit->GetCluster() : NULL;

    if (pclusterNext == NULL)
        return FindPathFrom(pclusterOrigin, positionOrigin, pside, ptarget, bCoward);

    PathList* ppath;

    if (pclusterNext == ptarget->GetCluster())
    {
        // The committed hop lands on the target: nothing further to search.
        ppath = new PathList;
    }
    else
    {
        ppath = FindPathFrom(pclusterNext, pwarpExit->GetPosition(), pside, ptarget, bCoward);

        // We know the leg it is flying but cannot see a way on from the far side. Drawing
        // the rest would be invention, so draw nothing.
        if (ppath == NULL)
            return NULL;
    }

    PathLink* plFirst = new PathLink;
    Path&     pathFirst = plFirst->data();

    pathFirst.pwarpStart = pwarpCommitted;
    pathFirst.pwarp      = pwarpCommitted;
    pathFirst.distance   = (pwarpCommitted->GetPosition() - positionOrigin).Length();
    pathFirst.pprev      = NULL;

    ppath->first(plFirst);

    // pwarpStart names the first warp of the whole route, so the searched remainder needs
    // correcting now that it has a hop in front of it.
    for (PathLink* plink = ppath->first(); plink != NULL; plink = plink->next())
        plink->data().pwarpStart = pwarpCommitted;

    return ppath;
}

void CommandGeo::DrawSelectedPaths(Context* pcontext)
{
    const ShipListIGC* pselected = GetWindow()->GetConsoleImage()->GetSubjects();
    if (pselected == NULL)
    {
        return;
    }
    ShipLinkIGC* pshipLink = pselected->first();
    IsideIGC* psideMine = trekClient.GetShip()->GetSide();

    while (pshipLink != NULL) {
        IshipIGC* pship = pshipLink->data();
        IsideIGC* pside = pship->GetSide();
        ShipID    shipID = pship->GetObjectID();

        bool bAllied = (pside == psideMine) || IsideIGC::AlliedSides(pside, psideMine);

        if (bAllied)
        {
            bool        bCoward = (pship->GetPilotType() < c_ptCarrier);
            // Get target and validate
            ImodelIGC* ptarget = pship->GetCommandTarget(c_cmdAccepted);
            if (!ptarget)
            {
                LogSelectedPath(shipID, "Selected ship %s: no c_cmdAccepted target",
                    pship->GetName());
                pshipLink = pshipLink->next();
                continue;
            }

            // Determine origin model based on ripcord state
            ImodelIGC* poriginModel = pship->fRipcordActive() ? pship->GetRipcordModel() : pship;
            assert(poriginModel);

            // Get cluster information
            IclusterIGC* poriginModelCluster = poriginModel->GetCluster();
            const char*  pszClusterSource = "current";

            if (!poriginModelCluster && (poriginModel->GetObjectType() == OT_ship))
            {
                // A ship outside the sector we are viewing has no cluster on the client:
                // the server only sends ship updates to the players flying in that sector
                // (CFSShip::SetCluster -> GetGroupSectorFlying). Its sector is still known,
                // because ShipStatus is broadcast for every ship on the side
                // (FM_S_SHIP_STATUS), and drones are given a PlayerInfo just like players.
                PlayerInfo* ppi = (PlayerInfo*)((IshipIGC*)poriginModel)->GetPrivateData();
                if (ppi && (ppi->LastSeenSector() != NA))
                {
                    poriginModelCluster = trekClient.GetCore()->GetCluster(ppi->LastSeenSector());
                    pszClusterSource = "last seen";
                }
            }

            IclusterIGC* ptargetCluster = ptarget->GetCluster();

            if (!poriginModelCluster)
            {
                // Selected ship is docked, or we have never been told where it is.
                LogSelectedPath(shipID,
                    "Selected ship %s: origin %s has no cluster (in station: %s) - nothing to draw",
                    pship->GetName(),
                    GetModelName(poriginModel),
                    pship->GetStation() ? pship->GetStation()->GetName() : "NO");
                pshipLink = pshipLink->next();
                continue;
            }

            ImodelIGC* pmodelOrigin = nullptr;
            ImodelIGC* pmodelDest = nullptr;
            const char* pszCase = "no path";

            // ===== CASE ANALYSIS AND LOGIC =====

            bool bOriginModelInOurCluster = (poriginModelCluster == m_pcluster);
            bool bTargetInOurCluster = (ptargetCluster == m_pcluster);

            if (bOriginModelInOurCluster && bTargetInOurCluster)
            {
                // CASE 1: Both origin model and target in our cluster
                // Draw line between origin model and target
                pszCase = "origin and target both in view sector";
                pmodelOrigin = poriginModel;
                pmodelDest = ptarget;
            }
            else if (bOriginModelInOurCluster && !bTargetInOurCluster)
            {
                // CASE 2: Origin model in our cluster, target not in our cluster.
                // Draw from the origin out to the warp it would leave through.
                // Ripcording has no route to draw - the ship is not flying one.
                pszCase = "origin in view sector, target elsewhere";

                if (!pship->fRipcordActive())
                {
                    PathList* ppath = BuildRoute(pship, poriginModelCluster, poriginModel, pside, ptarget, bCoward);
                    if (ppath && ppath->first())
                    {
                        pmodelOrigin = poriginModel;
                        pmodelDest = ppath->first()->data().pwarp;
                    }
                    delete ppath;
                }
                else
                {
                    pszCase = "origin in view sector, target elsewhere (ripcording)";
                }
            }
            else if (!bOriginModelInOurCluster && bTargetInOurCluster)
            {
                // CASES 3 & 5: Origin model not in our cluster, target in our cluster.
                // Find the warp through which the origin model enters our cluster and
                // draw from there to the target.
                pszCase = "origin elsewhere, target in view sector";

                PathList* ppath = BuildRoute(pship, poriginModelCluster, poriginModel, pside, ptarget, bCoward);
                if (ppath)
                {
                    for (PathLink* plink = ppath->first(); plink != NULL; plink = plink->next())
                    {
                        IwarpIGC* pwarpDest = plink->data().pwarp->GetDestination();

                        if (pwarpDest && (pwarpDest->GetCluster() == m_pcluster))
                        {
                            pmodelOrigin = pwarpDest;
                            pmodelDest = ptarget;
                            break;
                        }
                    }

                    delete ppath;
                }
            }
            else
            {
                // CASES 4 & 6: Neither origin model nor target in our cluster.
                // Draw the leg that crosses our cluster, if the route passes through it:
                // from the warp it arrives at, to the warp it leaves by.
                pszCase = "origin and target both elsewhere";

                PathList* ppath = BuildRoute(pship, poriginModelCluster, poriginModel, pside, ptarget, bCoward);
                if (ppath)
                {
                    IwarpIGC* pwarpEntryDest = nullptr;
                    IwarpIGC* pwarpExit = nullptr;

                    for (PathLink* plink = ppath->first(); plink != NULL; plink = plink->next())
                    {
                        IwarpIGC* pwarp = plink->data().pwarp;
                        IwarpIGC* pwarpDest = pwarp->GetDestination();

                        // The hop that lands in our cluster gives us the entry point...
                        if (!pwarpEntryDest && pwarpDest && (pwarpDest->GetCluster() == m_pcluster))
                        {
                            pwarpEntryDest = pwarpDest;
                            continue;
                        }

                        // ...and the next hop, leaving our cluster, gives us the exit point.
                        if (pwarpEntryDest && (pwarp->GetCluster() == m_pcluster))
                        {
                            pwarpExit = pwarp;
                            break;
                        }
                    }

                    if (pwarpEntryDest && pwarpExit)
                    {
                        pmodelOrigin = pwarpEntryDest;
                        pmodelDest = pwarpExit;
                    }

                    delete ppath;
                }
            }

            LogSelectedPath(shipID,
                "Selected ship %s: origin %s in %s (%s), target %s in %s, viewing %s -> %s, drawing %s to %s",
                pship->GetName(),
                GetModelName(poriginModel),
                poriginModelCluster->GetName(),
                pszClusterSource,
                GetModelName(ptarget),
                ptargetCluster ? ptargetCluster->GetName() : "NULL",
                m_pcluster ? m_pcluster->GetName() : "NULL",
                pszCase,
                pmodelOrigin ? GetModelName(pmodelOrigin) : "nothing",
                pmodelDest ? GetModelName(pmodelDest) : "nothing");

            // ===== DRAW THE PATH =====
            if (pmodelOrigin && pmodelDest && pmodelOrigin != pmodelDest)
            {
                Vector vOriginPos = pmodelOrigin->GetPosition();
                Vector vDestPos = pmodelDest->GetPosition();

                TVector<VertexL> lineVerts(2);
                TVector<WORD> lineInds(2);

                Color lineColor = pside ? pside->GetColor() : Color::White();

                lineVerts.Set(0, VertexL(vOriginPos, lineColor));
                lineVerts.Set(1, VertexL(vDestPos, lineColor));
                lineInds.Set(0, 0);
                lineInds.Set(1, 1);

                pcontext->PushState();
                pcontext->SetBlendMode(BlendModeAdd);
                pcontext->SetLineWidth(2.0f, true);
                pcontext->DrawLines(lineVerts, lineInds);
                pcontext->SetLineWidth(1.0f, true);
                pcontext->PopState();
            }
        }

        pshipLink = pshipLink->next();
    }
}

void CommandGeo::Render(Context* pcontext)
{
    // use flat shading
    pcontext->SetShadeMode(ShadeModeFlat);

    // draw the ships
    DrawShips(pcontext);
    DrawSelectedPaths(pcontext);

    // draw the grid
    pcontext->DrawLines(m_vertices, m_indices);

    DrawTop(pcontext);

    //Draw the drop line for the point in space (if there is one)
    {
        ConsoleImage*   pci = GetWindow()->GetConsoleImage();
        if (pci)
            pci->RenderDropLine(pcontext);
    }

}
 
void CommandGeo::SetCluster(IclusterIGC* pcluster)
{
    m_pcluster = pcluster;
}

const Color CommandGeo::s_colorGrid(0,0,64.0f/255.0f);
const Color CommandGeo::s_colorGridMajor(0,0,128.0f/255.0f);
const Color CommandGeo::s_colorDropLineUp(200.0f / 255.0f, 180.0f / 255.0f, 20.0f / 255.0f);
const Color CommandGeo::s_colorDropLineDown(150.0f / 255.0f, 100.0f / 255.0f, 0.0f / 255.0f);
const Color CommandGeo::s_colorFeet(0,0,128.0f/255.0f);
const Color CommandGeo::s_colorNeutral(1.0f, 1.0f, 1.0f);

const Color g_colorUp(75 / 255.0f, 124 / 255.0f, 88 / 255.0f);
const Color g_colorDown(2 / 255.0f, 84 / 255.0f, 84 / 255.0f);


//////////////////////////////////////////////////////////////////////////////
//
// ImageStringPane
//
//////////////////////////////////////////////////////////////////////////////

ImageStringPane::ImageStringPane(LPCSTR szText, LPCSTR szImage, bool bUp, int Layout)
{
    m_pStringPane = new StringPane(szText, TrekResources::SmallFont());
    m_pStringPane->SetTextColor(bUp ? g_colorUp : g_colorDown);

    TRef<SurfacePane> pSurfacePane;

    if (szImage)
        pSurfacePane = new SurfacePane(GetModeler()->LoadSurface(szImage, true));

    TRef<RowPane> pRow = new RowPane();

    if (Layout == ImageLeft && szImage)
        pRow->InsertAtBottom(pSurfacePane);

    pRow->InsertAtBottom(m_pStringPane);

    if (Layout != ImageLeft && szImage)
        pRow->InsertAtBottom(pSurfacePane);
    
    TRef<BorderPane> pBorderPane = 
        new BorderPane(
            1, 
            bUp ? g_colorDown : g_colorUp,
            pRow
        );

    InsertAtBottom(pBorderPane);
}


void ImageStringPane::UpdateLayout()
{
    DefaultUpdateLayout();
}


void ImageStringPane::SetString(LPCSTR szText)
{
    m_pStringPane->SetString(szText);
}


//////////////////////////////////////////////////////////////////////////////
//
// ImageStringButtonPane
//
//////////////////////////////////////////////////////////////////////////////

ImageStringButtonPane::ImageStringButtonPane(LPCSTR szText, LPCSTR szImageUp, LPCSTR szImageDn, int Layout)
{

     m_pButtonPane =
        CreateTrekButton(
            new EdgePane(m_pImageStringPaneUp = new ImageStringPane(szText, szImageUp, true, Layout), true),
            new EdgePane(m_pImageStringPaneDn = new ImageStringPane(szText, szImageDn, false, Layout), false)
        );

    InsertAtBottom(m_pButtonPane);
}


void ImageStringButtonPane::UpdateLayout()
{
    DefaultUpdateLayout();
}


void ImageStringButtonPane::SetString(LPCSTR szText)
{
    m_pImageStringPaneUp->SetString(szText);
    m_pImageStringPaneDn->SetString(szText);
}

IEventSource* ImageStringButtonPane::GetEventSource()
{
    return m_pButtonPane->GetEventSource();
}
