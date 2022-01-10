#include "GameFramework.h"

void GameFramework::OnLoad()
{
	//assign App
	Module::AssignApplication(GetApplication());

	if (!gladLoadGL())
	{
		//std::cout << "Failed to initialize OpenGL context" << std::endl;
	}

	LOG_INFO("Loaded Game Framework");
	Property* prop = GET_SINGLETON(PropertyRegistery).Make({"FloatProperty", "Engine"});

}

void GameFramework::OnUnload()
{


}

void GameFramework::OnUpdate(float DeltaTime)
{

}

EXTERN GAMEFRAMEWORK MODULEDEF(GameFramework)

