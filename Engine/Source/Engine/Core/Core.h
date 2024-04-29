#pragma once

#if defined(SL_WINDOWS)

#else
	#error "Only support Windows for now."
#endif

#if !defined(SL_FINAL)
	#if defined(SL_WINDOWS)
		#define SL_DEBUGBREAK() __debugbreak()
	#elif defined(SL_LINUX)
		#include <signal.h>
		#define SL_DEBUGBREAK() raise(SIGTRAP)
	#else
		#define SL_DEBUGBREAK()
	#endif
#else
	#define SL_DEBUGBREAK()
#endif
