/////////////////////////////////////////////////////////////////////////////
//
// Panes
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include <color.h>
#include <input.h>
#include <rect.h>

struct UIVERTEX;
class Surface;

/////////////////////////////////////////////////////////////////////////////
//
// SystemColors
//
/////////////////////////////////////////////////////////////////////////////

const int SystemColor3DHighLight   = 0;
const int SystemColor3DShadow      = 1;
const int SystemColor3DDKShadow    = 2;
const int SystemColorWindowText    = 3;
const int SystemColorHighLightText = 4;
const int SystemColorMax           = 5;

/////////////////////////////////////////////////////////////////////////////
//
// Pane
//
/////////////////////////////////////////////////////////////////////////////

class Pane : public IMouseInput {
    friend class TopPane;

private:
	// Pane attributes
	Pane*      m_pparent;      // parent
	TRef<Pane> m_pchild;       // child
	TRef<Pane> m_pnext;        // sibling under this pane

	bool       m_bHidden;      // is this pane hidden
	int        m_index;        // identifier

	// Added UI vertex storage.
protected:
	WinPoint		m_offset;							// offset from parent's origin
	WinPoint		m_size;								// size of pane
	UIVERTEX *		m_pPaneVerts;						// Vertices used to render a pane.
	DWORD			m_dwNumVerts;						// Number of verts in the m_pPaneVerts array.
	DWORD			m_dwNumPolys;						// Number of polys to render.
	DWORD			m_dwDataSize;						// Should stop on the fly allocation.

	// Colour fill vertices.
//	DWORD					m_dwFillVertsDataSize;	
//	UICOLOURFILLVERTEX *	m_pFillVerts;			// Colour fill vertices if used.

private:
	// Mouse Input
	TRef<Pane> m_ppaneCapture; // child that has capture
	TRef<Pane> m_ppaneHit;     // child that the mouse is over

	//
	// Painting
	//

	WinPoint   m_paintOffset;  // offset at last paint
	WinPoint   m_paintSize;    // size at last paint
	bool       m_bHiddenPaint; // Hidden at last paint?
	bool       m_bSelected;    // is this pane selected
	bool       m_bNeedPaint;   // true if the pane needs to be partially redrawn
	bool       m_bPaintAll;    // true if the pane needs to be completely redrawn
	bool       m_bOpaque;      // is pane compeletely opaque?

	//
	// Layout
	//

	WinPoint   m_expand;        // size pane should try to fill if possible
	bool       m_bXExpandable;  // is the pane expandable
	bool       m_bYExpandable;

	//
	// SystemColors
	//

	static Color s_colors[SystemColorMax];

	//
	// Internal Members
	//

	void TunnelPaint(Surface* psurface, bool bPaintAll);

protected:
	//
	// Called by TopPane
	//

	bool CalcPaint();
	void InternalPaint(Surface* psurface);

	void PaintAll(Surface* psurface);

public:
	//
	// Called by SubClasses
	//
protected:
	void DefaultUpdateLayout();

	virtual void InternalSetOffset(const WinPoint& point);
	void InternalSetExpand(const WinPoint& point);
	void InternalSetSize(const WinPoint& point);
	void InternalSetHidden(bool bHidden);

	static void InternalSetOffset(Pane* ppane, const WinPoint& point)
	{
		ppane->InternalSetOffset(point);
	}

	static void InternalSetExpand(Pane* ppane, const WinPoint& point)
	{
		ppane->InternalSetExpand(point);
	}

	static void InternalSetSize(Pane* ppane, const WinPoint& point)
	{
		ppane->InternalSetSize(point);
	}

	static void InternalSetHidden(Pane* ppane, bool bHidden)
	{
		ppane->InternalSetHidden(bHidden);
	}

	virtual void NeedPaintInternal();
	void NeedPaint();

	//
	// Called by Pane, Overridden by subclasses
	//

	virtual void Paint( Surface * psurface );

	Pane* GetHitPane();
	Pane* GetCapturePane();

public:
    Pane(Pane* pchild = NULL, const WinPoint& size = WinPoint(0, 0));
    ~Pane();

    //
    // Static functions
    //

    static void Initialize();
    static const Color& GetSystemColor(int index);
    static void SetSystemColor(int index, const Color& color);

    virtual void NeedLayout();

    //
    // Queries
    //

    bool     IsAncestor(Pane* ppane) const;
    Pane*    FindChild(int index)    const;
    int      FindChild(Pane* pchild) const;
    int      GetChildCount()         const;

    WinRect         GetRect()    const { return WinRect(m_offset, m_offset + m_size); }
    Pane*           Child()      const { return m_pchild;     }
    Pane*           Next()       const { return m_pnext;      }
    const WinPoint& GetSize()    const { return m_size;       }
    int             XSize()      const { return m_size.X();   }
    int             YSize()      const { return m_size.Y();   }
    const WinPoint& GetOffset()  const { return m_offset;     }
    int             XOffset()    const { return m_offset.X(); }
    int             YOffset()    const { return m_offset.Y(); }
    const WinPoint& GetExpand()  const { return m_expand;     }
    int             XExpand()    const { return m_expand.X(); }
    int             YExpand()    const { return m_expand.Y(); }
    int             GetIndex()   const { return m_index;      }
    bool            IsHidden()   const { return m_bHidden;    }
    bool            IsSelected() const;

    WinPoint GetOffsetFrom(Pane* ppane) const;

    virtual Point TransformLocalToImage(const WinPoint& point);
    virtual int   GetAlignedXSize(int xPos);


    //
    // Set attributes
    //

    void SetHidden(bool bHidden);
    void SetSelected(bool bSelected);
    void SetXExpandable(bool bExpandable);
    void SetYExpandable(bool bExpandable);
    void SetOffset(const WinPoint& point);
    void SetExpand(const WinPoint& point);
    void SetSize(const WinPoint& point);
    void SetIndex(int index);
    void SetOpaque(bool bOpaque) {	m_bOpaque = bOpaque; }

    //
    // Add/Remove children
    //

    void Insert(int index, Pane* ppane);
    void InsertAtBottom(Pane* ppane);
    void InsertAtTop(Pane* ppane);
    void RemoveAllChildren();
    void RemoveChild(Pane* ppane);
    void RemoveChild(int index);
    void RemoveSelf();

    //
    // Members to be overridden by children
    //

    virtual void UpdateLayout();
    virtual bool NeedEvenHeight();

    //
    // IMouseInput methods
    //

    virtual void        RemoveCapture();
    virtual MouseResult HitTest(IInputProvider* pprovider, const Point& point, bool bCaptured);
    virtual void        MouseLeave(IInputProvider* pprovider);
    virtual MouseResult Button(IInputProvider* pprovider, const Point& point, int button, bool bCaptured, bool bInside, bool bDown);
};