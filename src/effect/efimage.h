#pragma once

#include <inputengine.h>
#include <image.h>
#include <tref.h>

//////////////////////////////////////////////////////////////////////////////
//
// effect images
//
//////////////////////////////////////////////////////////////////////////////

TRef<Image> CreateBlendColorImage(Image* pimage, ColorValue* pcolor);

//////////////////////////////////////////////////////////////////////////////
//
// JoystickImage
//
//////////////////////////////////////////////////////////////////////////////

class JoystickImage : public Image, public JoystickInputStream {
public:
    virtual void        SetEnabled(bool bJoystickEnabled, bool bButtonsEnabled) = 0;
    virtual void        SetSensitivity(float sensitivity)                       = 0;
    virtual bool        GetJoystickEnabled()                                    = 0;
    virtual bool        GetButtonsEnabled()                                     = 0;
};

TRef<JoystickImage> CreateJoystickImage(float sensitivity);

//////////////////////////////////////////////////////////////////////////////
//
// String Grid Image
//
//////////////////////////////////////////////////////////////////////////////

class StringGridImage : public Image {
public:
    virtual void SetString(int row, int column, const ZString& str) = 0;
    virtual void SetColor(int row, const Color& color)              = 0;
    virtual void SetSize(int rows, int columns)                     = 0;
};

class IEngineFont;
TRef<StringGridImage> CreateStringGridImage(int columns, int rows, IEngineFont* pfont);

//////////////////////////////////////////////////////////////////////////////
//
// MuzzleFlare
//
//////////////////////////////////////////////////////////////////////////////

class MuzzleFlareImage : public Image {
public:
    MuzzleFlareImage(Number* ptime) :
        Image(ptime)
    {
    }

    virtual void AddFlare(const Point& point, float duration) = 0;
    virtual void SetVisible(bool bVisible) = 0;

    //
    // Value methods
    //

    ZString GetFunctionName() { return "MuzzleFlareImage"; }
};

TRef<MuzzleFlareImage> CreateMuzzleFlareImage(Modeler* pmodeler, Number* ptime);

//////////////////////////////////////////////////////////////////////////////
//
// Poster
//
//////////////////////////////////////////////////////////////////////////////
class Viewport;
class PosterImage : public Image {
public:
    PosterImage(Viewport* pviewport) :
        Image(pviewport)
    {
    }

    virtual void AddPoster(Image* pimage, const Vector& vec, float scale) = 0;

    //
    // Value methods
    //

    ZString GetFunctionName() { return "PosterImage"; }
};

TRef<PosterImage> CreatePosterImage(Viewport* pviewport);

//////////////////////////////////////////////////////////////////////////////
//
// LensFlare
//
//////////////////////////////////////////////////////////////////////////////

class LensFlareImage : public Image {
public:
    LensFlareImage(Viewport* pviewport) :
        Image(pviewport)
    {
    }

    virtual void SetLightDirection(const Vector& vec) = 0;

    //
    // Value methods
    //

    ZString GetFunctionName() { return "LensFlareImage"; }
};

TRef<LensFlareImage> CreateLensFlareImage(Modeler* pmodeler, Viewport* pviewport);