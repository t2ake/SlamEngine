#pragma once

#include "Core/Core.h"

#ifndef SL_FINAL

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <spdlog/spdlog.h>

#include <memory>
#include <sstream>

namespace sl
{

class Log final
{
public:
	Log() = delete;

	static void Init();

	static spdlog::logger *GetEngineLogger() { return s_pEngineLogger.get(); }

private:
	inline static std::unique_ptr<spdlog::logger> s_pEngineLogger;
	inline static std::unique_ptr<spdlog::logger> s_pEditorLogger;
};

} // namespace sl

template<>
struct std::formatter<glm::vec2> : std::formatter<std::string>
{
	auto format(const glm::vec2 &vec, std::format_context &context) const
	{
		return formatter<string>::format(std::format("vec2({}, {})", vec.x, vec.y), context);
	}
};

template<>
struct std::formatter<glm::vec3> : std::formatter<std::string>
{
	auto format(const glm::vec3 &vec, std::format_context &context) const
	{
		return formatter<string>::format(std::format("vec3({}, {}, {})", vec.x, vec.y, vec.z), context);
	}
};

template<>
struct std::formatter<glm::vec4> : std::formatter<std::string>
{
	auto format(const glm::vec4 &vec, std::format_context &context) const
	{
		return formatter<string>::format(std::format("vec4({}, {}, {}, {})", vec.x, vec.y, vec.z, vec.w), context);
	}
};

// TODO: Such a dirty implementation, improve it.
template<glm::length_t C, glm::length_t R, typename T, glm::qualifier Q>
struct std::formatter<glm::mat<C, R, T, Q>> : std::formatter<std::string>
{
	auto format(const glm::mat<C, R, T, Q> &mat, std::format_context &context) const
	{
		std::stringstream ss;
		ss << "mat" << C << "x" << R << "(";
		for (glm::length_t i = 0; i < C; ++i)
		{
			for (glm::length_t j = 0; j < R; ++j)
			{
				ss << mat[i][j];
				if (j < R - 1)
				{
					ss << ", ";
				}
			}
			if (i < C - 1)
			{
				// To approach [xx:xx:xx] Logger: matCxR(
				ss << "\n" << "                          ";
			}
		}
		ss << ")";

		return formatter<string>::format(ss.str(), context);
	}
};

#else

namespace sl
{
class Log final
{
public:
	static void Init() {}
};
}

#endif

#ifndef SL_FINAL
	// Engine logger macros
	#define SL_LOG_TRACE(...) ::sl::Log::GetEngineLogger()->trace(__VA_ARGS__)
	#define SL_LOG_DEBUG(...) ::sl::Log::GetEngineLogger()->debug(__VA_ARGS__)
	#define SL_LOG_INFO(...)  ::sl::Log::GetEngineLogger()->info(__VA_ARGS__)
	#define SL_LOG_WARN(...)  ::sl::Log::GetEngineLogger()->warn(__VA_ARGS__)
	#define SL_LOG_ERROR(...) ::sl::Log::GetEngineLogger()->error(__VA_ARGS__)
	#define SL_LOG_FATAL(...) ::sl::Log::GetEngineLogger()->critical(__VA_ARGS__)

	// Assertions
	#define SL_ASSERT(x) { if(!(x)) { SL_DEBUGBREAK(); } }
	#define SL_ASSERT_INFO(x, ...) { if(!(x)) { SL_LOG_FATAL(__VA_ARGS__); SL_DEBUGBREAK(); } }
	#define SL_ASSERT_OPT(x, ...) { if(!(x)) { __VA_OPT__(SL_LOG_FATAL(__VA_ARGS__);) SL_DEBUGBREAK(); } }
#else
	#define SL_LOG_TRACE(...)
	#define SL_LOG_DEBUG(...)
	#define SL_LOG_INFO(...)
	#define SL_LOG_WARN(...)
	#define SL_LOG_ERROR(...)
	#define SL_LOG_FATAL(...)
	#define SL_ASSERT(x)
	#define SL_ASSERT_INFO(x, ...)
#endif
