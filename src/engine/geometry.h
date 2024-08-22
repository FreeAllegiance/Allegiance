#pragma once

#include <event.h>
#include <matrix.h>

#include "material.h"
#include "model.h"
#include "transform.h"


//////////////////////////////////////////////////////////////////////////////
//
// WrapGeo, a Geo that wraps another geo
//
//////////////////////////////////////////////////////////////////////////////

class WrapGeo : public Geo {
public:
    int CastMember(WrapGeo);
    WrapGeo(Geo* pgeo);
    WrapGeo(Geo* pgeo, Value* pvalue1);

    void SetGeo(Geo* pvalue) { SetChild(0, pvalue); }
    Geo* GetGeo() { return Geo::Cast(GetChild(0)); }

    //
    // WrapGeo members
    //

    virtual TRef<WrapGeo> Duplicate(Geo* pgeo);

    //
    // Geo members
    //

    virtual Image* FindTexture();
    void Render(Context* pcontext);
    float GetRadius(const Matrix& mat = Matrix::GetIdentity());
    void CallGroupGeoCallback(const Matrix& mat, GroupGeoCallback* pcallback);
    TRef<Geo> FoldTexture();
    TRef<Geo> RemoveMaterial();
    TRef<Geo> ApplyTransform(Transform* ptrans);
    int       GetTriangleCount();
    bool      AnyChildGroups(bool bHead);

    void RemoveCapture();
    bool HitTest(const Vector& vecOrigin, const Vector& vecRay, HitData3D& hitData, bool bCaptured);
    void MouseMove(const Vector& vec, bool bCaptured, bool bInside);
    void MouseEnter(const Vector& vec);
    void MouseLeave();
    MouseResult Button(const Vector& vec, int button, bool bCaptured, bool bInside, bool bDown);

    //
    // Value members
    //

    TRef<Value> Fold();
    ZString GetFunctionName() { return "WrapGeo"; }
};

//////////////////////////////////////////////////////////////////////////////
//
// DecalGeo
//
//////////////////////////////////////////////////////////////////////////////

class DecalGeo : public Geo {
private:
    Color  m_color;
    Vector m_vecPosition;
    Vector m_vecForward;
    Vector m_vecRight;
    float  m_scale;
    float  m_angle;

public:
    DecalGeo(
        Image* pimage,
        const Color& color,
        const Vector& position,
        const Vector& forward,
        const Vector& right,
        float scale,
        float angle
    );

    void SetImage(Image* pvalue) { SetChild(0, pvalue); }
    Image* GetImage() { return Image::Cast(GetChild(0)); }

    float GetAngle()
    {
        return m_angle;
    }

    void SetAngle(float angle)
    {
        m_angle = angle;
    }

	const Color& GetColor()
	{
		return m_color;
	}

	void SetColor(const Color& color)
	{
		m_color = color;
	}

    const Vector& GetPosition()
    {
        return m_vecPosition;
    }

    void SetPosition(const Vector& vec)
    {
        m_vecPosition = vec;
    }

    void SetForward(const Vector& vec)
    {
        m_vecForward = vec;
    }

    const Vector& GetForward()
    {
        return m_vecForward;
    }

    void SetRight(const Vector& vec)
    {
        m_vecRight = vec;
    }

    const Vector& GetRight()
    {
        return m_vecRight;
    }

    float GetScale()
    {
        return m_scale;
    }

    void SetScale(float scale)
    {
        m_scale = scale;
    }

    void Render(Context* pcontext);

    int GetTriangleCount()
    {
        return 2;
    }

    //
    // Value members
    //

    ZString GetFunctionName() { return "DecalGeo"; }
};

//////////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////////

class VisibleGeo : public WrapGeo {
private:
    bool m_bVisible;

public:
    DefineCastMember(VisibleGeo);
    VisibleGeo(Geo* pgeo);

    void SetVisible(bool bVisible);
    bool IsVisible()
    {
        return m_bVisible;
    }

