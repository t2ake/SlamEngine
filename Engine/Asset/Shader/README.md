## File Name
- Shader file is named by its shader program and stage suffix.
  - ex: `AAA_vert.glsl`, `AAA_frag.glsl`, `BBB_comp.glsl`
- A shader containing code shared with other shader files becomes a library file with **no suffix** and is placed in the `Lib` folder.
  - ex: `CCC.glsl`

## Shared
To ensure consistency between the CPU and GPU, properties such as sampler slots and uniform locations are defined in the header file and placed in the `Shared` folder.
These files will be included in both shader and CPP code.

## Include
- `#include "..."` will start searching files based on the current path, which is `"/SlamEngine/Engine/Asset/Shader"`.
- `#include <...>` will start searching files based on a specific path, which is `"/SlamEngine/Engine/Asset/Shader/Lib"`.
- Can't use nested include, probably a shaderc bug, or some encoding issue.

## Version
- The first line of shader must be `#version`.
