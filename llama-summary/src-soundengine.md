# src/soundengine

[Back to the index](../LLAMA_SUMMARY.md) - 17 files, 9,117 lines. Notes by a local model, to orient by: not verified facts.

The `src/soundengine` folder in Allegiance contains essential components for managing audio playback and 3D sound positioning within the game. It includes classes and functions to handle sound buffers, engine management, utility tasks, virtual buffers, and template-based sound effects.

- ds3dbuffer.cpp: Manages sound buffers using DirectSound3D for playback.
- ds3dengine.cpp: Implements a sound engine using DirectSound3D for 3D audio rendering.
- ds3dvirtualbuffer.cpp: Handles virtual sound buffers for both 2D and 3D audio effects.
- soundtemplates.cpp: Manages and manipulates sound templates for various audio effects.
- soundutil.cpp: Loads, parses, caches wave files, and manages basic sound source and listener properties.

Largest files: soundtemplates.cpp (2101 lines), ds3dbuffer.cpp (1735 lines), soundutil.cpp (1131 lines), ds3dvirtualbuffer.cpp (916 lines), ds3dengine.cpp (732 lines).

## `src/soundengine/CMakeLists.txt`

PURPOSE: tiny file (14 lines), shown in full rather than summarized.

    ADD_DEFINITIONS(-D__MODULE__="SoundEngine")
    INCLUDE_DIRECTORIES(../Inc ../zlib)
    SET(SoundEngine_SRC
        ds3dbuffer.cpp
        ds3dengine.cpp
        ds3dutil.cpp
        ds3dvirtualbuffer.cpp
        dummysound.cpp
        soundbase.cpp
        soundtemplates.cpp
        soundutil.cpp
       )
    ADD_LIBRARY(SoundEngine ${SoundEngine_SRC})
    SET_PROPERTY(TARGET SoundEngine PROPERTY CXX_STANDARD 14)

## `src/soundengine/ds3dbuffer.cpp`

PURPOSE: Manages sound buffers for playback using DirectSound3D in Allegiance.

KEY PARTS:
- DS3DSoundBuffer: Represents a DirectSound3D buffer and handles basic sound operations.
- DSoundVolume: Converts gain in dB to DirectSound volume levels.
- CreateBuffer8 and CreateBuffer: Initialize sound buffers with DirectSound8 or DirectSound interfaces.
- DuplicateBuffer8 and DuplicateBuffer: Duplicate existing buffers, sharing memory for static buffers.
- DSBCAPS_CTRL3D and other flags: Define buffer capabilities like 3D control and frequency control.
- IDirectSound8 and IDirectSound: Interfaces for managing sound buffers in DirectX 8.
- LPDIRECTSOUNDBUFFER and LPDIRECTSOUND3DBUFFER: Pointers to DirectSound buffer and 3D buffer interfaces.
- ZAssert and ZFailed: Macros for assertions and error checking.

CONNECTS TO: Allegiance's audio system, dependent on DirectX 8 libraries.

NOTES: None.

PARTS (in file order):

- lines 1-368
  - DS3DSoundBuffer: Represents a DirectSound3D buffer.
  - DSoundVolume: Converts gain in dB to DirectSound volume.
  - CreateBuffer8 and CreateBuffer: Initialize buffers with DirectSound8 or DirectSound interfaces.
  - DuplicateBuffer8 and DuplicateBuffer: Duplicate existing buffers, sharing memory for static buffers.
  - DSBCAPS_CTRL3D and other flags: Define buffer capabilities like 3D control, frequency control, etc.
  - IDirectSound8 and IDirectSound: Interfaces for managing sound buffers in DirectX 8.
  - LPDIRECTSOUNDBUFFER and LPDIRECTSOUND3DBUFFER: Pointers to DirectSound buffer and 3D buffer interfaces.
  - ZAssert and ZFailed: Macros for assertions and error checking.
