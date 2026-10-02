# Allegiance codebase: a file-by-file summary by a local model

Written 2026-10-02 07:51 by Qwen2.5-Coder-7B, a small language model run on one PC. It read every one of the 1458 code and text files in this repository and wrote a note on each. These are notes to orient by, not verified facts.

What was checked against the source: every code name under KEY PARTS and in the PARTS outlines occurs in the file it is listed under (names that did not were dropped), and every file name in the overviews exists. What each thing is said to do was NOT checked. Files of 20 lines or fewer are shown in full. Files read in 4 or more parts carry a PARTS outline with line numbers.

## How to use it

One page a folder, in the table below. A page has the folder's overview and then every file in it: PURPOSE, KEY PARTS, CONNECTS TO and NOTES. To find a file, open its folder's page and search the page for the file's name. Open the real file before relying on what a note says it does.

## Overview

*The model's own overview, and the least reliable part of this summary: it is vague and it mislabels some folders. The README's list of projects is the better guide to how the parts fit.*

The Allegiance source code is organized into several key directories, each containing specific components essential for the game's functionality. The `src` folder houses the core client application, which includes modules like WinTrek (user interface), engine (rendering and input), Igc (game object management), and test (COM and UI components). These components work together to provide a rich gaming experience.

The `src/Lib/steam` directory is crucial for integrating Steam services, enabling features such as user authentication and matchmaking. This folder contains essential interfaces and implementations that facilitate communication between the game and the Steam client.

For server-side logic, the `src/FedSrv` folder is vital. It manages game state, player interactions, and mission handling, providing the backbone of multiplayer functionality in Allegiance.

The `src/engine` directory contains fundamental utilities and components for managing rendering, configuration, input, and other core functionalities, ensuring smooth performance and responsiveness in the game.

For development tools and utilities, the `src/clintlib`, `src/effect`, `_Utility`, and `soundengine` folders provide essential support. These modules handle tasks such as downloading files, processing server messages, managing audio playback, and collision detection, respectively.

The `src/training` folder focuses on training missions, defining conditions, actions, and goals to help players learn specific game mechanics. This ensures that new players can quickly grasp the rules and strategies needed to succeed in Allegiance.

For managing game objects and behaviors, the `src/Pigs` folder is essential. It includes classes for handling ships, parts, asteroids, and more, providing a comprehensive system for game object management.

The `src/Reloader` and `AutoUpdate` folders contain scripts and utilities for managing file operations and ensuring that players have the latest version of the game files.

In summary, Allegiance's source code is structured into client, server, lobby, game logic, engine, and tool components. Each directory contains specific modules that work together to create a cohesive and functional game experience. A programmer should start reading in the `src` folder to understand the core functionality, then explore other directories as needed based on their interests or areas of focus.

## Folders

