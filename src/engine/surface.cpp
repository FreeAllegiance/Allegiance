#include "surface.h"

#include <base.h>

#include "D3DDevice9.h"
#include "enginep.h"
#include "ImageTransfer.h"
#include "UIVertexDefn.h"
#include "VertexGenerator.h"

//////////////////////////////////////////////////////////////////////////////
//
// Surface
//
//////////////////////////////////////////////////////////////////////////////

class PrivateSurfaceImpl : public PrivateSurface {
public:
    //////////////////////////////////////////////////////////////////////////////
    //
    // Members
    //
    //////////////////////////////////////////////////////////////////////////////

    //
    // Debug Info
    //

    ZString                   m_strName;
    bool                      m_bDeviceFormat;
    bool                      m_bInContext;

    //
    // Surface Description
    //

    WinPoint                  m_size;
    TRef<PixelFormat>         m_ppf;

    //
    // Device Format surfaces
    //

    TRef<PrivateEngine>       m_pengine;
    TRef<SurfaceSite>         m_psite;

    //
    // Video Surface
//    TRef<DDSurface>				m_pvideoSurface;
    SurfaceType					m_stype;

    //
    // Drawing
    //

    TRef<PrivateContext>      m_pcontext;

    //
    // Transforms and clipping
    //

    WinPoint                  m_pointOffset;
    WinRect                   m_rectClip;
    WinPoint                  m_pointOffsetSave;
    WinRect                   m_rectClipSave;

	// Additional stuff.
	bool						m_bSurfaceAllocated;
	TEXHANDLE					m_hTexture;					// Handle of texture allocated by VRAM manager.

	// UI vertex storage.
	// This is just temporary to get the whole lot up and running.
	// Each surface configures a bunch of rhw vertices instead of using the old DDraw blt functions.
	// Ideally, this lot could go in one large dynamic VB.
	//UIVERTEX					m_pUIVertex[4];
	DWORD						m_dwNumVerts;
	DWORD						m_dwNumPolys;
	UIVERTEX *					m_pUIVerts;

    //////////////////////////////////////////////////////////////////////////////
    //
    //
    //
    //////////////////////////////////////////////////////////////////////////////

    void Initialize()
    {
        m_pointOffset = WinPoint(0, 0);
        m_rectClip    = WinRect(WinPoint(0, 0), m_size);

        m_bInContext  = false;

		// Added.
		m_bSurfaceAllocated		= false;
		m_hTexture				= INVALID_TEX_HANDLE;
		m_pUIVerts				= NULL;
		m_dwNumPolys			= 0;
		m_dwNumVerts			= 0;
    }

    //////////////////////////////////////////////////////////////////////////////
    // AllocateSurface()
    // Surface type no longer needed. SurfaceTypeZ is not supported and the
    // other 3 types are all the same thing now - a d3d9 texture.
    //////////////////////////////////////////////////////////////////////////////
    void AllocateSurface(const char * szTextureName = NULL, D3DFORMAT d3dOverrideFormat = D3DFMT_UNKNOWN )
    {
		HRESULT hr;
		bool bSystemMemory = m_stype.Test( SurfaceTypeSystemMemory() );
		CVRAMManager * pVRAMMan = CVRAMManager::Get();

		// Allocate a handle.
		// This function can be called even after a handle has been allocated. Not entirely sure
		// why at the moment.
		if( m_hTexture == INVALID_TEX_HANDLE )
		{
			m_hTexture = pVRAMMan->AllocateHandle();
		}

		// If the size is valid, we create the resource now.
		if( ( m_size.X() != 1 ) || ( m_size.Y() != 1 ) )
		{
            hr = pVRAMMan->CreateTexture(m_hTexture,
                d3dOverrideFormat != D3DFMT_UNKNOWN ? d3dOverrideFormat : m_ppf->GetEquivalentD3DFormat(),
                m_size.X(),
                m_size.Y(),
                false,
                szTextureName,
                ALLEG_D9EX_IF(bSystemMemory ? 0 : D3DUSAGE_DYNAMIC, 0),
                bSystemMemory ? D3DPOOL_SCRATCH : ALLEG_D9EX_IF(D3DPOOL_DEFAULT, D3DPOOL_MANAGED)
            );
            ZAssert( hr == D3D_OK );
            m_ppf = new PixelFormat(pVRAMMan->GetTextureFormat(m_hTexture));
			m_bSurfaceAllocated = true;
		}
	}

