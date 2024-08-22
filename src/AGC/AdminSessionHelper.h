#pragma once

/////////////////////////////////////////////////////////////////////////////
// AdminSessionHelper.h | Declaration of the CAdminSessionHelper class, which implements the
// CLSID_AdminSessionHelper component object.

#include "resource.h"
#include <AGC.h>
#include <objsafe.h>


/////////////////////////////////////////////////////////////////////////////
// CAdminSessionHelper

class ATL_NO_VTABLE CAdminSessionHelper :
  public IDispatchImpl<IAdminSessionHelper, &IID_IAdminSessionHelper, &LIBID_AGCLib>,
  public CComCoClass<CAdminSessionHelper, &CLSID_AdminSessionHelper>,
  public CComObjectRoot
{
// Declarations
public:
  DECLARE_REGISTRY_RESOURCEID(IDR_AdminSessionHelper)
  DECLARE_GET_CONTROLLING_UNKNOWN()

// Category Map
public:
  BEGIN_CATEGORY_MAP(CAdminSessionHelper)
    IMPLEMENTED_CATEGORY(CATID_AGC)
    IMPLEMENTED_CATEGORY(CATID_SafeForScripting)
    IMPLEMENTED_CATEGORY(CATID_SafeForInitializing)
  END_CATEGORY_MAP()

// Interface Map
public:
  BEGIN_COM_MAP(CAdminSessionHelper)
    COM_INTERFACE_ENTRY(IAdminSessionHelper)
    COM_INTERFACE_ENTRY2(IDispatch, IAdminSessionHelper)
  END_COM_MAP()

// Construction / Destruction
public:
  CAdminSessionHelper() {}

// IAdminSessionHelper Interface Methods
public:
  STDMETHODIMP get_IsAllSrvRunning(VARIANT_BOOL* pbIsRunning);
};


/////////////////////////////////////////////////////////////////////////////