# src/ui

[Back to the index](../LLAMA_SUMMARY.md) - 18 files, 3,081 lines. Notes by a local model, to orient by: not verified facts.

The `src/ui` folder in Allegiance contains essential components for managing and rendering user interfaces within the game. It includes classes and functions for handling UI states, image loading, event management, and screen interactions.

- Exposer.h: Provides mechanisms to expose types and functionality to Lua scripts.
- UiEngine.cpp: Implements the core logic for loading and managing UI elements from Lua scripts.
- UiEngine.h: Declares interfaces and utilities for the UI engine.
- UiState.cpp: Manages different states of the user interface within the game.
- UiState.h: Defines classes and structures for handling UI states and objects.
- ns_image.hpp: Offers functionalities for loading, manipulating, and managing images in the UI.

Largest files: UiEngine.cpp (500 lines), ui_types.h (373 lines), ns_event.hpp (326 lines), ns_image.hpp (315 lines), ns_screen.hpp (255 lines).

## `src/ui/Exposer.h`

PURPOSE: tiny file (20 lines), shown in full rather than summarized.

    #pragma once
    #define SOL_CHECK_ARGUMENTS 1
    #define SOL_ENABLE_INTEROP 1
    #include "sol.hpp"
    class Exposer {
    protected:
        std::shared_ptr<void> m_pobject;
    public:
        Exposer(std::shared_ptr<void>& pobject) : m_pobject(pobject) {
        }
        template <typename T>
        Exposer(T object) {
            m_pobject = std::make_shared<T>(object);
        }
        virtual sol::object ExposeSolObject(lua_State* L) = 0;
        template <typename T>
        operator const T&() const {
            return *std::static_pointer_cast<T>(m_pobject);
        }
    };

## `src/ui/UiEngine.cpp`

PURPOSE: This file contains the implementation of the UI engine for Allegiance, responsible for loading and managing user interface elements from Lua scripts.

KEY PARTS:
- UiEngineImpl: Main class implementing the UI engine.
- InnerLoadImageFromLua: Loads an image from a Lua script.
- ImageReloadSink: Reloads an image when triggered by an event.
- LoadImageFromLua: Public method to load images that can be reloaded dynamically.
- WriteLog: Writes log messages to a file if configured.

CONNECTS TO:
- Window, Engine, ISoundEngine, and other UI-related classes for context and functionality.

NOTES:
- Contains logic to handle keyboard focus and mouse events within UI elements.
- Uses Lua scripting for dynamic UI element creation and behavior.

## `src/ui/UiEngine.h`

PURPOSE: This file contains declarations for various components related to the user interface (UI) engine of Allegiance, including screen configurations, image loading, and Lua scripting.

KEY PARTS:
- UiScreenConfiguration: Manages UI screen configurations with event listeners.
- UiEngine: Main class managing the UI engine, including image loading and triggering reloads.
- PathFinder: Finds file paths among a list of directories.
- Loader: Loads Lua scripts using a provided file loader.
- Executor: Executes Lua functions within a script context.
- LuaScriptContext: Manages the Lua scripting environment, providing access to various game systems.

CONNECTS TO: FileLoader, Window, Engine, ISoundEngine

NOTES: None.

## `src/ui/UiState.cpp`

PURPOSE: This file contains definitions and implementations related to the `UiState` class, which is used in the Allegiance game for managing user interface states.

KEY PARTS:
- UiState<PropertiesContainerType>: A template class representing a UI state with a name and properties.
- Write(IMDLBinaryFile*, const std::shared_ptr<UiState>&): A function to write `UiState` objects to an MDL binary file.
- GetString(int, const std::shared_ptr<UiState>&): A function to get a string representation of a `UiState`.
- GetFunctionName(const std::shared_ptr<UiState>&): A function to get the function name associated with a `UiState`.

CONNECTS TO: UiObjectContainer (indirectly through template parameter)

NOTES: None.

## `src/ui/UiState.h`

PURPOSE: This file defines various UI-related classes and structures used in the Allegiance game for managing user interface states and objects.

KEY PARTS:
- UiObjectContainer: Manages a collection of Exposer objects, providing methods to retrieve them by key.
- UiState: A subclass of UiObjectContainer that includes a name and provides type-safe accessors for its contained values.
- SimpleUiState: A simple implementation of UiState.
- UiList: A generic list class that can hold any type of entry and supports operations like insert, remove, and evaluate.
- ContainerList and ImageList: Specialized versions of UiList for holding UiObjectContainer and Image objects, respectively.
- MappedList: A specialized version of UiList that maps entries from one type to another using a callback function.

