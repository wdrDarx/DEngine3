#pragma once
#include "EditorSection.h"
#include "DEngine.h"
#include "EditorApp.h"

class SceneObjectsSection : public EditorSection
{
public:
	using EditorSection::EditorSection;

	void Init() override
	{

	}

	void Render() override
	{
		ImGui::Begin("Scene Objects");

		for (auto& obj : GetEditorApp()->GetEditorScene()->GetSceneObjects())
		{
			DrawSceneObjectNode(obj.get());
		}

		ImGui::End();
	}


	void DrawSceneObjectNode(SceneObject* object)
	{
		ImGuiTreeNodeFlags flags = (m_SelectedObject.Get() == object ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth;

		std::string name = object->GetName();

		//prefab color
		ImGui::PushStyleColor(ImGuiCol_Text, object->IsPrefab() ? ImVec4{ 0.6f, 0.7f, 1.0f, 1.0f } : ImVec4{ 1.f, 1.f, 1.0f, 1.0f });
		bool expanded = ImGui::TreeNodeEx((void*)object->GetID().ID, flags, name.c_str());
		ImGui::PopStyleColor();
		if (ImGui::IsItemClicked())
		{
			m_SelectedObject.Get() = object;
			m_SelectedComponent.Get() = nullptr;
		}

		bool ObjectDeleted = false;
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Destroy Object"))
				ObjectDeleted = true;

// 			if (object->IsPrefab())
// 			{
// 				if (ImGui::MenuItem("Reload Prefab"))
// 				{
// 					auto prefabAsset = GetEditorApp()->GetAssetManager().LoadAsset(object->GetPrefabAssetRefMutable());
// 					if (prefabAsset)
// 						prefabAsset->LoadPrefab(object, false, true);
// 				}
// 			}

			ImGui::EndPopup();
		}

		if (expanded)
		{
			if (auto& root = object->GetRootComponent())
				DrawComponentNode(root.get());

			for (auto& comp : object->GetComponents())
			{
				//draw all unattached transform components or just normal components
				if (!Cast<TransformComponent>(comp))
				{
					DrawComponentNode(comp.get());
				}
				else
					if (!Cast<TransformComponent>(comp)->GetParent() && !Cast<TransformComponent>(comp)->IsRootComponent())
						DrawComponentNode(comp.get());
			}
			ImGui::TreePop();
		}

		if (ObjectDeleted)
		{
			if (object == m_SelectedObject.Get())
			{
				m_SelectedObject.Get() = nullptr;
				m_SelectedComponent.Get() = nullptr;
			}
			m_ObjectToDelete.Get() = object;

		}
	}

	void DrawComponentNode(ObjectComponent* comp)
	{
		ImGuiTreeNodeFlags flags = (m_SelectedComponent.Get() == comp ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
		std::string name = comp->GetName();

		bool expanded = ImGui::TreeNodeEx((void*)comp->GetID().ID, flags, name.c_str());

		if (ImGui::IsItemClicked())
		{
			m_SelectedComponent.Get() = comp;
			m_SelectedObject.Get() = nullptr;
		}

		bool CompDeleted = false;
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Destroy Component"))
				CompDeleted = true;

			ImGui::EndPopup();
		}

		if (expanded)
		{
			TransformComponent* tcomp = Cast<TransformComponent>(comp);
			if (tcomp)
			{
				for (auto& attached : tcomp->GetChildren())
				{
					DrawComponentNode(attached.get());
				}
			}
			ImGui::TreePop();
		}

		if (CompDeleted)
		{
			if (comp == m_SelectedComponent.Get())
			{
				m_SelectedObject.Get() = nullptr;
				m_SelectedComponent.Get() = nullptr;
			}
			m_ComponentToDelete.Get() = comp;
		}
	}

	SafeObjectPtr<ObjectComponent> m_ComponentToDelete = nullptr;
	SafeObjectPtr<SceneObject> m_ObjectToDelete = nullptr;
	SafeObjectPtr<SceneObject> m_SelectedObject = nullptr;
	SafeObjectPtr<ObjectComponent> m_SelectedComponent = nullptr;
};

