#include "button.h"

#include <event.h>
#include <tmap.h>
#include <tref.h>


#include "D3DDevice9.h"
#include "enginep.h"
#include "surface.h"
#include "UIVertexDefn.h"
#include "VertexGenerator.h"
#include "controls.h"

//////////////////////////////////////////////////////////////////////////////
//
// ImageButtonFacePane
//
//////////////////////////////////////////////////////////////////////////////

class ImageButtonFacePane : public ButtonFacePane {
private:
    TRef<Surface> m_psurface;
    DWORD         m_dwFaces;
    int           m_countFaces;
    int           m_xmin;
    int           m_xmax;
    int           m_xsize;
    int           m_ysize;
    bool          m_bFocus;
    bool          m_bEnabled;
    bool          m_bChecked;
    bool          m_bChecked2;
    bool          m_bDown;
    bool          m_bInside;
	
	// Added vars.
	static const int m_iNumVertsPerButton = 4;
	UIVERTEX *		m_pButtonVertices;
	DWORD			m_dwIndividualButtonStride;
	DWORD			m_dwTotalVerts;

private:
	////////////////////////////////////////////////////////////////////////////////////////////////
	// GenerateButtonVertices()
	// Generate a set of vertices for each button frame on the current surface.
	// Add vertices to static vertex buffer.
	////////////////////////////////////////////////////////////////////////////////////////////////
	void GenerateButtonVertices( )
	{
		DWORD i, dwVertOffset;

		m_dwTotalVerts		= m_countFaces * m_iNumVertsPerButton;
		m_pButtonVertices	= new UIVERTEX[ m_dwTotalVerts ]; //Fix memory leak -Imago 8/2/09

		// Blt area defined by. Buttons arranged vertically on the surface.
		PrivateSurface * pprivateSurfSource;
		CastTo( pprivateSurfSource, m_psurface );
		TEXHANDLE hTexture = pprivateSurfSource->GetTexHandle();
        uint32_t dwSurfaceWidth, dwSurfaceHeight;
		CVRAMManager::Get()->GetOriginalDimensions( hTexture, &dwSurfaceWidth, &dwSurfaceHeight );
		
		float fUpperV, fLowerV, fUpperY, fLowerY;
		int iYMax = m_ysize;

		fUpperY = 0.0f;
		for( i=0; i<(DWORD)m_countFaces; i++ )
		{
			fLowerY = fUpperY + (float) iYMax;
			fUpperV = (fUpperY) / (float) dwSurfaceHeight;
			fLowerV = (fLowerY) / (float) dwSurfaceHeight;

			// Incorporate 2d pixel offset for correct texturing.
			dwVertOffset = i * m_iNumVertsPerButton;
			m_pButtonVertices[dwVertOffset].x		= -0.5f;
			m_pButtonVertices[dwVertOffset].y		= -0.5f;
			m_pButtonVertices[dwVertOffset].z		= 0.5f;
			m_pButtonVertices[dwVertOffset].rhw		= 1.0f;
			m_pButtonVertices[dwVertOffset].fU		= (float) m_xmin / (float) dwSurfaceWidth;
			m_pButtonVertices[dwVertOffset++].fV	= fUpperV;

			m_pButtonVertices[dwVertOffset].x		= (float) ( m_xmax - m_xmin ) - 0.5f;
			m_pButtonVertices[dwVertOffset].y		= -0.5f;
			m_pButtonVertices[dwVertOffset].z		= 0.5f;
			m_pButtonVertices[dwVertOffset].rhw		= 1.0f;
			m_pButtonVertices[dwVertOffset].fU		= (float) m_xmax / (float) dwSurfaceWidth;
			m_pButtonVertices[dwVertOffset++].fV	= fUpperV;

			m_pButtonVertices[dwVertOffset].x		= -0.5f;
			m_pButtonVertices[dwVertOffset].y		= (float) m_ysize - 0.5f;
			m_pButtonVertices[dwVertOffset].z		= 0.5f;
			m_pButtonVertices[dwVertOffset].rhw		= 1.0f;
			m_pButtonVertices[dwVertOffset].fU		= (float) m_xmin / (float) dwSurfaceWidth;
			m_pButtonVertices[dwVertOffset++].fV	= fLowerV;

			m_pButtonVertices[dwVertOffset].x		= (float) ( m_xmax - m_xmin ) - 0.5f;
			m_pButtonVertices[dwVertOffset].y		= (float) m_ysize - 0.5f;
			m_pButtonVertices[dwVertOffset].z		= 0.5f;
			m_pButtonVertices[dwVertOffset].rhw		= 1.0f;
			m_pButtonVertices[dwVertOffset].fU		= (float) m_xmax / (float) dwSurfaceWidth;
			m_pButtonVertices[dwVertOffset++].fV	= fLowerV;

			fUpperY += (float) m_ysize;
		}
	}

public:
    ImageButtonFacePane(Surface* psurface, DWORD dwFaces, int xmin, int xmax) :
        m_psurface(psurface),
        m_dwFaces(dwFaces),
        m_bFocus(false),
        m_bEnabled(true),
        m_bChecked(false),
        m_bChecked2(false),
        m_bDown(false),
        m_bInside(false),
        m_xmin(xmin),
        m_xmax(xmax)
    {
        m_countFaces = CountBits(m_dwFaces);
        const WinPoint& size = m_psurface->GetSize();

        if (m_xmax == 0) {
            m_xmin = 0;
            m_xmax = size.X();
        }

        m_ysize = size.Y() / m_countFaces;

        InternalSetSize(WinPoint(m_xmax - m_xmin, m_ysize));

        //
        // Make sure the bitmap is an integer multiple on m_countFaces
        //

        ZAssert(m_countFaces * m_ysize == size.Y());

		// Generate the polys for each button.
		GenerateButtonVertices( );
    }

