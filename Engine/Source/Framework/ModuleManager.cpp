#include "ModuleManager.h"
#include "Module.h"
#include "Core/Log.h"
#include "Framework/Application.h"
#include "Utils/FileSystem.h"
#include "Utils/VectorUtils.h"
#include "Core/Profiling.h"


void ModuleManager::LoadModule(const std::string& FullPath, const std::string& BaseSearchDirectory)
{
	//full path points to a .Module file
	if (IsModuleLoaded(File::GetFileNameFromPath(FullPath))) return;

	std::ifstream in;
	in.open(FullPath);
	if(in.is_open())
	{
		Buffer FileBuf;
		File::ReadFile(FullPath, FileBuf);
		std::string stringValue = std::string(FileBuf.begin(), FileBuf.end());
		Json::Reader reader;
		Json::Value value;
		reader.parse(stringValue, value);

		ModuleMetadata metadata;
		metadata.FromJson(value);

		for (auto& dep : metadata.Dependencies)
		{
			if(!IsModuleLoaded(dep.ModuleName))
				LoadModuleFromName(dep.ModuleName, BaseSearchDirectory);
		}

		//finally load the actual module
		std::filesystem::path parent = std::filesystem::path(FullPath).parent_path();
		std::string dllPath = parent.string() + "\\" + File::GetFileNameFromPath(FullPath) + ".dll";
		LoadModuleDLL(dllPath, metadata);
	}
	else
	{
		LOG_ERROR("Failed to open " + FullPath);
	}
}

void ModuleManager::LoadModuleDLL(const std::string& ModuleDLLPath, const ModuleMetadata& metadata)
{
	//add the module dir (modules/myModule) as a dll path for external libs
	auto path = std::filesystem::path(ModuleDLLPath);
	SetDllDirectoryA(path.parent_path().string().c_str());

	//load the dll
	auto TempInstance = LoadLibraryA(ModuleDLLPath.c_str());
	ASSERT(TempInstance);
	auto func = (CreateModuleFunc)GetProcAddress(TempInstance, "CreateModule");
	Module* mod = func(); //get the custom class module instance
	mod->m_Instance = TempInstance; //assign the module its instance

	m_LoadedModules.push_back(mod); //add to module array
	mod->m_ModuleManager = this;
	mod->m_App = m_App;
	mod->SetMetadata(metadata);
	mod->OnLoad();

	EventModuleLoaded event;
	event.ModuleName = mod->GetThisModuleName();
	m_EventDispatcher.Dispatch(event);
}

void ModuleManager::LoadModuleFromName(const std::string& ModuleName, const std::string& SearchPath)
{
	if (IsModuleLoaded(ModuleName)) return;

	for (const auto& file : std::filesystem::directory_iterator(SearchPath))
	{
		std::string debug = file.path().filename().string();
		if (file.is_directory() && file.path().filename().string() == ModuleName)
		{
			for (const auto& file : std::filesystem::directory_iterator(file.path()))
			{
				if (file.is_regular_file() && file.path().extension().string() == ".Module")
				{
					LoadModule(file.path().string(), SearchPath);
					return;
				}
			}
		}
	}

	LOG_ERROR("Module '{0}' Was not found", ModuleName);
	
}

bool ModuleManager::IsModuleLoaded(const std::string& ModuleName)
{
	for (auto it = m_LoadedModules.begin(); it != m_LoadedModules.end(); it++)
	{
		Module* mod = *it;
		if (mod->m_Name == ModuleName)
		{
			return true;
		}
	}

	return false;
}

void ModuleManager::LoadAllModules(const std::string& FolderPath)
{
	std::vector<std::string> ModuleNames;
	for (const auto& file : std::filesystem::directory_iterator(FolderPath))
	{
		if (file.is_directory())
		{
			ModuleNames.push_back(file.path().filename().string());
		}
	}

	for (const auto& file : std::filesystem::recursive_directory_iterator(FolderPath))
	{
		if (file.is_regular_file() && file.path().extension().string() == ".Module" && std::count(ModuleNames.begin(), ModuleNames.end(), File::GetFileNameFromPath(file.path().string())) > 0)
		{
			LoadModule(file.path().string(), FolderPath);
		}
	}
}


std::vector<std::string> ModuleManager::GetAllAvailableModuleNames(const std::string& FolderPath) const
{
	std::vector<std::string> ModuleNames;
	for (const auto& file : std::filesystem::directory_iterator(FolderPath))
	{
		if (file.is_directory())
		{
			ModuleNames.push_back(file.path().filename().string());
		}
	}

	 return ModuleNames;
}

void ModuleManager::UnloadModule(const std::string& ModuleName)
{
	Module* ToUnload = nullptr;
	std::vector<Module*>::iterator  ToUnloadIt;
	for (auto it = m_LoadedModules.begin(); it != m_LoadedModules.end(); it++)
	{
		Module* mod = *it;
		if (mod->m_Name == ModuleName)
		{			
			ToUnload = *it;
			ToUnloadIt = it;
			break;
		}
	}
	if(!ToUnload) return; //no loaded module matches name

	//serach if other modules had this one as a dependency
	std::vector<std::string> ToUnloadChildren;
	for (auto it = m_LoadedModules.begin(); it != m_LoadedModules.end(); it++)
	{
		Module* mod = *it;
		for (auto& dep : mod->GetMetadata().Dependencies)
		{
			if(dep.ModuleName == ToUnload->GetThisModuleName())
				ToUnloadChildren.push_back(mod->GetThisModuleName()); //recurisvly unload this module too
		}
	}

	//unload all children
	for (auto& mod : ToUnloadChildren)
	{
		UnloadModule(mod);
	}

	//finally unload the og module 
	EventModuleUnloaded event;
	event.ModuleName = ToUnload->GetThisModuleName();
	m_EventDispatcher.Dispatch(event);

	ToUnload->OnUnload();
	delete ToUnload;
	m_LoadedModules.erase(ToUnloadIt);

	
}

void ModuleManager::UnloadAllModules()
{
	std::vector<std::string> AllModuleNames;
	for (auto& mod : m_LoadedModules)
	{
		AllModuleNames.push_back(mod->GetThisModuleName());
	}

	for (size_t i = 0; i < AllModuleNames.size(); i++)
	{
		UnloadModule(AllModuleNames[i]);
	}

	m_LoadedModules.clear();
}

void ModuleManager::Update(float DeltaTime)
{
	PROFILE_FUNC()

	for (auto it = m_LoadedModules.begin(); it != m_LoadedModules.end(); it++)
	{
		(*it)->OnUpdate(DeltaTime);
	}
}
