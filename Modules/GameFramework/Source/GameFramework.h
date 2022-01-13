#pragma once
#include "Framework/Module.h"
#include "DEngine.h"

#define GAMEFRAMEWORK_API

#ifdef GAMEFRAMEWORK_API
#define GAMEFRAMEWORK __declspec(dllexport)
#else
#define GAMEFRAMEWORK
#endif

class GAMEFRAMEWORK GameFramework : public Module
{
public:
	using Module::Module;

	void OnLoad() override;
	void OnUnload() override;
	void OnUpdate(float DeltaTime) override;

	void TestMethod(const ArgList& args);
};

struct Function
{
	Function(const std::function<void()>& func) : m_Function(func)
	{

	}

	Function()
	{

	}

	bool operator==(const Function& other) const
	{
		return true;
	}

	void operator()() const
	{
		m_Function();
	}

	std::function<void()> m_Function = nullptr;
};

struct FunctionProperty : public Property
{
	PROP_CLASS_DEF(FunctionProperty, Function)
	PROP_DEFAULT_ALLOCATE()

	Buffer MakeValueBuffer(const void* valuePtr) const override
	{
		Buffer out;
		
		return out;
	}

	void ValueFromBuffer(void* targetPtr, const Buffer& buffer) const override
	{
		
	}
};

struct ObjectClass
{
	ObjectClass() : m_BaseClass("ObjectBase")
	{

	}

	bool operator==(const ObjectClass& other) const
	{
		return m_ClassName == other.m_ClassName && m_BaseClass == other.m_BaseClass;
	}

	ObjectClass(const ClassType& BaseClass) : m_BaseClass(BaseClass.Name), m_ClassName(BaseClass.Name)
	{

	}

	ObjectClass(const std::string& BaseClassName) : m_BaseClass(BaseClassName), m_ClassName(BaseClassName)
	{

	}

	ObjectClass(const std::string& BaseClassName, const std::string& DefaultClassName) : m_BaseClass(BaseClassName), m_ClassName(DefaultClassName)
	{

	}

	ObjectClass(const ClassType& BaseClass, const ClassType& DefaultClass) : m_BaseClass(BaseClass.Name), m_ClassName(DefaultClass.Name)
	{

	}

	std::string m_ClassName;
	std::string m_BaseClass;
};

struct ObjectClassProperty : public Property
{
	PROP_CLASS_DEF(ObjectClassProperty, ObjectClass)
	PROP_DEFAULT_ALLOCATE()

	Buffer MakeValueBuffer(const void* valuePtr) const override
	{
		Buffer out;
		ObjectClass* ptr = (ObjectClass*)valuePtr;
		BufferWritter writter(out);
		writter.WriteString(ptr->m_BaseClass);
		writter.WriteString(ptr->m_ClassName);
		return out;
	}

	void ValueFromBuffer(void* targetPtr, const Buffer& buffer) const override
	{
		ObjectClass* ptr = (ObjectClass*)targetPtr;
		BufferReader reader(buffer);
		reader.ReadString(ptr->m_BaseClass);
		reader.ReadString(ptr->m_ClassName);
	}
};


