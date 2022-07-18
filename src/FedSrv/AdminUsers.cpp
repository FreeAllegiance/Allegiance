
/*-------------------------------------------------------------------------
 * fedsrv\AdminUsers.CPP
 * 
 * Implementation of CAdminUsers
 * 
 * Owner: 
 * 
 * Copyright 1986-1999 Microsoft Corporation, All Rights Reserved
 *-----------------------------------------------------------------------*/

#include "pch.h"


/////////////////////////////////////////////////////////////////////////////
// CAdminUsers

TC_OBJECT_EXTERN_NON_CREATEABLE_IMPL(CAdminUsers)


/////////////////////////////////////////////////////////////////////////////
// ISupportErrorInfo Interface Methods

STDMETHODIMP CAdminUsers::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IAdminUsers
	};
	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}




CAdminUsers::CAdminUsers() :
    m_pIGCmission(NULL)
{
}


CAdminUsers::~CAdminUsers()
{
}

      



/*-------------------------------------------------------------------------
 * get_Count()
 *-------------------------------------------------------------------------
 * Purpose:
 *    For some internal VB script stuff.
 *
 * Returns:
 *    pnCount: the size of the collection (in elements)
 * 
 */
STDMETHODIMP CAdminUsers :: get_Count(long* pnCount)
{
  *pnCount = 0;

  long i = 0;

  if (m_pIGCmission)
  {
    const ShipListIGC * plistShip = m_pIGCmission->GetShips();

    //
    // Iterate thru all ships in mission
    //
    for(ShipLinkIGC * plinkShip = plistShip->first(); plinkShip; plinkShip = plinkShip->next())
    {
      CFSShip * pfsShip = (CFSShip *) plinkShip->data()->GetPrivateData();
      if (pfsShip->IsPlayer())
        ++i;
    }

    *pnCount = i;
  }
  else
  {
    *pnCount = (long)g.pServerCounters->cPlayersOnline;
  }

  // Indicate success
  return S_OK;
}



/*-------------------------------------------------------------------------
 * get__NewEnum()
 *-------------------------------------------------------------------------
 * Purpose:
 *    Provide iteration support for things (like VB/Javascript languages) 
 *    that use this COM object.
 */
STDMETHODIMP CAdminUsers :: get__NewEnum(IUnknown** ppunkEnum)
{
  // Clear the [out] parameter
  CLEAROUT(ppunkEnum, (IUnknown*)NULL);

  // Create a new CComEnum enumerator object
  typedef CComObject<CComEnum<IEnumVARIANT, &IID_IEnumVARIANT, VARIANT,
    _Copy<VARIANT> > > CEnum;
  CEnum* pEnum = new CEnum;
  ZAssert(NULL != pEnum);

  //
  // Copy the pCAdminGame elements into to a temporary CComVariant vector
  //

  long cTotal;
  get_Count(&cTotal);

  CComVariant* pargTemp = new CComVariant[cTotal];

  long i = 0;

  if (m_pIGCmission)
  {
    //
    // Iterate thru all ships in mission
    //
    for(ShipLinkIGC * plinkShip = m_pIGCmission->GetShips()->first(); plinkShip; plinkShip = plinkShip->next())
    {
      CFSShip * pfsShip = (CFSShip *) plinkShip->data()->GetPrivateData();
      if (pfsShip->IsPlayer())
      {
        CFSPlayer * pfsPlayer = pfsShip->GetPlayer();

        IAdminUser *pIAdminUser;

        RETURN_FAILED (pfsPlayer->CAdminSponsor<CAdminUser>::Make(IID_IAdminUser, (void**)&pIAdminUser));

        pfsPlayer->CAdminSponsor<CAdminUser>::GetLimb()->Init(pfsPlayer);

        pargTemp[i] = pIAdminUser;

        ++i;

        // at this point we now we are done.  This is not only an optimization but
        // a safeguard incase IGC is flawed.
        if (i >= cTotal) 
            break;
      }
    }
  }
  else
  {
      //
      // Iterate thru all missions
      //
      const ListFSMission * plistMission = CFSMission::GetMissions();
      for (LinkFSMission * plinkMission = plistMission->first(); plinkMission; plinkMission = plinkMission->next())
      {
            CFSMission * pfsMission = plinkMission->data();

            ImissionIGC * pIGCmission = pfsMission->GetIGCMission();

            const ShipListIGC * plistShip = pIGCmission->GetShips();

            //
            // Iterate thru all ships in mission
            //
            for(ShipLinkIGC * plinkShip = plistShip->first(); plinkShip; plinkShip = plinkShip->next())
            {
              CFSShip * pfsShip = (CFSShip *) plinkShip->data()->GetPrivateData();
              if (pfsShip->IsPlayer())
              {
                CFSPlayer * pfsPlayer = pfsShip->GetPlayer();

                IAdminUser *pIAdminUser;

                RETURN_FAILED (pfsPlayer->CAdminSponsor<CAdminUser>::Make(IID_IAdminUser, (void**)&pIAdminUser));

                pfsPlayer->CAdminSponsor<CAdminUser>::GetLimb()->Init(pfsPlayer);

                pargTemp[i] = pIAdminUser;

                ++i;

                // at this point we now we are done.  This is not only an optimization but
                // a safeguard incase IGC is flawed.
                if (i >= cTotal) goto exit_loops;
              }
          }
      }
  }

exit_loops:


  // Initialize enumerator object with the temporary CComVariant vector
  HRESULT hr = pEnum->Init(&pargTemp[0], &pargTemp[cTotal], NULL, AtlFlagCopy);

  delete [] pargTemp;

  if (SUCCEEDED(hr))
    hr = pEnum->QueryInterface(IID_IEnumVARIANT, (void**)ppunkEnum);
  if (FAILED(hr))
    delete pEnum;


  // Return the last result
  return hr;
}



/*-------------------------------------------------------------------------
 * get_Item()
 *-------------------------------------------------------------------------
 * Purpose:
 *         
 * Remarks:
 *   This is not supported because it would be O(n) time for something that
 *   usually takes O(1) time.  Also, since this is a multi-process envirnoment
 *   an index in not constant.
 */
STDMETHODIMP CAdminUsers :: get_Item(VARIANT index, IAdminUser** ppUser)
{
  *ppUser = NULL;

//  // Attempt to convert the specified VARIANT to a long
//  CComVariant var;
//  HRESULT hr = VariantChangeType(&var, &index, 0, VT_I4);
//  if (FAILED(hr))
//    return hr;
//  const int iIndex(V_I4(&var));

  return Error("get_Item[] is intentionally not supported; use FOR...EACH instead.");
}



