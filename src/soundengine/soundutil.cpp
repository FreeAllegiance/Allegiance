//
// soundutil.cpp
//
// Useful utility classes/functions
//

#include "soundutil.h"

#include <algorithm>
#include <vorbis/vorbisfile.h>

#include "soundbase.h"

namespace SoundEngine {


//////////////////////////////////////////////////////////////////////////////
//
// LoadWaveFile function
//
//////////////////////////////////////////////////////////////////////////////

class SoundFile : public ISoundPCMData  SoundDebugImpl
{
private:

    // A map between open files and filenames, so that we only have one open 
    // copy of a given file.  
    typedef std::map<ZString, ISoundPCMData* > SoundFileCache;
    static SoundFileCache m_mapOpenFiles;

    // the number of channels (mono = 1, stereo = 2, etc.)
    unsigned m_uChannels;

    // the number of bits per sample
    unsigned m_uBitsPerSample;

    // the sample rate of the file
    unsigned m_uSampleRate;

    // the size of the wave data
    unsigned m_uSize;

    // the wave data itself
    void* m_pvData;

    // the memory mapped file base
    void* m_pvFileContents;

    // the file from which this is mapped
    ZString m_strFilename;

	// mdvalley: whether the file is Ogg (true) or Wav (false)
	bool m_bIsOgg;

    SoundFile()
        : m_pvData(nullptr), m_pvFileContents(nullptr), m_bIsOgg(false) {};

    ~SoundFile()
    {
        if (m_pvFileContents)
        {
            // close the file
            UnmapViewOfFile(m_pvFileContents);
        }

        if (m_pvData)
        {
            // remove this from the file cache
            m_mapOpenFiles.erase(m_strFilename);

			if(m_bIsOgg)
				// mdvalley: free the data malloc'd in DecodeOGG
				free(m_pvData);
        }
    }

    // Open a file and map the contents into memory
    HRESULT OpenFile(const ZString& strFilename)
    {
        ZAssert(m_pvFileContents == nullptr);

        HANDLE hFile, hMapping;

        hFile = CreateFile(strFilename, GENERIC_READ, FILE_SHARE_READ, nullptr,
            OPEN_EXISTING, 0, nullptr);

        if (hFile == INVALID_HANDLE_VALUE)
        {
            ZAssert(false);
            return STG_E_FILENOTFOUND;
        }

        // if the file is really large, we may do better if we have the file 
        // system assume we are going to read it sequentially. 
        const DWORD dwSequentialFileThreshold = 1000000;
        if (GetFileSize(hFile, nullptr) > dwSequentialFileThreshold)
        {
            CloseHandle(hFile); 
            
            hFile = CreateFile(strFilename, GENERIC_READ, FILE_SHARE_READ, nullptr,
                OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, nullptr);

            if (hFile == INVALID_HANDLE_VALUE)
            {
                ZAssert(false);
                return STG_E_FILENOTFOUND;
            }
        }

        hMapping = CreateFileMapping(hFile, nullptr, PAGE_READONLY, 0, 0, nullptr);

        CloseHandle(hFile); 

        if (hMapping == INVALID_HANDLE_VALUE)
        {
            ZAssert(false);
            return E_HANDLE;
        }

        m_pvFileContents = MapViewOfFile (hMapping, FILE_MAP_READ, 0, 0, 0);
        
        CloseHandle(hMapping);

        if (m_pvFileContents == nullptr)
        {
            ZAssert(false);
            return E_HANDLE;
        }

        return S_OK;
    }

