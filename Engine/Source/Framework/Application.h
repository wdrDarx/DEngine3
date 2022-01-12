#pragma once
#include "Core/Core.h"

//Framework
#include "Framework/Tick.h"
#include "Framework/AppObject.h"
#include "Framework/ModuleManager.h"
#include "Framework/Registry.h"

//Utils
#include "Utils/Thread.h"

//Events
#include "Event/Callback.h"

enum class AppState
{
	GAME = 1,
	EDITOR = 2
};

enum class AppType
{
	DEVELOPMENT = 1,
	SHIPPING = 2
};

/*
 Application is a singleton contains windows and houses application objects
*/
class Engine;
class DENGINE_API Application
{
public:
	friend class Engine;

	Application();

	virtual void OnUpdate(float DeltaSeconds);

	const AppState& GetAppState() const
	{		
		return m_AppState;
	}

	//do any register requests
	void CompleteRegisterRequests() const;

	void SetAppState(const AppState& state)
	{
		m_AppState = state;
	}

	/*
		Creates an appobject with an assigned pointer to this app and a random id, then adds it to app object list here
		WARNING : does not work when called from a module, the created app object must also be associated with a module before being created
	*/
	template<class T>
	Ref<T> CreateAppObject(ObjectInitializer& initializer = ObjectInitializer())
	{
		bool valid = std::is_base_of<AppObject, T>::value;
		ASSERT(valid);

		Ref<T> ptr = MakeRef<T>(this);
	
		Ref<AppObject> obj = Cast<AppObject>(ptr);
		m_AppObjects.push_back(obj);

		//Must call this
		initializer.Flags |= ConstructFlags::RANDOMID;
		ptr->Initialize(initializer);
		
		return ptr;
	}

	template<class T>
	std::vector<Ref<T>> FindObjectsByClass()
	{
		bool valid = std::is_base_of<AppObject, T>::value;
		ASSERT(valid);

		std::vector<Ref<T>> out;
		for (auto& obj : m_AppObjects)
		{
			if(Ref<T> outObj = Cast<T>(obj))
				out.push_back(outObj);
		}

		return out;
	}

	//removed object from appobject array
	void DestroyAppObject(Ref<AppObject> obj)
	{
		//call on destroy
		obj->OnDestroy();
		m_AppObjects.erase(std::find(m_AppObjects.begin(), m_AppObjects.end(), obj));
	}

	void DestroyAppObject(AppObject* obj)
	{
		auto remove = m_AppObjects.end();
		for (auto it = m_AppObjects.begin(); it != m_AppObjects.end(); it++)
		{
			if (obj == (*it).get())
			{
				remove = it;
				break;
			}
		}

		if (remove != m_AppObjects.end())
		{ 
			//call on destroy
			(*remove)->OnDestroy();
			m_AppObjects.erase(remove);
		}
	}

	//Will call initialize to an already existing object 
	Ref<AppObject> AddAppObject(AppObject* obj)
	{
		ASSERT(obj);

		//assign application and add to object array
		obj->m_Application = (this);
		Ref<AppObject> objref = ToRef<AppObject>(obj);
		m_AppObjects.push_back(objref);

		//Must call
		obj->Initialize(ObjectInitializer(ConstructFlags::RANDOMID));	
		return objref;
	}

	template<class T>
	Ref<T> GetAppObject()
	{
		for (auto& obj : GetAppObjects())
		{
			if(Ref<T> out = Cast<T>(obj))
				return out;
		}

		return nullptr;
	}

	const std::vector<Ref<AppObject>> GetAppObjects() const
	{
		return m_AppObjects;
	}

	ModuleManager& GetModuleManager()
	{
		return m_ModuleManager;
	}

	AppType GetAppType() const
	{
		return m_AppType;
	}

	void SetAppType(AppType type)
	{
		m_AppType = type;
	}

	const Tick& GetLastTick()
	{
		return m_LastTick;
	}

	VirtualThread& GetMainThread()
	{
		return m_MainThread;
	}


protected:
	void CoreUpdate(float DeltaTime);
	void Shutdown();

	bool m_IsShuttingDown = false;
	AppState m_AppState;
	AppType m_AppType;
	std::vector<Ref<AppObject>> m_AppObjects;
	ModuleManager m_ModuleManager;
	Tick m_LastTick;
	VirtualThread m_MainThread;

private:
		Callback<EventModuleUnloaded> m_ModuleUnloadedCallback = [&](EventModuleUnloaded& event)
		{
			//Auto unregister any keys associated with module
			ObjectRegistry& ObjectReg = GET_SINGLETON(ObjectRegistry);
			StructRegistry& StructReg = GET_SINGLETON(StructRegistry);
			PropertyRegistry& PropReg = GET_SINGLETON(PropertyRegistry);
			AssetRegistry& AssetReg = GET_SINGLETON(AssetRegistry);

			for (auto& reg : ObjectReg.GetRegisteredKeys())
			{
				if (reg.AssignedModuleName == event.ModuleName)
				{
					ObjectReg.Unregister(reg);
				}
			}

			for (auto& reg : StructReg.GetRegisteredKeys())
			{
				if (reg.AssignedModuleName == event.ModuleName)
				{
					StructReg.Unregister(reg);
				}
			}

			for (auto& reg : PropReg.GetRegisteredKeys())
			{
				if (reg.AssignedModuleName == event.ModuleName)
				{
					PropReg.Unregister(reg);
				}
			}

			for (auto& reg : AssetReg.GetRegisteredKeys())
			{
				if (reg.AssignedModuleName == event.ModuleName)
				{
					AssetReg.Unregister(reg);
				}
			}
		};

};