    void SetFocus(bool bFocus)
    {
        if (m_bFocus != bFocus) {
            m_bFocus = bFocus;
            NeedPaint();
        }
    }

    void SetEnabled(bool bEnabled)
    {
        if (m_bEnabled != bEnabled) {
            m_bEnabled = bEnabled;
            NeedPaint();
        }
    }

    void SetChecked(bool bChecked)
    {
        if (m_bChecked != bChecked) {
            m_bChecked = bChecked;
            NeedPaint();
        }
    }

    void SetChecked2(bool bChecked)
    {
        if (m_bChecked2 != bChecked) {
            m_bChecked2 = bChecked;
            NeedPaint();
        }
    }

    void SetInside(bool bInside)
    {
        if (m_bInside != bInside) {
            m_bInside = bInside;
            NeedPaint();
        }
    }

    void SetDown(bool bDown)
    {
        if (m_bDown != bDown) {
            m_bDown = bDown;
            NeedPaint();
        }
    }

    int GetFaceIndex(DWORD face)
    {
        int   index = 0;
        DWORD faces = m_dwFaces;

        while ((face & 1) == 0) {
            if (faces & 1) {
                index++;
            }

            faces = faces >> 1;
            face  = face  >> 1;
        }

        if ((faces & 1) == 0) {
            return -1;
        }

        return index;
    }

    void BltFace(Surface* psurface, DWORD face)
    {
        int index = GetFaceIndex(face);

        if (index != -1) {
            psurface->BitBlt(
                WinPoint(0, 0),
                m_psurface,
                WinRect(
                    m_xmin,
                    index * m_ysize,
                    m_xmax,
                    (index + 1) * m_ysize
                )
            );
        }
    }

    DWORD ChooseFace(DWORD dw, DWORD dwFallBack)
    {
        if (m_dwFaces & dw) {
            return dw;
        } else {
            return dwFallBack;
        }
    }