    // parse a chunk of memory in RIFF format for wave data
    HRESULT ParseWaveData(void* pvRiffFile)
    {
        ZAssert(m_pvData == nullptr);
    
        DWORD *pdwCurrent = (DWORD*)pvRiffFile;

        // check to make sure the first few bytes identify this as a RIFF file
        DWORD dwFileType = *(pdwCurrent++);
        if (dwFileType != mmioFOURCC('R', 'I', 'F', 'F'))
        {
            ZAssert(false);
            return E_FAIL;
        }

        // find the end of the file
        DWORD dwLength = *(pdwCurrent++);
        DWORD *pdwEnd = (DWORD*)(((BYTE*)pdwCurrent) + dwLength);

        // check to make sure the format of the contents is a wave file
        DWORD dwFormat = *(pdwCurrent++);
        if (dwFormat != mmioFOURCC('W', 'A', 'V', 'E'))
        {
            ZAssert(false);
            return E_FAIL;
        }

        bool bFoundFormat = false;
        bool bFoundData = false;
        
        // loop through the RIFF blocks until we find both the format block and
        // the data block.
        while ((pdwCurrent < pdwEnd) && (!bFoundFormat || !bFoundData))
        {
            DWORD dwBlockType = *(pdwCurrent++);
            DWORD dwBlockLength = *(pdwCurrent++);

            switch (dwBlockType)
            {
            // this is a format block, containing the format of the wave data
            case mmioFOURCC('f', 'm', 't', ' '):                
                {
                    if (dwBlockLength < sizeof(WAVEFORMAT))
                    {
                        // looks like a corrupt file
                        m_pvData = nullptr;
                        ZAssert(false);
                        return E_FAIL;
                    }
                    WAVEFORMAT *pFormat = (WAVEFORMAT*)pdwCurrent;

                    // make sure this is a type we can handle
                    if (pFormat->wFormatTag != WAVE_FORMAT_PCM)
                    {
                        // file is not PCM (compressed, perhaps?).  At the 
                        // moment, however, we only handle PCM wave files.
                        m_pvData = nullptr;
                        ZAssert(false);
                        return E_NOTIMPL;
                    }
                    
                    // read in the appropriate fields
                    m_uChannels = pFormat->nChannels;
                    m_uBitsPerSample = pFormat->nBlockAlign / pFormat->nChannels * 8;
                    m_uSampleRate = pFormat->nSamplesPerSec;

                    bFoundFormat = true;
                }
                break;

            // the contents of this chunk is the raw PCM data
            case mmioFOURCC('d', 'a', 't', 'a'):
                {
                    m_pvData = pdwCurrent;
                    m_uSize = dwBlockLength;
                    bFoundData = true;
                }
                break;
            }
    
            // advance the pointer to the next block
            pdwCurrent = (DWORD*)(((BYTE*)pdwCurrent) + ((dwBlockLength+1)&~1));
        }

        // if we did not find one of the blocks, it's not a usable wave file.
        if (!bFoundFormat || !bFoundData)
        {
            m_pvData = nullptr;
            ZAssert(false);
            return E_FAIL;
        }

      
        ZAssert(m_pvData != nullptr);
        return S_OK;
    }

