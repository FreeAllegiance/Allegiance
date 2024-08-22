#pragma once

#include <Utility.h>
#include <event.h>
#include <genericlist.h>
#include <tref.h>


//////////////////////////////////////////////////////////////////////////////
//
// ListEventSource
//
//////////////////////////////////////////////////////////////////////////////

class ListEventSource
{
    TRef<EventSourceImpl> m_peventSource;

public:
    ListEventSource()
        : m_peventSource(new EventSourceImpl()) {};

    ListEventSource(const ListEventSource& les)
        : m_peventSource(les.m_peventSource) {};

    void operator () ()
        { m_peventSource->Trigger(); };

    virtual IEventSource* GetEvent()
    {
        return m_peventSource;
    }
};

//////////////////////////////////////////////////////////////////////////////
//
// Slist_utlListWrapper
//
//////////////////////////////////////////////////////////////////////////////

// these could be implemented much more efficently if it's useful to do so
template <class T>
class Slist_utlListWrapper : public List, public Slist_utl<T, ListEventSource> {
public:

    typedef Slink_utl<T, ListEventSource> Link;

    virtual int           GetCount()
    {
        // review: we could change this to O(1)
        int nCount = 0;
        for (auto link = List_utl::first(); link != NULL; link = link->next())
            nCount++;

        return nCount;
    }

    virtual ItemID        GetItem(int index)
    {
        return (*this)[index];
    }

    virtual int           GetIndex(ItemID pitem)
    {
        int nIndex = 0;
        for (auto link = List_utl::first(); link != NULL; link = link->next())
        {
            if ((ItemID)link == pitem)
                return nIndex;
            nIndex++;
        }

        return -1;
    }

    virtual ItemID        GetNext(ItemID pitem)
    {
        return ((Link*)pitem)->next();
    }

    virtual IEventSource* GetChangedEvent()
    {
        return Slist_utl<T, ListEventSource>::sink.GetEvent();
    }
};


//////////////////////////////////////////////////////////////////////////////
//
// IntItemIDWrapper
//
//////////////////////////////////////////////////////////////////////////////

// unfortunately, ID=0 corresponds to NULL, so we can't use the default casts
// between ItemID and things like ShipID, since 0 is a valid ShipID
template <class T>
class IntItemIDWrapper
{
    T   m_t;

    enum { ZeroValue = ~(1 << (sizeof(T)*8 - 2)) };
public:
    IntItemIDWrapper() {}
    IntItemIDWrapper(T t) : m_t(t) {}
    IntItemIDWrapper(ItemID pitem) { m_t = ((int)pitem == ZeroValue) ? 0 : (T)pitem; }

    int operator = (T t) { m_t = t; }
    operator T () { return m_t; }
    operator ItemID () { return (ItemID)((m_t == 0) ? (T)ZeroValue : m_t); };

    bool operator == (IntItemIDWrapper<T> t) const { return m_t == t; }
    bool operator <= (IntItemIDWrapper<T> t) const { return m_t <= t; }
    bool operator >= (IntItemIDWrapper<T> t) const { return m_t >= t; }
    bool operator <  (IntItemIDWrapper<T> t) const { return m_t < t; }
    bool operator >  (IntItemIDWrapper<T> t) const { return m_t > t; }
};

//////////////////////////////////////////////////////////////////////////////
//
// TListListWrapper
//
//////////////////////////////////////////////////////////////////////////////

// these could be implemented much more efficently if it's useful to do so
template <
    class T,     
    class EqualsType  = DefaultEquals,
    class CompareType = DefaultNoCompare
>
class TListListWrapper : public List, public TList<T, EqualsType, CompareType, ListEventSource> {
public:
    using Iterator = typename TList<T, EqualsType, CompareType, ListEventSource>::Iterator;
    using ListType = TList<T, EqualsType, CompareType, ListEventSource>;

    TListListWrapper()
    {
    }

    TListListWrapper(EqualsType equals, CompareType compare)
        : TList<T, EqualsType, CompareType, ListEventSource>(equals, compare)
    {
    }

