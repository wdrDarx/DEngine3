#pragma once
#include "EditorSection.h"
#include "DEngine.h"
#include "EditorApp.h"

class RegistrySection : public EditorSection
{
public:
	using EditorSection::EditorSection;

	void Init() override
	{

	}

	void Render() override
	{
		ImGui::Begin("Registry");

		ObjectRegistry& ObjectReg = GET_SINGLETON(ObjectRegistry);
		StructRegistry& StructReg = GET_SINGLETON(StructRegistry);
		PropertyRegistry& PropReg = GET_SINGLETON(PropertyRegistry);
		EnumRegistry& EnumReg = GET_SINGLETON(EnumRegistry);

		if (ImGui::TreeNode("Objects"))
		{
			for (auto& reg : ObjectReg.GetRegisteredKeys())
			{
				std::string RegName = "(" + reg.AssignedModuleName + ") " + reg.name;
				ImGui::Text(RegName.c_str());
			}

			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Structs"))
		{
			for (auto& reg : StructReg.GetRegisteredKeys())
			{
				std::string RegName = "(" + reg.AssignedModuleName + ") " + reg.name;
				ImGui::Text(RegName.c_str());
			}

			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Properties"))
		{
			for (auto& reg : PropReg.GetRegisteredKeys())
			{
				std::string RegName = "(" + reg.AssignedModuleName + ") " + reg.name;
				ImGui::Text(RegName.c_str());
			}

			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Enums"))
		{
			for (auto& reg : EnumReg.GetRegisteredKeys())
			{
				std::string RegName = "(" + reg.AssignedModuleName + ") " + reg.name;
				ImGui::Text(RegName.c_str());
			}

			ImGui::TreePop();
		}

		ImGui::End();
	}
};

