#pragma once
#include "Framework/Property.h"

struct BoolProperty : public Property
{
	PROP_CLASS_DEF(BoolProperty, bool);

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
