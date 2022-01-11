#include "Log.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"

Log::Log()
{
	spdlog::set_pattern("%^[%T] %n: %v%$");
	m_CoreLogger = spdlog::stdout_color_mt("Core");
	m_CoreLogger->set_level(spdlog::level::trace);

	m_AppLogger = spdlog::stdout_color_mt("App");
	m_AppLogger->set_level(spdlog::level::trace);

	m_FileLogger = spdlog::basic_logger_mt("FileCore", "LastLog.txt");

	m_CoreLogger->info("Initialized Log");
}