- lines 370-717
  - DS3DSoundBuffer: Manages a DirectSound buffer for sound playback.
  - StartImpl: Starts playing the sound buffer, optionally looping.
  - UpdateState: Updates the volume and pitch of the sound buffer if they have changed.
  - UpdateState3D: Updates 3D-specific properties of the sound buffer if they have changed.
  - GetStatus: Retrieves the current status of the sound buffer (playing, buffer lost).
  - DS3DStaticSoundBuffer: Manages a static sound buffer that can be restored after loss.
  - RestoreBuffer: Restores and reloads the contents of the sound buffer after a buffer loss.
  - LoadData: Locks the sound buffer and fills it with data from the data source.
- lines 718-1075
  - DS3DStaticSoundBuffer: Manages static sound buffers.
  - DS3DStreamingSoundBuffer: Manages streaming sound buffers.
  - CacheKey: Used to cache sound buffer instances.
  - BufferCache: Stores cached sound buffer instances.
  - Init8 and Init: Initializes sound buffers with various parameters.
  - Start: Starts playing a sound buffer at a specified position.
  - Stop: Stops the sound buffer.
  - GetPosition: Retrieves the current position of the sound buffer.
- lines 1076-1441
  - DS3DStreamingSoundBuffer: Manages a streaming sound buffer for playback.
  - StreamData: Streams data into the sound buffer.
  - Init: Initializes the sound buffer with wave data and settings.
  - Start: Starts playing the sound buffer from a specified position.
  - Stop: Stops playing the sound buffer.
  - GetPosition: Retrieves the current position of the sound.
  - GetStatus: Checks if the buffer is playing or has been lost.
  - DS3DASRSoundBuffer: Manages an ASR (Attack-Sustain-Release) sound buffer for playback.
- lines 1442-1735
  - DS3DASRSoundBuffer::StreamData: Streams data out to a buffer pointed to by pvBuffer.
  - DS3DASRSoundBuffer::Init: Initializes this object with wave data, 3D support, and sound quality.
  - DS3DASRSoundBuffer::Start: Starts the given buffer playing at the given position.
  - DS3DASRSoundBuffer::Stop: Stops the given buffer.
  - m_dwLoopOffset: Offset where the loop starts in the sound data.
  - m_dwLoopLength: Length of the loop in the sound data.
  - m_bPlayingSustain: Indicates if the buffer is currently playing the sustain part of the sound.
  - m_bPlayingRelease: Indicates if the buffer is currently playing the release part of the sound.

## `src/soundengine/ds3dbuffer.h`

PURPOSE: This file contains definitions and implementations for various sound buffer classes in the Allegiance game's sound engine, utilizing DirectSound3D for audio playback.

KEY PARTS:
- DS3DSoundBuffer: Base class for managing DirectSound3D buffers, including methods to update state, start/stop, and handle buffer restoration.
- DS3DStaticSoundBuffer: Specialization of DS3DSoundBuffer for static sound data, caching instances for reuse.
- DS3DStreamingSoundBuffer: Specialization for streaming audio data, with a background thread to fill buffers as they play.
- DS3DASRSoundBuffer: Further specialization for ASR (Attack-Sustain-Release) sounds, managing different phases of playback.

CONNECTS TO: ds3dutil.h, soundbase.h, IDirectSound8, IDirectSoundBuffer, and other sound-related classes and interfaces.

NOTES: The file includes support for both DirectSound and DirectSound8, with some conditional compilation to handle the transition. There are TODOs related to buffer management optimizations and potential improvements in error handling.

## `src/soundengine/ds3dengine.cpp`

**PURPOSE:** The `ds3dengine.cpp` file is part of the Allegiance game's sound engine, specifically managing DirectSound3D for audio playback and 3D positioning.

