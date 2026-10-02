# src/effect

[Back to the index](../LLAMA_SUMMARY.md) - 43 files, 12,547 lines. Notes by a local model, to orient by: not verified facts.

The `src/effect` folder in Allegiance contains source code related to game effects and visual elements. It handles rendering, animation, and interaction of various graphical components within the game.

- thinggeo.cpp: Manages and renders game entities' geometry.
- efpane.cpp: Handles the creation and management of UI panes and their behaviors.
- efgeo.cpp: Implements various geometric effects used throughout the game.
- pagepane.cpp: Responsible for managing the rendering of UI elements such as words, strings, tabs, hyperlinks, and links within a page.
- listpane.cpp: Manages painting, selection, and user input for list pane components.

Largest files: thinggeo.cpp (1775 lines), efpane.cpp (1060 lines), efgeo.cpp (1055 lines), pagepane.cpp (1038 lines), listpane.cpp (922 lines).

## `src/effect/CMakeLists.txt`

PURPOSE: This file is a CMake configuration script for building the "Effect" module of the Allegiance game. It defines the source files and libraries needed to compile this module.

KEY PARTS:
- `ADD_DEFINITIONS`: Sets preprocessor definitions for the module.
- `INCLUDE_DIRECTORIES`: Specifies include paths for header files used by the module.
- `SET(effect_SRC)`: Lists all source files that make up the Effect module.
- `ADD_LIBRARY(effect ${effect_SRC})`: Creates a library target named "effect" from the listed source files.
- `SET_PROPERTY(TARGET effect PROPERTY CXX_STANDARD 14)`: Specifies that the C++ standard for this target should be C++14.

CONNECTS TO: Other modules and libraries such as "engine", "soundengine", and various geometric and utility classes.

NOTES: None.

## `src/effect/alephgeo.cpp`

PURPOSE: The file `alephgeo.cpp` contains the implementation of the `AlephGeo` class, which represents a geometric model for an Aleph star effect in the Allegiance game. It handles rendering and animation of the star.

KEY PARTS:
- `AlephGeo`: A class that inherits from `Geo`, `IGeoCallback`, and `TEvent<float>::Sink`. It manages the geometry and texture coordinates of the Aleph star, including initialization, event handling, and rendering.
- `InitializeVertices`: Initializes the vertex positions based on depth and section counts.
- `InitializeIndices`: Sets up the indices for drawing the star as a mesh.
- `UpdateTextureCoordinates`: Updates the texture coordinates over time to animate the star's appearance.
- `RenderCallback`: Handles the actual rendering of the star using the updated geometry and texture.

CONNECTS TO: 
- `event.h`, `geometry.h`, `model.h`, `surface.h`
- `efart.h` (indirectly through `CreateAlephGeo`)

NOTES:
- None

## `src/effect/combopane.cpp`

PURPOSE: This file contains implementations and factories for creating combo panes, which are interactive UI elements that display a selection of items and allow the user to choose one.

KEY PARTS:
- StringComboFacePane: Manages the face of a combo pane with text.
- ImageComboFacePane: Manages the face of a combo pane with an image.
- ComboPaneImpl: Implements the core functionality of a combo pane, including handling menu items and events.
- CreateStringComboFacePane: Factory function to create instances of StringComboFacePane.
- CreateImageComboFacePane: Factory function to create instances of ImageComboFacePane.
- CreateComboPane: Factory function to create instances of ComboPaneImpl.

CONNECTS TO:
- button.h, controls.h, font.h, menu.h for UI components and utilities.

NOTES:
- None.

## `src/effect/combopane.h`

PURPOSE: This file defines interfaces and implementations for combo panes, which are graphical components used in the Allegiance game for displaying a list of selectable items.

KEY PARTS:
- ComboFacePane: An interface for setting properties like string, inside/outside state, and color.
- ComboPane: An interface for managing menu items, selection, enabling/disabling, and event sources.
- CreateImageComboFacePane: Creates a combo face pane with an image.
- CreateStringComboFacePane: Creates a combo face pane with text.
- CreateComboPane: Creates a full combo pane with modeler, popup container, font, size, face pane, and mouse position.

CONNECTS TO: Pane.h, Value.h, IMenuItem.h, IIntegerEventSource.h, IEventSource.h, Image.h, IEngineFont.h, Modeler.h, IPopupContainer.h

NOTES: None.

## `src/effect/conegeo.cpp`

PURPOSE: This file contains the implementation of a cone geometry class for rendering in Allegiance.

KEY PARTS:
- ConeGeo: A class representing a cone geometry with customizable segments and angle.
- InitializeVertices: Initializes the vertex positions based on the angle and segment count.
- InitializeIndices: Generates the indices to form triangular faces for the cone.
- Evaluate: Calls InitializeVertices to update the geometry when needed.
- Render: Draws the cone using the provided context.

CONNECTS TO: material.h, model.h

NOTES: None

## `src/effect/debrisgeo.cpp`

PURPOSE: This file contains the implementation of a debris effect in Allegiance, which generates and renders visual debris particles based on camera movement.