    void Paint(Surface* psurface)
    {
        //
        // Figure out which face to draw
        //

        DWORD face = 0;

        if (m_bEnabled) {
            if (m_bDown) {
                if (m_bChecked2) {
                    face = ChooseFace(ButtonFaceChecked2Down, ButtonFaceUp);
                } else if (m_bChecked) {
                    face = ChooseFace(ButtonFaceCheckedDown, ButtonFaceUp);
                } else {
                    face = ChooseFace(ButtonFaceDown, ButtonFaceUp);
                }
            } else if (m_bInside) {
                if (m_bChecked2) {
                    face =
                        ChooseFace(
                            ButtonFaceChecked2Inside,
                            ChooseFace(
                                ButtonFaceInside,
                                ChooseFace(
                                    ButtonFaceChecked2Up,
                                    ButtonFaceUp
                                )
                            )
                        );
                } else if (m_bChecked) {
                    face =
                        ChooseFace(
                            ButtonFaceCheckedInside,
                            ChooseFace(
                                ButtonFaceInside,
                                ChooseFace(
                                    ButtonFaceCheckedUp,
                                    ButtonFaceUp
                                )
                            )
                        );
                } else {
                    face = ChooseFace(ButtonFaceInside, ButtonFaceUp);
                }
            } else {
                if (m_bChecked2) {
                    face = ChooseFace(ButtonFaceChecked2Up, ButtonFaceDown);
                } else if (m_bChecked) {
                    face = ChooseFace(ButtonFaceCheckedUp, ButtonFaceDown);
                } else {
                    face = ButtonFaceUp;
                }
            }
        } else {
            if (m_bChecked2) {
                face = ChooseFace(ButtonFaceChecked2Disabled, ButtonFaceDisabled);
            } else if (m_bChecked) {
                face = ChooseFace(ButtonFaceCheckedDisabled, ButtonFaceDisabled);
            } else {
                face = ButtonFaceDisabled;
            }
        }

/*		// Draw the face
		BltFace(psurface, face);

		// Overlay the focus bitmap on top of the button
		if (m_bFocus && (m_dwFaces & ButtonFaceFocus)) 
		{
			BltFace(psurface, ButtonFaceFocus);
		}*/

		PrivateSurface * pprivateSurfSource;
		CastTo( pprivateSurfSource, m_psurface );
		CVRAMManager::Get()->SetTexture( pprivateSurfSource->GetTexHandle(), 0 );

        int index = GetFaceIndex(face);

        if (index != -1) 
		{
			CD3DDevice9 * pDev = CD3DDevice9::Get();

			// Render this pane.
			float fXOffset, fYOffset;
			fXOffset = (float) psurface->GetOffset().X();
			fYOffset = (float) psurface->GetOffset().Y();

			// Render via a dynamic vertex buffer.
			UIVERTEX * pVerts;
//			CVBIBManager::SVBIBHandle * pDynVB = CVertexGenerator::Get()->GetUITexVertsVB();
			CVBIBManager::SVBIBHandle * pDynVB = CVertexGenerator::Get()->GetPredefinedDynamicBuffer( CVertexGenerator::ePDBT_UITexVB );
			if (CVBIBManager::Get()->LockDynamicVertexBuffer( pDynVB, m_iNumVertsPerButton, (void**)&pVerts ) == false) {
                return;
            }
												
			// Overlay the focus bitmap on top of the button
			if (m_bFocus && (m_dwFaces & ButtonFaceFocus)) 
			{
				index = GetFaceIndex(ButtonFaceFocus);
			}

			int iButtonVertIndex = ( index * m_iNumVertsPerButton );
			
			// Create the vertices and render the button.
			for( DWORD i=0; i<m_iNumVertsPerButton; i++ )
			{
				pVerts[i].x		= m_pButtonVertices[ iButtonVertIndex ].x + fXOffset;
				pVerts[i].y		= m_pButtonVertices[ iButtonVertIndex ].y + fYOffset;
				pVerts[i].z		= m_pButtonVertices[ iButtonVertIndex ].z;
				pVerts[i].rhw	= m_pButtonVertices[ iButtonVertIndex ].rhw;
				pVerts[i].fU	= m_pButtonVertices[ iButtonVertIndex ].fU;
				pVerts[i].fV	= m_pButtonVertices[ iButtonVertIndex ].fV;
				iButtonVertIndex ++;
			}
			CVBIBManager::Get()->UnlockDynamicVertexBuffer( pDynVB );
			CVBIBManager::Get()->SetVertexStream( pDynVB );
			pDev->SetRenderState( D3DRS_ZENABLE, D3DZB_FALSE );
			pDev->SetFVF( D3DFVF_UIVERTEX );
			pDev->DrawPrimitive(	D3DPT_TRIANGLESTRIP, pDynVB->dwFirstElementOffset, 2 );
		}
	}
};

TRef<ButtonFacePane> CreateButtonFacePane(Surface* psurface, DWORD dwFaces, int xmin, int xmax)
{
    return new ImageButtonFacePane(psurface, dwFaces, xmin, xmax);
}

