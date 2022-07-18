#ifndef _frameimage_H_
#define _frameimage_H_

#include <tref.h>

#include "pane.h"
#include "value.h"
#include "event.h"

class IFunction;
class Image;
class Surface;

//////////////////////////////////////////////////////////////////////////////
//
// RLE codes
//
//////////////////////////////////////////////////////////////////////////////

const WORD RLEMask       = 0xc000;
const WORD RLEMaskFill   = 0x0000;
const WORD RLEMaskBYTE   = 0x4000;
const WORD RLEMaskWORD   = 0x8000;
const WORD RLEMaskDWORD  = 0xc000;
const WORD RLELengthMask = 0x3fff;

//////////////////////////////////////////////////////////////////////////////
//
// FrameImage
//
//////////////////////////////////////////////////////////////////////////////
TRef<IFunction> CreateFrameImageFactory();

//////////////////////////////////////////////////////////////////////////////
//
// FrameImageButtonpane
//
//////////////////////////////////////////////////////////////////////////////

class FrameImageButtonPane : public Pane {
public:
    virtual IEventSource* GetEventSource() = 0;
    virtual IEventSource* GetMouseEnterEventSource() = 0;
    virtual IEventSource* GetMouseLeaveEventSource() = 0;
};

TRef<FrameImageButtonPane> CreateFrameImageButtonPane(
    Number*           ptime,
    Image*            pimage,
    ModifiableNumber* pframe,
    float             duration,
    bool              loop,
    bool              finish
);

#endif
