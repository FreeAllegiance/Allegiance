# src/guids

[Back to the index](../LLAMA_SUMMARY.md) - 2 files, 67 lines. Notes by a local model, to orient by: not verified facts.

The `src/guids` folder in Allegiance contains definitions of globally unique identifiers (GUIDs) used across various components of the game. These GUIDs facilitate the identification and management of different session applications and services.

- guids.cpp: Tiny file containing a few lines of code, likely related to the implementation or usage of GUIDs.
- guids.h: Contains detailed definitions of GUIDs essential for identifying and managing different session applications and services within the game.

## `src/guids/guids.cpp`

PURPOSE: tiny file (5 lines), shown in full rather than summarized.

    #define INITGUID
    #define DBINITCONSTANTS // Initialize OLE constants...
    #include <oledb.h>
    #include <sqloledb.h>
    #include "guids.h"

## `src/guids/guids.h`

PURPOSE: This file contains definitions of globally unique identifiers (GUIDs) used by various components within the Allegiance game. These GUIDs are essential for identifying and managing different session applications and services.

KEY PARTS:
- FEDSRV_GUID: Unique identifier for the dplay session application of the server.
- FEDLOBBYSERVERS_GUID: Unique identifier for the dplay session application of the CLUB lobby that game servers connect to.
- MSRG_CLSID_DPLAY: Monolithic dplay implementation used until all fixes are incorporated into standard released dplay.

CONNECTS TO: objbase.h (for GUID definitions)

NOTES: None.
