# src/Pigs/PigClient/PigClient/Properties

[Back to the index](../LLAMA_SUMMARY.md) - 3 files, 144 lines. Notes by a local model, to orient by: not verified facts.

This folder contains properties and configuration files for the PigClient project in Allegiance. It manages assembly information, localized resources, and user settings.

- AssemblyInfo.cs: Contains metadata and settings for the WpfApp1 project.

## `src/Pigs/PigClient/PigClient/Properties/AssemblyInfo.cs`

PURPOSE: This file contains assembly information and settings for the WpfApp1 project in Allegiance.

KEY PARTS:
- [assembly: AssemblyTitle("WpfApp1")] - Specifies the title of the application.
- [assembly: AssemblyDescription("")] - Provides a description of the application.
- [assembly: ComVisible(false)] - Hides the assembly from COM components.
- [assembly: ThemeInfo(ResourceDictionaryLocation.None, ResourceDictionaryLocation.SourceAssembly)] - Defines theme information for the application.
- [assembly: AssemblyVersion("1.0.0.0")] - Specifies the version number of the assembly.

CONNECTS TO: This file is part of the WpfApp1 project and does not directly connect to other systems or files outside of its own project.

NOTES: None.

## `src/Pigs/PigClient/PigClient/Properties/Resources.Designer.cs`

PURPOSE: This file defines a strongly-typed resource class for accessing localized strings and other resources in the Allegiance game's client application.

KEY PARTS:
- `Resources` class: Contains methods to access localized strings and manage resource culture.
- `ResourceManager`: Manages the retrieval of resources from resource files.
- `Culture`: Property to get or set the current UI culture used for resource lookups.

CONNECTS TO: Other resource files (.resx) that contain localized strings and other resources.

NOTES: None.

## `src/Pigs/PigClient/PigClient/Properties/Settings.Designer.cs`

PURPOSE: This file contains the auto-generated settings class for an application named WpfApp1. It manages user-specific configuration settings.

KEY PARTS:
- Settings: A sealed partial class derived from ApplicationSettingsBase that holds application settings.
- defaultInstance: A static instance of the Settings class, synchronized to ensure thread safety.
- Default: A property that provides access to the singleton instance of the Settings class.

CONNECTS TO: WpfApp1 application

NOTES: None.
