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
                    const Vector&   vecPosition = pmodel->GetPosition();

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
                                vecPosition.X()+sizeFeet,
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
                                vecPosition.X()-sizeFeet,
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
                                vecPosition.Y()+sizeFeet,
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
                                vecPosition.Y()-sizeFeet,
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

        if (pmodel->GetObjectType() == OT_ship)
        {
            IshipIGC* pship = static_cast<IshipIGC*>(pmodel);

            // Only draw for ships that are selected, allied, and on autopilot
            IsideIGC* psideMine = trekClient.GetShip()->GetSide();
            IsideIGC* pside = pship->GetSide();

            const ShipListIGC* pselected = GetWindow()->GetConsoleImage()->GetSubjects();

            bool bSelected = (pselected && pselected->find(pship));
            bool bAllied = (pside == psideMine) || IsideIGC::AlliedSides(pside, psideMine);

            if (bSelected && bAllied)
            {
                // Determine an ImodelIGC destination: prefer on-cluster waypoint buoy, otherwise use path warp or the target model.
                ImodelIGC* ptarget = pship->GetCommandTarget(c_cmdAccepted);
                if (!ptarget) ptarget = pship->GetCommandTarget(c_cmdPlan);
                
                ImodelIGC* pmodelDest = nullptr;

                if (ptarget)
                {
                    if (ptarget->GetObjectType() == OT_buoy)
                    {
                        // buoy itself has position; use the buoy model
                        IbuoyIGC* pbuoy = static_cast<IbuoyIGC*>(ptarget);
                        if (pbuoy && (pbuoy->GetBuoyType() == c_buoyWaypoint))
                            pmodelDest = pbuoy;
                    }

                    if (!pmodelDest)
                    {
                        IclusterIGC* pclusterTarget = ptarget->GetCluster();
                        if ((pclusterTarget == NULL) || (pclusterTarget != m_pcluster))
                        {
                            // If target is off-cluster and ship isn't ripcording, get the local warp (aleph) path
                            if (!pship->fRipcordActive())
                            {
                                ImodelIGC* ppath = FindPath(pship, ptarget, false);
                                if (ppath)
                                    pmodelDest = ppath;
                            }
                        }
                        else
                        {
                            // target is on this cluster; draw to the model directly
                            pmodelDest = ptarget;
                        }
                    }
                }

                // Draw line between ship position and destination model position (if available)
                if (pmodelDest)
                {
                    Vector vShipPos = pship->GetPosition();
                    Vector vDestPos = pmodelDest->GetPosition();

                    
                    Vector vShipGrid(vShipPos.X(), vShipPos.Y(), vShipPos.Z());
                    Vector vDestGrid(vDestPos.X(), vDestPos.Y(), vDestPos.Z());

                    TVector<VertexL> lineVerts(2);
                    TVector<WORD> lineInds(2);

                    Color lineColor = pside ? pside->GetColor() : Color::White();

                    lineVerts.Set(0, VertexL(vShipGrid, lineColor));
                    lineVerts.Set(1, VertexL(vDestGrid, lineColor));
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
        }
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

void CommandGeo::Render(Context* pcontext)
{
    // use flat shading
    pcontext->SetShadeMode(ShadeModeFlat);

    // draw the ships
    DrawShips(pcontext);

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
