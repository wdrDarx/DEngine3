#include "GameFramework.h"

void GameFramework::OnLoad()
{
	//assign App
	Module::AssignApplication(GetApplication());

	if (!gladLoadGL())
	{
		//std::cout << "Failed to initialize OpenGL context" << std::endl;
	}

	LOG_INFO("Loaded Game Framework, random = " + std::to_string(GET_SINGLETON(Rand).Float()));
	
}

void GameFramework::OnUnload()
{


}

void GameFramework::OnUpdate(float DeltaTime)
{

}

EXTERN GAMEFRAMEWORK MODULEDEF(GameFramework)