//////////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////////

class PaneButtonFacePane : public ButtonFacePane {
private:
    TRef<Pane> m_ppaneUp;
    TRef<Pane> m_ppaneDown;
    bool       m_bDown;
    bool       m_bChecked;

public:
    PaneButtonFacePane(Pane* ppaneUp, Pane* ppaneDown) :
        m_ppaneUp(ppaneUp),
        m_ppaneDown(ppaneDown),
        m_bDown(false),
        m_bChecked(false)
    {
        InsertAtBottom(m_ppaneUp);
        if (m_ppaneDown) {
            InsertAtBottom(m_ppaneDown);
            m_ppaneDown->SetHidden(true);
        }
    }

    void SetDown(bool bDown)
    {
        if (bDown != m_bDown) {
            m_bDown = bDown;
            NeedLayout();
            NeedPaint();
        }
    }

    virtual void SetChecked(bool bChecked)
    {
        if (bChecked != m_bChecked) {
            m_bChecked = bChecked;
            NeedLayout();
            NeedPaint();
        }
    }

    void UpdateLayout()
    {
        bool bFaceDown;

        if (m_bChecked) {
            bFaceDown = !m_bDown;
        } else {
            bFaceDown = m_bDown;
        }

        if (bFaceDown && m_ppaneDown) {
            InternalSetHidden(m_ppaneUp,   true);
            InternalSetHidden(m_ppaneDown, false);

            InternalSetExpand(m_ppaneDown, GetExpand());
            m_ppaneDown->UpdateLayout();
            InternalSetOffset(m_ppaneDown, WinPoint(0, 0));
            InternalSetSize(m_ppaneDown->GetSize());
        } else {
            InternalSetHidden(m_ppaneUp,   false);
            if (m_ppaneDown) {
                InternalSetHidden(m_ppaneDown, true);
            }

            InternalSetExpand(m_ppaneUp, GetExpand());
            m_ppaneUp->UpdateLayout();
            InternalSetOffset(m_ppaneUp, WinPoint(0, 0));
            InternalSetSize(m_ppaneUp->GetSize());
        }
    }
};

TRef<ButtonFacePane> CreateButtonFacePane(Pane* ppaneUp, Pane* ppaneDown)
{
    return new PaneButtonFacePane(ppaneUp, ppaneDown);
}

//////////////////////////////////////////////////////////////////////////////
//
// ButtonUIPane
//
//////////////////////////////////////////////////////////////////////////////

