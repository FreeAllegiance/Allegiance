#ifndef _D3DDEVICE9_H_
#define _D3DDEVICE9_H_

#include <point.h>

#include <d3d9.h>

#define ALLEG_D9EX

#ifdef ALLEG_D9EX
#define ALLEG_D9EX_IF(a, b) a
#else
#define ALLEG_D9EX_IF(a, b) b
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////
class CD3DDevice9
{
	friend class CVRAMManager;
	friend class CVBIBManager;

private:
	static CD3DDevice9 mSingleInstance;						// CD3DDevice9 singleton.

////////////////////////////////////////////////////////////////////////////////////////////////////
public:
	////////////////////////////////////////////////////////////////////////////////////////////
#ifdef EnablePerformanceCounters
	enum EDevice9Stat
	{
		eD9S_NumDrawPrims,
		eD9S_NumPrimsRendered,
		eD9S_InitialTexMem,
		eD9S_CurrentTexMem,
		eD9S_NumStateChanges,
		eD9S_NumTextureChanges,
		eD9S_NumShaderChanges,

		eD9S_NumStatTypes
	};
	int m_pPerformanceCounters[ eD9S_NumStatTypes ];
#endif // EnablePerformanceCounters

	////////////////////////////////////////////////////////////////////////////////////////////
	enum EMaxTextureSize
	{
		eMTS_256 = 0,
		eMTS_512,
		eMTS_1024,
		eMTS_2048,

		eMTS_NumTextureSizes
	};

	////////////////////////////////////////////////////////////////////////////////////////////
	struct SD3DDeviceMode
	{
        ALLEG_D9EX_IF(
            D3DDISPLAYMODEEX			mode;
            ,
            D3DDISPLAYMODE			mode;
        )
		D3DFORMAT				fmtDepthStencil;
		D3DMULTISAMPLE_TYPE		d3dMultiSampleSetting;
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////
	struct SD3DVideoResolution
	{
		int						iWidth;
		int						iHeight;
		int						iFreq;
	};

	////////////////////////////////////////////////////////////////////////////////////////////
	struct SD3DDeviceSetupParams
	{
		int						iAdapterID;
		HMONITOR				hMonitor;
		MONITORINFOEX			monitorInfo;
		bool					bRunWindowed;
		bool					bWindowModeValid;
		bool					bAntiAliased;
		bool					bWaitForVSync;
		int						iWindowOffsetX;
		int						iWindowOffsetY;

		SD3DDeviceMode			sWindowedMode;
		SD3DDeviceMode			sFullScreenMode;

		D3DTEXTUREFILTERTYPE	d3dMinFilter;
		D3DTEXTUREFILTERTYPE	d3dMagFilter;
		D3DTEXTUREFILTERTYPE	d3dMipFilter;

		char					szDevType[ 64 ];
		char					szAAType[ 64 ];

		int						iNumRes;
		int						iCurrentRes;
		SD3DVideoResolution *	pFullScreenResArray;

		//
		EMaxTextureSize			maxTextureSize;
		bool					bAutoGenMipmap;
		DWORD					dwMaxTextureSize;
	};

	SD3DDeviceSetupParams m_sDevSetupParams;

	// Note: D3DRS_BLENDOPALPHA is highest value in the D3DRENDERSTATETYPE enumerated type.
	// D3DTSS_CONSTANT is the highest value in the D3DTEXTURESTAGESTATETYPE enumerated type.
	// Max stages (8) physical limit taken from the SDK.
	static const DWORD dwMaxRenderStatesAllowed = D3DRS_BLENDOPALPHA + 1;
	static const DWORD dwMaxTextureStages = 8;
	static const DWORD dwMaxTextureStageStatesAllowed = D3DTSS_CONSTANT + 1;
	static const DWORD dwMaxSamplerStatesAllowed = D3DSAMP_DMAPOFFSET + 1;

////////////////////////////////////////////////////////////////////////////////////////////////////
private:
	////////////////////////////////////////////////////////////////////////////////////////////
	struct SD3DDevice9FormatFlags
	{
		bool		bSupportsA1R5G6B6Format : 1;
		bool		bCanAutoGenMipMaps : 1; 
	};