CONNECTS TO: model.h (likely contains definitions for Exposer, StringValue, Boolean, Number, etc.)

NOTES: None.

## `src/ui/items.hpp`

PURPOSE: This file defines a class `MouseEventImage` that extends `WrapImage` to handle mouse events and provide event sources for various mouse actions.

KEY PARTS:
- `MouseEventImage`: A class that wraps an image and handles mouse events like clicks, drags, and scrolls.
  - Manages event sources for different mouse events.
  - Triggers appropriate events based on mouse interactions.
- `GetEventSource` and `GetPointEventSource`: Methods to retrieve or create event sources for mouse events.
- `Trigger`, `TriggerPoint`: Methods to manually trigger events.
- `MouseEnter`, `MouseLeave`: Methods to handle mouse entering and leaving the image area.
- `MouseMove`: Handles mouse movement, triggering drag events if captured.
- `Button`: Handles mouse button presses and releases, capturing and releasing based on conditions.

CONNECTS TO: ui.h, enginep.h, font.h, valuetransform.h, imagetransform.h

NOTES: None.

## `src/ui/ns_color.hpp`

PURPOSE: This file defines a namespace for color-related operations in the Allegiance game, providing functions to create and manipulate colors through Lua scripting.

KEY PARTS:
- ColorNamespace: A class that encapsulates functionality related to color namespaces.
- AddNamespace: A static function within ColorNamespace that registers color creation functions with a Lua state.

CONNECTS TO: ui.h, items.hpp, sol::state (Lua binding library)

NOTES: The commented-out code in the file shows an alternative implementation of the `Create` function using overloads. The current implementation uses optional parameters and handles cases where alpha values are not provided by defaulting to 1.0f.

## `src/ui/ns_event.hpp`

PURPOSE: This file contains various event-handling utilities and classes for managing and propagating events in the Allegiance game.

KEY PARTS:
- EventToBoolean: Converts an event source to a boolean value.
- EventToMappedValue: Maps an event source to a specific value type.
- EventToMappedImage: Maps an event source to an image.
- EventOneToOne: Creates a one-to-one mapping between two types of events.
- EventVoidToOne: Propagates a void event to a single result.
- EventVoidToMultiple: Propagates a void event to multiple results.
- EventNamespace: Provides Lua bindings for event handling utilities.

CONNECTS TO: ui.h, items.hpp

NOTES: None.

## `src/ui/ns_file.hpp`

PURPOSE: tiny file (13 lines), shown in full rather than summarized.

    #pragma once
    #include "ui.h"
    #include "items.hpp"
    class FileNamespace {
    public:
        static void AddNamespace(sol::state* m_pLua, Loader* pLoader) {
            sol::table table = m_pLua->create_table();
            table["LoadLua"] = [pLoader](std::string path) {
                return pLoader->LoadScript(path);
            };
            m_pLua->set("File", table);
        }
    };

## `src/ui/ns_font.hpp`

PURPOSE:
The file `ns_font.hpp` defines a namespace for managing fonts in the Allegiance game. It provides functions to create and manipulate font objects using Lua scripting.

KEY PARTS:
- FontNamespace: A class containing static methods for interacting with fonts.
- AddNamespace: Registers Lua bindings for creating and querying font objects.
- Create: Creates a new font object with specified properties like size, bold, italic, and underline.
- Height: Retrieves the height of a given font object.

CONNECTS TO:
ui.h, items.hpp, sol2 (Lua binding library)

NOTES:
None.

## `src/ui/ns_image.hpp`

PURPOSE: This file defines a namespace `ImageNamespace` that provides functionality for loading and manipulating images in the Allegiance game. It includes functions to load images from files, create various image transformations, and manage image groups.

KEY PARTS:
- `LoadImageFile`: Loads an image from a specified path.
- `ConstantImage`, `Image`: Classes representing immutable and mutable images respectively.
- `TRef<Image>`, `TRef<ConstantImage>`: Template references for managing image objects.
- `AddNamespace`: Registers Lua bindings for image-related functions and classes.

CONNECTS TO:
- `ui.h`, `items.hpp`, `D3DDevice9.h`: Core UI and graphics headers.
- `LuaScriptContext`, `ZFile`, `ZString`, `PathString`: Dependencies on the game's scripting and file handling systems.

NOTES: None.

## `src/ui/ns_list.hpp`

PURPOSE: This file defines utility classes and functions for manipulating lists in the Allegiance game's user interface.

KEY PARTS:
- ListCount: A class that counts the number of entries in a list.
- sortReturnIndices: A function that returns indices of a vector sorted by its values.
- SortedList: A class that sorts a list based on a callback function returning a Number for each entry.
- ListNamespace: A namespace providing Lua bindings for list manipulation functions.

