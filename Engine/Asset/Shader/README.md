## File Name
- `<Name>_<Suffix>.glsl`
  - ex: `PBR_vert.glsl`, `Core_lib.glsl`
- A shader is named by its shader program name plus the stage suffix.
  - ex: `AAA_vert.glsl`, `AAA_frag.glsl`, `BBB_comp.glsl`
- A shader containing code shared by multiple files becomes a library file with the `_lib` suffix.
  - ex: `CCC_lib.glsl`
- The name of shader file must be unique.

## Include
- `#include "..."` will start searching files based on the current path, which is `"/SlamEngine/Engine/Asset/Shader"`.
- `#include <...>` will start searching files based on a specific path, which is `"/SlamEngine/Engine/Asset/Shader/Lib"`.

## Version
- The first line of shader must be `#version`
