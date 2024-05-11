#pragma once

//////////////////////////////////////////////////////////////////////////////
//
// SectorViewImage
//
//////////////////////////////////////////////////////////////////////////////

class SectorViewImage : public WrapImage {
protected:
    SectorViewImage() : 
        WrapImage(Image::GetEmpty()) 
    {
    }

public:
    static TRef<SectorViewImage> Create(Engine* pengine);

    virtual void SetWindowSize(const Point& point) = 0;

};