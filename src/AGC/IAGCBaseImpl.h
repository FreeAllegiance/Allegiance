#ifndef __IAGCBaseImpl_h__
#define __IAGCBaseImpl_h__

/////////////////////////////////////////////////////////////////////////////
// IAGCBaseImpl.h : Declaration of the IAGCBaseImpl class template.
//

#include "IAGCCommonImpl.h"


/////////////////////////////////////////////////////////////////////////////
// Interface Map Macro
//
// Classes derived from IAGCBaseImpl should include this macro in their
// interface maps.
//
#define COM_INTERFACE_ENTRIES_IAGCBaseImpl()                                \
    COM_INTERFACE_ENTRY(IAGCBase)                                           \
    COM_INTERFACE_ENTRY(IDispatch)                                          \
    COM_INTERFACE_ENTRIES_IAGCCommonImpl()


/////////////////////////////////////////////////////////////////////////////
//
// Note:  This is not a base class for all AGC objects, just AGC objects that
//        represent IGC objects that inherit IbaseIGC
//
//        All AGC object inherit IAGCCommon
//
template <class T, class IGC, class ITF, const GUID* plibid, class AGCIGC = IGC, class AGCITF = ITF>
class ATL_NO_VTABLE IAGCBaseImpl :
  public IAGCCommonImpl<T, IGC, ITF, plibid, AGCIGC, AGCITF>
{
// Types
public:
  typedef IAGCBaseImpl<T, IGC, ITF, plibid, AGCIGC, AGCITF> IAGCBaseImplBase;
  
// IAGCBase Interface Methods
public:

  /*-------------------------------------------------------------------------
   * get_ObjectType()
   *-------------------------------------------------------------------------
   * Purpose:
   *    return this object's type
   * 
   */
  STDMETHODIMP get_ObjectType(AGCObjectType* pObjectType)
  {
    ZAssert(GetIGC());
  //  CLEAROUT(pObjectType, );
    *pObjectType = (AGCObjectType) GetIGC()->GetObjectType();
    return S_OK;
  }


  /*-------------------------------------------------------------------------
   * get_ObjectID()
   *-------------------------------------------------------------------------
   * Purpose:
   *    return this object's ID
   * 
   */
  STDMETHODIMP get_ObjectID(AGCObjectID* pObjectID)
  {

    ZAssert(GetIGC());
  //  CLEAROUT(pObjectID, (AGCObjectID)m_pIGC->GetObjectID());
    *pObjectID = (AGCObjectID)GetIGC()->GetObjectID();
    return S_OK;
  }

  /*-------------------------------------------------------------------------
   * get_AGCID()
   *-------------------------------------------------------------------------
   * Purpose:
   *    return this object's unique AGC id number ID, which is a combo of its
   *  type id, and object id.
   * 
   */
  STDMETHODIMP get_UniqueID(AGCUniqueID* pObjectID)
  {
    ZAssert(GetIGC());

    #ifdef _DEBUG
      //
      //  These should be compile time asserts, but I forgot how to do that
      //
      if (sizeof(ObjectID) > sizeof(short) ||
          sizeof(ObjectType) > sizeof(short))
      {
        // this and the event system are probably broken if this triggers
        return T::Error("Recent changes in IGC broke this property, please notify the Allegiance team.");
      }
    #endif

    // merge to form unique id
    *pObjectID = (GetIGC()->GetObjectType() << 16) | GetIGC()->GetObjectID();

    return S_OK;
  }


  /*-------------------------------------------------------------------------
   * get_Game()
   *-------------------------------------------------------------------------
   * Purpose:
   *    return a AGCGame pointer of the game in which this object lives.
   * 
   */
  STDMETHODIMP get_Game(IAGCGame** ppGame)
  {
    ZAssert(GetIGC());
    ZAssert(GetIGC()->GetMission());
    return GetAGCGlobal()->GetAGCObject(GetIGC()->GetMission(), IID_IAGCGame,
      (void**)ppGame);
  }

};


/////////////////////////////////////////////////////////////////////////////

#endif //__IAGCBaseImpl_h__
