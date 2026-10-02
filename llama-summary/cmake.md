# cmake

[Back to the index](../LLAMA_SUMMARY.md) - 2 files, 24 lines. Notes by a local model, to orient by: not verified facts.

This folder contains toolchain configuration files for building Allegiance on MinGW (Minimalist GNU for Windows) environments.

- toolchain-mingw32.cmake: Configures the build environment for 32-bit MinGW.
- toolchain-mingw64.cmake: Configures the build environment for 64-bit MinGW.

## `cmake/toolchain-mingw32.cmake`

PURPOSE: tiny file (9 lines), shown in full rather than summarized.

    set(CMAKE_SYSTEM_NAME Windows)
    set(TOOLCHAIN_PREFIX i686-w64-mingw32)
    set(CMAKE_C_COMPILER ${TOOLCHAIN_PREFIX}-gcc)
    set(CMAKE_CXX_COMPILER ${TOOLCHAIN_PREFIX}-g++)
    set(CMAKE_RC_COMPILER ${TOOLCHAIN_PREFIX}-windres)
    set(CMAKE_FIND_ROOT_PATH /usr/${TOOLCHAIN_PREFIX})
    set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
    set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
    set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

## `cmake/toolchain-mingw64.cmake`

PURPOSE: tiny file (9 lines), shown in full rather than summarized.

    set(CMAKE_SYSTEM_NAME Windows)
    set(TOOLCHAIN_PREFIX x86_64-w64-mingw32)
    set(CMAKE_C_COMPILER ${TOOLCHAIN_PREFIX}-gcc)
    set(CMAKE_CXX_COMPILER ${TOOLCHAIN_PREFIX}-g++)
    set(CMAKE_RC_COMPILER ${TOOLCHAIN_PREFIX}-windres)
    set(CMAKE_FIND_ROOT_PATH /usr/${TOOLCHAIN_PREFIX})
    set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
    set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
    set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
