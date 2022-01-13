#pragma once
#include "Property.h"

struct _ArgType
{
	_ArgType() : Type(typeid(void)), Name("")
	{

	}

	std::string Name;
	ClassType Type;
};

template<typename T>
struct ArgType : public _ArgType
{
	ArgType() : _ArgType()
	{

	}

	ArgType(const std::string& name) : _ArgType()
	{
		Name = name;
		Type = ClassType(typeid(T));
	}
};

struct ArgList
{
	template<typename T>
	const T& Get(size_t index) const
	{
		void* value = m_Props[i].GetValue();
		return *(T*)value;
	}

	template<typename T>
	const T& Get(const std::string& name) const
	{
		const void* value = nullptr;

		for (uint i = 0; i < m_Props.size(); i++)
		{
			if (m_Props[i].m_Name == name)
			{
				value = m_Props[i].GetValue();
				break;
			}
		}

		return *(const T*)value;
	}

	static ArgList Make(const std::vector<StaticProperty>& Props)
	{
		ArgList out;
		out.m_Props = Props;
		return out;
	}

	std::vector<StaticProperty> m_Props;
};

struct Method
{
	std::function<void(const ArgList&)> m_Function;
	std::vector<Ref<_ArgType>> m_ArgumentTypes;

	void Call(const ArgList& args)
	{
		m_Function(args);
	}
};