    virtual int           GetCount()
    {
        return TList<T, EqualsType, CompareType, ListEventSource>::GetCount();
    }

    virtual ItemID        GetItem(int index)
    {
        if (index < 0 || index >= GetCount())
            return NULL;
        else
            return (ItemID)((*this)[index]);
    }

    virtual int           GetIndex(ItemID pitem)
    {
        // review: returning the key may be more efficient than returning the value.
        int nIndex = 0;

        for (Iterator iter(*this); !iter.End(); iter.Next())
        {
            if ((ItemID)iter.Value() == pitem)
                return nIndex;
            nIndex++;
        }

        return -1;
    }

    virtual ItemID        GetNext(ItemID pitem)
    {
        for (Iterator iter(*this); !iter.End(); iter.Next())
        {
            if ((ItemID)iter.Value() == pitem)
            {
                iter.Next();

                if (iter.End())
                    return NULL;
                else
                    return (ItemID)iter.Value();
            }
        }

        return NULL;
    }

    virtual IEventSource* GetChangedEvent()
    {
        return ListType::GetSink().GetEvent();
    }
};


//////////////////////////////////////////////////////////////////////////////
//
// TMapListWrapper
//
//////////////////////////////////////////////////////////////////////////////

// these could be implemented much more efficently if it's useful to do so
template <class T, class U>
class TMapListWrapper : public List, public TMap<T, U, ListEventSource> {
public:
    using Iterator = typename TMap<T, U, ListEventSource>::Iterator;
    using Map = TMap<T, U, ListEventSource>;

    virtual int           GetCount()
    {
        return TMap<T, U, ListEventSource>::Count();
    }

    virtual ItemID        GetItem(int index)
    {
        Iterator iter(*this);

        for (int currentIndex = 0; !iter.End() && currentIndex < index; currentIndex++)
        {
            iter.Next();
        }

        if (iter.End())
            return NULL;
        else
            return (ItemID)iter.Value();
    }

    virtual int           GetIndex(ItemID pitem)
    {
        // review: returning the key may be more efficient than returning the value.
        int nIndex = 0;

        for (Iterator iter(*this); !iter.End(); iter.Next())
        {
            if ((ItemID)iter.Value() == pitem)
                return nIndex;
            nIndex++;
        }

        return -1;
    }

    virtual ItemID        GetNext(ItemID pitem)
    {
        for (Iterator iter(*this); !iter.End(); iter.Next())
        {
            if ((ItemID)iter.Value() == pitem)
            {
                iter.Next();

                if (iter.End())
                    return NULL;
                else
                    return (ItemID)iter.Value();
            }
        }

        return NULL;
    }

    virtual IEventSource* GetChangedEvent()
    {
        return Map::GetSink().GetEvent();
    }
};


///////////////////////////////////////////////////////////////////////////////
//
// FilteredList
//
///////////////////////////////////////////////////////////////////////////////

template <class Filter>
class FilteredList : public List
{
    TRef<List> m_list;
    Filter m_filter;

public:

    FilteredList(List* list)
        : m_list(list) {};

    FilteredList(List* list, Filter filter)
        : m_list(list), m_filter(filter) {};

    void SetFilter(Filter filter)
    {
        m_filter = filter;
    }

    virtual int           GetCount()
    {
        // this could be O(n) if we hooked the "changed" event
        int nCount = 0;
        ItemID currentItem = m_list->GetItem(0);

        while (currentItem != NULL)
        {
            if (m_filter(currentItem))
                nCount++;
            currentItem = m_list->GetNext(currentItem);
        }

        return nCount;
    }

    virtual ItemID        GetItem(int index)
    {
        int nCurrentIndex = 0;
        ItemID currentItem = m_list->GetItem(0);

        while (currentItem != NULL)
        {
            if (m_filter(currentItem))
            {
                if (nCurrentIndex == index)
                    break;
                else
                    nCurrentIndex++;
            }
            currentItem = m_list->GetNext(currentItem);
        }

        return currentItem;
    }

