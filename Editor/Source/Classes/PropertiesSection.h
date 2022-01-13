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
		GetEditorApp()->GetModuleManager().BindOnModuleUnloaded(m_ModuleUnloadCallback);

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


		GetEditorApp()->GetAppObject<PropertyDrawTypes>()->AddDrawMethod<StructProperty>([&](ImGuiContext* context, Property* prop)
		{
			StructBase* structToDraw = (StructBase*)prop->GetValue();

			ImGui::PushItemWidth(ImGui::CalcItemWidth());
			bool expanded = ImGui::TreeNodeEx((void*)(structToDraw), ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_AllowItemOverlap, structToDraw->GetClassType().Name.c_str());
			ImGui::PopItemWidth();

			if (expanded)
			{
				for (auto& prop : structToDraw->GetPropertiesMutable())
				{
					ImGui::Text(prop->GetName().c_str());
					ImGui::SameLine();
					m_PropertyPannel->DrawPropRaw(prop.get());
				}
				ImGui::TreePop();
			}
		});

		GetEditorApp()->GetAppObject<PropertyDrawTypes>()->AddDrawMethod<EnumProperty>([&](ImGuiContext* context, Property* prop)
		{
			EnumProperty* EnumToDraw = (EnumProperty*)prop->GetValue();
			ImGui::Text("This is an enum");
		});

	}

	void OnObjectChanged()
	{
		if (auto Object = m_ObjectToDraw.Get())
		{
			if(Object)
			{ 
				ObjectRegistry& reg = GET_SINGLETON(ObjectRegistry);
				ObjectBase* ptr = reg.MakeObjectFromClassName(Object->GetClassType().Name);
				if(ptr)
				{ 
					m_DefaultObject = ToRef<ObjectBase>(ptr);
					m_DefaultObject->DefineProperties();
				}
			}
		}
	}

	void Render() override
	{
		if (m_LastObject.Get() != m_ObjectToDraw.Get())
		{
			m_LastObject = m_ObjectToDraw;
			OnObjectChanged();
		}

		ImGui::Begin("Properties");
		
		if (m_ObjectToDraw.Get())
		{
			m_PropertyPannel->DrawProperties(m_ObjectToDraw.Get()->GetProperties(), m_ObjectToDraw.Get(), m_DefaultObject.get());
		}

		ImGui::End();
	}

	SafeObjectPtr<ObjectBase> m_ObjectToDraw = nullptr;
	SafeObjectPtr<ObjectBase> m_LastObject = nullptr;

	Callback<EventModuleUnloaded> m_ModuleUnloadCallback = [&](EventModuleUnloaded& event)
	{
		if(m_DefaultObject && event.ModuleName == m_DefaultObject->GetAssociatedModuleName())
			m_DefaultObject = nullptr;
	};

	Ref<ObjectBase> m_DefaultObject = nullptr;
};

