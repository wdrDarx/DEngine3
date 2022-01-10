#pragma once
#include "Core/Core.h"
#include "json/json.h"

class Module;
typedef Module* (*CreateModuleFunc)();
class Application;

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
	void LoadModuleDLL(const std::string& ModuleDLLPath);

	//very carefull with this, does what it says
	void LoadModuleFromName(const std::string& ModuleName, const std::string& SearchPath);

	/*
		Scans directory for Folders matching a .Module file inside them, then it loads that module
	*/
	void LoadAllModules(const std::string& FolderPath);

	bool IsModuleLoaded(const std::string& ModuleName);

	void UnloadModule(const std::string& ModuleName);
	void UnloadAllModules();

	void Update(float DeltaTime);

	const std::vector<Module*>& GetLoadedModules() const
	{
		return m_LoadedModules;
	}

public:
	std::vector<Module*> m_LoadedModules;

	//reference to the host app for modules to take on
	Ref<Application> m_App;
};

