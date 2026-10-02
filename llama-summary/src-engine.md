# src/engine

[Back to the index](../LLAMA_SUMMARY.md) - 102 files, 49,855 lines. Notes by a local model, to orient by: not verified facts.

The `src/engine` folder contains core components and utilities essential for managing rendering, configuration, input, and other fundamental functionalities in the Allegiance game engine.

- CMakeLists.txt: Configures the build environment for the engine library.
- Configuration.cpp & .h: Manages and stores configuration settings for the game.
- D3DDevice9.cpp & .h: Handles DirectX 9 device management and rendering.
- DeviceModesDX9.cpp & .h: Manages display modes and device settings for DirectX 9.
- EngineSettings.h: Stores additional engine-specific settings for DirectX 9.
- HashTable.cpp & .h: Implements a hash table for efficient object storage.
- ImageTransfer.cpp & .h: Provides functions for image data manipulation.
- LogFile.cpp & .h: Manages logging of game events and errors.
- RasterD3D9.cpp: Implements a rasterizer for DirectX 9 rendering.
- VBIBManager.cpp & .h: Manages vertex and index buffers for efficient rendering.
- VRAMManager.cpp & .h: Handles memory management for textures in video memory.
- VertexGenerator.cpp & .h: Generates vertices for UI elements.
- VideoSettingsDX9.cpp & .h: Manages video settings through a dialog interface.

Largest files: device3d.cpp (2792 lines), modeler.cpp (2686 lines), context.cpp (2474 lines), geometry.cpp (2029 lines), D3DDevice9.cpp (1942 lines).

## `src/engine/CMakeLists.txt`

**PURPOSE:** This file is a CMake configuration script for the Allegiance game engine. It sets up build definitions, includes directories, and specifies source files to compile into the `Engine` library.

**KEY PARTS:**
- `- Engine`: The primary class representing the Allegiance game engine.
- `- DeviceModesDX9.cpp`: Handles DirectX 9 device modes.
- `- RasterD3D9.cpp`: Manages rasterization using DirectX 9.
- `- VBIBManager.cpp`: Manages vertex buffer, index buffer, and vertex declaration management.
- `- VideoSettingsDX9.cpp`: Manages video settings for DirectX 9.
- `- engine.cpp`: Main entry point for the engine.
- `- enginewindow.cpp`: Handles window creation and management.

**CONNECTS TO:** Other modules like `Graphics`, `Input`, and `Audio`.

**NOTES:** None.

## `src/engine/Configuration.cpp`

PURPOSE: The file `Configuration.cpp` is responsible for managing configuration settings in the Allegiance game engine. It provides methods to read and write various types of configuration values such as booleans, floats, integers, and strings.

KEY PARTS:
- `UpdatingConfiguration::Write()`: Writes all configuration values from internal maps to a persistent storage.
- `UpdatingConfiguration::GetBool()`, `UpdatingConfiguration::GetFloat()`, `UpdatingConfiguration::GetInt()`, `UpdatingConfiguration::GetString()`: Retrieve or create configuration values for the specified key, optionally providing default values.
- `UpdatingConfiguration::GetBoolValue()`, `UpdatingConfiguration::GetFloatValue()`, `UpdatingConfiguration::GetIntValue()`, `UpdatingConfiguration::GetStringValue()`: Retrieve the current value of a configuration setting.
- `UpdatingConfiguration::SetBoolValue()`, `UpdatingConfiguration::SetFloatValue()`, `UpdatingConfiguration::SetIntValue()`, `UpdatingConfiguration::SetStringValue()`: Set the value of a configuration setting.

CONNECTS TO: `SimpleModifiableValue<>` (for storing individual configuration values), `Configuration.h` (header file for class declarations).

NOTES: None.

## `src/engine/Configuration.h`

PURPOSE: This file defines a configuration system that manages and updates various types of configuration values such as booleans, floats, integers, and strings. It uses a caching mechanism to store these values efficiently.

KEY PARTS:
- CacheValueMap<T>: A template class that caches configuration values of type T.
  - Provides methods to get, set, and check the existence of cached values.
- UpdatingConfiguration: A class that extends Value and manages different types of configuration maps (bools, floats, ints, strings).
  - Uses IConfigurationStore for reading and writing configuration data.
  - Provides methods to get and set configuration values with default values.

CONNECTS TO:
- IConfigurationStore: The interface used for reading and writing configuration data.

NOTES:
- None.

## `src/engine/D3DDevice9.cpp`

PURPOSE: Manages the DirectX 9 device for rendering in Allegiance, handling initialization, resource management, and state caching.

KEY PARTS:
- CD3DDevice9: Main class managing DirectX device operations.
- Initialise(): Sets up the Direct3D 9 interface and initializes the device.
- Shutdown(): Cleans up resources and shuts down the D3D9 device.
- ResetDevice(): Resets the device to either windowed or full-screen mode.
- DrawIndexedPrimitiveUP(): Draws indexed primitives using user-provided data.
- SetRenderState(): Sets a render state and updates the cache if necessary.
- GetRenderState(): Retrieves the current value of a render state from the cache.

CONNECTS TO: Allegiance engine rendering subsystems, DirectX 9 libraries.

NOTES:
- Contains TODOs for future enhancements.
- Includes unusual code that forces an exception on DirectX device creation failure.

PARTS (in file order):

- lines 1-358
  - CD3DDevice9: Manages the Direct3D device for rendering.
  - Initialise(): Initializes the D3D9 interface and sets up the device.
  - Shutdown(): Cleans up resources and shuts down the D3D9 device.
  - ResetReferencedResources(): Resets any referenced resources to prevent memory leaks.
  - CreateD3D9(): Creates the Direct3D 9 interface.
  - GetValidBackBufferFormat(): Determines a valid back buffer format for the device.
  - GetValidDepthStencilFormat(): Finds a valid depth stencil format compatible with the back buffer format.
  - LastChanceCreateDevice(): Attempts to create a D3D9 device using hardware or software vertex processing if initial creation fails.
- lines 359-709
  - CD3DDevice9: Manages DirectX device creation and management.
  - CreateAADepthStencilBuffer: Creates an additional depth stencil buffer for anti-aliasing if needed.
  - ResetDevice: Resets the device to either windowed or full-screen mode.
  - UpdateCurrentMode: Updates the current display mode based on whether running in windowed or full-screen.
  - TODOs: "//imago 7/1/09 NYI test for multisample maskable optons (CSAA, etc) and set accordingly" "BT - Do not ship this one, this is just for Xynth to test!"
  - Unusual code: ((int)0) = 0; // Forces an exception if the DirectX device creation fails. Conditional compilation based on EnablePerformanceCounters.
- lines 710-1079
  - CD3DDevice9: Manages DirectX 9 device settings and operations.
  - InitialiseDeviceStateCache: Resets render state cache to default values.
  - ResetReferencedResources: Evicts resources from VRAM and VBIBManager pools.
  - CreateAADepthStencilBuffer: Recreates AA depth stencil buffer if needed.
  - Device: Handles reinitialization of the DirectX device if it becomes null.
  - InitialiseRenderStateCache: Initializes render state cache with default values.
  - RESET_STATE_CACHE_VALUE: Macro to set and apply a single render state value.
- lines 1080-1431
  - CD3DDevice9: Main class for managing DirectX device state and operations.
  - InitialiseTextureStageCache: Initializes cache values for texture stage states.
  - InitialiseSamplerStateCache: Initializes cache values for sampler states.
  - InitialiseTransformCache: Initializes transform cache (currently does nothing).
  - InitialiseMaterialCache: Initializes material cache (currently does nothing).
  - InitialiseLightCache: Initializes light cache, setting up the number of active lights.
  - SetRenderState: Sets a render state and updates the cache if necessary.
  - GetRenderState: Retrieves the current value of a render state from the cache.
- lines 1433-1824
  - CD3DDevice9: Main class for managing DirectX device state and operations.
  - GetTransform: Retrieves the current transformation matrix for a specified type (projection, view, or world).
  - SetMaterial: Sets the material properties to be used in rendering.
  - GetMaterial: Retrieves the current material properties being used.
  - SetLight: Configures a light source with the given parameters.
  - GetLight: Retrieves the configuration of a specific light source.
  - LightEnable: Enables or disables a light source.
  - GetLightEnable: Checks whether a light source is enabled or disabled.
- lines 1825-1942
  - CD3DDevice9: Manages DirectX device operations.
  - DrawIndexedPrimitiveUP: Draws indexed primitives using user-provided data.
  - DrawPrimitiveUP: Draws primitives using user-provided data.
  - ColorFill: Fills a surface with a specified color.
  - Clear: Clears the back buffer to a specified color and depth.
  - SetGammaRamp: Sets the gamma ramp for a swap chain.
  - ResetPerformanceCounters: Resets performance counters used for statistics gathering.
  - GetPerformanceCounter: Retrieves the value of a specific performance counter.

## `src/engine/D3DDevice9.h`

PURPOSE: This file defines the CD3DDevice9 class, which is responsible for managing the DirectX 9 device and its associated resources in Allegiance.

KEY PARTS:
- CD3DDevice9: Manages the DirectX 9 device, including initialization, rendering states, textures, lights, and drawing calls.
- SD3DDeviceSetupParams: Stores setup parameters for the DirectX device.
- SRenderStateCache: Caches render state settings to optimize performance.
- SD3D9DeviceStateCache: Combines various caches into a single structure for efficient access.
- SD3D9TransformCache: Holds transformation matrices (projection, view, world).
- SD3D9MaterialCache: Stores the current material used in rendering.
- SD3D9LightCache: Manages lighting settings.

CONNECTS TO: Allegiance engine systems, particularly those related to graphics and rendering.

NOTES: The class uses a singleton pattern for CD3DDevice9. It includes performance counters if enabled, which are not shown here. There is a TODO comment about setting up vertex shaders and pixel shaders.

## `src/engine/DeviceModesDX9.cpp`

PURPOSE: Manages device mode data for DirectX 9, including enumerating monitors, retrieving display modes, and checking format support.

KEY PARTS:
- CD3DDeviceModeData: Manages device mode data.
- enumMonitorProc: Callback function to enumerate monitors.
- getPrimaryMonitor: Retrieves the primary monitor handle.
- pSupportedModes: Array of supported buffer formats.
- GetNumAASettings: Returns number of anti-aliasing settings.
- GetResolutionDetails: Provides details about a resolution.
- CheckDeviceFormat: Checks if a format is supported for a given device type.

CONNECTS TO: DirectX API functions and Windows API functions like GetMonitorInfo.

NOTES:
- Contains TODOs for texture filter options.

PARTS (in file order):

- lines 1-357
  - CD3DDeviceModeData: Manages device mode data for DirectX 9.
  - enumMonitorProc: Callback function to enumerate monitors.
  - getPrimaryMonitor: Retrieves the primary monitor handle.
  - pSupportedModes: Array of supported buffer formats in order of preference.
  - pSupportedDepthStencil: Array of supported depth stencil formats in order of preference.
  - pMultiSampleTypes: Array of supported multisample types.
  - pTextureMinFilterTypes, pTextureMagFilterTypes, pTextureMipFilterTypes: Arrays of texture filter types.
  - pszAAStrings, pszShortAAStrings, pszMaxTextureSizeStrings: String arrays for antialiasing and texture size descriptions.
- lines 358-700
  - CD3DDeviceModeData: Manages device mode data for DirectX 9.
  - GetNumAASettings: Returns the number of anti-aliasing settings available for a given display mode.
  - GetAASettingString: Retrieves the string representation of an anti-aliasing setting.
  - GetResolutionDetails: Provides details about a specific resolution, such as width, height, and refresh rate.
  - GetRelatedResolutions: Finds related resolutions for changing up/down in full-screen mode.
  - GetFormatBitCount: Determines the bit count of a given D3DFORMAT.
  - ExtractAdapterData: Extracts data about a graphics adapter's display modes.
  - "try this hack Imago 7/10"
- lines 701-1034
  - CD3DDeviceModeData::GetModeParams: Retrieves device mode parameters based on given indices.
  - CD3DDevice9::SD3DDeviceSetupParams: Structure for device setup parameters.
  - SAdapterMode: Represents a display mode with various attributes like format, resolution, and depth stencil.
  - GetMonitorInfo: Windows API function to retrieve monitor information.
  - CheckDeviceType: DirectX function to check if a device type is supported.
  - CheckDeviceFormat: DirectX function to check if a format is supported for a given device type.
  - CheckDepthStencilMatch: DirectX function to check if a depth/stencil format matches the display mode.
  - CheckDeviceMultiSampleType: DirectX function to check if multisample types are supported.
- lines 1035-1162
  - CD3DDeviceModeData::SAdapterMode: Represents a display mode for an adapter.
  - FindMatchingWindowedMode(CD3DDevice9::SD3DDeviceSetupParams , WinPoint, int): Finds a matching windowed mode based on size and device index.
  - GetMaxTextureString(int): Returns the string representation of a maximum texture size.
  - TODOs: "Imago - Bug: the AA selection box is populated only with your primary adapter's caps" "// Texture minification filter." "// Texture magnification filter." "// Mipmap generation filter."

## `src/engine/DeviceModesDX9.h`

PURPOSE: The file `DeviceModesDX9.h` contains the definition of a class that manages and provides information about available display modes and device settings for DirectX 9 in a game engine.

KEY PARTS:
- CD3DDeviceModeData: Manages data about available adapters, display modes, and device settings.
- SVideoResolution: Represents a video resolution with width, height, and frequency.
- SAdapterMode: Stores information about a specific display mode on an adapter.
- SAdapter: Holds details about an adapter including its current display mode, identifier, monitor handle, and supported modes.
- GetDeviceCount(): Returns the number of adapters available.
- GetTotalResolutionCount(int iDeviceIndex): Returns the total number of resolutions for a given device index.
- GetResolutionDetails(int iDeviceIndex, int iModeIndex, ...): Retrieves detailed information about a specific resolution mode.

CONNECTS TO: `d3d9.h`, `D3DDevice9.h`, and `CLogFile`.

NOTES: None.

## `src/engine/EngineSettings.h`

PURPOSE: This file defines a class to store additional engine settings specific to the DirectX 9 version of Allegiance.

KEY PARTS:
- CDX9EngineSettings: A data container for various engine settings such as VSync, anti-aliasing, texture size, and more.
- m_refreshrate: Stores the screen refresh rate.
- m_iMaxTextureSize: Maximum texture size allowed.
- m_bAutoGenMipmaps: Boolean to enable automatic generation of mipmaps.
- m_bDither: Boolean to enable dithering for color depth.
- m_bVSync: Boolean to enable vertical synchronization.
- m_dwAA: Anti-aliasing level.
- m_dwAAQual: Quality setting for anti-aliasing.

CONNECTS TO: src/engine/Engine.cpp

NOTES: None.

## `src/engine/HashTable.cpp`

PURPOSE: This file contains the implementation of a hash table data structure used in Allegiance for efficient storage and retrieval of game objects based on their unique identifiers.

KEY PARTS:
- CHashTable: Manages the hash table operations such as adding entries, retrieving entries, and generating hash values.
- AddHashEntry: Adds a new entry to the hash table. Returns the existing entry if there is a hash collision.
- GetHashEntry: Retrieves an entry from the hash table based on its hash value.
- GenerateHashValue: Generates a hash value for a given string using the djb2 algorithm.

CONNECTS TO: CHashEntry (the class representing individual entries in the hash table).

NOTES: None.

## `src/engine/HashTable.h`

PURPOSE: This file defines a hash table implementation used in the Allegiance game engine to store and retrieve objects based on their hash values.

KEY PARTS:
- CHashEntry: Base class representing an entry in the hash table. Contains a hash value, a pointer to the next entry in case of collisions, and methods to get and set these properties.
- CHashTable: Class implementing the hash table itself. Manages the table size, total entries added, and provides methods to add entries, retrieve entries by hash value, and generate hash values from strings.

CONNECTS TO: src/engine/HashTable.cpp

NOTES: None

## `src/engine/ImageTransfer.cpp`

PURPOSE: This file contains various functions for transferring and manipulating image data between different color depths and formats in the Allegiance game engine.

KEY PARTS:
- CImageTransfer::Transfer16BitTo16BitNoColourKey: Transfers 16-bit image data without a colour key.
- CImageTransfer::Transfer16BitTo32BitWithColourKey: Transfers 16-bit image data to 32-bit format with a colour key.
- CImageTransfer::Transfer32BitTo32BitCopy: Copies 32-bit image data from one location to another.
- CImageTransfer::ColourImageFill16Bit: Fills a rectangular area of a surface with a 16-bit colour value.

