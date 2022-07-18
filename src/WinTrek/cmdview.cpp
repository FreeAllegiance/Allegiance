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
        m_zGrid(zGrid)
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

    ZAssert (m_pcluster);

    int                 index = 0;
    TVector<VertexL>    vertices;
    TVector<WORD>       indices;
    const ModelListIGC* models = m_pcluster->GetPickableModels();

    ZAssert (models);

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
    }

    // draw the drop lines
    if (index)
        pcontext->DrawLines(vertices, indices);
}


void CommandGeo::Render(Context* pcontext)
{
    // use flat shading
    pcontext->SetShadeMode(ShadeModeFlat);

    // draw the ships
    DrawShips(pcontext);

    // draw the grid
    pcontext->DrawLines(m_vertices, m_indices);

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
