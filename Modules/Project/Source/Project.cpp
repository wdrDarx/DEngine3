#include "Project.h"

void Project::OnLoad()
{
	//assign App
	Module::AssignApplication(GetApplication());

	if (!gladLoadGL())
	{
		//std::cout << "Failed to initialize OpenGL context" << std::endl;
	}

	LOG_WARN("Loaded Module : " + std::string(GetCurrentModuleName()));
}

void Project::OnUnload()
{
	LOG_WARN("Unloaded Module : " + std::string(GetCurrentModuleName()));
	
}

void Project::OnUpdate(float DeltaTime)
{
	
}

EXTERN PROJECT MODULEDEF(Project)

