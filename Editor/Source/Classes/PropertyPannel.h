#pragma once
#include "DEngine.h"
#include "EditorApp.h"

class PropertyDrawTypes : public AppObject
{
public:
	OBJECT_CLASS_DEF(PropertyDrawTypes, AppObject)

	void OnConstruct() override
	{
		Super::OnConstruct();

		GetApplication()->GetModuleManager().BindOnModuleUnloaded(m_ModuleUnloadedEvent);
	}

	template<class T>
	void AddDrawMethod(std::function<void(ImGuiContext*, Property*)> DrawFunc)
	{
		FORCE_BASE_CLASS(T, Property)
		m_Functions[ClassType(typeid(T))] = DrawFunc;
	}

	template<class T>
	std::function<void(ImGuiContext*, Property*)> GetDrawFuncForPropClass()
	{
		FORCE_BASE_CLASS(T, Property)

		ClassType type(typeid(T));

		if (m_Functions.find(type) == m_Functions.end()) return nullptr;

		return m_Functions[type];
	}

	std::function<void(ImGuiContext*, Property*)> GetDrawFuncForPropClass(const ClassType& PropClass)
	{
		if (m_Functions.find(PropClass) == m_Functions.end()) return nullptr;

		return m_Functions[PropClass];
	}

	std::map<ClassType, std::function<void(ImGuiContext*, Property*)>> m_Functions;

	//Auto remove draw functions of types that were unloaded (unregistration is always the first event to fire when a module is unloaded
	//so we just check any of the drawable types are missing from the registry)
	Callback<EventModuleUnloaded> m_ModuleUnloadedEvent = [&](EventModuleUnloaded& event)
	{
		PropertyRegistry& reg = GET_SINGLETON(PropertyRegistry);

		std::vector<std::map<ClassType, std::function<void(ImGuiContext*, Property*)>>::iterator> ToRemove;
		for (auto it = m_Functions.begin(); it != m_Functions.end(); it++)
		{
			bool remove = true;
			const ClassType& checkingPropType = (*it).first;
			for (auto& key : reg.GetRegisteredKeys())
			{
				if (key.Type == checkingPropType)
					remove = false;
			}
			if (remove)
				ToRemove.push_back(it);
		}

		for (auto& it : ToRemove)
		{
			m_Functions.erase(it);
		}
	};
};

class PropertyPannel
{
public:
	PropertyPannel(EditorApp* editorApp) : m_EditorApp(editorApp)
	{

	}

	void DrawPropRaw(Property* prop)
	{
		auto drawTypesObj = m_EditorApp->GetAppObject<PropertyDrawTypes>();
		ImGuiContext* context = ImGui::GetCurrentContext();

		ImGui::PushID(prop);

		if (auto func = drawTypesObj->GetDrawFuncForPropClass(prop->GetType()))
			func(context, prop);

		ImGui::PopID();

		if (ImGui::GetCurrentContext() != context)
			ImGui::SetCurrentContext(context);
	}

	void DrawProp(Property* prop, ObjectBase* Owner = nullptr, ObjectBase* BaseObj = nullptr)
	{
		if (Owner && BaseObj) 
		{
			if (ObjectUtils::HasPropertyChanged(*prop, *BaseObj))
			{
				if (ImGui::Button(">"))
				{
					ObjectUtils::ResetProperty(prop->GetName(), *Owner);
				}

				ImGui::SameLine();
			}
		}

		ImGui::Text(prop->GetName().c_str());
		ImGui::NextColumn();

		DrawPropRaw(prop);

		ImGui::NextColumn();
	}


	void DrawProperties(const std::vector<Ref<Property>>& props, ObjectBase* Owner = nullptr, ObjectBase* BaseObj = nullptr) //without the owner pointer there are no default resets
	{
		ImGui::Columns(2);
		for (auto& prop : props)
		{
			bool IsStruct = false;

			if (IsStruct)
			{

			}
			else
			{
				DrawProp(prop.get(), Owner, BaseObj);
			}
		}
		ImGui::Columns(1);
	}

	EditorApp* m_EditorApp = nullptr;
};