    //////////////////////////////////////////////////////////////////////////////
    // AllocateRenderTarget()
    // Surface type no longer needed. SurfaceTypeZ is not supported and the
    // other 3 types are all the same thing now - a d3d9 texture.
    //////////////////////////////////////////////////////////////////////////////
	void AllocateRenderTarget( )
    {
		HRESULT hr;

		// Allocate a handle.
		// This function can be called even after a handle has been allocated. Not entirely sure
		// why at the moment.
        ZAssert( m_hTexture == INVALID_TEX_HANDLE );
		if( m_hTexture == INVALID_TEX_HANDLE )
		{
			m_hTexture = CVRAMManager::Get()->AllocateHandle( );
		}

		// If the size is valid, we create the resource now.
		if( ( m_size.X() != 1 ) || ( m_size.Y() != 1 ) )
		{
			hr = CVRAMManager::Get()->CreateRenderTarget(	m_hTexture,
													m_size.X(),
													m_size.Y() );
            ZAssert( hr == D3D_OK );
            m_ppf = new PixelFormat(CVRAMManager::Get()->GetTextureFormat(m_hTexture));
			m_bSurfaceAllocated = true;
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	// PrivateSurfaceImpl()
	// This constructor creates a dummy top-level surface.
	// Add a colour key here!
	////////////////////////////////////////////////////////////////////////////////////////////////
	PrivateSurfaceImpl( PrivateEngine * pengine, 
						const WinPoint & size, 
						SurfaceSite * psite,
						SurfaceType stype ) :
		m_pengine( pengine ),
		m_stype( stype ),
		m_bDeviceFormat( true ),
		m_size( size ),
		m_psite( psite ),
		m_bInContext( false ),
		m_bSurfaceAllocated( false ),
		m_hTexture( INVALID_TEX_HANDLE ),
		m_pcontext( NULL )
	{
		Initialize();
        m_pointOffset			= WinPoint(0, 0);
        m_rectClip				= WinRect(WinPoint(0, 0), m_size);
		m_pUIVerts				= NULL;
		m_dwNumPolys			= 0;
		m_dwNumVerts			= 0;

		if( m_stype.Test( SurfaceTypeDummy() ) == true )
		{
			m_ppf = new PixelFormat( CD3DDevice9::Get()->GetCurrentMode()->mode.Format ); //Fix memory leak -Imago 8/2/09
		}
		else
		{
            ZAssert( m_stype.Test( SurfaceTypeRenderTarget() ) == true );
			AllocateRenderTarget( );
		}

        if (m_psite)
            m_psite->UpdateSurface(this);
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	// PrivateSurfaceImpl()
	//
	////////////////////////////////////////////////////////////////////////////////////////////////
    PrivateSurfaceImpl( D3DFORMAT texFormat, DWORD dwWidth, DWORD dwHeight, const char * szTexName /*=NULL*/ ) :
			m_pengine( NULL ),
			m_bDeviceFormat(false),
			m_stype( 0 ),
//			m_pvideoSurface( NULL ),
			m_ppf( NULL ),
			m_size( dwWidth, dwHeight ),
			m_psite( NULL ),
			m_hTexture( INVALID_TEX_HANDLE )
	{
		Initialize();
		AllocateSurface( szTexName, texFormat );
	}



    //////////////////////////////////////////////////////////////////////////////
	// GetTexHandle()
	//
    //////////////////////////////////////////////////////////////////////////////
	TEXHANDLE GetTexHandle( )
	{
        ZAssert(m_hTexture != INVALID_TEX_HANDLE);
		return m_hTexture;
	}

    //////////////////////////////////////////////////////////////////////////////
    //
    // A device format surface
    //
    //////////////////////////////////////////////////////////////////////////////

    PrivateSurfaceImpl(
        PrivateEngine*  pengine,
        PixelFormat*    ppf,
        const WinPoint& size,
        SurfaceType     stype,
        SurfaceSite*    psite
    ) :
        m_pengine(pengine),
        m_ppf(ppf),
        m_size(size),
        m_bDeviceFormat(true),
        m_stype(stype),
        m_psite(psite),
		m_hTexture(INVALID_TEX_HANDLE)
    {
        Initialize();
        AllocateSurface();

        if (m_psite)
            m_psite->UpdateSurface(this);
    }

	//////////////////////////////////////////////////////////////////////////////
    //
    // Create a surface from a binary representation
    // The binary representation in pdata.
    //////////////////////////////////////////////////////////////////////////////
	PrivateSurfaceImpl( PrivateEngine *			pengine, 
						const D3DXIMAGE_INFO *  pImageInfo, 
						const WinPoint *		pTargetSize, 
						IObject *				pobjectMemory, 
						const bool				bColorKey, 
						const Color &			cColorKey, 
						const ZString &			szTextureName, 
						const bool				bSystemMemory ) :
				m_pengine(pengine),
				m_size(*pTargetSize),
				m_bDeviceFormat(false),
				m_stype(SurfaceType2D()),
				m_hTexture( INVALID_TEX_HANDLE )

	{
		HRESULT hr;

		if( bSystemMemory == true )
		{
			m_stype = SurfaceType2D() | SurfaceTypeSystemMemory();
		}

		// Reset object state.
        Initialize();

		// Store these values here, after Initialize has cleared out all values.
		ZFile * pFile = (ZFile*) pobjectMemory;
		
		// Generate the filename data.
		char szTemp[32];
		if( szTextureName.GetLength() > 31 ) 
		{
			strcpy_s( szTemp, 32, &szTextureName[ szTextureName.GetLength() - 31 ] );
		}
		else
		{
			strcpy_s( szTemp, 32, &szTextureName[ 0 ] );
		}
		if( m_hTexture == INVALID_TEX_HANDLE )
		{
			m_hTexture = CVRAMManager::Get()->AllocateHandle();
			hr = CVRAMManager::Get()->CreateTextureD3DX(
				m_hTexture,
				pImageInfo,
				pTargetSize,
                pobjectMemory,
				bColorKey, 
				cColorKey, 
				szTemp );
            ZAssert( hr == D3D_OK );
		}
        m_ppf = new PixelFormat(CVRAMManager::Get()->GetTextureFormat(m_hTexture));
	}

	//////////////////////////////////////////////////////////////////////////////
    //
    // Create a surface from a binary representation
    // The binary representation in pdata.
    //////////////////////////////////////////////////////////////////////////////

    PrivateSurfaceImpl(	PrivateEngine*		pengine,
						PixelFormat*		ppf,
						const WinPoint&		size,
						int					pitch,
						BYTE*				pdata,
						IObject*			pobjectMemory,
						const bool			bColorKey,
						const Color &		cColorKey,
						const ZString &		szTextureName,
						const bool			bSystemMemory ) :
				m_pengine(pengine),
				m_bDeviceFormat(false),
				m_stype(SurfaceType2D()),
				m_size(size),
				m_ppf(ppf),
				m_hTexture( INVALID_TEX_HANDLE )
    {
		if( bSystemMemory == true )
		{
			m_stype = SurfaceType2D() | SurfaceTypeSystemMemory();
		}

		// Reset object state.
        Initialize();

		// Generate the filename data.
		char szTemp[32];
		if( szTextureName.GetLength() > 31 ) 
		{
			strcpy_s( szTemp, 32, &szTextureName[ szTextureName.GetLength() - 31 ] );
		}
		else
		{
			strcpy_s( szTemp, 32, &szTextureName[ 0 ] );
		}

		// Need an alpha texture to implement colour keying.
		// If the texture is 16 bit and the device supports the A1R5G5B5 texture format, we use one 
		// of those, otherwise unfortunately we need to use a 32 bit A8R8G8B8 texture.
		if( bColorKey == true )
		{
			if( ( ppf->PixelBytes() == 2 ) &&
				( CD3DDevice9::Get()->GetDevFlags()->bSupportsA1R5G6B6Format == true ) )
			{
				// Create a A1R5G5B5 texture.
				AllocateSurface( szTemp, D3DFMT_A1R5G5B5 );

                D3DLOCKED_RECT lockRect;
                HRESULT hr;
                hr = CVRAMManager::Get()->LockTexture(m_hTexture, &lockRect);

				// Copy texture in, updating alpha for colour.
                CImageTransfer::Transfer16BitTo16BitWithColourKey(pdata, pitch, (BYTE*)lockRect.pBits, lockRect.Pitch, WinPoint(0, 0), WinPoint(0, 0), size, cColorKey);

                // Unlock texture.
                hr = CVRAMManager::Get()->UnlockTexture(m_hTexture);
			}
			else
			{
				// Only 16 bit source textures supported at the moment.
                ZAssert( ppf->PixelBytes() >= 2 );

				// Create a A8R8G8B8 texture.
				AllocateSurface( szTemp, D3DFMT_A8R8G8B8 );


                // Lock target texture and copy over.
                D3DLOCKED_RECT lockRect;
                HRESULT hr;
                hr = CVRAMManager::Get()->LockTexture(m_hTexture, &lockRect);

				switch( ppf->PixelBytes() )
				{
				case 2:
					// Copy texture in, updating alpha for colour.
                    CImageTransfer::Transfer16BitTo32BitWithColourKey(pdata, pitch, (BYTE*)lockRect.pBits, lockRect.Pitch, WinPoint(0, 0), WinPoint(0, 0), size, cColorKey);
					break;
				default:
					// Unsupported pixel format.
                    ZAssert( false );
				}

                // Unlock texture.
                hr = CVRAMManager::Get()->UnlockTexture(m_hTexture);
			}
		}
		else
		{
			// Allocate a new D3D resource, and build texture from the passed in data.
			AllocateSurface( szTemp );

            ZAssert( m_hTexture != INVALID_TEX_HANDLE );

			// Lock this texture and copy over.
			D3DLOCKED_RECT lockRect;
			HRESULT hr;

			hr = CVRAMManager::Get()->LockTexture( m_hTexture, &lockRect );
            ZAssert( hr == D3D_OK );

			// Copy over data. Only works for 16 bit texture at the moment.
			ZAssert( m_ppf->PixelBytes() >= 2 );
			int iPitch				= pitch;
			const BYTE* pSrc		= pdata;
			BYTE * pDest			= (BYTE *) lockRect.pBits;
			int scanSize			= m_size.X() * 2;
			BYTE * pCurr;

			int x, y;

			switch( m_ppf->PixelBytes() )
			{
			case 2:
				// Copy the data over. Need to invert the data in the y direction.
				for( y=0; y<m_size.Y(); y++ )
				{
					for( x=0; x<m_size.X(); x +=2 )
					{
	//					pDest[	( ( m_size.Y() - y - 1 ) * lockRect.Pitch ) + ( x * 2 ) + 0 ] = pSrc[ ( y * iPitch ) + ( x * 2 ) + 0 ];
	//					pDest[	( ( m_size.Y() - y - 1 ) * lockRect.Pitch ) + ( x * 2 ) + 1 ] = pSrc[ ( y * iPitch ) + ( x * 2 ) + 1 ];
	//					pDest[	( ( m_size.Y() - y - 1 ) * lockRect.Pitch ) + ( x * 2 ) + 2 ] = pSrc[ ( y * iPitch ) + ( x * 2 ) + 2 ];
	//					pDest[	( ( m_size.Y() - y - 1 ) * lockRect.Pitch ) + ( x * 2 ) + 3 ] = pSrc[ ( y * iPitch ) + ( x * 2 ) + 3 ];
						pDest[	( y * lockRect.Pitch ) + ( x * 2 ) + 0 ] = pSrc[ ( y * iPitch ) + ( x * 2 ) + 0 ];
						pDest[	( y * lockRect.Pitch ) + ( x * 2 ) + 1 ] = pSrc[ ( y * iPitch ) + ( x * 2 ) + 1 ];
						pDest[	( y * lockRect.Pitch ) + ( x * 2 ) + 2 ] = pSrc[ ( y * iPitch ) + ( x * 2 ) + 2 ];
						pDest[	( y * lockRect.Pitch ) + ( x * 2 ) + 3 ] = pSrc[ ( y * iPitch ) + ( x * 2 ) + 3 ];
					}
				}
				break;

			case 3:
				// Copy the data over. Need to invert the data in the y direction.
				for( y=0; y<m_size.Y(); y++ )
				{
					pCurr = (BYTE*) &pSrc[ y * iPitch ];
					for( x=0; x<m_size.X(); x ++ )
					{
						pDest[	( y * lockRect.Pitch ) + ( x * 4 ) + 0 ] = *pCurr++;
						pDest[	( y * lockRect.Pitch ) + ( x * 4 ) + 1 ] = *pCurr++;
						pDest[	( y * lockRect.Pitch ) + ( x * 4 ) + 2 ] = *pCurr++;
						pDest[	( y * lockRect.Pitch ) + ( x * 4 ) + 3 ] = 255;
					}
				}
				break;

			case 4:
				// Copy the data over. Need to invert the data in the y direction.
				for( y=0; y<m_size.Y(); y++ )
				{
					pCurr = (BYTE*) &pSrc[ y * iPitch ];
					for( x=0; x<m_size.X(); x ++ )
					{
						pDest[	( y * lockRect.Pitch ) + ( x * 4 ) + 0 ] = *pCurr++;
						pDest[	( y * lockRect.Pitch ) + ( x * 4 ) + 1 ] = *pCurr++;
						pDest[	( y * lockRect.Pitch ) + ( x * 4 ) + 2 ] = *pCurr++;
						pDest[	( y * lockRect.Pitch ) + ( x * 4 ) + 3 ] = *pCurr++;
					}
				}
				break;

			default:
                ZAssert( false );
			}

			// Unlock texture.
			hr = CVRAMManager::Get()->UnlockTexture( m_hTexture );
		}

		// Test if a resize is required.
		if( bSystemMemory == false )
		{
			DWORD dwMaxTextureSize = CD3DDevice9::Get()->GetDeviceSetupParams()->dwMaxTextureSize;

			if( dwMaxTextureSize != 0 )
			{
				// Only apply texture reduction to power of 2 textures, otherwise
				// we assume it's by design for the UI.
                ZAssert( dwMaxTextureSize <= 2048 );			// 2048 max for time being.

				if( ( ( m_size.x == 512 ) || ( m_size.x == 1024 ) || ( m_size.x == 2048 ) ) &&
					( ( m_size.y == 512 ) || ( m_size.y == 1024 ) || ( m_size.y == 2048 ) ) )
				{
					WinPoint originalSize = m_size;

					while(	( m_size.x > (LONG)dwMaxTextureSize ) ||
							( m_size.y > (LONG)dwMaxTextureSize ) )
					{
						m_size.x = m_size.x >> 1;
						m_size.y = m_size.y >> 1;
					}

					if( m_size != originalSize )
					{
						// Create another texture and use D3DX to resize.
						HRESULT hr;
						TEXHANDLE hOld = m_hTexture;
						m_hTexture = INVALID_TEX_HANDLE;

						AllocateSurface( szTemp, CVRAMManager::Get()->GetTextureFormat( hOld ) );
                        ZAssert( m_hTexture != INVALID_TEX_HANDLE );
						
						DWORD dwColourKey = bColorKey ? 0xFF000000 : 0;
						LPDIRECT3DSURFACE9 pDest, pSrc;
						CVRAMManager::Get()->GetTextureSurface( hOld, &pSrc );
						CVRAMManager::Get()->GetTextureSurface( m_hTexture, &pDest );
						hr = D3DXLoadSurfaceFromSurface(	pDest, NULL, NULL,
															pSrc, NULL, NULL, 
															D3DX_DEFAULT, dwColourKey );
						pDest->Release();
						pSrc->Release();

						CVRAMManager::Get()->ReleaseHandle( hOld );

#ifdef _DEBUG
						char szBuffer[256];
						sprintf_s( szBuffer, 256, "Resized to %d x %d: %s\n",
							m_size.x, m_size.y, szTemp );
						OutputDebugString( szBuffer );
#endif // _DEBUG
					}
				}
			}
		}
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // More constructors
    //
    //////////////////////////////////////////////////////////////////////////////

    TRef<Surface> CreateCompatibleSurface(const WinPoint& size, SurfaceType stype, SurfaceSite* psite)
    {
        return m_pengine->CreateCompatibleSurface(this, size, stype, psite);
    }

    TRef<Surface> Copy()
    {
        TRef<Surface> psurface = m_pengine->CreateCompatibleSurface(this, m_size, GetSurfaceType(), m_psite);

        //  : turn off color key while blting

        psurface->BitBlt(WinPoint(0, 0), this);
        return psurface;
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Destructor
    //
    //////////////////////////////////////////////////////////////////////////////

    ~PrivateSurfaceImpl()
    {
		if( m_pengine )
		{
			m_pengine->RemovePrivateSurface(this);
		}

		// Release associated texture.
		if( m_hTexture != INVALID_TEX_HANDLE )			// Dummy surfaces == INVALID_TEX_HANDLE
		{
			if( CVRAMManager::Get()->ReleaseHandle( m_hTexture ) == true )
			{
				// Function returns true if resource ref count reaches zero.
				// Just a double check really.
				m_hTexture = INVALID_TEX_HANDLE;
			}
		}
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Site
    //
    //////////////////////////////////////////////////////////////////////////////

    void SetSite(SurfaceSite* psite)
    {
        m_psite = psite;
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Device Specific Surface
    //
    //////////////////////////////////////////////////////////////////////////////

    void ClearDevice()
    {
//        m_pvideoSurface = NULL;
    }

    void SetPixelFormat(PixelFormat* ppf)
    {
//        ZAssert(m_ppalette      == NULL);

        m_pcontext      = NULL;
//        m_pvideoSurface = NULL;

        m_ppf = ppf;
        AllocateSurface();

        if (m_psite) {
            m_psite->UpdateSurface(this);
        }
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Attributes
    //
    //////////////////////////////////////////////////////////////////////////////


    PixelFormat* GetPixelFormat()
    {
        return m_ppf;
    }

    void SetName(const ZString& str)
    {
        m_strName = str;
    }

    const ZString& GetName()
    {
        return m_strName;
    }

    Engine* GetEngine()
    { 
        return m_pengine;               
    }

    const WinPoint& GetSize()
    { 
        return m_size;                  
    }

    WinRect GetRect()
    {
        return 
            WinRect(
                WinPoint(0, 0),
                m_size
            );
    }

    SurfaceType GetSurfaceType() 
    { 
        return m_stype;           
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Drawing context
    //
    //////////////////////////////////////////////////////////////////////////////

    Context* GetContext()
    {
		ZAssert(!m_bInContext);

		m_bInContext = true;

        if (m_pcontext == NULL) 
		{
			m_pcontext = CreateContextImpl(this);
        }

        if (m_pcontext->IsValid()) {
            //
            // Remove any offset or clipping
            //

            m_rectClipSave    = m_rectClip;
            m_rectClip        = WinRect(WinPoint(0, 0), m_size);
            m_pointOffsetSave = m_pointOffset;
            m_pointOffset     = WinPoint(0, 0);

            m_pcontext->BeginRendering();

            return m_pcontext;
        }

        m_pcontext   = NULL;
        m_bInContext = false;
        return NULL;
    }

    void ReleaseContext(Context* pcontext)
    {
		ZAssert(m_bInContext);
        ZAssert(m_pcontext == pcontext);

		m_bInContext = false;

        m_pcontext->EndRendering();

        m_pointOffset = m_pointOffsetSave;
        m_rectClip    = m_rectClipSave;
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Clipping and transforms
    //
    //////////////////////////////////////////////////////////////////////////////

    void Offset(const WinPoint& pointOffset)
    {
        m_pointOffset += pointOffset;
    }

    const WinPoint& GetOffset()
    {
        return m_pointOffset;
    }

    WinRect GetClipRect()
    {
        WinRect rect = m_rectClip;
        rect.Offset(-m_pointOffset);
        return rect;
    }

    void SetClipRect(const WinRect& rectLocal)
    {
        WinRect rectClip(rectLocal);
        rectClip.Offset(m_pointOffset);
        m_rectClip.Intersect(rectClip);
    }

    void RestoreClipRect(const WinRect& rectLocal)
    {
        m_rectClip = rectLocal;
        m_rectClip.Offset(m_pointOffset);
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // GDI calls
    //
    //////////////////////////////////////////////////////////////////////////////

    void DrawString(
        IEngineFont* pfont, 
        const Color& color, 
        const WinPoint& point, 
        const ZString& str
    ) {
        pfont->DrawStringScreenSpace(
            point + m_pointOffset, 
            str, 
            color
        );
    }

    void DrawStringWithShadow(
        IEngineFont* pfont, 
        const Color& color, 
        const Color& colorShadow,
        const WinPoint& point, 
        const ZString& str
    ) {
        DrawString(pfont, colorShadow, point + WinPoint(1, 1), str);
        DrawString(pfont, color      , point                 , str);
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Blts
    //
    //////////////////////////////////////////////////////////////////////////////
    void UnclippedBlt(	const WinRect& rect, 
				        PrivateSurfaceImpl* psurfaceSourceOriginal, 
						const WinPoint& pointSource) 
	{
        if (rect.XSize() == 0 || rect.YSize() == 0) {
            return;            
        }

		HRESULT hr;
		CD3DDevice9 * pDev = CD3DDevice9::Get();
		if( pDev->IsInScene() == false )
		{
//			OutputDebugString("TODO: implement software blt.\n");
			// Software based texture copy.
			CopySubsetFromSrc( WinPoint(rect.XMin(), rect.YMin() ),
								psurfaceSourceOriginal,
								rect );
		}
		else
		{
			// Note: this call also configures the stream.
			CVertexGenerator::Get()->GenerateUITexturedVertices(	
							psurfaceSourceOriginal->GetTexHandle(), 
							rect, 
							true );
			// Render this pane.
			pDev->SetRenderState( D3DRS_ZENABLE, D3DZB_FALSE );
			hr = CVRAMManager::Get()->SetTexture( psurfaceSourceOriginal->GetTexHandle(), 0 );
			hr = pDev->SetFVF( D3DFVF_UIVERTEX );
			hr = pDev->DrawPrimitive( 
						D3DPT_TRIANGLESTRIP,
//						CVertexGenerator::GetUITexVertsVB()->dwFirstElementOffset, 
						CVertexGenerator::Get()->GetPredefinedDynamicBuffer( 
										CVertexGenerator::ePDBT_UITexVB )->dwFirstElementOffset,
						2 );
            ZAssert( hr == D3D_OK );
		}
    }

	////////////////////////////////////////////////////////////////////////////////////////////////
	// CopySubsetFromSrc()
	// Copy the area specified in "rectSourceArg" from the source surface to this one, at the coords
	// specified by "point".
	// If an ASSERT is encountered in the switch statement, it probably means that the code
	// hasn't been implemented for a particular source/dest combination.
	////////////////////////////////////////////////////////////////////////////////////////////////
    void CopySubsetFromSrc(const WinPoint& point, Surface* psurfaceSourceArg, const WinRect& rectSourceArg)
    {
        PrivateSurfaceImpl* psurfaceSource = (PrivateSurfaceImpl*)psurfaceSourceArg;

        // Calculate the target rectangle
        WinPoint origin = point + m_pointOffset;
        WinRect rectTarget(origin, origin + rectSourceArg.Size());
		HRESULT hr;

        // Clip it to the surface
        rectTarget.Intersect(m_rectClip);

        // Only blt if the target isn't empty
        if (!rectTarget.IsEmpty()) 
		{
            // adjust the source rect to match the target rect
            WinPoint SourceOrigin = rectSourceArg.Min() + (rectTarget.Min() - origin);
            WinRect rectSource(SourceOrigin, SourceOrigin + rectTarget.Size());

            // do the blt
            ZAssert( m_hTexture != INVALID_TEX_HANDLE );

            if (m_stype.Test(SurfaceTypeDummy()) == true) {
                // we are attempting to draw to a dummy surface, this should never happen
                debugf("Drawing to dummy surface: CopySubsetFromSrc");
                ZAssert(false);
                return;
            }

			// Lock this texture and copy over.
			D3DLOCKED_RECT lockRect;
			hr = CVRAMManager::Get()->LockTexture( m_hTexture, &lockRect );

			// BT - 10/17 - Fixing VRAMManager crash when a surface that doesn't have a texture is attempted to be painted.
			if (FAILED(hr))
				return;

            ZAssert( hr == D3D_OK );

            D3DLOCKED_RECT lockRectSrc;
            hr = CVRAMManager::Get()->LockTexture(psurfaceSource->GetTexHandle(), &lockRectSrc, 0, 0, &rectSource);
            if (FAILED(hr))
                return;
			switch( psurfaceSource->GetPixelFormat()->GetD3DFormat() )
			{
            case D3DFMT_A1R5G5B5:
                switch (m_ppf->GetD3DFormat())
                {
                case D3DFMT_A1R5G5B5:
                    CImageTransfer::Transfer16BitTo16BitCopy(
                        (BYTE *)lockRectSrc.pBits,
                        lockRectSrc.Pitch,
                        (BYTE *)lockRect.pBits,
                        lockRect.Pitch,
                        WinPoint(0, 0),
                        WinPoint(0, 0),
                        WinPoint(rectTarget.XSize(), rectTarget.YSize())
                    );
                    break;
                default:
                    ZAssert(false);
                    break;
                }
                break;
			case D3DFMT_R5G6B5:
				{
					switch( m_ppf->GetD3DFormat() )
					{
					case D3DFMT_R5G6B5:
						CImageTransfer::Transfer16BitTo16BitNoColourKey(
									(BYTE *)lockRectSrc.pBits,
                                    lockRectSrc.Pitch,
									(BYTE *) lockRect.pBits,
									lockRect.Pitch,
                                    WinPoint(0, 0),
									WinPoint( 0, 0 ),
									WinPoint( rectTarget.XSize(), rectTarget.YSize() ) );
						break;
					case D3DFMT_X8R8G8B8:
                        ZAssert( false );
						break;
					default:
                        ZAssert( false );
					}
				}
				break;

			case D3DFMT_X8R8G8B8:
				{
					switch( m_ppf->GetD3DFormat() )
					{
					case D3DFMT_A8R8G8B8:
                        ZAssert( false );
						break;
					case D3DFMT_X8R8G8B8:
                        ZAssert( false );
						break;
					default:
                        ZAssert( false );
					}
				}
				break;

			case D3DFMT_A8R8G8B8:
				{
					switch( m_ppf->GetD3DFormat() )
					{
					case D3DFMT_A8R8G8B8:
                        CImageTransfer::Transfer32BitTo32BitCopy(
                            (BYTE *)lockRectSrc.pBits,
                            lockRectSrc.Pitch,
                            (BYTE *)lockRect.pBits,
                            lockRect.Pitch,
                            WinPoint(0, 0),
                            WinPoint(0, 0),
                            WinPoint(rectTarget.XSize(), rectTarget.YSize())
                        );
						break;
					default:
                        ZAssert( false );
					}
				}
				break;

			default:
                ZAssert( false );
			}

			// Unlock texture.
			hr = CVRAMManager::Get()->UnlockTexture( m_hTexture );
            hr = CVRAMManager::Get()->UnlockTexture(psurfaceSource->GetTexHandle());
        }
    }

    void BitBlt(const WinPoint& point, Surface* psurfaceSourceArg, const WinRect& rectSourceArg)
    {
        PrivateSurfaceImpl* psurfaceSource = (PrivateSurfaceImpl*)psurfaceSourceArg;

        //
        // Calculate the target rectangle
        //

        WinPoint origin = point + m_pointOffset;
        WinRect rectTarget(origin, origin + rectSourceArg.Size());

        //
        // Clip it to the surface
        //

//        rectTarget.Intersect(m_rectClip);

        //
        // Only blt if the target isn't empty
        //

        if (!rectTarget.IsEmpty()) 
		{
            //
            // adjust the source rect to match the target rect
            //

            WinPoint SourceOrigin = rectSourceArg.Min() + (rectTarget.Min() - origin);
            WinRect rectSource(SourceOrigin, SourceOrigin + rectTarget.Size());

            //
            // do the blt
			//

            UnclippedBlt(rectTarget, psurfaceSource, rectSource.Min());
			//BitBlt( rectTarget, psurfaceSourceArg, rectSourceArg );
        }
    }

    void BitBlt(const WinPoint& point, Surface* psurfaceSource, bool bLocalCopy = false )
    {
        BitBlt(
            point, 
            psurfaceSource, 
            WinRect(
                WinPoint(0, 0), 
                psurfaceSource->GetSize()
            )
        );
    }

    void BitBltFromCenter(const WinPoint& point, Surface* psurfaceSource)
    {
        BitBlt(
            point - psurfaceSource->GetSize() / 2, 
            psurfaceSource
        );
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // StretchBlts
    // FUNCTION NOT USED.
    //////////////////////////////////////////////////////////////////////////////

    void BitBlt(const WinRect& rectTargetArg, Surface* psurfaceSourceArg, const WinRect& rectSource)
    {
        PrivateSurfaceImpl* psurfaceSource = (PrivateSurfaceImpl*)psurfaceSourceArg;

        ZAssert(false);

        //
        // Calculate the target rectangle
        //

        //ZAssert( false && "Is this called? If not, remove this function." );

        WinRect rectTarget = rectTargetArg;
        rectTarget.Offset(m_pointOffset);

        // !!! this function is unclipped
        // !!! rectTarget.Intersect(m_rectClip);

        //
        // Only blt if the target isn't empty
        if (!rectTarget.IsEmpty()) 
		{
            ZAssert( psurfaceSource->GetTexHandle() != INVALID_TEX_HANDLE );

			UIVERTEX pVerts[6];

			// Calculate texture coordinates.
            uint32_t dwUMax, dwVMax, dwUOMax, dwVOMax;
			float fUMax, fVMax;
			
			CVRAMManager::Get()->GetOriginalDimensions( psurfaceSource->GetTexHandle(), &dwUOMax, &dwVOMax );
			CVRAMManager::Get()->GetActualDimensions( psurfaceSource->GetTexHandle(), &dwUMax, &dwVMax );
			fUMax = 1.0f; //(float) dwUOMax / (float) (dwUMax - 1);
			fVMax = 1.0f; //(float) dwVOMax / (float) (dwVMax - 1);

			pVerts[0].x = (float) rectTarget.XMin();
			pVerts[0].y = (float) rectTarget.YMin();
			pVerts[0].z = 0.5f;
			pVerts[0].rhw = 1.0f;
			pVerts[0].fU = 0.0f;
			pVerts[0].fV = 0.0f;

			pVerts[1].x = (float) rectTarget.XMax();
			pVerts[1].y = (float) rectTarget.YMin();
			pVerts[1].z = 0.5f;
			pVerts[1].rhw = 1.0f;
			pVerts[1].fU = fUMax;
			pVerts[1].fV = 0.0f;

			pVerts[2].x = (float) rectTarget.XMin();
			pVerts[2].y = (float) rectTarget.YMax();
			pVerts[2].z = 0.5f;
			pVerts[2].rhw = 1.0f;
			pVerts[2].fU = 0.0f;
			pVerts[2].fV = fVMax;

			pVerts[3].x = (float) rectTarget.XMin();
			pVerts[3].y = (float) rectTarget.YMax();
			pVerts[3].z = 0.5f;
			pVerts[3].rhw = 1.0f;
			pVerts[3].fU = 0.0f;
			pVerts[3].fV = fVMax;

			pVerts[4].x = (float) rectTarget.XMax();
			pVerts[4].y = (float) rectTarget.YMin();
			pVerts[4].z = 0.5f;
			pVerts[4].rhw = 1.0f;
			pVerts[4].fU = fUMax;
			pVerts[4].fV = 0.0f;

			pVerts[5].x = (float) rectTarget.XMax();
			pVerts[5].y = (float) rectTarget.YMax();
			pVerts[5].z = 0.5f;
			pVerts[5].rhw = 1.0f;
			pVerts[5].fU = fUMax;
			pVerts[5].fV = fVMax;

			CD3DDevice9 * pDev = CD3DDevice9::Get();

			// Render this pane.
			pDev->SetRenderState( D3DRS_ZENABLE, D3DZB_FALSE );

			HRESULT hr = pDev->SetFVF( D3DFVF_UIVERTEX );
            ZAssert( hr == D3D_OK );
			hr = pDev->SetStreamSource( 0, NULL, 0, 0 );
            ZAssert( hr == D3D_OK );
			hr = CVRAMManager::Get()->SetTexture( psurfaceSource->GetTexHandle(), 0 );
            ZAssert( hr == D3D_OK );
			hr = pDev->DrawPrimitiveUP(	D3DPT_TRIANGLELIST,
										2,
										pVerts,
										sizeof( UIVERTEX ) );
            ZAssert( hr == D3D_OK );
        }
    }

    void BitBlt(const WinRect& rectTarget, Surface* psurfaceSource)
    {
        BitBlt(
            rectTarget,
            psurfaceSource, 
            WinRect(
                WinPoint(0, 0), 
                psurfaceSource->GetSize()
            )
        );
    }

    //////////////////////////////////////////////////////////////////////////////
    //
    // Direct Draw Fills
    //
    //////////////////////////////////////////////////////////////////////////////

    void UnclippedFill(const WinRect& rect, Pixel pixel)
    {
        ZAssert(rect.XMin() >= 0         );
        ZAssert(rect.XMax() <= m_size.X());
        ZAssert(rect.YMin() >= 0         );
        ZAssert(rect.YMax() <= m_size.Y());
		
		DWORD dwPixel = pixel.Value();
		dwPixel = dwPixel & 0x00FFFFFF;

		CD3DDevice9 * pDev = CD3DDevice9::Get();
		CVRAMManager * pVRAMMan = CVRAMManager::Get();
		CVertexGenerator * pVertGen = CVertexGenerator::Get();

		if( ( m_stype.Test( SurfaceTypeRenderTarget() ) == true ) &&
			( dwPixel == 0 ) &&
			( true  ) )
		{
			// Clear render target colour and alpha.
			LPDIRECT3DSURFACE9 pSurface = 0;

			// BT - 7/15 - Added try catch to isolate a memory exception error when getting the surface during chat rendering. 
			try
			{
				pVRAMMan->GetTextureSurface(m_hTexture, &pSurface);
				//			pDev->ColorFill( pSurface, &rect, (DWORD) pixel.Value() );
				pDev->ColorFill(pSurface, &rect, dwPixel);

				pSurface->Release();
			}
			catch (...)
			{
				// BT - 7/15 - There was a memory access error when getting GetSurfaceLevel(0).
				// It appears to happen when the chat window is being drawn while in full screen mode.
				debugf("Error calling pVRAMMan->GetTextureSurface()\r\n");

				if (pSurface > 0)
					pSurface->Release();
			}

			//pVRAMMan->GetTextureSurface(m_hTexture, &pSurface);
			////			pDev->ColorFill( pSurface, &rect, (DWORD) pixel.Value() );
			//pDev->ColorFill(pSurface, &rect, dwPixel);


			// Must remember to release the surface.
			/*if (pSurface > 0)
			pSurface->Release();*/
		}
		else
		{
			if( pDev->IsInScene() == true )
			{
				if (dwPixel != 0)
				{
					pVertGen->GenerateFillVerticesD3DColor((WinRect&)rect, true, (DWORD)pixel.Value());

					pVRAMMan->SetTexture(INVALID_TEX_HANDLE, 0);
					pDev->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
					pDev->SetFVF(D3DFVF_UICOLOURVERT);
					pDev->DrawPrimitive(
						D3DPT_TRIANGLESTRIP,
						//									pVertGen->GetUIFillVertsVB()->dwFirstElementOffset,
						pVertGen->GetPredefinedDynamicBuffer(CVertexGenerator::ePDBT_UIFillVB)->dwFirstElementOffset,
						2);
				}
			}
			else
			{
                if (m_stype.Test(SurfaceTypeDummy()) == true) {
                    // we are attempting to draw to a dummy surface, this should never happen
                    debugf("Drawing to dummy surface: UnclippedFill");
                    ZAssert(false);
                    return;
                }

				// Perform software fill. Lock the texture.
				HRESULT hr;
				D3DLOCKED_RECT lockRect;

				hr = pVRAMMan->LockTexture( m_hTexture, &lockRect );
                ZAssert( hr == D3D_OK );
				ZAssert( ( m_ppf->PixelBytes() == 2) || ( m_ppf->PixelBytes() == 4) );

				if( m_ppf->PixelBytes() == 2 )
				{
					WORD wPixel;
					if( m_ppf->AlphaMask() == 0 )
					{
						wPixel = (WORD) ( dwPixel & 0x00F80000 ) >> 9;
						wPixel |= ( dwPixel & 0x0000FB00 ) >> 5;
						wPixel |= ( dwPixel & 0x000000F8 ) >> 3;
					}
					else
					{
						wPixel = (WORD) ( dwPixel & 0x00F80000 ) >> 9;
						wPixel |= ( dwPixel & 0x0000F800 ) >> 6;
						wPixel |= ( dwPixel & 0x000000F8 ) >> 3;
						wPixel |= ( dwPixel != 0 ) ? 0x8000 : 0;
					}

					CImageTransfer::ColourImageFill16Bit(	lockRect.pBits,
															lockRect.Pitch,
															rect.Min(),
															rect.Size(),
															wPixel );
				}
				else
				{
					if( dwPixel != 0 )
					{
						dwPixel |= 0xFF000000;
					}
					CImageTransfer::ColourImageFill32Bit(	lockRect.pBits,
															lockRect.Pitch,
															rect.Min(),
															rect.Size(),
															dwPixel );
				}

				// Finished, unlock.
				hr = pVRAMMan->UnlockTexture( m_hTexture, 0 );
                ZAssert( hr == D3D_OK );
			}
		}
    }

    void FillRect(const WinRect& rectArg, Pixel pixel)
    {
        //
        // Offset the rect to the origin of the surface
        //

        WinRect rect = rectArg;
        rect.Offset(m_pointOffset);

        //
        // Clip it to the surface
        //

        rect.Intersect(m_rectClip);

        //
        // Only blt if the target isn't empty
        //

        if (!rect.IsEmpty()) {
            UnclippedFill(rect, pixel);
        }
    }

    void FillRect(const WinRect& rect, const Color& color)
    {
        FillRect(rect, m_ppf->MakeD3DPixel(color));
    }

    void FillSurface(Pixel pixel)
    {
        FillRect(WinRect(-m_pointOffset, m_size - m_pointOffset), pixel);
    }

    void FillSurface(const Color& color)
    {
        FillSurface(m_ppf->MakePixel(color));
    }
};

//////////////////////////////////////////////////////////////////////////////
// Used to create a dummy top level surface.
//////////////////////////////////////////////////////////////////////////////
TRef<PrivateSurface> CreatePrivateDummySurface(	PrivateEngine * pengine,
												const WinPoint & size,
												SurfaceSite * psite )
{
	return new PrivateSurfaceImpl( pengine, size, psite, SurfaceTypeDummy() ); //Fix memory leak -Imago 8/2/09
}

TRef<PrivateSurface> CreatePrivateRenderTargetSurface(	PrivateEngine * pengine,
														const WinPoint & size, 
														SurfaceSite*    psite = NULL )
{
	return new PrivateSurfaceImpl(pengine, size, psite, SurfaceTypeRenderTarget() );
}

//////////////////////////////////////////////////////////////////////////////
//
// Used to create a surface with a certain pixel format
//
//////////////////////////////////////////////////////////////////////////////

TRef<PrivateSurface> CreatePrivateSurface(
    PrivateEngine*  pengine,
    PixelFormat*    ppf,
    const WinPoint& size,
    SurfaceType     stype,
    SurfaceSite*    psite
) {
    return new PrivateSurfaceImpl(pengine, ppf, size, stype, psite); //Fix memory leak -Imago 8/2/09
}

//////////////////////////////////////////////////////////////////////////////
//
// Used to create a surface from a memory mapped file
//
//////////////////////////////////////////////////////////////////////////////

TRef<PrivateSurface> CreatePrivateSurface(	PrivateEngine*  pengine,
											PixelFormat*    ppf,
											const WinPoint& size,
											int             pitch,
											BYTE*           pdata,
											IObject*        pobjectMemory,
											const bool		bColorKey,
											const Color &	cColorKey,
											const ZString & szTextureName /*=""*/,
											const bool		bSystemMemory /*=false*/) 
{
	return new PrivateSurfaceImpl( pengine, ppf, size, pitch, pdata, pobjectMemory, bColorKey, Color(0,0,0), szTextureName, bSystemMemory );
}

//////////////////////////////////////////////////////////////////////////////
//
// Used to create a surface from a memory mapped file, using D3DX to perform
// image stretching if necessary. Supports bmp, png, dds, tga etc.
//
//////////////////////////////////////////////////////////////////////////////
TRef<PrivateSurface> CreatePrivateSurface(	
					PrivateEngine*			pengine,
					const D3DXIMAGE_INFO *	pImageInfo,
					const WinPoint *		pTargetSize,
					IObject *				pobjectMemory,
					const bool				bColorKey,
					const Color &			cColorKey,
					const ZString &			szTextureName /*= ""*/,
					const bool				bSystemMemory /*= false*/ )
{
	//imago 6/24/09 - fixed typo?
	return new PrivateSurfaceImpl( pengine, pImageInfo, pTargetSize, pobjectMemory, bColorKey, Color(0,0,0), szTextureName, bSystemMemory );
	
}

//////////////////////////////////////////////////////////////////////////////
//
// Create a Private surface from an existing VideoSurface.  This is used to
//
// - wrap the primary surface
// - wrap offscreen video memory surfaces
//
//////////////////////////////////////////////////////////////////////////////

/*TRef<PrivateSurface> CreatePrivateSurface(
    PrivateEngine * pengine,
    DDSurface *		pvideoSurface,
    SurfaceSite *   psite ) 
{
    return new PrivateSurfaceImpl(pengine, pvideoSurface, psite);
}*/



////////////////////////////////////////////////////////////////////////////////////////////////////
// CreatePrivateSurface()
// Create an empty texture with the supplied format and dimensions.
////////////////////////////////////////////////////////////////////////////////////////////////////
TRef<PrivateSurface> CreatePrivateSurface(	D3DFORMAT	texFormat,
											DWORD		dwWidth,
											DWORD		dwHeight,
                                            const char *szTexName /*=NULL*/ )
{
    return new PrivateSurfaceImpl( texFormat, dwWidth, dwHeight, szTexName );
}

