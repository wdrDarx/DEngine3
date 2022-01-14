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
			std::string Name = obj->GetName() + " (" + obj->GetAssociatedModuleName() + ")";
			ImGuiTreeNodeFlags flags = (m_SelectedObject.Get() == obj.get() ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
			flags |= ImGuiTreeNodeFlags_SpanAvailWidth;

			bool expanded = ImGui::TreeNodeEx((void*)obj->GetID().ID, flags, Name.c_str());

			if (ImGui::IsItemClicked())
			{
				m_SelectedObject.Get() = obj.get();
			}

			if(expanded)
				ImGui::TreePop();
		}

		ImGui::End();
	}

	SafeObjectPtr<AppObject> m_SelectedObject = nullptr;
};

