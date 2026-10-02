# src/Lib/AllegianceSecurity

[Back to the index](../LLAMA_SUMMARY.md) - 1 files, 110 lines. Notes by a local model, to orient by: not verified facts.

The `src/Lib/AllegianceSecurity` folder in the Allegiance game source code is dedicated to digital rights management (DRM) and file integrity verification. It ensures that game content remains protected and that files are not tampered with.

- AllegianceSecurity.h: Contains definitions for classes related to DRM and file integrity verification, including functionality for checking DRM wrappers and verifying files against expected hashes or signatures.

## `src/Lib/AllegianceSecurity/AllegianceSecurity.h`

PURPOSE:
The file `AllegianceSecurity.h` contains definitions for classes related to digital rights management (DRM) and file integrity verification in the Allegiance game. It includes functionality for checking DRM wrappers, verifying files against hashes, and managing artwork file integrity.

KEY PARTS:
- DrmChecker: Manages the retrieval of a DRM wrapper checksum.
- DrmVerifier: Verifies a given DRM value using an initializing vector.
- FileHash: Represents a file with its associated hash for integrity checks.
- FileHashTable: Provides methods to check if files have valid hashes and to verify their contents.

CONNECTS TO:
Depends on `base.h`, `tvector.h`, and `zstring.h` for basic utilities and data structures.

NOTES:
None.
