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
	Info  = 1 << 2,
	Warn  = 1 << 3,
	Error = 1 << 4,
	Fatal = 1 << 5,
};

struct LogInfo
{
	LogLevel m_level;
	std::string m_content;
};

} // namespace sl

#if !defined(SL_FINAL)

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <spdlog/spdlog.h>
#include <spdlog/fmt/bundled/format.h>

#include <memory>

namespace sl
{

class Log final
{
public:
	Log() = delete;
	Log(const Log &) = delete;
	Log &operator=(const Log &) = delete;
	Log(Log &&) = delete;
	Log &operator=(Log &&) = delete;
	~Log() = delete;

	static void Init();
	static spdlog::logger *GetEngineLogger() { return m_pEngineLogger.get(); }
	static std::vector<LogInfo> &GetLogInfos() { return m_logInfos; }

private:
	inline static std::unique_ptr<spdlog::logger> m_pEngineLogger;
	inline static std::vector<LogInfo> m_logInfos;
};

} // namespace sl

template<>
struct fmt::formatter<glm::vec2> : fmt::formatter<std::string>
{
	auto format(const glm::vec2 &vec, format_context &ctx) const -> decltype(ctx.out())
	{
		return fmt::format_to(ctx.out(), "vec2({}, {})", vec.x, vec.y);
	}
};

template<>
struct fmt::formatter<glm::vec3> : fmt::formatter<std::string>
{
	auto format(const glm::vec3 &vec, format_context &ctx) const -> decltype(ctx.out())
	{
		return fmt::format_to(ctx.out(), "vec3({}, {}, {})", vec.x, vec.y, vec.z);
	}
};

template<>
struct fmt::formatter<glm::vec4> : fmt::formatter<std::string>
{
	auto format(const glm::vec4 &vec, format_context &ctx) const -> decltype(ctx.out())
	{
		return fmt::format_to(ctx.out(), "vec4({}, {}, {}, {})", vec.x, vec.y, vec.z, vec.w);
	}
};

template<>
struct fmt::formatter<glm::ivec2> : fmt::formatter<std::string>
{
	auto format(const glm::ivec2 &vec, format_context &ctx) const -> decltype(ctx.out())
	{
		return fmt::format_to(ctx.out(), "ivec2({}, {})", vec.x, vec.y);
	}
};

template<>
struct fmt::formatter<glm::ivec3> : fmt::formatter<std::string>
{
	auto format(const glm::ivec3 &vec, format_context &ctx) const -> decltype(ctx.out())
	{
		return fmt::format_to(ctx.out(), "ivec3({}, {}, {})", vec.x, vec.y, vec.z);
	}
};

template<>
struct fmt::formatter<glm::ivec4> : fmt::formatter<std::string>
{
	auto format(const glm::ivec4 &vec, format_context &ctx) const -> decltype(ctx.out())
	{
		return fmt::format_to(ctx.out(), "ivec4({}, {}, {}, {})", vec.x, vec.y, vec.z, vec.w);
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
