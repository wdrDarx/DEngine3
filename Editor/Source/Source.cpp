#include "Core/Log.h"
#include "DEngine.h"

int main()
{
	Ref<Application> app = MakeRef<Application>();
	ModuleManager manager(app);

	manager.LoadAllModules("G:\\DEngine3\\bin\\Debug-windows-x86_64\\Editor\\Modules\\");

	

	return 0;
}