    virtual int           GetIndex(ItemID pitem)
    {
        // review: returning the key may be more efficient than returning the value.
        int nIndex = 0;
        ItemID currentItem = m_list->GetItem(0);

        while (currentItem != NULL)
        {
            if (m_filter(currentItem))
            {
                if (currentItem == pitem)
                    return nIndex;
                else
                    nIndex++;
            }
            currentItem = m_list->GetNext(currentItem);
        }

        return -1;
    }

    virtual ItemID        GetNext(ItemID pitem)
    {
        ItemID currentItem = m_list->GetNext(pitem);

        while (currentItem != NULL)
        {
            if (m_filter(currentItem))
            {
                return currentItem;
            }
            currentItem = m_list->GetNext(currentItem);
        }

        return NULL;
    }

    virtual IEventSource* GetChangedEvent()
    {
        return m_list->GetChangedEvent();
    }
};


///////////////////////////////////////////////////////////////////////////////
//
// ListDelegate
//
///////////////////////////////////////////////////////////////////////////////
class ListDelegate : public List
{
private:
    List* m_plist;
    TRef<IObject> m_pcontainer;

public:

    ListDelegate(List* plist, IObject* pcontainer = NULL)
        : m_plist(plist), m_pcontainer(pcontainer) {}

    virtual int           GetCount()
    {
        return m_plist->GetCount();
    }

    virtual ItemID        GetItem(int index)
    {
        return m_plist->GetItem(index);
    }

    virtual int           GetIndex(ItemID pitem)
    {
        return m_plist->GetIndex(pitem);
    }

    virtual ItemID        GetNext(ItemID pitem)
    {
        return m_plist->GetNext(pitem);
    }

    virtual IEventSource* GetChangedEvent()
    {
        return m_plist->GetChangedEvent();
    }
};




///////////////////////////////////////////////////////////////////////////////
//
// ConcatinatedList
//
///////////////////////////////////////////////////////////////////////////////

class ConcatinatedList : public List, public IEventSink
{
    TRef<List> m_list1;
    TRef<List> m_list2;
    ListEventSource m_pfnChanged;
    TRef<IEventSink> m_sinkDelegate;

public:

    ConcatinatedList(List* list1, List* list2)
        : m_list1(list1), m_list2(list2) 
    {
        // add this to the changed notifications of the sublists
        m_sinkDelegate = IEventSink::CreateDelegate(this);
        m_list1->GetChangedEvent()->AddSink(m_sinkDelegate);
        m_list2->GetChangedEvent()->AddSink(m_sinkDelegate);
    };

    ~ConcatinatedList()
    {
        m_list1->GetChangedEvent()->RemoveSink(m_sinkDelegate);
        m_list2->GetChangedEvent()->RemoveSink(m_sinkDelegate);
    }

    virtual int           GetCount()
    {
        return m_list1->GetCount() + m_list2->GetCount();
    }

    virtual ItemID        GetItem(int index)
    {
        int nList1Count = m_list1->GetCount();

        if (index < nList1Count)
            return m_list1->GetItem(index);
        else
            return m_list2->GetItem(index - nList1Count);
    }

    virtual int           GetIndex(ItemID pitem)
    {
        int index = m_list1->GetIndex(pitem);

        if (index == -1)
        {
            index = m_list2->GetIndex(pitem);

            if (index != -1) 
                index += m_list1->GetCount();
        }

        return index;
    }

    virtual ItemID        GetNext(ItemID pitem)
    {
        // case 1: pitem and pitem->next are in list1
        ItemID next = m_list1->GetNext(pitem);

        if (next == NULL)
        {
            // case 2: pitem and pitem->next are in list2
            next = m_list2->GetNext(pitem);

            if ((next == NULL) && (m_list1->GetIndex(pitem) != -1))
            {
                // case 3: pitem is in list1, pitem next is in list2
                next = m_list2->GetItem(0);
            }
        }

        return next;
    }

    virtual IEventSource* GetChangedEvent()
    {
        return m_pfnChanged.GetEvent();
    }

    virtual bool OnEvent(IEventSource* pevent)
    {
        m_pfnChanged();
        return true;
    }
};



