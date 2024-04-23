#pragma once

#ifndef SL_FINAL

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <spdlog/spdlog.h>

#include <sstream>

namespace sl
{

class Log final
{
public:
	static void Init();

	static std::shared_ptr<spdlog::logger> GetEngineLogger() { return s_pEngineLogger; }
	static std::shared_ptr<spdlog::logger> GetEditorLogger() { return s_pEditorLogger; }

private:
	static std::shared_ptr<spdlog::logger> s_pEngineLogger;
	static std::shared_ptr<spdlog::logger> s_pEditorLogger;

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
	#define SL_ENGINE_TRACE(...) ::sl::Log::GetEngineLogger()->trace(__VA_ARGS__)
	#define SL_ENGINE_DEBUG(...) ::sl::Log::GetEngineLogger()->debug(__VA_ARGS__)
	#define SL_ENGINE_INFO(...)  ::sl::Log::GetEngineLogger()->info(__VA_ARGS__)
	#define SL_ENGINE_WARN(...)  ::sl::Log::GetEngineLogger()->warn(__VA_ARGS__)
	#define SL_ENGINE_ERROR(...) ::sl::Log::GetEngineLogger()->error(__VA_ARGS__)
	#define SL_ENGINE_FATAL(...) ::sl::Log::GetEngineLogger()->critical(__VA_ARGS__)

	// Editor logger macros
	#define SL_EDITOR_TRACE(...) ::sl::Log::GetEditorLogger()->trace(__VA_ARGS__)
	#define SL_EDITOR_DEBUG(...) ::sl::Log::GetEngineLogger()->debug(__VA_ARGS__)
	#define SL_EDITOR_INFO(...)  ::sl::Log::GetEditorLogger()->info(__VA_ARGS__)
	#define SL_EDITOR_WARN(...)  ::sl::Log::GetEditorLogger()->warn(__VA_ARGS__)
	#define SL_EDITOR_ERROR(...) ::sl::Log::GetEditorLogger()->error(__VA_ARGS__)
	#define SL_EDITOR_FATAL(...) ::sl::Log::GetEditorLogger()->critical(__VA_ARGS__)

	// Assertions
	#define SL_ENGINE_ASSERT(x) {if(!(x)){__debugbreak();}}
	#define SL_ENGINE_ASSERT_INFO(x, ...) {if(!(x)){SL_ENGINE_FATAL(__VA_ARGS__); __debugbreak();}}
	#define SL_EDITOR_ASSERT(x) {if(!(x)){__debugbreak();}}
	#define SL_EDITOR_ASSERT_INFO(x, ...) {if(!(x)){SL_ENGINE_FATAL(__VA_ARGS__); __debugbreak();}}
#else
	#define SL_ENGINE_TRACE(...)
	#define SL_ENGINE_DEBUG(...)
	#define SL_ENGINE_INFO(...)
	#define SL_ENGINE_WARN(...)
	#define SL_ENGINE_ERROR(...)
	#define SL_ENGINE_FATAL(...)
	#define SL_EDITOR_TRACE(...)
	#define SL_EDITOR_DEBUG(...)
	#define SL_EDITOR_INFO(...)
	#define SL_EDITOR_WARN(...)
	#define SL_EDITOR_ERROR(...)
	#define SL_EDITOR_FATAL(...)
	#define SL_ENGINE_ASSERT(x)
	#define SL_ENGINE_ASSERT_INFO(x, ...)
	#define SL_EDITOR_ASSERT(x)
	#define SL_EDITOR_ASSERT_INFO(x, ...)
#endif
