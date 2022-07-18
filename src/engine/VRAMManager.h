#ifndef _VRAMMANAGER_H_
#define _VRAMMANAGER_H_

#include <cstdint>
#include <d3d9.h>
#include <d3dx9tex.h>
#include <point.h>
#include "D3DDevice9.h"

class Color;

////////////////////////////////////////////////////////////////////////////////////////////////////
// TEXHANDLE, indicates a particular texture.
// Top 8 bits indicate bank index. Bottom n bits indicate the index of the texture within that
// bank.
typedef uint32_t TEXHANDLE;

////////////////////////////////////////////////////////////////////////////////////////////////////
//#define CREATE_TEST_TEXTURE			// Create a dummy texture at handle 0.

////////////////////////////////////////////////////////////////////////////////////////////////////
// These settings give 16384 textures, max.
#define BANK_SIZE			64					// Number of texture allocated in one go.
#define NUM_BANKS			256
#define MAX_TEXTURES		( BANK_SIZE * NUM_BANKS )
#define INVALID_TEX_HANDLE 0xFFFFFFFF
#define MAKEHANDLE( bank, index )		( ( TEXHANDLE ) ( ( bank << 24 ) | index ) )
#define BANKINDEX( texHandle )			( ( texHandle & 0xFF000000 ) >> 24 )
#define TEXINDEX( texHandle )			( texHandle & 0x00FFFFFF )
#define RENDER_TARGET_STACK_SIZE 32

// DEBUG, converts a sequential number to a handle, used to display texture cycle.
#define SEQUENTIALTOTEXHANDLE( i )		( ( ( i & ~( BANK_SIZE - 1 )  ) << 18 ) | ( i & ( BANK_SIZE - 1 ) ) )

////////////////////////////////////////////////////////////////////////////////////////////////////
class CVRAMManager
{

////////////////////////////////////////////////////////////////////////////////////////////////////
private:
	static CVRAMManager mSingleInstance;

////////////////////////////////////////////////////////////////////////////////////////////////////
private:
	////////////////////////////////////////////////////////////////////////////////////////////
	struct STexture
	{
		union
		{
			LPDIRECT3DTEXTURE9		pTexture;				// Pointer to created textures interface.
			BYTE *					pSystemTexture;			// System memory texture.
		};
		D3DFORMAT				texFormat;					// Format of texture.
        uint32_t					dwOriginalWidth;			// Original width of source image.
        uint32_t					dwOriginalHeight;			// Original height of source image.
        uint32_t					dwActualWidth;				// Actual width of allocated D3D texture.
        uint32_t					dwActualHeight;				// Actual width of allocated D3D texture.

		char					szTextureName[32];			// DEBUG, to be removed at some point.

		struct
		{
			bool					bValid : 1;
			bool					bLocked : 1;
			bool					bRenderTarget : 1;
			bool					bLocatedInDefaultPool : 1;	// If true, requires releasing and recreating for res change.
			bool					bResourceEvicted : 1;		// Set to true if in default pool and a device reset occurred.
			bool					bMipMappedTexture : 1;
			bool					bSystemMemory : 1;
		};
	};

	////////////////////////////////////////////////////////////////////////////////////////////
	struct SBank
	{
        uint32_t					dwNumAllocated;
		STexture 				pTexArray[ BANK_SIZE ];
	};

	////////////////////////////////////////////////////////////////////////////////////////////
	struct SVRAMRenderTarget
	{
		LPDIRECT3DSURFACE9		pTargetSurface;
		TEXHANDLE				hRenderTargetTexture;
	};

	////////////////////////////////////////////////////////////////////////////////////////////
	struct SVRAMManagerState
	{
		// NOTE: this MUST be the FIRST item in the structure.
		bool					bInitialised;
		bool					bMipMapGenerationEnabled;

        uint32_t					dwCurrentBank;							// Current bank to allocate from, if all others are full.
        uint32_t					dwNumTextures;
		SBank *					ppBankArray[ NUM_BANKS ];				// Array of bank pointers.

		// Render target management.
		TEXHANDLE				hCurrentTargetTexture;
        uint32_t					dwNumTargetsPushed;
		SVRAMRenderTarget		pTargetStack[RENDER_TARGET_STACK_SIZE];
	};

	////////////////////////////////////////////////////////////////////////////////////////////
	SVRAMManagerState m_sVRAM;

////////////////////////////////////////////////////////////////////////////////////////////////////
public:
	CVRAMManager();
	virtual ~CVRAMManager();

