#include "pch.h"
#include "cmdview.h"

#include <button.h>
#include <controls.h>

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

        bool bAllied = (pside == psideMine) || IsideIGC::AlliedSides(pside, psideMine);

        if (bAllied)
        {
            bool        bCoward = (pship->GetPilotType() < c_ptCarrier);
            // Get target and validate
            ImodelIGC* ptarget = pship->GetCommandTarget(c_cmdAccepted);
            if (!ptarget)
            {
                pshipLink = pshipLink->next();
                continue;
            }

            // Determine origin model based on ripcord state
            ImodelIGC* poriginModel = pship->fRipcordActive() ? pship->GetRipcordModel() : pship;
            assert(poriginModel);

            // Get cluster information
            IclusterIGC* poriginModelCluster = poriginModel->GetCluster();
            if (!poriginModelCluster)
            {
                //selected ship is in base or is otherwise unable to get cluster
                //Student TODO getting the originModel of a ship not in the view cluster is not reliable
                //Unknown why
                pshipLink = pshipLink->next();
                continue;
            }
            IclusterIGC* ptargetCluster = ptarget->GetCluster();

            ImodelIGC* pmodelOrigin = nullptr;
            ImodelIGC* pmodelDest = nullptr;

            // ===== CASE ANALYSIS AND LOGIC =====

            bool bOriginModelInOurCluster = (poriginModelCluster == m_pcluster);
            bool bTargetInOurCluster = (ptargetCluster == m_pcluster);

            if (bOriginModelInOurCluster && bTargetInOurCluster)
            {
                // CASE 1: Both origin model and target in our cluster
                // Draw line between origin model and target
                pmodelOrigin = poriginModel;
                pmodelDest = ptarget;
            }
            else if (bOriginModelInOurCluster && !bTargetInOurCluster)
            {
                // CASE 2: Origin model in our cluster, target not in our cluster
                // Only draw if ship is not ripcording (ripcording has no path to draw)
                if (!pship->fRipcordActive())
                {
                    // Find path from origin model to target
                    IwarpIGC* warp = FindPath(poriginModel, ptarget, bCoward);
                    if (!warp && bCoward)
                    {
                        warp = FindPath(poriginModel, ptarget, false);
                    }
                    if (warp != NULL)
                    {    
                        pmodelOrigin = poriginModel;
                        pmodelDest = warp;
                    }
                }
            }
            else if (!bOriginModelInOurCluster && bTargetInOurCluster)
            {
                // CASES 3 & 5: Origin model not in our cluster, target in our cluster
                // We need to find the warp through which the origin model would enter our cluster

                // Find path from origin model to target
                PathList* ppath = FindPathList(poriginModel, ptarget, bCoward);

                //retry if we are coward without being coward
                if (ppath == NULL && bCoward) {
                    ppath = FindPathList(poriginModel, ptarget, false);
                }
                if (ppath && ppath->first() != NULL)
                {
                    // Walk through the path to find when we enter our cluster
                    bool bFoundEntryWarp = false;
                    PathLink* plink = ppath->first();

                    while (plink != NULL && !bFoundEntryWarp)
                    {
                        IwarpIGC* pwarp = plink->data().pwarp;
                        IwarpIGC* pwarpDest = pwarp->GetDestination();

                        // Check if this warp's destination is in our cluster
                        if (pwarpDest && pwarpDest->GetCluster() == m_pcluster)
                        {
                            pmodelOrigin = pwarpDest;
                            pmodelDest = ptarget;
                            bFoundEntryWarp = true;
                        }

                        plink = plink->next();
                    }

                    // Clean up the path list
                    plink = ppath->first();
                    while (plink != NULL)
                    {
                        PathLink* plinkNext = plink->next();
                        delete plink;
                        plink = plinkNext;
                    }
                    delete ppath;
                }
            }
            else
            {
                // CASES 4 & 6: Neither origin model nor target in our cluster
                // Check if the path passes through our cluster

                PathList* ppath = FindPathList(poriginModel, ptarget, bCoward);

                //retry if we are coward without being coward
                if (ppath == NULL && bCoward) {
                    ppath = FindPathList(poriginModel, ptarget, false);
                }
                if (ppath && ppath->first() != NULL)
                {
                    // Walk through the path to find entry and exit warps for our cluster
                    IwarpIGC* pwarpEntryDest = nullptr;
                    IwarpIGC* pwarpExitWarp = nullptr;

                    PathLink* plink = ppath->first();
                    while (plink != NULL)
                    {
                        IwarpIGC* pwarp = plink->data().pwarp;
                        IwarpIGC* pwarpDest = pwarp->GetDestination();

                        if (pwarpDest && pwarpDest->GetCluster() == m_pcluster)
                        {
                            // This warp's destination is in our cluster
                            if (!pwarpEntryDest)
                            {
                                // This is our entry point
                                pwarpEntryDest = pwarpDest;
                            }
                            // Keep updating to get the last (exit) warp
                            pwarpExitWarp = pwarp;
                        }

                        plink = plink->next();
                    }

                    // If we found an entry point and exit point, draw the path
                    if (pwarpEntryDest && pwarpExitWarp && pwarpExitWarp->GetCluster() == m_pcluster)
                    {
                        pmodelOrigin = pwarpEntryDest;
                        pmodelDest = pwarpExitWarp;
                    }

                    // Clean up the path list
                    plink = ppath->first();
                    while (plink != NULL)
                    {
                        PathLink* plinkNext = plink->next();
                        delete plink;
                        plink = plinkNext;
                    }
                    delete ppath;
                }
            }

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
