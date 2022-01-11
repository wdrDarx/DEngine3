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

	std::shared_ptr<spdlog::logger> m_CoreLogger;
	std::shared_ptr<spdlog::logger> m_FileLogger;
	std::shared_ptr<spdlog::logger> m_AppLogger;
};

DEFINE_SINGLETON(Log, Get_Log);

#define LOG_TEMP(...) GET_SINGLETON(Log).m_CoreLogger->trace(__VA_ARGS__); \
if (LOG_FILE) \
GET_SINGLETON(Log).m_FileLogger->trace(__VA_ARGS__); 

#define LOG_WARN(...) GET_SINGLETON(Log).m_CoreLogger->warn(__VA_ARGS__); \
if (LOG_FILE) \
GET_SINGLETON(Log).m_FileLogger->warn(__VA_ARGS__); 

#define LOG_ERROR(...) GET_SINGLETON(Log).m_CoreLogger->error(__VA_ARGS__); \
if (LOG_FILE) \
GET_SINGLETON(Log).m_FileLogger->error(__VA_ARGS__); 

#define LOG_INFO(...) GET_SINGLETON(Log).m_CoreLogger->info(__VA_ARGS__); \
if (LOG_FILE) \
GET_SINGLETON(Log).m_FileLogger->info(__VA_ARGS__); \