CONNECTS TO: color.h, WinPoint.h

NOTES: None.

## `src/engine/ImageTransfer.h`

PURPOSE: The file `ImageTransfer.h` defines a class `CImageTransfer` that provides various functions for transferring and manipulating image data between different color depths.

KEY PARTS:
- `Transfer16BitTo16BitCopy`: Copies 16-bit image data from source to destination without any color key.
- `Transfer16BitTo16BitNoColourKey`: Transfers 16-bit image data with no color key.
- `Transfer16BitTo16BitWithColourKey`: Transfers 16-bit image data using a specified color key.
- `Transfer16BitTo32BitNoColourKey`: Transfers 16-bit image data to 32-bit format without a color key.
- `Transfer16BitTo32BitWithColourKey`: Transfers 16-bit image data to 32-bit format using a specified color key.
- `Transfer32BitTo32BitCopy`: Copies 32-bit image data from source to destination.
- `ColourImageFill16Bit`: Fills a 16-bit image with a specified pixel color.
- `ColourImageFill32Bit`: Fills a 32-bit image with a specified pixel color.

CONNECTS TO: `Color` class, `point.h`, and other image handling functions or classes within the Allegiance engine.

NOTES: None.

## `src/engine/LogFile.cpp`

PURPOSE: The file `LogFile.cpp` implements a logging system for the Allegiance game engine. It handles opening, writing to, and closing log files, as well as appending timestamps to log entries.

KEY PARTS:
- `CLogFile`: Class that manages the creation, writing, and closure of log files.
- `OutputString`: Writes a standard string to the log file.
- `OutputStringV`: Writes formatted strings to the log file using variable arguments.
- `GetTimeStampString`: Generates a timestamp string in the format "Month Day, Hour:Minute".

CONNECTS TO: Depends on Windows API functions for file operations and time handling.

NOTES: None.

## `src/engine/LogFile.h`

PURPOSE: tiny file (17 lines), shown in full rather than summarized.

    #pragma once
    #include <windows.h>
    class CLogFile
    {
    private:
    	HANDLE m_hLogFile;
    	char m_szFilePath[ MAX_PATH ];
    	char m_szFileName[ MAX_PATH ];
    	char m_szTimeStamp[256];
    public:
        CLogFile(const char *szLogName );
    	~CLogFile(void);
    	void CloseLogFile();
    	void OutputString( const char * szString );
    	void OutputStringV( const char * szFormat, ... );
    	char * GetTimeStampString();
    };

## `src/engine/RasterD3D9.cpp`

PURPOSE: This file contains the implementation of a rasterizer for DirectX 9 used in Allegiance. It manages rendering states and performs drawing operations.

KEY PARTS:
- `D3D9RasterizerImpl`: Class implementing the rasterization logic using DirectX 9.
- `CreateD3D9Rasterizer`: Function to create an instance of `D3D9RasterizerImpl`.

CONNECTS TO: 
- `enginep.h`, `D3DDevice9.h`

NOTES:
- Contains debug error handling and state management for rendering operations.
- Some functions are commented out or disabled, indicating potential future enhancements or optimizations.

## `src/engine/UIVertexDefn.h`

PURPOSE: Defines vertex structures and their associated Flexible Vertex Formats (FVFs) for rendering UI elements in Allegiance.

KEY PARTS:
- UIVERTEX: Represents a single textured uncoloured vertex.
- D3DFVF_UIVERTEX: FVF for UIVERTEX, including position, texture coordinates.
- UICOLOURFILLVERTEX: Represents a vertex used to render blank coloured squares.
- D3DFVF_UICOLOURVERT: FVF for UICOLOURFILLVERTEX, including position and colour.
- UIFONTVERTEX: Represents a single textured uncoloured vertex with colour, not affected by transforms or lighting.
- D3DFVF_UIFONTVERTEX: FVF for UIFONTVERTEX, including position, colour, and texture coordinates.
- UIFONTVERTEX2: Represents a vertex affected by transforms and lighting.
- D3DFVF_UIFONTVERTEX2: FVF for UIFONTVERTEX2, including position, colour, and texture coordinates.

CONNECTS TO: DirectX rendering system (D3D).

NOTES: None.

## `src/engine/VBIBManager.cpp`

**PURPOSE:** The `src/engine/VBIBManager.cpp` file is responsible for managing vertex and index buffers in the Allegiance game engine. It handles buffer allocation, deallocation, and management to optimize rendering performance.

**KEY PARTS:**
- **Class: CVBIBManager**: Manages vertex and index buffers.
- **Function: Initialise()**: Initializes the buffer manager based on hardware capabilities.
- **Function: Shutdown()**: Cleans up all allocated buffers.
- **Function: AllocateVertexRegion()**: Allocates a vertex buffer region.
- **Function: AllocateIndexRegion()**: Allocates an index buffer region.
- **Function: ReleaseRegion()**: Releases a previously allocated buffer region.
- **Function: AddVerticesToBuffer()**: Adds vertices to an existing or new buffer.
- **Function: AddIndicesToBuffer()**: Adds indices to an existing or new buffer.
- **Function: FindBuffer()**: Searches for an existing buffer that matches specified criteria.
- **Data Structure: SVBIBManagerState**: Holds the state of the buffer manager, including a list of allocated buffers.

**CONNECTS TO:** CD3DDevice9.cpp (DirectX device management)

**NOTES:** The code contains several `ZAssert` statements for debugging purposes and conditional logic based on hardware vertex processing availability. There are TODOs indicating areas that need further investigation or potential issues with device initialization.

## `src/engine/VBIBManager.h`

PURPOSE: The file `VBIBManager.h` defines the class `CVBIBManager`, which manages vertex and index buffers for rendering in a 3D graphics application.

KEY PARTS:
- CVBIBManager: Manages the allocation, locking, and unlocking of vertex and index buffers.
- SVBIBHandle: Represents a handle to a buffer, including its type, format, and size.
- SD3DBuffer: Stores information about each buffer, such as its type, format, size, and whether it is locked or evicted.
- SVBIBManagerState: Holds the state of the buffer manager, including initialization status and buffer allocation.

CONNECTS TO: Direct3D rendering API (via D3DPOOL and LPDIRECT3DVERTEXBUFFER9/LPDIRECT3DINDEXBUFFER9).

NOTES: None.

## `src/engine/VRAMManager.cpp`

**PURPOSE:** The `VRAMManager.cpp` file manages VRAM resources for textures and render targets in the Allegiance game engine. It ensures efficient memory allocation, handles resource creation and destruction, and provides functions to lock, unlock, and set textures.

**KEY PARTS:**
- **Class: CVRAMManager**
  - Manages VRAM resources.
  - Implements a singleton pattern.
  
- **Function: Initialise()**
  - Initializes the VRAM manager by creating the first bank of textures.
  - Creates a test texture if `CREATE_TEST_TEXTURE` is defined.

- **Function: Shutdown()**
  - Cleans up resources, releasing handles and resetting states.

- **Function: EvictDefaultPoolResources()**
  - Releases resources in the DEFAULT pool, specifically render targets.

- **Function: GetTotalTextureCount()**
  - Returns the total number of textures managed by the VRAM manager.

- **Function: AllocateHandle()**
  - Allocates a new texture handle and assigns it to a bank.
  
- **Function: CreateTexture()**
  - Creates a D3D texture resource for a given texture handle.
  - Supports both system memory and GPU memory allocations.
  - Handles mipmap generation if enabled.

- **Function: CreateRenderTarget()**
  - Creates a render target texture with specified dimensions.
  - Render targets are always placed in the DEFAULT pool.

- **Function: ReleaseHandle()**
  - Releases a texture handle, freeing up resources when the reference count reaches zero.
  - Handles both system memory and GPU memory textures.

- **Function: LockTexture(TEXHANDLE texHandle, D3DLOCKED_RECT *pLockRect, uint32_t dwFlags = 0, uint32_t dwLevel = 0, CONST RECT *pAreaToLock = NULL)**
  - Locks a texture for direct access to its pixels.
  - Supports both system memory and VRAM textures.

- **Function: UnlockTexture(TEXHANDLE texHandle, uint32_t dwLevel = 0)**
  - Unlocks a previously locked texture.
  - Handles mip map generation if the texture is mip mapped.

- **Function: SetTexture(TEXHANDLE texHandle, uint32_t dwTextureStage)**
  - Sets a texture to a specific texture stage in the rendering pipeline.
  - Includes debug assertions and checks for valid textures.

- **Function: IsTextureValid(TEXHANDLE texHandle)**
  - Checks if a texture handle is valid.

- **Function: GetOriginalDimensions(TEXHANDLE texHandle, uint32_t *pdwWidth, uint32_t *pdwHeight)**
  - Retrieves the original dimensions of a texture.

- **Function: GetActualDimensions(TEXHANDLE texHandle, uint32_t *pdwWidth, uint32_t *pdwHeight)**
  - Retrieves the actual dimensions of a texture after any scaling or processing.

- **Function: GetTextureFormat(TEXHANDLE texHandle)**
  - Returns the format of a texture.

- **Function: GetPower2(uint32_t dwInitialValue)**
  - Calculates the smallest power of two greater than or equal to the given value.

- **Function: GetTextureSurface(TEXHANDLE texHandle, IDirect3DSurface9 **ppSurface)**
  - Retrieves the surface associated with a texture.
  - The caller is responsible for releasing the surface.

- **Function: PushRenderTarget(TEXHANDLE texHandle, uint32_t dwTargetIndex = 0)**
  - Sets a texture as the render target.

**CONNECTS TO:** Direct3D (D3D), rendering pipeline, and other VRAM-related classes.

**NOTES:** Contains `ZAssert` statements for debugging, forceful exception handling to debug crashes related to texture creation, and comments indicating potential issues or areas needing review.

## `src/engine/VRAMManager.h`

PURPOSE: The file `VRAMManager.h` defines the VRAM management system for Allegiance, responsible for allocating and managing textures in video memory.

KEY PARTS:
- CVRAMManager: Main class that manages texture allocation, creation, and rendering.
- STexture: Structure representing a single texture, including its format, dimensions, and state flags.
- SBank: Structure representing a bank of textures, used to manage multiple textures efficiently.
- SVRAMRenderTarget: Structure for managing render targets.
- SVRAMManagerState: State structure containing the current state of VRAM management.

CONNECTS TO:
- Direct3D (D3D9.h, D3DX9tex.h): For creating and managing DirectX textures and surfaces.

NOTES:
- Contains macros for handling texture handles and indices.
- Includes functions for creating system memory textures and render targets.
- Manages a stack of render targets for rendering operations.

## `src/engine/VertexGenerator.cpp`

**PURPOSE:** The file `VertexGenerator.cpp` is part of the Allegiance game engine and is responsible for generating vertices for UI elements such as textured rectangles and filled areas.

**KEY PARTS:**
- **Class: CVertexGenerator**
  - Manages dynamic vertex buffers for UI rendering.
  - Initializes predefined dynamic buffers for different types of vertices.
  - Generates vertices for textured UI elements and filled areas.
  
- **Function: Initialise()**
  - Allocates memory for predefined dynamic vertex buffers.

- **Function: GenerateUITexturedVertices()**
  - Creates vertices for a textured rectangle based on the provided texture handle and rectangle dimensions.

- **Function: GenerateFillVertices()**
  - Generates vertices for a filled rectangle with a specified color.

- **Function: GenerateFillVerticesD3DColor()**
  - Similar to `GenerateFillVertices`, but takes a D3DCOLOR directly instead of a Color object.

**CONNECTS TO:** 
- `CVBIBManager`: Manages vertex buffer allocation and locking.
- `CVRAMManager`: Provides texture dimensions.
- `UIVertexDefn.h`: Defines the structure of UI vertices.

**NOTES:**
- The file uses assertions (`ZAssert`) for error checking, indicating that it is part of a debug build where these checks are enabled.
- There is commented-out code related to vertex position texturing offsets, which suggests this feature might have been implemented but later disabled.

## `src/engine/VertexGenerator.h`

PURPOSE: The file defines a singleton class `CVertexGenerator` responsible for generating and managing vertex buffers for both 2D UI elements and 3D graphics in the Allegiance game engine.

KEY PARTS:
- CVertexGenerator: Singleton class that handles vertex buffer generation.
- EPredefinedDynamicBufferType: Enumerates different types of dynamic vertex buffers.
- SPredefinedDynamicBufferConfig: Struct defining configuration for predefined dynamic buffers.
- SVertexGeneratorState: Struct tracking the state of the vertex generator, including initialized status and handles to dynamic buffers.
- Initialise(): Initializes the vertex generator.
- GenerateUITexturedVertices(): Generates vertices for textured UI elements.
- GenerateFillVertices(): Generates filled rectangle vertices with a solid color.
- GenerateFillVerticesD3DColor(): Generates filled rectangle vertices with a D3D color.
- GetPredefinedDynamicBuffer(): Retrieves handles to predefined dynamic buffers.

CONNECTS TO: VBIBManager and VRAMManager for buffer management, and other rendering systems.

NOTES: None.

## `src/engine/VideoSettingsDX9.cpp`

PURPOSE: This file contains the implementation of video settings for DirectX 9 in Allegiance. It handles user interaction through a dialog to select video modes and antialiasing settings, and it also manages loading these settings from configuration files.

KEY PARTS:
- `PromptUserForVideoSettings`: Main function that prompts the user for video settings.
- `SetModeData`, `SetAAData`, `SetGFXSettings`: Functions to populate dropdown menus with available resolutions, antialiasing options, and graphics settings.
- `ResPickerDialogProc`: Dialog procedure handling user interactions in the video settings dialog.
- `GetMaxRate`: Retrieves the maximum refresh rate of the display device.

CONNECTS TO: 
- `EngineSettings.h` for global engine settings.
- `DeviceModesDX9.h` for device mode data.
- `LogFile.h` for logging purposes.

NOTES:
- Contains hardcoded default settings and TODOs for language-dependent support and registry keys.
- Includes logic to handle antialiasing settings based on available hardware capabilities.

## `src/engine/VideoSettingsDX9.h`

PURPOSE: tiny file (9 lines), shown in full rather than summarized.

    #ifndef _VIDEOSETTINGS_H_
    #define _VIDEOSETTINGS_H_
    #include <windows.h>
    #include <zstring.h>
    #include "Configuration.h"
    //Imago added bool bStartFullscreen, bool bRaise 6/29/09, only prompts when "Safe Mode" and restored the -windowed command line switch
    //Rock: Removed bRaise, maybe reintroduce when it would be used again. Config instead of registry path.
    bool PromptUserForVideoSettings(bool bStartFullscreen, int iAdapter, HINSTANCE hInstance, PathString & szArtPath, TRef<UpdatingConfiguration> config );
    #endif // _VIDEOSETTINGS_H_

## `src/engine/bounds.cpp`

PURPOSE: This file contains definitions and implementations for 2D and 3D bounds classes used to represent spatial regions in the game.

KEY PARTS:
- Bounds2: Represents a 2D spatial region defined by a rectangle.
- GetEmpty(): Returns an empty 2D bounds instance.
- Transform(const Matrix2& mat): Transforms the bounds using a 2x2 matrix.
- Union(const Bounds2& bounds1, const Bounds2& bounds2): Computes the union of two 2D bounds.
- Intersect(const Bounds2& bounds1, const Bounds2& bounds2): Computes the intersection of two 2D bounds.

CONNECTS TO: matrix.h (for Matrix2)

NOTES: The commented-out section at the bottom appears to be for a 3D bounds class that was not fully implemented.

## `src/engine/bounds.h`

PURPOSE: This file defines two classes, `Bounds2` and `Bounds`, which are used to represent and manipulate bounding boxes in a 2D and 3D space respectively.

KEY PARTS:
- Bounds2: Represents a 2D bounding box using a `Rect`.
  - Constructors: Default, from a `Rect`, and copy.
  - Methods: Transform, Union, Intersect, GetRect, SetRect.
- Bounds (commented out): Represents a 3D bounding box using minimum and maximum vectors.
  - Constructors: From min/max vectors, copy, default.
  - Methods: Transform, Union, GetMin, GetMax, GetCenter, GetDiagonal, GetRadius, SetVectors.

CONNECTS TO:
- rect.h (for the `Rect` class used by `Bounds2`)
- Matrix2 and Matrix (not shown in this snippet but referenced)

NOTES:
- The 3D `Bounds` class is commented out.
- There are no TODOs or hacks noted.

## `src/engine/bspgeo.cpp`

PURPOSE: This file contains the implementation of a BSP (Binary Space Partitioning) geometry class for rendering 3D models in Allegiance.

