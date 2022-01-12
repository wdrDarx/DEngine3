#include "GameFramework.h"
#include "../../Editor/Source/Classes/PropertyWindow.h"

void GameFramework::OnLoad()
{
	if (!gladLoadGL())
	{
		//std::cout << "Failed to initialize OpenGL context" << std::endl;
	}

	LOG_WARN("Loaded Module : " + std::string(GetCurrentModuleName()));



	GetApplication()->GetAppObject<PropertyDrawTypes>()->AddDrawMethod<IntProperty>([&]()
	{
		ImGui::Text("sus");	
	});

	GetApplication()->CreateAppObject<AppObject>();
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


