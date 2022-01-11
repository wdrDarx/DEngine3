#pragma once
#include "Core/Core.h"
#include "Framework/Property.h"

class ObjectBase;
struct StructBase;
class Application;

#ifdef STATICCLASS_DEPRECATED
class DENGINE_API StaticClass
{
public:	
	StaticClass(Ref<ObjectBase> obj);

	StaticClass();

	//need to manually deallocate all the static prop values
	~StaticClass();

	const std::vector<StaticProperty>& GetDefaultProperties() const
	{
		return m_StaticProperties;
	}

	const ClassType& GetClassType() const
	{
		return *m_ClassType;
	}

	template<class T>
	void FromTemlate()
	{
		bool valid = std::is_base_of<ObjectBase, T>::value;
		ASSERT(valid);

		//create an instance and create the static class
		Ref<T> newObj = MakeRef<T>();
		CreateStaticClass(newObj);
	}


private:
	void CreateStaticClass(Ref<ObjectBase> obj);
		
	std::vector<StaticProperty> m_StaticProperties;
	Ref<ClassType> m_ClassType;
	ObjectClassType m_ObjectClassType;
};
#endif

// stores things like default property values, and object registry type by hosting a scoped instance of the object/struct
// yes this supports structs too
class StaticClass
{
public:
	StaticClass()
	{

	}

	const std::vector<Property*>& GetDefaultProperties() const;

	const ClassType& GetClassType() const
	{
		return *m_ClassType;
	}

	const ObjectClassType& GetObjectClassType() const
	{		
		ASSERT(!m_IsStruct); //were a structure static class, cant do this
		return m_ObjectClassType;
	}

	template<class T>
	void FromTemlate()
	{
		if(!std::is_base_of<ObjectBase, T>::value)
		{
			if(std::is_base_of<StructBase, T>::value)
				m_IsStruct = true;
			else
				ASSERT(false) //type not an ObjectBase nor a StructBase
		}

		//create an instance and create the static class
		if(m_IsStruct)
			m_StructRef = Cast<StructBase>(MakeRef<T>());
		else
			m_ObjectRef = Cast<ObjectBase>(MakeRef<T>());

		CreateStaticClass();
	}

	std::vector<StaticProperty> GenerateStaticProperties() const;

private:
	void CreateStaticClass();
	bool m_IsStruct = false;

	Ref<ObjectBase> m_ObjectRef = nullptr;
	Ref<StructBase> m_StructRef = nullptr;
	Ref<ClassType> m_ClassType = nullptr;
	ObjectClassType m_ObjectClassType = ObjectClassType::OBJECT;
};