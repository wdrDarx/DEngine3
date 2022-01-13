#include "Property.h"


Buffer Property::MakeBuffer() const
{
	PROFILE_FUNC()

	Buffer out;
	BufferWritter writter(out);
	writter.WriteString(m_Name);
	writter.WriteString(m_Metadata);
	writter.Write(&m_Flags, sizeof(int));
	writter.WriteVec(MakeValueBuffer(m_Value)); // serialized value

	return out;
}

void Property::FromBuffer(const Buffer& buffer)
{
	PROFILE_FUNC()

	BufferReader reader(buffer);
	reader.ReadString(m_Name);
	reader.ReadString(m_Metadata);
	reader.Read(&m_Flags, sizeof(int));
	Buffer buf;
	reader.ReadBuffer(buf); // read the serialized value

	//set the value from the read buffer
	ValueFromBuffer(m_Value, buf);
}

void Property::LoadAllMetadata(const Buffer& buffer)
{
	PROFILE_FUNC()

	BufferReader reader(buffer);
	reader.ReadString(m_Name);
	reader.ReadString(m_Metadata);
	reader.Read(&m_Flags, sizeof(int));
}

void StaticProperty::FromProperty(const Property& prop)
{
	PROFILE_FUNC()

	prop.AllocateValue(m_Value); //allocate with the correct type for the value ptr
	ValueFromBuffer(m_Value, prop.MakeValueBuffer(prop.GetValue())); //serialize prop value and then deserialize it into the static value ptr

	//copy other metdata
	m_Type = prop.GetType();
	m_ValueType = prop.GetValueType();
	m_Name = prop.GetName();
	m_Metadata = prop.GetMetadata();
	m_Flags = prop.GetFlags();
}

void StaticProperty::CopyFrom(const StaticProperty& other)
{
	PROFILE_FUNC()

	SetName(other.GetName());
	m_Type = other.GetType();
	m_ValueType = other.GetValueType();
	m_Flags = other.GetFlags();
	m_Metadata = other.GetMetadata();
	
	//allocate correct value type from a prop class
	PropertyRegistry& registry = GET_SINGLETON(PropertyRegistry);
	for (auto& key : registry.GetRegisteredKeys())
	{
		Ref<Property> prop = ToRef<Property>(registry.Make(key));
		if (prop->GetType() == GetType()) // we found the correct prop type, now to use its function to allocate the value
		{
			prop->AllocateValue(m_Value);
			break;
		}
	}

	//copy over value via serialization
	ValueFromBuffer(m_Value, other.MakeValueBuffer(other.m_Value));
}

Buffer StaticProperty::MakeValueBuffer(const void* ValuePtr) const
{
	PROFILE_FUNC()

	Buffer out;
	PropertyRegistry& registry = GET_SINGLETON(PropertyRegistry);
	for (auto& key : registry.GetRegisteredKeys())
	{
		Ref<Property> prop = ToRef<Property>(registry.Make(key));
		if (prop->GetType() == GetType()) // we found the correct prop type, now to use its function for serialization
		{
			out = prop->MakeValueBuffer(ValuePtr); //call the serialization function
			return out;
		}
	}

	LOG_ERROR("Tried to make static buffer of property {0} with no registered property class!", GetName());
	return out;
}

void StaticProperty::ValueFromBuffer(void* TargetValuePtr, const Buffer& buffer) const
{
	PROFILE_FUNC()

	PropertyRegistry& registry = GET_SINGLETON(PropertyRegistry);
	for (auto& key : registry.GetRegisteredKeys())
	{
		Ref<Property> prop = ToRef<Property>(registry.Make(key));
		if (prop->GetType() == GetType()) // we found the correct prop type, now to use its function for deserialization
		{
			prop->ValueFromBuffer(TargetValuePtr, buffer); //call the deserialization function
			return;
		}
	}

	LOG_ERROR("Tried to read static buffer of property {0} with no registered property class!", GetName());
}

