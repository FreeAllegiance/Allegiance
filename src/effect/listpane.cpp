#include "listpane.h"

#include <controls.h>
#include <font.h>

/////////////////////////////////////////////////////////////////////////////
//
// Default ItemPainter
//
/////////////////////////////////////////////////////////////////////////////

class DefaultItemPainter : public ItemPainter {
public:

    int   GetXSize()
    {
        return 1;
    }

    int   GetYSize()
    {
        return 1;
    }

    void Paint(ItemID pitem, Surface* psurface, bool bSelected, bool bFocus)
    {
    }
};

/////////////////////////////////////////////////////////////////////////////
//
// List Pane
//
/////////////////////////////////////////////////////////////////////////////

template<class Interface>
class ListPaneImpl :
    public Interface, // should contain ListPane,
    public IEventSink,
    public IIntegerEventSink
{
private:
    TRef<List>                   m_plist;
    TRef<ItemPainter>            m_ppainter;
    TRef<ScrollPane>             m_pscroll;
    ItemID                       m_pitemSelection;
    ItemID                       m_pitemSelectionPrevious;
    TRef<IEventSink>             m_peventSink;

    TRef<IIntegerEventSink>      m_pintegerEventSink;
    TRef<IItemEvent::SourceImpl> m_peventSelection;
    TRef<EventSourceImpl>        m_peventSingleClick;
	TRef<EventSourceImpl>        m_peventSingleRightClick;
    TRef<EventSourceImpl>        m_peventDoubleClick;
	TRef<EventSourceImpl>        m_peventDoubleRightClick;

    int                          m_indexSelection;
    int                          m_posScroll;
    WinPoint                     m_sizeMin;

    bool                         m_bHorizontal;

    TVector<ItemID>              m_vectItemsOld;
    bool                         m_bNeedScrollUpdate;
    bool                         m_bNeedSelectionOnScreen;
    int                          m_iOldSelection;

public:
    ListPaneImpl(
        const WinPoint& sizeMin,
        List*           plist,
        ItemPainter*    ppainter,
        ScrollPane*     pscroll,
        bool            bHorizontal
    ) :
        m_sizeMin(sizeMin),
        m_ppainter(ppainter),
        m_pscroll(pscroll),
        m_indexSelection(0),
        m_posScroll(0),
        m_pitemSelection(NULL),
        m_pitemSelectionPrevious(NULL),
        m_bHorizontal(bHorizontal),
        m_iOldSelection(-1),
        m_bNeedScrollUpdate(false),
        m_bNeedSelectionOnScreen(false)
    {
        m_peventSelection   = new IItemEvent::SourceImpl();
        m_peventSingleClick = new EventSourceImpl();
        m_peventDoubleClick = new EventSourceImpl();
		m_peventSingleRightClick = new EventSourceImpl();
		m_peventDoubleRightClick = new EventSourceImpl();

        if (m_ppainter == NULL) {
            m_ppainter = new DefaultItemPainter();
        }

        if (m_pscroll) {
            m_pscroll->GetEventSource()->AddSink(
                m_pintegerEventSink = IIntegerEventSink::CreateDelegate(this)
            );
        }

        if (plist == NULL) {
            SetList(new EmptyList());
        } else {
            SetList(plist);
        }

        UpdateScrollBar();
    }

    ~ListPaneImpl()
    {
        m_plist->GetChangedEvent()->RemoveSink(m_peventSink);

        if (m_pscroll) {
            m_pscroll->GetEventSource()->RemoveSink(m_pintegerEventSink);
        }
    }

    const int GetSignificantSize() // returns size of the significant dimension
    {
        if (m_bHorizontal)
        {
            return Interface::GetSize().X();
        }
        else
        {
            return Interface::GetSize().Y();
        }
    }

    const int GetSignificantSize(ItemPainter & itempainter) // returns size of the significant dimension
    {
        if (m_bHorizontal)
        {
            return itempainter.GetXSize();
        }
        else
        {
            return itempainter.GetYSize();
        }
    }

    int GetScrollPos()
    {
        return m_posScroll;
    }

    void SetScrollPos(int pos)
    {
        m_posScroll = 
            std::max(std::min(pos, m_plist->GetCount() * GetSignificantSize(*m_ppainter) - GetSignificantSize()), 0);

        if (m_pscroll) {
            m_pscroll->SetPos(m_posScroll);
        }
    }

    void UpdateScrollBar()
    {
        if (m_pscroll) {
            m_pscroll->SetLineSize(GetSignificantSize(*m_ppainter));//1);
            m_pscroll->SetSizes(
                m_plist->GetCount() * GetSignificantSize(*m_ppainter),
                GetSignificantSize()/* / GetSignificantSize(*m_ppainter)*/
            );
        }
    }

    ScrollPane * GetScrollPane()
    {
        return m_pscroll;
    }

    void UpdateScrollPos()
    {
        m_bNeedScrollUpdate = true;
    }

    void NextItem()
    {
        if (m_indexSelection < m_plist->GetCount() - 1) {
            m_indexSelection++;
            m_pitemSelection  = m_plist->GetNext(m_pitemSelection);

            Interface::NeedPaint();
            SelectionChanged();
            m_bNeedScrollUpdate      = true;
            m_bNeedSelectionOnScreen = true;
        }
    }

    void PreviousItem()
    {
        if (m_indexSelection != 0) {
            m_indexSelection--;
            m_pitemSelection  = m_plist->GetItem(m_indexSelection);

            Interface::NeedPaint();
            SelectionChanged();
            m_bNeedScrollUpdate      = true;
            m_bNeedSelectionOnScreen = true;
        }
    }

    void PageUp()
    {
        int delta = GetSignificantSize() / GetSignificantSize(*m_ppainter);
        while (delta > 0) {
            PreviousItem();
            delta--;
        }
    }

    void PageDown()
    {
        int delta = GetSignificantSize() / GetSignificantSize(*m_ppainter);
        while (delta > 0) {
            NextItem();
            delta--;
        }
    }

    void SelectionChanged()
    {
        if (m_pitemSelectionPrevious != m_pitemSelection) {
            m_peventSelection->Trigger(m_pitemSelection);
            m_pitemSelectionPrevious = m_pitemSelection;
        }
    }

    void ListChanged()
    {
        int index;
        
        if (m_pitemSelection != NULL) 
            index = m_plist->GetIndex(m_pitemSelection);
        else 
            index = -1;

        if (index == -1) 
        {
            m_pitemSelection = NULL;
            m_iOldSelection = -1;
        }

        m_indexSelection  = index;

        UpdateScrollBar();
        UpdateScrollPos();
        Interface::NeedLayout();
        Interface::NeedPaint();
        SelectionChanged();
    }

    void ScrollBarChanged()
    {
        m_posScroll  = m_pscroll->GetPos()/* * GetSignificantSize(*m_ppainter)*/;
        Interface::NeedPaint();
    }

    //
    // ListPane Methods
    //

    void SetList(List* plist)
    {
        if (plist != m_plist) {
            if (m_plist != NULL) {
                m_plist->GetChangedEvent()->RemoveSink(m_peventSink);
            }

            if (plist == NULL) {
                m_plist = new EmptyList();
            } else {
                m_plist = plist;
            }

            m_plist->GetChangedEvent()->AddSink(
                m_peventSink = IEventSink::CreateDelegate(this)
            );
            ListChanged();
        }
    }

    void SetItemPainter(ItemPainter* ppainter)
    {
        if (ppainter == NULL) {
            m_ppainter = new DefaultItemPainter();
        } else {
            m_ppainter = ppainter;
        }

        UpdateScrollBar();
        UpdateScrollPos();
        Interface::NeedLayout();
        Interface::NeedPaint();
    }

    void SetSelection(ItemID pitem)
    {
        if (pitem != m_pitemSelection)
        {
            if (pitem)
            {
                m_indexSelection = m_plist->GetIndex(pitem);
                
                if (m_indexSelection == -1)
                {
                    ZAssert(false);
                    pitem = NULL;
                }
            }
            else
                m_indexSelection = -1;

            m_pitemSelection = pitem;
            if (m_bNeedScrollUpdate)
                m_iOldSelection = -1;
            else
                m_iOldSelection = m_indexSelection;
            SelectionChanged();
            Interface::NeedPaint();
        }
    }

    void SetSelection(int nIndex)
    {
        SetSelection(m_plist->GetItem(nIndex));
    }

    virtual ItemID GetSelection()
    {
        return m_pitemSelection;
    }

    IItemEvent::Source* GetSelectionEventSource()
    {
        return m_peventSelection;
    }

    IEventSource* GetDoubleClickEventSource()
    {
        return m_peventDoubleClick;
    }

	IEventSource* GetDoubleRightClickEventSource()
    {
        return m_peventDoubleRightClick;
    }

    IEventSource* GetSingleClickEventSource()
    {
        return m_peventSingleClick;
    }

    IEventSource* GetSingleRightClickEventSource()
    {
        return m_peventSingleRightClick;
    }

    void ScrollToItem(ItemID pitem)
    {
        int index = 0;
        
        if (pitem != NULL) 
            index = m_plist->GetIndex(pitem);
        
        // try to center the selected item
        SetScrollPos(((2 * index + 1) * GetSignificantSize(*m_ppainter) - GetSignificantSize())/2);
    }

    void ForceRefresh()
    {
        Interface::NeedPaint();
    }

    //
    // IEventSink Methods
    //

    bool OnEvent(IEventSource* pevent)
    {
        ListChanged();
        return true;
    }

    bool OnEvent(IIntegerEventSource* pevent, int value)
    {
        ScrollBarChanged();
        return true;
    }

    //
    // Pane Methods
    //

    void UpdateLayout()
    {
        Interface::InternalSetSize(
            WinPoint(
                std::max(m_ppainter->GetXSize(), m_sizeMin.X()),
                std::max(Interface::GetExpand().Y(), m_sizeMin.Y())
            )
        );

        UpdateScrollBar();
    }

    void VerifyScrollPos()
    {
        if (m_bNeedScrollUpdate) {
            m_bNeedScrollUpdate = false;

            if (m_bNeedSelectionOnScreen) {
                m_bNeedSelectionOnScreen = false;

                //
                // Figure out where the selection is
                //

                int nItemSize  = GetSignificantSize(*m_ppainter);
                int ySelection = m_indexSelection * nItemSize - GetScrollPos();

                //
                // Make sure the selection is in the window
                //
 
                if (ySelection + GetSignificantSize(*m_ppainter) >= GetSignificantSize()) {
                    ySelection = GetSignificantSize() - GetSignificantSize(*m_ppainter);
                }
     
                if (ySelection <  0) {
                    ySelection = 0;
                }
 
                 //
                // Adjust the scroll bar so that the selection doesn't move
                //
     
                if (m_pscroll && m_plist->GetCount() * GetSignificantSize(*m_ppainter) > GetSignificantSize()) {
                    SetScrollPos(m_indexSelection * GetSignificantSize(*m_ppainter) - ySelection);
                } else {
                    SetScrollPos(0);
                }
            } else {
                //
                // find the top item in the old list
                //

                int nItemSize = GetSignificantSize(*m_ppainter);
                int iOldTop   = GetScrollPos() / nItemSize;

                //
                // use the selected item as the basis if it was visible, otherwise use 
                // the top item
                //

                int nLinesPerScreen = GetSignificantSize() / nItemSize;
                int iOldBasis;

                if (
                       m_iOldSelection >= iOldTop 
                    && m_iOldSelection <  iOldTop + nLinesPerScreen
                ) {
                    //
                    // visible
                    //

                    iOldBasis = m_iOldSelection;
                } else {
                    iOldBasis = iOldTop;
                }

                //
                // get the scroll offset from the basis to the top, so we can try 
                // to keep the basis perfectly stable.
                //

                int nTopFromBasis = GetScrollPos() - iOldBasis * nItemSize;

                //
                // find the new basis...
                //

                int iNewBasis = FindNearestNewIndex(iOldBasis);

                //
                // set the scroll position relative to the new basis
                //

                SetScrollPos(iNewBasis * nItemSize + nTopFromBasis);
            }

            //
            // update m_vectItemsOld to contain the new contents
            //

            int nNewCount = m_plist->GetCount();

            m_vectItemsOld.SetCount(nNewCount);

            for (int i = 0; i < nNewCount; i++) {
                m_vectItemsOld.Set(i, m_plist->GetItem(i));
            }

            m_iOldSelection = m_indexSelection;
        }
    }

    int FindNearestNewIndex(int nOldIndex)
    {
        if (nOldIndex == -1 || m_vectItemsOld.GetCount() == 0)
            return -1;
        
        ZAssert(nOldIndex >= 0 && nOldIndex < m_vectItemsOld.GetCount());

        int nNewIndex;

        // check to see if the old item exists in the new list
        nNewIndex = m_plist->GetIndex(m_vectItemsOld[nOldIndex]);

        // if we haven't found a new index yet, try searching the items around it
        int iDelta = 1;
        while (nNewIndex == -1 
            && (nOldIndex - iDelta >= 0 
                || nOldIndex + iDelta < m_vectItemsOld.GetCount()))
        {
            if (nOldIndex - iDelta >= 0)
                nNewIndex = m_plist->GetIndex(m_vectItemsOld[nOldIndex - iDelta]);

            if (nNewIndex != -1 && nOldIndex + iDelta < m_vectItemsOld.GetCount())
                nNewIndex = m_plist->GetIndex(m_vectItemsOld[nOldIndex - iDelta]);

            ++iDelta;
        }

        // if we still haven't found a new index, default to the first item
        if (nNewIndex == -1)
        {
            nNewIndex = 0;
        }

        return nNewIndex;
    }

    void Paint(Surface* psurface)
    {
        VerifyScrollPos();

        //
        // Find the item at the top of the list
        //

        int ysize    = GetSignificantSize(*m_ppainter);
        int index    = GetScrollPos() / ysize;
        int y        = index * ysize - GetScrollPos();
        ItemID pitem = m_plist->GetItem(index);

        while (
               pitem != NULL
            && y < GetSignificantSize()
        ) {
            if (m_bHorizontal)
                psurface->Offset(WinPoint(y,  0));
            else
                psurface->Offset(WinPoint(0,  y));

            m_ppainter->Paint(
                pitem,
                psurface,
                pitem == m_pitemSelection,
                false
            );
            if (m_bHorizontal)
                psurface->Offset(WinPoint(-y,  0));
            else
                psurface->Offset(WinPoint(0,  -y));

            y += ysize;
            pitem = m_plist->GetNext(pitem);
        }
    }

    //
    // IMouseInput
    //

    MouseResult Button(
        IInputProvider* pprovider,
        const Point& point,
        int button,
        bool bCaptured,
        bool bInside,
        bool bDown
    ) {
        VerifyScrollPos();

        if (button == 0 && bDown) {
            int nSize = GetSignificantSize(*m_ppainter);

            int nSignificantMouseDim = (int)(m_bHorizontal ? point.X(): point.Y());

            SetSelection(m_plist->GetItem((nSignificantMouseDim + GetScrollPos()) / nSize));

            Interface::NeedPaint();

            SelectionChanged();
            if (pprovider->IsDoubleClick()) {
                m_peventDoubleClick->Trigger();
            }
            else {
                m_peventSingleClick->Trigger();
            }

        }
		else if(button == 1 && bDown)
		{
			int nSize = GetSignificantSize(*m_ppainter);

            int nSignificantMouseDim = (int)(m_bHorizontal ? point.X(): point.Y());

            SetSelection(m_plist->GetItem((nSignificantMouseDim + GetScrollPos()) / nSize));

            Interface::NeedPaint();

            SelectionChanged();
            if (pprovider->IsDoubleClick()) {
                m_peventDoubleRightClick->Trigger();
            }
            else {
                m_peventSingleRightClick->Trigger();
            }
		} else if(button == 8 && bDown) { //Imago 8/14/09 mouse wheel
            NextItem();
            SelectionChanged();
        } else if(button == 9 && bDown) {
            PreviousItem();
            SelectionChanged();
        }

        return MouseResult();
    }

    //
    // IKeyboardInput
    //

    bool OnKey(IInputProvider* pprovider, const KeyState& ks, bool& fForceTranslate)
    {
        VerifyScrollPos();

        if (ks.bDown) {
            switch (ks.vk) {
                case VK_DOWN:
                    NextItem();
                    SelectionChanged();
                    return true;

                case VK_UP:
                    PreviousItem();
                    SelectionChanged();
                    return true;
                
                case VK_PRIOR:      // page up
                    PageUp();
                    SelectionChanged();
                    return true;

                case VK_NEXT:       // page down
                    PageDown();
                    SelectionChanged();
                    return true;
            }
        }

        return false;
    }
};

