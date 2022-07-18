#ifndef __PigDirectPlay_h__
#define __PigDirectPlay_h__

/////////////////////////////////////////////////////////////////////////////
// PigDirectPlay.h : Declaration and implementation of the CPigDirectPlay
// class.
//

#include <..\TCLib\AutoCriticalSection.h>
#include <..\TCLib\ObjectLock.h>
#include <messagecore.h>

#define IDirectPlayX IDirectPlay8Client
#define IID_IDirectPlayX IID_IDirectPlay8Client

/////////////////////////////////////////////////////////////////////////////
// Macros to assist with the implementation of thunks.
//

#define CPigDirectPlay_Method0(fnName)                                      \
  STDMETHODIMP fnName()                                                     \
  {                                                                         \
    XLockStatic lock(GetCS());                                              \
    return m_pdp->fnName();                                                 \
  }

#define CPigDirectPlay_Method1(fnName,t1)                                   \
  STDMETHODIMP fnName(t1 p1)                                                \
  {                                                                         \
    XLockStatic lock(GetCS());                                              \
    return m_pdp->fnName(p1);                                               \
  }

#define CPigDirectPlay_Method2(fnName,t1,t2)                                \
  STDMETHODIMP fnName(t1 p1,t2 p2)                                          \
  {                                                                         \
    XLockStatic lock(GetCS());                                              \
    return m_pdp->fnName(p1,p2);                                            \
  }

#define CPigDirectPlay_Method3(fnName,t1,t2,t3)                             \
  STDMETHODIMP fnName(t1 p1,t2 p2,t3 p3)                                    \
  {                                                                         \
    XLockStatic lock(GetCS());                                              \
    return m_pdp->fnName(p1,p2,p3);                                         \
  }

#define CPigDirectPlay_Method4(fnName,t1,t2,t3,t4)                          \
  STDMETHODIMP fnName(t1 p1,t2 p2,t3 p3,t4 p4)                              \
  {                                                                         \
    XLockStatic lock(GetCS());                                              \
    return m_pdp->fnName(p1,p2,p3,p4);                                      \
  }

#define CPigDirectPlay_Method5(fnName,t1,t2,t3,t4,t5)                       \
  STDMETHODIMP fnName(t1 p1,t2 p2,t3 p3,t4 p4,t5 p5)                        \
  {                                                                         \
    XLockStatic lock(GetCS());                                              \
    return m_pdp->fnName(p1,p2,p3,p4,p5);                                   \
  }

#define CPigDirectPlay_Method6(fnName,t1,t2,t3,t4,t5,t6)                    \
  STDMETHODIMP fnName(t1 p1,t2 p2,t3 p3,t4 p4,t5 p5,t6 p6)                  \
  {                                                                         \
    XLockStatic lock(GetCS());                                              \
    return m_pdp->fnName(p1,p2,p3,p4,p5,p6);                                \
  }

#define CPigDirectPlay_Method7(fnName,t1,t2,t3,t4,t5,t6,t7)                 \
  STDMETHODIMP fnName(t1 p1,t2 p2,t3 p3,t4 p4,t5 p5,t6 p6,t7 p7)            \
  {                                                                         \
    XLockStatic lock(GetCS());                                              \
    return m_pdp->fnName(p1,p2,p3,p4,p5,p6,p7);                             \
  }

#define CPigDirectPlay_Method8(fnName,t1,t2,t3,t4,t5,t6,t7,t8)              \
  STDMETHODIMP fnName(t1 p1,t2 p2,t3 p3,t4 p4,t5 p5,t6 p6,t7 p7,t8 p8)      \
  {                                                                         \
    XLockStatic lock(GetCS());                                              \
    return m_pdp->fnName(p1,p2,p3,p4,p5,p6,p7,p8);                          \
  }

