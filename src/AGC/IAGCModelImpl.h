#ifndef __IAGCModelImpl_h__
#define __IAGCModelImpl_h__

/////////////////////////////////////////////////////////////////////////////
// IAGCModelImpl.h : Declaration of the IAGCModelImpl class template.
//

#include "IAGCBaseImpl.h"


/////////////////////////////////////////////////////////////////////////////
// Interface Map Macro
//
// Classes derived from IAGCModelImpl should include this macro in their
// interface maps.
//
#define COM_INTERFACE_ENTRIES_IAGCModelImpl()                               \
    COM_INTERFACE_ENTRY(IAGCModel)                                          \
    COM_INTERFACE_ENTRY(IDispatch)                                          \
    COM_INTERFACE_ENTRIES_IAGCBaseImpl()


/////////////////////////////////////////////////////////////////////////////
// IAGCModelImpl
//
template <class T, class IGC, class ITF, const GUID* plibid, class AGCIGC = IGC, class AGCITF = ITF>
class ATL_NO_VTABLE IAGCModelImpl :
  public IAGCBaseImpl<T, IGC, ITF, plibid, AGCIGC, AGCITF>
{
// Types
public:
  typedef IAGCModelImpl<T, IGC, ITF, plibid, AGCIGC, AGCITF> IAGCModelImplBase;

// IAGCModel Interface Methods
public:
  /*-------------------------------------------------------------------------
   * get_IsVisible()
   *-------------------------------------------------------------------------
   * Purpose:
   *    
   * 
   */
  STDMETHODIMP get_IsVisible(VARIANT_BOOL* pbVisible)
  {
    ZAssert(GetIGC());
    CLEAROUT(pbVisible, VARBOOL(GetIGC()->GetVisibleF()));
    return S_OK;
  }

  /*-------------------------------------------------------------------------
   * get_IsSeenBySide()
   *-------------------------------------------------------------------------
   * Purpose:
   *    
   * 
   */
  STDMETHODIMP get_IsSeenBySide(IAGCTeam* pTeam, VARIANT_BOOL* pbSeen)
  {
    ZAssert(GetIGC());

    IAGCPrivatePtr spPrivate(pTeam);
    if (NULL == spPrivate)
      return E_INVALIDARG;

    CLEAROUT(pbSeen,
      VARBOOL(GetIGC()->SeenBySide((IsideIGC*)spPrivate->GetIGCVoid())));
    return S_OK;
  }

  /*-------------------------------------------------------------------------
   * get_Position()
   *-------------------------------------------------------------------------
   * Purpose:
   *    
   * 
   */
  STDMETHODIMP get_Position(IAGCVector** ppVector)
  {
    ZAssert(GetIGC());

    // Create an instance of the AGCVector object
    return GetAGCGlobal()->MakeAGCVector(GetIGC()->GetHitTest() ?
      &GetIGC()->GetPosition() : NULL, ppVector);
  }


  /*-------------------------------------------------------------------------
   * get_Velocity()
   *-------------------------------------------------------------------------
   * Purpose:
   *    
   * 
   */
  STDMETHODIMP get_Velocity(IAGCVector** ppVector)
  {
    ZAssert(GetIGC());

    // Create an instance of the AGCVector object
    return GetAGCGlobal()->MakeAGCVector(GetIGC()->GetHitTest() ?
      &GetIGC()->GetVelocity() : NULL, ppVector);
  }


  /*-------------------------------------------------------------------------
   * get_Orientation()
   *-------------------------------------------------------------------------
   * Purpose:
   *    
   * 
   */
  STDMETHODIMP get_Orientation(IAGCOrientation** ppOrientation)
  {
    ZAssert(GetIGC());

    // Create an instance of the AGCOrientation object
    return GetAGCGlobal()->MakeAGCOrientation(GetIGC()->GetHitTest() ?
      &GetIGC()->GetOrientation() : NULL, ppOrientation);
  }


  /*-------------------------------------------------------------------------
   * get_Radius()
   *-------------------------------------------------------------------------
   * Purpose:
   *    
   * 
   */
  STDMETHODIMP get_Radius(float* pfRadius)
  {
    ZAssert(GetIGC());
    CLEAROUT(pfRadius, GetIGC()->GetHitTest() ? GetIGC()->GetRadius() : 0.f);
    return S_OK;
  }

  /*-------------------------------------------------------------------------
   * get_Team()
   *-------------------------------------------------------------------------
   * Purpose:
   *    
   * 
   */
  STDMETHODIMP get_Team(IAGCTeam** ppTeam)
  {
    ZAssert(GetIGC());
    if (GetIGC()->GetSide())
      return GetAGCGlobal()->GetAGCObject(GetIGC()->GetSide(), IID_IAGCTeam,
        (void**)ppTeam);
    CLEAROUT(ppTeam, (IAGCTeam*)NULL);
    return S_OK;
  }

  /*-------------------------------------------------------------------------
   * get_Mass()
   *-------------------------------------------------------------------------
   * Purpose:
   *    
   * 
   */
  STDMETHODIMP get_Mass(float* pfMass)
  {
    ZAssert(GetIGC());
    CLEAROUT(pfMass, GetIGC()->GetMass());
    return S_OK;
  }

  /*-------------------------------------------------------------------------
   * get_Sector()
   *-------------------------------------------------------------------------
   * Purpose:
   *    
   * 
   */
  STDMETHODIMP get_Sector(IAGCSector** ppSector)
  {
    ZAssert(GetIGC());
    if (!GetIGC()->GetCluster())
    {
      CLEAROUT(ppSector, (IAGCSector*)NULL);
      return S_OK;
    }
    return GetAGCGlobal()->GetAGCObject(GetIGC()->GetCluster(),
      IID_IAGCSector, (void**)ppSector);
  }

  /*-------------------------------------------------------------------------
   * get_Signature()
   *-------------------------------------------------------------------------
   * Purpose:
   *    
   * 
   */
  STDMETHODIMP get_Signature(float* pfSignature)
  {
    ZAssert(GetIGC());
    CLEAROUT(pfSignature, GetIGC()->GetSignature());
    return S_OK;
  }

  /*-------------------------------------------------------------------------
   * get_Name()
   *-------------------------------------------------------------------------
   * Purpose:
   *    
   * 
   */
  STDMETHOD(get_Name)(/*[out, retval]*/ BSTR* pVal)
  {
    ZAssert(GetIGC());
    CLEAROUT(pVal, (BSTR)NULL);
    CComBSTR bstrTemp(GetModelName(GetIGC())); 
    *pVal = bstrTemp.Detach();
    return S_OK;
  }

  /*-------------------------------------------------------------------------
   * get_IsSelectable()
   *-------------------------------------------------------------------------
   * Purpose:
   *    
   * 
   */
/*  STDMETHODIMP get_IsSelectable(VARIANT_BOOL* pbSelectable)
  {
    ZAssert(GetIGC());
    // FIX make this work
    // FIX implement clearout
//    * = GetIGC()->Get;
    return S_OK;
  }
*/
  /*-------------------------------------------------------------------------
   * get_IsScanner()
   *-------------------------------------------------------------------------
   * Purpose:
   *    
   * 
   */
/*  STDMETHODIMP get_IsScanner(VARIANT_BOOL* pbScanner)
  {
    ZAssert(GetIGC());
    // FIX implement clearout
    * = GetIGC()->Get;
    return S_OK;
  }
*/
  /*-------------------------------------------------------------------------
   * get_IsPredictable()
   *-------------------------------------------------------------------------
   * Purpose:
   *    
   * 
   */
/*  STDMETHODIMP get_IsPredictable(VARIANT_BOOL* pbPredictable)
  {
    ZAssert(GetIGC());
    // FIX implement clearout
    * = GetIGC()->Get;
    return S_OK;
  }
*/
  /*-------------------------------------------------------------------------
   * get_IsScanRequired()
   *-------------------------------------------------------------------------
   * Purpose:
   *    
   * 
   */
/*  STDMETHODIMP get_IsScanRequired(VARIANT_BOOL* pbScanRequired)
  {
    ZAssert(GetIGC());
    // FIX implement clearout
    * = GetIGC()->Get;
    return S_OK;
  }
*/
  /*-------------------------------------------------------------------------
   * get_IsStatic()
   *-------------------------------------------------------------------------
   * Purpose:
   *    
   * 
   */
/*  STDMETHODIMP get_IsStatic(VARIANT_BOOL* pbStatic)
  {
    ZAssert(GetIGC());
    // FIX implement clearout
    * = GetIGC()->Get;
    return S_OK;
  }
*/
  /*-------------------------------------------------------------------------
   * get_IsDamagable()
   *-------------------------------------------------------------------------
   * Purpose:
   *    
   * 
   */
/*  STDMETHODIMP get_IsDamagable(VARIANT_BOOL* pbDamagable)
  {
    ZAssert(GetIGC());
    // FIX implement clearout
    * = GetIGC()->Get;
    return S_OK;
  }
*/
  /*-------------------------------------------------------------------------
   * get_IsHitable()
   *-------------------------------------------------------------------------
   * Purpose:
   *    
   * 
   */
/*  STDMETHODIMP get_IsHitable(VARIANT_BOOL* pbHitable)
  {
    ZAssert(GetIGC());
    // FIX implement clearout
    * = GetIGC()->Get;
    return S_OK;
  }
*/
};


/////////////////////////////////////////////////////////////////////////////

#endif //__IAGCModelImpl_h__