KEY PARTS:
- `BSPGeo`: Main class representing the BSP geometry.
- `Node`, `SplitNode`, `LeafNode`: Classes defining nodes in the BSP tree, including split and leaf nodes.
- `CreateSphereSlice` and `CreateSphereLayer`: Methods for creating specific parts of a sphere model.

CONNECTS TO: 
- `context.h` and `model.h` for rendering context and model data.

NOTES:
- Contains commented-out code for initializing a simple cube with colored vertices.
- The `CreateSphereSlice` method is currently unimplemented.

## `src/engine/bspgeo.h`

PURPOSE: tiny file (11 lines), shown in full rather than summarized.

    #ifndef _bspgeo_h_
    #define _bspgeo_h_
    #include <tref.h>
    class Geo;
    //////////////////////////////////////////////////////////////////////////////
    //
    // BSP Geo
    //
    //////////////////////////////////////////////////////////////////////////////
    TRef<Geo> CreateBSPGeo();
    #endif

## `src/engine/button.cpp`

**PURPOSE:** The file `src/engine/button.cpp` contains the implementation for various classes and functions related to button UI elements in the Allegiance game engine. These components handle rendering, state management, and event handling for buttons.

**KEY PARTS:**
- **ImageButtonFacePane**: Manages rendering of buttons with multiple faces from a single surface.
- **GenerateButtonVertices()**: Generates vertices for each button frame on the current surface.
- **CreateButtonFacePane(Surface*, DWORD, int, int)**: Creates an instance of `ImageButtonFacePane`.
- **PaneButtonFacePane**: Represents a pane with two faces (up and down), switching based on button state.
- **CreateButtonFacePane(Pane*, Pane*)**: Creates an instance of `PaneButtonFacePane`.
- **ButtonUIPane**: Manages the state and behavior of a button, including mouse events and state transitions.
- **SetDown(bool)**: Updates whether the button is currently pressed down.
- **ButtonPane**: Manages the state and behavior of a single button, handling various states like enabled, checked, and focus.
- **SetDown(bool bDown)**: Updates whether the button is pressed down and notifies the associated `facePane`.
- **SetInside(bool bInside)**: Updates whether the mouse pointer is inside the button.
- **SetFocus(bool bFocus)**: Updates whether the button has focus.
- **ButtonBarPaneImpl**: Manages a collection of buttons in a bar layout, handling events and state management.
- **InsertButton(ButtonPane* pPane, int nCmd)**: Adds a new button to the bar and sets up event handling.

**CONNECTS TO:** Other UI components and systems within the Allegiance game engine.

**NOTES:** 
- Contains TODOs for memory leak fixes and redundant code calls.
- No implementation for `DefaultUpdateLayout` function in the `ButtonPane` class.

## `src/engine/button.h`

PURPOSE: This file defines interfaces and implementations for button-related UI components in the Allegiance game engine.

KEY PARTS:
- ButtonPane: Base class for button panes with methods to set and get button properties like enabled state, checked status, repeat behavior, and event sources.
- ButtonFacePane: Class representing the visual appearance of a button, including different states like normal, pressed, focused, etc.
- CreateButtonFacePane: Functions to create instances of ButtonFacePane with various configurations.
- ButtonBarPane: Class for creating a bar of buttons that can be toggled or act as tabs.

CONNECTS TO: pane.h (for Pane base class), tvector.h (for TVector template), and other UI component classes.

NOTES: None.

## `src/engine/camera.cpp`

**PURPOSE:** The `camera.cpp` file in Allegiance manages camera operations, including updating camera perspectives and handling user inputs for movement and orientation.

**KEY PARTS:**
- **Class: Camera**: Manages basic camera properties and transformations.
  - Functions:
    - `Evaluate()`: Updates the perspective matrix based on camera settings.
    - `TransformLocalToEye(const Vector& vec)`: Transforms a local space vector to eye space.
    - `TransformEyeToImage(const Vector& vecEye, Point& point)`: Transforms an eye space vector to image space.

- **Class: MotionCamera**: Specialized camera controlled by keyboard and mouse inputs.
  - Functions:
    - `Evaluate()`: Updates the camera's position and orientation based on user input.
    - `MouseMove(IInputProvider* pprovider, const Point& point, bool bCaptured, bool bInside)`: Handles mouse movement for rotation.
    - `Button(IInputProvider* pprovider, const Point& point, int button, bool bCaptured, bool bInside, bool bDown)`: Handles mouse button events.

- **Function: CreateMotionCamera**: Factory function to create an instance of `MotionCamera`.

**CONNECTS TO:** 
- Other camera-related classes and functions for handling different types of camera behaviors.
- Input handling systems (`IInputProvider`) for user interaction.

**NOTES:** 
- The `Evaluate()` function in `Camera` has a placeholder comment about `m_focus` not being defined for orthogonal cameras.
- The `TransformEyeToImage` function checks if `vecEye.Z() >= 0`, which might indicate potential issues with camera orientation.

## `src/engine/camera.h`

PURPOSE: This file defines the Camera class and related functionalities for managing camera properties and transformations in the Allegiance game engine.

KEY PARTS:
- Camera: Manages camera position, orientation, and projection matrices.
- CreateMotionCamera: Creates a motion-controlled camera.
- ISmoothPosition: Interface for smoothly transitioning camera positions over time.
- CreateSmoothPosition: Factory function to create smooth position transitions.
- ISmoothOrientation: Interface for smoothly transitioning camera orientations over time.
- CreateSmoothOrientation: Factory function to create smooth orientation transitions.
- LookAtOrientation: Creates an orientation that makes the camera look at a specific point.

CONNECTS TO: matrix.h, value.h, IKeyboardInput.h, Image.h

NOTES: None.

## `src/engine/caption.cpp`

PURPOSE: This file contains the implementation of a caption interface for a window in Allegiance. It manages the creation and behavior of buttons within the caption area, such as close, restore, and minimize.

KEY PARTS:
- CaptionImpl: Implements the ICaption interface and handles button events.
- DoCreateButton: Creates a button with a specific image and offset.
- OnClose, OnRestore, OnMinimize: Event handlers for button clicks that notify the caption site of the respective action.
- SetFullscreen: Hides or shows buttons based on whether the window is in fullscreen mode.

CONNECTS TO:
- caption.h: Header file defining the ICaption interface.
- event.h: For handling events.
- button.h and model.h: For creating and managing button components.
- surface.h: For loading button images.

NOTES: None.

## `src/engine/caption.h`

PURPOSE: Defines interfaces and a factory function for creating caption-related objects in the Allegiance game engine.

KEY PARTS:
- ICaptionSite: Interface for classes that handle caption events like minimize, maximize, fullscreen, restore, and close.
- ICaption: Interface for classes that manage the caption state, including setting fullscreen mode.
- CreateCaption: Factory function to create an instance of ICaption.

CONNECTS TO: tref.h, Modeler, Pane

NOTES: None.

## `src/engine/connectionmap.xml`

PURPOSE: tiny file (2 lines), shown in full rather than summarized.

    <?xml-version ='1.0' encoding='UTF-8'?>
    <ConnectionMapList varName="connectionmaplist" />

## `src/engine/connections.xml`

PURPOSE: tiny file (2 lines), shown in full rather than summarized.

    <?xml-version ='1.0' encoding='UTF-8'?>
    <ConnectionDefList varName="connectiondeflist" />

## `src/engine/context.cpp`

PURPOSE: Manages rendering operations for the Allegiance game engine, including state management, drawing images and strings, handling decals, and setting up lighting.

KEY PARTS:
- Plane: Represents a plane in 3D space with methods for distance calculation and intersection.
- StateChangeBase: Base class for state change flags.
- StateChange: Bitmask representing various rendering states that can be changed.
- State: Manages the current rendering state including matrices, materials, textures, and other properties.
- ContextImpl: Main implementation of the drawing context with methods for managing rendering state, vertex buffers, and decals.
- DrawString: Renders a string at a specified point using a given font and color.
- DrawRectangle: Draws a rectangle with specified corners and color.
- Begin3DLayer: Begins rendering in 3D mode, setting up camera and lighting.
- RenderCallbackGeos: Renders a list of geometric callbacks.
- End3DLayer: Ends 3D rendering, restores previous state, and renders any remaining callbacks.

CONNECTS TO: Allegiance game engine systems for graphics rendering.

NOTES: None.

PARTS (in file order):

- lines 1-354
  - Plane: Represents a plane in 3D space with methods for distance calculation and intersection.
  - StateChangeBase: Base class for state change flags.
  - StateChange: Bitmask representing various rendering states that can be changed.
  - State: Manages the current rendering state including matrices, materials, textures, and other properties.
  - ContextImpl: Main implementation of the drawing context with methods for managing rendering state, vertex buffers, and decals.
  - WhiteSurfaceSite: Updates a surface to fill it with white color.
  - Decal: Represents a decal with position, color, scale, angle, and orientation vectors.
  - DecalSet: Groups decals by their associated surface.
- lines 355-715
  - ContextImpl: Manages rendering context and state.
  - BeginRendering(): Initializes rendering process.
  - EndRendering(): Finalizes rendering process.
  - DrawImage(Surface, const Rect&, bool, const Point&): Renders a 2D image on the screen.
  - DrawImage3D(Surface, const Rect&, const Color&, bool, const Point&): Renders a 3D image using textures.
  - IsCulled(const Vector&, float, bool&): Determines if a sphere is within the view frustum.
  - SetYAxisInversion(bool): Inverts the Y-axis for rendering.
  - GetClipping(): Retrieves clipping state.
- lines 716-1094
  - DrawString: Renders a string at a specified point using a given font and color.
  - DrawRectangle: Draws a rectangle with specified corners and color.
  - GetQuadHandle: Creates a vertex buffer for rendering a quad (rectangle) with texture coordinates.
  - FillRect: Fills a rectangle with a solid color.
  - FillInfinite: Fills the entire surface with a solid color.
  - Begin3DLayer: Begins rendering in 3D mode, setting up camera and lighting.
  - RenderCallbackGeos: Renders a list of geometric callbacks.
  - End3DLayer: Ends 3D rendering, restores previous state, and renders any remaining callbacks.
- lines 1096-1440
  - DrawCachedDecals: Draws cached decals.
  - PopState: Ends the current state and pops the previous state.
  - BeginScene: Begins a new scene for rendering.
  - EndScene: Ends the current scene.
  - SetLOD: Sets the level of detail.
  - GetLOD: Retrieves the current level of detail.
  - DoSetState: Template function to efficiently set device states with optional override.
  - ForceState: Forces all state changes to be applied immediately.
- lines 1441-1816
  - StateChangeWrapMode: Sets the texture wrap mode.
  - SetWrapMode: Applies the texture wrap mode to the device.
  - StateChangeCullMode: Sets the culling mode for polygons.
  - SetCullMode: Applies the culling mode to the device.
  - PushState: Saves the current state on a stack.
  - PopState: Restores the previous state from the stack.
  - Transform2D: Transforms a 2D point to image coordinates.
  - Clip: Clips drawing to a specified rectangle.
- lines 1818-2179
  - TransformEyeToImage: Transforms a vector from eye space to image space.
  - TransformDirectionToImage: Transforms a direction vector to image space.
  - TransformLocalToImage: Transforms a local vector to image space.
  - TransformImageToSurface: Converts an image point to a surface point.
  - GetImageRadius: Calculates the radius of an object in image space.
  - DirectionalLight: Sets up directional lighting.
  - BidirectionalLight: Sets up bidirectional lighting.
  - SetAmbientLevel: Sets the ambient light level.
- lines 2180-2474
  - DrawTriangles: Draws triangles using vertex and index buffers.
  - AddDecal: Adds a new decal to a surface.
  - DrawDecal: Draws a decal on a surface facing the eye point.
  - DrawVDecalSet: Draws all decals in a set.
  - DrawCachedDecals: Renders cached decals with different blend modes.
  - CreateContextImpl: Creates an implementation of a context for rendering.

## `src/engine/context.h`

**PURPOSE:** The `src/engine/context.h` file defines various classes and structures used for rendering in the Allegiance game engine. It includes data types for vertices, 3D vectors, and shading modes, as well as methods for drawing primitives and managing rendering states.

**KEY PARTS:**
- **Plane**: Represents a plane in 3D space.
- **MeshIndex**: Index type for vertex arrays.
- **Vertex**: Basic vertex structure with position, texture coordinates, and normal.
- **VertexL**: Extended vertex structure including color information.
- **D3DVector**: Enhanced version of `D3DVECTOR` with additional functionality.
- **D3DVertex**: Enhanced version of `D3DVERTEX` providing methods for accessing and modifying properties.
- **D3DLVertex**: Enhanced version of `D3DLVERTEX` adding color and specular color properties.
- **MakeD3DCOLOR**: Converts a `Color` object to a `D3DCOLOR`.
- **MakeColor**: Functions to convert between `COLORVALUE`, `D3DCOLOR`, and `Color`.
- **D3DTLVertex**: Extended version of `D3DTLVERTEX` with additional properties.
- **GetString(const D3DLVertex& v)**: Converts a `D3DLVertex` object to a string for debugging.
- **GetString(const D3DVertex& v)**: Similar function for `D3DVertex`.
- **VertexScreen**: Represents a vertex in screen space.
- **ShadeMode**, **BlendMode**, **WrapMode**, **CullMode**, **Counter**: Enumerations defining various rendering modes and properties.
- **IGeoCallback**: Interface for callback objects used during rendering.
- **Deformation**: Interface for objects that modify vertex positions.
- **Context**: Main drawing context class providing methods for setting states, transformations, lighting, clipping, and rendering primitives.

**CONNECTS TO:** Other rendering-related files and systems within the Allegiance engine.

**NOTES:** The file includes TODO comments but no specific details are provided. There is a conditional compilation directive that may affect certain parts of the code.

## `src/engine/controls.cpp`

PURPOSE: Manages various UI components and their layouts for a graphical user interface in Allegiance.

KEY PARTS:
- JustifyPane: Manages layout justification of its child pane.
- FillPane: Fills itself with a specified color.
- BorderPane: Adds a border around its child pane.
- EdgePane: Draws an edge around its child pane.
- ColumnPane: Arranges child panes vertically, optionally showing grid lines.
- RowPane: Arranges child panes horizontally, optionally showing grid lines.
- StringPane: Displays a string with optional clipping and justification.
- EditPaneImpl: Implements an editable text pane, supports password masking and read-only mode.
- AnimatedImagePane: Manages animated images within a UI component.
- ImagePane: Blits static images to the surface.
- TroughPane: Manages a scrollable pane with navigation controls.
- ScrollPaneImpl: Implementation class for scroll panes, handling events and layout updates.

CONNECTS TO: Allegiance's graphical user interface system.

NOTES:
- Contains TODOs for future enhancements.
- Uses smart pointers (TRef) for managing UI component lifetimes.

PARTS (in file order):

- lines 1-367
  - JustifyPane: Manages layout justification of its child pane.
  - FillPane: Fills itself with a specified color.
  - BorderPane: Adds a border around its child pane.
  - EdgePane: Draws an edge around its child pane.
  - ColumnPane: Arranges child panes vertically, optionally showing grid lines.
  - RowPane: Arranges child panes horizontally, optionally showing grid lines.
  - "!!! expand right first" in ColumnPane::UpdateLayout()
  - yextra = 0 if yextra < 0 in ColumnPane::UpdateLayout()
- lines 368-721
  - RowPane: Manages a row of panes with optional grid lines.
  - ShowGrid(bool bShowGrid): Sets whether to show grid lines and triggers layout and repaint.
  - UpdateLayout(): Calculates and updates the layout of child panes based on their sizes and visibility.
  - Paint(Surface psurface): Draws the grid lines if enabled.
  - StringPane: Displays a string with optional clipping and justification.
  - SetString(const ZString& str): Updates the displayed string and triggers layout and repaint.
  - UpdateLayout(): Adjusts size based on whether text is clipped or not.
  - EditPaneImpl: Implements an editable text pane, supports password masking and read-only mode.
- lines 723-1091
  - EditPaneImpl::SetString: Sets the string value and updates display.
  - EditPaneImpl::SetType: Changes the type of edit pane and updates display.
  - EditPaneImpl::SetMaxLength: Limits the length of the input string.
  - EditPaneImpl::OnChar: Handles character input, including backspace and numeric checks.
  - EditPaneImpl::Paint: Draws the text and cursor on the surface.
  - AnimatedImagePane::UpdateLayout: Updates layout based on image size.
  - AnimatedImagePane::Paint: Renders the image with transformations.
  - ImagePane::Paint: Blits the image to the surface.
