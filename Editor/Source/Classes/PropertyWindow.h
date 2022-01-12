#pragma once
#include "DEngine.h"

class PropertyDrawTypes : public AppObject, AutoRegister<PropertyDrawTypes, Engine>
{
	public:
		AUTO_REGISTER()
		OBJECT_CLASS_DEF(PropertyDrawTypes, AppObject)

		void OnConstruct() override
		{
			Super::OnConstruct();

			GetApplication()->GetModuleManager().BindOnModuleUnloaded(m_ModuleUnloadedEvent);
		}

		template<class T>
		void AddDrawMethod(std::function<void()> DrawFunc)
		{
			FORCE_BASE_CLASS(T, Property)
			m_Functions[ClassType(typeid(T))] = DrawFunc;
		}

		template<class T>
		std::function<void()> GetDrawFuncForPropClass()
		{
			FORCE_BASE_CLASS(T, Property)

			ClassType type(typeid(T));

			if(m_Functions.find(type) == m_Functions.end()) return nullptr;

			return m_Functions[type];
		}

		std::map<ClassType, std::function<void()>> m_Functions;

		Callback<EventModuleUnloaded> m_ModuleUnloadedEvent = [&](EventModuleUnloaded& event)
		{
			PropertyRegistry& reg = GET_SINGLETON(PropertyRegistry);

			std::vector<std::map<ClassType, std::function<void()>>::iterator> ToRemove;
			for (auto it = m_Functions.begin(); it != m_Functions.end(); it++)
			{
				bool remove = true;
				for (auto& key : reg.GetRegisteredKeys())
				{
					Property* temp = reg.Make(key); //make a temporary prop to check types
					if (temp->GetType() == (*it).first) //Type of this prop is still registered
					{
						remove = false;
						delete temp;
						break;
					}
					else
						delete temp;
				}
				if(remove)
					ToRemove.push_back(it);
			}

			for (auto& it : ToRemove)
			{
				m_Functions.erase(it);
			}
		};
};


class PropertyWindow
{
	public:
		void DrawProperties(const std::vector<Property*>& props)
		{

		}
};

