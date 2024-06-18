#ifndef SL_FINAL

#include "Log.h"

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace sl
{

void Log::Init()
{
	// Output to console.
	auto pConsoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
	pConsoleSink->set_pattern("%^[%T] %n: %v%$");

	// Output to file.
	auto pFileSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("Slam.log", true);
	pFileSink->set_pattern("[%T] [%l] %n: %v");

	std::vector<spdlog::sink_ptr> sinks{ pConsoleSink  , pFileSink };

	s_pEngineLogger = std::make_unique<spdlog::logger>("Engine", sinks.begin(), sinks.end());
	s_pEngineLogger->set_level(spdlog::level::trace);

	s_pEditorLogger = std::make_unique<spdlog::logger>("Editor", sinks.begin(), sinks.end());
	s_pEditorLogger->set_level(spdlog::level::trace);
}

} // namespace sl

#endif
