#pragma once

/////////////////////////////////////////////////////////////////////////////
// AGCHullType.h : Declaration of the CAGCHullType
//

#include "resource.h"
#include "IAGCBuyableImpl.h"


/////////////////////////////////////////////////////////////////////////////
// CAGCHullType
//
class ATL_NO_VTABLE CAGCHullType : 
  public IAGCBuyableImpl<CAGCHullType, IhullTypeIGC, IAGCHullType, &LIBID_AGCLib>,
  public ISupportErrorInfo,
  public CComCoClass<CAGCHullType, &CLSID_AGCHullType>
{
// Declarations
public:
  DECLARE_REGISTRY_RESOURCEID(IDR_AGCHullType)
  DECLARE_PROTECT_FINAL_CONSTRUCT()

// Interface Map
public:
  BEGIN_COM_MAP(CAGCHullType)
    COM_INTERFACE_ENTRY(IAGCHullType)
    COM_INTERFACE_ENTRY(IDispatch)
    COM_INTERFACE_ENTRIES_IAGCBuyableImpl()
    COM_INTERFACE_ENTRY(ISupportErrorInfo)
  END_COM_MAP()

// Category Map
public:
  BEGIN_CATEGORY_MAP(CAGCHullType)
    IMPLEMENTED_CATEGORY(CATID_AGC)
  END_CATEGORY_MAP()

// ISupportsErrorInfo Interface Methods
public:
  STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// IAGCHullType Interface Methods
public:
  STDMETHODIMP get_Length(float* pfLength);
  STDMETHODIMP get_MaxSpeed(float* pfMaxSpeed);
  STDMETHODIMP get_MaxTurnRate(AGCAxis eAxis, float* pfMaxTurnRate);
  STDMETHODIMP get_TurnTorque(AGCAxis eAxis, float* pfTurnTorque);
  STDMETHODIMP get_Thrust(float* pfThrust);
  STDMETHODIMP get_SideMultiplier(float* pfSideMultiplier);
  STDMETHODIMP get_BackMultiplier(float* pfBackMultiplier);
  STDMETHODIMP get_ScannerRange(float* pfScannerRange);
  STDMETHODIMP get_MaxEnergy(float* pfMaxEnergy);
  STDMETHODIMP get_RechargeRate(float* pfRechargeRate);
  STDMETHODIMP get_HitPoints(AGCHitPoints* pHitPoints);
  STDMETHODIMP get_PartMask(AGCEquipmentType et, AGCMount mountID,
    AGCPartMask* pPartMask);
  STDMETHODIMP get_MaxWeapons(AGCMount* pMaxWeapons);
  STDMETHODIMP get_MaxFixedWeapons(AGCMount* pMaxFixedWeapons);
//  STDMETHODIMP get_CanMount(IAGCPartType* pPartType, AGCMount mountID,
//    VARIANT_BOOL* pbCanMount);
  STDMETHODIMP get_Mass(float* pfMass);
  STDMETHODIMP get_Signature(float* pfSignature);
  STDMETHODIMP get_Capabilities(AGCHullAbilityBitMask* phabmCapabilities);
  STDMETHODIMP get_HasCapability(AGCHullAbilityBitMask habm,
    VARIANT_BOOL* pbHasCapability);
  STDMETHODIMP get_MaxAmmo(short* pnMaxAmmo);
  STDMETHODIMP get_MaxFuel(float* pfMaxFuel);
};


/////////////////////////////////////////////////////////////////////////////