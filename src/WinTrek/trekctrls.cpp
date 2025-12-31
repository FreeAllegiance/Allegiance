#include "pch.h"
#include "trekctrls.h"


/////////////////////////////////////////////////////////////////////////////
//
// SubImagePane
//
/////////////////////////////////////////////////////////////////////////////

class SubImagePaneImpl : public SubImagePane
{
private:
    TRef<Image> m_pimage;
    WinRect     m_rect;

    void UpdateLayout()
    {
        InternalSetSize(m_rect.Size());
        Pane::UpdateLayout();
    }

    void Paint(Surface* psurface)
    {
        psurface->BitBlt(WinPoint(0, 0), m_pimage->GetSurface(), m_rect);
    }

public:
    SubImagePaneImpl(Image* pimage, const WinRect& rect) :
        m_pimage(pimage),
        m_rect(rect)
    {
    }
    
    void    SetImage(Image* pimage, const WinRect& rect)
    {
        m_pimage = pimage;
        if (rect.Size() != GetSize()) 
            NeedLayout();
        NeedPaint();
    }
};


TRef<SubImagePane> SubImagePane::Create(Image* pimage, const WinRect& rect)
{
    return new SubImagePaneImpl(pimage, rect);
}

/////////////////////////////////////////////////////////////////////////////
//
// TabPane
//
/////////////////////////////////////////////////////////////////////////////

class TabPaneImpl : public TabPane, public EventTargetContainer<TabPaneImpl>
{
private:
    TRef<IntegerEventSourceImpl> m_pEventSource;
    TRef<ButtonBarPane>          m_pButtonBarPane;
    TMap<int, TRef<Pane> >       m_mapPanes;
    TMap<int, TRef<TabPane> >    m_mapSubTabs;
    TRef<ColumnPane>             m_pColumnPane;
    int                          m_nPaneIDCurrent;
    TRef<Pane>                   m_pPaneCurrent;
    TRef<TabPane>                m_pSubTabCurrent;
    bool                         m_fShowSelPane;
        
public:

    TabPaneImpl() :
        m_nPaneIDCurrent(-1),
        m_fShowSelPane(false)
    {
        m_pColumnPane = new ColumnPane();
        m_pButtonBarPane = CreateButtonBarPane(true);
        m_pColumnPane->InsertAtBottom(m_pButtonBarPane);
        InsertAtBottom(m_pColumnPane);
        m_pEventSource = new IntegerEventSourceImpl();
		// mdvalley: Pointer, class name
        AddEventTarget(&TabPaneImpl::OnSelectTab, m_pButtonBarPane->GetEventSource());
    }

    ~TabPaneImpl()
    {
    }

    IIntegerEventSource* GetEventSource()
    {
        return m_pEventSource;
    }

    bool OnSelectTab(int value)
    {
        TRef<Pane> pPane;
        if (m_nPaneIDCurrent != value && m_mapPanes.Find(value, pPane)) {
            if (pPane != m_pPaneCurrent) {
                if (m_pSubTabCurrent) {
                    m_pSubTabCurrent->ShowSelPane(false);
                }

                m_pSubTabCurrent = NULL;

                if (m_mapSubTabs.Find(value, m_pSubTabCurrent)) {
                    m_pSubTabCurrent->ShowSelPane(true);
                }

                m_pColumnPane->RemoveChild(m_pPaneCurrent);
                m_pColumnPane->InsertAtBottom(m_pPaneCurrent = pPane);
            }

            m_nPaneIDCurrent = value;
            m_pEventSource->Trigger(value); // tell sinks what tab is active
        }
        return true;
    }

    void InsertPane(Pane* pPane, ButtonPane* pButtonPane, int nPaneID)
    {
        ZAssert(nPaneID != -1); // reserved
        ZAssert(!m_fShowSelPane);

        if (m_mapPanes.Count() == 0) {
            m_nPaneIDCurrent = nPaneID;
            m_pPaneCurrent = pPane;
        }
        m_mapPanes.Set(nPaneID, pPane);
        m_pButtonBarPane->InsertButton(pButtonPane, nPaneID);
    }

