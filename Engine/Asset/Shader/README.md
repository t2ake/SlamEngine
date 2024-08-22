## File Name
- Rule: `<ShaderProgram>_<Stage>.glsl`
  - ex: `PBR_vert.glsl`
- A shader file is named by its shader program and stage suffix.
  - ex: `AAA_vert.glsl`, `AAA_frag.glsl`, `BBB_comp.glsl`
- A shader containing code that is shared by more than one file becomes a library file with **no suffix** and is placed in the `Lib` folder.
  - ex: `CCC.glsl`
- The name of shader file must be unique.

## Include
- `#include "..."` will start searching files based on the current path, which is `"/SlamEngine/Engine/Asset/Shader"`.
- `#include <...>` will start searching files based on a specific path, which is `"/SlamEngine/Engine/Asset/Shader/Lib"`.

## Version
- The first line of shader must be `#version`.
