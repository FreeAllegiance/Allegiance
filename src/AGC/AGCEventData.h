#ifndef __AGCEventData_h__
#define __AGCEventData_h__

/////////////////////////////////////////////////////////////////////////////
// AGCEventData.h : Declaration of the CAGCEventData class.
//

#include <AGC.h>
#include "AGCEvent.h"


/////////////////////////////////////////////////////////////////////////////
// CAGCEventData
//
class CAGCEventData
{
// Types
public:
  typedef CAGCEvent::XProperties XProperties;

// Construction / Destruction
private:
  CAGCEventData(const CAGCEventData&); // Disable copy constructor
public:
  CAGCEventData(UINT cbData, BYTE* pbData);
  CAGCEventData(AGCEventID idEvent, LPCSTR pszContext, LPCOLESTR pszSubject,
    long idSubject, long cArgTriplets, va_list argptr);
  ~CAGCEventData();
  void Attach(UINT cbData, BYTE* pbData);
  BYTE* Detach(UINT* pcbData = NULL);

// Attributes
public:
  AGCEventID GetEventID() const;
  DATE GetTime     () const;
  long GetSubjectID() const;
  long GetArgCount () const;
  void GetVarData  (BSTR* pbstrContext, BSTR* pbstrSubject,
    XProperties* pProperties) const;

// Implementation
protected:
  static UINT ComputeVariableDataSize(LPCSTR pszContext,
    LPCOLESTR pszSubject, long cArgTriplets, va_list argptr);
  void CopyVariableData(LPCSTR pszContext, LPCOLESTR pszSubject,
    long cArgTriplets, va_list argptr);
  static UINT CreateVARIANTFromData(BYTE* pbData, CComVariant& var);
  static UINT CreateBSTRFromData(BYTE* pbData, BSTR* pbstr);
  static UINT CreateBSTRFromData_LPSTR(BYTE* pbData, BSTR* pbstr);
  static UINT CreateBSTRFromData_LPWSTR(BYTE* pbData, BSTR* pbstr);
  static UINT CreateObjectFromData(BYTE* pbData, REFIID riid, void** ppvUnk);

// Types
protected:
  #pragma pack(push, 4)
    struct XData
    {
      AGCEventID m_id;
      DATE       m_time;
      long       m_idSubject;
      long       m_cArgTriplets;
    };
  #pragma pack(pop)

// Data Members
protected:
  UINT  m_cbData;
  BYTE* m_pbData;
};


/////////////////////////////////////////////////////////////////////////////
// Construction / Destruction

/////////////////////////////////////////////////////////////////////////////
//
inline CAGCEventData::CAGCEventData(UINT cbData, BYTE* pbData) :
  m_cbData(0),
  m_pbData(NULL)
{
  Attach(cbData, pbData);
}

/////////////////////////////////////////////////////////////////////////////
//
inline CAGCEventData::CAGCEventData(AGCEventID idEvent, LPCSTR pszContext,
  LPCOLESTR pszSubject, long idSubject, long cArgTriplets, va_list argptr) :
  m_cbData(0),
  m_pbData(NULL)
{
  // Calculate the size needed for all the variable data
  m_cbData = ComputeVariableDataSize(pszContext, pszSubject, cArgTriplets, argptr);
  m_cbData += sizeof(XData);

  // Allocate memory for the data
  m_pbData = new BYTE[m_cbData];
  ZAssert(m_pbData);

  // Copy the fixed data to the memory
  XData* pFixed = reinterpret_cast<XData*>(m_pbData);
  pFixed->m_id           = idEvent;
  pFixed->m_time         = GetVariantDateTime();
  pFixed->m_idSubject    = idSubject;
  pFixed->m_cArgTriplets = cArgTriplets;

  // Copy the variable data to the memory
  CopyVariableData(pszContext, pszSubject, cArgTriplets, argptr);
}

/////////////////////////////////////////////////////////////////////////////
//
inline CAGCEventData::~CAGCEventData()
{
  delete [] Detach();
}

/////////////////////////////////////////////////////////////////////////////
//
inline void CAGCEventData::Attach(UINT cbData, BYTE* pbData)
{
  delete [] Detach();
  m_cbData = cbData;
  m_pbData = pbData;
}

/////////////////////////////////////////////////////////////////////////////
//
inline BYTE* CAGCEventData::Detach(UINT* pcbData)
{
  if (pcbData)
    *pcbData = m_cbData;
  BYTE* pbData = m_pbData;
  m_cbData = 0;
  m_pbData = NULL;
  return pbData;
}


/////////////////////////////////////////////////////////////////////////////
// Attributes

inline AGCEventID CAGCEventData::GetEventID() const
{
  ZAssert(m_pbData && m_cbData >= sizeof(XData));
  XData* pData = reinterpret_cast<XData*>(m_pbData);
  return pData->m_id;
}

inline DATE CAGCEventData::GetTime() const
{
  ZAssert(m_pbData && m_cbData >= sizeof(XData));
  XData* pData = reinterpret_cast<XData*>(m_pbData);
  return pData->m_time;
}

inline long CAGCEventData::GetSubjectID() const
{
  ZAssert(m_pbData && m_cbData >= sizeof(XData));
  XData* pData = reinterpret_cast<XData*>(m_pbData);
  return pData->m_idSubject;
}

inline long CAGCEventData::GetArgCount() const
{
  ZAssert(m_pbData && m_cbData >= sizeof(XData));
  XData* pData = reinterpret_cast<XData*>(m_pbData);
  return pData->m_cArgTriplets;
}


/////////////////////////////////////////////////////////////////////////////

#endif //__AGCEventData_h__
