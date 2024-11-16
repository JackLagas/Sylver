# Binary Formatting

***This is currently subject to change***
## Notes
- All offsets are relative to the beginning of the file
- An offset of 0 means the item does not exist


## Extensions

The extension is used for when the application searches for modules to load.
Since assets packs are never directly loaded by the engine they can technically
use any extension however this is not recommended.
Generic extensions are always read on startup to determine if they are scenes or
asset packs and over using may cause slightly longer startup

- ".slvb" or ".slvm" is used for a generic module extension. This can be any
module type
- ".slva" is used for modules that only only contain assets/scripts (ie. no 
direct functionality)
- ".slvs" is used for modules that contain scenes

## Header

Begins with characters "SLVB"
Short for Sylver Binary.
This is to identify this binary format. Readers should exit if this is not 
found at the beginning of the file.

Next is the format version as two 1-byte unsigned integers.
The first number is the major version. This changes when breaking changes
are made to the format.
The second is the minor version. This changes when non-breaking changes are
made. (ie. new asset type added.)

This document is for the initial format version 1.


## Module info

### Version

The next 12 bytes should contain the module's version (Not the format version)
this should be 3 4-byte unsigned integers. These should be the major, minor, 
and patch values in that order.

### Inherits

This is a list of paths to inherited modules.

This starts with a 1-byte unsigned integer that represents the number of 
inherited modules. Following is a list with 8-byte offsets for each inherited 
module. This list's length should be equal to the number of modules.

After this should be all the module paths. These should all be relative to 
the module folder(typically the "game" folder).
Each module path should begin with a 2-byte unsigned integer for the path 
length, followed by the path as a string using "/" for path seperators.

## Index

The next 48 bytes contain the offset, as 8-byte unsigned integers, of the 
scripts, shaders, and textures in that order.
24 bytes are reserved for additional asset types to be added.

## Assets

Each asset type should begin with a 4-byte unsigned integer representing the 
number of assets.
Next should be a list of offsets for each asset. Each offset should be an 
8-byte unsigned integer.
Following this should be all the asset data for this asset type.

Each asset type's data should begin with some metadata for the asset.

### Scripts

Each script should begin with a 1-byte unsigned integer representing the
length of the asset name. Following this should be the name of the asset
as 1-byte ascii characters. 
Next should be an 8-byte unsigned integer representing the size of
the script.


### Shaders

Each shader should begin with a 1-byte unsigned integer representing the
length of the asset name. Following this should be the name of the asset
as 1-byte ascii characters. 
Next should be an 8-byte unsigned integer representing the size of
the shader.

### Textures

Each texture should begin with a 1-byte unsigned integer representing the
length of the asset name. Following this should be the name of the asset
as 1-byte ascii characters. 
Next should be two 1-byte integers representing image tiling information.
The first is the number of horizontal tiles the second is the number of
vertical tiles (values of 1 or 0 mean no tiling in that direction).
Next should be an 8-byte usigned integer representing the size of the image 
(Only counting image's binary data).
