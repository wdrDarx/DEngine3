#pragma once
#include "EditorSection.h"
#include "DEngine.h"
#include "EditorApp.h"

class ModulesSection : public EditorSection
{
public:
	using EditorSection::EditorSection;

	void Init() override
	{

	}

	void Render() override
	{
		ImGui::Begin("Modules");

		auto modules = GetEditorApp()->GetModuleManager().GetLoadedModules();

		ImGui::Columns(2);
		for (auto& mod : modules)
		{
			ImGui::Text(mod->GetThisModuleName().c_str());
			ImGui::NextColumn();
			if(ImGui::Button("Unload"))
				GetEditorApp()->GetModuleManager().UnloadModule(mod->GetThisModuleName());

			ImGui::NextColumn();
		}
		ImGui::Columns(1);

		ImGui::End();
	}
};

