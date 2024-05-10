#pragma once

#include <cstdint>
#include <point.h>

class Color;

class CImageTransfer
{
public:
    ///////////////////////////////////////////////////////////////////////////////////
    static void Transfer16BitTo16BitCopy(const uint8_t * pSrc,
        const int iSrcPitch,
        uint8_t * pDst,
        const int iDstPitch,
        const WinPoint & srcPos,
        const WinPoint & dstPos,
        const WinPoint & size
    );

	///////////////////////////////////////////////////////////////////////////////////
    static void Transfer16BitTo16BitNoColourKey(	const uint8_t * pSrc,
													const int iSrcPitch,
                                                    uint8_t * pDst,
													const int iDstPitch,
													const WinPoint & srcPos,
													const WinPoint & dstPos,
													const WinPoint & size );

	///////////////////////////////////////////////////////////////////////////////////
    static void Transfer16BitTo16BitWithColourKey(	const uint8_t * pSrc,
													const int iSrcPitch,
                                                    uint8_t * pDst,
													const int iDstPitch,
													const WinPoint & srcPos,
													const WinPoint & dstPos,
													const WinPoint & size,
													const Color & colorKey );

	///////////////////////////////////////////////////////////////////////////////////
    static void Transfer16BitTo32BitNoColourKey(	const uint8_t * pSrc,
													const int iSrcPitch,
                                                    uint8_t * pDst,
													const int iDstPitch,
													const WinPoint & srcPos,
													const WinPoint & dstPos,
													const WinPoint & size );

	///////////////////////////////////////////////////////////////////////////////////
    static void Transfer16BitTo32BitWithColourKey(	const uint8_t * pSrc,
													const int iSrcPitch,
                                                    uint8_t * pDst,
													const int iDstPitch,
													const WinPoint & srcPos,
													const WinPoint & dstPos,
													const WinPoint & size,
													const Color & colorKey );

    ///////////////////////////////////////////////////////////////////////////////////
    static void Transfer32BitTo32BitCopy(
        const uint8_t * pSrc,
        const int iSrcPitch,
        uint8_t * pDst,
        const int iDstPitch,
        const WinPoint & srcPos,
        const WinPoint & dstPos,
        const WinPoint & size
    );

	///////////////////////////////////////////////////////////////////////////////////
	static void ColourImageFill16Bit(	void * pDst,
										const int iDstPitch,
										const WinPoint & dstPos,
										const WinPoint & size,
                                        const uint16_t wPixelColour );

	///////////////////////////////////////////////////////////////////////////////////
	static void ColourImageFill32Bit(	void * pDst,
										const int iDstPitch,
										const WinPoint & dstPos,
										const WinPoint & size,
                                        const uint32_t dwPixelColour );
};