	HRESULT DecodeOGG(const ZString& strFilename)
	{
//		struct _timeb timestart;
//		struct _timeb timedone;
//		double elapsed;
//		_ftime_s(&timestart);

		char pcmout[4096];

		OggVorbis_File vf;
		bool eof = false;
		int current_section;

		BYTE* writePtr;
        FILE* OggFile = nullptr;
#if _MSC_VER >= 1400		// mdvalley: Makes '05 happy
		fopen_s(&OggFile, strFilename, "rb");
#else
		OggFile = fopen(strFilename, "rb");
#endif
        if(OggFile == nullptr)
		{
			debugf("%s: File not found.\n", strFilename);
			return E_FAIL;
		}
		
        if(ov_open(OggFile, &vf, nullptr, 0) < 0)
		{
			debugf("%s is not a valid Ogg bitstream.\n", strFilename);
			fclose(OggFile);
			return E_FAIL;
		}

		{
			vorbis_info *vi = ov_info(&vf, -1);
			m_uChannels = vi->channels;		// Number of channels. Alleg's 3D sound only works on mono.
			m_uSampleRate = vi->rate;		// Bitrate
			m_uBitsPerSample = 16;			// The decoder only supports 8 and 16 bit audio.
			long sampleCount = (long)ov_pcm_total(&vf, -1);
			m_uSize = (m_uBitsPerSample / 8) * sampleCount * m_uChannels;		// Find the size of the resulting wave data
			m_pvData = malloc(m_uSize);
			writePtr = (BYTE*)m_pvData;
		}

		while(!eof)
		{
			long ret = ov_read(&vf, pcmout, 4096, 0, (m_uBitsPerSample / 8), 1, &current_section);
			if(ret == 0)
			{
				// End of file
				eof = true;
			}
			else if(ret < 0)
			{
				// Stream error. Should be OK to keep going
			}
			else
			{
				// Possible sample rate change. Anyone who tries to make a multi-rate
				// OGG for Alleg deserves the problems it'll cause with this decoder.
#if _MSC_VER >= 1400									// Might as well make it '05 compatible
				memcpy_s(writePtr, 4096, pcmout, ret);	// Write it to the wave data.
#else
				memcpy(writePtr, pcmout, ret);
#endif
				writePtr += ret;						// Increment write pointer.
			}
		}

		ov_clear(&vf);

		fclose(OggFile);

//		_ftime_s(&timedone);

//		elapsed = timedone.millitm - timestart.millitm;

//		debugf("%s decoded in %f.\n", strFilename, elapsed);
		
		return S_OK;
	}

void ToMono()
	{
		if(m_uChannels == 1)
			return;

		ZDebugOutput("ToMono() called on stereo sound file.\n");

		BYTE bytePerSample = m_uBitsPerSample / 8;
		unsigned int monoSize = m_uSize / m_uChannels;
		unsigned int numSamples = monoSize / bytePerSample;

		void* monoData = malloc(monoSize);

		BYTE* writePtr = (BYTE*)monoData;
		BYTE* readPtr = (BYTE*)m_pvData;

		for(unsigned int i = 0; i < numSamples; i++)
		{
			memcpy(writePtr, readPtr, bytePerSample);
			writePtr += bytePerSample;
			readPtr += bytePerSample * m_uChannels;
		}

		//The contents of the memory cuurently allocated are copied to monoData in the above for loop.  
		//Allocated m_pvData memory is now of no use since m_pvData pointer is changed below. (m_pvData = monoData;)
		
		//Sgt_Baker attempting to fix memory leak 15 Aug 2009
		if(m_bIsOgg)
		{
			//Oggs use manually allocated memory. Free it. Sgt_Baker.
			free(m_pvData); //Fix memory leak 8/3/09 Sgt_Baker
		}
		else
		{
			if (m_pvFileContents)
			{
				//Sgt_Baker 15 Aug 2009
				//Wavs use mapped files.  Since m_pvData is pointed elsewhere, we no longer need the mapping.
				//WILL THIS BREAK STREAMED FILES?
				// close the file
				UnmapViewOfFile(m_pvFileContents);
			}
		}

		m_uChannels = 1;
		m_uSize = monoSize;
		m_pvData = monoData;	// point the wave data to the new mono

		return;
	}

    // initialize the data object with the contents of strFilename
    HRESULT Init(const ZString& strFilename, bool convert) //AEM - Added convert parameter (7.4.07)
    {
        HRESULT hr;
            
        hr = OpenFile(strFilename);
        if (FAILED(hr)) return hr;

        hr = ParseWaveData(m_pvFileContents);
        if (FAILED(hr)) return hr;

		if (convert)
			ToMono();

        m_strFilename = strFilename;
		m_bIsOgg = false;
        m_mapOpenFiles.insert(std::pair<ZString, ISoundPCMData*>(m_strFilename, this));
        return S_OK;
    }

	// mdvalley: Initialize and convert OGG file, then process as normal
	HRESULT InitOgg(const ZString& strFilename, bool convert) //AEM - Added convert parameter (7.4.07)
	{
		HRESULT hr;

//		hr = OpenFile(strFilename);		// OggToWave opens it itself
//		if (FAILED(hr)) return hr;

		hr = DecodeOGG(strFilename);
		if (FAILED(hr)) return hr;

//		hr = ParseWaveData(m_pvPcmFromOgg);
//		if (FAILED(hr)) return hr;
		
		if (convert)
			ToMono();

		m_strFilename = strFilename;
		m_bIsOgg = true;
		m_mapOpenFiles.insert(std::pair<ZString, ISoundPCMData*>(m_strFilename, this));
		return S_OK;
	}

public:

