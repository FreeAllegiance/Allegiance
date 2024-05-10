#pragma once

#include <alloc.h>
#include <cstring>
#include <olddxdefns.h>

#include "pixelformat.h"

//////////////////////////////////////////////////////////////////////////////
//
// Zero fill for structs with a dwSize member
//
//////////////////////////////////////////////////////////////////////////////

template<class Type>
class TZeroFillWithSize : public Type {
protected:
    TZeroFillWithSize()
    {
        Reinitialize();
    }

public:
    void Reinitialize()
    {
        memset(this, 0, sizeof(Type));
        this->dwSize = sizeof(Type);
    }
};

//////////////////////////////////////////////////////////////////////////////
//
// 
//
//////////////////////////////////////////////////////////////////////////////

class D3D9PixelFormat      : public TZeroFillWithSize<D3D9PIXELFORMAT> {};

//////////////////////////////////////////////////////////////////////////////
//
// D3D Device Description
//
//////////////////////////////////////////////////////////////////////////////

class D3DDeviceDescription : public TZeroFill<D3DCAPS9> {};

//////////////////////////////////////////////////////////////////////////////
//
// D3D class wrappers
//
//////////////////////////////////////////////////////////////////////////////

class D3DRect : public D3DRECT {
public:
    D3DRect() {}
    D3DRect(long x1Arg, long y1Arg, long x2Arg, long y2Arg)
    {
        x1 = x1Arg;
        y1 = y1Arg;
        x2 = x2Arg;
        y2 = y2Arg;
    }
};

class D3DColorValue : public COLORVALUE { // was D3DCOLORVALUE
public:
    D3DColorValue() {}
    D3DColorValue(const Color& Color)
    {
        r = Color.GetRed();
        g = Color.GetGreen();
        b = Color.GetBlue();
        a = Color.GetAlpha();
    }

    friend bool operator==(const D3DColorValue& c1, const D3DColorValue& c2)
    {
        return
               c1.r == c2.r
            && c1.g == c2.g
            && c1.b == c2.b
            && c1.a == c2.a;
    }

#define D3DRGBA(r, g, b, a)		(   (((long)((a) * 255)) << 24) | (((long)((r) * 255)) << 16) \
								|   (((long)((g) * 255)) << 8) | (long)((b) * 255) )

	D3DCOLOR MakeD3DCOLOR()
    {
        return D3DRGBA(r, g, b, a);
    }

    operator Color()
    {
        return Color(r, g, b, a);
    }
};