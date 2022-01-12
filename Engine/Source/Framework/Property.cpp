#include "Property.h"


Buffer Property::MakeBuffer() const
{
	PROFILE_FUNC()

	Buffer out;
	BufferWritter writter(out);
	writter.WriteString(m_Name);
	writter.WriteClassType(m_Type); //Type of this property
	writter.WriteClassType(m_ValueType); //Type of value of this property
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
	reader.ReadClassType(m_Type); //Type of this property
	reader.ReadClassType(m_ValueType); //Type of value of this property
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
	reader.ReadClassType(m_Type); //Type of this property
	reader.ReadClassType(m_ValueType); //Type of value of this property
	reader.ReadString(m_Metadata);
	reader.Read(&m_Flags, sizeof(int));
}

void StaticProperty::FromProperty(const Property& prop)
{
	PROFILE_FUNC()

	m_Data = prop.MakeValueBuffer(prop.GetValue());
	m_Type = prop.m_Type;
	m_Name = prop.GetName();
	m_Metadata = prop.GetMetadata();
	m_Flags = prop.m_Flags;
}

void* StaticProperty::GetRawValue() const
{
	PROFILE_FUNC()

	PropertyRegistry& registry = GET_SINGLETON(PropertyRegistry);
	for (auto& reg : registry.GetRegisteredKeys())
	{
		//create temporary prop to call virtual functions
		Property* prop = registry.Make(reg);
		if (prop->m_Type.typeIndex == m_Type.typeIndex)
		{
			//allocate the value of the prop
			void* Value = nullptr;
			prop->AllocateValue(Value);
			prop->ValueFromBuffer(Value, m_Data);
			delete prop;
			return Value;
		}
	}

	return nullptr;
}