KEY PARTS:
- DebrisGeo: Main class responsible for managing and rendering debris particles.
- Evaluate: Updates the state of debris particles based on camera movement and user settings.
- Render: Draws the debris particles on the screen.
- DebrisData: Stores data for each individual debris particle, including position, color, and start time.

CONNECTS TO:
- Camera: For getting the current camera position and orientation.
- Modeler: For loading the surface used to render the debris particles.
- Viewport: For getting the view rectangle and other viewport-related information.

NOTES:
- None.

## `src/effect/debrisgeo.h`

PURPOSE: tiny file (8 lines), shown in full rather than summarized.

    #ifndef _DebrisGeo_h_
    #define _DebrisGeo_h_
    //////////////////////////////////////////////////////////////////////////////
    //
    // DebrisGeo header
    //
    //////////////////////////////////////////////////////////////////////////////
    #endif

## `src/effect/efapp.cpp`

**PURPOSE:** The file `efapp.cpp` manages the initialization and termination of the game's effect application, handles light effects and frame data, and provides factories for creating various game state-related objects.

**KEY PARTS:**
- **Name:** TrekResources - Manages font resources.
- **Name:** LightData - Holds data for a light effect.
- **Name:** LightsGeo - Represents geometry that renders lights.
- **Name:** LightsFactory - Creates instances of `LightsGeo`.
- **Name:** FrameDataFactory - Creates instances of `FrameDataListValue`.
- **Name:** GameStateContainerFactory - Creates instances of `GameStateContainer`.
- **Name:** GameStateImageFactory - Retrieves images associated with a game state container.
- **Name:** GroupGeoCallbackImpl - Callback for processing group geometry, handling light effects and frame data.

**CONNECTS TO:**
- Other rendering and game logic systems that depend on light effects and game states.

**NOTES:**
- Contains placeholder functions for LOD analysis (`ReportTriangles`).
- The `GroupGeoCallbackImpl` class is crucial for extracting lights and frames from a Geo object.
- The file includes various factory classes to create complex objects efficiently.

## `src/effect/efapp.h`

PURPOSE: This file defines the `EffectApp` class, which inherits from `EngineApp`, and provides methods for optimizing geometric objects and managing resources such as fonts.

KEY PARTS:
- EffectApp: Manages effects within the game, including optimizing geometric objects.
- OptimizeThingGeo: Optimizes a geometric object based on a string identifier and a number.
- TrekResources: Manages various resources used in the game, particularly fonts and colors.

CONNECTS TO: engineapp.h, namespace.h

NOTES: None

## `src/effect/efart.h`

PURPOSE: tiny file (14 lines), shown in full rather than summarized.

    #ifndef _EFFECTS_ARTWORK_H_
    #define _EFFECTS_ARTWORK_H_
    #define AWF_EFFECT_ALEPH_STAR                   "f02bmp"
    #define AWF_EFFECT_MUZZLE_FLARE                 "f103bmp"
    #define AWF_EFFECT_LENS_FLARE_STAR_AURA         "f105bmp"
    #define AWF_EFFECT_LENS_FLARE_MID_RING          "f104bmp"
    #define AWF_EFFECT_LENS_FLARE_END_RING          "f106bmp"
    #define AWF_EFFECT_AFTERBURNER_GLOW             "f101bmp"
    #define AWF_EFFECT_STROBE                       "f101bmp"
    #define AWF_EFFECT_TANGENT                      "f103bmp"
    #define AWF_EFFECT_NORMAL                       "fx11bmp"
    #define AWF_EFFECT_BIT                          "utl10"
    #define AWF_EFFECT_TRAIL                        "trailbmp"
    #endif

## `src/effect/effect.h`

PURPOSE: This header file includes declarations for various effect-related classes and functions used in Allegiance.

KEY PARTS:
- Effect: Base class for all effects.
- EfGeo: Manages geometric effects.
- ParticleGeo: Handles particle-based effects.
- EfImage: Manages image-based effects.
- EfPane: Represents a pane that can display effects.
- EfSound: Manages sound effects.
- GameState: Provides game state information to effects.
- EfPopup: Displays popup messages or notifications.
- ListPane: Manages list-based UI elements for effects.
- ComboPane: Handles combo box UI elements for effects.
- Help: Manages help-related effects.
- Explosion: Represents explosion effects.
- FrameData: Holds frame-specific data for animations.
- ThingGeo: Manages object-based geometric effects.
- StarImage: Handles star field images or backgrounds.
- EfApp: Main application class that manages effects.
- EfWindow: Manages window-based UI elements for effects.

CONNECTS TO: engine.h, enginep.h

NOTES: None.

## `src/effect/efgeo.cpp`

**Purpose:** The `efgeo.cpp` file contains implementations and factory functions for various geometric effects used in the Allegiance game, including mine fields, build effects, wireframe spheres, pulse effects, trails, hull hits, bits, bolts, culling, and copy mode rendering.

