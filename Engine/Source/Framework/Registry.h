#pragma once
#include "Core/Core.h"
#include "Core/Log.h"

#define REGISTER_OBJECT(RegistryRef, ObjectClass, ModuleClass) RegistryRef.Register<ObjectClass>({OBJECT_STATIC_CLASS(ObjectClass).GetObjectClassType(), #ObjectClass, #ModuleClass});
#define UNREGISTER_OBJECT(RegistryRef, ObjectClass, ModuleClass) RegistryRef.Unregister({OBJECT_STATIC_CLASS(ObjectClass).GetObjectClassType(), #ObjectClass, #ModuleClass});

#define REGISTER_STRUCT(RegistryRef, StructClass, ModuleClass) RegistryRef.Register<StructClass>({#StructClass, #ModuleClass});
#define UNREGISTER_STRUCT(RegistryRef, StructClass, ModuleClass) RegistryRef.Unregister({#StructClass, #ModuleClass});

//same as register struct
#define REGISTER_ENUM(RegistryRef, EnumClass, ModuleClass) RegistryRef.Register<EnumClass>({#EnumClass, #ModuleClass});
#define UNREGISTER_ENUM(RegistryRef, EnumClass, ModuleClass) RegistryRef.Unregister({#EnumClass, #ModuleClass});

#define REGISTER_ASSETCLASS(RegistryRef, AssetClass, ModuleClass) RegistryRef.Register<AssetClass>({#AssetClass, #ModuleClass});
#define UNREGISTER_ASSETCLASS(RegistryRef, AssetClass, ModuleClass) RegistryRef.Unregister({#AssetClass, #ModuleClass});


struct ObjectRegisterKey
{
	ObjectClassType type;
	std::string name;
	std::string AssignedModuleName;

	bool operator==(const ObjectRegisterKey& other) const
	{
		return (type == other.type && name == other.name && AssignedModuleName == other.AssignedModuleName);
	}
};

struct StructRegisterKey
{
	//just the name of the class
	std::string name;
	std::string AssignedModuleName;

	bool operator==(const StructRegisterKey& other) const
	{
		return (name == other.name && AssignedModuleName == other.AssignedModuleName);
	}
};

namespace std 
{
	template <>
	struct hash<ObjectRegisterKey>
	{
		std::size_t operator()(const ObjectRegisterKey& k) const
		{
			using std::size_t;
			using std::hash;
			using std::string;

			return ((hash<int>()((int)k.type)
				^ (hash<string>()(k.name) << 1)) >> 1)			
				^ (hash<string>()(k.AssignedModuleName) << 1);				
		}
	};

	template <>
	struct hash<StructRegisterKey>
	{
		std::size_t operator()(const StructRegisterKey& k) const
		{
			using std::size_t;
			using std::hash;
			using std::string;

			return hash<std::string>()(k.name)
				^ (hash<string>()(k.AssignedModuleName) << 1);
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
			//registered twice
		}
		subclassInstantiators.emplace(key, &createInstance<U>);
	}

	void Unregister(const Key& key)
	{
		auto it = subclassInstantiators.find(key);
		if (it != subclassInstantiators.end())
		{
			subclassInstantiators.erase(it);
		}
		else
		{
			LOG_WARN("Class isnt registered");
		}

	}

public:

	T* Make(const Key& subclass_key, ConstructionArgs... args) const
	{
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

using ObjectRegistry = _RegistryBase<ObjectRegisterKey, ObjectBase>;
using StructRegistry = _RegistryBase<StructRegisterKey, StructBase>;
using AssetRegistry = _RegistryBase<AssetRegisterKey, Asset>;