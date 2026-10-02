# src/mdledit

[Back to the index](../LLAMA_SUMMARY.md) - 2 files, 1,816 lines. Notes by a local model, to orient by: not verified facts.

The `src/mdledit` folder contains the source code for the MDL Editor application in Allegiance, which is used to create and edit 3D models for the game.

- mdledit.cpp: Implements the main window class and various test geometry classes for the MDL Editor.
- pch.h: A precompiled header file that includes necessary headers and disables certain compiler warnings for the MDLEdit application.

## `src/mdledit/mdledit.cpp`

PURPOSE: The file `mdledit.cpp` contains the implementation of the MDL Editor application's main window class and various test geometry classes.

KEY PARTS:
- **MDLEditWindow**: Main window class for the MDL Edit application, handling rendering, hit testing, and network communication.
- **TestGeo**: Class representing a test geometry with different rendering modes and tests.
- **InitializePolyTest**: Initializes polygonal test geometry.
- **AddMineFieldTest**: Function to add mine field test elements.
- **OpenXFile**: Loads an X file into the scene.
- **OnMenuCommand**: Handles menu commands for user interactions.
- **Render**: Method to render images on the screen.
- **HitTest**: Method to handle mouse hit testing.

CONNECTS TO: Allegiance engine, sound engine, network communication systems.

NOTES:
- Contains commented-out code related to fullscreen settings and sound engine initialization.
- Member variable `m_pimageCursor` is declared but not used in the provided code.

PARTS (in file order):

- lines 1-369
  - MDLEditWindow: Main window class for the MDL Editor.
  - TestGeo: Class representing a test geometry with various rendering modes and tests.
  - InitializePolyTest: Initializes polygonal test geometry.
  - InitializeGeoTest: Initializes geometric test using ThingGeo.
  - InitializeRingTest: Initializes ring test with multiple ThingGeos.
  - SetTest: Sets the current test mode based on an integer value.
  - NextTest: Advances to the next test mode.
  - SetMode: Configures rendering context for different test modes.
- lines 370-746
  - MDLEditWindow: Main window class for MDLEdit application.
  - Render: Method to render images on the screen.
  - HitTest: Method to handle mouse hit testing.
  - CursorSurfaceSite: Class to create a custom cursor surface.
  - MDLEditWindow constructor: Initializes various components of the MDLEdit application.
  - Add2DTests and Add3DTests: Methods to add 2D and 3D tests based on user input.
  - TODOs: "// Move this call here, so that engine initialisation is performed AFTER we have a valid HWND." "// SetFullscreen(true);" "// GetEngine()->SetDebugFullscreen(true);" "//m_psoundEngine = CreateSoundEngine(GetHWND());"
- lines 748-1118
  - MDLEditWindow: Main class for the MDL edit window.
  - AddMineFieldTest: Function to add mine field test elements.
  - AddBoltTest: Function to add bolt test element.
  - SweepNumber: Class that sweeps a number between a minimum and maximum value over a duration.
  - CreateGeoPane: Function to create a pane with a rotating geo image.
  - Receive, Disconnect, Connect, StartServer, ServerDoIdle, StartClient, ClientDoIdle: Functions related to network communication (commented out).
  - TODOs or unusual code: //KG - old version here: Comment indicating an older version of the code. m_pimageCursor: Member variable for cursor image, but not used in the provided code.
- lines 1119-1491
  - AddDebris: Adds debris to the scene.
  - AddThingGeo: Adds particle and bits geometry to the scene.
  - AddLODScrollBar: Adds a LOD (Level of Detail) scroll bar to the interface.
  - AddSphere: Adds a sphere to the scene (commented out).
  - AddIcosahedron: Adds an icosahedron to the scene.
  - OpenXFile: Loads an X file into the scene.
  - OpenMDL: Opens an MDL file and adds its geometry to the scene.
  - OpenImage: Loads an image from a path.
- lines 1492-1770
  - MDLEditWindow: Main window class for MDLEdit application.
  - OnMenuCommand: Handles menu commands.
  - OnKey: Processes keyboard input.
  - Initialize: Initializes the MDLEdit application.
  - Terminate: Cleans up resources before exiting.
  - TODOs: Remove commented-out code related to "client" and "server". Consider refactoring key handling logic for better readability.

## `src/mdledit/pch.h`

PURPOSE: This file is a precompiled header file for the MDLEdit application in Allegiance. It includes necessary headers and disables certain compiler warnings.

KEY PARTS:
- `__MODULE__`: Defines the module name used by assertions.
- `#pragma warning(disable ...)`: Disables specific compiler warnings to suppress noise during compilation.
- Includes various system, graphics, and custom headers required for MDLEdit functionality.

CONNECTS TO: engine.h, effect.h, geometry.h, D3DDevice9.h, controls.h, paneimage.h

NOTES:
- The file is named `pch.h` indicating it's a precompiled header.
- Some warnings are disabled to clean up the build output.
