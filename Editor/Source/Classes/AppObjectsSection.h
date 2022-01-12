#pragma once
#include "EditorSection.h"
#include "DEngine.h"
#include "EditorApp.h"

class AppObjectsSection : public EditorSection
{
public:
	using EditorSection::EditorSection;

	void Init() override
	{

	}

	void Render() override
	{
		ImGui::Begin("App Objects");

		for (auto& obj : GetEditorApp()->GetAppObjects())
		{
			std::string Name = obj->GetName() + " (" + obj->GetObjectInitializer().AssociatedModuleName + ")";
			ImGui::Text(Name.c_str());
		}

		ImGui::End();
	}
};

