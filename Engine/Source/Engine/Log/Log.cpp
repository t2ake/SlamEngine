#include "Log.h"

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace sl
{

std::shared_ptr<spdlog::logger> Log::s_engineLogger;
std::shared_ptr<spdlog::logger> Log::s_editorLogger;

void Log::Init()
{
	// Output to console.
	auto consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
	consoleSink->set_pattern("%^[%T] %n: %v%$");

	// Output to file.
	auto fileSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("Log/Slam.log", true);
	fileSink->set_pattern("[%T] [%l] %n: %v");

	std::vector<spdlog::sink_ptr> sinks{ consoleSink , fileSink };

	s_engineLogger = std::make_shared<spdlog::logger>("Engine", sinks.begin(), sinks.end());
	spdlog::register_logger(s_engineLogger);
	s_engineLogger->set_level(spdlog::level::trace);
	s_engineLogger->flush_on(spdlog::level::trace);

	s_editorLogger = std::make_shared<spdlog::logger>("Editor", sinks.begin(), sinks.end());
	spdlog::register_logger(s_editorLogger);
	s_editorLogger->set_level(spdlog::level::trace);
	s_editorLogger->flush_on(spdlog::level::trace);
}

}
