#include "Core/Log.h"
#include "DEngine.h"

int main()
{
	LOG_INFO("Started App, random = " + std::to_string(GET_SINGLETON(Rand).Float()));

	Ref<Application> app = MakeRef<Application>();
	ModuleManager manager(app);

	manager.LoadAllModules("G:\\DEngine3\\bin\\Debug-windows-x86_64\\Editor\\Modules\\");

	return 0;
}