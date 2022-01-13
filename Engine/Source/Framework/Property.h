#pragma once
#include "Serialization/Buffer.h"
#include "Framework/Engine.h"
#include "Framework/Registry.h"
#include "Framework/ClassType.h"
#include "Misc/Singleton.h"
#include "Core/Core.h"

//e.g  PROP_CLASS_DEF(FloatProperty, float)
#define PROP_CLASS_DEF(class, WrappingTypename) \
using Typename = WrappingTypename; \
using ThisClass = class; \
void AllocateValue(void*& ptr) const override { ptr = new WrappingTypename; } \
ThisClass(const std::string& name, Typename& value) : Property(ClassType(typeid(ThisClass)), ClassType(typeid(Typename))) { m_Value = &value; SetName(name); } \
ThisClass() : Property(ClassType(typeid(ThisClass)), ClassType(typeid(Typename))) { }; \
 bool operator==(const Property& other) const override { return (*(Typename*)GetValue()) == (*(Typename*)other.GetValue()); }


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

	//equals operator 
	virtual bool operator==(const Property& other) const
	{
		return false;
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

	virtual const void* GetValue() const
	{
		return m_Value;
	}

	void SetValue(void* ptr)
	{
		m_Value = ptr;
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
struct StaticProperty : public Property
{

	//need a copy constructor to explicitly copy the value of this property
	StaticProperty(const StaticProperty& other)
	{
		CopyFrom(other);
	}

	StaticProperty& operator=(const StaticProperty& other)
	{
		CopyFrom(other);
		return *this;
	}

	StaticProperty(StaticProperty&& other) noexcept
	{
		//not sure if we should also delete the other here because its a move
		CopyFrom(other);
	}

	StaticProperty() : Property()
	{

	}

	~StaticProperty()
	{
		//actually delete the value because we own it
		if (m_Value)
			delete m_Value;
	}
	
	void FromProperty(const Property& prop);

	//quick function to make a static property from a custom property class 
	template<class T>
	static StaticProperty Make(const T& SourceProp)
	{
		StaticProperty out;
		out.FromProperty(SourceProp);
		return out;
	}

	void CopyFrom(const StaticProperty& other);


	template<typename T>
	static StaticProperty Make(const std::string& name, const T& value)
	{
		StaticProperty out;
		out.SetName(name); //set the name
		out.m_ValueType = ClassType(typeid(value)); //set the value type
		PropertyRegistry& registry = GET_SINGLETON(PropertyRegistry);
		for (auto& key : registry.GetRegisteredKeys())
		{
			Ref<Property> prop = ToRef<Property>(registry.Make(key));
			if (prop->GetValueType().typeIndex == typeid(T))
			{
				out.m_Type = prop->GetType(); //find corresponding prop type to the value type and set it here
				prop->AllocateValue(out.m_Value); //Allocate correct pointer type
				out.ValueFromBuffer(out.m_Value, prop->MakeValueBuffer(&value)); //copy over the input value via serialzation with the correct functions
				return out;
			}
		}

		return out;
	}

	//makes the value buffer by searching for the correct serialization function based on the property type of this static property
	Buffer MakeValueBuffer(const void* ValuePtr) const override;

	//deserializes the value the same way as MakeValueBuffer
	void ValueFromBuffer(void* TargetValuePtr, const Buffer& buffer) const override;
};


//Creates a property if the type passed in is registered with a prop type
template<typename T>
inline Ref<Property> CreatePropertyFromMember(T& memeber, const std::string& name, const std::string& metadata, int Flags)
{
	ClassType type(typeid(T));
	PropertyRegistry& registry = GET_SINGLETON(PropertyRegistry);
	for (auto& key : registry.GetRegisteredKeys())
	{
		Ref<Property> prop = ToRef<Property>(registry.Make(key));
		if (prop->GetValueType() == type || ( ClassUtils::IsStruct(prop->GetValueType())  && ClassUtils::IsStruct(type)) ) // check if prop value type matches the memeber type we got
		{
			prop->m_Value = &memeber; //set value ptr to the member
			prop->SetName(name);
			//prop->SetCategory()
			prop->SetMetadata(metadata);
			prop->SetFlags(Flags);
			return prop;
		}
	}

	LOG_ERROR("Tried to define property {0} with no registered property class!", name);
	return nullptr;
}




