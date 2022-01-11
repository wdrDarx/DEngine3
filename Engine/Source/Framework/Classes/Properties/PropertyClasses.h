#pragma once
#include "Framework/Property.h"

struct FloatProperty : public Property, AutoRegister<FloatProperty, Engine>
{
	AUTO_REGISTER()
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

struct StringProperty : public Property, AutoRegister<StringProperty, Engine>
{
	AUTO_REGISTER()
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
