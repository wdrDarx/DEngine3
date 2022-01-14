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
		bool DrawCategories = true;
		if (DrawCategories)
		{
			if (props.size() < 1) return;
			struct CategoryPair
			{
				std::string Category;
				Property* Prop;
			};

			std::vector<CategoryPair> t_pairs;
			for (auto& prop : props)
			{
				if (!(prop->GetFlags() & PropFlags::EditAnywhere)) continue; //if cant edit or 

				if(prop->GetMetadata().count("Category") == 0) //if theres no category in metadata
					t_pairs.push_back({ "Default", prop.get() });

				std::string key = prop->GetMetadata().at("Category").empty() ? "Default" : prop->GetMetadata().at("Category");
				t_pairs.push_back({ key, prop.get() });
			}

			//sort by category
			cluster(t_pairs.begin(), t_pairs.end(), [](const CategoryPair& first, const CategoryPair& second)
			{
				return first.Category == second.Category;
			});

			std::string CurrentCategory;
			bool LastCategoryExpanded = false;
			uint CategoryIDindex = 0;
			for (uint i = 0; i < t_pairs.size(); i++)
			{
				auto& pair = t_pairs[i];

				if (pair.Category != CurrentCategory)
				{
					if (LastCategoryExpanded)
						ImGui::TreePop();

					CurrentCategory = pair.Category;

					//Draw category thing here
					ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 2,2 });
					LastCategoryExpanded = ImGui::TreeNodeEx((void*)(CategoryIDindex), ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_DefaultOpen, CurrentCategory.c_str());
					ImGui::PopStyleVar();
					CategoryIDindex++;
				}
				if (LastCategoryExpanded)
				{
					ImGui::Columns(2);
					Property* prop = pair.Prop;
					DrawProp(prop, Owner, BaseObj);

					//we at at the end - pop the tree
					if (i == t_pairs.size() - 1)
						ImGui::TreePop();

					ImGui::Columns(1);
				}
			}
		}
		else
		{
			ImGui::Columns(2);
			for (auto& prop : props)
			{
				DrawProp(prop.get(), Owner, BaseObj);
			}
			ImGui::Columns(1);
		}
	}

	static void DrawVec3Control(void* ID, float* value, float ResetValue = 0.f, const float& Speed = 1.0f, float columnWidth = 20.f)
	{
		auto BoldFont = ImGui::GetIO().Fonts->Fonts[0];

		ImGui::PushID(ID);
		float lineHeight = ImGui::GetFont()->FontSize + ImGui::GetStyle().FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		ImGui::PushMultiItemsWidths(3, (ImGui::GetContentRegionAvailWidth() - columnWidth - buttonSize.x * 3.f) + buttonSize.x / 3.f);
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0,0 });

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f,1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f,1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f,1.0f });
		ImGui::PushFont(BoldFont);
		if (ImGui::Button("X", buttonSize))
			value[0] = ResetValue;
		ImGui::PopFont();

		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##X", &value[0], Speed);
		ImGui::PopItemWidth();
		ImGui::SameLine();


		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f,1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.3f, 1.0f });
		ImGui::PushFont(BoldFont);
		if (ImGui::Button("Y", buttonSize))
			value[1] = ResetValue;
		ImGui::PopFont();

		ImGui::SameLine();
		ImGui::DragFloat("##Y", &value[1], Speed);
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PopStyleColor(3);

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f,1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f,1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f,1.0f });
		ImGui::PushFont(BoldFont);
		if (ImGui::Button("Z", buttonSize))
			value[2] = ResetValue;
		ImGui::PopFont();

		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Z", &value[2], Speed);
		ImGui::PopItemWidth();

		ImGui::PopStyleVar();
		ImGui::PopID();
	}

	static void DrawVec2Control(void* ID, float* value, float ResetValue = 0.f, const float& Speed = 1.0f, float columnWidth = 20.f)
	{
		auto BoldFont = ImGui::GetIO().Fonts->Fonts[0];

		ImGui::PushID(ID);
		float lineHeight = ImGui::GetFont()->FontSize + ImGui::GetStyle().FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		ImGui::PushMultiItemsWidths(2, (ImGui::GetContentRegionAvailWidth() - columnWidth - buttonSize.x * 3.f) + buttonSize.x / 3.f);
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0,0 });

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f,1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f,1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f,1.0f });
		ImGui::PushFont(BoldFont);
		if (ImGui::Button("X", buttonSize))
			value[0] = ResetValue;
		ImGui::PopFont();

		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##X", &value[0], Speed);
		ImGui::PopItemWidth();
		ImGui::SameLine();


		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f,1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.3f, 1.0f });
		ImGui::PushFont(BoldFont);
		if (ImGui::Button("Y", buttonSize))
			value[1] = ResetValue;
		ImGui::PopFont();

		ImGui::SameLine();
		ImGui::DragFloat("##Y", &value[1], Speed);
		ImGui::PopItemWidth();
		ImGui::PopStyleColor(3);

		ImGui::PopStyleVar();
		ImGui::PopID();
	}

	EditorApp* m_EditorApp = nullptr;
};

