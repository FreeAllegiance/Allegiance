#pragma once

#include "d3d9.h"
#include "D3DDevice9.h"

//////////////////////////////////////////////////////////////////////////////////////////////
class CD3DDeviceModeData
{
//////////////////////////////////////////////////////////////////////////////////////////////
private:
	//////////////////////////////////////////////////////////////////////////////////////
	enum EDMDCounts
	{
		eDMD_NumModes = 2,
		eDMD_NumDepthStencilFormats = 2,
		eDMD_NumAAFormats = 5,				// None, 2, 4, 6, 8.
		eDMD_NumMinFilters = 5,				// Point, bilinear, anisotropic, pyramidal, gaussian.
		eDMD_NumMagFilters = 5,				// Point, bilinear, anisotropic, pyramidal, gaussian.
		eDMD_NumMipFilters = 2,				// Point, bilinear.
	};

public:
	////////////////////////////////////////////////////////////////////////////////////////////////////
	struct SVideoResolution
	{
		int						iWidth;
		int						iHeight;
		int						iFreq;
	};

private:
	//////////////////////////////////////////////////////////////////////////////////////
	struct SAdapterMode
	{
		D3DDISPLAYMODE		mode;
		bool				bHWSupport;
		bool				bWindowAllowed;
		D3DFORMAT			d3dDepthStencil;
		bool				pFullScreenAA[eDMD_NumAAFormats];
		bool				pWindowedAA[eDMD_NumAAFormats];
	};

	//////////////////////////////////////////////////////////////////////////////////////
	struct SAdapter
	{
		D3DDISPLAYMODE			currentDisplayMode;
		D3DADAPTER_IDENTIFIER9	adapterID;
		HMONITOR				hMonitor;
		int	*					pModeCount;
		SAdapterMode **			ppAvailableModes;
		int						iTotalModeCount;
		D3DCAPS9				devCaps;
	};

	//////////////////////////////////////////////////////////////////////////////////////
	bool					m_bDataValid;
	int						m_iAdapterCount;
	SAdapter *				m_pAdapterArray;
	LPDIRECT3D9				m_pD3D9;
	int						m_iMinWidth;
	int						m_iMinHeight;

	//////////////////////////////////////////////////////////////////////////////////////
	class CLogFile *		m_pLogFile;		// Output stats to log file.

//////////////////////////////////////////////////////////////////////////////////////////////
public:
	//////////////////////////////////////////////////////////////////////////////////////
	// Constructor.
	// Generates information about all adapters in the current system.
	CD3DDeviceModeData( int iMinWidth, int iMinHeight, class CLogFile * pLogFile );
	virtual ~CD3DDeviceModeData();

	//////////////////////////////////////////////////////////////////////////////////////
	bool		IsValid();
	int			ValidateSettings( int iDeviceIndex, int iResolutionIndex, bool bWindowed );
	int			GetDeviceCount();
	int			GetTotalResolutionCount( int iDeviceIndex );
	int			GetResolutionCount( int iDeviceIndex, D3DFORMAT fmt );
	void		GetDeviceNameByIndex( int iDeviceIndex, char * pBuffer, int szBufferLen );
	void		GetResolutionStringByIndex( int iDeviceIndex, int iResolutionIndex, char * pBuffer, int iBufferLen );
	void		GetResolutionDetails(	int			iDeviceIndex, 
										int			iModeIndex, 
										int *		pWidth, 
										int *		pHeight, 
										int *		pMonitorFreq, 
										D3DFORMAT * pBackBufferFormat,
										D3DFORMAT * pDeviceFormat,
										HMONITOR *	pHMonitor );

	void		GetRelatedResolutions(	int iDeviceIndex,
										int iModeIndex,
										int * pNumResolutions,
										int * pSelectedResolution,
										SVideoResolution ** ppResolutionArray );

	int			GetNumAASettings( int iDeviceIndex, int iResolutionIndex, bool bWindowed );
	void		GetAASettingString( int iDeviceIndex, 
									int iResolutionIndex, 
									bool bWindowed, 
									int iAAIndex, 
									char * pBuffer, 
									int iBufferLen );
	bool		GetModeParams(	CD3DDevice9::SD3DDeviceSetupParams * pParams,
								int iDeviceIndex,
								int iResolutionIndex,
								int iAAIndex,
								class CLogFile * pLogFile );

    const char *		GetMaxTextureString( int iIndex );



private:
	bool		ExtractAdapterData( int iDeviceIndex );
	int			GetFormatBitCount( D3DFORMAT fmt );
	SAdapterMode * FindMatchingWindowedMode(	CD3DDevice9::SD3DDeviceSetupParams * pParams,
												SAdapterMode * pFSMode,
												int iDeviceIndex );
    SAdapterMode * FindMatchingWindowedMode(CD3DDevice9::SD3DDeviceSetupParams * pParams,
        WinPoint size,
        int iDeviceIndex);
};