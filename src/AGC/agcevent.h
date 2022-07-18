/////////////////////////////////////////////////////////////////////////////
// AGCEvent.h : Declaration of the CAGCEvent
//

#ifndef __AGCEvent_H_
#define __AGCEvent_H_

#include <AGC.h>
#include "resource.h"       // main symbols


/////////////////////////////////////////////////////////////////////////////
// Forward Declarations

class CAGCEventData;


/////////////////////////////////////////////////////////////////////////////
// CAGCEvent
//
class ATL_NO_VTABLE CAGCEvent : 
  public IDispatchImpl<IAGCEvent, &IID_IAGCEvent, &LIBID_AGCLib>,
  public IDispatchImpl<IAGCEventCreate, &IID_IAGCEventCreate, &LIBID_AGCLib>,
  public ISupportErrorInfo,
  public IPersistStreamInit,
  public AGCObjectSafetyImpl<CAGCEvent>,
  public CComObjectRootEx<CComMultiThreadModel>, 
  public CComCoClass<CAGCEvent, &CLSID_AGCEvent>
{
// Declarations
public:
  DECLARE_REGISTRY_RESOURCEID(IDR_AGCEvent)
  DECLARE_PROTECT_FINAL_CONSTRUCT()
  DECLARE_GET_CONTROLLING_UNKNOWN()

// Interface Map
public:
  BEGIN_COM_MAP(CAGCEvent)
    COM_INTERFACE_ENTRY(IAGCEvent)
    COM_INTERFACE_ENTRY(IAGCEventCreate)
    COM_INTERFACE_ENTRY2(IDispatch, IAGCEvent)
    COM_INTERFACE_ENTRY(ISupportErrorInfo)
    COM_INTERFACE_ENTRY(IPersist)
    COM_INTERFACE_ENTRY(IPersistStreamInit)
    COM_INTERFACE_ENTRY2(IPersistStream, IPersistStreamInit)
    COM_INTERFACE_ENTRY_AUTOAGGREGATE(IID_IMarshal, m_punkMBV.p, CLSID_TCMarshalByValue)
    COM_INTERFACE_ENTRY(IObjectSafety)
  END_COM_MAP()

// Category Map
public:
  BEGIN_CATEGORY_MAP(CAGCEvent)
    IMPLEMENTED_CATEGORY(CATID_AGC)
    IMPLEMENTED_CATEGORY(CATID_SafeForScripting)
    IMPLEMENTED_CATEGORY(CATID_SafeForInitializing)
  END_CATEGORY_MAP()

// Construction
public:
  HRESULT FinalConstruct();
  void FinalRelease();
  HRESULT Init(AGCEventID idEvent, LPCSTR pszContext,
    LPCOLESTR pszSubject, long idSubject, long cArgTriplets, va_list argptr);
  HRESULT Init(const CAGCEventData& data);

// Operations
public:
  HRESULT AddToDictionary(long cArgTriplets, ...);
  HRESULT AddToDictionaryV(long cArgTriplets, va_list argptr);
  HRESULT AddStaticFields();

// Implementation
protected:
  enum
  {
    LengthZero   = 0xF0,
    Length1Byte  = 0xF1,
    Length2Bytes = 0xF2,
    Length4Bytes = 0xF4
  };
  static BYTE GetSizePrefix(ULONG cch)
  {
    if (!cch)
      return LengthZero;
    if (!(0xFFFFFF00 & cch))
      return Length1Byte;
    if (!(0xFFFF0000 & cch))
      return Length2Bytes;
    return Length4Bytes;
  }
  static bool IsStaticField(BSTR bstr);
  static HRESULT WriteStringToStream(IStream* pStm, BSTR bstr);
  static HRESULT WriteVariantToStream(IStream* pStm, CComVariant& var);
  static HRESULT ReadStringFromStream(IStream* pStm, BSTR* pbstr, BYTE bIndicator = 0x00);
  static HRESULT ReadVariantFromStream(IStream* pStm, CComVariant& var);
  HRESULT SaveToStream(IStream* pStm, BOOL fClearDirty);
  HRESULT LoadFromStream(IStream* pStm);

// ISupportsErrorInfo Interface Methods
public:
  STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// IAGCEvent Interface Methods
public:
  STDMETHODIMP get_ID(AGCEventID* pVal);
  STDMETHODIMP get_Time(DATE* pVal);
  STDMETHODIMP get_Description(BSTR* pbstrDisplayString);
  STDMETHODIMP get_PropertyCount(long* pnCount);
  STDMETHODIMP get_PropertyExists(BSTR bstrKey, VARIANT_BOOL* pbExists);
  STDMETHODIMP get_Property(VARIANT* pvKey, VARIANT* pvValue);
  STDMETHODIMP get_ComputerName(BSTR* pbstrComputerName);
  STDMETHODIMP get_SubjectID(long* pidSubject);
  STDMETHODIMP get_SubjectName(BSTR* pbstrSubject);
  STDMETHODIMP SaveToString(BSTR* pbstr);
  STDMETHODIMP LoadFromString(BSTR bstr);
  STDMETHODIMP get_Context(BSTR* pbstrContext);

// IAGCEventCreate Interface Methods
public:
  STDMETHODIMP Init();
  STDMETHODIMP put_ID(AGCEventID Val);
  STDMETHODIMP put_Time(DATE Val);
  STDMETHODIMP SetTimeNow();
  STDMETHODIMP AddProperty(BSTR pbstrKey, VARIANT* pvValue);
  STDMETHODIMP RemoveProperty(BSTR pbstrKey, VARIANT* pvValue);
  STDMETHODIMP put_SubjectID(long idSubject);
  STDMETHODIMP put_SubjectName(BSTR bstrSubject);
  STDMETHODIMP put_Context(BSTR bstrContext);

// IPersist Interface Methods
public:
  STDMETHODIMP GetClassID(CLSID* pClassID);

// IPersistStreamInit Interface Methods
public:
  STDMETHODIMP IsDirty();
  STDMETHODIMP Load(LPSTREAM pStm);
  STDMETHODIMP Save(LPSTREAM pStm, BOOL fClearDirty);
  STDMETHODIMP GetSizeMax(ULARGE_INTEGER* pCbSize);
  STDMETHODIMP InitNew(void);

// Types
protected:
  typedef TCObjectLock<CAGCEvent> XLock;
  struct less_BSTR
  {
    bool operator()(CComBSTR a1, CComBSTR a2) const
    {
      return _wcsicmp((BSTR)a1, (BSTR)a2) < 0;
    }
  };
public:
  typedef std::map<CComBSTR, CComVariant, less_BSTR> XProperties;
  typedef XProperties::iterator                      XPropertyIt;
  typedef XProperties::reverse_iterator              XPropertyRevIt;

// Data Members
public:  
  CComPtr<IUnknown> m_punkMBV;
protected:
  AGCEventID     m_id;
  DATE           m_time;
  CComBSTR       m_bstrComputerName;
  long           m_idSubject;
  CComBSTR       m_bstrSubjectName;
  XProperties    m_Properties;
  CComBSTR       m_bstrContext;
};


/////////////////////////////////////////////////////////////////////////////

#endif //__AGCEvent_H_
