#pragma once
#include "Core/Core.h"
#include "json/json.h"
#include "Misc/Singleton.h"

#include "Event/Callback.h"
#include "Event/EventDispatcher.h"
#include "Event/Event.h"

class Module;
typedef Module* (*CreateModuleFunc)();
class Application;

struct Dependency
{
	std::string ModuleName;
};

struct ModuleMetadata
{
	std::vector<Dependency> Dependencies;

	void FromJson(const Json::Value& json)
	{
		if (json["Dependencies"].isArray())
		{
			for (uint i = 0; i < json["Dependencies"].size(); i++)
			{
				Dependencies.push_back({ json["Dependencies"][0].asString() });
			}
		}
	}

	Json::Value ToJson()
	{
		Json::Value out;
		for (uint i = 0; i < Dependencies.size(); i++)
		{
			out["Dependencies"][i] = Dependencies[i].ModuleName;
		}

		return out;
	}
};

struct EventModuleLoaded : public Event
{
	std::string ModuleName;
};

struct EventModuleUnloaded : public Event
{
	std::string ModuleName;
};


class DENGINE_API ModuleManager
{
public:
	ModuleManager(Ref<Application> app) : m_App(app)
	{

	}

	/*
		Loads module metadata from a .Module file, then after loading all dependencies actually loads the dll
	*/
	void LoadModule(const std::string& ModuleJsonPath, const std::string& BaseSearchDirectory);
	void LoadModuleDLL(const std::string& ModuleDLLPath, const ModuleMetadata& metadata = ModuleMetadata());

	//very carefull with this, does what it says
	void LoadModuleFromName(const std::string& ModuleName, const std::string& SearchPath);

	/*
		Scans directory for Folders matching a .Module file inside them, then it loads that module
	*/
	void LoadAllModules(const std::string& FolderPath);

	//scans for modules ready for loading (or already loaded) and returns the names
	std::vector<std::string> GetAllAvailableModuleNames(const std::string& FolderPath) const;


	bool IsModuleLoaded(const std::string& ModuleName);

	void UnloadModule(const std::string& ModuleName);
	void UnloadAllModules();

	void Update(float DeltaTime);

	const std::vector<Module*>& GetLoadedModules() const
	{
		return m_LoadedModules;
	}

	void BindOnModuleLoaded(Callback<EventModuleLoaded>& callback)
	{
		m_EventDispatcher.Bind(callback);
	}

	void BindOnModuleUnloaded(Callback<EventModuleUnloaded>& callback)
	{
		m_EventDispatcher.Bind(callback);
	}

public:
	std::vector<Module*> m_LoadedModules;

	//reference to the host app for modules to take on
	Ref<Application> m_App;

private:
	// for module events
	EventDispatcher m_EventDispatcher;
};

