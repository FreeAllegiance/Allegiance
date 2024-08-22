#pragma once

#include <ddraw.h>   
#include <mmstream.h> 
#include <amstream.h>
#include <ddstream.h> 
#include <DShow.h>


// +-----------------------------------+
// | THE DirectShow Class. 			   | 
// +-----------------------------------+
class CDShow
{
	// List of private members. Can only be accessed by internal object functions.
	private:

		// Variable used to check if media is open.
		// All functions that operate on an open video stream,
		// should abort if this is false.
		BOOL m_bMediaOpen; 

		// Variable used to check if media is playing.
		BOOL m_bPlaying;

		// Source rectangle for blit operation.
		// Contains size of video clip.
		RECT m_rSrcRect;
		
		// Multimedia stream interface
		IMultiMediaStream * m_pMMStream;

		// MediaStream interface
		IMediaStream *m_pPrimaryVideoStream;

		// DirectDraw media stream interface
		IDirectDrawMediaStream *m_pDDStream;
		
		// DirectDraw sample stream interface
		IDirectDrawStreamSample *m_pSample;
		
		// Old DirectDraw surface
		IDirectDrawSurface *m_pDDSurface;
		
		// DirectDrawSurface7. Used to 
		IDirectDrawSurface7 *m_pDDSurface7;	

	// List of public members. Can be accessed by anyone.
	public:
		
		// Constructor :
		// Takes care of resetting all members.
		CDShow();

		// Deconstructor :
		// Simply wraps CleanUp for proper exit.
		~CDShow();

		// Open multimedia stream.
		// Pass a filename and a DirectDraw pointer.
		// Tested with IV50 & WMV3.
		BOOL Open(ZString& pFileName, IDirectDraw7 *pDD);

		// Close multimedia stream
		BOOL Close();
		
		// CleanUp function. 
		// Called automatically upon object destruction.
		void CleanUp();

		// Draw video clip to DirectDraw surface.
		BOOL Draw(LPDIRECTDRAWSURFACE7 lpDDSurface7);

		// Start video playback
		BOOL Start();

		// Stop video playback
		BOOL Stop();

		// Check if media is currently playing
		BOOL IsPlaying() {return m_bPlaying;};

						
};