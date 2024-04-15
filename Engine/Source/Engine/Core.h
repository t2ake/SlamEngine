#pragma once

#include <functional>

#ifdef SL_WINDOWS

#else
	#error Only support Windows for now.
#endif

#define BIND_EVENT_CALLBACK(fun) std::bind(&fun, this, std::placeholders::_1)
