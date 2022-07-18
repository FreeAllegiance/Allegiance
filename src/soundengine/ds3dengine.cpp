//
// ds3dengine.cpp
//
// Classes representing a DirectSound3D implementation of a sound engine
//

#include "ds3dengine.h"

#include <algorithm>

#include "soundbase.h"
#include "ds3dutil.h"
#include "ds3dbuffer.h"
#include "ds3dvirtualbuffer.h"
#include "soundutil.h"
#include <functional>

namespace SoundEngine {

/////////////////////////////////////////////////////////////////////////////
//
// DefaultListener
//
/////////////////////////////////////////////////////////////////////////////

// a listener who simply sits at the origin
class DefaultListener : public ISoundListener
{
public:

    //
    // ISoundPositionSource
    //

    // Gets the position of the sound in space
    HRESULT GetPosition(Vector& vectPosition)
    {
        vectPosition = Vector(0, 0, 0);
        return S_OK;
    };

    // Gets the velocity of the sound in space
    HRESULT GetVelocity(Vector& vectVelocity)
    {
        vectVelocity = Vector(0, 0, 0);
        return S_OK;
    };

    // Gets the orientation of the sound in space, used for sound cones.
    virtual HRESULT GetOrientation(Vector& vectOrientation)
    {
        vectOrientation = Vector(0, 0, 1);
        return S_OK;
    };

    // Returns S_OK if the position, velocity and orientation reported are 
    // relative to the listener, S_FALSE otherwise.  
    virtual HRESULT IsListenerRelative()
    {
        return S_FALSE; // it would be useless if it was "listener relative"
    };

    // Returns S_OK if this source is still playing the sound, S_FALSE 
    // otherwise.  This might be false if a sound emitter is destroyed, for 
    // example, in which case the sound might fade out.  Once it returns
    // S_FALSE once, it should never return S_OK again.
    virtual HRESULT IsPlaying()
    {
        return S_OK;
    };
    
    //
    // ISoundListener 
    //