	////////////////////////////////////////////////////////////////////////////////////////////
	struct SRenderStateCache
	{
		DWORD dwUnused000, dwUnused001, dwUnused002, dwUnused003, dwUnused004, dwUnused005, dwUnused006;
		D3DZBUFFERTYPE ZEnable;
		D3DFILLMODE FillMode;
		D3DSHADEMODE ShadeMode;
		DWORD dwUnused010, dwUnused011, dwUnused012, dwUnused013;
		DWORD ZWriteEnable;
		DWORD AlphaTestEnable;
		DWORD LastPixel;
		DWORD dwUnused017, dwUnused018;
		D3DBLEND SrcBlend, DstBlend;
		DWORD dwUnused021;
		D3DCULL CullMode;
		D3DCMPFUNC ZFunc;
		DWORD AlphaRef;
		D3DCMPFUNC AlphaFunc;
		DWORD DitherEnable;
		DWORD AlphaBlendEnable;
		DWORD FogEnable;
		DWORD SpecularEnable;
		DWORD dwUnunsed030, dwUnunsed031, dwUnunsed032, dwUnunsed033;
		D3DCOLOR FogColor;
		D3DFOGMODE FogTableMode;
		float FogStart;
		float FogEnd;
		float FogDensity;
		DWORD dwUnunsed039, dwUnunsed040, dwUnunsed041, dwUnunsed042;
		DWORD dwUnunsed043, dwUnunsed044, dwUnunsed045, dwUnunsed046, dwUnunsed047;
		DWORD RangeFogEnable;
		DWORD dwUnunsed049, dwUnunsed050, dwUnunsed051;
		DWORD StencilEnable;
		D3DSTENCILOP StencilFail, StencilZFail, StencilPass;
		D3DCMPFUNC StencilFunc;
		int StencilRef;
		DWORD StencilMask;
		DWORD StencilWriteMask;
		D3DCOLOR TextureFactor;
		DWORD dwUnunsed061, dwUnunsed062, dwUnunsed063, dwUnunsed064, dwUnunsed065;
		DWORD dwUnunsed066, dwUnunsed067, dwUnunsed068, dwUnunsed069, dwUnunsed070;
		DWORD dwUnunsed071, dwUnunsed072, dwUnunsed073, dwUnunsed074, dwUnunsed075;
		DWORD dwUnunsed076, dwUnunsed077, dwUnunsed078, dwUnunsed079, dwUnunsed080;
		DWORD dwUnunsed081, dwUnunsed082, dwUnunsed083, dwUnunsed084, dwUnunsed085;
		DWORD dwUnunsed086, dwUnunsed087, dwUnunsed088, dwUnunsed089, dwUnunsed090;
		DWORD dwUnunsed091, dwUnunsed092, dwUnunsed093, dwUnunsed094, dwUnunsed095;
		DWORD dwUnunsed096, dwUnunsed097, dwUnunsed098, dwUnunsed099, dwUnunsed000;
		DWORD dwUnunsed101, dwUnunsed102, dwUnunsed103, dwUnunsed104, dwUnunsed105;
		DWORD dwUnunsed106, dwUnunsed107, dwUnunsed108, dwUnunsed109, dwUnunsed110;
		DWORD dwUnunsed111, dwUnunsed112, dwUnunsed113, dwUnunsed114, dwUnunsed115;
		DWORD dwUnunsed116, dwUnunsed117, dwUnunsed118, dwUnunsed119, dwUnunsed120;
		DWORD dwUnunsed121, dwUnunsed122, dwUnunsed123, dwUnunsed124, dwUnunsed125;
		DWORD dwUnunsed126, dwUnunsed127;
		DWORD Wrap00, Wrap01, Wrap02, Wrap03, Wrap04, Wrap05, Wrap06, Wrap07;
		DWORD Clipping;
		DWORD Lighting;
		DWORD dwUnused138;
		D3DCOLOR Ambient;
		D3DFOGMODE FogVertexMode;
		DWORD ColorVertex;
		DWORD LocalViewer;
		DWORD NormalizeNormals;
		DWORD dwUnused144;
		D3DMATERIALCOLORSOURCE DiffuseMaterialSource;
		D3DMATERIALCOLORSOURCE SpecularMaterialSource;
		D3DMATERIALCOLORSOURCE AmbientMaterialSource;
		D3DMATERIALCOLORSOURCE EmissiveMaterialSource;
		DWORD dwUnused149, dwUnused150;
		D3DVERTEXBLENDFLAGS VertexBlend;
		DWORD ClipPlaneEnable;
		DWORD dwUnused153;
		float PointSize, PointSize_Min;
		DWORD PointSpriteEnable;
		DWORD PointScaleEnable;
		float PointScale_A, PointScale_B, PointScale_C;
		DWORD MultiSampleAntiAlias, MultiSampleMask;
		D3DPATCHEDGESTYLE PatchEdgeStyle;
		DWORD dwUnused164;
		D3DDEBUGMONITORTOKENS DebugMonitorTokens;
		float PointSize_Max;
		DWORD IndexedVertexBlendEnable;
		DWORD ColorWriteEnable;
		DWORD dwUnused169;
		float TweenFactor;
		D3DBLENDOP BlendOp;
		D3DDEGREETYPE PositionDegree, NormalDegree;
		DWORD ScissorTestEnable;
		float SlopeScaleDepthBias;
		DWORD AntiAliasedLineEnable;
		DWORD dwUnused177;
		float MinTesselationLevel, MaxTesselationLevel;
		float AdaptiveTess_X, AdaptiveTess_Y, AdaptiveTess_Z, AdaptiveTess_W;
		DWORD EnableAdpativeTesselation, TwoSidedStencilMode;
		D3DSTENCILOP CCW_StencilFail, CCW_StencilZFail, CCW_StencilPass;
		D3DCMPFUNC CCW_StencilFunc;
		DWORD ColorWriteEnable1, ColorWriteEnable2, ColorWriteEnable3;
		D3DCOLOR BlendFactor;
		DWORD SRGBWriteEnable;
		float DepthBias;
		DWORD dwUnused196, dwUnused197;
		DWORD Wrap08, Wrap09, Wrap10, Wrap11, Wrap12, Wrap13, Wrap14, Wrap15;
		DWORD SeparateAlphaBlendEnable;
		D3DBLEND SrcBlendAlpha, DstBlendAlpha;
		D3DBLENDOP BlendOpAlpha;
	};

