#pragma once
//
// ds3dbuffer.h: low-level classes representing actual DirectSound3D buffers
//

#include "ds3dutil.h"
#include "soundbase.h"

namespace SoundEngine {

// the minimum audible sound we care about (in dB)
const float c_fMinAudible = -60.0f;

// The conversion ratio from min distance to max distance. 
const float c_fMinToMaxDistanceRatio = 4000.0f;


//
// A single DirectSound3D sound buffer.  This wrapper handles the painful tasks
// of dealing with buffer loss, etc..
//
class DS3DSoundBuffer : public ISoundObject  SoundDebugImpl
{
private:

    // the default sample rate for this sound (when pitch is set to 1)
    DWORD m_dwSampleRate;


    //
    // cached settings
    //

    // The current gain of the sound
    float m_fGain;

    // the current pitch of the sound
    float m_fPitch;

    // is 3D on at the moment?
    bool m_b3D;

    // are the positions and velocity listener-relative?
    bool m_bListenerRelative;

    // the current maximum distance of the sound
    float m_fMinimumDistance;

    // the current sound cone
    float m_fInnerAngle, m_fOuterAngle, m_fOutsideGain;

    // the current position of the sound
    Vector m_vectPosition; 

    // the current velocity of the sound
    Vector m_vectVelocity;

    // the current orientation of the sound
    Vector m_vectOrientation;

    
    // convert a gain in the range -100 to 0 dB to a direct sound volume
    inline static LONG DSoundVolume(float fGain);

protected:

    // The current sound buffer
	// mdvalley: Now with DirectSound8
    TRef<IDirectSoundBuffer8> m_pdirectsoundbuffer8;
	TRef<IDirectSoundBuffer> m_pdirectsoundbuffer;

	TRef<IDirectSound3DBuffer> m_pdirectsound3Dbuffer;

    // initializes the object, creating the DSoundBuffer itself and 
    // initializing local variables.
    virtual HRESULT CreateBuffer8(IDirectSound8* pDirectSound, ISoundPCMData* pdata,
        DWORD dwBufferSize, bool bStatic, bool bSupport3D, ISoundEngine::Quality quality, 
        bool bAllowHardware);

	virtual HRESULT CreateBuffer(IDirectSound* pDirectSound, ISoundPCMData* pdata,
		DWORD dwBufferSize, bool bStatic, bool bSupport3D, ISoundEngine::Quality quality,
		bool bAllowHardware);

    // Use an exisiting to initialize this buffer.  Note that the buffers will 
    // share memory, so this only really works for static buffers.
    virtual HRESULT DuplicateBuffer8(IDirectSound8* pDirectSound, DS3DSoundBuffer* pBuffer);
	virtual HRESULT DuplicateBuffer(IDirectSound* pDirectSound, DS3DSoundBuffer* pBuffer);

    // Start the buffer, starting as a looping buffer if requested.
    HRESULT StartImpl(bool bLooping);

    // restore the contents of the buffer after a buffer loss
    virtual HRESULT RestoreBuffer() = 0;

public:

    DS3DSoundBuffer();

    ~DS3DSoundBuffer();

    // Sets the volume and pitch iff the new values differ from the old.
    HRESULT UpdateState(float fGain, float fPitch, bool bCanDefer = true);

    // Sets the 3D info iff the new values differ from the old.
    HRESULT UpdateState3D(
        const Vector& vectPosition, 
        const Vector& vectVelocity, 
        const Vector& vectOrientation,
        float fMinimumDistance,
        float fInnerAngle,
        float fOuterAngle,
        float fOutsideGain,
        bool b3D,
        bool bListenerRelative,
        bool bCanDefer = true
        );

    // Starts the given buffer playing at the given position.
    virtual HRESULT Start(DWORD dwPosition, bool bIsStopping = false) = 0;

    // stops the given buffer.
    virtual HRESULT Stop(bool bForceNow = false) = 0;

    // gets the current status of the buffer
    HRESULT GetStatus(bool& bPlaying, bool& bBufferLost);

    // Gets the current position of the sound
    virtual HRESULT GetPosition(DWORD& dwPosition) = 0;


    //
    // ISoundDebugDump
    //

#ifdef _DEBUG
    // return a human-readable description of the object, prepending
    // strIndent to the beginning of each line.  
    ZString DebugDump(const ZString& strIndent = "");
#endif
};





//
// A specialization of DS3DSoundBuffer for static buffers
//
class DS3DStaticSoundBuffer : public DS3DSoundBuffer
{
    // true if the buffer has been played at least once
    bool m_bHasBeenPlayed;

