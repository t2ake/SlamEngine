#pragma once

// #define TRACY_ENABLE
#include <tracy/Tracy.hpp>

#define SL_PROFILE_FRAME FrameMark

#define SL_PROFILE ZoneScoped
#define SL_PROFILE_COLOR(x) ZoneScopedC(x)
#define SL_PROFILE_NAME(x) ZoneScopedN(x)
