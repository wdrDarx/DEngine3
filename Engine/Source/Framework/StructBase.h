#pragma once
#include "Core/Core.h"
#include "Serialization/Buffer.h"
#include "Property.h"
#include "StaticClass.h"
#include "Framework/Registry.h"
#include "FrameworkMacros.h"

/*
	Base Class for all structs
*/
struct DENGINE_API StructBase
{
protected:

	//dont explicitly call, properties are defined on construction
	virtual void DefineProperties()
	{

	}

public:
	 
	//compare each property
	bool operator==(const StructBase& other) const
	{
		bool out = true;
		for (uint i = 0; i < GetProperties().size(); i++)
		{
			auto& prop = GetProperties()[i];
			if( !(*prop == *other.GetProperties()[i]) )
				out = false;
		}
		return out;
	}

	//exists to prevent property redefinition by inheritance since this class doesnt have a seperate "Initialize" method
	StructBase(bool DefProps)
	{
		if (DefProps)
			DefineProperties();
	}


	StructBase()
	{

	}

	//get static class - use STRUCT_STATIC_CLASS()
	template<class T>
	static StaticClass _GetStaticClass()
	{
		StaticClass out;
		out.FromTemlate<T>();
		return out;
	}

	const std::vector<Ref<Property>>& GetProperties() const
	{
		return m_Properties;
	}

	std::vector<Ref<Property>>& GetPropertiesMutable()
	{
		return m_Properties;
	}

	//overriden by macros
	virtual ClassType GetClassType() const
	{
		return typeid(this);
	}

	//overriden by macros
	virtual ClassType GetParentClassType() const
	{
		return typeid(StructBase);
	}

	Buffer MakeBuffer() const;

	//only call after DefineProperties()
	void FromBuffer(const Buffer& buffer);

protected:
	std::vector<Ref<Property>> m_Properties;
};

using StructRegistry = _RegistryBase<ClassRegisterKey, StructBase>;
DEFINE_SINGLETON(StructRegistry, Get_StructRegistry);

#define REGISTER_STRUCT(StructClass) GET_SINGLETON(StructRegistry).Register<StructClass>({#StructClass, ClassType(typeid(StructClass)), GetCurrentModuleName()});
#define UNREGISTER_STRUCT(StructClass) GET_SINGLETON(StructRegistry).Unregister({#StructClass, ClassType(typeid(StructClass)), GetCurrentModuleName()});