**Key Parts:**
- **Name:** MineFieldGeoImpl - Represents a mine field effect with randomly positioned mines.
- **Name:** CreateMineFieldGeo - Factory function to create an instance of MineFieldGeoImpl.
- **Name:** BuildEffectGeoImpl - Represents a build effect with inner and outer spheres that rotate over time.
- **Name:** CreateBuildEffectGeo - Factory function to create an instance of BuildEffectGeoImpl.
- **Name:** WireSphereGeo - Represents a wireframe sphere geometry.
- **Name:** CreateWireSphereGeo - Factory function to create an instance of WireSphereGeo.
- **Name:** PulseGeoImpl - Represents a pulse effect that grows and fades over time.
- **Name:** CreatePulseGeo - Factory function to create an instance of PulseGeoImpl.
- **Name:** TrailGeo - Represents a trail effect with multiple sections, updating dynamically based on position and movement.
- **Name:** HullHitGeoImpl - Represents a visual effect for hull hits, showing decals at impact points.

**Connects To:** Other geometry-related classes and rendering systems within the game engine.

**Notes:**
- `MineFieldGeoImpl::GetCount` calculates the number of mines based on strength and radius using a specific formula.
- `BuildEffectGeoImpl::RenderCallback` uses complex transformations to render inner and outer spheres with different colors and blending modes.
- `WireSphereGeo` has a static member `vSegments` which is currently set to 3, but could be adjusted for more detailed or performance reasons.
- `PulseGeoImpl::RenderCallback` handles the rendering of multiple pulse effects, each scaling and fading over time based on their start and explode times.

## `src/effect/efgeo.h`

PURPOSE: This file contains declarations for various geometric effect classes and functions used in the Allegiance game to create dynamic visual effects such as debris, trails, pulses, and more.

KEY PARTS:
- Surface: Represents a surface for rendering.
- Viewport: Manages the view of the game world.
- CreateDebrisGeo: Creates a geo object representing debris.
- CreateCullGeo: Creates a geo object that can be culled from rendering.
- CreateCopyModeGeo: Creates a geo object in copy mode.
- CreateWireSphereGeo: Creates a wireframe sphere geo object.
- CreateBoltGeo: Creates a bolt (line) geo object.
- CreateConeGeo: Creates a cone geo object.
- CreateTrailGeo: Creates a trail geo object that follows a path.
- CreateAlephGeo: Creates an Aleph geo object, likely for special effects.
- MineFieldGeo: Base class for mine field geo objects with strength and radius properties.
- BuildEffectGeo: Base class for build effect geo objects with methods to set and get radius and position.
- PulseGeo: Base class for pulse geo objects with methods to add pulses.
- HullHitGeo: Base class for hull hit geo objects with methods to add hits.
- BitsGeo: Base class for bits geo objects with methods to add individual bits.

CONNECTS TO: event.h, model.h, tref.h, value.h

NOTES: None.

## `src/effect/efimage.cpp`

**PURPOSE:** The `efimage.cpp` file contains implementations and factory functions for various image-related classes used in the Allegiance game, including handling joystick input, blending images with colors, displaying text grids, rendering muzzle flares, and managing poster images on viewports.

**KEY PARTS:**
- **Name:** JoystickImageImpl - Implements the JoystickImage interface for handling joystick input.
- **Name:** CreateJoystickImage - Factory function to create an instance of JoystickImageImpl.
- **Name:** BlendColorImage - A class that wraps another image and blends it with a specified color.
- **Name:** CreateBlendColorImage - Factory function to create an instance of BlendColorImage.
- **Name:** StringGridImageImpl - Implements the StringGridImage interface for displaying text in a grid format.
- **Name:** CreateStringGridImage - Factory function to create an instance of StringGridImageImpl.
- **Name:** MuzzleFlareImageImpl - Implements the MuzzleFlareImage interface for rendering muzzle flares.
- **Name:** CreateMuzzleFlareImage - Factory function to create an instance of MuzzleFlareImageImpl.
- **Name:** PosterImageImpl - Implements the PosterImage interface for displaying images as posters in a viewport.
- **Name:** AddPoster - Adds a poster image with specified properties to the list of posters to be rendered.

**CONNECTS TO:** Other rendering and input handling systems within Allegiance, such as `Viewport`, `Modeler`, and `Joystick`.

**NOTES:**
- The commented-out code in the `Render` method of `LensFlareImageImpl` suggests a previous implementation that has been replaced or is no longer used.
- Conditional compilation based on whether a specific fix for Permedia hardware is enabled using `#ifdef FixPermedia`.

## `src/effect/efimage.h`

PURPOSE: This file defines various image-related classes and functions used for creating and manipulating images in the Allegiance game.

KEY PARTS:
- JoystickImage: Represents an image that can interact with a joystick.
- StringGridImage: Allows setting strings, colors, and size in a grid format.
- MuzzleFlareImage: Creates a muzzle flare effect at a specified point.
- PosterImage: Displays images on a viewport as posters.
- LensFlareImage: Simulates lens flares based on light direction.

CONNECTS TO: inputengine.h, image.h, tref.h

NOTES: None

## `src/effect/efpane.cpp`

**PURPOSE:** The `efpane.cpp` file in Allegiance is responsible for managing various UI elements and their behaviors within the game. It includes classes and functions to create and handle different types of panes such as screens, gauges, scroll bars, and hover effects.

