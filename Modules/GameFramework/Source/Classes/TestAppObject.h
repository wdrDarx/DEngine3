#pragma once
#include "GameFramework.h"

class GAMEFRAMEWORK TestAppObject : public AppObject
{
	public:
		OBJECT_CLASS_DEF(TestAppObject, AppObject)

		OBJECT_PROPS_BEGIN()
			PROPDEF(TestInt, EditAnywhere)
			PROPDEF(TestFloat, EditAnywhere)
			PROPDEF(TestString, EditAnywhere)
			PROPDEF(PrintMe, EditAnywhere)
			PROPDEF(SaveMeToFile, EditAnywhere)
			PROPDEF(LoadMeFromFile, EditAnywhere)
		OBJECT_PROPS_END()

		void OnConstruct() override
		{
			Super::OnConstruct();
		}


		int TestInt = 4;
		float TestFloat = 69.f;
		std::string TestString = "Osu sus";

		std::function<void()> PrintMe = [&]()
		{
			LOG_INFO(TestString);
		};

		std::function<void()> SaveMeToFile = [&]()
		{
			Buffer buf;
			Serialize(buf);
			std::string path = FileDialog::SaveFile("");
			File::WriteFile(path, buf);
		};


		std::function<void()> LoadMeFromFile = [&]()
		{
			std::string path = FileDialog::OpenFile("");
			Buffer buf;
			File::ReadFile(path, buf);
			Deserialize(buf);
		};
};
