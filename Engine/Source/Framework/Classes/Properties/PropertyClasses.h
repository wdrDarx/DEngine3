#pragma once
#include "Framework/Property.h"
#include "Framework/StructBase.h"
#include "Framework/EnumBase.h"
#include "Utils/ClassUtils.h"

struct BoolProperty : public Property
{
	PROP_CLASS_DEF(BoolProperty, bool);
	PROP_DEFAULT_ALLOCATE()

	Buffer MakeValueBuffer(const void* ValuePtr) const override
	{
		Buffer out;
		BufferWritter writter(out);
		writter.Write(ValuePtr, sizeof(bool));
		return out;
	}

	void ValueFromBuffer(void* TargetValuePtr, const Buffer& buffer) const override
	{
		BufferReader reader(buffer);
		reader.Read(TargetValuePtr, sizeof(bool));
	}
};

struct FloatProperty : public Property
{
	PROP_CLASS_DEF(FloatProperty, float)
	PROP_DEFAULT_ALLOCATE()

	Buffer MakeValueBuffer(const void* valuePtr) const override
	{
		Buffer out;
		BufferWritter writter(out);
		writter.Write(valuePtr, sizeof(float));
		return out;
	}

	void ValueFromBuffer(void* targetPtr, const Buffer& buffer) const override
	{
		BufferReader reader(buffer);
		reader.Read(targetPtr, sizeof(float));
	}
};

struct IntProperty : public Property
{
	PROP_CLASS_DEF(IntProperty, int);
	PROP_DEFAULT_ALLOCATE()

	Buffer MakeValueBuffer(const void* ValuePtr) const override
	{
		Buffer out;
		BufferWritter writter(out);
		writter.Write(ValuePtr, sizeof(int));
		return out;
	}

	void ValueFromBuffer(void* TargetValuePtr, const Buffer& buffer) const override
	{
		BufferReader reader(buffer);
		reader.Read(TargetValuePtr, sizeof(int));
	}
};

struct StringProperty : public Property
{
	PROP_CLASS_DEF(StringProperty, std::string)
	PROP_DEFAULT_ALLOCATE()

	Buffer MakeValueBuffer(const void* valuePtr) const override
	{
		Buffer out;
		BufferWritter writter(out);
		writter.WriteString(*(std::string*)valuePtr);
		return out;
	}

	void ValueFromBuffer(void* targetPtr, const Buffer& buffer) const override
	{
		BufferReader reader(buffer);
		reader.ReadString(*(std::string*)targetPtr);
	}
};

struct Vec2dProperty : public Property
{
	PROP_CLASS_DEF(Vec2dProperty, vec2d)
	PROP_DEFAULT_ALLOCATE()

	Buffer MakeValueBuffer(const void* valuePtr) const override
	{
		Buffer out;
		BufferWritter writter(out);
		writter.Write(valuePtr, sizeof(vec2d));
		return out;
	}

	void ValueFromBuffer(void* targetPtr, const Buffer& buffer) const override
	{
		BufferReader reader(buffer);
		reader.Read(targetPtr, sizeof(vec2d));
	}
};

struct Vec3dProperty : public Property
{
	PROP_CLASS_DEF(Vec3dProperty, vec2d)
	PROP_DEFAULT_ALLOCATE()

	Buffer MakeValueBuffer(const void* valuePtr) const override
	{
		Buffer out;
		BufferWritter writter(out);
		writter.Write(valuePtr, sizeof(vec3d));
		return out;
	}

	void ValueFromBuffer(void* targetPtr, const Buffer& buffer) const override
	{
		BufferReader reader(buffer);
		reader.Read(targetPtr, sizeof(vec3d));
	}
};

struct TransformProperty : public Property
{
	PROP_CLASS_DEF(TransformProperty, Transform)
	PROP_DEFAULT_ALLOCATE()

	Buffer MakeValueBuffer(const void* valuePtr) const override
	{
		Buffer out;
		BufferWritter writter(out);
		writter.Write(valuePtr, sizeof(Transform));
		return out;
	}

	void ValueFromBuffer(void* targetPtr, const Buffer& buffer) const override
	{
		BufferReader reader(buffer);
		reader.Read(targetPtr, sizeof(Transform));
	}
};