    void InsertSubTab(TabPane* pTabPane, ButtonPane* pButtonPane, int nPaneID)
    {
        if (m_mapPanes.Count() == 0)
            m_pSubTabCurrent = pTabPane;
        m_mapSubTabs.Set(nPaneID, pTabPane);
        InsertPane(pTabPane, pButtonPane, nPaneID);
    }
    
    void ShowSelPane(bool fVisible)
    {
        ZAssert(m_fShowSelPane != fVisible);
        m_fShowSelPane = fVisible;
        
        if (m_pSubTabCurrent)
            m_pSubTabCurrent->ShowSelPane(fVisible);

        if (fVisible)
            {
            m_pColumnPane->InsertAtBottom(m_pPaneCurrent);
            m_pEventSource->Trigger(m_nPaneIDCurrent);
            }
        else
            m_pColumnPane->RemoveChild(m_pPaneCurrent);
    }
    
    void UpdateLayout()
    {
        DefaultUpdateLayout();
    }

};

TRef<TabPane> TabPane::Create()
{
    return new TabPaneImpl();
}

//////////////////////////////////////////////////////////////////////////////
//
// ListPaneOld
//
//////////////////////////////////////////////////////////////////////////////

class TestListItem : public ListItem
{
public:
    short GetItemHeight() {return 1;}
    long GetItemData() {return 0;}
    bool Update() {return false;}
    void SetSortOrder(long lSortOrder) {}
    bool SetFilter(long lFilter) {return true;}
    void DrawItem(Surface* pSurface, const WinRect& rect, bool fSelected, int iFirstSlot)
    {
        int data = (int)this;
        char buf[30];
        sprintf(buf,"%d", data);
        pSurface->DrawString(TrekResources::SmallFont(), Color::White(), rect.Min(), buf);
    }
};

class ListPaneOldImpl : public ListPaneOld, public EventTargetContainer<ListPaneOldImpl>
{
private:
    TRef<ScrollPane> m_pScrollPane;
    TRef<Pane> m_pScrollBar;
    TRef<Pane> m_pBlankPane;
    TRef<IntegerEventSourceImpl> m_pEventSource;
    TRef<IntegerEventSourceImpl> m_pRightClickEventSource;	// TE: Declared new RightClick event
    TRef<IntegerEventSourceImpl> m_pDoubleClickEventSource;	// TE: Declared new RightClick event
    TRef<IntegerEventSourceImpl> m_pMouseOverEventSource;
    TRef<EventSourceImpl> m_pEventScroll;
    
    int m_iSelItem;
    int m_iTopItem;
    int m_cVisibleItems;
    int m_nItemHeight;
    int m_nItemWidth;

    bool m_bMouseSel;

    TRef<Image> m_pImageBkgnd;
    TRef<Image> m_pImageBkgndSel;
    WinPoint    m_ptImgOrigin;

