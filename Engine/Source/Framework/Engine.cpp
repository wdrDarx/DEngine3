#include "Engine.h"
#include "DEngine.h"

void Engine::SetApplication(Ref<Application> app)
{
	m_Application = app;
}

void Engine::Start()
{
	//main loop
	while (true)
	{
		if (m_MarkShutdown)
		{		
			m_Application = nullptr;
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
	m_Application->CoreUpdate(m_DeltaTime);
}