- lines 1092-1452
  - TroughPane: Manages a scrollable pane with left and right buttons for navigation.
  - MakePoint: Function to create a WinPoint object.
  - MakePointFlipped: Function to create a flipped WinPoint object.
  - TroughPane::SetSizes: Updates the size of the trough and triggers layout and paint updates if necessary.
  - TroughPane::SetPos: Sets the position of the scroll thumb and triggers layout and paint updates if necessary.
  - TroughPane::UpdateLayout: Calculates and sets the layout for the scroll thumb, left, and right buttons.
  - ScrollPaneImpl: Implementation class for ScrollPane interface, implementing IEventSink.
- lines 1453-1706
  - ScrollPaneImpl: Main class implementing a scroll pane with left and right controls.
  - CreateScrollPane: Function to create a scroll pane with specified parameters.
  - OnEvent: Event handler for button presses on the scroll pane.
  - UpdateLayout: Method to update the layout of the scroll pane.
  - TODOs: "//new BorderPane(0, RGB(255, 0, 0))," "//new BorderPane(0, RGB(0, 255, 0)),"
  - Unusual code: Use of TRef for smart pointers. Creation of button panes and event sources within the scroll pane constructor.

## `src/engine/controls.h`

PURPOSE: This file contains definitions and declarations for various pane control classes used in the Allegiance game engine. These classes are responsible for managing different types of graphical elements within the game's user interface.

KEY PARTS:
- JustifyPane: Manages layout justification for child panes.
- FillPane: Fills a pane with a solid color.
- BorderPane: Adds borders to a pane and can change colors based on selection.
- EdgePane: Draws an edge around its child pane.
- ColumnPane: Arranges child panes in columns, optionally showing grid lines.
- RowPane: Arranges child panes in rows, optionally showing grid lines.
- StringPane: Displays text with various formatting options.
- EditPane: A text input field that can be configured for different types (normal, password, numeric).
- ImagePane: Displays an image.
- AnimatedImagePane: Displays an animated image within a specified rectangle.
- SurfacePane: Displays a surface object.
- ScrollPane: Provides scrolling functionality for its child pane.

CONNECTS TO: engine.h, model.h, pane.h, rect.h, event.h

NOTES: None.

## `src/engine/dderror.cpp`

PURPOSE: This file contains error handling and logging mechanisms for HRESULT values in Allegiance, specifically focusing on DirectDraw, DirectInput, and Windows errors.

KEY PARTS:
- `HRESULTInfo`: A class that maps HRESULT values to their corresponding error messages.
- `DDError`: A function that logs an error message if the HRESULT indicates a failure.
- `DDSCallImpl`: A function that handles specific HRESULT values related to surface operations.

CONNECTS TO: Allegiance's graphics and input subsystems, as well as Windows API functions for error handling.

NOTES:
- The file includes commented-out sections for Direct3D and Direct3DRM errors, which were likely part of the original codebase but are not currently used.
- The `g_bDumpDD` flag controls whether HRESULT errors are logged.

## `src/engine/ddstruct.h`

PURPOSE: This file contains definitions and utility classes for managing Direct3D-related structures and data in the Allegiance game engine.

KEY PARTS:
- TZeroFillWithSize<Type>: A template class that initializes a struct with a `dwSize` member to zero.
- D3D9PixelFormat: A class wrapping `D3D9PIXELFORMAT` and ensuring it is zero-initialized.
- D3DDeviceDescription: A class wrapping `D3DCAPS9` and ensuring it is zero-initialized.
- D3DRect: A class wrapping `D3DRECT` with additional constructors for easier initialization.
- D3DColorValue: A class wrapping `COLORVALUE` (renamed from `D3DCOLORVALUE`) that provides methods to convert between Allegiance's Color type and Direct3D color formats.

CONNECTS TO: Allegiance game engine, specifically related to rendering and graphics subsystems.

NOTES: None.

## `src/engine/device3d.cpp`

PURPOSE: Manages 3D rendering operations for the Allegiance game engine.

KEY PARTS:
- Device3D: Main class managing device-specific rendering operations.
- VertexTL: Represents a transformed and lit vertex with additional clipping information.
- ClipFront, ClipBack: Constants representing different clipping planes.
- PFNClip, PFNDraw, PFNVertex: Function pointers for various rendering stages.
- Has3DAcceleration(): Checks if 3D acceleration is available.
- BeginScene(), EndScene(): Start and end a rendering scene.
- ClearZBuffer(): Clears the depth buffer.
- SetYAxisInversion(bool): Inverts the Y-axis for clipping.
- SetClipRect(const Rect&): Sets the clipping rectangle and updates projection transform.
- DirectionalLight, BidirectionalLight: Configures directional and bidirectional lighting.
- SetAmbientLevel(float), SetGlobalColor(const Color&): Sets ambient light level and global color.

CONNECTS TO: CVBIBManager (vertex buffer management), Rasterizer interface for rendering.

NOTES:
- Bitwise operations used in plane clipping logic.
- TODOs for fixing typos, removing unused assembly code.

PARTS (in file order):

- lines 1-371
  - Device3D: Manages 3D rendering operations.
  - VertexTL: Represents a transformed and lit vertex with additional clipping information.
  - ClipFront, ClipBack, etc.: Constants representing different clipping planes.
  - PFNClip, PFNDraw, PFNVertex, etc.: Function pointers for various rendering stages.
  - m_prasterizer: Reference to the rasterizer interface used for rendering.
  - m_sizeSurface: Size of the surface being rendered.
  - m_rectClip: Clipping rectangle in screen coordinates.
  - m_bClip: Flag indicating whether clipping is enabled.
- lines 372-719
  - Device3D::Has3DAcceleration(): Checks if 3D acceleration is available.
  - BeginScene(), EndScene(): Start and end a rendering scene.
  - ClearZBuffer(): Clears the depth buffer.
  - SetYAxisInversion(bool): Inverts the Y-axis for clipping.
  - SetClipRect(const Rect&): Sets the clipping rectangle and updates projection transform.
  - DirectionalLight(const Vector&, const Color&), BidirectionalLight(const Vector&, const Color&, const Color&): Configures directional and bidirectional lighting.
  - SetAmbientLevel(float): Sets the ambient light level.
  - SetGlobalColor(const Color&): Sets a global color for rendering.
- lines 720-1060
  - Device3D::SetMatrix: Sets the model matrix and world transformation matrix.
  - Device3D::SetPerspectiveMatrix: Sets the perspective projection matrix.
  - Device3D::SetViewMatrix: Sets the view matrix.
  - Device3D::UpdatePointers: Updates function pointers based on current state.
  - Device3D::GetClipping: Returns whether clipping is enabled.
  - Device3D::GetMatrix: Returns the current model matrix.
  - Device3D::GetInverseModelMatrix: Returns the inverse of the model matrix.
  - Device3D::GetInverseModelWorldMatrix: Returns the inverse of the world transformation matrix.
- lines 1061-1433
  - SetClipping(bool bClip): Enables or disables clipping.
  - RemoveClipPlane(int indexRemove): Removes a clip plane at the specified index.
  - AddClipPlane(const Plane& plane): Adds a new clip plane in eye coordinates.
  - SetShadeMode(ShadeMode shadeMode): Sets the shading mode for rendering.
  - SetMaterial(Material pmaterial): Sets the material to be used for rendering.
  - SizeVertexLBuffer(int count): Resizes the vertex buffer for long vertices.
  - SizeVertexTLBuffer(int count): Resizes the vertex buffer for transformed and lit vertices.
  - SizeVertexScreenBuffer(int count): Resizes the vertex buffer for screen-space vertices.
- lines 1434-1791
  - ClipPolygonToPlanes: Clips a polygon to a set of planes.
  - ClipPolygonToPlane: Clips a polygon to a single plane.
  - ClipTriangles: Clips triangles based on their vertices and indices.
  - ClipLineToPlanes: Clips a line to a set of planes.
  - ClipLines: Clips lines based on their vertices and indices.
  - TODOs: "// Additional guard to make sure we don't exceed array bounds in the next loop" "// Completely clipped"
  - Unusual code: The use of bitwise operations for plane clipping logic.
- lines 1792-2139
  - ClipPoints: Clips points based on their clip codes.
  - TransformDeformClip: Transforms vertices with deformation and clipping.
  - TransformClip: Transforms vertices with clipping.
  - TransformAndLightNoClip: Transforms and lights vertices without clipping.
  - TODOs: Fix typo in MakeInt function call. Consider removing unused assembly code if not needed.
- lines 2140-2510
  - TransformNoClip: Transforms vertices without clipping.
  - TransformNoClipNoLight: Transforms vertices without clipping or lighting.
  - LightVertexCopy: Copies lighting data to a new buffer and transforms it.
  - LightVertexGlobalColor: Applies global color to vertices and then transforms them.
  - BidirectionalLightVertex: Applies bidirectional lighting to vertices and then transforms them.
  - LightVertex: Applies directional lighting to vertices and then transforms them.
  - MaterialLightVertex: Applies material-based lighting to vertices and then transforms them.
  - StoreVertex: Stores vertex data in a structured format.
- lines 2511-2792
  - Device3D: Main class for managing device-specific rendering operations.
  - DrawTriangles: Function to draw triangles using different vertex formats and hardware acceleration paths.
  - DrawLines: Function to draw lines using different vertex formats and hardware acceleration paths.
  - DrawPoints: Function to draw points using different vertex formats and hardware acceleration paths.
  - DrawTriangles(const D3DPRIMITIVETYPE, const CVBIBManager::SVBIBHandle, const CVBIBManager::SVBIBHandle): Hardware-accelerated path for drawing indexed primitive lists using vertex and index buffers.
  - DrawTriangles(const D3DPRIMITIVETYPE, const DWORD, const CVBIBManager::SVBIBHandle): Hardware-accelerated path for drawing primitive lists using vertex buffers.
  - CreateDevice3D: Factory function to create an instance of Device3D.

## `src/engine/dinput.cpp`

PURPOSE: Handles input devices such as mice and joysticks using DirectInput API.

KEY PARTS:
- DIDeviceCaps: Represents capabilities of a DirectInput device.
- DDInputObject: Base class for input objects.
- JoystickInputStreamImpl: Manages joystick input and force feedback.
- InputEngineImpl: Implements the InputEngine interface for handling input devices.

CONNECTS TO: Allegiance game engine, other input-related files.

NOTES: None.

PARTS (in file order):

- lines 1-366
  - DIDeviceCaps: Represents capabilities of a DirectInput device.
  - DIDeviceInstance: Represents an instance of a DirectInput device.
  - DIDataFormat: Represents data format for DirectInput devices.
  - DIObjectDataFormat: Represents object data format for DirectInput devices.
  - g_pdfDIMouse: Global pointer to the mouse data format.
  - EFFECTS_NODE: Node in linked list for storing DirectInput effects.
  - DDInputObject: Base class for input objects.
  - ValueDDInputObject: Derived class for value-based input objects.
- lines 367-736
  - CalculateDelta: Calculates the delta value based on input and sensitivity settings.
  - DeltaPosition: Updates mouse position based on input deltas.
  - DeltaWheel: Handles mouse wheel movement.
  - ButtonChanged: Updates button state and triggers events.
  - UpdateBuffered: Reads buffered input data from DirectInput device.
  - UpdatePolled: Polls the DirectInput device for current state.
  - Update: Main update function to acquire device and call appropriate update method.
  - SetSensitivity and SetAccel: Set mouse sensitivity and acceleration.
- lines 738-1114
  - JoystickInputStreamImpl: Manages joystick input and force feedback.
  - EnumObjectsCallback: Callback function to enumerate device objects.
  - SetRanges: Sets properties for joystick axes and buttons.
  - Update: Polls the joystick and updates values.
  - SetFocus: Acquires or releases the joystick based on focus state.
  - EnumEffectTypeProc: Enumerates effect types.
  - EnumAndCreateBounceEffectsCallback: Creates force feedback effects from files.
- lines 1115-1468
  - EFFECTS_NODE: Represents a node in a linked list of DirectInput effects.
  - EnumAndCreateBounceEffectsCallback, EnumAndCreateFireEffectsCallback, EnumAndCreateExplodeEffectsCallback: Callback functions for enumerating and creating DirectInput effects from files.
  - PlayFFEFromFile: Plays all effects in a given linked list.
  - EmptyEffectLists: Frees all effects in a given linked list.
  - CreateEffects: Initializes and creates various force feedback effects.
  - PlayFFEffect: Plays a specific force feedback effect based on an ID.
  - HasForceFeedback: Checks if the joystick has force feedback capabilities.
  - GetShortDescription, GetDescription, GetValueDescription: Methods to retrieve descriptions of joystick inputs.
- lines 1470-1741
  - InputEngineImpl: Implements the InputEngine interface for handling input devices.
  - EnumDeviceCallback: Callback function to enumerate DirectInput devices and create appropriate InputStream objects.
  - StaticEnumDeviceCallback: Static wrapper around EnumDeviceCallback to pass context.
  - InputEngineImpl::InputEngineImpl: Constructor initializes DirectInput, enumerates devices, and sets up logging.
  - EnumerateJoysticks: Method to enumerate and initialize joystick devices.
  - GetJoystickCount: Returns the number of joystick devices.
  - GetJoystick: Retrieves a specific joystick device by index.
  - GetMouse: Retrieves the mouse input stream.

## `src/engine/engine.cpp`

PURPOSE: The file `engine.cpp` contains the implementation of the engine's core functionalities related to rendering and device management.

KEY PARTS:
- EngineImpl: Manages the private engine interface.
- SurfaceList: Stores a list of surfaces used for rendering.
- CDX9EngineSettings: Holds settings specific to DirectX 9 engine configurations.
- SetGammaLevel: Adjusts the gamma level for display output.
- CreatePrimarySurface: Initializes the primary surface for rendering operations.
- InitializeFullscreen: Attempts to initialize fullscreen mode with various resolutions.
- UpdateSurfacesPixelFormat: Updates the pixel format of surfaces as needed.
- DeviceOK: Validates and initializes the device based on fullscreen mode requirements.
- Flip: Swaps the back buffer with the front buffer for rendering output.

CONNECTS TO: DirectX 9 libraries, windowing system (e.g., `g_hwndMainWindow`), and other engine components.

NOTES:
- Contains commented-out sections related to previous functionality that has been removed or disabled.

PARTS (in file order):

- lines 1-359
  - EngineImpl: Implements the private engine interface.
  - SurfaceList: A list of surfaces.
  - CDX9EngineSettings: Represents DirectX 9 engine settings.
  - g_hwndMainWindow: Global handle to the main window.
  - m_bValid: Indicates if the engine is valid.
  - m_bValidDevice: Indicates if the device is valid.
  - m_ppf: Pixel format for surfaces.
  - m_sizeResolution: Resolution of the size.
- lines 360-737
  - SetGammaLevel: Sets the gamma level for the display.
  - GetGammaLevel: Retrieves the current gamma level.
  - CreatePrimarySurface: Creates a primary surface for rendering.
  - InitializeWindowed: Initializes the windowed mode for the game.
  - SwitchToFullscreenDevice: Switches to fullscreen mode with specified resolution and refresh rate.
  - TODO: The commented-out code related to fullscreen initialization and device switching needs review.
  - Unusual Code: The commented-out sections suggest that there was previous functionality for handling different display modes and cooperative levels, which has been removed or disabled.
- lines 738-1085
  - UpdateSurfacesPixelFormat(): Updates pixel format of surfaces.
  - InitializeFullscreen(bool& bChanges): Initializes fullscreen mode by trying different resolutions until a valid one is found.
  - DebugSetWindowed(): Sets windowed mode (commented out).
  - ForceReset(): Forces a reset of the device, clearing screen and rendering finished.
  - SetMaxTextureSize(int iMaxTextureSize): Sets maximum texture size and forces a reset if changed.
  - SetVSync(bool bVsync): Enables or disables vertical synchronization and forces a reset if changed.
  - SetAA(DWORD dwAA): Sets anti-aliasing level and forces a reset if changed.
  - SetAutoGenMipMaps(bool bUseAutoGenMipMaps): Enables or disables automatic mip-map generation and forces a reset if changed.
- lines 1086-1453
  - DeviceOK: Validates and initializes the device based on fullscreen mode.
  - IsDeviceReady: Checks if the device is ready to render, handling various states like lost or not reset.
  - Flip: Flips the back buffer to the front buffer for rendering.
  - BltToWindow: Blits a surface to a window at a specified position and rectangle.
  - GetPixelFormat: Retrieves a pixel format based on given parameters, caching results for efficiency.
  - GetPrimaryPixelFormat: Returns the primary pixel format used by the device.
  - RemovePrivateSurface: Removes a private surface from the cache and cleans up associated resources.
  - TODO: The commented-out code sections suggest potential optimizations or features that were not implemented.