    TVector<TRef<ListItem> > m_vItems;

public:
    ListPaneOldImpl(WinPoint size, int nItemHeight, bool bScroll, Pane* ppane) :
        m_iSelItem(-1),
        m_iTopItem(0),
        m_nItemHeight(nItemHeight),
        m_bMouseSel(false)
    {
        m_nItemWidth = size.X();
        m_pEventSource = new IntegerEventSourceImpl();
        m_pRightClickEventSource = new IntegerEventSourceImpl();	// TE: Initialized RightClick event
        m_pDoubleClickEventSource = new IntegerEventSourceImpl();	// TE: Initialized RightClick event
        m_pMouseOverEventSource = new IntegerEventSourceImpl();
        m_pEventScroll = new EventSourceImpl();
        
        m_cVisibleItems = size.Y()/nItemHeight;
    
        TRef<RowPane> pRowPane = new RowPane();
        m_pBlankPane = new Pane();
        InternalSetSize(m_pBlankPane, size);
        pRowPane->InsertAtBottom(m_pBlankPane);

        // Add the scroll bar
        TRef<Modeler> pModeler = GetModeler();
        
        TRef<Image> pImageUp      = pModeler->LoadImage(AWF_CONTROL_UP_ARROW,            true);
        TRef<Image> pImageUpSel   = pModeler->LoadImage(AWF_CONTROL_SELECTED_UP_ARROW,   true);
        TRef<Image> pImageDown    = pModeler->LoadImage(AWF_CONTROL_DOWN_ARROW,          true);
        TRef<Image> pImageDownSel = pModeler->LoadImage(AWF_CONTROL_SELECTED_DOWN_ARROW, true);

        TRef<ButtonPane> pbuttonUp   = CreateButton(new ImagePane(pImageUp),   new ImagePane(pImageUpSel),   false, 0.1f, 0.5f);
        TRef<ButtonPane> pbuttonDown = CreateButton(new ImagePane(pImageDown), new ImagePane(pImageDownSel), false, 0.1f, 0.5f);

        if (bScroll) {
            m_pScrollPane =
                CreateScrollPane(
                    new ColumnPane(),//new BorderPane(0, MakeColorFromCOLORREF(TrekResources::BlackColor())),
                    ThumbPane::Create(pModeler, false, NULL),
                    pbuttonUp, pbuttonUp->GetEventSource(),
                    pbuttonDown, pbuttonDown->GetEventSource(),
                    false, 0.25f, 1, 1, 1, 1
                );

			// mdvalley: another pointer &ClassName
            AddEventTarget(&ListPaneOldImpl::OnScroll, m_pScrollPane->GetEventSource());
            m_pScrollPane->SetPageSize(m_cVisibleItems);
            m_pScrollPane->SetSize(0);
        
  
            m_pScrollBar = 
                //new EdgePane( new JustifyPane( JustifyPane::Center, WinPoint(17, size.Y()-2), m_pScrollPane));
                //new EdgePane( 
                m_pScrollPane;// );

            TRef<ColumnPane> pcol = new ColumnPane();

            if (ppane) {
                pcol->InsertAtBottom(ppane);
            }

            pcol->InsertAtBottom(m_pScrollBar);
            pcol->InsertAtBottom(new Pane(NULL, WinPoint(0, 8)));

            pRowPane->InsertAtBottom(pcol);
        }

        InsertAtBottom(pRowPane);
    }

    virtual ~ListPaneOldImpl()
    {
    }

	void SetScrollbarHidden(bool isHidden)
	{
		m_pScrollBar->SetHidden(isHidden);
	}

    int GetCountItems()
    {
        return m_vItems.GetCount();
    }

    IIntegerEventSource* GetEventSource()
    {
        return m_pEventSource;
    }

	// TE: Added getter for the RightClickEvent
    IIntegerEventSource* GetRightClickEventSource()
    {
        return m_pRightClickEventSource;
    }

	// TE: Added getter for the DoubleClickEvent
    IIntegerEventSource* GetDoubleClickEventSource()
    {
        return m_pDoubleClickEventSource;
    }

    IIntegerEventSource* GetMouseOverEvent()
    {
        return m_pMouseOverEventSource;
    }

    IEventSource* GetScrollEvent()
    {
        return m_pEventScroll;
    }

    void SetListSize(WinPoint size)
    {
        m_nItemWidth = size.X();
        m_cVisibleItems = size.Y()/m_nItemHeight;
        if (m_pScrollPane) {
            m_pScrollPane->SetPageSize(m_cVisibleItems);
        }
        m_pBlankPane->SetSize(size);
    }

    // Update per-item height and reflow the list
    void SetItemHeight(int nItemHeight, WinPoint size)
    {
        if (nItemHeight <= 0)
            return;

        // update height and recompute visible count
        m_nItemHeight = nItemHeight;

        // Reuse SetListSize logic 
        SetListSize(size);

        // Clamp top item so it remains valid
        if (m_iTopItem > std::max(0, m_vItems.GetCount() - m_cVisibleItems))
            m_iTopItem = std::max(0, m_vItems.GetCount() - m_cVisibleItems);

        if (m_pScrollPane) {
            m_pScrollPane->SetPos(m_iTopItem);
            // ensure scroll pane knows the total size
            m_pScrollPane->SetSize(m_vItems.GetCount());
        }

        NeedLayout();
        NeedPaint();
    }

    void FillTestItems(int nItems)
    {
        for (int i=0; i<nItems; i++) {
            AppendItem( new TestListItem());
        }
    }

