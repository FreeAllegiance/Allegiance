#pragma once

/////////////////////////////////////////////////////////////////////////////
// AGCGame.h : Declaration of the IAGCGameImpl and CAGCGame
//

#include "resource.h"
#include "IAGCGameImpl.h"


/////////////////////////////////////////////////////////////////////////////
// CAGCGame
class ATL_NO_VTABLE CAGCGame : 
  public IAGCGameImpl<CAGCGame, ImissionIGC, IAGCGame, &LIBID_AGCLib>,
  public ISupportErrorInfo,
  public CComCoClass<CAGCGame, &CLSID_AGCGame>
{
// Declarations
public:
  DECLARE_REGISTRY_RESOURCEID(IDR_AGCGame)
  DECLARE_PROTECT_FINAL_CONSTRUCT()

// Interface Map
public:
  BEGIN_COM_MAP(CAGCGame)
    COM_INTERFACE_ENTRIES_IAGCGameImpl()
    COM_INTERFACE_ENTRY(ISupportErrorInfo)
  END_COM_MAP()

// Category Map
public:
  BEGIN_CATEGORY_MAP(CAGCGame)
    IMPLEMENTED_CATEGORY(CATID_AGC)
  END_CATEGORY_MAP()

// ISupportErrorInfo Interface Methods
public:
  STDMETHODIMP InterfaceSupportsErrorInfo(REFIID riid);
};


/////////////////////////////////////////////////////////////////////////////