class ButtonUIPane :
    public ButtonPane,
    public IEventSink
{
    TRef<ButtonFacePane>  m_pfacePane;
    TRef<EventSourceImpl> m_peventSource;
	TRef<EventSourceImpl> m_peventRightSource; //imago 7/6/09
    TRef<EventSourceImpl> m_peventSourceDoubleClick;
    TRef<EventSourceImpl> m_peventMouseEnterSource;
    TRef<EventSourceImpl> m_peventMouseLeaveSource;
    TRef<EventSourceImpl> m_peventMouseEnterWhileEnabledSource;
    TRef<IInputProvider>  m_pprovider;
    TRef<IEventSink>      m_peventSinkDelegate;

    bool  m_bToggle;
    bool  m_bFocus;
    bool  m_bEnabled;
    bool  m_bChecked;
    bool  m_bChecked2;
    bool  m_bDown;
    bool  m_bInside;
    bool  m_bFirstEvent;
    bool  m_bDownTrigger;
    float m_repeatDelay;
    float m_repeatRate;
	int m_button; //imago 7/6/09

public:
    ButtonUIPane(
        ButtonFacePane* ppane,
        float repeatRate,
        float repeatDelay,
        bool  bToggle
    ) :
        m_pfacePane(ppane),
        m_peventSource(new EventSourceImpl()),
		m_peventRightSource(new EventSourceImpl()), //imago 7/6/09
        m_peventSourceDoubleClick(new EventSourceImpl()),
        m_peventMouseEnterSource(new EventSourceImpl()),
        m_peventMouseLeaveSource(new EventSourceImpl()),
        m_peventMouseEnterWhileEnabledSource(new EventSourceImpl()),
        m_repeatDelay(repeatDelay),
        m_repeatRate(repeatRate),
        m_bToggle(bToggle),
        m_bFocus(false),
        m_bEnabled(true),
        m_bChecked(false),
        m_bChecked2(false),
        m_bDown(false),
        m_bInside(false),
        m_bDownTrigger(false)
    {
        InsertAtBottom(m_pfacePane);
        m_peventSinkDelegate = IEventSink::CreateDelegate(this);
    }

    ~ButtonUIPane()
    {
        if (m_pprovider)
        {
            m_pprovider->GetTimer()->RemoveSink(m_peventSinkDelegate);
            m_pprovider = NULL;
        }
    }

    void SetDown(bool bDown)
    {
        if (m_bDown != bDown) {
            m_bDown = bDown;
            m_pfacePane->SetDown(m_bDown);
        }
    }

    void SetInside(bool bInside)
    {
        if (m_bInside != bInside) {
            m_bInside = bInside;
            m_pfacePane->SetInside(m_bInside);
        }
    }

    void SetFocus(bool bFocus)
    {
        if (m_bFocus != bFocus) {
            m_bFocus = bFocus;
            m_pfacePane->SetFocus(m_bFocus);
        }
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // ButtonPane Methods
    //
    //////////////////////////////////////////////////////////////////////////////

    void SetDownTrigger(bool bDownTrigger) 
    {
        m_bDownTrigger = bDownTrigger;
    }

    void SetEnabled(bool bEnabled)
    {
        if (m_bEnabled != bEnabled) {
            m_bEnabled = bEnabled;
            m_pfacePane->SetEnabled(m_bEnabled);

            if (bEnabled && m_bInside)
                m_peventMouseEnterWhileEnabledSource->Trigger();
        }
    }

    void SetChecked(bool bChecked)
    {
        if (m_bChecked != bChecked) {
            m_bChecked = bChecked;
            m_pfacePane->SetChecked(m_bChecked);
        }
    }

    void SetChecked2(bool bChecked)
    {
        if (m_bChecked2 != bChecked) {
            m_bChecked2 = bChecked;
            m_pfacePane->SetChecked2(m_bChecked2);
        }
    }

    void SetRepeat(float repeatRate, float repeatDelay)
    {
        if (repeatRate == 0 && m_pprovider != NULL) {
            m_pprovider->GetTimer()->RemoveSink(m_peventSinkDelegate);
            m_pprovider = NULL;
        }

        m_repeatRate  = repeatRate;
        m_repeatDelay = repeatDelay;
    }

    bool GetChecked()
    {
        return m_bChecked;
    }

    bool GetChecked2()
    {
        return m_bChecked2;
    }

    bool GetEnabled()
    {
        return m_bEnabled;
    }

    IEventSource* GetEventSource()
    {
        return m_peventSource;
    }

    IEventSource* GetDoubleClickEventSource()
    {
        return m_peventSourceDoubleClick;
    }

	//Imago added right click to button UI 7/6/09
    IEventSource* GetRightEventSource()
    {
        return m_peventRightSource;
    }


    IEventSource* GetMouseEnterEventSource()
    {
        return m_peventMouseEnterSource;
    }

    IEventSource* GetMouseLeaveEventSource()
    {
        return m_peventMouseLeaveSource;
    }

    IEventSource* GetMouseEnterWhileEnabledEventSource()
    {
        return m_peventMouseEnterWhileEnabledSource;
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Pane Methods
    //
    //////////////////////////////////////////////////////////////////////////////

    void UpdateLayout()
    {
        DefaultUpdateLayout();
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // IMouseInput Methods
    //
    //////////////////////////////////////////////////////////////////////////////

    void RemoveCapture()
    {
        if (m_pprovider) {
            m_pprovider->GetTimer()->RemoveSink(m_peventSinkDelegate);
            m_pprovider = NULL;
        }   
    }

    void MouseEnter(IInputProvider* pprovider, const Point& point)
    {
        SetInside(true);
        m_peventMouseEnterSource->Trigger();
        if (m_bEnabled)
            m_peventMouseEnterWhileEnabledSource->Trigger();
    }

    void MouseLeave(IInputProvider* pprovider)
    {
        SetInside(false);
        m_peventMouseLeaveSource->Trigger();
    }

    void MouseMove(IInputProvider* pprovider, const Point& point, bool bCaptured, bool bInside)
    {
        SetInside(bInside);

        if (bCaptured) {
            SetDown(bInside);
        }
    }

    MouseResult Button(IInputProvider* pprovider, const Point& point, int button, bool bCaptured, bool bInside, bool bDown)
    {
        if (button == 0 || button == 1) { //imago 7/6/09 added single right mouse button click trigger
			m_button = button;
			if (bDown) {
                if (m_pprovider != NULL && button == 0) { //imago 7/10/09
                    ZAssert(false); // we should not have a timer set before the button is pressed
                    m_pprovider->GetTimer()->RemoveSink(m_peventSinkDelegate);
                }

                if (pprovider->IsDoubleClick()) {
                    m_peventSourceDoubleClick->Trigger();
                } else if (m_bDownTrigger) {
					if (button == 1) {
						m_peventRightSource->Trigger();  //imago 7/6/09
					} else {
						m_peventSource->Trigger();
					}
                } else {
                    SetDown(true);

                    if (m_repeatRate != 0 && m_bEnabled) {
						if (button == 1) {
							m_peventRightSource->Trigger(); //imago 7/6/09
						} else {
							m_peventSource->Trigger();
						}

                        if (m_repeatDelay != 0 && button == 0) {
                            m_bFirstEvent = true;
                            m_pprovider = pprovider;
                            pprovider->GetTimer()->AddSink(m_peventSinkDelegate, m_repeatDelay);
                        } else {
							if (button == 0) {
                            	m_bFirstEvent = false;
                            	m_pprovider = pprovider;
                            	pprovider->GetTimer()->AddSink(m_peventSinkDelegate, m_repeatRate);
							}
                        }
                    }

                    return MouseResultCapture();
                }
				//imago, button not down
            } else {
                if (bCaptured) {
                    bool bWasDown = m_bDown;
                    SetDown(false);

                    if (m_repeatRate == 0) {
                        if (bWasDown && m_bEnabled) {
                            if (m_bToggle) {
                                SetChecked(!m_bChecked);
                            }
							if (button == 1) {
								m_peventRightSource->Trigger();
							} else {
								m_peventSource->Trigger();
							}
                        }
                    } else {
                        ZAssert(m_pprovider == NULL || pprovider == m_pprovider);

                        if (m_pprovider) {
                            m_pprovider->GetTimer()->RemoveSink(m_peventSinkDelegate);
                            m_pprovider = NULL;
                        }
                    }

                    return MouseResultRelease();
                }
            }
        }

        return MouseResult();
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // IEventSink Methods
    //
    //////////////////////////////////////////////////////////////////////////////

    bool OnEvent(IEventSource* pevent)
    {
        if (m_bDown) {
			if (m_button == 1) { //imago 7/6/09
				m_peventRightSource->Trigger();
			} else {
				m_peventSource->Trigger();
			}
        }

        if (m_bFirstEvent) {
            m_bFirstEvent = false;
            m_pprovider->GetTimer()->AddSink(m_peventSinkDelegate, m_repeatRate);
            return false;
        }

        return true;
    }
};

//////////////////////////////////////////////////////////////////////////////
//
// ButtonPane Contructors
//
//////////////////////////////////////////////////////////////////////////////

TRef<ButtonPane> CreateButton(
    ButtonFacePane* ppane,
    bool            bToggle,
    float           repeatRate,
    float           repeatDelay
) {
    return new ButtonUIPane(ppane, repeatRate, repeatDelay, bToggle);
}

TRef<ButtonPane> CreateButton(
	Pane* ppaneUp, 
	Pane* ppaneDown, 
    bool  bToggle,
    float repeatRate,
    float repeatDelay
) {
    return
        CreateButton(
            CreateButtonFacePane(ppaneUp, ppaneDown),
            bToggle,
            repeatRate,
			repeatDelay
        );
}

TRef<ButtonPane> CreateButton(int size)
{
    return
        CreateButton(
            new EdgePane(new Pane(NULL, WinPoint(size, size)), true),
            new EdgePane(new Pane(NULL, WinPoint(size, size)), false)
        );
}

//////////////////////////////////////////////////////////////////////////////
//
// ButtonBar
//
//////////////////////////////////////////////////////////////////////////////

class ButtonBarPaneImpl :
    public ButtonBarPane,
    public IEventSink
{
private:
    TRef<IntegerEventSourceImpl>   m_peventSource;
	TRef<IntegerEventSourceImpl>   m_peventRightSource;
    TRef<IntegerEventSourceImpl>   m_peventMouseEnterWhileEnabledSource;
    TRef<IEventSink>               m_peventSink;
    TMap<TRef<IEventSource>, int>  m_mapEventSources;
    TMap<TRef<IEventSource>, int>  m_mapMouseEnterWhileEnabledSources;
    TMap<int, TRef<ButtonPane> >   m_mapButtonPanes;
    TRef<Pane>                     m_pRowPane;
    int                            m_nCmdSelected;
    bool                           m_bActAsTabs;

public:

    ButtonBarPaneImpl(bool bActAsTabs, bool bUseColumn) :
        m_nCmdSelected(-1),
        m_bActAsTabs(bActAsTabs)
    {
        m_peventSource = new IntegerEventSourceImpl();
		m_peventRightSource = new IntegerEventSourceImpl(); //imago 7/6/09
        m_peventMouseEnterWhileEnabledSource = new IntegerEventSourceImpl();
        m_peventSink = IEventSink::CreateDelegate(this);
        if (!bUseColumn)
            m_pRowPane = new RowPane();
        else
            m_pRowPane = new ColumnPane();
        InsertAtBottom(m_pRowPane);
    }

    ~ButtonBarPaneImpl()
    {
        {
            TMap<TRef<IEventSource>, int>::Iterator iter(m_mapMouseEnterWhileEnabledSources);
            while (!iter.End()) {
                iter.Key()->RemoveSink(m_peventSink);
                iter.Next();
            }
        }

        {
            TMap<TRef<IEventSource>, int>::Iterator iter(m_mapEventSources);
            while (!iter.End()) {
                iter.Key()->RemoveSink(m_peventSink);
                iter.Next();
            }
        }
    }

    bool OnEvent(IEventSource* pevent)
    {
        int nCmd;

        if (m_mapMouseEnterWhileEnabledSources.Find(pevent, nCmd))
        {
            m_peventMouseEnterWhileEnabledSource->Trigger(nCmd);
        }
        else
        {
            ZVerify(m_mapEventSources.Find(pevent, nCmd));

            if (m_bActAsTabs) {
                TRef<ButtonPane> pButtonPane;
                if (m_nCmdSelected != -1) {
                    ZVerify(m_mapButtonPanes.Find(m_nCmdSelected, pButtonPane));
                    pButtonPane->SetChecked(false);
                }
                ZVerify(m_mapButtonPanes.Find(nCmd, pButtonPane));
                pButtonPane->SetChecked(true);
                m_nCmdSelected = nCmd;
            }

            m_peventSource->Trigger(nCmd);
        }

        return true;
    }

    IIntegerEventSource* GetEventSource()
    {
        return m_peventSource;
    }

    IIntegerEventSource* GetRightEventSource()
    {
        return m_peventRightSource;
    }

    IIntegerEventSource* GetMouseEnterWhileEnabledEventSource()
    {
        return m_peventMouseEnterWhileEnabledSource;
    }

    void RemoveAll()
    {
        {
            TMap<TRef<IEventSource>, int>::Iterator iter(m_mapMouseEnterWhileEnabledSources);
            while (!iter.End()) {
                iter.Key()->RemoveSink(m_peventSink);
                iter.Next();
            }
            m_mapMouseEnterWhileEnabledSources.SetEmpty();
        }
        {
            TMap<TRef<IEventSource>, int>::Iterator iter(m_mapEventSources);
            while (!iter.End()) {
                iter.Key()->RemoveSink(m_peventSink);
                iter.Next();
            }
            m_mapEventSources.SetEmpty();
        }
        m_pRowPane->RemoveAllChildren();
    }

    void InsertButton(ButtonPane* pPane, int nCmd)
    {
        ZAssert(nCmd != -1);
        TRef<IEventSource> pEventSource = pPane->GetEventSource();
        m_mapEventSources.Set(pEventSource, nCmd);
        pEventSource->AddSink(m_peventSink);
        
        TRef<IEventSource> pEnterEventSource = pPane->GetMouseEnterWhileEnabledEventSource();
        m_mapMouseEnterWhileEnabledSources.Set(pEnterEventSource, nCmd);
        pEnterEventSource->AddSink(m_peventSink);

        m_mapButtonPanes.Set(nCmd, pPane);
        m_pRowPane->InsertAtBottom(pPane);

        if (m_bActAsTabs && m_mapButtonPanes.Count() == 1) {
            m_nCmdSelected = nCmd;
            pPane->SetChecked(true);
            pPane->SetChecked2(false);
        }
    }

    void SetHidden(int nCmd, bool bHidden)
    {
        TRef<ButtonPane> pButtonPane;
        ZVerify(m_mapButtonPanes.Find(nCmd, pButtonPane));
        pButtonPane->SetHidden(bHidden);
    }

    void SetEnabled(int nCmd, bool bEnabled)
    {
        TRef<ButtonPane> pButtonPane;
        ZVerify(m_mapButtonPanes.Find(nCmd, pButtonPane));
        pButtonPane->SetEnabled(bEnabled);
    }

    void SetChecked(int nCmd, bool bChecked)
    {
        TRef<ButtonPane> pButtonPane;
        ZVerify(m_mapButtonPanes.Find(nCmd, pButtonPane));
        pButtonPane->SetChecked(bChecked);
    }

    void SetChecked2(int nCmd, bool bChecked)
    {
        TRef<ButtonPane> pButtonPane;
        ZVerify(m_mapButtonPanes.Find(nCmd, pButtonPane));
        pButtonPane->SetChecked2(bChecked);
    }

    bool GetEnabled(int nCmd)
    {
        TRef<ButtonPane> pButtonPane;
        ZVerify(m_mapButtonPanes.Find(nCmd, pButtonPane));
        return pButtonPane->GetEnabled();
    }

    bool GetChecked(int nCmd)
    {
        TRef<ButtonPane> pButtonPane;
        ZVerify(m_mapButtonPanes.Find(nCmd, pButtonPane));
        return pButtonPane->GetChecked();
    }

    bool GetChecked2(int nCmd)
    {
        TRef<ButtonPane> pButtonPane;
        ZVerify(m_mapButtonPanes.Find(nCmd, pButtonPane));
        return pButtonPane->GetChecked2();
    }

    void FlashButton(int nCmd)
    {
        // NYI
    }

    int GetSelection()
    {
        return m_nCmdSelected;
    }
    
    void SetSelection(int nCmd)
    {
        if (m_bActAsTabs) {
            TRef<ButtonPane> pButtonPane;
            if (m_nCmdSelected != -1) {
                ZVerify(m_mapButtonPanes.Find(m_nCmdSelected, pButtonPane));
                pButtonPane->SetChecked(false);
                pButtonPane->SetChecked2(false);
            }
            if (nCmd != -1) {
                ZVerify(m_mapButtonPanes.Find(nCmd, pButtonPane));
                pButtonPane->SetChecked(true);
                m_nCmdSelected = nCmd;
            }
            m_nCmdSelected = nCmd;
        }
    }

    void UpdateLayout()
    {
        DefaultUpdateLayout();
    }
};

//////////////////////////////////////////////////////////////////////////////
//
// ButtonBar
//
//////////////////////////////////////////////////////////////////////////////

TRef<ButtonBarPane> CreateButtonBarPane(bool bActAsTabs, bool bUseColumn)
{
    return new ButtonBarPaneImpl(bActAsTabs, bUseColumn);
}

TRef<ButtonBarPane> CreateButtonBarPane(Surface* psurface, DWORD dwFaces, TVector<int>& vecColumns, bool bActAsTabs)
{
    TRef<ButtonBarPane> pbuttonBar = new ButtonBarPaneImpl(bActAsTabs, false);

    //
    // Create all the buttons
    //

    int count = vecColumns.GetCount();
    int xprev = 0;

    for (int index = 0; index < count; index++) {
        int x = vecColumns[index];

        pbuttonBar->InsertButton(
            CreateButton(CreateButtonFacePane(psurface, dwFaces, xprev, x)),
            index
        );

        xprev = x;
    }

    return pbuttonBar;
}
