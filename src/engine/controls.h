/////////////////////////////////////////////////////////////////////////////
//
// Pane Controls
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include <rect.h>
#include <event.h>

#include "engine.h"
#include "model.h"
#include "pane.h"

class IEngineFont;

/////////////////////////////////////////////////////////////////////////////
//
// Justify Pane
//
/////////////////////////////////////////////////////////////////////////////

class JustifyPane : public Pane {
public:
    enum {
        Center = 0,
        Left   = 1,
        Right  = 2,
        Top    = 4,
        Bottom = 8
    };

protected:
    int m_layout;
    WinPoint m_bigSize;

public:
    JustifyPane(int layout = Center, const WinPoint& size = WinPoint(0, 0), Pane* pchild = NULL);

    virtual void UpdateLayout();
};

/////////////////////////////////////////////////////////////////////////////
//
// Fill Pane
//
/////////////////////////////////////////////////////////////////////////////

class FillPane : public Pane {
protected:
    WinPoint m_sizeMin;
    Color    m_color;

public:
    FillPane(const WinPoint& sizeMin, const Color& color);

    void SetColor(const Color& color);

    virtual void UpdateLayout();
    virtual void Paint(Surface* psurface);
};

/////////////////////////////////////////////////////////////////////////////
//
// Border Pane
//
/////////////////////////////////////////////////////////////////////////////

class BorderPane : public Pane {
protected:
    int   m_xborder;
    int   m_yborder;
    Color m_color;
    Color m_colorSelected;

public:
    BorderPane(int width, const Color& color, Pane* pchild = NULL);
    BorderPane(int xborder, int yborder, const Color& color, Pane* pchild = NULL);

    void SetColor(const Color& color);
    void SetColorSelected(const Color& color);

    virtual void UpdateLayout();
    virtual void Paint(Surface* psurface);
};

/////////////////////////////////////////////////////////////////////////////
//
// Edge Pane
//
/////////////////////////////////////////////////////////////////////////////

class EdgePane : public Pane {
protected:
    bool m_bUp;

public:
    EdgePane(Pane* pchild = NULL, bool bUp = true);

    virtual void UpdateLayout();
    virtual void Paint(Surface* psurface);
};

/////////////////////////////////////////////////////////////////////////////
//
// Column Pane
//
/////////////////////////////////////////////////////////////////////////////

class ColumnPane : public Pane {
protected:
    bool m_bShowGrid;

public:
    ColumnPane(bool bShowGrid = FALSE);
    ColumnPane(Pane* ppane0, bool bShowGrid = FALSE);
    ColumnPane(Pane* ppane0, Pane* ppane1, bool bShowGrid = FALSE);
    ColumnPane(Pane* ppane0, Pane* ppane1, Pane* ppane2, bool bShowGrid = FALSE);

    void ShowGrid(bool bShowGrid);

    virtual void Paint(Surface* psurface);
    virtual void UpdateLayout();
};

/////////////////////////////////////////////////////////////////////////////
//
// Row Pane
//
/////////////////////////////////////////////////////////////////////////////

class RowPane : public Pane {
protected:
    bool m_bShowGrid;

public:
    RowPane(bool bShowGrid = FALSE);
    RowPane(Pane* ppane0, bool bShowGrid = FALSE);
    RowPane(Pane* ppane0, Pane* ppane1, bool bShowGrid = FALSE);

    void ShowGrid(bool bShowGrid);

    virtual void Paint(Surface* psurface);
    virtual void UpdateLayout();
};

/////////////////////////////////////////////////////////////////////////////
//
// String Pane
//
/////////////////////////////////////////////////////////////////////////////

class StringPane : public Pane {
protected:
    ZString m_str;
    Color m_textColor;
    Color m_textColorSelected;
    IEngineFont* m_pfont;
    bool m_bClipped;
    WinPoint m_ptSize;
    Justification m_justification;

    virtual void UpdateLayout();
    virtual void Paint(Surface* psurface);

public:
    StringPane(const ZString& str, IEngineFont* pfont);
    StringPane(
        const ZString& str, 
        IEngineFont*   pfont, 
        WinPoint       ptSize, 
        Justification  justification = JustifyLeft()
    );

    // StringPane methods

    const ZString& GetString() { return m_str; }
    IEngineFont* GetFont() { return m_pfont; };

    void SetTextColor(const Color& color);
    void SetTextColorSelected(const Color& color);
    void SetFont(IEngineFont* pfont);
    void SetJustification(Justification justification);

    virtual void SetString(const ZString& str);
};

/////////////////////////////////////////////////////////////////////////////
//
// EditPane
//
/////////////////////////////////////////////////////////////////////////////

