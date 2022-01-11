#pragma once
#include "Application.h"
#include "DEngine.h"

Application::Application() : m_ModuleManager(ToRef<Application>(this)) //initialize module manager
{

}

void Application::OnUpdate(float DeltaTime)
{
	PROFILE_TICK("Application")

	//update all the modules
	GetModuleManager().Update(DeltaTime);

	//update all the app objects
	for (uint i = 0; i < m_AppObjects.size(); i++)
	{
		auto obj = m_AppObjects[i];

		if (obj)
			obj->OnUpdate(Tick(TickGroup::EDITOR, DeltaTime));
	}
}

void Application::CoreUpdate(float DeltaTime)
{
	//NOTE: window start and end frame needs to be called manually because an app can have no window
	Tick tick(GetAppState() == AppState::EDITOR ? TickGroup::EDITOR : TickGroup::GAME, DeltaTime);
	m_LastTick = tick;

	OnUpdate(DeltaTime);

	//Poll the virtual thread
	{

	}
	GetMainThread().Poll();
}


void Application::Shutdown()
{
	
}
