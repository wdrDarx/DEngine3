#pragma once
#include "Serialization/Buffer.h"
#include "Framework/Engine.h"
#include "Framework/Registry.h"
#include "Misc/Singleton.h"
#include "Core/Core.h"

#define PROPDEF(member) [&]() -> Property* { Property* out = nullptr; \
if(typeid(member) == typeid(float)) { out = new FloatProperty(member); } \
 return out; }()

//e.g  PROP_CLASS_DEF(FloatProperty, float)
#define PROP_CLASS_DEF(class, WrappingTypename) \
using Typename = WrappingTypename; \
using ThisClass = class; \
void AllocateValue(void*& ptr) override { ptr = new WrappingTypename; } \
ThisClass(const std::string& name, Typename& value) : Property(ClassType(typeid(Typename))) { m_Value = &value;	SetName(name); } \
ThisClass() : Property(ClassType(typeid(Typename))) { };


//pointer to existing value 
struct Property
{
public:

	Property(const ClassType& type) : m_Type(type)
	{

	}

	virtual ~Property()
	{

	}

	//entire property
	Buffer MakeBuffer() const
	{
		Buffer out;
		BufferWritter writter(out);
		writter.WriteString(m_Name);
		writter.WriteString(m_Type.Name); //typename of this property
		writter.WriteString(m_Metadata);
		writter.Write(&m_Flags, sizeof(int));
		writter.WriteVec(MakeValueBuffer()); // serialized value
	}

	void FromBuffer(const Buffer& buffer)
	{
		BufferReader reader(buffer);
		reader.ReadString(m_Name);
		reader.ReadString(m_Type.Name); //typename of this property
		reader.ReadString(m_Metadata);
		reader.Read(&m_Flags, sizeof(int));
		Buffer buf;
		reader.ReadBuffer(buf); // read the serialized value

		//set the value from the read buffer
		ValueFromBuffer(buf);
	}

	//must be overriden (e.g ptr = new float(); )
	virtual void AllocateValue(void*& ptr)
	{

	}

	//for m_value only
	virtual Buffer MakeValueBuffer() const
	{
		return Buffer();
	}

	virtual void ValueFromBuffer(const Buffer& buffer)
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

	std::string SetMetadata(const std::string& meta)
	{
		m_Metadata = meta;
	}

	ClassType m_Type;
	std::string m_Name;
	std::string m_Metadata;
	int m_Flags;
	void* m_Value = nullptr;
};

struct StaticProperty
{

	StaticProperty() : m_Type(typeid(void))
	{

	}

	~StaticProperty()
	{
		delete m_LastRawValue;
	}

	void FromProperty(const Property& prop)
	{
		m_Data = prop.MakeValueBuffer();
		m_Type = prop.m_Type;
		m_Name = prop.GetName();
		m_Metadata = prop.GetMetadata();
		m_Flags = prop.m_Flags;
	}

	template<class T>
	static StaticProperty Make(const T& SourceProp)
	{
		StaticProperty out;
		out.FromProperty(SourceProp);
		return out;
	}

	void* GetRawValue() const
	{
		PropertyRegistery& registry = GET_SINGLETON(PropertyRegistery);
		for (auto& reg : registry.GetRegisteredKeys())
		{
			Property* prop = registry.Make(reg);
			if (prop->m_Type.Name == m_Type.Name)
			{
				//allocate the value of the prop
				prop->AllocateValue(prop->m_Value);
				prop->ValueFromBuffer(m_Data);
				return prop->m_Value;
			}
		}
	}

	void* m_LastRawValue = nullptr;
	Buffer m_Data;
	ClassType m_Type;
	std::string m_Name;
	std::string m_Metadata;
	int m_Flags;
};


struct FloatProperty : public Property, AutoRegister<FloatProperty, Engine>
{
	AUTO_REGISTER()
	PROP_CLASS_DEF(FloatProperty, float)

	Buffer MakeValueBuffer() const override
	{
		Buffer out;
		BufferWritter writter(out);
		writter.Write(m_Value, sizeof(float));
		return out;
	}

	void ValueFromBuffer(const Buffer& buffer) override
	{
		BufferReader reader(buffer);
		reader.Read(m_Value, sizeof(float));
	}
};

struct StringProperty : public Property, AutoRegister<StringProperty, Engine>
{
	AUTO_REGISTER()
	PROP_CLASS_DEF(StringProperty, std::string)

	Buffer MakeValueBuffer() const override
	{
		Buffer out;
		BufferWritter writter(out);
		writter.WriteString(*(std::string*)m_Value);
		return out;
	}

	void ValueFromBuffer(const Buffer& buffer) override
	{
		BufferReader reader(buffer);
		reader.ReadString(*(std::string*)m_Value);
	}
};


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