    void InsertItem(int iItem, ListItem* pListItem)
    {
        for (int i = 0; i < pListItem->GetItemHeight(); i++) {
            m_vItems.Insert(iItem, pListItem);
        }

        if (iItem <= m_iSelItem) {
            SetSelItemByIdx(m_iSelItem + pListItem->GetItemHeight());
        }

        if (m_pScrollPane) {
            m_pScrollPane->SetSize(m_vItems.GetCount());
            if (iItem <= m_iTopItem) {
                m_pScrollPane->SetPos(m_iTopItem + pListItem->GetItemHeight());
            }
        }
        NeedPaint();
    }

    int AppendItem(ListItem* pListItem)
    {
        int iItem = m_vItems.GetCount();
        InsertItem(iItem, pListItem);
        return iItem;
    }

    void RemoveItem(ListItem* pListItem)
    {
        RemoveItemByIdx(GetItemIdx(pListItem));
    }
    
    void RemoveItemByIdx(int iItem)
    {
        int numRemoved = 0;

        if (iItem != -1) {
            // find the first matching item

            while (iItem > 1 && m_vItems[iItem-1] == m_vItems[iItem]) {
                iItem--;
            }

            // now remove all the matching items

            TRef<ListItem> pListItem = m_vItems[iItem];
            while (iItem < m_vItems.GetCount() && m_vItems[iItem] == pListItem) {
                m_vItems.Remove(iItem);
                numRemoved++;
            }
                
            if (iItem <= m_iSelItem) {
                SetSelItemByIdx(m_iSelItem - numRemoved);
            } else {
                SetSelItemByIdx(m_iSelItem);
            }

            if (m_pScrollPane) {
                // save the old top item, because shrinking the scroll pane could change it
                int iOldTopItem = m_iTopItem;

                m_pScrollPane->SetSize(m_vItems.GetCount());
                
                if (iItem < iOldTopItem) {
                    SetScrollPosition(std::max(iOldTopItem - numRemoved, 0));
                } else {
                    SetScrollPosition(std::min(iOldTopItem, 
                        std::max(m_vItems.GetCount() - m_cVisibleItems, 0)));
                }
            }
            NeedPaint();
        }
    }
    
    void RemoveItemByData(long lItemData)
    {
        RemoveItemByIdx(GetItemIdxByData(lItemData));
    }

    void RemoveAll()
    {
        m_vItems.SetEmpty();
        if (m_pScrollPane) {
            m_pScrollPane->SetSize(m_vItems.GetCount());
            m_pScrollPane->SetPos(0);
        }
        SetSelItemByIdx(-1);
        NeedPaint();
    }

    void UpdateItem(ListItem* pListItem)
    {
        UpdateItemByIdx(GetItemIdx(pListItem));
    }
    
    void UpdateItemByIdx(int iItem)
    {
        if (iItem != -1)
            {
            // find the first matching item
            while (iItem > 1 && m_vItems[iItem-1] == m_vItems[iItem])
                iItem--;
            // update just the first occurence
            if (m_vItems[iItem]->Update())
                NeedPaint();
            }
    }
    
    void UpdateItemByData(long lItemData)
    {
        UpdateItemByIdx(GetItemIdxByData(lItemData));
    }

    void UpdateAll()
    {
        bool bNeedPaint = false;
        for (int iItem = 0; iItem < m_vItems.GetCount(); 
                    iItem += m_vItems[iItem]->GetItemHeight())
            {
            if (m_vItems[iItem]->Update())
                bNeedPaint = true;
            }
        if (bNeedPaint)
            NeedPaint();
    }
    
    void SetSelItem(ListItem* pListItem)     
    {
        SetSelItemByIdx(GetItemIdx(pListItem));
    }

    void SetSelItemByIdx(int iItem)
    {
        if (iItem < 0)
            iItem = -1;
        if (iItem >= m_vItems.GetCount())
            iItem = m_vItems.GetCount() - 1;

        // find the first matching item
        while (iItem >= 1 && m_vItems[iItem-1] == m_vItems[iItem])
            iItem--;

        m_iSelItem = iItem;
 
        NeedPaint();
        m_pEventSource->Trigger(m_iSelItem);
    }