class EditPane :
    public Pane,
    public IKeyboardInput
{
public:
    enum EditPaneType {
        Normal,
        Password,
        Numeric
    };

    virtual TRef<IEventSource>       GetClickEvent()                    = 0;
    virtual TRef<IStringEventSource> GetChangeEvent()                   = 0;
    virtual const ZString&           GetString()                        = 0;
    virtual bool                     IsReadOnly()                       = 0;
    virtual void                     SetTextColor(const Color& color)   = 0;
    virtual void                     SetReadOnly(bool bReadOnly = true) = 0;
    virtual void                     SetFont(IEngineFont* pfont)        = 0;
    virtual void                     SetString(const ZString& str)      = 0;
    virtual void                     SetType(EditPaneType type)         = 0;
    virtual void                     SetMaxLength(int length)           = 0;
};

TRef<EditPane> CreateEditPane(const ZString& str, IEngineFont* pfont, Number* ptime);

/////////////////////////////////////////////////////////////////////////////
//
// Image Pane
//
/////////////////////////////////////////////////////////////////////////////

class ImagePane : public Pane, public Value {
private:
    void UpdateLayout();
    void Paint(Surface* psurface);

    //
    // Value members
    //

    void ChildChanged(Value* pvalue, Value* pvalueNew);

public:
    ImagePane(Image* pimage);

    Image* GetImage() { return Image::Cast(GetChild(0)); }
    void   SetImage(Image* pimage) { SetChild(0, pimage); }
};

/////////////////////////////////////////////////////////////////////////////
//
// Image Pane
//
/////////////////////////////////////////////////////////////////////////////

class AnimatedImagePane : public Pane, public Value {
private:
    bool  m_bFixedRect;
    Rect  m_rect;
    float m_ysizeSurface;

    //
    // Implementation methods
    //

    Point GetImagePoint(const Point& point);

    //
    // Pane methods
    //

    void UpdateLayout();
    void Paint(Surface* psurface);

    //
    // Value methods
    //

    void ChildChanged(Value* pvalue, Value* pvalueNew);

    //
    // IMouseInput methods
    //

    MouseResult HitTest(IInputProvider* pprovider, const Point& point, bool bCaptured);
    void        MouseMove(IInputProvider* pprovider, const Point& point, bool bCaptured, bool bInside);
    void        MouseEnter(IInputProvider* pprovider, const Point& point);
    void        MouseLeave(IInputProvider* pprovider);
    MouseResult Button(IInputProvider* pprovider, const Point& point, int button, bool bCaptured, bool bInside, bool bDown);

public:
    AnimatedImagePane(Image* pimage, const Rect& rect);
    AnimatedImagePane(Image* pimage);

    Image* GetImage() { return Image::Cast(GetChild(0)); }
    void   SetImage(Image* pimage) { SetChild(0, pimage); }
};

/////////////////////////////////////////////////////////////////////////////
//
// Surface Pane
//
/////////////////////////////////////////////////////////////////////////////

class SurfacePane : public Pane {
private:
    TRef<Surface> m_psurface;

    void Paint(Surface* psurface);

public:
    SurfacePane(Surface* psurface);
    void    SetSurface(Surface* psurface);
};

/////////////////////////////////////////////////////////////////////////////
//
// Scroll Pane
//
/////////////////////////////////////////////////////////////////////////////

class ScrollPane : public Pane {
public:
    virtual IIntegerEventSource* GetEventSource() = 0;
    virtual int  GetPos() = 0;

    virtual int  GetSize() = 0;    
    virtual int  GetPageSize() = 0;
    virtual int  GetLineSize() = 0;
    virtual bool IsHorizontal() = 0;

    virtual void SetSizes(int size, int sizePage) = 0;
    virtual void SetSize(int size) = 0;
    virtual void SetPageSize(int sizePage) = 0;
    virtual void SetLineSize(int sizeLine) = 0;

    virtual void SetPos(int pos) = 0;
    virtual void PageUp() = 0;
    virtual void PageDown() = 0;
    virtual void LineUp() = 0;
    virtual void LineDown() = 0;
};

TRef<ScrollPane> CreateScrollPane(
    Pane* ppaneBackground,
    Pane* ppaneThumb,
    Pane* ppaneLeft,
    IEventSource* peventLeft,
    Pane* ppaneRight,
    IEventSource* peventRight,
    bool bHorizontal,
    float repeatRatePage,
    int size,
    int sizePage,
    int sizeLine,
    int pos,
    const WinPoint sizeMin = WinPoint(0, 0)
);

TRef<Pane> CreateScrollPane(const WinPoint& point,
    int size,
    int pageSize,
    int lineSize,
    int pos,
    TRef<ScrollPane> &pscrollPane,
    TRef<IIntegerEventSource> &pevent
);