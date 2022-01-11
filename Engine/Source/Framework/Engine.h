#pragma once
#include "Core/Core.h"
#include "Misc/Singleton.h"

class Application;
class DENGINE_API Engine
{
public:
	void AddApplication(Ref<Application> app);

	//start main while loop
	void Start();

	//flags for shutdown
	void Stop();

	std::vector<Ref<Application>> GetApplications() const
	{
		return m_Applications;
	}

	float GetCurrentDeltaTime() const
	{
		return m_DeltaTime;
	}

private:
	void Update();
	std::vector<Ref<Application>> m_Applications;
	std::chrono::time_point<std::chrono::steady_clock> m_LastTime = std::chrono::high_resolution_clock::now();
	float m_DeltaTime;
	bool m_MarkShutdown = false;
};

DEFINE_SINGLETON(Engine, Get_Engine);

inline float GetCurrentDeltaTime()
{
	return GET_SINGLETON(Engine).GetCurrentDeltaTime();
}

