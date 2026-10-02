# (top level)

[Back to the index](../LLAMA_SUMMARY.md) - 4 files, 92 lines. Notes by a local model, to orient by: not verified facts.

The folder contains essential configuration and documentation files for building and managing the Allegiance game.

- README.md: Provides detailed instructions on how to build and install the Allegiance game, including required tools and project structures.
- CMakeLists.txt: Contains the build configuration for the Allegiance game using CMake, a cross-platform build system generator.
- appveyor.yml: Defines the build process for continuous integration using AppVeyor, a cloud-based CI/CD platform.
- pigs.xml: Likely contains XML data related to the game's assets or configurations.

## `CMakeLists.txt`

PURPOSE: tiny file (14 lines), shown in full rather than summarized.

    CMAKE_MINIMUM_REQUIRED(VERSION 3.24)
    CMAKE_POLICY(SET CMP0005 NEW)
    PROJECT(Allegiance)
    OPTION(USE_STEAM "Use steam" OFF)
    MACRO(SET_MODULE MODULE_NAME)
        ADD_DEFINITIONS(-D__MODULE__="${MODULE_NAME}")
        INCLUDE_DIRECTORIES(../Inc)
        IF (USE_STEAM)
            INCLUDE_DIRECTORIES(../Lib/steam)
        ELSE()
            ADD_DEFINITIONS(-DNO_STEAM)
        ENDIF()
    ENDMACRO()
    ADD_SUBDIRECTORY(src)

## `README.md`

PURPOSE: This README.md file provides instructions and details for building and installing the Allegiance game, including information about required tools, build configurations, project structures, and links to additional resources.

KEY PARTS:
- Allegiance (`WinTrek` folder): The main client program.
- Server (`FedSrv` folder): The game server.
- Lobby (`lobby` folder): The game lobby.
- IGC (`Igc` folder): Game logic library.
- zlib: Various low-level libraries used by all projects.
- engine: 3D graphics engine.
- effect: FX engine above `engine`.
- clintlib: Client library.
- utility (nb: folder name is _Utility): Various high-level libraries used by the main projects.
- soundengine: Sound engine.
- training: Single-player training missions.

CONNECTS TO:
- Visual Studio Community Edition
- Microsoft DirectX SDK (Jun 2010)
- FreeAllegiance forums and Discord server

NOTES:
- The build status is checked via Appveyor CI.
- Configuration might need to change from `Debug` to `FZDebug`, `FZRelease`, or `Release`.
- Some projects are marked as obsolete or about to be removed.

## `appveyor.yml`

PURPOSE: tiny file (11 lines), shown in full rather than summarized.

    version: 1.0.{build}
    image: Visual Studio 2017
    configuration:
    - FZDebug
    - FZRetail
    before_build:
      - nuget restore VS2017/Allegiance.sln
    build:
      project: VS2017/Allegiance.sln
      parallel: true
      verbosity: minimal

## `pigs.xml`

PURPOSE: tiny file (1 lines), shown in full rather than summarized.

    <mxfile userAgent="Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/63.0.3239.84 Safari/537.36" version="7.8.6" editor="www.draw.io" type="github"><diagram id="64294a6c-b639-0982-2639-bffb041bd56e" nam
