#pragma once
#include "GameFramework.h"

struct GAMEFRAMEWORK TestStruct : public StructBase
{
	public:
		STRUCT_CLASS_DEF(TestStruct, StructBase)
		PROPS_BEGIN()
			PROPDEF(TestFloat, EditAnywhere)
		PROPS_END()

		float TestFloat = 4.f;
};

struct GAMEFRAMEWORK TestStruct2 : public StructBase
{
public:
	STRUCT_CLASS_DEF(TestStruct2, StructBase)
		PROPS_BEGIN()
			PROPDEF(TestFloat2, EditAnywhere)
			PROPDEF(OsuSus, EditAnywhere)
		PROPS_END()

		float TestFloat2 = 21.f;
		TestStruct OsuSus;
};

struct GAMEFRAMEWORK EnumTest : public EnumBase
{
	enum Enum
	{
		Layer_0 = BIT(1),
		Layer_1 = BIT(2),
		Layer_2 = BIT(3),
		Layer_3 = BIT(4),
		Layer_4 = BIT(5),
		Layer_5 = BIT(6)
	};

	BITMASK_DEF_BEGIN(EnumTest, Enum)
		ENUM_DEF(Layer_0);
		ENUM_DEF(Layer_1);
		ENUM_DEF(Layer_2);
		ENUM_DEF(Layer_3);
		ENUM_DEF(Layer_4);
		ENUM_DEF(Layer_5);
	ENUM_DEF_END()
};

class GAMEFRAMEWORK TestAppObject : public AppObject
{
	public:
		OBJECT_CLASS_DEF(TestAppObject, AppObject)

		PROPS_BEGIN()
			PROPDEF(TestInt, EditAnywhere)
			PROPDEF(TestFloat, EditAnywhere)
			PROPDEF(TestString, EditAnywhere)
			PROPDEF(TestClass, EditAnywhere)
		PROPS_CATEGORY(Tests);
			PROPDEF(TestEnum, EditAnywhere)
			PROPDEF(testStruct, EditAnywhere)
			PROPDEF(testStruct2, EditAnywhere)
			PROPDEF(SaveMeToFile, EditAnywhere)
			PROPDEF(LoadMeFromFile, EditAnywhere)
		PROPS_END()

		void OnConstruct() override
		{
			Super::OnConstruct();
		};

		ObjectClass<AppObject> TestClass;

		int TestInt = 4;
		float TestFloat = 69.f;
		std::string TestString = "Osu sus";

		EnumTest TestEnum = EnumTest::Enum::Layer_0;
		TestStruct testStruct;
		TestStruct2 testStruct2;

		Function SaveMeToFile = [&]()
		{
			Buffer buf;
			Serialize(buf);
			std::string path = FileDialog::SaveFile("");
			File::WriteFile(path, buf);
		};


		Function LoadMeFromFile = [&]()
		{
			std::string path = FileDialog::OpenFile("");
			Buffer buf;
			File::ReadFile(path, buf);
			Deserialize(buf);
		};
};

class GAMEFRAMEWORK TestAppObject2 : public AppObject
{
public:
	OBJECT_CLASS_DEF(TestAppObject, AppObject)

	PROPS_BEGIN()
		PROPDEF(ChildString, EditAnywhere);
	PROPS_END()

	void OnConstruct() override
	{
		Super::OnConstruct();
	};

	std::string ChildString = "im not a child";
};

class GAMEFRAMEWORK TestSceneObject : public SceneObject
{
public:
	OBJECT_CLASS_DEF(TestSceneObject, SceneObject)

	PROPS_BEGIN()
		PROPDEF(TestString, EditAnywhere);
	PROPS_END()

	void OnConstruct() override
	{
		Super::OnConstruct();

		Root = CreateComponent<TransformComponent>("Root");
		LOG_WARN("TestSceneObject");
	}

	Ref<TransformComponent> Root;
	std::string TestString = "osu";
};