    // true iff this sound is a looping sound.  
    bool m_bLooping;

    // The data source used to create this sound
    TRef<ISoundPCMData> m_pdata;

    // restore the contents of the buffer after a buffer loss
    virtual HRESULT RestoreBuffer();

    // fills the buffer with data from the data source
    virtual HRESULT LoadData();

    // a cache of currently playing static sound buffers
    class CacheKey
    {
        bool m_bSupport3D;
        ISoundEngine::Quality m_quality;
        bool m_bAllowHardware;
        ISoundPCMData* m_pdata;
    public:
        CacheKey(ISoundPCMData* pdata, bool bSupport3D, ISoundEngine::Quality quality, bool bAllowHardware)
            : m_pdata(pdata), m_bSupport3D(bSupport3D), m_quality(quality), m_bAllowHardware(bAllowHardware) {};

        bool operator < (const CacheKey& key) const
        {
            return (m_pdata < key.m_pdata) || (m_pdata == key.m_pdata) 
                && ((!m_bSupport3D && key.m_bSupport3D) || (m_bSupport3D == key.m_bSupport3D)
                    && ((m_quality < key.m_quality) || (m_quality == key.m_quality)
                        && (!m_bAllowHardware && key.m_bAllowHardware)));
        }
    };

    typedef std::multimap<CacheKey, DS3DStaticSoundBuffer*> BufferCache;
    static BufferCache bufferCache;
    BufferCache::iterator m_iterSelf;

public:

    // Constructor
    DS3DStaticSoundBuffer();

    // Destructor
    ~DS3DStaticSoundBuffer();

    // Initializes this object with the given wave data, 3D support, and sound 
    // quality.
	// mdvalley: more DirectSound8 portage.
    HRESULT Init8(IDirectSound8* pDirectSound, ISoundPCMData* pdata, 
        bool bLooping, bool bSupport3D, ISoundEngine::Quality quality,
        bool bAllowHardware
        );

	HRESULT Init(IDirectSound* pDirectSound, ISoundPCMData* pdata,
		bool bLooping, bool bSupport3D, ISoundEngine::Quality quality,
		bool bAllowHardware
		);

    // starts the given buffer playing at the given position.  
    virtual HRESULT Start(DWORD dwPosition, bool bIsStopping = false);

    // stops the given buffer.
    virtual HRESULT Stop(bool bForceNow = false);

    // Gets the current position of the sound
    virtual HRESULT GetPosition(DWORD& dwPosition);
};





//
// A specialization of DS3DSoundBuffer for streaming buffers
//
class DS3DStreamingSoundBuffer : public DS3DSoundBuffer, protected TaskListThread::Task
{
protected:
    // true if the buffer has been played at least once
    bool m_bHasBeenPlayed;

    // true iff this sound is a looping sound.  
    bool m_bLooping;

    // The data source used to create this sound
    TRef<ISoundPCMData> m_pdata;

    // the next source offset to write
    DWORD m_dwSourceOffset;

    // the next buffer offset at which to write
    DWORD m_dwWriteOffset;

    // the last buffer offset at which we saw the read pointer
    DWORD m_dwLastReadOffset;

    // the size of the streaming buffer.
    DWORD m_dwBufferSize;

    // an offset which, if the read pointer crosses, we know that we have 
    // finished playing the sound and are now playing silence.
    DWORD m_dwStopOffset;

    // true iff we are playing silence at this point.
    bool m_bPlayingSilence;

    // The update thread which handles filling the buffers as they play
    static TaskListThread m_threadUpdate;

    // determines if a buffer cursor starting at dwStart and ending at dwEnd
    // would have crossed event dwTrigger.
    inline bool CrossedBoundary(DWORD dwTrigger, DWORD dwStart, DWORD dwEnd);

    // updates anything waiting for the play/read pointer to cross a certain 
    // point in the buffer.
    virtual void ReadPointerUpdate(DWORD dwReadOffset);

    // gets the last played source offset from the last played buffer offset.
    virtual DWORD GetPlayedSourceOffset(DWORD dwLastPlayedPosition);

    // restore the contents of the buffer after a buffer loss
    virtual HRESULT RestoreBuffer();


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
    virtual HRESULT UpdateBufferContents(bool bTrustWritePtr = false);

