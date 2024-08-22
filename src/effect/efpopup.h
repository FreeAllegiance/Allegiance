#pragma once

#include <popup.h>

/////////////////////////////////////////////////////////////////////////////
//
// Message Box
//
/////////////////////////////////////////////////////////////////////////////
class IntegerEventSourceImpl;
class Modeler;
class ButtonPane;
class IMessageBox : public IPopup
{
public:
    virtual IntegerEventSourceImpl* GetEventSource() = 0;
};

TRef<IMessageBox> CreateMessageBox(
    ITimerEventSource* ptimerEventSource,
    Modeler*           pmodeler, 
    const ZString&     str, 
    ButtonPane*        pbuttonIn, 
    bool               fAddDefaultButton,
    ButtonPane*        pbuttonCancel,
    float              paintDelay
);