**KEY PARTS:**
- **ScreenPane**: Manages a window and handles mouse events.
- **GaugePane**: Displays a gauge with a flashing effect and updates its value based on input.
- **ParseIntVector, ParseFloatVector, ParseStringVector**: Parses vectors of integers, floats, and strings from an object list.
- **ThumbPaneImpl**: Implements the `ThumbPane` interface to handle painting a thumb for scrollbars.
- **CreateTrekScrollPane**: Creates a scroll pane with optional buttons and images, handling default image loading if no custom images are provided.
- **ScrollPaneFactory, ListPaneFactory, EditPaneFactory, StringListWithScrollBarPaneFactory, ListWithScrollBarPaneFactory**: Factory classes to create various types of panes dynamically using `ObjectStack`.
- **HoverSiteImpl**: Manages hover states for UI elements, tracking IDs when entered or left.
- **HoverPane and HoverPaneColumns**: Panes that trigger hover events on mouse enter and leave based on column positions.
- **ParentPaneFactory**: Creates a parent pane that can contain other panes.

**CONNECTS TO:** Other UI-related files and classes within the Allegiance game, particularly those involved in rendering and handling user interactions.

**NOTES:** The `ThumbPaneImpl` class has a TODO comment indicating that code for handling `pImageThumb == NULL` should be added.

## `src/effect/efpane.h`

PURPOSE: This file contains declarations and definitions related to various types of effect panes in the Allegiance game, including black background panes, hover sites, thumb panes, trek scroll bars, and gauge panes.

KEY PARTS:
- `Window`: Represents a window in the game interface.
- `SetEffectWindow`: Sets the effect window for the game.
- `Modeler`: Manages the creation of graphical models.
- `IPopupContainer`: Interface for managing popup containers.
- `AddPaneFactories`: Adds factories for creating various pane types to a namespace.
- `CreateBlackPane`: Creates a black background pane.
- `HoverSite`: Represents a hover site with enter and leave events.
- `CreateHoverSite`: Creates a hover site with a unique identifier.
- `CreateHoverPane`: Creates a hover pane associated with a hover site.
- `ThumbPane`: A pane that displays images in a thumb format.
- `CreateTrekScrollPane`: Creates a scroll pane with trek-specific styling.
- `GaugePane`: A pane that displays a gauge with customizable colors and values.
- `ParseIntVector`, `ParseFloatVector`, `ParseStringVector`: Functions to parse integer, float, and string vectors from an object.

CONNECTS TO: `pane.h`, `tref.h`, `value.h`, `ztime.h`

NOTES: None.

## `src/effect/efpopup.cpp`

PURPOSE:
The file `efpopup.cpp` contains the implementation for a message box dialog in the Allegiance game. It handles creating and managing a popup window with text and buttons.

KEY PARTS:
- `MessageBoxImpl`: A class that implements the `IMessageBox` interface to create and manage a message box.
- `CreateMessageBox`: A function to instantiate and return an instance of `MessageBoxImpl`.

CONNECTS TO:
Depends on other classes such as `Modeler`, `Pane`, `ButtonPane`, `WrapImage`, `ITimerEventSource`, and `TrekResources`.

NOTES:
- Contains memory leak fixes.
- Handles keyboard input for closing the message box with escape or return keys.

## `src/effect/efpopup.h`

PURPOSE: This file defines an interface and a function for creating message boxes in the Allegiance game. It includes classes and functions related to popup messages with buttons.

KEY PARTS:
- IMessageBox: An interface that extends IPopup, providing methods to get an event source.
- CreateMessageBox: A function to create a message box with various parameters such as timer event source, modeler, text, button panes, default button settings, and paint delay.

CONNECTS TO: popup.h

NOTES: None.

## `src/effect/efsound.cpp`

PURPOSE: This file contains implementations of various sound template factories for managing and manipulating sound effects in the Allegiance game engine. It allows for creating different types of sound templates such as wave files, 3D sounds, cones, pitch adjustments, gain changes, priorities, looping, ASR (Attack-Sustain-Release), paired sounds, repeating fire sounds, random sounds, and intermittent sounds.

KEY PARTS:
- WaveTemplateFactory: Creates a sound template from a WAV file.
- ThreeDSoundTemplateFactory: Creates a 3D sound template with specified minimum distance.
- ConeSoundTemplateFactory: Creates a cone-shaped sound template with inner and outer angles and gains.
- PitchSoundTemplateFactory: Adjusts the pitch of an existing sound template.
- SoundGainValueTemplate: A wrapper for adjusting the gain of a sound instance dynamically.
- GainSoundTemplateFactory: Creates a sound template with a specified gain adjustment.
- PrioritySoundTemplateFactory: Sets the priority of an existing sound template.
- LoopingSoundTemplateFactory: Makes an existing sound template loop.
- ASRSoundTemplateFactory: Creates an ASR (Attack-Sustain-Release) sound template.
- PairedSoundTemplateFactory: Combines two sound templates into one.
- RepeatingFireSoundTemplateFactory: Creates a repeating fire sound template with a specified repeat rate.
- RandomSoundTemplateFactory: Creates a random sound template from a list of weighted sound templates.
- IntermittentSoundTemplateFactory: Creates an intermittent sound template that plays at regular intervals.

CONNECTS TO: Allegiance game engine's sound system and modeler classes.

