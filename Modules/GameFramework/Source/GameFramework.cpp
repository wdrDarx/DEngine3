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

	Method method;
	method.m_Function = std::bind(&GameFramework::TestMethod, this, std::placeholders::_1);
	method.m_Arguments = { PropArgDef("Name", typeid(std::string)) };

	std::string NameStr = "Dababy";
	float age = 100;
	PropArray args;
	args.m_Props.push_back(StaticProperty::Make(StringProperty("Name", NameStr)));
	args.m_Props.push_back(StaticProperty::Make(FloatProperty("Age", age)));
	method.Call(args);
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


