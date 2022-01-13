#pragma once
#include "Framework/Property.h"
#include "Framework/StructBase.h"
#include "Framework/EnumBase.h"

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
