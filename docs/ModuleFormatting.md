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
    A map of textures
    Formatted as "<name>":"<texture>"

##### assets.textures.<texture>.path
Type
    String
Description
    Path relative to texture directory

##### assets.textures.<texture>.verticalTiles
Type
    Number
Default
    0
Description
    The number of vertical tiles for a tilemap.

##### assets.textures.<texture>.horizontalTiles
Type
    Number
Default
    0
Description
    The number of horizontal tiles for a tilemap.
    
#### assets.shaders
Type
    Object
Description
    A map of shaders
    Formatted as "<name>":"<shader>"

##### assets.shaders.<shader>.path
Type
    String
Description
    Path relative to shader directory
    
##### assets.shaders.<shader>.type
Type
    String
Options 
    Fragment fragment Frag frag Pixel pixel Pix pix
    Vertex vertex Vert vert
    Geometry geometry Geo geo
    Tessellation tessellation Tess tess
    Compute compute Comp comp
Description
    The type of shader

#### assets.scripts
Type
    Object
Description
    A map of scripts
    Formatted as "<name>":"\<script>"

##### assets.scripts.\<script>.path
Type
    String
Description
    Path relative to script directory
    


