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

		GetEditorApp()->GetAppObject<PropertyDrawTypes>()->AddDrawMethod<DebugTextProperty>([&](ImGuiContext* context, Property* prop)
		{
			ImGui::Text(((DebugText*)prop->GetValue())->Text.c_str());
		});

		GetEditorApp()->GetAppObject<PropertyDrawTypes>()->AddDrawMethod<BoolProperty>([&](ImGuiContext* context, Property* prop)
		{
			ImGui::Checkbox("", (bool*)prop->GetValue());
		});

		GetEditorApp()->GetAppObject<PropertyDrawTypes>()->AddDrawMethod<Vec3dProperty>([&](ImGuiContext* context, Property* prop)
		{
			PropertyPannel::DrawVec3Control(prop, (float*)prop->GetValue());
		});

		GetEditorApp()->GetAppObject<PropertyDrawTypes>()->AddDrawMethod<Vec2dProperty>([&](ImGuiContext* context, Property* prop)
		{
			PropertyPannel::DrawVec2Control(prop, (float*)prop->GetValue());
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
			EnumBase* enumValue = (EnumBase*)prop->GetValue();
			if (!enumValue->IsBitmask())
			{
				std::string SelectedItem = enumValue->ToString(enumValue->IntValue());
				if (ImGui::BeginCombo("", SelectedItem.c_str())) // The second parameter is the label previewed before opening the combo.
				{
					for (auto& e : enumValue->GetEnumMap())
					{
						bool selected = SelectedItem == e.first;
						if (ImGui::Selectable(e.first.c_str(), &selected))
						{
							enumValue->IntValue() = e.second;
							ImGui::SetItemDefaultFocus();
						}
					}
					ImGui::EndCombo();
				}
			}
			else
			{
				//Draw category thing here
				bool expanded = ImGui::TreeNodeEx((void*)(enumValue), ImGuiTreeNodeFlags_Framed, enumValue->GetType().Name.c_str());

				if (expanded)
				{
					for (auto& e : enumValue->GetEnumMap())
					{
						bool checked = (e.second & enumValue->IntValue());
						ImGui::Checkbox(e.first.c_str(), &checked);
						if (checked)
							enumValue->IntValue() |= e.second;
						else
							enumValue->IntValue() &= ~e.second;
					}
					ImGui::TreePop();
				}
			}
		});

		GetEditorApp()->GetAppObject<PropertyDrawTypes>()->AddDrawMethod<FunctionProperty>([&](ImGuiContext* context, Property* prop)
		{
			ImGui::SetCurrentContext(context);
			if (ImGui::Button("Function"))
				(*((Function*)prop->GetValue()))();
		});

		GetEditorApp()->GetAppObject<PropertyDrawTypes>()->AddDrawMethod<ObjectClassProperty>([&](ImGuiContext* context, Property* prop)
		{
			ImGui::SetCurrentContext(context);
			_ObjectClass* value = (_ObjectClass*)prop->GetValue();

			if (ImGui::BeginCombo("", value->m_ClassName.c_str()))
			{
				ObjectRegistry& reg = GET_SINGLETON(ObjectRegistry);
				for (auto& key : reg.GetRegisteredKeys())
				{
					if (!ClassUtils::IsObjectBaseOf(key.name, value->m_BaseClass))
						continue;

					bool selected = value->m_ClassName == key.name;
					if (ImGui::Selectable(key.name.c_str(), selected))
					{
						value->m_ClassName = key.name;
					}
					if (selected)
						ImGui::SetItemDefaultFocus();
				}

				ImGui::EndCombo();
			}
		});

		GetEditorApp()->GetAppObject<PropertyDrawTypes>()->AddDrawMethod<TransformProperty>([&](ImGuiContext* context, Property* prop)
		{
			Transform* ValueToDraw = (Transform*)prop->GetValue();

			ImGui::PushItemWidth(ImGui::CalcItemWidth());
			bool expanded = ImGui::TreeNodeEx((void*)(ValueToDraw), ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_DefaultOpen, "Transform");
			ImGui::PopItemWidth();
			if (expanded)
			{
				ImGui::BeginChild("Transform");
					ImGui::Columns(2);
					ImGui::Text("Position");
					ImGui::SetColumnWidth(0, 60);
					ImGui::NextColumn();
					PropertyPannel::DrawVec3Control("Position", glm::value_ptr(ValueToDraw->pos));
					ImGui::NextColumn();
					ImGui::Text("Rotation");
					ImGui::NextColumn();
					PropertyPannel::DrawVec3Control("Rotation", glm::value_ptr(ValueToDraw->rot));
					ImGui::NextColumn();
					ImGui::Text("Scale");
					ImGui::NextColumn();
					PropertyPannel::DrawVec3Control("Scale", glm::value_ptr(ValueToDraw->scale), 1.0f, 0.05f);
					ImGui::EndColumns();
				ImGui::EndChild();

				ImGui::TreePop();
			}
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

