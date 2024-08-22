/////////////////////////////////////////////////////////////////////////////

//
// Menu
//
/////////////////////////////////////////////////////////////////////////////
#pragma once

#include <tref.h>

#include "popup.h"

class Color;
class IEngineFont;
class Modeler;
class ZString;

/////////////////////////////////////////////////////////////////////////////
//
// IMenuItem
//
/////////////////////////////////////////////////////////////////////////////

class IMenuItem : public IObject {
public:
    virtual int   GetID()                       = 0;
    virtual const ZString& GetString()          = 0;
    virtual void  SetString(const ZString& str) = 0;
    virtual void  SetColors(
        const Color& colorBack,
        const Color& colorText,
        const Color& colorSelectedBack,
        const Color& colorSelectedText
    ) = 0;
    virtual Color GetColor()                = 0;
    virtual void  SetEnabled(bool bEnabled) = 0;
    virtual bool  IsEnabled()               = 0;
    virtual void  SetAccelerator(char ch)   = 0;
};

/////////////////////////////////////////////////////////////////////////////
//
// Sinks
//
/////////////////////////////////////////////////////////////////////////////

class ISubmenuEventSink : public IObject {
public:
    static TRef<ISubmenuEventSink> CreateDelegate(ISubmenuEventSink* psink);

    virtual TRef<IPopup> GetSubMenu(IMenuItem* pitem) = 0;
};

class IMenu;
class IMenuCommandSink : public IObject {
public:
    static TRef<IMenuCommandSink> CreateDelegate(IMenuCommandSink* psink);

    virtual void OnMenuCommand(IMenuItem* pitem) = 0;
    virtual void OnMenuSelect(IMenuItem* pitem) {};
    virtual void OnMenuClose(IMenu* pmenu) {};
};

/////////////////////////////////////////////////////////////////////////////
//
// IMenu
//
/////////////////////////////////////////////////////////////////////////////

class IMenu : public IPopup {
public:
    virtual TRef<IMenuItem> AddMenuItem(
        int                id,
        const ZString&     str,
        char               chAccelerator     = 0,
        ISubmenuEventSink* psubmenuEventSink = NULL
    ) = 0;

    virtual IMenuItem* FindMenuItem(int id) = 0;
    virtual void SetMenuCommandSink(IMenuCommandSink* psink) = 0;
};

TRef<IMenu> CreateMenu(
    Modeler* pmodeler,
    IEngineFont* pfont,
    IMenuCommandSink* psink
);