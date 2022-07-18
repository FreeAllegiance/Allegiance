#ifndef __PropertyClass_cpp__
#define __PropertyClass_cpp__

/////////////////////////////////////////////////////////////////////////////
// PropertyClass.cpp | Implementation of the TCComPropertyClassRoot class.
//

#include "PropertyClass.h"


/////////////////////////////////////////////////////////////////////////////
// TCComPropertyClassRoot


/////////////////////////////////////////////////////////////////////////////
// Construction

#ifdef _DEBUG

  ///////////////////////////////////////////////////////////////////////////
  // {group:Construction}
  // Parameters:
  //   pszType - The class name of the most-derived class, used only for
  // diagnostic purposes. This parameter only exists under _DEBUG builds
  TCComPropertyClassRoot::TCComPropertyClassRoot(LPCSTR pszType)
    : m_nLoadingRefs(0), m_nSavingRefs(0), m_bRequiresSave(FALSE),
    m_pszType(pszType)
  {
  }

#endif // _DEBUG
#ifndef _DEBUG

  TCComPropertyClassRoot::TCComPropertyClassRoot()
    : m_nLoadingRefs(0), m_nSavingRefs(0), m_bRequiresSave(FALSE)
  {
  }

#endif // !_DEBUG


/////////////////////////////////////////////////////////////////////////////
// Group=Attributes

/////////////////////////////////////////////////////////////////////////////
// Description: Sets or clears the object's dirty flag.
//
// Sets or clears the object's dirty flag.
//
// Parameters:
//   bDirty - *TRUE* to set the object's dirty flag, *FALSE* to clear it.
//
// See Also: TCComPropertyClassRoot::GetDirty,
// TCComPropertyClassRoot::m_bRequiresSave
void TCComPropertyClassRoot::SetDirty(BOOL bDirty)
{
  m_bRequiresSave = 0 != bDirty;
}