NOTES: None.

## `src/effect/efsound.h`

PURPOSE: tiny file (14 lines), shown in full rather than summarized.

    #ifndef _efsound_H_
    #define _efsound_H_
    //////////////////////////////////////////////////////////////////////////////
    //
    // effect sounds
    //
    //////////////////////////////////////////////////////////////////////////////
    class INameSpace;
    class Modeler;
    void AddSoundFactories(
        INameSpace*      pns,
        Modeler*         pmodeler
    );
    #endif

## `src/effect/efwindow.cpp`

PURPOSE: This file contains the implementation of the EffectWindow class, which is a subclass of EngineWindow used for managing windows in the Allegiance game.

KEY PARTS:
- EffectWindow: A class that inherits from EngineWindow and represents a window in the game.
- Constructor: Initializes an instance of EffectWindow with various parameters such as application pointer, configuration, command line, title, fullscreen flag, rectangle, minimum size, and menu handle.

CONNECTS TO: efapp.h (EngineWindow)

NOTES: None.

## `src/effect/efwindow.h`

PURPOSE: This file defines the `EffectWindow` class, which is a subclass of `EngineWindow` used for creating windows related to effects in the Allegiance game.

KEY PARTS:
- EffectApp*: Pointer to the application object.
- UpdatingConfiguration*: Configuration settings for the window.
- strCommandLine: Command line arguments passed to the window.
- strTitle: Title of the window (optional).
- bStartFullscreen: Boolean indicating whether to start in fullscreen mode.
- rect: Window rectangle.
- sizeMin: Minimum window size.
- hmenu: Handle to a menu.

CONNECTS TO: enginewindow.h

NOTES: None.

## `src/effect/explosion.cpp`

PURPOSE: This file contains the implementation of an explosion effect in Allegiance, including both shockwaves and individual explosions.

KEY PARTS:
- ExplosionGeoImpl: Implements the `ExplosionGeo` interface to manage and render explosion effects.
  - Contains nested classes `ExplosionData` and `ShockwaveData` to store data for each explosion and shockwave.
  - Manages lists of shockwaves and multiple lists of individual explosions.
  - Provides methods to add explosions, render shockwaves, render individual explosions, and render all effects.

- CreateExplosionGeo: Factory function to create an instance of `ExplosionGeoImpl`.

CONNECTS TO: 
- Depends on classes like `AnimatedImage`, `Vector`, `Color`, `Number`, `TRef`, `TList`, and `Context` from other files in the Allegiance codebase.

NOTES:
- None.

## `src/effect/explosion.h`

PURPOSE: Defines an interface for managing and rendering explosions in the game.

KEY PARTS:
- ExplosionGeo: Abstract base class representing explosion geometry with methods to add explosions at specific positions.
- AddExplosion: Pure virtual function that specifies parameters needed to create an explosion, including position, velocity, radius, color, and images.
- CreateExplosionGeo: Function to create an instance of the ExplosionGeo class.

CONNECTS TO: image.h, model.h

NOTES: None.

## `src/effect/framedata.cpp`

PURPOSE: The file `framedata.cpp` contains the implementation for managing and serializing frame data in Allegiance. Frame data includes properties like position, forward direction, and up vector for objects over time.

KEY PARTS:
- `GetString(int indent, const FrameData& data)`: Converts a `FrameData` object to a string representation with indentation.
- `FrameDataListValue::GetString(int indent)`: Converts a list of `FrameData` objects into a string representation.
- `FrameDataListValue::Write(IMDLBinaryFile* pmdlFile)`: Writes a list of `FrameData` objects to a binary file.

CONNECTS TO: `framedata.h`, `base.h`

NOTES: None.

## `src/effect/framedata.h`

PURPOSE: This file defines a class `FrameData` to represent a frame in a game, including its name and spatial orientation, as well as a class `FrameDataListValue` that manages a list of `FrameData` objects.

KEY PARTS:
- FrameData: Represents a single frame with properties for name, position, forward direction, and up direction.
- FrameDataListValue: Inherits from Value and manages a list of FrameData objects, providing methods to get the list, generate a string representation, and write to a binary file.

CONNECTS TO: Uses classes `ZString`, `Vector`, and `IMDLBinaryFile` from other files in the project.

NOTES: None.

## `src/effect/gamestate.cpp`

PURPOSE: This file contains the implementation of a game state container that manages and displays various game-related information such as time elapsed, remaining time, and custom tiles.

KEY PARTS:
- GameStateContainerImpl: Manages the layout and display of game state information.
- AddGameStateTile: Adds new game state tiles with titles and content.
- GetImage: Returns the image representing the current game state.
- SetTimeElapsed: Updates the displayed elapsed time.
- SetTimeRemaining: Updates the displayed remaining time.
- GetCloseEvent: Provides an event source for closing actions.

CONNECTS TO:
- Modeler, IEngineFont, ButtonPane, Pane, BorderPane, ColumnPane, ImagePane, StringPane

NOTES: None.

## `src/effect/gamestate.h`

PURPOSE:
This file defines an interface and a factory function for managing the game state in Allegiance. It provides methods to interact with various UI elements related to the game state.

