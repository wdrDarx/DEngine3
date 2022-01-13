#pragma once
#include "Core/Core.h"
#include "Event/Callback.h"
#include "Framework/ModuleManager.h"
#include "Framework/Engine.h"
#include "Framework/Application.h"
class ObjectBase;

//Object pointer wrapper that auto nullifies the pointer if the module this object belonds to is unloaded
template<class T = ObjectBase>
struct SafeObjectPtr
{
	~SafeObjectPtr()
	{
		m_ModuleUnloadEvent.Destroy();
	}

	SafeObjectPtr()
	{
		Engine& engine = GET_SINGLETON(Engine);
		auto app = engine.GetApplication();
		if (app)
		{
			m_ModuleUnloadEvent.Assign([&](EventModuleUnloaded& event)
			{
				if (!m_Ptr) return;
				if (event.ModuleName == m_Ptr->GetAssociatedModuleName())
					m_Ptr = nullptr;
			});

			app->GetModuleManager().BindOnModuleUnloaded(m_ModuleUnloadEvent);
		}
	}

	SafeObjectPtr(T* ptr) : SafeObjectPtr()
	{
		m_Ptr = ptr;
	}

	T*& Get()
	{
		return m_Ptr;
	}

	T* m_Ptr = nullptr;

	Callback<EventModuleUnloaded> m_ModuleUnloadEvent;
};