	////////////////////////////////////////////////////////////////////////////////////////////
	struct SD3D9DeviceStateCache
	{
		union 
		{
			SRenderStateCache	sRenderStateCache;
			DWORD				pRenderState[ dwMaxRenderStatesAllowed ];
		};

		DWORD					pTextureStageStates[ dwMaxTextureStages ][ dwMaxTextureStageStatesAllowed ];
		DWORD					dwNumSamplers;
		DWORD **				pSamplerState;
		DWORD					dwFVF;
	};

	////////////////////////////////////////////////////////////////////////////////////////////
	// For now, ignore the 256 world matrices and 8 texture matrices.
	struct SD3D9TransformCache
	{
		D3DMATRIX matProjection;
		D3DMATRIX matView;
		D3DMATRIX matWorld;
	};

	////////////////////////////////////////////////////////////////////////////////////////////
	// Material cache.
	struct SD3D9MaterialCache
	{
		D3DMATERIAL9 currentMaterial;
	};

	////////////////////////////////////////////////////////////////////////////////////////////
	// Lights cache, array size taken from the D3DCAPS9 struct.
	struct SD3D9LightCache
	{
		DWORD dwNumLights;
		D3DLIGHT9 * pLights;
		BOOL * pLightsActive;
	};

	////////////////////////////////////////////////////////////////////////////////////////////
	struct SD3DDevice9State
	{
		// NOTE: this MUST be the FIRST item in the structure.
		struct
		{
			bool					bInitialised : 1;
			bool					bHardwareVP : 1;
			bool					bPureDevice : 1;
			bool					bInScene : 1;		// True if inside a BeginScene/EndScene pair.
			bool					bIsWindowed : 1;
		};