    void InvestItemByIdx(int iItem)
    {
        if (iItem < 0)
            iItem = -1;
        if (iItem >= m_vItems.GetCount())
            iItem = m_vItems.GetCount() - 1;

        // find the first matching item
        while (iItem >= 1 && m_vItems[iItem-1] == m_vItems[iItem])
            iItem--;

        m_iSelItem = iItem;
 
        NeedPaint();
        m_pDoubleClickEventSource->Trigger(m_iSelItem);
    }

	// TE: Added PartialInvest method to trigger right-clicking
    void PartialInvestItemByIdx(int iItem)
    {
        if (iItem < 0)
            iItem = -1;
        if (iItem >= m_vItems.GetCount())
            iItem = m_vItems.GetCount() - 1;

        // find the first matching item
        while (iItem >= 1 && m_vItems[iItem-1] == m_vItems[iItem])
            iItem--;

        m_iSelItem = iItem;
 
        NeedPaint();
        m_pRightClickEventSource->Trigger(m_iSelItem);
    }

    void SetSelItemByData(long lItemData)     
    {
        SetSelItemByIdx(GetItemIdxByData(lItemData));
    }

    int GetSelItemIdx()
    {
        return m_iSelItem;
    }

    TRef<ListItem>  GetSelItem()
    {
        return GetItemByIdx(m_iSelItem);
    }

    TRef<ListItem>  GetItemByIdx(long iItem)
    {
        if ((-1 >= iItem) || (iItem >= m_vItems.GetCount()))
            return NULL;
        else
            return m_vItems[iItem];
    }

    TRef<ListItem>  GetItemByData(long lItemData)
    {
        return GetItemByIdx(GetItemIdxByData(lItemData));
    }

    int GetItemIdx(ListItem* pListItem)
    {
        int cItems = m_vItems.GetCount();
        for (int iItem = 0; iItem < cItems; iItem++)
            {
            if (m_vItems[iItem] == pListItem)
                return iItem;
            }
        return -1;
    }
    
    int GetItemIdxByData(long lItemData)
    {
        int cItems = m_vItems.GetCount();
        for (int iItem = 0; iItem < cItems; iItem++)
            {
            if (m_vItems[iItem]->GetItemData() == lItemData)
                return iItem;
            }
        return -1;
    }

    void SetSortOrder(long lSortOrder)
    {

    }

    void SetFilter(long lFilter)
    {

    }

    int GetScrollPosition()
    {
        return m_iTopItem;
    }

    void SetScrollPosition(int iPosition)
    {
        m_iTopItem = iPosition;

        if (m_iTopItem > (m_vItems.GetCount() - m_cVisibleItems)) {
            m_iTopItem = std::max(0, m_vItems.GetCount() - m_cVisibleItems);
        }

        if (m_iTopItem < 0) {
            m_iTopItem = 0;
        }
                
        if (m_pScrollPane) {
            m_pScrollPane->SetPos(m_iTopItem);
        }

        NeedPaint();
    }

    void ScrollToItemByIdx(int iItem)
    {        
        if (m_vItems.GetCount() == 0)
            {
            // nothing to do
            return;
            }

        ZAssert((iItem == 0) || ((iItem > 0) && (iItem < m_vItems.GetCount())));

        // find the first and last matching item
        int iTop, iBottom;
        iTop = iBottom = iItem;
        while (iTop > 0 && m_vItems[iTop-1] == m_vItems[iTop])
            iTop--;
        while (iBottom < m_vItems.GetCount()-1 && m_vItems[iBottom+1] == m_vItems[iBottom])
            iBottom++;

        if (iBottom > LastVisibleItem())
            m_iTopItem = std::max(iBottom - (m_cVisibleItems - 1), 0);
        if (iTop < m_iTopItem)
            m_iTopItem = iTop;

        if (m_pScrollPane)
            m_pScrollPane->SetPos(m_iTopItem);            

        NeedPaint();
    }

    void ScrollPageUp()
    {
        SetScrollPosition(m_iTopItem - m_cVisibleItems);
    }

    void ScrollPageDown()
    {
        SetScrollPosition(m_iTopItem + m_cVisibleItems);
    }

    void ScrollTop()
    {
        SetScrollPosition(0);
    }

