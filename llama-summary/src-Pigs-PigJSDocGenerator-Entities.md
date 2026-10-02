# src/Pigs/PigJSDocGenerator/Entities

[Back to the index](../LLAMA_SUMMARY.md) - 4 files, 63 lines. Notes by a local model, to orient by: not verified facts.

This folder contains classes related to generating JSDoc documentation for entities in Allegiance. It appears to be part of a toolset used to document game objects and their properties.

- EnumValue.cs: Defines the structure for representing enum values.
- Interface.cs: Specifies the interface for entities that can have properties and methods documented.
- Parameter.cs: Describes parameters that can be associated with methods or functions.
- Signature.cs: Holds the signature of a method or function, including its name, return type, and parameters.

## `src/Pigs/PigJSDocGenerator/Entities/EnumValue.cs`

PURPOSE: tiny file (13 lines), shown in full rather than summarized.

    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Text;
    using System.Threading.Tasks;
    namespace PigJSDocGenerator.Entities
    {
        public class EnumValue
        {
            public string Name;
            public string Value;
        }
    }

## `src/Pigs/PigJSDocGenerator/Entities/Interface.cs`

PURPOSE: tiny file (13 lines), shown in full rather than summarized.

    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Text;
    using System.Threading.Tasks;
    namespace PigJSDocGenerator.Entities
    {
        public class Interface
        {
            public string Name;
            public string HelpString;
        }
    }

## `src/Pigs/PigJSDocGenerator/Entities/Parameter.cs`

PURPOSE: tiny file (15 lines), shown in full rather than summarized.

    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Text;
    using System.Threading.Tasks;
    namespace PigJSDocGenerator.Entities
    {
        public class Parameter
        {
            public string Name;
            public string Type;
            public bool IsOutput = false;
            public bool IsReturnValue = false;
        }
    }

## `src/Pigs/PigJSDocGenerator/Entities/Signature.cs`

PURPOSE: tiny file (18 lines), shown in full rather than summarized.

    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Text;
    using System.Threading.Tasks;
    namespace PigJSDocGenerator.Entities
    {
        public class Signature
        {
            public bool GetProperty = false;
            public bool SetProperty = false;
            public bool IsFunction = false;
            public string Name;
            public List<Parameter> Paramaters = new List<Parameter>();
            public string ReturnType;
            public string HelpString;
        }
    }
