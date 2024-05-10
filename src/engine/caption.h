#pragma once

#include <tref.h>

class Modeler;
class Pane;

//////////////////////////////////////////////////////////////////////////////
//
// CaptionSite
//
//////////////////////////////////////////////////////////////////////////////

class ICaptionSite : public IObject {
public:
    virtual void OnCaptionMinimize()   = 0;
    virtual void OnCaptionMaximize()   = 0;
    virtual void OnCaptionFullscreen() = 0;
    virtual void OnCaptionRestore()    = 0;
    virtual void OnCaptionClose()      = 0;
};

//////////////////////////////////////////////////////////////////////////////
//
// Caption
//
//////////////////////////////////////////////////////////////////////////////

class ICaption : public IObject {
public:
    virtual void SetFullscreen(bool bFullscreen) = 0;
};

TRef<ICaption> CreateCaption(Modeler* pmodeler, Pane* ppane, ICaptionSite* psite);