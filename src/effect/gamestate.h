#pragma once

#include <tref.h>

//////////////////////////////////////////////////////////////////////////////
//
// Game State Container
//
//////////////////////////////////////////////////////////////////////////////
class Image;
class IEngineFont;
class StringPane;
class ZString;
class Pane;
class IEventSource;
class Modeler;
class ButtonPane;
class GameStateContainer : public IObject {
public:
    virtual TRef<Image>       GetImage()                                             = 0;
    virtual TRef<IEngineFont> GetFont()                                              = 0;
    virtual TRef<StringPane>  AddGameStateTile(const ZString& strTitle, Pane* ppane) = 0;
    virtual void              SetTimeRemaining(int seconds)                          = 0;
    virtual void              SetTimeElapsed(int seconds)                            = 0;
    virtual IEventSource*     GetCloseEvent()                                        = 0; 
};

TRef<GameStateContainer> CreateGameStateContainer(
    Modeler*     pmodeler,
    IEngineFont* pfontTitles,
    IEngineFont* pfontTime,
    ButtonPane*  pbutton
);