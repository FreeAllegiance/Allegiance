#pragma once

template <class T>
class CProxy_IAGCGameEvents : public IConnectionPointImpl<T, &DIID__IAGCGameEvents, CComDynamicUnkArray>
{
  //Warning this class may be recreated by the wizard.
public:
};


template <class T>
class CProxy_IAGCShipEvents : public IConnectionPointImpl<T, &DIID__IAGCShipEvents, CComDynamicUnkArray>
{
  //Warning this class may be recreated by the wizard.
public:
};



template <class T>
class CProxy_IAGCTeamEvents : public IConnectionPointImpl<T, &DIID__IAGCTeamEvents, CComDynamicUnkArray>
{
public:

  VOID Fire_Event_Blank(BSTR Text)
  {
    T* pT = static_cast<T*>(this);
    int nConnectionIndex;
    CComVariant* pvars = new CComVariant[1];
    int nConnections = m_vec.GetSize();
    
    for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
    {
      pT->Lock();
      CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
      pT->Unlock();
      IDispatch* pDispatch = reinterpret_cast<IDispatch*>(sp.p);
      if (pDispatch != NULL)
      {
        pvars[0] = Text;
        DISPPARAMS disp = { pvars, NULL, 1, 0 };
        pDispatch->Invoke(0x1, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, NULL, NULL, NULL);
      }
    }
    delete[] pvars;
  }


};




template <class T>
class CProxy_IAGCSectorEvents : public IConnectionPointImpl<T, &DIID__IAGCSectorEvents, CComDynamicUnkArray>
{
public:

  VOID Fire_OnNewStation(BSTR Text) /*TODO: IShip* */
  {
    T* pT = static_cast<T*>(this);
    int nConnectionIndex;
    CComVariant* pvars = new CComVariant[1];
    int nConnections = m_vec.GetSize();
    
    for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
    {
      pT->Lock();
      CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
      pT->Unlock();
      IDispatch* pDispatch = reinterpret_cast<IDispatch*>(sp.p);
      if (pDispatch != NULL)
      {
        pvars[0] = Text;
        DISPPARAMS disp = { pvars, NULL, 1, 0 };
        pDispatch->Invoke(0x1, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, NULL, NULL, NULL);
      }
    }
    delete[] pvars;
  }
};