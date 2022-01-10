#pragma once
#include "Serialization/Buffer.h"
#include "Framework/Engine.h"
#include "Framework/Registry.h"
#include "Misc/Singleton.h"
#include "Core/Core.h"

#define PROPDEF(member) [&]() -> Property* { Property* out = nullptr; \
if(typeid(member) == typeid(float)) { out = new FloatProperty(member); } \
 return out; }()


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
		return Buffer{};
	}

	void FromBuffer(const Buffer& buffer)
	{

	}

	//for m_value only
	virtual Buffer MakeValueBuffer() const = 0;
	virtual void ValueFromBuffer(const Buffer& buffer) = 0;

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

	void* GetRawValue()
	{
		if (!m_LastRawValue)
			delete m_LastRawValue;

		return nullptr;
	}

	void* m_LastRawValue;
	Buffer m_Data;
	ClassType m_Type;
	std::string m_Name;
	std::string m_Metadata;
	int m_Flags;
};


struct FloatProperty : public Property, AutoRegister<FloatProperty, Engine>
{
	AUTO_REGISTER()

	FloatProperty(const std::string& name, float& value) : Property(ClassType(typeid(float)))
	{
		m_Value = &value;
		SetName(name);
	}

	FloatProperty() : Property(ClassType(typeid(float)))
	{

	}

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

struct StringProperty : public Property
{
	StringProperty(const std::string& name, std::string& value) : Property(ClassType(typeid(std::string)))
	{
		m_Value = &value;
		SetName(name);
	}

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

	std::string Name;
	std::type_index Type;
};

struct PropArray
{
	template<typename T>
	const T& Get(size_t index) const
	{
		void* value = m_Props[index].m_Data.data();
		return *(T*)value;
	}

	template<typename T>
	const T& Get(const std::string& name) const
	{
		void* value = nullptr;

		for (int i = 0; i < m_Props.size(); i++)
		{
			if (m_Props[i].m_Name == name)
			{
				value = (void*)m_Props[i].m_Data.data();
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




