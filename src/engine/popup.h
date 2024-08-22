/////////////////////////////////////////////////////////////////////////////
//
// Popup
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include <input.h>
#include <rect.h>

#include "value.h"

class Image;
class Engine;
class Pane;

/////////////////////////////////////////////////////////////////////////////
//
// IPopup
//
/////////////////////////////////////////////////////////////////////////////

class IPopupContainer;
class IPopup : public IKeyboardInput {
protected:
    IPopupContainer*        m_pcontainer;
    IPopup*                 m_ppopupOwner;

    IPopup() :
        m_pcontainer(NULL),
        m_ppopupOwner(NULL)
    {
    }

public:
    virtual void        SetContainer(IPopupContainer* pcontainer);
    virtual void        SetOwner(IPopup* ppopupOwner);
    virtual void        OnClose();
    virtual void        ClosePopup(IPopup* ppopup);
    virtual Rect        GetRect();
    virtual TRef<Image> GetImage(Engine* pengine);

    virtual Pane*       GetPane() = 0;
};

TRef<IPopup> CreatePanePopup(Pane* ppane);

/////////////////////////////////////////////////////////////////////////////
//
// IPopupContainer
//
/////////////////////////////////////////////////////////////////////////////

class IPopupContainer : public IKeyboardInput {
public:
    virtual void   OpenPopup(IPopup* ppopup, PointValue* ppoint, bool bCloseAll, IPopup* ppopupOwner)                                         = 0;
    virtual void   OpenPopup(IPopup* ppopup, const Point& point, bool bCloseAll = true, IPopup* ppopupOwner = NULL)                           = 0;
    virtual void   OpenPopup(IPopup* ppopup, const Rect&  rect,  bool bCloseAll = true, bool bCascadeDown = true, IPopup* ppopupOwner = NULL) = 0;
    virtual void   OpenPopup(IPopup* ppopup, bool bCloseAll = true, IPopup* ppopupOwner = NULL)                                               = 0;
    virtual void   ClosePopup(IPopup* ppopup)                                                                                                 = 0;
    virtual bool   IsEmpty()                                                                                                                  = 0;
    virtual Image* GetImage()                                                                                                                 = 0;
};