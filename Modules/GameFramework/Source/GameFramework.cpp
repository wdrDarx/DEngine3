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
	REGISTER_PROPERTY(ObjectClassProperty);

	REGISTER_OBJECT(TestAppObject);
	REGISTER_OBJECT(TestAppObject2);

	REGISTER_STRUCT(TestStruct);
	REGISTER_STRUCT(TestStruct2);

	GetApplication()->GetAppObject<PropertyDrawTypes>()->AddDrawMethod<FunctionProperty>([&](ImGuiContext* context, Property* prop)
	{
		ImGui::SetCurrentContext(context);
		if(ImGui::Button("Function"))
			(*((Function*)prop->GetValue()))();
	});

	GetApplication()->GetAppObject<PropertyDrawTypes>()->AddDrawMethod<ObjectClassProperty>([&](ImGuiContext* context, Property* prop)
	{
		ImGui::SetCurrentContext(context);
		ObjectClass* value = (ObjectClass*)prop->GetValue();

		if (ImGui::BeginCombo("", value->m_ClassName.c_str()))
		{
			ObjectRegistry& reg = GET_SINGLETON(ObjectRegistry);
			for (auto& key : reg.GetRegisteredKeys())
			{
				if(!ClassUtils::IsObjectBaseOf(key.name, value->m_BaseClass))
					continue;

				bool selected = value->m_ClassName == key.name;
				if (ImGui::Selectable(key.name.c_str(), selected))
				{
					value->m_ClassName = key.name;
				}
				if (selected)
					ImGui::SetItemDefaultFocus();			
			}

			ImGui::EndCombo();
		}
	});

	GetApplication()->CreateAppObject<TestAppObject>();

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