- lines 1454-1710
  - CreateDummySurface: Creates a dummy surface with valid dimensions but no VRAM resources.
  - CreateRenderTargetSurface: Creates a render target surface.
  - CreateSurface: Creates a device format surface based on given parameters.
  - CreateCompatibleSurface: Creates a compatible surface from an existing one.
  - CreateSurface(HBITMAP): Creates a surface from an HBITMAP, currently commented out.
  - CreateSurface(WinPoint, PixelFormat, int, BYTE, IObject, bool, Color, ZString, bool): Creates a surface with custom pixel data and format.
  - CreateSurfaceD3DX: Creates a surface using D3DXIMAGE_INFO, currently commented out.
  - TODOs: Fix memory leak in CreateEngine Remove commented-out code related to bitmap handling

## `src/engine/engine.h`

PURPOSE: This file defines the interface and implementation for the Allegiance game engine's core functionality, including initialization, rendering, and surface management.

KEY PARTS:
- Engine: Main class defining the core methods for managing the game engine, such as setting focus, terminating, and handling device readiness.
- Surface: Class representing a graphical surface that can be drawn on.
- JustificationBase/Justification: Classes for defining text justification options.

CONNECTS TO: mask.h, rect.h, tref.h, vector.h, surface.h, value.h, valuetransform.h

NOTES: None.

## `src/engine/engineapp.cpp`

PURPOSE: This file contains the implementation of the `EngineApp` class, which is responsible for initializing and managing the Allegiance game engine.

KEY PARTS:
- `Initialize`: Initializes the engine, modeler, and popup container based on command line parameters.
- `Terminate`: Cleans up resources by terminating the engine and modeler.
- `ParseCommandLine`: Parses command line arguments to determine device options like software rendering, primary display, and bit depth.
- `SetMouse`: Sets the mouse input stream for the application.
- `OnException`: Handles exceptions by switching to windowed mode if in fullscreen and disabling the mouse.
- `IsBuildDX9`: Indicates that DirectX 9 is being used.

CONNECTS TO: `engine.h`, `enginep.h`, `inputengine.h`, `model.h`, `pane.h`, `popup.h`

NOTES: None.

## `src/engine/engineapp.h`

PURPOSE: This header file defines the `EngineApp` class, which is a subclass of `Win32App`. It manages the main application logic for Allegiance, including initializing and terminating the engine, modeler, popup container, and mouse input.

KEY PARTS:
- EngineApp: Manages the main application logic.
- GetEngine: Returns a pointer to the engine instance.
- GetModeler: Returns a pointer to the modeler instance.
- GetPopupContainer: Returns a pointer to the popup container instance.
- SetMouse: Sets the mouse input stream.
- Initialize: Initializes the application with command line parameters and creates necessary components.
- Terminate: Cleans up resources before exiting the application.
- OnException: Handles exceptions that occur during runtime.
- IsBuildDX9: Checks if the build is for DirectX 9.

CONNECTS TO:
- Win32App: Inherits from this base class to utilize Windows-specific functionality.
- Engine, Modeler, IPopupContainer, MouseInputStream: These classes are managed and interacted with by `EngineApp`.

NOTES:
- None.

## `src/engine/enginep.h`

PURPOSE: This header file contains declarations for various engine-related classes and functions used in the Allegiance game engine.

KEY PARTS:
- `Rasterizer`: Interface for rendering 2D and 3D graphics.
- `PrivateSurface`: Represents a surface that can be rendered on, including back buffers and render targets.
- `PrivateEngine`: Manages the overall state of the engine, including surfaces and devices.
- `CreateD3D9Rasterizer`, `CreateDevice3D`, `CreateContextImpl`, `CreatePrivateDummySurface`, etc.: Functions to create instances of various engine components.

CONNECTS TO: DirectDraw and DirectX libraries for graphics rendering.

NOTES:
- Contains global hacks like `g_bMDLLog` and `g_bWindowLog`.
- Some DirectX-related code is commented out, indicating it might be unused or replaced by newer APIs.

## `src/engine/enginewindow.cpp`

PURPOSE: Manages the main window of Allegiance, handling creation, rendering, input, and state changes.

KEY PARTS:
- EngineWindow: Manages the main game window lifecycle and rendering.
- MenuCommandSink: Handles menu commands for engine settings.
- UpdateSurfacePointer: Adjusts surface pointer based on fullscreen/windowed mode.
- SetFullscreen: Switches between fullscreen and windowed modes.
- RenderFrame: Handles frame rendering, including clipping and performance counters.
- OnPaint: Processes painting events for the window.
- HandleMouseMessage: Manages mouse input processing.
- UpdateInput: Updates input states like mouse position.

CONNECTS TO: Allegiance engine systems, particularly those related to graphics, input handling, and user interface elements.

NOTES: None.

PARTS (in file order):

- lines 1-376
  - EngineWindow: Manages the main window of the game.
  - MenuCommandSink: Handles menu commands for the engine window.
  - s_pmodes: Array defining available screen modes.
  - EngineWindowKeyboardInput: Filters keyboard input to handle specific keys like F4, Alt+Enter, etc.
  - EngineWindow::EngineWindow: Constructor initializing various components and settings.
  - EngineWindow::~EngineWindow: Destructor cleaning up resources.
  - PostWindowCreationInit: Initializes window-specific components after creation.
  - StartClose: Posts a close message to the window.
- lines 377-741
  - EngineWindow::UpdateSurfacePointer: Updates the surface pointer based on whether the game is in fullscreen or windowed mode.
  - EngineWindow::UpdateWindowStyle: Adjusts the window style and properties when switching between fullscreen and windowed modes.
  - EngineWindow::UpdateRectValues: Updates the screen and render rectangle values based on the current mode (fullscreen or windowed).
  - EngineWindow::SetMoveOnHide: Sets whether the window should move when hidden.
  - EngineWindow::UpdateCursor: Manages the visibility and position of the cursor in both fullscreen and windowed modes.
  - EngineWindow::GetScreenRectValue: Returns a pointer to the screen rectangle value object.
  - EngineWindow::GetRenderRectValue: Returns a pointer to the render rectangle value object.
  - EngineWindow::SetFullscreen: Switches the game between fullscreen and windowed modes.
- lines 742-1097
  - EngineWindow: Manages the main window and rendering loop.
  - GetFullscreenSize: Returns the size of the fullscreen mode.
  - SetFullscreenSize: Sets the size of the fullscreen mode and triggers a render size change.
  - ChangeFullscreenSize: Adjusts the fullscreen resolution based on user input.
  - GetEngineMenu: Creates a menu for engine settings such as brightness, resolution, and device information.
  - UpdatePerformanceCounters: Updates performance counters and outputs them to debug strings or renders them on the screen.
  - RenderFrame: Handles rendering of the frame by beginning and ending scenes, clipping, setting blend modes, rendering images, updating performance counters, and rendering performance counters.
  - OnPaint: Handles painting events for the window.
- lines 1098-1460
  - EngineWindow::ShouldDrawFrame: Determines if a frame should be drawn based on fullscreen mode and window state.
  - EngineWindow::DoIdle: Handles idle processing including input updates, device readiness checks, and rendering.
  - EngineWindow::SetShowFPS: Configures whether to show FPS and sets the label for it.
  - EngineWindow::ToggleShowFPS: Toggles the display of FPS between off and on states.
  - EngineWindow::OnActivate: Updates window activation state and focus.
  - EngineWindow::OnActivateApp: Handles application-wide activation events.
  - EngineWindow::SetCursorImage: Sets the cursor image for the window.
  - EngineWindow::GetCursorImage: Retrieves the current cursor image.
- lines 1461-1679
  - EngineWindow: Manages the window and handles mouse events.
  - HandleMouseMessage: Processes different mouse messages like button clicks and wheel movements.
  - OnEvent: Handles button state changes by calling HandleMouseMessage.
  - UpdateInput: Updates input states, including mouse position.
  - SetCaption: Sets the caption of the window.
  - OnCaptionMinimize: Minimizes the window.
  - OnCaptionMaximize: Maximizes the window.
  - OnCaptionFullscreen: Toggles fullscreen mode.

## `src/engine/enginewindow.h`

PURPOSE: The `enginewindow.h` file defines the `EngineWindow` class, which represents a window in Allegiance that is associated with an engine object. It handles various aspects of window management, input processing, and rendering.

KEY PARTS:
- `EngineWindow`: Main class representing a window with an associated engine.
- `ModeData`: Stores data related to different display modes.
- `MenuCommandSink`: Handles menu command events for the window.
- `UpdatePerformanceCounters`: Updates performance counters for FPS and other metrics.
- `RenderFrame`: Renders the current frame of the game.
- `HandleMouseMessage`: Processes mouse messages.
- `ParseCommandLine`: Parses command-line arguments to configure the window.

CONNECTS TO: Other classes such as `Engine`, `InputEngine`, `Menu`, and `Configuration`.

NOTES: None.

## `src/engine/font.cpp`

**PURPOSE:** The `font.cpp` file in Allegiance is responsible for managing and rendering fonts within the game. It handles the creation of font textures, character data processing, and rendering text at various positions with specified colors.

**KEY PARTS:**
- **Class: FontImpl**: Manages font rendering by handling character data, texture generation, and drawing.
- **Function: GenerateFontTextureFromFontData()**: Generates a texture for the font using bitmapped data, dynamically adjusting its size to fit all characters within a maximum limit (2048x2048 pixels).
- **Function: FillFontData(HFONT hfont)**: Fills character data with bitmap information from the provided `HFONT` using GDI functions.
- **Function: GetSizes(HFONT hfont)**: Retrieves and validates the dimensions of each character in the font.
- **Function: DrawString(const Point &point, const ZString &str, const Color &colour, bool bYAxisInversion)**: Renders a string at a specified point with the provided color, handling text alignment and nearest-point sampling for optimal rendering quality.
- **Function: DrawStringScreenSpace**: Renders a string at a specified screen point with optional color.
- **Function: DrawString**: Renders a string in a world space coordinate system.
- **Function: HexDigitToInt**, **ReadHexPair**, **IsValidColourCode**, **ReadColourD3D**: Helper functions for parsing and converting color codes from strings to Direct3D color values.
- **Function: CreateEngineFont(HFONT hfont)**, **CreateEngineFont(IBinaryReaderSite* psite)**: Factory functions to create instances of `FontImpl`.
- **Class: FontIdentifier**: Used to uniquely identify a font by its properties (name, size, stretch, etc.).
- **Variable: mapFontCache**: A cache of created `FontImpl` instances indexed by their `FontIdentifier`.

**CONNECTS TO:** Other rendering systems and components within the game engine.

**NOTES:** The code includes assertions to ensure character sizes are within expected ranges, suggesting potential issues if characters exceed these limits. There is a comment indicating that color handling logic may need adjustment when entering text from the client.

## `src/engine/font.h`

PURPOSE: This file defines an interface for a font engine in the Allegiance game engine, providing methods to measure and draw text.

KEY PARTS:
- IEngineFont: Interface class defining methods for font operations such as measuring text extent, drawing strings, and getting font height.
- GetString: Function to convert a FontValue to a string representation.
- Write: Function to write a FontValue to an MDL binary file.
- GetFunctionName: Function to get the function name associated with a FontValue.

CONNECTS TO: zlib\color.h for color codes, likely used in conjunction with text drawing operations.

NOTES: None.

## `src/engine/frameimage.cpp`

PURPOSE: The file `frameimage.cpp` contains the implementation of a class for managing animated images in Allegiance. It includes methods for loading and rendering frames, as well as handling frame transitions.

KEY PARTS:
- FrameImage: Manages an animated image with multiple frames.
- ExtractRLEFrame: Decompresses RLE (Run-Length Encoding) data to create individual animation frames.
- PlayRLE: Handles the decompression of RLE data.
- FrameImageButtonPaneImpl: A pane that displays an animated image and handles mouse events.

CONNECTS TO:
- Other classes in Allegiance's engine, such as `Image`, `Surface`, and `EventSource`.

NOTES:
- The file includes a factory class `FrameImageFactory` for creating instances of `FrameImage`.
- There are TODO comments indicating potential improvements or fixes.
- The code uses Microsoft DirectX functions for texture management.

## `src/engine/frameimage.h`

PURPOSE: This file defines interfaces and factory methods for creating frame-based images and image button panes in the Allegiance game engine.

KEY PARTS:
- FrameImage: Represents a frame-based image with RLE compression.
- CreateFrameImageFactory: Factory function to create instances of FrameImage.
- FrameImageButtonPane: Extends Pane to represent an image button pane with event sources for mouse enter and leave events.
- CreateFrameImageButtonPane: Factory function to create instances of FrameImageButtonPane.

CONNECTS TO: Depends on classes like Image, Surface, IFunction, and Pane.

NOTES: None.

## `src/engine/geometry.cpp`

PURPOSE: The file `geometry.cpp` contains implementations for various geometry-related classes and functions used in rendering and managing geometric objects in Allegiance.

KEY PARTS:
- EmptyGeo: Represents an empty geometry object.
- Geo: Base class for all geometry objects, providing default implementations for various methods.
- MeshGeo: Abstract base class for mesh-based geometry.
- TMeshGeo<VertexType>: Template class representing a specific type of mesh geometry.
- WrapGeo: Base class for various geometry transformations and effects.
- DecalGeo: Represents a decal applied to an image at a specific position and orientation.
- VisibleGeo: Controls visibility of a wrapped geometry.
- ClipGeo: Clips rendering based on a boolean flag.
- TransformGeo: Applies transformations to a geometry.
- MaterialGeo: Applies materials to a geometry.
- TextureGeo: Manages texture application and folding.
- TimedGeo: Represents a geometry that has a lifespan and evaluates whether to trigger based on time.
- GroupGeoImpl: Implements the GroupGeo interface with additional functionality for managing child geometries.
- LODGeoImpl: Implements level-of-detail (LOD) functionality for geometric objects, managing different levels based on radius.

CONNECTS TO: Other rendering systems within Allegiance.

NOTES:
- Use of TRef for smart pointers to manage memory automatically.

PARTS (in file order):

- lines 1-350
  - EmptyGeo: Represents an empty geometry object.
  - Geo: Base class for all geometry objects, providing default implementations for various methods.
  - MeshGeo: Abstract base class for mesh-based geometry.
  - TMeshGeo<VertexType>: Template class representing a specific type of mesh geometry.
  - GetString(Vertex): Converts a vertex to a string representation.
  - GetString(VertexL): Converts a prelit vertex to a string representation.
  - Render(Context pcontext): Renders the mesh geometry using the provided context.
  - CallGroupGeoCallback(const Matrix& mat, GroupGeoCallback pcallback): Calls a callback function for each triangle in the mesh.
- lines 351-711
  - TMeshGeo: Represents a mesh geometry with vertices and indices.
  - ApplyTransform: Applies a transformation to the mesh's vertices and normals.
  - GetTriangleCount: Returns the number of triangles in the mesh.
  - GetRadius: Calculates the radius of the mesh after applying a transformation.
  - GetString: Generates a string representation of the mesh for debugging or logging.
  - IsConstant: Indicates that the geometry is constant and does not change over time.
  - GetFunctionName: Returns the name of the function, which is "MeshGeo".
  - Write: Writes the mesh data to a binary file.
- lines 713-1077
  - WrapGeo: Base class for various geometry transformations and effects.
  - DecalGeo: Represents a decal applied to an image at a specific position and orientation.
  - VisibleGeo: Controls visibility of a wrapped geometry.
  - ClipGeo: Clips rendering based on a boolean flag.
  - ClipPlaneGeo: Clips rendering using a plane.
  - TransformGeo: Applies transformations to a geometry.
  - MaterialGeo: Applies materials to a geometry.
  - TextureGeo: Manages texture application and folding.
- lines 1079-1449
  - TextureGeo::Render: Renders a texture geometry using a context.
  - TimedGeo: Represents a geometry that has a lifespan and evaluates whether to trigger based on time.
  - GroupGeoImpl: Implements the GroupGeo interface with additional functionality for managing child geometries.
  - GroupGeoImpl::FoldTexture: Folds textures of child geometries into a single texture if possible.
  - GroupGeoImpl::Fold: Removes empty geometries, folds child groups, combines meshes, and determines whether to fold this node based on children count.
  - TODOs: "!!! should this be push end?"
  - Unusual code: Use of TRef for smart pointers, which is a common pattern in C++ for managing memory automatically.