/////////////////////////////////////////////////////////////////////////////
// Description: Gets the current state of the object's dirty flag.
//
// Gets the current state of the object's dirty flag.
//
// Return Value: *TRUE* if the object needs to be saved, otherwise *FALSE*.
//
// See Also: TCComPropertyClassRoot::SetDirty,
// TCComPropertyClassRoot::m_bRequiresSave
BOOL TCComPropertyClassRoot::GetDirty() const
{
  return m_bRequiresSave ? TRUE : FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// Description: Returns the object's current persistence load state.
//
// Determines if the object is currently being loaded from a persistence
// mechanism.
//
// Return Value: *true* if the object is being loaded from a persistence
// mechanism, otherwise *false*.
//
// See Also: TCComPropertyClassRoot::SetLoading,
// TCComPropertyClassRoot::IsSaving, TCComPropertyClassRoot::CLoadSaveScope
bool TCComPropertyClassRoot::IsLoading()
{
  return m_nLoadingRefs != 0;
}

/////////////////////////////////////////////////////////////////////////////
// Description: Sets or clears the object's current persistence load state.
//
// Sets or clears the object's current persistence state. Actually, since
// loading from a persistence mechanism may be nested into several levels,
// this method only increments or decrements a reference count of load
// operations. Typically, this method is only used internally by the
// TCComPropertyClassRoot::CLoadSaveScope class. Usually, there is no reason
// to call it directly.
//
// Parameters:
//   bLoading - *true* specifies that a persistence load operation is about
// to be entered; *false* indicates that such an operation is finished.
//
// See Also: TCComPropertyClassRoot::IsLoading,
// TCComPropertyClassRoot::SetSaving, TCComPropertyClassRoot::CLoadSaveScope
void TCComPropertyClassRoot::SetLoading(bool bLoading)
{
  if (bLoading)
    InterlockedIncrement(&m_nLoadingRefs);
  else
    InterlockedDecrement(&m_nLoadingRefs);
}

/////////////////////////////////////////////////////////////////////////////
// Description: Returns the object's current persistence save state.
//
// Determines if the object is currently being saved to a persistence
// mechanism.
//
// Return Value: *true* if the object is being saved to a persistence
// mechanism, otherwise *false*.
//
// See Also: TCComPropertyClassRoot::SetSaving,
// TCComPropertyClassRoot::IsLoading, TCComPropertyClassRoot::CLoadSaveScope
bool TCComPropertyClassRoot::IsSaving()
{
  return m_nSavingRefs != 0;
}

/////////////////////////////////////////////////////////////////////////////
// Description: Sets or clears the object's current persistence save state.
//
// Sets or clears the object's current persistence state. Actually, since
// saving to a persistence mechanism may be nested into several levels,
// this method only increments or decrements a reference count of save
// operations. Typically, this method is only used internally by the
// TCComPropertyClassRoot::CLoadSaveScope class. Usually, there is no reason
// to call it directly.
//
// Parameters:
//   bSaving - *true* specifies that a persistence save operation is about to
// be entered; *false* indicates that such an operation is finished.
//
// See Also: TCComPropertyClassRoot::IsSaving,
// TCComPropertyClassRoot::SetLoading, TCComPropertyClassRoot::CLoadSaveScope
void TCComPropertyClassRoot::SetSaving(bool bSaving)
{
  if (bSaving)
    InterlockedIncrement(&m_nSavingRefs);
  else
    InterlockedDecrement(&m_nSavingRefs);
}


/////////////////////////////////////////////////////////////////////////////
// Group=Implementation

/////////////////////////////////////////////////////////////////////////////
// Description: Implementation of *IPersistStreamInit::Load*.
//
// Initializes an object from the stream where it was previously saved.
//
// Note: The *ATL::IPersistStreamInitImpl* class uses this method for its
// implementation of *IPersistStreamInit::Load*.
//
// Parameters:
//   pStm - [in] *IStream* pointer to the stream from which the object should
// be loaded. 
//   pMap - [in] Pointer to the first element of an array of
// *ATL_PROPMAP_ENTRY* structures, as defined by the ATL property map macros.
//
// Return Value:
//
//   S_OK - The object was successfully loaded. 
//   E_OUTOFMEMORY - The object was not loaded due to a lack of memory. 
//   E_FAIL - The object was not loaded due to some reason other than a lack
// of memory. 
//
// See Also: TCComPropertyClass, *ATL::IPersistStreamInitImpl* „
HRESULT TCComPropertyClassRoot::TCPersistStreamInit_Load(LPSTREAM pStm,
  ATL_PROPMAP_ENTRY* pMap, void* pThis, IUnknown* pUnk)
{
  // Enter the Loading mode for this scope
  CLoadSaveScope loading(this);

  ZAssert(pMap != NULL);
  HRESULT hr = S_OK;
  DWORD dwVer;
  hr = pStm->Read(&dwVer, sizeof(DWORD), NULL);
  if (dwVer > _ATL_VER)
    return E_FAIL;
  if (FAILED(hr))
    return hr;

  CComPtr<IDispatch> pDispatch;
  const IID* piidOld = NULL;
  for(int i = 0; pMap[i].pclsidPropPage != NULL; i++)
  {
    if (pMap[i].szDesc == NULL)
      continue;

    // check if raw data entry
    if (pMap[i].dwSizeData != 0)
    {
      void* pData = (void*) (pMap[i].dwOffsetData + (DWORD)pThis);
      hr = pStm->Read(pData, pMap[i].dwSizeData, NULL);
      if (FAILED(hr))
        return hr;
      continue;
    }

    CComVariant var;

    // Handle safe arrays seperately from general VARTYPE's
    if (S_FALSE == (hr = TCReadSafeArrayFromStream(pStm, var)))
      hr = var.ReadFromStream(pStm);
    if (FAILED(hr))
      break;

    if(pMap[i].piidDispatch != piidOld)
    {
      pDispatch.Release();
      if(FAILED(pUnk->QueryInterface(*pMap[i].piidDispatch, (void**)&pDispatch)))
      {
        _TRACE_BEGIN
          _TRACE_PART1("TCComPropertyClass<%hs>::", m_pszType);
          _TRACE_PART0("IPersistStreamInit_Load(): Failed to get a dispatch ");
          _TRACE_PART2("pointer for property #%i, \"%ls\"\n", i, pMap[i].szDesc);
        _TRACE_END
        hr = E_FAIL;
        break;
      }
      piidOld = pMap[i].piidDispatch;
    }

    if (FAILED(CComDispatchDriver::PutProperty(pDispatch, pMap[i].dispid, &var)))
    {
      _TRACE_BEGIN
        _TRACE_PART1("TCComPropertyClass<%hs>::", m_pszType);
        _TRACE_PART0("IPersistStreamInit_Load(): Invoke failed on DISPID ");
        _TRACE_PART2("0x%x, \"%ls\"\n", pMap[i].dispid, pMap[i].szDesc);
      _TRACE_END
      hr = E_FAIL;
      break;
    }
  }
  return hr;
}

/////////////////////////////////////////////////////////////////////////////
// Description: Implementation of *IPersistStreamInit::Save*.
//
// Saves an object to the specified stream.
//
// Note: The *ATL::IPersistStreamInitImpl* class uses this method for its
// implementation of *IPersistStreamInit::Save*.
//
// Parameters:
//   pStm - [in] *IStream* pointer to the stream into which the object should
// be saved.
// fClearDirty - [in] Indicates whether to clear the dirty flag after the
// save is complete. If *TRUE*, the flag should be cleared. If *FALSE*, the
// flag should be left unchanged. 
//   pMap - [in] Pointer to the first element of an array of
// *ATL_PROPMAP_ENTRY* structures, as defined by the ATL property map macros.
//
// Return Value:
//
//   S_OK - The object was successfully saved to the stream. 
//   STG_E_CANTSAVE - The object could not save itself to the stream. This
// error could indicate, for example, that the object contains another object
// that is not serializable to a stream or that an
// *ISequentialStream::Write* call returned *STG_E_CANTSAVE*.
//   STG_E_MEDIUMFULL - The object could not be saved because there is no
// space left on the storage device. 
//
// See Also: TCComPropertyClass, *ATL::IPersistStreamInitImpl* „
HRESULT TCComPropertyClassRoot::TCPersistStreamInit_Save(LPSTREAM pStm,
  BOOL fClearDirty, ATL_PROPMAP_ENTRY* pMap, void* pThis, IUnknown* pUnk)
{
  // Enter the Saving mode for this scope
  CLoadSaveScope saving(this, false);

  ZAssert(pMap != NULL);
  DWORD dw = _ATL_VER;
  HRESULT hr = pStm->Write(&dw, sizeof(DWORD), NULL);
  if (FAILED(hr))
    return hr;

  CComPtr<IDispatch> pDispatch;
  const IID* piidOld = NULL;
  for(int i = 0; pMap[i].pclsidPropPage != NULL; i++)
  {
    if (pMap[i].szDesc == NULL)
      continue;

    // check if raw data entry
    if (pMap[i].dwSizeData != 0)
    {
      void* pData = (void*) (pMap[i].dwOffsetData + (DWORD)pThis);
      hr = pStm->Write(pData, pMap[i].dwSizeData, NULL);
      if (FAILED(hr))
        return hr;
      continue;
    }

    CComVariant var;

    if(pMap[i].piidDispatch != piidOld)
    {
      pDispatch.Release();
      if(FAILED(pUnk->QueryInterface(*pMap[i].piidDispatch, (void**)&pDispatch)))
      {
        _TRACE_BEGIN
          _TRACE_PART1("TCComPropertyClass<%hs>::", m_pszType);
          _TRACE_PART0("IPersistStreamInit_Save(): Failed to get a dispatch ");
          _TRACE_PART2("pointer for property #%i, \"%ls\"\n", i, pMap[i].szDesc);
        _TRACE_END
        hr = E_FAIL;
        break;
      }
      piidOld = pMap[i].piidDispatch;
    }

    if (FAILED(CComDispatchDriver::GetProperty(pDispatch, pMap[i].dispid, &var)))
    {
      _TRACE_BEGIN
        _TRACE_PART1("TCComPropertyClass<%hs>::", m_pszType);
        _TRACE_PART0("IPersistStreamInit_Save(): Invoke failed on DISPID ");
        _TRACE_PART2("0x%x, \"%ls\"\n", pMap[i].dispid, pMap[i].szDesc);
      _TRACE_END
      hr = E_FAIL;
      break;
    }

    // Handle safe arrays seperately from general VARTYPE's
    if (S_FALSE == (hr = TCWriteSafeArrayToStream(pStm, var)))
      hr = var.WriteToStream(pStm);
    if (FAILED(hr))
      break;
  }
  if (SUCCEEDED(hr) && fClearDirty)
    SetDirty(FALSE);

  return hr;
}

/////////////////////////////////////////////////////////////////////////////
// Description: Implementation of *IPersistPropertyBag::Load*.
//
// This method instructs the object to initialize itself using the properties
// available in the property bag, notifying the provided error log object
// when errors occur. All property storage must take place within this method
// call as the object cannot hold the IPropertyBag pointer.
//
// *E_NOTIMPL* is not a valid return code as any object implementing this
// interface must support the entire functionality of the interface.
//
// Note: The *ATL::IPersistPropertyBagImpl* class uses this method for its
// implementation of *IPersistPropertyBag::Load*.
//
// Parameters:
//   pPropBag - [in] Pointer to the caller's *IPropertyBag* interface bag
// that the object uses to read its properties. Cannot be *NULL*.
//   pErrorLog - [in] Pointer to the caller's *IErrorLog* interface in which
// the object stores any errors that occur during initialization. Can be
// *NULL* in which case the caller is not interested in errors.
//   pMap - [in] Pointer to the first element of an array of
// *ATL_PROPMAP_ENTRY* structures, as defined by the ATL property map macros.
//
// Return Value:
//
// S_OK - The object successfully initialized itself. 
// E_UNEXPECTED - This method was called after
// *IPersistPropertyBag::InitNew* has already been called. The two
// initialization methods are mutually exclusive. 
// E_OUTOFMEMORY - The properties were not loaded due to a lack of memory. 
// E_POINTER - The address in /pPropBag/ is not valid (such as *NULL*) and
// therefore the object cannot initialize itself. 
// E_FAIL - The object was unable to retrieve a critical property that is
// necessary for the object's successful operation. The object was therefore
// unable to initialize itself completely.
//
// See Also: TCComPropertyClass, *ATL::IPersistPropertyBagImpl* „
HRESULT TCComPropertyClassRoot::TCPersistPropertyBag_Load(
  LPPROPERTYBAG pPropBag, LPERRORLOG pErrorLog, ATL_PROPMAP_ENTRY* pMap,
  void* pThis, IUnknown* pUnk)
{
  // Enter the Loading mode for this scope
  CLoadSaveScope loading(this);

  USES_CONVERSION;
  CComPtr<IDispatch> pDispatch;
  const IID* piidOld = NULL;
  for(int i = 0; pMap[i].pclsidPropPage != NULL; i++)
  {
    if (pMap[i].szDesc == NULL)
      continue;
    CComVariant var;

    // If raw entry, handle specially
    if (pMap[i].dwSizeData != 0)
    {
      void* pData = (void*) (pMap[i].dwOffsetData + (DWORD)pThis);
      HRESULT hr = pPropBag->Read(pMap[i].szDesc, &var, pErrorLog);
      if (SUCCEEDED(hr))
      {
        // check the type - we only deal with limited set
        switch (pMap[i].vt)
        {
        case VT_UI1:
        case VT_I1:
          *((BYTE*)pData) = var.bVal;
          break;
        case VT_BOOL:
          *((VARIANT_BOOL*)pData) = var.boolVal;
          break;
        case VT_I2:         // Added by TC
        case VT_UI2:
          *((short*)pData) = var.iVal;
          break;
        case VT_I4:         // Added by TC
        case VT_UI4:
        case VT_INT:
        case VT_UINT:
          *((long*)pData) = var.lVal;
          break;
        /////////////////////////////////////////////////////////////////////
        // The following all added by TC                                  //
        case VT_R4:                                                        //
          *((float*)pData) = var.fltVal;                                   //
          break;                                                           //
        case VT_R8:                                                        //
          *((double*)pData) = var.dblVal;                                  //
          break;                                                           //
        case VT_CY:                                                        //
          *((CY*)pData) = var.cyVal;                                       //
          break;                                                           //
        case VT_DATE:                                                      //
          *((DATE*)pData) = var.date;                                      //
          break;                                                           //
        case VT_ERROR:                                                     //
          *((SCODE*)pData) = var.scode;                                    //
          break;                                                           //
        case VT_DECIMAL:                                                   //
          *((DECIMAL*)pData) = *var.pdecVal;                               //
          break;                                                           //
        /////////////////////////////////////////////////////////////////////
        // TODO: Implement the following VARIANT types                     //
        case VT_BSTR:       // Tricky: different ways to store a BSTR      //
          break;                                                           //
        case VT_UNKNOWN:    // Tricky: different ways to store a IUnknown* //
        case VT_DISPATCH:   // Tricky: different ways to store a IDispatch*//
          continue;                                                        //
        case VT_VARIANT:    // Tricky: different ways to store a VARIANT   //
          continue;                                                        //
        case VT_RECORD:     // Tricky: how possible?                       //
          continue;                                                        //
        case VT_ARRAY:      // Very tricky: Also, caution: OR flag         //
          continue;                                                        //
        case VT_BYREF:      // Shouldn't be TOO tricky. Caution: OR flag   //
          continue;                                                        //
        /////////////////////////////////////////////////////////////////////
        }
      }
      continue;
    }

    if(pMap[i].piidDispatch != piidOld)
    {
      pDispatch.Release();
      if(FAILED(pUnk->QueryInterface(*pMap[i].piidDispatch, (void**)&pDispatch)))
      {
        _TRACE_BEGIN
          _TRACE_PART1("TCComPropertyClass<%hs>::", m_pszType);
          _TRACE_PART0("IPersistPropertyBag_Load(): Failed to get a dispatch ");
          _TRACE_PART2("pointer for property #%i, \"%ls\"\n", i, pMap[i].szDesc);
        _TRACE_END
        return E_FAIL;
      }
      piidOld = pMap[i].piidDispatch;
    }

    if (FAILED(CComDispatchDriver::GetProperty(pDispatch, pMap[i].dispid, &var)))
    {
      _TRACE_BEGIN
        _TRACE_PART1("TCComPropertyClass<%hs>::", m_pszType);
        _TRACE_PART0("IPersistPropertyBag_Load(): Invoke failed on DISPID ");
        _TRACE_PART2("0x%x, \"%ls\"\n", pMap[i].dispid, pMap[i].szDesc);
      _TRACE_END
      return E_FAIL;
    }

    if (VT_DISPATCH == V_VT(&var))
      var.Clear();

    HRESULT hr = pPropBag->Read(pMap[i].szDesc, &var, pErrorLog);
    if (FAILED(hr))
    {
      if (hr == E_INVALIDARG)
      {
        _TRACE_BEGIN
          _TRACE_PART1("TCComPropertyClass<%hs>::", m_pszType);
          _TRACE_PART0("IPersistPropertyBag_Load(): ");
          _TRACE_PART1("Property %ls not in Bag\n", pMap[i].szDesc);
        _TRACE_END
      }
      else
      {
        // Many containers return different ERROR values for Member not found
        _TRACE_BEGIN
          _TRACE_PART1("TCComPropertyClass<%hs>::", m_pszType);
          _TRACE_PART0("IPersistPropertyBag_Load(): Error attempting to read ");
          _TRACE_PART1("Property %ls from PropertyBag\n", pMap[i].szDesc);
        _TRACE_END
      }
      continue;
    }

    if (FAILED(CComDispatchDriver::PutProperty(pDispatch, pMap[i].dispid, &var)))
    {
      _TRACE_BEGIN
        _TRACE_PART1("TCComPropertyClass<%hs>::", m_pszType);
        _TRACE_PART0("IPersistPropertyBag_Load(): Invoke failed on DISPID ");
        _TRACE_PART2("0x%x, \"%ls\"\n", pMap[i].dispid, pMap[i].szDesc);
      _TRACE_END
      return E_FAIL;
    }
  }
  return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
// Description: Implementation of *IPersistPropertyBag::Save*.
//
// This method instructs the object to save its properties to the specified
// property bag, optionally clearing the object's dirty flag. The caller can
// request that the object save all properties or that the object save only
// those that are known to have changed.
//
// *E_NOTIMPL* is not a valid return code as any object implementing this
// interface must support the entire functionality of the interface.
//
// Note: The *ATL::IPersistPropertyBagImpl* class uses this method for its
// implementation of *IPersistPropertyBag::Save*.
//
// Parameters:
//   pPropBag - [in] Pointer to the caller's *IPropertyBag* interface bag
// that the object uses to write its properties. Cannot be *NULL*.
//   fClearDirty - [in] A flag indicating whether the object should clear its
// dirty flag when saving is complete. *TRUE* means clear the flag, *FALSE* „
// means leave the flag unaffected. *FALSE* is used when the caller wishes to
// do a /Save/ /Copy/ /As/ type of operation.
//   fSaveAllProperties - [in] A flag indicating whether the object should
// save all its properties (TRUE) or only those that have changed since the
// last save or initialization (FALSE). 
//   pMap - [in] Pointer to the first element of an array of
// *ATL_PROPMAP_ENTRY* structures, as defined by the ATL property map macros.
//
// Return Value: 
//
// S_OK - The object successfully saved the requested properties itself. 
// E_FAIL - There was a problem saving one of the properties. The object can
// choose to fail only if a necessary property could not be saved, meaning
// that the object can assume default property values if a given property is
// not seen through *IPersistPropertyBag::Load* at some later time.
// E_POINTER - The address in /pPropBag/ is not valid (such as *NULL*) and
// therefore the object cannot save itself.
// STG_E_MEDIUMFULL - The object was not saved because of a lack of space on
// the disk. 
//
// See Also: TCComPropertyClass, *ATL::IPersistPropertyBagImpl* „
HRESULT TCComPropertyClassRoot::TCPersistPropertyBag_Save(
  LPPROPERTYBAG pPropBag, BOOL fClearDirty, BOOL fSaveAllProperties,
  ATL_PROPMAP_ENTRY* pMap, void* pThis, IUnknown* pUnk)
{
  // Do nothing if saving only the dirty properties and there are none
  if (!fSaveAllProperties && !GetDirty())
    return S_OK;

  if (pPropBag == NULL)
  {
    _TRACE_BEGIN
      _TRACE_PART1("TCComPropertyClass<%hs>::", m_pszType);
      _TRACE_PART0("IPersistPropertyBag_Save(): ");
      _TRACE_PART0("PropBag pointer passed in was invalid\n");
    _TRACE_END
    return E_POINTER;
  }

  // Enter the Saving mode for this scope
  CLoadSaveScope saving(this, false);

  CComPtr<IDispatch> pDispatch;
  const IID* piidOld = NULL;
  for(int i = 0; pMap[i].pclsidPropPage != NULL; i++)
  {
    if (pMap[i].szDesc == NULL)
      continue;
    CComVariant var;

    // If raw entry skip it - we don't handle it for property bags just yet
    if (pMap[i].dwSizeData != 0)
    {
      void* pData = (void*) (pMap[i].dwOffsetData + (DWORD)pThis);
      // check the type - we only deal with limited set
      bool bTypeOK = false;
      switch (pMap[i].vt)
      {
      case VT_UI1:
      case VT_I1:
        var.bVal = *((BYTE*)pData);
        bTypeOK = true;
        break;
      case VT_BOOL:
        var.boolVal = *((VARIANT_BOOL*)pData);
        bTypeOK = true;
        break;
      case VT_I2:         // Added by TC
      case VT_UI2:
        var.iVal = *((short*)pData);
        bTypeOK = true;
        break;
      case VT_I4:         // Added by TC
      case VT_UI4:
      case VT_INT:
      case VT_UINT:
        var.lVal = *((long*)pData);
        bTypeOK = true;
        break;
        /////////////////////////////////////////////////////////////////////
        // The following all added by TC                                  //
        case VT_R4:                                                        //
          var.fltVal = *((float*)pData);                                   //
          bTypeOK = true;                                                  //
          break;                                                           //
        case VT_R8:                                                        //
          var.dblVal = *((double*)pData);                                  //
          bTypeOK = true;                                                  //
          break;                                                           //
        case VT_CY:                                                        //
          var.cyVal = *((CY*)pData);                                       //
          bTypeOK = true;                                                  //
          break;                                                           //
        case VT_DATE:                                                      //
          var.date = *((DATE*)pData);                                      //
          bTypeOK = true;                                                  //
          break;                                                           //
        case VT_ERROR:                                                     //
          var.scode = *((SCODE*)pData);                                    //
          bTypeOK = true;                                                  //
          break;                                                           //
        case VT_DECIMAL:                                                   //
          var.pdecVal = ((DECIMAL*)pData);                                 //
          bTypeOK = true;                                                  //
          break;                                                           //
        /////////////////////////////////////////////////////////////////////
        // TODO: Implement the following VARIANT types                     //
        case VT_BSTR:       // Tricky: different ways to store a BSTR      //
          break;                                                           //
        case VT_UNKNOWN:    // Tricky: different ways to store a IUnknown* //
        case VT_DISPATCH:   // Tricky: different ways to store a IDispatch*//
          continue;                                                        //
        case VT_VARIANT:    // Tricky: different ways to store a VARIANT   //
          continue;                                                        //
        case VT_RECORD:     // Tricky: how possible?                       //
          continue;                                                        //
        case VT_ARRAY:      // Very tricky: Also, caution: OR flag         //
          continue;                                                        //
        case VT_BYREF:      // Shouldn't be TOO tricky. Caution: OR flag   //
          continue;                                                        //
        /////////////////////////////////////////////////////////////////////
      }
      if (bTypeOK)
      {
        var.vt = pMap[i].vt;
        HRESULT hr = pPropBag->Write(pMap[i].szDesc, &var);
        if (FAILED(hr))
          return hr;
      }
      continue;
    }

    if(pMap[i].piidDispatch != piidOld)
    {
      pDispatch.Release();
      if(FAILED(pUnk->QueryInterface(*pMap[i].piidDispatch, (void**)&pDispatch)))
      {
        _TRACE_BEGIN
          _TRACE_PART1("TCComPropertyClass<%hs>::", m_pszType);
          _TRACE_PART0("IPersistPropertyBag_Save(): Failed to get a dispatch ");
          _TRACE_PART2("pointer for property #%i, \"%ls\"\n", i, pMap[i].szDesc);
        _TRACE_END
        return E_FAIL;
      }
      piidOld = pMap[i].piidDispatch;
    }

    if (FAILED(CComDispatchDriver::GetProperty(pDispatch, pMap[i].dispid, &var)))
    {
      _TRACE_BEGIN
        _TRACE_PART1("TCComPropertyClass<%hs>::", m_pszType);
        _TRACE_PART0("IPersistPropertyBag_Save(): Invoke failed on DISPID ");
        _TRACE_PART2("0x%x, \"%ls\"\n", pMap[i].dispid, pMap[i].szDesc);
      _TRACE_END
      return E_FAIL;
    }

    if (var.vt == VT_UNKNOWN || var.vt == VT_DISPATCH)
    {
      if (var.ppunkVal == NULL)
      {
        _TRACE_BEGIN
          _TRACE_PART1("TCComPropertyClass<%hs>::", m_pszType);
          _TRACE_PART0("IPersistPropertyBag_Save(): ");
          _TRACE_PART0("Warning skipping empty IUnknown in Save\n");
        _TRACE_END
        continue;
      }
    }

    HRESULT hr = pPropBag->Write(pMap[i].szDesc, &var);
    if (FAILED(hr))
      return hr;
  }

  if (fClearDirty)
    SetDirty(FALSE);
  return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
// Description: Implementation of *ISpecifyPropertyPages::GetPages*.
//
// Fills a counted array of GUID values where each GUID specifies the CLSID
// of each property page that can be displayed in the property sheet for this
// object.
//
// Note: The *ATL::ISpecifyPropertyPagesImpl* class uses this method for its
// implementation of *ISpecifyPropertyPages::GetPages*.
//
// Parameters:
//   pPages - [out] Pointer to a caller-allocated *CAUUID* structure that must
// be initialized and filled before returning. The /pElems/ field in the
// *CAUUID* structure is allocated by the callee with *CoTaskMemAlloc* and
// freed by the caller with *CoTaskMemFree*.
//   pMap - [in] Pointer to the first element of an array of
// *ATL_PROPMAP_ENTRY* structures, as defined by the ATL property map macros.
//
// Return Value: This method supports the standard return values
// *E_OUTOFMEMORY* and *E_UNEXPECTED*, as well as the following: 
//
// S_OK - The array was filled successfully. 
// E_POINTER - The address in /pPages/ is not valid. For example, it may be
// *NULL*. 
//
// See Also: TCComPropertyClass, *ATL::ISpecifyPropertyPagesImpl* „
HRESULT TCComPropertyClassRoot::ISpecifyPropertyPages_GetPages(
  CAUUID* pPages, ATL_PROPMAP_ENTRY* pMap)
{
  ZAssert(pMap != NULL);
  int nCnt = 0;
  // Get count of unique pages
  for(int i = 0; pMap[i].pclsidPropPage != NULL; i++)
  {
    if (!InlineIsEqualGUID(*pMap[i].pclsidPropPage, CLSID_NULL))
      nCnt++;
  }
  pPages->pElems = NULL;
  pPages->pElems = (GUID*) CoTaskMemAlloc(sizeof(CLSID)*nCnt);
  if (pPages->pElems == NULL)
    return E_OUTOFMEMORY;
  nCnt = 0;
  for(int i = 0; pMap[i].pclsidPropPage != NULL; i++)
  {
    if (!InlineIsEqualGUID(*pMap[i].pclsidPropPage, CLSID_NULL))
    {
      BOOL bMatch = FALSE;
      for (int j=0;j<nCnt;j++)
      {
        if (InlineIsEqualGUID(*(pMap[i].pclsidPropPage), pPages->pElems[j]))
        {
          bMatch = TRUE;
          break;
        }
      }
      if (!bMatch)
        pPages->pElems[nCnt++] = *pMap[i].pclsidPropPage;
    }
  }
  pPages->cElems = nCnt;
  return S_OK;
}


/////////////////////////////////////////////////////////////////////////////
// Group=

/////////////////////////////////////////////////////////////////////////////
// Global Implementation

#ifdef _ATL
  ///////////////////////////////////////////////////////////////////////////
  // Description: Compares a *CComVariant* to a *VARIANT*.
  //
  // Since CComVariant::operator== does not compare arrays of *BSTR*'s, this
  // function was developed to first check for the variant type
  // *VT_ARRAY*|*VT_BSTR*. If both parameters are of that variant type,
  // the elements of the first dimension of the string array are compared.
  // Otherwise, processing is delegated to CComVariant::operator==.
  //
  // Parameters:
  //   dest - A reference to the *CComVariant* on the left-hand side of the
  // comparison.
  //   src - A reference to the *VARIANT* on the right-hand side of the
  // comparison.
  //
  // Return Value: *true* if the arguments are equal, otherwise *false*.
  //
  // See Also: TCComPropertyCompare, TCComPropertyPut
  bool TCComVariantCompare(CComVariant& dest, VARIANT& src)
  {
    // Check for (some) types that CComVariant::operator== doesnt' handle
    VARTYPE vtDest = V_VT(&dest);
    VARTYPE vtSrc = V_VT(&src);
    
    // Array of ...
    if ((VT_ARRAY & vtDest) && (VT_ARRAY & vtSrc))
    {
      // Check equal element size
      SAFEARRAY* psaDest = V_ARRAY(&dest);
      SAFEARRAY* psaSrc = V_ARRAY(&src);
      if (psaDest->cbElements != psaSrc->cbElements)
        return false;

      // Strip off array flag and check to see if BSTR's
      vtDest &= ~VT_ARRAY; 
      vtSrc &= ~VT_ARRAY;
      if (VT_BSTR == vtDest && VT_BSTR == vtSrc)
      {
        // Check array bounds for match
        LONG lLBoundDest, lUBoundDest;
        LONG lLBoundSrc, lUBoundSrc;

        // Upper, most likely to be different
        if (FAILED(SafeArrayGetUBound(psaDest, 1, &lUBoundDest)))
          return false;
        if (FAILED(SafeArrayGetUBound(psaSrc, 1, &lUBoundSrc)))
          return false;
        if (lUBoundDest != lUBoundSrc)
          return false;

        // Check the lower too
        if (FAILED(SafeArrayGetLBound(psaDest, 1, &lLBoundDest)))
          return false;
        if (FAILED(SafeArrayGetLBound(psaSrc, 1, &lLBoundSrc)))
          return false;
        if (lLBoundDest != lLBoundSrc)
          return false;

        // Compute the elements size, equal if no elements
        LONG cElements = lUBoundDest - lLBoundDest + 1;
        if (!cElements)
          return true;

        // Get pointers to the elements of the arrays
        BSTR *pbstrDest,*pbstrSrc;
        if (FAILED(SafeArrayAccessData(psaDest, (void**)&pbstrDest)))
          return false;
        if (FAILED(SafeArrayAccessData(psaSrc, (void**)&pbstrSrc)))
        {
          _VERIFYE(SUCCEEDED(SafeArrayUnaccessData(psaDest)));
          return false;
        }

        // Loop through all elements and check for difference
        bool bSame = true;
        __try
        {
          // See if the strings are the same
          for (int i = 0; i < cElements && bSame; i++)
          {
            // Check for NULL pointers
            if (!pbstrDest[i] || !pbstrSrc[i])
            {
              bSame = pbstrDest[i] == pbstrSrc[i];
              continue;
            }

            // Get string lengths
            UINT nLenDest = SysStringLen(pbstrDest[i]);
            UINT nLenSrc = SysStringLen(pbstrSrc[i]);

            // Compare lengths and contents
            if (nLenDest != nLenSrc || wcscmp(pbstrDest[i], pbstrSrc[i]))
              bSame = false;  
          }
        }
        __except(1)
        {
          _TRACE_BEGIN
            _TRACE_PART0("TCComPropertyCompare(): Caught an unknown exception ");
            _TRACE_PART0("(probably an invalid pointer was specified)");
          _TRACE_END
          bSame = false;
        }

        // Release access to the array elements
        _VERIFYE(SUCCEEDED(SafeArrayUnaccessData(psaDest)));
        _VERIFYE(SUCCEEDED(SafeArrayUnaccessData(psaSrc)));
        return bSame;
      }
    }

    // Default to CComVariant::operator==
    return dest == src;
  }
#endif // _ATL


/////////////////////////////////////////////////////////////////////////////

#endif // !__PropertyClass_cpp__
