#ifndef __Persist_h__
#define __Persist_h__

#pragma once


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
  ATL_PROPMAP_ENTRY* pMap)
{
  // Enter the Loading mode for this scope
  CLoadSaveScope loading(this);

  // Ensure that we get a pointer to the outer-most inheritance branch
	T* pT = static_cast<T*>(this);
  IUnknown* pUnk = pT->GetUnknown();

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
      if(FAILED(ControlQueryInterface(*pMap[i].piidDispatch, (void**)&pDispatch)))
      {
        _TRACE_BEGIN
          _TRACE_PART("TCComPropertyClass<%hs>::", m_pszType);
          _TRACE_PART("IPersistStreamInit_Load(): Failed to get a dispatch ");
          _TRACE_PART("pointer for property #%i, \"%ls\"\n", i, pMap[i].szDesc);
        _TRACE_END
        hr = E_FAIL;
        break;
      }
      piidOld = pMap[i].piidDispatch;
    }

    if (FAILED(CComDispatchDriver::PutProperty(pDispatch, pMap[i].dispid, &var)))
    {
      _TRACE_BEGIN
        _TRACE_PART("TCComPropertyClass<%hs>::", m_pszType);
        _TRACE_PART("IPersistStreamInit_Load(): Invoke failed on DISPID ");
        _TRACE_PART("0x%x, \"%ls\"\n", pMap[i].dispid, pMap[i].szDesc);
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
  BOOL fClearDirty, ATL_PROPMAP_ENTRY* pMap)
{
  // Enter the Saving mode for this scope
  CLoadSaveScope saving(this, false);

  // Ensure that we get a pointer to the outer-most inheritance branch
	T* pT = static_cast<T*>(this);
  IUnknown* pUnk = pT->GetUnknown();

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
      if(FAILED(ControlQueryInterface(*pMap[i].piidDispatch, (void**)&pDispatch)))
      {
        _TRACE_BEGIN
          _TRACE_PART("TCComPropertyClass<%hs>::", m_pszType);
          _TRACE_PART("IPersistStreamInit_Save(): Failed to get a dispatch ");
          _TRACE_PART("pointer for property #%i, \"%ls\"\n", i, pMap[i].szDesc);
        _TRACE_END
        hr = E_FAIL;
        break;
      }
      piidOld = pMap[i].piidDispatch;
    }

    if (FAILED(CComDispatchDriver::GetProperty(pDispatch, pMap[i].dispid, &var)))
    {
      _TRACE_BEGIN
        _TRACE_PART("TCComPropertyClass<%hs>::", m_pszType);
        _TRACE_PART("IPersistStreamInit_Save(): Invoke failed on DISPID ");
        _TRACE_PART("0x%x, \"%ls\"\n", pMap[i].dispid, pMap[i].szDesc);
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
  LPPROPERTYBAG pPropBag, LPERRORLOG pErrorLog, ATL_PROPMAP_ENTRY* pMap)
{
  // Enter the Loading mode for this scope
  CLoadSaveScope loading(this);

  // Ensure that we get a pointer to the outer-most inheritance branch
	T* pT = static_cast<T*>(this);
  IUnknown* pUnk = pT->GetUnknown();

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
      if(FAILED(ControlQueryInterface(*pMap[i].piidDispatch, (void**)&pDispatch)))
      {
        _TRACE_BEGIN
          _TRACE_PART("TCComPropertyClass<%hs>::", m_pszType);
          _TRACE_PART("IPersistPropertyBag_Load(): Failed to get a dispatch ");
          _TRACE_PART("pointer for property #%i, \"%ls\"\n", i, pMap[i].szDesc);
        _TRACE_END
        return E_FAIL;
      }
      piidOld = pMap[i].piidDispatch;
    }

    if (FAILED(CComDispatchDriver::GetProperty(pDispatch, pMap[i].dispid, &var)))
    {
      _TRACE_BEGIN
        _TRACE_PART("TCComPropertyClass<%hs>::", m_pszType);
        _TRACE_PART("IPersistPropertyBag_Load(): Invoke failed on DISPID ");
        _TRACE_PART("0x%x, \"%ls\"\n", pMap[i].dispid, pMap[i].szDesc);
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
          _TRACE_PART("TCComPropertyClass<%hs>::", m_pszType);
          _TRACE_PART("IPersistPropertyBag_Load(): ");
          _TRACE_PART("Property %ls not in Bag\n", pMap[i].szDesc);
        _TRACE_END
      }
      else
      {
        // Many containers return different ERROR values for Member not found
        _TRACE_BEGIN
          _TRACE_PART("TCComPropertyClass<%hs>::", m_pszType);
          _TRACE_PART("IPersistPropertyBag_Load(): Error attempting to read ");
          _TRACE_PART("Property %ls from PropertyBag\n", pMap[i].szDesc);
        _TRACE_END
      }
      continue;
    }

    if (FAILED(CComDispatchDriver::PutProperty(pDispatch, pMap[i].dispid, &var)))
    {
      _TRACE_BEGIN
        _TRACE_PART("TCComPropertyClass<%hs>::", m_pszType);
        _TRACE_PART("IPersistPropertyBag_Load(): Invoke failed on DISPID ");
        _TRACE_PART("0x%x, \"%ls\"\n", pMap[i].dispid, pMap[i].szDesc);
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
  ATL_PROPMAP_ENTRY* pMap)
{
  // Do nothing if saving only the dirty properties and there are none
  if (!fSaveAllProperties && !GetDirty())
    return S_OK;

  if (pPropBag == NULL)
  {
    _TRACE_BEGIN
      _TRACE_PART("TCComPropertyClass<%hs>::", m_pszType);
      _TRACE_PART("IPersistPropertyBag_Save(): ");
      _TRACE_PART("PropBag pointer passed in was invalid\n");
    _TRACE_END
    return E_POINTER;
  }

  // Enter the Saving mode for this scope
  CLoadSaveScope saving(this, false);

  // Ensure that we get a pointer to the outer-most inheritance branch
	T* pT = static_cast<T*>(this);
  IUnknown* pUnk = pT->GetUnknown();

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
      if(FAILED(ControlQueryInterface(*pMap[i].piidDispatch, (void**)&pDispatch)))
      {
        _TRACE_BEGIN
          _TRACE_PART("TCComPropertyClass<%hs>::", m_pszType);
          _TRACE_PART("IPersistPropertyBag_Save(): Failed to get a dispatch ");
          _TRACE_PART("pointer for property #%i, \"%ls\"\n", i, pMap[i].szDesc);
        _TRACE_END
        return E_FAIL;
      }
      piidOld = pMap[i].piidDispatch;
    }

    if (FAILED(CComDispatchDriver::GetProperty(pDispatch, pMap[i].dispid, &var)))
    {
      _TRACE_BEGIN
        _TRACE_PART("TCComPropertyClass<%hs>::", m_pszType);
        _TRACE_PART("IPersistPropertyBag_Save(): Invoke failed on DISPID ");
        _TRACE_PART("0x%x, \"%ls\"\n", pMap[i].dispid, pMap[i].szDesc);
      _TRACE_END
      return E_FAIL;
    }

    if (var.vt == VT_UNKNOWN || var.vt == VT_DISPATCH)
    {
      if (var.pUnkVal == NULL)
      {
        _TRACE_BEGIN
          _TRACE_PART("TCComPropertyClass<%hs>::", m_pszType);
          _TRACE_PART("IPersistPropertyBag_Save(): ");
          _TRACE_PART("Warning skipping empty IUnknown in Save\n");
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
// TCPersistStreamInitImpl extends ATL's IPersistStreamInitImpl class to
// provide an implementation of the *IPersistStreamInit::GetSizeMax* „
// interface method.
//
// Parameters:
//   T - The class derived from TCPersistStreamInitImpl.
//
template <class T>
class ATL_NO_VTABLE TCPersistStreamInitImpl :
  public IPersistStreamInit
{
// Group=Types
public:
  // Declares a type definition to allow derived classes an easier way to
  // refer to the base class.
  typedef TCPersistStreamInitImpl<T> TCPersistStreamInitImplBase;

// Group=IPersist Interface Methods
public:
  STDMETHODIMP GetClassID(CLSID *pClassID);

// Group=IPersistStream Interface Methods
public:
  STDMETHODIMP IsDirty();
  STDMETHODIMP Load(IStream* pStm);
  STDMETHODIMP Save(IStream* pStm, BOOL fClearDirty);
  STDMETHODIMP GetSizeMax(ULARGE_INTEGER* pcbSize);

// Group=IPersistStreamInit Interface Methods
public:
  STDMETHODIMP InitNew();

// Group=Implementation
public:
  HRESULT IPersistStreamInit_Load(IStream* pStm, ATL_PROPMAP_ENTRY* pMap);
  HRESULT IPersistStreamInit_Save(IStream* pStm, BOOL fClearDirty,
    ATL_PROPMAP_ENTRY* pMap);
};


/////////////////////////////////////////////////////////////////////////////
// Group=IPersist Interface Methods

template <class T>
STDMETHODIMP TCPersistStreamInitImpl::GetClassID(CLSID *pClassID)
{
  _TRACE0("TCPersistStreamInitImpl::GetClassID\n");
  *pClassID = T::GetObjectCLSID();
  return S_OK;
}


/////////////////////////////////////////////////////////////////////////////
// Group=IPersistStream Interface Methods

template <class T>
STDMETHODIMP TCPersistStreamInitImpl::IsDirty()
{
  _TRACE0("TCPersistStreamInitImpl::IsDirty\n");
  T* pT = static_cast<T*>(this);
  return (pT->m_bRequiresSave) ? S_OK : S_FALSE;
}

template <class T>
STDMETHODIMP TCPersistStreamInitImpl::Load(IStream* pStm)
{
  _TRACE0("TCPersistStreamInitImpl::Load\n");
  T* pT = static_cast<T*>(this);
  return pT->IPersistStreamInit_Load(pStm, T::GetPropertyMap());
}

template <class T>
STDMETHODIMP TCPersistStreamInitImpl::Save(IStream* pStm, BOOL fClearDirty)
{
  T* pT = static_cast<T*>(this);
  _TRACE0("TCPersistStreamInitImpl::Save\n");
  return pT->IPersistStreamInit_Save(pStm, fClearDirty, T::GetPropertyMap());
}

/////////////////////////////////////////////////////////////////////////////
// Description: Returns the size, in bytes, of the stream needed to save the
// object.
//
// This function implements the *IPersistStreamInit::GetSizeMax* interface
// method by calling the TCGetPersistStreamSize global function.
//
// Parameters:
//   pCbSize - [out] Points to a 64-bit unsigned integer value indicating the
// size in bytes of the stream needed to save this object. 
//
// Return Value: *S_OK* indicates that the size was successfully returned. 
//
// See Also: TCNullStream, CTCNullStream
template <class T>
STDMETHODIMP TCPersistStreamInitImpl<T>::GetSizeMax(ULARGE_INTEGER* pcbSize)
{
  // Get the derived class pointer
  T* pThis = static_cast<T*>(this);

  // Determine the max size by writing to a null stream
  HRESULT hr = TCGetPersistStreamSize(pThis->GetUnknown(), pcbSize);

  // Display trace information under _DEBUG builds
  _TRACE_BEGIN
    _TRACE_PART("TCPersistStreamInitImpl<%hs>::GetSizeMax():", TCTypeName(T));
    if (SUCCEEDED(hr))
      _TRACE_PART("%s Counted 0x%08X (%d) bytes\n", szMsg, pcbSize->LowPart,
        pcbSize->LowPart);
    else
      _TRACE_PART("%s Failed, hr = 0x%08X\n", szMsg, hr);
  _TRACE_END

  // Return the last HRESULT
  return hr;
}


/////////////////////////////////////////////////////////////////////////////
// Group=IPersistStreamInit Interface Methods

template <class T>
STDMETHODIMP TCPersistStreamInitImpl::InitNew()
{
  _TRACE0("TCPersistStreamInitImpl::InitNew\n");
  return S_OK;
}


/////////////////////////////////////////////////////////////////////////////
// Group=Implementation

template <class T>
HRESULT TCPersistStreamInitImpl::IPersistStreamInit_Load(IStream* pStm,
  ATL_PROPMAP_ENTRY* pMap)
{
  T* pT = static_cast<T*>(this);
  HRESULT hr = pT->TCPersistStreamInit_Load(pStm, pMap);
  if (SUCCEEDED(hr))
    pT->m_bRequiresSave = FALSE;
  return hr;

}

template <class T>
HRESULT TCPersistStreamInitImpl::IPersistStreamInit_Save(IStream* pStm,
  BOOL fClearDirty, ATL_PROPMAP_ENTRY* pMap)
{
  T* pT = static_cast<T*>(this);
  return pT->TCPersistStreamInit_Save(pStm, fClearDirty, pMap);
}



/////////////////////////////////////////////////////////////////////////////
// TCPersistPropertyBagImpl
//
//
// Parameters:
//   T - The class derived from TCPersistPropertyBagImpl
//
template <class T>
class ATL_NO_VTABLE TCPersistPropertyBagImpl :
  public IPersistPropertyBag
{
// IPersist Interface Methods
public:
	STDMETHODIMP GetClassID(CLSID *pClassID)

// IPersistPropertyBag Interface Methods
public:
	STDMETHODIMP InitNew();
	STDMETHODIMP Load(IPropertyBag* pPropBag, IErrorLog* pErrorLog);
	STDMETHODIMP Save(IPropertyBag* pPropBag, BOOL fClearDirty,
    BOOL fSaveAllProperties);

// Implementation
public:
	HRESULT IPersistPropertyBag_Load(IPropertyBag* pPropBag,
    IErrorLog* pErrorLog, ATL_PROPMAP_ENTRY* pMap);
	HRESULT IPersistPropertyBag_Save(IPropertyBag* pPropBag, BOOL fClearDirty,
    BOOL fSaveAllProperties, ATL_PROPMAP_ENTRY* pMap);
};


/////////////////////////////////////////////////////////////////////////////
// IPersist Interface Methods

STDMETHODIMP TCPersistPropertyBagImpl::GetClassID(CLSID *pClassID)
{
	_TRACE0("IPersistPropertyBagImpl::GetClassID\n");
	*pClassID = T::GetObjectCLSID();
	return S_OK;
}


/////////////////////////////////////////////////////////////////////////////
// IPersistPropertyBag Interface Methods

STDMETHODIMP TCPersistPropertyBagImpl::InitNew()
{
	_TRACE0("IPersistPropertyBagImpl::InitNew\n");
	return S_OK;
}

STDMETHODIMP TCPersistPropertyBagImpl::Load(IPropertyBag* pPropBag,
  IErrorLog* pErrorLog)
{
	_TRACE0("IPersistPropertyBagImpl::Load\n");
	T* pT = static_cast<T*>(this);
	ATL_PROPMAP_ENTRY* pMap = T::GetPropertyMap();
	ATLASSERT(pMap != NULL);
	return pT->IPersistPropertyBag_Load(pPropBag, pErrorLog, pMap);
}

STDMETHODIMP TCPersistPropertyBagImpl::Save(IPropertyBag* pPropBag,
  BOOL fClearDirty, BOOL fSaveAllProperties)
{
	_TRACE0("IPersistPropertyBagImpl::Save\n");
	T* pT = static_cast<T*>(this);
	ATL_PROPMAP_ENTRY* pMap = T::GetPropertyMap();
	ATLASSERT(pMap != NULL);
	return pT->IPersistPropertyBag_Save(pPropBag, fClearDirty,
    fSaveAllProperties, pMap);
}


/////////////////////////////////////////////////////////////////////////////
// Implementation

HRESULT TCPersistPropertyBagImpl::IPersistPropertyBag_Load(
  IPropertyBag* pPropBag, IErrorLog* pErrorLog, ATL_PROPMAP_ENTRY* pMap)
{
	T* pT = static_cast<T*>(this);
	HRESULT hr = pT->TCPersistPropertyBag_Load(pPropBag, pErrorLog, pMap);
	if (SUCCEEDED(hr))
		pT->m_bRequiresSave = FALSE;
	return hr;
}

HRESULT TCPersistPropertyBagImpl::IPersistPropertyBag_Save(
  IPropertyBag* pPropBag, BOOL fClearDirty, BOOL fSaveAllProperties,
   ATL_PROPMAP_ENTRY* pMap)
{
	T* pT = static_cast<T*>(this);
	return pT->TCPersistPropertyBag_Save(pPropBag, fClearDirty,
    fSaveAllProperties, pMap);
}



/////////////////////////////////////////////////////////////////////////////

#endif // !__Persist_h__
