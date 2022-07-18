#ifndef _Image_h_
#define _Image_h_

#include <tref.h>

#include "engine.h"
#include "model.h"
#include "transform.h"
#include "viewport.h"

//////////////////////////////////////////////////////////////////////////////
//
//  Other Images
//
//////////////////////////////////////////////////////////////////////////////

TRef<Image> CreateGeoImage2D(Geo* pgeo);
TRef<Image> CreateClipImage(Image* pimage, RectValue* prect);
TRef<Image> CreateColorImage(ColorValue* pcolor);
TRef<Image> CreateExtentImage(RectValue* prect, ColorValue* pcolor);
TRef<Image> CreateConstantImage3D(Surface* psurface, ColorValue* pcolor);
TRef<Image> CreateBlendImage(Image* pimage, BlendMode blendMode);
TRef<Image> CreateSetColorImage(Image* pimage, ColorValue* pcolor);
TRef<Image> CreateGaugeImage(Justification justification, Image* pimage, bool b3D, Number* pnumber);
TRef<Image> CreateGaugeImage(Justification justification, Image* pimage, const Rect& rect, bool b3D, Number* pnumber);
TRef<Image> CreateVisibleImage(Image* pimage, Boolean* bShow);
TRef<Image> CreateJustifyImage(Image* pimage, Justification justification);
TRef<Image> CreateUndetectableImage(Image* pimage);
TRef<Image> CreateTransparentImage();

//////////////////////////////////////////////////////////////////////////////
//
// Image Implementations
//
//////////////////////////////////////////////////////////////////////////////

class VideoImage : public Image {
protected:
    VideoImage(RectValue* prect) :
        Image(prect)
    {
    }
public:
    virtual IEventSource* GetEventSource() = 0;
};

//TRef<VideoImage> CreateVideoImage(Engine* pengine, RectValue* prect, const ZString& str);

//////////////////////////////////////////////////////////////////////////////
//
// Image Implementations
//
//////////////////////////////////////////////////////////////////////////////

class ConstantImage : public Image {
private:
    TRef<Surface> m_psurface;
    ZString       m_str;

public:
    ConstantImage(Surface* psurface, const ZString& str);

    //
    // Image methods
    //

    const Bounds2&      GetBounds();
          void          CalcBounds();
          void          Render(Context* pcontext);
          TRef<Surface> GetSurface();

    //
    // Value methods
    //

    ZString GetFunctionName();
    ZString GetString(int indent);

    //
    // IMouseInput methods
    //

    MouseResult HitTest(IInputProvider* pprovider, const Point& point, bool bCaptured);
};

//////////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////////

class AnimatedImage : public Image {
private:
    TVector<TRef<Surface> > m_psurfaces;
    int  m_index;

    Number* GetTime() { return Number::Cast(GetChild(0)); }

    void Evaluate();

public:
    AnimatedImage(Number* ptime, AnimatedImage* pimage);
    AnimatedImage(Number* ptime, Surface* psurface, int nRows = 0, int nCols = 0);

    int GetCount()
    {
        return m_psurfaces.GetCount();
    }

    TRef<Surface> GetSurface(int index)
    {
        return m_psurfaces[index];
    }

    //
    // Image methods
    //

    TRef<Surface> GetSurface();

    //
    // Value members
    //

    ZString GetFunctionName() { return "AnimatedImage"; }

    //
    // IMouseInput methods
    //

    MouseResult HitTest(IInputProvider* pprovider, const Point& point, bool bCaptured);
};

//////////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////////

class WrapImage : public Image {
private:
    bool    m_bHasCapture;

public:
    int CastMember(WrapImage);

    WrapImage(Image* pimage) : Image(pimage), m_bHasCapture(false)
    {
    }

    WrapImage(Image* pimage, Value* pvalue1) : 
        Image(pimage, pvalue1), m_bHasCapture(false)
    {
    }

    WrapImage(Image* pimage, Value* pvalue1, Value* pvalue2) : 
        Image(pimage, pvalue1, pvalue2), m_bHasCapture(false)
    {
    }

