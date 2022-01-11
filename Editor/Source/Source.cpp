#include "Core/Log.h"
#include "DEngine.h"

int main()
{
	Engine& engine = GET_SINGLETON(Engine);
	Ref<Application> app = MakeRef<Application>();
	engine.AddApplication(app);
	app->GetModuleManager().LoadAllModules("G:\\DEngine3\\bin\\Debug-windows-x86_64\\Editor\\Modules\\");
	LOG_WARN("This Module Name : " + std::string(GetCurrentModuleName()));
	engine.Start();
	

	return 0;
}