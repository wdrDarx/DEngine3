#pragma once
#include "spdlog/spdlog.h"
#include "Core/Core.h"


#define LOG_FILE 1


class ILog
{
public:
	virtual void LogTemp(const std::string& text) const = 0;
	virtual void LogWarn(const std::string& text) const = 0;
	virtual void LogError(const std::string& text) const = 0;
	virtual void LogInfo(const std::string& text) const = 0;
};

extern "C" __declspec(dllexport) ILog & GetSingleton();

class Log : public ILog
{
	public:

		Log();

		void LogTemp(const std::string& text) const override;
		void LogWarn(const std::string& text) const override;
		void LogError(const std::string& text) const override;
		void LogInfo(const std::string& text) const override;

		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_FileLogger;
		static std::shared_ptr<spdlog::logger> s_AppLogger;

		static bool s_isIinit;
};

DEFINE_SINGLETON(Log, Get_Log);

#define LOG_TEMP(...) GET_SINGLETON(Log).LogTemp(__VA_ARGS__)
#define LOG_WARN(...) GET_SINGLETON(Log).LogWarn(__VA_ARGS__)
#define LOG_ERROR(...) GET_SINGLETON(Log).LogError(__VA_ARGS__)
#define LOG_INFO(...) GET_SINGLETON(Log).LogInfo(__VA_ARGS__)
