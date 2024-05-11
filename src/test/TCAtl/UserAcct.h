#pragma once


/////////////////////////////////////////////////////////////////////////////
// UserAcct.h : Declaration of the TCUserAccount class.

#include <..\TCLib\NetApi.h>
#include <..\TCLib\AutoHandle.h>


/////////////////////////////////////////////////////////////////////////////

class TCUserAccount
{
// Construction / Destruction
public:
  TCUserAccount();
  ~TCUserAccount();
  HRESULT Init(LPCSTR szUser);
  HRESULT Init(LPCWSTR szUser);

// Attributes
public:
  // Security ID (SID) Lookup
  static HRESULT GetSID(LPCSTR  szUserName, PSID* ppSID, LPSTR* ppszDomain = NULL);
  static HRESULT GetSID(LPCWSTR szUserName, PSID* ppSID, LPWSTR* ppszDomain = NULL);

  // AppID Helpers
  static HRESULT ResolveAppID(LPWSTR pszAppID, HKEY* phKey, REGSAM samDesired = KEY_ALL_ACCESS);
  static HRESULT SetRunAsInteractiveUser(LPCTSTR szAppID);
  static HRESULT SetRunAsUser(LPCTSTR szAppID, LPCTSTR szUserName, LPCTSTR szPassword);
         HRESULT SetRunAsUser(LPCTSTR szAppID, LPCTSTR szPassword);

  // Property Accessors
  ULONG GetUserNameLength()                               const;
  ULONG GetUserName(LPSTR  pszUserName, ULONG cbUserName) const;
  ULONG GetUserName(LPWSTR pszUserName, ULONG cbUserName) const;
  LPCWSTR GetUserNameW() const;
  LPCWSTR GetDomainNameW() const;

  // Privilege Accessors
  HRESULT HasRight   (LPTSTR pszPrivilege) const;
  HRESULT SetRight   (LPTSTR pszPrivilege);
  HRESULT RemoveRight(LPTSTR pszPrivilege);

// Data Members
protected:
  TCLsaHandle         m_hPolicy;
  TCCoTaskPtr<WCHAR*> m_spszUserName;
  TCCoTaskPtr<WCHAR*> m_spszDomainName;
  TCCoTaskPtr<PSID>   m_spSIDPrincipal;
};


/////////////////////////////////////////////////////////////////////////////
// Construction / Destruction

inline TCUserAccount::TCUserAccount()
{
}


inline TCUserAccount::~TCUserAccount()
{
}



inline HRESULT TCUserAccount::Init(LPCSTR szUser)
{
  USES_CONVERSION;
  return Init(A2CW(szUser));
}


/////////////////////////////////////////////////////////////////////////////
// Attributes

// Security ID (SID) Lookup
inline HRESULT TCUserAccount::GetSID(LPCSTR szUserName, PSID* ppSID, LPSTR* ppszDomain)
{
  USES_CONVERSION;
  TCCoTaskPtr<LPWSTR> spszDomain;
  LPWSTR* ppszDomainWide = ppszDomain ? &spszDomain : NULL;
  RETURN_FAILED(GetSID(A2CW(szUserName), ppSID, ppszDomainWide));
  if (ppszDomainWide)
  {
    UINT cch = (wcslen(*ppszDomainWide) + 1) * sizeof(char);
    *ppszDomain = (LPSTR)CoTaskMemAlloc(cch);
    if (!*ppszDomain)
      return E_OUTOFMEMORY;
    strcpy(*ppszDomain, W2CA(*ppszDomainWide));
  }
  return S_OK;
}


// Property Accessors
inline ULONG TCUserAccount::GetUserNameLength() const
{
  return wcslen(m_spszUserName);
}

inline ULONG TCUserAccount::GetUserName(LPSTR pszUserName, ULONG cbUserName) const
{
  USES_CONVERSION;
  if (pszUserName)
    lstrcpynA(pszUserName, W2CA(m_spszUserName), cbUserName);
  return GetUserNameLength();
}

inline ULONG TCUserAccount::GetUserName(LPWSTR pszUserName, ULONG cbUserName) const
{
  if (pszUserName)
    lstrcpynW(pszUserName, m_spszUserName, cbUserName);
  return GetUserNameLength();    
}

inline LPCWSTR TCUserAccount::GetUserNameW() const
{
  return m_spszUserName;
}

inline LPCWSTR TCUserAccount::GetDomainNameW() const
{
  return m_spszDomainName;
}


/////////////////////////////////////////////////////////////////////////////