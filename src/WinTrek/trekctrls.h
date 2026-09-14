/*==========================================================================
 *
 *  Copyright (C) 1997,1998 Microsoft Corporation. All Rights Reserved.
 *
 *  File:  TrekCtrls.h
 *
 *  Author: 
 *
 ***************************************************************************/

#ifndef _TREKCTRLS_H
#define _TREKCTRLS_H


/////////////////////////////////////////////////////////////////////////////
//
// SubImagePane
//
/////////////////////////////////////////////////////////////////////////////

class SubImagePane : public Pane 
{
public:
    static TRef<SubImagePane> Create(Image* pimage, const WinRect& rect);
};

/////////////////////////////////////////////////////////////////////////////
//
// TabPane
//
/////////////////////////////////////////////////////////////////////////////

class TabPane : public Pane
{
public:
    static TRef<TabPane> Create();

    virtual IIntegerEventSource* GetEventSource() = 0;
    virtual void InsertPane(Pane* pPane, ButtonPane* pButtonPane, int nPaneID) = 0;
    virtual void InsertSubTab(TabPane* pTabPane, ButtonPane* pButtonPane, int nPaneID) = 0;
    virtual void ShowSelPane(bool fVisible = true) = 0;
};


//////////////////////////////////////////////////////////////////////////////
//
// ListPaneOld
//
//////////////////////////////////////////////////////////////////////////////

class ListPaneOld;

class ListItem : public IObjectSingle
{
public:
    virtual long GetItemData() = 0;
    virtual short GetItemHeight() = 0;
    virtual void DrawItem(Surface* pSurface, const WinRect& rect, bool fSelected, int iFirstSlot) = 0;
    virtual bool Update() = 0; // returns true if it needs repainting
    virtual void SetSortOrder(long lSortOrder) = 0; 
    virtual bool SetFilter(long lFilter) = 0; // returns true if visible
};

class ListPaneOld : public Pane
{
public:
    static TRef<ListPaneOld> Create(WinPoint size, int nItemHeight, bool bScroll, Pane* ppane);

    virtual IIntegerEventSource* GetEventSource() = 0;
    virtual IIntegerEventSource* GetRightClickEventSource() = 0;	// TE: Added to allow retrieval of the RightClick event
    virtual IIntegerEventSource* GetDoubleClickEventSource() = 0;	// TE: Added to allow retrieval of the DoubleClickClick event
    virtual IIntegerEventSource* GetMouseOverEvent() = 0;
    virtual IEventSource*   GetScrollEvent() = 0;

	virtual void			SetScrollbarHidden(bool isHidden) = 0;
    virtual void            SetListSize(WinPoint size) = 0;
    virtual void            SetItemHeight(int nItemHeight, const WinPoint size) = 0;
    virtual void            InsertItem(int iItem, ListItem* pListItem) = 0;
    virtual int             AppendItem(ListItem* pListItem) = 0;
    virtual void            RemoveItem(ListItem* pListItem) = 0;
    virtual void            RemoveItemByIdx(int iItem) = 0;
    virtual void            RemoveItemByData(long lItemData) = 0;
    virtual void            RemoveAll() = 0;
    virtual void            UpdateItem(ListItem* pListItem) = 0;
    virtual void            UpdateItemByIdx(int iItem) = 0;
    virtual void            UpdateItemByData(long lItemData) = 0;
    virtual void            UpdateAll() = 0;
    virtual void            SetSelItem(ListItem* pListItem) = 0;
    virtual void            SetSelItemByIdx(int iItem) = 0;
    virtual void            PartialInvestItemByIdx(int iItem) = 0;	// TE: Added to instigate PartialInvesting
    virtual void            InvestItemByIdx(int iItem) = 0;	// TE: Added to instigate DoubleclickInvesting
    virtual void            SetSelItemByData(long lItemData) = 0;
    virtual TRef<ListItem>  GetSelItem() = 0;
    virtual int             GetSelItemIdx() = 0;
    virtual TRef<ListItem>  GetItemByIdx(long iItem) = 0;
    virtual TRef<ListItem>  GetItemByData(long lItemData) = 0;
    virtual int             GetItemIdx(ListItem* pListItem) = 0;
    virtual int             GetItemIdxByData(long lItemData) = 0;
    virtual void            SetSortOrder(long lSortOrder) = 0;
    virtual void            SetFilter(long lFilter) = 0;
    virtual void            FillTestItems(int nItems) = 0;
    virtual int             GetScrollPosition() = 0;
    virtual void            SetScrollPosition(int iPosition) = 0;
    virtual void            ScrollToItemByIdx(int iItem) = 0;
    virtual void            ScrollPageUp() = 0;
    virtual void            ScrollPageDown() = 0;
    virtual void            ScrollTop() = 0;
    virtual void            ScrollBottom() = 0;
    virtual bool            MouseSel() = 0;
    virtual int             GetCountItems() = 0;
    virtual bool            IsItemVisible(ListItem* pListItem) = 0;
    virtual bool            IsItemVisibleByIdx(long iItem) = 0;
    virtual bool            IsItemVisibleByData(long lItemData) = 0;
};


//////////////////////////////////////////////////////////////////////////////
//
// AnimatedTransformImage
//
//////////////////////////////////////////////////////////////////////////////

enum {
    ATLeft,
    ATTopLeft,
    ATTop,
    ATTopRight,
    ATRight,
    ATBottomRight,
    ATBottom,
    ATBottomLeft,
    ATCenter
};

class AnimatedTranslateTransform2 : public Transform2, public EventSourceImpl  {
protected:
    AnimatedTranslateTransform2(Number* ptime, RectValue* prect) :
        Transform2(ptime, prect)
    {
    }
    
    Number*    GetTime() { return Number::Cast(GetChild(0)); }
    RectValue* GetRect() { return RectValue::Cast(GetChild(1)); }

public:
    static TRef<AnimatedTranslateTransform2> Create(
        Number* ptime,
        RectValue* prect,
        int side,
        const Point& vecStart,
        const Point& vecEnd,
        float dtimeAnimation
    );

    virtual void MoveStart() = 0;
    virtual void MoveEnd() = 0;
};


//////////////////////////////////////////////////////////////////////////////
//
// TrekClientEventSink
//
//////////////////////////////////////////////////////////////////////////////

class TrekClientEventSink : public IClientEventSink
{
private:
    TRef<IClientEventSink>      m_pClientEventSink;
    TRef<IClientEventSource>    m_pClientEventSource;

public:
    TrekClientEventSink()
    {
        m_pClientEventSink = IClientEventSink::CreateDelegate(this);
        m_pClientEventSource = trekClient.GetClientEventSource();
        m_pClientEventSource->AddSink(m_pClientEventSink);
    }

    ~TrekClientEventSink()
    {
        m_pClientEventSource->RemoveSink(m_pClientEventSink);
    }

};


#endif
