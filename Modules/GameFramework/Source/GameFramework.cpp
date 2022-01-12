#include "GameFramework.h"
#include "../../Editor/Source/Classes/PropertyPannel.h"
#include "../../Editor/Source/Classes/EditorApp.h"

//Classes
#include "Classes/TestAppObject.h"

void GameFramework::OnLoad()
{
	if (!gladLoadGL())
	{
		std::cout << "Failed to initialize OpenGL context" << std::endl;
	}

	LOG_WARN("Loaded Module : " + std::string(GetCurrentModuleName()));

	REGISTER_PROPERTY(FunctionProperty);
	REGISTER_OBJECT(TestAppObject);

	GetApplication()->GetAppObject<PropertyDrawTypes>()->AddDrawMethod<FunctionProperty>([&](ImGuiContext* context, Property* prop)
	{
		ImGui::SetCurrentContext(context);
		if(ImGui::Button("Function"))
			(*((std::function<void()>*)prop->GetValue()))();
	});

	GetApplication()->CreateAppObject<TestAppObject>();
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