CONNECTS TO: ui.h, items.hpp, UiState.h

NOTES: None.

## `src/ui/ns_math.hpp`

PURPOSE: This file defines namespaces and functions to expose mathematical operations and related classes to Lua scripting in the Allegiance game engine.

KEY PARTS:
- `NumberNamespace`: Provides functions for creating numbers, performing arithmetic operations, and converting numbers to strings.
- `RectNamespace`: Defines a namespace for rectangle operations, including creation and transformation.
- `BooleanNamespace`: Offers functionality for boolean values, including logical operations and conversions.

CONNECTS TO: `ui.h`, `items.hpp`, `sol2` (Lua binding library)

NOTES: None.

## `src/ui/ns_point.hpp`

PURPOSE: This file defines a namespace for handling points in the game, providing Lua bindings for point operations and creating event sinks.

KEY PARTS:
- PointNamespace: Contains static methods to add Lua bindings for point-related functionality.
- AddNamespace: Registers Lua functions and classes related to points.
- SimpleModifiableValue<Point>: A class representing a modifiable point value with Lua bindings.
- Create: Creates a new point from x and y values.
- CreateEventSink: Creates an event sink for a point.
- X and Y: Functions to get the x and y coordinates of a point.

CONNECTS TO: ui.h, items.hpp, sol.hpp (Lua binding library)

NOTES: None.

## `src/ui/ns_screen.hpp`

PURPOSE: This file defines various utility classes and functions for managing screen-related events and interactions in the Allegiance game.

KEY PARTS:
- PlaySoundSink: A class that plays a sound when an event is triggered.
- EventMapper: A template class that maps one type of event to another by applying a callback function.
- VoidEventMapper: A specialized version of EventMapper that doesn't transform the event value.
- EventFilter: A template class that filters events based on a predicate.
- ScreenNamespace: A namespace containing functions and classes for interacting with screen-related data and events.

CONNECTS TO: ui.h, items.hpp, ISoundEngine, ISoundTemplate, IEventSink, TRef, sol::table

NOTES: None.

## `src/ui/ns_string.hpp`

PURPOSE: This file defines a namespace for string operations and provides a mechanism to switch between different string values based on the input value.

KEY PARTS:
- `ValueToMappedValue<TypeResult, TypeArgument>`: A template class that maps an input value to a corresponding output value.
- `StringNamespace`: Contains functions to add string-related functionality to a Lua script context.
- `AddNamespace`: Registers string-related classes and functions with the Lua environment.

CONNECTS TO:
- `ui.h`
- `items.hpp`
- `TWrapValue<TypeResult>`
- `TStaticValue<TypeArgument>`
- `SimpleModifiableValue<ZString>`
- `StringValue`
- `Number`
- `Boolean`

NOTES: None.

## `src/ui/ui.h`

PURPOSE: This file contains definitions and declarations for various UI-related components in the Allegiance game, including classes for exposing types to Lua, managing callbacks, and integrating with the UI engine.

KEY PARTS:
- `TypeExposer<T>`: A template class that exposes a C++ object to Lua.
- `TStaticValueExposer<T>`: A specialized version of `TypeExposer` for static values.
- `BooleanExposer`, `NumberExposer`, `StringExposer`: Convenience typedefs for exposing boolean, numeric, and string values.
- `Exposed`: A class that encapsulates a Lua object.
- `CallbackSink`: An event sink that invokes a callback function when an event occurs.
- `CallbackValueSink<Types...>`: A template class for event sinks that pass event values to a callback function.

CONNECTS TO:
- `soundengine.h`, `model.h`, `image.h`, `engine.h`, `Exposer.h`, `UiState.h`, `ui_types.h`: Other UI-related headers.
- `UiEngine.h`: The main UI engine class.

NOTES: None.

## `src/ui/ui_types.h`

PURPOSE: This file contains type checking and retrieval mechanisms for various UI-related types in Allegiance, facilitating interaction between Lua scripts and C++ code.

KEY PARTS:
- `UiList<T>`: A template class representing a list of UI elements.
- `TRef<TStaticValue<T>>`: Represents a reference to a static value that can be modified.
- `TEvent<bool>::Sink`, `TEvent<float>::Sink`, etc.: Sink types for event handling in the UI system.
- `Image` and `ConstantImage`: Classes representing image resources used in the UI.
- `ConvertListTypeToGenericList<T>`: Converts a specific type of list to a generic list of `sol::object`.

CONNECTS TO: The Allegiance game engine, specifically the UI subsystem.

NOTES: None.
