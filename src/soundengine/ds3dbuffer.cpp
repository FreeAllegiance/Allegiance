//
// ds3dbuffer.cpp: low-level classes representing actual DirectSound3D buffers
//

#include "soundbase.h"

#include <algorithm>

#include "ds3dutil.h"
#include "ds3dbuffer.h"

namespace SoundEngine {


/////////////////////////////////////////////////////////////////////////////
//
// DS3DSoundBuffer
//
/////////////////////////////////////////////////////////////////////////////


// convert a gain in the range -100 to 0 dB to a direct sound volume
inline LONG DS3DSoundBuffer::DSoundVolume(float fGain)
{
    ZAssert((fGain >= -100) && (fGain <= 0));
    return (LONG)(100 * fGain);
}

// initializes the object, creating the DSoundBuffer itself and 
// initializing local variables.
HRESULT DS3DSoundBuffer::CreateBuffer8(IDirectSound8* pDirectSound, ISoundPCMData* pdata,
    DWORD dwBufferSize, bool bStatic, bool bSupport3D, ISoundEngine::Quality quality,
    bool bAllowHardware)
{
    HRESULT hr;
            
    // check the arguments
    if (!pdata || !pDirectSound)
    {
        ZAssert(false);
        return E_POINTER;
    }

    // set a few state variables with the new info we have
    m_b3D = bSupport3D;
    m_dwSampleRate = pdata->GetSampleRate();


    // describe the new buffer we want

    WAVEFORMATEX waveformatex;
    DSBUFFERDESC dsbufferdesc;

//    waveformatex.cbSize = sizeof(WAVEFORMATEX);
	waveformatex.cbSize = 0;	// mdvalley: Size refers to extra data, not to waveformatex size. PCM has 0.
    waveformatex.wFormatTag = WAVE_FORMAT_PCM; 
    waveformatex.nChannels = pdata->GetNumberOfChannels(); 
    waveformatex.nSamplesPerSec = m_dwSampleRate; 
    waveformatex.wBitsPerSample = pdata->GetBitsPerSample(); 
    waveformatex.nBlockAlign = waveformatex.wBitsPerSample / 8 * waveformatex.nChannels;
    waveformatex.nAvgBytesPerSec = waveformatex.nSamplesPerSec * waveformatex.nBlockAlign;

    dsbufferdesc.dwSize = sizeof(DSBUFFERDESC);
    dsbufferdesc.dwFlags = 
        (bSupport3D ? DSBCAPS_CTRL3D | DSBCAPS_MUTE3DATMAXDISTANCE : 0)
        | DSBCAPS_CTRLFREQUENCY | DSBCAPS_CTRLVOLUME 
        | DSBCAPS_GETCURRENTPOSITION2  
//        | (bStatic ? DSBCAPS_STATIC : 0)		// mdvalley: Modern sound hardware doesn't like this flag.
		;
    dsbufferdesc.dwBufferBytes = dwBufferSize;
    dsbufferdesc.dwReserved = 0;
	dsbufferdesc.lpwfxFormat = &waveformatex;

#if DIRECTSOUND_VERSION >= 0x0700
    if (bAllowHardware)
        dsbufferdesc.dwFlags |= DSBCAPS_LOCDEFER;
    else
        dsbufferdesc.dwFlags |= DSBCAPS_LOCSOFTWARE;

    if (bSupport3D)
    {
        switch (quality)
        {
        case ISoundEngine::minQuality:
#ifndef __GNUC__
            dsbufferdesc.guid3DAlgorithm = DS3DALG_NO_VIRTUALIZATION;
#endif
            break;

        case ISoundEngine::midQuality:
            dsbufferdesc.guid3DAlgorithm = DS3DALG_DEFAULT;
            break;

        case ISoundEngine::maxQuality:
#ifndef __GNUC__
            dsbufferdesc.guid3DAlgorithm = DS3DALG_HRTF_LIGHT;
#endif
            break;
        };
    }
    else
    {
        dsbufferdesc.guid3DAlgorithm = GUID_NULL;
    }
#endif

	// mdvalley: To use the dx8 buffers, you must create a dx4 buffer,
    // then query it up to DX8. Or something like that.

    LPDIRECTSOUNDBUFFER mdDsb = nullptr;
    // create the new buffer

    hr = pDirectSound->CreateSoundBuffer(&dsbufferdesc, &mdDsb, nullptr);
	if (ZFailed(hr)) return hr;

	// and up to DX8.

	hr = mdDsb->QueryInterface(IID_IDirectSoundBuffer8, (LPVOID*) &m_pdirectsoundbuffer8);
	mdDsb->Release();
	if (ZFailed(hr)) return hr;

//	hr = pDirectSound->CreateSoundBuffer(&dsbufferdesc, &m_pdirectsoundbuffer, nullptr);
//	if (FAILED(hr)) return hr;

	// get a handle to the 3D buffer, if this is 3D
    if (bSupport3D)
    {
        hr = m_pdirectsoundbuffer8->QueryInterface(IID_IDirectSound3DBuffer, (void**)&m_pdirectsound3Dbuffer);
        if (ZFailed(hr)) return hr;
    }

	BYTE nFillValue = (waveformatex.wBitsPerSample == 8) ? 0x80 : 0;
	LPVOID writePtr;
	DWORD writeBytes;

	// Fill the buffer with silence
    m_pdirectsoundbuffer8->Lock(0, 0, &writePtr, &writeBytes, nullptr, nullptr, DSBLOCK_ENTIREBUFFER);
	memset(writePtr, nFillValue, writeBytes);		// Seeing as we just created the buffer, the writePtr is at the beginning.
    m_pdirectsoundbuffer8->Unlock(writePtr, writeBytes, nullptr, 0);

    return S_OK;
};

// initializes the object, creating the DSoundBuffer itself and 
// initializing local variables.
HRESULT DS3DSoundBuffer::CreateBuffer(IDirectSound* pDirectSound, ISoundPCMData* pdata,
    DWORD dwBufferSize, bool bStatic, bool bSupport3D, ISoundEngine::Quality quality,
    bool bAllowHardware)
{
    HRESULT hr;
            
    // check the arguments
    if (!pdata || !pDirectSound)
    {
        ZAssert(false);
        return E_POINTER;
    }

    // set a few state variables with the new info we have
    m_b3D = bSupport3D;
    m_dwSampleRate = pdata->GetSampleRate();


    // describe the new buffer we want

    WAVEFORMATEX waveformatex;
    DSBUFFERDESC dsbufferdesc;

//    waveformatex.cbSize = sizeof(WAVEFORMATEX);
	waveformatex.cbSize = 0;	// mdvalley: Size refers to extra data, not to waveformatex size. PCM has 0.
    waveformatex.wFormatTag = WAVE_FORMAT_PCM; 
    waveformatex.nChannels = pdata->GetNumberOfChannels(); 
    waveformatex.nSamplesPerSec = m_dwSampleRate; 
    waveformatex.wBitsPerSample = pdata->GetBitsPerSample(); 
    waveformatex.nBlockAlign = waveformatex.wBitsPerSample / 8 * waveformatex.nChannels;
    waveformatex.nAvgBytesPerSec = waveformatex.nSamplesPerSec * waveformatex.nBlockAlign;

    dsbufferdesc.dwSize = sizeof(DSBUFFERDESC);
    dsbufferdesc.dwFlags = 
        (bSupport3D ? DSBCAPS_CTRL3D | DSBCAPS_MUTE3DATMAXDISTANCE : 0)
        | DSBCAPS_CTRLFREQUENCY | DSBCAPS_CTRLVOLUME 
        | DSBCAPS_GETCURRENTPOSITION2  
        | (bStatic ? DSBCAPS_STATIC : 0)
		;
    dsbufferdesc.dwBufferBytes = dwBufferSize;
    dsbufferdesc.dwReserved = 0;
	dsbufferdesc.lpwfxFormat = &waveformatex;

#if DIRECTSOUND_VERSION >= 0x0700
    if (bAllowHardware)
        dsbufferdesc.dwFlags |= DSBCAPS_LOCDEFER;
    else
        dsbufferdesc.dwFlags |= DSBCAPS_LOCSOFTWARE;

    if (bSupport3D)
    {
        switch (quality)
        {
        case ISoundEngine::minQuality:
#ifndef __GNUC__
            dsbufferdesc.guid3DAlgorithm = DS3DALG_NO_VIRTUALIZATION;
#endif
            break;

        case ISoundEngine::midQuality:
            dsbufferdesc.guid3DAlgorithm = DS3DALG_DEFAULT;
            break;

        case ISoundEngine::maxQuality:
#ifndef __GNUC__
            dsbufferdesc.guid3DAlgorithm = DS3DALG_HRTF_LIGHT;
#endif
            break;
        };
    }
    else
    {
        dsbufferdesc.guid3DAlgorithm = GUID_NULL;
    }
#endif

    hr = pDirectSound->CreateSoundBuffer(&dsbufferdesc, &m_pdirectsoundbuffer, nullptr);
	if (FAILED(hr)) return hr;

	// get a handle to the 3D buffer, if this is 3D
    if (bSupport3D)
    {
        hr = m_pdirectsoundbuffer->QueryInterface(IID_IDirectSound3DBuffer, (void**)&m_pdirectsound3Dbuffer);
        if (ZFailed(hr)) return hr;
    }

    return S_OK;
};

// Use an exisiting to initialize this buffer.  Note that the buffers will 
// share memory, so this only really works for static buffers.
HRESULT DS3DSoundBuffer::DuplicateBuffer8(IDirectSound8* pDirectSound, DS3DSoundBuffer* pBuffer)
{
    HRESULT hr; 

    // check the arguments
    if (!pBuffer || !pDirectSound)
    {
        ZAssert(false);
        return E_POINTER;
    }
    if (!pBuffer->m_pdirectsoundbuffer8)
    {
        ZAssert(false);
        return E_INVALIDARG;
    }

    // copy the basic info
    m_b3D = pBuffer->m_b3D;
    m_bListenerRelative = pBuffer->m_bListenerRelative;
    m_dwSampleRate = pBuffer->m_dwSampleRate;

    // duplicate the buffer

    // mdvalley: temp 'n' change. See last function.
    LPDIRECTSOUNDBUFFER mdDsb = nullptr;

    hr = pDirectSound->DuplicateSoundBuffer((IDirectSoundBuffer*)(IDirectSoundBuffer8*)pBuffer->m_pdirectsoundbuffer8, &mdDsb);
	if (ZFailed(hr)) return hr;

	hr = mdDsb->QueryInterface(IID_IDirectSoundBuffer8, (LPVOID*) &m_pdirectsoundbuffer8);
//	while(mdDsb->Release() > 1) {}
	mdDsb->Release();
	if (ZFailed(hr)) return hr;

//	hr = pDirectSound->DuplicateSoundBuffer(pBuffer->m_pdirectsoundbuffer, &m_pdirectsoundbuffer);
//  if (FAILED(hr)) return hr;

	// reset the 2D info.
    hr = m_pdirectsoundbuffer8->SetVolume(DSoundVolume(m_fGain));
    if (ZFailed(hr)) return hr;
    hr = m_pdirectsoundbuffer8->SetFrequency((LONG)(m_fPitch*m_dwSampleRate));
    if (ZFailed(hr)) return hr;

    // if this is 3D
    if (pBuffer->m_pdirectsound3Dbuffer != nullptr)
    {
        DS3DBUFFER ds3dbuf;

        // get a handle to the 3D buffer
        hr = m_pdirectsoundbuffer8->QueryInterface(IID_IDirectSound3DBuffer, (void**)&m_pdirectsound3Dbuffer);
        if (ZFailed(hr)) return hr;

        // reset the 3D info.
        ds3dbuf.dwSize = sizeof(ds3dbuf);
        ConvertVector(ds3dbuf.vPosition, m_vectPosition);
        ConvertVector(ds3dbuf.vVelocity, m_vectVelocity);
        ds3dbuf.dwInsideConeAngle = (LONG)m_fInnerAngle;
        ds3dbuf.dwOutsideConeAngle = (LONG)m_fOuterAngle;
        ConvertVector(ds3dbuf.vConeOrientation, m_vectOrientation);
        ds3dbuf.lConeOutsideVolume = DSoundVolume(m_fOutsideGain);
        ds3dbuf.flMaxDistance = m_fMinimumDistance * c_fMinToMaxDistanceRatio;
        ds3dbuf.flMinDistance = m_fMinimumDistance;
        ds3dbuf.dwMode = m_b3D 
            ? (m_bListenerRelative ? DS3DMODE_HEADRELATIVE : DS3DMODE_NORMAL) 
                : DS3DMODE_DISABLE;

        hr = m_pdirectsound3Dbuffer->SetAllParameters(&ds3dbuf, DS3D_IMMEDIATE);
        if (ZFailed(hr)) return hr;
    }

    return S_OK;
};

// Use an exisiting to initialize this buffer.  Note that the buffers will 
// share memory, so this only really works for static buffers.
HRESULT DS3DSoundBuffer::DuplicateBuffer(IDirectSound* pDirectSound, DS3DSoundBuffer* pBuffer)
{
    HRESULT hr; 

    // check the arguments
    if (!pBuffer || !pDirectSound)
    {
        ZAssert(false);
        return E_POINTER;
    }
    if (!pBuffer->m_pdirectsoundbuffer)
    {
        ZAssert(false);
        return E_INVALIDARG;
    }

    // copy the basic info
    m_b3D = pBuffer->m_b3D;
    m_bListenerRelative = pBuffer->m_bListenerRelative;
    m_dwSampleRate = pBuffer->m_dwSampleRate;

    // duplicate the buffer
	hr = pDirectSound->DuplicateSoundBuffer(pBuffer->m_pdirectsoundbuffer, &m_pdirectsoundbuffer);
    if (FAILED(hr)) return hr;

	// reset the 2D info.
    hr = m_pdirectsoundbuffer->SetVolume(DSoundVolume(m_fGain));
    if (ZFailed(hr)) return hr;
    hr = m_pdirectsoundbuffer->SetFrequency((LONG)(m_fPitch*m_dwSampleRate));
    if (ZFailed(hr)) return hr;

    // if this is 3D
    if (pBuffer->m_pdirectsound3Dbuffer != nullptr)
    {
        DS3DBUFFER ds3dbuf;

        // get a handle to the 3D buffer
        hr = m_pdirectsoundbuffer->QueryInterface(IID_IDirectSound3DBuffer, (void**)&m_pdirectsound3Dbuffer);
        if (ZFailed(hr)) return hr;

        // reset the 3D info.
        ds3dbuf.dwSize = sizeof(ds3dbuf);
        ConvertVector(ds3dbuf.vPosition, m_vectPosition);
        ConvertVector(ds3dbuf.vVelocity, m_vectVelocity);
        ds3dbuf.dwInsideConeAngle = (LONG)m_fInnerAngle;
        ds3dbuf.dwOutsideConeAngle = (LONG)m_fOuterAngle;
        ConvertVector(ds3dbuf.vConeOrientation, m_vectOrientation);
        ds3dbuf.lConeOutsideVolume = DSoundVolume(m_fOutsideGain);
        ds3dbuf.flMaxDistance = m_fMinimumDistance * c_fMinToMaxDistanceRatio;
        ds3dbuf.flMinDistance = m_fMinimumDistance;
        ds3dbuf.dwMode = m_b3D 
            ? (m_bListenerRelative ? DS3DMODE_HEADRELATIVE : DS3DMODE_NORMAL) 
                : DS3DMODE_DISABLE;

        hr = m_pdirectsound3Dbuffer->SetAllParameters(&ds3dbuf, DS3D_IMMEDIATE);
        if (ZFailed(hr)) return hr;
    }

    return S_OK;
};


// Start the buffer, starting as a looping buffer if requested.
HRESULT DS3DSoundBuffer::StartImpl(bool bLooping)
{
    HRESULT hr;
    DWORD dwFlags = bLooping ? DSBPLAY_LOOPING : 0;

    // DX7 bug: if the given sound is frequency shifted, we need to unshift 
    // it before starting the buffer.
    if (m_fPitch != 1.0)
    {
		if(m_pdirectsoundbuffer8)
			hr = m_pdirectsoundbuffer8->SetFrequency((LONG)m_dwSampleRate);
		else
			hr = m_pdirectsoundbuffer->SetFrequency((LONG)m_dwSampleRate);
        if (ZFailed(hr)) return hr;
    }

    // try starting the buffer
	if(m_pdirectsoundbuffer8)
		hr = m_pdirectsoundbuffer8->Play(0, 0, dwFlags);
	else
		hr = m_pdirectsoundbuffer->Play(0, 0, dwFlags);
    if (hr == DSERR_BUFFERLOST)
    {
        // the buffer was lost
        debugf("DSound buffer lost.\n");

        // try reloading it 
        hr = RestoreBuffer();
        if (ZFailed(hr)) return hr;

        // try starting it again
		if(m_pdirectsoundbuffer8)
			hr = m_pdirectsoundbuffer8->Play(0, 0, dwFlags);
		else
			hr = m_pdirectsoundbuffer->Play(0, 0, dwFlags);
    }
    if (FAILED(hr)) 
    { 
        ZAssert(hr == DSERR_INVALIDCALL);
        debugf("Error starting sound: %X\n", hr); 
        return hr; 
    }

    // DX7 bug: now reshift it since the buffer has been started
    if (m_fPitch != 1.0)
    {
		if(m_pdirectsoundbuffer8)
			hr = m_pdirectsoundbuffer8->SetFrequency((LONG)(m_fPitch*m_dwSampleRate));
		else
			hr = m_pdirectsoundbuffer->SetFrequency((LONG)(m_fPitch*m_dwSampleRate));
        if (ZFailed(hr)) return hr;
    }
    return S_OK;
}


DS3DSoundBuffer::DS3DSoundBuffer() :
    m_vectPosition(0, 0, 0),
    m_vectVelocity(0, 0, 0),
    m_vectOrientation(1.1f, 0, 0), // should be reset before playing
    m_fMinimumDistance(DS3D_DEFAULTMINDISTANCE + 0.01f), // should be reset
    m_fInnerAngle(360), m_fOuterAngle(360), m_fOutsideGain(0),
    m_fGain(0),
    m_fPitch(1.0f),
    m_bListenerRelative(false),
    m_pdirectsoundbuffer8(nullptr),
    m_pdirectsoundbuffer(nullptr)
{
}

DS3DSoundBuffer::~DS3DSoundBuffer()
{
    // make sure these are released before the DirectSound object.
    m_pdirectsoundbuffer = nullptr;
    m_pdirectsoundbuffer8 = nullptr;
    m_pdirectsound3Dbuffer = nullptr;
}


// Sets the volume and pitch iff the new values differ from the old.
HRESULT DS3DSoundBuffer::UpdateState(float fGain, float fPitch, bool bCanDefer)
{
    HRESULT hr;

    if (m_fGain != fGain)
    {
        if (fGain < -100 || fGain > 0)
        {
            ZAssert(false);
            return E_INVALIDARG;
        }

        m_fGain = fGain;
		if(m_pdirectsoundbuffer8)
			hr = m_pdirectsoundbuffer8->SetVolume(DSoundVolume(fGain));
		else
			hr = m_pdirectsoundbuffer->SetVolume(DSoundVolume(m_fGain));
        if (ZFailed(hr)) return hr;
    }

    if (m_fPitch != fPitch)
    {
        if (fPitch < (DSBFREQUENCY_MIN / m_dwSampleRate) 
            || (fPitch > DSBFREQUENCY_MAX / m_dwSampleRate))
        {
            ZAssert(false);
            return E_INVALIDARG;
        }

        m_fPitch = fPitch;
		if(m_pdirectsoundbuffer8)
			hr = m_pdirectsoundbuffer8->SetFrequency((LONG)(m_fPitch*m_dwSampleRate));
		else
			hr = m_pdirectsoundbuffer->SetFrequency((LONG)(m_fPitch*m_dwSampleRate));
        if (ZFailed(hr)) return hr;
    }

    return S_OK;
};


// Sets the 3D info iff the new values differ from the old.
HRESULT DS3DSoundBuffer::UpdateState3D(
    const Vector& vectPosition, 
    const Vector& vectVelocity, 
    const Vector& vectOrientation,
    float fMinimumDistance,
    float fInnerAngle,
    float fOuterAngle,
    float fOutsideGain,
    bool b3D,
    bool bListenerRelative,
    bool bCanDefer
    )
{
    // if this is not a 3D-capable sound, we should not be called
    if (m_pdirectsound3Dbuffer == nullptr)
    {
        ZAssert(FALSE);
        return E_NOTIMPL;
    }

    // if something has changed...
    if ((m_vectPosition != vectPosition)
        || (m_vectVelocity != vectVelocity)
        || (m_vectOrientation != vectOrientation)
        || (m_fMinimumDistance != fMinimumDistance)
        || (m_fInnerAngle != fInnerAngle)
        || (m_fOuterAngle != fOuterAngle)
        || (m_fOutsideGain != fOutsideGain)
        || (m_b3D != b3D)
        || (m_bListenerRelative != bListenerRelative)
        )
    {
        HRESULT hr;
        DS3DBUFFER ds3dbuf;

        // check the arguments
        ZAssertIsUnitVector(vectOrientation);
        if ((fMinimumDistance <= 0)
            || (fInnerAngle < 0 || fInnerAngle > 360)
            || (fOuterAngle < 0 || fOuterAngle > 360)
            || (fOutsideGain < -100 || fOutsideGain > 0)
            )
        {
            ZAssert(false);
            return E_INVALIDARG;
        }

        // set the cached parameters to the new info
        m_vectPosition = vectPosition;
        m_vectVelocity = vectVelocity;
        m_vectOrientation = vectOrientation;
        m_fMinimumDistance = fMinimumDistance;
        m_fInnerAngle = fInnerAngle;
        m_fOuterAngle = fOuterAngle;
        m_fOutsideGain = fOutsideGain;
        m_b3D = b3D;
        m_bListenerRelative = bListenerRelative;

        // set all of the parameters at once 
        ds3dbuf.dwSize = sizeof(ds3dbuf);
        ConvertVector(ds3dbuf.vPosition, m_vectPosition);
        ConvertVector(ds3dbuf.vVelocity, m_vectVelocity);
        ds3dbuf.dwInsideConeAngle = (LONG)m_fInnerAngle;
        ds3dbuf.dwOutsideConeAngle = (LONG)m_fOuterAngle;
        ConvertVector(ds3dbuf.vConeOrientation, m_vectOrientation);
        ds3dbuf.lConeOutsideVolume = DSoundVolume(m_fOutsideGain);
        ds3dbuf.flMaxDistance = m_fMinimumDistance * c_fMinToMaxDistanceRatio;
        ds3dbuf.flMinDistance = m_fMinimumDistance;
        ds3dbuf.dwMode = m_b3D 
            ? (m_bListenerRelative ? DS3DMODE_HEADRELATIVE : DS3DMODE_NORMAL) 
                : DS3DMODE_DISABLE;

        hr = m_pdirectsound3Dbuffer->SetAllParameters(
            &ds3dbuf, bCanDefer ? DS3D_DEFERRED : DS3D_IMMEDIATE);
        if (ZFailed(hr)) return hr;
    }

    return S_OK;
};


// gets the current status of the buffer
HRESULT DS3DSoundBuffer::GetStatus(bool& bPlaying, bool& bBufferLost)
{
    HRESULT hr;
    DWORD dwStatus;

	if(m_pdirectsoundbuffer8)
		hr = m_pdirectsoundbuffer8->GetStatus(&dwStatus);
	else
		hr = m_pdirectsoundbuffer->GetStatus(&dwStatus);

    bPlaying = (dwStatus & DSBSTATUS_PLAYING) != 0;
    bBufferLost = (dwStatus & DSBSTATUS_BUFFERLOST) != 0;

    return hr;
};


#ifdef _DEBUG
// return a human-readable description of the object, prepending
// strIndent to the beginning of each line.  
ZString DS3DSoundBuffer::DebugDump(const ZString& strIndent)
{
    DWORD dwStatus;
	if(m_pdirectsoundbuffer8)
		m_pdirectsoundbuffer8->GetStatus(&dwStatus);
	else
		m_pdirectsoundbuffer->GetStatus(&dwStatus);

    return strIndent + "DS3DSoundBuffer: "
        + (m_b3D ? " 3D" : " 2D")
        + ((dwStatus & DSBSTATUS_PLAYING) ? " playing" : "")
        + ((dwStatus & DSBSTATUS_LOOPING) ? " looping" : "")
#if DIRECTSOUND_VERSION >= 0x0700
        + ((dwStatus & DSBSTATUS_LOCHARDWARE) ? " in hardware" : "")
        + ((dwStatus & DSBSTATUS_LOCSOFTWARE) ? " in software" : "")
#endif
        + (m_b3D ? ("\n" + strIndent + "  Pos(" 
                + m_vectPosition.X() + ", " + m_vectPosition.Y() + ", " 
                + m_vectPosition.Z() + ")") 
                + (m_bListenerRelative ? " listener rel" : "")
            : ZString(""))
        + "\n";
}
#endif



/////////////////////////////////////////////////////////////////////////////
//
// DS3DStaticSoundBuffer
//
/////////////////////////////////////////////////////////////////////////////

// a cache of currently playing static sound buffers
DS3DStaticSoundBuffer::BufferCache DS3DStaticSoundBuffer::bufferCache;

// restore the contents of the buffer after a buffer loss
HRESULT DS3DStaticSoundBuffer::RestoreBuffer()
{
    HRESULT hr;

    // restore the buffer
	if(m_pdirectsoundbuffer8)
		hr = m_pdirectsoundbuffer8->Restore();
	else
		hr = m_pdirectsoundbuffer->Restore();
    if (ZFailed(hr)) return hr;

    // restore the contents of the buffer
    hr = LoadData();
    if (ZFailed(hr)) return hr;

    return S_OK;
};


// fills the buffer with data from the data source
HRESULT DS3DStaticSoundBuffer::LoadData()
{
    void *pvBlock1Data, *pvBlock2Data;
    DWORD dwBlock1Length, dwBlock2Length;
    HRESULT hr;

    // try locking the sound buffer
	if(m_pdirectsoundbuffer8)
		hr = m_pdirectsoundbuffer8->Lock(
			0, m_pdata->GetSize(),
			&pvBlock1Data, &dwBlock1Length,
			&pvBlock2Data, &dwBlock2Length,
			0
			);
	else
		hr = m_pdirectsoundbuffer->Lock(
			0, m_pdata->GetSize(),
			&pvBlock1Data, &dwBlock1Length,
			&pvBlock2Data, &dwBlock2Length,
			0
			);
    if (hr == DSERR_BUFFERLOST)
    {
        //
        // if the buffer was lost, try reloading it and starting it again.
        //
        
        // restore the buffer
		if(m_pdirectsoundbuffer8)
			hr = m_pdirectsoundbuffer8->Restore();
		else
			hr = m_pdirectsoundbuffer->Restore();
        if (ZFailed(hr)) return hr;

        // try locking it again
		if(m_pdirectsoundbuffer8)
			hr = m_pdirectsoundbuffer8->Lock(
				0, m_pdata->GetSize(),
				&pvBlock1Data, &dwBlock1Length,
				&pvBlock2Data, &dwBlock2Length,
				0
				);
		else
			hr = m_pdirectsoundbuffer->Lock(
				0, m_pdata->GetSize(),
				&pvBlock1Data, &dwBlock1Length,
				&pvBlock2Data, &dwBlock2Length,
				0
				);
    }
    if (ZFailed(hr)) return hr;

    // fill in the contents of the buffer
    m_pdata->GetData(pvBlock1Data, 0, dwBlock1Length);        
    if (pvBlock2Data)
    {
        m_pdata->GetData(pvBlock2Data, dwBlock1Length, dwBlock2Length);
    }

    // unlock the buffer
	if(m_pdirectsoundbuffer8)
		hr = m_pdirectsoundbuffer8->Unlock(
			pvBlock1Data, dwBlock1Length,
			pvBlock2Data, dwBlock2Length
			);
	else
		hr = m_pdirectsoundbuffer->Unlock(
			pvBlock1Data, dwBlock1Length,
			pvBlock2Data, dwBlock2Length
			);
    if (ZFailed(hr)) return hr;

    return S_OK;
}


// Constructor
DS3DStaticSoundBuffer::DS3DStaticSoundBuffer() :
    m_bHasBeenPlayed(false)
{
    m_iterSelf = bufferCache.end();
}

// Destructor
DS3DStaticSoundBuffer::~DS3DStaticSoundBuffer()
{
    // remove this buffer from the buffer cache.
    if (m_iterSelf != bufferCache.end())
    {
        bufferCache.erase(m_iterSelf);
    }
}

// Initializes this object with the given wave data, 3D support, and sound 
// quality.
HRESULT DS3DStaticSoundBuffer::Init8(IDirectSound8* pDirectSound, ISoundPCMData* pdata, 
    bool bLooping, bool bSupport3D, ISoundEngine::Quality quality, bool bAllowHardware
    )
{
    HRESULT hr;

    m_bLooping = bLooping;
    m_pdata = pdata;

    // see if we have an instance of this buffer which we can simply clone    
    CacheKey cacheKey(pdata, bSupport3D, quality, bAllowHardware);

    BufferCache::iterator iterCache = bufferCache.find(cacheKey);  
    if ((iterCache != bufferCache.end()) && false)					// mdvalley: disable buffer duplication
    {
		hr = DuplicateBuffer8(pDirectSound, (*iterCache).second);
        if (FAILED(hr)) return hr;
    }
    else
    {
        // create a new buffer for the sound
        hr = CreateBuffer8(pDirectSound, pdata, pdata->GetSize(), true, bSupport3D, quality, bAllowHardware);
        if (FAILED(hr)) return hr;

        // for static buffers, fill the buffer at creation time
        hr = LoadData();
        if (ZFailed(hr)) return hr;
    }

    m_iterSelf = bufferCache.insert(BufferCache::value_type(cacheKey, this));

    return S_OK;
}

// Initializes this object with the given wave data, 3D support, and sound 
// quality.
HRESULT DS3DStaticSoundBuffer::Init(IDirectSound* pDirectSound, ISoundPCMData* pdata, 
    bool bLooping, bool bSupport3D, ISoundEngine::Quality quality, bool bAllowHardware
    )
{
    HRESULT hr;

    m_bLooping = bLooping;
    m_pdata = pdata;

    // see if we have an instance of this buffer which we can simply clone    
    CacheKey cacheKey(pdata, bSupport3D, quality, bAllowHardware);

    BufferCache::iterator iterCache = bufferCache.find(cacheKey);  
    if (iterCache != bufferCache.end())
    {
		hr = DuplicateBuffer(pDirectSound, (*iterCache).second);
        if (FAILED(hr)) return hr;
    }
    else
    {
        // create a new buffer for the sound
        hr = CreateBuffer(pDirectSound, pdata, pdata->GetSize(), true, bSupport3D, quality, bAllowHardware);
        if (FAILED(hr)) return hr;

        // for static buffers, fill the buffer at creation time
        hr = LoadData();
        if (ZFailed(hr)) return hr;
    }

    m_iterSelf = bufferCache.insert(BufferCache::value_type(cacheKey, this));

    return S_OK;
}

// starts the given buffer playing at the given position.  
HRESULT DS3DStaticSoundBuffer::Start(DWORD dwPosition, bool bIsStopping)
{
    HRESULT hr;

    ZAssert(dwPosition <= m_pdata->GetSize());
    if (m_bHasBeenPlayed || dwPosition != 0)
    {
		if(m_pdirectsoundbuffer8)
			hr = m_pdirectsoundbuffer8->SetCurrentPosition(dwPosition);
		else
			hr = m_pdirectsoundbuffer->SetCurrentPosition(dwPosition);
        if (ZFailed(hr)) return hr;
    }

    hr = StartImpl(m_bLooping);
    if (FAILED(hr)) return hr;

    m_bHasBeenPlayed = true;

    return S_OK; 
};


// stops the given buffer.
HRESULT DS3DStaticSoundBuffer::Stop(bool bForceNow)
{
	if(m_pdirectsoundbuffer8)
		return m_pdirectsoundbuffer8->Stop();
	else
		return m_pdirectsoundbuffer->Stop();
};


// Gets the current position of the sound
HRESULT DS3DStaticSoundBuffer::GetPosition(DWORD& dwPosition)
{
    // for static sounds, the buffer position is the source position
	if(m_pdirectsoundbuffer8)
        return m_pdirectsoundbuffer8->GetCurrentPosition(&dwPosition, nullptr);
	else
        return m_pdirectsoundbuffer->GetCurrentPosition(&dwPosition, nullptr);
};



/////////////////////////////////////////////////////////////////////////////
//
// DS3DStreamingSoundBuffer
//
/////////////////////////////////////////////////////////////////////////////


// determines if a buffer cursor starting at dwStart and ending at dwEnd
// would have crossed event dwTrigger.
inline bool DS3DStreamingSoundBuffer::CrossedBoundary(DWORD dwTrigger, DWORD dwStart, DWORD dwEnd)
{
    return dwTrigger < m_dwBufferSize
        && ((dwStart <= dwTrigger && (dwEnd >= dwTrigger || dwEnd < dwStart))
            || (dwStart > dwTrigger && (dwEnd < dwStart && dwEnd >= dwTrigger)));
}

// updates anything waiting for the play/read pointer to cross a certain 
// point in the buffer.
void DS3DStreamingSoundBuffer::ReadPointerUpdate(DWORD dwReadOffset)
{
    if (CrossedBoundary(m_dwStopOffset, m_dwLastReadOffset, dwReadOffset))
    {
        m_bPlayingSilence = true;
    }

    // update the last known read offset
    m_dwLastReadOffset = dwReadOffset;
}

// gets the last played source offset from the last played buffer offset.
DWORD DS3DStreamingSoundBuffer::GetPlayedSourceOffset(DWORD dwLastPlayedPosition)
{
    if (m_bPlayingSilence)
    {
        return m_pdata->GetSize();
    }
    else 
    {
        // figure out the last offset where any real data was written
        DWORD dwWriteOffset = (m_dwStopOffset < m_dwBufferSize) 
            ? m_dwStopOffset : m_dwWriteOffset;

        if (dwLastPlayedPosition > dwWriteOffset)
        {
            int nOffset = (int)(m_dwSourceOffset - m_dwBufferSize - dwWriteOffset 
                + dwLastPlayedPosition);
            if (nOffset < 0)
                return nOffset + m_dwBufferSize;
            else
                return nOffset;
        }
        else
        {
            return m_dwSourceOffset - dwWriteOffset + dwLastPlayedPosition;
        }
    }
};

// restore the contents of the buffer after a buffer loss
HRESULT DS3DStreamingSoundBuffer::RestoreBuffer()
{
    HRESULT hr;

    // restore the buffer
	if(m_pdirectsoundbuffer8)
		hr = m_pdirectsoundbuffer8->Restore();
	else
		hr = m_pdirectsoundbuffer->Restore();
    if (ZFailed(hr)) return hr;

    // get the last known good playback position
    DWORD dwLastPlayedPosition;
	if(m_pdirectsoundbuffer8)
        hr = m_pdirectsoundbuffer8->GetCurrentPosition(&dwLastPlayedPosition, nullptr);
	else
        hr = m_pdirectsoundbuffer->GetCurrentPosition(&dwLastPlayedPosition, nullptr);
    if (ZFailed(hr)) return hr;

    // reset the buffer write pointer to the same point
    m_dwWriteOffset = dwLastPlayedPosition;

    // update the read pointer flags, as appropriate
    ReadPointerUpdate(dwLastPlayedPosition);

    // reset the source offset to the last played position (taking into 
    // account buffer wraparound).
    m_dwSourceOffset = GetPlayedSourceOffset(dwLastPlayedPosition);

    // restore the contents of the buffer from that position onwards.
    hr = UpdateBufferContents();
    if (ZFailed(hr)) return hr;

    return S_OK;
};


// Fills the buffer with as much data as it can handle.  
//
// bTrustWritePtr is a bit of a hack for ASR sounds.  Basically, the 
// problem is that when stop is called, an ASR sound needs to call
// GetCurrentPosition to figure out the first safe place it can start
// writing the sound's release.  When UpdateBufferContents calls 
// GetCurrentPosition a moment later, however, it may return a slightly
// larger offset for the minimum write position.  We want to use the result
// of the first call to GetCurrentPosition in this case, and not ZAssert
// that the write pointer is after the result returned by the second call
// to GetCurrentPosition.  
HRESULT DS3DStreamingSoundBuffer::UpdateBufferContents(bool bTrustWritePtr)
{
    HRESULT hr;
    DWORD dwReadOffset, dwMinWriteOffset;

    // get the boundaries of the sound buffer where we can write
	if(m_pdirectsoundbuffer8)
		hr = m_pdirectsoundbuffer8->GetCurrentPosition(&dwReadOffset, &dwMinWriteOffset);
	else
		hr = m_pdirectsoundbuffer->GetCurrentPosition(&dwReadOffset, &dwMinWriteOffset);
    if (ZFailed(hr)) return hr;

    // make sure we are writing _after_ the last writable Offset
    // (messy because we have to take into account buffer wraparound)
    if ((((dwReadOffset < m_dwWriteOffset)
            && (dwMinWriteOffset < dwReadOffset || dwMinWriteOffset > m_dwWriteOffset))
        || ((dwReadOffset >= m_dwWriteOffset)
            && (dwMinWriteOffset < dwReadOffset && dwMinWriteOffset > m_dwWriteOffset))
        )
        && !bTrustWritePtr
        )
    {
        // Doh! We've played incorrect sound!
        debugf("sound buffer underflow; read %d, write %d, min write %d\n", 
            dwReadOffset, m_dwWriteOffset, dwMinWriteOffset);

        // recover as best we can.
        m_dwWriteOffset = dwMinWriteOffset;
    }

    // Check to see if we've crossed any events waiting on the play pointer
    ReadPointerUpdate(dwReadOffset);

    // figure out the length of data we want.
    DWORD dwLength = ((dwReadOffset > m_dwWriteOffset) 
        ? (dwReadOffset - m_dwWriteOffset)
        : (m_dwBufferSize + dwReadOffset - m_dwWriteOffset));

    void *pvBlock1Data, *pvBlock2Data;
    DWORD dwBlock1Length, dwBlock2Length;

    // try locking the sound buffer
	if(m_pdirectsoundbuffer8)
		hr = m_pdirectsoundbuffer8->Lock(
			m_dwWriteOffset, dwLength,
			&pvBlock1Data, &dwBlock1Length,
			&pvBlock2Data, &dwBlock2Length,
			0
			);
	else
		hr = m_pdirectsoundbuffer->Lock(
			m_dwWriteOffset, dwLength,
			&pvBlock1Data, &dwBlock1Length,
			&pvBlock2Data, &dwBlock2Length,
			0
			);
    if (hr == DSERR_BUFFERLOST)
    {
        //
        // if the buffer was lost, try reloading it and starting it again.
        //
        
        // restore the buffer
		if(m_pdirectsoundbuffer8)
			hr = m_pdirectsoundbuffer8->Restore();
		else
			hr = m_pdirectsoundbuffer->Restore();
        if (ZFailed(hr)) return hr;

        // try locking it again
		if(m_pdirectsoundbuffer8)
			hr = m_pdirectsoundbuffer8->Lock(
				m_dwWriteOffset, dwLength,
				&pvBlock1Data, &dwBlock1Length,
				&pvBlock2Data, &dwBlock2Length,
				0
				);
		else
			hr = m_pdirectsoundbuffer->Lock(
				m_dwWriteOffset, dwLength,
				&pvBlock1Data, &dwBlock1Length,
				&pvBlock2Data, &dwBlock2Length,
				0
				);
    }
    if (ZFailed(hr)) return hr;

    // fill in the contents of the buffer
    hr = StreamData(pvBlock1Data, dwBlock1Length);
    if (ZFailed(hr)) return hr;

    if (pvBlock2Data)
    {
        hr = StreamData(pvBlock2Data, dwBlock2Length);
        if (ZFailed(hr)) return hr;
    }

    // unlock the buffer
	if(m_pdirectsoundbuffer8)
		hr = m_pdirectsoundbuffer8->Unlock(
			pvBlock1Data, dwBlock1Length,
			pvBlock2Data, dwBlock2Length
			);
	else
		hr = m_pdirectsoundbuffer->Unlock(
			pvBlock1Data, dwBlock1Length,
			pvBlock2Data, dwBlock2Length
			);
    if (ZFailed(hr)) return hr;

    return S_OK;
}


// streams the given length of data out to the buffer pointed to by pvBuffer
HRESULT DS3DStreamingSoundBuffer::StreamData(void *pvBuffer, DWORD dwLength)
{
    ZAssert(pvBuffer && dwLength != 0);

    DWORD dwLengthRemaining = dwLength;
    BYTE* pcWritePtr = (BYTE*)pvBuffer;
    DWORD dwSourceSize = m_pdata->GetSize();

    // just copy the source to the buffer until the buffer is filled or,
    // for non-looping sounds, we reach the end of the source.
    do {
        // copy data up until the end of the sound.
        DWORD dwCopyLength = std::min(dwLengthRemaining, dwSourceSize - m_dwSourceOffset);

        // copy the source
        if (dwCopyLength > 0)
            m_pdata->GetData(pcWritePtr, m_dwSourceOffset, dwCopyLength);
        
        // update the pointers
        pcWritePtr += dwCopyLength;
        m_dwSourceOffset += dwCopyLength;
        dwLengthRemaining -= dwCopyLength;

        // if we are looping and we've reached the end, go back the start.
        if (m_dwSourceOffset == dwSourceSize && m_bLooping)
            m_dwSourceOffset = 0;

    } while (dwLengthRemaining > 0 && m_bLooping);
    
    // if we have more that we need to fill in...
    if (dwLengthRemaining != 0)
    {
        // it's past the end of the sound, so fill it with 0's
        BYTE nFillValue = (m_pdata->GetBitsPerSample() == 8) ? 0x80 : 0;
        memset(pcWritePtr, nFillValue, dwLengthRemaining);

        // mark where the sound stops
        if (m_dwStopOffset >= m_dwBufferSize)
        {
            m_dwStopOffset = m_dwWriteOffset + dwLength - dwLengthRemaining;
        }
    }

    // update the write offset
    m_dwWriteOffset = (m_dwWriteOffset + dwLength) % m_dwBufferSize;

    return S_OK;
}

// Initializes this object with the given wave data, 3D support, sound 
// quality, and buffer length (in seconds)
HRESULT DS3DStreamingSoundBuffer::Init(IDirectSound* pDirectSound, ISoundPCMData* pdata, 
    bool bLooping, bool bSupport3D, ISoundEngine::Quality quality, bool bAllowHardware,
    float fBufferLength
    )
{
    HRESULT hr;

    m_bLooping = bLooping;
    m_pdata = pdata;

    m_dwBufferSize = (DWORD)(fBufferLength * pdata->GetBytesPerSec());

    // create a new buffer for the sound
    hr = CreateBuffer(pDirectSound, pdata, m_dwBufferSize, false, bSupport3D, quality, bAllowHardware);
    if (FAILED(hr)) return hr;

    return S_OK;
}

// Initializes this object with the given wave data, 3D support, sound 
// quality, and buffer length (in seconds)
HRESULT DS3DStreamingSoundBuffer::Init8(IDirectSound8* pDirectSound, ISoundPCMData* pdata, 
    bool bLooping, bool bSupport3D, ISoundEngine::Quality quality, bool bAllowHardware,
    float fBufferLength
    )
{
    HRESULT hr;

    m_bLooping = bLooping;
    m_pdata = pdata;

    m_dwBufferSize = (DWORD)(fBufferLength * pdata->GetBytesPerSec());

    // create a new buffer for the sound
    hr = CreateBuffer8(pDirectSound, pdata, m_dwBufferSize, false, bSupport3D, quality, bAllowHardware);
    if (FAILED(hr)) return hr;

    return S_OK;
}

// Updates the contents of the streaming buffer.
bool DS3DStreamingSoundBuffer::Execute()
{
    HRESULT hr = UpdateBufferContents();
    ZAssert(SUCCEEDED(hr));

    // if we are playing silence, stop the buffer.
    if (m_bPlayingSilence)
    {
		HRESULT hr;
		if(m_pdirectsoundbuffer8)
			hr = m_pdirectsoundbuffer8->Stop();
		else
			hr = m_pdirectsoundbuffer->Stop();
        ZAssert(SUCCEEDED(hr));

        return false;
    }
    else
    {
        return true;
    }
}

// Constructor
DS3DStreamingSoundBuffer::DS3DStreamingSoundBuffer() :
    m_bHasBeenPlayed(false)
{
}

// Destructor
DS3DStreamingSoundBuffer::~DS3DStreamingSoundBuffer()
{
    // if this has been played at least once
    if (m_bHasBeenPlayed)
    {
        // make sure we are not on the update list before we go away
        m_threadUpdate.RemoveTask(this);
    }
}

// Initializes this object with the given wave data, 3D support, and sound 
// quality.
HRESULT DS3DStreamingSoundBuffer::Init8(IDirectSound8* pDirectSound, ISoundPCMData* pdata, 
    bool bLooping, bool bSupport3D, ISoundEngine::Quality quality, bool bAllowHardware
    )
{
//    return Init8(pDirectSound, pdata, bLooping, bSupport3D, quality, bAllowHardware, 5.0f);
	return Init8(pDirectSound, pdata, bLooping, bSupport3D, quality, bAllowHardware, 1.0f);		// mdvalley: Make it shorter.
}

// Initializes this object with the given wave data, 3D support, and sound 
// quality.
HRESULT DS3DStreamingSoundBuffer::Init(IDirectSound* pDirectSound, ISoundPCMData* pdata,
	bool bLooping, bool bSupport3D, ISoundEngine::Quality quality, bool bAllowHardware
	)
{
	return Init(pDirectSound, pdata, bLooping, bSupport3D, quality, bAllowHardware, 1.0f);
}

// starts the given buffer playing at the given position.  
HRESULT DS3DStreamingSoundBuffer::Start(DWORD dwPosition, bool bIsStopping)
{
    HRESULT hr;

    // make sure this is not already playing
    ZAssert(!m_threadUpdate.HasTask(this));

    if (m_bHasBeenPlayed)
    {
		if(m_pdirectsoundbuffer8)
			hr = m_pdirectsoundbuffer8->SetCurrentPosition(0);
		else
			hr = m_pdirectsoundbuffer->SetCurrentPosition(0);
        if (ZFailed(hr)) return hr;
    }

    ZAssert(dwPosition <= m_pdata->GetSize());
    m_dwSourceOffset = dwPosition;
    m_dwWriteOffset = 0;
    m_dwLastReadOffset = 0;
    m_dwStopOffset = m_dwBufferSize;
    m_bPlayingSilence = false;

    UpdateBufferContents();

    hr = StartImpl(true);
    if (FAILED(hr)) return hr;

    m_bHasBeenPlayed = true;

    // add it to the update thread's list
    m_threadUpdate.AddTask(this);

    return S_OK; 
};

// stops the given buffer.
HRESULT DS3DStreamingSoundBuffer::Stop(bool bForceNow)
{
    // remove this from the update thread's task list
    m_threadUpdate.RemoveTask(this);

	if(m_pdirectsoundbuffer8)
		return m_pdirectsoundbuffer8->Stop();
	else
		return m_pdirectsoundbuffer->Stop();
};

// Gets the current position of the sound
HRESULT DS3DStreamingSoundBuffer::GetPosition(DWORD& dwPosition)
{
    DWORD dwBufferPosition;

    // get the buffer position is the source position
	HRESULT hr;
	if(m_pdirectsoundbuffer8)
        hr = m_pdirectsoundbuffer8->GetCurrentPosition(&dwBufferPosition, nullptr);
	else
        hr = m_pdirectsoundbuffer->GetCurrentPosition(&dwBufferPosition, nullptr);
    if (ZFailed(hr)) return hr;

    // update the read pointer flags, as appropriate
    ReadPointerUpdate(dwBufferPosition);

    // translate from that to the source position
    dwPosition = GetPlayedSourceOffset(dwBufferPosition);

    return S_OK;
};

// gets the current status of the buffer
HRESULT DS3DStreamingSoundBuffer::GetStatus(bool& bPlaying, bool& bBufferLost)
{
    HRESULT hr;
    DWORD dwStatus;

	if(m_pdirectsoundbuffer8)
		hr = m_pdirectsoundbuffer8->GetStatus(&dwStatus);
	else
		hr = m_pdirectsoundbuffer->GetStatus(&dwStatus);
    if (ZFailed(hr)) return hr;

    bPlaying = (dwStatus & DSBSTATUS_PLAYING) != 0;
    bBufferLost = (dwStatus & DSBSTATUS_BUFFERLOST) != 0;

    return S_OK;
};


// The update thread which handles filling the buffers as they play
//TaskListThread DS3DStreamingSoundBuffer::m_threadUpdate(THREAD_PRIORITY_TIME_CRITICAL, 500);
TaskListThread DS3DStreamingSoundBuffer::m_threadUpdate(THREAD_PRIORITY_TIME_CRITICAL, 250);		// mdvalley: Update streaming buffers more often




/////////////////////////////////////////////////////////////////////////////
//
// DS3DASRSoundBuffer
//
/////////////////////////////////////////////////////////////////////////////


// Destructor
DS3DASRSoundBuffer::~DS3DASRSoundBuffer()
{
    // if this has been played at least once
    if (m_bHasBeenPlayed)
    {
        // make sure we are not on the update list before we go away
        m_threadUpdate.RemoveTask(this);
        m_bHasBeenPlayed = false;
    }
}

// updates anything waiting for the play/read pointer to cross a certain 
// point in the buffer.
void DS3DASRSoundBuffer::ReadPointerUpdate(DWORD dwReadOffset)
{
    if (CrossedBoundary(m_dwSustainBufferOffset, m_dwLastReadOffset, dwReadOffset))
    {
        m_bPlayingSustain = true;
    }

    if (CrossedBoundary(m_dwReleaseBufferOffset, m_dwLastReadOffset, dwReadOffset))
    {
        ZAssert(m_bEnding);
        m_bPlayingRelease = true;
    }

    DS3DStreamingSoundBuffer::ReadPointerUpdate(dwReadOffset);
}

// gets the last played source offset from the last played buffer offset.
DWORD DS3DASRSoundBuffer::GetPlayedSourceOffset(DWORD dwLastPlayedPosition)
{
    if (m_bPlayingSilence)
    {
        return m_pdata->GetSize();
    }
    else 
    {
        // figure out the buffer offset and source offset that are most relevant
        DWORD dwReferenceOffset;
        DWORD dwSourceOffset;
        
        // if we are in the attack portion
        if (!m_bPlayingSustain)
        {
            // if we have written part of the sustain section
            if (m_dwSustainBufferOffset < m_dwBufferSize)
            {
                // use the start of the sustain section as our reference point.
                dwReferenceOffset = m_dwSustainBufferOffset;
                dwSourceOffset = m_dwLoopOffset;
            }
            else
            {
                // use the real values
                dwReferenceOffset = m_dwWriteOffset;
                dwSourceOffset = m_dwSourceOffset;
            }
        }
        // if we are in the sustain portion
        else if (!m_bPlayingRelease)
        {
            // if we have written part of the release section
            if (m_dwReleaseBufferOffset < m_dwBufferSize)
            {
                // use the start of the release section as our reference point.
                dwReferenceOffset = m_dwReleaseBufferOffset;
                dwSourceOffset = m_dwLoopOffset + m_dwLoopLength;
            }
            else
            {
                // use the real values
                dwReferenceOffset = m_dwWriteOffset;
                dwSourceOffset = m_dwSourceOffset;
            }
        }
        // if we are in the release portion heading towards silence
        else 
        {
            // if we have started writing silence
            if (m_dwStopOffset < m_dwBufferSize) 
            {
                // use the end of the sound as our reference point.
                dwReferenceOffset = m_dwStopOffset;
                dwSourceOffset = m_pdata->GetSize();
            }
            else
            {
                // use the real values
                dwReferenceOffset = m_dwWriteOffset;
                dwSourceOffset = m_dwSourceOffset;
            }
        }

        

        // figure out the space between the buffer and the reference point
        int nDelta = int(dwReferenceOffset) - int(dwLastPlayedPosition);
        if (nDelta <= 0)
        {
            nDelta += m_dwBufferSize;

            if (nDelta < 0)
            {
                ZAssert(false);
                nDelta = 0;
            }
        }

        // use that to figure out the offset

        // if we are in the sustain loop
        if (m_bPlayingSustain && !m_bPlayingRelease)
        {
            // we need to figure out where in the sustain loop this puts us
            int nOffset = dwSourceOffset - (nDelta % m_dwLoopLength);
            if (nOffset < m_dwLoopOffset)
                nOffset += m_dwLoopLength;
            return nOffset;
        }
        else
        {
            // just adjust in the obvious way
            ZAssert(int(dwSourceOffset) - nDelta >= 0);
            return int(dwSourceOffset) - nDelta;
        }
    }
};

// streams the given length of data out to the buffer pointed to by pvBuffer
HRESULT DS3DASRSoundBuffer::StreamData(void *pvBuffer, DWORD dwLength)
{
    ZAssert(pvBuffer && dwLength != 0);

    DWORD dwSourceSize = m_pdata->GetSize();

    DWORD dwLengthRemaining = dwLength;
    BYTE* pcWritePtr = (BYTE*)pvBuffer;
    DWORD dwCopyLength;

    // if we are writing the attack portion of the sound...
    if (m_dwSourceOffset < m_dwLoopOffset)
    {
        // copy data up until the end of the attack or end of the write block.
        dwCopyLength = std::min(dwLengthRemaining, m_dwLoopOffset - m_dwSourceOffset);

        // copy the source
        if (dwCopyLength > 0)
        {
            m_pdata->GetData(pcWritePtr, m_dwSourceOffset, dwCopyLength);                
        }
        
        // update the pointers
        pcWritePtr += dwCopyLength;
        m_dwSourceOffset += dwCopyLength;
        dwLengthRemaining -= dwCopyLength;

        // If we've finished writing the attack, mark the beginning of the 
        // sustain portion.
        if (m_dwSourceOffset >= m_dwLoopOffset)
        {
            ZAssert(m_dwSourceOffset == m_dwLoopOffset);
            m_dwSustainBufferOffset = 
                m_dwWriteOffset + dwLength - dwLengthRemaining;
        }
    }

    // write 0 or more sustain loops.  Do this by finishing any loop which 
    // we have not completed, and then write additional loops if we are not
    // trying to end the sound.
    while (dwLengthRemaining > 0 && (
           (m_dwSourceOffset == m_dwLoopOffset && !m_bEnding)
        || (m_dwSourceOffset > m_dwLoopOffset 
            && (m_dwSourceOffset < m_dwLoopOffset + m_dwLoopLength))))
    {
        // copy data up until the end of a sustain loop or end of the write block.
        dwCopyLength = std::min(dwLengthRemaining,
            m_dwLoopOffset + m_dwLoopLength - m_dwSourceOffset);

        // copy the source
        ZAssert(dwCopyLength > 0);
        m_pdata->GetData(pcWritePtr, m_dwSourceOffset, dwCopyLength);
        
        // update the pointers
        pcWritePtr += dwCopyLength;
        m_dwSourceOffset += dwCopyLength;
        dwLengthRemaining -= dwCopyLength;

        // If we've finished an entire loop...
        if (m_dwSourceOffset >= m_dwLoopOffset + m_dwLoopLength)
        {
            ZAssert(m_dwSourceOffset == m_dwLoopOffset + m_dwLoopLength);
            
            // reset the source offset to the beginning of the loop
            m_dwSourceOffset = m_dwLoopOffset;

            // if this is the end of the sustain section, keep track of 
            // that.
            if (m_bEnding)
            {
                m_dwReleaseBufferOffset = 
                    m_dwWriteOffset + dwLength - dwLengthRemaining;
            }
        }
    }


    // write the release section of the loop, if appropriate
    if (dwLengthRemaining > 0 && m_dwSourceOffset < dwSourceSize)
    {
        ZAssert(m_bEnding);
        ZAssert(m_dwSourceOffset == m_dwLoopOffset 
            || (m_dwSourceOffset >= m_dwLoopOffset + m_dwLoopLength));
        
        // when we first get here, adjust the read pointer from the 
        // beginning of the sustain loop to the begining of the release.
        if (m_dwSourceOffset < m_dwLoopOffset + m_dwLoopLength)
            m_dwSourceOffset = m_dwLoopOffset + m_dwLoopLength;
        
        // copy data up until the end or end of the write block.
        dwCopyLength = std::min(dwLengthRemaining, dwSourceSize - m_dwSourceOffset);

        // copy the source
        if (dwCopyLength > 0)
        {
            m_pdata->GetData(pcWritePtr, m_dwSourceOffset, dwCopyLength);                
        }
        
        // update the pointers
        pcWritePtr += dwCopyLength;
        m_dwSourceOffset += dwCopyLength;
        dwLengthRemaining -= dwCopyLength;

        // If we've finished writing the release, mark where the sound ends
        if (m_dwSourceOffset >= dwSourceSize)
        {
            ZAssert(m_dwSourceOffset == dwSourceSize);
            m_dwStopOffset = m_dwWriteOffset + dwLength - dwLengthRemaining;
        }
    }

    // if we have more that we need to fill in, then we need to fill with silence
    if (dwLengthRemaining != 0)
    {
        // it's past the end of the sound, so fill it with 0's
        BYTE nFillValue = (m_pdata->GetBitsPerSample() == 8) ? 0x80 : 0;
        memset(pcWritePtr, nFillValue, dwLengthRemaining);
    }

    // update the write offset
    m_dwWriteOffset = (m_dwWriteOffset + dwLength) % m_dwBufferSize;

    return S_OK;
}


// Initializes this object with the given wave data, 3D support, and sound 
// quality.
HRESULT DS3DASRSoundBuffer::Init(IDirectSound* pDirectSound, ISoundPCMData* pdata, 
    DWORD dwLoopOffset, DWORD dwLoopLength, bool bSupport3D, 
    ISoundEngine::Quality quality, bool bAllowHardware
    )
{
    ZAssert(dwLoopOffset + dwLoopLength < pdata->GetSize());
    ZAssert(dwLoopLength > 0);

    HRESULT hr;

    m_pdata = pdata;
    m_dwLoopOffset = dwLoopOffset;
    m_dwLoopLength = dwLoopLength;

    // REVIEW: what size buffer would work best for these sounds?
//    hr = DS3DStreamingSoundBuffer::Init(
//        pDirectSound, pdata, false, bSupport3D, quality, bAllowHardware, 1.5f);
	hr = DS3DStreamingSoundBuffer::Init(
		pDirectSound, pdata, false, bSupport3D, quality, bAllowHardware, 1.0f);		// mdvalley: shorten it to 1.0
    if (ZFailed(hr)) return hr;

    return S_OK;
}

// Initializes this object with the given wave data, 3D support, and sound 
// quality.
HRESULT DS3DASRSoundBuffer::Init8(IDirectSound8* pDirectSound, ISoundPCMData* pdata, 
    DWORD dwLoopOffset, DWORD dwLoopLength, bool bSupport3D, 
    ISoundEngine::Quality quality, bool bAllowHardware
    )
{
    ZAssert(dwLoopOffset + dwLoopLength < pdata->GetSize());
    ZAssert(dwLoopLength > 0);

    HRESULT hr;

    m_pdata = pdata;
    m_dwLoopOffset = dwLoopOffset;
    m_dwLoopLength = dwLoopLength;

    // REVIEW: what size buffer would work best for these sounds?
//    hr = DS3DStreamingSoundBuffer::Init(
//        pDirectSound, pdata, false, bSupport3D, quality, bAllowHardware, 1.5f);
	hr = DS3DStreamingSoundBuffer::Init8(
		pDirectSound, pdata, false, bSupport3D, quality, bAllowHardware, 1.0f);		// mdvalley: shorten it to 1.0
    if (ZFailed(hr)) return hr;

    return S_OK;
}

// starts the given buffer playing at the given position.  
HRESULT DS3DASRSoundBuffer::Start(DWORD dwPosition, bool bIsStopping)
{
    ZAssert(bIsStopping || dwPosition < m_dwLoopOffset + m_dwLoopLength);

    DWORD dwSourceLength = m_pdata->GetSize();

    m_dwSustainBufferOffset = m_dwBufferSize;
    m_bPlayingSustain = dwPosition >= m_dwLoopOffset;
    m_dwReleaseBufferOffset = m_dwBufferSize;
    m_bPlayingRelease = dwPosition >= m_dwLoopOffset + m_dwLoopLength;
    ZAssert(m_bPlayingRelease ? bIsStopping : true);
    m_bEnding = bIsStopping;

    DS3DStreamingSoundBuffer::Start(dwPosition, bIsStopping);

    return S_OK; 
};

// stops the given buffer.
HRESULT DS3DASRSoundBuffer::Stop(bool bForceNow)
{
    if (bForceNow)
    {
        return DS3DStreamingSoundBuffer::Stop(bForceNow);
    }
    else if (!m_bEnding)
    {
        // remove this from the update thread's task list to avoid race
        // conditions.
        m_threadUpdate.RemoveTask(this);

        // transition to the release as soon as possible.
        // to do this, we just invalidate the buffer as far back as possible
        // and rewrite it with the knowledge that we are ending the sound.

        HRESULT hr;

        // get the first possible write position
        DWORD dwWritePosition;
		if(m_pdirectsoundbuffer8)
            hr = m_pdirectsoundbuffer8->GetCurrentPosition(nullptr, &dwWritePosition);
		else
            hr = m_pdirectsoundbuffer->GetCurrentPosition(nullptr, &dwWritePosition);
        if (ZFailed(hr)) return hr;

        // reset the source offset to the new write position (taking into 
        // account buffer wraparound).
        int nWriteDelta = dwWritePosition - m_dwLastReadOffset;
        if (nWriteDelta < 0)
        {
            nWriteDelta += m_dwBufferSize;
        }
        ZAssert(nWriteDelta >= 0);
        DWORD dwLastPlayedSourceOffset = GetPlayedSourceOffset(m_dwLastReadOffset);
        if (dwLastPlayedSourceOffset + nWriteDelta > m_dwLoopOffset)
        {
            m_dwSourceOffset = 
                (dwLastPlayedSourceOffset + nWriteDelta - m_dwLoopOffset) 
                    % m_dwLoopLength + m_dwLoopOffset;
        }
        else
        {
            m_dwSourceOffset = dwLastPlayedSourceOffset + nWriteDelta;
        }

        // reset the buffer write pointer to the same point
        m_dwWriteOffset = dwWritePosition;

        // restore the contents of the buffer from that position onwards.
        m_bEnding = true;
        hr = UpdateBufferContents(true);
        if (ZFailed(hr)) return hr;

        // put it back in the update loop
        m_threadUpdate.AddTask(this);
    }

    return S_OK;
};


};