    void ScrollBottom()
    {
        SetScrollPosition(m_vItems.GetCount() - m_cVisibleItems);
    }

    bool IsItemVisible(ListItem* pListItem)
    {
        return IsItemVisibleByIdx(GetItemIdx(pListItem));
    }

    bool IsItemVisibleByIdx(long iItem)
    {
        return (iItem >= m_iTopItem) && (iItem < m_iTopItem + m_cVisibleItems);
    }

    bool IsItemVisibleByData(long lItemData)
    {
        return IsItemVisibleByIdx(GetItemIdxByData(lItemData));
    }

    MouseResult Button(IInputProvider* pprovider, const Point& point, int button, bool bCaptured, bool bInside, bool bDown)
    {
        if (point.X() < m_pBlankPane->GetSize().X())
            {
            if (button == 0 || button == 1) // TE: Allowed rightclicks to trigger button event
            {
                if (bDown) 
                    {
                    int iItem = PointToIdx(point);
                    if (iItem < m_vItems.GetCount())
                        {
                        m_bMouseSel = true; 
						
						if (button == 0)
						{	// If leftclicked and doubleclicked
							if (pprovider->IsDoubleClick())
								InvestItemByIdx(iItem);		// invest
							else
								SetSelItemByIdx(iItem);		// select if single-left clicked
						}
						else if (button == 1)	// if rightclicked, partial invest!
							PartialInvestItemByIdx(iItem);

                        ScrollToItemByIdx(iItem);
                        m_bMouseSel = false;
                        }
                    }
                }
            }
        
		//Imago 8/14/09 mouse wheel #286 backing out Spunky
        if (button == 8 && bDown) { 
            ScrollPageDown(); 
        } else if (button == 9 && bDown) { 
            ScrollPageUp();
        }
	

        return Pane::Button(pprovider, point, button, bCaptured, bInside, bDown);
    }

	//Imago #133 7/10
    void NextItem()
    {
        if (GetSelItemIdx() < m_vItems.GetCount() - 1) {
			SetSelItemByIdx(GetSelItemIdx()+1);
			if ((GetScrollPosition() + m_cVisibleItems) <= GetSelItemIdx())
				ScrollPageDown();
		}
    }
    void PreviousItem()
    {
        if (GetSelItemIdx() != 0) {
			SetSelItemByIdx(GetSelItemIdx()-1);
			if (GetScrollPosition() > GetSelItemIdx())
				ScrollPageUp();
		}
    }
	//

    void MouseOverPoint(const Point& point)
    {
        if (point.X() < m_pBlankPane->GetSize().X() && point.X() > 0)
            {
            int iItem = PointToIdx(point);
            if (iItem < m_vItems.GetCount())
                {
                m_pMouseOverEventSource->Trigger(iItem);
                }
            else
                {
                m_pMouseOverEventSource->Trigger(-1);
                }
            }
        else
            {
            m_pMouseOverEventSource->Trigger(-1);
            }
    }

    void MouseMove(IInputProvider* pprovider, const Point& point, bool bCaptured, bool bInside)
    { 
        if (bInside)
            MouseOverPoint(point);

        Pane::MouseMove(pprovider, point, bCaptured, bInside); 
    }

    void MouseEnter(IInputProvider* pprovider, const Point& point)
    { 
        MouseOverPoint(point);
        Pane::MouseEnter(pprovider, point); 
    }

    void MouseLeave(IInputProvider* pprovider)
    { 
        m_pMouseOverEventSource->Trigger(-1);

        Pane::MouseLeave(pprovider); 
    }

    bool MouseSel()
    {
        // this is somewhat of a hack to allow things being notified by the
        // List to tell if it was from a mouse click or via something being added or deleted
        return m_bMouseSel;
    }

    bool OnScroll(int value)
    {
        m_iTopItem = value;
        if (m_iTopItem < 0)
            m_iTopItem = 0;
        NeedPaint();
        m_pEventScroll->Trigger();
        return true;
    }

private:

    int PointToIdx(const Point& point)
    {
        return (int)point.Y()/m_nItemHeight + m_iTopItem;
    }

    int LastVisibleItem() const
    {
        return m_iTopItem + m_cVisibleItems - 1; 
    }

