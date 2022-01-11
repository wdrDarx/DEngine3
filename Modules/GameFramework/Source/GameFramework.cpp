#include "GameFramework.h"

void GameFramework::OnLoad()
{
	//assign App
	Module::AssignApplication(GetApplication());

	if (!gladLoadGL())
	{
		//std::cout << "Failed to initialize OpenGL context" << std::endl;
	}

	LOG_WARN("Loaded Module : " + std::string(GetCurrentModuleName()));
}

void GameFramework::TestMethod(const PropArray& args)
{
	
}


void GameFramework::OnUnload()
{
	LOG_WARN("Unloaded Module : " + std::string(GetCurrentModuleName()));

}

void GameFramework::OnUpdate(float DeltaTime)
{

}

EXTERN GAMEFRAMEWORK MODULEDEF(GameFramework)


