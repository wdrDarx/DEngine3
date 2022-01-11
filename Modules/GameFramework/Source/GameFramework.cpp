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

	StaticProperty myStaticProp = StaticProperty::Make<int>("SusTEst", 69);
	LOG_INFO(myStaticProp.m_Name + STRING(*(int*)myStaticProp.GetRawValue()));
	LOG_WARN("This Module Name : " + std::string(GetCurrentModuleName()));
}

void GameFramework::TestMethod(const PropArray& args)
{
	LOG_INFO(std::string("Name : ") + std::string(args.Get<std::string>("Name")));
	LOG_INFO(std::string("Age : ") + STRING(args.Get<float>("Age")));
}


void GameFramework::OnUnload()
{


}

void GameFramework::OnUpdate(float DeltaTime)
{

}

EXTERN GAMEFRAMEWORK MODULEDEF(GameFramework)


