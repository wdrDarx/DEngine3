#include "StaticClass.h"
#include "Framework/ObjectBase.h"
#include "Framework/Application.h"
#include "Framework/Registry.h"

void StaticClass::CreateStaticClass()
{
	if (m_IsStruct)
	{
		//set class type by copy
		//m_ClassType = MakeRef<ClassType>(m_StructRef->GetClassType());
	}
	else
	{
		//set class type by copy
		m_ClassType = MakeRef<ClassType>(m_ObjectRef->GetClassType());

		//determine ObjectClassType
		//m_ObjectClassType = ObjectUtils::GetObjectClassType(m_ObjectRef);

		//define properties for retrieval
		m_ObjectRef->DefineProperties();
	}	
}

//these property values could be null if you exit the scope
const std::vector<Ref<Property>>& StaticClass::GetDefaultProperties() const
{
	//if(m_IsStruct)
	//	return m_StructRef->GetProperties();
	//else
		return m_ObjectRef->GetProperties();
}

std::vector<StaticProperty> StaticClass::GenerateStaticProperties() const
{
	std::vector<StaticProperty> out;
	auto covertFunc = [&](const std::vector<Ref<Property>>& props)
	{
		//copy all property values to StaticProperty array
		for (auto& OriginalProp : props)
		{
			StaticProperty CopyProp;
			CopyProp.FromProperty(*OriginalProp);
			out.push_back(CopyProp);
		}
	};

	//if (m_IsStruct)
	//	covertFunc(m_StructRef->GetProperties());
	//else
		covertFunc(m_ObjectRef->GetProperties());

	return out;
}


