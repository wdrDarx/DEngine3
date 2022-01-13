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

class GAMEFRAMEWORK TestAppObject : public AppObject
{
	public:
		OBJECT_CLASS_DEF(TestAppObject, AppObject)

		PROPS_BEGIN()
			PROPDEF(TestInt, EditAnywhere)
			PROPDEF(TestFloat, EditAnywhere)
			PROPDEF(TestString, EditAnywhere)
			PROPDEF(TestClass, EditAnywhere)
			PROPDEF(testStruct, EditAnywhere)
			PROPDEF(testStruct2, EditAnywhere)
			PROPDEF(SaveMeToFile, EditAnywhere)
			PROPDEF(LoadMeFromFile, EditAnywhere)
		PROPS_END()

		void OnConstruct() override
		{
			Super::OnConstruct();
		};

		ObjectClass TestClass = ClassType(typeid(AppObject));

		int TestInt = 4;
		float TestFloat = 69.f;
		std::string TestString = "Osu sus";

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