    // returns a PCM data object for the specified file.
    static HRESULT LoadWaveFile(TRef<ISoundPCMData>& pdata, const ZString& strFilename, bool convertMono) //AEM - Added convertMono parameter (7.4.07)
    {
        // first, check the cache.
        SoundFileCache::iterator iterfile;

        iterfile = m_mapOpenFiles.find(strFilename);
        if (iterfile != m_mapOpenFiles.end())
        {
            pdata = (*iterfile).second;

            return S_OK;
        }
        else
        {
            // not found - let's try loading it.
            TRef<SoundFile> psoundfile = new SoundFile();

			HRESULT hr;

			if(strFilename.Right(4) == ".ogg")		// mdvalley: Find the file extension to decide whether to ogg it.
				hr = psoundfile->InitOgg(strFilename, convertMono ); //AEM - Added convertMono parameter (7.4.07)
			else
				hr = psoundfile->Init(strFilename, convertMono); //AEM - Added convertMono parameter (7.4.07)

            if (FAILED(hr)) return hr;
                        
            pdata = psoundfile;

            return S_OK;
        }
    }

    //
    // ISoundPCMData
    //

    // Gets the number of channels in the data
    unsigned GetNumberOfChannels()
    {
        ZAssert(m_pvData != nullptr);  // AKA this has been initialized
        return m_uChannels;
    };

    // Gets the number of bits per sample
    virtual unsigned GetBitsPerSample()
    {
        ZAssert(m_pvData != nullptr); // AKA this has been initialized
        return m_uBitsPerSample;
    };

    // Gets the default frequency (in samples per second) of the data
    virtual unsigned GetSampleRate()
    {
        ZAssert(m_pvData != nullptr); // AKA this has been initialized
        return m_uSampleRate;
    };

    // Gets the size of the data
    virtual unsigned GetSize()
    {
        ZAssert(m_pvData != nullptr); // AKA this has been initialized
        return m_uSize;
    };

    // Copies the specified portion of the data
    void GetData(void* dest, unsigned nOffset, unsigned nLength)
    {
        ZAssert(m_pvData != nullptr); // AKA this has been initialized
        ZAssert(nOffset + nLength <= m_uSize);
        memcpy(dest, ((BYTE*)m_pvData) + nOffset, nLength);
    };


    //
    // ISoundDebugDump
    //

#ifdef _DEBUG
    // return a human-readable description of the object, prepending
    // strIndent to the beginning of each line.  
    ZString DebugDump(const ZString& strIndent = "")
    {
        return strIndent + ZString("SoundFile \"") + m_strFilename 
            + "\" \n" + strIndent + "  ("  + ZString((int)GetSampleRate())
            + " Hz x " + ZString((int)GetBitsPerSample()) 
            + " bits x " + ZString((int)GetNumberOfChannels()) 
            + " channels, " + ZString(GetSize()/(float)GetBytesPerSec()) 
            + " seconds)\n";
    }
#endif
};

SoundFile::SoundFileCache SoundFile::m_mapOpenFiles;


// returns a PCM data object for a given wave file.
HRESULT LoadWaveFile(TRef<ISoundPCMData>& data, const ZString& strFilename, const bool convertMono) //AEM - Added convertMono parameter (7.4.07)
{
    return SoundFile::LoadWaveFile(data, strFilename, convertMono);
}


//////////////////////////////////////////////////////////////////////////////
//
// CreateDummyPCMData function
//
//////////////////////////////////////////////////////////////////////////////

class DummyPCMDataImpl : public ISoundPCMData
{
    enum { datasize = 44100 };

    BYTE m_data[datasize];

public:

    // Gets the number of channels in the data
    virtual unsigned GetNumberOfChannels()
    {
        return 2;
    };

    // Gets the number of bits per sample
    virtual unsigned GetBitsPerSample()
    {
        return 16;
    };

    // Gets the default frequency (in samples per second) of the data
    virtual unsigned GetSampleRate()
    {
        return 44100;
    };

    // Gets the size of the data
    virtual unsigned GetSize()
    {
        return datasize;
    };

