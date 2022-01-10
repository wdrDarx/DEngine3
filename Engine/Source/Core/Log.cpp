#include "Log.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"

Log::Log()
{
	s_isIinit = true;
	spdlog::set_pattern("%^[%T] %n: %v%$");
	s_CoreLogger = spdlog::stdout_color_mt("Core");
	s_CoreLogger->set_level(spdlog::level::trace);

	s_AppLogger = spdlog::stdout_color_mt("App");
	s_AppLogger->set_level(spdlog::level::trace);

	s_FileLogger = spdlog::basic_logger_mt("FileCore", "LastLog.txt");

	s_CoreLogger->info("Initialized Log");
}

void Log::LogTemp(const std::string& text) const
{
	s_CoreLogger->trace(text);

	if(LOG_FILE)
		s_FileLogger->trace(text);
}

void Log::LogWarn(const std::string& text) const
{
	s_CoreLogger->warn(text);

	if (LOG_FILE)
		s_FileLogger->warn(text);
}

void Log::LogError(const std::string& text) const
{
	s_CoreLogger->error(text);

	if (LOG_FILE)
		s_FileLogger->error(text);
}


void Log::LogInfo(const std::string& text) const
{
	s_CoreLogger->info(text);

	if (LOG_FILE)
		s_FileLogger->info(text);
}

std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
std::shared_ptr<spdlog::logger> Log::s_FileLogger;
std::shared_ptr<spdlog::logger> Log::s_AppLogger;


bool Log::s_isIinit;

ILog& GetSingleton()
{
	static Log l;
	return l;
}
