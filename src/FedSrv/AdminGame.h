#ifndef __ADMINGAME_H_
#define __ADMINGAME_H_

/*-------------------------------------------------------------------------
 * fedsrv\AdminGame.H
 * 
 * Declaration of the CAdminGame.  
 * 
 * Owner: 
 * 
 * Copyright 1986-1999 Microsoft Corporation, All Rights Reserved
 *-----------------------------------------------------------------------*/

#include "..\AGC\IAGCGameImpl.h"


/////////////////////////////////////////////////////////////////////////////
// Forward Declarations

class CAdminUsers;


/////////////////////////////////////////////////////////////////////////////
// CAdminGame
class ATL_NO_VTABLE CAdminGame :
  public IAGCGameImpl<CAdminGame, CFSMission, IAdminGame, &LIBID_ALLEGIANCESERVERLib>,
  public CAdminSponsor<CAdminUsers>,
	public CComCoClass<CAdminGame, &CLSID_AdminGame>
{
// Declarations
public:
  DECLARE_REGISTRY_RESOURCEID(IDR_ADMINGAME)
  DECLARE_PROTECT_FINAL_CONSTRUCT()
  DECLARE_AGC_TYPE(admin)

// Interface Map
public:
  BEGIN_COM_MAP(CAdminGame)
	  COM_INTERFACE_ENTRY(IAdminGame)
    COM_INTERFACE_ENTRIES_IAGCGameImpl()
  END_COM_MAP()

// Category Map
public:
  BEGIN_CATEGORY_MAP(CAdminGame)
    IMPLEMENTED_CATEGORY(CATID_AllegianceAdmin)
  END_CATEGORY_MAP()

// Construction / Destruction
public:
  CAdminGame();
  void Init(ImissionIGC* pIGC);

// IAGCGame Interface Methods
public:
  STDMETHODIMP get_GameParameters(IAGCGameParameters** ppParams); // overrides IAGCGameImpl

// IAdminGame Interface Methods
public:
  STDMETHODIMP SendMsg(BSTR bstrMessage);
  STDMETHODIMP Kill(); 
  STDMETHODIMP get_Users(IAdminUsers** ppUsers);
  STDMETHODIMP get_GameOwnerUser(IAdminUser** ppUser);
  STDMETHODIMP StartGame();
  STDMETHODIMP get_Description(BSTR * pbstrDescription);
  STDMETHODIMP StartCountdown();
  STDMETHODIMP SetTeamName(int iSideID, BSTR bstrName);
  STDMETHODIMP OverrideTechBit(int iSideID, int nTechBitID, BOOL bSetting);
  STDMETHODIMP SetDetailsFiles(BSTR bstrName);
  STDMETHODIMP RandomizeTeams();

public:

  // Data Members
protected:
  // A pointer to the Users collection describing the users currently playing
  // this game.  For efficiency, this is NULL until get_Users() is called.
  CAdminUsers*      m_pUsers;            
};


/////////////////////////////////////////////////////////////////////////////
      
#endif //__ADMINGAME_H_
