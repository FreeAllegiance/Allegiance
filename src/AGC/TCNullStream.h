#pragma once

/////////////////////////////////////////////////////////////////////////////
// TCNullStream.h | Declaration of the CTCNullStream, which implements the
// CLSID_TCNullStream component object.

#include "resource.h"
#include <AGC.h>
#include <..\TCLib\ObjectLock.h>


/////////////////////////////////////////////////////////////////////////////
// CTCNullStream

class ATL_NO_VTABLE CTCNullStream :
  public CTCNullStreamImpl,
  public CComCoClass<CTCNullStream, &CLSID_TCNullStream>
{
// Declarations
public:
  DECLARE_REGISTRY_RESOURCEID(IDR_TCNullStream)

// Category Map
public:
  BEGIN_CATEGORY_MAP(CTCNullStream)
    IMPLEMENTED_CATEGORY(CATID_AGC)
  END_CATEGORY_MAP()

// Interface Map
public:
  BEGIN_COM_MAP(CTCNullStream)
   COM_INTERFACE_ENTRY(IUnknown)
   COM_INTERFACE_ENTRY_CHAIN(CTCNullStreamImpl)
  END_COM_MAP()
};


/////////////////////////////////////////////////////////////////////////////