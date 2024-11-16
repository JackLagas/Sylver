# Modules Formatting


## Module Types

### Asset Pack

Contains assets to be used in other scenes.
To be used it must be inherited by other scenes.
All assets will be contained under the assets namespace (name of asset pack).

### Scene 

A module that will render data to the screen and contains functionality.


## Loose File Structure
    
moduleA file structure
```
moduleA
├── moduleA.json
└── assets
    ├── textures
    │   ├── textureA.png
    │   └── textureB.png 
    ├── shaders
    │   ├── effectA.spv
    │   └── effectB.spv 
    └── scripts
        ├── scriptA.lua
        └── scriptB.lua 
    
```

Each module is in a folder named the same as the module and contain a descriptor file
that describes the contents of the module. 
The Descriptor file format can be any of (ordered by precedence):
- json
- toml

The Descriptor file name (excluding extension) can be any of (ordered by precedence):
- <moduleName>
- index
- meta
- descriptor

## Descriptor File Options

### Global

#### name
Type 
    String
Description
    The name of the module.
    If the name here does not match the parent folder this should give a warning
    but should still attempt to load using the name of the parent folder.

#### format
Type 
    Number
Default
    1
Description
    The format version to use.
    
#### version
Type
    String
Default 
    "1.0.0"
Description
    The version of the module.
    Using semantic versioning formatting.

#### inherits
Type
    String
    List of Strings
Description
    Module(s) whos assets will be inherited by this module


### Assets

#### assets.textures
Type
    Object
Description
    A map of texture paths
    Paths should be relative to the texture directory
    Formatted as "<name>":"<path>"
    
#### assets.shaders
Type
    Object
Description
    A map of shader paths
    Paths should be relative to the shader directory
    Formatted as "<name>":"<path>"
    
#### assets.scripts
Type
    Object
Description
    A map of script paths
    Paths should be relative to the script directory
    Formatted as "<name>":"<path>"