    void UpdateLayout()
    {
        DefaultUpdateLayout();
    }

//#ifdef BUILD_DX9
#if 0
    void Paint( Surface * pSurface )
	{
        if (m_pImageBkgnd)
		{	
			// TBD: See original function below.
			_ASSERT( false );
		}

        // calc num Items to draw
        int iLastVisibleItem = LastVisibleItem();
        int iLastItem = m_vItems.GetCount() - 1;
        if (iLastVisibleItem > iLastItem)
            iLastVisibleItem = iLastItem;

        // draw each Item
        WinRect rectPaint = WinRect(0, 0, m_nItemWidth, YSize());
        WinRect rectItem = rectPaint;
        rectItem.bottom = rectItem.top;
        ZAssert(m_iTopItem >= 0);

        // count the number of slots for the first item which we are not drawing
        int nNumHiddenSlots = 0;
        
        if (m_vItems.GetCount() > 0)
        {
            while (m_iTopItem - nNumHiddenSlots > 0 
                && m_vItems[m_iTopItem - (nNumHiddenSlots + 1)] == m_vItems[m_iTopItem])
                nNumHiddenSlots++;

			for (int iItem = m_iTopItem;
						iItem <= iLastVisibleItem; 
						iItem += m_vItems[iItem]->GetItemHeight() - nNumHiddenSlots, nNumHiddenSlots = 0 )
			{
				rectItem.top = rectItem.bottom;
				int nLinesLeft = (iLastVisibleItem - iItem) + 1;
				int nLines = m_vItems[iItem]->GetItemHeight() - nNumHiddenSlots;
				rectItem.bottom += m_nItemHeight * (nLines > nLinesLeft ? nLinesLeft : nLines);

				// draw highlight if selected
				bool bItemSel = (m_iSelItem == iItem);
				if (bItemSel && m_pImageBkgndSel)
				{
					// TBD: See original function below.
					_ASSERT( false );
				}

				// draw item
				m_vItems[iItem]->DrawItem(pSurface, rectItem, bItemSel, nNumHiddenSlots);
			}
		}
	}

#else
    void Paint(Surface* pSurface)
    {

        if (m_pImageBkgnd)
            pSurface->BitBlt(WinPoint(0, 0), 
                m_pImageBkgnd->GetSurface(),
                WinRect(m_ptImgOrigin.X(), m_ptImgOrigin.Y(),
                    m_ptImgOrigin.X() + XSize(), m_ptImgOrigin.Y() + YSize()));

        // calc num Items to draw
        int iLastVisibleItem = LastVisibleItem();
        int iLastItem = m_vItems.GetCount() - 1;
        if (iLastVisibleItem > iLastItem)
            iLastVisibleItem = iLastItem;

        // draw each Item
        WinRect rectPaint = WinRect(0, 0, m_nItemWidth, YSize());
        WinRect rectItem = rectPaint;
        rectItem.bottom = rectItem.top;
        ZAssert(m_iTopItem >= 0);

        // count the number of slots for the first item which we are not drawing
        int nNumHiddenSlots = 0;
        
        if (m_vItems.GetCount() > 0)
            {
            while (m_iTopItem - nNumHiddenSlots > 0 
                && m_vItems[m_iTopItem - (nNumHiddenSlots + 1)] == m_vItems[m_iTopItem])
                nNumHiddenSlots++;

            for (int iItem = m_iTopItem;
                    iItem <= iLastVisibleItem; 
                    iItem += m_vItems[iItem]->GetItemHeight() - nNumHiddenSlots, nNumHiddenSlots = 0)
                {
                rectItem.top = rectItem.bottom;
                int nLinesLeft = (iLastVisibleItem - iItem) + 1;
                int nLines = m_vItems[iItem]->GetItemHeight() - nNumHiddenSlots;
                rectItem.bottom += m_nItemHeight * (nLines > nLinesLeft ? nLinesLeft : nLines);

                // draw highlight if selected
                bool bItemSel = (m_iSelItem == iItem);
                if (bItemSel && m_pImageBkgndSel)
                    pSurface->BitBlt(rectItem.Min(), 
                        m_pImageBkgndSel->GetSurface(),
                        rectItem);
                // draw item
                m_vItems[iItem]->DrawItem(pSurface, rectItem, bItemSel, nNumHiddenSlots);
                }
            }
    }
#endif // BUILD_DX9

};

