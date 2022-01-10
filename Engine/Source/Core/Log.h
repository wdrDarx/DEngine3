#pragma once
#include "spdlog/spdlog.h"
#include "Core/Core.h"
#include "Misc/Singleton.h"

#define LOG_FILE 1
#define STRING(x) std::to_string(x)

class Log 
{
	public:

		Log();

		void LogTemp(const std::string& text);
		void LogWarn(const std::string& text);
		void LogError(const std::string& text);
		void LogInfo(const std::string& text);

		std::shared_ptr<spdlog::logger> m_CoreLogger;
		std::shared_ptr<spdlog::logger> m_FileLogger;
		std::shared_ptr<spdlog::logger> m_AppLogger;

		int m_LogAmount;
};

DEFINE_SINGLETON(Log, Get_Log);

#define LOG_TEMP(...) GET_SINGLETON(Log).LogTemp(__VA_ARGS__)
#define LOG_WARN(...) GET_SINGLETON(Log).LogWarn(__VA_ARGS__)
#define LOG_ERROR(...) GET_SINGLETON(Log).LogError(__VA_ARGS__)
#define LOG_INFO(...) GET_SINGLETON(Log).LogInfo(__VA_ARGS__)