    void SetImage(Image* pvalue);
    Image* GetImage() 
	{
		// BT - 10/17 - If there are no children, then this was causing a crash.
		if (GetChildCount() > 0)
			return Image::Cast(GetChild(0));
		else
			return nullptr;
	}

    //
    // Image methods
    //

    void CalcBounds();
    void Render(Context* pcontext);
    TRef<Surface> GetSurface();

    //
    // Value members
    //

    ZString GetFunctionName() { return "WrapImage"; }

    //
    // IMouseInput methods
    //

    void RemoveCapture();
    MouseResult HitTest(IInputProvider* pprovider, const Point& point, bool bCaptured);
    void MouseMove(IInputProvider* pprovider, const Point& point, bool bCaptured, bool bInside);
    void MouseEnter(IInputProvider* pprovider, const Point& point);
    void MouseLeave(IInputProvider* pprovider);
    MouseResult Button(IInputProvider* pprovider, const Point& point, int button, bool bCaptured, bool bInside, bool bDown);
};

//////////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////////

TRef<Image> CreateStringImage(
    Justification justification,
    IEngineFont*  pfont,
    ColorValue*   pcolor,
    int           width,
    StringValue*  pstring,
    int           indent = 0
);

TRef<Image> CreateStringImage(
    Justification justification,
    IEngineFont*  pfont,
    ColorValue*   pcolor,
    Number*       pwidth,
    StringValue*  pstring,
    int           indent = 0
);

//////////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////////

class SwitchImage : public WrapImage {
protected:
    SwitchImage(Image* pimage, Number* pnumber) :
        WrapImage(pimage, pnumber)
    {
    }

public:
    virtual void AddImage(float, Image* pimage) = 0;
};

TRef<SwitchImage> CreateSwitchImage(Number* pnumber, Image* pimageDefault);

//////////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////////

class TransformImage : public WrapImage {
protected:
    Point TransformPoint(const Point& point);

public:
    TransformImage(Image* pimage, Transform2* ptrans);

    void        SetTransform2(Transform2* pvalue) { SetChild(1, pvalue);                  }
    Transform2* GetTransform2()                   { return Transform2::Cast(GetChild(1)); }

    //
    // Image members
    //

    void CalcBounds();
    void Render(Context* pcontext);

    //
    // Value members
    //

    ZString GetFunctionName() { return "TransformImage"; }

    //
    // IMouseInput methods
    //

    MouseResult HitTest(IInputProvider* pprovider, const Point& point, bool bCaptured);
    void MouseMove(IInputProvider* pprovider, const Point& point, bool bCaptured, bool bInside);
    void MouseEnter(IInputProvider* pprovider, const Point& point);
    MouseResult Button(IInputProvider* pprovider, const Point& point, int button, bool bCaptured, bool bInside, bool bDown);
};

//////////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////////

class TranslateImage : public WrapImage {
protected:
    Point TransformPoint(const Point& point);

    Point m_point;

public:
    TranslateImage(Image* pimage, const Point& point);

    void SetTranslation(const Point& point) { m_point = point; }

    //
    // Image members
    //

    void CalcBounds();
    void Render(Context* pcontext);

    //
    // Value members
    //

    ZString GetFunctionName() { return "TranslateImage"; }

    //
    // IMouseInput methods
    //

    MouseResult HitTest(IInputProvider* pprovider, const Point& point, bool bCaptured);
    void MouseMove(IInputProvider* pprovider, const Point& point, bool bCaptured, bool bInside);
    void MouseEnter(IInputProvider* pprovider, const Point& point);
    MouseResult Button(IInputProvider* pprovider, const Point& point, int button, bool bCaptured, bool bInside, bool bDown);
};

//////////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////////

class GroupImage : public Image, private ValueList::Site {
private:
    TRef<Image> m_pimageHit;
    TRef<Image> m_pimageCapture;

    ValueList* GetValueList() { return ValueList::Cast(GetChild(0)); }

