#pragma once

#include <color.h>
#include <cstdint>
#include <d3d9.h>
#include <tref.h>

#include "pixel.h"

#define D3D9PF_ALPHAPIXELS			0x00000001l		// The surface has alpha channel information in the pixel format.
#define D3D9PF_ALPHA                0x00000002l		// The pixel format contains alpha only information
#define D3D9PF_PALETTEINDEXED4      0x00000008l		// The surface is 4-bit color indexed.
#define D3D9PF_PALETTEINDEXED8      0x00000020l		// The surface is 8-bit color indexed.

//
// D3D9PIXELFORMAT
//
typedef struct _D3D9PIXELFORMAT
{
    uint32_t	dwSize;
    uint32_t	dwFlags;				// See above flags.
    uint32_t   dwRGBBitCount;          // how many bits per pixel
    uint32_t   dwRBitMask;             // mask for red bit
    uint32_t   dwGBitMask;             // mask for green bits
    uint32_t   dwBBitMask;             // mask for blue bits
    uint32_t   dwRGBAlphaBitMask;      // mask for alpha channel
} D3D9PIXELFORMAT;

typedef D3D9PIXELFORMAT FAR* LPD3D9PIXELFORMAT;

//////////////////////////////////////////////////////////////////////////////
//
// Pixel Formats
//
//////////////////////////////////////////////////////////////////////////////

class PixelFormat : public IObject {
private:
    D3D9PIXELFORMAT	m_ddpf;
	D3DFORMAT		m_d3dFormat;
    bool			m_bSoftwareTexture;

    void SetSoftwareTexture()
    {
        m_bSoftwareTexture = true;
    }

/*    PixelFormat(const DDPIXELFORMAT& ddpf) :
        m_ddpf(ddpf),
        m_bSoftwareTexture(false)
    {
    }*/

    friend class EngineImpl;

public:
    PixelFormat(
        int   bits,
        uint32_t redMask,
        uint32_t greenMask,
        uint32_t blueMask,
        uint32_t alphaMask
    );

	PixelFormat( D3DFORMAT d3dFmt );

    const D3D9PIXELFORMAT& GetDDPF() { return m_ddpf; }

    void SetPixelBits(uint32_t value) { m_ddpf.dwRGBBitCount = value; }

    bool IsSoftwareTexture() { return m_bSoftwareTexture; }

    uint32_t PixelBits()  const { return m_ddpf.dwRGBBitCount;     }
    uint32_t PixelBytes() const { return m_ddpf.dwRGBBitCount / 8; }
    uint32_t RedMask()    const { return m_ddpf.dwRBitMask;        }
    uint32_t GreenMask()  const { return m_ddpf.dwGBitMask;        }
    uint32_t BlueMask()   const { return m_ddpf.dwBBitMask;        }
    uint32_t AlphaMask()  const { return m_ddpf.dwRGBAlphaBitMask; }

    uint32_t RedSize()    const;
    uint32_t GreenSize()  const;
    uint32_t BlueSize()   const;
    uint32_t AlphaSize()  const;

    uint32_t RedShift()   const;
    uint32_t GreenShift() const;
    uint32_t BlueShift()  const;
    uint32_t AlphaShift() const;

    Pixel MakePixel(uint32_t red, uint32_t green, uint32_t blue) const;
    Pixel MakePixel(const Color& color)                 const;
	Pixel MakeD3DPixel( const Color& color)				const;
    Color MakeColor(Pixel pixel)                        const;
    void  SetPixel(BYTE* pb, Pixel pixel)               const;
    Pixel GetPixel(const BYTE* pb)                      const;
    void  SetColor(BYTE* pb, const Color& color)        const;
    Color GetColor(const BYTE* pb)                      const;
    bool  ValidGDIFormat()                              const;

    bool Equivalent(const D3D9PIXELFORMAT& ddpf);

	D3DFORMAT GetD3DFormat() const { return m_d3dFormat; }
	D3DFORMAT GetEquivalentD3DFormat( );
};