///////////////////////////////////////////////////////////////////////////////
//
// SortedList
//
///////////////////////////////////////////////////////////////////////////////

typedef bool (*ItemIDCompareFunction)(ItemID id1, ItemID id2);

template <class CompareFunctionType>
class SortedList : public List, public IEventSink
{
    // a sorted list of the items in question
    typedef TList<ItemID, DefaultEquals, CompareFunctionType> SortedListType;
    SortedListType m_listSorted;
    
    TRef<List> m_list;
    bool m_fNeedsSort;
    ListEventSource m_pfnChanged;
    TRef<IEventSink> m_sinkDelegate;

    void CheckSorting()
    {
        if (m_fNeedsSort)
        {
            m_listSorted.SetEmpty();

            // use an insertion sort - O(n^2), but it's a stable sort
            ItemID pitem = m_list->GetItem(0);

            while (pitem) {
                m_listSorted.InsertSorted(pitem);

                pitem = m_list->GetNext(pitem);
            }
            
            m_fNeedsSort = false;
        }
    }

public:

    SortedList(List* list, CompareFunctionType fnCompare)
        : m_list(list), m_listSorted(DefaultEquals(), fnCompare), m_fNeedsSort(true)
    {
        // add this to the changed notifications of the unsorted list
        m_sinkDelegate = IEventSink::CreateDelegate(this);
        m_list->GetChangedEvent()->AddSink(m_sinkDelegate);
    };

    ~SortedList()
    {
        m_list->GetChangedEvent()->RemoveSink(m_sinkDelegate);
    }

    virtual int           GetCount()
    {
        return m_list->GetCount();
    }

    virtual ItemID        GetItem(int index)
    {
        CheckSorting();

        typename SortedListType::Iterator iter(m_listSorted);

        for (int currentIndex = 0; !iter.End() && currentIndex < index; currentIndex++)
        {
            iter.Next();
        }

        if (iter.End())
            return NULL;
        else
            return (ItemID)iter.Value();
    }

    virtual int           GetIndex(ItemID pitem)
    {
        CheckSorting();

        int nIndex = 0;

        for (typename SortedListType::Iterator iter(m_listSorted); !iter.End(); iter.Next())
        {
            if ((ItemID)iter.Value() == pitem)
                return nIndex;
            nIndex++;
        }

        return -1;
    }

    virtual ItemID        GetNext(ItemID pitem)
    {
        CheckSorting();

        for (typename SortedListType::Iterator iter(m_listSorted); !iter.End(); iter.Next())
        {
            if ((ItemID)iter.Value() == pitem)
            {
                iter.Next();

                if (iter.End())
                    return NULL;
                else
                    return (ItemID)iter.Value();
            }
        }

        return NULL;
    }

    virtual IEventSource* GetChangedEvent()
    {
        return m_pfnChanged.GetEvent();
    }

    virtual bool OnEvent(IEventSource* pevent)
    {
        m_fNeedsSort = true;
        m_pfnChanged();
        return true;
    }
};



///////////////////////////////////////////////////////////////////////////////
//
// SingletonList
//
///////////////////////////////////////////////////////////////////////////////

class SingletonList : public List
{
    ListEventSource m_pfnChanged;
    ItemID m_itemID;

public:

    SingletonList(ItemID itemID = NULL)
        : m_itemID(itemID)
    {
    }

    void SetItem(ItemID itemID)
    {
        m_itemID = itemID;
        m_pfnChanged();
    }

    //
    // List interface:
    //

    virtual int           GetCount()
    {
        return 1;
    }

    virtual ItemID        GetItem(int index)
    {
        if (index == 0)
            return m_itemID;
        else
            return NULL;
    }

    virtual int           GetIndex(ItemID pitem)
    {
        if (pitem = m_itemID)
            return 0;
        else 
            return -1;
    }

    virtual ItemID        GetNext(ItemID pitem)
    {
        return NULL;
    }

    virtual IEventSource* GetChangedEvent()
    {
        return m_pfnChanged.GetEvent();
    }
};