    //
    // WrapGeo members
    //

    TRef<WrapGeo> Duplicate(Geo* pgeo);

    //
    // Geo members
    //

    void Render(Context* pcontext);

    //
    // Value members
    //

    ZString GetFunctionName() { return "VisibleGeo"; }
};

//////////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////////

class ClipGeo : public WrapGeo {
private:
    bool m_bClip;

public:
    DefineCastMember(ClipGeo);
    ClipGeo(Geo* pgeo);

    bool GetClip()
    {
        return m_bClip;
    }

    void SetClip(bool bClip);
    bool IsClip()
    {
        return m_bClip;
    }

    //
    // WrapGeo members
    //

    TRef<WrapGeo> Duplicate(Geo* pgeo);

    //
    // Geo members
    //

    void Render(Context* pcontext);

    //
    // Value members
    //

    ZString GetFunctionName() { return "ClipGeo"; }
};

//////////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////////

class ClipPlaneGeo : public WrapGeo {
private:
    Plane m_plane;

public:
    DefineCastMember(ClipPlaneGeo);
    ClipPlaneGeo(Geo* pgeo, const Plane& plane);

    void SetClipPlane(const Plane& plane);

    //
    // WrapGeo members
    //

    virtual TRef<WrapGeo> Duplicate(Geo* pgeo);

    //
    // Geo members
    //

    void Render(Context* pcontext);

    //
    // Value members
    //

    ZString GetFunctionName() { return "ClipPlaneGeo"; }
};

//////////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////////

class TransformGeo : public WrapGeo {
public:
    TransformGeo(Geo* pgeo, Transform* ptrans);

    void SetTransform(Transform* pvalue) { SetChild(1, pvalue); }
    Transform* GetTransform() { return Transform::Cast(GetChild(1)); }

    //
    // WrapGeo members
    //

    virtual TRef<WrapGeo> Duplicate(Geo* pgeo);

    //
    // Geo members
    //

    void Render(Context* pcontext);
    float GetRadius(const Matrix& mat = Matrix::GetIdentity());
    void CallGroupGeoCallback(const Matrix& mat, GroupGeoCallback* pcallback);

    //
    // Value members
    //

    TRef<Value> Fold();
    ZString GetFunctionName() { return "TransformGeo"; }
};

//////////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////////

class MaterialGeo : public WrapGeo {
private:
    TRef<Material> m_pmaterial;
    bool           m_bOverride;

public:
    MaterialGeo(Geo* pgeo, Material* pmaterial, bool bOverride = false);

    void SetMaterial(Material* pvalue) { SetChild(1, pvalue); }
    Material* GetMaterial() { return Material::Cast(GetChild(1)); }

    //
    // WrapGeo members
    //

    virtual TRef<WrapGeo> Duplicate(Geo* pgeo);

    //
    // Geo members
    //

    void Render(Context* pcontext);
    TRef<Geo> RemoveMaterial();

    //
    // Value members
    //

    ZString GetFunctionName() { return "MaterialGeo"; }
};

//////////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////////

class CullModeGeo : public WrapGeo {
private:
    CullMode m_mode;
    bool     m_bOveride;

public:
    CullModeGeo(Geo* pgeo, CullMode mode, bool bOveride = false) :
        WrapGeo(pgeo),
        m_mode(mode),
        m_bOveride(bOveride)
    {
    }

    void SetCullMode(CullMode mode) { m_mode = mode; }
    CullMode GetCullMode() { return m_mode; }

    //
    // WrapGeo members
    //

    TRef<WrapGeo> Duplicate(Geo* pgeo)
    {
        return new CullModeGeo(pgeo, m_mode, m_bOveride);
    }

    //
    // Geo members
    //

    void Render(Context* pcontext)
    {
        pcontext->SetCullMode(m_mode, m_bOveride);
        GetGeo()->Render(pcontext);
    }

    //
    // Value members
    //

