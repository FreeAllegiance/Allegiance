#pragma once

#include "event.h"
#include "tref.h"


//////////////////////////////////////////////////////////////////////////////
//
// List Item Identifier
//
//////////////////////////////////////////////////////////////////////////////

typedef void* ItemID;

//////////////////////////////////////////////////////////////////////////////
//
// List
//
//////////////////////////////////////////////////////////////////////////////
class List : public IObject {
public:
    virtual int           GetCount()             = 0;
    virtual ItemID        GetItem(int index)     = 0;
    virtual int           GetIndex(ItemID pitem) = 0;
    virtual ItemID        GetNext(ItemID pitem)  = 0;
    virtual IEventSource* GetChangedEvent()      = 0;
};

class EmptyList : public List {
public:
    int           GetCount()             { return 0;                        }
    ItemID        GetItem(int index)     { return NULL;                     }
    int           GetIndex(ItemID pitem) { return -1;                       }
    ItemID        GetNext(ItemID pitem)  { return NULL;                     }
    IEventSource* GetChangedEvent()      { return IEventSource::GetNever(); }
};