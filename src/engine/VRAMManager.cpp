#include "VRAMManager.h"

#include <base.h>
#include <zassert.h>

#include "D3DDevice9.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
// Class implemented as a singleton (mSingleInstance).
// Use static function Get() to get access to the single instance of this class.

CVRAMManager CVRAMManager::mSingleInstance;

////////////////////////////////////////////////////////////////////////////////////////////////////
#define NUM_MIPMAP_LEVELS 0			// Set to zero to generate all levels.

#ifdef CREATE_TEST_TEXTURE
TEXHANDLE g_TestTextureHandle = INVALID_TEX_HANDLE;
#endif // CREATE_TEST_TEXTURE


////////////////////////////////////////////////////////////////////////////////////////////////////
// CVRAMManager()
// Constructor.
////////////////////////////////////////////////////////////////////////////////////////////////////
CVRAMManager::CVRAMManager()
{
	memset(&m_sVRAM, 0, sizeof(CVRAMManager::SVRAMManagerState));
}


////////////////////////////////////////////////////////////////////////////////////////////////////
// ~CVRAMManager()
//
////////////////////////////////////////////////////////////////////////////////////////////////////
CVRAMManager::~CVRAMManager()
{
}


////////////////////////////////////////////////////////////////////////////////////////////////////
// Initialise()
//
////////////////////////////////////////////////////////////////////////////////////////////////////
void CVRAMManager::Initialise()
{
	if (m_sVRAM.bInitialised == false)
	{
		// Create the first bank, so that we know it's valid.
		m_sVRAM.ppBankArray[0] = new SBank;
		memset(m_sVRAM.ppBankArray[0], 0, sizeof(SBank));

		// Now valid for use.
		m_sVRAM.bInitialised = true;
		m_sVRAM.hCurrentTargetTexture = INVALID_TEX_HANDLE;
	}

#ifdef CREATE_TEST_TEXTURE
	g_TestTextureHandle = AllocateHandle();
	HRESULT hr = CreateTexture(g_TestTextureHandle,
		D3DFMT_A8R8G8B8,
		32,
		32,
		false,
		"Test Texture");
    ZAssert(hr == D3D_OK);
	D3DLOCKED_RECT lockRect;
	hr = LockTexture(g_TestTextureHandle, &lockRect);
    ZAssert(hr == D3D_OK);

	// Fill in data - blue and black solid texture.
    uint32_t dwX, dwY;
    uint32_t * pData;
	for (dwY = 0; dwY<32; dwY++)
	{
        pData = (uint32_t*)((BYTE*)lockRect.pBits + (dwY * lockRect.Pitch));
		for (dwX = 0; dwX<32; dwX += 4)
		{
			if ((dwY & 0x1) == 0)
			{
				*pData++ = 0xFF000000;
				*pData++ = 0xFF000000;
				*pData++ = 0xFF4040FF;
				*pData++ = 0xFF4040FF;
			}
			else
			{
				*pData++ = 0xFF4040FF;
				*pData++ = 0xFF4040FF;
				*pData++ = 0xFF000000;
				*pData++ = 0xFF000000;
			}
		}
	}

	UnlockTexture(g_TestTextureHandle);
#endif // CREATE_TEST_TEXTURE
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// Shutdown()
//
////////////////////////////////////////////////////////////////////////////////////////////////////
void CVRAMManager::Shutdown()
{
	if (m_sVRAM.bInitialised == true)
	{
        uint32_t i;

#ifdef CREATE_TEST_TEXTURE
		if (g_TestTextureHandle != INVALID_TEX_HANDLE)
		{
			ReleaseHandle(g_TestTextureHandle);
		}
#endif // CREATE_TEST_TEXTURE

		// Clear out any additional render target if currently pushed.
		if (m_sVRAM.dwNumTargetsPushed > 0)
		{
			PopRenderTarget(0);
		}

		// Clear each assigned texture.
		for (i = 0; i<CD3DDevice9::Get()->GetDevCaps()->MaxSimultaneousTextures; i++)
		{
			SetTexture(INVALID_TEX_HANDLE, i);
		}
	}
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// EvictDefaultPoolResources()
// Release any resources in the DEFAULT pool. Currently only render targets.
////////////////////////////////////////////////////////////////////////////////////////////////////
void CVRAMManager::EvictDefaultPoolResources()
{
    ZAssert(m_sVRAM.bInitialised);
    uint32_t dwBank, dwIndex, i;

	if (m_sVRAM.bInitialised == true)
	{
		// Clear out any additional render target if currently pushed.
		if (m_sVRAM.dwNumTargetsPushed > 0)
		{
			PopRenderTarget(0);
		}

		// Clear each assigned texture.
		for (i = 0; i<CD3DDevice9::Get()->GetDevCaps()->MaxSimultaneousTextures; i++)
		{
			SetTexture(INVALID_TEX_HANDLE, i);
		}

		for (dwBank = 0; dwBank<m_sVRAM.dwCurrentBank + 1; dwBank++)
		{
			for (dwIndex = 0; dwIndex<BANK_SIZE; dwIndex++)
			{
				STexture * pTexture = &m_sVRAM.ppBankArray[dwBank]->pTexArray[dwIndex];

				if ((pTexture->bValid == true) &&
					(pTexture->bLocatedInDefaultPool == true) &&
					(pTexture->bResourceEvicted == false))
				{
					ULONG refCount;
                    ZAssert(pTexture->bLocked == false);

					// Release this texture.
					refCount = pTexture->pTexture->Release();
					pTexture->pTexture = NULL;
                    ZAssert(refCount == 0);

					pTexture->bResourceEvicted = true;
				}
			}
		}
	}
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// GetTotalTextureCount()
//
////////////////////////////////////////////////////////////////////////////////////////////////////
int CVRAMManager::GetTotalTextureCount()
{
	return m_sVRAM.dwNumTextures;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// AllocateHandle()
//
////////////////////////////////////////////////////////////////////////////////////////////////////
TEXHANDLE CVRAMManager::AllocateHandle()
{
	// Is the VRAM manager full?
    ZAssert(m_sVRAM.dwNumTextures < MAX_TEXTURES);

    uint32_t dwBankIndex, dwTexIndex = 0xFFFFFFFF;

	for (dwBankIndex = 0; dwBankIndex <= m_sVRAM.dwCurrentBank; dwBankIndex++)
	{
		if (m_sVRAM.ppBankArray[dwBankIndex]->dwNumAllocated != BANK_SIZE)
		{
			for (dwTexIndex = 0; dwTexIndex<BANK_SIZE; dwTexIndex++)
			{
				if (m_sVRAM.ppBankArray[dwBankIndex]->pTexArray[dwTexIndex].bValid == false)
				{
					break;
				}
			}
		}
		if (dwTexIndex != 0xFFFFFFFF)
		{
			break;
		}
	}

	if (dwTexIndex == 0xFFFFFFFF)
	{
		// Check the previous bank was actually full.
        ZAssert(m_sVRAM.ppBankArray[dwBankIndex - 1]->dwNumAllocated == BANK_SIZE);

		// No space, allocate a new bank, then return a texhandle from there.
		m_sVRAM.dwCurrentBank++;
		dwBankIndex = m_sVRAM.dwCurrentBank;
		m_sVRAM.ppBankArray[dwBankIndex] = new SBank;

		// Reset the new bank.
		memset(m_sVRAM.ppBankArray[dwBankIndex], 0, sizeof(SBank));
		dwTexIndex = m_sVRAM.ppBankArray[dwBankIndex]->dwNumAllocated++;
	}

#ifdef _DEBUG
	//	char szBuffer[256];
	//	sprintf( szBuffer, "Allocated texture: %d, %d\n", dwBankIndex, dwTexIndex );
	//	OutputDebugString( szBuffer );
#endif //_DEBUG

	// Increment texture count for the bank it was allocated from.
	m_sVRAM.dwNumTextures++;
	m_sVRAM.ppBankArray[dwBankIndex]->dwNumAllocated++;

	return MAKEHANDLE(dwBankIndex, dwTexIndex);
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// CreateTexture()
// Physically allocate a D3D resource for the given texture handle.
// For now, usage and pool are assigned default values. We also don't create a mip map
// chain, although we probably want to enable this at some point and make use of decent hardware
// filtering to create a nice set of mipmaps.
// Must round width/height to nearest power 2 value, if not power 2.
////////////////////////////////////////////////////////////////////////////////////////////////////
HRESULT CVRAMManager::CreateTexture(TEXHANDLE	texHandle,
    D3DFORMAT	texFormat,
    uint32_t		dwWidth,
    uint32_t		dwHeight,
    bool		bSystemMemory,
    const char *szTextureName /*= NULL*/,
    uint32_t		dwUsageFlags /*= 0*/,
    D3DPOOL		texPool /*= D3DPOOL_DEFAULT*/)
{
	HRESULT hr;
	UINT uiNumLevels = 1;

    ZAssert(texHandle != INVALID_TEX_HANDLE);

	STexture * pTexture = &m_sVRAM.ppBankArray[BANKINDEX(texHandle)]->pTexArray[TEXINDEX(texHandle)];
	pTexture->dwOriginalWidth = dwWidth;
	pTexture->dwOriginalHeight = dwHeight;

	// Store texture name if one was supplied.
	if (szTextureName != NULL)
	{
		strcpy_s(pTexture->szTextureName, 32, szTextureName);
	}

	if (bSystemMemory == true)
	{
		// Allocate a system memory texture.
        uint32_t dwPixelSize, dwImageSize;
		dwPixelSize = GetPixelSize(texFormat);
		dwImageSize = dwWidth * dwHeight * dwPixelSize;
		pTexture->pSystemTexture = new BYTE[dwImageSize];
		pTexture->dwActualWidth = pTexture->dwOriginalWidth;
		pTexture->dwActualHeight = pTexture->dwOriginalHeight;
		pTexture->texFormat = texFormat;
		pTexture->bSystemMemory = true;
		pTexture->bValid = true;
		hr = D3D_OK;
	}
	else
	{
		// Textures greater than 2048 wide/height not supported on all cards.
        ZAssert(texPool == D3DPOOL_SCRATCH || dwWidth <= 2048);

		// Allocate a D3D texture.
		if (m_sVRAM.bMipMapGenerationEnabled == true)
		{
			uiNumLevels = NUM_MIPMAP_LEVELS;
			dwUsageFlags |= D3DUSAGE_AUTOGENMIPMAP;
		}

		// imago 6/26/09
		hr = CD3DDevice9::Get()->Device()->CreateTexture(pTexture->dwOriginalWidth,
			pTexture->dwOriginalHeight,
			uiNumLevels,
			dwUsageFlags,
			texFormat,
			texPool,
			&pTexture->pTexture,
			NULL);  //Fix memory leak -Imago 8/2/09

        HRESULT first_hr = hr;

		// BT - 10/17 - Check all available texture formats to see if we can find a match. On some cards, the target texture format cannot be 
		// created becuase it's not supported. Let's find one that is.
		for (int i = D3DFMT_R8G8B8; i < D3DFMT_A16B16G16R16 && FAILED(hr); i++)
		{
			hr = CD3DDevice9::Get()->Device()->CreateTexture(pTexture->dwOriginalWidth,
				pTexture->dwOriginalHeight,
				1, // Try again with only 1 level.
				0, // Try again without the mipmaps.
				(D3DFORMAT) i,
				texPool,
				&pTexture->pTexture,
				NULL);
		}

		// BT - 10/17 - Tracking down the CreateTexture crashes.
		if (FAILED(hr))
		{
            debugf(
                "Failed to create a texture. name=%s, size=(%d, %d), pool=%d, return=%d, last_try_return=%d", 
                (const char*)szTextureName, (int)dwWidth, (int)dwHeight, (int)texPool, (int)first_hr, (int)hr
            );
			(*(int*)0) = 0; // Force exception here.
		}

		D3DSURFACE_DESC surfDesc;
		pTexture->pTexture->GetLevelDesc(0, &surfDesc);
		pTexture->dwActualWidth = GetPower2(surfDesc.Width);
		pTexture->dwActualHeight = GetPower2(surfDesc.Height);

		// If it created ok, update the texture details.
		if (hr == D3D_OK)
		{
			pTexture->texFormat = texFormat;
			pTexture->bValid = true;
		}

		if (m_sVRAM.bMipMapGenerationEnabled == true)
		{
			pTexture->pTexture->SetAutoGenFilterType(CD3DDevice9::Get()->GetMipFilter());
			pTexture->bMipMappedTexture = true;
		}
	}

#ifdef _DEBUG
	//if( szTextureName != NULL )
	//{
	//	if( strnicmp( szTextureName, "heliumrock", 10 ) == 0 )
	//	{
	//		char szBuffer[256];
	//		sprintf_s( szBuffer, 256, "HANDLE: 0x%X   hr: 0x%X   FMT: %d", texHandle, hr, pTexture->texFormat );
	//	}
	//}
#endif //_DEBUG
	return hr;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// CreateTextureD3DX()
//
////////////////////////////////////////////////////////////////////////////////////////////////////
HRESULT CVRAMManager::CreateTextureD3DX(TEXHANDLE				texHandle,
	const D3DXIMAGE_INFO *	pImageInfo,
	const WinPoint *		pTargetSize,
	IObject *				pobjectMemory,
	const bool				bColorKey,
	const Color &			cColorKey,
	char *					szTextureName /*= NULL*/)
{
	HRESULT hr;
	UINT uiNumLevels = 1;

    ZAssert(texHandle != INVALID_TEX_HANDLE);

	STexture * pTexture = &m_sVRAM.ppBankArray[BANKINDEX(texHandle)]->pTexArray[TEXINDEX(texHandle)];

	if (szTextureName != NULL)
	{
		// Store texture name if one was supplied.
		strcpy_s(pTexture->szTextureName, 32, szTextureName);
	}

	// Mipping? //imago 7/10 #14
    //uint32_t		dwUsageFlags = 0;
	if (pImageInfo->ImageFileFormat == D3DXIFF_DDS &&  m_sVRAM.bMipMapGenerationEnabled == true)
	{
		uiNumLevels = D3DX_SKIP_DDS_MIP_LEVELS(pImageInfo->MipLevels, D3DX_FILTER_BOX);
		pTexture->bMipMappedTexture = true;
		//dwUsageFlags |= D3DUSAGE_AUTOGENMIPMAP;
	}
	else if (m_sVRAM.bMipMapGenerationEnabled == true)
	{
		pTexture->bMipMappedTexture = true;
		uiNumLevels = NUM_MIPMAP_LEVELS;
		//dwUsageFlags |= D3DUSAGE_AUTOGENMIPMAP;
	}

	ZFile * pFile = (ZFile*)pobjectMemory;

	//imago 6/26/09 (force system memory when device uses software processing)
	hr = D3DXCreateTextureFromFileInMemoryEx(
		CD3DDevice9::Get()->Device(),
		pFile->GetPointer(),
		pFile->GetLength(),
		pTargetSize->x,
		pTargetSize->y,
		uiNumLevels,
		0,
		D3DFMT_UNKNOWN,
        ALLEG_D9EX_IF(D3DPOOL_DEFAULT, D3DPOOL_MANAGED),
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		bColorKey ? 0xFF000000 : 0,
		NULL,
		NULL,
		&pTexture->pTexture);

    if (hr != D3D_OK) {
        debugf("Creating texture from file failed: 0x%x", hr);
    }
    ZAssert(hr == D3D_OK);

	D3DSURFACE_DESC surfDesc;
	pTexture->pTexture->GetLevelDesc(0, &surfDesc);
	pTexture->dwActualWidth = GetPower2(surfDesc.Width);
	pTexture->dwActualHeight = GetPower2(surfDesc.Height);
	pTexture->dwOriginalWidth = pTargetSize->x;
	pTexture->dwOriginalHeight = pTargetSize->y;

	// If it created ok, update the texture details.
	if (hr == D3D_OK)
	{
		pTexture->texFormat = surfDesc.Format;
		pTexture->bValid = true;
	}
	return hr;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// CreateRenderTarget()
//
////////////////////////////////////////////////////////////////////////////////////////////////////
HRESULT CVRAMManager::CreateRenderTarget(TEXHANDLE	texHandle,
    uint32_t		dwWidth,
    uint32_t		dwHeight)
{
	HRESULT hr;
    ZAssert(texHandle != INVALID_TEX_HANDLE);
    ZAssert((dwWidth != 0) && (dwHeight != 0) && "Render target created with zero dimension");

	STexture * pTexture = &m_sVRAM.ppBankArray[BANKINDEX(texHandle)]->pTexArray[TEXINDEX(texHandle)];
	pTexture->dwOriginalWidth = dwWidth;
	pTexture->dwOriginalHeight = dwHeight;

	// Determine texture size is good.
	dwWidth = GetPower2(dwWidth);
	dwHeight = GetPower2(dwHeight);
	pTexture->dwActualWidth = dwWidth;
	pTexture->dwActualHeight = dwHeight;

	// Fixed format for now. Render targets must go in DEFAULT pool.
	hr = CD3DDevice9::Get()->Device()->CreateTexture(pTexture->dwOriginalWidth,
		pTexture->dwOriginalHeight,
		1,
		D3DUSAGE_RENDERTARGET,
		D3DFMT_A8R8G8B8,
		D3DPOOL_DEFAULT,
		&pTexture->pTexture,
		NULL);
    //	ZAssert( hr == D3D_OK ); //imago removed 8/10 multimon glitch? REVIEW

	// If it created ok, update the texture details.
	if (hr == D3D_OK)
	{
		pTexture->texFormat = D3DFMT_A8R8G8B8;
		pTexture->bValid = true;
		pTexture->bRenderTarget = true;
		pTexture->bLocatedInDefaultPool = true;
		pTexture->bResourceEvicted = false;
	}
    ZAssert(hr == D3D_OK);
	return hr;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// ReleaseHandle()
// Function returns true if resource ref count reaches zero.
////////////////////////////////////////////////////////////////////////////////////////////////////
bool CVRAMManager::ReleaseHandle(TEXHANDLE texHandle)
{
    uint32_t dwBankIndex, dwTexIndex;
	bool bRetVal = false;

	dwBankIndex = BANKINDEX(texHandle);
	dwTexIndex = TEXINDEX(texHandle);

    ZAssert(dwBankIndex <= m_sVRAM.dwCurrentBank);
    ZAssert(dwTexIndex <= BANK_SIZE);
	if ((m_sVRAM.ppBankArray[dwBankIndex] != NULL) &&
		(m_sVRAM.ppBankArray[dwBankIndex]->pTexArray[dwTexIndex].bValid == true) &&
		(m_sVRAM.ppBankArray[dwBankIndex]->pTexArray[dwTexIndex].pTexture != NULL))
	{
		if (m_sVRAM.ppBankArray[dwBankIndex]->pTexArray[dwTexIndex].bSystemMemory == true)
		{
			delete[] m_sVRAM.ppBankArray[dwBankIndex]->pTexArray[dwTexIndex].pSystemTexture;
			memset(&m_sVRAM.ppBankArray[dwBankIndex]->pTexArray[dwTexIndex], 0, sizeof(CVRAMManager::STexture));
		}
		else
		{
			ULONG refCount;

			// Release the texture. Clear out the data when the reference count is zero.
			refCount = m_sVRAM.ppBankArray[dwBankIndex]->pTexArray[dwTexIndex].pTexture->Release(); //Imago 6/10 TODO REVIEW DEBUG CRASH HERE
			if (refCount == 0)
			{
				// Reduce the counts.
				m_sVRAM.ppBankArray[dwBankIndex]->dwNumAllocated--;
				m_sVRAM.dwNumTextures--;
				memset(&m_sVRAM.ppBankArray[dwBankIndex]->pTexArray[dwTexIndex], 0, sizeof(CVRAMManager::STexture));
				bRetVal = true;
			}
		}
	}
	return bRetVal;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// LockTexture()
// If pAreaToLock is NULL, the entire area is locked.
////////////////////////////////////////////////////////////////////////////////////////////////////
HRESULT	CVRAMManager::LockTexture(TEXHANDLE			texHandle,
	D3DLOCKED_RECT *	pLockRect,
    uint32_t				dwFlags /*= 0*/,
    uint32_t				dwLevel /*= 0*/,
	CONST RECT *		pAreaToLock /*= NULL*/)
{
	HRESULT hr;

    ZAssert(texHandle != INVALID_TEX_HANDLE);
	STexture * pTexture = &m_sVRAM.ppBankArray[BANKINDEX(texHandle)]->pTexArray[TEXINDEX(texHandle)];
    ZAssert(pTexture->bValid == true);

	if (pTexture->bSystemMemory == true)
	{
		// System memory locks only support entire texture lock.
        ZAssert(pAreaToLock == NULL);
		pTexture->bLocked = true;
		pLockRect->pBits = pTexture->pSystemTexture;
		pLockRect->Pitch = GetPixelSize(pTexture->texFormat) * pTexture->dwOriginalWidth;
		hr = D3D_OK;
	}
	else
	{
		// BT - 10/17 - Fixing VRAMManager crash when a surface that doesn't have a texture is attempted to be painted.
		if (pTexture->pTexture == nullptr)
		{
			pTexture->bLocked = false;
			return D3DERR_NOTAVAILABLE;
		}

		hr = pTexture->pTexture->LockRect(dwLevel, pLockRect, pAreaToLock, dwFlags);
		if (hr == D3D_OK)
		{
			pTexture->bLocked = true;
		}
	}
	return hr;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// UnlockTexture()
//
////////////////////////////////////////////////////////////////////////////////////////////////////
HRESULT CVRAMManager::UnlockTexture(TEXHANDLE	texHandle,
    uint32_t		dwLevel /*= 0*/)
{
	HRESULT hr;

    ZAssert(texHandle != INVALID_TEX_HANDLE);
	STexture * pTexture = &m_sVRAM.ppBankArray[BANKINDEX(texHandle)]->pTexArray[TEXINDEX(texHandle)];
    ZAssert(pTexture->bValid == true);
    ZAssert(pTexture->bLocked == true);

	if (pTexture->bSystemMemory == true)
	{
		pTexture->bLocked = false;
		hr = D3D_OK;
	}
	else
	{
		hr = pTexture->pTexture->UnlockRect(dwLevel);
		pTexture->bLocked = false;

		if (pTexture->bMipMappedTexture == true)
		{
            ZAssert(m_sVRAM.bMipMapGenerationEnabled == true);
            ZAssert(dwLevel == 0);
			pTexture->pTexture->GenerateMipSubLevels();
		}
	}
	return hr;
}


static TEXHANDLE g_DebugHandle = 0x0000002c;
#ifdef _DEBUG
static bool bBreakOnSizeTest = false;
#endif // _DEBUG

////////////////////////////////////////////////////////////////////////////////////////////////////
// SetTexture()
//
////////////////////////////////////////////////////////////////////////////////////////////////////
HRESULT CVRAMManager::SetTexture(TEXHANDLE texHandle, uint32_t dwTextureStage)
{
	HRESULT hr;

#ifdef _DEBUG
	if (g_DebugHandle != INVALID_TEX_HANDLE)
	{
        //		ZAssert( texHandle != g_DebugHandle );
	}
#endif // _DEBUG

	if (texHandle == INVALID_TEX_HANDLE)
	{
		hr = CD3DDevice9::Get()->Device()->SetTexture(dwTextureStage, NULL);
	}
	else
	{
		STexture * pTexture = &m_sVRAM.ppBankArray[BANKINDEX(texHandle)]->pTexArray[TEXINDEX(texHandle)];
        ZAssert(pTexture->bValid == true);
        ZAssert(pTexture->bLocked == false);
        ZAssert(pTexture->bSystemMemory == false);

		//		if( pTexture->dwOriginalWidth == 2048 )
		//		{
		//			OutputDebugString("Width == 2048\n");
		//		}

		//if( ( g_DebugHandle != INVALID_TEX_HANDLE ) &&
		//	( texHandle == g_DebugHandle ) )
		//{
		//	char szBuffer[256];
		//	sprintf_s( szBuffer, 256, "Fmt: %d\n", pTexture->texFormat );
		//	OutputDebugString( szBuffer );
		//}

#ifdef _DEBUG
		if (bBreakOnSizeTest == true)
		{
            ZAssert((pTexture->dwOriginalWidth != 800) && (pTexture->dwOriginalWidth != 600));
		}
#endif // _DEBUG

		hr = CD3DDevice9::Get()->Device()->SetTexture(dwTextureStage, pTexture->pTexture);
	}

	return hr;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// IsTextureValid()
//
////////////////////////////////////////////////////////////////////////////////////////////////////
bool CVRAMManager::IsTextureValid(TEXHANDLE texHandle)
{
    ZAssert(texHandle != INVALID_TEX_HANDLE);
	STexture * pTexture = &m_sVRAM.ppBankArray[BANKINDEX(texHandle)]->pTexArray[TEXINDEX(texHandle)];
	return pTexture->bValid;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// GetOriginalDimensions()
//
////////////////////////////////////////////////////////////////////////////////////////////////////
void CVRAMManager::GetOriginalDimensions(TEXHANDLE texHandle, uint32_t * pdwWidth, uint32_t * pdwHeight)
{
    ZAssert(texHandle != INVALID_TEX_HANDLE);
	STexture * pTexture = &m_sVRAM.ppBankArray[BANKINDEX(texHandle)]->pTexArray[TEXINDEX(texHandle)];
    ZAssert(pTexture->bValid == true);

	*pdwWidth = pTexture->dwOriginalWidth;
	*pdwHeight = pTexture->dwOriginalHeight;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// GetActualDimensions()
//
////////////////////////////////////////////////////////////////////////////////////////////////////
void CVRAMManager::GetActualDimensions(TEXHANDLE texHandle, uint32_t * pdwWidth, uint32_t * pdwHeight)
{
    ZAssert(texHandle != INVALID_TEX_HANDLE);
	STexture * pTexture = &m_sVRAM.ppBankArray[BANKINDEX(texHandle)]->pTexArray[TEXINDEX(texHandle)];
    ZAssert(pTexture->bValid == true);

	*pdwWidth = pTexture->dwActualWidth;
	*pdwHeight = pTexture->dwActualHeight;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// GetTextureFormat()
//
////////////////////////////////////////////////////////////////////////////////////////////////////
D3DFORMAT CVRAMManager::GetTextureFormat(TEXHANDLE texHandle)
{
    ZAssert(texHandle != INVALID_TEX_HANDLE);
	STexture * pTexture = &m_sVRAM.ppBankArray[BANKINDEX(texHandle)]->pTexArray[TEXINDEX(texHandle)];
    ZAssert(pTexture->bValid == true);

	return pTexture->texFormat;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// GetPower2()
//
////////////////////////////////////////////////////////////////////////////////////////////////////
uint32_t CVRAMManager::GetPower2(uint32_t dwInitialValue)
{
    uint32_t dwVal = dwInitialValue, dwNumBits = 0, dwCount = 0;

	while (dwVal != 0)
	{
		if ((dwVal & 0x00000001) == 1)
		{
			dwNumBits++;
		}
		dwVal = dwVal >> 1;
		dwCount++;
	}

	if (dwNumBits == 1)
	{
		return dwInitialValue;
	}

	return (1 << dwCount);
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// GetTextureSurface()
// Must remember to release the surface.
////////////////////////////////////////////////////////////////////////////////////////////////////
HRESULT CVRAMManager::GetTextureSurface(TEXHANDLE texHandle, IDirect3DSurface9 ** ppSurface)
{
    ZAssert(texHandle != INVALID_TEX_HANDLE);
	STexture * pTexture = &m_sVRAM.ppBankArray[BANKINDEX(texHandle)]->pTexArray[TEXINDEX(texHandle)];
    ZAssert(pTexture->bValid == true);
    ZAssert(pTexture->bSystemMemory == false);

	return pTexture->pTexture->GetSurfaceLevel(0, ppSurface);
}


////////////////////////////////////////////////////////////////////////////////////////////////////
// PushRenderTarget()
//
////////////////////////////////////////////////////////////////////////////////////////////////////
HRESULT CVRAMManager::PushRenderTarget(TEXHANDLE texHandle, uint32_t dwTargetIndex /*=0*/)
{
	HRESULT hr;

    ZAssert(dwTargetIndex == 0);			// Only support target index zero for now.
    ZAssert(texHandle != INVALID_TEX_HANDLE);
	STexture * pTexture = &m_sVRAM.ppBankArray[BANKINDEX(texHandle)]->pTexArray[TEXINDEX(texHandle)];
    ZAssert(pTexture->bValid == true);
    ZAssert(pTexture->bRenderTarget == true);

	// Do we need to recreate this render target?
	if ((pTexture->bLocatedInDefaultPool == true) &&
		(pTexture->bResourceEvicted == true))
	{
		// Yes, must have been destroyed following a device reset.
        ZAssert(pTexture->pTexture == NULL);
		hr = CreateRenderTarget(texHandle, pTexture->dwOriginalWidth, pTexture->dwOriginalHeight);
        ZAssert(hr == D3D_OK);
	}

	CD3DDevice9 * pDev = CD3DDevice9::Get();
	hr = pDev->Device()->GetRenderTarget(0, &m_sVRAM.pTargetStack[m_sVRAM.dwNumTargetsPushed].pTargetSurface);
	m_sVRAM.pTargetStack[m_sVRAM.dwNumTargetsPushed].hRenderTargetTexture = m_sVRAM.hCurrentTargetTexture;

	if (hr == D3D_OK)
	{
		ULONG refCount;

		if ((pDev->IsAntiAliased() == true) &&
			(m_sVRAM.dwNumTargetsPushed == 0))
		{
			hr = pDev->SetRTDepthStencil();
            ZAssert(hr == D3D_OK);
		}
		m_sVRAM.dwNumTargetsPushed++;

		//  BT - 7/15 - Added try/catch
		LPDIRECT3DSURFACE9 lpRTSurface = 0;

		try
		{
			hr = pTexture->pTexture->GetSurfaceLevel(0, &lpRTSurface);
            ZAssert(hr == D3D_OK);
			hr = pDev->Device()->SetRenderTarget(dwTargetIndex, lpRTSurface);
            ZAssert(hr == D3D_OK);

			// We've finished with the surface.
			refCount = lpRTSurface->Release();
		}
		catch (...)
		{
			// BT - 7/15 - There was a memory access error when getting GetSurfaceLevel(0), so disable the device's color buffer.
			// It appears to happen when the chat window is being drawn while in full screen mode.
			hr = pDev->Device()->SetRenderTarget(dwTargetIndex, NULL);

			if (lpRTSurface > 0)
				refCount = lpRTSurface->Release();
		}



		//LPDIRECT3DSURFACE9 lpRTSurface;
		//hr = pTexture->pTexture->GetSurfaceLevel(0, &lpRTSurface);
        //ZAssert(hr == D3D_OK);
		//hr = pDev->Device()->SetRenderTarget(dwTargetIndex, lpRTSurface);
        //ZAssert(hr == D3D_OK);

		//// We've finished with the surface.
		//refCount = lpRTSurface->Release();

		m_sVRAM.hCurrentTargetTexture = texHandle;

		// Update viewport.
		D3DVIEWPORT9 viewport;
		viewport.X = 0;
		viewport.Y = 0;
		viewport.Width = pTexture->dwOriginalWidth;
		viewport.Height = pTexture->dwOriginalHeight;
		viewport.MinZ = 0.0f;
		viewport.MaxZ = 1.0f;
		hr = pDev->Device()->SetViewport(&viewport);
        ZAssert(hr == D3D_OK);

		D3DRECT clearRect;
		clearRect.x1 = viewport.X;
		clearRect.x2 = pTexture->dwOriginalWidth;
		clearRect.y1 = viewport.Y;
		clearRect.y2 = pTexture->dwOriginalHeight;

		hr = pDev->Device()->Clear(1,
			&clearRect,
			D3DCLEAR_TARGET,
			0x00000000,
			1.0f,
			0);
        ZAssert(hr == D3D_OK);
	}
	return hr;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// PopRenderTarget()
//
////////////////////////////////////////////////////////////////////////////////////////////////////
HRESULT CVRAMManager::PopRenderTarget(uint32_t dwTargetIndex /*=0*/)
{
	HRESULT hr;
	D3DSURFACE_DESC surfdesc;

    ZAssert(dwTargetIndex == 0);				// Only support target index zero for now.
    ZAssert(&m_sVRAM.dwNumTargetsPushed > 0);

	CD3DDevice9 * pDev = CD3DDevice9::Get();

	m_sVRAM.dwNumTargetsPushed--;
	hr = pDev->Device()->SetRenderTarget(dwTargetIndex, m_sVRAM.pTargetStack[m_sVRAM.dwNumTargetsPushed].pTargetSurface);
    ZAssert(hr == D3D_OK);
	m_sVRAM.hCurrentTargetTexture = m_sVRAM.pTargetStack[m_sVRAM.dwNumTargetsPushed].hRenderTargetTexture;

	m_sVRAM.pTargetStack[m_sVRAM.dwNumTargetsPushed].pTargetSurface->GetDesc(&surfdesc);
	// Update viewport.
	D3DVIEWPORT9 viewport;
	viewport.X = 0;
	viewport.Y = 0;
	viewport.Width = surfdesc.Width;
	viewport.Height = surfdesc.Height;
	viewport.MinZ = 0.0f;
	viewport.MaxZ = 1.0f;
	hr = pDev->Device()->SetViewport(&viewport);
    ZAssert(hr == D3D_OK);

	m_sVRAM.pTargetStack[m_sVRAM.dwNumTargetsPushed].pTargetSurface->Release();
	m_sVRAM.pTargetStack[m_sVRAM.dwNumTargetsPushed].pTargetSurface = NULL;
	m_sVRAM.pTargetStack[m_sVRAM.dwNumTargetsPushed].hRenderTargetTexture = INVALID_TEX_HANDLE;

	if ((pDev->IsAntiAliased() == true) &&
		(m_sVRAM.dwNumTargetsPushed == 0))
	{
		hr = pDev->SetBackBufferDepthStencil();
        ZAssert(hr == D3D_OK);
	}
	return hr;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// SetEnableMipMapGeneration()
// If bMipMapGenerationEnabled is true, any textures created will also generate mipmaps.
////////////////////////////////////////////////////////////////////////////////////////////////////
void CVRAMManager::SetEnableMipMapGeneration(bool bEnable)
{
	// Don't allow set to true if the device doesn't support auto mipmap generation.
	//	m_sVRAM.bMipMapGenerationEnabled = false;
	m_sVRAM.bMipMapGenerationEnabled = bEnable && CD3DDevice9::Get()->GetDevFlags()->bCanAutoGenMipMaps;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
// GetPixelSize()
//
////////////////////////////////////////////////////////////////////////////////////////////////////
uint32_t CVRAMManager::GetPixelSize(D3DFORMAT pixelFormat)
{
    uint32_t dwPixelSize = 0;
	switch (pixelFormat)
	{
	case D3DFMT_A1R5G5B5:
	case D3DFMT_R5G6B5:
		dwPixelSize = 2;
		break;
	case D3DFMT_A8B8G8R8:
	case D3DFMT_A8R8G8B8:
	case D3DFMT_X8R8G8B8:
		dwPixelSize = 4;
		break;
	default:
        ZAssert(false && "Unsupported texture format.");
		dwPixelSize = 1;
	}
	return dwPixelSize;
}
