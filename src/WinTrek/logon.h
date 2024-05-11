#pragma once

//////////////////////////////////////////////////////////////////////////////
//
// logon
//
//////////////////////////////////////////////////////////////////////////////

class LogonSite : public IObject {
public:
    virtual void OnLogon(const ZString& strName, const ZString& strPassword, BOOL fRememberPW) = 0;
    virtual void OnAbort() = 0;
};

enum LogonType
{
  LogonAllegianceZone,
  LogonFreeZone,
  LogonLAN,
};

TRef<IPopup> CreateLogonPopup(Modeler* pmodeler, LogonSite* psite, LogonType lt, 
                              LPCSTR szPrompt, LPCSTR szName, LPCSTR szPW, BOOL fRememberPW);