    // Copies the specified portion of the data
    virtual void GetData(void* dest, unsigned nOffset, unsigned nLength)
    {
        ZAssert(nOffset + nLength <= datasize);
        memset(dest, 0, nLength);
    };
};

TRef<ISoundPCMData> psoundDataDummy;

HRESULT CreateDummyPCMData(TRef<ISoundPCMData>& pdata)
{
    if (psoundDataDummy == nullptr)
        psoundDataDummy = new DummyPCMDataImpl();

    pdata = psoundDataDummy;

    return S_OK;
}

//////////////////////////////////////////////////////////////////////////////
//
// GenericSoundSource
//
//////////////////////////////////////////////////////////////////////////////

GenericSoundSource::GenericSoundSource() :
    m_vectPosition(0, 0, 0),
    m_vectVelocity(0, 0, 0),
    m_vectOrientation(0, 0, 1),
    m_bIsPlaying(true)
{
}


GenericSoundSource::GenericSoundSource(
        const Vector& vectPosition, 
        const Vector& vectVelocity, 
        const Vector& vectOrientation,
        bool          bListenerRelative
        ) :
    m_vectPosition(vectPosition),
    m_vectVelocity(vectVelocity),
    m_vectOrientation(vectOrientation),
    m_bListenerRelative(bListenerRelative),
    m_bIsPlaying(true)
{
}


// Sets the position of the sound in space
void GenericSoundSource::SetPosition(const Vector& vectPosition)
{
    m_vectPosition = vectPosition;
}


// Sets the velocity of the sound in space
void GenericSoundSource::SetVelocity(const Vector& vectVelocity)
{
    m_vectVelocity = vectVelocity;
}


// Sets the orientation of the sound in space, used for sound cones.
void GenericSoundSource::SetOrientation(const Vector& vectOrientation)
{
    // make sure the vector is basically a unit vector
    ZAssert(vectOrientation.Length() > 0.98 && vectOrientation.Length() < 1.02);

    m_vectOrientation = vectOrientation;
}


// Sets the orientation of the sound in space, used for sound cones.
void GenericSoundSource::SetIsListenerRelative(bool bListenerRelative)
{
    m_bListenerRelative = bListenerRelative;
}


// Stops any sounds on this source
void GenericSoundSource::Stop()
{
    m_bIsPlaying = false;
}


//
// Position retrieval functions
//

// Gets the position of the sound in space
HRESULT GenericSoundSource::GetPosition(Vector& vectPosition)
{
    vectPosition = m_vectPosition;

    return S_OK;
};


// Gets the velocity of the sound in space
HRESULT GenericSoundSource::GetVelocity(Vector& vectVelocity)
{
    vectVelocity = m_vectVelocity;

    return S_OK;
};


// Gets the orientation of the sound in space, used for sound cones.
HRESULT GenericSoundSource::GetOrientation(Vector& vectOrientation)
{
    vectOrientation = m_vectOrientation;

    return S_OK;
};


// Returns S_OK if the position, velocity and orientation reported are 
// relative to the listener, S_FALSE otherwise.  
HRESULT GenericSoundSource::IsListenerRelative()
{
    return m_bListenerRelative ? S_OK : S_FALSE;
};

// Returns S_OK if this source is still playing the sound, S_FALSE 
// otherwise.  This might be false if a sound emitter is destroyed, for 
// example, in which case the sound might fade out.  Once it returns
// S_FALSE once, it should never return S_OK again.
HRESULT GenericSoundSource::IsPlaying() 
{
    return m_bIsPlaying ? S_OK : S_FALSE;
};


//////////////////////////////////////////////////////////////////////////////
//
// SoundPositionSourceWrapper
//
//////////////////////////////////////////////////////////////////////////////

// Gets the position of the sound in space
HRESULT SoundPositionSourceWrapper::GetPosition(Vector& vectPosition)
{
    return m_pBase->GetPosition(vectPosition);
};

// Gets the velocity of the sound in space
HRESULT SoundPositionSourceWrapper::GetVelocity(Vector& vectVelocity)
{
    return m_pBase->GetVelocity(vectVelocity);
};

// Gets the orientation of the sound in space, used for sound cones.
HRESULT SoundPositionSourceWrapper::GetOrientation(Vector& vectOrientation)
{
    return m_pBase->GetOrientation(vectOrientation);
};

// Returns S_OK if this source is still playing the sound, S_FALSE 
// otherwise.  This might be false if a sound emitter is destroyed, for 
// example, in which case the sound might fade out.  Once it returns
// S_FALSE once, it should never return S_OK again.
HRESULT SoundPositionSourceWrapper::IsPlaying()
{
    return m_pBase->IsPlaying();
};


//////////////////////////////////////////////////////////////////////////////
//
// GenericListener
//
//////////////////////////////////////////////////////////////////////////////


// constructor
GenericListener::GenericListener() :
    m_vectUp(0, 1, 0)
{
};

// sets the up direction for this listener
void GenericListener::SetUpDirection(const Vector& vectUp)
{    
    m_vectUp = vectUp;
};


//
// ISoundListener
//

// get the "up" vector for the listener
HRESULT GenericListener::GetUpDirection(Vector& vectUp)
{
    vectUp = m_vectUp;
    return S_OK;
};


//
// ISoundPositionSource
//

// Gets the position of the sound in space
HRESULT GenericListener::GetPosition(Vector& vectPosition)
{
    return GenericSoundSource::GetPosition(vectPosition);
};

// Gets the velocity of the sound in space
HRESULT GenericListener::GetVelocity(Vector& vectVelocity)
{
    return GenericSoundSource::GetVelocity(vectVelocity);
};

// Gets the orientation of the sound in space, used for sound cones.
HRESULT GenericListener::GetOrientation(Vector& vectOrientation)
{
    return GenericSoundSource::GetOrientation(vectOrientation);
};

// Returns S_OK if this source is still playing the sound, S_FALSE 
// otherwise.  This might be false if a sound emitter is destroyed, for 
// example, in which case the sound might fade out.  Once it returns
// S_FALSE once, it should never return S_OK again.
HRESULT GenericListener::IsPlaying()
{
    return GenericSoundSource::IsPlaying();
};


//////////////////////////////////////////////////////////////////////////////
//
// SoundInstanceWrapper
//
//////////////////////////////////////////////////////////////////////////////

// Stops the sound.  If bForceNow is true the sound will stop ASAP, 
// possibly popping.  If it is false some sounds may play a trail-off 
// sound or fade away.  
HRESULT SoundInstanceWrapper::Stop(bool bForceNow)
{
    return m_pBase->Stop(bForceNow);
};

// returns S_OK if the sound is currently playing, S_FALSE otherwise.
HRESULT SoundInstanceWrapper::IsPlaying()
{
    return m_pBase->IsPlaying();
};

// Gets an event which fires when the sound finishes playing (for any 
// reason)
IEventSource* SoundInstanceWrapper::GetFinishEventSource()
{
    return m_pBase->GetFinishEventSource();
};

// Gets an interface for tweaking the sound, if supported, nullptr otherwise.
TRef<ISoundTweakable> SoundInstanceWrapper::GetISoundTweakable()
{
    return m_pBase->GetISoundTweakable();
};

TRef<ISoundTweakable3D> SoundInstanceWrapper::GetISoundTweakable3D()
{
    return m_pBase->GetISoundTweakable3D();
};


//////////////////////////////////////////////////////////////////////////////
//
// SoundTweakableWrapper
//
//////////////////////////////////////////////////////////////////////////////

// Sets the gain, from 0 to -100 dB
HRESULT SoundTweakableWrapper::SetGain(float fGain)
{
    return m_pBase->SetGain(fGain);
};

// Sets the pitch shift, where 1.0 is normal, 0.5 is half of normal speed, 
// and 2.0 is twice normal speed.  
HRESULT SoundTweakableWrapper::SetPitch(float fPitch)
{
    return m_pBase->SetPitch(fPitch);
};

// sets the priority - used as a addition to volume when choosing which 
// sounds are most important to play.
HRESULT SoundTweakableWrapper::SetPriority(float fPriority)
{
    return m_pBase->SetPriority(fPriority);
};



//////////////////////////////////////////////////////////////////////////////
//
// SoundTweakable3DWrapper
//
//////////////////////////////////////////////////////////////////////////////


// toggles 3D Positioning on and off for the given sound.
HRESULT SoundTweakable3DWrapper::Set3D(bool b3D)
{
    return m_pBase->Set3D(b3D);
};


// Sets the distance at which the sound will be at max volume.  This
// effects how quickly the sound drops off with distance.  
HRESULT SoundTweakable3DWrapper::SetMinimumDistance(float fMinimumDistance)
{
    return m_pBase->SetMinimumDistance(fMinimumDistance);
};


// Sets a sound cone of size fInnerAngle (in degrees) where the volume is at 
// normal levels, outside of which it fades down by fOutsideGain 
// (range of 0 to -100 db) at fOuterAngle (degrees) and beyond.  
HRESULT SoundTweakable3DWrapper::SetCone(float fInnerAngle, float fOuterAngle, float fOutsideGain)
{
    return m_pBase->SetCone(fInnerAngle, fOuterAngle, fOutsideGain);
};


//////////////////////////////////////////////////////////////////////////////
//
// StubbedTweakableSoundInstance
//
//////////////////////////////////////////////////////////////////////////////

StubbedTweakableSoundInstance::StubbedTweakableSoundInstance(ISoundPositionSource* pposSource) :
    m_bPrioritySet(false),
    m_bGainSet(false),
    m_bPitchSet(false),
    m_pposSource(pposSource),
    m_b3DSet(false),
    m_bMinimumDistanceSet(false),
    m_bConeSet(false),
    m_bStopped(false),
    m_bForcedStop(false)
{
    m_peventsourceStopped = new EventSourceImpl();
}


// fires the m_peventsourceStopped event if bHasStopped is true
void StubbedTweakableSoundInstance::Update(DWORD dwElapsedTime, bool bHasStopped)
{
    if (bHasStopped)
    {
        m_peventsourceStopped->Trigger();
    }
}


// calls Stop(true) if m_pposSource has stopped. calls Update, 
// returns IsPlaying()
bool StubbedTweakableSoundInstance::OnEvent(IIntegerEventSource* pevent, int value)
{
    if (m_pposSource && (m_pposSource->IsPlaying() != S_OK))
    {
        Stop(true);
    }

    bool bIsPlaying = IsPlaying() == S_OK; 

    Update(value, !bIsPlaying);

    return bIsPlaying;
}

// copies any tweaks that have been made to the given sound instance
void StubbedTweakableSoundInstance::UpdateSound(ISoundInstance* psound)
{
    TRef<ISoundTweakable> ptweak = psound->GetISoundTweakable();
    ZAssert(ptweak);

    if (m_bGainSet)
        ZSucceeded(ptweak->SetGain(m_fGain));
    if (m_bPitchSet)
        ZSucceeded(ptweak->SetPitch(m_fPitch));
    if (m_bPrioritySet)
        ZSucceeded(ptweak->SetPriority(m_fPriority));

    if (m_pposSource)
    {
        TRef<ISoundTweakable3D> ptweak3d = 
            psound->GetISoundTweakable3D();
        ZAssert(ptweak3d);

        if (m_b3DSet)
            ZSucceeded(ptweak3d->Set3D(m_b3D));
        if (m_bConeSet)
            ZSucceeded(ptweak3d->SetCone(m_fInnerAngle, m_fOuterAngle,
                m_fOutsideGain));
        if (m_bMinimumDistanceSet)
            ZSucceeded(ptweak3d->SetMinimumDistance(m_fMinimumDistance));
    }
}


//
// ISoundInstance interface
//

// Stops the sound.  If bForceNow is true the sound will stop ASAP, 
// possibly popping.  If it is false some sounds may play a trail-off 
// sound or fade away.  
HRESULT StubbedTweakableSoundInstance::Stop(bool bForceNow)
{
    m_bStopped = true;
    m_bForcedStop = m_bForcedStop || bForceNow;

    return S_OK;
};


// returns S_OK if the sound is currently playing, S_FALSE otherwise.
HRESULT StubbedTweakableSoundInstance::IsPlaying()
{
    return m_bStopped;
};


// Gets an event which fires when the sound finishes playing (for any 
// reason)
IEventSource* StubbedTweakableSoundInstance::GetFinishEventSource()
{
    return m_peventsourceStopped;
};


// Gets an interface for tweaking the sound, if supported, nullptr otherwise.
TRef<ISoundTweakable> StubbedTweakableSoundInstance::GetISoundTweakable()
{
    return this;
};


TRef<ISoundTweakable3D> StubbedTweakableSoundInstance::GetISoundTweakable3D()
{
    if (m_pposSource)
        return this;
    else
        return nullptr;
};


// Sets the gain, from 0 to -100 dB
HRESULT StubbedTweakableSoundInstance::SetGain(float fGain)
{
    if (fGain > 0)
    {
        ZAssert(false);
        return E_INVALIDARG;
    }

    m_fGain = std::max(fGain, -100.0f);
    m_bGainSet = true;

    return S_OK;
};


// Sets the pitch shift, where 1.0 is normal, 0.5 is half of normal speed, 
// and 2.0 is twice normal speed.  
HRESULT StubbedTweakableSoundInstance::SetPitch(float fPitch)
{
    m_fPitch = fPitch;
    m_bPitchSet = true;

    return S_OK;
};


// sets the priority - used as a addition to volume when choosing which 
// sounds are most important to play.
HRESULT StubbedTweakableSoundInstance::SetPriority(float fPriority) 
{
    m_fPriority = fPriority;
    m_bPrioritySet = true;
    return S_OK;
};


// toggles 3D Positioning on and off for the given sound.
HRESULT StubbedTweakableSoundInstance::Set3D(bool b3D)
{
    m_b3D = b3D;
    m_b3DSet = true;

    return S_OK;
};


// Sets the distance at which the sound will be at max volume.  This
// effects how quickly the sound drops off with distance.  
HRESULT StubbedTweakableSoundInstance::SetMinimumDistance(float fMinimumDistance)
{
    // Any distance greater than 0 is OK.
    if (fMinimumDistance <= 0)
    {
        ZAssert(false);
        return E_INVALIDARG;
    }

    m_fMinimumDistance = fMinimumDistance;
    m_bMinimumDistanceSet = true;
    
    return S_OK;
};


// Sets a sound cone of size fInnerAngle (in degrees) where the volume is at 
// normal levels, outside of which it fades down by fOutsideGain 
// (range of 0 to -100 db) at fOuterAngle (degrees) and beyond.  
HRESULT StubbedTweakableSoundInstance::SetCone(float fInnerAngle, float fOuterAngle, float fOutsideGain)
{
    // check the parameters
    if ((fInnerAngle < 0 || fInnerAngle > 360)
        || (fOuterAngle < 0 || fOuterAngle > 360)
        || (fOuterAngle < fInnerAngle)
        || (fOutsideGain < -100 || fOutsideGain > 0)
        )
    {
        ZAssert(false);
        return E_INVALIDARG;
    };

    m_fInnerAngle = fInnerAngle;
    m_fOuterAngle = fOuterAngle;
    m_fOutsideGain = fOutsideGain;
    m_bConeSet = true;

    return S_OK;
};


//////////////////////////////////////////////////////////////////////////////
//
// SoundBufferSourceDelegate
//
//////////////////////////////////////////////////////////////////////////////


// Creates a static sound buffer of the given wave file.  If bLooping is 
// true, the sound will loop until stopped.
HRESULT SoundBufferSourceDelegate::CreateStaticBuffer(TRef<ISoundInstance>& psoundNew, 
    ISoundPCMData* pcmdata, bool bLooping, ISoundPositionSource* psource)
{
    return m_pBase->CreateStaticBuffer(psoundNew, pcmdata, bLooping, psource);
};


// Creates a sound buffer with a loop in the middle.  The sound will play
// the start sound, play the loop sound until it gets a soft stop, then
// play the rest of the sound.  
HRESULT SoundBufferSourceDelegate::CreateASRBuffer(TRef<ISoundInstance>& psoundNew, 
    ISoundPCMData* pcmdata, unsigned uLoopStart, unsigned uLoopLength, 
    ISoundPositionSource* psource)
{
    return m_pBase->CreateASRBuffer(psoundNew, pcmdata, uLoopStart, uLoopLength, psource);
};


// Gets an event which fires each time update is called.  This can be used
// for some of the trickier sounds that change with time.
IIntegerEventSource* SoundBufferSourceDelegate::GetUpdateEventSource()
{
    return m_pBase->GetUpdateEventSource();
};


};
