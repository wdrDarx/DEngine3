#include "Engine.h"
#include "DEngine.h"

void Engine::AddApplication(Ref<Application> app)
{
	m_Applications.push_back(app);
}

void Engine::Start()
{
	//main loop
	while (true)
	{
		if (m_MarkShutdown)
		{
			for (auto& app : m_Applications)
			{
				app->Shutdown();
			}			
			m_Applications.clear();
			break;
		}
		else
		{
			Update();
		}
	}
}

void Engine::Stop()
{
	m_MarkShutdown = true;
}

void Engine::Update()
{
	//get delta time
	auto now = std::chrono::high_resolution_clock::now();
	m_DeltaTime = std::chrono::duration<float, std::chrono::seconds::period>(now - m_LastTime).count();
	m_LastTime = now;

	for (auto it = m_Applications.begin(); it != m_Applications.end(); it++)
	{
		(*it)->CoreUpdate(m_DeltaTime);
	}
}