    // streams the given length of data out to the buffer pointed to by pvBuffer
    virtual HRESULT StreamData(void *pvBuffer, DWORD dwLength);

    // Initializes this object with the given wave data, 3D support, sound 
    // quality, and buffer length (in seconds)
	// mdvalley: DSound8 here.
    HRESULT Init8(IDirectSound8* pDirectSound, ISoundPCMData* pdata, 
        bool bLooping, bool bSupport3D, ISoundEngine::Quality quality,
        bool bAllowHardware, float fBufferLength
        );

	HRESULT Init(IDirectSound* pDirectSound, ISoundPCMData* pdata,
		bool bLooping, bool bSupport3D, ISoundEngine::Quality quality,
		bool bAllowHardware, float fBufferLength
		);

    //
    // TaskListThread::Execute interface
    //

    // Updates the contents of the streaming buffer.
    virtual bool Execute();

public:

    // Constructor
    DS3DStreamingSoundBuffer();

    // Destructor
    ~DS3DStreamingSoundBuffer();

    // Initializes this object with the given wave data, 3D support, and sound 
    // quality.
	// mdvalley: DirectSound8 makes md happy.
    HRESULT Init8(IDirectSound8* pDirectSound, ISoundPCMData* pdata, 
        bool bLooping, bool bSupport3D, ISoundEngine::Quality quality,
        bool bAllowHardware
        );

	HRESULT Init(IDirectSound* pDirectSound, ISoundPCMData* pdata,
		bool bLooping, bool bSupport3D, ISoundEngine::Quality quality,
		bool bAllowHardware
		);

    // starts the given buffer playing at the given position.  
    virtual HRESULT Start(DWORD dwPosition, bool bIsStopping = false);

    // stops the given buffer.
    virtual HRESULT Stop(bool bForceNow = false);

    // Gets the current position of the sound
    virtual HRESULT GetPosition(DWORD& dwPosition);

    // gets the current status of the buffer
    HRESULT GetStatus(bool& bPlaying, bool& bBufferLost);
};





//
// A specialization of DS3DSoundBuffer for ASR buffers
//
class DS3DASRSoundBuffer : public DS3DStreamingSoundBuffer
{
private:

    // the start of the sustain loop in the data source
    DWORD m_dwLoopOffset;

    // the length of the sustain loop in the data source
    DWORD m_dwLoopLength;

    // an offset which, if the read pointer crosses, we know that we have 
    // finished playing the attack part of the sound.
    DWORD m_dwSustainBufferOffset;

    // true if we are playing/have played the sustain portion of the sound
    bool m_bPlayingSustain;

    // an offset which, if the read pointer crosses, we know that we have 
    // finished playing the sustain part of the sound and are now playing
    // the release part of the sound.
    DWORD m_dwReleaseBufferOffset;

    // true if we are playing/have played the release portion of the sound
    bool m_bPlayingRelease;

    // true if we are trying to transition to the release portion
    bool m_bEnding;

    // updates anything waiting for the play/read pointer to cross a certain 
    // point in the buffer.
    virtual void ReadPointerUpdate(DWORD dwReadOffset);

    // gets the last played source offset from the last played buffer offset.
    virtual DWORD GetPlayedSourceOffset(DWORD dwLastPlayedPosition);

    // streams the given length of data out to the buffer pointed to by pvBuffer
    virtual HRESULT StreamData(void *pvBuffer, DWORD dwLength);

public:

    // Destructor
    ~DS3DASRSoundBuffer();

    // Initializes this object with the given wave data, 3D support, and sound 
    // quality.
	// mdvalley: DirectSound8
    HRESULT Init8(IDirectSound8* pDirectSound, ISoundPCMData* pdata, 
        DWORD dwLoopOffset, DWORD dwLoopLength, bool bSupport3D, 
        ISoundEngine::Quality quality , bool bAllowHardware
        );

	HRESULT Init(IDirectSound* pDirectSound, ISoundPCMData* pdata,
		DWORD dwLoopOffset, DWORD dwLoopLength, bool bSupport3D,
		ISoundEngine::Quality quality, bool bAllowHardware
		);

    // starts the given buffer playing at the given position.  
    virtual HRESULT Start(DWORD dwPosition, bool bIsStopping = false);

    // stops the given buffer.
    virtual HRESULT Stop(bool bForceNow = false);
};

};