**KEY PARTS:**
- **Class: DefaultListener**: Provides basic functionality for a listener's position, velocity, orientation, and up direction.
- **Function: DS3DSoundEngine::DumpCaps()**: Dumps the capabilities of the DirectSound driver to the debug output.
- **Function: DS3DSoundEngine::SetPrimaryBufferFormat(int nSampleRate, int nNumberOfBits, int nChannels)**: Sets the format of the primary DirectSound buffer.
- **Function: DS3DSoundEngine::UpdateListener()**: Updates the listener's position, velocity, orientation, and up direction in the DirectSound environment.
- **Function: DS3DSoundEngine::IsSeriousError(HRESULT hr)**: Determines if a given HRESULT indicates a serious error.
- **Class: DS3DSoundEngine**: Manages the DirectSound environment, including initialization, updating, and managing sound buffers. Handles listener updates and quality settings.
- **Function: EnableHardware(bool bEnable)**: Enables or disables hardware acceleration for sound processing.
- **Function: SetListener(ISoundListener* plistener)**: Sets the listener object defining the position and orientation of the sound source.
- **Function: SetDistanceFactor(float fMetersPerUnit)**: Converts game units to meters for distance calculations in sound attenuation.
- **Function: SetRolloffFactor(float fRolloffFactor)**: Adjusts the rolloff factor controlling sound attenuation with distance.
- **Function: SetDopplerFactor(float fDopplerFactor)**: Sets the doppler factor for simulating Doppler effect in sound.
- **Function: GetBufferSource()**: Returns a buffer source object for managing sound buffers.
- **Function: CreateStaticBuffer(TRef<ISoundInstance>& psoundNew, ISoundPCMData* pcmdata, bool bLooping, ISoundPositionSource* psource)**: Creates a static sound buffer from PCM data.
- **Function: CreateASRBuffer(TRef<ISoundInstance>& psoundNew, ISoundPCMData* pcmdata, unsigned uLoopStart, unsigned uLoopLength, ISoundPositionSource* psource)**: Creates an ASR sound buffer with a loop in the middle.
- **Function: GetUpdateEventSource()**: Returns an event source that fires each time the update method is called.

**CONNECTS TO:** Other systems or files it depends on or serves:
- DirectSound3D for audio playback and 3D positioning.
- `ISoundListener` interface for listener properties.
- `ISoundPCMData` for PCM data sources.
- `ISoundInstance` for sound instances.
- `ISoundPositionSource` for position sources.

**NOTES:**
- Contains TODO comments indicating areas that need review or improvement.
- Has a commented-out line `m_pDirectSound->Compact();` which is harmless in DirectX 8.
- The `Update()` method includes logic to update the listener's position every 30 milliseconds, ensuring updates are not too frequent.
- Includes a debug dump function (`DebugDump`) conditionally compiled in debug mode.
- Contains assertions for valid input parameters.

## `src/soundengine/ds3dengine.h`

PURPOSE: This file defines a class `DS3DSoundEngine` that implements a sound engine using DirectSound3D for 3D audio rendering in the Allegiance game.

KEY PARTS:
- DS3DSoundEngine: Manages sound resources, handles 3D audio positioning, and provides interfaces to create sound buffers.
- SetPrimaryBufferFormat: Configures the primary buffer format of the DirectSound instance.
- UpdateListener: Updates the listener's position and orientation for 3D audio rendering.
- CreateStaticBuffer: Creates a static sound buffer from a PCM data source.
- CreateASRBuffer: Creates an ASR (Attack-Sustain-Release) sound buffer with looping capabilities.
- GetUpdateEventSource: Provides an event source that fires on each update call.

CONNECTS TO:
- DirectSound3D libraries for audio rendering
- ISoundEngine, ISoundBufferSource interfaces for sound engine functionality

NOTES: None.

## `src/soundengine/ds3dutil.cpp`

PURPOSE: This file contains utility functions and classes for managing worker threads and task lists in the Allegiance game's sound engine.

KEY PARTS:
- WorkerThread: Manages a background thread that executes tasks.
- TaskListThread: Extends WorkerThread to manage a list of tasks, executing them until they complete or are removed.
- ThreadInit, ThreadCleanup, Execute: Methods for initializing, cleaning up, and running the worker thread's main loop.
- StartThread, StopThread: Functions to start and stop the worker thread with specified priority and sleep time.
- TaskListThread::ThreadIteration: Executes each task in the list until they complete or are removed.
- AddTask, RemoveTask, HasTask: Methods for managing tasks in the task list.

