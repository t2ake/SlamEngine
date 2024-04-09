#include "Log.h"

#ifndef SL_FINAL
	#include <spdlog/sinks/basic_file_sink.h>
	#include <spdlog/sinks/stdout_color_sinks.h>
#endif

namespace sl
{

#ifndef SL_FINAL

std::shared_ptr<spdlog::logger> Log::s_pEngineLogger;
std::shared_ptr<spdlog::logger> Log::s_pEditorLogger;

void Log::Init()
{
	// Output to console.
	auto pConsoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
	pConsoleSink->set_pattern("%^[%T] %n: %v%$");

	// Output to file.
	auto pFileSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("Slam.log", true);
	pFileSink->set_pattern("[%T] [%l] %n: %v");

	std::vector<spdlog::sink_ptr> sinks{ pConsoleSink  , pFileSink };

	s_pEngineLogger = std::make_shared<spdlog::logger>("Engine", sinks.begin(), sinks.end());
	s_pEngineLogger->set_level(spdlog::level::trace);

	s_pEditorLogger = std::make_shared<spdlog::logger>("Editor", sinks.begin(), sinks.end());
	s_pEditorLogger->set_level(spdlog::level::trace);
}

#else

void Log::Init() {}

#endif

} // namespace sl