#define CPigDirectPlay_Method9(fnName,t1,t2,t3,t4,t5,t6,t7,t8,t9)           \
  STDMETHODIMP fnName(t1 p1,t2 p2,t3 p3,t4 p4,t5 p5,t6 p6,t7 p7,t8 p8,t9 p9)\
  {                                                                         \
    XLockStatic lock(GetCS());                                              \
    return m_pdp->fnName(p1,p2,p3,p4,p5,p6,p7,p8,p9);                       \
  }

#define CPigDirectPlay_Method0_NoLock(fnName)                               \
  STDMETHODIMP fnName()                                                     \
  {                                                                         \
    return m_pdp->fnName();                                                 \
  }

#define CPigDirectPlay_Method1_NoLock(fnName,t1)                            \
  STDMETHODIMP fnName(t1 p1)                                                \
  {                                                                         \
    return m_pdp->fnName(p1);                                               \
  }

#define CPigDirectPlay_Method2_NoLock(fnName,t1,t2)                         \
  STDMETHODIMP fnName(t1 p1,t2 p2)                                          \
  {                                                                         \
    return m_pdp->fnName(p1,p2);                                            \
  }

#define CPigDirectPlay_Method3_NoLock(fnName,t1,t2,t3)                      \
  STDMETHODIMP fnName(t1 p1,t2 p2,t3 p3)                                    \
  {                                                                         \
    return m_pdp->fnName(p1,p2,p3);                                         \
  }

#define CPigDirectPlay_Method4_NoLock(fnName,t1,t2,t3,t4)                   \
  STDMETHODIMP fnName(t1 p1,t2 p2,t3 p3,t4 p4)                              \
  {                                                                         \
    return m_pdp->fnName(p1,p2,p3,p4);                                      \
  }

#define CPigDirectPlay_Method5_NoLock(fnName,t1,t2,t3,t4,t5)                \
  STDMETHODIMP fnName(t1 p1,t2 p2,t3 p3,t4 p4,t5 p5)                        \
  {                                                                         \
    return m_pdp->fnName(p1,p2,p3,p4,p5);                                   \
  }

#define CPigDirectPlay_Method6_NoLock(fnName,t1,t2,t3,t4,t5,t6)             \
  STDMETHODIMP fnName(t1 p1,t2 p2,t3 p3,t4 p4,t5 p5,t6 p6)                  \
  {                                                                         \
    return m_pdp->fnName(p1,p2,p3,p4,p5,p6);                                \
  }

#define CPigDirectPlay_Method7_NoLock(fnName,t1,t2,t3,t4,t5,t6,t7)          \
  STDMETHODIMP fnName(t1 p1,t2 p2,t3 p3,t4 p4,t5 p5,t6 p6,t7 p7)            \
  {                                                                         \
    return m_pdp->fnName(p1,p2,p3,p4,p5,p6,p7);                             \
  }

#define CPigDirectPlay_Method8_NoLock(fnName,t1,t2,t3,t4,t5,t6,t7,t8)       \
  STDMETHODIMP fnName(t1 p1,t2 p2,t3 p3,t4 p4,t5 p5,t6 p6,t7 p7,t8 p8)      \
  {                                                                         \
    return m_pdp->fnName(p1,p2,p3,p4,p5,p6,p7,p8);                          \
  }

#define CPigDirectPlay_Method9_NoLock(fnName,t1,t2,t3,t4,t5,t6,t7,t8,t9)    \
  STDMETHODIMP fnName(t1 p1,t2 p2,t3 p3,t4 p4,t5 p5,t6 p6,t7 p7,t8 p8,t9 p9)\
  {                                                                         \
    return m_pdp->fnName(p1,p2,p3,p4,p5,p6,p7,p8,p9);                       \
  }