CONNECTS TO: soundbase.h, ds3dutil.h

NOTES: None.

## `src/soundengine/ds3dutil.h`

PURPOSE: This file contains utility functions and classes used by the DirectSound3D wrappers in Allegiance. It provides functionality for vector conversions, unit vector assertions, and worker threads.

KEY PARTS:
- `ConvertVector`: Converts a `Vector` to a `D3DVECTOR`, adjusting for coordinate system differences.
- `ZAssertIsUnitVector`: Asserts that a given vector is approximately a unit vector.
- `WorkerThread`: A class for creating a background thread that executes a task periodically.
- `TaskListThread`: A subclass of `WorkerThread` that manages a list of tasks to be executed in sequence.

CONNECTS TO: DirectSound3D wrappers, Allegiance game engine.

NOTES:
- Contains commented-out code related to mutexes and debugging assertions.

## `src/soundengine/ds3dvirtualbuffer.cpp`

**PURPOSE:** The file `ds3dvirtualbuffer.cpp` is part of the Allegiance game's sound engine and manages virtual sound buffers for both 2D and 3D audio effects. It handles buffer preparation, state updates, playback control, and properties like gain, pitch, and priority.

**KEY PARTS:**
- **Class: DS3DBufferPositionTracker:** Tracks the position of a sound buffer, handling looping, stopping, and ASR behavior.
- **Class: DSVirtualSoundBuffer:** Manages virtual sound buffers, preparing them for playback, updating their state, starting and stopping them, and handling various properties like gain, pitch, and priority.
- **Function: PrepareBuffer8():** Initializes a DirectSound3D buffer with the given quality settings, supporting hardware acceleration and 3D support. Handles different types of sound buffers (static, streaming, ASR).
- **Function: Update():** Updates the state of the virtual sound buffer, including position tracking, volume adjustment, and determining if the sound is playing or stopped.
- **Function: StartBuffer8():** Creates and starts a real DirectSound3D buffer for the sound. Handles cases where a buffer is already playing.
- **Function: StopBuffer():** Forces the stop of the current buffer, updating the position tracker to reflect the final position.
- **Function: SetGain():** Sets the gain of the sound, clamping it between -100 dB and 0 dB.
- **Function: SetPitch():** Adjusts the pitch of the sound within valid frequency limits based on the sample rate.
- **TODO:** The code contains several TODO comments indicating areas that need review or further implementation. For example, handling fade out at higher quality settings when stopping a sound.

**CONNECTS TO:** DirectSound3D API for audio playback and management.

**NOTES:**
- The use of `ZAssert(false)` to indicate unexpected conditions and potential bugs.
- Conditional compilation for different DirectSound versions (`DirectSound8` vs. `DirectSound`).

## `src/soundengine/ds3dvirtualbuffer.h`

PURPOSE: This file defines classes related to virtual DirectSound3D buffers that may or may not have a real DirectSoundBuffer associated with them at any given moment. It includes functionality for tracking buffer positions, managing sound instances, and providing interfaces for tweaking sound properties.

KEY PARTS:
- BufferPositionTracker: Tracks the position of a sound buffer over its lifetime.
- DSVirtualSoundBuffer: Represents a virtual DirectSound3D sound buffer that may or may not have an actual DirectSoundBuffer associated with it.
- DS3DVirtualSoundBuffer: Extends DSVirtualSoundBuffer to add 3D positioning capabilities.

CONNECTS TO:
- ds3dbuffer.h
- soundbase.h

NOTES:
- None.

## `src/soundengine/dummysound.cpp`

PURPOSE: This file provides dummy implementations of sound interfaces for use in the Allegiance game when no sound card is available.

KEY PARTS:
- DummySoundInstance: A stubbed implementation of the ISoundInstance interface.
- DummySoundEngineImpl: A stubbed implementation of the ISoundEngine interface.
- CreateDummySoundEngine: Function to create a dummy sound engine instance.