KEY PARTS:
- GameStateContainer: Interface class defining methods to get image, font, add game state tiles, set time remaining and elapsed, and get close event.
- CreateGameStateContainer: Factory function to create an instance of GameStateContainer.

CONNECTS TO:
effect/gamestate.cpp

NOTES:
none

## `src/effect/help.cpp`

PURPOSE: The file `help.cpp` contains the implementation for a help system in Allegiance, providing functionality to display and navigate through help topics.

KEY PARTS:
- HelpPaneImpl: Main class implementing the help pane, including navigation, button events, and topic setting.
- CreateHelpPane: Factory function to create an instance of HelpPaneImpl.
- MainSink and SecondarySink: Event sinks for handling main and secondary topic events.
- OnButtonBack, OnMainTopic, OnSecondaryTopic: Methods to handle user interactions with the help pane.

CONNECTS TO: `help.h`, `controls.h`, `button.h`, `namespace.h`

NOTES: None.

## `src/effect/help.h`

**PURPOSE:** This file defines interfaces and classes related to the help system in Allegiance, including navigation panes, page panes, and a main help pane.

**KEY PARTS:**
- **NavPane:** Interface for a navigation pane that can display topics and handle events.
- **PagePane:** Interface for a page pane that can display content, set attributes, and handle link events.
- **HelpPane:** Interface for the main help pane that can set topics and strings, and get event sources.
- **AddNavPaneFactory:** Function to add a factory for creating navigation panes.
- **AddPagePaneFactory:** Function to add a factory for creating page panes.

**CONNECTS TO:** `event.h`, `pane.h`

**NOTES:** None.

## `src/effect/listpane.cpp`

**Purpose:** The `listpane.cpp` file contains the implementation of a list pane component used in Allegiance for displaying and interacting with lists of items. It manages painting, selection, and user input.

**Key Parts:**
- **Class: DefaultItemPainter**: Provides basic painting functionality for items.
- **Class: ListPaneImpl**: Manages the display, selection, and interaction with a list of items.
- **Function: ListPaneImpl::ListPaneImpl()**: Initializes the list pane.
- **Function: ListPaneImpl::~ListPaneImpl()**: Cleans up resources.
- **Function: ListPaneImpl::GetSignificantSize()**: Returns the significant dimension of the list pane.
- **Function: ListPaneImpl::SetList()**: Sets a new list and updates selection.
- **Function: ListPaneImpl::SetItemPainter()**: Sets a custom item painter or uses the default one.
- **Function: ListPaneImpl::SetSelection()**: Sets the current selection.
- **Function: ListPaneImpl::ScrollToItem()**: Scrolls to a specific item.
- **Function: ListPaneImpl::Paint()**: Handles painting of list items.

**Connects To:** `effect/listpane.h`, `itempainter.h`, `listpane.h`, `stringlist.h`, `stringlistpane.h`

**Notes:** Contains TODO comments and assertions for handling unexpected conditions.

## `src/effect/listpane.h`

PURPOSE: This file defines interfaces and implementations for a list pane component in the Allegiance game, which is used to display lists of items with scrollable functionality.

KEY PARTS:
- ItemPainter: Interface for painting individual items in the list.
- ListPane: Base class for a scrollable list pane, providing methods for setting the list, item painter, selection, and scrolling.
- StringList: Concrete implementation of List and ItemPainter for lists of strings.
- StringListItem: Represents an item in a StringList.
- StringListPane: Concrete implementation of ListPane that uses StringList.

CONNECTS TO: event.h, genericlist.h, pane.h

NOTES: None.

## `src/effect/mmlpopup.cpp`

PURPOSE: This file contains the implementation of a popup window for displaying help information in Allegiance. It includes functionality to show web pages and handle button clicks.

KEY PARTS:
- MMLPopup: Class representing the help popup window.
  - Manages buttons, page content, and scroll bars.
  - Handles events like button clicks and key presses.
- OnButtonClose: Closes the popup when the close button is clicked.
- OnButtonDiscord/OnButtonWiki: Opens Discord/Wiki web pages when their respective buttons are clicked.
- ShowWebPage: Uses ShellExecute to open a URL in the default browser.

CONNECTS TO:
- help.h, button.h, controls.h, event.h, popup.h, namespace.h (Allegiance header files for UI components and events).

NOTES:
- None.

## `src/effect/navpane.cpp`

PURPOSE: The file `navpane.cpp` contains the implementation of a navigation pane component used in the Allegiance game. This component displays a hierarchical list of topics that can be interacted with using mouse events.

KEY PARTS:
- **NavPaneImpl**: Main class representing the navigation pane.
  - Manages a list of topics and updates their visibility based on selection.
  - Handles mouse events to trigger actions associated with topics.
- **Topic**: Represents an individual topic within the navigation pane.
  - Contains properties like text, color, and indentation.
  - Responds to mouse enter, leave, and button click events.
- **NavPaneFactory**: Factory class for creating instances of `NavPaneImpl`.
  - Provides a way to instantiate `NavPaneImpl` through a function call.

CONNECTS TO: 
- Depends on classes from the Allegiance engine, such as `IEngineFont`, `ColumnPane`, and `AnimatedImagePane`.

