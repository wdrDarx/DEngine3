#include "Project.h"

void Project::OnLoad()
{
	//assign App
	Module::AssignApplication(GetApplication());

	if (!gladLoadGL())
	{
		//std::cout << "Failed to initialize OpenGL context" << std::endl;
	}

	LOG_INFO("Loaded Project");
}

void Project::OnUnload()
{

	
}

void Project::OnUpdate(float DeltaTime)
{
	
}

EXTERN PROJECT MODULEDEF(Project)

