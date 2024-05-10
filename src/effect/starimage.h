#pragma once

#include <image.h>

//////////////////////////////////////////////////////////////////////////////
//
// StarImage header
//
//////////////////////////////////////////////////////////////////////////////

class Viewport;
class StarImage : public Image {
protected:
    StarImage(Viewport* pviewport) :
        Image(pviewport)
    {
    }

public:
    static TRef<StarImage> Create(Viewport* pviewport, short count);

    virtual void SetCount(unsigned int seed, short count) = 0;
    virtual void Render(Context* pcontext) = 0;

    //
    // Value methods
    //

    ZString GetFunctionName() { return "StarImage"; }
};