NOTES:
- None.

## `src/effect/pagepane.cpp`

**PURPOSE:** The `pagepane.cpp` file in Allegiance is responsible for managing the creation and rendering of various UI elements such as words, strings, tabs, hyperlinks, and links within a page. It handles parsing input text to create different types of panes and manages color changes on mouse hover.

**KEY PARTS:**
- **Class: TabWordPane:** Manages tab size calculation based on position.
- **Class: LinkPane:** Handles mouse events for triggering actions and changing text color, emitting an event on link click.
- **Class: HyperlinkPane:** Similar to `LinkPane`, but opens web pages instead of triggering events, validating URLs before opening them.
- **Class: PagePaneImpl:** Manages insertion of various elements like words, strings, tabs, hyperlinks, and links, parsing input text to create panes, and handling color changes on mouse hover.
- **Function: InsertWord:** Inserts a word pane with specified font and color at the bottom of the page.
- **Function: InsertString:** Inserts a string pane with specified font and color at the bottom of the page.
- **Function: InsertTab:** Inserts a tab pane at the bottom of the page.
- **Function: InsertHyperlink:** Parses input to create and insert a hyperlink pane, validating URLs before insertion.
- **Function: InsertLink:** Parses input to create and insert a link pane, triggering an event on click.
- **Function: Error:** Sets text color to red and inserts an error message at the bottom of the page.
- **Function: MatchBrace:** Ensures braces are properly matched in the input string.
- **Function: ParseComment:** Parses comments by matching braces.
- **Function: ParseBullet:** Parses bullet points, handling nested pages within them.
- **Function: ParseSymbol:** Parses symbols up to a specified delimiter.
- **Function: Include:** Includes external files based on provided identifier and file path.

**CONNECTS TO:** Other UI components and classes such as `Pane`, `ScrollPane`, and `Modeler`.

**NOTES:** The code contains TODO comments and unusual patterns, including the use of `TRef` smart pointers for memory management. Some tags like "Bullet" are handled in `ParseTag` but lack implementation. Helper functions like `InsertLink` and `InsertHyperlink` are referenced but not defined in this snippet.

## `src/effect/panes.cpp`

PURPOSE:
The file `src/effect/panes.cpp` contains the implementation for a gauge pane component used in Allegiance. This component is responsible for rendering a visual representation of a value within a specified range, with optional flashing effects.

KEY PARTS:
- GaugePane: A class representing a gauge pane that can display a value and optionally flash when the value changes.
  - Constructor initializes the gauge pane with surface, color settings, and value range.
  - Paint: Renders the gauge pane on a given surface, showing the current value and optional flashing effect.
  - SetValue: Updates the internal value of the gauge based on a normalized input value.
  - Update: Handles the logic for updating the flashing effect and triggering repaints when necessary.

CONNECTS TO:
- `Surface`: The class representing a drawable surface where the gauge pane is rendered.
- `Color`: A utility class for handling color values.
- `Time`: A utility class for managing time-related operations, such as calculating elapsed time.

NOTES:
- None.

## `src/effect/particlegeo.cpp`

PURPOSE: This file defines particle behaviors and manages particle rendering in the Allegiance game.

KEY PARTS:
- SmokeBehavior: Manages smoke particle effects.
- FireBehavior: Handles fire particle effects with varying intensities over time.
- DamageBehavior: Intended for damage effects but currently uses smoke behavior instead.
- AfterburnerBehavior: Renders afterburner smoke with a ramp-up and fade-out effect.
- SparkBehavior: Creates spark effects that blur as they age.
- ParticleData: Holds data for individual particles, including their behavior and rendering properties.
- ParticleGeoImpl: Manages the particle system, creating and updating particles based on behaviors.

CONNECTS TO: thinggeo.h (for dependencies related to game objects).

NOTES: None.

## `src/effect/particlegeo.h`

PURPOSE: This file defines the interface and implementation for particle effects in the Allegiance game. It includes classes for managing particle data and behaviors.

KEY PARTS:
- ParticleBehavior: Abstract base class defining a method to render particles.
- ParticleData: Holds properties of individual particles such as position, velocity, size, and behavior.
- ParticleGeo: Inherits from Geo and provides methods to add various types of particle effects like afterburner, damage, smoke, fire, and sparks.

CONNECTS TO: model.h, value.h, vector.h, geo.h

NOTES: None

## `src/effect/starimage.cpp`

PURPOSE: This file contains the implementation of a star background image effect for a game viewport.

KEY PARTS:
- StarImageImpl: A class that implements the StarImage interface to manage and render star background images.
- StarData: A nested struct within StarImageImpl that holds data for each star, including direction, position, color, and first frame flag.
- SetCount: A method to initialize the star positions and colors based on a seed and count.
- Render: A method to render the star background by projecting star positions onto the screen and drawing points or lines.

CONNECTS TO: Viewport, Camera, Context

NOTES: None

## `src/effect/starimage.h`

PURPOSE: This file defines an interface for rendering star images in a viewport.

KEY PARTS:
- StarImage: A class that inherits from Image and represents a star image with methods to set the count of stars and render them.
- Create: A static method to create an instance of StarImage.
- SetCount: An abstract method to set the number of stars based on a seed value.
- Render: An abstract method to render the star image.