    ZString GetFunctionName() { return "CullModeGeo"; }
};

//////////////////////////////////////////////////////////////////////////////
//
// 
//
//////////////////////////////////////////////////////////////////////////////

class TextureGeo : public WrapGeo {
private:
    bool m_bOverride;

public:
    TextureGeo(Geo* pgeo, Image* pimageTexture, bool bOverride = false);

    void SetTexture(Image* pvalue) { SetChild(1, pvalue); }
    Image* GetTexture() { return Image::Cast(GetChild(1)); }

    //
    // WrapGeo members
    //

    TRef<WrapGeo> Duplicate(Geo* pgeo);
    TRef<Geo>     FoldTexture();

    //
    // Geo members
    //

    virtual Image* FindTexture();
    void Render(Context* pcontext);
    TextureGeo* GetTextureGeo();

    //
    // Value members
    //

    ZString GetFunctionName() { return "TextureGeo"; }
};

//////////////////////////////////////////////////////////////////////////////
//
// Timed Geo
//
//////////////////////////////////////////////////////////////////////////////

class TimedGeo : public WrapGeo, public EventSourceImpl {
private:
    float m_lifespan;

    void Evaluate();

    Number* GetTime() { return Number::Cast(GetChild(1)); }

public:
    TimedGeo(Geo* pgeo, Number* ptime, float lifespan);

    //
    // WrapGeo members
    //

    virtual TRef<WrapGeo> Duplicate(Geo* pgeo);
};

//////////////////////////////////////////////////////////////////////////////
//
// Group Geo
//
//////////////////////////////////////////////////////////////////////////////

class GroupGeo : public Geo {
protected:
    GroupGeo() : Geo() {}
    GroupGeo(Value* pvalue0) : Geo(pvalue0) {}
    GroupGeo(Value* pvalue0, Value* pvalue1) : Geo(pvalue0, pvalue1) {}

public:
    static TRef<GroupGeo> Create();
    static TRef<GroupGeo> Create(Geo* pgeo);
    static TRef<GroupGeo> Create(Geo* pgeo, Geo* pgeo2);
    static TRef<GroupGeo> Create(Geo* pgeo, Geo* pgeo2, Geo* pgeo3);

    virtual void           SetName(const ZString& strName) = 0;
    virtual const ZString& GetName()                       = 0;
    virtual void           AddGeo(Geo* pgeo)               = 0;
    virtual void           RemoveGeo(Geo* pgeo)            = 0;
    virtual ValueList*     GetList()                       = 0;

    //
    // Value members
    //

    ZString GetFunctionName() { return "GroupGeo"; }
};

//////////////////////////////////////////////////////////////////////////////
//
// LODGeo
//
//////////////////////////////////////////////////////////////////////////////

class LODGeo : public WrapGeo {
protected:
    LODGeo(Geo* pgeo) : WrapGeo(pgeo) {}

public:
    static TRef<LODGeo> Create(Geo* pgeo);

    //
    // LODGeo members
    //

    virtual void AddGeo(Geo* pgeo, float radius) = 0;
    virtual ZString GetInfo() = 0;

    //
    // Value members
    //

    ZString GetFunctionName() { return "LODGeo"; }
};

//////////////////////////////////////////////////////////////////////////////
//
// Callback Geo
//
//////////////////////////////////////////////////////////////////////////////

class CallbackGeo : public WrapGeo, IGeoCallback {
public:
    CallbackGeo(Geo* pgeo);

    //
    // WrapGeo members
    //

    virtual TRef<WrapGeo> Duplicate(Geo* pgeo);

    //
    // Geo members
    //

    void Render(Context* pcontext);
    void RenderCallback(Context* pcontext);
};

//////////////////////////////////////////////////////////////////////////////
//
// Blend Add Callback Geo
//
//////////////////////////////////////////////////////////////////////////////

TRef<Geo> CreateBlendGeo(Geo* pgeo, BlendMode blendMode);