	////////////////////////////////////////////////////////////////////////////////////////////
	static inline CVRAMManager * Get() { return &mSingleInstance;	}

	////////////////////////////////////////////////////////////////////////////////////////////
	void		Initialise( );

	////////////////////////////////////////////////////////////////////////////////////////////
	void		Shutdown( );

	////////////////////////////////////////////////////////////////////////////////////////////
	void		EvictDefaultPoolResources( );

	////////////////////////////////////////////////////////////////////////////////////////////
	TEXHANDLE	AllocateHandle( );

	////////////////////////////////////////////////////////////////////////////////////////////
	bool		ReleaseHandle( TEXHANDLE texHandle );

	////////////////////////////////////////////////////////////////////////////////////////////
	int			GetTotalTextureCount( );

	// BT - 10/17 - Fixing crash on D3D texture load.
    HRESULT CreateSystemMemoryTexture(D3DFORMAT texFormat, uint32_t dwWidth, uint32_t dwHeight, STexture * pTexture);

	////////////////////////////////////////////////////////////////////////////////////////////
	HRESULT		CreateTexture(	TEXHANDLE	texHandle,
								D3DFORMAT	texFormat,
                                uint32_t		dwWidth,
                                uint32_t		dwHeight,
								bool		bSystemMemory,
                                const char *	szTextureName = NULL,
                                uint32_t		dwUsageFlags = 0,
								D3DPOOL		texPool = ALLEG_D9EX_IF(D3DPOOL_DEFAULT, D3DPOOL_MANAGED));

	////////////////////////////////////////////////////////////////////////////////////////////
	HRESULT		CreateTextureD3DX(	TEXHANDLE				texHandle,
									const D3DXIMAGE_INFO *	pImageInfo,
									const WinPoint *		pTargetSize, 
									IObject *				pobjectMemory, 
									const bool				bColorKey,
									const Color &			cColorKey, 
									char *					szTextureName = NULL );

	////////////////////////////////////////////////////////////////////////////////////////////
	HRESULT		CreateRenderTarget(	TEXHANDLE	texHandle,
                                    uint32_t		dwWidth,
                                    uint32_t		dwHeight );

	////////////////////////////////////////////////////////////////////////////////////////////
	HRESULT		LockTexture(	TEXHANDLE			texHandle,
								D3DLOCKED_RECT *	pLockRect,
                                uint32_t				dwFlags = 0,
                                uint32_t				dwLevel = 0,
								CONST RECT *		pAreaToLock = NULL );

	////////////////////////////////////////////////////////////////////////////////////////////
	HRESULT		UnlockTexture(	TEXHANDLE			texHandle,
                                uint32_t				dwLevel = 0 );

	////////////////////////////////////////////////////////////////////////////////////////////
	HRESULT		SetTexture(		TEXHANDLE			texHandle,
                                uint32_t				dwTextureStage );

	////////////////////////////////////////////////////////////////////////////////////////////
	bool		IsTextureValid(	TEXHANDLE			texHandle );

	////////////////////////////////////////////////////////////////////////////////////////////
    uint32_t		GetPower2( uint32_t dwInitialValue );

	////////////////////////////////////////////////////////////////////////////////////////////
    void		GetOriginalDimensions( TEXHANDLE texHandle, uint32_t * pdwWidth, uint32_t * pdwHeight );

	////////////////////////////////////////////////////////////////////////////////////////////
    void		GetActualDimensions( TEXHANDLE texHandle, uint32_t * pdwWidth, uint32_t * pdwHeight );

	////////////////////////////////////////////////////////////////////////////////////////////
	D3DFORMAT	GetTextureFormat( TEXHANDLE texHandle );

	////////////////////////////////////////////////////////////////////////////////////////////
	HRESULT		GetTextureSurface( TEXHANDLE texHandle, IDirect3DSurface9 ** ppSurface );

	////////////////////////////////////////////////////////////////////////////////////////////
    HRESULT		PushRenderTarget( TEXHANDLE texHandle, uint32_t dwTargetIndex = 0 );

	////////////////////////////////////////////////////////////////////////////////////////////
    HRESULT		PopRenderTarget( uint32_t dwTargetIndex = 0);

	////////////////////////////////////////////////////////////////////////////////////////////
	void		SetEnableMipMapGeneration( bool bEnable );

	////////////////////////////////////////////////////////////////////////////////////////////
    uint32_t		GetPixelSize( D3DFORMAT pixelFormat );
};

#endif // _VRAMMANAGER_H_