- lines 1450-1822
  - GroupGeoImpl: Manages a group of geometric objects and provides methods for rendering, hit testing, and other operations.
  - LODGeoImpl: Implements level-of-detail (LOD) functionality for geometric objects, managing different levels based on radius.
  - AddGeo: Adds a geometric object to the group at the correct position based on its radius.
  - GetTriangleCount: Calculates the total number of triangles across all child geometries.
  - GetRadius: Determines the maximum radius of all child geometries when transformed by a given matrix.
  - Render: Renders the appropriate level of detail based on the current LOD setting in the context.
  - RemoveTextures: Creates a new LODGeo with only the textures removed from each child geometry.
  - HitTest: Checks if any child geometry intersects with a ray originating from a given point.
- lines 1823-2029
  - LODGeoImpl: Implementation of a level-of-detail geometry class.
  - Duplicate: Creates a duplicate of a Geo object.
  - Fold: Reduces the number of children to one if possible.
  - GetFunctionName: Returns the name of the function as "LODGeo".
  - GetString: Generates a string representation of the LODGeo object.
  - Write: Writes the LODGeo object to an MDL binary file.
  - LODGeo::Create: Factory method for creating a new LODGeo instance.
  - CallbackGeo: A callback geometry class that wraps another Geo object.

## `src/engine/geometry.h`

**PURPOSE:** This file contains definitions for various geometric objects and transformations used in the Allegiance game engine. These classes represent different types of geometry operations such as wrapping, clipping, transforming, and applying materials.

**KEY PARTS:**
- **WrapGeo:** A base class that wraps another Geo object, allowing modifications or enhancements.
- **DecalGeo:** Represents a decal (a 2D image applied to a 3D surface).
- **VisibleGeo:** Controls the visibility of another Geo object.
- **ClipGeo:** Clips rendering based on a boolean flag.
- **ClipPlaneGeo:** Clips rendering based on a plane.
- **TransformGeo:** Applies transformations to another Geo object.
- **MaterialGeo:** Applies materials to another Geo object.
- **CullModeGeo:** Controls the culling mode for rendering.
- **TextureGeo:** Applies textures to another Geo object.
- **TimedGeo:** Renders a Geo object only within a specified lifespan.
- **GroupGeo:** A container for multiple Geo objects, allowing them to be grouped and managed together.
- **LODGeo:** Represents Level of Detail (LOD) geometry, which can switch between different levels based on distance.
- **CallbackGeo:** Allows the execution of custom rendering callbacks.

**CONNECTS TO:** `event.h`, `matrix.h`, `material.h`, `model.h`, `transform.h`

**NOTES:** None.

## `src/engine/image.cpp`

PURPOSE: The file `image.cpp` contains various classes and functions related to image handling in Allegiance, including creating, manipulating, and rendering images.

KEY PARTS:
- EmptyImage: Represents an empty image.
- Image: Base class for all images, providing common functionality.
- ConstantImage: Represents a constant image with a surface and optional color.
- StringImage: Represents a string-based image that can be rendered as text.
- GaugeImage: Displays a gauge based on a number value, which can be 3D or flat.
- AnimatedImage: Manages an animated image by breaking it into frames from a source surface.
- WrapImage: Wraps another image, handling mouse events and capturing them if necessary.
- UndetectableImageImpl: A class that wraps an Image and implements IMouseInput methods to make the image undetectable.
- TransformImage: Class that applies a transformation to an Image, including translation, rotation, scaling, etc.
- GroupImage: Class that groups multiple Images together and renders them as a single entity. It also handles mouse events for the group.
- GeoImage: Represents a 3D image with lighting and rendering capabilities.
- SwitchImageImpl: Manages multiple images based on a numeric value.
- ColorImage: Represents an image with a single color.
- ExtentImage: Represents an image with a specified extent and color.

CONNECTS TO: Other systems or files it depends on or serves, one line. None

NOTES: Contains TODOs for fixing NaN & infinity handling in GaugeImage rendering and ensuring proper bounds calculation in WrapImage.

PARTS (in file order):

- lines 1-372
  - EmptyImage: Represents an empty image.
  - Image: Base class for all images, providing common functionality.
  - ConstantImage: Represents a constant image with a surface and optional color.
  - ConstantImage3D: Represents a 3D constant image with a surface and color.
  - StringImage: Represents a string-based image that can be rendered as text.
  - GetString(int indent): Returns the string representation of an image, useful for debugging or logging.
  - GetFunctionName(): Returns the function name associated with an image, typically used in scripting or configuration files.
  - Write(IMDLBinaryFile pfile): Writes the image to a binary file, likely for saving game state or configurations.
- lines 373-733
  - StringImage: Represents an image that displays a string with specified justification and color.
  - GaugeImage: Displays a gauge based on a number value, which can be 3D or flat.
  - AnimatedImage: Manages an animated image by breaking it into frames from a source surface.
  - WrapImage: Wraps another image, handling mouse events and capturing them if necessary.
  - CreateStringImage: Factory function to create instances of StringImage.
  - CreateGaugeImage: Factory function to create instances of GaugeImage.
  - AnimatedImage::AnimatedImage(Number ptime, Surface psurfaceSource, int nRows, int nCols): Constructor for creating an animated image from a source surface and dimensions.
  - TODOs: Fix NaN & infinity handling in GaugeImage rendering. Ensure proper bounds calculation in WrapImage.
- lines 734-1102
  - UndetectableImageImpl: A class that wraps an Image and implements IMouseInput methods to make the image undetectable.
  - CreateUndetectableImage: Function to create an instance of UndetectableImageImpl.
  - TransformImage: Class that applies a transformation to an Image, including translation, rotation, scaling, etc.
  - TranslateImage: A subclass of TransformImage that translates an Image by a specified offset.
  - GroupImage: Class that groups multiple Images together and renders them as a single entity. It also handles mouse events for the group.
  - HitTest: Method in GroupImage to determine which image within the group is being interacted with.
  - RemoveCapture: Method in GroupImage to release capture from any currently captured Image.
  - Button: Method in GroupImage to handle button press events for images within the group.
- lines 1103-1485
  - GeoImage: Represents a 3D image with lighting and rendering capabilities.
  - SwitchImageImpl: Manages multiple images based on a numeric value.
  - CreateSwitchImage: Factory function to create a SwitchImageImpl instance.
  - GeoImage2D: Represents a 2D image using a Geo object.
  - CreateGeoImage2D: Factory function to create a GeoImage2D instance.
  - ClipImage: Clips an image to a specified rectangle.
  - CreateClipImage: Factory function to create a ClipImage instance.
  - PickImageImpl: Allows picking an image by clicking, triggering events.
- lines 1486-1832
  - ColorImage: Represents an image with a single color.
  - ExtentImage: Represents an image with a specified extent and color.
  - TransparentImage: Represents an invisible image.
  - VisibleImage: Wraps another image and makes it visible or invisible based on a boolean value.
  - JustifyImage: Wraps another image and justifies its position based on a justification type.
  - MDLFileImageImpl: Loads an image from an MDL file.
  - CreateColorImage: Factory function to create a ColorImage object.
  - CreateExtentImage: Factory function to create an ExtentImage object.

## `src/engine/image.h`

PURPOSE: This file defines various image-related classes and functions used in the Allegiance game engine for rendering and manipulating images.

KEY PARTS:
- VideoImage: Base class for video-based images.
- ConstantImage: Represents an image with a constant surface and string value.
- AnimatedImage: Handles animation of multiple surfaces over time.
- WrapImage: A wrapper class that can hold another image and modify its behavior or appearance.
- SwitchImage: Allows switching between different images based on a numeric value.
- TransformImage: Applies transformations (like rotation, scaling) to an image.
- TranslateImage: Translates an image by a specified offset.
- GroupImage: Groups multiple images together for rendering as a single entity.
- GeoImage: Represents 3D geometric images that can be rendered in a viewport.
- MDLFileImage: A wrapper for loading and managing MDL (Model Definition Language) files.
- PickImage: Allows picking or selecting an image.

CONNECTS TO: engine.h, model.h, transform.h, viewport.h

NOTES: None.

## `src/engine/imagetransform.cpp`

PURPOSE: This file contains implementations of various image transformations and utilities in the Allegiance game engine. It provides classes and functions for resizing, scaling, rotating, translating, and manipulating images.

KEY PARTS:
- ImageSize: Calculates the size of an image.
- ImageBounds: Retrieves the bounds of an image.
- CutImage: Cuts a portion of an image based on a specified rectangle.
- StringImageCorrected: Renders text as an image with customizable properties like font, color, and justification.
- ImageTransform: Provides static methods for various image transformations such as translate, scale, rotate, and justify.

CONNECTS TO: The Allegiance game engine's rendering system and other image-related classes.

NOTES: None.

## `src/engine/imagetransform.h`

PURPOSE: This file contains definitions for various image transformation and manipulation utilities in the Allegiance game engine.

KEY PARTS:
- NonStaticCallbackImage<OriginalType>: A template class that applies a callback to an image based on a changing value.
- CallbackImage<OriginalType>: Similar to NonStaticCallbackImage but designed to work with static values.
- ImageTransform: A class containing static methods for various image transformations such as translation, scaling, rotation, and more.

CONNECTS TO: model.h, enginep.h, engine.h

NOTES: None.

## `src/engine/inputengine.h`

PURPOSE: Defines interfaces and classes related to input handling in the Allegiance game engine.

KEY PARTS:
- ButtonEventData: Stores button event data with a button ID and whether it is down.
- InputStream: Abstract base class for input streams, providing methods to get value and button counts, check button states, and access events.
- MouseInputStream: Derived from InputStream, specifically for mouse input, offering additional methods to set clipping rectangle, position, sensitivity, and wheel position.
- JoystickInputStream: Derived from InputStream, specifically for joystick input, with methods to set ranges, create force feedback effects, play effects, and retrieve descriptions.
- InputEngine: Manages input devices, enumerating joysticks, updating input states, setting focus, and providing access to mouse and joystick streams.

CONNECTS TO: event.h, value.h

NOTES: None

## `src/engine/keyframe.cpp`

PURPOSE: This file contains implementations of keyframed transformations for translation, scaling, and rotation in the Allegiance game engine. It allows objects to change their position, size, and orientation over time based on predefined keyframes.

KEY PARTS:
- KeyFramedTranslateTransformImpl: Manages keyframed translations.
- KeyFramedScaleTransformImpl: Manages keyframed scaling.
- KeyFramedRotateTransformImpl: Manages keyframed rotations.
- AddKey: Adds a new keyframe to the transformation.
- Evaluate: Calculates the current transformation based on the current frame and interpolates between keyframes if necessary.
- GetString: Generates a string representation of the transformation for debugging or logging.

CONNECTS TO: Uses classes from "keyframe.h" and "quaternion.h", likely connected to rendering systems that apply these transformations.

NOTES: None.

## `src/engine/keyframe.h`

PURPOSE: This file defines keyframe-based transformations for translation, scaling, and rotation in the Allegiance game engine. It provides a framework for creating and managing keyframes that can be used to animate objects over time.

KEY PARTS:
- KeyFramedTranslateTransform: A class representing a keyframe-based translate transformation.
- KeyFramedScaleTransform: A class representing a keyframe-based scale transformation.
- KeyFramedRotateTransform: A class representing a keyframe-based rotate transformation.
- AddKey: Virtual functions to add keyframes with specific values (translation vectors for translation and scaling, quaternions for rotation).
- CreateKeyFramedTranslateTransform, CreateKeyFramedScaleTransform, CreateKeyFramedRotateTransform: Functions to create instances of the respective keyframe transform classes.

CONNECTS TO: Transform.h, value.h, Quaternion.h

NOTES: None.

## `src/engine/material.cpp`

PURPOSE: This file defines a `Material` class and its implementation in C++, which is used to represent the properties of materials in the game Allegiance, such as diffuse color, specular color, emissive color, and power.

KEY PARTS:
- MaterialImpl: A concrete implementation of the Material interface that manages material properties using child nodes.
- SetDiffuse, SetSpecular, SetEmissive, SetPower: Methods to set the respective material properties.
- GetDiffuseValue, GetSpecularValue, GetEmissiveValue, GetPowerValue: Methods to retrieve the values of the material properties.
- CreateMaterial: Factory functions to create instances of MaterialImpl with either Color or ColorValue inputs.

CONNECTS TO: The game engine's rendering system, which uses materials to apply visual effects to objects in the game world.

NOTES: None.

## `src/engine/material.h`

PURPOSE: Defines a base class `Material` for representing material properties in the game engine, which includes diffuse, emissive, and specular colors, as well as a power value. Also provides functions to create instances of materials.

KEY PARTS:
- Material: Base class for material properties with pure virtual methods for accessing and modifying color values and power.
- CreateMaterial: Functions to create instances of `Material` using either `Color` or `ColorValue` parameters.

CONNECTS TO: `value.h`, which likely contains definitions for `ColorValue` and `Number`.

NOTES: None.

## `src/engine/mdl.cpp`

**PURPOSE:** The `mdl.cpp` file contains the implementation of various classes and functions related to the MDL (Model Description Language) system in Allegiance. It handles parsing MDL files, managing data types, and creating namespaces.

**KEY PARTS:**
- **Name:** MDL Types - Defines various types used in the MDL system.
- **Name:** PointerMDLType - Represents a pointer to another MDL object.
- **Name:** ListMDLType - Manages lists of MDL objects.
- **Name:** IObjectMDLType - Base class for MDL types that represent objects.
- **Name:** StructMDLType - Represents structs in MDL and manages member data.
- **Name:** MDLObjectFactory - Factory class for creating instances of MDL objects.
- **Name:** IFunction::Read - Default implementation causing an assertion error.
- **Name:** ObjectPair - Represents a pair of objects.
- **Name:** MDLTextToken - Tokenizer for text-based MDL files.
- **Name:** MDLParser - Parses MDL files, including reading imports and definitions.

**CONNECTS TO:** Other parts of the Allegiance engine that use MDL data to define models and objects.

**NOTES:** The code contains TODO comments indicating unimplemented methods and an unusual use of `PCC` which likely stands for "const char*".

## `src/engine/mdl.h`

PURPOSE: This file contains definitions and declarations related to the Model (MDL) system in Allegiance, which is responsible for managing data structures and their serialization.

KEY PARTS:
- `IBinaryReaderSite`: Interface for reading binary data from a source.
- `IFunction`: Interface for functions that can be applied to an object stack.
- `MDLType`: Base class for model types, defining methods for size, destruction, and reading.
- `TBaseMDLType`: Template class derived from `MDLType`, providing implementations for basic data types.
- `IMDLObject`: Represents a model object with a type and index.
- `IObjectPair`: Interface for pairs of objects.

CONNECTS TO: `tlist.h`, `tref.h`, `tstack.h`, `zstring.h`

NOTES: None.

## `src/engine/mdlbin.cpp`

PURPOSE: This file contains the implementation of a binary name space for reading and processing MDL (Model) files in Allegiance. It defines how to read and interpret binary data into objects within a namespace.

KEY PARTS:
- MDLBinaryNameSpace: Class that handles reading MDL files from a binary source.
- ReadFile: Function that reads the entire file, parsing namespaces, imports, exports, and definitions.
- ReadObject: Recursive function that parses individual objects based on tokens read from the binary stream.
- GetString, GetDWORD, GetPointer, MovePointer: Methods to interact with the binary data stream.

CONNECTS TO: enginep.h, model.h, namespace.h

NOTES:
- Contains a debug-specific hack for setting namespace information on values.
- Uses ZString and other Allegiance-specific classes and functions.

## `src/engine/menu.cpp`

**PURPOSE:** The `menu.cpp` file in Allegiance is responsible for managing the creation, rendering, and interaction of menus within the game. It includes classes and functions to handle menu items, submenus, and user input.

**KEY PARTS:**
- **SubmenuEventSinkDelegate**: Wraps an existing event sink to delegate calls.
- **MenuCommandSinkDelegate**: Wraps an existing command sink to delegate calls.
- **PopupSurfaceSite**: Customizes the drawing of a popup surface with specific colors and patterns.
- **IMenuItemImpl**: Manages the visual representation, interaction, and state of a menu item.
- **IMenuImpl**: Manages a list of menu items, handles selection and command events, and controls the opening and closing of submenus.
- **AddMenuItem**: Adds a new menu item to the menu.
- **FindMenuItem**: Searches for a menu item by its ID.
- **SetMenuCommandSink**: Sets the command sink that will receive notifications when a menu item is selected or activated.
- **GetPane**: Returns the pane associated with the menu.
- **ClosePopup**: Closes the popup associated with the menu.
- **OnClose**: Notifies the command sink that the menu has been closed.
- **OnKey**: Handles keyboard input events such as arrow keys, enter, and escape.
- **HandleAccelerator**: Checks if any of the menu items have an accelerator key that matches the provided character.

