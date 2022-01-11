#pragma once
#include "Core/Core.h"
#include "ModuleManager.h"


#define EXTERN extern "C"
#define MODULEDEF(name) Module* CreateModule() { return new name(#name); };




class Application;
class Module
{
public:
	Module(const std::string& name) : m_Name(name)
	{

	}

	~Module()
	{
		 FreeLibrary(m_Instance);
	}

	virtual void OnLoad()
	{

	}

	virtual void OnUpdate(float DeltaTime)
	{
		
	}

	virtual void OnUnload()
	{

	}

	Ref<Application> GetApplication()
	{
		return m_App;
	}

	static Ref<Application> GetStaticApplication()
	{
		return s_App;
	}

	static void AssignApplication(Ref<Application> app)
	{
		s_App = app;
	}

	template<typename T>
	T* GetModule()
	{
		for (auto& mod : m_ModuleManager->m_LoadedModules)
		{
			T* FoundModule = dynamic_cast<T*>(mod);
			if (FoundModule)
			{
				return FoundModule;
			}
		}

		std::cout << "Couldn't get module!" << std::endl;
		return nullptr;
	}

	const std::string GetThisModuleName() const
	{
		return m_Name;
	}

	const ModuleMetadata& GetMetadata() const
	{
		return m_MetaData;
	}

	void SetMetadata(const ModuleMetadata& metadata)
	{
		m_MetaData = metadata;
	}

	const HINSTANCE& GetInstance() const
	{
		return m_Instance;
	}

	std::string m_Name;
	HINSTANCE m_Instance = nullptr;
	ModuleManager* m_ModuleManager = nullptr;
	Ref<Application> m_App;
	ModuleMetadata m_MetaData;

	//static app ref
	static Ref<Application> s_App;
};