    bool RemoveValue(Value* pvalue);

public:
    GroupImage();
    GroupImage(Image* pimage1);
    GroupImage(Image* pimage1, Image* pimage2);
    GroupImage(Image* pimage1, Image* pimage2, Image* pimage3);
    GroupImage(Image* pimage1, Image* pimage2, Image* pimage3, Image* pimage4);

    void AddImage(Image* pimage);
    void AddImageToTop(Image* pimage);
    void RemoveImage(Image* pimage);

    //
    // Image Methods
    //

    void CalcBounds();
    void Render(Context* pcontext);

    //
    // Value members
    //

    ZString GetFunctionName() { return "GroupImage"; }

    //
    // IMouseInput methods
    //

    void RemoveCapture();
    void MouseLeave(IInputProvider* pprovider);
    MouseResult HitTest(IInputProvider* pprovider, const Point& point, bool bCaptured);
    MouseResult Button(IInputProvider* pprovider, const Point& point, int button, bool bCaptured, bool bInside, bool bDown);
};

//////////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////////

/*
class ClipImage : public WrapImage {
public:
    ClipImage(Image* pimage, RectValue* prect);

    void       SetClipRect(RectValue* pvalue) { SetChild(1, pvalue);                  }
    RectValue* GetClipRect()                  { return Transform2::Cast(GetChild(1)); }

    //
    // Image members
    //

    void CalcBounds();
    void Render(Context* pcontext);

    //
    // Value members
    //

    ZString GetFunctionName() { return "ClipImage"; }
};
*/

//////////////////////////////////////////////////////////////////////////////
//
// 
//
//////////////////////////////////////////////////////////////////////////////

class GeoImage : public Image {
private:
    TRef<Geo> m_pgeoCapture;
    TRef<Geo> m_pgeoHit;
    Vector    m_vecLight;
    bool      m_bZBuffer;
    bool      m_bBidirectional;
    Color     m_color;
    Color     m_colorAlt;
    float     m_ambientLevel;

public:
    GeoImage(Geo* pgeo, Viewport* pviewport, bool bZBuffer);

    void SetGeo(Geo* pvalue) { SetChild(0, pvalue); }
    Geo* GetGeo() { return Geo::Cast(GetChild(0)); }

    void SetViewport(Viewport* pvalue) { SetChild(1, pvalue); }

    Viewport*  GetViewport() { return Viewport::Cast(GetChild(1));  }
    Camera*    GetCamera()   { return GetViewport()->GetCamera();   }
    RectValue* GetViewRect() { return GetViewport()->GetViewRect(); }

    void SetLightDirection(const Vector& vector);
    void SetLight(const Color& color);
    void SetLight(const Color& color, const Color& colorAlt);
    void SetAmbientLevel(float level);

    //
    // Image methods
    //

    void CalcBounds();
    void Render(Context* pcontext);

    //
    // Value members
    //

    ZString GetFunctionName() { return "GeoImage"; }

    //
    // IMouseInput methods
    //

    void RemoveCapture();
    MouseResult HitTest(IInputProvider* pprovider, const Point& point, bool bCaptured);
    MouseResult Button(IInputProvider* pprovider, const Point& point, int button, bool bCaptured, bool bInside, bool bDown);
};

//////////////////////////////////////////////////////////////////////////////
//
// Image Implementations
//
//////////////////////////////////////////////////////////////////////////////

class MDLFileImage : public WrapImage {
protected:
    MDLFileImage() :
        WrapImage(Image::GetEmpty())
    {
    }

public:
    virtual void Load(const ZString& str) = 0;
    virtual void Reload()                 = 0;
};

TRef<MDLFileImage> CreateMDLFileImage(Modeler* pmodeler, const ZString& str);

//////////////////////////////////////////////////////////////////////////////
//
// Image Implementations
//
//////////////////////////////////////////////////////////////////////////////

class PickImage : public WrapImage {
protected:
    PickImage(Image* pimage) :
        WrapImage(pimage)
    {
    }

public:
    virtual IEventSource* GetEventSource() = 0;
};

TRef<PickImage> CreatePickImage(Image* pimage);

#endif
