#pragma once
#include "Core/Core.h"
#include "Core/Profiling.h"
#include "Core/Log.h"
#include "Utils/Thread.h"
#include "Utils/ModuleUtils.h"
#include "Framework/ClassType.h"

//#define REGISTER_ASSETCLASS(AssetClass) GET_SINGLETON(AssetRegistry).Register<AssetClass>({#AssetClass, ClassType(typeid(AssetClass)), GetCurrentModuleName()});
//#define UNREGISTER_ASSETCLASS(AssetClass) GET_SINGLETON(AssetRegistry).Unregister({#AssetClass, ClassType(typeid(AssetClass)), GetCurrentModuleName()});


//same class cuz of SetAssociatedModuleName for all objects
struct ClassRegisterKey
{
	std::string name;
	ClassType Type;
	std::string AssignedModuleName;

	bool operator==(const ClassRegisterKey& other) const
	{
		return (name == other.name && AssignedModuleName == other.AssignedModuleName && Type == other.Type);
	}
};

using PropertyRegisterKey = ClassRegisterKey;

namespace std 
{
	template <>
	struct hash<ClassRegisterKey>
	{
		std::size_t operator()(const ClassRegisterKey& k) const
		{
			size_t out = 0;
			hash_combine(out, k.name, k.AssignedModuleName, k.Type);
			return out;
		}
	};
}

class Application;
template<typename Key, typename T, typename... ConstructionArgs>
class _RegistryBase
{
public:
	using Instantiator = T* (*)(ConstructionArgs...);

protected:
	template<typename U>
	static T* createInstance(ConstructionArgs... args)
	{
		return new U(std::forward<ConstructionArgs>(args)...);
	}
	using Instantiators = std::unordered_map<Key, Instantiator>;
	Instantiators subclassInstantiators;

public:

	std::vector<Key> GetRegisteredKeys() const 
	{
		std::vector<Key> temp;
		for(auto it = subclassInstantiators.begin(); it != subclassInstantiators.end(); it++)
		{
			temp.push_back(it->first);
		}
		return temp;
	}

	template<typename U>
	void Register(const Key& key)
	{
		static_assert(std::is_base_of<T, U>::value, "Cant register this class");
		auto it = subclassInstantiators.find(key);
		if (it != subclassInstantiators.end())
		{
			LOG_WARN("{0} is already registered!", key.name);
		}
		subclassInstantiators.emplace(key, &createInstance<U>);
		LOG_INFO("Registered {0} from {1}", key.name, key.AssignedModuleName);
	}

	void Unregister(const Key& key)
	{
		auto it = subclassInstantiators.find(key);
		if (it != subclassInstantiators.end())
		{
			subclassInstantiators.erase(it);
			LOG_INFO("Unregistered {0} from {1}", key.name, key.AssignedModuleName);
		}
		else
		{
			LOG_WARN("Class isnt registered");
		}
	}

public:

	T* Make(const Key& subclass_key, ConstructionArgs... args) const
	{
		PROFILE_FUNC()

		auto it = subclassInstantiators.find(subclass_key);
		if (it == subclassInstantiators.end())
		{
			return nullptr;
		}
		auto instantiator = it->second;
		T* obj = instantiator(std::forward<ConstructionArgs>(args)...);

		return obj;
	}

	T* MakeObjectFromClassName(const std::string& FriendlyClassName, ConstructionArgs... args) const
	{
		PROFILE_FUNC()

		for (auto it = subclassInstantiators.begin(); it != subclassInstantiators.end(); it++)
		{
			if ((*it).first.name == FriendlyClassName)
			{
				auto instantiator = it->second;
				T* obj = instantiator(std::forward<ConstructionArgs>(args)...);
				return obj;
			}
		}

		//couldnt find a registered class
		return nullptr;
	}

	bool canProduce(const Key& subclass_key) const
	{
		return subclassInstantiators.find(subclass_key) != subclassInstantiators.end();
	}

	//here just in case i change some mad shit
	Ref<Application> m_App;
};

#if 0 //DEPRECATED 
struct RegisterRequestHolder
{
	//array of register functions
	std::vector<std::function<void()>> m_RegisterQueue;
};

DEFINE_SINGLETON(RegisterRequestHolder, Get_RegisterRequestHolder);

template<class T, class Module>
struct AutoRegister
{
	public:
		AutoRegister()
		{
			m_Dummy;
		}	
		
		virtual bool DoRegister() = 0;

		static bool Register()
		{
			ClassType t(typeid(T));
			ClassType m(typeid(Module));
			//push a request to register, these will be done by the application when its ready
			GET_SINGLETON(RegisterRequestHolder).m_RegisterQueue.push_back(std::bind([&](ClassType type, ClassType module)
			{
				if (!IsModuleLoaded(module.Name)) return; // dont register if this module isnt loaded

				if constexpr (std::is_base_of<ObjectBase, T>::value)
				{
					GET_SINGLETON(ObjectRegistry).Register<T>({ type.Name , type, module.Name });
				}

				if constexpr (std::is_base_of<StructBase, T>::value)
				{
					GET_SINGLETON(StructRegistry).Register<T>({ type.Name , type, module.Name });
				}

				if constexpr (std::is_base_of<Property, T>::value)
				{
					GET_SINGLETON(PropertyRegistry).Register<T>({ type.Name , type, module.Name });
				}		

// 				if constexpr (std::is_base_of<Asset, T>::value)
// 				{
// 					GET_SINGLETON(AssetRegistry).Register<T>({ type.Name , module.Name });
// 				}

			}, t, m));

			return true;
		}

		static bool m_Dummy;
};

template<class T, class Module>
bool AutoRegister<T, Module>::m_Dummy = AutoRegister<T, Module>::Register();

#define AUTO_REGISTER() bool DoRegister() override { return true; }
#endif
