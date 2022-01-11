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
	LOG_INFO(std::string("Name : ") + std::string(args.Get<std::string>("Name")));
	LOG_INFO(std::string("Age : ") + STRING(args.Get<float>("Age")));
}


void GameFramework::OnUnload()
{
	LOG_WARN("Unloaded Module : " + std::string(GetCurrentModuleName()));

}

void GameFramework::OnUpdate(float DeltaTime)
{

}

EXTERN GAMEFRAMEWORK MODULEDEF(GameFramework)