CONNECTS TO: soundbase.h, soundutil.h

NOTES: None.

## `src/soundengine/soundbase.cpp`

PURPOSE: This file contains the implementation of a sound engine for Allegiance, using DirectSound for audio playback.

KEY PARTS:
- `CreateSoundEngine`: Initializes and creates an instance of the sound engine.
- `DS3DSoundEngine`: The primary class implementing the sound engine functionality.
- `SoundDebugDump`: A debug helper function to dump information about sound objects.

CONNECTS TO: `ds3dutil.h`, `ds3dbuffer.h`, `ds3dvirtualbuffer.h`, and `ds3dengine.h`.

NOTES: None.

## `src/soundengine/soundbase.h`

PURPOSE: Defines interfaces and helper functions for managing sound in the Allegiance game engine.

KEY PARTS:
- ISoundObject: Base interface for all sound objects.
- ISoundTweakable: Interface for tweaking general sound properties like gain and pitch.
- ISoundTweakable3D: Interface for 3D sound properties including positioning and cones.
- ISoundInstance: Interface for controlling the playback of a sound instance.
- ISoundPositionSource: Provides positioning information for sound emitters or listeners.
- ISoundListener: Provides listener-specific positioning information.
- ISoundPCMData: Represents PCM wave data for sound buffers.
- ISoundBufferSource: Creates sound buffers from PCM data.
- ISoundEngine: Manages sound buffers and the overall sound environment.
- ISoundTemplate: Template for creating new sound instances.

CONNECTS TO: event.h, vector.h, tref.h

NOTES: None.

## `src/soundengine/soundengine.h`

PURPOSE: tiny file (6 lines), shown in full rather than summarized.

    #pragma once
    #include "zlib.h"
    #include "soundbase.h"
    #include "soundutil.h"
    #include "soundtemplates.h"
    using namespace SoundEngine;

## `src/soundengine/soundtemplates.cpp`

PURPOSE: Manages and manipulates sound templates for various audio effects in Allegiance.

KEY PARTS:
- WaveFileTemplate: Manages sound templates for wave files.
- ThreeDSoundTemplate: Converts a sound template into a 3D sound.
- SoundConeTemplate: Adds a sound cone to another sound template.
- CreateWaveFileSoundTemplate: Creates a sound template from a wave file.
- SoundGainTemplate: Adjusts the gain of a sound.
- GainInstance: A wrapper for ISoundInstance that adjusts the gain.
- CreateGainSoundTemplate: Creates a sound template with adjusted gain.
- PairedSoundTemplate: Merges two sound templates into one.
- RepeatFirePCMData: Generates data for repeat-fire sounds.
- RepeatFireBufferSource: Wraps a buffer source to create ASR sounds for repeat-fire effects.
- RepeatFireTemplate: Template for creating repeat-fire weapon sounds.
- IntermittentSoundTemplate: Manages a sound template for sounds that play intermittently.

CONNECTS TO: Allegiance's audio engine and game logic systems.

NOTES:
- Contains multiple sections with similar logic for adding and processing samples, which could be refactored into a common function.
- Assertions (ZAssert) throughout the code to ensure correct input values.

PARTS (in file order):

- lines 1-374
  - WaveFileTemplate: Manages sound templates for wave files.
  - ThreeDSoundTemplate: Converts a sound template into a 3D sound.
  - SoundConeTemplate: Adds a sound cone to another sound template.
  - SoundPitchTemplate: Adjusts the pitch of another sound template.
  - CreateWaveFileSoundTemplate: Creates a sound template from a wave file.
  - Create3DSoundTemplate: Creates a 3D sound template from another sound template.
  - CreateSoundConeTemplate: Creates a sound template with a sound cone.
  - CreatePitchSoundTemplate: Creates a sound template with adjusted pitch.
