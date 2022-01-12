#pragma once
#include "Core/Core.h"
#include "Core/Profiling.h"
#include "Core/Log.h"
#include "Utils/Thread.h"
#include "Framework/ClassType.h"

#define REGISTER_OBJECT(ObjectClass, ModuleClass) GET_SINGLETON(ObjectRegistry).Register<ObjectClass>({#ObjectClass, ClassType(typeid(ObjectClass)), #ModuleClass});
#define UNREGISTER_OBJECT(ObjectClass, ModuleClass) GET_SINGLETON(ObjectRegistry).Unregister({#ObjectClass, ClassType(typeid(ObjectClass)), #ModuleClass});

#define REGISTER_STRUCT(StructClass, ModuleClass) GET_SINGLETON(StructRegistry).Register<StructClass>({#StructClass, ClassType(typeid(StructClass)), #ModuleClass});
#define UNREGISTER_STRUCT(StructClass, ModuleClass) GET_SINGLETON(StructRegistry).Unregister({#StructClass, ClassType(typeid(StructClass)), #ModuleClass});

#define REGISTER_PROPERTY(PropertyClass, ModuleClass) GET_SINGLETON(PropertyRegistry).Register<PropertyClass>({#PropertyClass, ClassType(typeid(PropertyClass)), #ModuleClass});
#define UNREGISTER_PROPERTY(PropertyClass, ModuleClass) GET_SINGLETON(PropertyRegistry).Unregister({#PropertyClass, ClassType(typeid(PropertyClass)), #ModuleClass});

//same as register struct
#define REGISTER_ENUM(EnumClass, ModuleClass) GET_SINGLETON(StructRegistry).Register<EnumClass>({#EnumClass, ClassType(typeid(EnumClass)), #ModuleClass});
#define UNREGISTER_ENUM(EnumClass, ModuleClass) GET_SINGLETON(StructRegistry).Unregister({#EnumClass, ClassType(typeid(EnumClass)), #ModuleClass});

#define REGISTER_ASSETCLASS(AssetClass, ModuleClass) GET_SINGLETON(AssetRegistry).Register<AssetClass>({#AssetClass, ClassType(typeid(AssetClass)), #ModuleClass});
#define UNREGISTER_ASSETCLASS(AssetClass, ModuleClass) GET_SINGLETON(AssetRegistry).Unregister({#AssetClass, ClassType(typeid(AssetClass)), #ModuleClass});


struct ObjectRegisterKey
{
	std::string name;
	ClassType Type;
	std::string AssignedModuleName;

	bool operator==(const ObjectRegisterKey& other) const
	{
		return (name == other.name && AssignedModuleName == other.AssignedModuleName && Type == other.Type);
	}
};

//same class cuz of SetAssociatedModuleName for all objects
struct StructRegisterKey
{
	std::string name;
	ClassType Type;
	std::string AssignedModuleName;

	bool operator==(const StructRegisterKey& other) const
	{
		return (name == other.name && AssignedModuleName == other.AssignedModuleName && Type == other.Type);
	}
};

using PropertyRegisterKey = StructRegisterKey;

namespace std 
{
	template <>
	struct hash<ObjectRegisterKey>
	{
		std::size_t operator()(const ObjectRegisterKey& k) const
		{
			size_t out = 0;
			hash_combine(out, k.name, k.AssignedModuleName, k.Type);
			return out;
		}
	};

	template <>
	struct hash<StructRegisterKey>
	{
		std::size_t operator()(const StructRegisterKey& k) const
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

		//set the assigned module name for the object
		if constexpr (std::is_same<Key, ObjectRegisterKey>::value)
			obj->SetAssociatedModuleName(subclass_key.AssignedModuleName);

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

				//set the assigned module name for the object
				if constexpr (std::is_same<Key, ObjectRegisterKey>::value)
					obj->SetAssociatedModuleName((*it).first.AssignedModuleName);

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

//they are the same basically
using AssetRegisterKey = StructRegisterKey;
class Asset;
class ObjectBase;
struct StructBase;
struct Property;

using ObjectRegistry = _RegistryBase<ObjectRegisterKey, ObjectBase>;
using StructRegistry = _RegistryBase<StructRegisterKey, StructBase>;
using AssetRegistry = _RegistryBase<AssetRegisterKey, Asset>;
using PropertyRegistry = _RegistryBase<PropertyRegisterKey, Property>;

DEFINE_SINGLETON(ObjectRegistry, Get_ObjectRegistry);
DEFINE_SINGLETON(StructRegistry, Get_StructRegistry);
DEFINE_SINGLETON(AssetRegistry, Get_AssetRegistry);
DEFINE_SINGLETON(PropertyRegistry, Get_PropertyRegistry);

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
