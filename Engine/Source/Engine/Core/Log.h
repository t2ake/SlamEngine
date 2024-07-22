#pragma once

#include "Core/Core.h"

#include <cstdint>
#include <string>
#include <vector>

namespace sl
{

enum class LogLevel : uint8_t
{
	Trace = 1 << 0,
	Debug = 1 << 1,
	Info = 1 << 2,
	Warn = 1 << 3,
	Error = 1 << 4,
	Critical = 1 << 5,
};

struct LogInfo
{
	LogInfo(LogLevel level, std::string_view text) :
		m_level(level), m_text(text)
	{

	}

	LogLevel m_level;
	std::string m_text;
};

} // namespace sl

#if !defined(SL_FINAL)

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
	static spdlog::logger *GetEngineLogger() { return m_pEngineLogger.get(); }
	static std::vector<LogInfo> &GetLogInfos() { return m_logInfos; }

private:
	inline static std::unique_ptr<spdlog::logger> m_pEngineLogger;
	inline static std::vector<LogInfo> m_logInfos;
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
	static std::vector<LogInfo> &GetLogInfos() { return m_logInfos; }
private:
	inline static std::vector<LogInfo> m_logInfos;
};
}

#endif

#if !defined(SL_FINAL)
	#define SL_LOG_TRACE(...) ::sl::Log::GetEngineLogger()->trace(__VA_ARGS__)
	#define SL_LOG_DEBUG(...) ::sl::Log::GetEngineLogger()->debug(__VA_ARGS__)
	#define SL_LOG_INFO(...)  ::sl::Log::GetEngineLogger()->info(__VA_ARGS__)
	#define SL_LOG_WARN(...)  ::sl::Log::GetEngineLogger()->warn(__VA_ARGS__)
	#define SL_LOG_ERROR(...) ::sl::Log::GetEngineLogger()->error(__VA_ARGS__)
	#define SL_LOG_FATAL(...) ::sl::Log::GetEngineLogger()->critical(__VA_ARGS__)
	
	#define SL_ASSERT(x, ...) { if(!(x)) { __VA_OPT__(SL_LOG_FATAL(__VA_ARGS__);) SL_DEBUGBREAK(); } }
#else
	#define SL_LOG_TRACE(...)
	#define SL_LOG_DEBUG(...)
	#define SL_LOG_INFO(...)
	#define SL_LOG_WARN(...)
	#define SL_LOG_ERROR(...)
	#define SL_LOG_FATAL(...)
	#define SL_ASSERT(x, ...)
#endif
