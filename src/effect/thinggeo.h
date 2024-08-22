//////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 1996, 1997 Microsoft Corporation. All Rights Reserved.
//
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include <tref.h>
#include <value.h>

//////////////////////////////////////////////////////////////////////////////
//
// ThingGeo
//
//////////////////////////////////////////////////////////////////////////////
class BitsGeo;
class Geo;
class Image;
class Modeler;
class ParticleGeo;
class ThingGeo : public IObject {
protected:
    static float s_lodBias;
    static bool  s_bShowLights;
    static bool  s_bShowHullHits;
    static bool  s_bShowTrails;
    static int   s_iShowSmoke;
    static bool  s_bShowBounds;
    static bool  s_bTransparentObjects;
    static int   s_crashCount;
    static int   s_trashCount;
    static bool  s_bUsePrivateAfterburners;

public:
    static TRef<ThingGeo> Create(Modeler* pmodeler, Number* ptime);

    //
    // Static members
    //

    static void SetShowBounds(bool bBounds);
    static bool GetShowBounds();
    static void SetTransparentObjects(bool b);
    static bool GetTransparentObjects();
    static void SetShowTrails(bool bShowTrails);
    static bool GetShowTrails();
    static void SetShowSmoke(int iShowSmoke);
    static int  GetShowSmoke();
    static void SetShowLights(bool bShowLights);
    static bool GetShowLights();
    static void SetShowHullHits(bool bShowHullHits);
    static bool GetShowHullHits();
    static void SetLODBias(float lodBias);
    static int  GetCrashCount();
    static int  GetTrashCount();
    static void SetPerformance(bool bUse); //imago 8/16/09

    //
    // ThingGeo members
    //

    virtual Geo*    GetGeo()       = 0;
    virtual Geo*    GetTargetGeo() = 0;

    virtual float   GetFlareCount(void) const = 0;
    virtual void    AddFlare(Geo* pgeo, Number* ptime, const Vector& vecDirection, const Vector* ellipseEquation) = 0;
    virtual void    AddHullHit(const Vector& vecPosition, const Vector& vecNormal) = 0;

    virtual void    SetShowDamage (bool bShowDamage) = 0;
    virtual void    AddDamage (const Vector& vecDamagePosition, float fDamageFraction) = 0;
    virtual void    RemoveDamage (float fDamageFraction) = 0;

    virtual void    SetTimeUntilRipcord (float fTimeUntilTeleport) = 0;
    virtual void    SetTimeUntilAleph (float fTimeUntilTeleport) = 0;

    virtual void    SetPosition(const Vector& vec) = 0;
    virtual void    SetOrientation(const Orientation& o) = 0;
    virtual void    SetVisible(bool bVisible) = 0;
    virtual void    SetVisibleShip(bool bVisible) = 0;
    virtual void    SetThrust(float size) = 0;

    virtual void    SetParticleGeo (ParticleGeo* pParticleGeo) = 0;
    virtual void    SetAfterburnerThrust (const Vector& vecThrustDirection, float power) = 0;
    virtual void    SetAfterburnerSmokeSize (float fSize) = 0;
    virtual void    SetAfterburnerFireDuration (float fDuration) = 0;
    virtual void    SetAfterburnerSmokeDuration (float fDuration) = 0;

    virtual void    SetBitsGeo(BitsGeo* pbitsGeo) = 0;
    virtual void    SetTrailColor(const Color& color) = 0;
    virtual void    SetTexture(Image* pimageTexture) = 0;
    virtual void    SetShadeAlways(bool bShadeAlways) = 0;

    virtual float   GetRadius() = 0;
    virtual float   SetRadius(float radius) = 0;

    virtual bool    GetChildModelOffset(const ZString& str, Vector& vec) = 0;
    virtual bool    GetChildOffset(const ZString& str, Vector& vec) = 0;

    virtual HRESULT LoadMDL(short options, INameSpace* pns, Image* pimageTexture) = 0;
    virtual HRESULT Load(short options, Geo* pgeo, Image* pimageTexture) = 0;

    virtual void    SetBoundsGeo(Geo* pgeo) = 0;
};