TRef<ListPaneOld> ListPaneOld::Create(WinPoint size, int nItemHeight, bool bScroll, Pane* ppane)
{
    return new ListPaneOldImpl(size, nItemHeight, bScroll, ppane);
}

//////////////////////////////////////////////////////////////////////////////
//
// AnimatedTransformImage
//
//////////////////////////////////////////////////////////////////////////////

class AnimatedTranslateTransform2Impl : public AnimatedTranslateTransform2
{
private:
    int   m_side;
    Point m_pointStart;
    Point m_pointEnd;
    bool  m_bGotoStart;
    bool  m_bStationary;
    float m_timeStart;
    float m_dtimeAnimation;

public:
    AnimatedTranslateTransform2Impl(
        Number* ptime,
        RectValue* prect,
        int side,
        const Point& vecStart,
        const Point& vecEnd,
        float dtimeAnimation
    ) :
        AnimatedTranslateTransform2(ptime, prect),
        m_side(side),
        m_pointStart(vecStart),
        m_pointEnd(vecEnd),
        m_dtimeAnimation(dtimeAnimation),
        m_bGotoStart(true),
        m_bStationary(true),
        m_timeStart(ptime->GetValue() - dtimeAnimation)
    {
        Evaluate();
    }

    void MoveStart()
    {
        m_timeStart  = GetTime()->GetValue();
        m_bGotoStart = true;
        m_bStationary = false;
        Changed();
    }

    void MoveEnd()
    {
        m_timeStart  = GetTime()->GetValue();
        m_bGotoStart = false;
        m_bStationary = false;
        Changed();
    }

    ZString GetFunctionName() 
    { 
        return "AnimatedTranslateTransform2"; 
    }

private:

    void Evaluate()
    {
        GetValueInternal().SetTranslate(
            GetSidePoint(m_side) + GetAnimatedPoint()
        );
    }

    Point GetSidePoint(int side)
    {
        const Rect& rect = GetRect()->GetValue();
        float xmin  = rect.XMin();
        float xmax  = rect.XMax();
        float xhalf = (xmin + xmax) / 2;
        float ymin  = rect.YMin();
        float ymax  = rect.YMax();
        float yhalf = (ymin + ymax) / 2;

        switch (side) {
            case ATLeft:        return Point(xmin,  yhalf);
            case ATTopLeft:     return Point(xmin,  ymax );
            case ATTop:         return Point(xhalf, ymax );
            case ATTopRight:    return Point(xmax,  ymax );
            case ATRight:       return Point(xmax,  yhalf);
            case ATBottomRight: return Point(xmax,  ymin );
            case ATBottom:      return Point(xhalf, ymin );
            case ATBottomLeft:  return Point(xmin,  ymin );
            case ATCenter:      return Point(xhalf, yhalf);

            default:
                ZError("Unknown side");
                return Point::GetZero();
        }
    }

    Point GetAnimatedPoint()
    {
        if (m_bStationary)
        {
            return (m_bGotoStart)
                   ? m_pointStart
                   : m_pointEnd;
        }
        else
        {
            float dPos = (GetTime()->GetValue() - m_timeStart) / m_dtimeAnimation;

            if (dPos >= 1.0)
            {
                dPos = 1.0;
                m_bStationary = true;
                if (m_bGotoStart)
                    Trigger();
            }

            return m_bGotoStart
                   ? (m_pointEnd + (m_pointStart - m_pointEnd) * dPos)
                   : (m_pointStart + (m_pointEnd - m_pointStart) * dPos);
        }
    }
};

TRef<AnimatedTranslateTransform2> AnimatedTranslateTransform2::Create(
    Number* ptime,
    RectValue* prect,
    int side, 
    const Point& vecStart,
    const Point& vecEnd,
    float dtimeAnimation
) {
    return 
        new AnimatedTranslateTransform2Impl(
            ptime, 
            prect, 
            side,
            vecStart, 
            vecEnd, 
            dtimeAnimation
        );
}
