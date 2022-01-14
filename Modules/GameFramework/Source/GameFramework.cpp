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

	

	REGISTER_OBJECT(TestAppObject);
	REGISTER_OBJECT(TestAppObject2);
	REGISTER_OBJECT(TestSceneObject);

	REGISTER_STRUCT(TestStruct);
	REGISTER_STRUCT(TestStruct2);

	REGISTER_ENUM(EnumTest);

	bool c = ClassUtils::IsTemplateType(ClassType(typeid(ObjectClass<ObjectBase>)));
	std::string test = ClassUtils::GetTemplateBaseFromType(ClassType(typeid(ObjectClass<ObjectBase>)));
	std::string test2 = ClassUtils::GetTemplateFromType(ClassType(typeid(ObjectClass<ObjectBase>)));

	ObjectRegistry& reg = GET_SINGLETON(ObjectRegistry);
	Ref<ObjectBase> TestChild = MakeRef<TestAppObject2>();
	Ref<ObjectBase> TestParent1 = MakeRef<AppObject>();
	Ref<ObjectBase> TestParent2 = MakeRef<ObjectBase>();
	Ref<ObjectBase> TestParent3 = MakeRef<TestAppObject>();

	Timer timer;
	bool IsAppObject = ClassUtils::IsObjectBaseOf(TestChild, TestParent1);
	bool isObject = ClassUtils::IsObjectBaseOf(TestChild, TestParent2);
	bool isChild = ClassUtils::IsObjectBaseOf(TestChild, TestParent3);
	LOG_INFO(timer.GetSecondsElapsed());


	GetApplication()->CreateAppObject<TestAppObject>();
}

void GameFramework::TestMethod(const ArgList& args)
{
	LOG_WARN("Name {0}", args.Get<std::string>("Name"));
	LOG_WARN("Age {0}", args.Get<int>("Age"));
}

void GameFramework::OnUnload()
{
	LOG_WARN("Unloaded Module : " + std::string(GetCurrentModuleName()));

}

void GameFramework::OnUpdate(float DeltaTime)
{	
	PROFILE_FUNC()

	return;
	Method test;
	test.m_Function = std::bind(&GameFramework::TestMethod, this, std::placeholders::_1);
	test.m_ArgumentTypes = { MakeRef<ArgType<std::string>>("Name"), MakeRef<ArgType<int>>("Age") };

	test.Call(ArgList::Make({ StaticProperty::Make<std::string>("Name", "DababySus"), StaticProperty::Make<int>("Age", 21) }));
}

EXTERN GAMEFRAMEWORK MODULEDEF(GameFramework)