**CONNECTS TO:** Other systems or files it depends on or serves include `SurfaceSite`, `IMenuItemPrivate`, `IMenuPrivate`, and other related classes for handling user input and rendering.

**NOTES:**
- The comment `//        psurface->FillSurface(Color::Black());` suggests a potential TODO to uncomment this line if needed.
- The use of `Pixel::Create(0)` in the `PopupSurfaceSite::UpdateSurface` method likely represents a transparent pixel.

## `src/engine/menu.h`

PURPOSE: This file defines interfaces and classes related to menu functionality in the Allegiance game engine.

KEY PARTS:
- IMenuItem: Interface for a menu item, providing methods to get/set properties like ID, string, colors, enabled status, and accelerator.
- ISubmenuEventSink: Interface for handling events when a submenu is requested from a menu item.
- IMenuCommandSink: Interface for handling commands and selections made in the menu.
- IMenu: Interface for managing a menu, including adding items, finding items by ID, and setting command sinks.

CONNECTS TO: popup.h (for IPopup interface), engine_font.h (for IEngineFont), modeler.h (for Modeler).

NOTES: None.

## `src/engine/model.h`

PURPOSE: The file `model.h` contains declarations for various classes related to 3D models and rendering in the Allegiance game engine. It defines interfaces and implementations for geometry, images, animations, and model loading.

KEY PARTS:
- `Geo`: Base class representing a geometric object with methods for rendering, hit testing, and transformation.
- `Image`: Represents an image with bounds calculation and rendering capabilities.
- `AnimationSet`: Interface for managing animation sets.
- `ModelerSite`: Interface for handling errors in the modeler.
- `Modeler`: Manages loading and managing 3D models, including images, geometry, and animations.

CONNECTS TO: Depends on other classes like `Context`, `Surface`, `Transform`, and `FileLoader`.

NOTES: None.

## `src/engine/modeler.cpp`

PURPOSE: The file `modeler.cpp` contains various factory classes and utility functions for creating and managing different types of model elements and resources in a game engine.

KEY PARTS:
- ModelerSiteImpl: Manages error handling for modeler sites.
- AnimatedImagePaneRectFactory: Creates animated image panes with rectangular transformations.
- GaugeImageFactory: Generates gauge images based on specified parameters.
- RGBColorFactory: Creates ColorValues from RGB color components.
- TStaticValue<StaticType>: Represents a static value with a specific type.
- ModelerImpl: Manages model loading and resource management.
- CameraFactory: Creates camera objects for different viewing perspectives.

CONNECTS TO: Other model-related files, such as `modeler.h`, and utility libraries like `mathutils.cpp`.

NOTES: None.

PARTS (in file order):

- lines 1-360
  - ModelerSiteImpl: Implements error handling for a modeler site.
  - AnimatedImagePaneRectFactory: Creates an animated image pane with a rectangle.
  - AnimatedImagePaneFactory: Creates an animated image pane.
  - PaneImageFactory: Creates a pane image based on the provided parameters.
  - FrameImageButtonPaneFactory: Creates a frame image button pane.
  - PointXFactory: Applies the X transformation to a point value.
  - PointYFactory: Applies the Y transformation to a point value.
  - ImageSizeFactory: Retrieves the size of an image.
- lines 361-713
  - GaugeImageFactory: Creates a gauge image with specified justification and number.
  - GaugeImageRectFactory: Creates a gauge image with specified justification, rectangle, boolean flag, and number.
  - UndetectableImageFactory: Creates an undetectable image from the given image.
  - ClipImageFactory: Clips an image to a specified rectangle.
  - PickImageFactory: Creates a pickable image from the given image.
  - ColorImageFactory: Applies a color to an image.
  - ExtentImageFactory: Creates an extent image with specified rectangle and color.
  - ImportXFileFactory: Imports a model from an X file.
- lines 714-1083
  - ImportFontFactory: Creates a font value by reading from a binary reader site.
  - ImportImageLRFactory: Imports an image and applies a vertical translation.
  - ImportImage3DFactory: Loads a surface and creates a constant 3D image with a specified color.
  - FillImageFactory: Creates a fillable image that can be evaluated to generate a surface filled with a given color.
  - BlendImageFactory: Blends two images based on a blend mode.
  - TranslateImageFactory: Translates an image by a specified point.
  - VisibleImageFactory: Makes an image visible or invisible based on a boolean value.
  - ScaleImageFactory: Scales an image by a specified factor.
- lines 1084-1443
  - TRefIObject: A typedef for a reference-counted pointer to an IObject.
  - TIFunctionBinary: A template class that implements the IFunction interface for binary operations.
  - RGBColorFactory: Creates a ColorValue from three float values representing RGB colors.
  - RGBAColorFactory: Creates a ColorValue from four float values representing RGBA colors.
  - VectorFactory: Creates a VectorValue from three float values representing a vector.
  - PointFactory: Creates a PointValue from two float values representing a point.
  - PointVFactory: Creates a PointValue using a transformation.
  - RectFactory: Creates a RectValue from four float values representing a rectangle.
- lines 1444-1826
  - TStaticValue<StaticType>: Represents a static value with a specific type.
  - TSwitchFactory<StaticType>: A factory class for creating switch values based on conditions.
  - SwitchImageFactory: Factory class for creating switch images based on conditions.
  - GroupImageFactory: Factory class for grouping multiple images together.
  - GroupGeoFactory: Factory class for grouping multiple geos (geometric shapes) together.
  - MeshGeoFactory: Factory class for creating mesh geometries from vertex and index lists.
  - LODGeoFactory: Factory class for creating level-of-detail geometries.
  - KeyFramedTranslateFactory: Factory class for creating key-framed translate transformations.
- lines 1827-2203
  - ModelerImpl: Manages model loading and resource management.
  - CameraFactory: Creates camera objects based on parameters.
  - Win32FontFactory: Creates font objects for Windows platforms.
  - ModFactory: Performs modulo operation on numbers.
  - MinFactory: Returns the minimum of two numbers.
  - MaxFactory: Returns the maximum of two numbers.
  - AddFactory: Adds two numbers together.
  - SubtractFactory: Subtracts one number from another.
- lines 2204-2686
  - Modeler::Create: Factory method to create a new instance of Modeler.
  - ModelerImpl: Implementation class for the Modeler interface.
  - TBaseMDLType: Base class for model types.
  - Number, Boolean, Color, Point, Vector, Rect, Orientation, String, Image, Geo: Specific model types.
  - CreateStringMDLType, CreateIObjectMDLType: Functions to create specific model types.
  - ModifiableNumberFactory, WrapNumberFactory, etc.: Factories for creating numbers, colors, vectors, etc.
  - MinFactory, MaxFactory, etc.: Factories for mathematical operations on numbers.
  - ImportImageFactory, ImportImageFromFileFactory, etc.: Factories for importing images in various formats.

## `src/engine/namespace.cpp`

PURPOSE: This file contains the implementation of a namespace system used in Allegiance for managing and organizing objects and types.

KEY PARTS:
- NameSpaceInfo: Represents information about a name space.
- CreateNameSpaceInfo: Creates an instance of NameSpaceInfo.
- NameSpaceImpl: Implements the INameSpace interface, providing functionality to manage symbols and types within a namespace.
- AddMember: Adds a member (object) to the namespace.
- FindMemberLocal: Finds a member locally in the namespace.
- FindMemberAndNameSpace: Recursively finds a member in the namespace or its parents.
- AddType: Adds a type to the namespace.
- FindTypeLocal: Finds a type locally in the namespace.
- FindTypeAndNameSpace: Recursively finds a type in the namespace or its parents.
- WriteToBinaryFile: Writes the namespace and its contents to a binary file.
- WriteToTextFile: Writes the namespace and its contents to a text file.

CONNECTS TO: base.h, IMDLBinaryFile.h

NOTES:
- Contains a hack for setting NameSpaceInfo on Value objects in debug builds.
- The constructor for NameSpaceImpl that takes an INameSpaceList is commented out with TODOs.

## `src/engine/namespace.h`

PURPOSE: This file defines the interface and implementation for a namespace system in Allegiance. The namespace allows for organizing and accessing various types of objects and values by name.

KEY PARTS:
- INameSpace: Interface defining methods to add, find, and retrieve members and types within a namespace.
- FindNumber, FindBoolean, FindFont, etc.: Convenience functions to retrieve specific types of values from the namespace with default values if not found.
- TList<TRef<INameSpace> > INameSpaceList: A list of namespace references.

CONNECTS TO: font.h, model.h, tref.h

NOTES: None.

## `src/engine/pane.cpp`

**Purpose:** The `pane.cpp` file contains the implementation of the `Pane` class, which is a fundamental component in Allegiance's user interface. It manages graphical elements, layout, painting, and mouse input.

**Key Parts:**
- **Class: Pane**: Represents a UI element that can contain other UI elements (children).
- **Function: Initialize()**: Initializes system colors used by the Pane class.
- **Function: GetSystemColor(int index)**: Retrieves a system color based on an index.
- **Function: SetSystemColor(int index, const Color& color)**: Sets a system color to a new value.
- **Constructor: Pane(Pane* pchild, const WinPoint& size)**: Initializes a new Pane with an optional child and size.
- **Destructor: ~Pane()**: Cleans up the Pane and its children.
- **Function: RemoveSelf()**: Removes the Pane from its parent.
- **Function: RemoveChild(Pane* ppane)**: Removes a specific child Pane.
- **Function: RemoveAllChildren()**: Removes all child Panes.
- **Function: Insert(int index, Pane* ppane)**: Inserts a Pane at a specified index.
- **Function: InsertAtBottom(Pane* ppane)**: Inserts a Pane at the bottom of the children list.
- **Function: InsertAtTop(Pane* ppane)**: Inserts a Pane at the top of the children list.
- **Function: NeedPaint()**: Marks the Pane and its parent for repainting.
- **Function: PaintAll(Surface* psurface)**: Recursively paints the Pane and all its children.
- **Function: TunnelPaint(Surface* psurface, bool bPaintAll)**: Handles painting through transparent Panes.
- **Function: InternalPaint(Surface* psurface)**: Handles internal painting logic.

**Connects To:** Other UI elements and input handling systems within Allegiance.

**Notes:** The commented-out `GenerateScreenVertices()` function suggests that vertex generation for surfaces with dimensions greater than 256 was previously implemented but is now unused.

## `src/engine/pane.h`

PURPOSE: The file `pane.h` defines the Pane class and related structures for managing graphical user interface elements in Allegiance. It includes definitions for layout, painting, and mouse input handling.

KEY PARTS:
- Pane: Manages a single UI element with properties like size, position, visibility, and children.
- SystemColors: Stores predefined color constants used throughout the UI.
- UIVERTEX: Structure representing vertices used for rendering UI elements.
- Surface: Class responsible for drawing to a surface.

CONNECTS TO: `color.h`, `input.h`, `rect.h`, and other UI-related files.

NOTES: None.

## `src/engine/paneimage.cpp`

PURPOSE: This file contains the implementation of the PaneImage class, which is a type of image that uses a TopPane to render its content.

KEY PARTS:
- PaneImage: A subclass of Image that renders its content using a TopPane.
- ImageTopPaneSiteImpl: An implementation of the TopPaneSite interface used by PaneImage to handle events and transformations.
- PaneChanged: Method called when the pane's state changes, triggering updates in PaneImage.
- CalcBounds: Calculates the bounds of the image based on the size of its TopPane.
- Render: Renders the content of the TopPane onto a context.
- GetSurface: Returns the surface associated with the TopPane.
- Evaluate: Evaluates the content and properties of the PaneImage.
- TransformLocalToImage: Transforms local coordinates to image coordinates using an origin point.

CONNECTS TO: model.h, surface.h, toppane.h

NOTES: None.

## `src/engine/paneimage.h`

PURPOSE: tiny file (13 lines), shown in full rather than summarized.

    #ifndef _PaneImage_H_
    #define _PaneImage_H_
    #include <tref.h>
    class Engine;
    class Image;
    class Pane;
    /////////////////////////////////////////////////////////////////////////////
    //
    // PaneImage
    //
    /////////////////////////////////////////////////////////////////////////////
    TRef<Image> CreatePaneImage(Engine* pengine, bool bColorKey, Pane* ppane);
    #endif

## `src/engine/pixel.h`

PURPOSE: This file defines a simple class to represent a pixel in the Allegiance game engine. It encapsulates a 32-bit value that represents the color of the pixel.

KEY PARTS:
- Pixel: A class representing a pixel with a 32-bit integer value.
- Create: A static function to create a new Pixel object from a 32-bit value.
- Value: A member function to get the 32-bit value of the pixel.

CONNECTS TO: None

NOTES: none

## `src/engine/pixelformat.cpp`

PURPOSE: This file contains definitions and implementations related to pixel formats used in the Allegiance game engine for rendering textures and surfaces.

KEY PARTS:
- PixelFormat: Class representing a pixel format with methods to create, manipulate, and convert pixels.
- FillDDPF: Function to fill a D3D9PIXELFORMAT structure with information from a GDI bitmap.
- GetEquivalentD3DFormat: Method to get the equivalent Direct3D format for a given pixel format.

CONNECTS TO: Allegiance game engine's rendering subsystems, specifically related to texture and surface management.

NOTES:
- Contains assertions and commented-out code indicating potential future enhancements or bug fixes.

## `src/engine/pixelformat.h`

PURPOSE: This file defines a class `PixelFormat` that encapsulates information about pixel formats used in the Allegiance game engine. It provides methods to create and manipulate pixel formats, as well as convert between different color representations.

KEY PARTS:
- PixelFormat: A class representing a pixel format with properties like bit count, masks for red, green, blue, and alpha channels.
- D3D9PIXELFORMAT: A structure used internally by the `PixelFormat` class to store detailed information about the pixel format.
- SetSoftwareTexture: A method to mark the pixel format as using software textures.
- MakePixel: Methods to create a pixel from red, green, blue values or a `Color` object.
- GetPixel: Method to retrieve a `Color` object from a pixel value.

CONNECTS TO:
- color.h: For the `Color` class used in pixel operations.
- d3d9.h: For Direct3D 9 pixel format constants and structures.
- tref.h: For reference counting, likely used by the `IObject` interface.
- pixel.h: For the `Pixel` class representing a single pixel.

NOTES:
- The commented-out constructor for creating a `PixelFormat` from an existing `DDPIXELFORMAT` is not implemented.
- There are no TODOs or security/network handling comments in this file.

## `src/engine/popup.cpp`

PURPOSE: The file `popup.cpp` contains the implementation for managing pop-up windows within the Allegiance game engine. It handles the creation, positioning, and closing of pop-ups.

KEY PARTS:
- IPopup: Interface defining methods for pop-up operations.
- PanePopup: Concrete class implementing IPopup using a Pane as its content.
- PopupContainerImpl: Class that manages multiple pop-ups, handling their opening, closing, and layout.
- PickImage: Custom image used to detect clicks outside of pop-ups.
- PopupData: Struct storing data about each open pop-up.

CONNECTS TO: enginep.h, enginewindow.h, image.h, pane.h, paneimage.h

NOTES: None.

## `src/engine/popup.h`

PURPOSE: This file defines interfaces and classes related to pop-up windows in the Allegiance game engine. It includes definitions for popup containers and individual popups.

KEY PARTS:
- IPopup: Interface for a popup window, providing methods to set container, owner, close popup, get rectangle, image, and pane.
- IPopupContainer: Interface for managing popup containers, including opening and closing popups, checking if empty, and getting images.
- CreatePanePopup: Function to create a popup from a pane.

CONNECTS TO: input.h, rect.h, value.h, Image.h, Engine.h, Pane.h

NOTES: None.

## `src/engine/settings.xml`

PURPOSE: tiny file (13 lines), shown in full rather than summarized.

    <?xml-version ='1.0' encoding='UTF-8'?>
    <PropertyList varName="Settings" >
     <Property>
      <Associations varName="Diff Associations" >
       <RunExternal>false</RunExternal>
      </Associations>
     </Property>
     <Property>
      <Associations varName="Merge Associations" >
       <RunExternal>false</RunExternal>
      </Associations>
     </Property>
    </PropertyList>

## `src/engine/stdafx.cpp`