- lines 375-745
  - SoundGainTemplate: Adjusts the gain of a sound.
  - GainInstance: A wrapper for ISoundInstance that adjusts the gain.
  - CreateGainSoundTemplate: Creates a sound template with adjusted gain.
  - SoundPriorityTemplate: Adjusts the priority of a sound.
  - PriorityInstance: A wrapper for ISoundInstance that adjusts the priority.
  - CreatePrioritySoundTemplate: Creates a sound template with adjusted priority.
  - LoopingSoundTemplate: Makes a sound loop.
  - LoopingSoundBufferSource: A wrapper for ISoundBufferSource that forces looping sounds.
- lines 746-1124
  - PairedSoundTemplate: A class that merges two sound templates into one.
  - CreatePairedSoundTemplate: A function to create a new instance of PairedSoundTemplate.
  - RepeatFirePCMData: A class to generate data for repeat-fire sounds.
  - RepeatFirePCMData::Create: A static method to create a repeat fire data source for the given sample and repeat delay.
- lines 1125-1480
  - RepeatFirePCMData: Manages sound data for repeat-fire weapons.
  - RepeatFireBufferSource: Wraps a buffer source to create ASR sounds for repeat-fire effects.
  - RepeatFireTemplate: Template for creating repeat-fire weapon sounds.
  - CreateRepeatingFireSoundTemplate: Function to create a template for repeat-fire weapon sounds.
  - RandomSoundTemplate: Template for randomly selecting sound templates.
  - CreateRandomSoundTemplate: Function to create a random sound template.
  - IntermittentSoundTemplate: Template for intermittently playing sounds.
  - TODOs or unusual code: The code has multiple sections with similar logic for adding and processing samples, which could be refactored into a common function. There are assertions (ZAssert) throughout the code to ensure correct input values, which is good practice but might be overkill depending on the context.
- lines 1481-1838
  - IntermittentSoundTemplate: Manages a sound template for sounds that play intermittently.
  - CreateIntermittentSoundTemplate: Creates an instance of IntermittentSoundTemplate.
  - DelayedSoundInstance: Represents a sound instance with a delayed start.
  - SoundMutex: A helper class for serialized sounds, handling serialization.
  - SoundInstanceList: An iterator over a list of playing sound instances.
  - IsPlaying: Checks if the sound is currently playing.
  - Stop: Stops the sound, either immediately or after fading out.
  - SetGain: Adjusts the volume of the sound.
- lines 1839-2101
  - TemplateData: Holds data about a sound template instance.
  - TemplateMap: Maps sound templates to their data.
  - StartSound: Starts a new sound and handles bookkeeping.
  - QueueNextSound: Queues the next sound based on urgency and expiration.
  - CreateSound: Creates or updates a sound template, handling single-instance sounds.
  - OnEvent: Handles when a sound finishes playing.
  - Reset: Resets all pending sounds without playing them.
  - CreateSoundMutex: Creates a mutex for serialized sound templates.

## `src/soundengine/soundtemplates.h`

PURPOSE: This file contains various implementations of sound templates used in the Allegiance game engine for managing and manipulating audio resources.

KEY PARTS:
- `CreateWaveFileSoundTemplate`: Creates a sound template from a wave file.
- `CreateDummySoundTemplate`: Creates a dummy sound template for error handling.
- `Create3DSoundTemplate`: Converts a sound template into a 3D sound with specified minimum distance.
- `CreateSoundConeTemplate`: Adds a sound cone to the given sound template.
- `CreatePitchSoundTemplate`: Adjusts the pitch of the created sound.
- `CreateGainSoundTemplate`: Adjusts the gain (volume) of the created sound.
- `CreatePrioritySoundTemplate`: Adjusts the priority of the created sound.
- `CreateLoopingSoundTemplate`: Creates a looping version of the given sound template.
- `CreateASRSoundTemplate`: Creates an ASR (Adaptive Sound Rate) sound from the given template with specified loop start and length.
- `CreatePairedSoundTemplate`: Combines two sound templates into one.
- `CreateRepeatingFireSoundTemplate`: Creates a repeating fire sound template for weapons.
- `IRandomSoundTemplate`: Interface for adding new sound templates to a random sound template.
- `CreateRandomSoundTemplate`: Creates a sound template that randomly chooses from multiple sound templates.
- `CreateIntermittentSoundTemplate`: Creates an intermittent sound template that plays at specified intervals.
- `ISoundMutex`: Interface for managing sound mutexes.
- `CreateSoundMutex`: Creates a sound mutex for serialized sound templates.
- `CreateSerializedSoundTemplate`: Creates a serialized sound template that prevents overlapping sounds with the same mutex.