| folder | files | lines | what the model says it is |
|---|---|---|---|
| [(top level)](llama-summary/top-level.md) | 4 | 92 | The folder contains essential configuration and documentation files for building and managing the Allegiance game. |
| [.github/scripts](llama-summary/github-scripts.md) | 1 | 83 | This folder contains scripts used by the Allegiance project, specifically focusing on automating interactions with external services to enhance game functionality. |
| [.github/workflows](llama-summary/github-workflows.md) | 1 | 220 | The `.github/workflows` folder contains configuration files for GitHub Actions workflows used in the Allegiance project. |
| [VS2005](llama-summary/VS2005.md) | 1 | 4 | The Allegiance source code folder VS2005 contains a single text file named README.txt. |
| [VS2017](llama-summary/VS2017.md) | 3 | 225 | The folder contains documentation and configuration files related to running a local lobby and server in Allegiance. |
| [VS2022](llama-summary/VS2022.md) | 3 | 238 | The folder contains documentation and configuration files related to running a local lobby and server in Allegiance. |
| [cmake](llama-summary/cmake.md) | 2 | 24 | This folder contains toolchain configuration files for building Allegiance on MinGW (Minimalist GNU for Windows) environments. |
| [src](llama-summary/src.md) | 4 | 114 | This folder contains essential documentation and configuration files for the Allegiance source code. |
| [src/AGC](llama-summary/src-AGC.md) | 155 | 21,796 | The AGC folder in Allegiance's source code contains essential components for managing game objects and operations. |
| [src/AllSrvUI](llama-summary/src-AllSrvUI.md) | 36 | 7,238 | The `src/AllSrvUI` folder contains source code for the Allegiance game server's user interface, responsible for managing administrative tasks and player interactions. |
| [src/AutoUpdate](llama-summary/src-AutoUpdate.md) | 6 | 796 | The AutoUpdate folder in Allegiance's source code is responsible for managing automatic updates of the game files to ensure players have the latest version. |
| [src/FedSrv](llama-summary/src-FedSrv.md) | 66 | 32,750 | The `src/FedSrv` folder contains essential server-side logic and components for managing Allegiance games, including game state, player interactions, and mission handling. |
| [src/Igc](llama-summary/src-Igc.md) | 82 | 43,522 | The Igc folder in Allegiance contains core game object and mission management system components. |
| [src/Inc](llama-summary/src-Inc.md) | 19 | 9,846 | The `src/Inc` folder contains header files essential for various aspects of the Allegiance game, including networking, messaging, and database operations. |
| [src/Inc/cpp-httplib](llama-summary/src-Inc-cpp-httplib.md) | 2 | 9,252 | This folder contains essential components for HTTP client and server operations in Allegiance, a 2000 Microsoft multiplayer space game. |
| [src/Inc/ogg](llama-summary/src-Inc-ogg.md) | 2 | 347 | This folder contains core definitions and functions for working with Ogg bitstreams in Allegiance, a 2000 Microsoft multiplayer space game. |
| [src/Inc/vorbis](llama-summary/src-Inc-vorbis.md) | 2 | 449 | This folder contains essential headers and declarations for handling audio data in the OggVorbis format within the Allegiance game. |
| [src/Lang/Usa/allegiance](llama-summary/src-Lang-Usa-allegiance.md) | 1 | 44 | The Allegiance game uses a resource system to manage strings and dialog items across various parts of the application. |
| [src/Lib/AllegianceSecurity](llama-summary/src-Lib-AllegianceSecurity.md) | 1 | 110 | The `src/Lib/AllegianceSecurity` folder in the Allegiance game source code is dedicated to digital rights management (DRM) and file integrity verification. |
| [src/Lib/lua](llama-summary/src-Lib-lua.md) | 1 | 6 | The `src/Lib/lua` folder in Allegiance contains Lua-related libraries and resources used within the game. |
| [src/Lib/lua/include](llama-summary/src-Lib-lua-include.md) | 5 | 1,605 | This folder contains essential header files for integrating and utilizing Lua scripting in the Allegiance game. |
| [src/Lib/nlohmann/json](llama-summary/src-Lib-nlohmann-json.md) | 1 | 21 | The `src/Lib/nlohmann/json` folder contains a single file related to JSON handling in Allegiance. |
| [src/Lib/nlohmann/json/include](llama-summary/src-Lib-nlohmann-json-include.md) | 1 | 13,003 | The `src/Lib/nlohmann/json/include` folder contains essential components for handling JSON data in Allegiance, a 2000 Microsoft multiplayer space game. |
| [src/Lib/sol](llama-summary/src-Lib-sol.md) | 1 | 20 | The Allegiance source code folder `src/Lib/sol` contains a single file named `LICENSE.txt`, which is a tiny file (16 lines) containing the license information for the game or toolset. |
| [src/Lib/sol/include](llama-summary/src-Lib-sol-include.md) | 1 | 20,223 | The `src/Lib/sol/include` folder contains essential components for integrating Lua scripting into Allegiance, a 2000 Microsoft multiplayer space game. |
| [src/Lib/steam](llama-summary/src-Lib-steam.md) | 39 | 22,381 | The `src/Lib/steam` folder contains essential interfaces and implementations for interacting with Steam services within the Allegiance game. |
| [src/Pigs/PigAccts](llama-summary/src-Pigs-PigAccts.md) | 16 | 893 | This folder contains source code related to managing player accounts in Allegiance, a 2000 Microsoft multiplayer space game. |
| [src/Pigs/PigClient/PigClient](llama-summary/src-Pigs-PigClient-PigClient.md) | 5 | 571 | This folder contains client-side code for managing pigs in Allegiance, a multiplayer space game. |
| [src/Pigs/PigClient/PigClient/Properties](llama-summary/src-Pigs-PigClient-PigClient-Properties.md) | 3 | 144 | This folder contains properties and configuration files for the PigClient project in Allegiance. |
| [src/Pigs/PigConfig](llama-summary/src-Pigs-PigConfig.md) | 15 | 2,151 | This folder contains source code related to configuration settings in Allegiance's Pig client. |
| [src/Pigs/PigGuid](llama-summary/src-Pigs-PigGuid.md) | 1 | 2 | This folder contains source code related to GUIDs (Globally Unique Identifiers) used in Allegiance for identifying game objects and entities. |
| [src/Pigs/PigJSDocGenerator](llama-summary/src-Pigs-PigJSDocGenerator.md) | 2 | 1,075 | The `src/Pigs/PigJSDocGenerator` folder contains tools for generating JSDoc and TypeScript documentation from IDL files in Allegiance. |
| [src/Pigs/PigJSDocGenerator/Entities](llama-summary/src-Pigs-PigJSDocGenerator-Entities.md) | 4 | 63 | This folder contains classes related to generating JSDoc documentation for entities in Allegiance. |
| [src/Pigs/PigJSDocGenerator/Properties](llama-summary/src-Pigs-PigJSDocGenerator-Properties.md) | 1 | 36 | This folder contains properties and configuration files for the PigJSDocGenerator project in Allegiance. |
| [src/Pigs/PigSrv](llama-summary/src-Pigs-PigSrv.md) | 76 | 14,336 | This folder contains source code related to the Allegiance game's PigSrv component, which manages game objects and behaviors in the client-server architecture of the game. |
| [src/Pigs/Piglets](llama-summary/src-Pigs-Piglets.md) | 35 | 7,705 | This folder contains various behavior scripts and configuration files for Allegiance's Piglets AI system. |
| [src/Pigs/Piglets/include](llama-summary/src-Pigs-Piglets-include.md) | 11 | 1,719 | This folder contains JavaScript and VBScript files related to game automation and management in Allegiance. |
| [src/Pigs/Piglets/moldy](llama-summary/src-Pigs-Piglets-moldy.md) | 3 | 1,045 | This folder contains JScript behavior scripts for piglets in Allegiance, focusing on mission-related actions and state transitions. |
| [src/Pigs/PigsLib](llama-summary/src-Pigs-PigsLib.md) | 18 | 1,671 | This folder contains source code for Allegiance's PigsLib library, which provides functionality related to game objects and behaviors. |
| [src/Pigs/Scripts](llama-summary/src-Pigs-Scripts.md) | 37 | 3,117 | This folder contains JavaScript and VBScript scripts used in Allegiance for managing various aspects of the game server and client interactions. |
| [src/Reloader](llama-summary/src-Reloader.md) | 4 | 466 | The `src/Reloader` folder contains source code for a Windows application named Reloader.exe, which is responsible for managing file operations and restarting Allegiance.exe after it exits. |
| [src/WinTrek](llama-summary/src-WinTrek.md) | 89 | 67,940 | The WinTrek folder contains core functionality and user interface components of the Allegiance game client application. |
| [src/Wopr/AllegianceInterop](llama-summary/src-Wopr-AllegianceInterop.md) | 24 | 19,342 | This folder contains interop code for Allegiance, facilitating communication between managed and native C++ components. |
| [src/Wopr/AllegianceInteropClassGenerator](llama-summary/src-Wopr-AllegianceInteropClassGenerator.md) | 1 | 676 | This folder contains tools and classes used by Allegiance to generate C wrapper classes for interfacing with native C++ code. |
| [src/Wopr/AllegianceInteropClassGenerator/Properties](llama-summary/src-Wopr-AllegianceInteropClassGenerator-Properties.md) | 1 | 36 | This folder contains properties and configuration files for the AllegianceInteropClassGenerator project, which is likely responsible for generating interop classes for interfacing with external systems or libraries in Allegiance. |
| [src/Wopr/Strategies](llama-summary/src-Wopr-Strategies.md) | 3 | 1,502 | This folder contains strategies and related logic for managing game operations in Allegiance. |
| [src/Wopr/Strategies/Loadouts](llama-summary/src-Wopr-Strategies-Loadouts.md) | 2 | 91 | This folder contains classes related to loadouts in the Allegiance game, which specify various item types that can be equipped by different units. |
| [src/Wopr/Strategies/Properties](llama-summary/src-Wopr-Strategies-Properties.md) | 1 | 36 | This folder contains metadata and configuration settings for the Strategies assembly in Allegiance, a Microsoft multiplayer space game. |
| [src/Wopr/Wopr](llama-summary/src-Wopr-Wopr.md) | 8 | 2,747 | The `src/Wopr/Wopr` folder contains essential components for managing game sessions and client interactions in Allegiance. |
| [src/Wopr/Wopr/Constants](llama-summary/src-Wopr-Wopr-Constants.md) | 20 | 771 | This folder contains various constants and enumerations used throughout the Allegiance game engine to define different types of data and actions. |
| [src/Wopr/Wopr/Entities](llama-summary/src-Wopr-Wopr-Entities.md) | 10 | 322 | This folder contains classes and structures related to game entities and information management in Allegiance. |
| [src/Wopr/Wopr/Loadouts](llama-summary/src-Wopr-Wopr-Loadouts.md) | 2 | 91 | The `Loadouts` folder in Allegiance contains classes related to managing different configurations of equipment and items that can be equipped on ships. |
| [src/Wopr/Wopr/Properties](llama-summary/src-Wopr-Wopr-Properties.md) | 1 | 36 | This folder contains metadata and configuration settings for the Wopr assembly in Allegiance, a 2000 Microsoft multiplayer space game. |
| [src/Wopr/Wopr/Strategies](llama-summary/src-Wopr-Wopr-Strategies.md) | 4 | 3,855 | The `src/Wopr/Wopr/Strategies` folder in Allegiance contains essential strategy implementations for managing game operations such as research and expansion, scouting, and connection management. |
| [src/Zone](llama-summary/src-Zone.md) | 4 | 220 | The `src/Zone` folder in Allegiance contains essential components for managing game zones and their members. |
| [src/_Utility](llama-summary/src-_Utility.md) | 19 | 9,260 | The `src/_Utility` folder in Allegiance contains essential utility functions and classes used across various parts of the game. |
| [src/clintlib](llama-summary/src-clintlib.md) | 9 | 13,165 | The `src/clintlib` folder in Allegiance contains core client-side functionality essential for managing game operations and interactions. |
| [src/cvh](llama-summary/src-cvh.md) | 3 | 789 | The `src/cvh` folder contains essential components for processing convex hull data in Allegiance, a 2000 Microsoft multiplayer space game. |
| [src/effect](llama-summary/src-effect.md) | 43 | 12,547 | The `src/effect` folder in Allegiance contains source code related to game effects and visual elements. |
| [src/engine](llama-summary/src-engine.md) | 102 | 49,855 | The `src/engine` folder contains core components and utilities essential for managing rendering, configuration, input, and other fundamental functionalities in the Allegiance game engine. |
| [src/guids](llama-summary/src-guids.md) | 2 | 67 | The `src/guids` folder in Allegiance contains definitions of globally unique identifiers (GUIDs) used across various components of the game. |
| [src/lobby](llama-summary/src-lobby.md) | 32 | 5,837 | The `src/lobby` folder contains essential components of Allegiance's lobby system, responsible for managing player logins, mission creation, and server interactions. |
| [src/mdlc](llama-summary/src-mdlc.md) | 3 | 431 | The `src/mdlc` folder contains essential components for handling and compiling 3D model files in Allegiance. |
| [src/mdledit](llama-summary/src-mdledit.md) | 2 | 1,816 | The `src/mdledit` folder contains the source code for the MDL Editor application in Allegiance, which is used to create and edit 3D models for the game. |
| [src/sharemem](llama-summary/src-sharemem.md) | 3 | 506 | This folder contains source code related to shared memory and synchronization mechanisms used in performance monitoring within the Allegiance game. |
| [src/soundengine](llama-summary/src-soundengine.md) | 17 | 9,117 | The `src/soundengine` folder in Allegiance contains essential components for managing audio playback and 3D sound positioning within the game. |
| [src/test/Inc](llama-summary/src-test-Inc.md) | 11 | 3,447 | This folder contains header and implementation files related to testing and development utilities in Allegiance. |
| [src/test/TCAtl](llama-summary/src-test-TCAtl.md) | 33 | 17,100 | This folder contains source code related to COM (Component Object Model) functionality and Windows-based UI components in the Allegiance game. |
| [src/test/TCLib](llama-summary/src-test-TCLib.md) | 47 | 14,033 | This folder contains utility classes and functions for managing various aspects of the Allegiance game engine, including COM operations, threading, memory management, and string manipulation. |
| [src/test/TCObj](llama-summary/src-test-TCObj.md) | 27 | 3,125 | This folder contains source code related to COM (Component Object Model) components and utilities in Allegiance, a 2000 Microsoft multiplayer space game. |
| [src/training](llama-summary/src-training.md) | 180 | 16,282 | The `src/training` folder in Allegiance contains essential components for managing and executing training missions within the game. |
| [src/training/asteroids](llama-summary/src-training-asteroids.md) | 1 | 55 | This folder contains code related to generating random asteroid data for a game. |
| [src/ui](llama-summary/src-ui.md) | 18 | 3,081 | The `src/ui` folder in Allegiance contains essential components for managing and rendering user interfaces within the game. |
| [src/zlib](llama-summary/src-zlib.md) | 65 | 16,523 | The Allegiance source code folder `src/zlib` contains essential utilities and components for various aspects of the game's functionality, including networking, logging, memory management, and event handling. |

## Files not read (binary, art, or project files)

.vcxproj: 111, .vcproj: 38, .mdl: 25, .png: 24, .ico: 21, .rc: 14, .lib: 14, .bin: 14, .filters: 13, .rc2: 11, .doc: 9, .sln: 8, .csproj: 7, .config: 6, (none): 5, .htm: 5, .json: 4, .dll: 4, .user: 3, .ffe: 3, .igc: 3, .bmp: 2, .msg: 2, .hxx: 2, .so: 2, .xaml: 2, .aps: 2, .ts: 2, .xls: 2, .props: 1, .pdf: 1, .rc3: 1, .a: 1, .dylib: 1, .ini: 1, .linq: 1, .resx: 1, .settings: 1, .reg: 1, .njsproj: 1, .map: 1, .tlb: 1, .lpk: 1, .cab: 1, .hold: 1, .html: 1, .pl: 1, .cf: 1, .pi: 1, .pr: 1, .ps: 1, .wk: 1, .asm: 1