		D3DPRESENT_PARAMETERS	d3dPresParams;
#ifdef ALLEG_D9EX
		LPDIRECT3D9EX			pD3D9;
        LPDIRECT3DDEVICE9EX		pD3DDevice;
#else
        LPDIRECT3D9				pD3D9;
        LPDIRECT3DDEVICE9		pD3DDevice;
#endif
		D3DADAPTER_IDENTIFIER9	d3dAdapterID;
		D3DCAPS9				sD3DDevCaps;
		SD3DDeviceMode *		pCurrentMode;
		char					pszDevSetupString[256];
		DWORD					dwCurrentWindowedWidth;
		DWORD					dwCurrentWindowedHeight;
		DWORD					dwCurrentFullscreenWidth;
		DWORD					dwCurrentFullscreenHeight;
		HWND					hParentWindow; // BT - 10/17 - If the D3D device becomes null, re-create it to get it up and rolling again.

		SD3DDevice9FormatFlags	sFormatFlags;

		// If antialiasing is being used, create a separate non-AA depthstencil
		// buffer for use with additional render targets.
		LPDIRECT3DSURFACE9		pRTDepthStencilSurface;
		LPDIRECT3DSURFACE9		pBackBufferDepthStencilSurface;

		// Device state cache.
		SD3D9DeviceStateCache	sDeviceStateCache;
		SD3D9TransformCache		sTransformCache;
		SD3D9MaterialCache		sMaterialCache;
		SD3D9LightCache			sLightCache;
	};

	SD3DDevice9State m_sD3DDev9;

    HRESULT CreateDevice(HWND hParentWindow, D3DDEVTYPE deviceType, DWORD flags);
	
////////////////////////////////////////////////////////////////////////////////////////////////////
public:
	CD3DDevice9();
	virtual ~CD3DDevice9();

	static inline CD3DDevice9 * Get()
	{
		return &mSingleInstance;
	}

	void		Initialise( class CLogFile * pLogFile );
	void		Shutdown( );
	void		ResetReferencedResources( );
	HRESULT	CreateD3D9( class CLogFile * pLogFile );
	HRESULT	CreateDevice( HWND hParentWindow, class CLogFile * pLogFile );

	// BT - 10/17 - Building a last chance device creation. We're gonna create SOMETHING dammit.
	D3DFORMAT GetValidBackBufferFormat(class CLogFile * pLogFile);
	D3DFORMAT GetValidDepthStencilFormat(D3DFORMAT backbufferFormat, class CLogFile * pLogFile);
	HRESULT LastChanceCreateDevice(HWND hParentWindow, class CLogFile * pLogFile);
	
	
	HRESULT	ResetDevice( bool bWindowed, DWORD dwWidth = 0, DWORD dwHeight = 0, int iRate = 60 ); //imago added refresh rate 7/1/09
	HRESULT	ClearScreen( );
	HRESULT	RenderFinished( );
	WinPoint	GetCurrentResolution( );
	bool		IsDeviceValid( );
	bool		IsInScene( );
	bool		IsWindowed( );