/////////////////////////////////////////////////////////////////////////////
//
// Constructor
//
/////////////////////////////////////////////////////////////////////////////

TRef<ListPane> CreateListPane(
    const WinPoint& sizeMin,
    List*           plist,
    ItemPainter*    ppainter,
    ScrollPane*     pscroll,
    bool            bHorizontal
) {
    return new ListPaneImpl<ListPane>(sizeMin, plist, ppainter, pscroll, bHorizontal);
}

/////////////////////////////////////////////////////////////////////////////
//
// StringList
//
/////////////////////////////////////////////////////////////////////////////

class StringListImpl :
    public StringList
{
private:

    //////////////////////////////////////////////////////////////////////////////
    //
    // Data members
    //
    //////////////////////////////////////////////////////////////////////////////

    TRef<IEngineFont>     m_pfont;
    Color                 m_color;
    Color                 m_colorSelected;
    int                   m_xsize;
    TRef<EventSourceImpl> m_peventSource;
    TRef<StringListItem>      m_pitem;
    int                   m_count;

public:
    //////////////////////////////////////////////////////////////////////////////
    //
    // Constructor
    //
    //////////////////////////////////////////////////////////////////////////////

    StringListImpl(
        IEngineFont* pfont,
        const Color& color,
        const Color& colorSelected,
        int          xsize
    ) :
        m_pfont(pfont),
        m_color(color),
        m_colorSelected(colorSelected),
        m_xsize(xsize),
        m_count(0)
    {
        m_peventSource = new EventSourceImpl();
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // StringList Methods
    //
    //////////////////////////////////////////////////////////////////////////////

    void AddItem(const ZString& str)
    {
        m_count++;
        m_pitem = new StringListItem(str, m_pitem);

        m_peventSource->Trigger();
    }

    void SetEmpty()
    {
        m_count = 0;
        m_pitem = NULL;

        m_peventSource->Trigger();
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // List Methods
    //
    //////////////////////////////////////////////////////////////////////////////

    int GetCount()
    {
        return m_count;
    }

    ItemID GetItem(int index)
    {
        ZAssert(index >= 0);

        if (index < m_count) {
            StringListItem* pitem = m_pitem;

            while (index > 0) {
                pitem = pitem->GetNext();
                index--;
            }

            return pitem;
        } else {
            return NULL;
        }
    }

    int GetIndex(ItemID pitemFind)
    {
        StringListItem* pitem = m_pitem;
        int index = 0;

        while (pitem != NULL) {
            if ((ItemID)pitem == pitemFind) {
                return index;
            }
            pitem = pitem->GetNext();
            index++;
        }

        return -1;
    }

    ItemID GetNext(ItemID pitem)
    {
        return ((StringListItem*)pitem)->GetNext();
    }

    IEventSource* GetChangedEvent()
    {
        return m_peventSource;
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // ItemPainter Methods
    //
    //////////////////////////////////////////////////////////////////////////////

    int GetXSize()
    {
        return m_xsize;
    }

    int GetYSize()
    {
        return m_pfont->GetHeight();
    }

    void Paint(ItemID pitemArg, Surface* psurface, bool bSelected, bool bFocus)
    {
        StringListItem* pitem = (StringListItem*)pitemArg;

        if (bSelected) {
            psurface->FillRect(
                WinRect(0, 0, GetXSize(), GetYSize()),
                m_colorSelected
            );
        }

        psurface->DrawString(m_pfont, m_color, WinPoint(0, 0), pitem->GetString());
    }
};

TRef<StringList> CreateStringList(
    IEngineFont* pfont,
    const Color& color,
    const Color& colorSelected,
    int          xsize
) {
    return
        new StringListImpl(
            pfont,
            color,
            colorSelected,
            xsize
        );
}

/////////////////////////////////////////////////////////////////////////////
//
// StringListPane
//
/////////////////////////////////////////////////////////////////////////////

class StringListPaneImpl:
    public ListPaneImpl<StringListPane>
{
public:
    StringListPaneImpl(
        const WinPoint& sizeMin,
        List*           plist,
        ItemPainter*    ppainter,
        ScrollPane*     pscroll,
        bool            bHorizontal,
        IEngineFont*    pfont,
        const Color&    color,
        const Color&    colorSelected
    ) :
    ListPaneImpl<StringListPane>(sizeMin, plist, ppainter, pscroll, bHorizontal)
    {
        m_pStringList = CreateStringList(pfont, color, colorSelected, sizeMin.X());

        SetList(m_pStringList);
        SetItemPainter(m_pStringList);
    }

    TRef<StringList> GetStringList()
    {
        return m_pStringList;
    }

private:

    TRef<StringList>    m_pStringList;
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
)
{
    return new StringListPaneImpl(sizeMin, plist, ppainter, pscroll, bHorizontal, pfont, color, colorSelected);
}