    // get the "up" vector for the listener
    virtual HRESULT GetUpDirection(Vector& vectUp)
    {
        vectUp = Vector(0, 1, 0);
        return S_OK;
    }
};



/////////////////////////////////////////////////////////////////////////////
//
// DS3DSoundEngine
//
/////////////////////////////////////////////////////////////////////////////

// dumps the capablilites of this DirectSound implementation to the 
// debug output.
void DS3DSoundEngine::DumpCaps()
{
    debugf(
        "Directsound driver is %scertified by Microsoft "
            "and is %semulated.\n", 
        (m_dscaps.dwFlags & DSCAPS_CERTIFIED) ? "" : "not ",
        (m_dscaps.dwFlags & DSCAPS_EMULDRIVER) ? "" : "not "
        );
    debugf(
        "  The primary buffer can be%s%s%s%s.\n",
        (m_dscaps.dwFlags & DSCAPS_PRIMARY8BIT) ? " 8 bit" : "",
        (m_dscaps.dwFlags & DSCAPS_PRIMARY16BIT) ? " 16 bit" : "",
        (m_dscaps.dwFlags & DSCAPS_PRIMARYMONO) ? " mono" : "",
        (m_dscaps.dwFlags & DSCAPS_PRIMARYSTEREO) ? " stereo" : ""
        );
    debugf(
        "  The secondary buffers can range %s from %d Hz to %d Hz, and can be %s%s%s%s.\n",
        (m_dscaps.dwFlags & DSCAPS_CONTINUOUSRATE) ? "continuously" : "in steps",
        m_dscaps.dwMinSecondarySampleRate,
        m_dscaps.dwMaxSecondarySampleRate,
        (m_dscaps.dwFlags & DSCAPS_SECONDARY8BIT) ? " 8 bit" : "",
        (m_dscaps.dwFlags & DSCAPS_SECONDARY16BIT) ? " 16 bit" : "",
        (m_dscaps.dwFlags & DSCAPS_SECONDARYMONO) ? " mono" : "",
        (m_dscaps.dwFlags & DSCAPS_SECONDARYSTEREO) ? " stereo" : ""
        );
    debugf(
        "  There is hardware support for mixing %d buffers, including %d static and %d streaming buffers.\n",
        m_dscaps.dwMaxHwMixingAllBuffers,
        m_dscaps.dwMaxHwMixingStaticBuffers,
        m_dscaps.dwMaxHwMixingStreamingBuffers
        );
    debugf(
        "  There is hardware support for %d 3D buffers, including %d static 3D and %d streaming 3D buffers.\n",
        m_dscaps.dwMaxHw3DAllBuffers,
        m_dscaps.dwMaxHw3DStaticBuffers,
        m_dscaps.dwMaxHw3DStreamingBuffers
        );
};


// Sets the format of the primary buffer to the given sample rate, number 
// of bits, and number of channels
HRESULT DS3DSoundEngine::SetPrimaryBufferFormat(int nSampleRate, int nNumberOfBits, int nChannels)
{
    HRESULT hr;
    WAVEFORMATEX waveformatex;

	memset(&waveformatex, 0, sizeof(WAVEFORMATEX));	// mdvalley: Zero it

    waveformatex.cbSize = sizeof(WAVEFORMATEX);
    waveformatex.wFormatTag = WAVE_FORMAT_PCM; 
    waveformatex.nChannels = nChannels; 
    waveformatex.nSamplesPerSec = nSampleRate; 
    waveformatex.wBitsPerSample = nNumberOfBits; 
    waveformatex.nBlockAlign = waveformatex.wBitsPerSample / 8 * waveformatex.nChannels;
    waveformatex.nAvgBytesPerSec = waveformatex.nSamplesPerSec * waveformatex.nBlockAlign;

    hr = m_pPrimaryBuffer->SetFormat(&waveformatex);
    if (ZFailed(hr)) return hr;

#ifdef _DEBUG
    m_pPrimaryBuffer->GetFormat(&waveformatex, sizeof(waveformatex), nullptr);

    // print the new format in the debug window
    debugf(
        "Primary buffer set to %d Hz, %d bits, %d channels (attempted %d Hz, %d bits, %d channels)\n",
        waveformatex.nSamplesPerSec, waveformatex.wBitsPerSample, waveformatex.nChannels,
        nSampleRate, nNumberOfBits, nChannels
        );
#endif

    return S_OK;
};


// updates the listener position, orientation, etc. for direct sound 3D.
HRESULT DS3DSoundEngine::UpdateListener()
{
    HRESULT hr;
    DS3DLISTENER listenerdata;

    listenerdata.dwSize = sizeof(listenerdata);
    listenerdata.flDistanceFactor = m_fDistanceFactor;
    listenerdata.flDopplerFactor = m_fDopplerFactor;
    listenerdata.flRolloffFactor = m_fRolloffFactor;
    
    Vector vectPosition;
    hr = m_plistener->GetPosition(vectPosition);
    if (ZFailed(hr)) return hr;
    ConvertVector(listenerdata.vPosition, vectPosition);

    Vector vectVelocity;
    hr = m_plistener->GetVelocity(vectVelocity);
    if (ZFailed(hr)) return hr;
    ConvertVector(listenerdata.vVelocity, vectVelocity);

    Vector vectOrientation;
    hr = m_plistener->GetOrientation(vectOrientation);
    if (ZFailed(hr)) return hr;
    ZAssertIsUnitVector(vectOrientation);
    ConvertVector(listenerdata.vOrientFront, vectOrientation);

    Vector vectUp;
    hr = m_plistener->GetUpDirection(vectUp);
    if (ZFailed(hr)) return hr;
    ZAssertIsUnitVector(vectUp);
    ConvertVector(listenerdata.vOrientTop, vectUp);

    return m_pDSListener->SetAllParameters(&listenerdata, DS3D_DEFERRED);
};


// Is this an error worth killing a virtual sound for, or just something 
// transitory?  For example, we may lose a sound buffer when being swapped 
// to the background, but we want the sound to continue playing when we 
// are swapped back in.  
bool DS3DSoundEngine::IsSeriousError(HRESULT hr)
{
    switch (hr)
    {
    case S_OK:
    case S_FALSE:
        return false;

    case DSERR_BUFFERLOST:
        debugf("Sound buffer lost.\n");
        return false;

    case DSERR_OUTOFMEMORY:
        debugf("Out of sound memory.\n");
        return false;
    
    default:
        return FAILED(hr);
    }
}


// Constructor - the real initialization is done in init.
DS3DSoundEngine::DS3DSoundEngine() :
    m_quality(midQuality),
    m_bAllowHardware(true),
    m_fRolloffFactor(1.0f),
    m_fDopplerFactor(1.0f),
    m_fDistanceFactor(1.0f),
    m_pDirectSound8(nullptr),
    m_pDirectSound(nullptr)
{
    m_plistener = new DefaultListener();
    m_peventsourceUpdate = new IntegerEventSourceImpl();
    m_dwLastUpdateTime = timeGetTime(); 
    m_pBufferSourceDelegate = new SoundBufferSourceDelegate(this);
}


DS3DSoundEngine::~DS3DSoundEngine()
{
    HRESULT hr;

    m_peventsourceUpdate = nullptr;

    // stop the primary buffer from playing continuously
    if (m_pPrimaryBuffer)
    {
        hr = m_pPrimaryBuffer->Stop();
        ZFailed(hr);
    }

    // release all dsound buffers
	// mdvalley: Call Release on the primary
	m_pPrimaryBuffer->Release();

    m_pPrimaryBuffer = nullptr;
    m_pDSListener = nullptr;
}


// Basic initialization.  This was pulled out of the constructor so that we
// can return error values.
HRESULT DS3DSoundEngine::Init(HWND hwnd, bool bUseDSound8)
{
    HRESULT hr;

    // Create the device
	// mdvalley: Changed to Create8
	if(bUseDSound8)
        hr = DirectSoundCreate8(nullptr, &m_pDirectSound8, nullptr);
	else
        hr = DirectSoundCreate(nullptr, &m_pDirectSound, nullptr);
    if (hr == DSERR_NODRIVER || hr == DSERR_ALLOCATED || ZFailed(hr)) return hr;

	if(bUseDSound8)
		hr = m_pDirectSound8->SetCooperativeLevel(hwnd, DSSCL_PRIORITY);
	else
		hr = m_pDirectSound->SetCooperativeLevel(hwnd, DSSCL_PRIORITY);
    if (hr == DSERR_ALLOCATED) 
    {
        debugf("Failure: unable to get DSSCL_PRIORITY access to DSound.  Failing over to DSSCL_NORMAL.\n");
		if(bUseDSound8)
			hr = m_pDirectSound->SetCooperativeLevel(hwnd, DSSCL_NORMAL);
		else
			hr = m_pDirectSound->SetCooperativeLevel(hwnd, DSSCL_NORMAL);
    }
    if (ZFailed(hr)) return hr;

    // go ahead and try compacting the memory; it's not neccessary but may 
    // give us better hardware utilization on some sound cards.
	// mdvalley: This has no function in DX8, but it's harmless.
	if(!bUseDSound8)
		m_pDirectSound->Compact(); // if it fails, who cares.  

    // get the primary buffer
    DSBUFFERDESC dsbufferdesc;
    memset(&dsbufferdesc, 0, sizeof(DSBUFFERDESC));
    dsbufferdesc.dwSize = sizeof(dsbufferdesc);
    dsbufferdesc.dwFlags = DSBCAPS_CTRL3D | DSBCAPS_PRIMARYBUFFER;
	dsbufferdesc.dwBufferBytes = 0;		// mdvalley: Set these for the primary
    dsbufferdesc.lpwfxFormat = nullptr;
	if(bUseDSound8)
        hr = m_pDirectSound8->CreateSoundBuffer(&dsbufferdesc, &m_pPrimaryBuffer, nullptr);
	else
        hr = m_pDirectSound->CreateSoundBuffer(&dsbufferdesc, &m_pPrimaryBuffer, nullptr);
    if (ZFailed(hr)) return hr;

    // get the DirectSound listener
    hr = m_pPrimaryBuffer->QueryInterface(IID_IDirectSound3DListener, (LPVOID *)&m_pDSListener);
    if (ZFailed(hr)) return hr;

    // get the capabilities of the hardware
    memset(&m_dscaps, 0, sizeof(DSCAPS));
    m_dscaps.dwSize = sizeof(DSCAPS);
	if(bUseDSound8)
		hr = m_pDirectSound8->GetCaps(&m_dscaps);
	else
		hr = m_pDirectSound->GetCaps(&m_dscaps);
    if (ZFailed(hr)) return hr;
    DumpCaps();

    // Set the quality-related settings
    hr = SetQuality(midQuality);
    if (ZFailed(hr)) return hr;

    // start the primary buffer playing continuously
    hr = m_pPrimaryBuffer->Play(0, 0, DSBPLAY_LOOPING);
    if ((hr != DSERR_PRIOLEVELNEEDED) && ZFailed(hr)) return hr;

    return S_OK;
}


// Rebuild the sound stage to reflect any recent changes in sound
HRESULT DS3DSoundEngine::Update()
{
    HRESULT hr;
    Vector vectListenerPosition;
    DWORD dwUpdateTime = timeGetTime();
    DWORD dwUpdatePeriod = dwUpdateTime - m_dwLastUpdateTime;

	// mdvalley: Update no faster than every 30ms
	if(dwUpdatePeriod < 30)
		return S_OK;

    // fire the update event.
    m_peventsourceUpdate->Trigger(dwUpdatePeriod);

    typedef std::vector<DSVirtualSoundBuffer*> TempSoundList;

    // get the current position of the listener, to use for future 
    // calculations.
    hr = m_plistener->GetPosition(vectListenerPosition);
    if (ZFailed(hr)) return hr;

    
    // Walk the list of active sounds and call update for each one, 
    // removing the sounds which have ended, keeping track of which sounds
    // have playing buffers
    
    TempSoundList listPrevSounds;
    listPrevSounds.reserve(m_nNumBuffersMax);

    VirtualSoundList::iterator iterSound;
    for (iterSound = m_listActiveSounds.begin();
        iterSound != m_listActiveSounds.end();)
    {
        hr = (*iterSound)->Update(dwUpdatePeriod, vectListenerPosition, 
            m_fRolloffFactor);
        if (ZFailed(hr)) return hr;

        // if this is no longer playing, remove it from the list.
        if ((*iterSound)->IsStopped())
        {
            iterSound = m_listActiveSounds.erase(iterSound);
        }
        else
        {
            // if the sound has an active buffer, keep track of it.
            if ((*iterSound)->HasPlayingBuffer())
                listPrevSounds.push_back(
                    (TRef<DSVirtualSoundBuffer>)(*iterSound)
                    );
            ++iterSound;
        }
    }


    // get the n most important sounds that we wish to play
    TempSoundList listNewSounds;
    if (!m_listActiveSounds.empty())
    {
        // create a list of all of the sounds
        TempSoundList listAllSounds(m_listActiveSounds.size());
        std::transform(m_listActiveSounds.begin(), m_listActiveSounds.end(), 
            listAllSounds.begin(), RefToPtr<DSVirtualSoundBuffer>());

        // turn it into a priority queue
        std::make_heap(listAllSounds.begin(), listAllSounds.end(), std::not2(SoundPriorityCompare()));

        // get the m_nNumBuffersDesired most important sounds by popping 
        // them off of the queue
        listNewSounds.reserve(m_nNumBuffersDesired);
        for (int nSound = 0; nSound < std::min<int>(m_nNumBuffersDesired, listAllSounds.size()); nSound++)
        {
            std::pop_heap(listAllSounds.begin(), listAllSounds.end() - nSound, 
                std::not2(SoundPriorityCompare()));

            DSVirtualSoundBuffer* pbuffer = (*(listAllSounds.end() - nSound - 1));
            if (pbuffer->IsAudible())
                listNewSounds.push_back(pbuffer);
        }
    }

   
    // Compare the previously playing sounds and the list of new sounds we
    // want to play, and start and stop the sound buffers as needed to 
    // achieve this.
    {
        // sort the old list by priority
        std::sort(listPrevSounds.begin(), listPrevSounds.end(), SoundPriorityCompare());

        // find the first sound which is not in the new list
        TempSoundList::iterator iterDead = 
            (listNewSounds.empty()) ? listPrevSounds.begin() :
            std::find_if(listPrevSounds.begin(), listPrevSounds.end(), 
                [&](DSVirtualSoundBuffer* sound) {
                    //select those where the priority of the last sound in the new list is higher.
                    return SoundPriorityCompare()(*(listNewSounds.end() - 1), sound);
                }
            );

        // stop all of the old buffers 
        // REVIEW: should fade out, if we have buffers.
        for (; iterDead != listPrevSounds.end(); iterDead++)
        {
            hr = (*iterDead)->StopBuffer();
            if (ZFailed(hr)) return hr;
        }

        // Start any new sounds            
        for (TempSoundList::iterator iterNew = listNewSounds.begin();
            iterNew != listNewSounds.end(); ++iterNew)
        {
            if (!(*iterNew)->HasPlayingBuffer())
            {
				if(m_pDirectSound8)
					hr = (*iterNew)->StartBuffer8(m_pDirectSound8, m_quality, m_bAllowHardware);
				else
					hr = (*iterNew)->StartBuffer(m_pDirectSound, m_quality, m_bAllowHardware);
                if (IsSeriousError(hr))
                {
                    // Silently fail
                    debugf("Serious error during update while starting sound: %X\n", hr);
                }
            }
        };
    }

    // update the listener's position, velocity, etc., 
    hr = UpdateListener();
    if (ZFailed(hr)) return hr;

    // Commit any and all deferred changes.
    hr = m_pDSListener->CommitDeferredSettings();
    if (ZFailed(hr)) return hr;

    m_dwLastUpdateTime = dwUpdateTime;

    return S_OK;
}


// Gets the number of virtual sound buffers that are playing at a given 
// moment.  (no guarantees on how this number changes - it's just a perf.
// number to use.)
HRESULT DS3DSoundEngine::GetNumPlayingVirtualBuffers(int& nBuffers)
{
    nBuffers = m_listActiveSounds.size();

    return S_OK;
}


// Sets a general quality of playback (CPU time vs. fidelity)
HRESULT DS3DSoundEngine::SetQuality(Quality quality)
{
    HRESULT hr;

    // stop all playing sound buffers
    VirtualSoundList::iterator iterSound;
    for (iterSound = m_listActiveSounds.begin();
        iterSound != m_listActiveSounds.end(); ++iterSound)
    {
        if ((*iterSound)->HasPlayingBuffer())
        {
            hr = (*iterSound)->StopBuffer();
            if (ZFailed(hr)) return hr;
        }
    }
    
    switch (quality)
    {
    case minQuality:
        hr = SetPrimaryBufferFormat(22050, 8, 1);
        if ((hr != DSERR_PRIOLEVELNEEDED) && ZFailed(hr)) return hr;
        m_nNumBuffersDesired = 8;
        m_nNumBuffersMax = 8;
        break;

    case midQuality:
    default:
        hr = SetPrimaryBufferFormat(22050, 16, 2);
        if ((hr != DSERR_PRIOLEVELNEEDED) && ZFailed(hr)) return hr;
        if (m_bAllowHardware)
        {
            m_nNumBuffersDesired = std::max(16, (int)m_dscaps.dwMaxHwMixingStreamingBuffers * 2 / 3);
            m_nNumBuffersMax = std::max(24, (int)m_dscaps.dwMaxHwMixingStreamingBuffers);
        }
        else
        {
            m_nNumBuffersDesired = 16;
            m_nNumBuffersMax = 24;
        }
        break;

    case maxQuality:
        hr = SetPrimaryBufferFormat(44100, 16, 2);
        if ((hr != DSERR_PRIOLEVELNEEDED) && ZFailed(hr)) return hr;
        if (m_bAllowHardware)
        {
            m_nNumBuffersDesired = std::max(24, (int)m_dscaps.dwMaxHwMixingStreamingBuffers - 8);
            m_nNumBuffersMax = std::max(32, (int)m_dscaps.dwMaxHwMixingStreamingBuffers);
        }
        else
        {
            m_nNumBuffersDesired = 24;
            m_nNumBuffersMax = 32;
        }
        break;
    }

    m_quality = quality;

    return S_OK;
};


// Allows/disallows hardware acceleration.
HRESULT DS3DSoundEngine::EnableHardware(bool bEnable)
{
    if (m_bAllowHardware != bEnable)
    {
        m_bAllowHardware = bEnable;

        // force the re-creation of all playing sounds, and adjust the 
        // voice limits.
        return SetQuality(m_quality);
    }
    else
    {
        return S_OK;
    }
}


// Sets the listener to use for the current sounds
HRESULT DS3DSoundEngine::SetListener(ISoundListener* plistener)
{
    if (plistener == nullptr)
    {
        ZAssert(false);
        return E_POINTER;
    }

    m_plistener = plistener;

    return S_OK;
}


// Sets the conversion from game units to meters
HRESULT DS3DSoundEngine::SetDistanceFactor(float fMetersPerUnit)
{
    if (fMetersPerUnit <= 0)
    {
        ZAssert(false);
        return E_INVALIDARG;
    };

    m_fDistanceFactor = fMetersPerUnit;
    return S_OK;
}


// Sets the rolloff factor, where 1.0 is the real world attenuation with 
// distance, 2.0 is twice the attenuation of the real world, etc..
HRESULT DS3DSoundEngine::SetRolloffFactor(float fRolloffFactor)
{
    if (fRolloffFactor < DS3D_MINROLLOFFFACTOR
        || fRolloffFactor > DS3D_MAXROLLOFFFACTOR)
    {
        ZAssert(false);
        return E_INVALIDARG;
    };

    m_fRolloffFactor = fRolloffFactor;
    return S_OK;
}


// Sets the doppler factor, where 1.0 is real-world doppler
HRESULT DS3DSoundEngine::SetDopplerFactor(float fDopplerFactor)
{
    if (fDopplerFactor < DS3D_MINDOPPLERFACTOR
        || fDopplerFactor > DS3D_MAXDOPPLERFACTOR)
    {
        ZAssert(false);
        return E_INVALIDARG;
    };

    m_fDopplerFactor = fDopplerFactor;
    return S_OK;
}


// Gets a buffer source for this object (not guarenteed to keep the sound 
// engine alive due to circular reference problems)
ISoundBufferSource* DS3DSoundEngine::GetBufferSource()
{
    return m_pBufferSourceDelegate;
};


// Creates a static sound buffer of the given wave file.  If bLooping is 
// true, the sound will loop until stopped.
HRESULT DS3DSoundEngine::CreateStaticBuffer(TRef<ISoundInstance>& psoundNew, 
    ISoundPCMData* pcmdata, bool bLooping, ISoundPositionSource* psource)
{
    TRef<DSVirtualSoundBuffer> pvirtualsound;

    if (psource != nullptr)
    {
        pvirtualsound = new DS3DVirtualSoundBuffer(pcmdata, bLooping, psource);
    }
    else
    {
        pvirtualsound = new DSVirtualSoundBuffer(pcmdata, bLooping);
    }

    m_listActiveSounds.push_back(pvirtualsound);
    psoundNew = pvirtualsound;

    return S_OK;
}


// Creates a sound buffer with a loop in the middle.  The sound will play
// the start sound, play the loop sound until it gets a soft stop, then
// play the rest of the sound.  
HRESULT DS3DSoundEngine::CreateASRBuffer(TRef<ISoundInstance>& psoundNew, 
    ISoundPCMData* pcmdata, unsigned uLoopStart, unsigned uLoopLength, 
    ISoundPositionSource* psource)
{
    // check the parameters
    if (pcmdata == nullptr)
    {
        ZAssert(false);
        return E_POINTER;
    }

    // make sure the loop is contained in the buffer
    if (uLoopStart + uLoopLength > pcmdata->GetSize())
    {
        ZAssert(false);
        return E_INVALIDARG;
    }

    TRef<DSVirtualSoundBuffer> pvirtualsound;

    if (psource != nullptr)
    {
        pvirtualsound = new DS3DVirtualSoundBuffer(pcmdata, uLoopStart, uLoopLength, psource);
    }
    else
    {
        pvirtualsound = new DSVirtualSoundBuffer(pcmdata, uLoopStart, uLoopLength);
    }

    m_listActiveSounds.push_back(pvirtualsound);
    psoundNew = pvirtualsound;

    return S_OK;
}


// Gets an event which fires each time update is called.  This can be used
// for some of the trickier sounds that change with time.
IIntegerEventSource* DS3DSoundEngine::GetUpdateEventSource()
{
    return m_peventsourceUpdate;
};


#ifdef _DEBUG
// return a human-readable description of the object, prepending
// strIndent to the beginning of each line.  
ZString DS3DSoundEngine::DebugDump(const ZString& strIndent)
{
    ZString strResult = strIndent + "DS3DSoundEngine: \n";

    // Walk the list of active sounds and dump each one
    VirtualSoundList::iterator iterSound;
    for (iterSound = m_listActiveSounds.begin();
        iterSound != m_listActiveSounds.end(); ++iterSound)
    {
        strResult += SoundDebugDump(
            (ISoundInstance*)(TRef<DSVirtualSoundBuffer>&)(*iterSound), 
            strIndent + "  ");
    }

    return strResult;
}
#endif

};
