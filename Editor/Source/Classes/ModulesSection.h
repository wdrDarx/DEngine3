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



		auto modules = GetEditorApp()->GetModuleManager().GetAllAvailableModuleNames(Paths::GetModulesDirectory());

		if (ImGui::Button("Load All"))
		{
			GetEditorApp()->GetModuleManager().LoadAllModules(Paths::GetModulesDirectory());
		}
		ImGui::SameLine();
		if (ImGui::Button("Unload All"))
		{
			GetEditorApp()->GetModuleManager().UnloadAllModules();
		}

		ImGui::Columns(2);
		for (auto& mod : modules)
		{
			bool isLoaded = GetEditorApp()->GetModuleManager().IsModuleLoaded(mod);

			if(!isLoaded)
				ImGui::PushStyleColor(ImGuiCol_Text, { 1,1,1, 0.6 });

			ImGui::Text(mod.c_str());

			if (!isLoaded)
				ImGui::PopStyleColor();

			ImGui::NextColumn();
			ImGui::PushID(VectorUtils::IndexOf(mod, modules));
			if(ImGui::Button(isLoaded ? "Unload" : "Load"))
			{ 
				if(isLoaded)
					GetEditorApp()->GetModuleManager().UnloadModule(mod);
				else
					GetEditorApp()->GetModuleManager().LoadModuleFromName(mod, Paths::GetModulesDirectory());
			}
			ImGui::PopID();

			ImGui::NextColumn();
		}
		ImGui::Columns(1);

		ImGui::End();
	}
};

