#pragma once
#include "EditorSection.h"
#include "DEngine.h"
#include "EditorApp.h"
#include "PropertyPannel.h"

class PropertiesSection : public EditorSection
{
public:
	using EditorSection::EditorSection;
	Ref<PropertyPannel> m_PropertyPannel = nullptr;

	void Init() override
	{
		m_PropertyPannel = MakeRef<PropertyPannel>(GetEditorApp());

		GetEditorApp()->GetAppObject<PropertyDrawTypes>()->AddDrawMethod<IntProperty>([&](ImGuiContext* context, Property* prop)
		{
			ImGui::DragInt("", (int*)prop->GetValue());
		});

		GetEditorApp()->GetAppObject<PropertyDrawTypes>()->AddDrawMethod<FloatProperty>([&](ImGuiContext* context, Property* prop)
		{
			ImGui::DragFloat("", (float*)prop->GetValue());
		});

		GetEditorApp()->GetAppObject<PropertyDrawTypes>()->AddDrawMethod<StringProperty>([&](ImGuiContext* context, Property* prop)
		{
			ImGui::InputText("", (std::string*)prop->GetValue());
		});

		GetEditorApp()->GetAppObject<PropertyDrawTypes>()->AddDrawMethod<BoolProperty>([&](ImGuiContext* context, Property* prop)
		{
			ImGui::Checkbox("", (bool*)prop->GetValue());
		});

	}

	void Render() override
	{
		ImGui::Begin("Properties");
		
		if (m_ObjectToDraw)
		{
			m_PropertyPannel->DrawProperties(m_ObjectToDraw->GetProperties());
		}

		ImGui::End();
	}

	ObjectBase* m_ObjectToDraw = nullptr;
};

