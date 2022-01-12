#pragma once
#include "Serialization/Buffer.h"
#include "Framework/Engine.h"
#include "Framework/Registry.h"
#include "Framework/ClassType.h"
#include "Misc/Singleton.h"
#include "Core/Core.h"

#define PROPDEF(member) [&]() -> Property* { Property* out = nullptr; \
if(typeid(member) == typeid(float)) { out = new FloatProperty(member); } \
 return out; }()

//e.g  PROP_CLASS_DEF(FloatProperty, float)
#define PROP_CLASS_DEF(class, WrappingTypename) \
using Typename = WrappingTypename; \
using ThisClass = class; \
void AllocateValue(void*& ptr) const override { ptr = new WrappingTypename; } \
ThisClass(const std::string& name, Typename& value) : Property(ClassType(typeid(ThisClass)), ClassType(typeid(Typename))) { m_Value = &value; SetName(name); } \
ThisClass() : Property(ClassType(typeid(ThisClass)), ClassType(typeid(Typename))) { };


enum PropFlags
{
	NoSerialize = BIT(0),
	EditAnywhere = BIT(1)
};

//pointer to existing value 
struct Property
{
public:

	Property(const ClassType& type, const ClassType& ValueType) : m_ValueType(ValueType), m_Type(type)
	{

	}

	Property() : m_ValueType(ClassType(typeid(void))), m_Type(typeid(Property))
	{

	}

	virtual ~Property()
	{

	}

	//entire property
	Buffer MakeBuffer() const;

	void FromBuffer(const Buffer& buffer);

	//only loads metadata from a buffer, ignoring the value deserialization process 
	void LoadAllMetadata(const Buffer& buffer);

	//Allocated a pointer with the correct type e.g StringProperty will allocate a std::string*
	virtual void AllocateValue(void*& ptr) const
	{

	}

	//Creates a buffer from a value ptr, pointer type must match property type
	virtual Buffer MakeValueBuffer(const void* ValuePtr) const
	{
		return Buffer();
	}

	//Deserializes a buffer from a value ptr, pointer type must match property type
	virtual void ValueFromBuffer(void* TargetValuePtr, const Buffer& buffer) const
	{

	}

	std::string GetName() const
	{
		return m_Name;
	}

	void SetName(const std::string& name)
	{
		m_Name = name;
	}

	std::string GetMetadata() const
	{
		return m_Metadata;
	}

	void SetMetadata(const std::string& meta)
	{
		m_Metadata = meta;
	}

	const void* GetValue() const
	{
		return m_Value;
	}

	//the type points to an actual C type e.g float
	const ClassType& GetValueType() const
	{
		return m_ValueType;
	}

	//the type points to the class of this property
	const ClassType& GetType() const
	{
		return m_Type;
	}

	int GetFlags() const
	{
		return m_Flags;
	}

	void SetFlags(int flags)
	{
		m_Flags = flags;
	}

	//Class type of the value of the prop
	ClassType m_ValueType;

	//Class Type of the class of the prop
	ClassType m_Type;

	std::string m_Name;
	std::string m_Metadata;
	int m_Flags;
	void* m_Value = nullptr;
};

//Unlike a normal property, a static property just stores the value inside itself within a buffer. It knows its type however to get the raw value of the buffer it needs to find the correct function to deserialize it
struct StaticProperty
{

	StaticProperty() : m_Type(typeid(void))
	{

	}

	~StaticProperty()
	{
		
	}
	
	
	void FromProperty(const Property& prop);

	//quick function to make a static property from a custom property class 
	//e.g Make(StringProperty("Name"), &MyString)
	template<class T>
	static StaticProperty Make(const T& SourceProp)
	{
		StaticProperty out;
		out.FromProperty(SourceProp);
		return out;
	}

	template<typename T>
	static StaticProperty Make(const std::string& name, const T& value)
	{
		StaticProperty out;
		out.m_Name = name;
		out.m_Type = ClassType(typeid(value));
		PropertyRegistry& registry = GET_SINGLETON(PropertyRegistry);
		for (auto& key : registry.GetRegisteredKeys())
		{
			Property* prop = registry.Make(key);
			if (prop->GetValueType().typeIndex == typeid(T))
			{
				out.m_Data = prop->MakeValueBuffer(&value);
				delete prop; //property class has done its job
				return out;
			}
		}

		return out;
	}

	//Gets pointer to raw value of this property by correctly deserializing the value, this pointer can be casted to the desired type
	void* GetRawValue() const;

	Buffer m_Data;
	ClassType m_Type;
	std::string m_Name;
	std::string m_Metadata;
	int m_Flags;
};


//Creates a property if the type passed in is registered with a prop type
template<typename T>
inline Property* CreatePropertyFromMember(T& memeber, const std::string& name, const std::string& metadata, int Flags)
{
	ClassType type(typeid(T));
	PropertyRegistry& registry = GET_SINGLETON(PropertyRegistry);
	for (auto& key : registry.GetRegisteredKeys())
	{
		Property* prop = registry.Make(key);
		if (prop->GetValueType() == type) // check if prop value type matches the memeber type we got
		{
			prop->m_Value = &memeber; //set value ptr to the member
			prop->SetName(name);
			//prop->SetCategory()
			prop->SetMetadata(metadata);
			prop->SetFlags(Flags);
			return prop;
		}
		else
			delete prop; //no match so delete prop
	}

	LOG_ERROR("Tried to define property {0} with no registered property class!");
	return nullptr;
}


struct PropArgDef
{
	PropArgDef() : Name(""), Type(typeid(void))
	{

	}

	PropArgDef(const std::string& name, const std::type_index& type) : Name(name), Type(type)
	{

	}

	std::string Name;
	std::type_index Type;
};

struct PropArray
{
	template<typename T>
	const T& Get(size_t index) const
	{
		void* value = m_Props[i].GetRawValue();
		return *(T*)value;
	}

	template<typename T>
	const T& Get(const std::string& name) const
	{
		void* value = nullptr;

		for (uint i = 0; i < m_Props.size(); i++)
		{
			if (m_Props[i].m_Name == name)
			{
				value = m_Props[i].GetRawValue();
				break;
			}
		}

		return *(T*)value;
	}

	std::vector<StaticProperty> m_Props;
};

using PropArgsDefs = std::vector<PropArgDef>;

struct Method
{
	std::function<void(const PropArray&)> m_Function;
	PropArgsDefs m_Arguments;

	void Call(const PropArray& args)
	{
		m_Function(args);
	}
};