PURPOSE: tiny file (6 lines), shown in full rather than summarized.

    // stdafx.cpp : source file that includes just the standard includes
    // Engine.pch will be the pre-compiled header
    // stdafx.obj will contain the pre-compiled type information
    #include "stdafx.h"
    // TODO: reference any additional headers you need in STDAFX.H
    // and not in this file

## `src/engine/stdafx.h`

PURPOSE: tiny file (7 lines), shown in full rather than summarized.

    // stdafx.h : include file for standard system include files,
    // or project specific include files that are used frequently, but
    // are changed infrequently
    //
    #pragma once
    #define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
    // TODO: reference additional headers your program requires here

## `src/engine/surface.cpp`

PURPOSE: Manages surfaces and textures for rendering in Allegiance, handling surface creation, management, and operations.

KEY PARTS:
- PrivateSurfaceImpl: Main class for managing surfaces with detailed logic.
- AllocateSurface: Allocates a surface and creates a texture.
- CreateCompatibleSurface: Creates a compatible surface based on current properties.
- BitBlt: Blits an area from one surface to another.
- DrawString: Draws a string on the surface.
- FillRect: Fills a rectangle on the surface with a given pixel or color.

CONNECTS TO: Allegiance engine systems, DirectX for rendering.

NOTES: None.

PARTS (in file order):

- lines 1-345
  - PrivateSurfaceImpl: Main class for managing surfaces.
  - Initialize(): Initializes member variables.
  - AllocateSurface(): Allocates a surface and creates a texture.
  - AllocateRenderTarget(): Allocates a render target surface.
  - GetTexHandle(): Returns the texture handle.
  - PrivateSurfaceImpl(PrivateEngine, const WinPoint&, SurfaceSite, SurfaceType): Constructor for creating a dummy top-level surface.
  - PrivateSurfaceImpl(D3DFORMAT, DWORD, DWORD, const char): Constructor for creating a surface from binary data.
  - PrivateSurfaceImpl(PrivateEngine, PixelFormat, const WinPoint&, SurfaceType, SurfaceSite): Constructor for creating a device format surface.
- lines 346-705
  - Surface: Manages a surface object with texture handling.
  - PrivateSurfaceImpl: Implementation class for Surface, containing detailed logic for surface creation and management.
  - CreateCompatibleSurface: Creates a compatible surface based on the current surface's properties.
  - Copy: Creates a copy of the current surface.
  - ~PrivateSurfaceImpl: Destructor that releases associated resources.
  - SetSite: Sets the site for the surface.
  - ClearDevice: Clears device-specific surface data (commented out).
  - SetPixelFormat: Updates the pixel format and re-allocates the surface.
- lines 707-1054
  - Surface: Manages rendering and surface operations.
  - UnclippedBlt: Performs a blit operation without clipping.
  - CopySubsetFromSrc: Copies a subset of another surface to this one.
  - BitBlt: Blits (copies) an area from one surface to another.
  - BitBltFromCenter: Blits an area from one surface to another centered at the specified point.
  - DrawString: Draws a string on the surface.
  - DrawStringWithShadow: Draws a string with a shadow effect.
  - Offset: Offsets the rendering context by a given point.
- lines 1056-1421
  - BitBlt: Blits a source surface to a target rectangle.
  - UnclippedFill: Fills a rectangle with a specified pixel color using hardware acceleration if possible.
  - FillRect: Fills a rectangle on the surface with a given pixel or color.
  - FillSurface: Fills the entire surface with a given pixel or color.
  - CreatePrivateDummySurface: Creates a dummy top-level surface.
  - CreatePrivateRenderTargetSurface: Creates a render target surface.
  - CreatePrivateSurface: Creates a surface with a specified pixel format and dimensions.
  - CreatePrivateSurface (memory mapped file): Creates a surface from a memory-mapped file.

## `src/engine/surface.h`

PURPOSE: This file defines the interface and base class for surfaces in the Allegiance game engine. Surfaces represent graphical objects that can be drawn on and manipulated.

KEY PARTS:
- SurfaceTypeBase: Base class for surface type bitmask.
- SurfaceType: Bitmask representing different types of surfaces (2D, 3D, ZBuffer, etc.).
- SurfaceSite: Interface for managing the site where a surface is used.
- Surface: Main interface for creating, manipulating, and rendering graphical surfaces.

CONNECTS TO:
- mask.h, point.h, rect.h, tref.h, pixel.h, VRAMManager.h, Color.h, Context.h, Engine.h, IEngineFont.h, PixelFormat.h, ZFile.h

NOTES: None.

## `src/engine/toppane.cpp`

PURPOSE: The file `toppane.cpp` contains the implementation of the `TopPane` class, which is a top-level pane in the Allegiance game engine. It handles surface management, layout updates, and input events.

KEY PARTS:
- **TopPaneSurfaceSite**: A subclass of `SurfaceSite` that updates the `TopPane` when its surface needs to be updated.
- **TopPane**: Manages the top-level pane's surface, layout, and painting. Handles mouse events and input redirection.
- **RepaintSurface**: Marks the pane as needing a repaint.
- **NeedLayout**: Signals that the layout of the pane has changed.
- **Paint**: Placeholder for custom painting logic (currently empty).
- **Evaluate**: Recreates the surface if its size changes or is uninitialized.
- **UpdateBits**: Handles the actual rendering and painting of the pane.

CONNECTS TO: `engine.h`, `enginep.h`, `surface.h`

NOTES: The file contains commented-out code from an original Allegiance version, which may indicate that some features were removed or modified.

## `src/engine/toppane.h`

PURPOSE: This file defines the `TopPane` class and its associated interfaces for managing a top-level pane in the Allegiance game engine.

KEY PARTS:
- `TopPaneSite`: Interface defining methods for transforming local coordinates to image coordinates, handling surface changes, and size changes.
- `TopPane`: Class representing a top-level pane with functionality for layout management, painting, and mouse input handling.
- `TTopPaneSite`: Template class implementing the `TopPaneSite` interface by delegating calls to an instance of a specified type.

CONNECTS TO: `pane.h`, `engine.h`

NOTES: None.

## `src/engine/transform.cpp`

**PURPOSE:** The file `transform.cpp` contains the implementation of various transformation classes used in the Allegiance game engine to handle spatial transformations such as translation, rotation, scaling, and component-based transformations.

**KEY PARTS:**
- **Transform Class:** Base class for all transformation types.
  - `IsIdentity()`: Checks if the transformation is an identity transform.
  - `Fold()`: Simplifies the transformation if it's constant.
  
- **Identity Transform:** Represents a no-op transformation.
  - `GetIdentityTransform()`: Returns the singleton instance of the identity transform.

- **Translate Transform:** Applies translation to a point or vector.
  - `Evaluate()`: Sets the translation value in the internal transform.
  - `SetVector()`: Updates the translation vector and marks the transformation as changed.

- **Rotate Transform:** Applies rotation around an axis by a specified angle.
  - `Evaluate()`: Sets the rotation values in the internal transform.
  - `SetAngle()` and `SetAxis()`: Update the rotation parameters and mark the transformation as changed.

- **Scale Transform:** Applies scaling to a point or vector.
  - `Evaluate()`: Sets the scale values in the internal transform, handling dynamic scaling if a `Number` object is provided.
  - `SetScale()`: Updates the scale value and marks the transformation as changed.

- **Component Transform:** Combines translation, rotation, and scaling into a single transformation.
  - `Evaluate()`: Applies the component-based transformation to the internal transform.
  - `SetPosition()`, `SetScale()`, `SetOrientation()`: Update the respective parameters and mark the transformation as changed.

**CONNECTS TO:**
- Other transformation classes (`TranslateTransform`, `RotateTransform`, `ScaleTransform`) and possibly other systems for rendering or physics calculations.

**NOTES:** None.

## `src/engine/transform.h`

PURPOSE: This file defines various transformation classes used in the Allegiance game engine for manipulating 2D and 3D space transformations.

KEY PARTS:
- Transform: Base class for all transformation types.
- MatrixTransform: Represents a transformation using a matrix.
- Transform2: Base class for 2D transformation types.
- MatrixTransform2: Represents a 2D transformation using a matrix.
- TranslateTransform: Translates an object by a vector.
- TranslateTransform2: Translates a 2D object by a point.
- ScaleTransform2: Scales a 2D object by a point.
- RotateTransform2: Rotates a 2D object around an angle.
- RotateTransform: Rotates an object around an axis and angle.
- ScaleTransform: Scales an object by a vector or number.
- ComponentTransform: Represents a complex transformation with position, orientation, and scale.
- AnimateRotateTransform: Animates the rotation of an object.

CONNECTS TO: Depends on classes from "value.h" for value handling and "zstring.h" for string operations.

NOTES: None.

## `src/engine/transform2.cpp`

PURPOSE: This file contains implementations of various 2D transformation classes used in the Allegiance game engine. These transformations include identity, translation, scaling, and rotation.

KEY PARTS:
- IdentityTransform2: Represents an identity transformation that does not alter coordinates.
- TranslateTransform2: Applies a translation to points based on a given offset.
- ScaleTransform2: Scales points by a specified factor.
- RotateTransform2: Rotates points around the origin by a given angle.

CONNECTS TO: transform.h, PointValue.cpp, Number.cpp

NOTES: None.

## `src/engine/value.cpp`

**PURPOSE:** The `value.cpp` file in Allegiance contains the implementation of a value tree system used for managing and evaluating various data types within the game. This includes classes and functions for creating, manipulating, and converting values to strings or binary formats.

**KEY PARTS:**
- **Class: Value**: Represents a node in a value tree with methods for adding/removing children, evaluating, updating, and folding values.
- **Function: Value::Evaluate()**: Virtual function to evaluate the value. Currently empty.
- **Function: ValueList::PushFront(Value* pvalue)**: Adds a value to the front of the list and updates parent references.
- **Class: OrNumber, AndNumber, XOrNumber**: Inherit from `Number` and implement bitwise operations (`OR`, `AND`, `XOR`).
- **Function: GetString(int indent)**: Converts various data types into their string representations with proper indentation.
- **Function: Write(IMDLBinaryFile* pmdlFile, const ZString& value)**: Writes a string to a binary file using the `IMDLBinaryFile` interface.

**CONNECTS TO:** Other systems or files it depends on or serves:
- The game engine's core logic for managing and evaluating values.
- Files related to data serialization and storage.

**NOTES:** 
- Contains numerous TODO comments and unusual patterns such as direct binary serialization of complex types like `Orientation`, `Rect`, and `Color`.

## `src/engine/value.h`

PURPOSE: The file defines a generic Value class and its subclasses for various types of values such as strings, colors, points, etc., along with functions to convert these values to strings and write them to binary files.

KEY PARTS:
- `Value`: Base class representing a value that can have children and be part of a namespace.
- `TStaticValue`: Template class for static values that do not change after creation.
- `TModifiableValue`: Template class for modifiable values that can change their state.
- `StringValue`, `ColorValue`, etc.: Specific subclasses for different types of values like strings, colors, points, etc.
- `IMDLBinaryFile`: Interface for writing binary MDL files.

CONNECTS TO: 
- Other classes and functions in the Allegiance engine, particularly those related to MDL file handling and value manipulation.

NOTES:
- Contains a mix of debug code that is commented out.
- Uses templates extensively to handle different types of values uniformly.

## `src/engine/valuetransform.cpp`

PURPOSE: The file `valuetransform.cpp` contains implementations for various transformations and operations on different types of values such as numbers, booleans, strings, points, colors, and rectangles. These transformations include arithmetic operations, logical operations, string manipulations, and more.

KEY PARTS:
- `EqualsTransform`: A template class that checks if two values are equal.
- `Number`: Base class for numeric values with various subclasses like `AddNumber`, `SubtractNumber`, etc., performing arithmetic operations.
- `Boolean`: Base class for boolean values with subclasses like `AndBoolean`, `OrBoolean`, and `NotBoolean` for logical operations.
- `StringValue`: Base class for string values with subclasses like `ConcatenatedString` for concatenation.
- `PointValue`: Represents a point in 2D space, with subclasses `PointX` and `PointY` to access the X and Y coordinates respectively.
- `ColorValue`: Represents a color value.
- `RectValue`: Represents a rectangle value.

CONNECTS TO: Other files like `valuetransform.h`, which contains declarations for these classes and functions.

NOTES: None.

## `src/engine/valuetransform.h`

PURPOSE:
This file contains a collection of transformation classes and templates that handle various types of value transformations in the Allegiance game engine. These transformations include arithmetic operations for numbers, creation and manipulation of points and colors, boolean logic, string manipulations, and more.

KEY PARTS:
- NumberTransform: Provides static methods to perform arithmetic operations on numbers.
- PointTransform: Offers functions to create and manipulate point values.
- ColorTransform: Allows the creation of color values.
- RectTransform: Handles rectangle value transformations.
- BooleanTransform: Includes methods for boolean logic and conversions.
- StringTransform: Provides string manipulation functions.
- TransformedValue, TransformedValue2, TransformedValue3, TransformedValue4: Templates for creating transformed values based on callback functions.
- SimpleModifiableValue: A modifiable value that can be updated via events.
- VectorTransformValue: Applies a transformation to a vector of values.

CONNECTS TO:
value.h and event.h

NOTES:
none

## `src/engine/viewport.cpp`

PURPOSE: The file `viewport.cpp` contains implementation for managing the viewport in the Allegiance game engine. It handles transformations between different coordinate systems used in rendering and user interaction.

KEY PARTS:
- Viewport::GetRect(): Returns the current view rectangle.
- Viewport::GetAspect(): Calculates the aspect ratio of the viewport.
- Viewport::TransformNDCToImage(): Transforms normalized device coordinates (NDC) to image space.
- Viewport::TransformImageToScreen(): Transforms image space coordinates to screen space.
- Viewport::TransformNDCToScreen(): Combines NDC to image and then image to screen transformations.

CONNECTS TO: viewport.h

NOTES: None.

## `src/engine/viewport.h`

PURPOSE: Defines a class representing a viewport in the game engine, which manages camera and view rectangle settings.

KEY PARTS:
- Viewport: Main class that inherits from Value. Manages camera and view rectangle.
- SetCamera: Sets the camera for the viewport.
- SetViewRect: Sets the view rectangle for the viewport.
- GetCamera: Retrieves the camera associated with the viewport.
- GetViewRect: Retrieves the view rectangle associated with the viewport.
- GetRect: Returns the current rectangle of the viewport.
- GetAspect: Calculates and returns the aspect ratio of the viewport.
- TransformNDCToImage: Transforms normalized device coordinates to image space.
- TransformNDCToScreen: Transforms normalized device coordinates to screen space.
- TransformImageToScreen: Transforms image space coordinates to screen space.

CONNECTS TO: camera.h, RectValue.h

NOTES: None.

## `src/engine/xfile.cpp`

PURPOSE: The file `xfile.cpp` contains classes and functions for parsing X-Files, a file format used by Microsoft games for storing 3D models, animations, and other resources.

KEY PARTS:
- XToken: Abstract base class for tokenizing X-Files.
- XBinaryToken: Concrete implementation of XToken for binary X-Files.
- XTextToken: Concrete implementation of XToken for text X-Files.
- XParser: Class responsible for parsing X-Files using a tokenizer.
- ReadTransform: Reads and constructs a Transform object from the token stream.
- ReadMesh: Reads and constructs a Geo (mesh) object from the token stream.
- TRef<T>: A smart pointer template for managing resources.
- ZString: A string class with additional functionality.
- Matrix: Represents a 4x4 transformation matrix.
- Vertex: Represents a vertex with position, texture coordinates, and normal.
- Geo: Represents a mesh geometry.
- ReadFrame: Reads frame data from an X-File and creates a GroupGeo object.
- ReadAnimationKey: Reads animation keyframe data from an X-File and creates a Transform object.
- ReadAnimation: Reads animation data from an X-File and applies it to a frame's transform.
- ReadAnimationSet: Reads a set of animations from an X-File.
- ImportXFile: Entry point for importing X-File data, creating a parser and reading the file.

CONNECTS TO: Other systems or files it depends on or serves, one line. None

NOTES: "ZError("Not Implemented");" in ReadAnimationKey

## `src/engine/xfile.h`

PURPOSE: tiny file (12 lines), shown in full rather than summarized.

    #ifndef _xfile_h_
    #define _xfile_h_
    #include "value.h"
    class Geo;
    class Modeler;
    //////////////////////////////////////////////////////////////////////////////
    //
    //
    //
    //////////////////////////////////////////////////////////////////////////////
    TRef<Geo> ImportXFile(Modeler* pmodeler, ZFile* pfile, Number* pnumberFrame, bool& bAnimation);
    #endif
