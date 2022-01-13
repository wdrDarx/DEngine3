#pragma once
#include "Application.h"
#include "DEngine.h"

#define AUTO_UNREGISTER 1

Application::Application() : m_ModuleManager(ToRef<Application>(this)) //initialize module manager
{
	if(AUTO_UNREGISTER)
		m_ModuleManager.BindOnModuleUnloaded(m_ModuleUnloadedCallback);

	RegisterBaseClasses();
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


void Application::RegisterBaseClasses() const
{
	//Property classes
	REGISTER_PROPERTY(StringProperty);
	REGISTER_PROPERTY(FloatProperty);
	REGISTER_PROPERTY(IntProperty);
	REGISTER_PROPERTY(BoolProperty);
	REGISTER_PROPERTY(StructProperty);

	//Objects
	REGISTER_OBJECT(ObjectBase);
	REGISTER_OBJECT(AppObject);
}

void Application::CoreUpdate(float DeltaTime)
{
	//NOTE: window start and end frame needs to be called manually because an app can have no window
	Tick tick(GetAppState() == AppState::EDITOR ? TickGroup::EDITOR : TickGroup::GAME, DeltaTime);
	m_LastTick = tick;

	OnUpdate(DeltaTime);

	//Complete all tasks the virtual thread
	PROFILE_FUNC("Main Thread Tasks")
	GetMainThread().Wait();
}


void Application::Shutdown()
{
	GET_SINGLETON(Engine).Stop();
}
