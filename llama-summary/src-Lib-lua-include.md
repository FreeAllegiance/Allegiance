# src/Lib/lua/include

[Back to the index](../LLAMA_SUMMARY.md) - 5 files, 1,605 lines. Notes by a local model, to orient by: not verified facts.

This folder contains essential header files for integrating and utilizing Lua scripting in the Allegiance game. Lua is a powerful, lightweight scripting language used extensively within the game for various functionalities.

- luaconf.h: Configures and defines system-specific parameters, default paths, API marks, and compatibility options for the Lua interpreter.
- lua.h: Provides the core definitions and declarations necessary for interacting with Lua scripts from C/C++ code in Allegiance.
- lauxlib.h: Offers auxiliary functions and macros to facilitate the creation of Lua libraries within the game, enhancing error handling and type checking.
- lualib.h: Defines the interface and function prototypes for Lua standard libraries included in Allegiance, enabling developers to utilize these libraries seamlessly.

## `src/Lib/lua/include/lauxlib.h`

PURPOSE: This file contains auxiliary functions and macros for building Lua libraries in the Allegiance game. It provides utilities for error handling, type checking, string manipulation, and more.

KEY PARTS:
- `luaL_Reg`: Structure to define a library function.
- `luaL_checkversion_`: Checks if the loaded Lua version is compatible with the one expected by the library.
- `luaL_getmetafield`: Retrieves a metatable field from an object.
- `luaL_callmeta`: Calls a metamethod on an object.
- `luaL_tolstring`: Converts a value to a string, ensuring it's not nil.
- `luaL_argerror`: Reports an argument error with a custom message.
- `luaL_newmetatable`: Creates a new metatable and sets its name.

CONNECTS TO: lua.h

NOTES: None.

## `src/Lib/lua/include/lua.h`

PURPOSE: This file contains the header definitions and declarations for the Lua scripting language library used in Allegiance. It defines types, constants, and functions necessary for interacting with Lua scripts from C/C++ code.

KEY PARTS:
- `lua_State`: Represents a Lua state.
- `lua_CFunction`: Type for C functions registered with Lua.
- `lua_pushnumber`, `lua_getglobal`, `lua_pcallk`: Functions for manipulating the stack and calling Lua functions from C.
- `LUA_VERSION`: Defines the version of Lua being used.

CONNECTS TO: Other Allegiance modules that use Lua scripting, such as game logic or user interface scripts.

NOTES: None.

## `src/Lib/lua/include/lua.hpp`

PURPOSE: tiny file (8 lines), shown in full rather than summarized.

    // lua.hpp
    // Lua header files for C++
    // <<extern "C">> not supplied automatically because Lua also compiles as C++
    extern "C" {
    #include "lua.h"
    #include "lualib.h"
    #include "lauxlib.h"
    }

## `src/Lib/lua/include/luaconf.h`

**PURPOSE:** The `luaconf.h` header file in Allegiance is responsible for configuring and defining various aspects of the Lua interpreter used within the game. It sets up system-specific parameters, default paths, API marks, compatibility options, numeric types, and provides macros for safe string formatting and mathematical operations.

**KEY PARTS:**
- **Name:** LUA_32BITS  
  What it does: Defines whether Lua should be compiled with 32-bit integers and floats.
- **Name:** LUA_USE_C89  
  What it does: Controls the use of non-ISO-C89 features to ensure compatibility with older compilers or platforms.
- **Name:** LUA_PATH_DEFAULT, LUA_CPATH_DEFAULT  
  What it does: Defines default paths for Lua libraries and C libraries, respectively.
- **Name:** LUA_API, LUALIB_API, LUAMOD_API  
  What it does: Marks functions as exported or imported in the C code, allowing for proper linkage between different parts of the library.
- **Name:** LUA_COMPAT_5_2, LUA_COMPAT_5_1  
  What it does: Enables compatibility with older Lua versions (5.2 and 5.1) by defining macros that emulate deprecated functions or behaviors.
- **Name:** LUA_NUMBER, LUA_INTEGER  
  What it does: Defines the floating-point and integer types used by Lua, along with their formatting and conversion functions.
- **Name:** l_sprintf  
  What it does: Equivalent to `snprintf` or `sprintf`, depending on whether C99 is supported. This function is used for safe string formatting in Lua.
- **Name:** lua_strx2number  
  What it does: Converts an hexadecimal numeric string to a number, using `strtod` if available in C99.
- **Name:** lua_number2strx  
  What it does: Converts a float to an hexadecimal numeric string, using `sprintf` with format specifiers '%a'/'%A' if available in C99.

**CONNECTS TO:** This file is used by other Lua-related files and the Allegiance game engine to ensure proper configuration and compatibility of the Lua interpreter.

**NOTES:** None.

## `src/Lib/lua/include/lualib.h`

PURPOSE: This file defines the interface and function prototypes for Lua standard libraries that are included in the Allegiance game.

KEY PARTS:
- `luaopen_base`: Initializes the base library.
- `luaopen_coroutine`: Initializes the coroutine library.
- `luaopen_table`: Initializes the table library.
- `luaopen_io`: Initializes the I/O library.
- `luaopen_os`: Initializes the OS library.
- `luaopen_string`: Initializes the string library.
- `luaopen_utf8`: Initializes the UTF-8 library.
- `luaopen_bit32`: Initializes the bit32 library.
- `luaopen_math`: Initializes the math library.
- `luaopen_debug`: Initializes the debug library.
- `luaopen_package`: Initializes the package library.
- `luaL_openlibs`: Opens all previous libraries.

CONNECTS TO: This file is used by the Allegiance game to load and initialize Lua standard libraries when a Lua state is created.

NOTES: None.