	// Settings.
	void UpdateCurrentMode( );
	inline SD3DDeviceSetupParams * GetDeviceSetupParams()	{ return &m_sDevSetupParams;	}
	inline SD3DDeviceMode * GetCurrentMode( )				{ return m_sD3DDev9.pCurrentMode;	}
	inline bool IsHardwareVP( )								{ return m_sD3DDev9.bHardwareVP;	}
	inline D3DTEXTUREFILTERTYPE GetMinFilter( )				{ return m_sDevSetupParams.d3dMinFilter; }
	inline D3DTEXTUREFILTERTYPE GetMagFilter( )				{ return m_sDevSetupParams.d3dMagFilter; }
	inline D3DTEXTUREFILTERTYPE GetMipFilter( )				{ return m_sDevSetupParams.d3dMipFilter; }
	
	// Antialias related access functions.
	inline bool IsAntiAliased( )							{ return m_sDevSetupParams.bAntiAliased;	}
	inline LPDIRECT3DSURFACE9 GetBackBufferDepthStencil( )	{ return m_sD3DDev9.pBackBufferDepthStencilSurface; }
	inline LPDIRECT3DSURFACE9 GetRTDepthStencil( )			{ return m_sD3DDev9.pRTDepthStencilSurface; }
	HRESULT SetRTDepthStencil( );
	HRESULT SetBackBufferDepthStencil( );

	inline const ALLEG_D9EX_IF(LPDIRECT3D9EX, LPDIRECT3D9) GetD3D9() { return m_sD3DDev9.pD3D9; }
	D3DCAPS9 * GetDevCaps( ) { return &m_sD3DDev9.sD3DDevCaps; }
	inline const SD3DDevice9FormatFlags * GetDevFlags() { return &m_sD3DDev9.sFormatFlags; }

	inline DWORD GetMaxTextureSize() { return m_sDevSetupParams.dwMaxTextureSize; }

	/////////////////////////////////////////////////////////////////////////////////////
	// Device-level functions accessing cached values. Prototypes taken from and 
	// and matching exactly those in the SDK. Behaviour also identical.

	// TODO:
	// Set/GetFVF, Set/GetTextureStageState, Set/GetTexture, Set/GetViewport, 
	// Set/GetIndices, Set/GetStreamSource, DIP, DIPUP
	//
	// At some point:
	// Set/GetVertexShader, Set/GetPixelShader, DP, DPUP

	HRESULT	SetRenderState( D3DRENDERSTATETYPE State, DWORD Value );
	HRESULT	GetRenderState( D3DRENDERSTATETYPE State, DWORD * pValue );
	HRESULT	SetTextureStageState( DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD Value );
	HRESULT	GetTextureStageState( DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD * pValue );
	HRESULT	SetSamplerState( DWORD Sampler, D3DSAMPLERSTATETYPE Type, DWORD Value );
	HRESULT	GetSamplerState( DWORD Sampler, D3DSAMPLERSTATETYPE Type, DWORD * pValue );
	HRESULT	SetTransform( D3DTRANSFORMSTATETYPE State, CONST D3DMATRIX * pMatrix );
	HRESULT	GetTransform( D3DTRANSFORMSTATETYPE State, D3DMATRIX * pMatrix );
	HRESULT	SetMaterial( CONST D3DMATERIAL9 * pMaterial );
	HRESULT	GetMaterial( D3DMATERIAL9 * pMaterial );
	HRESULT	SetLight( DWORD Index, CONST D3DLIGHT9 * pLight );
	HRESULT	GetLight( DWORD Index, D3DLIGHT9 * pLight );
	HRESULT	LightEnable( DWORD Index, BOOL bEnable );
	HRESULT	GetLightEnable( DWORD Index, BOOL * pEnable );
	HRESULT	SetFVF( DWORD FVF );
	HRESULT	GetFVF( DWORD * pFVF );
	HRESULT	SetTexture( DWORD Sampler, LPDIRECT3DBASETEXTURE9 pTexture );
	HRESULT	GetTexture( DWORD Sampler, LPDIRECT3DBASETEXTURE9 * ppTexture );
	HRESULT	SetViewport( CONST D3DVIEWPORT9 * pViewport );
	HRESULT	GetViewport( D3DVIEWPORT9 * pViewport );
	HRESULT	SetIndices( LPDIRECT3DINDEXBUFFER9  pIndexData );
	HRESULT	GetIndices( LPDIRECT3DINDEXBUFFER9 * ppIndexData );
	HRESULT	SetStreamSource( UINT StreamNumber, LPDIRECT3DVERTEXBUFFER9 pStreamData, UINT OffsetInBytes, UINT Stride );
	HRESULT	GetStreamSource( UINT StreamNumber, LPDIRECT3DVERTEXBUFFER9 * ppStreamData, UINT * pOffsetInBytes, UINT * pStride );
	