struct StructProperty : public Property
{
	PROP_CLASS_DEF(StructProperty, StructBase)

	void AllocateValue(void*& ptr) const override
	{
		//Allocate the pointer as the correct new struct type 
		ptr = GET_SINGLETON(StructRegistry).MakeObjectFromClassName(((StructBase*)GetValue())->GetClassType().Name);
	}

	bool IsMatchingValueType(void* ValuePtr, const ClassType& ValueType) const override
	{
		return ClassUtils::IsStruct(ValueType); //matching if its a struct
	}

	Buffer MakeValueBuffer(const void* valuePtr) const override
	{
		Buffer out;
		BufferWritter writter(out);
		writter.WriteBuffer(((StructBase*)valuePtr)->MakeBuffer());
		return out;
	}

	void ValueFromBuffer(void* targetPtr, const Buffer& buffer) const override
	{
		BufferReader reader(buffer);
		Buffer StructBuffer;
		reader.ReadBuffer(StructBuffer);
		((StructBase*)targetPtr)->FromBuffer(StructBuffer);
	}
};

struct EnumProperty : public Property
{
	PROP_CLASS_DEF(EnumProperty, EnumBase)

	bool IsMatchingValueType(void* ValuePtr, const ClassType& ValueType) const override
	{
		return ClassUtils::IsEnum(ValueType); //enum if it casts to an enum class
	}

	Buffer MakeValueBuffer(const void* valuePtr) const override
	{
		Buffer out;
		BufferWritter writter(out);
		EnumBase* enumValue = (EnumBase*)valuePtr;
		int value = enumValue->ConstIntValue();
		writter.Write( &value, sizeof(int) );
		return out;
	}

	void ValueFromBuffer(void* targetPtr, const Buffer& buffer) const override
	{
		BufferReader reader(buffer);
		Buffer StructBuffer;
		EnumBase* enumValue = (EnumBase*)targetPtr;
		int value;
		reader.Read(&value, sizeof(int));
		enumValue->IntValue() = value;
	}
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

struct _ObjectClass
{
	_ObjectClass() : m_BaseClass("ObjectBase")
	{

	}

	bool operator==(const _ObjectClass& other) const
	{
		return m_ClassName == other.m_ClassName && m_BaseClass == other.m_BaseClass;
	}

	std::string m_ClassName;
	std::string m_BaseClass;
};

template<class T>
struct ObjectClass : public _ObjectClass
{
	ObjectClass(const ClassType& DefaultClass)
	{
		m_BaseClass = ClassType(typeid(T)).Name;
		m_ClassName = DefaultClass.Name;
	}

	ObjectClass()
	{
		m_BaseClass = ClassType(typeid(T)).Name;
		m_ClassName = ClassType(typeid(T)).Name;
	}
};

struct ObjectClassProperty : public Property
{
	PROP_CLASS_DEF(ObjectClassProperty, _ObjectClass)
	PROP_DEFAULT_ALLOCATE()

	bool IsMatchingValueType(void* ValuePtr, const ClassType& ValueType) const override
	{
		return ClassUtils::IsTemplateType(ValueType) && ClassUtils::GetTemplateBaseFromType(ValueType) == "ObjectClass";
	}

	Buffer MakeValueBuffer(const void* valuePtr) const override
	{
		Buffer out;
		_ObjectClass* ptr = (_ObjectClass*)valuePtr;
		BufferWritter writter(out);
		writter.WriteString(ptr->m_BaseClass);
		writter.WriteString(ptr->m_ClassName);
		return out;
	}

	void ValueFromBuffer(void* targetPtr, const Buffer& buffer) const override
	{
		_ObjectClass* ptr = (_ObjectClass*)targetPtr;
		BufferReader reader(buffer);
		reader.ReadString(ptr->m_BaseClass);
		reader.ReadString(ptr->m_ClassName);
	}
};

struct DebugText
{
	std::string Text;

	bool operator==(const DebugText& other) const
	{
		return Text == other.Text;
	}
};

struct DebugTextProperty : public Property
{
	PROP_CLASS_DEF(DebugTextProperty, DebugText)
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