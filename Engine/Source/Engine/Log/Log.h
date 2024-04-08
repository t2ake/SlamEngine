#pragma once

#include <spdlog/spdlog.h>

namespace sl
{

class Log final
{
public:
	static void Init();

	static std::shared_ptr<spdlog::logger> GetEngineLogger() { return s_engineLogger; }
	static std::shared_ptr<spdlog::logger> GetEditorLogger() { return s_editorLogger; }

private:
	static std::shared_ptr<spdlog::logger> s_engineLogger;
	static std::shared_ptr<spdlog::logger> s_editorLogger;
};

}

#ifndef SL_FINAL
	// Engine log macros.
	#define SL_ENGINE_TRACE(...) ::sl::Log::GetEngineLogger()->trace(__VA_ARGS__)
	#define SL_ENGINE_DEBUG(...) ::sl::Log::GetEngineLogger()->debug(__VA_ARGS__)
	#define SL_ENGINE_INFO(...)  ::sl::Log::GetEngineLogger()->info(__VA_ARGS__)
	#define SL_ENGINE_WARN(...)  ::sl::Log::GetEngineLogger()->warn(__VA_ARGS__)
	#define SL_ENGINE_ERROR(...) ::sl::Log::GetEngineLogger()->error(__VA_ARGS__)
	#define SL_ENGINE_FATAL(...) ::sl::Log::GetEngineLogger()->critical(__VA_ARGS__)
	
	// Editor log macros.
	#define SL_EDITOR_TRACE(...) ::sl::Log::GetEditorLogger()->trace(__VA_ARGS__)
	#define SL_EDITOR_DEBUG(...) ::sl::Log::GetEngineLogger()->debug(__VA_ARGS__)
	#define SL_EDITOR_INFO(...)  ::sl::Log::GetEditorLogger()->info(__VA_ARGS__)
	#define SL_EDITOR_WARN(...)  ::sl::Log::GetEditorLogger()->warn(__VA_ARGS__)
	#define SL_EDITOR_ERROR(...) ::sl::Log::GetEditorLogger()->error(__VA_ARGS__)
	#define SL_EDITOR_FATAL(...) ::sl::Log::GetEditorLogger()->critical(__VA_ARGS__)
#else
	// Engine log macros.
	#define SL_ENGINE_TRACE(...)
	#define SL_ENGINE_DEBUG(...)
	#define SL_ENGINE_INFO(...)
	#define SL_ENGINE_WARN(...)
	#define SL_ENGINE_ERROR(...)
	#define SL_ENGINE_FATAL(...)
	
	// Editor log macros.
	#define SL_EDITOR_TRACE(...)
	#define SL_EDITOR_DEBUG(...)
	#define SL_EDITOR_INFO(...)
	#define SL_EDITOR_WARN(...)
	#define SL_EDITOR_ERROR(...)
	#define SL_EDITOR_FATAL(...)
#endif