CONNECTS TO: Image, Viewport

NOTES: None.

## `src/effect/thinggeo.cpp`

PURPOSE: The file `src/effect/thinggeo.cpp` contains implementation details for managing and rendering game entities' geometry, including their position, orientation, radius, and visual effects like thrust particles and trails.

KEY PARTS:
- ThingRef: Manages references to objects with reference counting.
- ThingGeoImpl: Implements the ThingGeo interface, handling properties and methods related to game entities.
- ThingGeo: Manages rendering and evaluation of a thing's geometry, including scaling, rendering, and evaluating state changes.
- Thrust: Manages thrust properties and visual effects like glow and particle emission.
- AddThrustParticles: Adds thrust particles based on frame velocity and acceleration.
- LoadMDL: Loads a model from an MDL file.
- TransformOffset: Transforms a vector based on orientation and scale.
- GetChildModelOffset: Retrieves the offset of a child model by name.
- SetVisible: Sets whether the thing is visible.
- RenderTargetGeo: Renders the target geometry of the object.
- RenderGeo: Handles the main rendering logic of the object.

CONNECTS TO: Modeler, Number, Context, Geo interface

NOTES:
- The use of random function for generating random values.
- "some awkwardness of the engine means that we have to use the last position but current characteristics like velocity and such."

PARTS (in file order):

- lines 1-366
  - ThingRef: A template class for managing references to objects with reference counting.
  - ThingGeoImpl: The implementation of the ThingGeo interface, containing various properties and methods related to game entities.
  - m_pmodeler: A reference to a Modeler object used for rendering.
  - m_ptime: A reference to a Number object representing time.
  - m_timeLast: Stores the last recorded time.
  - m_radius: Radius of the entity.
  - m_orientation: Orientation of the entity.
  - m_vecPosition: Current position of the entity.
- lines 368-729
  - ThingGeo: Manages rendering and evaluation of a thing's geometry.
  - GetRadius(const Matrix& mat): Returns the radius of the thing's geometry based on a transformation matrix.
  - Render(Context pcontext): Scales the target geometry to unit radius and renders it.
  - Evaluate(): Evaluates the state of the thing and notifies changes.
  - ThingGeoWrapper: A wrapper class for ThingGeoImpl that implements the Geo interface.
  - GetGeo(): Returns a new instance of ThingGeoWrapper.
  - Thrust: Manages thrust properties and visual effects like glow and particle emission.
  - UpdateGlow(): Updates the glow color and scale based on current thrust value.
- lines 730-1087
  - AddThrustParticles: Adds thrust particles based on frame velocity and acceleration.
  - ComputeFireParticleParameters: Computes parameters for fire particles.
  - ComputeSmokeParticleParameters: Computes parameters for smoke particles.
  - SetTrailColor: Sets the color of the trail effect.
  - LoadMDL: Loads a model from an MDL file.
  - TransformOffset: Transforms a vector based on orientation and scale.
  - TODO: "some awkwardness of the engine means that we have to use the last position but current characteristics like velocity and such."
  - Unusual code: The use of random function for generating random values, which is not shown in the provided snippet.
- lines 1089-1466
  - GetChildModelOffset: Retrieves the offset of a child model by name.
  - GetChildOffset: Calculates the offset of a child based on its model offset and orientation.
  - GetRadius: Returns the current radius of the thing.
  - GetRadius(const Matrix& mat): Returns the radius scaled by a given matrix.
  - SetVisible: Sets whether the thing is visible.
  - SetVisibleShip: Sets visibility specifically for ships.
  - UpdateTrailPosition: Updates the position of the trail.
  - SetPosition: Sets the new position of the thing and updates the trail if necessary.
- lines 1467-1775
  - ThingGeo: Main class for rendering a thing with geometry.
  - RenderTargetGeo: Renders the target geometry of the object.
  - RenderGeo: Handles the main rendering logic of the object.
  - Evaluate: Updates the state and evaluates the object's properties.
  - s_lodBias: LOD bias factor for level of detail.
  - s_bShowLights, s_bShowHullHits, s_bShowTrails, s_bShowBounds: Flags to control visibility of various elements.
  - SetTransparentObjects, GetTransparentObjects: Functions to set and get the transparent objects flag.
  - SetShowTrails, GetShowTrails: Functions to set and get the show trails flag.

## `src/effect/thinggeo.h`

PURPOSE: The file defines the `ThingGeo` class, which is a base class for representing objects in the game that can have geometric properties and behaviors.

KEY PARTS:
- ThingGeo: Base class for objects with geometric properties.
- GetGeo(): Returns the primary geometry of the object.
- GetTargetGeo(): Returns the target geometry of the object.
- AddFlare(): Adds a flare effect to the object.
- SetPosition(), SetOrientation(): Sets the position and orientation of the object.
- SetVisible(): Controls whether the object is visible.
- LoadMDL(), Load(): Loads the model and texture for the object.

CONNECTS TO: `BitsGeo`, `Geo`, `Image`, `Modeler`, `ParticleGeo`

NOTES: None.
