#pragma once

// #define TRACY_ENABLE

// TODO: RHI
#include <glad/glad.h>
#include <tracy/TracyOpenGL.hpp>

#define SL_PROFILE_GPU(x) TracyGpuZone(x)
#define SL_PROFILE_GPU_CONTEXT TracyGpuContext
#define SL_PROFILE_GPU_COLLECT TracyGpuCollect
