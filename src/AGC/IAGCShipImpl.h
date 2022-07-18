#ifndef __IAGCShipImpl_h__
#define __IAGCShipImpl_h__

/////////////////////////////////////////////////////////////////////////////
// IAGCShipImpl.h : Declaration of the IAGCShipImpl class template.
//

#include "IAGCScannerImpl.h"
#include "resource.h"
#include "AGCChat.h"


/////////////////////////////////////////////////////////////////////////////
// Interface Map Macro
//
// Classes derived from IAGCShipImpl should include this macro in their
// interface maps.
//
#define COM_INTERFACE_ENTRIES_IAGCShipImpl()                                \
    COM_INTERFACE_ENTRY(IAGCShip)                                           \
    COM_INTERFACE_ENTRY(IDispatch)                                          \
    COM_INTERFACE_ENTRIES_IAGCScannerImpl()


/////////////////////////////////////////////////////////////////////////////
// IAGCShipImpl
//
template <class T, class IGC, class ITF, const GUID* plibid>
class ATL_NO_VTABLE IAGCShipImpl : 
  public IAGCScannerImpl<T, IGC, ITF, plibid, IshipIGC, IAGCShip>
{
// Types
public:
  typedef IAGCShipImpl<T, IGC, ITF, plibid> IAGCShipImplBase;

// IAGCShip Interface Methods
public:
  STDMETHODIMP put_Ammo(short Val)
  {
    // Property is read-only at the AGC level
    return AtlReportError(T::GetObjectCLSID(), IDS_E_PROP_READONLY,
      __uuidof(ITF), MAKE_HRESULT(1, FACILITY_ITF, IDS_E_PROP_READONLY),
      GetAGCGlobal()->GetResourceInstance());
  }

  STDMETHODIMP get_Ammo(short* pVal)
  {
    ZAssert(GetIGC());
    CLEAROUT(pVal, GetIGC()->GetAmmo());
    return S_OK;
  }

  STDMETHODIMP put_Fuel(float Val)
  {
    // Property is read-only at the AGC level
    return AtlReportError(T::GetObjectCLSID(), IDS_E_PROP_READONLY,
      __uuidof(ITF), MAKE_HRESULT(1, FACILITY_ITF, IDS_E_PROP_READONLY),
      GetAGCGlobal()->GetResourceInstance());
  }
  
  STDMETHODIMP get_Fuel(float* pVal)
  {
    ZAssert(GetIGC());
    CLEAROUT(pVal, GetIGC()->GetFuel());
    return S_OK;
  }

  STDMETHODIMP put_Energy(float Val)
  {
    // Property is read-only at the AGC level
    return AtlReportError(T::GetObjectCLSID(), IDS_E_PROP_READONLY,
      __uuidof(ITF), MAKE_HRESULT(1, FACILITY_ITF, IDS_E_PROP_READONLY),
      GetAGCGlobal()->GetResourceInstance());
  }
  
  STDMETHODIMP get_Energy(float* pVal)
  {
    ZAssert(GetIGC());
    CLEAROUT(pVal, GetIGC()->GetEnergy());
    return S_OK;
  }

  STDMETHODIMP put_WingID(short Val)
  {
    // Property is read-only at the AGC level
    return AtlReportError(T::GetObjectCLSID(), IDS_E_PROP_READONLY,
      __uuidof(ITF), MAKE_HRESULT(1, FACILITY_ITF, IDS_E_PROP_READONLY),
      GetAGCGlobal()->GetResourceInstance());
  }

  STDMETHODIMP get_WingID(short* pVal)
  {
    ZAssert(GetIGC());
    CLEAROUT(pVal, GetIGC()->GetWingID());
    return S_OK;
  }


  ///////////////////////////////////////////////////////////////////////////  
  STDMETHODIMP SendChat(BSTR bstrText, AGCSoundID idSound)
  {
    ZAssert(GetIGC());
    ZAssert(GetIGC()->GetMission()->GetIgcSite());

    // Do nothing if string is empty
    if (!BSTRLen(bstrText))
      return S_FALSE;

    // Send the chat
    USES_CONVERSION;
    GetIGC()->GetMission()->GetIgcSite()->SendChat(NULL, CHAT_INDIVIDUAL,
      GetIGC()->GetObjectID(), idSound, OLE2CA(bstrText), c_cidNone, NA, NA, NULL, true); 

    // Indicate success
    return S_OK;
  }


  ///////////////////////////////////////////////////////////////////////////  
  STDMETHODIMP SendCommand(BSTR bstrCommand, IAGCModel* pModelTarget,
    AGCSoundID idSound)
  {
    ZAssert(GetIGC());
    ZAssert(GetIGC()->GetMission()->GetIgcSite());

    // Convert the command string to a command ID
    CommandID idCmd;
    RETURN_FAILED(FindCommandName(bstrCommand, &idCmd, T::GetObjectCLSID(),
      IID_IAGCShip));

    // Verify that the specified model supports IAGCPrivate
    IAGCPrivatePtr spPrivate(pModelTarget);
    if (NULL == spPrivate)
      return E_INVALIDARG;

    // Get the target's IGC pointer
    ImodelIGC* pModel = reinterpret_cast<ImodelIGC*>(spPrivate->GetIGCVoid());

    // Send the chat
    GetIGC()->GetMission()->GetIgcSite()->SendChat(NULL, CHAT_INDIVIDUAL,
      GetIGC()->GetObjectID(), idSound, "", idCmd, pModel->GetObjectType(),
      pModel->GetObjectID(), pModel, true);
      
    // Indicate success
    return S_OK;
  }

  ///////////////////////////////////////////////////////////////////////////  
  STDMETHODIMP put_AutoDonate(IAGCShip* pShip)
  {
    // Property is read-only at the AGC level
    return AtlReportError(T::GetObjectCLSID(), IDS_E_PROP_READONLY,
      __uuidof(ITF), MAKE_HRESULT(1, FACILITY_ITF, IDS_E_PROP_READONLY),
      GetAGCGlobal()->GetResourceInstance());
  }

  ///////////////////////////////////////////////////////////////////////////  
  STDMETHODIMP get_AutoDonate(IAGCShip** ppShip)
  {
    ZAssert(GetIGC());
    if (GetIGC()->GetAutoDonate())
      GetAGCGlobal()->GetAGCObject(GetIGC()->GetAutoDonate(), IID_IAGCShip,
        (void**)ppShip);
    else
      CLEAROUT(ppShip, (IAGCShip*)NULL);
    return S_OK;
  }

  ///////////////////////////////////////////////////////////////////////////  
  STDMETHODIMP put_ShieldFraction(float Val)
  {
    ZAssert(GetIGC());

    IshieldIGC* pshield = (IshieldIGC*)(GetIGC()->GetMountedPart(ET_Shield, 0));
    if (pshield)
      pshield->SetFraction(Val);

    return S_OK;
  }

  ///////////////////////////////////////////////////////////////////////////  
  STDMETHODIMP get_ShieldFraction(float* pVal)
  {
    ZAssert(GetIGC());

    IshieldIGC* pshield = (IshieldIGC*)(GetIGC()->GetMountedPart(ET_Shield, 0));
    if (pshield)
    {
      CLEAROUT(pVal, pshield->GetFraction());   
    }
    else
      CLEAROUT(pVal, 0.0f);   

    return S_OK;
  }

  ///////////////////////////////////////////////////////////////////////////  
  STDMETHODIMP get_HullType(IAGCHullType** ppHullType)
  {
    ZAssert(GetIGC());
    ZAssert(GetIGC()->GetHullType());
    return GetAGCGlobal()->GetAGCObject(GetIGC()->GetHullType(),
      IID_IAGCHullType, (void**)ppHullType);
  }

  ///////////////////////////////////////////////////////////////////////////  
  STDMETHODIMP get_BaseHullType(IAGCHullType** ppHullType)
  {
    ZAssert(GetIGC());
    if (GetIGC()->GetBaseHullType())
      return GetAGCGlobal()->GetAGCObject(GetIGC()->GetBaseHullType(),
        IID_IAGCHullType, (void**)ppHullType);
    else
      return NULL;
  }

};


/////////////////////////////////////////////////////////////////////////////

#endif //__IAGCShipImpl_h__