	// Draw calls.
	HRESULT DrawPrimitive(	D3DPRIMITIVETYPE Type,
									UINT StartVertex,
									UINT PrimitiveCount );

	HRESULT DrawIndexedPrimitive(	D3DPRIMITIVETYPE Type, 
											INT BaseVertexIndex, 
											UINT MinIndex, 
											UINT NumVertices, 
											UINT StartIndex, 
											UINT PrimitiveCount );

	HRESULT DrawIndexedPrimitiveUP(	D3DPRIMITIVETYPE PrimitiveType, 
											UINT MinVertexIndex, 
											UINT NumVertices, 
											UINT PrimitiveCount, 
											CONST void * pIndexData, 
											D3DFORMAT IndexDataFormat, 
											CONST void * pVertexStreamZeroData,
											UINT VertexStreamZeroStride );

	HRESULT DrawPrimitiveUP(	D3DPRIMITIVETYPE PrimitiveType, 
									UINT PrimitiveCount,
									CONST void * pVertexStreamZeroData,
									UINT VertexStreamZeroStride );

	HRESULT ColorFill( LPDIRECT3DSURFACE9 pSurface, CONST RECT * pRect, D3DCOLOR color );
	HRESULT Clear( DWORD Count, CONST D3DRECT * pRects, DWORD Flags, D3DCOLOR Color, float Z, DWORD Stencil );

	// Miscellaneous device calls.
	HRESULT BeginScene();
	HRESULT EndScene();
	HRESULT TestCooperativeLevel( );
	void SetGammaRamp( UINT iSwapChain, DWORD Flags, CONST D3DGAMMARAMP * pRamp );

#ifdef EnablePerformanceCounters
	void ResetPerformanceCounters();
	int GetPerformanceCounter(EDevice9Stat stat );
	const char * GetDeviceSetupString()	{ return m_sD3DDev9.pszDevSetupString; }
#endif // EnablePerformanceCounters

////////////////////////////////////////////////////////////////////////////////////////////////////
private:
	// Access functions.
	const ALLEG_D9EX_IF(LPDIRECT3DDEVICE9EX, LPDIRECT3DDEVICE9) Device(); // BT - 10/17 - If the D3D device becomes null, re-create it to get it up and rolling again.

	// State initialisation functions.
	void		InitialiseDeviceStateCache( SD3D9DeviceStateCache * pCache );
	void		InitialiseRenderStateCache( DWORD * pRenderStateCache );
	void		InitialiseTextureStageCache( DWORD dwStageIndex, DWORD * pTextureStateCache );
	void		InitialiseSamplerStateCache( DWORD dwSamplerIndex, DWORD * pSamplerStateCache );
	void		InitialiseTransformCache( SD3D9TransformCache * pCache );
	void		InitialiseMaterialCache( SD3D9MaterialCache * pCache );
	void		InitialiseLightCache( SD3D9LightCache * pCache );
	void		CreateAADepthStencilBuffer( );
};


#endif // _D3DDEVICE9_H_
