# src/Pigs/PigJSDocGenerator

[Back to the index](../LLAMA_SUMMARY.md) - 2 files, 1,075 lines. Notes by a local model, to orient by: not verified facts.

The `src/Pigs/PigJSDocGenerator` folder contains tools for generating JSDoc and TypeScript documentation from IDL files in Allegiance. This helps ensure that developers have up-to-date documentation for the game's interfaces, typedefs, and enums.

- Configuration.cs: Manages reading and parsing configuration settings from an XML section, including output file paths for TypeScript definitions and JavaScript.
- Program.cs: Handles the core logic of generating JSDoc and TypeScript documentation by parsing IDL files and writing the documentation to specified output files.

## `src/Pigs/PigJSDocGenerator/Configuration.cs`

PURPOSE: The file `Configuration.cs` is responsible for reading and parsing configuration settings from an XML section in the application's configuration file. It manages the output files for TypeScript definitions and JavaScript, as well as a list of IDL (Interface Definition Language) files.

KEY PARTS:
- Configuration class: Implements IConfigurationSectionHandler to handle the loading of configuration data.
- Filenames property: Stores a list of filenames specified in the configuration.
- OutputTypescriptDefinitionFile property: Stores the filename for the TypeScript definition output file.
- OutputJavascriptFile property: Stores the filename for the JavaScript output file.
- LoadData method: Parses the XML section to extract configuration values and populate the class properties.
- GetAttributeValue method: Retrieves an attribute value from an XML node, throwing an exception if the attribute is not found.

CONNECTS TO: This file depends on `System.Configuration` for accessing configuration sections and `System.Xml` for parsing XML data.

NOTES: None.

## `src/Pigs/PigJSDocGenerator/Program.cs`

**PURPOSE:** The `Program.cs` file in Allegiance's Pigs project is responsible for generating JSDoc and TypeScript documentation from IDL (Interface Definition Language) files. This includes parsing interface blocks, typedefs, and enums, and writing the corresponding documentation to output files.

**KEY PARTS:**
- **Class: Program**: Main entry point of the application.
- **Function: Main(string[] args)**: Initializes the program and calls the `Run` method.
- **Function: Run()**: Orchestrates reading IDL files, parsing their contents, and writing JSDoc and TypeScript definitions to output files.
- **Class: Signature**: Represents a function signature or property in an interface.
- **Class: Parameter**: Represents a parameter in a function signature.
- **Function: FindInterfaceBlocksAndWriteToFile(StreamWriter streamWriterJS, StreamWriter streamWriterTS, string idlContents)**: Parses interface blocks and writes JSDoc and TypeScript definitions to the respective files.
- **Function: WriteJSDocCasts(StreamWriter streamWriter, List<Interface> interfaces)**: Writes JavaScript functions for casting objects to COM interfaces.
- **Function: WriteTSDocCasts(StreamWriter streamWriter, List<Interface> interfaces)**: Writes TypeScript declarations for casting JavaScript objects to COM interfaces.
- **Function: WriteJSDocInterface(StreamWriter streamWriter, string helpstring, string name, List<Signature> signatures)**: Writes JSDoc comments and function definitions for interfaces in JavaScript.
- **Function: WriteTSDocInterface(StreamWriter streamWriter, string helpstring, string name, string inherits, List<Signature> signatures)**: Writes TypeScript interface declarations with properties and methods.
- **Function: ParseInterfaceBlock(string lines)**: Parses a block of text representing an interface or typedef from the IDL content into a list of `Signature` objects.
- **Function: FixupJavascriptType(string type)**: Converts COM types to JavaScript types.

**CONNECTS TO:** Other files that handle specific parts of the documentation generation process, such as `TSDocGenerator.cs`, `JSDocGenerator.cs`, and others.

**NOTES:** The file contains TODOs for additional features or improvements that were not fully implemented.
