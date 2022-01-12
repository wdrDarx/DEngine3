
#include "Framework/Engine.h"
#include "Framework/Application.h"
#include "Framework/Module.h"
#include "ModuleUtils.h"

std::string GetCurrentModuleName()
{
	Engine& engine = GET_SINGLETON(Engine);
	auto& app = engine.GetApplication();
	if (!app) return "Engine";

	for (auto& mod : app->GetModuleManager().GetLoadedModules())
	{
		if (mod->GetInstance() == GetCurrentModuleHandle())
		{
			return mod->GetThisModuleName();
		}
	}

	return "Engine";
}

bool IsModuleLoaded(const std::string& ModuleName)
{
	if (ModuleName == "Engine") return true;

	Engine& engine = GET_SINGLETON(Engine);
	auto& app = engine.GetApplication();

	if (!app)
		return false;

	return app->GetModuleManager().IsModuleLoaded(ModuleName);
}