CONNECTS TO: `soundbase.h` (contains base interfaces and classes for sound objects).

NOTES: None.

## `src/soundengine/soundutil.cpp`

**PURPOSE:** Manages loading and parsing of wave files (WAV and OGG formats), caching to avoid redundant file opens, and provides basic sound source and listener management for the audio system in Allegiance.

**KEY PARTS:**
- **SoundFile**: Manages loading and caching of wave files.
- **LoadWaveFile**: Static function to load PCM data objects from wave files.
- **DummyPCMDataImpl**: Provides a dummy PCM data implementation with fixed parameters.
- **CreateDummyPCMData**: Creates and returns a reference to the dummy PCM data object.
- **GenericSoundSource**: Manages basic properties of a sound source like position, velocity, orientation, and playback state.
- **SetPosition**, **SetVelocity**, **SetOrientation**: Functions to update the sound source's position, velocity, and orientation.
- **GetPosition**, **GetVelocity**, **GetOrientation**: Functions to retrieve the current position, velocity, and orientation of the sound source.
- **Stop**: Function to stop the sound from playing.
- **SoundPositionSourceWrapper**: A wrapper class delegating calls to a base `ISoundPositionSource`.
- **GenericListener**: Represents the listener in the audio system, managing properties like up direction.
- **SetUpDirection**: Function to update the up direction of the listener.
- **GetUpDirection**: Function to retrieve the current up direction of the listener.
- **SoundInstanceWrapper**: A wrapper class delegating calls to a base `ISoundInstance`.
- **Stop**: Function to stop the sound instance, with an option for immediate stopping.
- **IsPlaying**: Function to check if the sound instance is currently playing.
- **StubbedTweakableSoundInstance**: A stub implementation of `ISoundInstance` and `ISoundTweakable`, used for testing or when actual sound instances are not available.
- **SetGain**, **SetPitch**, **SetPriority**: Functions to adjust the gain, pitch, and priority of the sound instance.
- **Set3D**, **SetMinimumDistance**, **SetCone**: Functions to configure 3D sound properties such as whether it's in 3D space, minimum distance for maximum volume, and sound cones for directional effects.
- **SoundBufferSourceDelegate**: A delegate class handling creation of different types of sound buffers from PCM data.

**CONNECTS TO:** Other audio-related systems and files within Allegiance.

**NOTES:** The file includes commented-out sections related to performance measurement (`_ftime_s`) and assertions with debug output statements for development and testing. Memory management is handled carefully, with explicit freeing of allocated memory when no longer needed.

## `src/soundengine/soundutil.h`

PURPOSE: This file contains utility classes and functions for managing sound in the Allegiance game engine.

KEY PARTS:
- `LoadWaveFile`: Loads a PCM data object from a wave file.
- `CreateDummyPCMData`: Creates a dummy PCM data object.
- `GenericSoundSource`: A generic implementation of a sound source with position, velocity, and orientation control.
- `SoundPositionSourceWrapper`: A wrapper class for an ISoundPositionSource that delegates all calls to the wrapped object.
- `GenericListener`: A generic implementation of a listener with position and up direction control.
- `SoundInstanceWrapper`: A wrapper class for an ISoundInstance that delegates all calls to the wrapped object.
- `SoundTweakableWrapper`: A wrapper class for an ISoundTweakable that delegates all calls to the wrapped object.
- `SoundTweakable3DWrapper`: A wrapper class for an ISoundTweakable3D that delegates all calls to the wrapped object.

CONNECTS TO: `soundbase.h` (for interfaces like ISoundPCMData, ISoundPositionSource, etc.)

NOTES: None.
