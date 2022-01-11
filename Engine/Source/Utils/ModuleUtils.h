#pragma once
#if _MSC_VER >= 1400

#include "Framework/Engine.h"
#include "Framework/Application.h"
#include "Framework/Module.h"

EXTERN_C IMAGE_DOS_HEADER __ImageBase;

inline HMODULE GetCurrentModuleHandle()
{
	return (HMODULE)&__ImageBase;
}

inline std::string GetCurrentModuleName()
{
	Engine& engine = GET_SINGLETON(Engine);
	for (auto& app : engine.GetApplications())
	{
		for (auto& mod : app->GetModuleManager().GetLoadedModules())
		{
			if (mod->GetInstance() == GetCurrentModuleHandle())
			{
				return mod->GetThisModuleName();
			}
		}
	}

	return "Engine";
}

#endif
