#ifndef _button_H_
#define _button_H_

#include "pane.h"

#include <tvector.h>
#include "event.h"

//////////////////////////////////////////////////////////////////////////////
//
// ButtonPane
//
//////////////////////////////////////////////////////////////////////////////

class ButtonPane : public Pane {
public:
    virtual void SetEnabled(bool bEnable)  = 0;
    virtual void SetChecked(bool bChecked) = 0;
    virtual void SetChecked2(bool bChecked) = 0;
    virtual void SetRepeat(float repeatRate, float repeatDelay) = 0;
    virtual void SetDownTrigger(bool bDownTrigger)  = 0;

    virtual bool GetChecked()  = 0;
    virtual bool GetChecked2() = 0;
    virtual bool GetEnabled()  = 0;

    virtual IEventSource* GetEventSource() = 0;
	virtual IEventSource* GetRightEventSource() = 0;
    virtual IEventSource* GetDoubleClickEventSource() = 0;
    virtual IEventSource* GetMouseEnterEventSource() = 0;
    virtual IEventSource* GetMouseLeaveEventSource() = 0;
    virtual IEventSource* GetMouseEnterWhileEnabledEventSource() = 0;
};

/////////////////////////////////////////////////////////////////////////////
//
// ButtonFacePane
//
/////////////////////////////////////////////////////////////////////////////

class ButtonFacePane : public Pane {
private:
public:
    virtual void SetFocus(bool bFocus)
    {
    }

    virtual void SetEnabled(bool bEnabled)
    {
    }

    virtual void SetChecked(bool bChecked)
    {
    }

    virtual void SetChecked2(bool bChecked)
    {
    }

    virtual void SetInside(bool bInside)
    {
    }

    virtual void SetDown(bool bDown)
    {
    }
};

/////////////////////////////////////////////////////////////////////////////
//
// Image ButtonFacePane Constructor
//
/////////////////////////////////////////////////////////////////////////////

const DWORD ButtonFaceUp               = 0x0001;
const DWORD ButtonFaceDown             = 0x0002;
const DWORD ButtonFaceInside           = 0x0004;
const DWORD ButtonFaceDisabled         = 0x0008;
const DWORD ButtonFaceCheckedUp        = 0x0010;
const DWORD ButtonFaceCheckedDown      = 0x0020;
const DWORD ButtonFaceCheckedInside    = 0x0040;
const DWORD ButtonFaceCheckedDisabled  = 0x0080;
const DWORD ButtonFaceChecked2Up       = 0x0100;
const DWORD ButtonFaceChecked2Down     = 0x0200;
const DWORD ButtonFaceChecked2Inside   = 0x0400;
const DWORD ButtonFaceChecked2Disabled = 0x0800;
const DWORD ButtonFaceFocus            = 0x1000;

const DWORD ButtonNormal =
      ButtonFaceUp
    | ButtonFaceDown
    | ButtonFaceInside
    | ButtonFaceDisabled;

const DWORD ButtonNormalCheckBox =
    (
          ButtonNormal
        | ButtonFaceCheckedUp
        | ButtonFaceCheckedDown
        | ButtonFaceCheckedInside
        | ButtonFaceCheckedDisabled
    );

const DWORD Button3State =
    (
          ButtonNormalCheckBox
        | ButtonFaceChecked2Up
        | ButtonFaceChecked2Down
        | ButtonFaceChecked2Inside
        | ButtonFaceChecked2Disabled
    );

TRef<ButtonFacePane> CreateButtonFacePane(
    Surface* psurface, 
    DWORD    dwFaces,
    int      xmin = 0,
    int      xmax = 0
);

/////////////////////////////////////////////////////////////////////////////
//
// Image ButtonFacePane Constructor
//
/////////////////////////////////////////////////////////////////////////////

TRef<ButtonFacePane> CreateButtonFacePane(
    Pane* ppaneUp,
    Pane* ppaneDown
);

/////////////////////////////////////////////////////////////////////////////
//
// ButtonPane Constructors
//
/////////////////////////////////////////////////////////////////////////////

TRef<ButtonPane> CreateButton(int size);

TRef<ButtonPane> CreateButton(
    ButtonFacePane* ppane,
    bool  bToggle     = false,
    float repeatRate  = 0.0f,
    float repeatDelay = 0.0f
);

TRef<ButtonPane> CreateButton(
	Pane* ppaneUp, 
	Pane* ppaneDown, 
    bool  bToggle     = false,
    float repeatRate  = 0.0f,
    float repeatDelay = 0.0f
);

/////////////////////////////////////////////////////////////////////////////
//
// ButtonBar Constructors
//
/////////////////////////////////////////////////////////////////////////////

class ButtonBarPane : public Pane {
public:
    virtual IIntegerEventSource* GetEventSource() = 0;
    virtual IIntegerEventSource* GetMouseEnterWhileEnabledEventSource() = 0;

    virtual void RemoveAll() = 0;
    virtual void InsertButton(ButtonPane* pPane, int nCmd) = 0;

    virtual void SetHidden(int nCmd, bool bHidden) = 0;
    virtual void SetEnabled(int nCmd, bool bEnabled) = 0;
    virtual void SetChecked(int nCmd, bool bChecked) = 0;
    virtual void SetChecked2(int nCmd, bool bChecked) = 0;
    virtual void FlashButton(int nCmd) = 0;

    virtual bool GetEnabled(int nCmd) = 0;
    virtual bool GetChecked(int nCmd) = 0;
    virtual bool GetChecked2(int nCmd) = 0;

    virtual int GetSelection() = 0;
    virtual void SetSelection(int nCmd) = 0;
};

TRef<ButtonBarPane> CreateButtonBarPane(bool bActAsTabs = false, bool bUseColumn = false);
TRef<ButtonBarPane> CreateButtonBarPane(Surface* psurface, DWORD dwFaces, TVector<int>& vecColumns, bool bActAsTabs);

#endif