/////////////////////////////////////////////////////////////////////////////
// Provides a simple thunking layer which just wraps each call to the
// encapsulated IDirectPlay4 interface with a static critical section.
// This is necessary at this time because it appears that Direct Play is not
// entirely thread-safe.
//
class CPigDirectPlay : public IDirectPlayX
{
// Construction
public:
  CPigDirectPlay(IDirectPlayX* pdp) :
    m_pdp(pdp)
  {
    ZAssert(m_pdp);
  }
private:
  CPigDirectPlay(const CPigDirectPlay&); // Disable copy constructor

// Operations
public:
  static void Lock()
  {
    GetCS()->Lock();
  }
  static void Unlock()
  {
    GetCS()->Unlock();
  }

// IUnknown Interface Methods
public:
  STDMETHODIMP QueryInterface(REFIID riid, LPVOID * ppvObj)
  {
    if (
      IID_IUnknown     == riid ||
      IID_IDirectPlay2 == riid ||
      IID_IDirectPlay3 == riid ||
      IID_IDirectPlay4 == riid ||
      IID_IDirectPlayX == riid)
    {
      *ppvObj = static_cast<IDirectPlayX*>(this);
      AddRef();
      return S_OK;
    }
    // Note: A truly robust delegator would create a delegator wrapper around
    // each interface returned from the following call, and would return the
    // wrapped interface. However, this should not be a problem for the way
    // that the IDirectPlay4 interface is being used by FedMessaging.
    return m_pdp->QueryInterface(riid, ppvObj);
  }
  STDMETHODIMP_(ULONG) AddRef()
  {
    return m_pdp->AddRef();
  }
  STDMETHODIMP_(ULONG) Release()
  {
    ULONG nRefs = m_pdp->Release();
    if (!nRefs)
      delete this;
    return nRefs;
  }

// IDirectPlay2 Interface Methods
public:
  CPigDirectPlay_Method2_NoLock(AddPlayerToGroup, DPID, DPID)
  CPigDirectPlay_Method0_NoLock(Close)
  CPigDirectPlay_Method5_NoLock(CreateGroup, LPDPID,LPDPNAME,LPVOID,DWORD,DWORD)
  CPigDirectPlay_Method6       (CreatePlayer, LPDPID,LPDPNAME,HANDLE,LPVOID,DWORD,DWORD)
  CPigDirectPlay_Method2_NoLock(DeletePlayerFromGroup, DPID,DPID)
  CPigDirectPlay_Method1_NoLock(DestroyGroup, DPID)
  CPigDirectPlay_Method1       (DestroyPlayer, DPID)
  CPigDirectPlay_Method5_NoLock(EnumGroupPlayers, DPID,LPGUID,LPDPENUMPLAYERSCALLBACK2,LPVOID,DWORD)
  CPigDirectPlay_Method4_NoLock(EnumGroups, LPGUID,LPDPENUMPLAYERSCALLBACK2,LPVOID,DWORD)
  CPigDirectPlay_Method4       (EnumPlayers, LPGUID,LPDPENUMPLAYERSCALLBACK2,LPVOID,DWORD)
  CPigDirectPlay_Method5       (EnumSessions, LPDPSESSIONDESC2,DWORD,LPDPENUMSESSIONSCALLBACK2,LPVOID,DWORD)
  CPigDirectPlay_Method2_NoLock(GetCaps, LPDPCAPS,DWORD)
  CPigDirectPlay_Method4_NoLock(GetGroupData, DPID,LPVOID,LPDWORD,DWORD)
  CPigDirectPlay_Method3_NoLock(GetGroupName, DPID,LPVOID,LPDWORD)
  CPigDirectPlay_Method2_NoLock(GetMessageCount, DPID, LPDWORD)
  CPigDirectPlay_Method3_NoLock(GetPlayerAddress, DPID,LPVOID,LPDWORD)
  CPigDirectPlay_Method3_NoLock(GetPlayerCaps, DPID,LPDPCAPS,DWORD)
  CPigDirectPlay_Method4_NoLock(GetPlayerData, DPID,LPVOID,LPDWORD,DWORD)
  CPigDirectPlay_Method3_NoLock(GetPlayerName, DPID,LPVOID,LPDWORD)
  CPigDirectPlay_Method2_NoLock(GetSessionDesc, LPVOID,LPDWORD)
  CPigDirectPlay_Method1_NoLock(Initialize, LPGUID)
  CPigDirectPlay_Method2_NoLock(Open, LPDPSESSIONDESC2,DWORD)
  CPigDirectPlay_Method5       (Receive, LPDPID,LPDPID,DWORD,LPVOID,LPDWORD)
  CPigDirectPlay_Method5       (Send, DPID, DPID, DWORD, LPVOID, DWORD)
  CPigDirectPlay_Method4_NoLock(SetGroupData, DPID,LPVOID,DWORD,DWORD)
  CPigDirectPlay_Method3_NoLock(SetGroupName, DPID,LPDPNAME,DWORD)
  CPigDirectPlay_Method4_NoLock(SetPlayerData, DPID,LPVOID,DWORD,DWORD)
  CPigDirectPlay_Method3_NoLock(SetPlayerName, DPID,LPDPNAME,DWORD)
  CPigDirectPlay_Method2_NoLock(SetSessionDesc, LPDPSESSIONDESC2,DWORD)

// IDirectPlay3 Interface Methods
public:
  CPigDirectPlay_Method2_NoLock(AddGroupToGroup, DPID, DPID)
  CPigDirectPlay_Method6_NoLock(CreateGroupInGroup, DPID,LPDPID,LPDPNAME,LPVOID,DWORD,DWORD)
  CPigDirectPlay_Method2_NoLock(DeleteGroupFromGroup, DPID,DPID)
  CPigDirectPlay_Method4_NoLock(EnumConnections, LPCGUID,LPDPENUMCONNECTIONSCALLBACK,LPVOID,DWORD)
  CPigDirectPlay_Method5_NoLock(EnumGroupsInGroup, DPID,LPGUID,LPDPENUMPLAYERSCALLBACK2,LPVOID,DWORD)
  CPigDirectPlay_Method4_NoLock(GetGroupConnectionSettings, DWORD, DPID, LPVOID, LPDWORD)
  CPigDirectPlay_Method2       (InitializeConnection, LPVOID,DWORD)
  CPigDirectPlay_Method4       (SecureOpen, LPCDPSESSIONDESC2,DWORD,LPCDPSECURITYDESC,LPCDPCREDENTIALS)
  CPigDirectPlay_Method4_NoLock(SendChatMessage, DPID,DPID,DWORD,LPDPCHAT)
  CPigDirectPlay_Method3_NoLock(SetGroupConnectionSettings, DWORD,DPID,LPDPLCONNECTION)
  CPigDirectPlay_Method2_NoLock(StartSession, DWORD,DPID)
  CPigDirectPlay_Method2_NoLock(GetGroupFlags, DPID,LPDWORD)
  CPigDirectPlay_Method2_NoLock(GetGroupParent, DPID,LPDPID)
  CPigDirectPlay_Method4_NoLock(GetPlayerAccount, DPID, DWORD, LPVOID, LPDWORD)
  CPigDirectPlay_Method2_NoLock(GetPlayerFlags, DPID,LPDWORD)

// IDirectPlay4 Interface Methods
public:
  CPigDirectPlay_Method2_NoLock(GetGroupOwner, DPID, LPDPID)
  CPigDirectPlay_Method2_NoLock(SetGroupOwner, DPID, DPID)
  CPigDirectPlay_Method9       (SendEx, DPID, DPID, DWORD, LPVOID, DWORD, DWORD, DWORD, LPVOID, LPDWORD)
  CPigDirectPlay_Method5_NoLock(GetMessageQueue, DPID, DPID, DWORD, LPDWORD, LPDWORD)
  CPigDirectPlay_Method2_NoLock(CancelMessage, DWORD, DWORD)
  CPigDirectPlay_Method3_NoLock(CancelPriority, DWORD, DWORD, DWORD)

// Implementation
protected:
  static TCAutoCriticalSection* GetCS()
  {
    static TCAutoCriticalSection s_cs;
    return &s_cs;
  }

// Types
protected:
  typedef TCObjectLock<TCAutoCriticalSection> XLockStatic;

// Data Members
protected:
  IDirectPlayX* m_pdp;
};


/////////////////////////////////////////////////////////////////////////////

#endif __PigDirectPlay_h__
