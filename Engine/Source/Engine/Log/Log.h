#pragma once

#ifndef SL_FINAL

#include "Event/Event.h"

#include <spdlog/spdlog.h>

#include <format>

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

// Specializations of std::formatter for user defined types.
template<>
struct std::formatter<sl::Event> : std::formatter<std::string>
{
	auto format(const sl::Event &event, std::format_context &context) const
	{
		return formatter<string>::format(event.ToString(), context);
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
