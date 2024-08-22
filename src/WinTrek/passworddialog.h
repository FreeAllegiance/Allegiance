#pragma once

//////////////////////////////////////////////////////////////////////////////
//
// Password Dialog
//
//////////////////////////////////////////////////////////////////////////////

class PasswordDialogSink
{
public:
    virtual void OnCancelPassword() = 0;
    virtual void OnPassword(ZString strPassword) = 0;
};

TRef<IPopup> CreatePasswordPopup(PasswordDialogSink* psink, const ZString& strPassword, MissionInfo* pmission = NULL);