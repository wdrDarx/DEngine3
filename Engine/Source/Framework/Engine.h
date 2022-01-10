#pragma once
#include "Core/Core.h"

class Application;
class DENGINE_API Engine
{
public:
	void AddApplication(Ref<Application> app);

	//start main while loop
	void Start();

	//flags for shutdown
	void Stop();

private:
	void Update();
	std::vector<Ref<Application>> m_Applications;
	std::chrono::time_point<std::chrono::steady_clock> m_LastTime = std::chrono::high_resolution_clock::now();
	float m_DeltaTime;
	bool m_MarkShutdown = false;
};

