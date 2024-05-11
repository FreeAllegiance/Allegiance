#pragma once

//////////////////////////////////////////////////////////////////////////////
//
// HUD Images
//
//////////////////////////////////////////////////////////////////////////////

class HUDImage : public WrapImage {
protected:
    HUDImage(Camera* pcamera, RectValue* prectView) :
        WrapImage(Image::GetEmpty(), pcamera, prectView)
    {
    }

    Camera*    GetCamera()   { return Camera::Cast(GetChild(1));    }
    RectValue* GetViewRect() { return RectValue::Cast(GetChild(2)); }

public:
    static TRef<HUDImage> Create(Engine* pengine, Camera* pcamera, RectValue* prectView);

    virtual void Update(Time now) = 0;

    virtual void SetClusterName(const char* value) = 0;
    virtual void SetWindowSize(const Point& point) = 0;
};