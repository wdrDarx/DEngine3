#include "Project.h"

void Project::OnLoad()
{
	//assign App
	Module::AssignApplication(GetApplication());

	if (!gladLoadGL())
	{
		//std::cout << "Failed to initialize OpenGL context" << std::endl;
	}

	LOG_INFO("Loaded Project, random = " + std::to_string(GET_SINGLETON(Rand).Float()));
}

void Project::OnUnload()
{

	
}

void Project::OnUpdate(float DeltaTime)
{
	
}

EXTERN PROJECT MODULEDEF(Project)

