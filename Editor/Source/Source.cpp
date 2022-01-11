#include "Core/Log.h"
#include "DEngine.h"
#include "Classes/EditorApp.h"

int main()
{
	Engine& engine = GET_SINGLETON(Engine);
	Ref<Application> app = MakeRef<EditorApp>();
	engine.SetApplication(app);
	Cast<EditorApp>(app)->Init();
	engine.Start();

	return 0;
}