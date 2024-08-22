#pragma once

#include <event.h>
#include <pane.h>

//////////////////////////////////////////////////////////////////////////////
//
// Nav Pane
//
//////////////////////////////////////////////////////////////////////////////

class NavPane : public Pane {
public:
    virtual void                     SetTopic(const ZString& str)     = 0;
    virtual const ZString&           GetSecondary(const ZString& str) = 0;
    virtual TRef<IStringEventSource> GetEventSource()                 = 0;
};

class INameSpace;
void AddNavPaneFactory(INameSpace* pns);

//////////////////////////////////////////////////////////////////////////////
//
// Page Pane
//
//////////////////////////////////////////////////////////////////////////////

class ZFile;
class PagePaneIncluder : public IObject {
public:
    virtual TRef<ZFile> Include(const ZString& str) = 0;
};

class IEngineFont;
class PagePane : public Pane {
public:
    virtual void SetAttributes(
        IEngineFont* pfont,
        const Color& color,
        const Color& colorMain,
        const Color& colorSecondary,
        const Color& colorHighlight
    ) = 0;

    virtual void                     SetTopic(INameSpace* pns, const ZString& str)  = 0;
    virtual void                     SetTopicText(INameSpace* pns, const ZString& str) = 0;
    virtual TRef<IStringEventSource> GetMainLinkEventSource()      = 0;
    virtual TRef<IStringEventSource> GetSecondaryLinkEventSource() = 0;
    virtual void                     SetPagePaneIncluder(PagePaneIncluder* ppagePaneIncluder) = 0;
};

class Modeler;
void AddPagePaneFactory(INameSpace* pns, Modeler* pmodeler);

//////////////////////////////////////////////////////////////////////////////
//
// Help
//
//////////////////////////////////////////////////////////////////////////////

class HelpPane : public Pane {
public:
    virtual void          SetTopic(const ZString& str)                                     = 0;
    virtual void          SetString(const ZString& strIdentifier, const ZString& strValue) = 0;
    virtual IEventSource* GetEventSourceClose()                                            = 0;
};

//////////////////////////////////////////////////////////////////////////////
//
// Help
//
//////////////////////////////////////////////////////////////////////////////

TRef<HelpPane> CreateHelpPane(Modeler* pmodeler, const ZString& strTopic, PagePaneIncluder* ppagePaneIncluder);
class IPopup;
TRef<IPopup>   CreateMMLPopup(Modeler* pmodeler, const ZString& strTopic, bool bText);