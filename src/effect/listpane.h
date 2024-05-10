#pragma once

#include <event.h>
#include <genericlist.h>
#include <pane.h>

typedef TEvent<ItemID> IItemEvent;

//////////////////////////////////////////////////////////////////////////////
//
// Item Painter
//
//////////////////////////////////////////////////////////////////////////////

class ItemPainter : public IObject {
public:
    virtual int  GetXSize()                                                          = 0;
    virtual int  GetYSize()                                                          = 0;
    virtual void Paint(ItemID pitem, Surface* psurface, bool bSelected, bool bFocus) = 0;
};

//////////////////////////////////////////////////////////////////////////////
//
// List Pane
//
//////////////////////////////////////////////////////////////////////////////

class ScrollPane;
class ListPane : 
    public Pane,
    public IKeyboardInput
{
public:
    virtual void   SetList(List* plist)                  = 0;
    virtual void   SetItemPainter(ItemPainter* ppainter) = 0;
    virtual void   SetSelection(ItemID pitem)            = 0;
    virtual ItemID GetSelection()                        = 0;
    virtual int    GetScrollPos()                        = 0;
    virtual void   ScrollToItem(ItemID pitem)            = 0;
    virtual void   ForceRefresh()                        = 0;
    virtual void   SetScrollPos(int pos)                 = 0;
    virtual ScrollPane * GetScrollPane()                 = 0;

    virtual IItemEvent::Source* GetSelectionEventSource()     = 0;
    virtual IEventSource*       GetSingleClickEventSource()   = 0;
	virtual IEventSource*       GetSingleRightClickEventSource()   = 0;
    virtual IEventSource*       GetDoubleClickEventSource()   = 0;
	virtual IEventSource*       GetDoubleRightClickEventSource()   = 0;
};

TRef<ListPane> CreateListPane(
    const WinPoint& sizeMin,
    List*           plist,
    ItemPainter*    ppainter,
    ScrollPane*     pscroll,
    bool            bHorizontal
);

//////////////////////////////////////////////////////////////////////////////
//
// StringList
//
//////////////////////////////////////////////////////////////////////////////

class StringList : 
    public List,
    public ItemPainter
{
public:
    virtual void AddItem(const ZString& str) = 0;
    virtual void SetEmpty()                  = 0;
};

class IEngineFont;
TRef<StringList> CreateStringList(
    IEngineFont* pfont,
    const Color& color,
    const Color& colorSelected,
    int          xsize
);

//////////////////////////////////////////////////////////////////////////////
//
// StringListItem
//
//////////////////////////////////////////////////////////////////////////////

class StringListItem : public IObject {
private:
    TRef<StringListItem> m_pnext;
    ZString          m_str;

public:
    StringListItem(const ZString& str, StringListItem* pnext) :
        m_str(str),
        m_pnext(pnext)
    {
    }

    StringListItem* GetNext()
    {
        return m_pnext;
    }

    const ZString& GetString()
    {
        return m_str;
    }
};

//////////////////////////////////////////////////////////////////////////////
//
// StringListPane
//
//////////////////////////////////////////////////////////////////////////////

class StringListPane : 
    public ListPane
{
public:
    virtual TRef<StringList> GetStringList() = 0;
};


TRef<StringListPane> CreateStringListPane(
    const WinPoint& sizeMin,
    List*           plist,
    ItemPainter*    ppainter,
    ScrollPane*     pscroll,
    bool            bHorizontal,
    IEngineFont*    pfont,
    const Color&    color,
    const Color&    colorSelected
);