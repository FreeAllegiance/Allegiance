#pragma once

//////////////////////////////////////////////////////////////////////////////
//
// RadarImage header
//
//////////////////////////////////////////////////////////////////////////////
/*
All things
Important things only (Target, selection, guy whacking on me, objective) only.
Powerups only
Stations and Asteroids
Ships
Alephs show up in all modes.
Important things defined as.. Target, selection, Guy whacking on me and Objective. These always show up in  100% brightness in the gutter whilst all other objects show  in 75% brightness.
Peripheral radar mode which shows icons but no ranges for debris, until the item is on my screen, then show ranges.rob - copied from your gloss list in spreadsheet.  is there where u want this?
rob - don't we need some kind of label in hud as to what mode i'm in?
*/
static const float c_flBorderSide = 10.0f;

const char* c_szRadarLODs[];

class RadarImage : public Image {
protected:
    RadarImage(Viewport* pviewport) :
        Image(pviewport)
    {
    }

    void SetViewport(Viewport* pvalue) { SetChild(0, pvalue); }

    Viewport*  GetViewport() { return Viewport::Cast(GetChild(0));  }
    Camera*    GetCamera()   { return GetViewport()->GetCamera();   }
    RectValue* GetViewRect() { return GetViewport()->GetViewRect(); }

    virtual void        Evaluate (void) = 0;

public:
    static TRef<RadarImage> Create(Modeler* pmodeler, Viewport* pviewport);

    enum RadarLOD
    {
        c_rlNone = -1,
        c_rlAll,
        c_rlDefault,
        c_rlTarget,
        c_rlMinimal,
        /*c_rlShips,
        c_rlStations,
        c_rlTreasure,
        c_rlAsteroids,*/  c_rlMax = c_rlMinimal
    };
        
    virtual void        SetRadarLOD(RadarLOD radarLOD) = 0;
    virtual RadarLOD    GetRadarLOD(void) const = 0;

    //
    // Image methods
    //

    virtual void        Render(Context* pcontext) = 0;

    //
    // Value members
    //

            ZString     GetFunctionName() { return "RadarImage"; }
};