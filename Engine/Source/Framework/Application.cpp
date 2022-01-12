#pragma once
#include "Application.h"
#include "DEngine.h"

#define AUTO_UNREGISTER 1

Application::Application() : m_ModuleManager(ToRef<Application>(this)) //initialize module manager
{
	if(AUTO_UNREGISTER)
		m_ModuleManager.BindOnModuleUnloaded(m_ModuleUnloadedCallback);
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


void Application::CompleteRegisterRequests() const
{
	PROFILE_FUNC("Register Requests")

	RegisterRequestHolder& holder = GET_SINGLETON(RegisterRequestHolder);

	//execute the requests
	for(auto& req : holder.m_RegisterQueue)
	{ 
		req();
	}
}

void Application::CoreUpdate(float DeltaTime)
{
	//NOTE: window start and end frame needs to be called manually because an app can have no window
	Tick tick(GetAppState() == AppState::EDITOR ? TickGroup::EDITOR : TickGroup::GAME, DeltaTime);
	m_LastTick = tick;

	OnUpdate(DeltaTime);

	//do all register requests
	CompleteRegisterRequests();

	//Complete all tasks the virtual thread
	PROFILE_FUNC("Main Thread Tasks")
	GetMainThread().Wait();
}


void Application::Shutdown()
{
	GET_SINGLETON(Engine).Stop();
}
