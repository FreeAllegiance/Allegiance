# src

[Back to the index](../LLAMA_SUMMARY.md) - 4 files, 114 lines. Notes by a local model, to orient by: not verified facts.

This folder contains essential documentation and configuration files for the Allegiance source code. The SolapReadme.txt file provides release notes and license information, while the todo.txt lists tasks and improvements needed. The CMakeLists.txt file is a small build script used in the development process.

- SolapReadme.txt: Contains release notes and license information for the Allegiance source code.
- todo.txt: Lists tasks and improvements that need to be addressed in the Allegiance game engine and its related components.
- license.txt: Tiny file containing license information (13 lines).
- CMakeLists.txt: Small build script used in the development process (9 lines).

## `src/CMakeLists.txt`

PURPOSE: tiny file (9 lines), shown in full rather than summarized.

    ADD_SUBDIRECTORY(zlib)
    ADD_SUBDIRECTORY(soundengine)
    ADD_SUBDIRECTORY(engine)
    ADD_SUBDIRECTORY(effect)
    #ADD_SUBDIRECTORY(_Utility)
    ADD_SUBDIRECTORY(Igc)
    #ADD_SUBDIRECTORY(training)
    #ADD_SUBDIRECTORY(clintlib)
    #ADD_SUBDIRECTORY(WinTrek)

## `src/SolapReadme.txt`

PURPOSE: This file contains release notes and license information for the Allegiance source code.

KEY PARTS:
- License Update (2017): Microsoft Research has changed the license to an MIT-based license.
- Version 1.3 Release (2015): Work in progress version with additional test files.
- Version 1.2 Release (2004): Refreshed release adding test files for bot testing.
- Version 1.1 Release (2004): Added necessary build files under the \test folder.
- Version 1.0 Release (2004): Initial release of Allegiance source code to the community.

CONNECTS TO: None

NOTES: The sources are built to point to an authentication service that no longer exists.

## `src/license.txt`

PURPOSE: tiny file (13 lines), shown in full rather than summarized.

    Copyright Microsoft Corporation
    Permission is hereby granted, free of charge, to any person obtaining a copy of this software
    and associated documentation files (the "Software"), to deal in the Software without restriction,
    including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
    and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
    subject to the following conditions:
    The above copyright notice and this permission notice shall be included in all copies or substantial
    portions of the Software.
    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT
    LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
    IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
    WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
    OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

## `src/todo.txt`

PURPOSE: This file contains a list of tasks and improvements that need to be addressed in the Allegiance game engine and its related components.

KEY PARTS:
- Add support for other types of rendering with buffers and HW TnL.
- Add more pixel formats to UnclippedBlt, for surfaces outside of Begin/EndScene pairs.
- Finish ImageTransfer 16/24bit->32bit (to restore mdlc on the original artwork).
- Fix mdlc conversion issues, including adding 24bpp -> 32bpp support and enhancing CreateCompatibleSurface.
- Change imbal behavior for alliances to improve balance.
- Add game types other than conquest.
- Implement "Nerf" options like visibility and scan toggles.

CONNECTS TO: Engine components, rendering subsystems, image handling, game logic, and project configurations.

NOTES: The file includes a mix of technical tasks, bug fixes, and feature enhancements. Some items are marked as serious issues that may require reverting to the original MSOriginal codebase. There is also a focus on cleaning up project files and dependencies for better build efficiency.
