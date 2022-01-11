#pragma once
#include "Application.h"
#include "DEngine.h"

Application::Application() : m_ModuleManager(ToRef<Application>(this)) //initialize module manager
{

}

void Application::OnUpdate(const Tick& tick)
{
	//update all the modules
	GetModuleManager().Update(tick.DeltaTime);

	//update all the app objects
	for (uint i = 0; i < m_AppObjects.size(); i++)
	{
		auto obj = m_AppObjects[i];

		if (obj)
			obj->OnUpdate(tick);
	}
}

void Application::CoreUpdate(float DeltaTime)
{
	//NOTE: window start and end frame needs to be called manually because an app can have no window
	Tick tick(GetAppState() == AppState::EDITOR ? TickGroup::EDITOR : TickGroup::GAME, DeltaTime);
	m_LastTick = tick;
	OnUpdate(tick);
}


void Application::Shutdown()
{
	
}
